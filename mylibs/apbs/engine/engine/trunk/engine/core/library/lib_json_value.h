/**
 * \file lib_json_value.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief JSON値定義部
 */

#ifndef LIB_JSON_VALUE_H_
#define LIB_JSON_VALUE_H_

#include <core/debug/debug_assert.h>
#include <core/library/lib_json_types.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <debug_profiling/error/result.h>

#if 0
/**
 * パース時の文字列をコピーさせる際のプリプロセッサ
 */
#define LIB_JSON_COPY_STRING
#endif

ADK_BEGIN_NAMESPACE

/* forward declaration */
typedef struct LIB_TJsonMember LIB_TJsonMember;
class LIB_JsonDocument;

/* class definition */
/**
 * \~english
 * \~japanese
 *   \brief JSONで使われる値を表すクラス\n
 */
class LIB_JsonValue {
 public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値をnullとして作成します
   */
  LIB_JsonValue();
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値タイプを指定して作成します
   */
  LIB_JsonValue(LIB_TJsonType type);
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値をbooleanとして作成します
   *  \param[in] b
   */
  LIB_JsonValue(bool b);
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値を文字列として作成します
   *  \param[in] str 文字列
   *  \param[in] length 文字列長
   *  \param[in] isStrDup 文字列を複製するかのフラグ
   */
  LIB_JsonValue(const Char *str, U32 length, Bool isStrDup = false);
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値をS32として作成します
   *  \param[in] i
   */
  LIB_JsonValue(S32 i);
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値をU32として作成します
   *  \param[in] u
   */
  LIB_JsonValue(U32 u);
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値をS64として作成します
   *  \param[in] i
   */
  LIB_JsonValue(S64 i);
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値をU64として作成します
   *  \param[in] u
   */
  LIB_JsonValue(U64 u);
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値をDoubleとして作成します
   *  \param[in] d
   */
  LIB_JsonValue(Double d);
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値をバイナリとして作成します
   *  \param[in] b
   *  \param[in] size
   */
  LIB_JsonValue(const void *b, U32 size);
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値を \ref LIB_JsonDocument として作成します
   *  \param[in] jdoc \ref LIB_JsonDocument
   */
  LIB_JsonValue(const LIB_JsonDocument *jdoc);
  /**
   * \~english
   * \~japanese
   *  \brief 本クラスの値を \ref LIB_JsonValue として作成します
   *  \param[in] jval \ref LIB_JsonValue
   */
  LIB_JsonValue(const LIB_JsonValue *jval);
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~LIB_JsonValue();

  /* value management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値が不正なものかを返します
   *  \return 不正かのフラグ。以下の値が返ります
   *  \retval true  値が不正だったときに返ります
   *  \retval false  値が不正ではないときに返ります
   */
  ADK_INLINE Bool IsValid() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値がnullかを返します
   *  \return nullかのフラグ。以下の値が返ります
   *  \retval true  値がnullだったときに返ります
   *  \retval false  値がnullではないときに返ります
   */
  Bool IsNull() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値がfalseかを返します
   *  \return fasleかのフラグ。以下の値が返ります
   *  \retval true  値がfalseだったときに返ります
   *  \retval false  値がfalseではないときに返ります
   */
  Bool IsFalse() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値がtrueかを返します
   *  \return trueかのフラグ。以下の値が返ります
   *  \retval true  値がtrueだったときに返ります
   *  \retval false  値がtrueではないときに返ります
   */
  Bool IsTrue() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値がboolかを返します
   *  \return boolかのフラグ。以下の値が返ります
   *  \retval true  値がboolだったときに返ります
   *  \retval false  値がboolではないときに返ります
   */
  Bool IsBool() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスの持つ値をbooleanとして返します
   *  \return boolean
   */
  Bool GetBool() const;
  /**
   * \brief クラスの持つ値Boolとして取得します
   * \param[out] Bool Boolの格納先
   * \return 取得の成功判定。以下の値が返ります
   * \retval true 取得に成功したときに返ります
   * \retval false 取得に失敗したときに返ります
   */
  Bool GetBool(Bool &out) const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値がオブジェクトかを返します
   *  \return オブジェクトかのフラグ。以下の値が返ります
   *  \retval true  値がオブジェクトだったときに返ります
   *  \retval false  値がオブジェクトではないときに返ります
   */
  Bool IsObject() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値が配列かを返します
   *  \return 配列かのフラグ。以下の値が返ります
   *  \retval true  値が配列だったときに返ります
   *  \retval false  値が配列ではないときに返ります
   */
  Bool IsArray() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値が配列であったときの先頭要素を返します
   *  \return 先頭要素 \ref LIB_JsonValue
   */
  const LIB_JsonValue *BeginArray() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値が配列であったときの終端要素を返します
   *  \return 先頭要素 \ref LIB_JsonValue
   */
  const LIB_JsonValue *EndArray() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値が配列であったときの要素数を返します
   *  \return 要素数
   */
  U32 Size() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値が数値かを返します
   *  \return 数値かのフラグ。以下の値が返ります
   *  \retval true 値が数値だったときに返ります
   *  \retval false 値が数値ではないときに返ります
   */
  Bool IsNumber() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ数値を任意の型で取得します
   *  \param[out] dst 数値の格納先
   *  \param[in] type キャストしたい \ref LIB_TJsonType を設定します。(数値のみ)
   *  \return 取得の成功判定。以下の値が返ります
   *  \retval ResultSuccess  取得に成功したときに返ります
   *  \retval ResultInvalidVariable  設定された引数が不正だったときに返ります
   */
  Result GetNumber(void *dst, LIB_TJsonType type) const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値がS32かを返します
   *  \return S32かのフラグ。以下の値が返ります
   *  \retval true  値がS32だったときに返ります
   *  \retval false  値がS32ではないときに返ります
   */
  Bool IsS32() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスの持つ値をS32として返します
   *  \return S32
   */
  S32 GetS32() const;
  /**
   * \brief クラスの持つ値S32として取得します
   * \param[out] S32 S32の格納先
   * \return 取得の成功判定。以下の値が返ります
   * \retval true 取得に成功したときに返ります
   * \retval false 取得に失敗したときに返ります
   */
  Bool GetS32(S32 &out) const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値がU32かを返します
   *  \return U32かのフラグ。以下の値が返ります
   *  \retval true  値がU32だったときに返ります
   *  \retval false  値がU32ではないときに返ります
   */
  Bool IsU32() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスの持つ値をU32として返します
   *  \return U32
   */
  U32 GetU32() const;
  /**
   * \brief クラスの持つ値U32として取得します
   * \param[out] U32 U32の格納先
   * \return 取得の成功判定。以下の値が返ります
   * \retval true 取得に成功したときに返ります
   * \retval false 取得に失敗したときに返ります
   */
  Bool GetU32(U32 &out) const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値がS64かを返します
   *  \return S64かのフラグ。以下の値が返ります
   *  \retval true  値がS64だったときに返ります
   *  \retval false  値がS64ではないときに返ります
   */
  Bool IsS64() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスの持つ値をS64として返します
   *  \return S64
   */
  S64 GetS64() const;
  /**
   * \brief クラスの持つ値S64として取得します
   * \param[out] S64 S64の格納先
   * \return 取得の成功判定。以下の値が返ります
   * \retval true 取得に成功したときに返ります
   * \retval false 取得に失敗したときに返ります
   */
  Bool GetS64(S64 &out) const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値がU64かを返します
   *  \return U64かのフラグ。以下の値が返ります
   *  \retval true  値がU64だったときに返ります
   *  \retval false  値がU64ではないときに返ります
   */
  Bool IsU64() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスの持つ値をU64として返します
   *  \return U64
   */
  U64 GetU64() const;
  /**
   * \brief クラスの持つ値U64として取得します
   * \param[out] U64 U64の格納先
   * \return 取得の成功判定。以下の値が返ります
   * \retval true 取得に成功したときに返ります
   * \retval false 取得に失敗したときに返ります
   */
  Bool GetU64(U64 &out) const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値がdoubleかを返します
   *  \return doubleかのフラグ。以下の値が返ります
   *  \retval true 値がdoubleだったときに返ります
   *  \retval false 値がdoubleではないときに返ります
   */
  Bool IsDouble() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスの持つ値をDoubleとして返します
   *  \return Double
   */
  Double GetDouble() const;
  /**
   * \brief クラスの持つ値Doubleとして取得します
   * \param[out] Double Doubleの格納先
   * \return 取得の成功判定。以下の値が返ります
   * \retval true 取得に成功したときに返ります
   * \retval false 取得に失敗したときに返ります
   */
  Bool GetDouble(Double &out) const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値が文字列かを返します
   *  \return 文字列かのフラグ。以下の値が返ります
   *  \retval true 値が文字列だったときに返ります
   *  \retval false 値が文字列ではないときに返ります
   */
  Bool IsString() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスの持つ値を文字列として返します
   *  \return 文字列
   */
  const Char *GetString() const;
  /**
   * \brief クラスの持つ値を文字列として取得します
   * \param[in] out_length 格納先の最大長
   * \param[out] out 文字列の格納先
   * \return 取得の成功判定。以下の値が返ります
   * \retval true 取得に成功したときに返ります
   * \retval false 取得に失敗したときに返ります
   */
  Bool GetString(S32 out_length, char *out) const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスの持つ値が文字列だったときの文字列長を返します
   *  \return 文字列長
   */
  U32 GetStringLength() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値がバイナリかを返します
   *  \return バイナリかのフラグ。以下の値が返ります
   *  \retval true  値がバイナリだったときに返ります
   *  \retval false  値がバイナリではないときに返ります
   */
  Bool IsBinary() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスの持つ値がバイナリだったときのアドレスを返します
   *  \return バイナリの先頭アドレス
   */
  const void *GetBinary() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値が \ref LIB_JsonDocument かを返します
   *  \return \ref LIB_JsonDocument かのフラグ。以下の値が返ります
   *  \retval true  値が \ref LIB_JsonDocument だったときに返ります
   *  \retval false  値が \ref LIB_JsonDocument ではないときに返ります
   */
  Bool IsDocument() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスの持つ値がドキュメントだったときのアドレスを返します
   *  \return \ref LIB_JsonDocument
   */
  const LIB_JsonDocument *GetDocument() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスが持つ値が \ref LIB_JsonValue 値かを返します
   *  \return \ref LIB_JsonValue かのフラグ。以下の値が返ります
   *  \retval true  値が \ref LIB_JsonValue だったときに返ります
   *  \retval false  値が \ref LIB_JsonValue ではないときに返ります
   */
  ADK_INLINE Bool IsValue() const;
  /**
   * \~english
   * \~japanese
   *  \brief クラスの持つ値がドキュメントだったときのアドレスを返します
   *  \return \ref LIB_JsonDocument
   */
  ADK_INLINE const LIB_JsonValue *GetValue() const;
  /**
   * \~english
   * \~japanese
   *  \brief 値の持つタイプを返します
   */
  LIB_TJsonType GetType() const;
  /**
   * \~english
   * \~japanese
   *  \brief 値のバイナリサイズを返します。
   *  \param[in] alignment 内部でアライメント値を計算する為に必要なバッファ
   *  \attention このメソッドは値が S32/U32/S64/U64/Double/Bool
   * のときのみ有効です
   */
  U32 GetTypeBinarySize(size_t *alignment) const;
  /**
   * \~english
   * \~japanese
   *  \brief 値の型サイズを返します
   *  \return サイズ
   */
  size_t TypeToBinarySize() const;
  /**
   * \~english
   * \~japanese
   *  \brief 名前から該当する値を探して返します
   *  \return 値
   */
  LIB_TJsonMember &operator[](const Char *name);
  /**
   * \~english
   * \~japanese
   *  \brief 名前から該当する値を探して返します
   *  \return 値
   */
  const LIB_TJsonMember &operator[](const Char *name) const;
  /**
   * \~english
   * \~japanese
   *  \brief 名前から該当する値を探して返します
   *  \return 値
   */
  LIB_JsonValue &operator[](S32 index);
  /**
   * \~english
   * \~japanese
   *  \brief 名前から該当する値を探して返します
   *  \return 値
   */
  const LIB_JsonValue &operator[](const S32 index) const;
  /**
   * \~english
   * \~japanese
   *  \brief 指定のメンバーを返します
   *  \param[in] index 0 <= index < \ref Size()
   *  \return \ref LIB_TJsonMember
   */
  const LIB_TJsonMember &GetMember(S32 index) const;
  /**
   * \~english
   * \~japanese
   *  \brief 値のコピーを行う
   *  \return
   */
  void Copy(LIB_JsonValue *value);

 protected:
  /* resource managment functionally */
  /**
   * \~english
   * \~japanese
   *  \brief リソースの削除
   *  \param[in] isStrDup 複製した文字列かのフラグ
   */
  void Dispose(Bool isStrDup);

  /* value management functionally */
  friend class LIB_JsonDocument;
  /**
   * \~english
   * \~japanese
   *  \brief メンバーを追加します
   *  \param[in] members メンバーの格納元
   *  \param[in] membercount メンバー数
   */
  void AddMember(LIB_TJsonMember *members, U32 membercount);
  /**
   * \~english
   * \~japanese
   *  \brief メンバーを探して返します
   *  \param[in] name メンバー名
   *  \return \ref LIB_JsonValue
   */
  LIB_TJsonMember *FindMember(const Char *name);
  /**
   * \~english
   * \~japanese
   *  \brief 配列要素を追加します
   *  \param[in] elements 要素の格納元
   *  \param[in] elementcount 要素数
   */
  void AddElements(LIB_JsonValue *elements, U32 elementcount);

  /* value types */
  enum TType {
    kUnknown = 0x0000,
    kNull = 0x0001,
    kBool = 0x0002,
    kTrue = 0x0004,
    kFalse = 0x0008,
    kNumber = 0x0010,
    kS32 = 0x0020,
    kU32 = 0x0040,
    kS64 = 0x0080,
    kU64 = 0x0100,
    kDouble = 0x0200,
    kString = 0x0400,
    kObject = 0x0800,
    kArray = 0x1000,
    kBinary = 0x2000,
    kDocument = 0x4000,
    kValue = 0x8000
  };
  typedef struct {
    Char *str;
    U32 length;
  } TString;
  typedef union {
    struct {
      S32 i;
      Byte reserve[4];
    } i;
    struct {
      U32 u;
      Byte reserve[4];
    } u;
    S64 i64;
    U64 u64;
    Double d;
  } TNumber;
  typedef struct {
    LIB_TJsonMember *members;
    U32 size;
  } TObject;
  typedef struct {
    LIB_JsonValue *elements;
    U32 size;
  } TArray;
  typedef struct j {
    const void *binary;
    U32 size;
  } TBinary;
  typedef struct {
    const LIB_JsonDocument *doc;
    U32 reserve;
  } TDocument;
  typedef struct {
    const LIB_JsonValue *value;
    U32 reserve;
  } TValue;
  typedef union {
    TString s;
    TNumber n;
    TObject o;
    TArray a;
    TBinary b;
    TDocument d;
    TValue v;
  } TData;

  /* vars */
  /**
   * \~english
   * \~japanese
   *  \brief 値データ
   */
  TData data_;
  /**
   * \~english
   * \~japanese
   *  \brief データフラグ
   */
  TType type_;

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("LIB_JsonValue");
};

/**
 * \~english
 * \~japanese
 *  \brief JSONフォーマットのパラメータ単位
 */
struct LIB_TJsonMember {
  LIB_JsonValue name;
  LIB_JsonValue data;
};

ADK_END_NAMESPACE

#include <core/library/intern/lib_json_value-inl.h>

#endif /* LIB_JSON_VALUE_H_ */

