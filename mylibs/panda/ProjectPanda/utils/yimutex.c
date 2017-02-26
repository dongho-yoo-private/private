#ifdef __POSIX__
#include <unistd.h>
#else
#include <Windows.h>
#pragma (lib, "Kernel32.lib");
#endif
#include <stdlib.h>
#include <stdio.h>

#include "yimutex.h"
#include "yistd.h"

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
static inline HandleMutex __yiSemaphoreCreate__(HandleBuffer buffer, Bool bIsWait, int n)
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
	if (CreatePipe(&p->hRead, &p->hWrite, 0, NULL)==FALSE)
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
		WriteFile(p->hWrite, dummy_buffer, n, &nn, Null);
	}
#endif
	return (HandleMutex)p;
}

/**/
static inline void __yiSemaphoreLock__(HandleMutex h)
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
static inline void __yiSemaphoreUnlock__(HandleMutex h)
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
static inline void __yiSemaphoreDestroy__(HandleMutex h)
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
static inline void __yiSemaphoreInit__(HandleMutex h)
{
	char dummy_buffer[MAX_SEMAPHORE_COUNT];
	smutex* p = (smutex*)h;

#ifdef __POSIX__
	read(p->fd[0], dummy_buffer, p->uInitialCount);
#else
	DWORD nn;
	ReadFile(p->hRead, dummy_buffer, (long)p->uInitialCount, &nn, Null);
#endif
}

/*
 * */
HandleMutex yiMutexCreate(HandleBuffer buffer)
{
	return __yiSemaphoreCreate__(buffer, True, 1);
	//pthread_mutex_t* handle = (pthread_mutex_t*)yialloci(sizeof(pthread_mutex_t));
	//pthread_mutex_init(handle, 0);
}

/*

/*
 * */
void FASTCALL yiMutexLock(HandleMutex h)
{
	if (h==0)
		return ;

	__yiSemaphoreLock__(h);
}

void FASTCALL yiMutexWait(HandleMutex h)
{
	fd_set fds;
	smutex* p = (smutex*)h;

	if (h==0)
		return ;

	FD_ZERO(&fds);
	FD_SET(p->fd[0], &fds);

	select(p->fd[0]+1, &fds, 0, 0, 0);
}

/**/
void FASTCALL yiMutexUnlock(HandleMutex h)
{
	if (h==0)
		return ;
	__yiSemaphoreUnlock__(h);
}


/**/
void yiMutexDestroy(HandleMutex h)
{
	if (h==0)
		return ;
	__yiSemaphoreDestroy__(h);
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
	ReadFile(p->hRead, &sig, sizeof(long), &nn, NULL);
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
	register int i;

	//for (i=0; i<p->nWaitingCnt; i++)
#ifdef __POSIX__
	write(p->fd[1], &uSigMask, sizeof(long));
#else
	WriteFile(p->hWrite, &uSigMask, sizeof(long), &nn, NULL);
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

	if (base->hMutex!=Null)
		return ;

	p = (HandleBuffer*) base->__allocatorf__(base->hMem, sizeof(smutex));

	base->hMutex = (Handle)yiMutexCreateb(p, attr);
	__allocator_copy__((Handle)base, base->hMutex);
	((HandleBase*)base->hMutex)->wFlagz |= HANDLE_MUST_FREE;

	base->__lockf__ = yiMutexLock;
	base->__unlockf__ = yiMutexUnlock;
}*/

#ifdef __POSIX__
#include <pthread.h>
// -------------------------------
HandleMutexx yiMutexCreatex()
{
	pthread_mutex_t* p = (pthread_mutex_t* )yialloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(p, 0);

	return (HandleMutexx)p;
}

Bool yiMutexTryx(HandleMutexx h)
{
	if (h==0)
		return ;
	return pthread_mutex_trylock((pthread_mutex_t*)h)==0?True:False;
}

// -------------------------------
void yiMutexLx(HandleMutexx h)
{
	if (h==0)
		return ;
	pthread_mutex_lock((pthread_mutex_t*)h);	
}


// -------------------------------
void yiMutexUnLx(HandleMutexx h)
{
	if (h==0)
		return ;
	pthread_mutex_unlock((pthread_mutex_t*)h);	
}


// -------------------------------
void yiMutexDestroyx(HandleMutexx h)
{
	if (h==0)
		return ;
	pthread_mutex_destroy((pthread_mutex_t*)h);
}


// -------------------------------
void yiMutexWaitx(HandleMutex h)
{
	if (h==0)
		return ;
	pthread_mutex_lock((pthread_mutex_t*)h);	
	pthread_mutex_unlock((pthread_mutex_t*)h);	
}
#endif



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
		if (CreatePipe(&hRead, &hWrite, 0, NULL)==FALSE)
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
RE_WAIT:
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
	if (WaitForSingleObject(discriptor, timeout)==TIME_OUT)
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

