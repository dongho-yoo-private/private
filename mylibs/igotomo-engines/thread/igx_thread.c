#include "igx_thread.h"

#ifdef __POSIX__
#include <pthread.h>
#endif
#ifdef WIN32
#include <process.h>
typedef uintptr_t pthread_t;
#endif

static void* __on_thread__(void* param)
{
#ifdef __POSIX__
    struct __thread_args__{
        igx_thread_callback_t callback;
        void* parameter;
        igx_mutex_t mutex;
    } thread_args = *((struct __thread_args__*)param);

    igx_mutex_unlock(&thread_args.mutex);
    return thread_args.callback(thread_args.parameter);
#endif
}
igx_thread_t igx_thread_createj(igx_thread_callback_t callback, void* parameter)
{
#ifdef WIN32
#else
	pthread_t th;
    struct {
        igx_thread_callback_t callback;
        void* parameter;
        igx_mutex_t mutex;
    } thread_args = {callback, parameter, 0};

    igx_mutex_init(&thread_args.mutex);
    igx_mutex_lock(&thread_args.mutex);
	if (pthread_create(&th, 0, __on_thread__, &thread_args)!=0)
	{
        igx_mutex_unlock(&thread_args.mutex);
		return 0;
	}
    // やった！デッドロック!
    igx_mutex_lock(&thread_args.mutex);
    igx_mutex_unlock(&thread_args.mutex);
    igx_mutex_term(&thread_args.mutex);

    return th;
#endif
}
igx_thread_t igx_thread_create(igx_thread_callback_t callback, void* parameter)
{
#ifdef WIN32
#else
    igx_thread_t ret=igx_thread_createj(callback, parameter);
    pthread_detach(ret);
    return ret;
#endif
}

void igx_thread_join(igx_thread_t handle)
{
#ifdef WIN32
    WaitForSingleObject(handle, INFINITE);
#endif
#ifdef __POSIX__
    void* ret=0;
    pthread_join(handle, &ret);
#endif
}
igx_mutex_t* igx_mutex_init(igx_mutex_t* mutex)
{
#ifdef __POSIX__
    return pthread_mutex_init(mutex, 0)==0?mutex:0;
#endif
#ifdef WIN32
    //CRITICAL_SECTION critical_section;
#endif
}
bool_t igx_mutex_lock(igx_mutex_t* mutex)
{
#ifdef __POSIX__
    return (pthread_mutex_lock(mutex)==0)?true:false;
#endif
    return false;
}
bool_t igx_mutex_trylock(igx_mutex_t* mutex)
{
#ifdef __POSIX__
    return (pthread_mutex_trylock(mutex)==0)?true:false;
#endif
    return false;
}
void igx_mutex_unlock(igx_mutex_t* mutex)
{
#ifdef __POSIX__
    pthread_mutex_unlock(mutex);
#endif
}
void igx_mutex_term(igx_mutex_t* mutex)
{
#ifdef __POSIX__
    pthread_mutex_destroy(mutex);
#endif
    return ;
}



