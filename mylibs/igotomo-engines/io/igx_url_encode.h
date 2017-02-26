/**
* Copyright (c) 2015
*
* Authorized by dongho.yoo.
*/
#ifndef __IGX_URL_ENCODE_H__
#define __IGX_URL_ENCODE_H__

#include "igx_types.h"

#ifdef __cplusplus
#   define IGX_URL_ENCODE_EXTERN extern "C"
#else
#   define IGX_URL_ENCODE_EXTERN extern 
/**
* @brief urlエンコードします。
* 
* @param buffer バッファーに０指定の場合は、ヒープ上にメモリを割り当てます。
* @param len    バッファーサイズ
* @param src    ソース
* @param src_len ソースのサイズ 0指定の場合は、文字列として扱い、長さを計ります。
* @param encoded_len エンコードされたサイズ
*
* @return bufferのポインター
*/
IGX_URL_ENCODE_EXTERN char* igx_url_encode(char* buffer, size_t len, const char* src, size_t src_len, size_t* encoded_len);
/**
* @brief urlデコードします。
* 
* @param buffer バッファーに０指定の場合は、ヒープ上にメモリを割り当てます。
* @param len    バッファーサイズ
* @param src    ソース
* @param src_len ソースのサイズ 0の場合は文字列の長さを内部で計ります。
* @param encoded_len エンコードされたサイズ
*
* @return bufferのポインター
*/
IGX_URL_ENCODE_EXTERN char* igx_url_decode(char* buffer, size_t len, const char* src, size_t src_len, size_t* encoded_len);
#endif


#endif
