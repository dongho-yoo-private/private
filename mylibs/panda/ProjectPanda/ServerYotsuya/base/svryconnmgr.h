#ifndef __CONNECTION_MGR__
#define __CONNECTION_MGR__

#include <yiList.h>
#include "svryconn.h"

// ---------------------------
class csvryConnMgr
{
public:
	// -------------------------------------
	csvryConnMgr();

	// -------------------------------------
	~csvryConnMgr();


	// -------------------------------------
	Bool Create(int fd, int nOneMaxConnection, int nArrayCnt);

	// -------------------------------------
	void Destroy();

	// -------------------------------------
	csvryConn* GetBlankConnectionInfo();

	// -------------------------------------
	Bool Add(csvryConn* pConn);

	// -------------------------------------
	long GetArrayCnt();

	// -------------------------------------
	void BroadCastQuit(int mode);

	// ------------------------------------
	csvryConn* GetConnectionInfo(int index);

private:
	cyiList* m_conn_list;

	long m_nCurrentArrayCnt;
	int m_nMaxConnection;
	int m_nMaxArraryCnt;

	int m_MainLoopFd;

};

#endif
