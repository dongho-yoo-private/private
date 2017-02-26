#ifndef __CONNECTION_INFO_H__
#define __CONNECTION_INFO_H__

#include <sys/select.h>
#include <pthread.h>
#include <unistd.h>
#include <yi.h>
#include <yiQueue.h>
#include <yimutex.h>

// ---------------------------
class cConnectionInfo
{
public:
	cConnectionInfo(int n, int acceptFd, Bool bIsLocal);
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
	Bool Wait(cyiQueue& rdFds);

	// ------------------------------------
	void BroadCastMessage(void* encoded_packet, int size);

	// ------------------------------------
	void RequestTerminate(int reason);

	// ------------------------------------
	void RemoveAll();


private:
	// ------------------------------------
	int AddFromQueue();

	// ------------------------------------
	void UpdateMaxValue();

	// ------------------------------------
	void FdSets();
private:

	int* m_fds;
	fd_set m_rfdset;	
	int m_r_acceptFd;
	int m_w_acceptFd;
	int m_cnt;
	int m_maxcnt;

	cyiQueue m_brank_queue;
	cyiQueue m_add_queue;
	cyiQueue m_fdset_queue;
	HandleMutex m_hMutex;

	Bool m_bIsLocal;
	int m_AcceptFd;
};

#endif
