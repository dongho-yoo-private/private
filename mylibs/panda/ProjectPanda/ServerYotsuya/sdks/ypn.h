#pragma once

#include "yi.h"
#include "yiQueue.h"
#include "uthp.h"
#include "svry/svrydefs.h"
#include "yiupnp.h"

#define YPN_DEFAULT_PORT 33310
#define YPN_TID_KEEP_ALIVE 0xFFFFFFFF


// ---------------------------
typedef struct xsypnUserInfo 
{
	unsigned int id;
	char szUserName[16];
	char szTemporaryName[16];
}sypnUserInfo;

// ---------------------------
typedef struct xsypnUserInfox
{
	unsigned int id;
	int area_code;
	char szUserName[16];
	sNetworkInfo net;
}sypnUserInfox;

typedef Handle HandleUser;

// ------------------------------------
enum eNotifyReservedMessage {
	eNRMSG_SERVER_CLOSED=0xFFFFFFFF,
	eNRMSG_CONNECTION_CLOSED=0xFFFFFFFE,	
	eNRMSG_WAIT_ERROR=0xFFFFFFFD,	
	eNRMSG_CONNECTION_FAILURE=0xFFFFFFFC,
	eNRMSG_DESCRIPTOR_FULL=0xFFFFFFFB,
	eNRMSG_ADD_USER=0xFFFFFFFA,
	eNRMSG_SYSTEM_ERROR=0xFFFFFFF9,
	eNRMSG_LOOP_EXIT=0xFFFFFFF8
};

// ------------------------------------
enum eReservedTimerId {
	eRTID_KEEP_ALIVE=0xFFFFFFFF
};

// ------------------------------------
enum eReservedMessageMessage {
	eRMSG_CLOSE=0xFF
};

class cyPrivateNetwork
{
public:
	cyPrivateNetwork(void);
	~cyPrivateNetwork(void);

	// ---------------------------
	static cyPrivateNetwork* GetInstance();

	// ---------------------------
	void InitNetwork(unsigned int address=0, int port=0);

	// ---------------------------
	// changed. 2010.2.10.
#ifdef WIN32
	BOOL Start(HWND hNotifyWnd, unsigned int server, int port, unsigned int security, unsigned int id, const char* pszName, const char* pszPassword, unsigned int localaddr=0);
	BOOL Start(HWND hNotifyWnd, unsigned int msgId, unsigned int server, int port, unsigned int security, 
					sypnUserInfo* user, const char* pszPassword, const char* pszTemporaryUserName=0, unsigned int localaddr=0);
#else
	BOOL Start(unsigned int event, unsigned int server, int port, unsigned int security, unsigned int id, const char* pszName, const char* pszPassword, unsigned int localaddr=0);
#endif
	// ------------------------------------
	// added.
	static const char* GetUserName(HandleUser hUser);

	// ------------------------------------
	// added.
	static const char* GetUserTemporaryName(HandleUser hUser);

	// ------------------------------------
	void Terminate();

	// ------------------------------------
	const sypnUserInfo* GetUserInfo(int index);

	// ------------------------------------
	const sNetworkInfo* GetNetworkInfo();

	// ------------------------------------
	// changed. 2010.2.10.
	HandleUser Connect(int Id, const char* pszUserName, int nRoomNo, const char* pszPassword);

	// ------------------------------------
	HandleUser Connect(unsigned int address, int port, eNatType type, void* p);

	// ------------------------------------
	BOOL DisConnectUser(HandleUser user);

	// ------------------------------------
	// changed.
	BOOL SendToUser(HandleUser hUser, int id, char* data, int size);

	// ------------------------------------
	Bool Wakeup(int id, unsigned int data);

	// ------------------------------------
	Bool SetTimer(int id, int nTime, Bool bIsSoon=False, Bool bIsOnce=False);

	// ------------------------------------
	void StopTimer(int id);

	// ------------------------------------
	void GetEvent(unsigned int event, int*id, void** data);

	// ------------------------------------
	void OnMsgLoop();

protected:
	// ---------------------------
	virtual void OnWakeUpMsg(int* buffer);

	// ---------------------------
	virtual void OnTimer(int id);

	// ---------------------------
	virtual Bool OnMessage(socket_t sock, int id, void* buffer);

private:
	socket_t m_MainSock;
	socket_t m_AcceptSock;
	socket_t m_UdpAcceptSock;
	socket_t m_wakeupSock;
	socket_t m_socks[128];
	sNetworkInfo m_MyNetwork;
	sypnUserInfo m_MyInfo;
	Bool m_bStartKeepAlive;

	HandleUpnp m_hUpnp;
	Bool m_QuitMessage;
	int m_TimerList[32];
#ifdef WIN32
	HWND m_hNotifyWnd;	
#else
	unsigned int m_NotifyEvent;
	cyiQueue* m_NotifyEventQueue;
#endif
	// ---------------------------
	static cyPrivateNetwork* me;
private:
	// ---------------------------
	int FdSet(fd_set* fds);

	// ---------------------------
	Bool AddDescriptor(socket_t sock);

	// ---------------------------
	void RemoveDescriptor(socket_t sock, int index);

	// ---------------------------
	BOOL UpnpTest(const char* pszLocalAddress, int local_port, Bool* bReasonIsPort);

	// ---------------------------
	Bool OnNotifyFromServer(socket_t sock, int id, void* data);

	// ------------------------------------
	socket_t ConnectRemote(address_t addres, int port);

	// ------------------------------------
	socket_t StartHolePunching(socket_t sock, int port, int security);

	// ------------------------------------
	Bool WaitMessage(socket_t sock, int wid, int timeout, void* buffer, int*size);

	// ---------------------------
	Bool OnPreMessage(socket_t sock, syipacketu* packet, void* buffer);

	// ------------------------------------
	socket_t WaitAcceptMessage(int index, int timeout);

	// ---------------------------
	void SendKeepAlive();

	// ---------------------------
	socket_t OnAcceptClient(socket_t sock);

	// ---------------------------
	void NotifyMessage(int id, void* data);

	// ---------------------------
	void OnTimerClose(int sock);

	// --------------------
	void ConnectionClose(socket_t sock);

	// --------------------
	void Init();

	// --------------------
	HandleUser CreateUserHandle(socket_t sock);


#ifdef WIN32
	friend void xOnTimer(void* param);
#else
	friend void* xOntimer(void* param);
#endif

};
