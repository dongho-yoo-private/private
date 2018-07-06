/**
* CopyRight 2017 (c) WonderPlanet Inc. All Rights Reserved.
* \author dongho.yoo.
*/
#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__
#include "memory_pool_defs.h"

#ifdef __cplusplus
#   define EXTERN_C extern "C"
#else
#   define EXTERN_C extern
#endif
/**
* \brief メモリプールを初期化します。
*
* \param buffer メモリプールに使うメモリのアドレス、0の場合はmemory_pool_initの中で生成します。
* \param size 指定したメモリのサイズ、ptrが0の場合はサイズ分を内部で割り当てます。
* \return メモリプールのハンドル
*/
EXTERN_C handle_t mp_init(void* buffer, size_t size, size_t align);
/**
* \brief メモリプールを削除します。
* 
* \param handle メモリプールのハンドル
*/
EXTERN_C void mp_destroy(handle_t handle);
/**
* \brief メモリプールの機能を一時無効にします。
* \param handle メモリプールのハンドル
*/
EXTERN_C void mp_hook_pause(handle_t handle);
/**
* \brief メモリプールの機能を再開します。
* \param handle メモリプールのハンドル
*/
EXTERN_C void mp_hook_resume(handle_t ptr);
/**
* \brief メモリプールからメモリを割り当てます。\n
* ptrが0の場合は、mallocと同じ
* \param handle メモリプールのハンドル
* \param n 割り当てるメモリのサイズ
* \return 割り当てられたメモリのアドレス
*/
EXTERN_C void* mp_malloc(handle_t handle, size_t n);
/**
* \brief メモリプールからメモリを削除します。\n
* ptrが0の場合は、freeと同じ
* \param handle メモリプールのハンドル
* \param data mp_mallocで割り当てられたアドレス
*/
EXTERN_C void mp_free(handle_t ptr,void*data);
/**
* \brief 現在のメモリプールの状態を出力します。（デバッグ用）
*/
EXTERN_C void mp_print(handle_t handle, bit32_t sequence_no);
/**
* \brief シーケンス番号を設定します。
* \param handle ハンドル
* \param sequence_no シーケンス番号
*/
EXTERN_C void mp_set_sequence_no(handle_t handle, bit32_t sequence_no);
#endif
