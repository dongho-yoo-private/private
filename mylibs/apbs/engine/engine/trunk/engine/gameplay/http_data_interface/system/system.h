/**
 * \file system.h
 * \ingroup adkgrp_system
 * \author tomonori tanaka
 * \~english
 *  \brief Network SDK System
 * \~japanese
 *  \brief ADK全体のコンポーネント管理を行うシステム定義部
 */


#ifndef HTTP_DATA_INTERFACE_SYSTEM_H_
#define HTTP_DATA_INTERFACE_SYSTEM_H_

#include <pthread.h>

#include <core/debug/logging_types.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <debug_profiling/error/result.h>
#include <debug_profiling/error/err_error_manager.h>
#include <gameplay/http_data_interface/dms/adk_dms.h>
#include <gameplay/http_data_interface/dms/dms_schema.h>
#include <gameplay/http_data_interface/hostlist/adk_host_list.h>
#include <network/http/adk_i_np_response.h>
#include <network/http/adk_network_protocol.h>
#include <gameplay/http_data_interface/request/adk_request_manager.h>
#include <gameplay/http_data_interface/startup/adk_startup_info.h>


ADK_BEGIN_NAMESPACE


/* forward declaration */
class HttpResponse;
class DMS;


/* class definition */
/**
 * \~english
 * \~japanese
 *   \brief ADK全体のコンポーネント管理を行うシステムクラス\n
 *          Mediatorパターンによって各コンポーネントを管理/実装している
 */
class System {
public:
  /* static method */
  /**
   * \~english
   * \~japanese
   *  \brief ADK を使用するための初期化処理を行います
   *  \return 初期化処理の成功判定が返ります
   *  \retval ResultSuccess 初期化処理に成功したときに返ります
   *  \retval ResultDoAlready 既に初期化されているときに返ります
   *  \retval ResultOutOfResource システムリソースが不足しているときに返ります
   */
  static Result Initialize();
  /**
   * \~english
   * \~japanese
   *  \brief 使用されているADK を停止し終了します
   *  \return 終了処理の成功判定が返ります
   *  \retval ResultSuccess 終了処理に成功したときに返ります
   *  \retval ResultNotInitialized 初期化が行われていないのに呼び出されたときに返ります
   */
  static Result Terminate();
  /**
   * \~english
   * \~japanese
   *   \brief システムクラスを返します
   *   \return システムクラス
   */
  static System *GetSystem();

public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   */
  System();
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  ~System();

  /* initialize & terminate */
  /**
   * \~english
   * \~japanese
   *   \brief システムの初期化処理を行います
   *   \return 初期化の成功判定。以下の値が返ります
   *   \retval ResultSuccess    初期化処理に成功したときに返ります
   *   \retval ResultDoAlready    既に初期化されているときに返ります
   *   \retval ResultOutOfResource  システムリソースが不足しているときに返ります
   */
  Result Init();
  /**
   * \~english
   * \~japanese
   *   \brief システムの終了処理を行います
   *   \return 終了の成功判定。以下の値が返ります
   *   \retval ResultSuccess      終了処理に成功したときに返ります
   *   \retval ResultNotInitialized  初期化が行われていないのに呼び出されたときに返ります
   */
  Result Term();
  /**
   * \~english
   * \~japanese
   *  \brief システムが終了中かを返します。
   *  \return 終了中かのフラグ
   */
  ADK_INLINE Bool IsDoTerm();

  /* ADK process */
  /**
   * \~english
   * \~japanese
   *   \brief 任意のホストへログインする処理を行います
   *   \param[in] host ホスト名
   *   \return ログインの成功判定。以下の値が返ります
   *   \retval ResultSuccess      ログインに成功したときに返ります
   *   \retval ResultNotInitialized  初期化が行われていないのに呼び出されたときに返ります
   *   \retval ResultInvalidVariable  不正な引数が設定されていたときに返ります
   *   \retval ResultConnectionError  通信処理に失敗したときに返ります
   */
  Result Login( const Char *host);
  /**
   * \~english
   * \~japanese
   *   \brief ADKのディスパッチ処理を行います
   *   \return ディスパッチ処理の成功判定。以下の値が返ります
   *   \retval ResultSuccess      ログインに成功したときに返ります
   *   \retval ResultNotInitialized  初期化が行われていないのに呼び出されたときに返ります
   */
  Result DispatchProcess();

  /* host list management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief サーバー情報クラスを返します
   *   \return \ref ADK_HostList
   */
  ADK_INLINE const Char* GetMasterServerUrl() const;
  /**
   * \~english
   * \~japanese
   *   \brief 接続可能なサーバー数を返します。
   *   \return サーバー数
   */
  ADK_INLINE S32 GetServerCount() const;
  /**
   * \~english
   * \~japanese
   *   \brief 接続可能なサーバーのURLを返します
   *   \param[in] server 取得したいサーバーのインデックス 0 <= server < \ref GetServerCount()
   *   \return URL
   */
  ADK_INLINE const Char *GetServerUrl( S32 server) const;
  /**
   * \~english
   * \~japanese
   *   \brief [未実装]接続可能なサーバーの表示名を返します
   *   \param[in] server 取得したいサーバーのインデックス 0 <= server < \ref GetServerCount()
   *   \return 名前
   */
  ADK_INLINE const char *GetServerDisplayName( S32 server) const;
  /**
   * \~english
   * \~japanese
   *   \brief アクセスポイントを設定します
   *   \param[in] url 接続先ホスト
   *   \return 設定の成功判定。以下の値が返ります
   *   \retval ResultSuccess      ログインに成功したときに返ります
   *   \retval ResultNotInitialized  初期化が行われていないのに呼び出されたときに返ります
   *   \retval ResultInvalidVariable  不正な引数が設定されていたときに返ります
   *   \retval ResultOutOfResource    システムリソースが不足しているときに返ります
   */
  Result SetAccessPoint( const Char *url);
  /**
   * \~english
   * \~japanese
   *   \brief 設定されているアクセスポイントを返します
   *   \return アクセスポイント。以下の値が返ります
   *   \retval null  アクセスポイントが設定されていないときに返ります
   *   \retval etc    設定されたアクセスポイントのアドレスが返ります
   */
  ADK_INLINE const Char* GetAccessPoint() const;

  /* request management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief リクエストする
   */
  Result Request(
      const Char *url,
      IRequest::TRequestType reqtype,
      DMS_Schema *schema);
  /**
   * \~english
   * \~japanese
   *   \brief リクエストする
   */
  Result RequestAsync(
      const Char *url,
      IRequest::TRequestType reqtype,
      DMS_Schema *schema);
  /**
   * \~english
   * \~japanese
   *   \brief リクエスト処理を進行させる\n
   *          \ref RequestAsync でリクエストを呼び出した際は、
   *          このメソッドでリクエスト処理を進行させる。
   */
  void DoRequestProcess();

  /* memory management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief メモリスレッドのロックコールバック
   */
  void DoMemoryLock();
  /**
   * \~english
   * \~japanese
   *   \brief メモリスレッドのアンロックコールバック
   */
  void DoMemoryUnlock();

#if 0
  /* phase management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief フェーズを切り替える
   *   \param[in] phase フェーズ
   */
  Result ReplacePhase( IPhase *phase);
  /**
   * \~english
   * \~japanese
   *   \brief フェーズ処理を進行させる
   */
  Result DoPhaseProcess();
#endif

  /* DMS management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief DMSスキーマを返します
   *   \param[in] url 取得したいスキーマのもつURL
   *   \return \ref DMS_Schema
   */
  DMS_Schema* GetSchema( const Char *url);
  /**
   * \~english
   * \~japanese
   *   \brief 共通レスポンスDMSスキーマを返します
   *   \return \ref DMS_Schema
   */
  DMS_Schema* GetCommonResponseSchema();
  /**
   * \~english
   * \~japanese
   *   \brief スキーマのリクエスト情報をリセットします
   */
  void ResetRequestSchema();

  /* network protocol managment functionally */
  /**
   * \~english
   * \~japanese
   *   \brief WASにデータ取得をリクエストする
   *   \param[out] response 取得情報格納先
   *   \param[in] url リクエスト先のURL
   *   \param[in] data リクエストデータ
   *   \param[in] datasize リクエストデータサイズ
   *   \return リクエストの成功判定。以下の値が返ります
   *   \retval ResultSuccess  リクエストに成功したときに返ります
   */
  Result RequestToWas( const HttpResponse **response, const Char *url, const void *data, U32 datasize);
  /**
   * \~english
   * \~japanese
   *   \brief WASへのリクエストで受け取ったデータを削除する
   */
  Result DisposeRequestToWas( const HttpResponse *response);
  /**
   * \~english
   * \~japanese
   *  \brief リクエストヘッダのキーのバッファサイズ
   */
  ADK_INLINE U32 GetHttpKeyLength() const;
  /**
   * \~english
   * \~japanese
   *  \brief リクエストヘッダの値のバッファサイズ
   */
  ADK_INLINE U32 GetHttpValueLength() const;

  /* error handling management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief エラー処理を開始します
   *   \return 開始の成功判定。以下の値が返ります
   *   \retval ResultSuccess  開始に成功したときに返ります
   *   \retval ResultDoAlready  既に開始されているときに返ります
   */
  Result StartErrorProcess();
  /**
   * \~english
   * \~japanese
   *   \brief エラー処理を終了します
   *   \return 終了の成功判定。以下の値が返ります
   *   \retval ResultSuccess      終了に成功したときに返ります
   *   \retval ResultNotInitialized  開始処理が行われていないときに返ります
   */
  Result EndErrorProcess();
  /**
   * \~englisth
   * \~japanese
   *   \brief エラーが発生したかを返します
   *   \return エラーフラグ。以下の値が返ります
   *   \retval true  エラーが発生したときに返ります
   *   \retval false  エラーが発生していないときに返ります
   */
  Bool IsError();
  /**
   * \~englisth
   * \~japanese
   *   \brief エラーメッセージを設定します
   *   \deprecated この関数はADK内部で扱うものなので、\n
   *               アプリケーション側で使う事はありません
   *   \param[in] code エラーコード
   *   \param[in] message エラーメッセージ
   */
  Result SetErrorMessage(S32 code,const Char *message);
  /**
   * \~englisth
   * \~japanese
   *   \brief エラー情報を取得します
   *   \param[out] code エラーコード格納先
   *   \param[out] message エラーメッセージ格納先
   *   \param[in] messageLen エラーメッセージ格納先の文字列長
   */
  Result GetErrorMessage(S32 *code, Char *message, U32 messageLen) const;

  /* http management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief JSON共通パラメータ { "key":"value" } をADKに追加する
   *  \param[in] key 共通パラメータの"key"を指定します。\n
   *                 既に設定された"key"だった場合、"value"が上書きされます。\n
   *                 最大長は \ref ADK_JSON_COMMON_PARAMETER_KEY_LEN になります
   *  \param[in] value リクエストヘッダの"value"を設定します。\n
   *                   最大長は \ref ADK_JSON_COMMON_PARAMETER_VALUE_LEN になります
   *  \return 追加の成功判定。以下の値が返ります
   *  \retval ResultSuccess 追加に成功したときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  Result AddJsonCommonParameter(const Char *key,const Char *value);
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
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  Result AddJsonCommonParameter(const Char *key,S32 value);
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
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  Result AddJsonCommonParameter(const Char *key,U64 value);
  /**
   * \~englisth
   * \~japanese
   *  \brief 作成されたJSON共通パラメータ文字列を返します
   *  \return 文字列のポインタ。以下の値が返ります
   *  \retval null 共通パラメータが存在しない場合に返ります
   *  \retval etc 作成された文字列のアドレスが返ります
   */
  ADK_INLINE const Char* GetJsonCommonParameter() const;
  /**
   * \~englisth
   * \~japanese
   *  \brief HTTPリクエストヘッダ { "key":"value" } をADKに適用する
   *  \param[in] key HTTPヘッダの"key"を指定します。\n
   *                 既に設定された"key"だった場合、"value"が上書きされます。
   *  \param[in] value リクエストヘッダの"value"を設定します。
   *  \return 適用の成功判定。以下の値が返ります
   *  \retval ResultSuccess 適用に成功したときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  Result ApplyHttpRequestHeader(const Char *key,const Char *value);
  /**
   * \~english
   * \~japanese
   *  \brief HTTPのPOSTリクエストを行う
   *  \param[out] data POSTしたデータの格納先
   *  \param[in] url リクエスト先のURL
   *  \param[in] user_request_header \ref ApplyHttpRequestHeader() 関数で
   *                                 設定されているヘッダを使用するかのフラグ
   *  \return リクエストの成功判定。以下の値が返ります
   *  \retval ResultSuccess リクエストに成功したときに返ります
   *  \retval ResultNotInitialized 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  Result RequestPostHttp(
      const HttpResponse **data,const Char *url,Bool user_request_header);

  /* debug management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief ログの出力レベルを設定します。
   *  \param[in] level ログ出力レベル \ref TLogLevel を設定します
   */
  void SetDebugLogLevel(TLogLevel level);

  /* System group to be managed by the mediator */
  /**
   * \~english
   * \~japanese
   *   \brief ADK起動情報クラス \ref ADK_StartupInfo を返します
   *   \return \ref ADK_StartupInfo
   *   \retval null  \ref ADK_StartupInfo オブジェクトが存在しないときに返ります
   */
  ADK_INLINE ADK_StartupInfo* GetStartupInfo();

  /**
   * \~english
   * \~japanese
   *  \brief DMSを返します
   */
  ADK_INLINE DMS *GetDMS();

private:
  /* vars */
  /**
   * \~english
   * \~japanese
   *   \brief 唯一無二のシステム
   */
  static System *system_;
  /**
   * \~english
   * \~japanese
   *   \brief 終了中かのフラグ
   */
  Bool is_do_terminate_;
  /**
   * \~english
   * \~japanese
   *   \brief メモリ関係
   */
  struct{
    Bool initialized;
    pthread_mutex_t mutex;
  }memory_;
  /**
   * \~english
   * \~japanese
   *   \brief スレッド
   */
  struct{
    Bool initialized;
    pthread_mutex_t mutex;
  }thread_;
  /**
   * \~english
   * \~japanese
   *   \brief 起動情報管理クラス
   */
  ADK_StartupInfo *startup_;
  /**
   * \~english
   * \~japanese
   *   \brief ホストリスト
   */
  ADK_HostList *hostlist_;
  /**
   * \~english
   * \~japanese
   *   \brief データマネージメントシステム
   */
  DMS  *_dms;
  /**
   * \~english
   * \~japanese
   *   \brief リクエストマネージャ
   */
  RequestManager  *request_manager_;
  /**
   * \~english
   * \~japanese
   *   \brief エラーマネージャ
   */
  ERR_ErrorManager  *error_manager_;
#if 0
  /**
   * \~english
   * \~japanese
   *   \brief フェーズマネージャ
   */
  PhaseManager  *phase_manager_;
#endif
  /**
   * \~english
   * \~japanese
   *   \brief ネットワークプロトコル
   */
  struct{
    NetworkProtocol  *protocol;
  }network_protocol_;

  /**
   * \~english
   * \~japanese
   *   \brief クラス内 new/delete オペレータ定義
   */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("System");
  /**
   * \~english
   * \~japanese
   *   \brief コピー禁止定義
   */
  ADK_NON_COPYABLE_CLASS(System);
};
ADK_INLINE const Char* System::GetMasterServerUrl() const
{
  return hostlist_->GetMasterServerUrl();
}
ADK_INLINE S32 System::GetServerCount() const
{
  return hostlist_->GetServerCount();
}
ADK_INLINE const Char *System::GetServerUrl( S32 server) const
{
  return hostlist_->GetServerUrl(server);
}
ADK_INLINE const char *System::GetServerDisplayName( S32 server) const
{
  return hostlist_->GetServerDisplayName(server);
}
ADK_INLINE const Char* System::GetAccessPoint() const
{
  return hostlist_->GetAccessPoint();
}
ADK_INLINE ADK_StartupInfo* System::GetStartupInfo()
{
  return startup_;
}
ADK_INLINE const Char* System::GetJsonCommonParameter() const {
  return request_manager_->GetJsonCommonParameter();
}
ADK_INLINE U32 System::GetHttpKeyLength() const {
  return startup_->GetHttpKeyLength();
}
ADK_INLINE U32 System::GetHttpValueLength() const {
  return startup_->GetHttpValueLength();
}
ADK_INLINE DMS *System::GetDMS() {
  return _dms;
}
ADK_INLINE Bool System::IsDoTerm() {
  return is_do_terminate_;
}

ADK_END_NAMESPACE

#endif  /* HTTP_DATA_INTERFACE_SYSTEM_H_ */

