#include "ypn.h"
#include "yistd.h"
#include "yiEncoder.h"
//#include "yithread.h"
#include "yisock.h"
#include "yiencodex.h"
#include "yiupnp.h"

#ifdef WIN32
#include <process.h>
#else
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#endif

#define YPN_DEFAULT_SECURITY 0xe317af5c

// ---------------------------
cyPrivateNetwork* cyPrivateNetwork::me =0;

cyPrivateNetwork::cyPrivateNetwork()
: m_nLastError(0)
, m_MainSock(0)
, m_udpMainSock(0)
{
	me=this;
	Init();
}

// ---------------------------
cyPrivateNetwork::~cyPrivateNetwork(void)
{
}

// ---------------------------
cyPrivateNetwork* cyPrivateNetwork::GetInstance(void)
{
	if (me)
		return me;
	return 0;
}

// ---------------------------
#ifdef WIN32
static unsigned int __stdcall xOnMsgLoop(void* param)
#else
void* xOnMsgLoop(void* param)
#endif
{
	cyPrivateNetwork* p = (cyPrivateNetwork*)param;

	p->OnMsgLoop();
	return 0;
}

// ---------------------------
BOOL cyPrivateNetwork::UpnpTest(const char* pszLocalAddress, int localport, Bool* bReasonIsPort)
{
	char szExternalAddress[16];
	m_hUpnp=yiupnpCreatex();
	int i=0;

	*bReasonIsPort=False;

	if (m_hUpnp==0)
	{
		return FALSE;
	}

	if (yiupnpGetExternalAddressx(m_hUpnp, szExternalAddress)==FALSE)
	{
		yiupnpDestroyx(m_hUpnp);
		m_hUpnp=0;
		return False;
	}

	xTRACE("cyPrivateNetwork::UpnpTest() GetExternalAddress:%s\n", szExternalAddress);

	if (yiupnpAddPortMapx(m_hUpnp, (char*)pszLocalAddress, localport, True, "GoYotsuyaClient")!=True)
	{
		yiupnpDestroyx(m_hUpnp);
		m_hUpnp=0;
		*bReasonIsPort=True;
		xTRACE("cyPrivateNetwork::UpnpTest() PortMap Failure Tcp\n");
		return False;
	}
	if (yiupnpAddPortMapx(m_hUpnp, (char*)pszLocalAddress, localport, False, "GoYotsuyaClientUdp")!=True)
	{
		yiupnpDestroyx(m_hUpnp);
		yiupnpDeletePortMapx(m_hUpnp, localport, True);
		*bReasonIsPort=True;
		m_hUpnp=0;
		xTRACE("cyPrivateNetwork::UpnpTest() PortMap Failure Udp\n");
		return False;
	}

	m_myinfo.network.type    = eNT_UPNP;
	m_myinfo.network.address = yiitol(szExternalAddress);
	m_myinfo.network.port    = localport;
	m_myinfo.network.security = rand();

	return TRUE;
}

// ---------------------------
void cyPrivateNetwork::Init()
{
	m_MainSock = 0;
	m_udpMainSock=0;
	m_AcceptSock =0;
	m_UdpAcceptSock=0;
	m_wakeupSock=0;
	m_socks[128];
	memset(&m_myinfo.network, 0, sizeof(m_myinfo.network));
	memset(&m_myinfo, 0, sizeof(m_myinfo));
	m_hUpnp=0;
	m_QuitMessage=False;
	m_bStartKeepAlive=False;


	for (int i=0; i<32; i++)
	{
		m_TimerList[i] = 0;
	}
#ifdef WIN32
	m_hNotifyWnd = 0;	
#else
	m_NotifyEvent = 0;
	m_NotifyEventQueue =0;
#endif

	for (int i=0; i<128; i++)
	{
		m_socks[i]=0;
	}
}

// ---------------------------
void cyPrivateNetwork::InitNetwork(unsigned int address, int port)
{
	Bool reasonIsPort=False;
	int nRetestCnt=3;

	m_myinfo.network.localaddr 	= address ? address:yigetlocaladdress(0, 0);
	m_myinfo.network.localport 	= port ? port : YPN_DEFAULT_PORT+((m_myinfo.network.localaddr&0xFF000000)>>24);

RE_TEST:
	if (nRetestCnt==0)
	{
		m_myinfo.network.type=eNT_UNKNOWN;
		m_AcceptSock = 0;
		return ;
	}

	m_myinfo.network.type = UpnpTest(yiltostr(m_myinfo.network.localaddr), m_myinfo.network.localport, &reasonIsPort) ? eNT_UPNP:eNT_UNKNOWN;

	if (reasonIsPort==True)
	{
		m_myinfo.network.localport++;
		nRetestCnt--;
		goto RE_TEST;
	}

	m_AcceptSock=0;
	if (m_myinfo.network.type==eNT_UPNP)
	{
		int sock = yilisten(0, m_myinfo.network.localport, 16);	
		int udpSock;

		if (sock<0)
		{
			yiupnpDeletePortMapx(m_hUpnp, m_myinfo.network.localport, True);
			yiupnpDeletePortMapx(m_hUpnp, m_myinfo.network.localport, False);
			m_myinfo.network.localport++;
			goto RE_TEST;
		}

		if ((udpSock=yiudp(0, m_myinfo.network.localport))<0)
		{
			yiupnpDeletePortMapx(m_hUpnp, m_myinfo.network.localport, True);
			yiupnpDeletePortMapx(m_hUpnp, m_myinfo.network.localport, False);
			m_myinfo.network.localport++;
			yiclose(sock);
			goto RE_TEST;
		}

		m_AcceptSock = uthpSock(sock, eUST_ACCEPT_SOCK, 0, 0, 0);
		m_UdpAcceptSock = uthpSock(udpSock, eUST_ACCEPT_UDP_SOCK, 0, 0, 0);
	}

	if (m_myinfo.network.type==eNT_UPNP)
	{
		yiupnpSleep(m_hUpnp);
		xTRACE("upnp success...%s:%d\n", yiltostr(m_myinfo.network.address), m_myinfo.network.port);
	}
	else
	{
		yiupnpDestroyx(m_hUpnp);
		m_hUpnp=0;
	}
	return ;
}

// ------------------------------------
socket_t cyPrivateNetwork::ConnectToServer(unsigned int server, int port, unsigned int security)
{
	return uthpTcpConnect(server, port, security);
}

// ------------------------------------
void cyPrivateNetwork::DisconnectFromServer(socket_t sock)
{
	if (sock==0)
	{
		sock=m_MainSock;
	}
	//RemoveDescriptor(sock);
	DestroySocket(sock);
	m_MainSock=0;
	return ;
}

// ------------------------------------
Bool cyPrivateNetwork::Send(socket_t sock, int id, void* buffer, int size)
{
	return uthpSend(sock, id, buffer, size);
}

// ------------------------------------
int cyPrivateNetwork::Receive(socket_t sock, int* id, void* buffer, int* size)
{
	if (sock==0)
	{
		sock=m_MainSock;
	}
	return uthpRecv(sock, id, buffer, size, 1000);
}

// ------------------------------------
int cyPrivateNetwork::Receive(socket_t sock, void* buffer, int size, int nTimeout)
{
	int receved=0;
	if (sock==0)
	{
		sock=m_MainSock;
	}

	if ((receved=yirecv(sock->sock, buffer, size, nTimeout==-1?0:&nTimeout))!=size)
	{
		return False;
	}

	return True;
}

// ---------------------------
#ifdef WIN32
BOOL cyPrivateNetwork::Start(HWND hWndNotify, UINT msgId, unsigned int server, int port, unsigned int security, unsigned int id, const char* pszName, const char* pszPassword, unsigned int localaddr)
#else
BOOL cyPrivateNetwork::Start(unsigned int event, unsigned int server, int port, unsigned int security, unsigned int id, const char* pszName, const char* pszPassword, unsigned int localaddr)
#endif
{
	Bool bIsUpnpSuccess=False;
	int udpSock;

#ifdef WIN32
	m_hNotifyWnd = hWndNotify;
	m_NotifyMessageId = msgId&0xFFFF;
	m_NotifyServerMessageId=msgId>>16;
#else
	m_NotifyEvent = event;
	m_NotifyEventQueue = new cyiQueue(True);
#endif
	//if (pszName!=0xFF)
	//{
	strcpy(m_myinfo.szUserName, pszName);
	//}
	//else
	//{
	//	strcpy(m_myinfo.szTemporaryName, &pszName[1]);
	//}

	if (m_MainSock==0)
		m_MainSock = uthpTcpConnect(server, port, security);

	if (m_MainSock==0)
	{
		xTRACE("uthpTcpConnect() failure ip:%s, port:%d, security:%x\n", yiltostr(server), port, security);
		return False;
	}

	//if (pszName[0]==0xFF)
	//{
	//	pszName=0;
	//}

	//char buffer[1024];
	sGeneralParam buffer;
	int nReceived=0;
	//memset(buffer, 0, sizeof(buffer));
	if (uthpTcpLogin(m_MainSock, id, pszName, pszPassword, &m_myinfo.network)/*, m_AcceptSock, &buffer, &nReceived)*/<0)
	{
		xTRACE("uthpTcpLogin() login failure\n");
		m_nLastError=m_myinfo.id;
		DestroySocket(m_MainSock);
		m_MainSock=0;
		return FALSE;
	}

	if (id)
	{
		m_myinfo.id=buffer.param[1];
	}

	// ---------------------
	if (OnLogin((void*)&buffer, nReceived)==False)
	{
		// Unknown error.
		m_nLastError=-6;
		DestroySocket(m_MainSock);
		m_MainSock=0;
		return False;
	}

	xTRACE("uthpTcpLogin() login success.... %x, %s", m_myinfo.id, pszName);

	if (m_myinfo.network.type==eNT_UPNP)
	{
		goto SKIP_CHECK_NETWORK;
	}

	udpSock=yiudp(localaddr?localaddr:0, 0);
	if (udpSock<0)
	{
		DestroySocket(m_MainSock);
		m_MainSock=0;
		return FALSE;
	}
	//m_NotifyEventQueue=new cyiQueue(True);

	if (localaddr==0)
	{
		char szIpAddr[32];
		char szPort[32];
		unsigned int port = yigetport(udpSock, 0);

		itoa(port, szPort, 10);
		localaddr=yigetlocaladdress(szIpAddr, szPort);
	}

	m_myinfo.network.localaddr = localaddr?localaddr:0;
	m_myinfo.network.localport = yigetport(udpSock, 0);

	if (uthpCheckMyNetwork(m_MainSock, udpSock, m_myinfo.network.localaddr, m_myinfo.network.localport, &m_myinfo.network.type, &m_myinfo.network.address, &m_myinfo.network.port)==FALSE)
	{
		xTRACE("uthpCheckMyNetwork() failure\n");
		DestroySocket(m_MainSock);
		m_MainSock=0;
		return FALSE;
	}

	yiclose(udpSock);

	if (m_myinfo.network.type==eNT_NONE ||
		m_myinfo.network.type==eNT_FULL_CON)
	{
		int port=m_myinfo.network.localport;
		int sock; 
		int udpSock;
		int nRetry=3;

RE_CREATE:
		if (nRetry==0)
		{
			DestroySocket(m_MainSock);
			m_MainSock=0;
			return False;
		}

		if ((sock=yilisten(m_myinfo.network.localaddr, port, 16))<0)
		{
			nRetry--;
			goto RE_CREATE;
		}

		port = yigetport(sock, 0);

		if ((udpSock=yiudp(m_myinfo.network.localaddr, port))<0)
		{
			yiclose(sock);
			port++;
			nRetry--;
			goto RE_CREATE;
		}

		m_AcceptSock = uthpSock(sock, eUST_ACCEPT_SOCK, 0, 0, 0);
		m_UdpAcceptSock = uthpSock(udpSock, eUST_ACCEPT_UDP_SOCK, 0, 0, 0);
	}
	else
	{
		m_myinfo.network.localport=0;
		m_myinfo.network.port=0;
	}

SKIP_CHECK_NETWORK:
	xTRACE("uthpRegisterMyNetwork() call\n");
	if (uthpRegisterMyNetwork(m_MainSock, m_myinfo.network.type, m_myinfo.network.address, m_myinfo.network.port)==FALSE)
	{
		xTRACE("uthpRegisterMyNetwork() failure\n");
		DestroySocket(m_MainSock);
		m_MainSock=0;

		return FALSE;
	}

	if (pszName!=0)
	{
		m_myinfo.network.security = cyiEncoder::GetYotsuyaSecurityCode(pszName);
	}
	else
	{
		m_myinfo.network.security = YPN_DEFAULT_SECURITY;
	}

	int wakeupSock = yiudp(yiitol("127.0.0.1"), 0);

	{
		syiAddress me = {0, yigetport(wakeupSock, 0)};
		m_wakeupSock = uthpSock(wakeupSock, eUST_WAKEUP_UDP, 0, 0, 0);
	}

	if (m_wakeupSock==0)
	{
		xTRACE("m_wakeupSock failure\n");
		DestroySocket(m_MainSock);
		m_MainSock=0;

		return FALSE;
	}


	m_QuitMessage=False;
	if (yiThreadStartx(xOnMsgLoop, this, 0)==FALSE)
	{
		xTRACE("xOnMsgLoop() failure\n");
		DestroySocket(m_MainSock);
		m_MainSock=0;

		return FALSE;
	}

	m_hDescriptorMutex = yiMutexCreateEx(3);
	return TRUE;
}

// ---------------------------
enum eWakeupMessage {
	eWUMSG_ANY=0,
	eWUMSG_TIMER=1,
	eWUMSG_ADD_DESCRIPTOR=2,
	eWUMSG_REMOVE_DESCRIPTOR=3,
	eWUMSG_TERMINATE=-1
		
};

// ------------------------------------
void cyPrivateNetwork::Terminate()
{
	Wakeup(eWUMSG_TERMINATE, 0);
}

// ---------------------------
Bool cyPrivateNetwork::Wakeup(int id, unsigned int data)
{
	int buffer[2] = {id, data};

	if (id==eWUMSG_TIMER)
	{
		Bool bIsTimerFound=False;
		for (int i=0; i<32; i++)
		{
			if (m_TimerList[i]==data)
			{
				bIsTimerFound=True;
				break;
			}
		}

		if (bIsTimerFound==False)
		{
			return False;
		}
	}

	yiMutexLock(m_wakeupSock->hMutex);
	xTRACE("Wakeup msg:%d, address:%u, port:%u", m_wakeupSock->remote.address, m_wakeupSock->remote.port);
	yisendu((int)m_wakeupSock->pExtraData, buffer, sizeof(buffer), m_wakeupSock->remote.address, m_wakeupSock->remote.port);
	yiMutexUnlock(m_wakeupSock->hMutex);

	return True;
}

#ifdef WIN32
unsigned int __stdcall xOnTimer(void* param)
#else
void* xOnTimer(void* param)
#endif
{
	sGeneralParam* x = (sGeneralParam* )param;
	int id = x->param[0];
#ifdef WIN32
	int nTime = x->param[1];
#else
	int nTime = (x->param[1])*1000;
#endif
	int bIsSoon = x->param[2];
	int bIsOnce = x->param[3];
	
	if (bIsOnce==True)
	{
#ifdef WIN32
		Sleep(nTime);
#else
		usleep(nTime);
#endif
		cyPrivateNetwork::GetInstance()->Wakeup(eWUMSG_TIMER, id);
		yifree(x);
#ifdef WIN32
		return 0;
#else
		return 0;
#endif
	}

	while(1)
	{
#ifdef WIN32
		Sleep(nTime);
#else
		usleep(nTime);
#endif
		if (cyPrivateNetwork::GetInstance()->Wakeup(eWUMSG_TIMER, id)==False)
		{
			break;
		}
	}

	yifree(x);
#ifdef WIN32
		return 0;
#else
		return 0;
#endif
}

// ------------------------------------
Bool cyPrivateNetwork::SetTimer(int id, int nTime, Bool bIsSoon, Bool bIsOnce)
{
	sGeneralParam param = {id, nTime, bIsSoon, bIsOnce, 0};
	Bool bIsFull=True;

	for (int i=0; i<32; i++)
	{
		if (m_TimerList[i]==0)
		{
			m_TimerList[i]=id;
			bIsFull=False;
			break;
		}
	}

	if (bIsFull==True)
	{
		return False;
	}

	return yiThreadStartx(xOnTimer, yitoheap(&param, GP_SIZE(4)), GP_SIZE(4));
}

// ------------------------------------
void cyPrivateNetwork::StopTimer(int id)
{
	for (int i=0; i<id; i++)
	{
		if (m_TimerList[i]==id)
		{
			m_TimerList[i]=0;
			break;
		}
	}	
}

// ------------------------------------
socket_t cyPrivateNetwork::ConnectRemote(address_t address, int port, sypnUserInfo* remote)
{
	int sock = yiconnect(address, port, 3);
	if (sock<0)
	{
		xTRACE("yiconnection fialure %s:%d\n", yiltostr(address), port);
		return 0;
	}
	
	if (yisendx(sock, 0, YPN_DEFAULT_SECURITY, YPN_DEFAULT_SECURITY, &m_myinfo, sizeof(m_myinfo), False)!=sizeof(m_myinfo))
	{
		yiclose(sock);
		return 0;
	}

	syipacket packet;
	if (yirecvx(sock, &packet, remote, YPN_DEFAULT_SECURITY, 500)!=sizeof(*remote))
	{
		yiclose(sock);
		return 0;
	}

	

	socket_t sock_remote = uthpSock(sock, eUST_TCP, m_myinfo.network.security, &remote->network, yitoheap(remote, sizeof(*remote)));
	//sock_remote->pExtraData = yitoheap(&info, sizeof(info));
	return sock_remote;
}

// ------------------------------------
Bool cyPrivateNetwork::WaitMessage(socket_t sock, int w_id, int timeout, void* buffer, int*size)
{
	sock->w_id=w_id;

	if (size)
		*size=0;

	xTRACE("Event Reset %d\n", sock->hEvent);
	ResetEvent(sock->hEvent);
	if (WaitForSingleObject(sock->hEvent, timeout)!=WAIT_OBJECT_0)
	{
		SetEvent(sock->hEvent);
		sock->w_id=0;
		return False;
	}

	yiMutexLock(sock->hMutex);
	if (sock->wbuffer==0)
	{
		yiMutexUnlock(sock->hMutex);
		return True;
	}

	if (buffer==0)
	{
		yifree(sock->wbuffer);
		sock->wbuffer=0;
		yiMutexUnlock(sock->hMutex);
		return True;
	}

	memcpy(buffer, sock->wbuffer, sock->wsize);
	
	if (size)
	{
		*size=sock->wsize;
	}

	yifree(sock->wbuffer);
	sock->wbuffer=0;
	sock->w_id=0;
	yiMutexUnlock(sock->hMutex);

	return True;
}

// ------------------------------------
void cyPrivateNetwork::AddUdpMainSock(int sock)
{
	m_udpMainSock = uthpSock(sock, eUST_UDP, 0, 0, 0);
	Wakeup(eWUMSG_ADD_DESCRIPTOR, (unsigned int)0);
	//AddDescriptor(m_udpMainSock, 0);
	return ;
}

// ------------------------------------
socket_t cyPrivateNetwork::StartHolePunching(int sockUdp, address_t addr, int port, int security, sypnUserInfo* remote)
{
	socket_t sock=0;

	if ((sock=uthpUdpHolePunching2(sockUdp, addr, port, security, 5))==0)
	{
		return 0;
	}

	if (uthpSend(sock, 0, &m_myinfo, sizeof(m_myinfo))!=True)
	{
		xTRACE("cyPrivateNetwork::StartHolePunching() send MyInfo error");
		uthpClose(sock);
		return 0;
	}

	int id, size;
	if (uthpRecv(sock, &id, remote, &size, 1000)==False)
	{
		xTRACE("cyPrivateNetwork::StartHolePunching() recv remote error");
		uthpClose(sock);
		return 0;
	}

	sock->remote.type = remote->network.type;
	sock->remote.index = remote->network.index;
	sock->remote.localaddr = remote->network.localaddr;
	sock->remote.localport = remote->network.localport;
	sock->remote.security = remote->network.security;

	//memcpy(&sock->remote, &user_info.network, sizeof(sNetworkInfo));

	//sock->pExtraData = yitoheap(&user_info, sizeof(user_info));

	return sock;
}



// ------------------------------------
socket_t cyPrivateNetwork::StartHolePunching(socket_t sock, int port, int security, sypnUserInfo* remote)
{
	if ((sock=uthpUdpHolePunching(sock, port, security, 5))==0)
	{
		return 0;
	}

	xTRACE("cyPrivateNetwork::StartHolePunching() holepunching success sock:%x", sock);


	if (uthpSend(sock, 0, &m_myinfo, sizeof(m_myinfo))!=True)
	{
		xTRACE("cyPrivateNetwork::StartHolePunching() send MyInfo error");
		uthpClose(sock);
		return 0;
	}

	/*char buffer[1024];
	int res = recvfrom(sock->sock, buffer, 1024, 0, 0, 0);*/

	int id, size;
	if (uthpRecv(sock, &id, remote, &size, 1000)==False)
	{
		xTRACE("cyPrivateNetwork::StartHolePunching() recv remote error");
		uthpClose(sock);
		return 0;
	}


	sock->remote.type = remote->network.type;
	sock->remote.index = remote->network.index;
	sock->remote.localaddr = remote->network.localaddr;
	sock->remote.localport = remote->network.localport;
	sock->remote.security = remote->network.security;

	//memcpy(&sock->remote, &user_info.network, sizeof(sNetworkInfo));

	//sock->pExtraData = yitoheap(&user_info, sizeof(user_info));

	return sock;
}

// ------------------------------------
socket_t cyPrivateNetwork::WaitAcceptMessage(int index, int timeout)
{
	yiMutexLock(m_AcceptSock->hMutex);
	m_AcceptSock->w_id=index;
	ResetEvent(m_AcceptSock->hEvent);
	yiMutexUnlock(m_AcceptSock->hMutex);
	if (WaitForSingleObject(m_AcceptSock->hEvent, 1000)==False)
	{
		yiMutexLock(m_AcceptSock->hMutex);
		m_AcceptSock->w_id=0;
		ResetEvent(m_AcceptSock->hEvent);
		return 0;
	}
	yiMutexLock(m_AcceptSock->hMutex);
	m_AcceptSock->w_id=0;
	ResetEvent(m_AcceptSock->hEvent);

	return (socket_t)m_AcceptSock->wbuffer;
}

// ------------------------------------
Bool cyPrivateNetwork::AcceptFrom(int sock, sypnUserInfo* remote)
{
	socket_t ret=0;
	address_t	addr=0;
	int			port=0;
	int			timeout=500;
	char		ch=0;

	if (yirecvu(sock, &ch, 1, &timeout, &addr, &port)<1)
	{
		return False;
	}

	if ((ret=StartHolePunching(sock, addr, port, 0, remote))!=0)
	{
		int res = AddDescriptor(ret, remote);
		if (res==-1)
		{
			this->DestroySocket(ret);
			return -1;
		}
		Wakeup(eWUMSG_ADD_DESCRIPTOR, (unsigned int)0);
		return True;
	}

	return False;
}


// ------------------------------------
Bool cyPrivateNetwork::Connect(address_t addr, int port, sypnUserInfo* info)
{
	int sock = yiudp(0, 0);
	socket_t ret=0;

	if ((ret=StartHolePunching(sock, addr, port, 0, info))!=0)
	{
		int res = AddDescriptor(ret, info);
		if (res==-1)
		{
			this->DestroySocket(ret);
			return -1;
		}
		Wakeup(eWUMSG_ADD_DESCRIPTOR, (unsigned int)0);
		return True;
	}

	return False;
}

// ------------------------------------
BOOL cyPrivateNetwork::Connect(int Id, const char* pszUserName, int nRoomNo, const char* pszPassword, sypnUserInfo* info)
{
	int res=-1;
	sGeneralParam param;
	int size;

	if (Id==0)
	{
		if (uthpSendRCU(m_MainSock, m_myinfo.network.index, nRoomNo, pszPassword)==False)
		{
			xTRACE("uthpSendRCU() False\n");
			return -1;
		}
	}
	else
	{
		int size;
		if (uthpSendRCUx(m_MainSock, Id)==False)//m_myinfo.network.index, nRoomNo, pszPassword)==False)
		{
			xTRACE("uthpSendRCU() False\n");
			return -1;
		}
	}

	if (WaitMessage(m_MainSock, eUSPID_RPY_CONNECT_UDP, 500, &param, &size)==False)
	{
		xTRACE("WaitMessage() False\n");
		return -1;
	}

	if (param.param[0]!=eERCU_OK)
	{
		xTRACE("eUSPID_RPY_CONNECT_UDP result:%d\n", param.param[0]);
		return -1;
	}

	xTRACE("Connect method %d\n", param.param[1]);

	switch(param.param[1])
	{
		// 直接　TCPコネクションを行う（廃止するかも。）
		case eCM_DIRECT:
		{
			xTRACE("Connect Remote.... Direct\n");
			socket_t sock = ConnectRemote(param.param[2], param.param[3], info);

			if (sock==0)
			{
				return -1;
			}

			res = AddDescriptor(sock, info);
			if (res==-1)
			{
				uthpClose(sock);
				return -1;
			}
			Wakeup(eWUMSG_ADD_DESCRIPTOR, (unsigned int)0);
			break;

		}
		// 相手からのコネクションを待機する。
		case eCM_WAIT:
		{
			socket_t sock;
			xTRACE("Connect Wait.... \n");
			if ((sock=WaitAcceptMessage(param.param[2], 1000))==0)
			{
				return -1;
			}
			
			res = AddDescriptor(sock, (sypnUserInfo*)sock->pExtraData);
			yifree(sock->pExtraData);
			sock->pExtraData=0;
			if (res==-1)
			{
				uthpClose(sock);
				return -1;
			}
			Wakeup(eWUMSG_ADD_DESCRIPTOR, (unsigned int)0);
			break;


		}
		case eCM_USE_STUNE: // udp. hole punching.
		{
			xTRACE("udp Holepunching.... \n");
			socket_t sock = StartHolePunching(m_MainSock, param.param[3], param.param[4], info);

			if (!sock)
			{
				xTRACE("Holepunching.... failure\n");
				return -1;
			}

			res = AddDescriptor(sock, info);

			if (res==-1)
			{
				uthpClose(sock);
				return -1;
			}
			Wakeup(eWUMSG_ADD_DESCRIPTOR, (unsigned int)0);
			//res=sock->remote.index;
			break;

		}
		default:
			return -1;
	}
	
	return 0;
}

// ------------------------------------
BOOL cyPrivateNetwork::Connect(unsigned int address, int port, eNatType type, void* p)
{
	
	return True;
}

// ---------------------------
int cyPrivateNetwork::FdSet(fd_set* fds)
{
	int maxs=0;

	FD_ZERO(fds);
	FD_SET(m_MainSock->sock, fds);

	if (m_udpMainSock)
	{
		FD_SET(m_udpMainSock->sock, fds);
	}

	if (m_AcceptSock)
	{
		FD_SET(m_AcceptSock->sock, fds);
	}

	if (m_UdpAcceptSock)
	{
		FD_SET(m_UdpAcceptSock->sock, fds);
	}

	FD_SET(m_wakeupSock->sock, fds);

	maxs=m_wakeupSock->sock;

	for (int i=0; i<128; i++)
	{
		if (m_socks[i]!=0)
		{
			maxs = maxs>m_socks[i]->sock?maxs:m_socks[i]->sock;
			FD_SET(m_socks[i]->sock, fds);
		}
	}

	return maxs;
}

// ------------------------------------
const sNetworkInfo* cyPrivateNetwork::GetNetworkInfo()
{
	return &m_myinfo.network;
}

// ------------------------------------
sypnUserInfo* cyPrivateNetwork::GetMyInfo()
{
	return &m_myinfo;
}

// ---------------------------
const sypnUserInfo* cyPrivateNetwork::GetUserInfo(int index)
{
	if (m_socks[index]==0)
		return 0;
	return (sypnUserInfo* )m_socks[index]->pExtraData;
}

// ---------------------------
int cyPrivateNetwork::AddDescriptor(socket_t sock, sypnUserInfo* remote)
{
	yiMutexLock(m_hDescriptorMutex);
	for (int i=0; i<128; i++)
	{
		if (m_socks[i]==0)
		{
			m_socks[i]=sock;

			if (sock->type==eUST_UDP&& m_bStartKeepAlive==False)
			{
				m_bStartKeepAlive=True;
				SetTimer(eRTID_KEEP_ALIVE, 10000, FALSE, FALSE);
			}
			remote->index=i;
			OnAddUser(m_socks[i], i, remote);
			NotifyMessage(eNRMSG_ADD_USER, (void*)i);
			yiMutexUnlock(m_hDescriptorMutex);
			return i;
		}
	}

	yiMutexUnlock(m_hDescriptorMutex);
	return -1;
}

// ---------------------------
void cyPrivateNetwork::DestroySocket(socket_t sock)
{
	if (sock==0)
		return ;

	if (sock->hEvent)
		CloseHandle(sock->hEvent);

	if (sock->sock)
		closesocket(sock->sock);

	if (sock->hMutex)
		yiMutexDestroy(sock->hMutex);
	
	if (sock->pExtraData)
		yifree(sock->pExtraData);

	if (sock->wbuffer)
		yifree(sock->wbuffer);

	yifree(sock);
}

// ---------------------------
void cyPrivateNetwork::RemoveDescriptor(socket_t sock, int index)
{
	yiMutexLock(m_hDescriptorMutex);
	if (index!=-1)
	{
		//NotifyMessage(eNRMSG_REMOVE_USER, (void*)index, True);
		//OnRemoveUser(m_socks[index], index);
		//DestroySocket(m_socks[index]);

		m_socks[index]=0;
		yiMutexUnlock(m_hDescriptorMutex);
		return ;
	}

	for (int i=0; i<128; i++)
	{
		if (m_socks[i]==sock)
		{
			//NotifyMessage(eNRMSG_REMOVE_USER, (void*)i, True);
			//OnRemoveUser(m_socks[i], i);
			//DestroySocket(m_socks[i]);

			m_socks[i]=0;
			yiMutexUnlock(m_hDescriptorMutex);
			return ;
		}
	}

	yiMutexUnlock(m_hDescriptorMutex);
	return ;
}

// ---------------------------
Bool cyPrivateNetwork::OnNotifyFromServer(socket_t sock, int id, void* data, int size)
{
	sGeneralParam* param = (sGeneralParam*)data;
	switch(id)
	{
		case eUSPID_REQ_CONNECT_UDP:
		{
			if (param->param[1]==eCM_USE_STUNE)
			{
				sGeneralParam buffer;
				buffer.param[0]=param->param[2];
				buffer.param[1]=param->param[3];
				if (uthpSend(sock, eUSPID_REQ_CONNECT_UDP_OK, &buffer, GP_SIZE(2))==False)
				{
					return False;
				}
				return True;
			}
			else if (param->param[1]==eCM_DIRECT)// direct.
			{
				sypnUserInfo info;
				socket_t newsock = ConnectRemote(param->param[2], param->param[3], &info);

				if (newsock==0)
				{
					// ???
					return True;
				}

				if (AddDescriptor(newsock, &info)==-1)
				{
					// ???
					uthpClose(newsock);
					return True;
				}

			}	
			else
			{
				// ????
			}
			break;
		}
		case eUSPID_RPY_CONNECT_UDP:
		{
			xTRACE("eUSPID_RPY_CONNECT_UDP \n");
			if (param->param[0]!=eERCU_OK)
			{
				//???
				xTRACE("eUSPID_RPY_CONNECT_UDP Error! \n");
				return True;
			}

			if (param->param[1]==eCM_USE_STUNE)
			{
				xTRACE("eUSPID_RPY_CONNECT_UDP Holepunching (%d, %d)\n", param->param[3], param->param[4]);
				sypnUserInfo remote;
				socket_t sock = StartHolePunching(m_MainSock, param->param[3], param->param[4], &remote);
				
				if (sock==0)
				{
					return True;
				}
				if (AddDescriptor(sock, &remote)==-1)
				{
					uthpClose(sock);
					//???
					return True;
				}
				return True;
			}
			break;
		}
		default:
		{
			return OnMessage(m_MainSock, -1,  id, data, size);
		}
	}
	
	return True;
}


// ---------------------------
void cyPrivateNetwork::OnWakeUpMsg(int* buffer)
{
//	xTRACE("cyPrivateNetwork::OnWakeUpMsg() called");
	switch(buffer[0])
	{
		case eWUMSG_TIMER:
		{
			//xTRACE("cyPrivateNetwork::OnWakeUpMsg() OnTimer Call");
			OnTimer(buffer[1]);
			break;
		}
		case eWUMSG_ADD_DESCRIPTOR:
		{
			//if (buffer[1]!=0)
			//{
			//	AddDescriptor((socket_t)buffer[1]);
			//}
			break;
		}
		case eWUMSG_REMOVE_DESCRIPTOR:
		{
			RemoveDescriptor((socket_t)buffer[1], -1);
			break;
		}
		case eWUMSG_TERMINATE:
		{
			m_QuitMessage=True;
			break;
		}
		default:
			break;
	}
}

// ---------------------------
void cyPrivateNetwork::SendKeepAlive()
{
	for (int i=0; i<128; i++)
	{
		if (m_socks[i]==0)
		{
			continue;
		}
		
		if (m_socks[i]->type==eUST_UDP)
		{
			xTRACE("cyPrivateNetwork::SendKeepAlive() called");
			yiMutexLock(m_socks[i]->hMutex);

			if (m_socks[i]->kabuff[0]==-2)
			{
				xTRACE("no keep alive.... so, close\n");
				yiMutexUnlock(m_socks[i]->hMutex);
				ConnectionClose(m_socks[i], i);
				continue;
			}

			if (m_socks[i]->kabuff[2]==1)
			{
				m_socks[i]->kabuff[0]=0;
			}
			else
			{
				m_socks[i]->kabuff[0]--;
			}

			m_socks[i]->kabuff[2]=0;

			if (m_socks[i]->kabuff[3]==1)
			{
				xTRACE("cyPrivateNetwork::SendKeepAlive() before sent");
				m_socks[i]->kabuff[3]=0;
				yiMutexUnlock(m_socks[i]->hMutex);
				continue;
			}

			syipacketu packet;

			packet.flag32= 0;
			packet.security=m_socks[i]->security; 
			packet.flag16=ePF16_UDP_KEEP_ALIVE;

			xTRACE("cyPrivateNetwork::SendKeepAlive() Keep Alive Send");
			yiencodex(&packet, sizeof(packet), m_socks[i]->security);
			yisendu(m_socks[i]->sock, &packet, sizeof(packet), m_socks[i]->remote.address, m_socks[i]->remote.port);
			yiMutexUnlock(m_socks[i]->hMutex);
		}
	}	
}

// ---------------------------
void cyPrivateNetwork::OnTimer(int id)
{
	if (id==eRTID_KEEP_ALIVE)
	{
		SendKeepAlive();
	}

	return ;
}

#ifdef WIN32
static unsigned int __stdcall xOnTimerClose(void* p)
#else
static void* xOnTimerClose(void*p)
#endif
{
#ifdef WIN32
	Sleep(5000);
#else
	usleep(5000000);
#endif
	yiclose((int)p);
#ifndef WIN32
	return 0;
#endif
	return 0;
}

// ---------------------------
void cyPrivateNetwork::OnTimerClose(int sock)
{
	yiThreadStartx(xOnTimerClose, (void*)sock, 0);
}

// ---------------------------
socket_t cyPrivateNetwork::OnAcceptClient(socket_t sock, sypnUserInfo* remote)
{
	int newsock = yiaccept(sock->sock, 0, 0, 0);
	//char recv_buffer[sizeof(sypnUserInfo)+sizeof(sNetworkInfo)];
	//char send_buffer[sizeof(sypnUserInfo)+sizeof(sNetworkInfo)];
	//sypnUserInfo *remoteinfo = (sypnUserInfo *)recv_buffer;
	//sypnUserInfo *myinfo = (sypnUserInfo *)send_buffer;;
	
	//sypnUserInfo remote;
	syipacket packet;

	yisockblk(newsock, True);

	if (newsock<0)
	{
		return 0;
	}

	xTRACE("cyPrivateNetwork::OnAcceptClient() tcp connection\n");
	if (yirecvx(newsock, &packet, remote, YPN_DEFAULT_SECURITY, 500)!=sizeof(remote))
	{
		OnTimerClose(newsock);
		//::MessageBoxA(GetActiveWindow(), "recv timeout!!", "error", MB_OK);
		//yiclose(newsock);
		return 0;
	}



	//memcpy(send_buffer, &m_myinfo, sizeof(m_myinfo));
	//memcpy(&send_buffer[sizeof(sypnUserInfo)], &m_myinfo.network, sizeof(sNetworkInfo));

	if (yisendx(newsock, 0, YPN_DEFAULT_SECURITY, YPN_DEFAULT_SECURITY, &m_myinfo, sizeof(m_myinfo), False)!=sizeof(m_myinfo))
	{
		OnTimerClose(newsock);
		//::MessageBoxA(GetActiveWindow(), "send error!!", "error", MB_OK);
		//yiclose(newsock);
		return 0;
	}
		
	socket_t newsockx = uthpSock(newsock, eUST_TCP, m_myinfo.network.security, &remote->network, yitoheap(remote, sizeof(remote)));


	yiMutexLock(sock->hMutex);
	if (newsockx->remote.index==sock->w_id)	
	{
		sock->w_id=0;	
		sock->wbuffer=(void*)newsock;
		sock->pExtraData=yitoheap(remote, sizeof(*remote));
		yiMutexUnlock(sock->hMutex);
		SetEvent(sock->hEvent);
		return 0;
	}
	yiMutexUnlock(sock->hMutex);

	return newsockx;
}

// ---------------------------
Bool cyPrivateNetwork::OnPreMessage(socket_t sock, syipacketu* packet, void* buffer)
{
	int size = __UF32_SIZE__(packet->flag32);
	int id =__UF32_ID__(packet->flag32);
	int check_sum = __UF32_CHECKSUM__(packet->flag32);

	//xTRACE("cyPrivateNetwork::OnPreMessage() called");
	if (packet->flag16&ePF16_UDP_REPLY)
	{
		xTRACE("recv REPLY");
		sock->kabuff[2]=1;
		yiMutexLock(sock->hMutex);
		if (id!=0 && sock->w_id==id)
		{
			sock->wbuffer=0;
			sock->wsize=0;
			sock->w_id=0;
			xTRACE("set event %d", id);
			SetEvent(sock->hEvent);
		}
		yiMutexUnlock(sock->hMutex);
		return False;
	}

	if (packet->flag16&ePF16_UDP_KEEP_ALIVE)
	{
		xTRACE("recv KeepAlive\n");
		sock->kabuff[0]=1;
		return False;
	}

	if (packet->flag16&ePF16_REQ_UDP_CONFIRM)
	{
		syipacketu r;
		memset(&r, 0, sizeof(r));
		r.flag16=ePF16_UDP_REPLY;

		r.flag32=0;
		__UF32_SET_ID__(r.flag32, id);
		r.security=sock->security;

		xTRACE("OnPreMessage() reply confirm msg");
		yiencodex(&r, sizeof(r), r.security);
		yiMutexLock(sock->hMutex);
		yisendu(sock->sock, &r, sizeof(r), sock->remote.address, sock->remote.port);
		yiMutexUnlock(sock->hMutex);

		sock->kabuff[3]=1;
	}

	if (id!=0 && sock->w_id==id)
	{
		//sock->wbuffer=0;
		//sock->wsize=0;
		sock->w_id=0;
		if (sock->wbuffer)
			yifree(sock->wbuffer);

		sock->wbuffer=0;
		sock->wsize=__UF32_SIZE__(packet->flag32);
		if (sock->wsize)
		{
			sock->wbuffer=yitoheap(buffer, sock->wsize);
		}
		xTRACE("wait release %d", id);
		SetEvent(sock->hEvent);
	}

	sock->kabuff[2]=1;
	return True;
}

// ---------------------------
Bool cyPrivateNetwork::OnMessage(socket_t sock, int index, int id, void* buffer, int size)
{
	if (id==eRMSG_CLOSE)
	{
		return False;
	}
	return True;
}


// ---------------------------
void cyPrivateNetwork::OnMsgLoop()
{
	fd_set fds;
	FD_ZERO(&fds);
	while(!m_QuitMessage)
	{
		syipacket packet;
		int max = FdSet(&fds);
		int res = select(max+1, &fds, 0, 0, 0);

		//xTRACE("exit select %d", res);
		if (res<0)
		{
			continue;
		}

		if (FD_ISSET(m_wakeupSock->sock, &fds))
		{
			int buffer[2];
			xTRACE("is wakeup sock %d", res);
			if (yirecvu(m_wakeupSock->sock, buffer, sizeof(buffer), 0, 0, 0)!=sizeof(buffer))
			{
				NotifyMessage(eNRMSG_SYSTEM_ERROR, 0);
				break;
			}
			OnWakeUpMsg(buffer);
			continue;
		}
		else if (FD_ISSET(m_MainSock->sock, &fds))
		{
			char buffer[4096];
			int  size;

			yiMutexLock(m_MainSock->hMutex);
			if ((size=yirecvx(m_MainSock->sock, &packet, buffer, m_MainSock->security, 0))<0)
			{
				yiclose(m_MainSock->sock);
				yiMutexUnlock(m_MainSock->hMutex);
				NotifyMessage(eNRMSG_SERVER_CLOSED, 0);
				continue;
			}
			yiMutexUnlock(m_MainSock->hMutex);

			//xTRACE("from server.........\n");
			if (m_MainSock->w_id==packet.id)
			{
				//xTRACE("wait message found.........\n");
				yiMutexLock(m_MainSock->hMutex);

				if (m_MainSock->wbuffer)
					yifree(m_MainSock->wbuffer);

				m_MainSock->wbuffer=0;
				if (size)
				{
					m_MainSock->wbuffer = yitoheap(buffer, size);
					m_MainSock->wsize=size;
				}
				yiMutexUnlock(m_MainSock->hMutex);
				//xTRACE("wait message event set.........\n");
				SetEvent(m_MainSock->hEvent);
				continue;
			}
			else if (OnNotifyFromServer(m_MainSock, packet.id, buffer, packet.sizex)==False)
			{
				yiclose(m_MainSock->sock);
				NotifyMessage(eNRMSG_SERVER_CLOSED, 0);
				continue;
			}
		}
		else if (m_udpMainSock&&FD_ISSET(m_udpMainSock->sock, &fds))
		{
			address_t addr=0;
			int port=0;
			int timeout=-1;

			// Main socket.
			// 0: addr
			// 1: port
			// 2: from userId;
			sGeneralParam param;

			yirecvu(m_udpMainSock->sock, &param.param[2], 32, &timeout, &addr, &port);
			param.param[0]=addr;
			param.param[1]=port;
			OnNotifyFromMainUdp(param.param[2], &param);
			continue;
		}
		else if (m_AcceptSock&&FD_ISSET(m_AcceptSock->sock, &fds))
		{
			sypnUserInfo remote;
			socket_t newsock = OnAcceptClient(m_AcceptSock, &remote);
			if (newsock==0)
			{
				continue;
			}
			if (AddDescriptor(newsock, &remote)==-1)
			{
				NotifyMessage(eNRMSG_DESCRIPTOR_FULL, yitoheap(newsock->pExtraData, sizeof(sypnUserInfo)));
				uthpClose(newsock);
				continue;
			}
		}
		else if (m_UdpAcceptSock&&FD_ISSET(m_UdpAcceptSock->sock, &fds))
		{
			
		}
		else 
		{
			for (int i=0; i<128; i++)
			{
				if (m_socks[i]==0)
				{
					continue;
				}
				if (FD_ISSET(m_socks[i]->sock, &fds))
				{
					if (m_socks[i]->type==eUST_UDP)
					{
						int timeout=100;
						syipacketu packet;
						char buffer[512];
						int id;
						int nReceived=0;
						void* pLargeBuffer=0;

						if ((nReceived=yirecvxu(m_socks[i]->sock, &packet, buffer, m_socks[i]->security, timeout, 0, 0))<0)
						{
							xTRACE("receive error! closed %d (error:%d)\n", i, WSAGetLastError());
							ConnectionClose(m_socks[i], i);
							continue;
						}

						id = __UF32_ID__(packet.flag32);

						if (packet.flag16&ePF16_UDP_LARGE)
						{
							pLargeBuffer=(void*)nReceived;
						
							// リトライ？
							if (pLargeBuffer==0)
							{
								// 何もしない。?
								continue;
							}
							memcpy(&nReceived, buffer, sizeof(int));
						}


						if (pLargeBuffer==0)
						{
							if (OnPreMessage(m_socks[i], &packet, buffer)==False)
							{
								continue;
							}
						}

						xTRACE("From Message (index:%d, id:%d)", i, id);
						if (OnMessage(m_socks[i], i, id, pLargeBuffer?pLargeBuffer:buffer, nReceived)==False)
						{
							xTRACE("connection closed ... messeage(%d) is false\n", id);
							ConnectionClose(m_socks[i], i);
							HeapFree(GetProcessHeap(), 1, pLargeBuffer);
							continue;
						}
						HeapFree(GetProcessHeap(), 1, pLargeBuffer);
					}
					else // eUST_TCP
					{
						syipacket packet;
						int n;
						char buffer[512];

						if ((n=yirecvx(m_socks[i]->sock, &packet, buffer, m_socks[i]->remote.security, 0))<0)
						{
							uthpClose(m_socks[i]);
							RemoveDescriptor(m_socks[i], i);
							continue;
						}

						if (m_socks[i]->w_id!=0)
						{
							if (n!=0)
							{
								yiMutexLock(m_socks[i]->hMutex);
								m_socks[i]->wbuffer = yitoheap(buffer, n);
								m_socks[i]->wsize = n;
								yiMutexUnlock(m_socks[i]->hMutex);
							}
							SetEvent(m_socks[i]->hEvent);
							continue;
						}
							
						if (OnMessage(m_socks[i], i, packet.id, buffer, n)==False)
						{
							uthpClose(m_socks[i]);
							RemoveDescriptor(m_socks[i], i);
							continue;
						}
					}
				}
			}

		} //else
	}// while

	uthpClose(m_AcceptSock);
	uthpClose(m_UdpAcceptSock);
	for (int i=0; i<128; i++)
	{
		if (m_socks[i]==0)
			continue;
		uthpClose(m_socks[i]);
		m_socks[i]=0;
	}

	if (m_hUpnp)
	{
		yiupnpDeletePortMapx(m_hUpnp, m_myinfo.network.port, True);
		yiupnpDeletePortMapx(m_hUpnp, m_myinfo.network.port, False);
		yiupnpDestroyx(m_hUpnp);
	}

	uthpClose(m_MainSock);

	NotifyMessage(eNRMSG_LOOP_EXIT, 0);
}

// --------------------
void cyPrivateNetwork::ConnectionClose(socket_t sock, int index)
{
	void* param=0;


	//DebugBreak();
	//param = yitoheap(&sock->remote, sizeof(sypnUserInfo));

	if (sock==0)
	{
		sock = m_socks[index];
	}

	if (sock==0 &&
			index==-1)
	{
		return ;
	}

	//if (sock->pExtraData!=0)
	//{
	//	param = yitoheap(sock->pExtraData, sock->nExtraDataSize);//sizeof(sGoUserInfo));
	//}
	RemoveDescriptor(sock, -1);
	NotifyMessage(eNRMSG_CONNECTION_CLOSED, (void*)sock->pExtraData, False);
	//uthpClose(sock);
}
// ------------------------------------
void cyPrivateNetwork::GetEvent(unsigned int event, int*id, void** data)
{
#ifdef __POSIX__
RE_WAIT:
	//ResetEvent(event);
	if (WaitForSingleObject(event, INFINITE)==True)
	{
		int* qdata;
		if (m_NotifyEventQueue->deq((yiAnyValue&)qdata)==True)
		{
			*id=qdata[0];
			*data=(void*)qdata[1];
			yifree(qdata);
			return ;
		}
	}

	goto RE_WAIT;
#endif
}

// ---------------------------
void cyPrivateNetwork::OnNotifyFromMainUdp(int id, void* data)
{
	id|=0x80000000;
	PostMessage(m_hNotifyWnd, m_NotifyMessageId, id, (LPARAM)yitoheap(data, 32));
}

// ---------------------------
void cyPrivateNetwork::NotifyMessage(int id, void* data, Bool bIsDirect)
{
#ifdef WIN32

	if (bIsDirect==True)
	{
		SendMessage(m_hNotifyWnd, m_NotifyMessageId, (WPARAM)id, (LPARAM)data);
	}
	else
	{
		PostMessage(m_hNotifyWnd, m_NotifyMessageId, (WPARAM)id, (LPARAM)data);
	}
#else
	int buffer[2];
	buffer[0]=id;
	buffer[1]=(int)data;	
	m_NotifyEventQueue->lock();
	m_NotifyEventQueue->enq((yiAnyValue)yitoheap(buffer, sizeof(buffer)));
	m_NotifyEventQueue->unlock();
	SetEvent(m_NotifyEvent);
#endif
}

// ---------------------------
void cyPrivateNetwork::NotifyServerMessage(int id, void* data, Bool bIsDirect)
{
#ifdef WIN32

	if (bIsDirect==True)
	{
		SendMessage(m_hNotifyWnd, m_NotifyServerMessageId, (WPARAM)id, (LPARAM)data);
	}
	else
	{
		PostMessage(m_hNotifyWnd, m_NotifyServerMessageId, (WPARAM)id, (LPARAM)data);
	}
#else
	int buffer[2];
	buffer[0]=id;
	buffer[1]=(int)data;	
	m_NotifyEventQueue->lock();
	m_NotifyEventQueue->enq((yiAnyValue)yitoheap(buffer, sizeof(buffer)));
	m_NotifyEventQueue->unlock();
	SetEvent(m_NotifyEvent);
#endif
}

// ------------------------------------
int cyPrivateNetwork::GetServerSock()
{
	if (m_MainSock)
		return m_MainSock->sock;
	return 0;
}

// ------------------------------------
Bool cyPrivateNetwork::SendToServer(int id, void* data, int size, Bool Wait)
{
	//yiMutexLock(m_MainSock->hMutex);
	Bool res = uthpSend(m_MainSock, id, data, size);
	//yiMutexUnlockEx(m_MainSock->hMutex);

	if (res==False)
		return False;

	if (Wait==True)
	{
		WaitForSingleObject(m_MainSock->hEvent, INFINITE);
	}
	return res;
}

// ------------------------------------
void* cyPrivateNetwork::GetFromQueue(socket_t sock, int* id, int* size)
{
	if (sock==0)
		sock=m_MainSock;

	if (id)
		*id=sock->w_id;
	if (size)
		*size=sock->wsize;
	sock->w_id=0;
	return sock->wbuffer;
}


// ------------------------------------
//Bool cyPrivateNetwork::SendToServerAndWait(int id, void* data, int size, int timeout, 
//										   int w_id, void* buffer)
//{
//	m_MainSock->w_id=w_id;
//	if (uthpSend(m_MainSock, id, data, size)==True)
//	{
//		if (uthpWait(m_MainSock, timeout)==True)
//		{
//			int rcvd;
//			uthpRecv(m_MainSock, buffer, &rcvd, 0);
//			m_MainSock->w_id=0;
//			return True;
//		}
//	}
//
//	return False;
//}


// ------------------------------------
BOOL cyPrivateNetwork::SendToUser(socket_t sock, int id, const void* data, int size, Bool bIsConfirm)
{
	if (sock==0)
		return False;

	if (uthpSend(sock, id, data, size, bIsConfirm)==False)
	{
		//DisConnectUser(sock);
		ConnectionClose(sock);
		return False;
	}
	return True;//!=size?False:True;	
}

// ------------------------------------
BOOL cyPrivateNetwork::SendToUser(int index, int id, const void* data, int size, Bool bIsConfirm)
{
	return SendToUser(m_socks[index], id, data, size, bIsConfirm);//!=size?False:True;	
}

// ------------------------------------
//BOOL cyPrivateNetwork::DisConnectUser(socket_t sock)
//{
//	int index=-1;
//	for (int i=0; i<128; i++)
//	{
//		if (m_socks[i]==sock)
//		{
//			index=i;
//			break;
//		}
//	}
//
//	if (index!=-1)
//	{
//		return DisConnectUser(index);
//	}
//	return False;
//}

// ------------------------------------
BOOL cyPrivateNetwork::DisConnectUser(int index)
{
	return Wakeup(eWUMSG_REMOVE_DESCRIPTOR, (int)m_socks[index]);
}
	

// ------------------------------------
int cyPrivateNetwork::GetUserIndex(unsigned int id, const char* pszName, int nRoomNo)
{
	if (id==0 && pszName==0)
	{
		for (int i=0; i<128; i++)
		{
			if (m_socks[i]==0)
			{
				continue;
			}
			if (m_socks[i]->remote.index==nRoomNo)
			{
				return i;
			}
		}
	}

	return -1;
}

// ---------------------------
void cyPrivateNetwork::OnAddUser(socket_t sock, int index, void* param)
{
}

// ---------------------------
void cyPrivateNetwork::OnRemoveUser(socket_t sock, int index)
{
}

// ---------------------------
Bool cyPrivateNetwork::OnLogin(void* buffer, int size)
{
	return True;
}
