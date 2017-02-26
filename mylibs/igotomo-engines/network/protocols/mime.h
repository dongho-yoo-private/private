/**
* Copyright (c) 2015 igotomo.
*
* Authorized by dongho.yoo.
*/
#ifndef __IGX_MIME_H__
#define __IGX_MIME_H__

#include "igx_types.h"
#include "library/igx_string.h"

#ifdef __cplusplus
#   define IGX_MIME_EXTERN extern "C"
#else
#   define IGX_MIME_EXTERN extern 
#endif

/**
* @brief デフォルトの文字列のロケールを取得します。
*/
IGX_MIME_EXTERN const char*  igx_mime_get_default_local_charset(int locale); 
/**
* @brief "anystring" -> =?charset?B?encoded-text?=
* 
* @param [out] buffer 受け取るバッファー
* @param [in][out] buffer_size バッファーサイズ　
* @param text エンコードするテキスト
* @param carset 文字コード
* @return 成功した場合はbufferのアドレスを返します。
*/
IGX_MIME_EXTERN char* igx_mime_encrypt_base64(char* buffer, size_t* buffer_size, const char* text, const size_t text_len, const char* charset);
/**
* @brief =?charset?B?encoded-text?= -> "anystring" utf-8に変換して返します。
* 
* @param [out] buffer 受け取るバッファー
* @param [in][out] buffer_size バッファーサイズと受け取るバッファーサイズ
* @param mime base64形式のデータ
* @param mime の長さ
* @return 成功した場合はbufferのアドレスを返します。
*/
IGX_MIME_EXTERN char* igx_mime_decrypt_base64(char* buffer, size_t* buffer_size, const char* mime, size_t mime_size);
/**
* @breif <emailaddress> displayname などの形式からdisplay名とemailアドレスを取得します。
* 
* @param [in] email_address mime形式のメールアドレス\n
*  case1. =?charset?B?string?= <email> \n
*  case2. =?charser?Q?string?= <email> \n
*  case3. "displayname" <email> \n
*  case4. <email> \n
*  case5. email  
* @param [out] display ディスプレイ名 (文字列のポインターとサイズが返ります。)
* @param [out] email  emailアドレス (文字列のポインターとサイズが返ります。)
*/
IGX_MIME_EXTERN bool_t igx_mime_get_name_and(const char* email_address, const size_t email_len, igx_string_t* display, igx_string_t* email);
/**
* @brief 文字列が'=?charset?B?encoded-text?=' こんなものかどうかを返します。
*
* @param text テキスト
* @param len textのサイズ
*/
IGX_MIME_EXTERN bool_t igx_is_mime_format(const char* text, size_t len);
/**
* @brief 文字列が7bitであるかどうかをチェックします。
* @param text テキスト
* @param len テキストの長さ
* @return true 7bit, false 8bit
*/
IGX_MIME_EXTERN bool_t igx_mime_is_7bit(const char* text, size_t len);
/**
* @brief 以下の酔うなフォーマットに変換します。
*  case1. =?charset?B?string?= <email> \n
*  case2. =?charser?Q?string?= <email> \n
*  case3. "displayname" <email> \n
*  case4. <email> \n
*  case5. email  
* @param [out] buffer 格納するバッファー
* @param [in]  buffer_size バッファーサイズ
* @param [in] name 表示名
* @param [in] email email
* @return 変換された文字列の長さ
*/
IGX_MIME_EXTERN size_t igx_mime_user(char* buffer, size_t buffer_size, const igx_string_t* name, const igx_string_t* email);
#endif
