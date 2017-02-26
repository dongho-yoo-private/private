#pragma once


#include "yiGoApp.h"
#include "glsGoban.h"
#include "glsSystem.h"
#include "glsPopupMenu.h"
#include "yiThreadObject.h"

#include "yitList.h"
#include "UserManager.h"
#include "yiupnp.h"

#ifdef _GOYOTSUYA
#include "ypnGoNetwork.h"
#include "svry/svrydefs.h"
#include "svry/svrydefs.register.h"
#endif



typedef struct {
	sRegisterUserInfo info;
	int size;
} sUserCachedData;



class CGoEditorApp : public cyiGoApp, public cglsCommandCenter//, public cyiThreadObject
{
public:
	CGoEditorApp(const char* pszSoftwareName, const char* pszApplicationName, Bool bIsMultiple=0);
	~CGoEditorApp(void);

	// ------------------------
	//static unsigned int GetUserHash(const char* pszUserName);

	static void xOnSearchRouter(void* pHandleUpnp);


	Bool OnCreate(HWND hWnd);

	// ------------------------
	virtual Bool Initialize(const char* pszCommandLine=0);

	// ------------------------
	//virtual void OnApplicationDuplicated(HWND hWnd);

	// ------------------------
	virtual void OnMainWindowCreateAfter(const char* pszCommandLine=0);

	// ------------------------
	virtual Bool Finalize();

	// --------------------------------------
	virtual Bool OnCommand(int nSystemId, cGlassInterface* from, IntPtr param1=0, LongPtr param2=0);

	// --------------------------------------
	virtual Bool PostCommand(int nSystemId, cGlassInterface* from, unsigned int param1=0, unsigned int param2=0);


	// --------------------------------------
	void OnNodeChanged(WPARAM wParam, LPARAM lParam);

#ifdef _GOYOTSUYA

	// ------------------------
	Bool GetPropertyUser(const char* Key, char* value);

	// ------------------------
	Bool GetPropertyUserInt(const char* Key, int& value, int defaultValue=-1);

	// ------------------------
	Bool SetPropertyUser(const char* Key, char* value);

	// ------------------------
	Bool SetPropertyUserInt(const char* Key, int n);

	// --------------------------------
	Bool xSaveId(unsigned int id, const char* pszId, const char* pszPass);

	// --------------------------------
	unsigned int GetFromIdList(const char* id);

	// --------------------------------
	static unsigned int IdToInteger(const char* id);

	// --------------------------------
	static sMsg* CreateMsg(const wchar_t* pszMsg);

	// --------------------------------
	Bool PostMessageBox(sMsg* msg);

	// --------------------------------
	void OnRequestDataFrom(HandleUser hUser, int id, int sock, address_t addr, int port);

	// --------------------------------
	void OnSendDataToUser(HandleUser hUser, int id, int sock, address_t addr, int port, unsigned int seq);
#endif

private:
	// --------------------------------------
	void InitailizeHotKey();

	// --------------------------------------
	cglsPopupMenu* xCreateMenu();

#ifdef _GOYOTSUYA
	// --------------------------------
	void OnNetworkThread();

	// --------------------------------
	address_t xGetServerAddress(int* port);

	// --------------------------------
	//Bool xLoadMyFriendList(unsigned int sequnceNo);

	// --------------------------------
	Bool xLoadMyFriendList(unsigned int id, const char* pszMyId, unsigned int sequnceNo);

	// --------------------------------
	Bool xLogin(const char* szId, const char* pszPass, Bool bIsNormal=True);

	// --------------------------------
	Bool xUpdateMessage(cyiList* msg);

	// --------------------------------
	Bool xSaveGoData(ugoid_t* goid, unsigned int result);

	// --------------------------------
	Bool HolePunching(int sock, address_t addr, int port);

	// --------------------------------
	void FindRouter();

	cyiQueue m_qWait;

	// --------------------------------
	Bool xOnRequestDataFrom(HandleUser hUser, sGeneralParam* param);

	// --------------------------------
	Bool RequestDataFrom(HandleUser hUser, int id, unsigned int seq);

	// --------------------------------
	void OnDataReceived(HandleUser hUser, int id, void* p, int size, unsigned int seq);

	// --------------------------------
	Bool SendDataToUser(HandleUser hUser, int id, address_t addr, int port, unsigned int seq);

	// --------------------------------
	unsigned int xRegisterUser(char* szUserName, char* szPassword);

	// --------------------------------------
	void SendBroadCast(ugoid_t& uid, int packetId, void* data, int size);


	// --------------------------------
	Bool StopTimer(unsigned int goid);

	// --------------------------------
	Bool SaveGoData(int index, sGoHeader* header, char* pszFileName, map_t map, int lastOrder);

	// --------------------------------
	// result: -1:lose, 0:draw, 1:win, 0xFF:canceled.
	Bool SaveGoDataEx(unsigned int goid);//, int result);

	// --------------------------------
	Bool Msg32ToMessage(sMsg32* msg32, int index, sMsg* msg, wchar_t* buffer);

	// --------------------------------
	void MakeMessage(sMsg* msg, int type, unsigned int remoteId, const char* pszUserName, const wchar_t* txt);

	// --------------------------------
	//virtual void OnThreadStart(int id, int param1, int param2);

	// --------------------------------
	Bool xAddMyFriend(sRegisterUserInfo* user);

	// --------------------------------
	Bool LoadDictionary();

	// --------------------------------
	Bool AddDictionary(unsigned int id, const char* pszUserName , int flag);

	// --------------------------------
	const char* GetUserName(unsigned int id);

	// --------------------------------
	unsigned int GetUserId(const char* pszUserName, Bool bSearchNetwork=True);

	// --------------------------------
	Bool AddCached(sRegisterUserInfo* user, const char* pszImageFileName=0);

	// --------------------------------
	void OnConnectFriends();

	// --------------------------------
	void ConnectMyFriends();

	// --------------------------------
	Bool QueryUserName(unsigned int id, char* pszOutUserName);

	// --------------------------------
	sSimpleGoData* GetGoData(ugoid_t& id, int* size);

	// --------------------------------------
	cypnGoNetwork* m_network;


	// --------------------------------------
	//char m_szId[32];

	// --------------------------------------
	char m_szUserDir[256];

	// --------------------------------------
	char m_szMyDir[256];

	// --------------------------------------
	Bool m_bIsLoginSuccess;



	sUserDictionaryFile m_dictionary;

	// --------------------------------------
	Bool xRegisterUpnpPort();

	// --------------------------------------
	Bool xGetRemoteAbsAddr(unsigned int userId, address_t* addr, int* port);

	// --------------------------------------
	void xOnMainUdpPacket(int id, void* p);


	// --------------------------------------
	void xRemovePlayingGo(unsigned int remoteId, ugoid_t& uid, Bool bDataDelete);
#endif
	// Commands
	Bool OnReqPlayDialog(cGlassInterface* from, unsigned int param1, unsigned int param2);



public:
	Bool				m_bIsSizeDefault;
	cglsGoban*			m_goban;
	HWND				m_hWnd;
	cGlassInterface*	m_main;
	cglsSystem*			m_system;

	Bool m_bIsNoUser;

#ifdef _GOYOTSUYA
	address_t m_server_addr;
	int m_server_port;
	CUserManager* m_user;
	// ------------------------------
	unsigned int m_myId;

	// ------------------------------
	char m_szMyId[16];

	sSimpleYgfHeader* m_goHeaders;

	int m_goHeaderSize;
#endif


private:
#ifdef _GOYOTSUYA
	// --------------------------------
	Bool m_bIsLoginNormal;

	// --------------------------------
	friend void xOnConnectMyFriend(void* param);

	// --------------------------------
	HandleUpnp m_hUpnp;

	// --------------------------------
	Bool m_bIsFindRouterDone;

	// --------------------------------
	address_t m_myAbsAddr;

	// --------------------------------------
	void OnSearchRouter();
#endif

#ifdef _GOMAIL
private:
	// ------------------------
	Bool CreateGomailMainView();

	// ------------------------
	Bool ShowEmailSetup();
#endif

};

extern CGoEditorApp* xGetMyApp();

