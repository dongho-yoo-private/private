/**
 * \author dongho yoo
 * \file lib_profile.h
 * \~english
 * \~japanese
 *   \brief Linked List Library.
 */
#ifndef LIB_PROFILE_H__
#define LIB_PROFILE_H__

#include <core/types.h>

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern 
#endif

typedef void* Handle;

typedef Handle HandleProfile;

#if 0
#ifndef True
typedef S32 Bool;
#define False 0
#define True 1
#endif
#endif

typedef struct {
  Bool bIsDeleteAtTime;
  S32 nElementCount;
  S32 nExtraIdSize;
  S32 nTotalSizeLimitted;
} sLIB_ProfileDetailForBinaryMode;

/**@brief profileファイルを開きます。
*
*@param pszFileName Profileファイル名
*
*@return 成功した場合は0でないハンドルが返ります。
*/
EXTERN_C HandleProfile LIB_ProfileOpen(const char* pszFileName);//, sLIB_ProfileDetailForBinaryMode* detail);//Bool bIsBinaryMode, S32 nMaxElementSize, S32 nIdSize);

/**@brief profileファイルからkeyと値を文字列で取得します。
*
*@param [in] hProfile LIB_ProfileOpenで取得したハンドル
*@param [in] section  文字列のセクションで最大64バイトまで 
*@param [in] key   文字列のキー
*@param [out] value    文字列の値
*@return 成功した場合はtrueが返ります。
*@retval true 成功
*@retval false 失敗
*@code
* // sample.
* char version[32];
* HandleProfile hProfile = LIB_ProfileOpen("StartupInfo.inf");
* if (LIB_ProfileReadStringEx(hProfile, "System", "Version", version)==false)
* {
*   // エラー処理
* }
* LIB_ProfileClose(hProfile);
*@endcode
*/
EXTERN_C Bool LIB_ProfileReadStringEx(HandleProfile hProfile, const char* section, const char* key, char* value);

/**@brief profileファイルからkeyと値を整数で取得します。
*
*@param [in] hProfile LIB_ProfileOpenで取得したハンドル
*@param [in] section  文字列のセクションで最大64バイトまで 
*@param [in] key   文字列のキー
*@param [out] value    整数のポインター
*
*@return 成功した場合はtrueが返ります。
*@retval true 成功
*@retval false 失敗
*@code
* // sample.
* S32 nConnectTimeout;
* HandleProfile hProfile = LIB_ProfileOpen("StartupInfo.inf");
* if (LIB_ProfileReadIntEx(hProfile, "Network", "ConnectTimeout", &nConnectTimeout)==false)
* {
*   // エラー処理
* }
* LIB_ProfileClose(hProfile);
*@endcode
*/
EXTERN_C Bool LIB_ProfileReadIntEx(HandleProfile hProfile, const char* section, const char* key, S32* val);

/**@brief profileファイルにsection, key, valueを設定します。\n
* sectionやkeyがない場合は新たに生成されます。
*
*@param [in] hProfile LIB_ProfileOpenで取得したハンドル
*@param [in] section  文字列のセクションで最大64バイトまで 
*@param [in] key   文字列のキー
*@param [in] value    整数値
*
*@return 成功した場合はtrueが返ります。
*@retval true 成功
*@retval false 失敗
*@code
* // sample.
* S32 nConnectTimeout;
* HandleProfile hProfile = LIB_ProfileOpen("StartupInfo.inf");
* if (LIB_ProfileWriteIntEx(hProfile, "Network", "ConnectTimeout", 30000)==false) 
* {
*   // エラー処理
* }
* LIB_ProfileClose(hProfile);
*@endcode
*/
EXTERN_C Bool LIB_ProfileWriteIntEx(HandleProfile hProfile, const char* section, const char* key, S32 nDefaultNum);


/**@brief profileファイルにsection, key, valueを設定します。\n
* sectionやkeyがない場合は新たに生成されます。
*
*@param [in] hProfile LIB_ProfileOpenで取得したハンドル
*@param [in] section  文字列のセクションで最大64バイトまで 
*@param [in] key   文字列のキー
*@param [in] value    文字列
*
*@return 成功した場合はtrueが返ります。
*@retval true 成功
*@retval false 失敗
*@code
* // sample.
* S32 nConnectTimeout;
* HandleProfile hProfile = LIB_ProfileOpen("StartupInfo.inf");
* if (LIB_ProfileWriteStringEx(hProfile, "Debug", "Debug", "ON")==false) 
* {
*   // エラー処理
* }
* LIB_ProfileClose(hProfile);
*@endcode
*/
EXTERN_C Bool LIB_ProfileWriteStringEx(HandleProfile hProfile, const char* section, const char* key, const char* value);

/**@brief 指定したsectionのキーを列挙します。
*
*@param [in] hProfile LIB_ProfileOpenで取得したハンドル
*@param [in] section  文字列のセクションで最大64バイトまで 
*@param [out] num     リストの数
*
*@return 成功した場合は0以外のポインターが返ります。
*@retval 0 失敗(セクションが存在しないか、keyがない)
*@code
* // sample.
* S32 nConnectTimeout;
* char** keyz;
* HandleProfile hProfile = LIB_ProfileOpen("StartupInfo.inf");
* keyz = LIB_ProfileEnumKeysEx(hProfile, "HostList", &cnt);
* if (keyz==0)
* {
*   // エラー処理
* }
* LIB_ProfileClose(hProfile);
* 
* for (i=0; i<cnt; i++)
*  prS32f("key(%d)=%s\n", i, keyz[i]);
* LIB_ProfileReleaseKeys(keyz);
*@endcode
*/
EXTERN_C const char** LIB_ProfileEnumKeysEx(HandleProfile hProfile, const char* section, S32* cnt);

/**@brief LIB_ProfileEnumKeyExで取得したメモリを削除します。
*
* @param p LIB_ProfileEnumKeyExの戻り値
*/
EXTERN_C void LIB_ProfileReleaseEnumKeys(const char** p, S32 cnt);

/**@brief LIB_Profileのハンドルを解放します。
*
* @param h LIB_ProfileOpenの戻り値
*/
EXTERN_C void LIB_ProfileClose(HandleProfile h);

// 以下の関数はセクションを作らないので。。。（削除予定)

// ------------------------------------------
EXTERN_C sLIB_ProfileDetailForBinaryMode* LIB_ProfileSetDefaultParam(sLIB_ProfileDetailForBinaryMode* mode);


// ------------------------------------------
EXTERN_C Bool LIB_ProfileReadString(HandleProfile hProfile, const char* key, char* value);

// ------------------------------------------
EXTERN_C S32 LIB_ProfileReadInt(HandleProfile hProfile, const char* key, S32 nDefaultNum);

// ------------------------------------------
EXTERN_C Bool LIB_ProfileWriteInt(HandleProfile hProfile, const char* key, S32 nDefaultNum);

// ------------------------------------------
EXTERN_C Bool LIB_ProfileWriteString(HandleProfile hProfile, const char* key, const char* value);

// ------------------------------------------
EXTERN_C Bool LIB_ProfileWriteStringEx(HandleProfile hProfile, const char* section, const char* key, const char* value);


// ------------------------------------------
EXTERN_C const char** LIB_ProfileGetEnumKeys(HandleProfile hProfile, S32* cnt);


// ------------------------------------------
EXTERN_C const void* LIB_ProfileReadBinary(HandleProfile hProfile, S32 index, S32* size);

// ------------------------------------------
EXTERN_C const void* LIB_ProfileReadBinaryById(HandleProfile hProfile, S32 id, void* exid, S32* size);

// ------------------------------------------
EXTERN_C Bool LIB_ProfileWriteBinary(HandleProfile hProfile, S32 index, void* data, S32 size, void* id);

// ------------------------------------------
EXTERN_C Bool LIB_ProfileWriteBinaryById(HandleProfile hProfile, S32 id, void* exid, void* data, S32 size);

// ------------------------------------------
EXTERN_C void LIB_ProfileGetDetail(HandleProfile hProfile, sLIB_ProfileDetailForBinaryMode* mode);

// ------------------------------------------
EXTERN_C void LIB_ProfileUpdateDetail(HandleProfile hProfile, sLIB_ProfileDetailForBinaryMode* mode);



// ------------------------------------------
EXTERN_C Bool yiProfileWriteString(const char* pszTag, const char* value, const char* pszFileName);

// ------------------------------------------
EXTERN_C Bool yiProfileWriteInt(const char* pszTag, S32 value, const char* pszFileName);

// ------------------------------------------
EXTERN_C Bool yiProfileReadString(const char* pszTag, char* value, const char* pszFileName);

// ------------------------------------------
EXTERN_C S32 yiProfileReadInt(const char* pszTag, S32 defaultValue, const char* pszFileName);


#endif

