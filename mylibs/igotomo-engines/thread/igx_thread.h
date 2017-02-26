/** 
 * Copyright (c) 2015 
 *
 * @brief スレッドを生成します。
 * 
 * @startcode
 * // ++++++++++++++++ for C
 *  igx_thread_t handle = igx_thread_create(OnThreadCallback, (void*)any_parameter);
 * 
 * // ++++++++++++++++ for C++
 * igx_thread_t handle = igx::thread::create(this, (igx::thread::callback_t)Callback, (void*)any_parameter); 
 *
 * @endcode
 */
#ifndef _IGX_THREAD_H_
#define _IGX_THREAD_H_

#include "igx_types.h"
#ifdef __POSIX__
#   include <pthread.h>
#endif
#ifdef WIN32
#   include <process.h>
#endif

#ifdef __cplusplus
#   define IGX_THREAD_EXTERN extern "C"
#else
#   define IGX_THREAD_EXTERN extern 
#endif

/**/
#ifdef __POSIX__
typedef void* (*igx_thread_callback_t)(void*);
typedef pthread_t igx_thread_t;
#endif 
#ifdef WIN32
typedef void* __attribute__((pascall)) (*igx_thread_callback_t)(void*)
typedef uintptr_t igx_thread_t;
#endif


#ifdef __cplusplus
namespace igx{ namespace thread {
/**@brief for this call*/
class Target {
};
#ifdef __POSIX__
typedef void* (Target::*callback_t)(void* );
#endif
#ifdef WIN32
typedef void* __attribute__((pascal)) (Target::*callback_t)(void* );
#endif
}
}

#endif

/**
* @brief スレッドを生成します。(detatch)
*
* @param callback コールバック
* @param parameter スレッドに渡すパラメーター
* @return スレッドハンドル
*/
IGX_THREAD_EXTERN igx_thread_t igx_thread_create(igx_thread_callback_t callback, void* parameter);
/**
* @brief スレッドを生成します。(joinable)
*
* @param callback コールバック
* @param parameter スレッドに渡すパラメーター
* @return スレッドハンドル
*/
IGX_THREAD_EXTERN igx_thread_t igx_thread_createj(igx_thread_callback_t callback, void* parameter);

#ifdef __cplusplus /* for thiscall */
namespace igx { namespace thread {
/**
* @brief スレッドを生成します。(detatch)\n
* thiscall 用
*
* @param target コールバックのクラスのインスタンス
* @param callback コールバック
* @param parameter スレッドに渡すパラメーター
* @return スレッドハンドル
*/
IGX_THREAD_EXTERN igx_thread_t create(void* target, callback_t callback, void* parameter);
/**
* @brief スレッドを生成します。(joinable)\n
* thiscall 用
*
* @param target コールバックのクラスのインスタンス
* @param callback コールバック
* @param parameter スレッドに渡すパラメーター
* @return スレッドハンドル
*/
IGX_THREAD_EXTERN igx_thread_t createj(void* target, callback_t callback, void* parameter);
}}
#endif
/**
* @brief joinableのスレッドの終了を待ちます。
*/
IGX_THREAD_EXTERN void igx_thread_join(igx_thread_t handle);

#ifdef __POSIX__
typedef pthread_mutex_t igx_mutex_t;
#endif
#ifdef WIN32
typedef CRETICAL_SECTION igx_mutex_t;
#endif

/**
* @brief ミューテックスを生成します。
* @brief mutex ハンドルのアドレス。NULL不可
* @return ミューテックスのハンドルが返ります。NULLの場合はしっぱい
*/
IGX_THREAD_EXTERN igx_mutex_t* igx_mutex_init(igx_mutex_t* mutex);
/**
* @brief ミューテックスロック
* @param mutex ハンドル
* @retval true ロックに成功
* @retval false ロックに失敗
*/
IGX_THREAD_EXTERN bool_t igx_mutex_lock(igx_mutex_t* mutex);
/**
* @brief ミューテックスアンロック
* @param mutex ハンドル
* @retval true ロックに成功
* @retval false ロックに失敗
*/
IGX_THREAD_EXTERN void igx_mutex_unlock(igx_mutex_t* mutex);
/**
* @brief ミューテックストライロック
* @param mutex ハンドル
*
* @retval true ロックに成功
* @retval false ロックに失敗
*/
IGX_THREAD_EXTERN bool_t igx_mutex_trylock(igx_mutex_t* mutex);
/**
* @brief ミューテックスのハンドルを解放します。
* @param mutex ハンドル
*
* @retval true ロックに成功
* @retval false ロックに失敗
*/
IGX_THREAD_EXTERN void igx_mutex_term(igx_mutex_t* mutex);

#endif
