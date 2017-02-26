#ifndef __svryssvr_h__
#define __svryssvr_h__

#include "yi.h"
#include "yiPacket.h"
#include "svrybase.h"
#include "sycHashL3.h"

// ------------------------------
class csvryproxy : public csvrybase
{
public:
	// --------------------------
	csvryproxy(const char* servername);

	// --------------------------
	~csvryproxy();
	

	//
	virtual Bool OnServerStart();

	//
	virtual Bool OnServerEnd();
protected:
	// -----------------------------------
	virtual void OnClientClose(int sock, void** pExtraDataRef);

	// --------------------------
	virtual Bool OnMessage(int sock, ssyNormalPacket& packet, void** pExtraDataRef);

private:
	// --------------------------
	Bit32 GetUserId(const char* pszUserName);

	// -------------------------- database.
	csycHashL3 m_UserDataHash;
	ssycHashL3* m_pUserHash;
	
	// -------------------------- database.
	csycHashL3 m_GroupDataHash;
	ssycHashL3* m_pGroupHash;
};


#endif
