/**
 * \file adk-api.h
 * \ingroup adkgrp_api
 * \author tomonori tanaka
 * \~english
 *  \brief Http data interface API
 * \~japanese
 *  \brief Http data interface のアプリケーションインタフェース定義群
 */

#ifndef HTTP_DATA_INTERFACE_API_H_
#define HTTP_DATA_INTERFACE_API_H_

#include <core/debug/logging_types.h>
#include <core/types.h>
#include <debug_profiling/error/result.h>
#include <debug_profiling/error/result_types.h>
#include <gameplay/http_data_interface/dms/dms_schema.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class HttpResponse;

class HttpDataInterface {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief ADKを使用するための初期化処理を行います
   *  \return 初期化処理の成功判定。以下の値が返ります
   *  \retval ResultSuccess        初期化処理に成功したときに返ります
   *  \retval ResultDoAlready      既に初期化されているときに返ります
   *  \retval ResultOutOfResource  システムリソースが不足しているときに返ります
   */
  static Result Initialize();
  /**
   * \~english
   * \~japanese
   *  \brief 使用中のADKの処理を停止し終了処理を行います
   *  \return 終了処理の成功判定。以下の値が返ります
   *  \retval ResultSuccess        終了処理に成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   */
  static Result Terminate();

  /* Access point management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief マスターサーバーのホスト名を取得します
   *  \param[out] url ホスト名格納先
   *  \param[in] urllen ホスト名格納先の文字列長
   *  \return 取得の成功判定。以下の値が返ります
   *  \retval ResultSuccess         取得に成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable 不正な文字列が設定されていたときに返ります
   */
  static Result GetMasterServerUrl(Char *url, U32 urllen);
  /**
   * \~english
   * \~japanese
   *  \brief 接続可能なサーバー数を取得します
   *  \param[out] count サーバー数格納先
   *  \return 取得の成功判定。以下の値が返ります
   *  \retval ResultSuccess          取得に成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable  不正な文字列が設定されていたときに返ります
   */
  static Result GetServerCount(S32 *count);
  /**
   * \~english
   * \~japanese
   *  \brief 接続可能なサーバーのホスト名を取得します
   *  \param[out] url ホスト名格納先
   *  \param[in] urllen ホスト名格納先の文字列長
   *  \param[in] server 取得するサーバーインデックス( 0 <= server < \ref
   * GetServerCount() )
   *  \return 取得の成功判定。以下の値が返ります
   *  \retval ResultSuccess          取得に成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable  不正な文字列が設定されていたときに返ります
   */
  static Result GetServerUrl(Char *url, U32 urllen, S32 server);
  /**
   * \~english
   * \~japanese
   *  \brief 接続可能なサーバーの表示名を取得します
   *  \param[out] name 表示名の格納先
   *  \param[in] namelen 表示名格納先の文字列長
   *  \param[in] server 取得したいサーバー( 0 <= server < \ref GetServerCount() )
   *  \return 取得の成功判定。以下の値が返ります
   *  \retval ResultSuccess          取得に成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable  不正な文字列が設定されていたときに返ります
   *  \retval ResultOutOfRange 引数に指定された値が範囲を超えていたときに返ります
   */
  static Result GetServerDisplayName(Char *name, U32 namelen, S32 server);

  /* ADK process */
  /**
   * \~english
   * \~japanese
   *  \brief 任意のホストへログインする処理を行います
   *  \param[in] host ホスト名
   *  \return ログインの成功判定。以下の値が返ります
   *  \retval ResultSuccess      ログインに成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されていたときに返ります
   *  \retval ResultConnectionError 通信処理に失敗したときに返ります
   */
  static Result Login(const Char *host);
  /**
   * \~english
   * \~japanese
   *  \brief ADKのディスパッチ処理を行います
   *  \return ディスパッチ処理の成功判定。以下の値が返ります
   *  \retval ResultSuccess         ログインに成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   */
  static Result DispatchProcess();
#if 0
  /**
   * \~english
   * \~japanese
   *  \brief 新しいフェーズを既存のフェーズと入れ替えて実行します
   *  \param[in] phase 新しく実行するフェーズ
   *  \return 実行の成功判定。以下の値が返ります
   *  \retval ResultSuccess         ログインに成功したときに返ります
   *  \retval ResultNotInitialized  初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されていたときに返ります
   */
  static Result ReplacePhase(IPhase *phase);
#endif

  /* DMS management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief DMSスキーマを返します
   *  \param[in] url 取得したいスキーマのもつURL
   *  \return \ref DMS_Schema
   */
  static DMS_Schema *GetSchema(const Char *url);

  /* error handling management functionally */
  /**
   * \~englisth
   * \~japanese
   *  \brief エラーが発生したかを返します
   *  \return エラーフラグ。以下の値が返ります
   *  \retval true  エラーが発生したときに返ります
   *  \retval false エラーが発生していないときに返ります
   */
  static Bool IsError();
  /**
   * \~englisth
   * \~japanese
   *  \brief エラー情報を取得します
   *  \param[out] code エラーコード格納先
   *  \param[out] message エラーメッセージ格納先
   *  \param[in] messageLen エラーメッセージ格納先の文字列長
   *  \return 取得の成功判定。以下の値が返ります
   *  \retval ResultSuccess         取得に成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されていたときに返ります
   */
  static Result GetError(S32 *code, Char *message, U32 messageLen);

  /* http management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief JSON共通パラメータ { "key":"value" } をADKに追加する
   *  \param[in] key 共通パラメータの"key"を指定します。\n
   *                 既に設定された"key"だった場合、"value"が上書きされます。\n
   *                 最大長は \ref ADK_JSON_COMMON_PARAMETER_KEY_LEN になります
   *  \param[in] value リクエストヘッダの"value"を設定します。\n
   *                   最大長は \ref ADK_JSON_COMMON_PARAMETER_VALUE_LEN
   * になります
   *  \return 追加の成功判定。以下の値が返ります
   *  \retval ResultSuccess 追加に成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  static Result AddJsonCommonParameter(const Char *key, const Char *value);
  /**
   * \~english
   * \~japanese
   *  \brief JSON共通パラメータ { "key":S32 } をADKに追加する
   *  \param[in] key 共通パラメータの"key"を指定します。\n
   *                 既に設定された"key"だった場合、"value"が上書きされます。\n
   *                 最大長は \ref ADK_JSON_COMMON_PARAMETER_KEY_LEN になります
   *  \param[in] value リクエストヘッダのS32を設定します。
   *  \return 追加の成功判定。以下の値が返ります
   *  \retval ResultSuccess 追加に成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  static Result AddJsonCommonParameter(const Char *key, S32 value);
  /**
   * \~english
   * \~japanese
   *  \brief JSON共通パラメータ { "key":U64 } をADKに追加する
   *  \param[in] key 共通パラメータの"key"を指定します。\n
   *                 既に設定された"key"だった場合、"value"が上書きされます。\n
   *                 最大長は \ref ADK_JSON_COMMON_PARAMETER_KEY_LEN になります
   *  \param[in] value リクエストヘッダのU64を設定します。
   *  \return 追加の成功判定。以下の値が返ります
   *  \retval ResultSuccess 追加に成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  static Result AddJsonCommonParameter(const Char *key, U64 value);
  /**
   * \~englisth
   * \~japanese
   *  \brief HTTPリクエストヘッダ { "key":"value" } をADKに適用する
   *  \param[in] key HTTPヘッダの"key"を指定します。\n
   *                 既に設定された"key"だった場合、"value"が上書きされます。\n
   *                 最大長は \ref ADK_HTTP_REQUEST_HEADER_KEY_LEN になります
   *  \param[in] value リクエストヘッダの"value"を設定します。\n
   *                   最大長は \ref ADK_HTTP_REQUEST_HEADER_VALUE_LEN になります
   *  \return 適用の成功判定。以下の値が返ります
   *  \retval ResultSuccess 適用に成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  static Result ApplyHttpRequestHeader(const Char *key, const Char *value);
  /**
   * \~english
   * \~japanese
   *  \brief HTTPのPOSTリクエストを行う(この関数はブロックします)
   *  \param[out] data POSTしたデータの格納先
   *  \param[in] url リクエスト先のURL
   *  \param[in] user_request_header \ref ApplyHttpRequestHeader() 関数で
   *                                 設定されているヘッダを使用するかのフラグ
   *  \return リクエストの成功判定。以下の値が返ります
   *  \retval ResultSuccess リクエストに成功したときに返ります
   *  \retval ResultNotInitialized
   * 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  static Result RequestPostHttp(const HttpResponse **data, const Char *url,
                                Bool user_request_header);

  /* debug management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief ログの出力レベルを設定します。
   *  \param[in] level ログ出力レベル \ref TLogLevel を設定します
   */
  static Result SetDebugLogLevel(TLogLevel level);
};

ADK_END_NAMESPACE

#endif /* HTTP_DATA_INTERFACE_API_H_ */

