/** 
 * wsxthread.h
 *
 * スレッドを生成します。
 *
 * Updated: 2009. 7.31. 初版 (version. 0.9.1)
 *
 */
#ifndef __yithread_h__
#define __yithread_h__

#include <stdarg.h>
#include "yi.h"
#ifdef __POSIX__
#include <pthread.h>
#endif
/**/
#ifdef __OSX__
typedef void (*OnThreadCallback)(unsigned long va1, unsigned long val2);
#else
typedef void __attribute__((cdecl)) (*OnThreadCallback)(unsigned long va1, unsigned long val2);
#endif

/**/
#ifdef __OSX__
typedef void* (*OnNormalThreadCallback)(void*);
#else
typedef void* __attribute__((cdecl)) (*OnNormalThreadCallback)(void*);
#endif

/*
 * Updated' 2009. 7.31.
 */
EXTERN_C pthread_t yiThreadStart(OnThreadCallback callback, unsigned long val, unsigned long val2);

/*
 * ex) for C++
 * AnyType AnyClass::AnyFunction(AnyType a....)
 * {
 *
 * 	unsigned long id = yiThreadStart(this, &AnyClass::OnThreadCallback, param1, param2);
 *
 * 			:
 * 			:
 * 			:
 *  }
 *
 *  void AnyClass::OnThreadCallback(unsigned long param1, unsigned long param2)
 *  {
 *  			;
 *  			;
 *  }
 *
 * Updated' 2009. 7.31.
 * */
EXTERN_C pthread_t yiThreadStartEx(void* pClassInstance, ...);

/*
 * 上記のyiThreadStartExとの違いはJoinが可能である。
 * */
EXTERN_C pthread_t yiThreadStartExj(void* pClassInstance, ...);

/*
 * yiThreadStartExjで実行されたスレッドに対してのみ
 * */
EXTERN_C void yiThreadWait(pthread_t id);

/*
 * yiThreadStartExjで実行されたスレッドに対してのみ
 * */
EXTERN_C pthread_t yiThreadStartx(OnNormalThreadCallback callback, void* p, int size);


typedef void* (*xThreadCallback)(void* param);

typedef struct {
	void* pClassInstance;
	unsigned long param[6];	
} sThreadParam;

/*
 * Updated' 2011.11.13.
 */
EXTERN_C pthread_t yiStartThread(xThreadCallback callback, sThreadParam* param, Bool bIsJoinable);

#define _THREAD_END_(p) pthread_exit(0);\
yifree(p);

/* template */
/*
void* template(void* param)
{
	sThreadParam* p = param;
		
	_THREAD_END_(param)
	return 0;
}
*/

#endif
