#include "UserManager.h"
#include "yisock.h"

CUserManager::CUserManager(void)
: m_listFriend(0)
, m_listGroupUser(0)
, m_listNotify(0)
, m_myRegisterInfo(0)
, m_base(0)
, m_myMsg32(0)
, m_myFriend(0)
, m_listUserHandle(0)
, m_pGoHeader(0)
, m_myBmp(0)
, m_nBmpSeqNo(0)
, m_nRoomNo(0)
, m_myId(0)
, m_listWaitGo(0)
{
}

CUserManager::~CUserManager(void)
{
}

// --------------------------------------
CUserManager* CUserManager::me=0;

// --------------------------------------
CUserManager* CUserManager::GetInstance()
{
	if (me==0)
	{
		me = new CUserManager();
	}

	return me;
}

// --------------------------------------
void CUserManager::SetInstance(CUserManager* instance)
{
	me = instance;
}



// --------------------------------------
Bool CUserManager::Initailize(unsigned int myId, const char* pszMyId, const char* pszUserDir, 
								unsigned int server_addr, unsigned int udpPort, unsigned int nRoomNo)
{
	m_nRoomNo=nRoomNo;
	m_myId=myId;
	strcpy(m_szUserDir, pszUserDir);
	strncpy(m_szMyName, pszMyId, 16);

	memset(m_szGoFilePath, 0, sizeof(m_szGoFilePath));

	wsprintf(m_szMyDir, "%s\\%s", m_szUserDir, m_szMyName);
	m_listFriend = new cyiList(True);

	if (m_base)
		yifree(m_base);

	LoadBaseInfo();

	if (m_listUserHandle)
		delete m_listUserHandle;

	m_listUserHandle = new cyiList(True);

	char path[256];
	wsprintf(path, "%s\\cached", m_szUserDir);

	yiMakeDir(path);

	m_dictionary.n=0;
	m_dictionary.user=0;
	LoadDictionary();
	m_udpPort=udpPort;
	m_server_addr=server_addr;
	return True;
}

typedef struct {
	HWND hwnd;
	UINT msg;
} sNotifyList;

// --------------------------------------
sUserData* CUserManager::GetUserData(unsigned int id)
{
	cyiList* list = m_listFriend;
	Bool bIsFriendList=True;

TOP:
	if (list)
	{
		list->lock();
		for (cyiNode* node = list->GetNodeTopFrom(); node; node=node->np)
		{
			sUserData* data = (sUserData*)node->ref;

			if (data==0)
				continue;

			if (data->id==id)
			{
				list->unlock();
				return data;
			}
		}

		list->unlock();
	}

	if (bIsFriendList)
	{
		bIsFriendList=False;
		list=m_listGroupUser;
		goto TOP;
	}

	return 0;
}

// --------------------------------------
Bool CUserManager::RegisterNotifyWindow(HWND hWndNotifyWnd, UINT msg)
{
	sNotifyList s = {hWndNotifyWnd, msg};

	if (m_listNotify==0)
	{
		m_listNotify = new cyiList();
	}

	m_listNotify->addref(yitoheap(&s, sizeof(s)));

	return True;
}

// --------------------------------------
Bool CUserManager::NotifyMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_listNotify==0)
		return False;

	for (cyiNode* node = m_listNotify->GetNodeTopFrom(); node; node=node->np)
	{
		sNotifyList* s = (sNotifyList*)node->ref;
		PostMessage(s->hwnd, s->msg, wParam, lParam);
	}

	return True;
}


// --------------------------------------
void CUserManager::LoadRegisterInfo(unsigned int userId)
{
	char path[256];

	if (userId==0)
	{
		wsprintf(path, "%s\\0", m_szMyDir);
		sRegisterUserInfo*p = (sRegisterUserInfo*)yiFileLoadEx(path, 0);
		m_myRegisterInfo=p;
		return ;
	}

	wsprintf(path, "%s\\cached\\%x.reg", m_szUserDir, userId);
	sRegisterUserInfo*p = (sRegisterUserInfo*)yiFileLoadEx(path, 0);

	if (p==0)
		return ;

	sUserData* data = GetUserData(userId);

	if (data==0)
		return ;

	if (data->reg)
		yifree(data->reg);

	data->reg=p;

	return ;
}


// --------------------------------------
sRegisterUserInfo* CUserManager::GetRegisterInfo(unsigned int userId)
{
	if (userId==0)
	{
		if (m_myRegisterInfo==0)
		{
			LoadRegisterInfo(0);
		}
		return m_myRegisterInfo;
	}

	sUserData* data = GetUserData(userId);

	if (data==0)
	{
		return 0;//goto NODATA;
	}

	if (data->reg==0)
	{
		LoadRegisterInfo(userId);
	}
	
	if (data->reg)
		return data->reg;

NODATA:
	sGeneralParam param={4, userId, 0, };
	int sock = yiudp(0, 0);

	yisendu(sock, &param, 32, m_server_addr, m_udpPort);//m_network->m_myGoInfo.base.udpPort);

	yirecvu(sock, &param, 4, 0, 0, 0);

	if (param.param[0]==-1)
	{
		return 0;
	}
	sRegisterUserInfo reg;
	yirecvu(sock, &reg, sizeof(reg), 0, 0, 0);
	yiclose(sock);

	SetRegisterInfo(&reg);

	data = GetUserData(userId);

	if (data==0)
		return 0;
	return data->reg;

}

// --------------------------------------
void CUserManager::GetMyScore(unsigned int* pScore)
{
	pScore[0]=(m_base->wins<<16)|(m_base->loses);
	pScore[1]=(m_base->draws<<16);

	return ;
}


// --------------------------------------
void CUserManager::SetRegisterInfo(sRegisterUserInfo* info)
{
	if (info->id==0)
	{
		//m_myRegisterInfo=(sRegisterUserInfo*)yitoheap(info, sizeof(*info));
		return ;
	}

	if (info->id==m_myId)
	{
		char path[256];
		wsprintf(path, "%s\\0", m_szMyDir);

		if (yiFileWriteEx(path, 0, info, sizeof(*info), True)==False)
		{
			if (yiFileCreateEx(path, 0, info, sizeof(*info), True)==False)
			{
				return ;
			}
		}

		m_myRegisterInfo=(sRegisterUserInfo*)yitoheap(info, sizeof(*info));
		return ;
	}

	char path[256];
	wsprintf(path, "%s\\cached\\%x.reg", m_szUserDir, info->id);
	
	if (yiFileWriteEx(path, 0, info, sizeof(*info), True)==False)
	{
		if (yiFileCreateEx(path, 0, info, sizeof(*info), True)==False)
		{
			return ;
		}
	}

	sUserData* data = GetUserData(info->id);

	if (data==0)
		return ;

	if (data->reg)
		yifree(data->reg);

	data->reg=(sRegisterUserInfo*)yitoheap(info, sizeof(sRegisterUserInfo));
	NotifyMessage(0, info->id);

	return ;
}

// --------------------------------------
int CUserManager::AddPlayingGoHeader(unsigned int id, sSimpleGoHeader* header)
{
	sUserData* data = GetUserData(id);

	if (data==0)
		return 0;

	if (data->listGo==0)
	{
		data->listGo = new cyiList();
	}

	data->listGo->addref(yitoheap(header, sizeof(sSimpleGoHeader)));

	return 0;
}

// --------------------------------------
void CUserManager::DeletePlayingGoHeader(unsigned int id, int index)
{
	sUserData* data = GetUserData(id);

	if (data==0)
		return ;

	if (data->listGo==0)
		return ;

	if (index=-1)
	{
		int n = data->listGo->count();
		for (int i=0; i<n; i++)
		{
			sSimpleGoHeader* hd = (sSimpleGoHeader* )data->listGo->removeEx(0);
			if (hd)
				yifree(hd);
		}
		return ;
	}

	sSimpleGoHeader* hd = (sSimpleGoHeader*)data->listGo->removeEx(index);
	yifree(hd);
	return ;
}

// --------------------------------------
void CUserManager::DeletePlayingGoHeader(unsigned int id, ugoid_t uid)
{
	sUserData* data = GetUserData(id);

	if (data==0)
		return ;

	if (data->listGo==0)
		return ;


	for (cyiNode* node=data->listGo->GetNodeTopFrom(0); node; node=node->np)
	{
		sSimpleGoHeader* header=(sSimpleGoHeader*)node->ref;
		ugoid_t xx(id, header->remoteId);
		xx.day=header->day;
		xx.time=header->time;

		if ((header->result&0x80000000)==0)
		{
			xx.b_id=header->remoteId;
			xx.w_id=id;
		}

		if (xx==uid)
		{
			data->listGo->remove(node);
			yifree(header);
			return;
		}

	}

	return ;
}

// --------------------------------------
sSimpleGoHeader* CUserManager::SearchPlayingGoHeader(ugoid_t& id)
{
	sUserData* data = 0;
	if (id.b_id!=m_myId)
	{
		cyiList* list = GetPlayingGoHeader(id.b_id);

		if (list==0)
		{
			goto SKIP_LOOP;
		}

		for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
		{
			sSimpleGoHeader* header = (sSimpleGoHeader*)node->ref;

			if (header)
			{
				if ((header->remoteId==id.w_id)&&
						(header->day==id.day) &&
							(header->time==id.time))
				{
					return header;
				}
			}
		}
	}
SKIP_LOOP:
	if (id.w_id!=m_myId)
	{
		cyiList* list = GetPlayingGoHeader(id.w_id);

		if (list==0)
			return 0;
		
		for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
		{
			sSimpleGoHeader* header = (sSimpleGoHeader*)node->ref;

			if (header)
			{
				if ((header->remoteId==id.b_id)&&
						(header->day==id.day) &&
							(header->time==id.time))
				{
					return header;
				}
			}
		}
	}

	return 0;
}


// --------------------------------------
cyiList* CUserManager::GetPlayingGoHeader(unsigned int id, ugoid_t* uid)//, int* size)
{
	sUserData* data = GetUserData(id);

	if (data==0)
		return 0;

	if (data->listGo==0)
		return 0;

	if (uid==0)
		return data->listGo;

	for (cyiNode* node=data->listGo->GetNodeTopFrom(0); node; node=node->np)
	{
		sSimpleGoHeader* header=(sSimpleGoHeader*)node->ref;
		ugoid_t xx(id, header->remoteId);
		xx.day=header->day;
		xx.time=header->time;

		if ((header->result&0x80000000)==0)
		{
			xx.b_id=header->remoteId;
			xx.w_id=id;
		}

		if (xx==*uid)
		{
			return (cyiList*)header;
		}

	}

	return 0;
}

// --------------------------------------
sUser* CUserManager::LoadMyFriends()
{
	char path[256];

	wsprintf(path, "%s\\2", m_szMyDir);
	sUser*p = (sUser*)yiFileLoadEx(path, 0);
	
	if (m_myFriend)
		yifree(m_myFriend);
	
	m_myFriend=p;

	if (m_listFriend)
	{
		int n = m_base->sizes[2]/sizeof(sUser);
		int cnt=m_listFriend->count();

		for (int i=0; i<cnt; i++)
		{
			yifree(m_listFriend->removeEx(0));
		}

		for (int i=0; i<n; i++)
		{
			if (m_myFriend[i].id!=0)
			{
				sUserData data;
				memset(&data, 0, sizeof(data));
				data.id=m_myFriend[i].id;

				m_listFriend->addref(yitoheap(&data, sizeof(data)));
			}
		}
	}


	return p;
}

// --------------------------------------
//Bool CUserManager::GetUserName(unsigned int id, char* szUserName)
//{
//	sUserData* data = GetUserData(id);
//
//	if (data)
//	{
//		strncpy(szUserName, data->reg->szUserName);
//		return False;
//	}
//
//
//}

// --------------------------------------
void CUserManager::GetVsScore(unsigned int id, int& wins, int& loses, int& draws)
{
	wins=0;
	loses=0;
	draws=0;
	int size;

	sSimpleGoHeader* header = GetGoHeader(0, &size);

	if (header==0)
		return ;

	int n = size/sizeof(sSimpleGoHeader);

	for (int i=0; i<n; i++)
	{
		if (header[i].remoteId==id)
		{
			Bool bIsIamBlack = (header[i].result&0x80000000)!=0;
			int  result = (header[i].result>>29)&0x03;

			if (result==3)
				draws++;
			if (bIsIamBlack==True)
			{
				if (result==1)
					wins++;
				else if (result==2)
					loses++;
			}
			else
			{
				if (result==2)
					wins++;
				else if (result==1)
					loses++;
			}
		}
	}

	return ;
}


// --------------------------------------
int CUserManager::GetMyFriendsCount()
{
	if (m_base!=0 &&
				m_base->sizes[2])
	{
		return m_base->sizes[2]/sizeof(sUser);
	}

	return 0;
}


// --------------------------------------
int CUserManager::GetMyFriendIndex(unsigned int id)
{
	int n = m_base->sizes[2]/sizeof(sUser);

	for (int i=0; i<n; i++)
	{
		if (m_myFriend[i].id==id)
		{
			return i;
		}
	}

	return -1;
}

// --------------------------------------
unsigned int CUserManager::GetMyFriendId(const char* pszUserName)
{
	int n = m_base->sizes[2]/sizeof(sUser);

	for (int i=0; i<n; i++)
	{
		if (strncmp(m_myFriend[i].szUserName, pszUserName, 16)==0)
		{
			return m_myFriend[i].id;
		}
	}

	return 0;
}



// --------------------------------------
void CUserManager::SetMyFriends(sUser* info)
{
	char path[256];
	wsprintf(path, "%s\\2", m_szMyDir);

	if (yiFileWriteEx(path, 0, info, m_base->sizes[2], True)==False)
	{
		if (yiFileCreateEx(path, 0, info, m_base->sizes[2], True)==False)
		{
			return ;
		}
	}

	if (m_myFriend)
		yifree(m_myFriend);

	m_myFriend=(sUser*)yitoheap(info, m_base->sizes[2]);

	if (m_listFriend)
	{
		int n = m_base->sizes[2]/sizeof(sUser);
		int cnt=m_listFriend->count();

		for (int i=0; i<cnt; i++)
		{
			yifree(m_listFriend->removeEx(0));
		}

		for (int i=0; i<n; i++)
		{
			if (m_myFriend[i].id!=0)
			{
				sUserData data;
				memset(&data, 0, sizeof(data));
				data.id=m_myFriend[i].id;

				m_listFriend->addref(yitoheap(&data, sizeof(data)));
			}
		}
	}

	NotifyMessage(2, (LPARAM)m_myFriend);
	return ;
}

// --------------------------------------
sUser* CUserManager::GetMyFriends()
{
	if (m_myFriend==0)
	{
		m_myFriend = LoadMyFriends();
	}
	return m_myFriend;
}

// --------------------------------------
sMsg32* CUserManager::LoadMyMsg32()
{
	char path[256];

	wsprintf(path, "%s\\3", m_szMyDir);
	sMsg32*p = (sMsg32*)yiFileLoadEx(path, 0);
	
	if (m_myMsg32)
		yifree(m_myMsg32);
	
	m_myMsg32=p;
	return p;
}

// --------------------------------------
sMsg32* CUserManager::GetMyMsg32(int* n)
{
	if (m_myMsg32==0)
	{
		if (LoadMyMsg32()==0)
		{
			if (n)
				*n=0;
			return 0;
		}
	}

	if (n)
	{
		*n=m_base->sizes[3]/sizeof(sMsg32);
	}
	return m_myMsg32;
}

// --------------------------------------
void CUserManager::SetMyMsg32(sMsg32* msg)
{
	Bool bIsWriteOnly=False;
	char path[256];
	wsprintf(path, "%s\\3", m_szMyDir);

	if (msg==0)
	{
		bIsWriteOnly=True;
		msg=m_myMsg32;
	}

	if (yiFileWriteEx(path, 0, msg, m_base->sizes[3], True)==False)
	{
		if (yiFileCreateEx(path, 0, msg, m_base->sizes[3], True)==False)
		{
			return ;
		}
	}

	if (bIsWriteOnly!=True)
	{
		m_myMsg32 =(sMsg32*)yitoheap(msg, m_base->sizes[3]);
	}
	return ;
}

// --------------------------------------
int CUserManager::AddMsg32(sMsg32* msg)
{
	int n = m_base->sizes[3]/sizeof(sMsg32);

	for (int i=0; i<n; i++)
	{
		if (m_myMsg32[i].type==msg->type &&
				m_myMsg32[i].remoteId==msg->remoteId &&
					m_myMsg32[i].time==msg->time &&
						m_myMsg32[i].day==msg->day)
		{
			return i;
		}

		if (m_myMsg32[i].type==0)
		{
			m_myMsg32[i]=*msg;

			SetMyMsg32(0);
			m_base->sequenceNo[3]++;
			return i;
		}
	}

	return -1;
}

// --------------------------------------
void CUserManager::DeleteMsg32(int index)
{
	m_myMsg32[index].type=0;
	m_myMsg32[index].remoteId=0;
	SetMyMsg32(0);
	m_base->sequenceNo[3]++;
}

// --------------------------------------
Bool CUserManager::AppendGoHeader(unsigned int userId, sSimpleGoHeader* header, int cnt)
{
	char path[256];

	if (userId==0 ||
			userId==m_myId)
	{
		wsprintf(path, "%s\\go", m_szMyDir);

		if (yiFileWriteEx(path, -1, header, cnt*sizeof(sSimpleGoHeader), True)==False)
		{
			if (yiFileCreateEx(path, -1, header, cnt*sizeof(sSimpleGoHeader), True)==False)
			{
				return False;
			}
		}

		//if (m_pGoHeader)
		//	yifree(m_pGoHeader);

		int size;
		LoadGoHeader(0, &size);
		return (m_nGoHeaderSize/sizeof(sSimpleGoHeader));
		//return True;
	}

	wsprintf(path, "%s\\cached\\%x.go", m_szUserDir, userId);
	if (yiFileWriteEx(path, -1, header, cnt*sizeof(sSimpleGoHeader), True)==False)
	{
		if (yiFileCreateEx(path, -1, header, cnt*sizeof(sSimpleGoHeader), True)==False)
		{
			return False;
		}
	}

	int size;
	LoadGoHeader(userId, &size);
	sUserData* data = GetUserData(userId);
	return (data->goHeaderSize/sizeof(sSimpleGoHeader));

	//return True;
}

// --------------------------------------
sSimpleGoHeader* CUserManager::GetGoHeader(unsigned int userId, int* size)
{
	*size=0;
	if (userId==0)
	{
		if (m_pGoHeader==0)
		{
			LoadGoHeader(0, size);
		}
		*size=m_nGoHeaderSize;
		return m_pGoHeader;
	}

	sUserData* data = GetUserData(userId);

	if (data==0)
		return 0;

	if (data->hd==0)
	{
		LoadGoHeader(userId, size);
	}

	*size=data->goHeaderSize;
	return data->hd;
}

// --------------------------------------
void CUserManager::LoadGoHeader(unsigned int userId, int* size)
{
	char path[256];

	if (userId==0 ||
			userId==m_myId)
	{
		wsprintf(path, "%s\\go", m_szMyDir);
		sSimpleGoHeader*p = (sSimpleGoHeader*)yiFileLoadEx(path, size);

		if (m_pGoHeader!=0)
			yifree(m_pGoHeader);

		m_pGoHeader=p;
		m_nGoHeaderSize=*size;
		return ;
	}


	wsprintf(path, "%s\\cached\\%x.go", m_szUserDir, userId);
	sSimpleGoHeader*p = (sSimpleGoHeader*)yiFileLoadEx(path, size);

	if (p==0)
		return ;

	sUserData* data = GetUserData(userId);

	if (data==0)
		return ;

	if (data->hd)
		yifree(data->hd);

	data->goHeaderSize=*size;

	data->hd=p;

}


// --------------------------------------
void CUserManager::LoadPicture(unsigned int userId)
{
	unsigned int seq=0;
	char path[256];

	if (userId==0 ||
			userId==m_myId)
	{
		userId=m_myId;
	}

	wsprintf(path, "%s\\cached\\%x.img", m_szUserDir, userId);
	wchar_t wpath[256];
	yiToUnicode(wpath, path, 0);

	strcat(path, ".seq");
	if (yiFileRead(path, &seq, 0, sizeof(unsigned int))==False)
	{
		seq=0;

		if (userId==m_myId)
			m_nBmpSeqNo=0;
	}

	Bitmap x(wpath);

	if (x.GetLastStatus()!=Status::Ok)
	{
		return ;
	}
	
	Bitmap* bmp = BITMAP32(x.GetWidth(), x.GetHeight());
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighSpeed);
	g.DrawImage(&x, 0, 0);
//	Bitmap* bmp = cyiBitmapBlender::BitmapCopy(0, &x);
//	Bitmap* bmp = new Bitmap(wpath);

	if (bmp==0 ||
		bmp->GetLastStatus()!=Status::Ok)
	{
		return ;
	}

	if (userId==0 ||
			userId==m_myId)
	{
		if (m_myBmp)
			delete m_myBmp;
		m_myBmp=bmp;
		m_nBmpSeqNo=seq;
	}
	else
	{
		sUserData* data = GetUserData(userId);

		if (data->bmp)
			delete data->bmp;

		data->bmp=bmp;
		data->seq_bmp=seq;
	}

	return ;
}

// --------------------------------------
void* CUserManager::GetPictureFile(unsigned int userId, int* size)
{
	char path[256];
	if (userId==0 ||
			userId==m_myId)
	{
		userId=m_myId;
	}

	wsprintf(path, "%s\\cached\\%x.img", m_szUserDir, userId);
	return yiFileLoad(path, size);
}
#include "yiStream.h"
// --------------------------------------
void CUserManager::SetPictureFile(unsigned int userId, void *data, int size, unsigned int seq)
{
	IO::Stream stream;
	stream.CreateOnMemory(data, size);//(GetModuleHandle("yctrlGo.dll"), MAKEINTRESOURCE(IDB_GOBAN), "PNG");

	SetPicture(userId, new Bitmap(stream), seq);
}

// --------------------------------------
Bool CUserManager::IsMyFriend(unsigned int userId)
{
	cyiList* list = m_listFriend;

	if (list)
	{
		list->lock();
		for (cyiNode* node = list->GetNodeTopFrom(); node; node=node->np)
		{
			sUserData* data = (sUserData*)node->ref;

			if (data==0)
				continue;

			if (data->id==userId)
			{
				list->unlock();
				return True;
			}
		}

		list->unlock();
	}

	return False;
}

// --------------------------------------
Bool CUserManager::IsGroupUser(unsigned int userId)
{
	cyiList* list = m_listGroupUser;

	if (list)
	{
		list->lock();
		for (cyiNode* node = list->GetNodeTopFrom(); node; node=node->np)
		{
			sUserData* data = (sUserData*)node->ref;

			if (data==0)
				continue;

			if (data->id==userId)
			{
				list->unlock();
				return True;
			}
		}

		list->unlock();
	}

	return False;
}

// --------------------------------------
Bitmap* CUserManager::GetPicture(unsigned int userId, unsigned int& seq)
{
	Bitmap* ret=0;
	seq=0;

	if (userId==0 ||
			userId==m_myId)
	{
		if (m_myBmp==0)
			LoadPicture(0);
		seq=m_nBmpSeqNo;
		return m_myBmp;
	}

	sUserData* data = GetUserData(userId);

	if (data==0)
	{
		return 0;
	}

	if(data->bmp==0)
	{
		LoadPicture(userId);
	}
	seq=data->seq_bmp;
	return data->bmp;
}

// --------------------------------------
void CUserManager::SetPicture(unsigned int userId, Bitmap* bmp, unsigned int seq)
{
	char path[256];

	if (userId==0)
		userId=m_myId;

	wsprintf(path, "%s\\cached\\%x.img", m_szUserDir, userId);

	wchar_t wpath[256];
	yiToUnicode(wpath, path, 0);

	if (cyiShape::SaveBitmap(wpath, bmp, L"image/jpeg")==False)
	{
		return ;
	}

	strcat(path, ".seq");
	yiFileWrite(path, &seq, 0, sizeof(unsigned int));

	if (userId==m_myId)
	{
		if (m_myBmp)
			delete m_myBmp;

		m_myBmp=bmp;
		m_nBmpSeqNo=seq;
		return ;
	}

	sUserData* data = GetUserData(userId);

	if (data==0)
		return ;

	if (data->bmp)
		delete data->bmp;

	data->bmp=bmp;
	data->seq_bmp=seq;

	NotifyMessage(7, userId);
	return ;
}

// --------------------------------------
void CUserManager::LoadScore(unsigned int userId)
{
	char path[256];

	if (userId==0 ||
			userId==m_myId)
		return ;

	wsprintf(path, "%s\\cached\\%x.other", userId);
	
	int size;
	unsigned int* p = (unsigned int* )yiFileLoadEx(path, &size);//0, &score, sizeof(score), True);

	if (p==0 ||
			size==0)
		return ;



	sUserData* data = GetUserData(userId);

	if (data==0)
		return ;

	memcpy(data->score, p, sizeof(data->score));
	yiFileUnLoadEx(p);
}

// --------------------------------------
Bool CUserManager::GetScore(unsigned int userId, int& wins, int& loses, int& draws)
{
	Bitmap* ret=0;
	wins=-1;
	loses=-1;
	draws=-1;

	sUserData* data = GetUserData(userId);

	if (data==0)
		return False;

	wins=data->score[0]>>16;
	loses=data->score[0]&0xFFFF;
	draws=data->score[2]>>16;

	return True;
}

// --------------------------------------
void  CUserManager::SetScore(unsigned int userId, int win, int loses, int draw)
{
	unsigned int score[4]={0, };
	score[0]=(win<<16)|loses;
	score[1]=(draw<<16);
	SetScore(userId, score);
}

// --------------------------------------
void CUserManager::SetScore(unsigned int userId, unsigned int* score)
{
	sUserData* data = GetUserData(userId);

	if (data==0)
		return ;

	char path[256];
	wsprintf(path, "%s\\cached\\%x.other", m_szUserDir, userId);
	
	if (yiFileWriteEx(path, 0, score, sizeof(*score), True)==False)
	{
		if (yiFileCreateEx(path, 0, score, sizeof(*score), True)==False)
		{
			return ;
		}
	}
	
	memcpy(data->score, score, 16);

	return ;
}

// --------------------------------------
Bool CUserManager::UpdateSequenceNo(int nFileNo)
{
	char path[256];
	wsprintf(path, "%s\\%s\\1", m_szUserDir, m_szMyName);
	m_base->sequenceNo[nFileNo]++;
	return yiFileWriteEx(path, offsetof(sBaseUserInfo, sequenceNo)+sizeof(unsigned int)*nFileNo, &m_base->sequenceNo[nFileNo], sizeof(unsigned int), True);
}


// --------------------------------------
//Bool CUserManager::UpdateFriend()
//{
//	char path[256];
//	wsprintf(path, "%s\\%s\\2", m_szUserDir, m_szMyName);
//
//	int size;
//	sUser* f = (sUser*)yiFileLoadEx(path, &size);
//
//	if (f==0)
//	{
//		return False;
//	}
//
//	int n = size/sizeof(sUser);
//	
//	m_listFriend->clear();
//	for (int i=0; i<n; i++)
//	{
//		if (f[i].id==0)
//			continue;
//
//		sRegisterUserInfo* info = (sRegisterUserInfo*)
//		sUserData s = {};
//		m_listFriend->addref(yitoheap
//	}
//
//	NotifyMessage(1, (LPARAM)m_listFriend);
//
//	return True;
//}

// --------------------------------------
const char* CUserManager::GetGoFilePath(int index)
{
	return m_szGoFilePath[index];
}

// --------------------------------------
void CUserManager::SetGoFilePath(int index, const char* pszFilePath)
{
	strncpy(m_szGoFilePath[index], pszFilePath, 128);
}



// --------------------------------------
unsigned int CUserManager::GetMyId()
{
	return m_myId;
	/*if (m_myRegisterInfo)
		return m_myRegisterInfo->id;
	return 0;*/
}

// --------------------------------------
const char* CUserManager::GetMyName()
{
	if (m_myRegisterInfo)
		return m_myRegisterInfo->szUserName;

	if (m_nRoomNo!=0)
	{
		static char szUserName[16];

		wsprintf(szUserName, "%d番部屋", m_nRoomNo);
		return szUserName;
	}

	return 0;
}


// --------------------------------------
sBaseUserInfo* CUserManager::GetMyBaseInfo()
{
	return m_base;
}

// --------------------------------------
Bool CUserManager::SaveMyBaseInfo()
{
	char path[256];
	wsprintf(path, "%s\\%s\\1", m_szUserDir, m_szMyName);

	if (m_base==0)
	{
		return False;
	}

	if (yiFileWriteEx(path, 0, m_base, sizeof(*m_base), True)==False)
	{
		return yiFileCreateEx(path, 0, m_base, sizeof(*m_base), True);
	}

	return  True;
}



// --------------------------------------
// ログイン時に一度のみ設定される。
void CUserManager::SetMyBaseInfo(sBaseUserInfo* info)
{
	char path[256];
	wsprintf(path, "%s\\%s\\1", m_szUserDir, m_szMyName);
	
	if (info==0)
	{
		if (m_base)
			yifree(m_base);
		info=0;
		DeleteFileA(path);
		m_base=0;

		return ;

	}

	if (m_base==0)
	{
		m_base = (sBaseUserInfo*)yialloci(sizeof(*m_base));
	}

	*m_base=*info;
	//memcpy(m_base, info, sizeof(*m_base));

	if (yiFileWriteEx(path, 0, m_base, sizeof(*m_base), True)==False)
	{
		yiFileCreateEx(path, 0, m_base, sizeof(*m_base), True);
	}
}

// --------------------------------------
sBaseUserInfo* CUserManager::LoadBaseInfo()
{
	char path[256];
	wsprintf(path, "%s\\%s\\1", m_szUserDir, m_szMyName);
	m_base = (sBaseUserInfo*)yiFileLoadEx(path, 0);

	return m_base;
}

// --------------------------------------
void CUserManager::RemoveUserHandle(unsigned int id)
{
	m_listUserHandle->lock();
	for (cyiNode* node=m_listUserHandle->GetNodeTopFrom(); node; node=node->np)
	{
		 unsigned int* p = (unsigned int*)node->ref;

		if (*p==id)
		{
			m_listUserHandle->remove(node);
			break;
		}
	}
	m_listUserHandle->unlock();
}

// --------------------------------------
Bool CUserManager::AddUserHandle(Handle hUser)
{
	m_listUserHandle->lock();
	m_listUserHandle->addref((void*)hUser);
	m_listUserHandle->unlock();
	return True;
}

// --------------------------------------
Handle CUserManager::GetUserHandle(unsigned int id)
{
	Handle h=0;

	if (id==0)
		return (Handle)m_listUserHandle;

	m_listUserHandle->lock();
	for (cyiNode* node=m_listUserHandle->GetNodeTopFrom(); node; node=node->np)
	{
		unsigned int* p = (unsigned int*)node->ref;

		if (*p==id)
		{
			h=(Handle)p;
		}
			//return (Handle)p;
	}
	m_listUserHandle->unlock();
	return h;
}

// --------------------------------
Bool CUserManager::QueryUserName(unsigned int id, char* pszOutUserName)
{
	sGeneralParam param={1, id, 0, };
	int sock = yiudp(0, 0);

	yisendu(sock, &param, 32, m_server_addr, m_udpPort);//m_network->m_myGoInfo.base.udpPort);

	yirecvu(sock, &param, 4, 0, 0, 0);

	if (param.param[0]==-1)
	{
		*pszOutUserName=0;
		return False;
	}

	yirecvu(sock, pszOutUserName, 16, 0, 0, 0);

	yiclose(sock);
	return True;
}

// --------------------------------
Bool CUserManager::LoadDictionary()
{
	char path[256];
	int size=0;
	wsprintf(path, "%s\\.dictionary", m_szUserDir);

	if (m_dictionary.user==0)
	{
		m_dictionary.user=(sUserDictionary*)yiFileLoadEx(path, &size);
		m_dictionary.n = size/sizeof(sUserDictionary);
	}

	return True;
}


// --------------------------------
Bool CUserManager::AddDictionary(unsigned int id, const char* pszUserName, int flag)
{
	char path[256];
	int size=0;

	wsprintf(path, "%s\\.dictionary", m_szUserDir);

	if (m_dictionary.user==0)
	{
		LoadDictionary();
	}

	if (m_dictionary.n==0)
		goto NOT_FOUND;

	for (int i=0; i<m_dictionary.n; i++)
	{
		if (m_dictionary.user[i].id==id)
		{
			if (m_dictionary.user[i].groupId!=flag)
			{
				m_dictionary.user[i].groupId=flag;
				yiFileWriteEx(path, 0, m_dictionary.user, m_dictionary.n*sizeof(sUserDictionary), True);
			}
			return True;
		}
	}

NOT_FOUND:
	sUserDictionary append;
	append.id=id;
	//append.hash=yihf_encoded_additional(pszUserName, 31);
	strncpy(append.szName, pszUserName, 16);


	if (m_dictionary.n==0)
	{
		if (yiFileCreateEx(path, 0, &append, sizeof(append), True)==False)
			return False;
	}
	else
	{
		if (yiFileWriteEx(path, -1, &append, sizeof(append), True)==False)
			return False;

		yiFileUnLoadEx(m_dictionary.user);
	}

	m_dictionary.user=(sUserDictionary*)yiFileLoadEx(path, &size);
	m_dictionary.n = size/sizeof(sUserDictionary);

	if (m_dictionary.user==0)
		return False;
		

	return True;
}

// --------------------------------
const char* CUserManager::GetUserName(unsigned int id)
{
	sGeneralParam param={0, };
	Bool bIsFoundFromServer=False;

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
		if (m_dictionary.user[i].id==id)
		{
			return m_dictionary.user[i].szName;
		}
	}

NOT_FOUND:

	if (bIsFoundFromServer==True)
	{
		return 0;
	}

	char szUserName[16];
	Bool res = QueryUserName(id, szUserName);

	if (res==True)
	{
		AddDictionary(id, szUserName, 0);
		bIsFoundFromServer=True;
		goto TOP;
	}

	//param.param[0]=id;
	//param.param[1]=0;
	//if (m_network->SendToServer(eUSPID_REQ_QUERY_USER, &param, GP_SIZE(5), True)==True)
	//{
	//	int id;
	//	int size;
	//	sGeneralParam* ret = (sGeneralParam*)m_network->GetFromQueue(0, &id, &size);

	//	if (ret==0)
	//		return False;
	//	if (ret->param[0]==0)
	//	{
	//		return False;
	//	}

	//	if (AddDictionary(ret->param[0], (const char*)&ret->param[1], 0x00000100)==False)
	//	{
	//		return False;
	//	}
	//	
	//	goto TOP;
	//}
	return 0;
}

// --------------------------------
// 接続待ち・予定のあるデータ
//int CUserManager::AddWaitGoHeader(sSimpleGoHeader* data)
//{
//	if (m_listWaitGo==0)
//	{
//		m_listWaitGo = new cyiList();
//	}
//
//	m_listWaitGo->addref(data);
//
//	return m_listWaitGo->count();
//	
//}

// --------------------------------
int CUserManager::AddWaitGoHeader(sSimpleGoHeader* header)
{
	if (m_listWaitGo==0)
	{
		m_listWaitGo = new cyiList();
	}

	if (m_listWaitGo->count()!=0)
	{
		for (cyiNode* node = m_listWaitGo->GetNodeTopFrom(); node; node=node->np)
		{
			sSimpleGoHeader* _header = (sSimpleGoHeader* )node->ref;

			if (_header->remoteId==header->remoteId &&
					_header->day==header->day &&
						_header->time==header->time)
			{
				return 0;
			}
		}
	}

	m_listWaitGo->addref(yitoheap(header, sizeof(sSimpleGoHeader)));

	return m_listWaitGo->count();
}

// --------------------------------
sSimpleGoData* CUserManager::GetGoData(ugoid_t& uid, int& size, Bool bWait)
{
	sGeneralParam param={2, uid.b_id, uid.w_id, uid.day, uid.time, bWait, 0};
	int sock = yiudp(0, 0);

	yisendu(sock, &param, 32, m_server_addr, m_udpPort);

	yirecvu(sock, &param, 4, 0, 0, 0);

	if (param.param[0]==-1)
	{
		size=0;
		return 0;
	}

	size=param.param[0];
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

	return p;
}



// --------------------------------
// 接続待ち・予定のあるデータ
sSimpleGoHeader* CUserManager::GetWaitGoHeader(ubit32_t id, ugoid_t& _uid)
{
	int n=0;

	if (m_listWaitGo==0)
	{
		LoadWaitGoHeader();

		if (m_listWaitGo==0)
			return 0;
	}

	for (cyiNode* node=m_listWaitGo->GetNodeTopFrom(); node; node=node->np)
	{
		sSimpleGoHeader* data = (sSimpleGoHeader*)node->ref;

		if (data)
		{
			ugoid_t uid(0, 0);

			if (data->remoteId==id)
			{
				if ((data->result&0x80000000)!=0)
				{
					uid.b_id=m_myId;
					uid.w_id=data->remoteId;
				}
				else
				{
					uid.b_id=data->remoteId;
					uid.w_id=m_myId;
				}
			}
			uid.day=data->day;
			uid.time=data->time;

			if (uid==_uid)
			{
				return data;
			}
		}
	}

	return 0;
}

// --------------------------------
void CUserManager::RemoveWaitGoHeader(ubit32_t id, ugoid_t& _uid)
{
	int n=0;

	if (m_listWaitGo==0)
	{
		LoadWaitGoHeader();

		if (m_listWaitGo==0)
			return ;
	}

	for (cyiNode* node=m_listWaitGo->GetNodeTopFrom(); node; node=node->np)
	{
		sSimpleGoHeader* data = (sSimpleGoHeader*)node->ref;

		if (data)
		{
			ugoid_t uid(0, 0);

			if (data->remoteId==id)
			{
				if ((data->result&0x80000000)!=0)
				{
					uid.b_id=m_myId;
					uid.w_id=data->remoteId;
				}
				else
				{
					uid.b_id=data->remoteId;
					uid.w_id=m_myId;
				}
			}
			uid.day=data->day;
			uid.time=data->time;

			if (uid==_uid)
			{
				m_listWaitGo->remove(node);
				yifree(data);
				break;
			}
		}
	}

}


// --------------------------------
// 待ち碁をロード
void CUserManager::LoadWaitGoHeader()
{
	char path[256];
	int size;

	wsprintf(path, "%s\\5", m_szMyDir);
	sSimpleGoHeader*p = (sSimpleGoHeader*)yiFileLoadEx(path, &size);

	if (p==0)
	{
		if (m_listWaitGo==0)
			m_listWaitGo = new cyiList();

		return ;
	}

	int n = size>>4;

	if (m_listWaitGo==0)
	{
		m_listWaitGo = new cyiList();
	}

	for (int i=0; i<n; i++)
	{
		if (p[i].remoteId==0)
			continue;
		m_listWaitGo->addref(yitoheap(&p[i], sizeof(sSimpleGoHeader)));
	}

	if (p)
		yiFileUnLoadEx(p);//yifree(p);
}

// --------------------------------
// 待ち碁をロード
void CUserManager::SaveWaitGoHeader()
{
	char path[256];
	int size;

	wsprintf(path, "%s\\5", m_szMyDir);
	::yiRemove(path, True);

	if (m_listWaitGo==0)
		return ;

	for (cyiNode* node = m_listWaitGo->GetNodeTopFrom(); node; node=node->np)
	{
		sSimpleGoHeader* header = (sSimpleGoHeader*)node->ref;
		if (yiFileWriteEx(path, -1, header, sizeof(sSimpleGoHeader), True)==False)
		{
			if (yiFileCreateEx(path, -1, header, sizeof(sSimpleGoHeader), True)==False)
			{
				return ;
			}
		}
	}

}

// --------------------------------
// 接続待ち・予定のあるデータ
sSimpleGoHeader* CUserManager::GetWaitGoHeader(ubit32_t id, int index)
{
	int n=0;

	if (m_listWaitGo==0)
	{
		LoadWaitGoHeader();

		if (m_listWaitGo==0)
			return 0;
	}

	for (cyiNode* node=m_listWaitGo->GetNodeTopFrom(); node; node=node->np)
	{
		sSimpleGoHeader* data = (sSimpleGoHeader*)node->ref;

		if (data)
		{
			if (data->remoteId==id)
			{
				if (index==n)
				{
					return data;
				}
				n++;
			}
		}
	}

	return 0;
}

