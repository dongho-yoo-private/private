#include "svryuser.h"
#include "yifile.h"
#include "svrydefs.h"
#include "yitime.h"

#include <unistd.h>
#include <time.h>
#include <stdlib.h>

// -------------------------------------
static void xIdToPath(unsigned int id, char* pszPath)
{
	static char* root = (char*)csvryEnv::GetUserRoot();
	sprintf(pszPath, "%s/%03d/%03d/%03d/%d", root, id>>24, (id>>16)&0xFF, (id>>8)&0xFF, 0xFF&id);
}

// -------------------------------------
static Bool xMakePath(const char* fullpath, int len)
{
	Bool bIsDir;
	if (yiIsExist(fullpath, &bIsDir)==False)
	{
		char path[256];
		int i=0;
		strncpy(path, fullpath, len);
		path[len]=0;

		for (i=len; i>=0; i--)
		{
			if (path[i]=='/')
			{
				path[i]=0;
				break;
			}
		}
		if (xMakePath(path, i)==False)
		{
			return False;
		}
	}
	else
	{
		return bIsDir;
	}

	return mkdir(fullpath, MODE_755)==0?True:False;
}

typedef struct {
	unsigned int id;
	char szEmailAddr[80];
} sEmailStruct;
	

// --------------------------
unsigned int xOnFoundEmailPath(const char* pszPath, unsigned key, const char* pszEmailAddress)
{
	char path[256];
	sEmailStruct* s;
	int n=0;
	int size=0;

	if ((key&0xFFFF)==0)
	{
		sprintf(path, "%s/def", pszPath);
		s=(sEmailStruct*)yiFileLoadEx(path, &size);
		n=size/sizeof(sEmailStruct);

		if (s==0)
		{
			YIERROR("yixSearchPath() %s....... invalid.....\n", path);
			return 0;
		}

		for (int i=0; i<n; i++)
		{
			if (strncmp(s[i].szEmailAddr, pszEmailAddress, 80)==0)
			{
				int ret = s[i].id;
				yiFileUnLoadEx(s);
				return ret;
			}
		}
		yiFileUnLoadEx(s);
		return 0;
	}
	else
	{
		sprintf(path, "%s/email", pszPath);
		s = (sEmailStruct*)yiFileLoadEx(path, &size);

		if (s==0)
		{
			YIERROR("yixSearchPath() %s....... invalid.....", path);
			return 0;
		}

		if (strncmp(s->szEmailAddr, pszEmailAddress, 80)==0)
		{
			int ret = s->id;
			yiFileUnLoadEx(s);
			return ret;
		}
		yiFileUnLoadEx(s);
	}
	return 0;
}

// --------------------------
Bool csvryuser::IsEmailValid(unsigned int _key, const char* pszEmailAddress)
{
	/*if (yixSearchPath(csvryEnv::GetEmailRoot() , key, pszEmailAddress, 0, 2, xOnFoundEmailPath)!=0)
	{
		return True;
	}*/

	if (m_EmailHash==0)
		return False;

	unsigned int key = m_EmailHash->GetKey(_key, pszEmailAddress);

	if (key==0)
		return False;

	return True;
}

typedef struct {
	unsigned int randCode1;
	unsigned int randCode2;
	char szEmailAddress[80];
} sTemporaryMail;

// --------------------------
Bool csvryuser::OnPreRegister(int sock, unsigned int layer, const char* pszEmailAddress)
{
	sGeneralParam param;
	unsigned int randNo;// 	= rand();
	unsigned int xTime;//	= yiLocalTime();

	param.param[0]=eGE_OK;

	YIDEBUG("csvryuser::OnPreRegister() called\n");

	// find email address from temporary email address.
	if (m_listEmail!=0)
	{
		m_listEmail->lock();
		for (cyiNode* node=m_listEmail->GetNodeTopFrom(0); node; node=node->np)
		{
			sTemporaryMail* s = (sTemporaryMail* )node->ref;
			if (strncmp(pszEmailAddress, s->szEmailAddress, 80)==0)
			{
				m_listEmail->unlock();
				param.param[0]=4;
				goto ALREADY_REGISTERED_EMAIL;
			}
		}
		m_listEmail->unlock();
	}	
	
	// find email address	
	if (IsEmailValid(layer, pszEmailAddress)==True)
	{
		param.param[0]=3;
		YIDEBUG("%s is already registerd!", pszEmailAddress);
		goto ALREADY_REGISTERED_EMAIL;
	}

	if (m_listEmail==0)
		m_listEmail = new cyiList(True);	


	{
		srand(time(0));
		randNo 	= rand();
		xTime= yiLocalTime();
		sTemporaryMail ss;// = {time, randNo, };

		ss.randCode1=xTime;
		ss.randCode2=randNo;
		strcpy(ss.szEmailAddress, pszEmailAddress);
		m_listEmail->lock();
		m_listEmail->addref(yitoheap(&ss, sizeof(ss)));//yistrheap(pszEmailAddress));
		m_listEmail->unlock();
	}

	if (xSendMail(pszEmailAddress, xTime, randNo)==True)
	{
		if (yisendx(sock, eUSPID_REQ_REGISTER_EMAIL, m_ServerInfo.security, m_ServerInfo.security, &param, GP_SIZE(1), True)!=GP_SIZE(1))
		{
			YIERROR("csvryuser::OnPreRegister() send error!\n");
			return False;
		}
		return True;
	}
	else
	{
		YIERROR("csvryuser::OnPreRegister() xSendMail Failure!!\n");
	}


	// send error!
	// server is busy?
	param.param[0]=1;
	if (yisendx(sock, eUSPID_REQ_REGISTER_EMAIL, m_ServerInfo.security, m_ServerInfo.security, &param, GP_SIZE(1), True)!=GP_SIZE(1))
	{
		YIERROR("csvryuser::OnPreRegister() send error!\n");
		return False;
	}

	return True;
		
	
ALREADY_REGISTERED_EMAIL:
	param.param[0]=2;
	if (yisendx(sock, eUSPID_REQ_REGISTER_EMAIL, m_ServerInfo.security, m_ServerInfo.security, &param, GP_SIZE(1), True)!=GP_SIZE(1))
	{
		YIERROR("csvryuser::OnPreRegister() send error2!\n");
		return False;
	}
		
	return True;
}

const char _title[] = {
	0x47, 0x6f, 0x21, 0x21, 0x59, 0x6f, 0x74, 0x73, 0x75, 0x79, 0x61, 0x1b, 0x24, 0x42, 0x24, 0x4e, 0x47, 
	0x27, 0x3e, 0x5a, 0x25, 0x33, 0x21, 0x3c, 0x25, 0x49, 0x1b, 0x28, 0x42, 0x00
};

const char _contents[] = {
	0x1b, 0x24, 0x42, 0x32, 0x3c, 0x35, 0x2d, 0x24, 0x4e, 0x47, 0x27, 0x3e, 0x5a, 0x25, 0x33, 0x21, 0x3c, 
	0x25, 0x49, 0x24, 0x47, 0x47, 0x27, 0x3e, 0x5a, 0x24, 0x72, 0x39, 0x54, 0x24, 0x43, 0x24, 0x46, 
	0x24, 0x2f, 0x24, 0x40, 0x24, 0x35, 0x24, 0x24, 0x21, 0x23, 0x1b, 0x28, 0x42, 0x0d, 0x0a, 0x1b, 
	0x24, 0x42, 0x47, 0x27, 0x3e, 0x5a, 0x25, 0x33, 0x21, 0x3c, 0x25, 0x49, 0x1b, 0x28, 0x42, 0x3a, 
	0x20, 0x00
};

#include "yiSmtp.h"
// -------------------------------------
Bool csvryuser::xSendMail(const char* pszEmailAddress, unsigned int r1, unsigned int r2)
{
	char buffer[1024];

	YIDEBUG("csvryuser::xSendMail(%s, %u, %u) called\n", pszEmailAddress, r1, r2);

	char* mail = (char*)yialloci(82);
	strncpy(mail, pszEmailAddress, 80);

	sprintf(buffer, "%s %u-%u", _contents, r1, r2);//rand(), rand());
	YIDEBUG(buffer);
	int res = yiSmtpSend("smtp.mail.yahoo.co.jp", 587, "ZG9uaG9feW9v", "OTIwMDY4", "donho_yoo@yahoo.co.jp", "admin@goyotsuya.com", 
			mail, _title, yistrheap(buffer, 0), True, 0x04|0x01);

	return res==0;
}

// -------------------------------------
Bool csvryuser::OnReleaseEmailAddress(int sock, const char* pszEmailAddress)//(const char*)&param->param[1])
{
	YIDEBUG("csvryuser::OnReleaseEmailAddress(%s) called\n", pszEmailAddress);
	Bool bIsRemoved=False;
	if (m_listEmail)
	{
		for (cyiNode* node = m_listEmail->GetNodeTopFrom(0);node; node=node->np)
		{
			sTemporaryMail*s = (sTemporaryMail*)node->ref;
			if (strncmp(pszEmailAddress, s->szEmailAddress, 80)==0)
			{
				YIDEBUG("csvryuser::OnReleaseEmailAddress(%s) OK\n", pszEmailAddress);
				m_listEmail->remove(node);
				yifree(s);
				break;
			}
		}
	}

	return True;	
}

// -------------------------------------
Bool csvryuser::OnQueryUser(int sock, unsigned int id, const char* pszId)
{
	unsigned int key=id;
	char* pStrKey = (char*)pszId;
	sGeneralParam param;
	int size=0;

	if ((id&0x000000FF)!=0)
	{	
		key = m_UserHash->GetKey(id, pszId);
		param.param[0]=key;
		size = GP_SIZE(1);
	}
	else
	{
		if (m_UserHash->GetValue(key, &pStrKey)==0)
		{
			param.param[0]=0;
			size = GP_SIZE(1);
		}
		else
		{
			param.param[0]=key;
			strncpy((char*)&param.param[1], pStrKey, 16);
			size = GP_SIZE(5);
		}
	}


	
	YIDEBUG("csvryuser::OnQueryUser..... (%x:%s) key:%x res:%x\n", id, pszId, key, param.param[0]);
	if (yisendx(sock, eUSPID_REQ_QUERY_USER, m_ServerInfo.security, m_ServerInfo.security, &param, size, True)!=size)
	{
		YIDEBUG("csvryuser::OnQueryUser() unknown error\n", pszId);
		return True;
	}

	return True;
	
}

// -------------------------------------
Bool csvryuser::OnReconizedData(int sock, unsigned int randCode1, unsigned int randCode2)//param->param[0], param->param[1])
{
	Bool bIsFound=False;
	YIDEBUG("csvryuser::OnReconizedData() id is %u!\n", randCode1, randCode2);
	if (m_listEmail)
	{
		for (cyiNode* node = m_listEmail->GetNodeTopFrom(0);node; node=node->np)
		{
			sTemporaryMail*s = (sTemporaryMail*)node->ref;

			if (randCode1==s->randCode1 &&
				randCode2==s->randCode2)
			{
				YIDEBUG("csvryuser::OnReconizedData() email:%s is released\n", s->szEmailAddress);
				bIsFound=True;
				m_listEmail->remove(node);
				yifree(s);
				break;
			}
		}
	}

	sGeneralParam param;
	
	if (bIsFound==True)
		param.param[0]=0;
	else
	{
		YIDEBUG("csvryuser::OnReconizedData() invalid code!\n");
		param.param[0]=-1;
	}

	if (yisendx(sock, eUSPID_REQ_RECONIZED_CODE, m_ServerInfo.security, m_ServerInfo.security, &param, GP_SIZE(1), True)!=GP_SIZE(1))
	{
		return True;
	}
		
	return True;
}

// -------------------------------------
/*typedef struct {
	char szUserName[16];
	char szPassword[8];
	char szEmailAddress[80];
	unsigned short nContryCode;
	unsigned short nAreaCode;
	unsigned int	birthday;
	unsigned int 	flag; // flag 31: true:male, false:femail, 30:openflag:nAreaCode, 29:openflag:birthday, .... 7-0: level
} sRegisterUserInfo;*/

// -------------------------------------
Bool csvryuser::xCreateDefaultUserFiles(unsigned int id, sRegisterUserInfo* info)
{
	char buffer[256];
	char path[256];

	sprintf(path, "%s/%03d/%03d/%03d/%d", m_szUserRoot, (id>>24)&0xFF, (id>>16)&0xff, (id>>8)&0xFF, id&0xFF);
	yiMakeDir(path, True);	

	sprintf(buffer, "%s/0", path);
	if (yiFileCreateEx(buffer, 0, info, sizeof(*info), True)==False)
	{
		// ユーザ登録失敗？
		YIERROR("At RegisterUser(%s) write error!\n", buffer);
		return False;
	}

	sprintf(buffer, "%s/1", path);
	sBaseUserInfo one={0,};
	one.point=m_nRegisterPoint;
	one.reserved[0] = m_nInitialFriendCount;
	
	if (yiFileCreateEx(buffer, 0, &one, sizeof(one), True)==False)
	{
		// ユーザ登録失敗？
		YIERROR("At RegisterUser(%s) write error!\n", buffer);
		return False;
	}

	sprintf(buffer, "%s/2", path);

	if (yiFileCreateEx(buffer, 0, 0, sizeof(sUser)<<5, True)==False)
	{
		// ユーザ登録失敗？
		YIERROR("xCreateDefaultUserFiles (%s) create error!\n", buffer);
		return False;
	}

	sprintf(buffer, "%s/3", path);

	if (yiFileCreateEx(buffer, 0, 0, sizeof(sMsg32)<<5, True)==False)
	{
		// ユーザ登録失敗？
		YIERROR("xCreateDefaultUserFiles (%s) create error!\n", buffer);
		return False;
	}

	sprintf(buffer, "%s/wmq8", path);
	if (yiFileCreateEx(buffer, 0, 0, 32<<4, True)==False)
	{
		// ユーザ登録失敗？
		YIERROR("xCreateDefaultUserFiles (%s) create error!\n", buffer);
		return False;
	}

	sRegisterEmailInfo emailInfo;

	emailInfo.id=info->emailId;
	emailInfo.userId=info->id;
	strcpy(emailInfo.szEmailAddress, info->szEmailAddress);
	// for email.
	sprintf(path, "%s/%03d/%03d/%d", csvryEnv::GetEmailRoot(), (info->emailId>>24)&0xFF, (info->emailId>>16)&0xff, (info->emailId)&0xFFFF);
	yiMakeDir(path, True);	
	sprintf(buffer, "%s/0", path);

	if (yiFileCreateEx(buffer, 0, &emailInfo, sizeof(emailInfo), True)==False)
	{
		// ユーザ登録失敗？
		YIERROR("At RegisterEmail(%s) write error!\n", buffer);
		return False;
	}

	// メッセージキューを作成
	YIDEBUG("csvryuser::xCreateDefaultUserFiles() success!\n");
	return True;
}

// -------------------------------------
#define ADD_FRIEND_ERROR_INVALID_REMOTE -1
#define ADD_FRIEND_ERROR_INVALID_SYSTEM -2
#define ADD_FRIEND_ERROR_ALREADY_REQUEST 1
#define ADD_FRIEND_ERROR_OTHER		-3
// -------------------------------------
Bool csvryuser::OnRegisterUser(int sock, unsigned int id, sRegisterUserInfo* info)//param->param[0], param->param[1])
{
	sGeneralParam param={0, };

	yiTraceDump(info, sizeof(*info), 32, "sRegisteruserInfo");
	YIDEBUG("csvryuser::OnRegisterUser()%s, %s\n", info->szUserName, info->szEmailAddress);
	YIDEBUG("csvryuser::OnRegisterUser()id:%x, emailId:%x, ContryCode:%d, AreaCode:%d, birthday:%x, flag:%x\n", 
							info->id, info->emailId, info->nContryCode, 
									info->nAreaCode, info->birthday, info->flag);

	// 1. check user id.
	if (m_UserHash->GetKey(info->id, info->szUserName)!=0)
	{
		param.param[0]=-1;
		goto DUPLICATE_USER_NAME;
	}

	//int count = xGetUserCount(id);

	// 2. register user id.
	if ((id=m_UserHash->Add(id, info->szUserName, 0))==0)
	{
		param.param[0]=-1;
		goto DUPLICATE_USER_NAME;
	}

	// 3. register email address.
	info->id=id;
	info->emailId = m_EmailHash->Add(info->emailId, info->szEmailAddress, 0);

	YIDEBUG("csvryuser::OnRegisterUser() emailId:%x", info->emailId);

	// 4. make user files.
	if (xCreateDefaultUserFiles(id, info)==False)
	{
		YIERROR("create default user data failure!!\n");
		m_EmailHash->Remove(info->emailId);
		m_UserHash->Remove(id);
		YIERROR("user removed!!\n");
		param.param[0]=ADD_FRIEND_ERROR_INVALID_SYSTEM;
	}
	else
	{
		char path[256];
		param.param[0]=0;
		param.param[1]=id;

		sprintf(path, "%s/%03d/%03d/%03d/%d", csvryEnv::GetUserRoot(), (info->id>>24)&0xFF, (info->id>>16)&0xFF, (info->id>>8)&0xFF, info->id&0xFF);
		if (OnLoadUserData(path, info->id, True)==False)
		{
			YIERROR("User(%s) Data Load Error!", path);
		}
		sprintf(path, "%s/%03d/%03d/%d", csvryEnv::GetEmailRoot(), (info->emailId>>24)&0xFF, (info->emailId>>16)&0xFF, info->emailId&0xFFFF);

		if (OnLoadEmailAddress(path, info->emailId, True)==False)
		{
			YIERROR("Email(%s) Data Load Error!", path);
		}
	}


	YIDEBUG("csvryuser::OnRegisterUser() success id:%x, emailId:%x\n", id, info->emailId);
	// 6. error.
DUPLICATE_USER_NAME:
	// connection closed.
	if (yisendx(sock, eUSPID_REQ_REGISTER_USER, m_ServerInfo.security, m_ServerInfo.security, &param, GP_SIZE(2), True)!=GP_SIZE(2))
	{
		return False;
	}
	return True;
}

// -------------------------------------
Bool csvryuser::OnReqDeleteFriend(int sock, unsigned int myId, unsigned int remoteId, void** pExtraDataRef)
{
	return True;
}



// -------------------------------------
Bool csvryuser::OnReqAddFriend(int sock, unsigned int myId, Bool bIsSave, unsigned int remoteId, const char* pszFriendName, void** pExtraDataRef)
{
	sGeneralParam param = {0, };
	HandleUserFile hMyFile=0;
	HandleUserFile hRemoteFile=0;
	sRegisterUserInfo* pMyRegisterInfo;

	YIDEBUG("csvryuser::OnReqAddFriend(from:%x, to:%s) bIsSave=%d called\n", myId, pszFriendName, bIsSave);
	if ((myId&0xFF)==0)
	{
		YIERROR("csvryuser::OnReqAddFriend() Invalid user! %x reason: no user files\n", myId);
		return False;
	}

	hMyFile = (void**)*pExtraDataRef;
	hRemoteFile = GetUserExtraData(remoteId, pszFriendName);

	if (hMyFile==0 ||
		hRemoteFile==0)
	{
		SendError(sock, eUSPID_REQ_ADD_FRIEND, ADD_FRIEND_ERROR_INVALID_REMOTE, pszFriendName, 16);
		return True;
	}
	pMyRegisterInfo = REGISTER_INFO(hMyFile);

	if (pMyRegisterInfo==0)
	{
		SendError(sock, eUSPID_REQ_ADD_FRIEND, ADD_FRIEND_ERROR_INVALID_SYSTEM, pszFriendName, 16);
		return True;
	}

	//YIDEBUG("trace...................1 pRemoteLoginDatas %x\n", pRemoteLoginDatas );
	//
	if (bIsSave==-1)
	{
		FindWaitMsg8(hRemoteFile, myId, eWMT_ADD_FRIEND, True);
		// 成功したか失敗したかは重要ではない？

		// 断られた。
		AddMsg32(hMyFile, eWMT_ADD_FRIEND_NG, hRemoteFile, False);
		ReleaseUserExtraData(hRemoteFile);
		return True;
	}

	if (bIsSave==True)
	{
		sBaseUserInfo* base = BASE_INFO(hMyFile); 

		if (base==0)
		{
			YIERROR("csvryuser::OnReqAddFriend() invalid user! reason: no one file\n");
			ReleaseUserExtraData(hRemoteFile);
			return False;
		}

		int nMaxUserCnt = base->sizes[2]/sizeof(sUser);
		int nMaxFriendSize = base->sizes[2];
		int nFriendCnt=0;

		sUser* pFriend;
		if ((pFriend=FRIEND_INFO(hMyFile))==0)
		{
			YIERROR("csvryuser::OnReqAddFriend() invalid user! reason: no two file\n");
			ReleaseUserExtraData(hRemoteFile);
			return False;
		}

		if (AddFriend(hMyFile, remoteId, pszFriendName)==False)
		{
			DeleteFriend(hRemoteFile, myId);
			// friend is full! 
			SendError(sock, eUSPID_REQ_ADD_FRIEND, ADD_FRIEND_ERROR_INVALID_SYSTEM, pszFriendName, 16);

			AddMsg32(hMyFile, eWMT_ADD_FRIEND_NG, hRemoteFile, False, ADD_FRIEND_ERROR_INVALID_SYSTEM);
			ReleaseUserExtraData(hRemoteFile);
			return True;
		}

		int friendSize = base->sizes[2];/*((pFriend->cnt&0xFFFF)-1)*sizeof(sUser)+sizeof(sFriendList);*/
		int dataSize = sizeof(sRegisterUserInfo)+sizeof(sBaseUserInfo)+friendSize;
		sAddFriendOk* data = (sAddFriendOk*)yialloci(dataSize);//sizeof(sRegisterUserInfo));
		
		memcpy(&data->reg, REGISTER_INFO(hRemoteFile), sizeof(sRegisterUserInfo));
		memset(data->reg.szPassword, 0, 8);

		memcpy(&data->login, BASE_INFO(hRemoteFile), sizeof(sBaseUserInfo));
		memcpy(data->list, FRIEND_INFO(hMyFile), friendSize);

		YIDEBUG("send(eUSPID_REQ_ADD_FRIEND_OK) size:%d\n", dataSize);

		if (yisendx(sock, eUSPID_REQ_ADD_FRIEND_OK, m_ServerInfo.security, m_ServerInfo.security, data, dataSize, True)!=dataSize)
		{
			YIERROR("send error! (eUSPID_REQ_ADD_FRIEND_OK)\n");
			ReleaseUserExtraData(hRemoteFile);
			return False;
		}

		if (DeleteMsg32(hMyFile, remoteId, eWMT_ADD_FRIEND)==False)
		{
			DeleteMsg32(hMyFile, remoteId, eWMT_ADD_FRIEND_OK);
			FindWaitMsg8(hMyFile, remoteId, eWMT_ADD_FRIEND, True);
		}
		else
		{
			if (AddMsg32(hMyFile, eWMT_ADD_FRIEND_OK, hRemoteFile, False)==False)
			{
				YIERROR("AddMsg32 Error! (eUSPID_REQ_ADD_FRIEND_OK)\n");
			}
		}

/*SEND_RESULT:
		param.param[1]=remoteId;
		strcpy((char*)&param.param[2], pszFriendName);
		if (yisendx(sock, eUSPID_REQ_ADD_FRIEND_OK, m_ServerInfo.security, m_ServerInfo.security, &param, GP_SIZE(6), True)!=GP_SIZE(6))
		{
			YIERROR("send error! (eUSPID_GET_FRIEND_INFO)\n");
			ReleaseUserExtraData(hRemoteFile);
			return False;
		}*/

		ReleaseUserExtraData(hRemoteFile);
		return True;
	}

	//YIDEBUG("trace...................2\n");

	if ((remoteId&0xFF)==0)
	{
		remoteId = m_UserHash->GetKey(remoteId, pszFriendName);

		if (remoteId==0)
		{
			SendError(sock, eUSPID_REQ_ADD_FRIEND, ADD_FRIEND_ERROR_INVALID_SYSTEM, pszFriendName, 16);
			ReleaseUserExtraData(hRemoteFile);
			return True;
		}
	}

	if (FindWaitMsg8(hMyFile, remoteId, eWMT_ADD_FRIEND)==True)
	{
		SendError(sock, eUSPID_REQ_ADD_FRIEND, ADD_FRIEND_ERROR_ALREADY_REQUEST, pszFriendName, 16);
		ReleaseUserExtraData(hRemoteFile);
		YIDEBUG("Not already request sent!\n");
		return True;
	}

	//YIDEBUG("trace...................3 (remoteId:%x)\n", remoteId);
	{
		syiLoginUserInfo2* pLoginUserInfo = LOGIN_INFO2(hRemoteFile);//(syiLoginUserInfo2*)pRemoteLoginDatas[USER_DATA_COUNT];
		sRegisterUserInfo* regInfo = REGISTER_INFO(hRemoteFile);//(sRegisterUserInfo* )pRemoteLoginDatas[0];

		AddMsg32(hMyFile, eWMT_ADD_FRIEND, hRemoteFile, True);
	}

	if (param.param[0]!=0)
	{
		param.param[0]=ADD_FRIEND_ERROR_OTHER;
	}

	YIDEBUG("Add Friend OK!\n");
	param.param[1]=remoteId;
	strncpy((char*)&param.param[2], pszFriendName, 16);

	if (yisendx(sock, eUSPID_REQ_ADD_FRIEND, m_ServerInfo.security, m_ServerInfo.security, &param, GP_SIZE(6), True)!=GP_SIZE(6))
	{
		YIERROR("eUSPID_REQ_ADD_FRIEND send error!\n");
		ReleaseUserExtraData(hRemoteFile);
		return False;
	}

	ReleaseUserExtraData(hRemoteFile);
	return True;
}

