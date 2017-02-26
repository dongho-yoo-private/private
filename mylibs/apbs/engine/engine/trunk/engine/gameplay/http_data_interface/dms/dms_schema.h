/**
 * \file dms_schema.h
 * \ingrup adkgrp_dms
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief DMSで扱うスキーマ定義部
 */


#ifndef DMS_SCHEMA_H_
#define DMS_SCHEMA_H_


#include <core/types.h>
#include <gameplay/http_data_interface/dms/dms_schema_types.h>
#include <debug_profiling/error/result.h>
#include <core/memory_allocation/guarded_alloc.h>


ADK_BEGIN_NAMESPACE


/* class definition */
class DMS_Value;
class LIB_JsonDocument;
struct LIB_TJsonMember;
/**
 * \~english
 * \~japanese
 *   \brief DMSスキーマクラス
 */
class DMS_Schema {
public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   */
  DMS_Schema();
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  virtual ~DMS_Schema();

  /* initialize & terminate */
  /**
   * \~english
   * \~japanese
   *   \brief 指定されたファイルからスキーマ情報を初期化します
   *   \return 初期化の成功判定。以下の値が返ります
   *   \retval ResultSuccess    初期化に成功したときに返ります
   *   \retval ResultNotFileExist  スキーマファイルが存在しなかったときに返ります
   */
  Result Initialize( const Char *filename);
  /**
   * \~english
   * \~japanese
   *   \brief 初期化されたスキーマ情報を終了します
   *   \return 削除の成功判定。以下の値が返ります
   *   \retval ResultSuccess      終了に成功したときに返ります
   *   \retval ResultNotInitialized  初期化処理が行われていないときに返ります
   */
  Result Terminate();

  /* request management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief リクエスト情報をリセットします
   */
  void ResetRequest();
  /**
   * \~english
   * \~japanese
   *   \brief スキーマ情報の取得リクエストを行います\n
   *          このメソッドはリクエスト処理が完了するまでブロックします。
   *   \param[in] commonHeader 共通ヘッダ
   *   \param[in] requestParam リクエスト時に付加するデータ
   *   \param[in] errorcallback エラーコールバック
   */
  Result Request(const Char *requestData,void(*errorcallback)(S32,const Char*));
  /**
   * \~english
   * \~japanese
   *   \brief 非同期でスキーマ情報の取得リクエストを行います\n
   *          このメソッドは呼び出したタイミングで requestData をコピーしすぐ返ります
   *   \param[in] commonHeader 共通ヘッダ
   *   \param[in] requestParam リクエスト時に付加するデータ
   *   \param[in] errorcallback エラーコールバック
   */
  Result RequestAsync(const Char *requestData,void(*errorcallback)(S32,const Char*));
  /**
   * \~english
   * \~japanese
   *   \brief リクエスト時に付加するデータを返します
   *   \return データの先頭アドレス
   *   \retval null  データが存在しないときに返ります
   */
  ADK_INLINE const Char *GetRequestData() const;
  /**
   * \~english
   * \~japanese
   *   \brief 値の取得処理を開始します。
   */
  Result DoRequestStart();
  /**
   * \~english
   * \~japanese
   *   \brief 値の取得処理を開始します。
   */
  Result DoRequestStartAsync();
  /**
   * \~english
   * \~japanese
   *   \brief 値の取得を待ちます
   */
  Result DoRequestWait();
  /**
   * \~english
   * \~japanese
   *   \brief 値の取得を待ちます
   */
  Result DoRequestEnd();
  /**
   * \~english
   * \~japanese
   *   \brief 処理を中断する
   */
  void DoRequestInterruption();
  /**
   * \~english
   * \~japanese
   *  \brief リクエストが完了したかを返します
   *  \return 完了フラグ。以下の値が返ります
   *  \retval true  リクエストが完了しているときに返ります
   *  \retval false リクエストが完了していないときに返ります
   */
  ADK_INLINE Bool IsRequestCompleted() const;

  /* response management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief 受信したデータをパースします
   *   \param[in] data レスポンスデータ
   *   \param[in] size レスポンスデータのサイズ
   *   \return パースの成功判定。以下の値が返ります
   *   \retval ResultSuccess  パース処理が成功したときに返ります
   */
  virtual Result ParseResponseData( const Byte *data, U32 size) = 0;
  /**
   * \~english
   * \~japanese
   *   \brief レスポンスした情報を削除します
   */
  virtual Result DisposeResponseData() = 0;
  /**
   * \~english
   * \~japanese
   *   \brief Booleanとして値を返します
   *   \param[in] key 取得したい値のキー
   *   \param[out] result 不正な値かのリザルト格納先
   *   \return Bool
   */
  virtual Bool GetBoolean( const Char *key, Result *result) const = 0;
  /**
   * \~english
   * \~japanese
   *   \brief 文字列として値を返します
   *   \param[in] key 取得したい値のキー
   *   \param[out] result 不正な値かのリザルト格納先
   *   \return 文字列
   */
  virtual const Char* GetString( const Char *key, Result *result) const = 0;
  /**
   * \~english
   * \~japanese
   *   \brief S32として値を返します
   *   \param[in] key 取得したい値のキー
   *   \param[out] result 不正な値かのリザルト格納先
   *   \return S32
   */
  virtual S32 GetS32( const Char *key, Result *result) const = 0;
  /**
   * \~english
   * \~japanese
   *   \brief U32として値を返します
   *   \param[in] key 取得したい値のキー
   *   \param[out] result 不正な値かのリザルト格納先
   *   \return U32
   */
  virtual U32 GetU32( const Char *key, Result *result) const = 0;
  /**
   * \~english
   * \~japanese
   *   \brief S64として値を返します
   *   \param[in] key 取得したい値のキー
   *   \param[out] result 不正な値かのリザルト格納先
   *   \return S64
   */
  virtual S64 GetS64( const Char *key, Result *result) const = 0;
  /**
   * \~english
   * \~japanese
   *   \brief U64として値を返します
   *   \param[in] key 取得したい値のキー
   *   \param[out] result 不正な値かのリザルト格納先
   *   \return U64
   */
  virtual U64 GetU64( const Char *key, Result *result) const = 0;
  /**
   * \~english
   * \~japanese
   *   \brief Doubleとして値を返します
   *   \param[in] key 取得したい値のキー
   *   \param[out] result 不正な値かのリザルト格納先
   *   \return Double
   */
  virtual Double GetDouble( const Char *key, Result *result) const = 0;
  /**
   * \~english
   * \~japanese
   *   \brief オブジェクトとして値を返します
   *   \param[in] key 取得したい値のキー
   *   \param[in] result 不正な値かのリザルト格納先
   *   \return オブジェクト
   */
  virtual const void *GetObject(const Char *key, Result *reslut) const = 0;
  /**
   * \~english
   * \~japanese
   *   \brief 配列の数を返します
   *   \param[in] key 数を取得したい配列のキー
   *   \param[out] result 不正な値かのリザルト格納先
   *   \return 配列数
   */
  virtual S32 GetArrayCount(const Char *key, Result *result) const = 0;
  /**
   * \~english
   * \~japanese
   *   \brief 配列として値を返します
   *   \param[in] key 取得したい値のキー
   *   \param[out] result 不正な値かのリザルト格納先
   *   \return 配列
   */
  virtual const void* GetArray( const Char *key, Result *result) const = 0;

  /* error management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief エラー処理を行う
   *   \param[in] code エラーコード
   *   \param[in] message エラーメッセージ
   */
  void DoError(S32 code, const Char *message);

  /* schema management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief スキーマのアクセス先URLを返します
   *   \return アクセス先URL文字列
   */
  virtual const Char* GetAccessUrl() const = 0;

  /**
   * \~english
   * \~japnaese
   *  \brief 処理が中断されたかを返します
   */
  ADK_INLINE Bool IsInterrupte() const;

protected:
  /* parse management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief ファイルのパース処理を行います
   *   \param[in] bin パースするバイナリ
   *   \param[in] size バイナリのサイズ
   *   \return パースの成功判定。以下の値が返ります
   *   \retval ResultSuccess  パースに成功したときに返ります
   */
  virtual Result Parse( void *bin, U32 size) = 0;
  /**
   * \~english
   * \~japanese
   *  \brief パースしたスキーマ情報を削除します
   */
  virtual void DisposeSchema() = 0;

  /* request management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief リクエストに付加するデータを作成して返します
   *   \param[in] commonHeader 共通ヘッダ
   *   \param[in] requestData リクエストデータ
   *   \return 作成したリクエストデータポインタ。以下の値が返ります
   *   \retval null  リクエストデータの作成に失敗したときに返ります
   *   \retval etc    リクエストデータの作成に成功したときに返ります
   */
  virtual void *CreateRequestData(void *commonHeader,void *requestData) = 0;
  /**
   * \~english
   * \~japanese
   *   \brief リクエストデータの削除を行います
   *   \param[in] requestData リクエストデータ
   */
  virtual void DisposeRequestData(void *requestData) = 0;
  /**
   * \~english
   * \~japanese
   *  \brief リクエスト完了を通知する
   */
  ADK_INLINE void DoRequestComplete();
  /**
   * \~english
   * \~japnaese
   *  \brief リクエストの完了を戻す
   */
  ADK_INLINE void RequestUnComplete();

private:
  /* vars */
  /**
   * \~english
   * \~japanese
   *   \brief スキーマの元となるバイナリ情報
   */
  struct {
    Byte  *addr;
    U32  size;
  }binary_;
  /**
   * \~english
   * \~japanese
   *   \brief リクエスト情報
   */
  struct {
    /**
     * \~english
     * \~japanese
     *   \brief リクエスト時に送信するデータ
     */
    void  *data;
    /**
     * \~english
     * \~japanese
     *   \brief リクエスト時に送信するデータのサイズ
     */
    U32  size;
    /**
     * \~english
     * \~japanese
     *  \brief リクエスト完了フラグ
     */
    Bool  isCompleted;
    /**
     * \~english
     * \~japanese
     *  \brief 中断フラグ
     */
    Bool is_interruption;
  }request_;
  /**
   * \~english
   * \~japanese
   *   \brief エラーコールバック
   */
  void(*errorcallback_)(S32,const Char*);

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("DMS_Schema");
  ADK_NON_COPYABLE_CLASS(DMS_Schema);
};
ADK_INLINE const Char *DMS_Schema::GetRequestData() const
{
  return static_cast<Char*>(request_.data);
}
ADK_INLINE void DMS_Schema::DoRequestComplete() {
  request_.isCompleted = true;
}
ADK_INLINE Bool DMS_Schema::IsRequestCompleted() const {
  return request_.isCompleted;
}
ADK_INLINE void DMS_Schema::RequestUnComplete() {
  request_.isCompleted = false;
  request_.is_interruption = false;
}
ADK_INLINE Bool DMS_Schema::IsInterrupte() const {
  return request_.is_interruption;
}


ADK_END_NAMESPACE


#endif  /* DMS_SCHEMA_H_ */


