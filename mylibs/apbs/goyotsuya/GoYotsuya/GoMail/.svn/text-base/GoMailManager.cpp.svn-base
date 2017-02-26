#include "GoMailManager.h"

//typedef struct {
//	unsigned int hashId;
//	char id[64];
//} sMailFileFormat;

cyiList* cGoMailManager::m_mailz=0;
cyiList* cGoMailManager::m_windows=0;


#define _MAIL_GO_HEADER_ "##GOYOTSUYAMAIL:"
#define _MAIL_GO_VERSION_  "VER.##:"
#define _MAIL_GO_SUBJECT_LEN_ 17
#define _MAIL_GO_VERSION_LEN_ 8


#define _MAILGO_HEADER_ "Go!Yotsuya MailGo"

// --------------------------------
cGoMailManager* cGoMailManager::m_me[16]={0,};

// --------------------------------
#include "yiHashFunc.h"
static unsigned int xCreateHashCode(const char* uid)
{
	unsigned int n1 = yihf_additional((char*)uid, 8);
	unsigned int n2 = yihf_encoded_additional((char*)uid, 8);
	unsigned int n3 = yihf_general(uid, 8);
	unsigned int n4 = yihf_sub(uid, 8);

	return __COLORp__(n1, n2, n3, n4);
}


// ----------------------
int cGoMailManager::Create(sPopServer* svr, const char* pszRootDir)
{
	for (int i=0; i<16; i++)
	{
		if (m_me[i]==0)
		{
			m_me[i] = new cGoMailManager(svr, pszRootDir);
			return i;
		}
	}
	return -1;
}


// -----------------------
cGoMailManager::cGoMailManager(sPopServer* svr, const char* pszRootdir)
:m_listMailBox(0)
,m_uidz(0)
{
	m_server=*svr;
	strcpy(m_szBaseAddress, pszRootdir);
	m_server.hMutex=::yiMutexCreateEx(0);
	yiGetSystemLocale(m_server.charset);

	m_listMailBox = new cyiList();
}

// -----------------------
cGoMailManager::~cGoMailManager(void)
{
}



// -----------------------
Bool cGoMailManager::LoadEmailAddress(const char* pszPath)
{
	char path[256];
	wsprintf(path, "%s\\email.list", pszPath);
	
	m_mailz = new cyiList(True);
	m_mailz->Import(pszPath, 0, strlen(_MAILGO_HEADER_));
	return True;
}

// -----------------------
Bool cGoMailManager::SaveEmailAddress(const char* pszPath)
{
	m_mailz->Export(pszPath, (int)sizeof(sMailList), (void*)_MAILGO_HEADER_, strlen(_MAILGO_HEADER_));
	return True;
}


// -----------------------
cGoMailManager* cGoMailManager::GetInstance(int index)
{
	return m_me[index];
}

typedef struct {
	HWND hWnd;
	UINT msg;
	UINT msg2;
} sWindowMessage;


// -----------------------
Bool cGoMailManager::RegisterNotifyWindow(HWND hWnd, UINT msg, UINT msgSmtp)
{
	sWindowMessage s = {hWnd, msg, msgSmtp};
	
	if (m_windows==0)
	{
		m_windows = new cyiList(True);
	}

	m_windows->lock();
	for (cyiNode* node=m_windows->GetNodeTopFrom(); node; node=node->np)
	{
		sWindowMessage* s = (sWindowMessage*)node->ref;

		if (s->hWnd==hWnd)
		{
			m_windows->unlock();
			return False;
		}
	}

	m_windows->addref(yitoheap(&s, sizeof(s)));
	m_windows->unlock();
	return True;
}

// -----------------------
Bool cGoMailManager::UnregisterNotifyWindow(HWND hWnd)
{
	if (m_windows==0)
	{
		return False;
	}

	m_windows->lock();
	for (cyiNode* node=m_windows->GetNodeTopFrom(); node; node=node->np)
	{
		sWindowMessage* s = (sWindowMessage*)node->ref;

		if (s->hWnd==hWnd)
		{
			m_windows->remove(node, 0);
			yifree(s);
			m_windows->unlock();
			return True;
		}
	}
	m_windows->unlock();

	return False;
}

#include "yiencodex.h"

#define _SECURITY_KEY_ 0xe2a1d375


#include "yipop3.h"

// -----------------------
Bool cGoMailManager::NotifyMessage(int id, void* hPop)
{
	if (!m_windows)
		return False;
	m_windows->lock();

	for (cyiNode* node = m_windows->GetNodeTopFrom(); node; node=node->np)
	{
		sWindowMessage* msg = (sWindowMessage*)node->ref;
		PostMessage(msg->hWnd, msg->msg, id, (LPARAM)hPop);
	}
	m_windows->unlock();

	return True;
}

// -----------------------
void xOnCheckEmail(void* p)
{
	cGoMailManager* pClass = (cGoMailManager*)p;
	pClass->xCheckEmail();
	_endthread();
}



// -----------------------
void cGoMailManager::xCheckEmail()
{	
	sPopServer* s = &m_server;

	if (s->hMutex==0)
	{
		// のんびりでよし。
		m_server.hMutex =yiMutexCreateEx(0);
	}

	yiMutexLockEx(s->hMutex);

	address_t addr = yihtol(s->popserver, 0);

	HandlePop3 hPop = yipop3Create(addr, s->port, s->user, s->pass, s->bUseSsl);

	if (hPop==0)
	{
		NotifyMessage(-1, 0);
		yiMutexUnlockEx(s->hMutex);
		return ;
	}

	int new_mail_count=0;
	int cnt = yipop3Stat(hPop, 0);

	HandleList hList = ::yipop3UniqIdListening(hPop);
	cyiStack stack;
	stack.clear();

	for (syiNode* node = yiListGetTopFrom(hList, 0); node; node=node->np)
	{
		sMailList* mail = (sMailList*)node->ref;
		
		if (IsUidExsist(mail->uuid)==True)
		{
			continue;
		}
		sMailFormat* data = yipop3Get(hPop, mail->id);

		// タイトルを見て関係しているかをチェックする。
		if (data->subject)
		{
			int len = strlen(data->subject);

			if (len<(_MAIL_GO_SUBJECT_LEN_+_MAIL_GO_VERSION_LEN_))
			{
				AddUidList(data->uid);
				continue;
			}


			// subject format.
			// ##GOYOTSUYAMAIL:XXXXX VS XXXXXX
			// body.
			// request, putstone, msg
			// ex)
			// putstone+###########################

			if (memcmp(data->subject, _MAIL_GO_HEADER_, _MAIL_GO_SUBJECT_LEN_)==0)
			{
				unsigned int type=0;
				char* pszTitle = data->subject+_MAIL_GO_SUBJECT_LEN_;
				int n=0;

				char version[_MAIL_GO_VERSION_LEN_];
				int  nVer=0;
				memcpy(version, pszTitle, _MAIL_GO_VERSION_LEN_-1);
				version[_MAIL_GO_VERSION_LEN_-1]=0;
				nVer = atoi(&version[4]);

				pszTitle+=_MAIL_GO_VERSION_LEN_;

				if (strncmp(data->body, "request", (n=strlen("request")))==0)
				{
					type=1;
				}
				else if (strncmp(data->body, "putstone",(n=strlen("putstone")) )==0)
				{
					type=2;
				}
				else if (strncmp(data->body, "msg", (n=strlen("putstone")) )==0)
				{
					type=3;
				}
				else
				{
					this->AddUidList(data->uid);
					continue;
				}

				char* pOriginalUid=data->body+n+1;

				AddMailBox(data, type, pszTitle, pOriginalUid);
				
				if (*pOriginalUid==0)
				{
					AddUidList(data->uid);
					continue;
				}
				
				yiPop3Delete(hPop, mail->id);

				continue;

			}
			else
			{
				AddUidList(data->uid);
				continue;
			}

		}
		else
		{
			AddUidList(data->uid);
			continue;
		}

		yipop3Release(data);
	}


	yiMutexUnlockEx(s->hMutex);

	return ;
}

// -----------------------
Bool cGoMailManager::CheckEmail()
{
	_beginthread(xOnCheckEmail, 0, (void*)this);
	return True;
}



typedef struct {
	unsigned int hash;
	unsigned short id;
	char email[81];
	unsigned char contrycode;
	unsigned short reserved;
	char displayname[32];
}sEmailAddress;

// -----------------------
unsigned short cGoMailManager::AddMailAddress(const char* pszEmail, const char* pszDisplayName)
{
	unsigned int hash = xCreateHashCode(pszEmail);
	unsigned short id=0;
	m_mailz->lock();
	for (cyiNode* node = m_mailz->GetNodeTopFrom(); node; node=node->np)
	{
		sEmailAddress* addr = (sEmailAddress*)node->ref;
		if (hash==addr->hash)
		{
			if (strcmp(addr->email, pszEmail)==0)
			{
				if (pszDisplayName!=0 && pszDisplayName[0]!=0)
				{
					strcpy(addr->displayname, pszDisplayName);
					m_mailz->unlock();
					return addr->id;
				}
			}
		}
		id = id<addr->id?addr->id:id;
	}

	// 新たに登録
	sEmailAddress* addr = (sEmailAddress*)yialloci(sizeof(*addr));
	strcpy(addr->email, pszEmail);
	strcpy(addr->displayname, pszDisplayName);
	addr->id=++id;
	m_mailz->addref(addr);
	m_mailz->unlock();

	return m_mailz->count();
}

// -----------------------
const char* cGoMailManager::GetEmailAddress(unsigned short id, char* displayName)
{
	m_mailz->lock();
	for (cyiNode* node = m_mailz->GetNodeTopFrom(); node; node=node->np)
	{
		sEmailAddress* addr = (sEmailAddress*)node->ref;
		if (id==addr->id)
		{
			if (addr->displayname)
			{
				strcpy(displayName, addr->displayname);
			}
			m_mailz->unlock();
			return addr->email;
		}
	}
	m_mailz->unlock();
	
	if (displayName)
		*displayName=0;
	return 0;
}


// -----------------------
const char* cGoMailManager::GetDisplayName(const char* pszEmail)
{
	unsigned int hash = xCreateHashCode(pszEmail);
	m_mailz->lock();
	for (cyiNode* node = m_mailz->GetNodeTopFrom(); node; node=node->np)
	{
		sEmailAddress* addr = (sEmailAddress*)node->ref;
		if (hash==addr->hash)
		{
			if (strcmp(addr->email, pszEmail)==0)
			{
				m_mailz->unlock();
				return addr->displayname;
			}
		}
	}


	m_mailz->unlock();

	return 0;
}


#include "yismtp.h"
// -----------------------
// sgf+zip format.

void cGoMailManager::SendGoMessage(const char* pszSubject, void* p, int size, const char* to, cyiList* cc, HWND hWnd, UINT msg)
{
	address_t addr = ::yihtol("smtp.mail.yahoo.co.jp", 0);
	HandleSmtp hSmtp = ::yiSmtpCreate(addr, 587, "goyotsuya", "9334080");

	if (hSmtp==0)
		return ;

	char buffer[256];
	wsprintf(buffer, "%sVER.%2d:%s", _MAIL_GO_HEADER_, pszSubject);
	::yiSmtpSetSubject(hSmtp, buffer, m_server.charset);
	::yiSmtpAddFile(hSmtp, p, size, "igo.nfz", "application/goyotsuya", "base64"); 


	const char* pszDisplayName = GetDisplayName(to);
	yiSmtpAddTo(hSmtp, to, pszDisplayName, m_server.charset);

	for (cyiNode* node=cc->GetNodeTopFrom(0); node; node=node->np)
	{
		const char* pAddr = (const char*)node->ref;
		pszDisplayName = GetDisplayName(to);
		
		yiSmtpAddCc(hSmtp, pAddr, pszDisplayName, m_server.charset);
	}

	yiSmtpSetBody(hSmtp, "msg", "text/plain", m_server.charset, "8bit");
	pszDisplayName = GetDisplayName(m_server.email);
	yiSmtpSetFrom(hSmtp, m_server.email, pszDisplayName, m_server.charset);
	yiSmtpSetReply(hSmtp, m_server.email, pszDisplayName, m_server.charset);

	yiSmtpSendEx(hSmtp, True, hWnd, msg);

	return ;
}

// -----------------------
// ngf format.
void cGoMailManager::SendStone(const char* pszSubject, const char* pszRemoteAddress, void* p, int size, const char* to, cyiList* cc)
{
	return ;
}


typedef struct {
	char uid[64];
	unsigned int hash;
}sMailUidList;

typedef struct {
	char uid[64];
	unsigned int hash;
	int type;
	char szTitle[128];
	unsigned short from;
	unsigned short to[16];
	unsigned short cc[16];
} sMailBoxList;


// -----------------------
Bool cGoMailManager::SaveMailBox()
{
	char path[256];
	wsprintf(path, "%s\\box-%s.gml", m_server.email, m_szBaseAddress);

	if (m_listMailBox)
	{
		m_listMailBox->Export(path, sizeof(sMailBoxList),(void*)_MAILGO_HEADER_, strlen(_MAILGO_HEADER_));
	}
	return True;
}

// -----------------------
Bool cGoMailManager::SaveUidList()
{
	char path[256];
	wsprintf(path, "%s\\uid-%s.gml",m_server.email, m_szBaseAddress);

	if (m_uidz)
	{
		m_uidz->Export(path, sizeof(sMailUidList), (void*)_MAILGO_HEADER_, strlen(_MAILGO_HEADER_));
	}
	return True;
}

// -----------------------
int cGoMailManager::AddMailBox(sMailFormat* data, unsigned int type, const char* pszTitle, const char* originalUid)
{
	if (m_listMailBox==0)
	{
		m_listMailBox = new cyiList();
	}

	Bool bHasOriginalUid = *originalUid?True:False;
	
	sMailBoxList s;
	s.hash=xCreateHashCode(data->uid);
	strcpy(s.szTitle, pszTitle);

	s.type=type;
	s.type|=bHasOriginalUid?0x40000000:0;

	s.type|=0x80000000; // new.

	s.from=AddMailAddress(yistrzGet(data->from, 1, 0), ::yistrzGet(data->from, 1, 0));

	if (data->cc)
	{
		int cnt = yistrzpCount(data->cc);
		if (cnt>16)
			cnt=16;

		for (int i=0; i<cnt; i++)
		{
			strz str = yistrzpGet(data->cc, i);
			s.cc[i]=AddMailAddress(yistrzGet(str, 1, 0), yistrzGet(str, 0, 0));
		}
	}

	if (data->to)
	{
		int cnt = yistrzpCount(data->to);

		if (cnt>16)
			cnt=16;

		for (int i=0; i<cnt; i++)
		{
			strz str =  yistrzpGet(data->to, i);
			s.to[i]=AddMailAddress(yistrzGet(str, 1, 0), yistrzGet(str, 0, 0));
		}
	}

	strcpy(s.uid, data->uid);

	char path[256];
	
	if (bHasOriginalUid)
	{
		wsprintf("%s.mlx", originalUid);
	}
	else
	{
		wsprintf("%s.mlx", data->uid);
	}

	if (::yiFileWriteEx(path, 0, data->binary, data->size, True)==False)
	{
		return False;
	}



	sMailBoxList* pNew=(sMailBoxList* )yitoheap(&s, sizeof(s));
	m_listMailBox->addref(pNew);

	// 1 is added.
	NotifyMessage(1, pNew);

	return m_listMailBox->count();
}

// -----------------------
int cGoMailManager::AddUidList(const char* uid)
{
	if (m_uidz==0)
	{
		m_uidz = new cyiList();
	}

	sMailUidList* x = (sMailUidList*)yialloc(sizeof(sMailUidList));

	strcpy(x->uid, uid);
	x->hash = xCreateHashCode(uid);

	m_uidz->addref(x);
	return m_uidz->count();
}

// -----------------------
Bool cGoMailManager::IsUidExsist(const char* uid)
{
	unsigned int hash = xCreateHashCode(uid);

	for (cyiNode* node=m_uidz->GetNodeTopFrom(0); node; node=node->np)
	{
		sMailUidList* u = (sMailUidList*)node->ref;
		if (u->hash==hash)
		{
			if (strcmp(u->uid, uid)==0)
			{
				return True;
			}
		}
	}

	return False;
}



// -----------------------
Bool cGoMailManager::LoadMailBox()
{
	int size=0;
	int n=0;
	char path[256];
	wsprintf(path, "%s\\box.gml", m_szBaseAddress);

	if (m_uidz)
	{
		int cnt = m_listMailBox->count();
		for (int i=0; i<cnt; i++)
			yifree(m_listMailBox->removeEx(0));
		delete m_listMailBox;
	}

	m_listMailBox = new cyiList();
	m_listMailBox->Import(path, 0, 0);
	m_listMailBox->Export(path, sizeof(sMailBoxList),(void*)_MAILGO_HEADER_, strlen(_MAILGO_HEADER_));

	return True;
}



// -----------------------
Bool cGoMailManager::LoadUidList()
{
	int size=0;
	int n=0;
	char path[256];
	wsprintf(path, "%s\\uidz.gml", m_szBaseAddress);

	sMailUidList*p = (sMailUidList*)yiFileLoadEx(path, &size);

	if (p==0)
		return False;

	if (m_uidz)
	{
		int cnt = m_uidz->count();
		for (int i=0; i<cnt; i++)
			yifree(m_uidz->removeEx(0));
		delete m_uidz;
	}

	m_uidz = new cyiList();
	m_uidz->Import(path, 0, 0);

	return True;
}
