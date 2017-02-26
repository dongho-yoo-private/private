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
 * Updated: 2009. 7.31.
 * */
EXTERN_C void FASTCALL yiMutexUnlock(HandleMutex h);

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

#endif
