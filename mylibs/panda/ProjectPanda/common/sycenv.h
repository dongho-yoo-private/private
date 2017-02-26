#ifndef __yi_env_h__
#define __yi_env_h__
#include "yi.h"

// -----------------------------------------------
enum eServerType {
	SERVER_TYPE_NORMAL,
	SERVER_TYPE_LIMITED	
};

// ----------------------------------
typedef struct __sServerInformation 
{
	unsigned long nMaxConnections;
	unsigned long nMaxArrayCount;
	unsigned long nMaxListenWaitCnt;
	int	nMaxPacketSize;
	char szServerAddress[256];
	unsigned long nPortNo;
	eServerType type;

	unsigned int nLimitedTypeTimeOut;

	Bool bIsPacketClear;

	Bit32 extra[8];
	
} sServerInformation;

// ----------
class csycEnv
{
public:
	// ---------------------------
	static Bool GetServerDefinition(const char* pszServerName, sServerInformation* server, sServerInformation* local);

	// ---------------------------
	static const char* GetLocalPortName(const char* ServerName, char* out_path);

	// ---------------------------
	static const char* GetUserRoot();

	// ---------------------------
	static const char* GetEmailRoot();

	// ---------------------------
	static const char* GetGroupRoot();

	// ---------------------------
	static const char* GetPrivateRoot();

	// ---------------------------
	static const char* GetRecordRoot();

	// ---------------------------
	static const char* GetShadowRoot();

	// ---------------------------
	// ??
	static const char* GetLogRoot();
};


#endif
