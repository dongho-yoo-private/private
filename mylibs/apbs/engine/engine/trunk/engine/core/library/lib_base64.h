/**
 * \file lib_base64.h
 * \ingroup adkgrp_library
 * \author dongho yoo
 * \~english
 * \~japanese
 *   \brief Base64 encode&decode
*/

#ifndef BASE64_h__
#define BASE64_h__

#include <core/types.h>
#include <debug_profiling/error/result.h>

ADK_BEGIN_NAMESPACE

#ifndef __cplusplus
#define BASE64_EXTERN extern "C"
#else
#define BASE64_EXTERN extern 
#endif

// --------------------------------
/**@brief base64形式に変換します。
*
*@param [out]dest データのポインター
*@param [in]source 元データ
*@param [in]size   元データのサイズ
*@param [out]changedSize 元データのサイズ
*
*@return 成功した場合はResultSuccessが返ります。
*@retval ResultOutOfResource メモリ不足
*/
BASE64_EXTERN Result LIB_Base64Encode(char** dest, const void* source, S32 size, S32* changedSize);
//BASE64_EXTERN char* LIB_Base64Encode(const void* source, S32 size, S32* changedSize);

// --------------------------------
/**@brief base64形式のデータを復元します。
*
*@param [out]dest データを受け取るアドレス。
*@param [in]source Base64の文字列
*@param [out]convertedSize 変換されたサイズ
*
*@return 成功した場合はResultSuccessが返ります。
*@retval ResultOutOfResource メモリ不足
*@retval ResultUnknown base64の形式ではない。
*/
BASE64_EXTERN Result LIB_Base64Decode(void** dest, const char* source, S32* convertedSize);
//BASE64_EXTERN void* LIB_Base64Decode(const char* source, S32* convertedSize);

/**@brief LIB_Base64で生成されたメモリを削除します。
* @param LIB_Base64Encode, LIB_Base64Decodeで生成されたメモリ
*/
void LIB_Base64Free(void* p);

ADK_END_NAMESPACE

#endif


