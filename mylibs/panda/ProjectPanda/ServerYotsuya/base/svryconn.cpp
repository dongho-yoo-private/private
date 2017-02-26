#include "svryconn.h"
#include "yistd.h"
//#include "yiPacket.h"
#include <assert.h>

// ------------------------------------
csvryConn::csvryConn(int n, int index, int acceptFd, Bool bIsLocal)
{
	int pipefd[2];

	if (pipe(pipefd)!=0)
	{
		delete this;
		assert(0);
	}
	m_r_acceptFd = pipefd[0];
	m_w_acceptFd = pipefd[1];	

	FD_ZERO(&m_rfdset);

	m_fds = new int[n+1];
	m_fdsExInfo = (void**)yialloci((n+1)*sizeof(void*));
		
	for (int i=0; i<n; i++)
	{
		m_brank_queue.enq((yiAnyValue) i);
		m_fds[i]=-1;
	}
		
	FD_SET(m_r_acceptFd, &m_rfdset);
	m_fds[n]=m_r_acceptFd;
	m_cnt=0;
	m_maxcnt=n;
	m_index=index;

	//HandleBuffer buff;
	m_hMutex = yiMutexCreate(0);

	m_bIsLocal=bIsLocal;
	m_AcceptFd=acceptFd;
}

// ------------------------------------
csvryConn::~csvryConn()
{
	yiMutexDestroy(m_hMutex);
	delete m_fds;
	close(m_r_acceptFd);
	close(m_w_acceptFd);
}


// ------------------------------------
Bool csvryConn::Add(int fd)
{
YIDEBUG("csvryConn::Add(%d)\n", fd);
	yiMutexLock(m_hMutex);

	if (m_cnt==m_maxcnt)
	{
		assert(0);
		return False;
	}

	m_add_queue.enq(fd);
	m_cnt++;

	yiMutexUnlock(m_hMutex);

	int id=ePID_ADD_CONNECTION;
	if (write(m_w_acceptFd, &id, sizeof(int))!=sizeof(int))
	{
		assert(0);
	}

YIDEBUG("csvryConn::Add(%d) exit\n", fd);
//yiTrace("out add...................\n");
	return True;
}

// ------------------------------------
int csvryConn::AddFromQueue(OnAcceptedCallback callback, void* pClassCallback)
{
	yiMutexLock(m_hMutex);

	int n = m_add_queue.count();

	for (int i=0; i<n; i++)
	{
		int sock;
		int index;

		if (m_add_queue.deq((yiAnyValue&)sock)==false)
		{
			assert(0);
		}

		if (m_brank_queue.count()==0)
		{
			assert(0);
			return -1;
		}
		if (m_brank_queue.deq((yiAnyValue&)index)==false)
		{
			assert(0);
			return -1;
		}

YIDEBUG("csvryConn::AddFromQueue() %d/%d (room:%d)\n", i, n, m_index*m_maxcnt+index+1);
		if (callback(pClassCallback, sock, m_index*m_maxcnt+index+1, &m_fdsExInfo[index])==True)
		{
			m_fds[index] = sock;
			//FdSet(index);
			//FD_SET(m_fds[index], &m_rfdset);
			m_fds[m_maxcnt] = sock > m_fds[m_maxcnt] ? sock: m_fds[m_maxcnt]; 
		}
		else
		{
			m_fds[index] = -1;
			m_brank_queue.enq((yiAnyValue)index);
			close(sock);
		}
	}

	yiMutexUnlock(m_hMutex);

	return n;
}

// ------------------------------------
void csvryConn::RemoveFd(int n)
{
	yiMutexLock(m_hMutex);

	int sock = m_fds[n];

	FD_CLR(sock, &m_rfdset);
	m_fds[n] = -1;
	m_fdsExInfo[n]=NULL;
	m_brank_queue.enq((yiAnyValue)n);

	if (sock==m_fds[m_maxcnt])
	{
		UpdateMaxValue();
	}

	m_cnt--;
	yiMutexUnlock(m_hMutex);
}

//
Bool csvryConn::IsFull()
{
	return (m_cnt==m_maxcnt);
}

//
int* csvryConn::GetFds()
{
	return m_fds;
}

//
void** csvryConn::GetExtraDataPtr(int index)
{
	return &m_fdsExInfo[index];
}

//
void csvryConn::FdSets()
{
	for (int i=0; i<m_maxcnt; i++)
	{
		if (m_fds[i]==-1)
			continue;
		/*if (write(m_fds[i], &i, 0)!=0)
		{
			yiTrace("in fdsets connection closed\n");
			close(m_fds[i]);
			RemoveFd(i);
			continue;
		}*/
		FD_SET(m_fds[i], &m_rfdset);
	}
}

//
Bool csvryConn::Wait(cyiQueue& rdFds, OnAcceptedCallback callback, OnClosedCallback callback_close, void* pClassCallback)
{
	int res;

__LOOP__:
	FdSets();
	FD_SET(m_r_acceptFd, &m_rfdset);

	//yiTrace("in select........ %x %d\n", m_rfdset, sizeof(m_rfdset));
	res = select(m_fds[m_maxcnt]+1, &m_rfdset, 0, 0, 0);
	//yiTrace("out select........%x\n", m_rfdset);

	if (res<0)
	{
		yiTrace("select error.......... %d\n", res);	

		if (errno==EBADF)
		{
			struct timeval zerotv = {0, 0};
			fd_set temp;
			FD_ZERO(&temp);
			
			for (int i=0; i<m_maxcnt; i++)
			{
				FD_SET(m_fds[i], &temp);
				if (select(m_fds[i]+1, &temp, 0, 0, &zerotv)<0)
				{
					yiTrace("already closed (%d):%d\n", i, m_fds[i]);
					callback_close(pClassCallback, m_fds[i], i, &m_fdsExInfo[i]);
					RemoveFd(i);
				}
				FD_CLR(m_fds[i], &temp);
			}

			goto __LOOP__;
		}
	
		return False;
	}

	if (FD_ISSET(m_r_acceptFd, &m_rfdset)!=0)
	{
		int id;
		read(m_r_acceptFd, &id, sizeof(int));

		if ((id&ePID_TERMINATE)!=0)
		{
			return False;
		}
		else if (id==ePID_ADD_CONNECTION)
		{
			AddFromQueue(callback, pClassCallback);
		}
	}

	for (int i=0; i<m_maxcnt; i++)
	{
		if (m_fds[i]==-1)
			continue;

		if (FD_ISSET(m_fds[i], &m_rfdset)!=0)
		{
			//yiTrace("fdisset true: %d, %d\n", i, m_fds[i]);
			if (rdFds.enq((yiAnyValue)i)==false)
			{
				assert(0);
			}
		}
	}	

	if (rdFds.count()==0)
	{
		goto __LOOP__;
	}

	return True;	
}

// ------------------------------------
void csvryConn::UpdateMaxValue()
{
	int max = m_r_acceptFd;

	for (int i=0; i<m_maxcnt; i++)
	{
		max = (max < m_fds[i]) ? m_fds[i]:max;
	}

	m_fds[m_maxcnt] = max;
}

// ------------------------------------
void csvryConn::BroadCastMessage(void* encoded_packet, int size)
{
	for (int i=0; i<m_maxcnt; i++)
	{
		if (m_fds[i]==-1)
			continue;

		yisend(m_fds[i], encoded_packet, size, FALSE);
	}
}

// ------------------------------------
void csvryConn::RequestTerminate(int reason)
{
	if (write(m_w_acceptFd, &reason, sizeof(int))!=sizeof(int))
	{
		assert(0);
	}
}

// ------------------------------------
void csvryConn::RemoveAll()
{
	for (int i=0; i<m_maxcnt; i++)
	{
		if (m_fds[i]!=-1)
			close(m_fds[i]);
	}
}
