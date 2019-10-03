/**
* Copyright 2019 (c) Wonderplanet Inc.
* Authorized by dongho.yoo.
* 
* @brief base64エンコーダー
*/
#ifndef _BASE64_H_
#define _BASE64_H_

#include <sys/types.h>
#include "igx_types.h"

#ifndef __cplusplus
#   define BASE64_EXTERN extern
#else
#   define BASE64_EXTERN extern "C"
#endif

#ifndef SECURE_NOT_EXPORT
#   define SECURE_NOT_EXPORT __attribute__((visibility("hidden")))
#endif

/**
* @brief base64エンコードします。
* @param data エンコードするデータ
* @param data_size エンコードするデータのサイズ
* @param [out] out_buffer 出力バッファー、NULLの場合はヒープから割り当てます。
* @param out_buffer_size 出力バッファーサイズ、out_bufferがNULLの場合は無視されます。
* @param [out] encoded_size
* @return エンコードされたバッファー
*/
BASE64_EXTERN void* SECURE_NOT_EXPORT BASE64_Encode(const void* data,const size_t data_size,void* out_buffer,const size_t out_buffer_size,size_t* encoded_size);
/**
* @brief base64デコードします。
* @param data デコードするデータ
* @param data_size エンコードするデータのサイズ
* @param [out] out_buffer 出力バッファー、NULLの場合はヒープから割り当てます。
* @param out_buffer_size 出力バッファーサイズ、out_bufferがNULLの場合は無視されます。
* @param [out] encoded_size
* @return デコードされたバッファー
*/
BASE64_EXTERN void* SECURE_NOT_EXPORT BASE64_Decode(const void* data,const size_t data_size,void* out_buffer,const size_t out_buffer_size,size_t* encoded_size);
/**
* @brief BASE64_Encode,BASE64_Decodeでout_bufferにNULLを指定した場合\n
* 以下の関数でメモリを削除します。
* @param data 削除するメモリ
*/
BASE64_EXTERN void SECURE_NOT_EXPORT BASE64_Free(void* data);
/**
* @brief BASE64に変換された際のサイズを取得します。
* @param n 返還前のデータサイズ
*/
BASE64_EXTERN size_t SECURE_NOT_EXPORT BASE64_GetBase64Size(size_t n);

#endif
