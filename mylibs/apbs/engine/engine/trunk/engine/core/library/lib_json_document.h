/**
 * \file lib_json_document.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief JSONドキュメント定義部
 *   \page adkpage_jdoc JSON Document
 *   \tableofcontents
 *   \section adksct_jdoc_summary 1.JSON Document について
 *     \ref LIB_JsonDocument
は、JSONフォーマットのデータを操作する機能を備えたライブラリです
 *   \section adksct_jdoc_parse 2.JSONフォーマットをパースする
 *     パースを行うには \ref LIB_JsonDocument::Parse() メソッドを使用します。
 *     実際のパースの例を以下に記載します。
\code
void func() {
  static const char *kJSON = "{"
                              "  \"bool\":true,"
                              "  \"number\":1234,"
                              "  \"string\":\"name\","
                              "  \"array\":["
                              "    {"
                              "      \"name\":\"abce\","
                              "      \"id\":0"
                              "    },"
                              "    {"
                              "      \"name\":\"fghi\","
                              "      \"id\":1"
                              "    }"
                              "  ]"
                              "}";
  adk::LIB_JsonDocument jdoc;
  adk::Result result;

  result = jdoc.Parse(kJSON);
  if (result.IsSuccess()==false) {
    // パース失敗処理
  } else {
    // パース成功処理
  }
}
\endcode
 *   \section adksct_jdoc_get 3.値を取得する
 *     \subsection adksubsct_jdoc_get_bool 3.1.Booleanを取得する
\code
{
  const adk::LIB_TJsonMember &boolean = jdoc["bool"];
  printf("%d\n",boolean.data.GetBool());
}
\endcode
 *     \subsection adksubsct_jdoc_get_s32 3.2.signed
int(32bit符号付整数)を取得する
\code
{
  const adk::LIB_TJsonMember &number = jdoc["number"];
  printf("%d\n",number.data.GetS32());
}
\endcode
 *     \subsection adksubsct_jdoc_get_string 3.3.文字列を取得する
\code
{
  const adk::LIB_TJsonMember &string = jdoc["string"];
  printf("%s\n",string.data.GetString());
}
\endcode
 *     \subsection adksubsct_jdoc_get_array 3.4.配列を取得する
\code
{
  const adk::LIB_TJsonMember &array = jdoc["array"];
  const int max = array.data.Size();
  int index;
  for (index=0;index<max;++index) {
    const adk::LIB_JsonValue &object = array.data[index];  // 配列内の要素を取得
  }
}
\endcode
 *     \subsection adksubsct_jdoc_get_array 3.5.オブジェクトを取得する
 *       オブジェクトとは配列内の要素毎の固まりなどの事を言います。\n
 *       \ref adksubsct_jdoc_get_array を元に追記したコードを以下に記載します。
\code
{
  const adk::LIB_TJsonMember &array = jdoc["array"];
  const int max = array.data.Size();
  int index;
  for (index=0;index<max;++index) {
    const adk::LIB_JsonValue &object = array.data[index];  // 配列内の要素を取得
    printf("%s / %d\n",
           object["name"].data.GetString(),
           object["id"].data.GetS32());
  }
}
\endcode
 */

#ifndef LIB_JSON_DOCUMENT_H_
#define LIB_JSON_DOCUMENT_H_

#include <stdarg.h>

#include <core/library/lib_json_value.h>
#include <core/library/lib_stack.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <debug_profiling/error/result.h>

ADK_BEGIN_NAMESPACE

/* class definition */
class LIB_JsonReader;
/**
 * \~english
 * \~japanese
 *  \brief JSONドキュメントクラス\n
 *         JSONフォーマットパース後のJSON情報が格納されているクラス\n
 *         使い方については「 \ref adkpage_jdoc 」を参照して下さい
 */
class LIB_JsonDocument : public LIB_JsonValue {
 public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  LIB_JsonDocument();
  /**
   * \~english
   * \~japanese
   * \brief デストラクタ
   */
  virtual ~LIB_JsonDocument();

  /* parse management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief JSON解析処理を行います
   *  \attentino このメソッドは引数 'json' で渡された文字列の値を、
   *             書き換えることに注意して下さい。
   *  \param[in] json JSON文字列
   *  \param[in] isHoldBinary json引数を、このクラスで保持するかを指定します。
   *  \param[in] disposejson バイナリ削除コールバック関数\n
   *                         isHoldBinary に'true'を設定した場合、
   *                         バイナリ削除用のコールバック関数を設定する必要があります。
   *  \return パースの成功判定。以下の値が返ります
   *  \retval ResultSuccess パース処理に成功したときに返ります
   *  \retval ResultOutOfResource システムリソースが不足しているときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されていたときに返ります
   *  \retval ResultUnknown その他、パース処理に失敗したときなどに返ります
   */
  Result Parse(Char *json, Bool isHoldBinary,
               void (*disposeHoldBinary)(void *));
  /**
   * \~english
   * \~japanese
   *  \brief パース処理で作成されたリソースを解放します
   */
  Result Release();

  /* create management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief オブジェクトの開始処理を行います
   *  \attention 名前付きオブジェクトはJSONの先頭では使用できません
   *  \param[in] name
   * オブジェクトの名前(オブジェクトの名前は'null'でも構いません)
   */
  void BeginObject(const Char *name = 0);
  /**
   * \~english
   * \~japanese
   *  \brief オブジェクトの終了処理を行います
   *  \param[in] membercount メンバー数
   */
  void EndObject(U32 membercount);
  /**
   * \~english
   * \~japanese
   *  \brief 配列の開始処理を行います
   *  \attention 名前付き配列はJSONの先頭では使用できません
   *  \param[in] name 配列の名前(配列の名前は'null'でも構いません)
   */
  void BeginArray(const Char *name = 0);
  /**
   * \~english
   * \~japanese
   *  \brief 配列の終了処理を行います
   *  \param[in] elementcount 要素数
   */
  void EndArray(U32 elementcount);
  /**
   * \~english
   * \~japanese
   *  \brief Nullを追加します。
   *  \param[in] name 値の名前
   */
  void AddNull(const Char *name);
  /**
   * \~english
   * \~japanese
   *  \brief Bool を追加します。
   *  \param[in] name 値の名前
   *  \param[in] b
   */
  void AddBool(const Char *name, Bool b);
  /**
   * \~english
   * \~japanese
   *  \brief 文字列を追加します。
   *  \param[in] name 値の名前
   *  \param[in] str 文字列の格納元
   */
  void AddString(const Char *name, const Char *str);
  /**
   * \~english
   * \~japanese
   *  \brief S32を追加します。
   *  \param[in] name 値の名前
   *  \param[in] i
   */
  void AddS32(const Char *name, S32 i);
  /**
   * \~english
   * \~japanese
   *  \brief U32を追加します。
   *  \param[in] name 値の名前
   *  \param[in] u
   */
  void AddU32(const Char *name, U32 u);
  /**
   * \~english
   * \~japanese
   *  \brief S64を追加します。
   *  \param[in] name 値の名前
   *  \param[in] i
   */
  void AddS64(const Char *name, S64 i);
  /**
   * \~english
   * \~japanese
   *  \brief U64を追加します。
   *  \param[in] name 値の名前
   *  \param[in] u
   */
  void AddU64(const Char *name, U64 u);
  /**
   * \~english
   * \~japanese
   *  \brief Doubleを追加します。
   *  \param[in] name 値の名前
   *  \param[in] d
   */
  void AddDouble(const Char *name, Double d);
  /**
   * \~english
   * \~japanese
   *  \brief バイナリ参照を追加します
   *  \param[in] name 値の名前
   *  \param[in] b バイナリへのポインタ
   *  \param[in] size バイナリのサイズ
   */
  void AddBinary(const Char *name, const void *b, U32 size);
  /**
   * \~english
   * \~japanese
   *  \brief \ref LIB_JsonDocument を追加します
   *  \param[in] name 値の名前
   *  \param[in] doc \ref LIB_JsonDocument
   */
  void AddDocument(const Char *name, const LIB_JsonDocument *doc);
  /**
   * \~english
   * \~japanese
   *  \brief \ref LIB_JsonValue を追加します
   *  \param[in] name 値の名前
   *  \param[in] val \ref LIB_JsonValue
   */
  void AddValue(const Char *name, const LIB_JsonValue *val);
  /**
   * \~english
   * \~japanese
   *  \brief Nullを追加します。
   */
  void AddNull();
  /**
   * \~english
   * \~japanese
   *  \brief Bool を追加します。
   *  \param[in] b
   */
  void AddBool(Bool b);
  /**
   * \~english
   * \~japanese
   *  \brief 文字列を追加します。
   *  \param[in] str 文字列の格納元
   *  \param[in] length 格納する文字列長
   */
  void AddString(const Char *str, U32 length, Bool isWriteNull = true);
  /**
   * \~english
   * \~japanese
   *  \brief S32を追加します。
   *  \param[in] i
   */
  void AddS32(S32 i);
  /**
   * \~english
   * \~japanese
   *  \brief U32を追加します。
   *  \param[in] u
   */
  void AddU32(U32 u);
  /**
   * \~english
   * \~japanese
   *  \brief S64を追加します。
   *  \param[in] i
   */
  void AddS64(S64 i);
  /**
   * \~english
   * \~japanese
   *  \brief U64を追加します。
   *  \param[in] u
   */
  void AddU64(U64 u);
  /**
   * \~english
   * \~japanese
   *  \brief Doubleを追加します。
   *  \param[in] d
   */
  void AddDouble(Double d);
  /**
   * \~english
   * \~japanese
   *  \brief バイナリを追加します。
   *  \param[in] b
   *  \param[in] size
   */
  void AddBinary(const void *b, U32 size);
  /**
   * \~english
   * \~japanese
   *  \brief JSONをドキュメントに割り当てる
   */
  void Assign();
  /**
   * \~english
   * \~japanese
   *  \brief JSON文字列を作成します
   *  \param[in] dst 作成したJSON文字列格納先
   *  \param[in] length 作成したJSON文字列長格納先
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval ResultSuccess 作成に成功したときに返ります
   *  \retval ResultOutOfResource システムリソースが不足しているときに返ります
   */
  Result CreateString(Char **dst, U32 *length);
  /**
   * \~english
   * \~japanese
   *  \brief JSON文字列を内部情報のみを作成します\n
   *         JSON文字列として存在する先頭'{'と終端の'}'を省いた文字列を作成します
   *  \param[in] dst 作成したJSON文字列格納先
   *  \param[in] length 作成したJSON文字列長格納先
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval ResultSuccess 作成に成功したときに返ります
   *  \retval ResultOutOfResource システムリソースが不足しているときに返ります
   */
  Result CreateStringWithoutTopObject(Char **dst, U32 *length);

 private:
  /* value management functionally */
  friend class LIB_JsonReader;
  /**
   * \~english
   * \~japanese
   *  \brief オブジェクトの開始処理を行います
   */
  void StartObject();
  /**
   * \~english
   * \~japanese
   *  \brief オブジェクトの終了処理を行います
   *  \param[in] membercount メンバー数
   */
  void EndObject(S32 membercount);
  /**
   * \~english
   * \~japanese
   *  \brief 配列の開始処理を行います
   */
  void StartArray();
  /**
   * \~english
   * \~japanese
   *  \brief 配列の終了処理を行います
   *  \param[in] elementcount 要素数
   */
  void EndArray(S32 elementcount);
  /**
   * \~english
   * \~japanese
   *  \brief \ref LIB_JsonDocument を追加します。\n
   *         これで追加された \ref LIB_JsonDocument は、
   *         このクラスの \ref LIB_JsonDocument::Release() が呼ばれた時に
   *         一緒に解放処理が行われます
   *  \param[in] doc
   */
  void AddDocument(const LIB_JsonDocument *doc);
  /**
   * \~english
   * \~japanese
   *  \brief \ref LIB_JsonValue を追加します。
   *  \param[in] val
   */
  void AddValue(const LIB_JsonValue *val);

  /* parse management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief 発生したエラー情報を設定します
   *  \param[in] msg エラーメッセージ
   *  \param[in] offset エラー発生位置オフセット
   */
  void SetError(const Char *msg, size_t offset);

  /* create management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief JSONを文字列かしたときの文字列長を求めます
   *  \param[out] len 文字列長格納先
   */
  void CreateStringLength(U32 *len, const LIB_JsonValue &value);
  /**
   * \~english
   * \~japanese
   *  \brief JSONを文字列化します
   */
  void CreateString(const LIB_JsonValue &value);
  /**
   * \~english
   * \~japanese
   *  \brief 作成中のJSON文字列に文字列を追加します
   *  \param[in] str 追加する文字列
   */
  void StrCat(const Char *str, ...);
  /**
   * \~english
   * \~japanese
   *  \brief 作成中のJSON文字列に文字列を追加します
   *  \param[in] length 追加文字列長
   *  \param[in] str 追加する文字列
   */
  void StringCat(U32 length, const Char *str);

  /* vars */
  /**
   * \~english
   * \~japanese
   *  \brief バイナリ情報
   */
  struct {
    const Char *json;
    Bool isHoldBinary;
    void (*disposeHoldBinary)(void *);
  } binary_;
  /**
   * \~english
   * \~japanese
   *  \brief
   */
  struct {
    LIB_JsonValue *cur;
    LIB_JsonValue *top;
    LIB_Stack stack;
    Bool isStrDup;
  } value_;
  /**
   * \~english
   * \~japanese
   *  \brief エラー情報
   */
  struct {
    Bool isError;
    const Char *msg;
    U32 offset;
  } error_;

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("LIB_JsonDocument");
};

ADK_END_NAMESPACE

#endif /* LIB_JSON_DOCUMENT_H_ */

