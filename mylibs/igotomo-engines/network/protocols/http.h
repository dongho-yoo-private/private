/**
* Copyright (c) 2015 igotomo
* authorized dongho.yoo.
*
* @brief httpプロトコル
* @startcode
* 
* igx_http_t http;
* igx_http_detail detail;
* addrv4_t addr;
* igx_http_error_t error;
*
* igx_http_defalut_detail(&detail);
* detail.port=5555;
* detail.use_ssl=1;
* detail.read_time_out=5000;
* addr = igx_get_host_name("www.yahoo.co.jp");
* 
* if (igx_http_create(&http, addr, &detail, &error)==0)
* {
*    // TODO. erorr
*    printf("%s\n", igx_http_get_error_msg(error));
*    exit(0);
* }
* igx_http_set_content_type("application/x-www-form-urlencoded");
* igx_http_set_charset("utf-8");
* igx_http_add_header("keepAlive", "3000");
*               :
*               :
* igx_http_error_t error;
* if ((error=igx_http_connect(&http))!=kHttpErrorSuccess) // connectはkeepAliveが設定されている場合のみ
* {
*       printf("%s\n", igx_http_get_error_msg(error));
*       exit(0);
* }
* // connectが呼び出されてない場合は、内部でconnectも行う。
* // connectを行うためにはアドレスにホスト名も記入する。ex) "http://www.yahoo.co.jp:8080/login.cgi?igotmo%20password"
* if ((error=igx_http_post(&http, "/login.cgi?igotomo%20password", data, data_size, timeout)!=kHttpErrorSuccess)
* {
*       printf("%s\n", igx_http_get_error_msg(error));
*       exit(0);
* }
* 
* if (igx_http_get_status()==200)
* {
*   igx_http_response_t* response = igx_http_get_response(&http);
*   printf("%s", response->header);
*   printf("%s", response->body);
*   igx_http_release(response);
* }
*
* // igx_http_connectを呼んだ時のみ
* igx_http_close(&http);
* igx_http_destroy(&http);
*
* @endcode
*/
#ifndef __IGX_HTTP_H__
#define __IGX_HTTP_H__

#include "network/protocols/http_types.h"

#ifdef __cplusplus
#   define IGX_HTTP_EXTERN extern "C"
#else
#   define IGX_HTTP_EXTERN extern
#endif

/** @brief エラーコードを取得します。
*  
* @param code エラーコード
*
* @return エラーコードに関する詳細な文字列のメッセージを返します。
* @retval NULL エラーコードが無効である。
*/
IGX_HTTP_EXTERN const char* igx_http_get_error_msg(igx_http_error_t code);
/*
* @brief igx_httpのデフォルトの値を設定します。\n
* デフォルトは、@ref igx_http_detail_tを参照
*
* @param detail @ref igx_http_detail_t
* @return detailのアドレス
*/
IGX_HTTP_EXTERN igx_http_detail_t* igx_http_default_detail(igx_http_detail_t* detail);
/**
* @brief サーバに接続します。
*/
IGX_HTTP_EXTERN igx_http_error_t igx_http_connect(igx_http_t* http);
/** @brief コネクションを保ったまま使いたい場合は以下のAPI群を使います。\n
* この中ではコネクションまで行います。 
*
* @param http    ハンドル
* @param addr    32ビットのIpV4(must) 
* @param hostname ホスト名(optional)
* @param detail   詳細設定（optional)
* @param error    失敗時のエラー(error)
*
* @return NULLの場合は、errorを参照。
*
* @return 通信状態を介します。
*/
IGX_HTTP_EXTERN igx_http_t* igx_http_create(igx_http_t* http, igx_addrv4_t addr, const char* hostname, igx_http_detail_t* detail, igx_http_error_t* error);
/**
* @brief ホスト名を指定します。
* @param ホスト名
*/
IGX_HTTP_EXTERN void igx_http_set_host_name(igx_http_t* http, const char* host_name);
/**
* @brief コンテントタイプを設定します。設定しない場合は、デフォルトの値が設定されます。
* 
* @param content_type コンテントタイプ
*/
IGX_HTTP_EXTERN void igx_http_set_content_type(igx_http_t* http, const char* content_type);
/**
* @brief キャラクターセットを設定します。設定しない場合は、デフォルトのutf-8になります。
* @param char_set キャラクターセット
*/
IGX_HTTP_EXTERN void igx_http_set_charset(igx_http_t* http, const char* char_set);
/**
* @brief ホスト名を指定します。（必須）
*/
IGX_HTTP_EXTERN void igx_http_set_hostname(igx_http_t* http, const char* hostname);
/**
* @brief ヘッダーに項目を追加します。
* @param key キー
* @param value 値
*/
IGX_HTTP_EXTERN void igx_http_add_header(igx_http_t* http, const char* key, const char* value);
/*
* @brief ヘッダーをそのまま追加します。
* @param ヘッダーの内容
* @param ヘッダーの長さ
*/
IGX_HTTP_EXTERN void igx_http_add_header_direct(igx_http_t* http, const char* header, size_t header_size);
/** @brief POSTリクエスト
*  
* @param [in] http       igx_http_createで生成されたハンドル
* @param [in]path        パス
* @param [in]data        POSTで送るデータ
* @param [in]contenttype ContentType
* @param [in]dataSize    dataのサイズ
*
* @return 通信状態を返します。
*/
IGX_HTTP_EXTERN igx_http_error_t igx_http_post(igx_http_t* http, const char* path, const void* data, size_t data_size);

/** @brief GETリクエスト
*  
* @param [in] http     igx_http_createで生成されたハンドル
* @param [in] path     urlの後ろのパス (フルパスでも可能)
* @param [in] path_len パスの長さ
*
* @return 通信状態を返します。
*/
IGX_HTTP_EXTERN igx_http_error_t igx_http_get(igx_http_t* http, const char* path, size_t path_len);

/** @brief httpリクエスト
*  
* @param [in] http igx_http_createで生成されたハンドル
* @param [in] header httpヘッダー
* @param [in] header_size ヘッダーサイズ
* @param [in] body ボディーサイズ (PUT, POST only)
* @param [in] body_size ボディーサイズ
*
* @return @ref igx_http_error_t
* @retval kHttpErrorSuccessの場合は成功。ただし、httpのステータスが200であることを意味している訳ではない。
*/
IGX_HTTP_EXTERN igx_http_error_t igx_http_message(igx_http_t* http, const char* header, size_t header_size, const void* data, size_t data_size);
/** @brief GET/POST後、レスポンスを取得します。
*  
* @param http        igx_http_createで生成されたハンドル
*
* @return レスポンスの結果
* @retval kHE_SUCCESS 成功
* @retval 詳細はigx_http_error_t参照
*/
IGX_HTTP_EXTERN const igx_http_response_t* igx_http_get_response(igx_http_t* http);
/** 
* @brief レスポンスやヘッダーのデータなどをクリアーします。
* @param http igx_http_createで生成されたハンドル
*/
IGX_HTTP_EXTERN void igx_http_clear(igx_http_t* http);
/** @brief コネクションを切り、リソースを解放します。
*
* @param http igx_http_createで生成されたハンドル
*
*/
IGX_HTTP_EXTERN void igx_http_close(igx_http_t* http);
/** @brief 最後に発生したエラーを取得します。
*
* @param http igx_http_createで生成されたハンドル
* 
* @return エラーコード
* @retval 
*/
IGX_HTTP_EXTERN igx_http_error_t igx_http_get_last_error(igx_http_t* http);
/** 
* @brief Httpのコネクションが有効であるか無効であるかを取得します。
*
* @param http igx_http_createで生成されたハンドル
* @return 接続がされているかどうか
* @retval  true 接続中
* @retval  flase 接続切れ
*/
IGX_HTTP_EXTERN bool_t igx_http_is_connected(igx_http_t* hHttp);
/**
* @brief urlをパースします。
* @param [in] url あなぽんするので、ReadOnly領域にはしないように。
* @param [out]protocol_is_ssl プロトコルがhttpsかどうか
* @param [out]host_name ホスト名
* @param [out]path パス（/がないので、使う際には/を付けること!）
*
* @return falseの場合は不正なurl
*
* @startcode
* const char* url = "https://www.yahoo.co.jp:445/login.cgi?igotomo%20paswword";
* char buffer[512], *host_name, *path;
* bool_t use_ssl;
* bit32_t port;
* strcpy(buffer, url);
* 
* igx_http_url_parse(buffer, &use_ssl, &host_name, &port, &path);
*
* @endcode
*/
IGX_HTTP_EXTERN bool_t igx_http_url_parse(char* url, bool_t* protocol_is_ssl, bit32_t* port, char** host_name, char** path);

#endif

