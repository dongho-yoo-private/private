/**
 * Copyright (c) 2015 igotomo
 * Authorized by dongho.yoo.
 * @brief ソケット+SSLライブラリ
*/

#ifndef __IGX_SOCK_H__
#define __IGX_SOCK_H__

#include "igx_types.h"
#include "igx_sock_types.h"

#ifdef __cplusplus
#define IGX_EXTERN extern "C"
#else
#define IGX_EXTERN extern 
#endif

// define macros for environment.
#define _USE_OPEN_SSL_
#if defined(WITH_ANDROID)
# define _USE_SELECT_ 1
#elif defined(WITH_IOS)
# define _USE_KQUEUE_ 1
#endif

#ifndef __POSIX__
#define __POSIX__
#endif

/** @brief ソケットハンドル */
typedef struct {
    char buffer[sizeof(sockssl_t)];
} igx_sock_t;

typedef enum {
    /**@brief 成功 */
    kSockSuccess=0,
    /**@brief タイムアウト */
    kSockTimeout,
    /**@brief writeエラー */
    kSockWriteError,
    /**@brief readエラー */
    kSockReadError,
    /**@brief SSL関連エラー */
    kSockSSL,
    /**@brief 不明なエラー */
    kSockUnknown
} igx_sock_error_t;


/** 
* @brief 文字列のIPアドレスを32ビットのアドレスに変換します。
*
* @param ip 文字列のipアドレス
* @return 32ビットのアドレス
*/
NPC_EXTERN addrv4_t igx_sock_ip_2_addr(const char* ip);
/** 
* @brief 32ビットのアドレスを文字列のアドレスに変換します。
* @param addr 32ビットのアドレス
* @param ip 文字列のipアドレスを受け取るバッファー
* @return バッファーのポインター
*/
NPC_EXTERN Char* igx_sock_adr_2_ip(addrv4_t addr, Char* ip);
/** 
* @brief ホスト名を32ビットのアドレスに変換します。\n
* 内部的にはDNSに問い合わせます。
* @param hostname ホスト名
* @param index インデックス（複数アドレスがある場合)、-1でランダムで選びます。
*
* @return 32ビットのアドレス
*/
NPC_EXTERN addrv4_t igx_sock_get_host_addr(const Char* hostname, bit32_t index);
/** 
* @brief TCPコネクション開始
*
* @param [out] socket ハンドルのバッファー
* @param addr   接続先のアドレス
* @param port   接続先のポート
* @param time_out_sec タイムアウト（秒数）、-1でシステムタイムアウト
* @param use_ssl SSLを使用する場合は、true
* @param err    エラーコード（0で無視）
*
* @return 成功した場合は、指定したバッファーのアドレスが返ります。
* @retval 0 失敗
*/
NPC_EXTERN igx_sock_t* igx_sock_connect(igx_sock_t* socket, addrv4_t addr, bit32_t port, 
                                    bit32_t time_out_sec, bool_t use_ssl, igx_sock_error_t* err);
/** 
* @brief ソケットを終了します。
*/
NPC_EXTERN void igx_sock_close(igx_sock_t* socket);
/** 
* @brief ストリームデータを読み込みます。
*
* @param socket ハンドル
* @param [out] buffer データを受け取るバッファー
* @param size   バッファーサイズ
* @param  timeout タイムアウト（秒）
* @param [out] progress 進行状況0.0〜1.0（0なら無視）
*/
NPC_EXTERN igx_sock_error_t igx_sock_read(igx_sock_t* socket, char* buffer, bit32_t size, bit32_t timeout, float* progress);

/** @brief
*/
NPC_EXTERN TNPError igx_sock_read_once(igx_sock_t* hSock, Char*buffer, bit32_t size, bit32_t * nRecved, bit32_t timeout);

/** @brief
*/
NPC_EXTERN TNPError igx_sock_Write(igx_sock_t* hSock, const Char*buffer, bit32_t size, float* progress);

/** @brief
*/
NPC_EXTERN bool_t igx_sock_IsConnected(igx_sock_t* hSock);

/** @brief
*/
NPC_EXTERN bool_t igx_sock_Wait(igx_sock_t* hSock, bit32_t timeout);

/** \brief urlをパースします。
* 
* \param [in] url (ex "http://www.yahoo.co.jp:80/abcde/kkkkk?id=xxxx, pass=????")
* \param [out] protocol プロトコル(ex http, ftp, svn ...)
* \param [out] hostname ホスト名
* \param [out] port	ポーと番号(ない場合はゼロ)
*
* \retval ホスト名の後の文字列のアドレスを返します。
*/
NPC_EXTERN const Char* igx_sock_UrlParse(const Char* url, Char* protocol, Char* hostname, bit32_t* port);

ADK_END_NAMESPACE

#endif
