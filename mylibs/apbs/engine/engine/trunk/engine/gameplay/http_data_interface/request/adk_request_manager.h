/**
 * \file adk_request_manager.h
 * \ingroup adkgrp_request
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief リクエストマネージャクラス定義部
 */


#ifndef ADK_REQUEST_MANAGER_H_
#define ADK_REQUEST_MANAGER_H_


#include <pthread.h>

#include <core/library/lib_json_document.h>
#include <core/library/lib_list.h>
#include <core/types.h>
#include <gameplay/http_data_interface/dms/dms_schema.h>
#include <network/http/adk_network_types.h>
#include <gameplay/http_data_interface/request/adk_i_request.h>
#include <gameplay/http_data_interface/request/adk_request_types.h>


ADK_BEGIN_NAMESPACE


/* class definition */
/**
 * \~english
 * \~japanese
 *   \brief リクエストマネージャ
 */
class RequestManager{
public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   */
  RequestManager();
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  virtual ~RequestManager();

  /* initialize & terminate */
  /**
   * \~english
   * \~japanese
   *   \brief リクエストマネージャの初期化処理を行います
   *   \return 初期化処理の成功判定。以下の値が返ります。
   *   \retval ResultSuccess  初期化に成功したときに返ります
   */
  Result Initialize();
  /**
   * \~english
   * \~japanese
   *   \brief リクエストマネージャの終了処理を行います
   *   \return 終了処理の成功判定。以下の値が返ります。
   *   \retval ResultSuccess  終了に成功したときに返ります
   */
  Result Terminate();

  /* request management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief 指定された URL に対してリクエストを行います
   *   \param[in] url リクエスト先の URL
   *   \param[in] reqtype リクエストタイプ \ref IRequest::TRequestType
   *   \param[in] schema リクエスト結果の格納先スキーマ \ref DMS_Schema クラスへのポインタ
   *   \return リクエストの成功判定。以下の値が返ります
   *   \retval ResultSuccess  リクエストに成功したときに返ります
   */
  Result Request( const Char *url, IRequest::TRequestType reqtype, DMS_Schema *schema);
  /**
   * \~english
   * \~japanese
   *   \brief リクエストキューが存在していれば、リクエスト処理を実行してキューから取り外す
   */
  void DoRequestProcess();

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
   *  \retval ResultNotInitialized 初期化が行われていないのに呼び出されたときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  Result ApplyHttpRequestHeader(const Char *key,const Char *value);
  /**
   * \~englisth
   * \~japanese
   *  \brief HTTPリクエストヘッダ数を返します
   */
  ADK_INLINE S32 GetHttpRequestHeaderCount() const;
  /**
   * \~englisth
   * \~japanese
   *  \brief HTTPリクエストヘッダのキーを返します
   *  \param[in] header 参照したいヘッダインデックス\n
   *                    0 <= header < \ref GetHttpRequestHeaderCount()
   */
  ADK_INLINE const TRequestHttpHeader* GetHttpRequestHeader() const;

private:
  /* http management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief 指定キーの共通パラメータが存在していたら返す
   *  \param[in] key 探すキー文字列
   *  \return \ref TJsonCommonParameter
   */
  TJsonCommonParameter *FindJsonCommonParameter(const Char *key);
  /**
   * \~english
   * \~japanese
   *  \brief 指定キーの共通パラメータが存在していたら返す
   *  \param[in] key 探すキー文字列
   *  \return \ref TJsonCommonParameter
   */
  TJsonCommonParameter *CreateJsonCommonParameter(const Char *key);
  /**
   * \~english
   * \~japanese
   *  \brief JSON共通パラメータ文字列を作成する
   */
  Result CreateJsonCommonParameterString();

  /* vars */
  /**
   * \~english
   * \~japanese
   *   \brief 通信スレッド関連
   */
  struct{
    struct{
      Bool initialized;
      pthread_t thread;
    }thread;
  }network_;
  /**
   * \~english
   * \~japanese
   *   \brief リクエスト
   */
  struct{
    struct{
      Bool  initmutex;
      pthread_mutex_t  mutex;
    }queue;
  }_request;
  /**
   * \~english
   * \~japanese
   *   \brief エラー情報
   */
  struct{
    Char  *bin;
    U32  size;
    LIB_JsonDocument  jdoc;
  }_error;
  /**
   * \~english
   * \~japanese
   *   \brief リクエストキュー管理リスト
   */
  HandleList  _requests;
  /**
   * \~english
   * \~japanese
   *  \brief 共通パラメータ
   */
  struct {
    S32 paramCount;
    TJsonCommonParameter params[ADK_JSON_COMMON_PARAMETER_MAX];
    Char *request;
  }common_;
  /**
   * \~english
   * \~japanese
   *  \~brief HTTPリクエストヘッダ群
   */
  struct {
    struct {
      Byte *binary;
      U32 key;
      U32 value;
      U32 once;
      U32 size;
    }data;
    S32 count;
    TRequestHttpHeader *headers;
  }httpHeader_;

  /**
   * \~english
   * \~japanese
   *   \brief クラス内 new/delete オペレータ定義
   */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("RequestManager");
  /**
   * \~english
   * \~japanese
   *   \brief コピー禁止定義
   */
  ADK_NON_COPYABLE_CLASS(RequestManager);
};
ADK_INLINE const Char* RequestManager::GetJsonCommonParameter() const {
  return common_.request;
}
ADK_INLINE S32 RequestManager::GetHttpRequestHeaderCount() const {
  return httpHeader_.count;
}
ADK_INLINE const TRequestHttpHeader* RequestManager::GetHttpRequestHeader() const {
  return httpHeader_.headers;
}


ADK_END_NAMESPACE


#endif  /* ADK_REQUEST_MANAGER_H_ */


