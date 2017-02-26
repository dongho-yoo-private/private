/**
 * \file lib_device.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief デバイス情報に関する定義部です
 */


#ifndef LIB_DEVICE_H_
#define LIB_DEVICE_H_


#include <core/types.h>


ADK_BEGIN_NAMESPACE


/* extern function definition */
/**
 * \~english
 * \~japanese
 *   \brief デバイス名を返します
 *   \return デバイス名
 */
extern const Char *DEV_GetDeviceName();
/**
 * \~english
 * \~japanese
 *   \brief デバイスの OS 名を返します
 *   \return OS 名
 */
extern const Char *DEV_GetOSName();
/**
 * \~english
 * \~japanese
 *   \brief OS のバージョンを返します
 *   \return OS のバージョン
 */
extern const Char *DEV_GetOSVersion();
/**
 * \~english
 * \~japanese
 *   \brief デバイスに設定されている言語を返します
 *   \return 言語
 */
extern const Char *DEV_GetLanguage();
/**
 * \~english
 * \~japanese
 *   \brief デバイスに設定されている地域コードを返します
 *   \return 地域コード
 */
extern const Char *DEV_GetCountryCode();
/**
 * \~english
 * \~japanese
 *   \brief GMTとの時差を返します
 *   \return 時差
 */
extern const Char *DEV_GetDifferenceGMT();
/**
 * \~english
 * \~japanese
 *   \brief UUIDを取得します。
 *   \return UUID
 */
extern const Char *DEV_GetUuId();
/**
 * \~english
 * \~japanese
 *   \brief IDFVを取得します。
 *   \return IDFV
 */
extern const Char *DEV_GetIdentifierForVender();
/**
 * \~english
 * \~japanese
 *   \brief IDFAを取得します。
 *   \return IDFA (Androidは空の文字列を返します。)
 */
extern const Char* DEV_GetIdentifierForAdvertiser();
/**
 * \~english
 * \~japanese
 *   \brief AndroidIdを取得します。
 *   \return AndroidId (iosは空の文字列を返します。)
 */
extern const Char* DEV_GetAndroidId();
/**
 * \~english
 * \~japanese
 *   \brief AdvertisingIdを取得します。
 *   \return AdvertisingId (iosは空の文字列を返します。)
 */
extern const Char* DEV_GetAdvertisingId();
/**
 * \~english
 * \~japanese
 *   \brief UDIDを取得します。
 *   \return UDID
 */
extern const Char *DEV_GetOpenUdId();
/**
 * \~english
 * \~japanese
 *   \brief UDIDを取得します。
 *   \return UDID
 */
extern const Char *DEV_GetSecureUdId();
/**
 * \~english
 * \~japanese
 *   \brief DeviceTokenを取得します。
 *   \return DeviceToken
 */
extern const Char *DEV_GetDeviceToken();
/**
 * \~english
 * \~japanese
 *   \brief DeviceTokenを設定します。
 */
extern void DEV_SetDeviceToken(const unsigned char* data, unsigned int size);
/**
 * \~english
 * \~japanese
 *   \brief MacAddressを取得します。
 *   \return MacAddress
 */
extern const Char *DEV_GetMacAddress();
/**
 * \~english
 * \~japanese
 *   \brief EmailAddressを取得します。
 *   \return EmailAddress
 */
extern const Char *DEV_GetEmailAddress();

/**
 * \~english
 * \~japanese
 *   \brief HDDの最大容量を取得します。
 *   \return HDDの最大容量
 */
extern U64 DEV_GetFileSystemSize();
/**
 * \~english
 * \~japanese
 *   \brief HDDの空き容量を取得します。
 *   \return HDDの空き容量
 */
extern U64 DEV_GetFileSystemFreeSize();

/**
 * \~english
 * \~japanese
 *   \brief OSメモリの空き容量を取得します
 *   \return OSメモリの空き容量 (bytes)
 */
extern U64 DEV_GetSystemFreeMemory();

/**
 * \~english
 * \~japanese
 *   \brief 端末へのアプリのインストール回数を取得します
 *   \return 端末へのアプリのインストール回数
 */
extern U32 DEV_GetInstallationCount();



#ifdef WITH_ANDROID

/**
 * \~english
 * \~japanese
 *   \brief UDIDを設定します。
 *   \return UDID
 */
extern void DEV_SetUuId();

#endif


ADK_END_NAMESPACE


#endif  /* LIB_DEVICE_H_ */


