#include "svrybase.h"
#include <string.h>
#include <stdlib.h>
#include "yistd.h"
#include "yifile.h"
#include "yipsr.h"

// ----------------------------------
// static
//Bool csvrybase::LoadServerInformation(const char* pszDefnitionFileName, sServerInformation* server, sServerInformation* local)
//{
//	return yiEnvGetServerDefinition();
//}


// ----------------------------------
void csvrybase::Init()
{
	MEM_ZERO(m_ServerInfo);
	MEM_ZERO(m_LocalInfo);
	m_security=0;

	m_LoopThread_t = 0;
	m_LocalLoopThread_t = 0;

	m_LocalSock=0;
	m_Sock =0;

	m_bTerminate=False;
}

// ----------------------------------
csvrybase::csvrybase(const char* pszServerName)
{
	Init();
	//yistrcpy(m_szServerName, pszServerName);
	strcpy(m_szServerName, pszServerName);
}

// ----------------------------------
csvrybase::~csvrybase()
{
}
