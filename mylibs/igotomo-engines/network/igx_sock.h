/**
 * Copyright (c) 2015 igotomo
 * Authorized by dongho.yoo.
 *
 * @brief ソケット+SSLライブラリ
*/

#ifndef __IGX_SOCK_H__
#define __IGX_SOCK_H__

#include "igx_types.h"
#include "igx_sock_types.h"

#ifdef __cplusplus
#define IGX_SOCK_EXTERN extern "C"
#else
#define IGX_SOCK_EXTERN extern 
#endif


/** 
* @brief 文字列のIPアドレスを32ビットのアドレスに変換します。
*
* @param ip 文字列のipアドレス
* @return 32ビットのアドレス
*/
IGX_SOCK_EXTERN igx_addrv4_t igx_sock_ip_2_addr(const char* ip);
/** 
* @brief 32ビットのアドレスを文字列のアドレスに変換します。
* @param addr 32ビットのアドレス
* @param ip 文字列のipアドレスを受け取るバッファー
* @return バッファーのポインター
*/
IGX_SOCK_EXTERN char* igx_sock_addr_2_ip(igx_addrv4_t addr, char* ip);
/** 
* @brief ホスト名を32ビットのアドレスに変換します。\n
* 内部的にはキャッシュのアドレスを検索し、なければDNSに問い合わせます。
* @param hostname ホスト名
*
* @return 32ビットのアドレス
*/
IGX_SOCK_EXTERN igx_addrv4_t igx_sock_get_host_addr(const char* hostname);
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
IGX_SOCK_EXTERN igx_sock_t* igx_sock_connect(igx_sock_t* socket, igx_addrv4_t addr, bit32_t port, 
                                    bit32_t time_out_sec, bool_t use_ssl, igx_sock_error_t* err);
/**
* @brief ネゴエーションを開始します。\n
* 開始時はsslでなく、途中からSSLになるシチュエーションで使われます。\n
* 既にsslだった場合はエラーが返ります。
*
* @param sock ハンドル
* @retval kSockErrorSuccess 成功
* @retval kSockErrorUnknown 失敗
*/
IGX_SOCK_EXTERN igx_sock_error_t igx_sock_negotiation(igx_sock_t* sock);
/** 
* @brief ソケットを終了します。
*/
IGX_SOCK_EXTERN void igx_sock_close(igx_sock_t* socket);
/** 
* @brief ソケットからデータを読み込みます。
*
* @param socket ハンドル
* @param [out] buffer データを受け取るバッファー
* @param size   バッファーサイズ
* @param  timeout タイムアウト（秒）
* @param [out] progress 進行状況0.0〜1.0（0なら無視）
*/
IGX_SOCK_EXTERN igx_sock_error_t igx_sock_read(igx_sock_t* socket, char* buffer, bit32_t size, bit32_t timeout, float* progress);
/** 
* @brief ソケットから一度のみデータを読み込みます。\n
* igx_sock_readの違いは、igx_sock_readは、指定したバイト数分を読み込むまで、\n
* タイムアウトにならない限り読み込もうとしますが、この関数は一度のみ、読み込みます。
*
* @param socket ハンドル
* @param [out] buffer 受け取るバッファー
* @param size   バッファーサイズ
* @param [out] received 雨蹴っとったサイズ
* @param timeout タイムアウト
*
* @return \ref igx_sock_error_t
*
* @param 
*/
IGX_SOCK_EXTERN igx_sock_error_t igx_sock_read_once(igx_sock_t* socket, char*buffer, size_t size, size_t* received, bit32_t timeout);
/** 
* @brief ソケットに書き込みます。
*
* @param socket ハンドル
* @param [out] buffer バッファー
* @param size  バッファーサイズ
* @param [out] progress 進行度
*
* @return \ref igx_sock_error_t
*/
IGX_SOCK_EXTERN igx_sock_error_t igx_sock_write(igx_sock_t* socket, const char*buffer, size_t size, float* progress);
/** 
* @brief 接続状態を確認します。
*
* @param socket ハンドル
* @return true接続中
*/
IGX_SOCK_EXTERN bool_t igx_sock_is_connected(igx_sock_t* socket);

/** 
* @brief ソケットを待ちます。
*
* @param socket ハンドル
* @param timeout タイムアウト
*
* @retval true 成功、falseタイムアウト
*/
IGX_SOCK_EXTERN bool_t igx_sock_wait(igx_sock_t* socket, bit32_t timeout);
/** \brief urlをパースします。
* 
* \param [in] url (ex "http://www.yahoo.co.jp:80/abcde/kkkkk?id=xxxx, pass=????")
* \param [out] protocol プロトコル(ex http, ftp, svn ...)
* \param [out] hostname ホスト名
* \param [out] port	ポーと番号(ない場合はゼロ)
*
* \retval ホスト名の後の文字列のアドレスを返します。
*/
IGX_SOCK_EXTERN const char* igx_sock_url_parse(const char* url, char* protocol, char* hostname, bit32_t* port);

#endif
