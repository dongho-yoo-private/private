#include <core/library/lib_thread.h>


#ifdef __USE_PTHREAD__
#include <pthread.h>
#endif
#include <sys/syscall.h>
#include <sys/types.h>

#include <core/memory_allocation/guarded_alloc.h>

#define __USE_PTHREAD__ 

ADK_BEGIN_NAMESPACE

typedef struct {
#ifdef __USE_PTHREAD__
pthread_t id;
pid_t	  tid;
#endif
void* param;
Bool isJoinable;
LIB_TThreadCallback pfnCallback;
} _TThread;

// ---------------------------------------
static void* xOnThread(void* param)
{
	_TThread* s = (_TThread*)param;

	if (s==0)
		return 0;

	s->tid=LIB_ThreadGetThreadId(0);

	s->pfnCallback(s->param);

	if (s->isJoinable!=0)
	{
#ifdef __USE_PTHREAD__
        // する必要ないけど、念のため。
	// するにしてもこのタイミングで良いかどうか、
	// 生成されてすぐにすべきかどうか。
        pthread_detach(s->id);
#endif
		ADK_Free(s);
	}
    
    return 0;
}

// --------------------------------
IntPtr LIB_ThreadCreate(LIB_TThreadCallback pfnCallback, void* param, Bool isJoinable)
{
	_TThread* s=0;

#ifdef __USE_PTHREAD__
	pthread_attr_t attr;
	
	if (pthread_attr_init(&attr)!=0)
	{
		return 0;
	}

	if (isJoinable==false)
	{
		if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)!=0)
		{
			return 0;
		}
	}
	
	s = (_TThread*)ADK_Malloc(sizeof(*s), "LIB_THREAD_CREATE");
	s->param=param;
	s->isJoinable=isJoinable;
	s->pfnCallback=pfnCallback;
	if (pthread_create(&s->id, &attr, xOnThread, s)!=0)
	{
		ADK_Free(s);
		return 0;
	}

	pthread_attr_destroy(&attr);
#endif /*__USE_PTHREAD__ */

	return (IntPtr)s;
}

// -----------------------------
Bool LIB_ThreadJoin(IntPtr threadId)
{
#ifdef __USE_PTHREAD__
	void* ret;
#endif
	_TThread* s = (_TThread*)threadId;
	if (s==0)
	{
		return false;
	}

#ifdef __USE_PTHREAD__
	if (pthread_join(s->id, &ret)==0)
	{
		return true;
	}
#endif

	return false;	
}

// ----------------------------------
U32 LIB_ThreadGetThreadId(U32 id)
{
	_TThread* s = (_TThread*)id;


	if (s==0)
	{
#ifdef __USE_PTHREAD__
		return (U32)(size_t)pthread_self();
#else
		return 0;
#endif
		//return 0;//sysca(SYS_gettid);
	}

	return s->tid;
}

// ----------------------------------
S32 LIB_MutexGetHandleSize()
{
	return sizeof(pthread_mutex_t)+sizeof(S32);
}

// ----------------------------------
HandleMutex LIB_MutexCreate()
{
	return LIB_MutexCreateInBuffer(0);
}

// ----------------------------------
HandleMutex LIB_MutexCreateInBuffer(void* buffer)
{
#ifdef __USE_PTHREAD__
	pthread_mutex_t* s = buffer?(pthread_mutex_t*)buffer:(pthread_mutex_t*)ADK_Malloc(sizeof(*s)+sizeof(S32), "LIB_MutexCreate");
	pthread_mutex_init(s, 0);

	if (buffer)
	{
		*((int*)&s[1])=1;
	}
	else
	{
		*((int*)&s[1])=0;
	}

	return (HandleMutex)s;
#endif
#ifdef __USE_SEMAPHORE__
	sem_t* s = buffer?(sem_t*)buffer:(sem_t*)ADK_Malloc(sizeof(*s)+sizeof(S32), "LIB_MutexCreate");
	pthread_mutex_init(s, 0);

	if (buffer)
	{
		*((int*)&s[1])=1;
	}
	else
	{
		*((int*)&s[1])=0;
	}

	return (HandleMutex)s;
#endif
	return 0;
}

// -------------------------------------
void LIB_MutexDestroy(HandleMutex hMutex)
{
	Bool bFree=true;

	if (hMutex)
	{
#ifdef __USE_PTHREAD__
		pthread_mutex_t* s=(pthread_mutex_t* )hMutex;
		pthread_mutex_destroy((pthread_mutex_t*)hMutex);
		if (*((S32*)&s[1])==1)
		{
			bFree=false;
		}
#endif

		if (bFree)
		{
			ADK_Free(hMutex);
		}
	}
}

// -------------------------------------
Bool LIB_MutexLock(HandleMutex hMutex, Bool isTry)
{
	if (hMutex)
	{
#ifdef __USE_PTHREAD__

		if (isTry==true)
		{
			int ret;
			if ((ret=pthread_mutex_trylock((pthread_mutex_t*)hMutex))==0)
			{
				return true;
			}
		}
		else
		{
			if (pthread_mutex_lock((pthread_mutex_t*)hMutex)==0)
			{
				return true;
			}
		}
#endif
	}

	return false;
}

// -------------------------------------
Bool LIB_MutexUnlock(HandleMutex hMutex)
{
	if (hMutex)
	{
#ifdef __USE_PTHREAD__
		return !pthread_mutex_unlock((pthread_mutex_t*)hMutex);
#endif
	}
	return false;
}
ADK_END_NAMESPACE

