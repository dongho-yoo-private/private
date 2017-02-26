/**
 * \author tomonori tanaka
 * \file dms_schema_json.h
 * \~english
 * \~japanese
 *  \brief DMSで扱うスキーマ定義部 for JSON
 */


#ifndef DMS_SCHEMA_JSON_H_
#define DMS_SCHEMA_JSON_H_


#include <gameplay/http_data_interface/dms/dms_schema.h>
#include <core/library/lib_json_document.h>
#include <core/library/lib_list.h>


ADK_BEGIN_NAMESPACE

class JsonBinary;

/* class definition */
/**
 * \~english
 * \~japanese
 *  \brief DMSスキーマクラス for JSON
 */
class DMS_SchemaJson : public DMS_Schema {
 public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  DMS_SchemaJson();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~DMS_SchemaJson();

  /* schema management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief スキーマのアクセス先URLを返します
   *  \return アクセス先URL文字列
   */
  virtual const Char* GetAccessUrl() const;

 protected:
  /* parse management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief ファイルのパース処理を行います
   *  \param[in] bin パースするバイナリ
   *  \param[in] size バイナリのサイズ
   *  \return パースの成功判定。以下の値が返ります
   *  \retval ResultSuccess  パースに成功したときに返ります
   */
  virtual Result Parse(void *bin,U32 size);
  /**
   * \~english
   * \~japanese
   *  \brief パースしたスキーマ情報を削除します
   */
  virtual void DisposeSchema();

  /* response management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief 受信したデータをパースします
   */
  virtual Result ParseResponseData( const Byte *data, U32 size);
  /**
   * \~english
   * \~japanese
   *  \brief 受信したJSONデータをパースします
   */
  Result ParseResponseJsonData(void *json,U32 size);
  /**
   * \~english
   * \~japanese
   *  \brief 受信したデータを削除します
   */
  virtual Result DisposeResponseData();
  /**
   * \~english
   * \~japanese
   *  \brief Booleanとして値を返します
   *  \param[in] key 取得したい値のキー
   *  \param[out] result 不正な値かのリザルト格納先
   *  \return Bool
   */
  virtual Bool GetBoolean( const Char *key, Result *result) const;
  /**
   * \~english
   * \~japanese
   *  \brief 文字列として値を返します
   *  \param[in] key 取得したい値のキー
   *  \param[out] result 不正な値かのリザルト格納先
   *  \return 文字列
   */
  virtual const Char* GetString( const Char *key, Result *result) const;
  /**
   * \~english
   * \~japanese
   *  \brief S32として値を返します
   *  \param[in] key 取得したい値のキー
   *  \param[out] result 不正な値かのリザルト格納先
   *  \return S32
   */
  virtual S32 GetS32( const Char *key, Result *result) const;
  /**
   * \~english
   * \~japanese
   *  \brief U32として値を返します
   *  \param[in] key 取得したい値のキー
   *  \param[out] result 不正な値かのリザルト格納先
   *  \return U32
   */
  virtual U32 GetU32( const Char *key, Result *result) const;
  /**
   * \~english
   * \~japanese
   *  \brief S64として値を返します
   *  \param[in] key 取得したい値のキー
   *  \param[out] result 不正な値かのリザルト格納先
   *  \return S64
   */
  virtual S64 GetS64( const Char *key, Result *result) const;
  /**
   * \~english
   * \~japanese
   *  \brief U64として値を返します
   *  \param[in] key 取得したい値のキー
   *  \param[out] result 不正な値かのリザルト格納先
   *  \return U64
   */
  virtual U64 GetU64( const Char *key, Result *result) const;
  /**
   * \~english
   * \~japanese
   *  \brief Doubleとして値を返します
   *  \param[in] key 取得したい値のキー
   *  \param[out] result 不正な値かのリザルト格納先
   *  \return Double
   */
  virtual Double GetDouble( const Char *key, Result *result) const;
  /**
   * \~english
   * \~japanese
   *  \brief オブジェクトとして値を返します
   *  \param[in] key 取得したい値のキー
   *  \param[in] result 不正な値かのリザルト格納先
   *  \return オブジェクト
   */
  virtual const void *GetObject(const Char *key, Result *result) const;
  /**
   * \~english
   * \~japanese
   *  \brief 配列の数を返します
   *  \param[in] key 数を取得したい配列のキー
   *  \param[out] result 不正な値かのリザルト格納先
   *  \return 配列数
   */
  virtual S32 GetArrayCount(const Char *key, Result *result) const;
  /**
   * \~english
   * \~japanese
   *  \brief 配列として値を返します
   *  \param[in] key 取得したい値のキー
   *  \param[out] result 不正な値かのリザルト格納先
   *  \return 配列
   */
  virtual const void* GetArray( const Char *key, Result *result) const;

 private:
  /* parse management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief スキーマのヘッダ情報のパース処理を行います
   *  \return パースの成功判定。以下の値が返ります
   *  \retval ResultSuccess  パースに成功したときに返ります
   *  \retval ResultUnknown  パースに失敗したときに返ります
   */
  Result ParseHeader();
  /**
   * \~english
   * \~japanese
   *  \brief スキーマのリクエスト情報のパース処理を行います
   *  \return パースの成功判定。以下の値が返ります
   *  \retval ResultSuccess  パースに成功したときに返ります
   *  \retval ResultUnknown  パースに失敗したときに返ります
   */
  Result ParseRequest();
  /**
   * \~english
   * \~japanese
   *  \brief スキーマのレスポンス情報のパース処理を行います
   *  \return パースの成功判定。以下の値が返ります
   *  \retval ResultSuccess  パースに成功したときに返ります
   *  \retval ResultUnknown  パースに失敗したときに返ります
   */
  Result ParseResponse();
  /**
   * \~english
   * \~japanese
   *  \brief 共通レスポンスのパース処理を行います
   *  \param[in] common 共通レスポンスのオブジェクト
   *  \return パースの成功判定。以下の値が返ります
   *  \retval ResultSuccess  パースに成功したときに返ります
   *  \retval ResultUnknown  パースに失敗したときに返ります
   */
  Result ParseCommonResponse(
      const LIB_JsonValue &response,
      const LIB_TJsonMember &common);

  /* request management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief リクエストに付加するデータを作成して返します
   *  \param[in] commonHeader 共通ヘッダ
   *  \param[in] requestData リクエストデータ
   *  \return 作成したリクエストデータポインタ。以下の値が返ります
   *  \retval null リクエストデータの作成に失敗したときに返ります
   *  \retval other リクエストデータの作成に成功したときに返ります
   */
  virtual void *CreateRequestData(void *commonHeader,void *requestData);
  /**
   * \~english
   * \~japanese
   *  \brief 共通ヘッダのみのリクエストデータを作成して返す。
   *  \param[in] common_header 共通ヘッダへのポインタ
   *  \return 作成したリクエストデータのポインタ。以下の値が返ります
   *  \retval null リクエストデータの作成に失敗したときに返ります
   *  \retval other リクエストデータの作成に成功したときに返ります
   */
  void *CreateRequestDataForCommonHeader(void *common_header);
  /**
   * \~english
   * \~japanese
   *  \brief 共通ヘッダ&リクエストデータのリクエストデータを作成して返す。
   *  \param[in] common_header 共通ヘッダへのポインタ
   *  \param[in] request_data リクエストデータ
   *  \return 作成したリクエストデータのポインタ。以下の値が返ります
   *  \retval null リクエストデータの作成に失敗したときに返ります
   *  \retval other リクエストデータの作成に成功したときに返ります
   */
  void *CreateRequestDataForCommonHeaderAndRequestData(
      void *common_header,
      void *request_data);
  /**
   * \~english
   * \~japanese
   *  \brief リクエストに付加するデータ内部を作成します
   */
  void CreateRequestDataInternal(
      void **pos,
      size_t *alignment,
      LIB_JsonDocument &jdoc,
      const LIB_TJsonMember &member);
  /**
   * \~english
   * \~japanese
   *  \brief リクエスト内部データを作成する for Number
   */
  void CreateRequestDataInternalNumber(
      void **pos,
      const LIB_JsonValue &name,
      const LIB_JsonValue &data,
      LIB_JsonDocument &jdoc);
  /**
   * \~english
   * \~japanese
   *  \brief リクエスト内部データを作成する for String
   */
  void CreateRequestDataInternalString(
      void **pos,
      const LIB_JsonValue &name,
      LIB_JsonDocument &jdoc);
  /**
   * \~english
   * \~japanese
   *  \brief リクエスト内部データを作成する for Object
   */
  void CreateRequestDataInternalObject(
      void **pos,
      const LIB_JsonValue &name,
      const LIB_JsonValue &data,
      size_t *alignment,
      LIB_JsonDocument &jdoc);
  /**
   * \~english
   * \~japanese
   *  \brief リクエスト内部データを作成する for Array
   */
  void CreateRequestDataInternalArray(
      void **pos,
      const LIB_JsonValue &name,
      const LIB_JsonValue &data,
      size_t *alignment,
      LIB_JsonDocument &jdoc);
#if 0
  /**
   * \~english
   * \~japanese
   *  \brief リクエスト内部データを作成する for Object array
   */
  void CreateRequestDataInternalArrayForObject(
      void **pos,
      S32 max,
      const LIB_JsonValue &name,
      const LIB_JsonValue &data,
      void **array_addr,
      size_t *alignment,
      LIB_JsonDocument &jdoc);
  /**
   * \~english
   * \~japanese
   *  \brief リクエスト内部データを作成する for Non object array
   */
  void CreateRequestDataInternalArrayForNonObject(
      void **pos,
      S32 max,
      const LIB_JsonValue &name,
      const LIB_JsonValue &data,
      void **array_addr,
      size_t *alignment,
      LIB_JsonDocument &jdoc);
#endif
  /**
   * \~english
   * \~japanese
   *  \brief リクエストデータの削除を行います
   *  \param[in] requestData リクエストデータ
   */
  virtual void DisposeRequestData(void *requestData);

  /* response management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief 
   */
  Result CreateResponseDataInternal(
      LIB_TListBase *lb,
      void *parent,
      const LIB_TJsonMember &member,
      Bool isTop);
  /**
   * \~english
   * \~japanese
   *  \brief 
   */
  Result CreateResponseDataValue(
      void **pos,
      LIB_TListBase *lb,
      void *parent,
      const LIB_TJsonMember &member,
      Bool isTop,
      size_t *alignment);
  /**
   * \~english
   * \~japanese
   *  \brief 受信したデータをパースします
   */
  Result ParseResponseDataInternal(
      const LIB_JsonValue &resopnse,
      const LIB_TJsonMember &base,
      void *parent,Bool isTop);
  Result ParseResponseDataInternal2(
      const LIB_JsonValue &resopnse,
      const LIB_TJsonMember &base,
      void *parent,Bool isTop);
  /**
   * \~english
   * \~japanese
   *  \brief 受信したデータをパースします
   */
  Result ParseResponseDataValue(
      JsonBinary *bin,
      void *parent,
      const LIB_JsonValue &response,
      const LIB_TJsonMember &base,
      const LIB_JsonValue &write,
      Bool isTop,
      Bool isTypeCheck);
  Result ParseResponseDataValue(
      void **pos,
      size_t *alignment,
      void *parent,
      const LIB_JsonValue &response,
      const LIB_TJsonMember &base,
      const LIB_JsonValue &write,
      Bool isTop,
      Bool isTypeCheck);
  /**
   * \~english
   * \~japanese
   *  \brief 受信したデータを書き込みます
   */
  Result WriteResponseDataString(
    void **pos,
    const LIB_JsonValue &write);
  /**
   * \~english
   * \~japanese
   *  \brief 受信したデータを書き込みます
   */
  Result WriteResponseDataNumber(
      JsonBinary *bin,
      const LIB_JsonValue &base,
      const LIB_JsonValue &write);
  Result WriteResponseDataNumber(
    void **pos,
    const LIB_JsonValue &base,
    const LIB_JsonValue &write);
  /**
   * \~english
   * \~japanese
   *  \brief 受信したデータを書き込みます
   */
  Result WriteResponseDataBool(
    void **pos,
    const LIB_JsonValue &write);
  /**
   * \~english
   * \~japanese
   *  \brief パース中のJSONオブジェクトのタイプが等しいかチェックする
   *  \param[in] value1 比較する値
   *  \param[in] value2 比較する値
   *  \param[in] name 比較する値の名前
   *  \return チェックの判定。以下の値が返ります。
   *  \retval true 等しいときに返ります
   *  \retval false 等しくないときに返ります
   */
  Bool JsonValueTypeEqual(
      const LIB_JsonValue &value1,
      const LIB_JsonValue &value2,
      const Char *name,
      Bool isTypeCheck);
  /**
   * \~english
   * \~japanese
   *  \brief アライメントの差分を調べて差を返します
   * \retval 0 指定されたアライメントのときに返ります
   * \retval 0以上 指定されたアライメントではないので、アライメントの差が返ります
   */
  size_t CheckAlignment(size_t now_alignment,size_t alignment);

  /* value management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief キー名から値オブジェクトを探して返します
   */
  const DMS_Value* FindValue(const LIB_TListBase *lb,const Char *key) const;
  /**
   * \~english
   * \~japanese
   *  \brief キー名から値オブジェクトを探して返します
   */
  const DMS_Value* FindValue(const Char *key,Result *result) const;

  /* vars */
  /**
   * \~english
   * \~japanese
   *  \brief スキーマ情報
   */
  struct {
    /**
     * \~english
     * \~japanese
     *  \brief "*.adkschema"ファイルのJSONドキュメント
     */
    LIB_JsonDocument jdoc;
    /**
     * \~english
     * \~japanese
     *  \brief レスポンス情報
     */
    struct {
      /**
       * \~english
       * \~japanese
       *  \brief レスポンスする値の数
       */
      S32 valueCount;
      /**
       * \~english
       * \~japanese
       *  \brief レスポンスする値本体
       */
      LIB_TListBase values;
    }response;
  }schema_;
  /**
   * \~english
   * \~japanese
   *  \brief レスポンス
   */
  struct {
    /**
     * \~english
     * \~japanese
     *  \brief レスポンスしたバイナリ情報
     */
    struct {
      /**
       * \~english
       * \~japanese
       *  \brief バイナリの先頭アドレス
       */
      void *addr;
      /**
       * \~english
       * \~japanese
       *  \brief バイナリのサイズ
       */
      U32 size;
    }binary;
    /**
     * \~english
     * \~japanese
     *  \brief レスポンスしたJSONドキュメント
     */
    LIB_JsonDocument jdoc;
    /**
     * \~english
     * \~japanese
     *  \brief レスポンスしたデータ
     */
    LIB_TListBase values;
  }response_;

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("DMS_SchemaJson");
  ADK_NON_COPYABLE_CLASS(DMS_SchemaJson);
};


ADK_END_NAMESPACE


#endif  /* DMS_SCHEMA_JSON_H_ */


