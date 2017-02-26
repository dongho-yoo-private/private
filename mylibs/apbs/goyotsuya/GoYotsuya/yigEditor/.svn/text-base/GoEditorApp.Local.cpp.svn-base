#include "GoEditorApp.h"
#include "yiGoCommandz.h"

#ifdef _GOYOTSUYA
// ------------------------
address_t CGoEditorApp::xGetServerAddress(int* port)
{
	char szIpServerName[256];
	char szAddressFile[256];



	// 村田さんのためのコード
	if (GetProperty(0, "ServerAddr", szIpServerName)==True)
	{
		if (GetPropertyInt(0, "ServerPort", *port)==True)
		{
			int a1, a2, a3, a4;
			if (sscanf(szIpServerName, "%d.%d.%d.%d", &a1, &a2, &a3, &a4)==4)
			{
				return __COLORp__(a4, a3, a2, a1);
			}
		}
	}

	if (GetProperty(0, "ipserver", szIpServerName)==False)
	{
		return 0;
	}

	if (GetProperty(0, "addf", szAddressFile)==False)
	{
		return 0;
	}

	int len=512;

	char* pszServerInfo = yiHttpGet(szIpServerName, szAddressFile, &len);

	if (pszServerInfo==0)
		return 0;

	char* pIndex=strstr(pszServerInfo, "text/plain");
	pIndex+=strlen("text/plain");

	while(*pIndex)
	{
		if (*pIndex=='\r' || 
				*pIndex=='\n' ||
					*pIndex==' ' ||
						*pIndex=='\t')
		{
			pIndex++;
			continue;
		}

		break;
	}

	char szServerIp[32];
	int a1, a2, a3, a4;
	if (sscanf(pIndex, "%d.%d.%d.%d:%d", &a1, &a2, &a3, &a4, port)!=5)
	{
		yifree(pszServerInfo);
		return 0;
	}
	yifree(pszServerInfo);

	return __COLORp__(a4, a3, a2, a1);
}

#include "yiEncoder.h"
#include "MyMainFrame.h"
#include "yiEncoder.h"

typedef struct 
{
	char szId[16];
	char szPass[16];
	unsigned int id;
} xIdx;

#define IDFILE_RAND_NO 0x31e57ab2
// --------------------------------
Bool CGoEditorApp::xSaveId(unsigned int id, const char* pszId, const char* pszPass)
{
	xIdx xx;
	char path[256];

	if (GetFromIdList(pszId)!=0)
	{
		return True;
	}

	memset(&xx, 0, sizeof(xx));
	wsprintf(path, "%s\\.idx", m_szUserDir);

	strcpy(xx.szId, pszId);
	strcpy(xx.szPass, pszPass);

	cyiEncoder en(IDFILE_RAND_NO);

	void* p = (void*)en.Encode(0, &xx, sizeof(xx));
	Bool res = yiFileWrite(path, p, -1, sizeof(xx)<<1); 
	yifree(p);

	return res;
}

// --------------------------------
unsigned int CGoEditorApp::GetFromIdList(const char* id)
{
	// id ファイルを開く
	// 同じidがあるかをみる。
	char path[256];

	wsprintf(path, "%s\\.idx", m_szUserDir);

	int size;
	void* p = yiFileLoad(path, &size);

	if (p==0)
		return 0;

	char* pIndex=(char*)p;

	cyiEncoder en(IDFILE_RAND_NO);

	while(1)
	{
		char buffer[sizeof(xIdx)<<1];
		xIdx* x=(xIdx*)buffer;
		memcpy(buffer, pIndex, sizeof(xIdx)<<1);

		x = (xIdx*)en.Decode(0, buffer, sizeof(xIdx)<<1);

		if (strcmp(x->szId, id)==0)
		{
			yiFileUnload(p);
			yifree(x);
			return x->id;
		}
		yifree(x);

		pIndex=&pIndex[sizeof(xIdx)<<1];

		if ((pIndex-p)>=size)
			break;
	}


	yiFileUnload(p);
	return 0;
}

// --------------------------------
#include "yiHashFunc.h"
unsigned int CGoEditorApp::IdToInteger(const char* id)
{
	unsigned int n1 = yihf_additional((char*)id, 8);
	unsigned int n2 = yihf_encoded_additional((char*)id, 8);
	unsigned int n3 = yihf_general(id, 8);

	return __COLORp__(n1, n2, n3, 0);
}

// --------------------------------
Bool CGoEditorApp::xUpdateMessage(cyiList* msg)
{
	if (msg==0 ||
			msg->count()==0)
	{
		return False;
	}

	m_goban->m_container->SetHiLight(True);
	return True;
}

// --------------------------------
//Bool CGoEditorApp::xLoadMyFriendList(unsigned int id, const char* pszMyId, unsigned int sequnceNo)
//{
//	char path[256];
//	int size;
//	wsprintf(path, "%s\\2", m_szMyDir);
//
//	sFriendList* list = (sFriendList*)yiFileLoadEx(path, &size);
//
//	// ファイルが存在しない？
//	if (list==0)
//	{
//		if (sequnceNo==0)
//			return True;
//
//		return False;
//	}
//
//	if (list->seq_no==sequnceNo)
//	{
//		// この場合は友達リストに追加する。
//		if (m_friendz==0)
//			m_friendz = new cyiList(True);
//
//		m_friendz->lock();
//		m_friendz->clear();
//
//		for (int i=0; i<(list->cnt>>16); i++)
//		{
//			m_friendz->addref(yitoheap(&list->user[i], sizeof(sUser)));
//		}
//		m_friendz->unlock();
//		yiFileUnLoadEx(list);
//
//		m_goban->UpdateMyFriend(m_friendz);
//		return True;
//	}
//
//	yiFileUnLoadEx(list);
//	return False;
//
//}

// --------------------------------
void CGoEditorApp::xOnSearchRouter(void* pHandleUpnp)
{
	CGoEditorApp* app = (CGoEditorApp*)pHandleUpnp;
	app->OnSearchRouter();
}

void CGoEditorApp::OnSearchRouter()
{
	//::yiTraceM2("c:\\yotsuya\\upnp.log", "OnSearchRouter called");
	m_hUpnp = yiupnpCreatex();
	//::yiTraceM2("c:\\yotsuya\\upnp.log", "OnSearchRouter yiupnpCreatex=%x", m_hUpnp);
	//m_bIsFindRouterDone=True;

	char addr[32];
	if (yiupnpGetExternalAddressx(m_hUpnp, addr)==False)
	{
		yiupnpDestroyx(m_hUpnp);
		m_hUpnp=0;
	}
	PostCommand(eGCID_ROUTER_FOUND, 0);
}

// --------------------------------
void CGoEditorApp::FindRouter()
{
	_beginthread(CGoEditorApp::xOnSearchRouter, 0, this);
}

// --------------------------------
Bool CGoEditorApp::xLogin(const char* szId, const char* pszPass, Bool bIsNormal)
{
	unsigned int id = 0;
	sBaseUserInfo* before	= 0;

	//if (m_dictionary.user==0)
	//{
	//	char path[256];
	//	int size=0;
	//	m_dictionary.user=(sUserDictionary*)yiFileLoadEx(path, &size);
	//	m_dictionary.n = size/sizeof(sUserDictionary);
	//}

	m_bIsLoginNormal=bIsNormal;

	if (bIsNormal==True)
	{
		id = GetUserId(szId, False);

		if (id==0)
		{
			id = CGoEditorApp::IdToInteger(szId);
		}
	}

	if (m_network==0)
	{
		m_network=new cypnGoNetwork();//(cypnGoNetwork*)cypnGoNetwork::GetInstance();
	}


	//if (m_network->Start(m_hWnd, WM_NOTIFY_FROM_NETWORK|(WM_NOTIFY_FROM_SERVER<<16), 
	//	m_server_addr, m_server_port, cyiEncoder::GetYotsuyaSecurityCode("UserServerYotsuya"), id, szId, pszPass, 0)==FALSE)
	//{
	//	return FALSE;
	//}

	Bool reason;
	
	{
		int size;
		char szUpnpPath[256];
		wsprintf(szUpnpPath, "%s\\%s", m_szInstDir, "upnp");

		void* p = ::yiFileLoad(szUpnpPath, &size);

		if (p)
		{
			m_hUpnp = yiupnpImport(p);
		}

		m_bIsFindRouterDone=True;
		if (m_hUpnp==0)
		{
			FindRouter();
			m_bIsFindRouterDone=False;
		}
	}

	//m_network->InitNetwork();//UpnpTest("192.168.1.5", 10008, &reason);

	//m_server_port=10004;
	if (m_network->Login(m_hWnd, WM_NOTIFY_FROM_NETWORK|(WM_NOTIFY_FROM_SERVER<<16), m_server_addr, m_server_port, 
									cyiEncoder::GetYotsuyaSecurityCode("UserServerYotsuya"), id, szId, pszPass)==False)
	{
		return False;
	}
	m_myId=m_network->m_myinfo.id;
	strncpy(m_szMyId, szId, 16);
	CUserManager::GetInstance()->Initailize(m_myId, m_szMyId, m_szUserDir, m_server_addr, m_network->GetMyGoInfo()->base.udpPort);

	//CUserManager::GetInstance()->SetMyId(m_myId, m_szMyId);
	CUserManager::GetInstance()->AddDictionary(m_myId, m_szMyId, 0);

	wsprintf(m_szMyDir, "%s\\%s", m_szUserDir, szId);
	yiMakeDir(m_szMyDir);

	if (bIsNormal==True)
	{
		unsigned char flagz[USER_DATA_COUNT]={0, };
		Bool bUpdateFile=False;
		// 1. データファイルを読み込み、シーケンス番号を保持、
		// 2. サーバからbaseファイルを読み込み、比較する。
		// 3. 全て最新であれば、ネットワークをチェックし、そうでなければ、ファイルをダウンロードする。
		m_user = CUserManager::GetInstance();

		sBaseUserInfo* after	= 0;
		int sock = m_network->GetServerSock();
		int nHeaderCount;
		
		before=m_user->GetMyBaseInfo()?(sBaseUserInfo*)yitoheap(m_user->GetMyBaseInfo(), sizeof(sBaseUserInfo)):0;

		sBaseUserInfo base;
		int size;
		int packetId;
		if (m_network->Receive(0, &packetId, &base, &size)==False)
		{
			goto LOGIN_X;// False;
		}

		after  = &base;

		if (before==0)
		{
			for (int i=0; i<USER_DATA_COUNT; i++)
			{
				flagz[i]=1;
			}
			bUpdateFile=True;
		}
		else
		{
			char path[256];
			for (int i=0; i<USER_DATA_COUNT; i++)
			{
				if (i==1 ||
						i==4
						/*|| i==5*/
							|| i==6)
					continue;

				flagz[i]=after->sequenceNo[i]-before->sequenceNo[i];
				wsprintf(path, "%s/%d", m_szMyDir, i);

				if (flagz[i]!=0)
				{
					bUpdateFile=True;
				}
				else if (before->sequenceNo[i]==0)
				{
					if (yiIsExist(path, 0)==False)
					{
						flagz[i]=1;
						bUpdateFile=True;
					}
				}
			}
		}

		// TODO:本来ならば、失敗時に戻す処理を行わないといけない。

		if (before==0)
		{
			CUserManager::GetInstance()->SetMyBaseInfo(&base);
		}

		if (bUpdateFile==True)
		{
			if (m_network->SendToServer(eUSPID_REQ_FILES ,flagz, sizeof(flagz), False)==False)
			{
				goto LOGIN_X;
			}

			if (flagz[0]!=0)
			{
				sRegisterUserInfo info;
				if (m_network->Receive(0, &info, sizeof(info))==False)
				{
					goto LOGIN_X;
				}

				CUserManager::GetInstance()->SetRegisterInfo(&info);
			}

			// 友人
			if (flagz[2]!=0)
			{
				sUser* p = (sUser*)yialloci(after->sizes[2]);
				if (m_network->Receive(0, p, after->sizes[2])==False)
				{
					goto LOGIN_X;
				}

				CUserManager::GetInstance()->SetMyFriends(p);
			}

			// メッセージ
			if (flagz[3]!=0)
			{
				sMsg32* p = (sMsg32*)yialloci(after->sizes[3]);
				if (m_network->Receive(0, p, after->sizes[3])==False)
				{
					goto LOGIN_X;
				}

				CUserManager::GetInstance()->SetMyMsg32(p);
			}

			if (flagz[5]!=0)
			{
				sSimpleGoHeader* p = (sSimpleGoHeader*)yialloci(after->sizes[5]);
				if (m_network->Receive(0, p, after->sizes[5], 1000)==False)
				{
					//goto LOGIN_X;
				}

				int n = after->sizes[5]/sizeof(sSimpleGoHeader);

				for (int i=0; i<n; i++)
				{
					CUserManager::GetInstance()->AddWaitGoHeader(&p[i]);
				}
				CUserManager::GetInstance()->SaveWaitGoHeader();
				yifree(p);
			}

		}

		{
			int seq = before==0?0:before->sequenceNo[4];

			if (base.sequenceNo[4]>seq)
			{
				if (m_network->SendToServer(eUSPID_REQ_GO_HEADER, &seq, sizeof(int), False)==False)
				{
						goto LOGIN_X;
				}

				if (yirecv(sock, &nHeaderCount, sizeof(int), 0)!=sizeof(int))
				{
						goto LOGIN_X;
				}

				if (nHeaderCount)
				{
					int size = nHeaderCount<<4;
					sSimpleGoHeader* hd = (sSimpleGoHeader*)yialloci(size);
					
					if (yirecv(sock, hd, size, 0)!=size)
					{
						goto LOGIN_X;
					}

					CUserManager::GetInstance()->AppendGoHeader(0, hd, nHeaderCount);
				}
			}
		}

		if (before)
			yifree(before);

		CUserManager::GetInstance()->SetMyBaseInfo(&base);
	}
	


	if (m_network->CheckMyNetwork(0, 0)==False)
	{
		m_network->DisconnectFromServer(0);
		return False;
	}

	if (m_network->StartLoop()==False)
	{
		m_network->DisconnectFromServer(0);
		return False;
	}
	
	m_network->m_myGoInfo.reg=*CUserManager::GetInstance()->GetRegisterInfo(0);
	CUserManager::GetInstance()->GetMyScore(m_network->m_myGoInfo.score);
	m_network->m_myGoInfo.state=eGIUS_WAIT;
	
	strncpy(m_network->m_myGoInfo.base.szName, szId, 16);

	if (m_bIsFindRouterDone==True)
	{
		if (m_hUpnp)
		{
			xRegisterUpnpPort();
		}

		ConnectMyFriends();
	}

	return True;


LOGIN_X:

	if (before==0)
	{
		CUserManager::GetInstance()->SetMyBaseInfo(0);
	}
	if (before)
		yifree(before);
	m_network->DisconnectFromServer(0);

	return False;
}

// --------------------------------------
Bool CGoEditorApp::xRegisterUpnpPort()
{
	int port = m_network->GetMyGoInfo()->base.udpPort;

	int sock = yiudp(0, 0);
	int newport = yigetport(sock, 0);
	char szLocalAddr[32];
	char szExternalAddr[32];
	address_t local=yigetlocaladdress(szLocalAddr, 0);

	if (yiupnpGetExternalAddressx(m_hUpnp, szExternalAddr)==True)
	{
		address_t ex_addr = ::yiitol(szExternalAddr);
		m_myAbsAddr=ex_addr;

		// 保存
		int value=0;
		GetPropertyUserInt("upnp", value, -1);

		if (value!=-1)
		{
			yiupnpDeletePortMapx(m_hUpnp, value, False);
		}

		if (yiupnpAddPortMapx(m_hUpnp, szLocalAddr ,newport, False, "GoYotsuya Main Port")==True)
		{
			sGeneralParam param;
			param.param[0]=6;
			param.param[1]=m_network->GetMyInfo()->network.security;
			param.param[2]=ex_addr;
			param.param[3]=newport;
			yisendu(sock, &param, 32, m_server_addr, port);

			// 保存
			SetPropertyUserInt("upnp", newport);
			m_network->AddUdpMainSock(sock);

			return True;
		}

		yiupnpDestroyx(m_hUpnp);
		m_hUpnp=0;

		
	}

	return False;
}

// --------------------------------------
void CGoEditorApp::xOnMainUdpPacket(int id, void* p)
{
	sGeneralParam* param = (sGeneralParam*)p;

	switch(id)
	{
		// Request Connect.
		case 1:
		{
			//unsigned int id = param->param[0];
			address_t addr = param->param[0];
			int port = param->param[1];
			sypnUserInfo remote;

			if (m_network->Connect(addr, port, &remote)==False)
			{
				return ;
			}
			break;
		}
		default:
			break;
	}
}

// --------------------------------------
Bool CGoEditorApp::xGetRemoteAbsAddr(unsigned int userId, address_t* addr, int* port)
{
	sGeneralParam param;
	param.param[0]=7;
	param.param[1]=userId;

	int sock = yiudp(0, 0);
	yisendu(sock, &param, GP_SIZE(2), m_server_addr, m_network->GetMyGoInfo()->base.udpPort);

	int timeout=1000;
	int n=0;
	if ((n=yirecvu(sock, &param, GP_SIZE(4), &timeout, 0, 0))<0)
	{
		yiclose(sock);
		return 0;
	}

	if (n==4)
	{
		// not login
		if (param.param[0]==-2)
			return False;

		// invalid user.
		yiclose(sock);
		return False;
	}

	yiclose(sock);

	if (port)
	{
		*port=param.param[2];
	}

	if (m_myAbsAddr==param.param[1])
	{
		return param.param[3];
	}


	return param.param[1];
}


// --------------------------------
static void xOnConnectMyFriend(void* param)
{
	CGoEditorApp* p = (CGoEditorApp*)param;
	p->OnConnectFriends();
}

// --------------------------------
void CGoEditorApp::OnConnectFriends()
{
	int n=CUserManager::GetInstance()->GetMyFriendsCount();
	int cnt=0;
	if (n==0)
	{
		_endthread();
		return ;
	}

	sUser* users = CUserManager::GetInstance()->GetMyFriends();

	for (int i=0; i<n; i++)
	{
		if (users[i].id==0)
		{
			continue;
		}

		if ((users[i].id&0xFF)!=0)
		{
			sGeneralParam param ={ 7, users[i].id, 0};
			int sock = yiudp(0, 0);
			if (yisendu(sock, &param, 32, m_server_addr, m_network->GetMyGoInfo()->base.udpPort)==32)
			{
				int timeout=500;
				address_t addr=0;
				int port=0;
				int res = yirecvu(sock, &param, GP_SIZE(4), &timeout, &addr, &port);

				if (res==4)
				{
					// invalid user, no login.
					if (param.param[0]==-1 ||
							param.param[0]==-2)
					{
						continue;
					}
				}
				else if (res==GP_SIZE(4))
				{
					// has abs ip.
					if (param.param[1]!=0)
					{
						address_t remote = param.param[1];
						if (param.param[1]==m_myAbsAddr)
						{
							remote=param.param[3];
						}

						param.param[0]=1;
						yisendu(sock, &param, 32, remote, param.param[2]);

						sypnUserInfo info;
						
						if (m_network->AcceptFrom(sock, &info)==True)
						{
							CUserManager::GetInstance()->AddUserHandle((Handle)m_network->IndexToUser(info.index));
							cnt++;
						}
						continue;
					}
				}
			}
		}

		if (m_hUpnp &&
				m_myAbsAddr)
		{
			sGeneralParam param = {users[i].id};
			m_network->SendToServer(eUSPID_REQ_CONNECT_UDP_ME, &param, GP_SIZE(1), False);
			continue;
		}

		// no abs ip.
		sypnUserInfo info;
		if (m_network->Connect(users[i].id, 0, 0, 0, &info)<0)
		{
			continue;
		}

		CUserManager::GetInstance()->AddUserHandle((Handle)m_network->IndexToUser(info.index));
		cnt++;
	}

	if (cnt)
	{
		PostCommand(eGCID_UPDATE_FRIEND_VIEW, 0, 0, 0);
	}
}



// --------------------------------
sSimpleGoData* CGoEditorApp::GetGoData(ugoid_t& id, int* size)
{
	int xsize=0;
	sSimpleGoData* data = CUserManager::GetInstance()->GetGoData(id, xsize);

	if (size)
		*size=xsize;
	return data;
/*
	sGeneralParam param={2, id.b_id, id.w_id, id.day, id.time, 0, };
	int sock = yiudp(0, 0);

	yisendu(sock, &param, 32, m_server_addr, m_network->m_myGoInfo.base.udpPort);

	yirecvu(sock, &param, 4, 0, 0, 0);

	if (param.param[0]==-1)
	{
		*size=0;
		return 0;
	}

	*size=param.param[0];
	sSimpleGoData* p = (sSimpleGoData*)yialloc(param.param[0]);
	
	if (param.param[0]<=512)
	{
		yirecvu(sock, p, param.param[0], 0, 0, 0);
		return p;
	}

	int n = param.param[0]>>9;
	int remain = param.param[0]&0x1FF;

	int i=0;
	char* pIndex=(char*)p;
	for (i=0; i<n; i++)
	{
		yirecvu(sock, &pIndex[i<<9], 512, 0, 0, 0);
	}

	if (remain)
	{
		yirecvu(sock, &pIndex[i<<9], remain, 0, 0, 0);
	}
	
	yiclose(sock);
	return p;*/
}

// --------------------------------
void CGoEditorApp::ConnectMyFriends()
{
	_beginthread(xOnConnectMyFriend, 0, this);
}

#include "yigRegisterDialog.h"
// --------------------------------
unsigned int CGoEditorApp::xRegisterUser(char* szUserName, char* szPassword)
{
	// まず、新たなコネクションを張る。
	if (m_network==0)
	{
		m_network = new cypnGoNetwork();
	}

	socket_t sock = m_network->ConnectToServer(m_server_addr, m_server_port, 0);

	if (sock==0)
	{
		return 0;
	}

	m_network->Send(sock, eUSPID_NO_LOGIN,0, 0);

	cyigRegisterDialog regster;	

	unsigned int userId;
	float fontSize=20.0*m_goban->GetScale();
	fontSize=fontSize<12.0?12.0:fontSize;

	if ((userId=regster.RegisterUser(m_main, sock, &Font(L"Meiryo", fontSize)))==0)
	{
		return 0;
	}

	regster.GetValue(szUserName, szPassword);
	return userId;
}

// --------------------------------
Bool CGoEditorApp::StopTimer(unsigned int goid)
{
	xTRACE("CGoEditorApp::StopTimer(%d)", goid);

	sGoPlayInfo* s = (sGoPlayInfo*)m_goban->GetExtraData(goid);
	sGoRemainTime* remain = m_goban->ImBlack(goid)==True?&s->black:&s->white;
	
	if (m_goban->IsCurrentBlack(goid)==m_goban->ImBlack(goid))
	{
		unsigned int used = cyiTime::CurrentmSec(remain->startTime);
		m_goban->StopTimer(goid, used);
		xTRACE("CGoEditorApp::StopTimer(%d) .............. send eGNPMSG_STOP_TIMER", goid);
		m_network->SendToUserEx((HandleUser)s->hUser, eGNPMSG_STOP_TIMER, &used, sizeof(used));
	}
	else
	{
		unsigned int used=0;
		//m_goban->StopTimer(goid, used, m_goban->ImBlack(goid));
		xTRACE("CGoEditorApp::StopTimer(%d) .............. send eGNPMSG_STOP_TIMER Only", goid);
		m_network->SendToUserEx((HandleUser)s->hUser, eGNPMSG_STOP_TIMER, &used, sizeof(used));

		return False;
	}

	return True;
}

// --------------------------------
Bool CGoEditorApp::SaveGoDataEx(unsigned int goid)
{
	unsigned int day=0;
	unsigned int time = cyiTime::GetLocalTime(day);
	char szNewFileName[256];
	char path[256];

	sGoHeaderEx hd;
	m_goban->GetHeaderEx(goid, &hd);

	if (CUserManager::GetInstance()->GetMyId()==0)
	{
		wsprintf(szNewFileName, "%d%02d%02d%02d%02d%02d.sgf", DAY_YEAR(day), DAY_MON(day), DAY_DAY(day),
													xHOUR(time), xMINUTE(time), xSECOND(time));

		if (m_goban->Save(path)==False)
		{
			return False;
		}
		//wsprintf(path, "%s\\data\\other\\%s", m_szUserDir, szNewFileName);
		//if (m_goban->Save(path)==False)
		//	return False;

		//wsprintf(path, "%s\\data\\%s.map", m_szMyDir, szNewFileName);
		//
		//return yiFileWrite(path, hd.map, 0, sizeof(hd.map));

		return SaveGoData(1, &hd.header, szNewFileName, hd.map, hd.n);
	}

	ugoid_t* id = m_goban->GetUniversalGoid(goid);
	wsprintf(szNewFileName, "%08x%08x%08x%08x", id->b_id, id->w_id, id->day, id->time);
	wsprintf(path, "%s\\data\\%s.sgf", m_szMyDir, szNewFileName);
	m_goban->Save(path);
	
	wsprintf(path, "%s\\data\\%s.map", m_szMyDir, szNewFileName);

	if (yiFileWrite(path, hd.map, 0, sizeof(hd.map))==False)
	{
		return False;
	}

	int size;
	sSimpleGoData* data = m_goban->ExportSimpleGoData(goid, &size);

	if (data==0)
	{
		return False;
	}
	if (m_goban->ImBlack(goid)==True)
	{
		m_network->SendToServer(eUSPID_UP_GODATA, data, size, False); 
	}
	else
	{
		m_network->SendToServer(eUSPID_UP_GODATA, data, sizeof(sSimpleGoHeader), False); 
	}

	int win = 1;

	win = m_goban->ImWin(goid);

	if (win==1)
	{
		CUserManager::GetInstance()->GetMyBaseInfo()->point++;
		CUserManager::GetInstance()->GetMyBaseInfo()->wins++;
	}
	else if (win<0)
	{
		CUserManager::GetInstance()->GetMyBaseInfo()->loses++;
	}
	else if (win==0)
	{
		CUserManager::GetInstance()->GetMyBaseInfo()->draws++;
	}

	CUserManager::GetInstance()->GetMyBaseInfo()->sequenceNo[4]++;

	CUserManager::GetInstance()->SaveMyBaseInfo();

	int n = CUserManager::GetInstance()->AppendGoHeader(0, (sSimpleGoHeader*)data, 1);
	//m_goban->UpdateMyView(0, 0, 0, 0xFF);

	cglsMyView* myView = (cglsMyView*)m_goban->GetMyView(0);
	if (myView)
	{
		myView->AddGo(n, (sSimpleGoHeader*)data);
		myView->UpdateMyView();
	}
	yifree(data);

	return True;
}


// --------------------------------
Bool CGoEditorApp::SaveGoData(int index, sGoHeader* header, char* pszFileName, map_t map, int lastOrder)
{
	char path[256];

	if (index==1)
	{
		wsprintf(path, "%s\\data\\other\\.list", m_szUserDir);
	}
	else
	{
		wsprintf(path, "%s\\data\\played\\.list", m_szMyDir);
	}

	unsigned int hash = IdToInteger(pszFileName);
	
	if (m_goHeaders==0)
	{
		int size=0;
		m_goHeaders = (sSimpleYgfHeader *)yiFileLoadEx(path, &size);
		m_goHeaderSize = size;
	}

	if (m_goHeaders)
	{
		int size=m_goHeaderSize;
		int n = size/sizeof(sSimpleYgfHeader);

		for (int i=0; i<n; i++)
		{
			if (m_goHeaders[i].goid[0]==hash)
			{
				m_goHeaders[i].condition=header->condition;
				m_goHeaders[i].condition2=header->condition;
				m_goHeaders[i].flag=header->flag32;
				m_goHeaders[i].day = header->playDate;

				if (header->pszBlackName)
					strncpy(m_goHeaders[i].szBlackName, header->pszBlackName, 32);

				if (header->pszWhiteName)
					strncpy(m_goHeaders[i].szWhiteName, header->pszWhiteName, 32);

				m_goHeaders[i].last_order=lastOrder;

				//yiFileUnloadEx(m_goHeaders);
				// 上書きする。
				if (yiFileWriteEx(path, i*sizeof(sSimpleYgfHeader), &m_goHeaders[i], sizeof(sSimpleYgfHeader), True)==False)
					return False;

					cglsMyView* myView = (cglsMyView*)m_goban->GetMyView(0);
					if (myView)
					{
						myView->UpdateGo(index, i, &m_goHeaders[i]);
					}
				return True;
			}
		}
	}

	sSimpleYgfHeader*s = (sSimpleYgfHeader*)yialloci(sizeof(sSimpleYgfHeader));
	s->goid[0]=hash;

	if (::yiIsExist(path, 0)==False)
	{
		s->id=1;
	}
	else
		s->id=(yiFileSize(path)/sizeof(*s))+1;//m_listOtherGoFile->count();

	s->condition=header->condition;
	s->condition2 = header->condition2;
	s->flag = header->flag32;
	s->day	= header->playDate;

	if (header->pszBlackName)
		strncpy(s->szBlackName, header->pszBlackName, 32);

	if (header->pszWhiteName)
		strncpy(s->szWhiteName, header->pszWhiteName, 32);

	s->last_order=lastOrder;
	//s->map=hd.map;
	memcpy(s->map, map, sizeof(s->map));
	strcpy(s->szFileName, pszFileName);

	//yifree(hd.header.pszBlackName);
	//yifree(hd.header.pszEventName);
	//yifree(hd.header.pszWhiteName);

	Bool dir;
	if (::yiIsExist(path, &dir)==False)
	{
		if (yiFileCreateEx(path, -1, s, sizeof(*s), True)==False)
			return False;
	}
	else
	{
		if (yiFileWriteEx(path, -1, s, sizeof(*s), True)==False)
			return False;
	}

	if (m_goHeaders)
	{
		yiFileUnLoadEx(m_goHeaders);
	}
	
	m_goHeaders = (sSimpleYgfHeader *)yiFileLoadEx(path, &m_goHeaderSize);
	

	cglsMyView* myView = (cglsMyView*)m_goban->GetMyView(0);
	if (myView)
	{
		myView->AddGo(index, s);
	}
	return True;
}


// --------------------------------
Bool CGoEditorApp::Msg32ToMessage(sMsg32* msg32, int index, sMsg* msg, wchar_t* buffer)
{
	buffer[0]=0;
	msg->msgId=index;
	wchar_t xbuff[1024];
	wchar_t wszUserName[16];

	::yiToUnicode(wszUserName, msg32->szUserName, 0);
	switch(msg32->type)
	{
		case eWMT_ADD_FRIEND:
		{
			wsprintfW(xbuff, L"%sさんが友人の登録をリクエストしました。\r\n登録しますか？", wszUserName);
			MakeMessage(msg, (msg32->type<<16)|2, msg32->remoteId, msg32->szUserName, xbuff);
			msg->day=msg32->day;
			msg->time=msg32->time;

			wcscpy(buffer, L"友人登録リクエスト");
			break;
		}
		case eWMT_ADD_FRIEND_OK:
		{
			sGeneralParam param= {m_network->m_myinfo.id, True, msg32->remoteId, 0, };
			strncpy((char*)&param.param[3], msg32->szUserName, 16);//msg->szUserName, 0);
			m_network->SendToServer(eUSPID_REQ_ADD_FRIEND, &param, GP_SIZE(7), False);
			return False;
		}
		case eWMT_ADD_FRIEND_NG:
		{
			wsprintfW(xbuff, L"%sから友人登録をリクエスを断られました。", wszUserName);
			MakeMessage(msg, (msg32->type<<16)|2, msg32->remoteId, msg32->szUserName, xbuff);
			msg->day=msg32->day;
			msg->time=msg32->time;
			wcscpy(buffer, L"友人登録リクエスト終了");
			break;
		}
		default:
			return False;
	}

	return True;
}

// --------------------------------
void CGoEditorApp::MakeMessage(sMsg* msg, int type, unsigned int remoteId, const char* pszUserName, const wchar_t* txt)
{
	msg->time = cyiTime::GetLocalTime(msg->day);
	msg->type=type;
	msg->remoteId=remoteId;
	msg->msgId=0xFF;

	if (pszUserName)
		yiToUnicode(msg->szUserName, pszUserName, 0);
	if (msg!=0)
		wcscpy(msg->data, txt);
}



//enum eCacheType {
//	eCT_IMAGE,
//	eCT_GO,
//
//};

// --------------------------------
//Bool CGoEditorApp::xAddMyFriend(sRegisterUserInfo* user)
//{
//	char path[256];
//	AddDictionary(user->id, user->szUserName, 0);
//	AddCached(user);
//
//	wsprintf(path, "%s\\2", m_szMyDir);
//
//	int size;
//	sFriendList* pUser = (sFriendList*)yiFileLoadEx(path, &size);
//
//	if (pUser==0)
//	{
//		int size = sizeof(sFriendList)+29*sizeof(sUser);
//		sFriendList* pFriend = (sFriendList*)yialloci(size);
//
//		pFriend->cnt=(1<<16)|30;
//		pFriend->seq_no=1;
//		pFriend->user[0].id=user->id;
//		strncpy(pFriend->user[0].szUserName, user->szUserName, 16);
//
//		if (yiFileCreateEx(path, 0, pFriend, size, True)==False)
//			return False;
//
//		return True;
//	}
//
//	int index = pUser->cnt>>16;
//	pUser->user[index].id=user->id;
//	strncpy(pUser->user[index].szUserName, user->szUserName, 16);
//	pUser->cnt=(++index<<16)|(pUser->cnt&0xFFFF);
//
//	if (yiFileWriteEx(path, 0, pUser, size, True)==False)
//	{
//		return False;
//	}
//
//	return True;
//}
#include "yigUserDialog.h"
#include "yigNoIdDialog.h"
// --------------------------------
Bool CGoEditorApp::OnReqPlayDialog(cGlassInterface* from, unsigned int param1, unsigned int param2)
{
	float fontSize=20.0*m_goban->GetScale();
	fontSize=fontSize<12.0?12.0:fontSize;
	unsigned int condition1, condition2;
	cyigUserDialog::xDefaultCondition(0, condition1, condition2);
	HandleUser hUser=0;


	if (m_network->m_myinfo.id==0)
	{
		cyigNoIdDialog dlg;
		unsigned int n=param1;
		char szRemoteUser[16];

		//if (dlg.ConnectRoom(m_main, &n, szRemoteUser, &Font(L"Meiryo UI", fontSize))==False)
		//{
		//	return False;
		//}
		sypnUserInfo info;
		if (m_network->Connect(0, 0, param1, (const char*)param2, &info)==-1)
		{
			cglsMessageBox msg;
			msg.Show(m_main, L"相手の部屋に接続できません。\r\n部屋番号や部屋のパスワードを確認してください", L"エラー", MB_OK, &Font(L"Meiryo UI", 12.0));
			return False;
		}

		wsprintf(szRemoteUser, "%d番部屋", param1);
		cyigUserDialog user;
		if (user.Show(m_main, 0, szRemoteUser, condition1, condition2, &Font(L"Meiryo UI", fontSize))==False)
		{
			return False;
		}
		user.GetValue(condition1, condition2);
		hUser = m_network->FindUserByRommNo(n);
	}
	else
	{
		hUser = (HandleUser)CUserManager::GetInstance()->GetUserHandle(param1);

		if (hUser==0)
			return False;

		sGoUserInfo* info = (sGoUserInfo*)hUser;

		if (info->goid!=0)
		{
			return False;
		}

		cyigUserDialog user;
		if (user.Show(m_main, 0, info->szName, condition1, condition2, &Font(L"Meiryo UI", fontSize))==False)
		{
			return False;
		}
		user.GetValue(condition1, condition2);
	}

	if (hUser!=0)
	{

		xTRACE("Send requestPlay..... condition:%x, condition2:%x", condition1, condition2);
		m_network->SendRequestPlay(hUser, condition1, condition2);
	}
	return True;
}

// --------------------------------
unsigned int CGoEditorApp::GetUserId(const char* pszUserName, Bool bSearchNetwork)
{
	sGeneralParam param={0, };
	unsigned int hash = IdToInteger(pszUserName);//yihf_encoded_additional(pszUserName, 31);
TOP:
	if (m_dictionary.user==0)
	{
		char path[256];
		int size=0;
		wsprintf(path, "%s\\.dictionary", m_szUserDir);
		m_dictionary.user=(sUserDictionary*)yiFileLoadEx(path, &size);
		m_dictionary.n = size/sizeof(sUserDictionary);
	}

	if (m_dictionary.n==0)
		goto NOT_FOUND;

	for (int i=0; i<m_dictionary.n; i++)
	{
		if ((m_dictionary.user[i].id&0xFFFFFF00)==hash)
		{
			if (strncmp(m_dictionary.user[i].szName, pszUserName, 16)==0)
			{
				return m_dictionary.user[i].id;
			}
		}
	}

NOT_FOUND:

	if (bSearchNetwork==False)
	{
		return 0;
	}

	param.param[0]=IdToInteger(pszUserName);
	strncpy((char*)&param.param[1], pszUserName, 16);
	if (m_network->SendToServer(eUSPID_REQ_QUERY_USER, &param, GP_SIZE(5), True)==True)
	{
		int id;
		int size;
		sGeneralParam* ret = (sGeneralParam*)m_network->GetFromQueue(0, &id, &size);

		if (ret==0)
			return False;
		if (ret->param[0]==0)
		{
			return False;
		}

		if (CUserManager::GetInstance()->AddDictionary(ret->param[0], (const char*)&ret->param[1], 0x00000100)==False)
		{
			return False;
		}
		
		goto TOP;
	}
	return 0;
}

static void xOnRequestDataFromx(void* p)
{
	sGeneralParam* param = (sGeneralParam*)p;
	CGoEditorApp* pClass= (CGoEditorApp*)param->param[5];

	pClass->OnRequestDataFrom((HandleUser)param->param[0], param->param[1], param->param[4], param->param[2], param->param[3]);

	yifree(param);
	_endthread();
}


static void xOnSendDataTo(void* p)
{
	sGeneralParam* param = (sGeneralParam*)p;
	CGoEditorApp* pClass= (CGoEditorApp*)param->param[4];

	pClass->OnSendDataToUser((HandleUser)param->param[0], param->param[1], param->param[6], param->param[2], param->param[3], param->param[5]);

	yifree(param);
	_endthread();
}

// --------------------------------
Bool CGoEditorApp::HolePunching(int sock, address_t addr, int port)
{
	char buffer;//[16];

	buffer=0;
	int nRetry=5;
	Bool bIsSkipReceive=False;
	while(nRetry)
	{
		int timeout=100;

		if (yisendu(sock, &buffer, 1, addr ,port)==False)
		{
			xTRACE("CGoEditorApp::HolePunching() send error!\n");
			return False;
		}

		if (yirecvu(sock, &buffer, 1, &timeout, &addr, &port)!=1)
		{
			nRetry--;
			continue;
		}


		if (buffer==(char)0xFF) // 既に終了コードを受け取っていれば？
		{
			bIsSkipReceive=True;
			xTRACE("CGoEditorApp::HolePunching() accept end code\n");

		}
		
		buffer=0xFF;
		xTRACE("CGoEditorApp::HolePunching() send end code!\n");
		if (yisendu(sock, &buffer, 1, addr ,port)==False)
		{
			xTRACE("CGoEditorApp::HolePunching() send err!\n");
			return False;
		}
		break;
	}

	if (nRetry==0)
	{
		xTRACE("CGoEditorApp::HolePunching() over retry cnt!\n");

		return False;
	}

	if (bIsSkipReceive==False)
	{
		// 合わせて0.5秒（しかし、ここまで来たら大丈夫)
		int timeout=500;
		for (int i=0; i<5; i++)
		{
			if (yirecvu(sock, &buffer, 1, &timeout, &addr, &port)!=1)
			{
				return False;
			}
			if (buffer==(char)0xFF)
			{
				xTRACE("CGoEditorApp::HolePunching() accept end code\n");
				break;
			}
			xTRACE("CGoEditorApp::HolePunching() received %x\n", buffer&0xFF);
		}
	}

	return True;
}

// --------------------------------
void CGoEditorApp::OnSendDataToUser(HandleUser hUser, int id, int sock, address_t addr, int port, unsigned int seq)
{
	void* p=0;
	int size=0;

	// picture.
	if (id==1)
	{
		CUserManager::GetInstance()->GetPicture(0, seq);
		p = CUserManager::GetInstance()->GetPictureFile(0, &size);


		if (p==0 ||
				size==0)
		{
			xTRACE("CGoEditorApp::OnSendDataToUser() to:%x, id:%d, addr:%u.%u.%u.%u, port:%d GetPictureFile Error!", ((sGoUserInfo*)hUser)->id,id, 
						(addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xFF, addr&0xFF, port);
			return ;
		}
	}
	else if (id==2) // 棋譜リスト
	{
		sSimpleGoHeader* px = CUserManager::GetInstance()->GetGoHeader(0, &size);

		if (px==0 ||
				size==0)
		{
			xTRACE("CGoEditorApp::OnSendDataToUser() to:%x, id:%d, addr:%u.%u.%u.%u, port:%d GetGoHeader null!", ((sGoUserInfo*)hUser)->id,id, 
						(addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xFF, addr&0xFF, port);
			return ;
		}

		if (seq>size)
		{
			return ;
		}

		int last = size/sizeof(sSimpleGoHeader);
		int n = (size-(int)seq)/sizeof(sSimpleGoHeader);

		//if (n<0)
		//{
		//	return ;
		//}

		sSimpleGoHeader* xx = (sSimpleGoHeader*)yialloc(n*sizeof(sSimpleGoHeader));

		for (int i=0; i<n; i++)
		{
			memcpy(&xx[i], &px[last-1-i], sizeof(sSimpleGoHeader));
		}

		p=xx;
		size=(sizeof(sSimpleGoHeader)*n);

	}
	else if (id==3) // 棋譜
	{
		return ;
	}

	if (p==0)
	{
		xTRACE("CGoEditorApp::OnSendDataToUser() data is null");
		return ;
	}
	
	xTRACE("CGoEditorApp::OnSendDataToUser() to:%x, id:%d, addr:%u.%u.%u.%u, port:%d", ((sGoUserInfo*)hUser)->id,id, 
				(addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xFF, addr&0xFF, port);

	if (HolePunching(sock, addr, port)==False)
	{
		xTRACE("CGoEditorApp::OnSendDataToUser() Holepunching failure! ");
		yiclose(sock);

		if (id==2)
			yifree(p);
		return ;
	}
	xTRACE("CGoEditorApp::OnSendDataToUser() Holepunching success! ");

	if (yisendu(sock, &size, 4, addr, port)!=4)
	{
		xTRACE("CGoEditorApp::OnSendDataToUser() to:%x, id:%d, addr:%x, port:%d send size error (%d)", ((sGoUserInfo*)hUser)->id,id, addr, port, size);
		yiclose(sock);
		if (id==2)
			yifree(p);

		return ;
	}

	if (yisendu(sock, &seq, 4, addr, port)!=4)
	{
		xTRACE("CGoEditorApp::OnSendDataToUser() to:%x, id:%d, addr:%x, port:%d send seq error (%d)", ((sGoUserInfo*)hUser)->id,id, addr, port, size);
		yiclose(sock);
		if (id==2)
			yifree(p);

		return ;
	}

	char* pIndex=(char*)p;
	int   n = (size>>9);
	int   remain = (size&0x01FF);
	char buffer[16];
	for (int i=0; i<n; i++)
	{
		int timeout=500;
		if (yisendu(sock, pIndex, 512, addr, port)!=512)
		{
			yiclose(sock);

			if (id==2)
				yifree(p);

			return ;
		}
		if (yirecvu(sock, buffer, 1, &timeout, &addr, &port)!=1)
		{
			yiclose(sock);
			if (id==2)
				yifree(p);

			return ;
		}
		pIndex=&pIndex[512];
	}

	if (remain)
	{
		if (yisendu(sock, pIndex, remain, addr, port)!=remain)
		{
			yiclose(sock);
			if (id==2)
				yifree(p);

			return ;
		}
	}
	
	yiclose(sock);
	if (id==2)
		yifree(p);

	xTRACE("CGoEditorApp::OnSendDataToUser() to:%x, id:%d, addr:%x, port:%d send success!", ((sGoUserInfo*)hUser)->id,id, addr, port);

	return ;
}




// --------------------------------
void CGoEditorApp::OnRequestDataFrom(HandleUser hUser, int id, int sock, address_t _addr, int _port)
{
	char buffer[16];
	int timeout=1000;
	address_t addr=0;
	int port=0;
	unsigned int seq=0;

	buffer[0]=0;

	if (HolePunching(sock, _addr, _port)==False)
	{
		xTRACE("CGoEditorApp::OnSendDataToUser() Holepunching failure! ");
		yiclose(sock);
		return ;
	}
	xTRACE("CGoEditorApp::OnRequestDataFrom() Holepunching success! ");

	addr=_addr;
	port=_port;

	unsigned int size;
	if (yirecvu(sock, &size, 4, &timeout, &addr, &port)!=4)
	{
		yiclose(sock);
		xTRACE("CGoEditorApp::OnRequestDataFrom(from:%x, id:%d) recv size timeout ! (%u.%u.%u.%u:%d)", ((sGoUserInfo*)hUser)->id, id, (addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xFF, (addr&0xFF), port);
		return ;
	}

	// データなし。
	if (size==0)
	{
		yiclose(sock);
		xTRACE("CGoEditorApp::OnRequestDataFrom(from:%x, id:%d) recved no data (%u.%u.%u.%u:%d)", ((sGoUserInfo*)hUser)->id, id, (addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xFF, (addr&0xFF), port);
		return ;
	}
	xTRACE("CGoEditorApp::OnRequestDataFrom(from:%x, id:%d) start receive packet.... %d byte (%u.%u.%u.%u:%d)", ((sGoUserInfo*)hUser)->id, id, size, (addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xFF, (addr&0xFF), port);


	if (yirecvu(sock, &seq, 4, &timeout, &addr, &port)!=4)
	{
		yiclose(sock);
		xTRACE("CGoEditorApp::OnRequestDataFrom(from:%x, id:%d) recv seq timeout ! (%u.%u.%u.%u:%d)", ((sGoUserInfo*)hUser)->id, id, (addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xFF, (addr&0xFF), port);
		return ;
	}

	char* pBuffer = (char*)yialloc(size);
	int   n = (size>>9);
	int   remain = (size&0x01FF);
	char* pIndex=pBuffer;

	for (int i=0; i<n; i++)
	{
		timeout=500;
		if (yirecvu(sock, pIndex, 512, &timeout, &addr, &port)!=512)
		{
			yiclose(sock);
			xTRACE("CGoEditorApp::OnRequestDataFrom(from:%x, id:%d) recv packet timeout ! (%u.%u.%u.%u:%d)", ((sGoUserInfo*)hUser)->id, id, (addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xFF, (addr&0xFF), port);
			return ;
		}
		pIndex=&pIndex[512];

		if (yisendu(sock, pBuffer, 1, addr, port)!=1)
		{
			yiclose(sock);
			return ;
		}
	}

	if (remain)
	{
		yirecvu(sock, pIndex, remain, &timeout, &addr, &port);
	}

	yiclose(sock);

	unsigned int* xxxx = (unsigned int*)yialloc(5<<2);
	xxxx[0]=(unsigned int)hUser;
	xxxx[1]=size;
	xxxx[2]=id;
	xxxx[3]=(unsigned int)pBuffer;
	xxxx[4]=seq;
	xTRACE("CGoEditorApp::OnRequestDataFrom(from:%x, id:%d) recved.... %d byte success! (%u.%u.%u.%u:%d), %x", ((sGoUserInfo*)hUser)->id, id, size, (addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xFF, (addr&0xFF), port, xxxx);
	PostCommand(eGCID_DATA_RECEIVED, 0, (unsigned int)((((unsigned int)xxxx)>>16)&0xFFFF), (unsigned int)((unsigned int)xxxx&0xFFFF));

}

// --------------------------------
void CGoEditorApp::OnDataReceived(HandleUser hUser, int id, void* p, int size, unsigned int _seq)
{
	xTRACE("CGoEditorApp::OnDataReceived() %d, size:%d", id, size);
	if (id==1)
	{
		sGoUserInfo* info = (sGoUserInfo*)hUser;
		CUserManager::GetInstance()->SetPictureFile(info->id, p, size, _seq);

		m_goban->UpdateMyView(1, info->id, 0, eMFVU_PICTURE);
	}
	else if (id==2)
	{
		CUserManager::GetInstance()->AppendGoHeader(((sGoUserInfo*)hUser)->id, (sSimpleGoHeader*)p, size/sizeof(sSimpleGoHeader));
		m_goban->UpdateMyView(1, ((sGoUserInfo*)hUser)->id, 0, eMFVU_GODATA);
	}
}

// --------------------------------
Bool CGoEditorApp::SendDataToUser(HandleUser hUser, int id, address_t addr, int port, unsigned int _seq)
{
	int sock=0;
	sGeneralParam param;
	sGeneralParam reply={0, };
	Bool bIsNoData=False;

	xTRACE("CGoEditorApp::SendDataToUser() from:%x, id:%d, addr:%x, port:%d", ((sGoUserInfo*)hUser)->id,id, addr, port);

	if (id==1) // picture
	{
		unsigned int seq=0;
		Bitmap* bmp = CUserManager::GetInstance()->GetPicture(0, seq);

		if (bmp==0)
		{
			xTRACE("CGoEditorApp::SendDataToUser() from:%x, id:%d, addr:%x, port:%d no picture", ((sGoUserInfo*)hUser)->id,id, addr, port);
			bIsNoData=True;
		}

		if (_seq>=seq)
		{
			xTRACE("CGoEditorApp::SendDataToUser() from:%x, id:%d, addr:%x, port:%d sequence no is new! (%d, %d)", ((sGoUserInfo*)hUser)->id,id, addr, port, seq, _seq);
			bIsNoData=True;
		}
	}
	else if (id==2)
	{
		int size;//=_seq;
		CUserManager::GetInstance()->GetGoHeader(0, &size);

		if (size==0 ||
				size==_seq)
		{
			bIsNoData=True;
		}
	}

	if (bIsNoData==True)
	{
		reply.param[0]=0xFFFFFFFF;
	}
	else
	{
		sock = m_network->CreateUdpSock(reply.param[0], (int&)reply.param[1], reply.param[2], (int&)reply.param[3]);
	}

	xTRACE("send eGNPMSG_RPY_DATA (%x, %x, %x, %x)", reply.param[0], reply.param[1], reply.param[2], reply.param[3]);
	if (m_network->SendToUserEx(hUser, eGNPMSG_RPY_DATA, &reply, GP_SIZE(4))==False)
	{
		yiclose(sock);
		return False;
	}

	if (sock==0)
	{
		return False;
	}

	if (bIsNoData==True)
	{
		yiclose(sock);
		return False;
	}

	param.param[0]=(unsigned int)hUser;
	param.param[1]=id;
	param.param[2]=addr;
	param.param[3]=port;
	param.param[4]=(unsigned int)this;
	param.param[5]=_seq;
	param.param[6]=sock;
	
	_beginthread(xOnSendDataTo, 0, yitoheap(&param, GP_SIZE(7)));
	

	return True;
}

// --------------------------------
Bool CGoEditorApp::xOnRequestDataFrom(HandleUser hUser, sGeneralParam* s)
{
	xTRACE("CGoEditorApp::xOnRequestDataFrom(result:%x", s->param[0]);
	yiAnyValue val;

	if (m_qWait.deq((yiAnyValue&)val)==False)
	{
		// ありえない？
		return False;
	}

	sGeneralParam* param = (sGeneralParam*)val;

	if (hUser!=(HandleUser)param->param[6])
	{
		// どうする？
		return False;
	}

	//sGeneralParam s;
	//int n;
	//if (m_network->WaitMessage(((sGoUserInfo*)hUser)->sock, eGNPMSG_RPY_DATA, 500, &s, &n)==False)
	//{
	//	xTRACE("CGoEditorApp::RequestDataFrom(id:%d, seq:%d) WaitMessage Failure!", id, seq);
	//	return False;
	//}
	//xTRACE("CGoEditorApp::RequestDataFrom(id:%d, seq:%d) WaitMessage success!(%x, %x, %x, %x)", id, seq, s.param[0], s.param[1], s.param[2], s.param[3]);

	// no data.
	if (s->param[0]==0 ||
			s->param[0]==0xFFFFFFFF)
	{
		xTRACE("CGoEditorApp::xOnRequestDataFrom(id:%d, seq:%d) no data!", param->param[0], param->param[5]);
		yiclose(param->param[7]);
		yifree(param);

		return False;
	}

	if (s->param[0]==param->param[1])
	{
		param->param[2]=s->param[2];
		param->param[3]=s->param[3];
	}
	else
	{
		param->param[2]=s->param[0];
		param->param[3]=s->param[1];
	}

	int id = param->param[0];
	param->param[0]=(unsigned int)hUser;
	param->param[1]=id;
	param->param[4]=param->param[7];
	param->param[5]=(unsigned int)this;

	_beginthread(xOnRequestDataFromx, 0, yitoheap(param, GP_SIZE(6)));

	yifree(param);

	return True;
}

// --------------------------------
Bool CGoEditorApp::RequestDataFrom(HandleUser hUser, int id, unsigned int seq)
{
	sGeneralParam param = {0, };

	int sock = m_network->CreateUdpSock(param.param[1], (int&)param.param[2], param.param[3], (int&)param.param[4]);
	
	if (sock==0)
		return False;

	param.param[0]=id;
	/*param.param[1]=addr;
	param.param[2]=port;
	param.param[3]=yigetlocaladdress(0, 0);
	param.param[4]=yigetport(sock, 0);*/
	param.param[5]=seq;

	if (m_network->SendToUserEx(hUser, eGNPMSG_REQ_DATA, &param, GP_SIZE(6))==False)
	{
		return False;
	}

	param.param[6]=(unsigned int)hUser;
	param.param[7]=sock;
	m_qWait.enq((yiAnyValue)yitoheap(&param, GP_SIZE(8)));

	return True;
}

// --------------------------------------
void CGoEditorApp::SendBroadCast(ugoid_t& uid, int packetId, void* data, int size)
{
	cyiList* list = (cyiList*)CUserManager::GetInstance()->GetUserHandle(0);
	if (list &&
			list->count()!=0)
	{
		unsigned char* x= (unsigned char*)yialloci(size+16);
		memcpy(x, &uid, 16);
		memcpy(&x[16], data, size);
		m_network->BroadCastSend(list, packetId, x, size+16);
	}

	return ;
}

// --------------------------------------
void CGoEditorApp::xRemovePlayingGo(unsigned int remoteId, ugoid_t& uid, Bool bDataRemove)
{
	unsigned int myId = CUserManager::GetInstance()->GetMyId();

	sSimpleGoHeader* header = CUserManager::GetInstance()->GetWaitGoHeader(remoteId, uid);

	if (header)
	{
		if (bDataRemove)
		{
			m_network->SendToServer(eUSPID_DEL_GODATA, header, sizeof(sSimpleGoHeader), False);
		}
		else
		{
			m_network->SendToServer(eUSPID_DEL_GOHEADER, header, sizeof(sSimpleGoHeader), False);
		}

		CUserManager::GetInstance()->RemoveWaitGoHeader(remoteId, uid);

	}
}

#endif