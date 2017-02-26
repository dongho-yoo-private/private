#pragma once

#include "yistd.h"
#include "yipop3.h"

typedef struct {
	unsigned int id;
	char displayName[32];
	char email[80];
	char popserver[32];
	char user[16];
	char pass[16];
	char charset[16];
	int  port;
	Bool bUseSsl;
	HandleMutexx hMutex;
} sPopServer;

// -----------------------
class cGoMailManager
{
public:
	cGoMailManager(sPopServer* svr, const char* pszRootdir);
	~cGoMailManager(void);

	// -----------------------
	static int Create(sPopServer* svr, const char* pszRootdir);

	// -----------------------
	static cGoMailManager* GetInstance(int id);


	// -----------------------
	static Bool RegisterNotifyWindow(HWND hWnd, UINT msg, UINT msgSmtp=0);

	// -----------------------
	static Bool UnregisterNotifyWindow(HWND hWnd);

	// -----------------------
	static Bool LoadEmailAddress(const char* pszPath);

	// -----------------------
	static Bool SaveEmailAddress(const char* pszPath);


	// -----------------------
	Bool SendPlayRequest(const char* pszEmailAddress, unsigned int condition, unsigned int condition2, unsigned int flag32);

	// -----------------------
	// sgf+zip format.
	void SendGoMessage(const char* pszSubject, void* p, int size, const char* to, cyiList* cc, HWND hWnd, UINT msg);

	// -----------------------
	// ngf format.
	void SendStone(const char* pszSubject, const char* pszRemoteAddress, void* p, int size, const char* to, cyiList* cc);

	// -----------------------
	Bool CheckEmail();

	// -----------------------
	unsigned short AddMailAddress(const char* pszEmail, const char* pszDisplayName);

	// -----------------------
	const char* GetDisplayName(const char* pszEmail);

	// -----------------------
	const char* GetEmailAddress(unsigned short id, char* displayName);



	// -----------------------
	cyiList* GetRegisteredUserList();

private:
	static cGoMailManager* m_me[16];
	static cyiList* m_mailz;
	static cyiList* m_windows;

	// -----------------------
	void xCheckEmail();

	// -----------------------
	friend void xOnCheckEmail(void* p);

	// -----------------------
	Bool NotifyMessage(int id, void* hPop);

	// -----------------------
	sPopServer m_server;

	// -----------------------
	cyiList* m_listMailBox;

	// -----------------------
	cyiList* m_uidz;


	// -----------------------
	Bool SaveMailBox();

	// -----------------------
	Bool SaveUidList();

	// -----------------------
	int AddMailBox(sMailFormat* data, unsigned int type, const char* pszTitle, const char* originalUid);

	// -----------------------
	int AddUidList(const char* uid);

	// -----------------------
	Bool IsUidExsist(const char* uid);

	// -----------------------
	Bool LoadMailBox();

	// -----------------------
	Bool LoadUidList();

	char m_szBaseAddress[256];

};
