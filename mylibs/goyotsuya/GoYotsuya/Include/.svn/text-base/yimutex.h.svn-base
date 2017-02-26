
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


typedef Handle HandleMutexx;

/*
 * */
typedef Handle HandleSignal;



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
xEXTERN_C HandleMutex yiMutexCreate(HandleBuffer in_handle);

/*
 *
 * Updated: 2009. 7.31.
 * */
xEXTERN_C void yiMutexDestroy(HandleMutex h);

/*
 * Updated: 2009. 7.31.
 * */
xEXTERN_C void FASTCALL yiMutexLock(HandleMutex h);

/*
 * Updated: 2009. 7.31.
 * */
xEXTERN_C void FASTCALL yiMutexUnlock(HandleMutex h);

/**/
xEXTERN_C HandleMutexx yiMutexCreateEx(int nSpinCount);

/**/
xEXTERN_C void yiMutexLockEx(HandleMutexx h);

/**/
xEXTERN_C Bool yiMutexTryEx(HandleMutexx h);

/**/
xEXTERN_C void yiMutexUnlockEx(HandleMutexx h);

/**/
xEXTERN_C void yiMutexDestroyEx(HandleMutexx h);

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
xEXTERN_C HandleSemaphore yiSemaphoreCreate(HandleBuffer h, int nCount);

/*
 * Updated: 2009. 7.31.
 * */
xEXTERN_C void FASTCALL yiSemaphoreLock(HandleSemaphore h);

/*
 * Updated: 2009. 7.31.
 * */
xEXTERN_C void FASTCALL yiSemaphoreUnlock(HandleSemaphore h);

xEXTERN_C void yiSemaphoreDestroy(HandleSignal h);

/*
 * Updated: 2009. 8. 3.
 * */
xEXTERN_C HandleSignal yiSignalCreate(HandleBuffer h);

/*
 * Updated: 2009. 7.31.
 * */
xEXTERN_C void yiSignalWait(HandleSignal h, unsigned long uSigMask);

/*
 * Updated: 2009. 7.31.
 * */
xEXTERN_C void yiSignalSend(HandleSignal h, unsigned long uSigMask);

xEXTERN_C void yiSignalDestroy(HandleSignal h);
#ifdef __POSIX__

typedef Handle HandleMutexx;

// -------------------------------
xEXTERN_C HandleMutexx yiMutexCreatex();

// -------------------------------
xEXTERN_C void yiMutexLx(HandleMutexx h);

// -------------------------------
xEXTERN_C void yiMutexUnLx(HandleMutexx h);

// -------------------------------
xEXTERN_C void yiMutexDestroyx(HandleMutexx h);

// -------------------------------
xEXTERN_C void yiMutexWaitx(HandleMutexx h);
#else
#define yiMutexCreatex() yiMutexCreate(0)
#define yiMutexLx(h) yiMutexLock(h)
#define yiMutexUnLx(h) yiMutexUnlock(h)
#define yiMutexDestroyx(h) yiMutexDestroy(h)
#endif 

#ifdef WIN32
#define AnyDescriptor HANDLE 
#else
#define AnyDescriptor int 
#define INFINITE	-1
#endif

// -------------------------------
xEXTERN_C Bool yiWaitObject(AnyDescriptor discriptor, int timeout);

// -------------------------------
xEXTERN_C unsigned int yiEventInit();

// -------------------------------
xEXTERN_C void yiEventSet(unsigned int event);

// -------------------------------
xEXTERN_C void yiEventReset(unsigned int event);

// -------------------------------
xEXTERN_C Bool yiEventWait(unsigned int event, int timeout);

// -------------------------------
xEXTERN_C void yiEventDelete(unsigned int event);
#endif
