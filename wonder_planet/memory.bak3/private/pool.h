/**
* Copyright 2017 (c) WonderPlanet Inc. All Rights Reserved.
* \author dongho.yoo
*/
#ifndef __POOL_H__
#define __POOL_H__

#include "../memory_pool_defs.h"

#ifdef __cplusplus
#   define EXTERN_C extern "C"
#else
#   define EXTERN_C extern
#endif

/**
* \brief メモリプールを初期化します。\n
* デバッグモード時には割り当てた際のタイムスタンプとバックとレースを取っておく
* \param ptr メモリのアドレス
* \param size サイズ
* \param alignment アライメント
* \param for_debug 1の場合はデバッグモード、0の場合はデバッグモードではない
*/
EXTERN_C void memory_pool_init_struct(void* ptr,size_t size,size_t alignment);
/**
* \brief メモリプールからメモリを割り当てます。
* \param ptr メモリのアドレス
* \param size サイズ
* \param alignment アライメント
*/
EXTERN_C void* memory_pool_malloc(void* ptr, size_t n);
/**
* \brief メモリプールからメモリを削除します。
* \param ptr メモリプールのアドレス
* \param data 削除するメモリ
*/
EXTERN_C void memory_pool_free(void* ptr, void* data);
/**
* \brief 
* \param ptr メモリのアドレス
* \param size サイズ
* \param alignment アライメント
*/
EXTERN_C void memory_hook_pause(void* ptr);
/**
* \brief メモリプールからメモリを割り当てます。
* \param ptr メモリのアドレス
* \param size サイズ
* \param alignment アライメント
*/
EXTERN_C void memory_hook_resume(void* ptr);
/**
* \brief 現在のプールの状態をプリントします。
*/
EXTERN_C void memory_pool_print(void* ptr);
#endif
