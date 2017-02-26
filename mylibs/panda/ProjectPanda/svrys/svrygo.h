#ifndef __svrygo_h__
#define __svrygo_h__

#include "csvrybase.h"

class csvrygo : public csvrybase
{
public:
	csvrygo(const char* servername);
	~csvrygo();

protected:
	// ------------------------
	virtual Bool OnMessage(int sock, ssyNormalPacket& packet);

	// ------------------------
	virtual Bool OnServerStart();

	// ------------------------
	virtual Bool OnMessageLocal(int sock, ssyNormalPacket& packet);

	// ------------------------
	virtual Bool OnServerEnd();

private:
	// -----------------------------------------
	Bool OnLogin(int sock, ssyNOrmalPacket& packet);	
};

#endif

