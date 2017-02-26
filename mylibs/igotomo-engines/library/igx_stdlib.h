/**
 * Copyright 2015 (c) 
 * authorized by dongho.yoo.
 * 
 * @brief 便利系のカテゴリー化が難しいライブラリ群\n
 * 普遍的に使われるものの中で、カテゴリー化が難しいモジュールはこちらにはいる。\n
 * c++の場合は必ず__cplusplusマクロを使用し、ファイルはigx_stdlib_cpp.ccの様に別のファイルに分ける。
*/
#ifndef _IGX_STD_LIB_H_
#define _IGX_STD_LIB_H_

#include "igx_types.h"

#ifdef __cplusplus
#   define IGX_STDLIB_EXTERN extern "C"
#else
#   define IGX_STDLIB_EXTERN extern 
#endif

/**
* @brief 文字列を整数に変換します。
*
* @param string 文字列
* @param [out]ret 整数を受け取るバッファー
* @return trueの場合は整数、falseの場合は整数でない。
*/
IGX_STDLIB_EXTERN bool_t igx_atoi(const char* string, number_t* ret);
/**
* @brief 文字列をfloatに変換します。
*
* @param string 文字列
* @param [out]ret floatを受け取るバッファー
* @return trueの場合はfloat、falseの場合はfloatでない。
*/
IGX_STDLIB_EXTERN bool_t igx_atof(const char* string, float* ret);

/**
* @brief 文字列が数字であるかどうかを判別します。
*
* @param string 文字列
* @return true 数字、false 数字でない。
*/
IGX_STDLIB_EXTERN bool_t igx_is_number(const char* string);
/**
* @brief 整数を文字列に変換します。(sprintfを使うのは負荷が高いので設けた)
*
* @param buffer バッファー
* @param number 整数
* @param n 進数
*
* @return 変換された文字列の長さ
*/
IGX_STDLIB_EXTERN bit32_t igx_itoa(char* buffer, number_t number, bit32_t n);
#ifdef __cplusplus
/** TODO. for c++ **/
#endif

#endif
