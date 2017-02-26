#include "ypnGoNetwork.h"

// ---------------------------
cypnGoNetwork::cypnGoNetwork(void)
	: cyPrivateNetwork()
	, m_msg(0)
{
	m_hUserMutex = yiMutexCreateEx(3);
	memset(m_hUserList, 0, sizeof(m_hUserList));
	memset(&m_myGoInfo, 0, sizeof(m_myGoInfo));
}

// ---------------------------
cypnGoNetwork::~cypnGoNetwork(void)
{
}

// ---------------------------
void cypnGoNetwork::SetMyGoInfo(sGoUserInfoEx info)
{
	m_myGoInfo=info;
}

// ------------------------------------
//HandleUser cypnGoNetwork::ConnectToUser(int Id, const char* pszUserName, int nRoomNo, const char* pszPassword, sypnUserInfo* info)
//{
//	if (Connect(Id, pszUserName, nRoomNo, pszPassword, info)==True)
//	{
//		sGoUserInfoDetail detail;
//		if (SendToUser(info->index, (int)::eGNPMSG_REQ_INFO_DETAIL, 0, 0)==True)
//		{
//			if (WaitMessage(m_socks[info->index], eGNPMSG_REQ_INFO, &detail, sizeof(detail))==False)
//			{
//				OnRemoveUser(m_socks[info->index], info->index);
//				return 0;
//			}
//
//			HandleUser hUser = 
//		}
//		OnRemoveUser(m_socks[info->index], info->index);
//		return 0;
//	}
//
//	return 0;
//}

// ---------------------------
Bool cypnGoNetwork::Login(HWND hNotifyWnd, UINT msg, address_t server, int port, unsigned int security, 
					unsigned int id, const char* pszUserName, const char* pszPassword)
{
	Bool bIsUpnpSuccess=False;

	m_hNotifyWnd = hNotifyWnd;
	m_NotifyMessageId = msg&0xFFFF;
	m_NotifyServerMessageId=msg>>16;
	strcpy(m_myinfo.szUserName, pszUserName);

	if (m_MainSock==0)
		m_MainSock = uthpTcpConnect(server, port, security);

	if (m_MainSock==0)
	{
		xTRACE("uthpTcpConnect() failure ip:%s, port:%d, security:%x\n", yiltostr(server), port, security);
		return False;
	}

	unsigned int udpPort;
	if (uthpTcpLogin(m_MainSock, id, pszUserName, pszPassword, &m_myinfo.network, &udpPort)/*, m_AcceptSock, &buffer, &nReceived)*/<0)
	{
		xTRACE("uthpTcpLogin() login failure\n");
		m_nLastError=m_myinfo.id;
		DestroySocket(m_MainSock);
		m_MainSock=0;
		return FALSE;
	}

	// Keep Alive 10分間隔
	if (yiTcpSetKeepAlive(m_MainSock->sock, True, 60*1000, 1000)==-1)
	{
		xTRACE("Keep Alive Failure!");
	}

	//m_myinfo.udpPort=udpPort;
	m_myinfo.id=id;
	memset(&m_myGoInfo, 0, sizeof(m_myGoInfo));
	//m_myGoInfo.index = m_myinfo.index;
	m_myGoInfo.base.udpPort=udpPort;//m_myinfo.network.index;
	strncpy(m_myGoInfo.base.szName, m_myinfo.szUserName, 16);

	return True;
}

// ---------------------------
Bool cypnGoNetwork::CheckMyNetwork(sNetworkInfo* info, address_t localaddr)
{
	int udpSock=0;

	if (info!=0)
	{
		m_myinfo.network=*info;
		goto SKIP_CHECK;
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

/*	if (m_myinfo.network.type==eNT_NONE ||
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
	}*/

	m_myinfo.network.localport=0;
	m_myinfo.network.port=0;

SKIP_CHECK:
	xTRACE("uthpRegisterMyNetwork() call\n");
	if (uthpRegisterMyNetwork(m_MainSock, m_myinfo.network.type, m_myinfo.network.address, m_myinfo.network.port)==FALSE)
	{
		xTRACE("uthpRegisterMyNetwork() failure\n");
		DestroySocket(m_MainSock);
		m_MainSock=0;

		return FALSE;
	}

	//if (pszName!=0)
	//{
	//	m_myinfo.network.security = cyiEncoder::GetYotsuyaSecurityCode(pszName);
	//}
	//else
	//{
	//	m_myinfo.network.security = YPN_DEFAULT_SECURITY;
	//}
	return True;
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
Bool cypnGoNetwork::StartLoop()
{
	if (m_MainSock==0)
		return False;

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

//// ---------------------------
Bool cypnGoNetwork::SendTextMessage(HandleUser hUser,  const char* buffer)
{
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_SEND_TEXT_MESSAGE, (void*)buffer, strlen(buffer)+1);	
}

static unsigned int xReverseCondition(unsigned int condition)
{
	unsigned int rconditon = condition;
	if (IAM_BLACK(rconditon)==TRUE)
	{
		SET_IAM_WHITE(rconditon);
	}
	else
	{
		SET_IAM_BLACK(rconditon);
	}

	return rconditon;
}

// ---------------------------
Bool cypnGoNetwork::SendRequestPlay(HandleUser hUser, unsigned int con1, unsigned int con2)
{
	unsigned int reverse = con1;

	//if (IS_NIGIRI(con1)==False)
	//{
	reverse=::xReverseCondition(con1);
	//}

	int buffer[2]={reverse, con2};

	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_REQ_PLAY, (char*)buffer, sizeof(buffer));
}


// ---------------------------
Bool cypnGoNetwork::SendReplyPlay(HandleUser hUser, unsigned int con1, unsigned int con2)
{
	unsigned int reverse = con1;

	//if (IS_NIGIRI(con1)==False)
	//{
	reverse=xReverseCondition(con1);
	//}

	unsigned int buffer[2]={(con1==0xFFFFFFFF)?0xFFFFFFFF:reverse, con2};

	sGoUserInfo* info = (sGoUserInfo*)hUser;
	
	return SendToUser(info->index, (int)eGNPMSG_REPLY_PLAY, (char*)buffer, sizeof(buffer));
}

#define __PRINT_ADDR__(a) a&0xFF, (a>>8)&0xFF, (a>>16)&0xFF, (a>>24)&0xFF
// ---------------------------
int cypnGoNetwork::CreateUdpSock(address_t& addr, int& port, address_t& addr_local, int& port_local)
{
	sGeneralParam param = { 3, 0};

	addr=0;

	int sock=yiudp(0, 0);

	if (sock<=0)
	{
		return 0;
	}
	
	if (yisendu(sock, &param, 32, m_MainSock->remote.address, m_myGoInfo.base.udpPort)!=32)
	{
		yiclose(sock);
		return 0;
	}

	if (yirecvu(sock, &param, 8, 0, 0, 0)!=8)
	{
		yiclose(sock);
		return 0;
	}

	addr			= param.param[0];
	port			= param.param[1];
	addr_local		= yigetlocaladdress(0, 0);
	port_local		= yigetport(sock, 0);

	xTRACE("cypnGoNetwork::CreateUdpSock()%u.%u.%u.%u:%d, %u.%u.%u.%u:%d", __PRINT_ADDR__(addr), port, __PRINT_ADDR__(addr_local), port_local);

	return sock;
}


// ------------------------------------
BOOL cypnGoNetwork::SendToUserEx(HandleUser hUser, int id, const void* data, int size, Bool bIsConfirm)
{
	int index = ((sGoUserInfo*)hUser)->index;
	return SendToUser(index, id, data, size, bIsConfirm);
}

// ---------------------------
Bool cypnGoNetwork::BroadCastSend(HandleUser* hUsers, int n, int id, const void* data, int size)
{
	for (int i=0; i<n; i++)
	{
		if (hUsers[i])
			SendToUserEx(hUsers[i], id, data, size, False);
	}

	return True;
}

// ---------------------------
Bool cypnGoNetwork::BroadCastSend(cyiList* listUser, int id, const void* data, int size)
{
	for (cyiNode* node = listUser->GetNodeTopFrom(); node; node=node->np)
	{
		SendToUserEx((HandleUser)node->ref, id, data, size, False);
	}

	return True;
}


// ---------------------------
unsigned int cypnGoNetwork::SendRequestPlayStart(HandleUser hUser, unsigned int* con1, unsigned int con2, ugoid_t& uid)
{
	sGoUserInfo* info = (sGoUserInfo*)hUser;

	if (IS_NIGIRI(*con1)==TRUE)
	{
		srand(cyiTime::CurrentmSec());
		Bool bIsBlack = rand()%2;

		xTRACE("cypnGoNetwork::SendRequestPlayStart(%x, %x)", *con1, con2);

		if (bIsBlack==True)
		{
			SET_IAM_BLACK(*con1);

			if (uid.b_id==info->id)
			{
				unsigned int temp = uid.b_id;
				uid.b_id=uid.w_id;
				uid.w_id=temp;
			}
		}
		else
		{
			SET_IAM_WHITE(*con1);

			if (uid.w_id==info->id)
			{
				unsigned int temp = uid.b_id;
				uid.b_id=uid.w_id;
				uid.w_id=temp;
			}
		}
	}

	//unsigned int id =cyiTime::GetLocalTime();
	unsigned int condition = xReverseCondition(*con1);
	unsigned int buffer[6] = {condition, con2, uid.b_id, uid.w_id, uid.day, uid.time};
	
	xTRACE("cypnGoNetwork::SendRequestPlayStart reversed(%x, %x)", condition, con2);


	//info->id = id;

	//yiTrace("SendRequestPlayStart go id is %d", id);
	if (cyPrivateNetwork::SendToUser(info->sock, (int)eGNPMSG_REQ_PLAY_START, (char*)buffer, sizeof(buffer))==FALSE)
	{
		return 0;
	}

	return True;
}



// ---------------------------
Bool cypnGoNetwork::SendRequestPlayEnd(HandleUser hUser, unsigned int id, ePlayEndStatus status)
{
	unsigned int buffer[2]={id, status};
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_REQ_PLAY_END, (char*)buffer, sizeof(buffer));
}

// ---------------------------
Bool cypnGoNetwork::SendRequestPlayEndOk(HandleUser hUser, unsigned int id, ePlayEndStatus status, Bool bIsOK)
{
	unsigned int buffer[3]={id, status, bIsOK};
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_REQ_PLAY_END_OK, (char*)buffer, sizeof(buffer));
}

// ---------------------------
Bool cypnGoNetwork::SendRequestCalculateEnd(HandleUser hUser, unsigned int id)
{
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_REQ_CALCULATE_END, &id, sizeof(id));
}

// ---------------------------
Bool cypnGoNetwork::SendRequestCalculateEndOk(HandleUser hUser, unsigned int id, Bool bIsOk)
{
	unsigned int buffer[2]={id, bIsOk};
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_REQ_CALCULATE_END_OK, buffer, sizeof(buffer));
}

// ---------------------------
Bool cypnGoNetwork::SendStone(HandleUser hUser, stone_t stone, unsigned int usedTime)
{
	unsigned int buffer[2]={stone, usedTime};
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	xTRACE("cypnGoNetwork::SendStone(%d, %u)", STONE_TYPE(stone), usedTime);
	return SendToUser(info->index, (int)eGNPMSG_SEND_STONE, (char*)buffer, sizeof(buffer));
}

// -------------------------------
Bool cypnGoNetwork::SendGiveup(HandleUser hUser)
{
	xTRACE("SendGiveup!");
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_GIVEUP, (char*)0, 0);
}



// -------------------------------
Bool cypnGoNetwork::SendTimeout(HandleUser hUser, Bool bIsBlack, unsigned int goid)
{
	xTRACE("SendTimeout!");
	unsigned int buffer[2] = {bIsBlack, goid};
	sGoUserInfo* info = (sGoUserInfo*)hUser;
	return SendToUser(info->index, (int)eGNPMSG_TIME_LOSE, (char*)&buffer, sizeof(buffer));
}

// -------------------------------
void cypnGoNetwork::DestroyUser(HandleUser hUser)
{
	sGoUserInfo* info = (sGoUserInfo*)hUser;

	RemoveDescriptor(info->sock, -1);
	//uthpClose(info->sock);
	DestroySocket(info->sock);

}


static void* MakeNotifyMsg(HandleUser hUser, void* data, int size)
{
	char* p = (char*)yialloc(sizeof(int)+size+sizeof(hUser));
	HandleUser* x = (HandleUser*)p;
	x[0]=hUser;
	int* pIndex=(int*)p;
	pIndex[1]=size;
	memcpy(&pIndex[2], data, size);
	return x;

}

// ---------------------------
Bool cypnGoNetwork::OnMessage(socket_t sock, int index, int id, void* buffer, int size)
{
	// From Server Notify.
	if (index==-1)
	{
		NotifyServerMessage((size<<16|id), yitoheap(buffer, size));
		return True;
	}

	if (cyPrivateNetwork::OnMessage(sock, index, id, buffer, size)==False)
	{	
		return False;
	}

	if (m_hUserList[index]==0)
	{
		return False;
	}

	switch(id)
	{
		case eGNPMSG_SEND_TEXT_MESSAGE :
		{
			xTRACE("received text:%s", buffer);
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, strlen((const char*)buffer)+1));
			break;
		}
		case eGNPMSG_RPY_INFO_DETAIL:
		{
			xTRACE("eGNPMSG_RPY_INFO_DETAIL received");

			if (m_myinfo.id==0)
			{
				sGoUserInfo* s=(sGoUserInfo*)buffer;
				sGoUserInfo* x=(sGoUserInfo*)m_hUserList[index];
				x->room=s->room;
				strncpy(x->szName, s->szName, 16);
			}
			else
			{
				//sGoUserInfoEx* s=(sGoUserInfoEx*)buffer;
				//sGoUserInfoEx* x=(sGoUserInfoEx*)m_hUserList[index];
				//x->base.room=s->base.room;
				//x->base.id=s->base.id;
				//strncpy(x->base.szName, s->base.szName, 16);
				//x->reg=s->reg;
				//memcpy(x->score, s->score, sizeof(x->score));
				//x->seq_pic=s->seq_pic;
				//x->state=s->state;
				NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, size));
			}
			// ここで詳細情報を設定。
			// 親に知らせる？
			
			//yifree(buffer);
			break;
		}
		//case eGNPMSG_REQ_PLAY_START:
		//{
		//	xTRACE("eGNPMSG_REQ_PLAY_START received");
		//	sGoUserInfo* info = (sGoUserInfo*)m_hUserList[index];
		//	unsigned int* _buffer = (unsigned int*)buffer;
		//	//info->id = _buffer[0];
		//		
		//	NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 8));
		//	break;
		//}
		case eGNPMSG_REQ_PLAY_END:
		{
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 8));
			break;
		}
		case eGNPMSG_REQ_PLAY_END_OK:
		{
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 12));
			break;
		}
		case eGNPMSG_REQ_CALCULATE_END:
		{
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 4));
			break;
		}
		case eGNPMSG_REQ_CALCULATE_END_OK:
		{
			NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, 8));
			break;
		}

		default:
		{
			xTRACE("cypnGoNetwork::OnMessage() received..... %d", id);
			if (size==0)
			{
				NotifyMessage(id, (void*)index);
			}
			else
			{
				NotifyMessage(id, MakeNotifyMsg(m_hUserList[index], buffer, size));
			}
			break;
		}
	}

	//yifree(buffer);

	return True;
}	



// ---------------------------
void cypnGoNetwork::OnAddUser(socket_t sock, int index, void* param)
{
	sypnUserInfo* remote = (sypnUserInfo*)param;

	sGoUserInfo info;// = {sock, index, 0};
	memset(&info, 0, sizeof(info));
	info.sock=sock;
	info.index=index;
	info.room=remote->network.index;
	info.id=remote->id;

	strncpy(info.szName, remote->szUserName, 16);

	xTRACE("cypnGoNetwork::OnAddUser() name:%s", info.szName);
	//if (SendToUser(info.index, (int)eGNPMSG_REQ_INFO_DETAIL, 0, 0)==False)
	//{
	//	xTRACE("cypnGoNetwork::OnAddUser() eGNPMSG_REQ_INFO_DETAIL send error");
	//	DisConnectUser(info.index);
	//	return ;
	//}
	yiMutexLock(m_hUserMutex);
	m_hUserList[index] = (HandleUser)yitoheap(&info, sizeof(info));
	sock->pExtraData=m_hUserList[index];
	sock->nExtraDataSize=sizeof(sGoUserInfo);

	yiMutexUnlock(m_hUserMutex);

}

// ---------------------------
void cypnGoNetwork::OnRemoveUser(socket_t sock, int index)
{
	if (m_hUserList[index]==0)
		return ;

	yiMutexLock(m_hUserMutex);
	yifree((void*)m_hUserList[index]);
	m_hUserList[index]=0;
	yiMutexUnlock(m_hUserMutex);

	//DestroySocket(sock);
}

// ---------------------------
Bool cypnGoNetwork::OnLogin(void* buffer, int size)
{
	// ログイン情報設定
	sGeneralParam* param = (sGeneralParam*)buffer;

	memset(&m_myGoInfo, 0, sizeof(m_myGoInfo));
	//m_myGoInfo.index = m_myinfo.index;
	m_myGoInfo.base.room=m_myinfo.network.index;
	strncpy(m_myGoInfo.base.szName, m_myinfo.szUserName, 16);
/*	if (m_myinfo.id!=0)
	{
		int size=0;
		int id=0;
		//if (uthpRecv(this->m_MainSock, &id, &m_register, &size, 1000)==False)
		//{
		//	// どうする？
		//	return False;
		//}

		if (uthpRecv(m_MainSock, &id, &m_base, &size, 1000)==False)
		{
			// どうする？
			return False;
		}

		if (param->param[4])
		{
			char buffer[1024];
			char* pMsg=buffer;

			if (m_msg==0)
				m_msg = new cyiList(True);

			if (uthpRecv(m_MainSock, &id, pMsg, &size, 1000)==False)
			{
			}
			else
			{
				int n = size>>5;
				int index=0;
				for (int i=0; i<n; i++)
				{
					m_msg->addref(yitoheap(&pMsg[index], 32));
					index+=32;
				}
			}
		}

		if (param->param[5])
		{
			char buffer[4096];
			char* pMsg=buffer;

			if (m_msg==0)
				m_msg = new cyiList(True);

			if (uthpRecv(m_MainSock, &id, pMsg, &size, 1000)==False)
			{
			}
			else
			{
				int n = size>>7;
				int index=0;
				for (int i=0; i<n; i++)
				{
					m_msg->addref(yitoheap(&pMsg[index], 32));
					index+=32;
				}
			}
		}

	}*/

	return True;
	/*else
	{
		sGoUserInfo* myinfo = (sGoUserInfo*)buffer;
		memcpy(&m_myGoInfo, myinfo, sizeof(m_myGoInfo));
		m_myGoInfo.index=m_myinfo.network.index;


		
		// 友人情報取得
		// 

		// グループ情報取得
		// myinfo->groups;

	}*/
}

// ---------------------------
HandleUser cypnGoNetwork::FindUserByRommNo(int nRoomNo)
{
	yiMutexLock(m_hUserMutex);
	for (int i=0; i<MAX_CONNECT_USER_CNT; i++)
	{
		if (m_hUserList[i]==0)
		{
			continue;
		}
		sGoUserInfo* s = (sGoUserInfo*)m_hUserList[i];

		if (s->room==nRoomNo)
		{
			yiMutexUnlock(m_hUserMutex);
			return m_hUserList[i];
		}
	}
	yiMutexUnlock(m_hUserMutex);
	return 0;
}


// ---------------------------
HandleUser cypnGoNetwork::FindUser(const char* pszUserName, unsigned int id, unsigned int go_id)
{
	yiMutexLock(m_hUserMutex);
	for (int i=0; i<MAX_CONNECT_USER_CNT; i++)
	{
		if (m_hUserList[i]==0)
		{
			continue;
		}
		sGoUserInfo* s = (sGoUserInfo*)m_hUserList[i];

		if (go_id!=0)
		{
			if (s->goid==go_id)
			{
				yiMutexUnlock(m_hUserMutex);
				return m_hUserList[i];
			}
			continue;
		}

		sypnUserInfo* info = (sypnUserInfo* )s->sock->pExtraData;
		if (info==0)
		{
			continue;
		}


		if (id!=0)
		{
			if (info->id==id)
			{
				yiMutexUnlock(m_hUserMutex);
				return m_hUserList[i];
			}
			continue;
		}


		if (pszUserName && strcmp(info->szUserName, pszUserName)==0)
		{
			yiMutexUnlock(m_hUserMutex);
			return m_hUserList[i];
		}
	}

	yiMutexUnlock(m_hUserMutex);
	return 0;
}


// ---------------------------
HandleUser cypnGoNetwork::IndexToUser(int i)
{
	return m_hUserList[i];
}

// -------------------------------------------
sGoUserInfo* cypnGoNetwork::GetGoUserInfo(HandleUser hUser)
{
	return (sGoUserInfo*)hUser;
}

// -------------------------------------------
sGoUserInfoEx* cypnGoNetwork::GetMyGoInfo()
{
	return &m_myGoInfo;
}