/**
 * \author dongho yoo
 * \file igx_profile.h
 * \~english
 * \~japanese
 * 	\brief Linked List Library.
 */
#ifndef __IGX_PROFILE_H__
#define __IGX_PROFILE_H__

#include "igx_types.h"

#ifdef __cplusplus
#define IGX_PROFILE_EXTERN extern "C"
#else
#define IGX_PROFILE_EXTERN extern 
#endif

typedef void* HIGX_PROFILE;

/**@brief profileファイルを開きます。
*
*@param pszFileName Profileファイル名
*
*@return 成功した場合は0でないハンドルが返ります。
*/
IGX_PROFILE_EXTERN HIGX_PROFILE igx_profile_open(const char* pszFileName);

/**@brief profileファイルからkeyと値を文字列で取得します。
*
*@param [in] hProfile igx_profile_Openで取得したハンドル
*@param [in] section  文字列のセクションで最大64バイトまで 
*@param [in] key	 文字列のキー
*@param [out] value 	 文字列の値
*@return 成功した場合はtrueが返ります。
*@retval true 成功
*@retval false 失敗
*@code
* // sample.
* char version[32];
* HIGX_PROFILE hProfile = igx_profile_Open("StartupInfo.inf");
* if (igx_profile_ReadStringEx(hProfile, "System", "Version", version)==false)
* {
* 	// エラー処理
* }
* igx_profile_Close(hProfile);
*@endcode
*/
IGX_PROFILE_EXTERN Bool igx_profile_read_string_ex(HIGX_PROFILE hProfile, const char* section, const char* key, char* value);
/**
* @brief profileファイルからkeyと値を整数で取得します。
*
* @param [in] hProfile igx_profile_Openで取得したハンドル
* @param [in] section  文字列のセクションで最大64バイトまで 
* @param [in] key	 文字列のキー
* @param [out] value 	 整数のポインター
*
* @return 成功した場合はtrueが返ります。
* @retval true 成功
* @retval false 失敗
* @code
* // sample.
* bit32_t nConnectTimeout;
* HIGX_PROFILE hProfile = igx_profile_Open("StartupInfo.inf");
* if (igx_profile_read_int_ex(hProfile, "Network", "ConnectTimeout", &nConnectTimeout)==false)
* {
* 	// エラー処理
* }
* igx_profile_Close(hProfile);
* @endcode
*/
IGX_PROFILE_EXTERN Bool igx_profile_read_int_ex(HIGX_PROFILE hProfile, const char* section, const char* key, bit32_t* val);

/**@brief profileファイルにsection, key, valueを設定します。\n
* sectionやkeyがない場合は新たに生成されます。
*
*@param [in] hProfile igx_profile_Openで取得したハンドル
*@param [in] section  文字列のセクションで最大64バイトまで 
*@param [in] key	 文字列のキー
*@param [in] value 	 整数値
*
*@return 成功した場合はtrueが返ります。
*@retval true 成功
*@retval false 失敗
*@code
* // sample.
* bit32_t nConnectTimeout;
* HIGX_PROFILE hProfile = igx_profile_Open("StartupInfo.inf");
* if (igx_profile_WriteIntEx(hProfile, "Network", "ConnectTimeout", 30000)==false) 
* {
* 	// エラー処理
* }
* igx_profile_Close(hProfile);
*@endcode
*/
IGX_PROFILE_EXTERN Bool igx_profile_write_int_ex(HIGX_PROFILE hProfile, const char* section, const char* key, bit32_t nDefaultNum);
/**
* @brief profileファイルにsection, key, valueを設定します。\n
* sectionやkeyがない場合は新たに生成されます。
*
* @param [in] hProfile igx_profile_Openで取得したハンドル
* @param [in] section  文字列のセクションで最大64バイトまで 
* @param [in] key	 文字列のキー
* @param [in] value 	 文字列
* 
* @return 成功した場合はtrueが返ります。
* @retval true 成功
* @retval false 失敗
* @code
*  // sample.
*  bit32_t nConnectTimeout;
*  HIGX_PROFILE hProfile = igx_profile_Open("StartupInfo.inf");
*  if (igx_profile_WriteStringEx(hProfile, "Debug", "Debug", "ON")==false) 
*  {
*  	// エラー処理
*  }
*  igx_profile_Close(hProfile);
* @endcode
*/
IGX_PROFILE_EXTERN Bool igx_profile_write_string_ex(HIGX_PROFILE hProfile, const char* section, const char* key, const char* value);

/**@brief 指定したsectionのキーを列挙します。
*
* @param [in] hProfile igx_profile_Openで取得したハンドル
* @param [in] section  文字列のセクションで最大64バイトまで 
* @param [out] num     リストの数
* 
* @return 成功した場合は0以外のポインターが返ります。
* @retval 0 失敗(セクションが存在しないか、keyがない)
* @code
*  // sample.
*  bit32_t nConnectTimeout;
*  char** keyz;
*  HIGX_PROFILE hProfile = igx_profile_Open("StartupInfo.inf");
*  keyz = igx_profile_EnumKeysEx(hProfile, "HostList", &cnt);
*  if (keyz==0)
*  {
*  	// エラー処理
*  }
*  igx_profile_Close(hProfile);
*  
*  for (i=0; i<cnt; i++)
* 	prbit32_tf("key(%d)=%s\n", i, keyz[i]);
*  igx_profile_ReleaseKeys(keyz);
* @endcode
*/
IGX_PROFILE_EXTERN const char** igx_profile_enum_key_ex(HIGX_PROFILE hProfile, const char* section, bit32_t* cnt);

/**@brief igx_profile_enum_key_exで取得したメモリを削除します。
*
* @param p igx_profile_EnumKeyExの戻り値
*/
IGX_PROFILE_EXTERN void igx_profile_release_enum_keys(const char** p, bit32_t cnt);

/**@brief igx_profile_のハンドルを解放します。
*
* @param h igx_profile_Openの戻り値
*/
IGX_PROFILE_EXTERN void igx_profile_Close(HIGX_PROFILE h);

#endif

