#pragma once

#include "yi.h"
#include "yiQueue.h"
#include "uthp.h"
#include "svry/svrydefs.h"
#include "yiupnp.h"

#define YPN_DEFAULT_PORT 37710
#define YPN_TID_KEEP_ALIVE 0xFFFFFFFF




// ---------------------------
typedef struct xsypnUserInfo 
{
	unsigned int id;
	unsigned int index;
	unsigned int flag32;
	char szUserName[16];
	sNetworkInfo network;
}sypnUserInfo;

// ------------------------------------
enum eNotifyReservedMessage {
	eNRMSG_SERVER_CLOSED=0xFFFFFFFF,
	eNRMSG_CONNECTION_CLOSED=0xFFFFFFFE,	
	eNRMSG_WAIT_ERROR=0xFFFFFFFD,	
	eNRMSG_CONNECTION_FAILURE=0xFFFFFFFC,
	eNRMSG_DESCRIPTOR_FULL=0xFFFFFFFB,
	eNRMSG_ADD_USER=0xFFFFFFFA,
	eNRMSG_SYSTEM_ERROR=0xFFFFFFF9,
	eNRMSG_LOOP_EXIT=0xFFFFFFF8,
	eNRMSG_REMOVE_USER=0xFFFFFFF7,
	eNRMSG_RPY_DETAIL_INFO=0xFFFFFFF6
};

// ------------------------------------
enum eReservedTimerId {
	eRTID_KEEP_ALIVE=0xFFFFFFFF
};

// ------------------------------------
enum eReservedMessageMessage {
	eRMSG_CLOSE=0xFF
};

class __declspec(dllexport)  cyPrivateNetwork
{
public:
	cyPrivateNetwork(void);
	~cyPrivateNetwork(void);

	// ---------------------------
	static cyPrivateNetwork* GetInstance();

	// ---------------------------
	void InitNetwork(unsigned int address=0, int port=0);

	// ---------------------------
#ifdef WIN32
	BOOL Start(HWND hNotifyWnd, UINT msg, unsigned int server, int port, unsigned int security, unsigned int id, const char* pszName, const char* pszPassword, unsigned int localaddr=0);
#else
	BOOL Start(unsigned int event, unsigned int server, int port, unsigned int security, unsigned int id, const char* pszName, const char* pszPassword, unsigned int localaddr=0);
#endif

	// ------------------------------------
	Bool Send(socket_t sock, int id, void* buffer, int size);

	// ------------------------------------
	int Receive(socket_t sock, int* id, void* buffer, int* size);

	// ------------------------------------
	//int Receive(socket_t sock, void* buffer, int* size);

	// ------------------------------------
	int Receive(socket_t sock, void* buffer, int size, int nTimeout=-1);

	// ------------------------------------
	socket_t ConnectToServer(unsigned int server, int port, unsigned int security);

	// ------------------------------------
	void DisconnectFromServer(socket_t sock);


	// ------------------------------------
	void Terminate();

	// ------------------------------------
	const sypnUserInfo* GetUserInfo(int index);

	// ------------------------------------
	const sNetworkInfo* GetNetworkInfo();

	// ------------------------------------
	sypnUserInfo* GetMyInfo();

	// ------------------------------------
	Bool AcceptFrom(int sock, sypnUserInfo* remote);

	// ------------------------------------
	BOOL Connect(int Id, const char* pszUserName, int nRoomNo, const char* pszPassword, sypnUserInfo* info);

	// ------------------------------------
	Bool Connect(address_t addr, int port, sypnUserInfo* info);

	// ------------------------------------
	BOOL Connect(unsigned int address, int port, eNatType type, void* p);

	// ------------------------------------
	BOOL DisConnectUser(int index);

	// ------------------------------------
	BOOL DisConnectUser(socket_t sock);

	// ------------------------------------
	int GetServerSock();

	// ------------------------------------
	virtual Bool SendToServer(int id, void* data, int size, Bool bWait=True);

	// ------------------------------------
	virtual BOOL SendToUser(int index, int id, const void* data, int size, Bool bIsConfirm=True);

	// ------------------------------------
	BOOL SendToUser(socket_t sock, int id, const void* data, int size, Bool bIsConfirm=True);

	// ------------------------------------
	void*  GetFromQueue(socket_t sock, int* id, int* size);


	// ------------------------------------
	Bool Wakeup(int id, unsigned int data);

	// ------------------------------------
	Bool SetTimer(int id, int nTime, Bool bIsSoon=False, Bool bIsOnce=False);

	// ------------------------------------
	void StopTimer(int id);

	// ------------------------------------
	void GetEvent(unsigned int event, int*id, void** data);

	// ------------------------------------
	int GetUserIndex(unsigned int id, const char* pszName, int nRoomNo);

	// ------------------------------------
	void OnMsgLoop();

	// ---------------------------
	void OnNotifyFromMainUdp(int id, void* param);

	// ---------------------------
	void NotifyMessage(int id, void* data, Bool bIsDirect=False);

	// ---------------------------
	void NotifyServerMessage(int id, void* data, Bool bIsDirect=False);


	// ---------------------------
	void DestroySocket(socket_t sock);

	// ------------------------------------
	Bool WaitMessage(socket_t sock, int wid, int timeout, void* buffer, int*size);

	// --------------------
	void ConnectionClose(socket_t sock, int index=-1);


protected:
	// ---------------------------
	virtual void OnWakeUpMsg(int* buffer);

	// ---------------------------
	virtual void OnTimer(int id);

	// ---------------------------
	virtual Bool OnMessage(socket_t sock, int index, int id, void* buffer, int size);

	// ---------------------------
	virtual void OnAddUser(socket_t sock, int index, void* param);

	// ---------------------------
	virtual void OnRemoveUser(socket_t sock, int index);

	// ---------------------------
	virtual Bool OnLogin(void* buffer, int size);

	// ---------------------------
	int AddDescriptor(socket_t sock, sypnUserInfo* remote);

	// ---------------------------
	void RemoveDescriptor(socket_t sock, int index);

protected:
	socket_t m_MainSock;
	socket_t m_udpMainSock;
	socket_t m_AcceptSock;
	socket_t m_UdpAcceptSock;
	socket_t m_wakeupSock;
	socket_t m_socks[128];
	//sNetworkInfo m_MyNetwork;
	sypnUserInfo m_myinfo;
	Bool m_bStartKeepAlive;

	HandleUpnp m_hUpnp;
	Bool m_QuitMessage;
	int m_TimerList[32];
	HandleMutex m_hDescriptorMutex;

	int m_nLastError;

#ifdef WIN32
	HWND m_hNotifyWnd;	
	UINT m_NotifyMessageId;
	UINT m_NotifyServerMessageId;
#else
	unsigned int m_NotifyEvent;
	cyiQueue* m_NotifyEventQueue;
#endif
	// ---------------------------
	static cyPrivateNetwork* me;
private:
	// ---------------------------
	int FdSet(fd_set* fds);


public:
	// ---------------------------
	BOOL UpnpTest(const char* pszLocalAddress, int local_port, Bool* bReasonIsPort);

	// ---------------------------
	Bool OnNotifyFromServer(socket_t sock, int id, void* data, int size);

	// ---------------------------
	//Bool OnNotifyFromMainUdp(socket_t sock, sGeneralParam* param);

	// ------------------------------------
	// ëäéËÇ∆TCPÇ≈ê⁄ë±Ç∑ÇÈèÍçáÅiîpé~ó\íËÅHÅj
	socket_t ConnectRemote(address_t addres, int port, sypnUserInfo* info);

	// ------------------------------------
	socket_t StartHolePunching(socket_t sock, int port, int security, sypnUserInfo* remote);

	// ------------------------------------
	socket_t StartHolePunching(int sockUdp, address_t addr, int port, int security, sypnUserInfo* remote);


	// ------------------------------------
	void AddUdpMainSock(int sock);


	// ---------------------------
	Bool OnPreMessage(socket_t sock, syipacketu* packet, void* buffer);

	// ------------------------------------
	socket_t WaitAcceptMessage(int index, int timeout);

	// ---------------------------
	void SendKeepAlive();

	// ---------------------------
	socket_t OnAcceptClient(socket_t sock, sypnUserInfo* remote);



	// ---------------------------
	void OnTimerClose(int sock);


	// --------------------
	void Init();

#ifdef WIN32
	friend unsigned int __stdcall xOnTimer(void* param);
#else
	friend void* xOntimer(void* param);
#endif

};
