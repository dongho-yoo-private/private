#ifndef __CONNECTION_MGR__
#define __CONNECTION_MGR__

#include <yiList.h>
#include "ConnectionInfo.h"

// ---------------------------
class cConnectionManager
{
public:
	// -------------------------------------
	cConnectionManager();

	// -------------------------------------
	~cConnectionManager();


	// -------------------------------------
	Bool Create(int fd, int nOneMaxConnection, int nArrayCnt);

	// -------------------------------------
	void Destroy();

	// -------------------------------------
	cConnectionInfo* GetBlankConnectionInfo();

	// -------------------------------------
	Bool Add(cConnectionInfo* pConn);

	// -------------------------------------
	long GetArrayCnt();

	// -------------------------------------
	void BroadCastQuit(int mode);



private:
	cyiList* m_conn_list;

	long m_nCurrentArrayCnt;
	int m_nMaxConnection;
	int m_nMaxArraryCnt;

	int m_MainLoopFd;

};

#endif
