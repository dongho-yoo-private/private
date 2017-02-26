#ifdef __POSIX__
#include <unistd.h>
#else
//#include <Windows.h>
#pragma comment(lib, "Kernel32.lib")
#endif
#include <stdlib.h>
#include <stdio.h>

#include "yi.h"
#include "yimutex.h"
#include "yistd.h"

#include <process.h>

/**/
typedef struct __mutex__
{
#ifdef __POSIX__
	int fd[2];
#else
	HANDLE hRead;
	HANDLE hWrite;
#endif
	unsigned long uInitialCount;
	unsigned long nWaitingCnt;
	Bool bIsHeap;
	Bool bIsWait;
} smutex;


/**/
static __inline HandleMutex __yiSemaphoreCreate__(HandleBuffer buffer, Bool bIsWait, int n)
{
	smutex* p = (smutex* )buffer;
	char dummy_buffer[MAX_SEMAPHORE_COUNT];


	if (p==0)
	{
		p=(smutex*)malloc(sizeof(smutex));
		if (p==0)
		{
			yiTrace("malloc failure!!\n");
			return 0;
		}
		p->bIsHeap=True;
	}

	p->uInitialCount = n;
	p->bIsWait= bIsWait;

#ifdef __POSIX__
	if (pipe(p->fd)!=0)
#else
	if (CreatePipe(&p->hRead, &p->hWrite, 0, 0)==FALSE)
#endif
	{

		if (p->bIsHeap==True)
		{
			free(p);
		}
		yiTrace("mutex create error\n");
		return 0;
	}

	if (p->bIsWait==False)
	{
		p->nWaitingCnt = 0;
		return (HandleMutex)p;
	}


#ifdef __POSIX__
	{
		int nwrite = write(p->fd[1], (void*)dummy_buffer, n); 
		/*int i=0;
		for (i=0; i<n; i++)
		{
			write(p->fd[1], &i, sizeof(long));
		}*/
	}
#else
	{
		DWORD nn;
		WriteFile(p->hWrite, dummy_buffer, n, &nn, NULL);
	}
#endif
	return (HandleMutex)p;
}

/**/
static __inline void __yiSemaphoreLock__(HandleMutex h)
{
	int i;
	smutex* p = (smutex*)h;
#ifdef __POSIX__
	int n=read(p->fd[0], &i, 1);
#else
	DWORD n;
	ReadFile(p->hRead, &i, sizeof(long), &n, NULL);
#endif
}

/**/
static __inline void __yiSemaphoreUnlock__(HandleMutex h)
{
	smutex* p = (smutex*)h;
#ifdef __POSIX__
	int n = write(p->fd[1], (void*)h, 1);

#else
	DWORD n;
	WriteFile(p->hWrite, (void*)h , 1, &n, NULL);
#endif
}

/*
 * */
static __inline void __yiSemaphoreDestroy__(HandleMutex h)
{
	if (h) 
	{
		smutex* p = (smutex*)h;
#ifdef __POSIX__
		close(p->fd[0]);
		close(p->fd[1]);
#else
		CloseHandle(p->hWrite);
		CloseHandle(p->hRead);
#endif
		if (p->bIsHeap==True)
		{
			free(p);
		}
	}
}

/*
 * */
static __inline void __yiSemaphoreInit__(HandleMutex h)
{
	char dummy_buffer[MAX_SEMAPHORE_COUNT];
	smutex* p = (smutex*)h;

#ifdef __POSIX__
	read(p->fd[0], dummy_buffer, p->uInitialCount);
#else
	DWORD nn;
	ReadFile(p->hRead, dummy_buffer, (long)p->uInitialCount, &nn, NULL);
#endif
}

/*
 * */
HandleMutex yiMutexCreate(HandleBuffer buffer)
{
	return (HandleMutex)CreateMutex(0, FALSE, 0);
}

/*

/*
 * */
void FASTCALL yiMutexLock(HandleMutex h)
{
	WaitForSingleObject((HANDLE)h, INFINITE);
	//__yiSemaphoreLock__(h);
}

/**/
void FASTCALL yiMutexUnlock(HandleMutex h)
{
	ReleaseMutex((HANDLE)h);
	//__yiSemaphoreUnlock__(h);
}


/**/
void yiMutexDestroy(HandleMutex h)
{
	CloseHandle((HANDLE)h);
	//__yiSemaphoreDestroy__(h);
}

/**/
HandleMutexx yiMutexCreateEx(int nSpinCount)
{
	CRITICAL_SECTION* sec = (CRITICAL_SECTION*)yialloci(sizeof(CRITICAL_SECTION));

	if (InitializeCriticalSectionAndSpinCount(sec, nSpinCount)==TRUE)
	{
		return (HandleMutexx)sec;
	}

	yifree(sec);
	return 0;

}

void yiMutexLockEx(HandleMutexx h)
{
	if (h)
	{
		int res = TryEnterCriticalSection((LPCRITICAL_SECTION)h);

		if (res)
		{
			//xTRACE("mutex lock by (%x,%x)", h, GetCurrentThreadId());
			return ;
		}

		EnterCriticalSection((LPCRITICAL_SECTION)h);
	}

	//xTRACE("mutex lock by (%x,%x)", h, GetCurrentThreadId());
	//return ;
	//if (h)
	//	EnterCriticalSection((LPCRITICAL_SECTION)h);
}


/**/
Bool yiMutexTryEx(HandleMutexx h)
{
	if (h)
	{
		Bool res = TryEnterCriticalSection((LPCRITICAL_SECTION)h)?True:False;

		if (res)
		{
			//xTRACE("mutex lock by (%x,%x)", h, GetCurrentThreadId());
		}
		return res;

	}
	return False;
}

void yiMutexUnlockEx(HandleMutexx h)
{
	if (h)
		LeaveCriticalSection((LPCRITICAL_SECTION)h);
	//xTRACE("mutex unlock by (%x, %x)", h, GetCurrentThreadId());
}

void yiMutexDestroyEx(HandleMutexx h)
{
	if (h)
		yifree((void*)h);
}



/**/
void yiMutexInit(HandleMutex h)
{
	__yiSemaphoreInit__(h);
}

/**/
HandleSemaphore yiSemaphoreCreate(HandleBuffer buff, int n)
{
	return __yiSemaphoreCreate__(buff, True, n);
}

/**/
void FASTCALL yiSemaphoreLock(HandleSemaphore h)
{
	__yiSemaphoreLock__(h);
}


/**/
void FASTCALL yiSemaphoreUnlock(HandleMutex h)
{
	__yiSemaphoreUnlock__(h);
}

/**/
void yiSemaphoreDestroy(HandleMutex h)
{
	__yiSemaphoreDestroy__(h);
}

/**/
void yiSemaphoreInit(HandleMutex h)
{
	__yiSemaphoreInit__(h);
}

/**/
HandleSignal yiSignalCreate(HandleBuffer buffer)
{
	return __yiSemaphoreCreate__(buffer, False, 0);
}

void yiSignalWait(HandleSignal h, unsigned long uSigMask)
{
	smutex* p = (smutex* )h;
	unsigned long sig;

	p->nWaitingCnt++;
WAIT:
#ifdef __POSIX__
	read(p->fd[0], &sig, sizeof(long));
#else
	{
		DWORD nn;
		ReadFile(p->hRead, &sig, sizeof(long), &nn, NULL);
	}
#endif
	if (sig & uSigMask)
	{
		p->nWaitingCnt--;
		return ;
	}

	goto WAIT;
}

/*
 * */
void yiSignalSend(HandleSignal h, unsigned long uSigMask)
{
	smutex* p = (smutex* )h;

	//for (i=0; i<p->nWaitingCnt; i++)
#ifdef __POSIX__
	write(p->fd[1], &uSigMask, sizeof(long));
#else
	{
		DWORD nn;
		WriteFile(p->hWrite, &uSigMask, sizeof(long), &nn, NULL);
	}
#endif
		
}

/*
 * */
void yiSignalDestroy(HandleSignal h)
{
	__yiSemaphoreDestroy__(h);
}


/**/
static void FASTCALL __yi_mutex_dummy_call__(Handle h)
{
}

/**/
/*void yiSetThreadSafe(Handle h)
{
	HandleBase* base = (HandleBase* )h;
	HandleBuffer* p; 

	if (base->hMutex!=NULL)
		return ;

	p = (HandleBuffer*) base->__allocatorf__(base->hMem, sizeof(smutex));

	base->hMutex = (Handle)yiMutexCreateb(p, attr);
	__allocator_copy__((Handle)base, base->hMutex);
	((HandleBase*)base->hMutex)->wFlagz |= HANDLE_MUST_FREE;

	base->__lockf__ = yiMutexLock;
	base->__unlockf__ = yiMutexUnlock;
}*/

// ------------------------------
HandleThread yiStartThreadx(OnThreadCallbackx callback, void* param, int size)
{
	unsigned int id;
	HANDLE hThread;
	if ((hThread=(HANDLE)_beginthreadex(0, 0, callback, size!=0 ? yitoheap(param, size):param, 0, &id))==0)
	{
		CloseHandle(hThread);
		return 0;
	}

	return (HandleThread)hThread;
}

// ------------------------------
Bool yiThreadWait(HandleThread hThread, int nTimeOut)
{
	if (WaitForSingleObject((HANDLE)hThread, nTimeOut)!=WAIT_OBJECT_0)
	{
		return False;
	}
	return True;
}


// ------------------------------
Bool yiStartThread(OnThreadCallback callback, void* param, int size)
{
	if (_beginthread(callback, 0, size!=0 ? yitoheap(param, size):param)==-1)
	{
		return FALSE;
	}

	return TRUE;
}


#ifdef __POSIX__
#define HandleEvent int
#define READ_PIPE(fd, buffer, size) read(fd, buffer, size)
#define WRITE_PIPE(fd, buffer, size) write(fd, buffer, size)
#else
#define HandleEvent HANDLE
#define READ_PIPE(fd, buffer, size) ReadFile(fd, buffer, size, 0, 0)
#define WRITE_PIPE(fd, buffer, size) WriteFile(fd, buffer, size, 0, 0)
#endif

HandleEvent g_event_wfd=0;
HandleEvent g_event_rfd=0;
unsigned int g_event_cnt=0;
unsigned int g_event_id=0;
HandleMutex g_hMutex=0;
HandleMutex g_hReadMutex=0;

// -------------------------------
unsigned int yiEventInit()
{
	if (g_event_wfd==0)
	{
#ifdef __POSIX__
		int fd[2];
		if (pipe(fd)!=0)
#else
		HANDLE hRead, hWrite;
		if (CreatePipe(&hRead, &hWrite, 0, 0)==FALSE)
#endif
		{
			return 0;
		}
#ifdef __POSIX__
		g_event_wfd=fd[1];		
		g_event_rfd=fd[0];		
#else
		g_event_wfd=hWrite;
		g_event_rfd=hRead;
#endif
		g_hMutex=yiMutexCreatex();
		g_hReadMutex=yiMutexCreatex();
	}

	return ++g_event_id;
}


// -------------------------------
void yiEventSet(unsigned int event)
{
	yiMutexLx(g_hMutex);
	WRITE_PIPE(g_event_wfd, &event, sizeof(event));
	g_event_cnt++;
	yiMutexUnLx(g_hMutex);
}

// -------------------------------
void yiEventReset(unsigned int event)
{
	int n;
	int i;
	yiMutexLx(g_hReadMutex);
	yiMutexLx(g_hMutex);
	n=g_event_cnt;
	yiMutexUnLx(g_hMutex);
	

	if (n==0)
	{
		goto NO_READ;
	}

	for (i=0; i<n; i++)
	{
		unsigned int temp;
		READ_PIPE(g_event_rfd, &temp, sizeof(temp));
		if (temp!=event)
		{
			yiMutexLx(g_hMutex);
			WRITE_PIPE(g_event_wfd, &temp, sizeof(temp));
			yiMutexUnLx(g_hMutex);
			continue;
		}
		yiMutexLx(g_hMutex);
		g_event_cnt--;
		yiMutexUnLx(g_hMutex);
	}

NO_READ:
	yiMutexUnLx(g_hReadMutex);
}

//#include <sys/select.h>
// -------------------------------
Bool yiWaitObject(AnyDescriptor discriptor, int timeout)
{
#ifdef __POSIX__
	struct timeval tv = {timeout/1000, (timeout%1000)*1000};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(discriptor, &fds);

	if (timeout==INFINITE)
	{
		select(discriptor+1, &fds, 0, 0, 0);
		return False;
	}

	if (select(discriptor+1, &fds, 0, 0, &tv)==0)
	{
		if (tv.tv_sec==0 &&
			tv.tv_usec==0)
		{
			return True;
		}
	}
#else
	if (WaitForSingleObject(discriptor, timeout)==WAIT_TIMEOUT)
	{
		return True;
	}
#endif
	return False;
}

// -------------------------------
Bool yiEventWait(unsigned int event, int timeout)
{
	int temp;
	int cnt=0;
	int i=0;
	
WAIT:
	if (yiWaitObject(g_event_rfd, timeout)==True)
	{
		return False;
	}

	yiMutexLx(g_hReadMutex);
	yiMutexLx(g_hMutex);
	cnt=g_event_cnt;
	yiMutexUnLx(g_hMutex);


	for (i=0; i<cnt; i++)
	{
		READ_PIPE(g_event_rfd, &temp, sizeof(temp));

		if (temp!=event)
		{
			yiMutexLx(g_hMutex);
			WRITE_PIPE(g_event_wfd, &temp, sizeof(temp));
			yiMutexUnLx(g_hMutex);
			continue;
		}
		yiMutexLx(g_hMutex);
		g_event_cnt--;
		yiMutexUnLx(g_hMutex);
		yiMutexUnLx(g_hReadMutex);
		return True;
	}
	yiMutexUnLx(g_hReadMutex);
	goto WAIT;

}

// -------------------------------
void yiEventDelete(unsigned int event)
{
	yiEventReset(event);
}
