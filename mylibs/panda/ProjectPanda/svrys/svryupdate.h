#ifndef __svryupdate_h__
#define __svryupdate_h__


/** comment : Server Difinition.
 *
 * local:  limited.
 * server: limited.
 *
 * extra0: max file write thread.
 * extra1: max local packet thread.
 *
 */

#include "yi.h"
#include "svrybase.h"

// ------------------------------
typedef struct xsPackageInfoHeader{
	unsigned long ver;
	int n;
} sPackageInfoHeader;

// ------------------------------
enum eInstallationPath {
	eIP_WINDOWS=0,
	eIP_PROGRAM_FILES,
	eIP_FONT,
	eIP_APPDIR
};

// ------------------------------
typedef struct xsPackageInfo {
	const char name[64];
	eInstallationPath path;
	int nFileSize;
	void* p;
};

// ------------------------------
class csvryupdate : public csvrybase
{
public:
	csvryupdate(const char* pszServerName);
	~csvryupdate();

protected:
	//
	virtual Bool OnMessage(int sock, ssyNormalPacket& packet, void** pExtraDataRef);

	//
	virtual Bool OnServerStart();

	//
	virtual Bool OnMessageLocal(int sock, ssyNormalPacket& packet, void** pExtraDataRef);

	//
	virtual Bool OnServerEnd();
};

#endif
