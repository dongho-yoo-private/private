// uthp.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "uthp.h"
#include "svry/svrydefs.h"
#include "yistd.h"

#define UDP_REPLY_WAIT_TIMEOUT 1000

// ------------------------------------
BOOL uthpSend(socket_t sock, int id, const void* buffer, int size, Bool bIsConfirm)
{
	BOOL res;

	yiMutexLock(sock->hMutex);

	if (sock->type==eUST_NORMAL ||
		sock->type==eUST_TCP)
	{
		xTRACE("send(id:%d, security:%d, size:%d", id, sock->remote.security, size);
		res = yisendx(sock->sock, id, sock->remote.security, sock->security, buffer, size, FALSE)!=size ? FALSE:TRUE;
		yiMutexUnlock(sock->hMutex);
	}
	else if (sock->type==eUST_UDP)
	{
		res = yisendxu(sock->sock, id, sock->security, sock->remote.security, buffer, size, sock->remote.address, sock->remote.port, bIsConfirm, sock->seq++)!=size?FALSE:TRUE;


		//yiTrace("uthpSend size:%d, sock:%x\n", size, sock);
		if (res==False)
		{
			yiMutexUnlock(sock->hMutex);
			return False;
		}

		sock->kabuff[3]=(char)1;

		if (bIsConfirm==False)
		{
			yiMutexUnlock(sock->hMutex);
			return True;
		}

		if (id!=0)
		{
			sock->w_id=id;
			//yiTrace("reset event %d.......\n", sock->hEvent);
			ResetEvent(sock->hEvent);
			yiMutexUnlock(sock->hMutex);

			xTRACE("wait event %d.......\n", sock->w_id);
			DWORD res = WaitForSingleObject(sock->hEvent, UDP_REPLY_WAIT_TIMEOUT);
			if (res==WAIT_TIMEOUT)
			{
				xTRACE("Send recv failure....... wait timeout\n");
				sock->w_id=0;
				return False;
			}
			else if (res==WAIT_ABANDONED)
			{
				xTRACE("Send recv failure....... connection closed\n");
				return False;
			}
			sock->w_id=0;
			return True;
		}
		yiMutexUnlock(sock->hMutex);
		return True;
	}
	//yiMutexUnlock(sock->hMutex);

	return res;
}

// ------------------------------------
_EXTERN BOOL uthpWait(socket_t sock, int timeout)
{
	if (sock->type==eUST_TCP)
	{
		int t=timeout;
		int res=0;
		if ((res=yiwaitsock(sock->sock, &t))==0)
		{
			// time out
			return False;
		}
		else if (res<0)
		{
			return False;
		}

		return True;
	}
}


// ------------------------------------
BOOL uthpRecv(socket_t sock, int* id, void* buffer, int* size, int timeout)
{
	if (sock->type==eUST_NORMAL ||
		sock->type==eUST_TCP)
	{
		syipacket packet;

		int n = yirecvx(sock->sock, &packet, buffer, sock->security, timeout);

		if (n<0)
		{
			return False;
		}
		*id = packet.id;
		*size = n;

		return (n!=packet.sizex)?FALSE:TRUE;
	}
	else if (sock->type==eUST_UDP)
	{
		syipacketu packet;

		int n = yirecvxu(sock->sock, &packet, buffer, sock->security, timeout, &sock->remote.address, &sock->remote.port);
		if (n<0)
		{
			yiTrace("yirecvxu() error %%d\n", n);
			return False;
		}
		sock->kabuff[2]=1;
		*id = __UF32_ID__(packet.flag32);
		*size = __UF32_SIZE__(packet.flag32);
	
		return (n!=*size)?FALSE:TRUE;
	}

	return FALSE;
}


// ------------------------------------
socket_t uthpSock(int sock, eUthpSocketType type, int security, sNetworkInfo* remote, void* p)
{
	socket_t xsock=0;
	if (type==eUST_MAIN_UDP)
	{
		xsock = (socket_t)yialloci(sizeof(*xsock));
		xsock->sock = sock;
		xsock->type = type;
		xsock->security = security;
		memcpy(&xsock->remote, remote, sizeof(sNetworkInfo));

		xsock->pExtraData=p;
		xsock->hEvent = CreateEvent(0, FALSE, FALSE, 0);
		xsock->hMutex = yiMutexCreate(0);

		return xsock;
	}
	if (type==eUST_UDP)
	{
		if (sock==0)
		{
			sock = yiudp(0, 0);
			if (sock<0)
			{
				return 0;
			}
		}

		xsock = (socket_t)yialloci(sizeof(*xsock));
		xsock->sock = sock;
		xsock->type = type;
		xsock->security = security;

		if (remote!=0)
		{
			memcpy(&xsock->remote, remote, sizeof(sNetworkInfo));
		}

		xsock->pExtraData=p;
		xsock->hEvent = CreateEvent(0, FALSE, FALSE, 0);
		xsock->hMutex = yiMutexCreate(0);

		return xsock;
	}
	else if (type==eUST_WAKEUP_UDP)
	{
		xsock = (socket_t)yialloci(sizeof(*xsock));

			
		if (sock==0)
		{
			sock = yiudp(yiitol("127.0.0.1"), 0);
			if (sock<0)
			{
				return 0;
			}
		}
		xsock->sock = sock;
		xsock->type = type;
		xsock->remote.address=yiitol("127.0.0.1");
		xsock->remote.port=yigetport(xsock->sock, 0);
		xsock->hMutex = yiMutexCreate(0);
		xsock->pExtraData = (void*)yiudp(0, 0);

		yiTrace("WakeupSock Create %u:%d, %d", xsock->remote.address, xsock->remote.port, xsock->pExtraData);
	}
	else if (type==eUST_ACCEPT_SOCK)
	{
		xsock = (socket_t)yialloci(sizeof(*xsock));
		xsock->sock = sock;
		xsock->type = type;
	}
	else if (type==eUST_ACCEPT_UDP_SOCK)
	{
		xsock = (socket_t)yialloci(sizeof(*xsock));
		xsock->sock = sock;
		xsock->type = type;
	}
	else if (type==eUST_TCP)
	{
		xsock = (socket_t)yialloci(sizeof(*xsock));
		xsock->sock = sock;
		xsock->type = type;
		xsock->security = security;
		memcpy(&xsock->remote, remote, sizeof(sNetworkInfo));
		xsock->hEvent = CreateEvent(0, FALSE, FALSE, 0);
		xsock->hMutex = yiMutexCreate(0);
		xsock->pExtraData=p;
	}

	return xsock;
}

// ------------------------------------
socket_t uthpTcpConnect(address_t server, int port, unsigned long security)
{
	xTRACE("connect:%s:%d", yiltostr(server), port);
	int sock = yiconnect(server, port, 3);
	socket_t xsock=0;

	if (sock==-1)
		return 0;

	xsock = (socket_t)yialloci(sizeof(*xsock));
	xsock->sock = sock;
	xsock->type = eUST_NORMAL;
	xsock->security = security;
	xsock->remote.security = security;
	xsock->remote.address=server;
	xsock->remote.port=port;
	xsock->hEvent = CreateEvent(0, FALSE, FALSE, 0);
	xsock->hMutex = yiMutexCreate(0);

	return xsock;
}

// ------------------------------------
void uthpClose(socket_t sock)
{
	if (sock==0)
	{
		return ;
	}

	if (sock->hEvent)
	{
		CloseHandle(sock->hEvent);
		sock->hEvent=0;
	}

	if (sock->hMutex)
	{
		yiMutexDestroy(sock->hMutex);
		sock->hMutex;
	}

	//if (sock->pExtraData)
	//	yifree(sock->pExtraData);

	if (sock->wbuffer)
		yifree(sock->wbuffer);

	yifree(sock);
}



#include <Mstcpip.h>


// ------------------------------------
int uthpTcpLogin(socket_t sock, unsigned int& id, const char* pszName, 
				 const char* pszPassword, sNetworkInfo* network, unsigned int* udpPort)//, socket_t sockAccept, void* buffer, int* recevdSize)
{
	// idなしログイン
	if (id==0)
	{
		syipacket packet;
		syiLoginData data;
		syiLoginReply r;
		int res;

		data.id=id;
		data.szName[0]=0;
		if (pszName!=0)
		{
			strcpy(data.szName, pszName);
		}
		strncpy(data.szPassword, pszPassword, 8);
		data.szPassword[7]=0;
		memcpy(&data.network, network, sizeof(*network));

		if (uthpSend(sock, eUSPID_NO_ID_LOGIN, &data, sizeof(data))==False)
		{
			return -1;
		}

		res = yirecvx(sock->sock, &packet, &r, sock->remote.security, 300);

		if (res<0)
		{
			return -1;
		}

		if (packet.id!=eUSPID_NO_ID_LOGIN)
		{
			return -1;
		}

		sock->security = r.security;
		network->index=r.index;	

		return 0;
	}
	else
	{
		syipacket packet;
		syiLoginData data;
		syiLoginReply r;
		int res;

		data.id=id;
		data.szName[0]=0;
		if (pszName!=0)
		{
			strncpy(data.szName, pszName, 16);
		}
		strncpy(data.szPassword, pszPassword, 8);
		//data.szPassword[7]=0;
		memcpy(&data.network, network, sizeof(*network));
		if (uthpSend(sock, eUSPID_LOGIN, &data, sizeof(data))==False)
		{
			return -1;
		}


		
		sGeneralParam s={0, };// = (sGeneralParam*)buffer;
		res = yirecvx(sock->sock, &packet, &s, sock->remote.security, 300);

		if (res<=0)
		{
			return -1;
		}
		
		if (s.param[0]!=eGE_OK)
		{
			return (int)s.param[0];
		}

		id = s.param[1];
		network->index=s.param[2];
		network->security=sock->security=s.param[3];
	
		*udpPort=s.param[4];

		//*recevdSize=res;
		//memcpy(buffer, &s, packet.sizex);

		//network->security=s.param[3];

		return 0;//s.param[1];
/*		sock->security = r.security;
		network->index=r.index;	

		*recevdSize=0;

		return r.id;*/
	}
	// TODO.

	// ver 0.2.0
	return 0;
}

// ------------------------------------
BOOL uthpSendRCUOk(socket_t sock, int myIndex, int remoteIndex)
{
	sUthpConnectData data = {myIndex, remoteIndex};
	return uthpSend(sock, eUSPID_REQ_CONNECT_UDP_OK, &data, sizeof(data));
}

// ------------------------------------
BOOL uthpSendRCU(socket_t sock, int myIndex, int remoteIndex, const char* pszPassword)
{
	sUthpConnectData data = {myIndex, remoteIndex};
	memset(data.password, 0, sizeof(data.password));
	strcpy(data.password, pszPassword);
	return uthpSend(sock, eUSPID_REQ_CONNECT_UDP, &data, sizeof(data));
}

// ------------------------------------
BOOL uthpSendRCUx(socket_t sock, unsigned int remoteId)
{
	sGeneralParam param;
	param.param[0]=remoteId;

	return uthpSend(sock, eUSPID_REQ_CONNECT_UDP_BY_ID, &param, GP_SIZE(1));
}

// ------------------------------------
socket_t uthpUdpHolePunching2(int sockUdp, address_t remoteAddr, int port, int nRandNo, int nRetry)
{
	char ch=0;

	// 初めは2回送る。
	if (yisendu(sockUdp, &ch, 1, remoteAddr, port)<1)
		return 0;

	if (yisendu(sockUdp, &ch, 1, remoteAddr, port)<1)
		return 0;

	Bool bSendOk=False;
	int  timeout=500;
	int i=0;
	for (i=0; i<nRetry; i++)
	{
		if (yirecvu(sockUdp, &ch, 1, &timeout, &remoteAddr, &port)<1)
		{
			ch=0;
			if (yisendu(sockUdp, &ch, 1, remoteAddr, port)<0)
				return 0;
			continue;
		}

		// 向こうから1を受け取った。
		// すなわち向こうもこちらのパケットを受け取っている。
		if (ch==1)
		{
			i=11;

			if (bSendOk==True)
				break;
		}

		if (bSendOk==False)
		{
			ch=1;
			if (yisendu(sockUdp, &ch, 1, remoteAddr, port)<1)
			{
				return 0;
			}
			bSendOk=True;
		}
		
	}

	if (bSendOk==False)
	{
		return 0;
	}

	// ここまで来たら接続された。
	
	xTRACE("uthpUdpHolePunching2() success\n");
	sNetworkInfo network={eNT_LIMITED, remoteAddr, port, 0, nRandNo, 0, 0};
	return uthpSock(sockUdp, eUST_UDP, nRandNo, &network, 0);
}


// ------------------------------------
socket_t uthpUdpHolePunching(socket_t serverSock, int server_udpport, int nRandNo, int nRetry/*, address_t localaddr, int local_port*/)
{
	sUthpTest ut;
	sUthpTestResult utr;
	int timeout=500;
	int udpSock = yiudp(0, 0);
	address_t addr=0;
	int port=0;
	address_t remote_addr=0;
	int remote_port=0;
	Bool bIsHollpunchingSuccess=False;
	address_t localaddr;
	int		  local_port;


	if (udpSock==0)
	{
		return 0;
	}

	local_port = yigetport(udpSock, &localaddr);
	ut.randNo=nRandNo;
	ut.local.address=::yigetlocaladdress(0, 0);//0x0100007F;//localaddr;
	ut.local.port=local_port;

	xTRACE("hthpUdpHolePunching(Local %x:%d)", ut.local.address, ut.local.port);
	if (yisendu(udpSock, &ut, sizeof(ut), serverSock->remote.address, server_udpport)!=sizeof(ut))
	{
		return 0;
	}


	while(1)
	{	
		address_t addr=0;
		int port=0;
		int n = yirecvu(udpSock, &utr, sizeof(utr), &timeout, &addr, &port);

		if (n<0 ||
			timeout==0)
		{
			xTRACE("uthpUdpHolePunching() recv error");
			return 0;
		}

		if (addr==serverSock->remote.address &&
				port==server_udpport)
		{
			if (n!=sizeof(utr))
			{
				return 0;
			}
			break;
		}
		else
		{
			remote_addr=addr;			
			remote_port=port;			
		}
	}

	xTRACE("uthpUdpHolePunching() remote:%x, port:%d, remote_local:%x, remote_port:%x", utr.remote.address, utr.remote.port, 
							utr.remoteLocal.address, utr.remoteLocal.port);

	//if (utr.remote.address==)
	//{
	//	utr.remote.address=0x0100007F;
	//}

	if (remote_addr==utr.remote.address &&
		remote_port==utr.remote.port)
	{
		char d=0;
		yisendu(udpSock, &d, 1, utr.remote.address, utr.remote.port);	
		d=1;
		yisendu(udpSock, &d, 1, utr.remote.address, utr.remote.port);
		goto SKIP_HOLEPUNCHING;
	}

	// 同じルータ内から出ているので、ローカルアドレスで通信を行う。
	if (utr.me.address==utr.remote.address)//.remoteLocal.address==localaddr)
	{
		utr.remote.address=utr.remoteLocal.address;
		utr.remote.port=utr.remoteLocal.port;
		xTRACE("address same!! so, connect local address %x:%d", utr.remote.address, utr.remote.port);
	}

	xTRACE("uthpUdpHolePunching() trace........1 retry:%d", nRetry);
	for (int i=0; i<nRetry; i++)
	{
		address_t addr=0;
		int port=0;
		char d=0;

		yisendu(udpSock, &d, 1, utr.remote.address, utr.remote.port);

		timeout=100;
		if (yirecvu(udpSock, &d, 1, &timeout, &addr, &port)==1)
		{
			utr.remote.address=addr;
			utr.remote.port=port;

			if (d==1)
			{
				d=1;
				yisendu(udpSock, &d, 1, utr.remote.address, utr.remote.port);
				xTRACE("uthpUdpHolePunching() trace........2 ");
				goto SKIP_RECIEVE;
			}

			bIsHollpunchingSuccess=True;
			d=1;
			yisendu(udpSock, &d, 1, utr.remote.address, utr.remote.port);
			xTRACE("uthpUdpHolePunching() trace........3 ");

			break;
		}
		xTRACE("recv failure.... retry:%d", i);

	}

	if (bIsHollpunchingSuccess==False)
	{
		xTRACE("uthpUdpHolePunching() punching failure");
		yiclose(udpSock);
		return 0;
	}

SKIP_HOLEPUNCHING:
	timeout=300;
	while(1)
	{
		char d=0;
		if (yirecvu(udpSock, &d, 1, &timeout, &addr, &port)==1)
		{
			if (d!=1)
			{
				continue;
			}
			break;
		}

		yiclose(udpSock);
		return 0;
	}
		
SKIP_RECIEVE:
	{
		xTRACE("uthpUdpHolePunching() success\n");
		sNetworkInfo network={eNT_LIMITED, utr.remote.address, utr.remote.port, 0, nRandNo, 0, 0};
		return uthpSock(udpSock, eUST_UDP, nRandNo, &network, 0);
	}
}

// ------------------------------------
BOOL uthpCheckMyNetwork(socket_t sock, int sockUdp, unsigned int in_address, int in_port, eNatType* type, unsigned int* out_address, int* out_port)
{
	sAddress addr = {in_address, in_port};
	if (uthpSend(sock, eUSPID_REQ_CHECK_MY_NETWORK, &addr, sizeof(addr))==FALSE)
	{
		return FALSE;
	}

	int id;
	sGeneralParam param;
	int size;
	int timeout=200;

	yiTrace("uthpCheckMyNetwork(%s, %d)\n", yiltostr(in_address), in_port);

	while(1)
	{
		if (uthpRecv(sock, &id, &param, &size, timeout)==FALSE)
		{
			yiTrace("uthpCheckMyNetwork(%s, %d) recv timeout\n", yiltostr(in_address), in_port);
			return FALSE;
		}

		if (id!=eUSPID_RPY_CHECK_MY_NETWORK)
		{
			continue;
		}

		break;
	}

	if (param.param[0]!=eERCU_OK)
	{
		yiTrace("uthpCheckMyNetwork(%s, %d) param not eERCU_OK\n", yiltostr(in_address), in_port);
		return FALSE;
	}

	int port1=param.param[1];
	int port2=param.param[2];
	int randomKey=param.param[3];

	yiTrace("Server from .... port1:%d, port2:%d, randomKey:%x\n", port1, port2, randomKey);
	if (yisendu(sockUdp, &randomKey, sizeof(int), sock->remote.address, port1)!=sizeof(int))
	{
		return FALSE;
	}

	if (yisendu(sockUdp, &randomKey, sizeof(int), sock->remote.address, port2)!=sizeof(int))
	{
		return FALSE;
	}
/*
	if (yirecvu(sockUdp, &temp, sizeof(int), &timeout, &sock->remote.address, &port2)!=sizeof(int))
	{
		if (timeout==0)
		{
			int temp=-1;
			yiTrace("yirecvu() port2 timeout\n");
			if (yisendu(sockUdp, &temp, sizeof(int), sock->remote.address, port1)!=sizeof(int))
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		yiTrace("yirecvu() port2 success\n");
		if (yisendu(sockUdp, &randomKey, sizeof(int), sock->remote.address, port2)!=sizeof(int))
		{
			return FALSE;
		}
	}
*/
	sGeneralParam result;
	timeout=500;
	if (yirecvu(sockUdp, &result, GP_SIZE(3), &timeout, 0, 0)!=GP_SIZE(3))
	{
		yiTrace("uthpCheckMyNetwork(%s, %d) recv timeout 2\n", yiltostr(in_address), in_port);
		return FALSE;
	}

	*type = (eNatType)result.param[0];
	*out_address=result.param[1];
	*out_port=result.param[2];

	return TRUE;
}

// ------------------------------------
BOOL uthpRegisterMyNetwork(socket_t sock, eNatType type, unsigned int address, int port)
{
	sGeneralParam param = {(unsigned int)type, address, (unsigned int)port};
	if (uthpSend(sock, eUSPID_REQ_REGISTER_MY_NETWORK, &param, GP_SIZE(3))==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

