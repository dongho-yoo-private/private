/**
* Copyright (c) 2015
*
* Authorized by dongho.yoo.
*/
#ifndef __IGX_CHECKSUM_H__
#define __IGX_CHECKSUM_H__

#include "igx_types.h"

#ifdef __cplusplus
#   define IGX_CHECK_SUM_EXTERN extern "C"
#else
#   define IGX_CHECK_SUM_EXTERN extern 
#endif

/**
* @brief チェックサムを返します。
* 
* @param data チェックサムを計算するバッファー
* @param size データのサイズ
* @param read_bytes 一度に読み込むバイト数 (1〜4) 0の場合は4
*
* @return チェックサム
*/
IGX_CHECK_SUM_EXTERN ubit32_t igx_get_checksum(const void* data, size_t size, bit32_t read_bytes);

#endif
