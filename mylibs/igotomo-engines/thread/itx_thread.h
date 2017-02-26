/** 
 * Copyright (c) 2015 Igotomo
 *
 * @brief thread for itx
 * 
 * @startcode
 * #include <itx.h>
 *
 * AnyClass::AnyMethod()
 * {
 *     igx_thread_t thread = itx::thread::createj<AnyClass>(this, &AnyClass::OnThreadCallback);
 *     igx_thread_wait(&thread);
 * }
 * void AnyClass::OnThreadCallback(void* param)
 * {
       // TODO 
 * }
 *
 * @endcode
 */
#ifndef _ITX_THREAD_H_
#define _ITX_THREAD_H_
#ifdef __cplusplus
#include "igx_thread.h"

namespace itx { namespace thread {

#ifdef __POSIX__
template <typename T> 
igx_thread_t create(T* target, void* (T::*callback)(void*))
{
    return igx::thread::create(target, (igx::thread::callback_t)callback);
}
template <typename T> 
igx_thread_t createj(T* target, void* (T::*callback)(void*))
{
    return igx::thread::create(target, (igx::thread::callback_t)callback);
}
#endif
#ifdef WIN32
template <typename T> 
igx_thread_t create(T* target, void* PASCAL (T::*callback)(void*))
{
    return igx::thread::create(target, (igx::thread::callback_t)callback);
}
template <typename T> 
igx_thread_t createj(T* target, void* PASCAL (T::*callback)(void*))
{
    return igx::thread::create(target, (igx::thread::callback_t)callback);
}
#endif

}
}
#endif



#endif
