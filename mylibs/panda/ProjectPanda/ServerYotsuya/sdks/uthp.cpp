// uthp.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "uthp.h"
#include "svry/svrydefs.h"
#include "yistd.h"

#define UDP_REPLY_WAIT_TIMEOUT 500

// ------------------------------------
BOOL uthpSend(socket_t sock, int id, void* buffer, int size)
{
	BOOL res;

	yiMutexLock(sock->hMutex);

	if (sock->type==eUST_NORMAL ||
		sock->type==eUST_TCP)
	{
		res = yisendx(sock->sock, id, sock->remote.security, sock->security, buffer, size, FALSE)!=size ? FALSE:TRUE;
		yiMutexUnlock(sock->hMutex);
	}
	else if (sock->type==eUST_UDP)
	{
		res = yisendxu(sock->sock, id, sock->security, sock->remote.security, buffer, size, sock->remote.address, sock->remote.port, TRUE, sock->seq++)!=size?FALSE:TRUE;


		//yiTrace("uthpSend size:%d, sock:%x\n", size, sock);
		if (res==False)
		{
			return False;
		}

		sock->kabuff[3]=(char)1;

		if (id!=0)
		{
			sock->w_id=id;
			//yiTrace("reset event %d.......\n", sock->event);
			yiEventReset(sock->event);
			yiMutexUnlock(sock->hMutex);

			//yiTrace("wait event %d.......\n", sock->event);
			if (yiEventWait(sock->event, UDP_REPLY_WAIT_TIMEOUT)==False)
			{
				//yiTrace("Send recv failure.......\n");
				sock->w_id=0;
				return False;
			}
			sock->w_id=0;
			return res;
		}
		yiMutexUnlock(sock->hMutex);
		return res;
	}
	//yiMutexUnlock(sock->hMutex);

	return res;
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
			yiTrace("yirecvxu() error %d\n", n);
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
		memcpy(&xsock->remote, remote, sizeof(sNetworkInfo));

		xsock->pExtraData=p;
		xsock->event = yiEventInit();//CreateEvent(0, FALSE, TRUE, 0);
		xsock->hMutex = yiMutexCreate(0);

		return xsock;
	}
	else if (type==eUST_WAKEUP_UDP)
	{
		xsock = (socket_t)yialloci(sizeof(*xsock));

			
		if (sock==0)
		{
			sock = yiudp(0, 0);
			if (sock<0)
			{
				return 0;
			}
		}
		xsock->sock = sock;
		xsock->type = type;
		xsock->remote.address=0;
		xsock->remote.port=yigetport(xsock->sock, &xsock->remote.address);
		xsock->hMutex = yiMutexCreate(0);
		xsock->pExtraData = (void*)yiudp(0, 0);
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
		xsock->event = yiEventInit();//CreateEvent(0, FALSE, TRUE, 0);
		xsock->hMutex = yiMutexCreate(0);
		xsock->pExtraData=p;
	}

	return xsock;
}

// ------------------------------------
socket_t uthpTcpConnect(address_t server, int port, unsigned long security)
{
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
	xsock->event = yiEventInit();
	xsock->hMutex = yiMutexCreate(0);

	return xsock;
}

// ------------------------------------
// added.
Bool uthpUdpConnect(socket_t sock)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(sock->remote.port);
	addr.sin_addr.s_addr = sock->remote.address;

	if (connect(sock->sock, (struct sockaddr *)&addr, sizeof(addr))!=0)
	{
		//sock->bIsConnect=False;
		return False;
	}

	//sock->bIsConnect=True;
	return True;
}

// ------------------------------------
void uthpClose(socket_t sock)
{
	if (sock==0)
	{
		return ;
	}

	if (sock->event)
	{
		yiEventDelete(sock->event);
	}
	if (sock->hMutex)
	{
		yiMutexDestroy(sock->hMutex);
	}

	yiclose(sock->sock);
	yifree(sock);
}

// ------------------------------------
int uthpTcpLogin(socket_t sock, unsigned long id, const char* pszName, const char* pszPassword, sNetworkInfo* network, socket_t sockAccept)
{
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
		data.szPassword[0]=0;
		memcpy(&data.network, network, sizeof(*network));

		if (uthpSend(sock, eUSPID_NO_ID_LOGIN, &data, sizeof(data))==False)
		{
			return -1;
		}

		res = yirecvx(sock->sock, &packet, &r, sock->remote.security, 300);

		/*if (sockAccept!=0)
		{
			int timeout=300;
			yiaccept(sockAccept->sock, 0, 0, &timeout);
		}*/

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

		return r.id;
	}

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
BOOL uthpSendRCU(socket_t sock, int myIndex, int remoteIndex)
{
	sUthpConnectData data = {myIndex, remoteIndex};
	return uthpSend(sock, eUSPID_REQ_CONNECT_UDP, &data, sizeof(data));
}


// ------------------------------------
socket_t uthpUdpHolePunching(socket_t serverSock, int server_udpport, int nRandNo, int nRetry)
{
	sUthpTestResult utr;
	int timeout=500;
	int udpSock = yiudp(0, 0);
	address_t addr=0;
	int port=0;
	address_t remote_addr=0;
	int remote_port=0;
	Bool bIsHollpunchingSuccess=False;

	if (udpSock==0)
	{
		return 0;
	}

	if (yisendu(udpSock, &nRandNo, sizeof(int), serverSock->remote.address, server_udpport)!=sizeof(int))
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
			yiTrace("uthpUdpHolePunching() recv error\n");
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

	if (remote_addr==utr.remote.address &&
		remote_port==utr.remote.port)
	{
		char d=0;
		yisendu(udpSock, &d, 1, utr.remote.address, utr.remote.port);	
		d=1;
		yisendu(udpSock, &d, 1, utr.remote.address, utr.remote.port);
		goto SKIP_HOLEPUNCHING;
	}

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
				goto SKIP_RECIEVE;
			}

			bIsHollpunchingSuccess=True;
			d=1;
			yisendu(udpSock, &d, 1, utr.remote.address, utr.remote.port);

			break;
		}

	}

	if (bIsHollpunchingSuccess==False)
	{
		yiTrace("uthpUdpHolePunching() punching failure\n");
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
		yiTrace("uthpUdpHolePunching() success\n");
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
	int temp;

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

