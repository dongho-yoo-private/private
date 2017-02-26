#ifndef __ypnGoNetwork_h__
#define __ypnGoNetwork_h__
#include "ypn.h"

enum eypnGoNetworkPacketMessage {
	eGNPMSG_TEST = 1,
	eGNPMSG_REQ_PLAY,
	eGNPMSG_SEND_STONE
};

class cypnGoNetwork : public cyPrivateNetwork
{
public:
	// ---------------------------
	cypnGoNetwork(void);

	// ---------------------------
	~cypnGoNetwork(void);

	// ---------------------------
	Bool SendTestMessage(int n, char* buffer, int size);	
protected:
	// ---------------------------
	virtual Bool OnMessage(socket_t sock, int id, void* buffer);
	
};

#endif
