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
* \param filters フィルター（バックとレースに文字列が含まれているとプリントしない）
* \param sequence_no 特定のシーケンス番号を出力します。（0の場合は全て）
*/
EXTERN_C void memory_pool_print(void* ptr, const char* filters[], bit32_t sequence_no);
/**
* \brief 現在のシーケンス番号を設定します。(optional)
* \param ptr メモリプールのアドレス
* \param sequence_no シーケンス番号
*/
EXTERN_C void memory_pool_set_sequence_no(void* ptr, bit32_t sequence_no);
/**
* \brief 特定のシーケンス番号が割振れているメモリを全て解放します。
* \param ptr メモリプールのアドレス
* \param sequence_no シーケンス番号
*/
EXTERN_C void memory_pool_free_by_sequence_no(void* ptr, bit32_t sequence_no);
/**
* \brief メモリが正常化どうかをチェックします。
* \param ptr 割り当てられたメモリのアドレス
* \return 状態（0:正常、1:エラー)
*/
EXTERN_C int memory_pool_check(void* ptr);
#endif
