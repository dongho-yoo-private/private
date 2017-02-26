#include "svrybase.h"


// -----------------------------------------------
Bool csvrybase::Start(sServerInformation* server, sServerInformation* local)
{
#ifndef WIN32
	::daemon(1,1);
	yiTrace("start > %s\n", m_szServerName);
	if (csycEnv::GetServerDefinition(m_szServerName, &m_ServerInfo, &m_LocalInfo)==False)
	{
		SYSLOG(LogLevel::NOTIFY, "no server difinition file\n");
		return False;
	}

	m_LocalInfo  = local ? *local:m_LocalInfo;
	m_ServerInfo = local ? *server:m_ServerInfo;

#endif
	if (OnServerStart()==False)
	{
		yiTrace("OnServerStart Failure\n");
		return False;
	}

#ifndef WIN32
	if (CreateLocalSession(m_LocalInfo)==-1)
	{
		yiTrace("CreateLocalSession error\n");
		return False;
	}
#endif

	if (CreateServerSession(m_ServerInfo)==-1)
	{
		yiTrace("CreateServerSession error\n");
		return False;
	}


	Loop();
	OnServerEnd();

	return True;
}

// -----------------------------------------------
int csvrybase::Accept(int listensock, int pipefd, Bool& isTerminate, unsigned int* from)
{
	struct sockaddr_un addr_accept_un;
	struct sockaddr_in addr_accept_in;
	struct sockaddr*p = (struct sockaddr*)&addr_accept_un;
	fd_set rfdset;

	FD_ZERO(&rfdset);

	if (listensock!=m_LocalSock)
	{
		p=(struct sockaddr*)&addr_accept_in;
	}
	
	int max = listensock> pipefd ? listensock:pipefd ;

	FD_SET(listensock, &rfdset);
	FD_SET(pipefd, &rfdset);
	if (select(max+1, &rfdset, 0, 0, 0)<0)
	{
		SYSLOG(LogLevel::NOTIFY, "accept select error");
		return -1;
	}

	if (FD_ISSET(pipefd, &rfdset)!=0)
	{
		int id;
		if (read(pipefd, &id, sizeof(int))!=sizeof(int))
		{
			isTerminate=true;
			return -1;
		}

		if ((id&ePID_TERMINATE)!=0)
		{
			isTerminate=true;
			return -1;
		}
	}

	if (FD_ISSET(listensock, &rfdset)!=0)
	{
		socklen_t addr_size=sizeof(*p);
		int ret = accept(listensock, p, &addr_size);

		//yiTrace("accept...... (%d)\n", ret);

		if (ret==-1)
		{
			return -1;
		}

		if (from)
		{
			*from = addr_accept_in.sin_addr.s_addr;
		}

		return ret;
	}

	return -1;
}

// -----------------------------------------------
long csvrybase::CreateLocalSession(sServerInformation& info)
{
	int sock;
	if ((sock =  socket(AF_UNIX, SOCK_STREAM, 0))<0)	
	{
		SYSLOG(LogLevel::ERROR, "create local socket error (%s)\n");
		return -1;
	}
	
	struct sockaddr_un my_addrLocal;
	struct sockaddr* p;
	int nAddrSize;
	char localport[256];
	
	memset(&my_addrLocal, 0, sizeof(my_addrLocal));
	my_addrLocal.sun_family = AF_UNIX;
	strncpy(my_addrLocal.sun_path, csycEnv::GetLocalPortName(m_szServerName, localport), sizeof(my_addrLocal.sun_path));
	p=(struct sockaddr*)&my_addrLocal;
	nAddrSize = sizeof(my_addrLocal);

	::remove(localport);
	if (bind(sock, (struct sockaddr*)p, nAddrSize)==-1)
	{
		SYSLOG(LogLevel::ERROR, "Local bind error (Local) %s\n", info.szServerAddress);
		__close__(sock);
		return -1;
	}

	//yiTrace("listen queue %d\n", info.nMaxListenWaitCnt);
	if (listen(sock, info.nMaxListenWaitCnt)==-1)
	{
		SYSLOG(LogLevel::ERROR, "Local listen error (Local)\n");
		__close__(sock);
		return -1;
	}

	m_security = cyiEncoder::GetYotsuyaSecurityCode(m_szServerName);
	if (m_security==0)
	{
		__close__(sock);
		return -1;
	}

	yiTrace("security code (%x)\n", m_security);//.nRandNo, m_security.nMask, m_security.nSecurity);

	m_LocalSock=sock;
	m_LocalInfo=info;

	if (info.type==SERVER_TYPE_NORMAL)
	{
		m_LocalLoopThread_t = yiThreadStartExj(this, &csvrybase::OnLoopNormal, sock, 0);
	}
	else
	{
		m_LocalLoopThread_t = yiThreadStartExj(this, &csvrybase::OnLoopLimited, sock, 0);
	}

	if (m_LocalLoopThread_t==-1)
	{
		return -1;
	}

	return 0;
}


// -----------------------------------------------
long csvrybase::CreateServerSession(sServerInformation& info)
{
	int sock;
	if ((sock =  socket(AF_INET, SOCK_STREAM, 0))<0)	
	{
		SYSLOG(LogLevel::ERROR, "create local socket error (%s)\n");
		return -1;
	}
	
	struct sockaddr_in my_addr;
	struct sockaddr* p;
	int nAddrSize;
	
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(info.nPortNo);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	nAddrSize = sizeof(my_addr);

	int on=1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));	
	if (bind(sock, (struct sockaddr*)&my_addr, nAddrSize)==-1)
	{
		SYSLOG(LogLevel::ERROR, "Server bind error (%d)\n", info.nPortNo);
		__close__(sock);
		return -1;
	}

	if (listen(sock, info.nMaxListenWaitCnt)==-1)
	{
		SYSLOG(LogLevel::ERROR, "Server listen error \n");
		__close__(sock);
		return -1;
	}

	m_Sock=sock;
	m_ServerInfo=info;
	if (info.type==SERVER_TYPE_NORMAL)
	{
		m_LoopThread_t = yiThreadStartExj(this, &csvrybase::OnLoopNormal, sock, 0);
	}
	else
	{
		m_LoopThread_t = yiThreadStartExj(this, &csvrybase::OnLoopLimited, sock, 0);
	}

	return 0;
}

// -----------------------------------------------
void csvrybase::QuitMessage(int mode)
{
	m_QuitMode=mode;

	if (write(m_LocalLoopFd, &m_QuitMode, sizeof(int))!=sizeof(int))
	{
		assert(0);
	}

	if (write(m_ServerLoopFd, &m_QuitMode, sizeof(int))!=sizeof(int))
	{
		assert(0);
	}
}
// -----------------------------------------------
Bool csvrybase::Loop()
{
	yiThreadWait(m_LoopThread_t);
	yiThreadWait(m_LocalLoopThread_t);
}

