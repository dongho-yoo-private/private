#ifndef __CONNECTION_INFO_H__
#define __CONNECTION_INFO_H__

#include <sys/select.h>
#include <pthread.h>
#include <unistd.h>
#include <yi.h>
#include <yiQueue.h>
#include <yimutex.h>
#include <yisock.h>

// ---------------------------
typedef Bool (*OnAcceptedCallback)(void* pClass, int sock, int id, void** pExtra);

// ---------------------------
typedef Bool (*OnClosedCallback)(void* pClass, int sock, int index, void** pExtra);

// ---------------------------
class cConnectionInfo
{
public:
	cConnectionInfo(int n, int index, int acceptFd, Bool bIsLocal);
	~cConnectionInfo();

	//
	Bool Add(int fd);	

	//
	void RemoveFd(int n);

	//
	Bool IsFull();

	//
	int* GetFds();

	//
	void** GetExtraDataPtr(int index);

	//
	Bool Wait(cyiQueue& rdFds, OnAcceptedCallback callback_accepted, OnClosedCallback callback_closed, void* p);

	// ------------------------------------
	void BroadCastMessage(void* encoded_packet, int size);

	// ------------------------------------
	void RequestTerminate(int reason);

	// ------------------------------------
	void RemoveAll();


private:
	// ------------------------------------
	int AddFromQueue(OnAcceptedCallback callback, void* ClassCallback);

	// ------------------------------------
	void UpdateMaxValue();

	// ------------------------------------
	void FdSets();
private:

	int* m_fds;
	void** m_fdsExInfo;
	fd_set m_rfdset;	
	int m_r_acceptFd;
	int m_w_acceptFd;
	int m_cnt;
	int m_maxcnt;
	int m_index;

	cyiQueue m_brank_queue;
	cyiQueue m_add_queue;
	cyiQueue m_fdset_queue;
	HandleMutex m_hMutex;

	Bool m_bIsLocal;
	int m_AcceptFd;
};

#endif
