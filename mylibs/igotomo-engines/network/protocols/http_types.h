/**
* Copyright (c) 2015 igotomo
* authorized dongho.yoo.
*/
#ifndef _HTTP_TYPES_H_
#define _HTTP_TYPES_H_
#include "igx_types.h"
#include "network/igx_sock_types.h"

#ifndef kHTTP_MAX_HOST_NAME_LEN
# define kHTTP_MAX_HOST_NAME_LEN (1023)
#endif

#ifndef kHTTP_MAX_HEADER_LEN
# define kHTTP_MAX_HEADER_LEN (1023)
#endif

#define HTTP_USE_LINKED_LIST

/** \brief 内部でLinked Listを使わない場合*/
#ifndef HTTP_USE_LINKED_LIST
/** \brief 最大のチャンクデータの数、無限にしたい場合はHTTP_USE_LINKED_LISTをdefineします。*/
#   define kHTTP_MAX_CHUNK_COUNT (128)
#endif

#define kHTTP_MAX_CONTENT_TYPE_LEN (64)

#define kIGX_TTP_CONTENT_TYPE_URL_ENCODED ("application/x-www-form-urlencoded")
#define kIGX_TTP_CONTENT_TYPE_TEXT_HTML ("text/html")
#define kIGX_TTP_CONTENT_TYPE_PLAN_TEXT ("plan/text")
#define kIGX_TTP_CONTENT_TYPE_TEXT_JSON ("text/json")

#define kIGX_HTTP_DEFAULT_CONTENT_TYPE kIGX_TTP_CONTENT_TYPE_TEXT_HTML 

#define kIGX_HTTP_CHAR_SET_SJIS ("shift-jis")
#define kIGX_HTTP_CHAR_SET_EUC  ("euc")
#define kIGX_HTTP_CHAR_SET_EUC_JP  ("euc-jp")
#define kIGX_HTTP_CHAR_SET_UTF8 ("utf-8")

#define kIGX_HTTP_DEFAULT_CHAR_SET kIGX_HTTP_CHAR_SET_UTF8

#define kIGX_HTTP_DEFAULT_PORT (80)
#define kIGX_HTTPS_DEFAULT_PORT (443)
#define xHTTP_STR_PROTOCOL_HTTP "http"
#define xHTTP_STR_PROTOCOL_HTTP_LEN (4)
#define xHTTP_STR_PROTOCOL_HTTPS "https"
#define xHTTP_STR_PROTOCOL_HTTPS_LEN (5)

#define xHTTP_DEFAULT_CONNECT_TIMEOUT (10*1000)
#define xHTTP_DEFAULT_READ_TIMEOUT (30*1000)

#define _HEADER_READ_ONEBYTE_

/** 
* @brief igx http エラー
*/
typedef enum {
    /**@brief 成功 */
    kHttpErrorSuccess,
    /**@brief コネクトエラー*/
    kHttpErrorConnect,
    /**@brief 接続タイムアウト*/
    kHttpErrorConnectTimeout,
    /**@brief リクエストエラー */
    kHttpErrorRequest,
    /**@brief レスポンスエラー */
    kHttpErrorResponse,
    /**@brief レスポンスタイムアウト */
    kHttpErrorResponseTimeout,
    /**@brief リダイレクトエラー*/
    kHttpErrorRedirect,
    /**@brief 不明なエラー */
    kHttpErrorUnknown
} igx_http_error_t;

/**@brief httpの状態 */
typedef enum {
    /**@brief アイドル */
    kHttpStatusIdle,
    /**@brief 接続中 */
    kHttpStatusConnect,
    /**@brief リクエスト中 */
    kHttpStatusRequest,
    /**@brief レスポンス待ち状態*/
    kHttpStatusResponseWait,
    /**@brief レスポンス */
    kHttpStatusResponse
} igx_http_status_t;

/** @brief インジゲータの構造体*/
typedef struct 
{
    /**@brief 進行状態 */
    igx_http_status_t status;
    /** @brief 進行率 0.0-1.0f*/
    float fprogress;    
} igx_http_progress_t;
/* 
*@brief 細かい設定
*/
typedef struct {
    /**@brief ポーと番号 default:80*/
    bit16_t port;             
    /**@brief コネクト時のタイムアウト(msec) default:10000*/
    bit16_t connect_timeout;
    /**@brief Read時のタイムアウト(msec) default:10000*/
    bit16_t read_timeout;;
    /**@brief コネクションを保つ際のタイムアウト値(sec) default:0*/
    bit16_t keep_alive_timeout;
    /**@brief インジゲータ*/
    igx_http_progress_t* progress;    
    struct {
        /**@brief SSL通信をする場合は1層でない場合は0 default:0*/
        unsigned use_ssl:1;
        /**@brief itx_mem_pageを使うかどうか */
        unsigned use_mem_page:1;
        /**@brief チャンクデータをもっているかどうか */
        unsigned has_chunk:1;
        /**@brief リダイレクトされた！*/
        unsigned redirect:1;
        /**@brief レザーブ*/
        unsigned reserved:5;
    };
} igx_http_detail_t;

/**
* @brief httpレスポンス
*/
typedef struct {
    /** @brief ステータス*/
    bit32_t status;
    /**@brief コンテントタイプ */
    char* content_type;
    /**@brief redirect時に使います。 */
    char* location;
    /** @brief データのサイズ */
    size_t  size;
    /** @brief データ */
    char*   data;
} igx_http_response_t;

/** @brief igx_http ハンドル*/
typedef struct 
{
    ubit8_t buffer[256];
} igx_http_t;




#endif
