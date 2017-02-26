#include "ypn.h"
#include "yistd.h"
#include "yiEncoder.h"
#include "yithread.h"
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
void xOnMsgLoop(void* param)
#else
void* xOnMsgLoop(void* param)
#endif
{
	cyPrivateNetwork* p = (cyPrivateNetwork*)param;

	p->OnMsgLoop();
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

	yiTrace("cyPrivateNetwork::UpnpTest() GetExternalAddress:%s\n", szExternalAddress);

	if (yiupnpAddPortMapx(m_hUpnp, (char*)pszLocalAddress, localport, True, "GoYotsuyaClient")!=True)
	{
		yiupnpDestroyx(m_hUpnp);
		m_hUpnp=0;
		*bReasonIsPort=True;
		yiTrace("cyPrivateNetwork::UpnpTest() PortMap Failure Tcp\n");
		return False;
	}
	if (yiupnpAddPortMapx(m_hUpnp, (char*)pszLocalAddress, localport, False, "GoYotsuyaClientUdp")!=True)
	{
		yiupnpDestroyx(m_hUpnp);
		yiupnpDeletePortMapx(m_hUpnp, localport, True);
		*bReasonIsPort=True;
		m_hUpnp=0;
		yiTrace("cyPrivateNetwork::UpnpTest() PortMap Failure Udp\n");
		return False;
	}

	m_MyNetwork.type    = eNT_UPNP;
	m_MyNetwork.address = yiitol(szExternalAddress);
	m_MyNetwork.port    = localport;
	m_MyNetwork.security = rand();

	return TRUE;
}

// ---------------------------
void cyPrivateNetwork::Init()
{
	m_MainSock = 0;
	m_AcceptSock =0;
	m_UdpAcceptSock=0;
	m_wakeupSock=0;
	m_socks[128];
	memset(&m_MyNetwork, 0, sizeof(m_MyNetwork));
	memset(&m_MyInfo, 0, sizeof(m_MyInfo));
	m_hUpnp=0;
	m_QuitMessage=False;
	m_bStartKeepAlive=False;

	for (int i=0; i<32; i++)
	{
		m_TimerList[32] = 0;
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
	char szLocalAddress[32];

	m_MyNetwork.localaddr 	= address ? address:yigetlocaladdress();
	m_MyNetwork.localport 	= port ? port : YPN_DEFAULT_PORT+((m_MyNetwork.localaddr&0xFF000000)>>24);

RE_TEST:
	if (nRetestCnt==0)
	{
		m_MyNetwork.type==eNT_UNKNOWN;
		m_AcceptSock = 0;
		return ;
	}

	m_MyNetwork.type = UpnpTest(yiltostr(m_MyNetwork.localaddr), m_MyNetwork.localport, &reasonIsPort) ? eNT_UPNP:eNT_UNKNOWN;

	if (reasonIsPort==True)
	{
		m_MyNetwork.localport++;
		nRetestCnt--;
		goto RE_TEST;
	}

	m_AcceptSock=0;
	if (m_MyNetwork.type==eNT_UPNP)
	{
		int sock = yilisten(0, m_MyNetwork.localport, 16);	
		int udpSock;

		if (sock<0)
		{
			yiupnpDeletePortMapx(m_hUpnp, m_MyNetwork.localport, True);
			yiupnpDeletePortMapx(m_hUpnp, m_MyNetwork.localport, False);
			m_MyNetwork.localport++;
			goto RE_TEST;
		}

		if ((udpSock=yiudp(0, m_MyNetwork.localport))<0)
		{
			yiupnpDeletePortMapx(m_hUpnp, m_MyNetwork.localport, True);
			yiupnpDeletePortMapx(m_hUpnp, m_MyNetwork.localport, False);
			m_MyNetwork.localport++;
			yiclose(sock);
			goto RE_TEST;
		}

		m_AcceptSock = uthpSock(sock, eUST_ACCEPT_SOCK, 0, 0, 0);
		m_UdpAcceptSock = uthpSock(udpSock, eUST_ACCEPT_UDP_SOCK, 0, 0, 0);
	}

	if (m_MyNetwork.type==eNT_UPNP)
	{
		yiupnpSleep(m_hUpnp);
		yiTrace("upnp success...%s:%d\n", yiltostr(m_MyNetwork.address), m_MyNetwork.port);
	}
	else
	{
		yiupnpDestroyx(m_hUpnp);
		m_hUpnp=0;
	}
	return ;
}


// ---------------------------
#ifdef WIN32
BOOL cyPrivateNetwork::Start(HWND hWndNotify, unsigned int server, int port, unsigned int security, unsigned int id, const char* pszName, const char* pszPassword, unsigned int localaddr)
#else
BOOL cyPrivateNetwork::Start(unsigned int event, unsigned int server, int port, unsigned int security, unsigned int id, const char* pszName, const char* pszPassword, unsigned int localaddr)
#endif
{
	Bool bIsUpnpSuccess=False;
	int udpSock;
	int local_port;

#ifdef WIN32
	m_hNotifyWnd = hWndNotify;
#else
	m_NotifyEvent = event;
	m_NotifyEventQueue = new cyiQueue(True);
#endif
	if (pszName!=0)
	{
		strcpy(m_MyInfo.szUserName, pszName);
	}

	m_MainSock = uthpTcpConnect(server, port, security);

	if (m_MainSock==0)
	{
		yiTrace("uthpTcpConnect() failure ip:%s, port:%d, security:%x\n", yiltostr(server), port, security);
		return False;
	}

	if ((m_MyInfo.id=uthpTcpLogin(m_MainSock, id, pszName, pszPassword, &m_MyNetwork, m_AcceptSock))<0)
	{
		yiTrace("uthpTcpLogin() login failure\n");
		return FALSE;
	}

	if (m_MyNetwork.type==eNT_UPNP)
	{
		goto SKIP_CHECK_NETWORK;
	}

	udpSock=yiudp(localaddr?localaddr:0, 0);
	if (udpSock<0)
	{
		return FALSE;
	}
	m_NotifyEventQueue=new cyiQueue(True);

	m_MyNetwork.localaddr = localaddr?localaddr:0;
	m_MyNetwork.localport = yigetport(udpSock, 0);

	if (uthpCheckMyNetwork(m_MainSock, udpSock, m_MyNetwork.localaddr, m_MyNetwork.localport, &m_MyNetwork.type, &m_MyNetwork.address, &m_MyNetwork.port)==FALSE)
	{
		yiTrace("uthpCheckMyNetwork() failure\n");
		return FALSE;
	}

	yiclose(udpSock);

	if (m_MyNetwork.type==eNT_NONE ||
		m_MyNetwork.type==eNT_FULL_CON)
	{
		int port=m_MyNetwork.localport;
		int sock; 
		int udpSock;
		int nRetry=3;

RE_CREATE:
		if (nRetry==0)
		{
			return False;
		}

		if ((sock=yilisten(m_MyNetwork.localaddr, port, 16))<0)
		{
			nRetry--;
			goto RE_CREATE;
		}

		port = yigetport(sock, 0);

		if ((udpSock=yiudp(m_MyNetwork.localaddr, port))<0)
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
NOCREATE:
		m_MyNetwork.localport=0;
		m_MyNetwork.port=0;
	}

SKIP_CHECK_NETWORK:
	yiTrace("uthpRegisterMyNetwork() call\n");
	if (uthpRegisterMyNetwork(m_MainSock, m_MyNetwork.type, m_MyNetwork.address, m_MyNetwork.port)==FALSE)
	{
		yiTrace("uthpRegisterMyNetwork() failure\n");
		return FALSE;
	}

	if (pszName!=0)
	{
		m_MyNetwork.security = cyiEncoder::GetYotsuyaSecurityCode(pszName);
	}
	else
	{
		m_MyNetwork.security = YPN_DEFAULT_SECURITY;
	}

	int wakeupSock = yiudp(0, 0);

	{
		syiAddress me = {0, yigetport(wakeupSock, 0)};
		m_wakeupSock = uthpSock(wakeupSock, eUST_WAKEUP_UDP, 0, 0, 0);
	}

	if (m_wakeupSock==0)
	{
		yiTrace("m_wakeupSock failure\n");
		return FALSE;
	}


	m_QuitMessage=False;
	if (yiThreadStartx(xOnMsgLoop, this, 0)==FALSE)
	{
		yiTrace("xOnMsgLoop() failure\n");
		return FALSE;
	}

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
	yisendu((int)m_wakeupSock->pExtraData, buffer, sizeof(buffer), m_wakeupSock->remote.address, m_wakeupSock->remote.port);
	yiMutexUnlock(m_wakeupSock->hMutex);

	return True;
}

#ifdef WIN32
void xOnTimer(void* param)
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
		return ;
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
		return ;
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
socket_t cyPrivateNetwork::ConnectRemote(address_t address, int port)
{
	int sock = yiconnect(address, port, 3);
	if (sock<0)
	{
		yiTrace("yiconnection fialure %s:%d\n", yiltostr(address), port);
		return 0;
	}

	char buffer[sizeof(sNetworkInfo)+sizeof(sypnUserInfo)];
	sypnUserInfo* pInfo=(sypnUserInfo*)buffer;
	sNetworkInfo* pNetwork = (sNetworkInfo*)&pInfo[1];

	memcpy(pInfo, &m_MyInfo, sizeof(m_MyInfo));	
	memcpy(pNetwork, &m_MyNetwork, sizeof(m_MyNetwork));	

	if (yisendx(sock, 0, YPN_DEFAULT_SECURITY, YPN_DEFAULT_SECURITY, buffer, sizeof(buffer), False)!=sizeof(buffer))
	{
		yiclose(sock);
		return 0;
	}

	syipacket packet;
	if (yirecvx(sock, &packet, buffer, YPN_DEFAULT_SECURITY, 500)!=sizeof(buffer))
	{
		yiclose(sock);
		return 0;
	} 

	return uthpSock(sock, eUST_TCP, m_MyNetwork.security, pNetwork, pInfo);
}

// ------------------------------------
Bool cyPrivateNetwork::WaitMessage(socket_t sock, int w_id, int timeout, void* buffer, int*size)
{
	sock->w_id=w_id;

	yiTrace("Event Reset %d\n", sock->event);
	yiEventReset(sock->event);
	if (yiEventWait(sock->event, timeout)==False)
	{
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
	yifree(sock->wbuffer);
	sock->wbuffer=0;
	sock->w_id=0;
	yiMutexUnlock(sock->hMutex);

	return True;
}

// ------------------------------------
socket_t cyPrivateNetwork::StartHolePunching(socket_t sock, int port, int security)
{
	if ((sock=uthpUdpHolePunching(sock, port, security, 5))==0)
	{
		return 0;
	}

	yiTrace("sock:%x\n", sock);

	if (uthpSend(sock, 0, &m_MyInfo, sizeof(m_MyInfo))!=True)
	{
		yiTrace("cyPrivateNetwork::StartHolePunching() send MyInfo error\n");
		uthpClose(sock);
		return 0;
	}

	int id, size;
	sypnUserInfo user_info;
	if (uthpRecv(sock, &id, &user_info, &size, 500)==False)
	{
		yiTrace("cyPrivateNetwork::StartHolePunching() recv remote error\n");
		uthpClose(sock);
		return 0;
	}

	sock->pExtraData = yitoheap(&user_info, sizeof(user_info));

	return sock;
}

// ------------------------------------
socket_t cyPrivateNetwork::WaitAcceptMessage(int index, int timeout)
{
	yiMutexLock(m_AcceptSock->hMutex);
	m_AcceptSock->w_id=index;
	yiEventReset(m_AcceptSock->event);
	yiMutexUnlock(m_AcceptSock->hMutex);
	if (yiEventWait(m_AcceptSock->event, 1000)==False)
	{
		yiMutexLock(m_AcceptSock->hMutex);
		m_AcceptSock->w_id=0;
		yiEventReset(m_AcceptSock->event);
		return 0;
	}
	yiMutexLock(m_AcceptSock->hMutex);
	m_AcceptSock->w_id=0;
	yiEventReset(m_AcceptSock->event);

	return (socket_t)m_AcceptSock->wbuffer;
}

// ------------------------------------
BOOL cyPrivateNetwork::Connect(int Id, const char* pszUserName, int nRoomNo, const char* pszPassword)
{
	if (Id==0 ||
		pszUserName==0)
	{
		sGeneralParam param;
		int size;

		if (uthpSendRCU(m_MainSock, m_MyNetwork.index, nRoomNo)==False)
		{
			yiTrace("uthpSendRCU() False\n");
			return False;
		}

		if (WaitMessage(m_MainSock, eUSPID_RPY_CONNECT_UDP, 500, &param, &size)==False)
		{
			yiTrace("WaitMessage() False\n");
			return False;
		}

		if (param.param[0]!=eERCU_OK)
		{
			yiTrace("eUSPID_RPY_CONNECT_UDP result:%d\n", param.param[0]);
			return False;
		}

		yiTrace("Connect method %d\n", param.param[1]);
		switch(param.param[1])
		{
			case eCM_DIRECT:
			{
				yiTrace("Connect Remote.... Direct\n");
				socket_t sock = ConnectRemote(param.param[2], param.param[3]);

				if (sock==0)
				{
					return False;
				}

				return Wakeup(eWUMSG_ADD_DESCRIPTOR, (int)sock);
			}
			case eCM_WAIT:
			{
				socket_t sock;
				yiTrace("Connect Wait.... \n");
				if ((sock=WaitAcceptMessage(param.param[2], 1000))==False)
				{
					return False;
				}
				return Wakeup(eWUMSG_ADD_DESCRIPTOR, (unsigned int)sock);
			}
			case eCM_USE_STUNE: // udp.
			{
				yiTrace("udp Holepunching.... \n");
				socket_t sock = StartHolePunching(m_MainSock, param.param[3], param.param[4]);

				if (!sock)
				{
					yiTrace("Holepunching.... failure\n");
					return False;
				}

				return Wakeup(eWUMSG_ADD_DESCRIPTOR, (unsigned int)sock);
			}
		}
			
	}
	
	return False;
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
	return &m_MyNetwork;
}

// ---------------------------
const sypnUserInfo* cyPrivateNetwork::GetUserInfo(int index)
{
	if (m_socks[index]==0)
		return 0;
	return (sypnUserInfo* )m_socks[index]->pExtraData;
}

// ---------------------------
Bool cyPrivateNetwork::AddDescriptor(socket_t sock)
{
	for (int i=0; i<128; i++)
	{
		if (m_socks[i]==0)
		{
			m_socks[i]=sock;
			NotifyMessage(eNRMSG_ADD_USER, (void*)i);
			if (sock->type==eUST_UDP&& m_bStartKeepAlive==False)
			{
				m_bStartKeepAlive=True;
				SetTimer(eRTID_KEEP_ALIVE, 10000, FALSE, FALSE);
			}
			return True;
		}
	}

	return False;
}

// ---------------------------
void cyPrivateNetwork::RemoveDescriptor(socket_t sock, int index)
{
	if (index!=-1)
	{
		m_socks[index]=0;
		return ;
	}

	for (int i=0; i<128; i++)
	{
		if (m_socks[i]==sock)
		{
			m_socks[i]=0;
			return ;
		}
	}

	return ;
}

// ---------------------------
Bool cyPrivateNetwork::OnNotifyFromServer(socket_t sock, int id, void* data)
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
				socket_t newsock = ConnectRemote(param->param[2], param->param[3]);

				if (newsock==0)
				{
					// ???
					return True;
				}

				if (AddDescriptor(newsock)==0)
				{
					// ???
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
			if (param->param[0]!=eERCU_OK)
			{
				//???
				return True;
			}

			if (param->param[1]==eCM_USE_STUNE)
			{
				yiTrace("eUSPID_RPY_CONNECT_UDP Holepunching (%d, %d)\n", param->param[3], param->param[4]);
				socket_t sock = StartHolePunching(m_MainSock, param->param[3], param->param[4]);
				
				if (sock==0)
				{
					return True;
				}
				if (AddDescriptor(sock)==False)
				{
					//???
					return True;
				}
				return True;
			}
			break;
		}
	}
	
	return True;
}


// ---------------------------
void cyPrivateNetwork::OnWakeUpMsg(int* buffer)
{
	switch(buffer[0])
	{
		case eWUMSG_TIMER:
		{
			OnTimer(buffer[1]);
			break;
		}
		case eWUMSG_ADD_DESCRIPTOR:
		{
			AddDescriptor((socket_t)buffer[1]);
			break;
		}
		case eWUMSG_REMOVE_DESCRIPTOR: // changed.
		{
			ConnectionClose((socket_t)buffer[1]);
			//RemoveDescriptor((socket_t)buffer[1], -1);
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
			yiTrace("cyPrivateNetwork::SendKeepAlive() called\n");
			yiMutexLock(m_socks[i]->hMutex);

			if (m_socks[i]->kabuff[0]==-3)
			{
				yiTrace("no keep alive.... so, close\n");
				yiMutexUnlock(m_socks[i]->hMutex);
				ConnectionClose(m_socks[i]);
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
				yiTrace("cyPrivateNetwork::SendKeepAlive() before sent\n");
				m_socks[i]->kabuff[3]=0;
				yiMutexUnlock(m_socks[i]->hMutex);
				continue;
			}

			syipacketu packet;

			packet.flag32= 0;
			packet.security=m_socks[i]->security; 
			packet.flag16=ePF16_UDP_KEEP_ALIVE;

			yiTrace("cyPrivateNetwork::SendKeepAlive() Keep Alive Send\n");
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
static void xOnTimerClose(void* p)
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
}

// ---------------------------
void cyPrivateNetwork::OnTimerClose(int sock)
{
	yiThreadStartx(xOnTimerClose, (void*)sock, 0);
}

// ---------------------------
socket_t cyPrivateNetwork::OnAcceptClient(socket_t sock)
{
	int newsock = yiaccept(sock->sock, 0, 0, 0);
	char recv_buffer[sizeof(sypnUserInfo)+sizeof(sNetworkInfo)];
	char send_buffer[sizeof(sypnUserInfo)+sizeof(sNetworkInfo)];
	sypnUserInfo *remoteinfo = (sypnUserInfo *)recv_buffer;
	sypnUserInfo *myinfo = (sypnUserInfo *)send_buffer;;

	syipacket packet;

	yiTrace("cyPrivateNetwork::OnAcceptClient() tcp connection\n");
	if (yirecvx(newsock, &packet, recv_buffer, YPN_DEFAULT_SECURITY, 200)!=sizeof(recv_buffer))
	{
		OnTimerClose(newsock);
		//yiclose(newsock);
		return 0;
	}

	memcpy(send_buffer, &m_MyInfo, sizeof(m_MyInfo));
	memcpy(&send_buffer[sizeof(sypnUserInfo)], &m_MyNetwork, sizeof(sNetworkInfo));

	if (yisendx(newsock, 0, YPN_DEFAULT_SECURITY, YPN_DEFAULT_SECURITY, send_buffer, sizeof(send_buffer), False)!=sizeof(send_buffer))
	{
		OnTimerClose(newsock);
		//yiclose(newsock);
		return 0;
	}
		
	socket_t newsockx = uthpSock(newsock, eUST_TCP, m_MyNetwork.security, (sNetworkInfo*)(&recv_buffer[sizeof(sypnUserInfo)]), yitoheap(recv_buffer, sizeof(sypnUserInfo)));

	yiMutexLock(sock->hMutex);
	if (newsockx->remote.index==sock->w_id)	
	{
		sock->w_id=0;	
		sock->wbuffer=(void*)newsock;	
		yiMutexUnlock(sock->hMutex);
		yiEventSet(sock->event);
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

	if (packet->flag16&ePF16_UDP_REPLY)
	{
		//yiTrace("recv REPLY\n");
		sock->kabuff[2]=1;
		yiMutexLock(sock->hMutex);
		if (id!=0 && sock->w_id==id)
		{
			sock->wbuffer=0;
			sock->wsize=0;
			sock->w_id=0;
			//yiTrace("set event %d\n", sock->event);
			yiEventSet(sock->event);
		}
		yiMutexUnlock(sock->hMutex);
		return False;
	}

	if (packet->flag16&ePF16_UDP_KEEP_ALIVE)
	{
		yiTrace("recv KeepAlive\n");
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
		yiencodex(&r, sizeof(r), r.security);
		yisendu(sock->sock, &r, sizeof(r), sock->remote.address, sock->remote.port);
		sock->kabuff[3]=1;
	}

	sock->kabuff[2]=1;
	return True;
}

// ---------------------------
Bool cyPrivateNetwork::OnMessage(socket_t sock, int id, void* buffer)
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

		if (res<0)
		{
			continue;
		}

		if (FD_ISSET(m_wakeupSock->sock, &fds))
		{
			int buffer[2];
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

			if ((size=yirecvx(m_MainSock->sock, &packet, buffer, m_MainSock->security, 0))<0)
			{
				yiclose(m_MainSock->sock);
				NotifyMessage(eNRMSG_SERVER_CLOSED, 0);
				break;
			}

			//yiTrace("from server.........\n");
			if (m_MainSock->w_id==packet.id)
			{
				//yiTrace("wait message found.........\n");
				yiMutexLock(m_MainSock->hMutex);
				m_MainSock->wbuffer=0;
				if (size)
				{
					m_MainSock->wbuffer = yitoheap(buffer, size);
					m_MainSock->wsize=size;
				}
				yiMutexUnlock(m_MainSock->hMutex);
				//yiTrace("wait message event set.........\n");
				yiEventSet(m_MainSock->event);
				continue;
			}
			else if (OnNotifyFromServer(m_MainSock, packet.id, buffer)==False)
			{
				yiclose(m_MainSock->sock);
				NotifyMessage(eNRMSG_SERVER_CLOSED, 0);
				break;
			}
		}
		else if (FD_ISSET(m_AcceptSock->sock, &fds))
		{
			socket_t newsock = OnAcceptClient(m_AcceptSock);
			if (newsock==0)
			{
				continue;
			}
			if (AddDescriptor(newsock)==False)
			{
				NotifyMessage(eNRMSG_DESCRIPTOR_FULL, yitoheap(newsock->pExtraData, sizeof(sypnUserInfo)));
				uthpClose(newsock);
				continue;
			}
		}
		else if (FD_ISSET(m_UdpAcceptSock->sock, &fds))
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

						if (yirecvxu(m_socks[i]->sock, &packet, buffer, m_socks[i]->security, timeout, 0, 0)<0)
						{
							yiTrace("closed %d\n", i);
							ConnectionClose(m_socks[i]);
							continue;
						}

						id = __UF32_ID__(packet.flag32);

						if (OnPreMessage(m_socks[i], &packet, buffer)==False)
						{
							continue;
						}
						if (OnMessage(m_socks[i], id, buffer)==False)
						{
							ConnectionClose(m_socks[i]);
							continue;
						}
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
							yiEventSet(m_socks[i]->event);
							continue;
						}
							
						if (OnMessage(m_socks[i], packet.id, buffer)==False)
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
	}

	if (m_hUpnp)
	{
		yiupnpDeletePortMapx(m_hUpnp, m_MyNetwork.port, True);
		yiupnpDeletePortMapx(m_hUpnp, m_MyNetwork.port, False);
		yiupnpDestroyx(m_hUpnp);
	}

	uthpClose(m_MainSock);

	NotifyMessage(eNRMSG_LOOP_EXIT, 0);
}

// --------------------
void cyPrivateNetwork::ConnectionClose(socket_t sock)
{
	void* param=0;
	if (sock->pExtraData!=0)
	{
		param = yitoheap(sock->pExtraData, sizeof(sypnUserInfo));
	}
	NotifyMessage(eNRMSG_CONNECTION_CLOSED, param);
	RemoveDescriptor(sock, 0);
	uthpClose(sock);
}
// ------------------------------------
void cyPrivateNetwork::GetEvent(unsigned int event, int*id, void** data)
{
RE_WAIT:
	//yiEventReset(event);
	if (yiEventWait(event, INFINITE)==True)
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
}

// ---------------------------
void cyPrivateNetwork::NotifyMessage(int id, void* data)
{
#ifdef WIN32
	PostMessage(m_hNotifyWnd, WM_FROM_YOTSUYA_PRIVATE_NETWORK, (WPARAM)id, (LPARAM)data);
#else
	int buffer[2];
	buffer[0]=id;
	buffer[1]=(int)data;	
	m_NotifyEventQueue->lock();
	m_NotifyEventQueue->enq((yiAnyValue)yitoheap(buffer, sizeof(buffer)));
	m_NotifyEventQueue->unlock();
	yiEventSet(m_NotifyEvent);
#endif
}


// ------------------------------------
BOOL cyPrivateNetwork::SendToUser(int index, int id, char* data, int size)
{
	if (m_socks[index]==0)
	{
		return False;
	}

	return uthpSend(m_socks[index], id, data, size);	
}

// ------------------------------------
BOOL cyPrivateNetwork::DisConnectUser(int index)
{
	return Wakeup(eWUMSG_REMOVE_DESCRIPTOR, (int)m_socks[index]);
}
	

