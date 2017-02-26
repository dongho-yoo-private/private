#include "svrybase.h"
#include "yiQueue.h"
#include "yithread.h"
#include "yitrace.h"

#include "yistd.h"
// --------------------------------------------------------
void* xOnDispatch(void* param)
{
	sThreadParam* p = (sThreadParam*)param;
	csvrybase* pClass = (csvrybase*)p->pClassInstance;
	pClass->OnDispatch(p->param[0], p->param[1], p->param[2]);

	_THREAD_END_(param);	
}

// --------------------------------------------------------
void csvrybase::OnLoopNormal(unsigned long param1, unsigned long param2, unsigned long param3)
{
	Bool bIsLocalLoop= True;
#ifndef WIN32
	int pipe_fd[2];
#else
	HANDLE pipe_fd[2];
#endif
	int listensock = (int)param1;
	cyiQueue threadQueue;
	//csvryConnMgr& mgr = *(new csvryConnMgr());
	csvryConnMgr mgr; //= *(new csvryConnMgr());


	sServerInformation* pInfo=&m_LocalInfo;

	if (listensock!=m_LocalSock)
	{
		pInfo=&m_ServerInfo;
		bIsLocalLoop=False;
		m_GlobalConMgr=&mgr;
	}
	else
	{
		m_LocalConMgr=&mgr;
	}

	YIINFO( "OnLoopNormal %s listen success\n", bIsLocalLoop?"Local":"Server");


	if (pipe(pipe_fd)!=0)
	{
		YIERROR( "pipe error!\n");
		assert(0);
	}

	if (bIsLocalLoop==True)
	{
		m_LocalLoopFd = pipe_fd[1];
	}
	else
	{
		m_ServerLoopFd = pipe_fd[1];
	}


	if (mgr.Create(pipe_fd[1], pInfo->nMaxConnections, pInfo->nMaxArrayCount)==False)
	{
		YIERROR( "unexpected case!\n");
		return ;
	}


	Bool bIsTerminate=False;
	int listcnt=0;
	// -----------------------------------
	while(bIsTerminate==False)
	{
		unsigned int from;
		int sock = Accept(listensock, pipe_fd[0], bIsTerminate, &from);

		if (bIsTerminate==True)
		{
			break;
		}

		if (sock==-1)
			continue;

		Bool bIsAcceptable;
		if (bIsLocalLoop==True)
		{
			bIsAcceptable = OnPreAcceptLocal(sock);
		}
		else
		{
			bIsAcceptable = OnPreAccept(sock, from);
			m_nConnectionCount++;
			m_nPeekConnectionCount=m_nConnectionCount>m_nPeekConnectionCount?m_nConnectionCount:m_nPeekConnectionCount;
		}

		if (bIsAcceptable==False)
		{
			m_nConnectionCount--;
			__close__(sock);
			continue;
		}

		csvryConn* pConnInfo=0;
	
		if ((pConnInfo = mgr.GetBlankConnectionInfo())!=0)
		{
			pConnInfo->Add(sock);	
			continue;
		}

		yiTrace("new connection list create.... cnt:%d\n", listcnt++);
		m_nConMgrCount++;
		pConnInfo = new csvryConn(pInfo->nMaxConnections, mgr.GetArrayCnt(), pipe_fd[1], bIsLocalLoop);
		pConnInfo->Add(sock);
		mgr.Add(pConnInfo);

		sThreadParam s = {this, (unsigned long)pConnInfo, bIsLocalLoop, pipe_fd[1], 0};
		pthread_t id = yiStartThread(xOnDispatch, &s, True);
		//yiThreadStartExj(this, &csvrybase::OnDispatch, pConnInfo, bIsLocalLoop, pipe_fd[1]);


		if (id==(pthread_t)-1)
		{
			YIERROR( "Thread Create Failure!!\n");
			m_bTerminate=True;
			break;
		}

		threadQueue.enq((yiAnyValue)id);

	} // while

	mgr.BroadCastQuit(m_QuitMode);

	if (m_bTerminate==True)
	{
		pthread_t threadId;
		while(threadQueue.deq((yiAnyValue&)threadId)==True)
		{
			yiThreadWait(threadId);	
		}	
	}

	__close__(pipe_fd[0]);
	__close__(pipe_fd[1]);
	__close__(listensock);

	mgr.Destroy();

	//delete &mgr;

	YIINFO("csvrybase::OnLoopNormal() exit\n");

	return ;
		
}

void* xOnLimitedDispatch(void* param)
{
	sThreadParam* p = (sThreadParam*)param;
	csvrybase* pClass = (csvrybase*)p->pClassInstance;
	pClass->OnLimitedDispatch(p->param[0], p->param[1], p->param[2]);

	_THREAD_END_(param);	
}

// --------------------------------------------------------
void csvrybase::OnLoopLimited(unsigned long param1, unsigned long param2, unsigned long param3)
{
	int listensock = (int)param1;
	int pipe_fd[2];
	Bool bIsLocalLoop=False;
	cyiQueue threadQueue;


	if (listensock==m_LocalSock)
	{
		bIsLocalLoop = True;
	}

	YIINFO( "OnLoopLimited %s listen success\n", bIsLocalLoop?"Local":"Server");
	if (pipe(pipe_fd)!=0)
	{
		YIERROR( "pipe error!\n");
		assert(0);
	}

	if (bIsLocalLoop==True)
	{
		m_LocalLoopFd = pipe_fd[1];
	}
	else
	{
		m_ServerLoopFd = pipe_fd[1];
	}


	Bool bIsTerminate=False;
	
	while(bIsTerminate==False)
	{
		unsigned int from;
		int sock = Accept(listensock, pipe_fd[0], bIsTerminate, &from);

		if (bIsTerminate==True)
		{
			m_bTerminate=True;
			break;
		}

		if (sock==-1)
		{
			continue;
		}

		if (bIsLocalLoop == False)
		{
			YIDEBUG("before OnPreAccept()\n");
			if (OnPreAccept(sock, from)==False)
			{
				__close__(sock);
				continue;
			}
		}
		else
		{
			if (OnPreAcceptLocal(sock)==False)
			{
				__close__(sock);
				continue;
			}
		}

		sThreadParam s = {this, sock, bIsLocalLoop, pipe_fd[1], 0, };
		pthread_t id = yiStartThread(xOnLimitedDispatch, &s, True);//sock, bIsLocalLoop, pipe_fd[1]);
		//pthread_t id = yiThreadStartEx(this, &csvrybase::OnLimitedDispatch, sock, bIsLocalLoop, pipe_fd[1]);

		if (id==(pthread_t)-1)
		{
			YIERROR( "Thread Create Failure!!\n");
			m_bTerminate=True;
			break;
		}
	}

	close(pipe_fd[0]);
	close(pipe_fd[1]);
	__close__(listensock);

	YIINFO("csvrybase::OnLoopLimited() exit\n");
}

