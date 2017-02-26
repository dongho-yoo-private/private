#ifndef __SERVER_YOTSUYA__
#define __SERVER_YOTSUYA__

#include <assert.h>

#include "yi.h"
#include "yitrace.h"
#include "yiPacket.h"
#include "ConnectionInfo.h"
#include "ConnectionManager.h"
#include "yienv.h"

// --------------------------------------------
class cServerYotsuya
{
public:
	cServerYotsuya(const char* pszServerName);
	~cServerYotsuya();

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
	bool Loop();

protected:
	// -------------------------------------
	virtual bool OnAcceptLocal(int sock);

	// -------------------------------------
	virtual bool OnAccept(int sock, unsigned int from);

	//
	virtual bool OnMessage(int sock, ssyNormalPacket& packet)=0;

	//
	virtual bool OnMessageLocal(int sock, ssyNormalPacket& packet);
	
	//
	virtual void OnTeminate(int sock, cConnectionManager& mgr);

	//
	virtual void OnServerStart()=0;

	//
	virtual void OnServerEnd()=0;

private:
	void Init();

	// ServerYotsuya.Create.cpp
	int Accept(int listenSock, int pipefd, Bool& isTerminate, unsigned int* from=0);


	// ServerYotsuya.Dispatch.cpp
	void OnDispatch(unsigned long param1, unsigned long param2, unsigned long param3);

	//
	void OnLimitedDispatch(unsigned long param1, unsigned long param2, unsigned long param3);

	//
	void OnLoopNormal(unsigned long param1, unsigned long param2, unsigned long param3);

	// 
	void OnLoopLimited(unsigned long param1, unsigned long param2, unsigned long param3);

	// 
	void QuitMessage(int mode);

private:
	// -----------------------------------
	char m_szServerName[64];
	sSecurityInfo m_security;

	// -----------------------------------
	sServerInformation m_ServerInfo;
	sServerInformation m_LocalInfo;

	// -----------------------------------
	pthread_t m_LoopThread_t ;
	pthread_t m_LocalLoopThread_t;

	// -----------------------------------
	cConnectionManager m_LocalConMgr;
	cConnectionManager m_GlobalConMgr;


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

