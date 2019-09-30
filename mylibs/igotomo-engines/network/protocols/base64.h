/**
* Copyright 2015 (c) igotomo
* Authorized by dongho.yoo.
* 
* @brief base64エンコーダー
*/
#ifndef _IGX_BASE64_H_
#define _IGX_BASE64_H_

#include "igx_types.h"

#ifndef __cplusplus
#   define IGX_BASE64_EXTERN extern
#else
#   define IGX_BASE64_EXTERN extern "C"
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
IGX_BASE64_EXTERN void* igx_base64_encode(const void* data,const size_t data_size,void* out_buffer,const size_t out_buffer_size,size_t* encoded_size);
/**
* @brief base64デコードします。
* @param data デコードするデータ
* @param data_size エンコードするデータのサイズ
* @param [out] out_buffer 出力バッファー、NULLの場合はヒープから割り当てます。
* @param out_buffer_size 出力バッファーサイズ、out_bufferがNULLの場合は無視されます。
* @param [out] encoded_size
* @return デコードされたバッファー
*/
IGX_BASE64_EXTERN void* igx_base64_decode(const void* data,const size_t data_size,void* out_buffer,const size_t out_buffer_size,size_t* encoded_size);
/**
* @brief igx_base64_encode,igx_base64_decodeでout_bufferにNULLを指定した場合\n
* 以下の関数でメモリを削除します。
* @param data 削除するメモリ
*/
IGX_BASE64_EXTERN void igx_base64_free(void* data);
/**
* @brief BASE64に変換された際のサイズを取得します。
* @param n 返還前のデータサイズ
*/
IGX_BASE64_EXTERN size_t igx_base64_size(size_t n);

#endif
