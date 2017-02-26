/**
 * Copyright 2015 (c) igotomo
 * authorized by dongho.yoo.
*/
#ifndef _IGX_MEMORY_H_
#define _IGX_MEMORY_H_

#include "igx_types.h"

#ifndef STORE
#   define IGX_MEMORY_DEBUG 
#   define IGX_PRINT_FREE_BACKTRACE
#endif

#ifdef __cplusplus
#   define IGX_MEMORY_EXTERN extern "C"
#else
#   define IGX_MEMORY_EXTERN extern
#endif
/**
* @brief メモリを割り当てます。(DEBUG用)
* 
* @param size サイズ
* @param tag  タグ
* @return 割り当てられたメモリ
*/
IGX_MEMORY_EXTERN void* igx_malloc(size_t size, const char* tag);
/**
* @brief メモリのサイズを変えて再度割り当てます。(DEBUG用)
*
* @param before 前回のメモリ 
* @param size サイズ
* @param tag  タグ
* @return 割り当てられたメモリ
*/
IGX_MEMORY_EXTERN void* igx_realloc(void* before, size_t size, const char* tag);
/**
* @brief igx_mallocで割り当てられたメモリを削除します。(DEBUG用)
* 
* @param p 割り当てられたメモリ
*/
IGX_MEMORY_EXTERN void igx_free(void* p);
/**
* @brief ヘッダーのサイズを取得します。
*/
IGX_MEMORY_EXTERN size_t igx_mem_header_size();
/**
* @brief タグの文字列を含むタグづけられたメモリをプリントします。
* @param tag タグ
*/
IGX_MEMORY_EXTERN void igx_memory_print(const char* tag);
/**
* @brief mmapを用いてメモリを割り当てます。大きなメモリを割り当てる場合などに使います。
* @param size 割り当てるメモリのサイズ
* @ret 割り当てられたメモリのアドレス。
*/
IGX_MEMORY_EXTERN void* igx_vmalloc(size_t size);
/**
* @brief vmallocで割り当てたメモリを削除します。
* @param 
*/
IGX_MEMORY_EXTERN void igx_vfree(void*);
/**
* @brief システムのメモリサイズを取得します。
*/
IGX_MEMORY_EXTERN size_t igx_get_page_size();

/** Cから呼び出すItxMemPageのラッパー **/
#ifndef __cplusplus
/**
* @brief メモりプールを生成します。
*
* @param buffer 初期バッファー
* @param buffer_size バッファーサイズ
* @param alignment アライメント
* @param additional_allocation 追加割当を可能にするかどうか
*
* @return ハンドル、0は失敗
*/
IGX_MEMORY_EXTERN HITX igx_new_mem_page(char* buffer, size_t buffer_size, bit32_t alignment, bool_t additional_buffer);
/**
* @brief 初期化した状態に戻します。
*/
IGX_MEMORY_EXTERN void igx_clear_mem_page(HITX handle);
/**
* @brief igx_new_mem_pageで割り当てたハンドルを解放します。
*/
IGX_MEMORY_EXTERN void igx_delete_mem_page(HITX handle);
/**
* @brief 生成したページからメモリを割り当てます。
* 
* @param handle ハンドル
* @param size   メモリのサイズ
* 
* @return 割り当てたメモリ
*/
IGX_MEMORY_EXTERN void* pmalloc(HITX handle, size_t size);

#endif

#ifndef DEBUG
#   define IGX_MALLOC(size, tag) malloc(size)
#   define IGX_REALLOC(p, size, tag) realloc(p, size)
#   define IGX_FREE(data) free(data)
#else
#   define IGX_MALLOC(size, tag) igx_malloc(size, tag)
#   define IGX_REALLOC(p, size, tag) igx_realloc(p, size)
#   define IGX_FREE(data) igx_free(data)
#endif

#ifdef __cplusplus
/**
* @brief メモリを割り当てます。(DEBUG用)
* 
* @param size サイズ
* @param tag  タグ
* @return 割り当てられたメモリ
*/
extern void* operator new(size_t size, const char* tag);
/**
* @brief igx_mallocで割り当てられたメモリを削除します。(DEBUG用)
* 
* @param p 割り当てられたメモリ
* @param null_tag 0固定
*/
extern void operator delete(void* p, const char* null_tag);

/** @brief use the macros in fllows. */
#   ifndef IGX_MEMORY_DEBUG 
#       define IGX_NEW(tag) new
#       define IGX_DEL(p) delete(p)
#   else
#       define IGX_NEW(tag) new(tag)
#       define IGX_DEL(p) operator delete((void*)p, (const char*)0) 
#   endif
#endif /* __cplusplus */

#endif
