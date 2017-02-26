#include "yi.h"
#include "yitrace.h"
#include "ConnectionManager.h"
#include <assert.h>

// ----------------------------------------
cConnectionManager::cConnectionManager()
{
	m_conn_list=0;
}

// ----------------------------------------
cConnectionManager::~cConnectionManager()
{
	if (m_conn_list!=0)
	{
		yiTrace("cConnectionManager::~cConnectionManager() called\n");
		delete m_conn_list;
	}
}

// ----------------------------------------
Bool cConnectionManager::Create(int fd, int nOneMaxConnection, int nArrayCnt)
{
	m_MainLoopFd=fd;
	m_nMaxConnection = nOneMaxConnection;
	m_nMaxArraryCnt = nArrayCnt;
	m_nCurrentArrayCnt=0;

	m_conn_list = new cyiList();
	assert(m_conn_list );

	return True;
}

// -------------------------------------
cConnectionInfo* cConnectionManager::GetBlankConnectionInfo()
{
	m_conn_list->lock();
	int n = m_conn_list->count();

	if (n==0)
	{
		m_conn_list->unlock();
		return 0;
	}

	for (cyiNode* node = m_conn_list->GetNodeTopFrom(0);node!=0; node=node->np)
	{
		cConnectionInfo* pConn = (cConnectionInfo* )node->ref;
		if (pConn==0)
		{
			assert(0);
		}

		if (pConn->IsFull()==False)
		{
			m_conn_list->unlock();
			return pConn;
		}
	}

	m_conn_list->unlock();
	return 0;
}

// -------------------------------------
void cConnectionManager::Destroy()
{
	delete m_conn_list;
	m_conn_list=0;
}

// -------------------------------------
Bool cConnectionManager::Add(cConnectionInfo* pConn)
{
	m_conn_list->lock();
	m_conn_list->addref(pConn);
	m_conn_list->unlock();
	return True;
}

// -------------------------------------
long cConnectionManager::GetArrayCnt()
{
	return m_conn_list->count();
}

// -------------------------------------
void cConnectionManager::BroadCastQuit(int mode)
{
	m_conn_list->lock();
	for (cyiNode* pNode=m_conn_list->GetNodeTopFrom(0); pNode!=0; pNode=pNode->np)
	{
		cConnectionInfo* pConn = (cConnectionInfo* )pNode->ref;
		pConn->RequestTerminate(mode);
	}
	m_conn_list->unlock();
}

// -------------------------------------
cConnectionInfo* cConnectionManager::GetConnectionInfo(int index)
{
	int n =0;
	assert(m_conn_list);
	m_conn_list->lock();
	cyiNode* pNode = m_conn_list->GetNodeTopFrom(index);

	m_conn_list->unlock();

	if (pNode!=0)
	{
		return (cConnectionInfo*)pNode->ref;
	}
	return 0;

}

