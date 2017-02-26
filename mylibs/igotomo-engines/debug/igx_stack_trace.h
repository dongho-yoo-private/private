/**
* Copyright (C) 2015 igotomo.
*
* authorized by dongho.yoo.
*/
#ifndef _IGX_STACK_TRACE_H_
#define _IGX_STACK_TRACE_H_

#include "igx_types.h"

#ifdef __cplusplus
#   define IGX_STACK_TRACE_EXTERN extern "C"
#else
#   define IGX_STACK_TRACE_EXTERN extern
#endif

/**
* @brief スタックトレースを取得します。
*
* @param buffer 受け取るバッファー
* @param max_count バッファーの最大の数
*
* @return 受け取ったバックとレースの数
*
* @startcode
*     :
* void* stack[10];
* int n;
* n = igx_get_stacktrace(stack, 10);
*
* for (i=0; i<n;++i)
* {
*    printf("%s\n", igx_get_symbole_name(stack[i]));
* }
* @endcode
*/
IGX_STACK_TRACE_EXTERN bit32_t igx_get_stacktrace(void* buffer[], bit32_t max_count);
/**
* @brief 関数のアドレスからシンボルを取得します。
* 
* @param addr 関数のアドレス
* @return シンボルの文字列
*/
IGX_STACK_TRACE_EXTERN const char* igx_get_symbole_name(void* addr);
/**
* @brief バックトレースを出力します。
* @param addr アドレス
* @param count バッファーの数
* @param start 表示開始のインデックス
*/
IGX_STACK_TRACE_EXTERN void igx_print_stacktrace(void* addr[], bit32_t count, bit32_t start);
/**
* @brief 現在のバックとレースを出力します。
*/
IGX_STACK_TRACE_EXTERN void igx_print_current_stacktrace();
/**
* @brief アサーション
*/
IGX_STACK_TRACE_EXTERN void igx_assert(bool_t condition, const char* format, ...);
#endif
