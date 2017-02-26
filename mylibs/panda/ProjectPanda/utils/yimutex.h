/** 
 *
 * 内部的にはパイプになっている。
 * mutex, semaphore, wait.
 *
 * Updated: 2009. 7.31. 初版 (version. 0.8.7)
 *
 */
#ifndef __yimutex_h__
#define __yimutex_h__

#include "yi.h"

#define MAX_SEMAPHORE_COUNT 512

/*
 * */
typedef Handle HandleSemaphore;
/*
 * */
typedef Handle HandleMutex;
/*
 * */
typedef Handle HandleSignal;

#ifdef WIN32
#define AnyDescriptor HANDLE 
#else
#define AnyDescriptor int 
#define INFINITE	-1
#endif

// -------------------------------
EXTERN_C Bool yiWaitObject(AnyDescriptor discriptor, int timeout);

/* create mutex object
 *
 * @param  buffer 
 *
 * ex)
 * HandleMutex h = yiMutexCreate(Null);
 *
 * 		:
 * 		:
 * yiMutexLock(h);
 * 		:
 *      Critical Session
 *      	:
 * yiMutexUnlock(h);
 *
 * Updated: 2009. 7.31.
 * */
EXTERN_C HandleMutex yiMutexCreate(HandleBuffer in_handle);



/*
 *
 * Updated: 2009. 7.31.
 * */
EXTERN_C void yiMutexDestroy(HandleMutex h);

/*
 * Updated: 2009. 7.31.
 * */
EXTERN_C void FASTCALL yiMutexLock(HandleMutex h);

/*
 * Updated: 2010. 2. 4.
 * */
EXTERN_C void FASTCALL yiMutexWait(HandleMutex h);

/*
 * Updated: 2009. 7.31.
 * */
EXTERN_C void FASTCALL yiMutexUnlock(HandleMutex h);


#ifndef _WIN32
#define yiMutexCreateEx(n) yiMutexCreatex()
#define yiMutexDestroyEx yiMutexDestroyx
#define yiMutexLockEx yiMutexLx
#define yiMutexUnlockEx yiMutexUnLx
#define yiMutexTryEx yiMutexTryx
#endif
/**/

/*
 *
 * ex)
 * HandleSemaphore h = yiSemaphoreCreate(5);
 *
 *		:
 *		:
 * yiSemaphoreLock(h);
 *		:
 * int n = yiThreadCreate(this, &AnyClass::OnThread, (yiAnyValue)param1, (WsxAnyValue)param2);
 *		:
 * yiSemaphoreUnlock(h);
 *
 * Updated: 2009. 7.31.
 * */
EXTERN_C HandleSemaphore yiSemaphoreCreate(HandleBuffer h, int nCount);

/*
 * Updated: 2009. 7.31.
 * */
EXTERN_C void FASTCALL yiSemaphoreLock(HandleSemaphore h);

/*
 * Updated: 2009. 7.31.
 * */
EXTERN_C void FASTCALL yiSemaphoreUnlock(HandleSemaphore h);

EXTERN_C void yiSemaphoreDestroy(HandleSignal h);

/*
 * Updated: 2009. 8. 3.
 * */
EXTERN_C HandleSignal yiSignalCreate(HandleBuffer h);

/*
 * Updated: 2009. 7.31.
 * */
EXTERN_C void yiSignalWait(HandleSignal h, unsigned long uSigMask);

/*
 * Updated: 2009. 7.31.
 * */
EXTERN_C void yiSignalSend(HandleSignal h, unsigned long uSigMask);

EXTERN_C void yiSignalDestroy(HandleSignal h);

#ifdef __POSIX__

typedef Handle HandleMutexx;

// -------------------------------
EXTERN_C Bool yiMutexTryx(HandleMutexx h);

// -------------------------------
EXTERN_C HandleMutexx yiMutexCreatex();

// -------------------------------
EXTERN_C void yiMutexLx(HandleMutexx h);

// -------------------------------
EXTERN_C void yiMutexUnLx(HandleMutexx h);

// -------------------------------
EXTERN_C void yiMutexDestroyx(HandleMutexx h);

// -------------------------------
EXTERN_C void yiMutexWaitx(HandleMutexx h);
#else
#define yiMutexCreatex() yiMutexCreate(0)
#define yiMutexLx(h) yiMutexLock(h)
#define yiMutexUnLx() yiMutexUnlock(h)
#define yiMutexDestroyx() yiMutexDestroy(h)
#endif 

// -------------------------------
EXTERN_C unsigned int yiEventInit();

// -------------------------------
EXTERN_C void yiEventSet(unsigned int event);

// -------------------------------
EXTERN_C void yiEventReset(unsigned int event);

// -------------------------------
EXTERN_C Bool yiEventWait(unsigned int event, int timeout);

// -------------------------------
EXTERN_C void yiEventDelete(unsigned int event);

//#endif

#endif
