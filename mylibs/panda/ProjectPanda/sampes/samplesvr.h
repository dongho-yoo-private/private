#ifndef __samplesvr_h__
#define __samplesvr_h__

#include "yi.h"
#include "svrybase.h"
#include "sycHashL2.h"
#include "sycHashL3.h"

/** comment : Server Difinition.
 *
 * local: limited.
 * server: normal
 *
 * extra0: max file write thread.
 * extra1: max local packet thread.
 *
 */

// ------------------------------
class csamplesvr : public csvrybase
{
public:
	csamplesvr(const char* servername);
	~csamplesvr();


protected:
	//
	virtual Bool OnMessage(int sock, ssyNormalPacket& packet, void** pExtra=0);

	//
	virtual Bool OnServerStart();

	//
	virtual Bool OnMessageLocal(int sock, ssyNormalPacket& packet, void** pExtra=0);

	//
	virtual Bool OnServerEnd();
private:
};

#endif
