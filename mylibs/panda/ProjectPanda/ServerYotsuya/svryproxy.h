#ifndef __svryproxy_h__ 
#define __svryproxy_h__ 

#include "yi.h"
#include "yisock.h"
#include "svrybase.h"
#include "svrydefs.h"
#include "yistd.h"

// ------------------------------
typedef struct xsProxyTable 
{
	unsigned long created_time;
	int  	passport;
	int 	sock1;
	int 	sock2;
} sProxyTable;

// ------------------------------
class csvryproxy : public csvrybase
{
public:
	// --------------------------
	csvryproxy(const char* servername);

	// --------------------------
	~csvryproxy();
	
	virtual Bool OnServerStart();

	//
	virtual Bool OnServerEnd();

	// operations
protected:
	// -----------------------------------
	Bool OnReqGetPassport(int* id, int* passport);

protected:
	// -----------------------------------
	virtual void OnClientClose(int sock, void** pExtraDataRef);

	// --------------------------
	virtual Bool OnPreMessage(int sock, void** pExtraDataRef, Bool& bIsHandled);

	// --------------------------
	virtual Bool OnMessagex(int sock, int id, void* data, int size, void** pExtraDataRef){return True;}

	// --------------------------
	virtual Bool OnMessageLocal(int sock, syipacket& packet, void** pExtraData=0);

	// --------------------------
	virtual Bool OnAccepted(int sock, int index, void** pExtra);

private:
	HandleMutex m_hMutex;
	sProxyTable* m_table;
	void InitTable();
	int m_nMaxTableCnt;
};


#endif
