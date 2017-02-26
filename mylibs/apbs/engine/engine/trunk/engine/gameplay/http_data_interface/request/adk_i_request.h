/**
 * \file adk_i_request.h
 * \ingroup adkgrp_request
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief リクエストインターフェースクラス定義部
 */


#ifndef ADK_I_REQUEST_H_
#define ADK_I_REQUEST_H_


#include <core/library/lib_i_object.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <debug_profiling/error/result.h>
#include <debug_profiling/error/result_types.h>
#include <network/http/adk_http_response.h>


ADK_BEGIN_NAMESPACE


/* class definition */
/**
 * \~english
 * \~japanese
 *   \brief リクエストクラス
 */
class IRequest : public IObject {
 public:
  /* reqeust type */
  typedef enum {
    kRequestUnknown,
    kRequestResponse,
    kRequestAck
  }TRequestType;

  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   */
  IRequest();
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  virtual ~IRequest();

  /* request management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief リクエストの初期化処理を行います
   *   \param[in] url リクエスト先のホスト名
   *   \param[in] reqtype リクエストタイプ
   *   \param[in] requestData リクエストデータ
   *   \return 初期化の成功判定。以下の値が返ります
   *   \retval ResultSuccess      初期化に成功したときに返ります
   *   \retval ResultDoAlready      既に初期化されているときに返ります
   *   \retval ResultInvalidVariable  不正な引数が設定されていたときに返ります
   *   \retval ResultOutOfResource    システムリソースが不足しているときに返ります
   */
  Result Initialize( const Char *url, TRequestType reqtype, const Char *requestData);
  /**
   * \~english
   * \~japanese
   *   \brief リクエストの終了処理を行います
   *   \return 終了の成功判定。以下の値が返ります
   *   \retval ResultSuccess      終了に成功したときに返ります
   *   \retval ResultNotInitialized  初期化が行われていないのに呼び出されたときに返ります
   */
  Result Terminate();
  /**
   * \~english
   * \~japanese
   *   \brief リクエスト処理を実行します。
   *   \return 実行の成功判定。以下の値が返ります
   *   \retval ResultSuccess      削除に成功したときに返ります
   *   \retval ResultNotInitialized  初期化が行われていないのに呼び出されたときに返ります
   */
  Result DoRequest();

  /* data management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief リクエストした URL 文字列を返します
   *   \return URL 文字列
   */
  ADK_INLINE const Char *GetUrl() const;
  /**
   * \~english
   * \~japanese
   *   \brief ステータスを返します
   *   \return ステータス
   */
  ADK_INLINE S32 GetStatus() const;
  /**
   * \~english
   * \~japanese
   *   \brief レスポンスデータを返します
   *   \return データ
   */
  ADK_INLINE const Byte *GetResponseData() const;
  /**
   * \~english
   * \~japanese
   *   \brief レスポンスデータサイズを返します
   *   \return データサイズ
   */
  ADK_INLINE U32 GetResponseDataSize() const;

#if 0
 protected:
  /* header management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief 共通ヘッダ情報を作成する
   *   \return 作成されたヘッダ文字列を返します
   */
  virtual Char* CreateCommonHeader() = 0;
  /**
   * \~english
   * \~japanese
   *   \brief 作成された共通ヘッダ情報を削除する
   *   \param[in] header 削除する共通ヘッダ情報
   */
  void DisposeCommonHeader( Char *header);
#endif

 private:
  /* request management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief リクエストレスポンスを実行します
   *   \return 実行の成功判定。以下の値が返ります
   */
  Result DoRequestResponse();
  /**
   * \~english
   * \~japanese
   *   \brief リクエストACKを実行します
   *   \return 実行の成功判定。以下の値が返ります
   */
  Result DoRequestAck();

  /* vars */
  /**
   * \~english
   * \~japanese
   *   \brief リクエストタイプ
   */
  TRequestType  _requestType;
  /**
   * \~english
   * \~japanese
   *   \brief リクエストURL
   */
  Char  *url_;
  /**
   * \~english
   * \~japanese
   *   \brief リクエスト時に付加するデータ
   */
  struct {
    /**
     * \~english
     * \~japanese
     *   \brief データアドレス
     */
    void  *addr;
    /**
     * \~english
     * \~japanese
     *   \brief データのサイズ
     */
    U32  size;
  }data_;
  /**
   * \~english
   * \~japanese
   *   \brief 受け取った情報
   */
  struct{
    /**
     * \~english
     * \~japanese
     *   \brief ネットワークプロトコルレスポンス
     */
    HttpResponse *response;
    /**
     * \~english
     * \~japanese
     *   \brief 受信ステータス
     */
    S32  status;
    /**
     * \~english
     * \~japanese
     *   \brief 受信ステータス
     */
    const Char  *contentType;
    /**
     * \~english
     * \~japanese
     *   \brief データ
     */
    const void  *data;
    /**
     * \~english
     * \~japanese
     *   \brief データサイズ
     */
    U32  size;
  }response_;

  /**
   * \~english
   * \~japanese
   *   \brief クラス内 new/delete オペレータ定義
   */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("IRequest");
  /**
   * \~english
   * \~japanese
   *   \brief コピー禁止定義
   */
  ADK_NON_COPYABLE_CLASS(IRequest);
};


/* class inline implementation */
ADK_INLINE const Char *IRequest::GetUrl() const {
  return url_;
}
ADK_INLINE S32 IRequest::GetStatus() const {
  return response_.status;
}
ADK_INLINE const Byte *IRequest::GetResponseData() const {
  return (const Byte*)response_.data;
}
ADK_INLINE U32 IRequest::GetResponseDataSize() const {
  return response_.size;
}


ADK_END_NAMESPACE


#endif  /* ADK_I_REQUEST_H_ */


