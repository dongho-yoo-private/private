#include "svryuser.h"
#include "svryenv.h"
#include "yiEncoder.h"
#include "yiHashFunc.h"
#include "string.h"
#include "svrydefs.h"
#include "yitrace.h"

#include <unistd.h>
#include <stdlib.h>

csvryuser* csvryuser::me = 0;



// --------------------------
csvryuser::csvryuser(const char* servername)
: csvrybase(servername)
, m_UserHash(0)
, m_GroupHash(0)
, m_nUnUsedUser(0)
, m_nLoadedUserCnt(0)
, m_nUsedMemory(0)
, m_nRegisteredEmailCount(0)
, m_listEmail(0)
, m_nRegisterPoint(10)
, m_nInitialFriendCount(30)
, m_bIsTerminate(0)
, m_myAddress (0)
{
	if (me==0)
	{
		me=this;
	}
}

// --------------------------
csvryuser::~csvryuser()
{
}

// --------------------------
csvryuser* csvryuser::GetInstance()
{
	return me;
}

// --------------------------
void csvryuser::GetUserFilePath(char* path, unsigned int id, const char* pszFileName)
{
	if (pszFileName==0)
	{
		sprintf(path, "%s/%03d/%03d/%03d/%d", m_szUserRoot, (id>>24)&0xFF, (id>>16)&0xFF, (id>>8)&0xFF, id&0xFF);
	}
	else
	{
		sprintf(path, "%s/%03d/%03d/%03d/%d/%s", m_szUserRoot, (id>>24)&0xFF, (id>>16)&0xFF, (id>>8)&0xFF, id&0xFF, pszFileName);
	}

}

// --------------------------
Bit32 csvryuser::GetUserId(const char* pszUserName)
{
/*	int randuser1 = yihf_additional(pszUserName, 8);
	int randuser2 = yihf_encoded_additional(pszUserName, 8);
	int randuser3 = yihf_general(pszUserName, 8);
	int n = strlen(pszUserName);
	int cnt=0;	
	cyiList* list = m_UserDataHash.GetList(__8to32__(randuser1, randuser2, randuser3, 0)); 

	for (cyiNode* p = list->GetNodeTopFrom(0); p!=0; p=p->np)
	{
		++cnt;
		if (memcmp(pszUserName, p->ref, n+1)==0)
		{
			m_UserDataHash.ReleaseList(list);
			return __8to32__(randuser1, randuser2, randuser3, cnt);
		}
	}

	m_UserDataHash.ReleaseList(list);*/
	return 0;
}

// --------------------------
Bool csvryuser::OnMessagex(int sock, int id, void* data, int size, void** pExtraDataRef)
{
	Bool res=False;
#ifdef YIDEBUG
	//unsigned int start_time=0;
	YIDEBUG("csvryuser::OnMessage(id=%d), size:%d, %x\n", (int)id, size, data);
	syiSecs sec = yiGetCurrentSec();	
#endif

	HandleUserFile hFile = (void**)*pExtraDataRef;
	syiLoginUserInfo2* loginInfo=0;

	if (hFile==0)
	{
		if (id!=eUSPID_REQ_REGISTER_EMAIL &&
			id!=eUSPID_RELEASE_EMAIL &&
				id!=eUSPID_REQ_RECONIZED_CODE &&
					id!=eUSPID_REQ_REGISTER_USER &&
						id!=eUSPID_REQ_QUERY_USER)
		{
			YIERROR("csvryuser::OnMessagex() UserFile is null .... connection closed or not logined!\n");
			return False;
		}
	}

	if (hFile!=0)
	{
		loginInfo = LOGIN_INFO2(hFile);

		if (loginInfo)
		{
			yiMutexLx(loginInfo->hMutex);
		}
	}
	
	switch(id)
	{
		case eUSPID_LOGIN_OK:
		{
			res = OnLoginOK(sock, hFile);
			break;
		}
		case eUSPID_REQ_FILES:
		{
			res = OnReqFiles(sock, data, size, hFile);
			break;
		}
		case eUSPID_REQ_GO_HEADER:
		{
			unsigned int* seq = (unsigned int*)data;
			res = OnReqGoHeader(sock, *seq, hFile);
			break;
		}
		case eUSPID_REQ_GODATA:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = (Bool)(unsigned long)OnReqGoData(sock, param->param[0], param->param[1], param->param[2], param->param[3], hFile);
			break;
		}
		case eUSPID_UP_GODATA:
		{
			sSimpleGoData* goData = (sSimpleGoData*)data;
			res = OnUploadGoData(sock, goData, size, hFile);
			break;
		}
		case eUSPID_UP_GODATA2:
		{
			sSimpleGoData* goData = (sSimpleGoData*)data;
			res = OnUploadGoData2(sock, goData, size, hFile);
			break;
		}
		case eUSPID_DEL_GODATA:
		{
			res = OnDeleteGoData(sock, (sSimpleGoHeader*)data, hFile);
			break;
		}
		case eUSPID_DEL_GOHEADER:
		{
			res = OnDeleteGoHeader(sock, (sSimpleGoHeader*)data, hFile);
			break;
		}
		case eUSPID_REQ_CONNECT_UDP_ME:
		{
			unsigned int remoteId = *((unsigned int*)data);
			res = OnReqConnectionMe(sock, remoteId, (HandleUserFile)*pExtraDataRef);
			break;
		}
		case eUSPID_REQ_CONNECT_UDP:
		{
			sUthpConnectData* p = (sUthpConnectData*)data;
			res = OnReqConnectionUdp(sock, p->myIndex, p->remoteIndex, (HandleUserFile)*pExtraDataRef, p->password);
			break;
		}
		case eUSPID_REQ_CONNECT_UDP_BY_ID:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = OnReqConnectionUdp(sock, param->param[0], (HandleUserFile)*pExtraDataRef);//, (char*)&param->param[1], pExtraDataRef);
			break;
		}
		case eUSPID_REQ_CONNECT_UDP_OK:
		{
			sUthpConnectData* p = (sUthpConnectData*)data;
			res = OnReqConnectionUdpOk(sock, p->myIndex, p->remoteIndex, pExtraDataRef);
			break;
		}
		case eUSPID_REQ_CHECK_MY_NETWORK:
		{
			sAddress* p = (sAddress*)data;
			res = OnReqCheckMyNetwork(sock, p->address, p->port, pExtraDataRef);
			break;
		}
		case eUSPID_REQ_REGISTER_MY_NETWORK:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = OnReqRegisterMyNetwork(sock, (eNatType)param->param[0], param->param[1], (int)param->param[2], pExtraDataRef);
			break;
		}
		case eUSPID_REQ_REGISTER_EMAIL:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = OnPreRegister(sock, param->param[0], (const char*)&param->param[1]);
			break;
		}
		case eUSPID_RELEASE_EMAIL:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = OnReleaseEmailAddress(sock, (const char*)&param->param[0]);
			break;
		}
		case eUSPID_REQ_RECONIZED_CODE:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = OnReconizedData(sock, param->param[0], param->param[1]);
			break;
		}
		case eUSPID_REQ_QUERY_USER:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = OnQueryUser(sock, param->param[0], (const char*)&param->param[1]);
			break;
		}
		case eUSPID_REQ_REGISTER_USER:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = OnRegisterUser(sock, param->param[0], (sRegisterUserInfo*)&param->param[0]);
			break;
		}
		case eUSPID_REQ_ADD_FRIEND:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = OnReqAddFriend(sock, param->param[0], param->param[1], param->param[2], (const char*)&param->param[3], pExtraDataRef);
			break;
		}
		case eUSPID_GET_MSG32:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = OnGetMsg32(sock, param->param[0], pExtraDataRef);
			break;
		}
		case eUSPID_REQ_DELETE_FRIEND:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = OnReqDeleteFriend(sock, param->param[0], param->param[1], pExtraDataRef);
			break;
		}
		case eUSPID_GET_FRIEND_LIST:
		{
			sGeneralParam* param = (sGeneralParam*)data;
			res = OnGetFriendList(sock, param->param[0], pExtraDataRef);
			break;
		}
		case eUSPID_REQ_DEL_MSG32:
		{
			unsigned int* index = (unsigned int*)data;
			res = OnReqDelMsg32(sock, (HandleUserFile)*pExtraDataRef, *index);
			break;
		}
/*
		case eUSPID_REQ_BUY_POINT:
		{
			res = OnReqBuyPoint(data, (HandleUserFile)*pExtraDataRef);
			break;
		}
		case eUSPID_REQ_BUY_POINT2:
		{
			res = OnReqBuyPoint2(data, (HandleUserFile)*pExtraDataRef);
			break;
		}
		case eUSPID_REQ_BUY_SPETIAL:
		{
			res = OnReqBuyPointSpecial(data, (HandleUserFile)*pExtraDataRef);
			break;
		}
*/
		default:
			break;
	}
	
	if (loginInfo)
	{
		yiMutexUnLx(loginInfo->hMutex);
	} 

#ifdef YIDEBUG
{
	syiSecs sec2 = yiCompareSecs(sec, yiGetCurrentSec());
	YIDEBUG("csvryuser::OnMessage(id=%d) exit, access time: %d.%06d secs\n", id, sec2.sec, sec2.usec);
}
#endif
	return res;
}


// -------------------------------------
Bool csvryuser::OnAccepted(int sock, int index, void** pExtra)
{
	syipacket packet;
	syiLoginData login;
	Bool res = False;

	YIDEBUG("csvryuser::OnAccepted(%d) called\n", index);
#ifdef YIDEBUG
	syiSecs sec = yiGetCurrentSec();	
#endif

	if (yirecvx(sock, &packet, &login, /*m_security*/m_ServerInfo.security, 500)!=sizeof(login))
	{
		if (packet.id!=eUSPID_NO_LOGIN)
		{
			goto RETURN;
		}
	}
	YIDEBUG("csvryuser::OnAccepted(%d) recieved packet (id:%d, size:%d)\n", index, packet.id, (int)packet.sizex);

	if (packet.id==eUSPID_NO_ID_LOGIN)
	{
		res = OnNoIdLogin(sock, index, &login, pExtra);
		goto RETURN;
	}
	else if (packet.id==eUSPID_LOGIN)
	{
		res = OnLogin(sock, index, &login, pExtra);
		goto RETURN;
	}
	else if (packet.id==eUSPID_NO_LOGIN)
	{
		if (packet.sizex==0)
		{
			YIDEBUG("csvryuser::OnAccepted() No login packet, index:%d\n", index);
			*pExtra=0;
			res=True;
			goto RETURN;
		}
	}

RETURN:
#ifdef YIDEBUG
{
	syiSecs sec2 = yiCompareSecs(sec, yiGetCurrentSec());
	YIDEBUG("csvryuser::OnAccepted(index=%d) exit=%s, access time: %d.%06d secs\n", index, (res==True)?"True":"False", sec2.sec, sec2.usec);
}
#endif
	return res;
}

// --------------------------
Bool csvryuser::OnStatus(char* buffer)
{
	sprintf(buffer, "Current :%d, Peek:%d, Mgr:%d TotalUsedMemory:%d bytes\n"
		, m_nConnectionCount
		, m_nPeekConnectionCount
		, m_nConMgrCount
		, m_nUsedMemory);
	return True;
}

// --------------------------
Bool csvryuser::OnMessageLocal(int sock, syipacket& packet, void** pExtraDataRef)
{
	switch(packet.id)
	{	
		/*case eULPID_PREREGISTER:
		{
			if (packet.sizex!=0)
			{
				recv(sock, 
			}
			break;
		}
		case eUSPID_REQ_REGISTER_USER:
		{
			syiUserRegisterData data;

			//YIDEBUG("OnMessageLocal() eUSPID_REQ_REGISTER_USER\n");
			if (packet.sizex!=sizeof(data))
			{
				YIWARN("eUSPID_REQ_REGISTER_USER invalid packet\n");
				return False;
			}
			if (yirecv(sock, &data, sizeof(data), 0)!=sizeof(data))
			{
				YIWARN("eUSPID_REQ_REGISTER_USER recv error\n");
				return False;
			}

			sGeneralParam param = {eGE_OK, };
			Bool IsDuplicateId;

			if (RegisterUser(&data, &IsDuplicateId)==True)
			{
				yisendx(sock, eUSPID_RPY_REGISTER_USER, 0, 0, &param, GP_SIZE(1), True);
				return False;
			}
			else
			{
				param.param[0]=eGE_DUPLICATE_ID;
				if (IsDuplicateId==True)
				{
					yisendx(sock, eUSPID_RPY_REGISTER_USER, 0, 0, &param, GP_SIZE(1), True);
					return False;
				}
				else
				{
					param.param[0]=eGE_SERVER_IS_BUSY;
					yisendx(sock, eUSPID_RPY_REGISTER_USER, 0, 0, &param, GP_SIZE(1), True);
					return False;
				}
			}
			return True;
		}*/
		default:
			break;
	}

	return csvrybase::OnMessageLocal(sock, packet, pExtraDataRef);
}

// -----------------------------------
void csvryuser::OnClientClose(int sock, void** pExtraDataRef)
{
	void** pExtra = (void**)*pExtraDataRef;

	if (pExtra==0)
	{
		YIERROR("csvryuser::OnClientClose() pExtra is null!\n");
		return ;
	}

	syiLoginUserInfo* info = (syiLoginUserInfo*)pExtra[USER_DATA_COUNT];

	if (info==0)
	{
		YIERROR("csvryuser::OnClientClose() Already closed!\n");
		return ;
	}

	YIDEBUG("csvryuser::OnClientClose called id:%x\n", info->id);

	yiMutexLx(info->hMutex);
	pExtra[USER_DATA_COUNT]=0;
	yiMutexUnLx(info->hMutex);
	yiMutexDestroyx(info->hMutex);
	if (info)
		yifree(info);//pExtra[USER_DATA_COUNT]);

	*pExtraDataRef=0;

	// no id login.
	if (pExtra[0]==0)
	{
		yifree(pExtra);
	}

}

// --------------------------
void csvryuser::InitUdpPortTable()
{
	m_UdpTables = (sUdpTables*)yialloc(sizeof(sUdpTables)*m_ServerInfo.extra[0]);
	for (int i=0; i<m_ServerInfo.extra[0]; i++)
	{
		m_UdpTables[i].bIsUsed=False;
		m_UdpTables[i].port=m_ServerInfo.extra[1]+i;
	}

	m_UdpTestServerAddress=m_ServerInfo.extra[2];
	m_hUdpTableMutex = yiMutexCreate(0);
}

// --------------------------
int csvryuser::GetFreeUdpPort()
{
	int i=0;
	yiMutexLock(m_hUdpTableMutex );

	for (i=0; i<m_ServerInfo.extra[0]; i++)
	{
		if (m_UdpTables[i].bIsUsed==True)
			continue;

		break;
	}

	if (i==m_ServerInfo.extra[0])
	{
		return -1;
	}

	m_UdpTables[i].bIsUsed=True;
	
	yiMutexUnlock(m_hUdpTableMutex );

	return m_UdpTables[i].port;
}

// --------------------------
void csvryuser::ReleaseUdpPort(int port)
{
	int i=0;
	yiMutexLock(m_hUdpTableMutex);

	for (i=0; i<m_ServerInfo.extra[0]; i++)
	{
		if (m_UdpTables[i].port==port)
		{
			m_UdpTables[i].bIsUsed=False;
			break;
		}
	}
	yiMutexUnlock(m_hUdpTableMutex);
}

// --------------------------
Bool csvryuser::OnServerStart()
{
	syiSecs sec = yiGetCurrentSec();
	InitUdpPortTable();

	yiLogStart(0, "svryuser");
	//m_GroupHash = new cyiHashTable(2, 8, -1, xOnGroupHashDelete, True);
	//m_EmailHash = new cyiHashTable(2, 8, -1, xOnEmailHashDelete, True);
	strcpy(m_szUserRoot, csvryEnv::GetUserRoot());
	sprintf(m_szCommonDir, "%s/common", csvryEnv::GetUserRoot());

	yiMakeDir(m_szCommonDir, False);

	if (LoadUserData()==False)
	{
		return False;
	}

	if (LoadGroupData()==False)
	{
		return False;
	}

	if (LoadEmailAddress()==False)
	{
		return False;
	}

	m_UserHash->GetCount();
	OpenUdpPort();

	if (m_UserHash)
	{
		int n = m_UserHash->GetCount();
		YIINFO("Total User %d, Total used memory %dKBytes(%d bytes)\n", n, (m_UserHash->GetUsedMemorySize()+ m_nUsedMemory)/1024, (m_UserHash->GetUsedMemorySize()+ m_nUsedMemory));
		syiSecs sec2 = yiCompareSecs(sec, yiGetCurrentSec());
		YIINFO("UserServerYotsuya Started......! access time %d minutes and %d.%06d secs\n", sec2.sec/60, sec2.sec%60, sec2.usec);
	}

		
	return True;
}

void* xOnUdpLoop(void* param)
{
	sThreadParam* p = (sThreadParam*)param;
	csvryuser* pClass = (csvryuser*)p->pClassInstance;

	pClass->OnUdpLoop(p->param[0], p->param[1], p->param[2]);	
		
	_THREAD_END_(param)
	return 0;
}

#include "yiupnp.h"
#define _USE_UPNP_ 1
// --------------------------
Bool csvryuser::OpenUdpPort()
{
#ifdef _USE_UPNP_
	char szExternalAddress[32];
	char path[128];
	char szUpnp[128];
	sprintf(path, "%s/upnps", m_szUserRoot);
	sprintf(szUpnp, "%s/upnpinfo", m_szUserRoot);
	void* p = yiFileLoad(szUpnp, 0);
	HandleUpnp hUpnp = 0;//yiFileLoad(szUpnp, 0);

	char szLocalAddress[32];
	
	yigetlocaladdress(szLocalAddress, 0);

	if (p)
	{
		hUpnp = yiupnpImport(p);
		yifree(p);
	}

	if (hUpnp==0)
	{
		hUpnp = yiupnpCreatex();
		
		if (hUpnp)
		{	
			int size=0;
			const void* x = yiupnpExport(hUpnp, &size);

			if (yiFileWrite(szUpnp, (void*)x, 0, size)==False)
			{
				YIERROR("upnp(%s) write error!\n", szUpnp);
			}
		}
	}

	

	if (hUpnp==0)
	{
		YIERROR("not found upnp router!\n");
		return False;
	}


	if (yiupnpGetExternalAddressx(hUpnp, szExternalAddress)==False)
	{
		YIERROR("GetExternalAddressx failure!\n");
		return False;
	}

	m_myAddress = yiitol(szExternalAddress);	

	int* ports = (int*)yiFileLoadEx(path, 0);

	if (ports)
	{
		for (int i=0; i<10; i++)
		{
			YIDEBUG("ypnpDelete port %d\n", ports[i]);
			yiupnpDeletePortMapx(hUpnp, ports[i], False);
		}
	}

	for (int i=0; i<10; i++)
	{
		address_t addr;
		m_udpSocks[i] = yiudp(0, 0);
		m_udpPorts[i] = yigetport(m_udpSocks[i], &addr);
		yiupnpAddPortMapx(hUpnp, (char*)szLocalAddress, m_udpPorts[i], False, "yotsuya udp!");
		YIDEBUG("ypnpAddPortMap port %d\n", m_udpPorts[i]);
	}


	if (yiFileWriteEx(path, 0, m_udpPorts, sizeof(m_udpPorts), True)==False)
	{
		if (yiFileCreateEx(path, 0, m_udpPorts, sizeof(m_udpPorts), True)==False)
		{
			YIERROR("upnp file create error!\n");
			return False;
		}
	}
#else

	srand(time(0));	
	unsigned int start = rand();
	start=(start%100);

	for (int i=0; i<10; i++)
	{
		address_t addr;
		m_udpSocks[i] = yiudp(0, m_ServerInfo.extra[1]+100+start);
		m_udpPorts[i] = yigetport(m_udpSocks[i], &addr);
		YIDEBUG("open udpport %d\n", m_udpPorts[i]);
	}
#endif

	for (int i=0; i<10; i++)
	{
		sThreadParam s = {this, m_udpSocks[i], 0, 0,};
		yiStartThread(xOnUdpLoop, &s, False);
		//yiThreadStartEx(this, &csvryuser::OnUdpLoop, m_udpSocks[i], 0, 0);
	}

	YIDEBUG("OpenUdpPort() exit\n");
	return True;

}

// --------------------------
void csvryuser::OnUdpLoop(unsigned long param1, unsigned long parm2, unsigned long param3)
{
	int sock = param1;
	char buffer[32];
	unsigned int* param=(unsigned int*)buffer;
	int result=0;
	//int timeout = WAIT_INFINITE;

	//YIDEBUG("start udp loop! \n");
#ifdef YIDEBUG
	syiSecs sec = yiGetCurrentSec();
	syiSecs sec2;
#endif
	while(m_bIsTerminate!=True)
	{
		address_t addr=0;
		int	  port=0;

#ifdef YIDEBUG
{
	sec2 = yiCompareSecs(sec, yiGetCurrentSec());
	YIDEBUG("csvryuser::OnUdpLoop() access time: %d.%06d secs\n", sec2.sec, sec2.usec);
}
#endif
		yirecvu(sock, buffer, 32, WAIT_INFINITE, &addr, &port);

		if (param[0]==0xFFFFFFFF)
		{
			YIDEBUG("exit udp loop\n");
			break;
		}
#ifdef YIDEBUG
	sec = yiGetCurrentSec();	
#endif
		if (param[0]==1) // query user name.
		{
			HandleUserFile hRemoteFile = GetUserExtraData(param[1], 0);
		
			if (hRemoteFile==0)
			{
				result=-1;
				yisendu(sock,&result, sizeof(result), addr, port);
				YIDEBUG("csvryuser::OnUdpLoop() not found user:%x\n", param[1]);
				ReleaseUserExtraData(hRemoteFile);
			}
			else
			{
				result=0;
				sRegisterUserInfo* reg = REGISTER_INFO(hRemoteFile);
				yisendu(sock, &result, sizeof(result), addr, port);
				yisendu(sock, reg->szUserName, 16, addr, port);
				YIDEBUG("csvryuser::OnUdpLoop() found user:%x, %s\n", param[1], reg->szUserName);
				ReleaseUserExtraData(hRemoteFile);
			}
		}
		else if (param[0]==2) // query kihu.
		{
			int size;

			void* p = 0;
		
			if (param[5]==0)
			{	
				p=(void*)OnReqGoData(0, param[1], param[2], param[3], param[4], 0, &size);
			}
			else
			{
				p=(void*)OnReqGoData2(0, param[1], param[2], param[3], param[4], 0, &size);
			}

			if (p==0)
			{
				result=-1;
				yisendu(sock,&result, sizeof(result), addr, port);
				YIDEBUG("csvryuser::OnUdpLoop() not found godata, (%x, %x, %x, %x)\n", param[1], param[2], param[3], param[4]);
			}
			else
			{
				result=size;
				int n = size>>9;
				int remain = size&0x1FF;
				yisendu(sock,&result, sizeof(result), addr, port);

				if (n==0)
				{
					yisendu(sock, p, size, addr, port);
					yifree(p);
					continue;
				}

				int i=0;
				char* pIndex=(char*)p;
				for (i=0; i<n; i++)
				{
					yisendu(sock, &pIndex[i<<9], 512, addr, port);
				}
			
				if (remain)
					yisendu(sock, &pIndex[i<<9], remain, addr, port);

				YIDEBUG("csvryuser::OnUdpLoop() found godata !\n");
			}	
		}
		else if (param[0]==3) // stune.
		{
			unsigned int addrs[2];
			addrs[0]=addr;
			addrs[1]=port;

			YIDEBUG("csvryuser::OnUdpLoop() your address (%d.%d.%d.%d:%d)!\n", (addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xff, addr&0xFF, port);
			yisendu(sock, addrs, sizeof(addrs), addr, port);
		}
		else if (param[0]==4) // register info.
		{
			HandleUserFile hRemoteFile = GetUserExtraData(param[1], 0);
		
			if (hRemoteFile==0)
			{
				result=-1;
				yisendu(sock,&result, sizeof(result), addr, port);
				YIDEBUG("csvryuser::OnUdpLoop() not found user:%x\n", param[1]);
				ReleaseUserExtraData(hRemoteFile);
			}
			else
			{
				result=0;
				sRegisterUserInfo* reg = REGISTER_INFO(hRemoteFile);
				sRegisterUserInfo reg2=*reg;
				memset(reg2.szPassword, 0, 8);

				yisendu(sock, &result, sizeof(result), addr, port);
				yisendu(sock, &reg2, sizeof(reg2), addr, port);
				YIDEBUG("csvryuser::OnUdpLoop() found user:%x, %s\n", param[1], reg->szUserName);
				ReleaseUserExtraData(hRemoteFile);
			}
				
		}
		else if (param[0]==5) // remove go data.
		{
		}
		else if (param[0]==6) // register upnp port
		{
			HandleUserFile hMyFile = (HandleUserFile )param[1];


			YIDEBUG("register upnp port. %x\n", hMyFile);

			syiLoginUserInfo2* info = LOGIN_INFO2(hMyFile);


			if (info)
			{
				info->absAddr = param[2];
				info->absPort = param[3];	
				YIDEBUG("csvryuser::OnUdpLoop() user:%x absIp:%x, absPort:%x\n", info->id, info->absAddr, info->absPort);
			}
		}
		else if (param[0]==7) // get remote abs addr
		{
			HandleUserFile hRemoteFile = GetUserExtraData(param[1], 0);
			if (hRemoteFile==0)
			{
				result=-1;
				yisendu(sock,&result, sizeof(result), addr, port);
				YIDEBUG("csvryuser::OnUdpLoop() not found user:%x\n", param[1]);
				ReleaseUserExtraData(hRemoteFile);
			}
			else
			{
				result=0;
				syiLoginUserInfo2* info = LOGIN_INFO2(hRemoteFile);

				if (info==0)
				{
					result=-2;
					yisendu(sock,&result, sizeof(result), addr, port);
					YIDEBUG("csvryuser::OnUdpLoop() user:%x is not logined \n", param[1]);
				}
				else
				{
					sGeneralParam xxx = {0, info->absAddr, info->absPort, info->network.localaddr};
					YIDEBUG("csvryuser::OnUdpLoop() user:%x abs:%x, local:%x\n", param[1], info->absPort, info->network.localaddr);
					yisendu(sock,&xxx, GP_SIZE(4), addr, port);
				}
				ReleaseUserExtraData(hRemoteFile);
			}
		}
	}

}


// --------------------------
Bool csvryuser::OnServerEnd()
{
	m_bIsTerminate=True;
	int sock = yiudp(0, 0);
	unsigned int data=0xFFFFFFFF;
	for (int i=0; i<10; i++)
	{
		yisendu(sock, &data, sizeof(unsigned int), 0, m_udpPorts[i]);
		yiclose(m_udpSocks[i]);
	}
	yiclose(sock);
	
	yiLogEnd();
	return True;
}

// --------------------------
HandleUserFile csvryuser::GetUserExtraData(unsigned int id, const char* pszUserName)
{
	HandleUserFile h = (HandleUserFile)m_UserHash->GetValue(id, pszUserName);

	if (h==0)
	{
		return 0;
	}

	syiLoginUserInfo* info = LOGIN_INFO(h);//(syiLoginUserInfo*)p[USER_DATA_COUNT];
	
	if (info==0)
	{
		return h;
	}

	yiMutexLx(info->hMutex);
	return h;
}

// --------------------------
HandleUserFile csvryuser::GetUserExtraData(unsigned int index)//, const char* pszUserName)
{
	int h = (index-1)/m_ServerInfo.nMaxConnections;
	int l = (index-1)%m_ServerInfo.nMaxConnections;
	csvryConn* p = m_GlobalConMgr->GetConnectionInfo(h);

	if (p==0)
		return 0;
	
	void** ptr = p->GetExtraDataPtr(l);

	if (ptr==0)
	{
		return 0;
	}

	HandleUserFile hUserFile = (HandleUserFile)*ptr;

	if (hUserFile==0)
	{
		return 0;
	}

	syiLoginUserInfo* info = LOGIN_INFO(hUserFile);//(syiLoginUserInfo*)p[USER_DATA_COUNT];
	
	if (info==0)
	{
		return hUserFile;
	}

	yiMutexLx(info->hMutex);
	return hUserFile;
}

// --------------------------
void csvryuser::ReleaseUserExtraData(HandleUserFile p)
{
	if (p==0)
	{
		YIERROR("csvryuser::ReleaseUserExtraData() p is null!\n");
		return ;
	}

	syiLoginUserInfo* info = (syiLoginUserInfo*)p[USER_DATA_COUNT];
	
	if (info==0)
	{
		return;
	}
	
	yiMutexUnLx(info->hMutex);
}

// --------------------------
// for no id login.
/*syiLoginUserInfo* csvryuser::GetUserLoginInfo(int index)
{
	int h = (index-1)/m_ServerInfo.nMaxConnections;
	int l = (index-1)%m_ServerInfo.nMaxConnections;

	csvryConn* p = m_GlobalConMgr->GetConnectionInfo(h);
	
	if (p==0)
		return 0;

	void** ptr = p->GetExtraDataPtr(l);

	if (ptr==0)
		return 0;

	if (*ptr==0)
		return 0;

	return (syiLoginUserInfo* )*ptr;
}*/


#include "yisock.h"
// ---------------------------------------
Bool csvryuser::GetPassportFromProxy(int* passport, int* id, int* port)
{
	char buffer[64];
	int sock = yiconnect_unix(csvryEnv::GetLocalPortName("ProxyServerYotsuya", buffer), 1);

	if (m_sockProxy<0)
	{
		return False;
	}

	if (yisendx(sock, ePID_REQ_PASSPORT, 0, 0, 0, 0, False)!=0)
	{
		close(sock);
		return False;
	}

	sProxyPassportInfo info;
	syipacket packet;
	if (yirecvx(sock, &packet, &info, 0, 100)!=sizeof(info))
	{
		close(sock);
		return False;
	}
	close(sock);

	if (info.port==0)
	{
		return False;
	}
	
	*passport=info.passport;
	*port = info.port;
	*id = info.id;
	
	return True;
}

// --------------------------
int csvryuser::sendx(syiLoginUserInfo* user, int id, void* data, int n, int bBackground)
{
	int res;
	//yiMutexLx(user->hMutex);
	return yisendx(user->sock, id, user->security, m_ServerInfo.security, data, n, bBackground);		
	//yiMutexUnLx(user->hMutex);
	return res;
}

// --------------------------
Bool csvryuser::UpdateUserFile(HandleUserFile hUserFile, int FileNo, void* p, int size, int flag)
{
	char path[256];
	sRegisterUserInfo* reg = REGISTER_INFO(hUserFile);
	unsigned int userId=reg->id;
	int n1=(userId>>24)&0xFF;
	int n2=(userId>>16)&0xFF;
	int n3=(userId>>8)&0xFF;
	int n4=userId&0xFF;

	sprintf(path, "%s/%03d/%03d/%03d/%d/%d", m_szUserRoot/*csvryEnv::GetUserRoot()*/,n1, n2, n3, n4, FileNo);
	sBaseUserInfo* base = BASE_INFO(hUserFile);

	if (base==0)
		return False;

	if (FileNo!=1)
	{
		int index = flag==-1?0:flag*size;
		if (yiFileWriteEx(path, index, p, size, True)==False)
		{
			if (yiFileCreateEx(path, 0, p, size, True)==False)
			{
				YIERROR("Write friend file....(%s) write error!\n", path);
				return False;
			}
		}

		goto UPDATE_SEQUENCE_NO;
	}
	else // file no is 1
	{
		if (flag==eULFF_UPDATE_ALL)
		{
			if (yiFileWriteEx(path, 0, base, sizeof(sBaseUserInfo), True)==False)
			{
			// ユーザ登録失敗？
				YIERROR("Write one file....(%s) write error!\n", path);
				return False;
			}
			return True;
		}

		if (flag&eULFF_UPDATE_TIME)
		{
			if (yiFileWriteEx(path, offsetof(sBaseUserInfo, lastLoginDate), base, 8, True)==False)
			{
				YIERROR("Write one file....(%s) write error! (eULFF_UPDATE_TIME)\n", path);
				return False;
			}
		}
		if (flag&eULFF_UPDATE_SEQUENCE_NO)
		{
			YIDEBUG("Write(eULFF_UPDATE_SEQUENCE_NO) sizeof:%d\n", sizeof(base->sequenceNo));
			if (yiFileWriteEx(path, offsetof(sBaseUserInfo, sequenceNo), (void*)base->sequenceNo, sizeof(base->sequenceNo), True)==False)
			{
				YIERROR("Write one file....(%s) write error! (eULFF_UPDATE_SEQUENCE_NO)\n", path);
				return False;
			}
		}
		if (flag&eULFF_UPDATE_POINT)
		{
			YIDEBUG("updated point %d\n", base->point);
			if (yiFileWriteEx(path, offsetof(sBaseUserInfo, point), &base->point, 4, True)==False)
			{
				YIERROR("Write one file....(%s) write error! (eULFF_UPDATE_SEQUENCE_NO)\n", path);
				return False;
			}
		}
		if (flag&eULFF_UPDATE_SCORE)
		{
			YIDEBUG("updated score wins:%d, loses:%d, draws:%d\n", base->wins, base->loses, base->draws);
			if (yiFileWriteEx(path, offsetof(sBaseUserInfo, total), &base->total, 16, True)==False)
			{
				YIERROR("Write one file....(%s) write error! (eULFF_UPDATE_SEQUENCE_NO)\n", path);
				return False;
			}
		}
		return True;

	}

	YIDEBUG("Write(%s) success\n", path);
	return True;
UPDATE_SEQUENCE_NO:

	{
		base->sequenceNo[FileNo]++;
		sprintf(path, "%s/%03d/%03d/%03d/%d/1", m_szUserRoot/*csvryEnv::GetUserRoot()*/,n1, n2, n3, n4);
		if (yiFileWriteEx(path, offsetof(sBaseUserInfo, sequenceNo) + sizeof(unsigned int)*(FileNo), &base->sequenceNo[FileNo], sizeof(unsigned int), True)==False)
		{
			YIERROR("Write SequenceNo Error(%s)\n", path);
			return False;
		}
	}

	YIDEBUG("csvryuser::UpdateUserFile(%d) success\n", FileNo);
	return True;
}

// -----------------------------------------------------------
/*syiLoginUserInfo2* csvryuser::GetUserLoginInfo(unsigned int id, void** pFileList)
{
	syiLoginUserInfo2* info = (syiLoginUserInfo2*)pFileList[USER_DATA_COUNT];
	if (info==0)
	{
		return 0;
	}

	yiMutexLx(user->hMutex);
	return info;
}*/

// -------------------------------------
/*void csvryuser::ReleaseUserLoginInfo(syiLoginUserInfo2* info)
{
	syiLoginUserInfo2* info = (syiLoginUserInfo2*)pFileList[USER_DATA_COUNT];
	if (info==0)
		return ;

	yiMutexUnLx(user->hMutex);
}*/

// -------------------------------------
int csvryuser::FindMsg32(HandleUserFile hUser, unsigned int from, eMessageType type)
{
	if (hUser==0)
		return -1;
	sMsg32* msg = MSG32(hUser);

	if (msg==0)
		return -1;

	sBaseUserInfo* base = (sBaseUserInfo*)BASE_INFO(hUser);
	if (base==0)
		return -1;

	int n = base->sizes[3]>>5;
	for (int i=0; i<n; i++)
	{
		if (msg[i].remoteId==from && 
				msg[i].type==type)
		{
			return i;
		}
	}

	return -1;
}

// -------------------------------------
Bool csvryuser::DeleteMsg32(HandleUserFile hUser, unsigned int remoteId, int type)
{
	if (hUser==0)
		return False;
	
	// the file create at registered.
	if (hUser[3]==0)
		return False;

	sBaseUserInfo* info = BASE_INFO(hUser);//(sBaseUserInfo*)pFileList[1];
	sMsg32* msg = MSG32(hUser);//(sMsg32*)pFileList[3];
	int	n   =  info->sizes[3]>>5;

	for (int i=0; i<n; i++)
	{
		if (msg[i].type==type &&
			msg[i].remoteId==remoteId)
		{
			msg[i].type=0;
			msg[i].remoteId=0;
			YIDEBUG("csvryuser::DeleteMsg32() Message is deleted (%d:%x)\n", type, remoteId);
			return UpdateUserFile(hUser, 3, &msg[i], sizeof(sMsg32), i);
		}
	}

	return False;
}

// -------------------------------------
Bool csvryuser::DeleteMsg32(HandleUserFile hUser, int index)
{
	if (hUser==0)
		return False;
	
	// the file create at registered.
	if (hUser[3]==0)
		return False;

	sBaseUserInfo* info = BASE_INFO(hUser);//(sBaseUserInfo*)pFileList[1];
	sMsg32* msg = MSG32(hUser);//(sMsg32*)pFileList[3];
	int	n   =  info->sizes[3]>>5;

	if (index>=n)
		return False;

	FindWaitMsg8(hUser, msg[index].remoteId, (eMessageType)msg[index].type, True);
	msg[index].type=0;
	msg[index].remoteId=0;
	return UpdateUserFile(hUser, 3, &msg[index], sizeof(sMsg32), index);
}

// -------------------------------------
/*int csvryuser::FindMsg32(unsigned int id, eMessageType type, HandleUserFile hUserFile)
{
	sMsg32* msg;
	sBaseUserInfo* info;

	if (hUserFile==0)
		return -1;
	
	if ((msg = MSG32(hUserFile))==0)
		return -1;

	if ((info = BASE_INFO(hUserFile))==0)
		return -1;

	int	n   =  (int)info->reserved[1]==0?32:(int)info->reserved[1];
	for (int i=0; i<n; i++)
	{
		if (msg[i].type==type &&
			msg[i].remoteId==remoteId)
		{
			return i;
		}
	}	

	return -1;
}*/

// -------------------------------------
cyiList* csvryuser::LoadMsgEx(HandleUserFile hFile)
{
	if (hFile==0)
		return 0;

	int size=0;
	int sum=0;
	char path[256];
	sRegisterUserInfo* reg = REGISTER_INFO(hFile);

	if (reg==0)
	{
		return 0;
	}

	GetUserFilePath(path, reg->id, "5");
	sMsgEx* msg = (sMsgEx*)yiFileLoadEx(path, &size);

	if (msg==0)
		return 0;
	
	cyiList* list = new cyiList();
	
	while(sum<size)
	{
		sMsgEx* p=(sMsgEx*)yiallocex(msg->length);
		memcpy(p, msg, msg->length);	
		list->insertref(0, p);
		sum+=msg->length;
		p = (sMsgEx*)&((char*)msg)[sum];
	}		

	return list;
}

// -------------------------------------
Bool csvryuser::AddMsgEx(HandleUserFile hFile, eMessageType type, HandleUserFile pRemoteFileList, void* pMsg, int size, Bool bIsReception)
{
	return True;
}

// -------------------------------------
Bool csvryuser::DeleteMsgEx(HandleUserFile hFile, unsigned int id)
{
	return True;
}

// -------------------------------------
Bool csvryuser::AddMsg32(HandleUserFile hUserFile, eMessageType type, 
							HandleUserFile hRemoteFile, Bool bIsReception, unsigned short data)
{
	sMsg32* msg;
	sBaseUserInfo* base;
	unsigned int sequencyNo;
	sRegisterUserInfo* reg = REGISTER_INFO(hUserFile);
	unsigned int myId = reg->id;
	unsigned int remoteId;
	char* pszUserName = reg->szUserName;	

	if (hRemoteFile==0)
		return False;

	remoteId=GETID(hRemoteFile);

	if (remoteId==0)
	{
		return False;
	}
	
	if ((msg = MSG32(hRemoteFile))==0)
		return False;

	if ((base = BASE_INFO(hRemoteFile))==0)
		return False;

	int	n   =  (int)base->reserved[1]==0?32:(int)base->reserved[1];
	int	index=-1;

	for (int i=0; i<n; i++)
	{
		if (msg[i].type==0)
		{
			index=i;
			break;
		}
	}

	if (index==-1)
	{
		// message box is full!
		return False;
	}

	msg[index].type=type;
	msg[index].remoteId=myId;
	msg[index].reserved=data;
	strncpy(msg[index].szUserName, pszUserName, 16);
	msg[index].day = yiLocalTime2(&msg[index].time);

	if (UpdateUserFile(hRemoteFile, 3, &msg[index], sizeof(sMsg32), index)==False)
	{
		YIERROR("csvryuser::AddMsg32() UpdateUserFile(3) error!\n");
		return False;
	}

	if (bIsReception==True)
	{
		AddWaitMsg8(hUserFile, remoteId, type);
	}

	syiLoginUserInfo2* login=LOGIN_INFO2(hRemoteFile);

	if (login==0 ||
		login->isLogined==False)
		return True;


	/*sGeneralParam param;
	param.param[0]=base->sequenceNo[3];
	param.param[1]=index;
	memcpy(&param.param[2], &msg[index], 32);*/

	if (yisendx(login->sock, eUSPID_NOTIFY_MSG32, m_ServerInfo.security, m_ServerInfo.security, &msg[index], 32, True)!=32)
	{
		YIERROR("send error! (eUSPID_NOTIFY_MSG32)\n");
		return True;
	}


	return True;
}

// -------------------------------------
Bool csvryuser::AddWaitMsg8(HandleUserFile hMyData, unsigned int remoteId, eMessageType type)
{
	int size;
	unsigned int myId=GETID(hMyData);
	sMsg8* pWaitMsg;
	sBaseUserInfo* base;

	YIDEBUG("csvryuser::AddWaitMsg8(type:%d, remoteId:%x)\n", type, remoteId);

	if (hMyData==0)
	{
		YIERROR("csvryuser::AddWaitMsg8() hMyData is null\n");
		return False;
	}

	if ((pWaitMsg=WAIT_QUEUE(hMyData))==0)
	{
		YIERROR("csvryuser::AddWaitMsg8() no load wmq8\n");
		return False;
	}

	if ((base=BASE_INFO(hMyData))==0)
	{
		YIERROR("csvryuser::AddWaitMsg8() base is null\n");
		return False;
	}

	int index=-1;
	int n = base->reserved[2]==0?32:base->reserved[2];

	
	for (int i=0; i<n; i++)
	{
		if (pWaitMsg[i].type==0)
		{
			index=i;
			break;
		}
	}

	if (index==-1)
	{
		YIDEBUG("csvryuser::AddWaitMsg8() wait queue is full!\n");
		return False;
	}

	pWaitMsg[index].type=type;
	pWaitMsg[index].remoteId=remoteId;

	{
		char path[256];
		GetUserFilePath(path, myId, "wmq8");
		if (yiFileWriteEx(path, index<<3, &pWaitMsg[index], 8, True)==False)
		{
			YIERROR("csvryuser::AddWaitMsg8(%s) write error!\n", path);
			return False;
		}
	}


	return True;
}

// -------------------------------------
Bool csvryuser::FindWaitMsg8(HandleUserFile hMyData, unsigned int remoteId, eMessageType type, Bool bRemove)
{
	int size;
	sMsg8* pWaitMsg;
	sBaseUserInfo* base;

	if (hMyData==0)
	{
		YIERROR("csvryuser::FindWaitMsg8() hMyData is null\n");
		return False;
	}

	if ((pWaitMsg=WAIT_QUEUE(hMyData))==0)
	{
		YIERROR("csvryuser::FindWaitMsg8() no load wmq8\n");
		return False;
	}

	if ((base=BASE_INFO(hMyData))==0)
	{
		YIERROR("csvryuser::FindWaitMsg8() base is null\n");
		return False;
	}

	int index=-1;
	int n = base->reserved[2]==0?32:base->reserved[2];

	for (int i=0; i<n; i++)
	{
		if (pWaitMsg[i].type==type &&
			pWaitMsg[i].remoteId==remoteId)
		{
			index=i;
			if (bRemove==True)
			{
				pWaitMsg[index].type=0;
			}
			YIDEBUG("wait msg, type:%d, remoteId:%x Found!\n", pWaitMsg[i].type, pWaitMsg[i].remoteId);
			break;
		}
	}

	if (index==-1)
	{
		return False;
	}

	if (bRemove==True)
	{
		char path[256];
		unsigned int myId = GETID(hMyData);
		GetUserFilePath(path, myId, "wmq8");
		if (yiFileWriteEx(path, index<<3, &pWaitMsg[index], 8, True)==False)
		{
			YIERROR("csvryuser::FindWaitMsg32(%s) write error!\n", path);
			return False;
		}
	}

	return True;
}

typedef struct {
	int param;
	int size;
	char data[8];
} sGeneralParamEx;

// -------------------------------------
Bool csvryuser::SendError(int sock, int id, int result, const void* data, int size)
{
	sGeneralParamEx* param = (sGeneralParamEx*)yialloc(size+8);
	param->param=result;
	param->size=0;
	if (size!=0)
	{
		param->size=size;
		memcpy(param->data, data, size);
	}

	if (yisendx(sock, id, m_ServerInfo.security, m_ServerInfo.security, param, size+8, True)!=(size+8))
	{
		YIERROR("csvryuser::SendError(id:%d) error! \n", id);
		yifree(param);
		return False;
	}
	yifree(param);

	return True;
}

// -------------------------------------
Bool csvryuser::SendError(HandleUserFile hFile, int id, int result, int no, ...)
{
	syiLoginUserInfo* p=0;
	unsigned int* param = (unsigned int*)&no;
	int size=0;
	sGeneralParam s= {0, };

	s.param[0]=result;

	if (hFile==0)
		return False;

	if ((p=LOGIN_INFO(hFile))==0)
	{
		return False;
	}
	
	if (no==0)
		goto SEND;

	param++;

	s.param[0]=result;
	for (int i=1; i<no; i++)
	{
		s.param[i]=*param;
		size+=sizeof(unsigned int);
		param++;
	}

SEND:
	if (yisendx(p->sock, id, m_ServerInfo.security, m_ServerInfo.security, &s, size, True)!=size)
	{
		YIERROR("csvryuser::SendError(id:%d) error! \n", id);
		return False;
	}

	return True;
}

// -------------------------------------
Bool csvryuser::SendError(int sock, int id, int result, int no, ...)
{
	unsigned int* param = (unsigned int*)&no;
	int size=sizeof(result);
	sGeneralParam s= {0, };
	s.param[0]=result;

	if (no==0)
		goto SEND;

	param++;

	for (int i=0; i<no; i++)
	{
		s.param[i+1]=*param;
		size+=sizeof(unsigned int);
		param++;
	}

SEND:
	if (yisendx(sock, id, m_ServerInfo.security, m_ServerInfo.security, &s, size, True)!=size)
	{
		YIERROR("csvryuser::SendError(id:%d) error! \n", id);
		return False;
	}

	return True;
}

// -------------------------------------
Bool csvryuser::DeleteFriend(HandleUserFile hUserFile, unsigned int friendId)
{
	sBaseUserInfo* base=BASE_INFO(hUserFile);
	sUser* friendz = FRIEND_INFO(hUserFile);
	int n = base->sizes[2]/sizeof(sUser);
	int index=-1;
	for (int i=0; i<n; i++)
	{
		if (friendz[i].id==friendId)
		{
			friendz[i].id=0;
			index=i;
			break;
		}
	}

	return UpdateUserFile(hUserFile, 2, &friendz[index], sizeof(sUser), index);
}

// -------------------------------------
Bool csvryuser::AddFriend(HandleUserFile hUserFile, unsigned int remoteId, const char* pszFriendName)
{
	sBaseUserInfo* base = BASE_INFO(hUserFile);
	unsigned int myId = GETID(hUserFile);
	sUser* friendz = FRIEND_INFO(hUserFile);
	int n = base->sizes[2]/sizeof(sUser);
	int index=-1;
	for (int i=0; i<n; i++)
	{
		if (friendz[i].id==0)
		{
			index=i;
			break;
		}
		else if (friendz[i].id==remoteId)
		{
			return True;
		}
	}

	// friend is full!
	if (index==-1)
		return False;

	friendz[index].id=remoteId;
	strncpy(friendz[index].szUserName, pszFriendName, 16);

	return UpdateUserFile(hUserFile, 2, &friendz[index], sizeof(sUser), index);
}

// -------------------------------------
void csvryuser::GetGoDataPath(char* path, sGoHeader* header)
{
	char buffer[256];
	sprintf(buffer, "%s/%d/%02d/%02d/%02d", m_szCommonDir, DAY_YEAR(header->id[2]), DAY_MON(header->id[2]), DAY_DAY(header->id[2]), YLT_HOUR(header->id[3]));
	sprintf(path, "%s/%x%x%x%x", buffer, header->id[0], header->id[1], header->id[2], header->id[3]);
	
	return ;
}

// -------------------------------------
Bool csvryuser::AddGoData(sGoHeader* header, void* data, int size, HandleUserFile hUserFile)
{
	char szGoFileName[256];
	Bool dir;

	GetGoDataPath(szGoFileName, header);

	if (yiIsExist(szGoFileName, &dir)==True)
	{
		return False;
	}

	char path[256];
	char filename[256];
	sprintf(path, "%s/%d/%02d/%02d/%02d", m_szCommonDir, DAY_YEAR(header->id[2]), DAY_MON(header->id[2]), DAY_DAY(header->id[2]), YLT_HOUR(header->id[3]));

	if (yiIsDir(path)==False)
	{
		yiMakeDir(path, True);
	}

	unsigned int seq=0;
	for (int i=0; i<2; i++)
	{
		GetUserFilePath(path, header->id[0], "go");

		if (yiIsExist(path, 0)==False)
		{
			seq=0;
			yiFileCreateEx(path, 0, &seq, sizeof(unsigned int), True);
		}
		if (yiFileReadEx(path, 0, &seq, sizeof(unsigned int), True)==False)
		{
			YIERROR("csvryuser::AddGoData(%s) read error!\n", path);
			return False;
		}
		seq++;
		if (yiFileWriteEx(path, 0, &seq, sizeof(unsigned int), True)==False)
		{
			YIERROR("csvryuser::AddGoData(%s) write sequency number error!\n", path);
			return False;
		}

		if (yiFileWriteEx(path, -1, header, sizeof(*header), True)==False)
		{
			YIERROR("csvryuser::AddGoData(%s) write sequency number error!\n", path);
			return False;
		}
	}

	
	if (yiFileCreateEx(szGoFileName, 0, data, size, True)==False)
	{
		YIERROR("csvryuser::AddGoData(%s) failure!!\n", szGoFileName);
		return False;
	}

	return True;
}

// -------------------------------------
/*Bool csvryuser::SaveWaitMessageQueue(unsigned int myId, unsigned int remoteId, const char* pszUserName, eMessageType type, void* data, int size)
{
	char buffer[32]={0,};
	char path[256];
	GetUserFilePath(path, myId, "wq32");
	//sprintf(path, "%s/%03d/%03d/%03d/%d/wq32", m_szUserRoot, (myId>>24)&0xFF, (myId>>16)&0xFF, (myId>>8)&0xFF, info->id&0xFF);

	if (yiIsExsist(path, &bIsDir)==False)
	{
		if (yiFileCreateQueue(path, 32, 32)==False)
		{
			YIERROR("csvryuser::SaveWaitMessageQueue() create failure!! %s", path);
			return False;
		}
	}

	if (type==eWMT_ADD_FRIEND)
	{
		memcpy(buffer, &type, sizeof(int));
		memcpy(&buffer[8], remoteId, sizeof(unsigned int));
		memcpy(&buffer[12], pszUserName, 16);
	}

	if (yiFileEnQ(path, buffer)==False)
	{
		// queue is full!
		YIERROR("csvryuser::SaveWaitMessageQueue() queue is full!! %s", path);
		return False;
	}

	return True;
}
*/
