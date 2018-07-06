/**
* CopyRight 2017 (c) WonderPlanet Inc. All Rights Reserved.
* \author dongho.yoo.
*/
#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__
#include "memory_pool_defs.h"
/**
* \brief メモリプールを初期化します。
* 
* \param buffer メモリプールに使うメモリのアドレス、0の場合はmemory_pool_initの中で生成します。
* \param size 指定したメモリのサイズ、ptrが0の場合はサイズ分を内部で割り当てます。
* \return メモリプールのハンドル
*/
extern handle_t mp_init(void* buffer, size_t size, size_t align=16);
/**
* \brief メモリプールを削除します。
* 
* \param handle メモリプールのハンドル
*/
extern void mp_destroy(handle_t handle);
/**
* \brief メモリプールの機能を一時無効にします。
* \param handle メモリプールのハンドル
*/
extern void mp_hook_pause(handle_t handle);
/**
* \brief メモリプールの機能を再開します。
* \param handle メモリプールのハンドル
*/
extern void mp_hook_resume(handle_t ptr);
/**
* \brief メモリプールからメモリを割り当てます。\n
* ptrが0の場合は、mallocと同じ
* \param handle メモリプールのハンドル
* \param n 割り当てるメモリのサイズ
* \return 割り当てられたメモリのアドレス
*/
extern void* mp_malloc(handle_t handle, size_t n);
/**
* \brief メモリプールからメモリを削除します。\n
* ptrが0の場合は、freeと同じ
* \param handle メモリプールのハンドル
* \param data mp_mallocで割り当てられたアドレス
*/
extern void mp_free(handle_t ptr,void*data);
/**
* \brief 現在のメモリプールの状態を出力します。（デバッグ用）
*/
extern void mp_print(handle_t handle, bit32_t sequence_no=0);
/**
* \brief シーケンス番号を設定します。
* \param handle ハンドル
* \param sequence_no シーケンス番号
*/
extern void mp_set_sequence_no(handle_t handle, bit32_t sequence_no);

#ifdef __cplusplus
/**
* \brief メモリプールから new します。
* \param handle メモリプールのハンドル
*/
extern void* operator new (size_t size, handle_t handle);
/**
* \brief メモリプールから delete します。
* \param handle メモリプールのハンドル
*/
extern void operator delete (void* data, handle_t handle);
#endif /* __cplusplus */

#endif
