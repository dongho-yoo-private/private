#include "svryuser.h"
#include <stdlib.h>
#include "yitime.h"

// -------------------------------------
Bool csvryuser::OnNoIdLogin(int sock, int index, syiLoginData* data, void** pExtra)
{
	void** px = (void**)yialloci(sizeof(void*)*(USER_DATA_COUNT+1));
	syiLoginUserInfo* p = (syiLoginUserInfo*)yialloci(sizeof(*p));	

	p->sock=sock;
	p->network.index = index;
	p->network.type=data->network.type;
	p->network.address=data->network.address;
	p->network.port=data->network.port;
	p->security = rand();
	//p->passport = rand();
	strncpy(p->szUserName, data->szName, 16);
	strncpy(p->szPassword, data->szPassword, 8);

	px[USER_DATA_COUNT]=p;	
	*pExtra=px;

	syiLoginReply r;
	r.id=0;
	r.index = index;
	r.sock = sock;
	r.security = p->security;

	if (p->network.type==eNT_UPNP ||
		p->network.type==eNT_NONE)
	{
		/*int testSock;
		if ((testSock=yiconnect(p->network.address, p->network.port, 1))<0)
		{
			p->network.type=eNT_UNKNOWN;
			p->network.address=0;
			p->network.port=0;
			r.network=False;
		}
		else
		{
			r.network=True;
			close(testSock);
		}
		*/
	}

	YIINFO("login: name:%s, index:%d, sock:%d, security:%x, nat_type:%d, address:%s, port:%d, p:%x\n", p->szUserName, r.index, r.sock, r.security, 
													p->network.type, yiltostr(p->network.address), p->network.port, p);

	if (yisendx(sock, eUSPID_NO_ID_LOGIN, m_ServerInfo.security, m_ServerInfo.security, &r, sizeof(r), True)!=sizeof(r))
	{
		yifree(p);
		yifree(px);
		*pExtra	= 0;
		yiTrace("csvryuser::OnNoIdLogin() failure\n");
		return False;
	}

	p->hMutex = yiMutexCreatex();
	p->isLogined=True;

	return True;
}

// -------------------------------------
void* csvryuser::GetValue(unsigned int userId, char** szUserName)
{
	return 0;
}

#include "svrydefs.register.h"
// -------------------------------------
Bool csvryuser::OnLogin(int sock, int index, syiLoginData* logindata, void** pExtra)
{
	sGeneralParam param;
	unsigned int userId = logindata->id;
	//void** pLoginDatas=0;
	HandleUserFile hUserFile=0;

	// 別の環境でログインする場合は、
	// 完全なIDが分からないので、残りのIDを検索する。
	if ((logindata->id&0xFF)==0)
	{
		userId = m_UserHash->GetKey(logindata->id, logindata->szName);

		if (userId==0)
		{
			YIDEBUG("csvryuser::OnLogin() index:%d, user:%s, pass:%s user is not found!\n", index, logindata->szName, logindata->szPassword);
			SendError(sock, eUSPID_LOGIN, eGE_INVALID_USER);
			return False;
		}

		YIDEBUG("user is found! %x\n", userId);
	}

	// ここからが重要
	{
		// ロード時の読み込んだファイルリストを取得
		char* pszUserName=0;
		hUserFile = (HandleUserFile)m_UserHash->GetValue(userId, &pszUserName);

		if (hUserFile==0 ||
			pszUserName==0)
		{
			YIDEBUG("csvryuser::OnLogin(%x) hUserFile is null! \n", userId);
			SendError(sock, eUSPID_LOGIN, eGE_INVALID_USER);
			return False;
		}

		sRegisterUserInfo* data = REGISTER_INFO(hUserFile);//(sRegisterUserInfo*)pLoginDatas[0];
		*pExtra=(void*)hUserFile;//pLoginDatas;

		YIDEBUG("GetValue(%x), pLoginDatas:%x, pszUserName:%s, data:%x\n", userId, hUserFile, pszUserName, data);


		if (memcmp(logindata->szName, data->szUserName, 16)!=0)
		{
			YIDEBUG("csvryuser::OnLogin(%s) Invalid User Name! \n", data->szUserName);
			SendError(sock, eUSPID_LOGIN, eGE_INVALID_USER);
			return False;
		}

		if (memcmp(logindata->szPassword, data->szPassword, 8)!=0)
		{
			YIDEBUG("csvryuser::OnLogin(%s) Invalid password! \n", data->szUserName);
			SendError(sock, eUSPID_LOGIN, eGE_INVALID_USER_PASSWORD);
			return False;
		}

		sGeneralParam param;// = (sGeneralParam* ) yialloc(all_size);
		//srand(time(0));

		param.param[0]=eGE_OK;
		param.param[1]=userId;
		param.param[2]=index;
		param.param[3]=(unsigned int)(unsigned long)hUserFile;//rand();
		param.param[4]=m_udpPorts[param.param[3]%10];


		if (yisendx(sock, eUSPID_LOGIN, m_ServerInfo.security, m_ServerInfo.security, &param, GP_SIZE(5), True)!=GP_SIZE(5))
		{
			YIDEBUG("eUSPID_LOGIN send error!\n");	
			return False;
		}

		sBaseUserInfo* base = BASE_INFO(hUserFile);
		if (base==0)
		{
			YIDEBUG("eUSPID_LOGIN base file not found!\n");	
			return False;
		}

		if (yisendx(sock, eUSPID_ONE_FILE, m_ServerInfo.security, m_ServerInfo.security, base, sizeof(sBaseUserInfo), False)!=sizeof(sBaseUserInfo))
		{
			YIERROR("OneFile send error!\n");
			return False;
		}

		base->lastLoginDate = yiLocalTime2(&base->lastLoginTime);
		UpdateUserFile(hUserFile, 1, &base->lastLoginDate, eULFF_UPDATE_TIME);

	
		// 二重ログイン時は最後のログインを優先する。	
		if (hUserFile[USER_DATA_COUNT]!=0)
		{
			//syiLoginUserInfo* p = (syiLoginUserInfo*)hUserFile[USER_DATA_COUNT];

			//if (p->sock)
			//{
			//	yiclose(p->sock);
				YIDEBUG("Already logined...... so close socket\n");	
			//}
		}

		syiLoginUserInfo2* p = (syiLoginUserInfo2*)yialloci(sizeof(*p));	

		p->sock=sock;
		p->id=userId;
		p->network.index=index;
		p->network.type = logindata->network.type;
		/*p->network.index = index;
		p->network.type=logindata->network.type;
		p->network.address=logindata->network.address;
		p->network.port=logindata->network.port;
		p->security = rand();*/
		//p->passport = rand();
		p->security = rand();
		p->pUserFileTable=hUserFile;
		p->hMutex = yiMutexCreatex();
		p->isLogined=False;

		hUserFile[USER_DATA_COUNT]=p;
		*pExtra=(void*)hUserFile;
		
		YIDEBUG("Login Success! id:%x, name:%s, index:%d, udpport:%d rand:%x\n", userId, logindata->szName, index, param.param[4], hUserFile);	
	}

	return True; 
}

// -------------------------------------
Bool csvryuser::OnLoginOK(int sock, HandleUserFile hFile)
{
	if (hFile==0)
		return False;

	syiLoginUserInfo* info = LOGIN_INFO(hFile);

	if (info==0)
		return False;

	info->isLogined=True;
	return True;
}

// -----------------------------------
Bool csvryuser::OnReqFiles(int sock, void* data, int size, HandleUserFile hUserFile)
{
	sBaseUserInfo* base = BASE_INFO(hUserFile);
	char* pRecevBuffer=(char*)data;

	YIDEBUG("csvryuser::OnReqFiles() called!\n");
	if (base==0)
	{
		YIERROR("csvryuser::OnReqFiles() base file is null!\n");
		return False;
	}

	if (size<USER_DATA_COUNT)
	{
		YIERROR("csvryuser::OnReqFiles() packet size error %d bytes\n", size);
		return False;
	}

	for (int i=0; i<USER_DATA_COUNT; i++)
	{
		if (i==1 || 
			i==4)
			continue;
		if (base->sizes[i]!=0 &&
			pRecevBuffer[i]!=0)
		{
			YIDEBUG("send %d file size:%d\n", i, base->sizes[i]);
			if (yisend(sock, hUserFile[i], base->sizes[i], False)!=base->sizes[i])
			{
				YIERROR("eUSPID_REQ_FILES send error %d, %d bytes\n", i, base->sizes[i]);
				return False;
			}
		}
	}
	return True;
}


// -----------------------------------
Bool csvryuser::OnReqGoHeader(int sock, unsigned int sequenceNo, HandleUserFile hFile)
{
	sBaseUserInfo* base = BASE_INFO(hFile);
	unsigned int userId = GETID(hFile);

	YIDEBUG("csvryuser::OnReqGoHeader(id:%x, sequencNo:%d) \n", userId, sequenceNo);

	if (base==0 ||
		userId==0)
	{
		YIERROR("csvryuser::OnReqGoHeader() base file is null or userId is zero\n");
		return False;
	}

	int cnt = base->sequenceNo[4]-sequenceNo;//param.param[0];

	char path[256];
	if (yisend(sock, &cnt, sizeof(int), False)!=sizeof(int))
	{
		YIDEBUG("recv error! Login Failure\n");
		return False;
	}

	YIDEBUG("csvryuser::OnReqGoHeader(id:%x, sequencNo:%d) send %d\n", userId, sequenceNo, cnt);
	if (cnt>0)
	{
		int size = cnt<<4;
		GetUserFilePath(path, userId, "go");
		sSimpleGoHeader* hd = (sSimpleGoHeader*)yialloc(size);

		if (yiFileReadEx(path, -1, hd, size, True)==True)
		{
			if (yisend(sock, hd, size, False)!=size)
			{
				YIDEBUG("send go header error!\n");
				yifree(hd);
				return False;
			}
		}
		else
		{
			YIERROR("read go header error!\n");
			yifree(hd);
			unsigned int result=-1;
			if (yisend(sock, &result, sizeof(unsigned int), True)!=size)
			{
				YIDEBUG("send go header error!\n");
				return False;
			}
			return True;
		}
		yifree(hd);
	}
	YIDEBUG("csvryuser::OnReqGoHeader(id:%x, sequencNo:%d) ok\n", userId, sequenceNo);
	return True;
}

// -----------------------------------
// result code: -1 file not found.
// result code: filesize
void* csvryuser::OnReqGoData(int sock, unsigned int black, unsigned int white, unsigned int day, unsigned int time, HandleUserFile hFile, int* nFileSize)
{
	char path[256];
	sprintf(path, "%s/%04d%02d%02d/%02d/%x%x%02d%02d", m_szCommonDir, DAY_YEAR(day), DAY_MON(day), DAY_DAY(day), 
				YLT_HOUR(time), black, white, YLT_MIN(time), YLT_SEC(time));
	int   size=0;
	void* data = yiFileLoadEx(path, &size);

	if (sock==0)
	{
		*nFileSize=size;
		return data;
	}

	if (data==0 ||
		size==0)
	{
		YIERROR("csvryuser::OnReqGoData(%s) file not found!\n", path);
		if (SendError(sock, eUSPID_REQ_GODATA, -1)==False)
		{
			return (void*)False;
		}
		return (void*)True;
	}

	if (SendResult(sock, eUSPID_REQ_GODATA, size)==False)
	{
		return (void*)False;
	}

	YIERROR("csvryuser::OnReqGoData(%s) send file size %d!\n", path, size);
	if (yisend(sock, data, size, True)!=size)
	{
		return (void*)False;
	}
	yifree(data);
	return (void*)True;
}

// -----------------------------------------------
void* csvryuser::OnReqGoData2(int sock, unsigned int black, unsigned int white, unsigned int day, unsigned int time, HandleUserFile hFile, int* nFileSize)
{
	char path[256];
	char date[32];

	GetUserFilePath(path, black, "pausedgo");
	sprintf(date, ".%x.%x.%04d%02d%02d%02d%02d%02d", black, white, DAY_YEAR(day), DAY_MON(day), DAY_DAY(day), YLT_HOUR(time), YLT_MIN(time), YLT_SEC(time));
	strcat(path, date);

	int   size=0;
	void* data = yiFileLoadEx(path, &size);

	if (sock==0)
	{
		*nFileSize=size;
		return data;
	}

	if (data==0 ||
		size==0)
	{
		YIERROR("csvryuser::OnReqGoData2(%s) file not found!\n", path);
		if (SendError(sock, eUSPID_REQ_GODATA, -1)==False)
		{
			return False;
		}
		return (void*)True;
	}

	if (SendResult(sock, eUSPID_REQ_GODATA, size)==False)
	{
		return (void*)False;
	}

	YIERROR("csvryuser::OnReqGoData(%s) send file size %d!\n", path, size);
	if (yisend(sock, data, size, True)!=size)
	{
		return (void*)False;
	}
	yifree(data);
	return (void*)True;
}

Bool csvryuser::OnDeleteGoHeader(int sock, sSimpleGoHeader* data, HandleUserFile hFile)
{
	Bool res = AppendGoHeader2(hFile, data, True);
	return True;
}

Bool csvryuser::OnDeleteGoData(int sock, sSimpleGoHeader* header, HandleUserFile hFile)
{
	Bool res = AppendGoHeader2(hFile, header, True);

	char path[256];
	char date[32];

	sRegisterUserInfo* reg = REGISTER_INFO(hFile);
	unsigned int blackId=reg->id;
	unsigned int whiteId=header->remoteId;

	if ((header->result&0x80000000)==0)
	{
		blackId=header->remoteId;
		whiteId=reg->id;
	}


	GetUserFilePath(path, blackId, "pausedgo");
	sprintf(date, ".%x.%x.%04d%02d%02d%02d%02d%02d", 
			blackId, whiteId, DAY_YEAR(header->day), DAY_MON(header->day), DAY_DAY(header->day), 
						YLT_HOUR(header->time), YLT_MIN(header->time), YLT_SEC(header->time));

	strcat(path, date);
	unlink(path);

	YIDEBUG("unlink file %s\n", path);

	return True;
}

// -----------------------------------
Bool csvryuser::AppendGoHeader2(HandleUserFile hFile, sSimpleGoHeader* header, Bool bIsDelete)
{
	char path[128];
	sRegisterUserInfo* reg= REGISTER_INFO(hFile);
	sBaseUserInfo* base = BASE_INFO(hFile);
	GetUserFilePath(path, reg->id, "5");

	sSimpleGoHeader* _header = PAUSED_GO(hFile);
	int n = (base->sizes[5]>>4);
	int nSameIndex=-1;
	int i=0;
	
	if (header!=0)
	{
		for (i=0; i<n; i++)
		{
			if (_header[i].remoteId==header->remoteId &&
				_header[i].time==header->time &&
					_header[i].day==header->day)
			{
				nSameIndex=i;
				if (bIsDelete==True)
				{
					YIDEBUG("csvryuser::AppendGoHeader2() delete success!\n");
					header->remoteId=0;
					goto WRITE;
				}
				break;
			}
		}
	}
	
	if (bIsDelete==True &&
		nSameIndex==-1)
	{
		YIDEBUG("csvryuser::AppendGoHeader2() already deleted!\n");
		return True;
	}

	if (nSameIndex!=-1)
	{
		nSameIndex = i*sizeof(sSimpleGoHeader);	
	}
	else
	{
		for (i=0; i<n; i++)
		{
			if (_header[i].remoteId==0)
			{
				nSameIndex=i;
				break;
			}
		}
	}

WRITE:
	YIDEBUG("csvryuser::AppendGoHeader2(%s)\n", path);
	if (yiFileWriteEx(path, nSameIndex, header, sizeof(*header), True)==False)
	{
		if (yiFileCreateEx(path, nSameIndex, header, sizeof(*header), True)==False)
		{
			YIERROR("csvryuser::AppendGoHeader(%s) error!\n", path);
			return False;
		}
	}

	base->sequenceNo[5]++;

	if (nSameIndex==-1)
	{
		base->sizes[5]+=sizeof(sSimpleGoHeader);
	}
	return UpdateUserFile(hFile, 1, 0, 0, eULFF_UPDATE_SEQUENCE_NO);
}

// -----------------------------------
Bool csvryuser::AppendGoHeader(HandleUserFile hFile, sSimpleGoHeader* header)
{
	char path[128];
	sRegisterUserInfo* reg= REGISTER_INFO(hFile);
	sBaseUserInfo* base = BASE_INFO(hFile);
	GetUserFilePath(path, reg->id, "go");

	if (yiFileWriteEx(path, -1, header, sizeof(*header), True)==False)
	{
		if (yiFileCreateEx(path, -1, header, sizeof(*header), True)==False)
		{
			YIERROR("csvryuser::AppendGoHeader(%s) error!\n", path);
			return False;
		}
	}

	base->sequenceNo[4]++;
	return UpdateUserFile(hFile, 1, 0, 0, eULFF_UPDATE_SEQUENCE_NO);
}

// -----------------------------------
// result code.
// 0: success, scores.
// -1: write go data error!
// -2: write go header error!
typedef struct {
	unsigned int b_id;
	unsigned int w_id;
	unsigned int day;
	unsigned int time;
} sUploadGoDataError;

Bool csvryuser::OnUploadGoData2(int sock, sSimpleGoData* data, int size, HandleUserFile hFile)
{
	char path[128];
	char szDir[128];
	sRegisterUserInfo* reg = REGISTER_INFO(hFile);
	unsigned int black = reg->id;
	unsigned int white = data->hd.remoteId;
	Bool bIsIamBlack=((data->hd.result&0x80000000)!=0);
	sSimpleGoHeader* header = &data->hd;

	YIDEBUG("csvryuser::OnUploadGoData2(myId:%x, remoteId:%x, day:%d, time:%d)\n", reg->id, header->remoteId, header->day, header->time);

	if (size>sizeof(sSimpleGoHeader))
	{
		char path[256];
		char date[32];
		unsigned int blackId=reg->id;
		unsigned int whiteId=header->remoteId;

		if ((data->hd.result&0x80000000)==0)
		{
			blackId=header->remoteId;
			whiteId=reg->id;
		}


		GetUserFilePath(path, blackId, "pausedgo");
		sprintf(date, ".%x.%x.%04d%02d%02d%02d%02d%02d", 
				blackId, whiteId, DAY_YEAR(header->day), DAY_MON(header->day), DAY_DAY(header->day), 
							YLT_HOUR(header->time), YLT_MIN(header->time), YLT_SEC(header->time));

		strcat(path, date);

		YIDEBUG("csvryuser::OnUploadGoData2(%s) %d bytes\n", path, size);
		if (yiFileWriteEx(path, 0, data, size, True)==False)
		{
			if (yiFileCreateEx(path, 0, data, size, True)==False)
			{
				sRegisterUserInfo* reg = REGISTER_INFO(hFile);
				sSimpleGoHeader* header = (sSimpleGoHeader*)data;				
				sUploadGoDataError s = {reg->id, header->remoteId, header->day, header->time};
			
				SendError(sock, eUSPID_UP_GODATA, -1, &s, sizeof(s));
				return True;
			}
		}
	}

	// save header.
	if (AppendGoHeader2(hFile, &data->hd, False)==False)
	{
		sRegisterUserInfo* reg = REGISTER_INFO(hFile);
		sSimpleGoHeader* header = (sSimpleGoHeader*)data;				
		sUploadGoDataError s = {bIsIamBlack?reg->id:header->remoteId, bIsIamBlack?header->remoteId:reg->id, header->day, header->time};
		
		if (SendError(sock, eUSPID_UP_GODATA, -2, &s, sizeof(s))==False)
		{
			YIERROR("csvryuser::OnUploadGoData2() SendError error!\n");
			return False;
		}
		YIERROR("AppendGoHeader2 error!\n");
		return True;
	}
	YIDEBUG("AppendGoHeader2 success!\n");

	return True;
}

Bool csvryuser::OnUploadGoData(int sock, sSimpleGoData* data, int size, HandleUserFile hFile)
{
	char path[128];
	char szDir[128];
	sRegisterUserInfo* reg = REGISTER_INFO(hFile);
	unsigned int black = reg->id;
	unsigned int white = data->hd.remoteId;
	Bool bIsIamBlack=False;
	sSimpleGoHeader* header = &data->hd;

	YIDEBUG("csvryuser::OnUploadGoData(myId:%x, remoteId:%x, day:%d, time:%d)\n", reg->id, header->remoteId, header->day, header->time);

	if (size>sizeof(sSimpleGoHeader))
	{
		bIsIamBlack=True;
		sprintf(szDir, "%s/%04d%02d%02d/%02d", m_szCommonDir, DAY_YEAR(header->day), DAY_MON(header->day), DAY_DAY(header->day), YLT_HOUR(header->time));
		sprintf(path, "%s/%x%x%02d%02d", szDir, black, white, YLT_MIN(header->time), YLT_SEC(header->time));

		YIDEBUG("csvryuser::OnUploadGoData(%s) %d bytes\n", path, size);
		if (yiFileCreateEx(path, 0, data, size, True)==False)
		{
			yiMakeDir(szDir, True);

			if (yiFileCreateEx(path, 0, data, size, True)==False)
			{
				sRegisterUserInfo* reg = REGISTER_INFO(hFile);
				sSimpleGoHeader* header = (sSimpleGoHeader*)data;				
				sUploadGoDataError s = {reg->id, header->remoteId, header->day, header->time};
			
				SendError(sock, eUSPID_UP_GODATA, -1, &s, sizeof(s));
				return True;
			}
		}
	}

	// save header.
	if (AppendGoHeader(hFile, &data->hd)==False)
	{
		sRegisterUserInfo* reg = REGISTER_INFO(hFile);
		sSimpleGoHeader* header = (sSimpleGoHeader*)data;				
		sUploadGoDataError s = {bIsIamBlack?reg->id:header->remoteId, bIsIamBlack?header->remoteId:reg->id, header->day, header->time};
		
		if (SendError(sock, eUSPID_UP_GODATA, -2, &s, sizeof(s))==False)
		{
			YIERROR("csvryuser::OnUploadGoData() SendError error!\n");
			return False;
		}
		YIERROR("AppendGoHeader error!\n");
		return True;
	}
	YIDEBUG("AppendGoHeader success!\n");

	sBaseUserInfo* base = BASE_INFO(hFile);

	int win = ((header->result>>29)&0x03);

	if (bIsIamBlack==True)
	{
		if (win==1)
		{
			base->wins++;
			base->point++;
			UpdateUserFile(hFile, 1, 0, 0, eULFF_UPDATE_ALL);
			YIDEBUG("point up! (user:%x, wins:%d, loses:%d, draws:%d, point:%d\n", reg->id, base->wins, base->loses, base->draws, base->point);
			return True;
		}
		else if (win==2)
			base->loses++;
		else if (win==3)
			base->draws++;	
	}
	else
	{
		if (win==2)
		{
			base->wins++;
			base->point++;
			UpdateUserFile(hFile, 1, 0, 0, eULFF_UPDATE_ALL);
			YIDEBUG("point up! (user:%x, wins:%d, loses:%d, draws:%d, point:%d\n", reg->id, base->wins, base->loses, base->draws, base->point);
			return True;
		}
		else if (win==1)
			base->loses++;
		else if (win==3)
			base->draws++;	
	}

	YIDEBUG("wins:%d, draws:%d, loses:%d, point:%d\n", base->wins, base->draws, base->loses, base->point);
	UpdateUserFile(hFile, 1, 0, 0, eULFF_UPDATE_SCORE);

	/*if (SendResult(sock, eUSPID_UP_GODATA, 0, &base->total, 8)==False)
	{
		YIERROR("csvryuser::OnUploadGoData() SendResult error!\n");
		return False;
	}*/

	return True;
}


// -------------------------------------
Bool csvryuser::OnGetMsg32(int sock, unsigned int id, void** pExtra)
{
	char path[256];
	GetUserFilePath(path, id, "mq32");
	int n=0;
	sMsg32* p = (sMsg32*)yiFileLoadQ(path, &n, True);

	YIDEBUG("csvryuser::OnGetMsg32(%s) p:%x, n=%d called\n", path, id, p, n);
	if (yisendx(sock, eUSPID_GET_MSG32, m_ServerInfo.security, m_ServerInfo.security, p, n<<5, True)!=(n<<5))
	{
		YIERROR("csvryuser::OnGetMsg32() send error!\n");
		for (int i=0; i<n; i++)
			yiFileEnQ(path, &p[i]);
		if (p)
			yifree(p);
		return True;
	}
	if (p)
		yifree(p);
	
	return True;
}


Bool csvryuser::OnGetFriendList(int sock, unsigned int id, void** pExtraDataRef)
{
	char path[256];
	int  n;
	void** pFileList = (void**)*pExtraDataRef;

	YIDEBUG("csvryuser::OnGetFriendList(%x)\n", id);
	if (pFileList[2]==0)
	{
		GetUserFilePath(path, id, "2");
		pFileList[2]=yiFileLoadEx(path, &n);

		if (pFileList[2]==0)
		{
			YIDEBUG("csvryuser::OnGetFriendList(%x), 2 File Load Failure!\n", id);
			return True;
		}
	}
	else
	{
		sFriendList* pFriend = (sFriendList*)pFileList[2];
	}
	

	YIDEBUG("csvryuser::OnGetFriendList(%x) send:(%x, %d) byte\n", id, pFileList[2], n);
	if (yisendx(sock, eUSPID_GET_FRIEND_LIST, m_ServerInfo.security, m_ServerInfo.security, pFileList[2], n, True)!=(n<<5))
	{
		return True;
	}
	YIERROR("csvryuser::OnGetFriendList() send error!\n");
	
	return True;
}

