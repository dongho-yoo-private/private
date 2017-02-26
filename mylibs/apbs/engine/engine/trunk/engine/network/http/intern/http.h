#ifndef __https_h__
#define __https_h__

/**
 * \author dongho.yoo
 * \file Http.h
 * \ingroup grp_api
 * \~english
 * 	\brief https library.
 * \~japanese
 * 	\brief https通信ライブラリ
 * 	\page page_https 使用方法
 * 		\tableofcontents
 * 		\section sct_instructions 使用手順
 * 			\subsection subsct_sample サンプル1
 * 					-# サンプル1
 * 						\code
 * 						#include <Http.h>
 *
 * 						int main(int argc, char* argv[])
 * 						{
 *							THttpDetail detail;
 *							THttpError  err;
 * 						    HandleHttp hHttps = HttpCreate("https://www.any.com:777", HttpSetDefaultDetail(&detail), &err);
 * 							
 * 						    if (hHttps==NULL)	
 * 							{	
 * 								if (err==kHE_CONNECT)
 *								{
 *									// TODO.
 *								}
 *								exit(0);
 *							}
 *							// この時点でSSL Negotiationが確立しています。
 *									:
 *									:
 * 						    if (HttpPost(hHttps, "/anypath/animethod", data, dataSize)!=kHE_SUCCESS)
 *							{
 *								HttpClose(hHttps);	
 *								exit(0);
 *							}
 * 								
 * 							const THttpResponse* res = HttpGetResponse(hHttps);
 *							void* data = res->data;
 *							int   size = res->size;
 *											:
 *											:
 *							
 * 								
 * 						    if (HttpGet(hHttps, "/anypath/animethod?anykey=anyValue", data, dataSize)!=kHE_SUCCESS)
 *							{
 *								HttpClose(hHttps);	
 *								exit(0);
 *							}
 *
 * 							res = HttpGetResponse(hHttps);
 *							data = res->data;
 *							size = res->size;
 *											:
 *											:
 *							HttpClose(hHttps);
 * 								
 * 						};
 * 						\endcode
 * 					-# サンプル2 (上記の処理はこのようにもできます)
 * 						\code
 * 						#include <Http.h>
 *
 * 						void main()
 * 						{
 *							THttpResponse res;
 *							THttpDetail detail;
 *							THttpProgress progress;
 *							
 *							HttpSetDefaultDetail(&detail);
 *							detail.progress = &progress; // インジゲータ設定
 *
 *							if (HttpPostOnce("https://www.any.com:777/anypath/animethod", data, dataSize, &res, &detail)!=kHE_SUCCESS)
 *							{
 *								exit(0);
 *							}
 *							
 *							void* data  = res.data;
 *							int   size  = res.size;
 *										:
 *										:
 *							HttpDeleteResponse(&res); // 忘れないように。。。
 * 						}
 * 						\endcode
 */

#include <core/types.h>
#include <network/http/intern/np_cmn.h>

#ifdef __cplusplus
#define SSL_EXTERN extern "C"
#else
#define SSL_EXTERN extern
#endif

/** \brief 使う側がCである場合は以下をコメントアウトしてください。
*   このこのマクロを使うには、Httpの拡張子を.cppにするか、コンパイル時のオプションに-Tpオプションをつけてコンパイルします。\n
*   使わない場合はHttpの拡張子を.cに変更するかコンパイル時のオプションに-Tcをつけます。
*/
#define HTTP_USING_C_PLUS_PLUS
#include <network/http/adk_http_response.h>
#include <network/http/adk_network_types.h>

#ifdef __cplusplus
ADK_BEGIN_NAMESPACE
#endif

#ifndef HTTP_MAX_HOST_NAME_LEN
# define HTTP_MAX_HOST_NAME_LEN (1023)
#endif

#ifndef HTTP_MAX_HEADER_LEN
# define HTTP_MAX_HEADER_LEN (1023)
#endif

#define xHEADER_READ_ONEBYTE

/** 最大のホスト名サイズ*/
#define HTTP_MAX_HOST_NAME_LEN (255)

/** \brief 内部でLinked Listを使わない場合*/
#ifndef HTTP_USE_LINKED_LIST
	/** \brief 最大のチャンクデータの数、無限にしたい場合はHTTP_USE_LINKED_LISTをdefineします。*/
	#define HTTP_MAX_CHUNK_COUNT (128)
#endif

#define HTTP_MAX_CONTENT_TYPE_LEN (64)

#define HTTP_CONTENT_TYPE_URL_ENCODED ("application/x-www-form-urlencoded")
#define HTTP_CONTENT_TYPE_TEXT_HTML ("text/html")
#define HTTP_CONTENT_TYPE_PLAN_TEXT ("plan/text")

#define HTTP_DEFAULT_CONTENT_TYPE HTTP_CONTENT_TYPE_URL_ENCODED

#define HTTP_CHAR_SET_SJIS ("shift-jis")
#define HTTP_CHAR_SET_EUC  ("euc")
#define HTTP_CHAR_SET_EUC_JP  ("euc-jp")
#define HTTP_CHAR_SET_UTF8 ("utf-8")

#define HTTP_DEFAULT_CHAR_SET HTTP_CHAR_SET_UTF8

#define HTTPS_DEFAULT_PORT (80)
#define HTTP_DEFAULT_PORT (443)
#define HTTP_STR_PROTOCOL_HTTP "http"
#define HTTP_STR_PROTOCOL_HTTP_LEN (4)
#define HTTP_STR_PROTOCOL_HTTPS "https"
#define HTTP_STR_PROTOCOL_HTTPS_LEN (5)

#define HTTP_DEFAULT_CONNECT_TIMEOUT (10*1000)
#define HTTP_DEFAULT_READ_TIMEOUT (30*1000)



/** @brief 通信状態の戻り値
*/
typedef TNPError THttpError;

/** \brief httpのハンドル -> ADK_HANDLE?に置き換える*/
typedef void* HandleHttp;

/** @brief インジゲータの構造体*/
typedef struct {
	/** @brief status \n
	* ex) "connecting...", "connected.", "sending...", "reading..." and so on.
	*/
	const char* msg;
	/** @brief 進行率(%)
	*/
	float fprocess;	
} THttpProgress;

/* @brief 細かい設定
 */
typedef struct {
	/**@brief ポーと番号 default:443*/
	int 	port; 			
	/**@brief コネクト時のタイムアウト(秒単位) default:10*/
	int 	connectTimeout;
	/**@brief Read時のタイムアウト(秒単位) default:10*/
	int 	readTimeout;
	/**@brief コネクションを保つ際のタイムアウト値(秒単位) default:0*/
	int 	keepAliveTimeout;
	/**@brief SSL通信をする場合は1層でない場合は0 default:1*/
	int	useSSL;
	/**@brief 受信は別のところでしたい場合、設定します。default:0*/
	int isReceiveAfter;	
	/**@brief 文字コード default:"utf-8"*/
	const char*	charset;

	/**@brief 文字コード default:"utf-8"*/
	const char*	contentType;

	/**@brief 通人状態のインジゲータ*/
	THttpProgress* progress;	

	/**@brief 自動メモリ削除 デフォルトはfalse*/
	Bool 		isAutoFree;
} THttpDetail;


// こうした方がよかったのか。。
//typedef struct {
//	int status;
//	int size;
//	char data[0];
//} THttpResponse;

/** @brief エラーコードを取得します。
*  
* @param code エラーコード
*
* @return エラーコードに関する詳細な文字列のメッセージを返します。
* @retval NULL エラーコードが無効である。
*/
SSL_EXTERN const Char* HttpGetErrorMsg(THttpError code);

/** @brief コネクションを保ったまま使いたい場合は以下のAPI群を使います。\n
* この中ではコネクションまで行います。 
*
* @param addr		32ビットのIpV4(optional) この値がゼロの場合は、ホスト名でIPアドレスを取得します。
* @param hostname 	ホスト名
* @param detail 	詳細設定（optional)
* @param error		失敗時のエラー(error)
*
* @return NULLの場合は、errorを参照。
*
* @return 通信状態を介します。
*/
SSL_EXTERN HandleHttp HttpCreate(address_t addr, const char* hostname, THttpDetail* detail, THttpError* error);

/** \brief ハンドルサイズを取得します。
*
*/
SSL_EXTERN S32 HttpGetHandleSize();

/** @brief コネクションを保ったまま使いたい場合は以下のAPI群を使います。\n
* この中ではコネクションまで行います。 
*
* @param buffer		ハンドルを割り当てるバッファー
* @param addr		32ビットのIpV4(optional) この値がゼロの場合は、ホスト名でIPアドレスを取得します。
* @param hostname 	ホスト名
* @param detail 	詳細設定（optional)
* @param error		失敗時のエラー(error)
*
* @return NULLの場合は、errorを参照。
*
* @return 通信状態を介します。
*/
SSL_EXTERN HandleHttp HttpCreateInBuffer(void* buffer, address_t addr, const char* hostname, THttpDetail* detail, THttpError* error);


/** @brief POSTリクエスト
*  
* @param [in] hHttps		HttpCreateで生成されたハンドル
* @param [in]path		パス
* @param [in]data		POSTで送るデータ
* @param [in]contenttype ContentType
* @param [in]dataSize	dataのサイズ
*
* @return 通信状態を返します。
*/
SSL_EXTERN THttpError HttpPost(HandleHttp hHttps, const char* path, const void* data, const char* contentType, int dataSize);

/** @brief GETリクエスト
*  
* @param [in] hHttps	HttpCreateで生成されたハンドル
* @param [in]path		urlの後ろのパス (フルパスでも可能)
*
* @return 通信状態を返します。
*/
SSL_EXTERN THttpError HttpGet(HandleHttp hHttps, const char* path);

/** @brief GETリクエスト
*  
* @param [in] hHttps	HttpCreateで生成されたハンドル
* @param [in]path		urlの後ろのパス
* @param [in]contenttype ContentType
*
* @return 通信状態を返します。
*/
SSL_EXTERN THttpError HttpMessage(HandleHttp hHttps, const Char* header, const void* data, int size);

#ifdef HTTP_USING_C_PLUS_PLUS

/** @brief GET/POST後、レスポンスを取得します。
*  
* @param hHttps		HttpCreateで生成されたハンドル
*
* @return レスポンスの結果
* @retval kHE_SUCCESS 成功
* @retval 詳細はTHttpError参照
*/
SSL_EXTERN const HttpResponse* HttpGetResponse(HandleHttp hHttps);

#else
/** @brief GET/POST後、レスポンスを取得します。
*  
* @param hHttps		HttpCreateで生成されたハンドル
*
* @return レスポンスの結果
* @retval kHE_SUCCESS 成功
* @retval 詳細はTHttpError参照
*/
SSL_EXTERN const THttpResponse* HttpGetResponse(HandleHttp hHttps);

#endif

#ifdef HTTP_USING_C_PLUS_PLUS
/** \brief HttpGetResponseで取得したレスポンスを削除します。\n
* THttpDetailのisAutoFreeがtrueの場合は次のリクエストか、HttpClose時に削除されます。
*
*\param HttpGetResponseから取得したレスポンス
*/
SSL_EXTERN void HttpDeleteResponse(const HttpResponse* response);
#else
/** \brief HttpGetResponseで取得したレスポンスを削除します。\n
* THttpDetailのisAutoFreeがtrueの場合は次のリクエストか、HttpClose時に削除されます。
*
*\param HttpGetResponseから取得したレスポンス
*/
SSL_EXTERN void HttpDeleteResponse(THttpResponse* response);
#endif

/** @brief コネクションを切り、リソースを解放します。
*
* @param hHttps HttpCreateで生成されたハンドル
*
*/
SSL_EXTERN void HttpClose(HandleHttp hHttps);

/** @brief 最後に発生したエラーを取得します。
*
* @param hHttps HttpCreateで生成されたハンドル
* 
* @return エラーコード
* @retval 
*/
SSL_EXTERN THttpError HttpGetLastError(HandleHttp hHttps);


/** @brief THttpDetail構造体をデフォルト値に設定します。
*
* @param detail THttpDetailのポインター
* 
* @return 指定した構造体のポインター
* @retval 
*/
SSL_EXTERN THttpDetail* HttpSetDefaultDetail(THttpDetail* detail);

/** @brief Httpのコネクションが有効であるか無効であるかを取得します。
*
* @param hHttps HttpCreateで生成されたハンドル
* 
* @return 接続がされているかどうか
* @retval  true 接続中
* @retval  flase 接続切れ
*/
SSL_EXTERN Bool HttpIsConnected(HandleHttp hHttp);

/** @brief Httpのリクエストヘッダーを作成します。
*/
SSL_EXTERN Char* HttpMakeHeader(const Char* kHostName, S32 dataSize, S32 port,
			const Char* path, U32 keepAliveTime, const Char* contentType, const Char* charSet);

/**
*/
SSL_EXTERN Char* HttpMakeGeneralHeader(char* buffer, int bufferLen, const Char* kHostName, S32 dataSize, S32 port, const char* path, const TRequestHttpHeader* headers, int headerCount);

/** @brief Httpのリクエストヘッダーを削除します。
*/
SSL_EXTERN void HttpHeaderDepose(Char* header);

/** @brief Httpのリクエストヘッダーを作成します。
*/
SSL_EXTERN Char* HttpMakeHeaderInBuffer(char* buffer, int bufferSize, const Char* kHostName, S32 dataSize, S32 port,
			const Char* path, U32 keepAliveTime, const Char* contentType, const Char* charSet);



#ifdef __cplusplus
ADK_END_NAMESPACE
#endif
#endif

