#ifndef __yi_client_h__
#define __yi_client_h__

#include "yiPacket.h"

class cyiClient
{
public:
	// --------------------------------
	cyiClient(const char* pszPortName, const char* pszServerName=0);

	// --------------------------------
	cyiClient(unsigned int address, int port, const char* pszServerName=0);

	// --------------------------------
	~cyiClient();

	// --------------------------------
	Bool connect(int nRetry=0);

	// --------------------------------
	Bool disconnect();

	// --------------------------------
	int write(void* buffer, int size);

	// --------------------------------
	Bool write(ssyNormalPacket& packet);

	// --------------------------------
	Bool writex(ssyNormalPacket& packet, const void* extData, int extSize);

	// --------------------------------
	int read(void* buffer, int size, int TimeOutMilliSecond=-1);

	// --------------------------------
	Bool read(ssyNormalPacket& packet, int TimeOutMilliSecond=-1);

	// --------------------------------
	Bool readex(void* extData, int extSize, int TimeOutMilliSecond=0);

	// --------------------------------
	int GetSock();


	// --------------------------------
	static long htol(const char* pszHostName, int index=0);

	// --------------------------------
	static long itol(const char* pszIpAddress);

private:
	// --------------------------------
	Bool connect_svr(int nRetry);

	// --------------------------------
	Bool connect_local(int nRetry);
private:
	int m_sock;
	int m_nPortNo;
	int m_addr;
	char m_szServerName[128];
	char m_szPortName[256];
	//sSecurityInfo m_security;
	unsigned long m_security;
	Bool m_bNoEncode;
};

#endif

