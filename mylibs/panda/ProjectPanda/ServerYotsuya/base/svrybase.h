#ifndef __SERVER_YOTSUYA__

#define __SERVER_YOTSUYA__

#ifndef WIN32
#include <assert.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#else
#include <windows.h>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib");
#endif

#include "yi.h"
#include "yithread.h"
#include "yiEncoder.h"
#include "yitrace.h"
#include "svryconn.h"
#include "svryconnmgr.h"
#include "svryenv.h"
#include "yisock.h"
#include "yiencodex.h"

#ifdef WIN32
#undef SYSLOG
#define SYSLOG
#endif

#ifdef WIN32
#define __close__ closesocket
#else
#define __close__ close
#endif

#define ssyNormalPacket syipacket

// --------------------------------------------
class csvrybase
{
public:
	csvrybase(const char* pszServerName);
	~csvrybase();

	// -------------------------------------
	//static Bool LoadServerInformation(const char* pszDefnitionFileName, sServerInformation* server, sServerInformation* local);

	// -------------------------------------
	Bool Start(sServerInformation* server=0, sServerInformation* local=0);


protected:
	// -------------------------------------
	long CreateServerSession(sServerInformation& info);

	// -------------------------------------
	long CreateLocalSession(sServerInformation& info);

	// -------------------------------------
	Bool CreditWrite(const char* pszFileName, const void* buffer, int size, int check_sum_interval);

	// -------------------------------------
	Bool CreditAppendRecord(const char* pszFileName, const void* buffer, int size, int check_sum_interval);

	// -------------------------------------
	Bool Loop();

	// -----------------------------------------------
	void NotifyError(unsigned int id, int error, void* extra);
protected:
	// -------------------------------------
	virtual Bool OnError(int error, unsigned int userId, void* p);

	// -------------------------------------
	virtual Bool OnPreAcceptLocal(int sock);

	// -------------------------------------
	virtual Bool OnPreAccept(int sock, unsigned int from);

	// -------------------------------------
	virtual Bool OnStatus(char* buffer);

	// -------------------------------------
	virtual Bool OnPacket(int sock, Bool bIsLocal, Bool& bIsConnectionClosed);

public:
	// -------------------------------------
	virtual Bool OnAccepted(int sock, int index, void** pExtra);

	// -----------------------------------
	virtual void OnClientClose(int sock, void** pExtraDataRef);

protected:
	//
	virtual Bool OnPreMessage(int sock, void** pExtraData, Bool& bHandled);

	//
	virtual Bool OnMessage(int sock, ssyNormalPacket& packet, void** pExtraData=0);

	//
	virtual Bool OnMessagex(int sock, int id, void* data, int size, void** pExtraData=0)=0;

	//
	virtual Bool OnMessageLocal(int sock, ssyNormalPacket& packet, void** pExtraData=0);
	
	//
	virtual Bool OnTeminate(int sock, csvryConnMgr& mgr);

	//
	virtual Bool OnServerStart()=0;

	//
	virtual Bool OnServerEnd()=0;

private:
	void Init();

	// svrybase.Create.cpp
	int Accept(int listenSock, int pipefd, Bool& isTerminate, unsigned int* from=0);


	// svrybase.Dispatch.cpp
	void OnDispatch(unsigned long param1, unsigned long param2, unsigned long param3);
friend void* xOnDispatch(void* param);

	//
	void OnLimitedDispatch(unsigned long param1, unsigned long param2, unsigned long param3);
friend void* xOnLimitedDispatch(void* param);

	//
	void OnLoopNormal(unsigned long param1, unsigned long param2, unsigned long param3);

friend void* xOnLoopNormal(void* param);

	// 
	void OnLoopLimited(unsigned long param1, unsigned long param2, unsigned long param3);

friend void* xOnLoopLimited(void* param);
	// 
	void QuitMessage(int mode);

	//
	void OnCreditWrite(unsigned long param1, unsigned long param2, unsigned long param3);

	//
	void OnCreditAppendRecord(unsigned long param1, unsigned long param2, unsigned long param3);
protected:
	// -----------------------------------
	char m_szServerName[64];
	//unsigned long m_security;

	// -----------------------------------
	sServerInformation m_ServerInfo;
	sServerInformation m_LocalInfo;

	cyiList m_FileErrorList;

private:

	// -----------------------------------
	pthread_t m_LoopThread_t ;
	pthread_t m_LocalLoopThread_t;

protected:
	// -----------------------------------
	csvryConnMgr* m_LocalConMgr;
	csvryConnMgr* m_GlobalConMgr;

	int m_nConnectionCount;
	int m_nPeekConnectionCount;
	int m_nConMgrCount;
private:
	// -----------------------------------
	int m_LocalSock;
	int m_Sock;
	int m_QuitMode;
	int m_LocalLoopFd;
	int m_ServerLoopFd;


	// delete?
	Bool m_bTerminate;
};

#endif

