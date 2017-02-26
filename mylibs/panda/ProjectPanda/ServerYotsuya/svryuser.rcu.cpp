#include "yi.h"
#include "svryuser.h"
#include <stdlib.h>

// --------------------------
static void xAnalizeConnection(eNatType type1, eNatType type2, eConnectMethod* method1, eConnectMethod* method2)
{

	Bool bIsType1=True;
	int type=type1;
	eConnectMethod method;


CHECK:
	if (type==eNT_UPNP ||
			type==eNT_NONE)
	{
		method=eCM_DIRECT;
	}
	else if (type==eNT_FULL_CON||
			type==eNT_LIMITED ||
				type==eNT_LIMITED_PORT)
	{
		method=eCM_USE_STUNE;
	}
	else if (type==eNT_SYMMETRIC)
	{
		method=eCM_TCP_PROXY;
	}
	else // Unknown
	{
		method=eCM_REGISTER_NETWORK;
	}

	if (bIsType1==True)
	{
		bIsType1=False;
		type=type2;
		*method1=method;
		goto CHECK;
	}

	*method2=method;

	return;

}

// --------------------------
Bool csvryuser::OnReqConnectionUdp(int sock, unsigned int remoteId, HandleUserFile hMyFile)//, const char* pszName, void** pExtraDataRef)
{

	HandleUserFile hRemoteFile = GetUserExtraData(remoteId, 0);

	if (hRemoteFile==0)
	{
		YIDEBUG("invalid remote user %x\n", remoteId);
		SendError(sock, eUSPID_REQ_CONNECT_UDP_BY_ID, eGE_INVALID_USER);
		ReleaseUserExtraData(hRemoteFile);
		return True;
	}

	syiLoginUserInfo* remote = LOGIN_INFO(hRemoteFile);//GetUserLoginInfo(id, pszName);

	if (remote==0 ||
		remote->isLogined==False)
	{
		YIDEBUG("now ready.... remote user %x\n", remoteId);
		SendError(sock, eUSPID_REQ_CONNECT_UDP_BY_ID, eGE_REMOTE_BUSY);
		ReleaseUserExtraData(hRemoteFile);
		return True;
	}

	syiLoginUserInfo* me = LOGIN_INFO(hMyFile);//(syiLoginUserInfo*)*pExtraDataRef;

	ReleaseUserExtraData(hRemoteFile);
	YIDEBUG("csvryuser::OnReqConnectionUdp() myId:%x, index:%d, remoteId:%x index is %d\n", me->id, me->network.index, remoteId, remote->network.index);
	return OnReqConnectionUdp(sock, me->network.index, remote->network.index, hMyFile);	
}

// --------------------------
Bool csvryuser::OnReqConnectionMe(int sock, unsigned int remoteId, HandleUserFile hFile)
{
	YIDEBUG("csvryuser::OnReqConnectionMe() called\n");
	HandleUserFile hRemoteFile = GetUserExtraData(remoteId, 0);
	syiLoginUserInfo2* myInfo = LOGIN_INFO2(hFile);

	if (hRemoteFile==0)
	{
		YIERROR("csvryuser::OnReqConnectionMe() invalid remote id:%x\n", remoteId);
		return True;
	}

	syiLoginUserInfo2* info = LOGIN_INFO2(hRemoteFile);

	if (info==0)
	{
		YIERROR("csvryuser::OnReqConnectionMe() remote id:%x not logined\n", remoteId);
		return True;
	}

	sGeneralParam param = {myInfo->id, myInfo->absAddr, myInfo->absPort, 0};
	yisendx(info->sock, eUSPID_REQ_CONNECT_UDP_ME, info->security, m_ServerInfo.security, &param, GP_SIZE(3), True);

	return True;
	
}

// --------------------------
Bool csvryuser::OnReqConnectionUdp(int sock, int myIndex, int remote, HandleUserFile hMyData, const char* pszPassword/*=0*/)
{
	syiLoginUserInfo* p;// = (syiLoginUserInfo*)*pExtraDataRef;
	syiLoginUserInfo* pUserInfo;
	//HandleUserFile hMyData = (HandleUserFile)*pExtraDataRef;
	HandleUserFile hRemote = GetUserExtraData(remote);

	if (hRemote==0)
	{
		SendError(sock, eUSPID_RPY_CONNECT_UDP, eERCU_INVALID_REMOTE_INDEX);
		return True;
	}

	p=LOGIN_INFO(hMyData);
	pUserInfo=LOGIN_INFO(hRemote);	

	if (p==0 ||
		p->network.index!=myIndex ||
			myIndex==remote)
	{
		SendError(sock, eUSPID_RPY_CONNECT_UDP, eERCU_INVALID_YOUR_INDEX);
		ReleaseUserExtraData(hRemote);
		return True;
	}

	if (pUserInfo==0 ||
		pUserInfo->network.index!=remote)
	{
		SendError(sock, eUSPID_RPY_CONNECT_UDP, eERCU_INVALID_REMOTE_INDEX);
		ReleaseUserExtraData(hRemote);
		return True;
	}

	sGeneralParam param;
	eConnectMethod method1, method2;
	int randNo;

	if (pszPassword)
	{
		if (memcmp(pUserInfo->szPassword, pszPassword, 8)!=0)
		{
			yiTrace("csvryuser::OnReqConnectionUdp() eERCU_INVALID_REMOTE_PASSWORD\n");
			SendError(sock, eUSPID_RPY_CONNECT_UDP, eERCU_INVALID_REMOTE_INDEX);
			ReleaseUserExtraData(hRemote);
			return True;
		}
	}

	xAnalizeConnection(p->network.type, pUserInfo->network.type, &method1, &method2);
	yiTrace("csvryuser::OnReqConnectionUdp() method (%d, %d)\n", method1, method2);

	if (method1==eCM_DIRECT ||
		method2==eCM_DIRECT)
	{
		goto USE_STUNE;
		goto DIRECT_CONNECTION;
	}
	else if (method1==eCM_TCP_PROXY ||
			method2==eCM_TCP_PROXY)
	{
		goto PROXY_CONNECTION;
	}

	if (pUserInfo->network.address==0 ||
		p->network.address==0)
	{
		goto USE_STUNE;
	}

	if (method1==eCM_USE_STUNE && 
		method2==eCM_USE_STUNE )
	{
		goto USE_STUNE;
	}


DIRECT_CONNECTION:
	yiTrace("csvryuser::OnReqConnectionUdp() method directly\n");
	param.param[0]=eERCU_OK;

	if (method2==eCM_DIRECT)
	{
		param.param[1]=eCM_DIRECT;
		param.param[2]=pUserInfo->network.address;
		param.param[3]=pUserInfo->network.port;
		if (yisendx(p->sock, eUSPID_RPY_CONNECT_UDP, p->security, m_ServerInfo.security, &param, GENERAL_PARAM_SIZE(4), True)!=GENERAL_PARAM_SIZE(5))
		{
			ReleaseUserExtraData(hRemote);
			return True;
		}
		ReleaseUserExtraData(hRemote);
		return True;
	}

	//randNo=rand();
	param.param[1]=eCM_DIRECT;
	param.param[2]=p->network.address;
	param.param[3]=p->network.port;
	if (sendx(pUserInfo, eUSPID_REQ_CONNECT_UDP, &param, GENERAL_PARAM_SIZE(4), True)!=GENERAL_PARAM_SIZE(4))
	{
		ReleaseUserExtraData(hRemote);
		return True;
	}

	param.param[1]=eCM_WAIT;
	param.param[2]=remote;
	if (yisendx(p->sock, eUSPID_RPY_CONNECT_UDP, p->security, m_ServerInfo.security, &param, GENERAL_PARAM_SIZE(3), True)!=GENERAL_PARAM_SIZE(3))
	{
		ReleaseUserExtraData(hRemote);
		return True;
	}

	ReleaseUserExtraData(hRemote);
	return True;

USE_STUNE:
{
	yiTrace("csvryuser::OnReqConnectionUdp() method stune\n");
	param.param[0]=eERCU_OK;
	param.param[1]=eCM_USE_STUNE;
	param.param[2]=remote;
	param.param[3]=myIndex;

	//yiTrace("(remote:%d), pUserInfo:%x\n", remote, pUserInfo);
	if (sendx(pUserInfo, eUSPID_REQ_CONNECT_UDP, &param, GP_SIZE(4), True)!=GP_SIZE(4))
	{
		yiTrace("csvryuser::OnReqConnectionUdp() eERCU_INVALID_REMOTE_PASSWORD\n");
		SendError(sock, eUSPID_REQ_CONNECT_UDP, eERCU_INVALID_REMOTE_PASSWORD);
		ReleaseUserExtraData(hRemote);
		return True;
	}
	ReleaseUserExtraData(hRemote);
	return True;
}
PROXY_CONNECTION:
	yiTrace("csvryuser::OnReqConnectionUdp() method proxy\n");
	param.param[0]=eERCU_NO_SUPPORT;

	return False;
}

// --------------------------
#define MAX_DUMMY_PACKET_SIZE 4096

// --------------------------
void* __on_udp_test__(void* param)
{
	//sUthpConnectReply* p = (sUthpConnectReply *)param;
	sGeneralParam* p = (sGeneralParam*)param;
	int sock = p->param[5];
	address_t addr_pare[2] = {0, 0};
	int port_pare[2] = {0, 0};
	sAddress remoteLocal[2]= { {0, 0}, {0, 0}};
	int timeout = 500;
	int cnt=0;

	yiTrace("__on_udp_test__(sock:%d) rand:%d called\n", sock, p->param[4]);
	while(cnt!=2)
	{
		address_t addr=0;
		int port=0;
		sUthpTest test;
		//int randNo;
		if (yirecvu(sock, &test, MAX_DUMMY_PACKET_SIZE, &timeout, &addr, &port)!=sizeof(test))
		{
			if (timeout==0)
			{
				yiTrace("udp time out !!\n");
				close(sock);
				csvryuser::GetInstance()->ReleaseUdpPort(p->param[3]);
				yifree(p);
				return 0;
			}
			continue;
		}
	
		if (test.randNo!=p->param[4])
		{
			yiTrace("invalid random number %d!=%d!!\n", test.randNo, p->param[4]);
			continue;
		}
		
		yiTrace("udp recieve success %s:%d!!\n", yiltostr(addr), port);
		yiTrace("udp recieve success (%s:%d)!!\n", yiltostr(test.local.address), test.local.port);

		addr_pare[cnt]=addr;
		remoteLocal[cnt].address=test.local.address;
		remoteLocal[cnt].port=test.local.port;
		port_pare[cnt++]=port;
	}

	sUthpTestResult result;
	result.me.address = addr_pare[0];
	result.me.port = port_pare[0];
	result.remote.address = addr_pare[1];
	result.remote.port = port_pare[1];
	result.remoteLocal=remoteLocal[1];

	yiTrace("server send to %s:%d\n", yiltostr(addr_pare[0]), port_pare[0]);
	if (yisendu(sock, &result, sizeof(result), addr_pare[0], port_pare[0])!=sizeof(result))
	{
		yiTrace("yisendu error\n");
	}
	//usleep(1000000);

	result.me.address = addr_pare[1];
	result.me.port = port_pare[1];
	result.remote.address = addr_pare[0];
	result.remote.port = port_pare[0];
	result.remoteLocal=remoteLocal[0];

	yiTrace("server send to %s:%d\n", yiltostr(addr_pare[1]), port_pare[1]);
	if (yisendu(sock, &result, sizeof(result), addr_pare[1], port_pare[1])!=sizeof(result))
	{
		yiTrace("yisendu error\n");
	}

	//usleep(1000000);
	close(sock);
	csvryuser::GetInstance()->ReleaseUdpPort(p->param[3]);

	yifree(p);

	yiTrace("udp test quit\n");

	return 0;
}


// --------------------------
Bool csvryuser::OnReqConnectionUdpOk(int sock, int myIndex, int remote, void** pExtraDataRef)
{
	HandleUserFile hMe = (HandleUserFile)*pExtraDataRef;
	HandleUserFile hRemote = GetUserExtraData(remote);

	if (hRemote==0)
	{
		YIERROR("invalid remote index: %d\n", remote);
		SendError(sock, eUSPID_RPY_CONNECT_UDP, -1);
		return True;
	}

	syiLoginUserInfo* p = LOGIN_INFO(hMe);//(syiLoginUserInfo*)*pExtraDataRef;
	syiLoginUserInfo* info = LOGIN_INFO(hRemote);//GetUserLoginInfo(remote);
	int port, udpSock;
	int randNo;
	int sock2=0;
	sGeneralParam param;

	yiTrace("csvryuser::OnReqConnectionUdpOk(%d, %d) called (%x, %x)\n", myIndex, remote, p, info);

	if (p==0 || 
		p->network.index!=myIndex ||
			myIndex==remote)
	{
		param.param[0]=eERCU_INVALID_YOUR_INDEX;
		goto FAILURE;
	}

	if (info==0)
	{
		param.param[0]=eERCU_INVALID_REMOTE_INDEX;
		goto FAILURE;
	}

	sock2=info->sock;
	port = GetFreeUdpPort();
	if (port<0)
	{
		param.param[0]=eERCU_SERVER_IS_BUSY;
		goto FAILURE;
	}

	
	udpSock = yiudp(0, port);
	
	if (udpSock<0)
	{
		param.param[0]=eERCU_SERVER_IS_BUSY;
		goto FAILURE;
	}

	randNo = rand();

	param.param[0]=eERCU_OK;
	param.param[1]=eCM_USE_STUNE;
	param.param[2]=m_UdpTestServerAddress;
	param.param[3]=port;
	param.param[4]=randNo;


	if (sendx(p, eUSPID_RPY_CONNECT_UDP, &param, GP_SIZE(5), False)!=GP_SIZE(5))
	{
		param.param[0]=eERCU_SERVER_IS_BUSY;
		YIDEBUG("send eUSPID_RPY_CONNECT_UDP Error!\n");
		goto FAILURE;
	}
	if (sendx(info, eUSPID_RPY_CONNECT_UDP, &param, GP_SIZE(5), False)!=GP_SIZE(5))
	{
		param.param[0]=eERCU_SERVER_IS_BUSY;
		YIDEBUG("send eUSPID_RPY_CONNECT_UDP Error!\n");	
		goto FAILURE;
	}


	yiTrace("csvryuser::OnReqConnectionUdpOk(%d, %d) success... socket:%d port:%d, randNo:%x\n", myIndex, remote, udpSock, port, randNo);

	param.param[5]=udpSock;
	if (yiThreadStartx(__on_udp_test__, &param, GP_SIZE(6))==0)
	{
		param.param[0]=eERCU_SERVER_IS_BUSY;
		goto FAILURE2;
	}

	ReleaseUserExtraData(hRemote);
	return True;

FAILURE2:
	close(udpSock);

FAILURE:
	sendx(p, eUSPID_RPY_CONNECT_UDP, &param, GENERAL_PARAM_SIZE(1), True);
	if (sock2)
	{
		sendx(info, eUSPID_RPY_CONNECT_UDP, &param, GENERAL_PARAM_SIZE(1), True);
	}

	ReleaseUserExtraData(hRemote);
	return True;
}


// --------------------------
static Bool xReqCheckNatTypeRecv(int sock, int security, unsigned int* addr, int*port, int* timeout)
{
	yiTrace("xReqCheckNatTypeRecv() called\n");
	while(1)
	{
		int recvRand=0;
		unsigned int uaddr=0;
		int uport=0;
		int recved = yirecvu(sock, &recvRand, sizeof(int), timeout, &uaddr, &uport);

		yiTrace("yirecvu: size:%d\n", recved);
		if (*timeout==0)
		{
			yiTrace("xReqCheckNatTypeRecv() timeout\n");
			return False;
		}

		if (recvRand==-1)
		{
			yiTrace("xReqCheckNatTypeRecv() rand is -1\n");
			return False;
		}
		if (recved!=sizeof(int))
		{
			continue;
		}

		if (recvRand!=security)
		{
			continue;
		}


		*addr=uaddr;
		*port=uport;
		break;
	}

	yiTrace("xReqCheckNatTypeRecv() recieve success (%s:%d)\n", yiltostr(*addr), *port);
	return True;
}

// --------------------------
static void* xOnReqCheckMyNetwork(void* p)
{
	sGeneralParam* param = (sGeneralParam*)p;
	int sock1 = param->param[0];
	int sock2 = param->param[1];
	int local_addr = param->param[2];
	int local_port = param->param[3];
	int randomKey = param->param[4];
	int udpPort1=param->param[5];
	int udpPort2=param->param[6];
	unsigned int addr=0, addr2=0;
	int port=0, port2=0;
	int timeout=500;
	eNatType type = eNT_UNKNOWN;

	YIDEBUG("xOnReqCheckMyNetwork(%x) called %s:%d\n", randomKey, yiltostr(local_addr), local_port);
	if (xReqCheckNatTypeRecv(sock1, randomKey, &addr, &port, &timeout)==False)
	{
		yiTrace("xReqCheckNatTypeRecv(%x) failure\n", randomKey);
		goto FAILURE;
	}
	YIDEBUG("xOnReqCheckMyNetwork(%x) sock1 from %s:%d\n", randomKey, yiltostr(addr), port);

	if (xReqCheckNatTypeRecv(sock2, randomKey, &addr2, &port2, &timeout)==False)
	{
		yiTrace("xReqCheckNatTypeRecv(%x) failure\n", randomKey);
		goto FAILURE;
	}
	YIDEBUG("xOnReqCheckMyNetwork(%x) sock2 from %s:%d\n", randomKey, yiltostr(addr2), port2);

	if (port2!=port)
	{
		type = eNT_SYMMETRIC;
		goto SUCCESS;
	}

	if (addr==local_addr &&
		port==local_port)
	{
		type = eNT_NONE;

/*		if (bIsSameServerLayer)
		{
			addr=m_myAddress;
			type=eNT_LIMITED;
			YIDEBUG("xOnReqCheckMyNetwork() same server layer!\n", );
		}*/
		goto SUCCESS;
	}
	else if (port==local_port)
	{
		type = eNT_FULL_CON;
		goto SUCCESS;
	}

	type = eNT_LIMITED_PORT;

SUCCESS:
	param->param[0]=type;
	param->param[1]=addr;
	param->param[2]=port;
	yisendu(sock1, param, GP_SIZE(3), addr, port);

FAILURE:
	close(sock1);
	close(sock2);
	csvryuser::GetInstance()->ReleaseUdpPort(udpPort1);
	csvryuser::GetInstance()->ReleaseUdpPort(udpPort2);

	yifree(p);

	return 0;
}

// --------------------------
Bool csvryuser::OnReqCheckMyNetwork(int sock, unsigned int local_address, int local_port, void** pExtraDataRef)
{
	HandleUserFile hUserFile = (HandleUserFile)*pExtraDataRef;
	syiLoginUserInfo* p = LOGIN_INFO(hUserFile);//(syiLoginUserInfo*)*pExtraDataRef;

	if (p==0)
	{
		YIERROR("csvryuser::OnReqCheckMyNetwork() user is not login!\n");
		return False;
	}

	
	int port1 = GetFreeUdpPort();
	int port2 = GetFreeUdpPort();
	int sock1 = yiudp(0, port1);
	int sock2 = yiudp(0, port2);
	int randomKey;
	sGeneralParam param = {eERCU_SERVER_IS_BUSY, 0, };

	if (p!=0 &&
		p->id!=0)
	{
		void** pLoginFiles = (void**)*pExtraDataRef;
		p = (syiLoginUserInfo* )pLoginFiles[USER_DATA_COUNT];
		p->network.localaddr=local_address;
	}

	if (sock1<0 ||
		sock2<0)
	{
		yisendx(p->sock, eUSPID_RPY_CHECK_MY_NETWORK, p->security, m_ServerInfo.security, &param, sizeof(int), True);
		goto FAILURE2;
	}

	randomKey=rand();
	param.param[0]=eERCU_OK;
	param.param[1]=port1;
	param.param[2]=port2;
	param.param[3]=randomKey;

	yiTrace("OnReqCheckMyNetwork() %d:%d:%x called\n", port1, port2, randomKey);
	if (yisendx(p->sock, eUSPID_RPY_CHECK_MY_NETWORK, 0/*p->security*/, m_ServerInfo.security, &param, GP_SIZE(4), True)!=GP_SIZE(4))
	{
		yiTrace("OnReqCheckMyNetwork() send error\n");
		goto FAILURE;
	}

	param.param[0]=sock1;
	param.param[1]=sock2;
	param.param[2]=local_address;
	param.param[3]=local_port;
	param.param[4]=randomKey;
	param.param[5]=port1;
	param.param[6]=port2;

	if (yiThreadStartx(xOnReqCheckMyNetwork, &param, GP_SIZE(7))==0)
	{
		yiTrace("xOnReqCheckMyNetwork() ThreadStart error\n");
		param.param[0]=eERCU_SERVER_IS_BUSY;
		yisendx(p->sock, eUSPID_RPY_CHECK_MY_NETWORK, p->security, m_ServerInfo.security, &param, sizeof(int), True);
		goto FAILURE;
	}


SUCCESS:
	return True;

FAILURE:
	close(sock1);
	close(sock2);
FAILURE2:
	ReleaseUdpPort(port1);
	ReleaseUdpPort(port2);

	return True;
}

// --------------------------
Bool csvryuser::OnReqRegisterMyNetwork(int sock, eNatType type, unsigned int my_address, int my_port, /*unsigned int local_address, unsigned int local_port, */void** pExtraDataRef)
{
	HandleUserFile hUser = (HandleUserFile)*pExtraDataRef;

	if (hUser==0)
		return False;

	syiLoginUserInfo* p = LOGIN_INFO(hUser);

	if (p==0)
	{
		return False;
	}

	sGeneralParam param = {eERCU_SERVER_IS_BUSY, 0,};
	//int udpPort = 0;
	//int udpSock = 0;
	if (p!=0 &&
		p->id!=0)
	{
		void** pLoginFiles = (void**)*pExtraDataRef;
		p = (syiLoginUserInfo* )pLoginFiles[USER_DATA_COUNT];
	}

	p->network.type = type;
	p->network.address=my_address;
	p->network.port=my_port;

	{
		char* p=(char*)&my_address;
		yiTrace("register net work.... type:%d, address:%d.%d.%d.%d, port:%d\n", type, 0xFF&p[0], 0xFF&p[1], 0xFF&p[2], 0xFF&p[3], my_port);
	}

	p->isLogined=True;
	//p->network.localaddr=local_address;
	//p->network.localport=local_port;

	/*if (type!=eNT_UNKNOWN && 
		type!=eNT_SYMMETRIC)
	{
		int randNo;
		int randomNo;
		unsigned int addr=0;
		int port=0;
		int timeout=300;
		udpPort = GetFreeUdpPort();
		udpSock = yiudp(0, udpPort);

		if (udpSock<0)
		{
			ReleaseUdpPort(udpPort);
			return True;
		}


		param.param[0]=eERCU_OK;
		param.param[1]=udpPort;
		if (yisendx(sock, eUSPID_RPY_REGISTER_MY_NETWORK, m_security, p->security, &param, GP_SIZE(2), True)!=GP_SIZE(2))
		{
			goto CLOSE_SOCKET;
		}

		while(1)
		{
			addr=0;
			port=0;
			if (yirecvu(udpSock, &randomNo, sizeof(int), &timeout, &addr, &port)!=sizeof(int))
			{
				continue;
			}

			if (timeout==0)
			{
				goto CLOSE_SOCKET;
			}

			if (randomNo==p->security)
			{
				if (yisendu(udpSock, &port, sizeof(int), addr, port)==sizeof(int))
				{
					p->nat_type=type;
					p->port = port;
					yiTrace("Register MyNetwork type:%d, port:%d\n", type, port);
				}
				goto CLOSE_SOCKET;
			}
		}
	}

CLOSE_SOCKET:
	if (udpSock!=0)
	{
		ReleaseUdpPort(udpSock);
		close(udpSock);
	}*/
	return True;
}
