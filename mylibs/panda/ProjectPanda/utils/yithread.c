#ifdef __POSIX__
#include <pthread.h>
#else
#include <Process.h>
typedef unsigned long pthread_t;
#endif
#include <stdarg.h>
#include <stdio.h>
#include "yi.h"
#include "yistd.h"
#include "yimutex.h"
#include "yithread.h"

/**/
typedef void (*class_threadf)(void*, unsigned long, unsigned long, unsigned long);

/**/
typedef void (*class_threadf_typec)(unsigned long, unsigned long);

/**/
static void* __dummy_thread_callback__(void* p)
{
	void** pArgz = (void**)p;
	void* pClassInstance = pArgz[0];
	class_threadf f = (class_threadf)pArgz[1];
	HandleSignal h = (HandleMutex)pArgz[2];
	unsigned long param1 = (unsigned long )pArgz[3];
	unsigned long param2 = (unsigned long )pArgz[4];
	unsigned long param3 = (unsigned long )pArgz[5];


	yiSignalSend(h, 1);

	if (pClassInstance==0)
	{
		((class_threadf_typec)f)(param1, param2);
#ifdef __WIN32__
		_endthread();
#endif
		return 0;
	}
#ifndef __WIN32__
	//YIDEBUG("in thread %x, %x, %d\n", pClassInstance, f, param1);
	((class_threadf)f)(pClassInstance, param1, param2, param3);
#else
	__asm__ {
		push rcx;
		mov  rcx, pClassInstance
	}
	((class_threadf_typec)f)(param1, param2, param3);

	_endthread();
#endif
	pthread_exit(0);
}


pthread_t yiStartThread(xThreadCallback callback, sThreadParam* s, Bool bIsJoinable)
{
	pthread_t th;
	//YIDEBUG("yiStartThread() called\n");
	if (pthread_create(&th, 0, callback, (void*)yitoheap(s, sizeof(*s)))!=0)
	{
		return 0;
	}

	if (bIsJoinable==False)
	{
		pthread_detach(th);
	}

	return th;
}

/**/
pthread_t yiThreadStartEx(void* pClassInstance, ...)
{
	pthread_t th;
	HandleBuffer buffer;
	HandleSignal h = yiSignalCreate(buffer);
	void** pargv = ((void**)&pClassInstance);
	void* pThreadArgz[] = {pargv[0], pargv[1], (void*)h, pargv[3], pargv[4], pargv[5]};

#ifdef __POSIX__
	if (pthread_create(&th, 0, __dummy_thread_callback__, (void*)pThreadArgz)!=0)
	{
		goto FAILURE;
	}
#else
	if (_beginthread(&th, __dummy_thread_callback__, (void*)pThreadArgz, NULL)==FALSE)
	{
		goto FAILURE;
	}
#endif
	yiSignalWait(h, 1);
	yiSignalDestroy(h);

#ifdef __POSIX__
	pthread_detach(th);
#endif

	return th;

FAILURE:
	yiSignalDestroy(h);
	return -1;

}

/**/
pthread_t yiThreadStartExj(void* pClassInstance, ...)
{
	pthread_t th;
	HandleBuffer buffer;
	HandleSignal h = yiSignalCreate(buffer);
	va_list ap;
	int i;
	//void** pargv = ((void**)&pClassInstance);
	void* pThreadArgz[6];// = {pargv[0], pargv[1], (void*)h, pargv[3], pargv[4], pargv[5]};

	YIDEBUG("yiThreadStartExj called\n");

	va_start(ap, pClassInstance);

	pThreadArgz[0] = va_arg(ap, void*);
	pThreadArgz[1] = va_arg(ap, int);
	pThreadArgz[2] = (void*)h;//va_arg(ap, void*);
	pThreadArgz[3] = va_arg(ap, int);
	pThreadArgz[4] = va_arg(ap, int);
	pThreadArgz[5] = va_arg(ap, int);

	va_end(ap);

	//YIDEBUG("arg0:%x, arg1:%x, arg2:%x, arg3:%x, arg4:%x, arg5:%x\n", pargv[0], pargv[1], h, pargv[3], pargv[4], pargv[5]);
#ifdef __POSIX__
	if (pthread_create(&th, 0, __dummy_thread_callback__, (void*)pThreadArgz)!=0)
	{
		goto FAILURE;
	}
#else
	if (_beginthread(&th, __dummy_thread_callback__, (void*)pThreadArgz, NULL)==FALSE)
	{
		goto FAILURE;
	}
#endif
	yiSignalWait(h, 1);
	yiSignalDestroy(h);

	return th;

FAILURE:
	yiSignalDestroy(h);
	return -1;

}

pthread_t yiThreadStart(OnThreadCallback callback, unsigned long val, unsigned long val2)
{
	pthread_t th;
	HandleBuffer buffer;
	HandleSignal h = yiSignalCreate(buffer);
	void* pThreadArgz[] = {0, (void*)callback, (void*)val, (void*)val2};

#ifdef __POSIX__
	if (pthread_create(&th, 0, __dummy_thread_callback__, (void*)pThreadArgz)!=0)
	{
		goto FAILURE;
	}
#else
	if (_beginthread(&th, __dummy_thread_callback__, (void*)pThreadArgz, NULL)==FALSE)
	{
		goto FAILURE;
	}
#endif
	yiSignalWait(h, 1);
	yiSignalDestroy(h);

#ifdef __POSIX__
	pthread_detach(th);
#endif

FAILURE:
	yiSignalDestroy(h);
	return -1;
	
}

// ---------------------
EXTERN_C void yiThreadWait(pthread_t id)
{
	pthread_join(id, NULL);
}

// -------------------------------------
pthread_t yiThreadStartx(OnNormalThreadCallback callback, void* p, int size)
{
	pthread_t th;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&th, &attr, callback, (size==0)?p:yitoheap(p, size))!=0)
	{
		return 0;
	}

	return th;
}

