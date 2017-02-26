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
typedef void __attribute__((cdecl)) (*OnThreadCallback)(unsigned long va1, unsigned long val2);

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
#endif
