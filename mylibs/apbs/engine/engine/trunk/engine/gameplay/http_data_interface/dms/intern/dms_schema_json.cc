/**
 * \author tomonori tanaka
 * \file dms_schema_json.cpp
 * \~english
 * \~japanese
 *  \brief DMSで扱うスキーマ実装部 for JSON
 *  \section adksct_json_format ADKスキーマ独自のJSONフォーマット対応
 *    \subsection adksubsct_null_object null オブジェクトへの対応
 *      レスポンスデータにおいて null
 * が設定されていた値に対しては以下の対応を行う
 *      |型|対応方法|
 *      |:-|:-------|
 *      |配列|配列要素を 0 個とし、配列要素が存在しないものとする。|
 *      |その他|スキーマの情報を使う|
 *  \section adksct_binary_format JSONフォーマット >
 * バイナリフォーマットによる問題点
 *    現在64bit変数があるときに対応しきれていないシチュエーションがある。\n
 *    構造体Aの中に構造体Bが存在したとする。\n
 *    この構造体Bの先頭が64bit変数であった場合にアライメントがずれる可能性がある。
 *    \code
 *    typedef struct {
 *      U64 value;
 *    }TStructB;
 *    typedef struct {
 *      S32 value;
 *      TStructB b; // 構造体Bの先頭が64bit変数のため8byteアラインでないと壊れる
 *    }TStructA;
 *    \endcode
 */

#include <gameplay/http_data_interface/dms/intern/dms_schema_json.h>

#include <core/debug/logging.h>
#include <core/library/lib_string.h>
#include <debug_profiling/error/result_types.h>
#include <debug_profiling/error/err_dms_result_types.h>
#include <gameplay/http_data_interface/dms/intern/dms_value.h>
#include <gameplay/http_data_interface/system/system.h>

ADK_BEGIN_NAMESPACE

/*
 * ParseResponseData() メソッドで受け取るデータをコピーする
 * 現状レスポンスして受け取ったデータは
 * パース完了後に破棄されるのでコピーしておく
 */
#define WITH_COPY_DATA

/*
 * jsonのリクエストデータの先頭に'json='をつける
 * 臨時的な対応。
 * 関連修正は以下のマクロで検索。
 * dongho.yoo.
 */
#define JSON_REQUEST_HEADER_WITH_JSON_EQUL

/* key types */
#define KEY_HEADER ("header")
#define KEY_REQUEST ("request")
#define KEY_RESPONSE ("response")
/* common response */
#define KEY_COMMON ("common")

/* value managment functionally */
typedef struct {
  const Char *str;
} TStringPtr;
typedef struct {
  void *child;
} TParent;

class JsonBinary {
 public:
  JsonBinary(void *src) : src_(src), cur_((Byte *)src) {}
  virtual ~JsonBinary() {}

  void WriteS32(S32 value) {
    S32 *dst = (S32 *)cur_;
    *dst = value;
    cur_ += sizeof(S32);
  }
  void WriteU32(U32 value) {
    U32 *dst = (U32 *)cur_;
    *dst = value;
    cur_ += sizeof(U32);
  }
  void WriteS64(S64 value) {
    S64 *dst = (S64 *)cur_;
    *dst = value;
    cur_ += sizeof(S64);
  }
  void WriteU64(U64 value) {
    U64 *dst = (U64 *)cur_;
    *dst = value;
    cur_ += sizeof(U64);
  }
  void WriteDouble(Double value) {
    Double *dst = (Double *)cur_;
    *dst = value;
    cur_ += sizeof(Double);
  }
  void WriteBool(Bool value) {
    Bool *dst = (Bool *)cur_;
    *dst = value;
    cur_ += sizeof(Bool);
  }
  void WriteString(const Char *value) {
    TStringPtr *dst = (TStringPtr *)cur_;
    dst->str = value;
    cur_ += sizeof(const Char *);
  }
  void WriteChild(void *child) {
    TParent *parent = (TParent *)cur_;
    parent->child = (void *)child;
    cur_ += sizeof(void *);
  }
  void Alignment(size_t alignment) {
    size_t sub_alignment = CheckAlignment((size_t)cur_, alignment);
    if (sub_alignment != 0) {
      cur_ += sub_alignment;
    }
  }

 private:
  size_t CheckAlignment(size_t now_alignment, size_t alignment) {
    size_t ret = 0;
    size_t check_alignment = ADK_ALIGN(now_alignment, alignment);
    ret = check_alignment - now_alignment;
    return ret;
  }

  void *src_;
  Byte *cur_;

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("JsonBinary");
  ADK_NON_COPYABLE_CLASS(JsonBinary);
};
/**
 * \~english
 * \~japanese
 *  \brief 値情報
 */
typedef struct TValue {
  TValue *next, *prev;
  DMS_Value value;
  Bool isTop;
} TValue;
static DMS_Value *CreateValue(LIB_TListBase *lb, Bool isTop) {
  TValue *value = new TValue;
  if (value == 0) {
    return 0;
  } else {
    value->next = value->prev = 0;
    value->isTop = isTop;
    LIB_ListAddTail(lb, value);
  }

  return &value->value;
}
static void DisposeAllValue(LIB_TListBase *lb) {
  TValue *next = 0;
  for (TValue *value = (TValue *)lb->first; value; value = next) {
    next = value->next;
    LIB_ListRemLink(lb, value);
    value->value.Terminate();
    delete value, value = 0;
  }
}

/* class implementation ---------*/
DMS_SchemaJson::DMS_SchemaJson() {
  schema_.response.valueCount = 0;
  memset(&schema_.response.values, 0, sizeof(schema_.response.values));
  memset(&response_.binary, 0, sizeof(response_.binary));
  memset(&response_.values, 0, sizeof(response_.values));
}
DMS_SchemaJson::~DMS_SchemaJson() { schema_.jdoc.Release(); }

Result DMS_SchemaJson::Parse(void *bin, U32 size) {
  Result ret = ResultSuccess;

  ret = schema_.jdoc.Parse(static_cast<Char *>(bin), false, 0);
  if (ret.IsSuccess() == false) {
    goto errend;
  }

  if (schema_.jdoc.Size() < 2) { /* header+response */
    ret =
        ADK_RESULT_PROGRAM(ResultUnknown, kDmsComponent, ERR_DMS_JSON_PARSE, 0);
    goto errend;
  }

  ret = ParseHeader();
  if (ret.IsSuccess() == false) {
    DEBUG_LOG("SchemaJson::Parse(): Non header.\n");
  }

  ret = ParseRequest();
  if (ret.IsSuccess() == false) {
    DEBUG_LOG("SchemaJson::Parse(): Non header.\n");
  }

  ret = ParseResponse();
  if (ret.IsSuccess() == false) {
    goto errend;
  }

  return ret;

errend:
  schema_.jdoc.Release();

  return ret;
}
void DMS_SchemaJson::DisposeSchema() {
  response_.jdoc.Release();
  DisposeAllValue(&schema_.response.values);
  schema_.jdoc.Release();
}
Result DMS_SchemaJson::ParseResponseData(const Byte *data, U32 size) {
  Result ret = ResultSuccess;

  /*
   * 外で削除されなかった時の為にココで一旦前回レスポンスした情報を削除する。
   * 本来こういう事はしたくないのだが、アサートしても良いのだろうか。。。
   */
  DisposeResponseData();

#ifdef WITH_COPY_DATA
  response_.binary.addr = ADK_Malloc(size, "Response Binary");
  if (response_.binary.addr == 0) {
    ret = ADK_RESULT(ResultOutOfResource, true, false, kDmsComponent, 0, 0);
  } else {
    memcpy(response_.binary.addr, data, size);
    response_.binary.size = size + 1;
  }
#else
  response_.binary.addr = (void *)data;
  response_.binary.size = size;
#endif /* WITH_COPY_DATA */
  if (ret.IsSuccess()) {
    ret = ParseResponseJsonData(response_.binary.addr, response_.binary.size);
  }

  DoRequestComplete();

  return ret;
}
Result DMS_SchemaJson::ParseResponseJsonData(void *json, U32 size) {
  Result ret = response_.jdoc.Parse((Char *)json, false, 0);
  if (ret.IsSuccess() == false) {
    response_.jdoc.Release();
  } else {
    /*
     * まず先にレスポンスデータ内に"common"があるか調べパースする
     * 次に、元となるレスポンスJSONドキュメントを元に、
     * 今回レスポンスした情報を作成する。
     * もしレスポンスしたデータ内に存在しないデータがあったらスキーマの情報を入れる
     * もしレスポンスしたデータ内にスキーマに存在しない情報があったら無視する
     */
    const LIB_TJsonMember &common = response_.jdoc[KEY_COMMON];
    if (common.data.IsValid()) {
      System *system = System::GetSystem();
      DMS_SchemaJson *common_schema =
          (DMS_SchemaJson *)system->GetCommonResponseSchema();
      common_schema->ParseCommonResponse((LIB_JsonValue &)response_.jdoc,
                                         common);
    }

    const LIB_JsonDocument &base = schema_.jdoc;
    const LIB_TJsonMember &base_member = base[KEY_RESPONSE];
    const LIB_JsonValue &base_data = base_member.data;
    ADK_ASSERT(base_data.IsValid());
    const U32 base_size = base_data.Size();
    if (base_size > 0) {
      for (S32 index = 0; index < base_size; ++index) {
        const LIB_TJsonMember &member = base_data.GetMember(index);
        ParseResponseDataInternal((LIB_JsonValue &)response_.jdoc, member, 0,
                                  true);
      }
    }
  }

  return ret;
}
Result DMS_SchemaJson::DisposeResponseData() {
  DisposeAllValue(&response_.values);
#ifdef WITH_COPY_DATA
  if (response_.binary.addr) {
    ADK_Free(response_.binary.addr), response_.binary.addr = 0;
  }
#endif
  response_.jdoc.Release();
  RequestUnComplete();

  return ResultSuccess;
}
Bool DMS_SchemaJson::GetBoolean(const Char *key, Result *result) const {
  const DMS_Value *value = FindValue(key, result);
  if (value == 0) {
    return false;
  } else {
    return value->GetBoolean();
  }
}
const Char *DMS_SchemaJson::GetString(const Char *key, Result *result) const {
  const DMS_Value *value = FindValue(key, result);
  if (value == 0) {
    return 0;
  } else {
    return value->GetString();
  }
}
S32 DMS_SchemaJson::GetS32(const Char *key, Result *result) const {
  const DMS_Value *value = FindValue(key, result);
  if (value == 0) {
    return 0;
  } else {
    return value->GetS32();
  }
}
U32 DMS_SchemaJson::GetU32(const Char *key, Result *result) const {
  const DMS_Value *value = FindValue(key, result);
  if (value == 0) {
    return 0;
  } else {
    return value->GetU32();
  }
}
S64 DMS_SchemaJson::GetS64(const Char *key, Result *result) const {
  const DMS_Value *value = FindValue(key, result);
  if (value == 0) {
    return 0;
  } else {
    return value->GetS64();
  }
}
U64 DMS_SchemaJson::GetU64(const Char *key, Result *result) const {
  const DMS_Value *value = FindValue(key, result);
  if (value == 0) {
    return 0;
  } else {
    return value->GetU64();
  }
}
Double DMS_SchemaJson::GetDouble(const Char *key, Result *result) const {
  const DMS_Value *value = FindValue(key, result);
  if (value == 0) {
    return 0;
  } else {
    return value->GetDouble();
  }
}
const void *DMS_SchemaJson::GetObject(const Char *key, Result *result) const {
  const DMS_Value *value = FindValue(key, result);
  if (value == 0) {
    return 0;
  } else {
    return value->GetObject();
  }
}
S32 DMS_SchemaJson::GetArrayCount(const Char *key, Result *result) const {
  const DMS_Value *value = FindValue(key, result);
  if (value == 0) {
    return 0;
  } else {
    return value->GetCount();
  }
}
const void *DMS_SchemaJson::GetArray(const Char *key, Result *result) const {
  const DMS_Value *value = FindValue(key, result);
  if (value == 0) {
    return 0;
  } else {
    return (void *)((intptr_t)value->GetArray());
  }
}
/* schema management functionally */
const Char *DMS_SchemaJson::GetAccessUrl() const {
  const LIB_JsonDocument &jdoc = schema_.jdoc;
  const LIB_JsonValue &header = jdoc["header"].data;
  ADK_ASSERT_MSG(header.IsValid(), "Not found \"header\"");
  const LIB_JsonValue &url = header["url"].data;
  ADK_ASSERT_MSG(url.IsValid(), "Not found \"url\"");

  return url.GetString();
}
/* parse management functinoally */
Result DMS_SchemaJson::ParseHeader() {
  const LIB_JsonDocument &jdoc = schema_.jdoc;
  const LIB_JsonValue &header = jdoc["header"].data;
  if (header.IsValid() == false) {
    return ADK_RESULT(ResultUnknown, true, false, kDmsComponent, 0, 0);
  }

  return ResultSuccess;
}
Result DMS_SchemaJson::ParseRequest() {
  const LIB_JsonDocument &jdoc = schema_.jdoc;
  const LIB_JsonValue &request = jdoc["request"].data;
  if (request.IsValid() == false) {
    return ADK_RESULT(ResultUnknown, true, false, kDmsComponent, 0, 0);
  }

  return ResultSuccess;
}
Result DMS_SchemaJson::ParseResponse() {
  Result ret = ResultSuccess;
  const LIB_JsonDocument &jdoc = schema_.jdoc;
  const LIB_JsonValue &response = jdoc["response"].data;
  if (response.IsValid() == false) {
    return ADK_RESULT(ResultUnknown, true, false, kDmsComponent, 0, 0);
  }
  const U32 responseSize = response.Size();
  if (responseSize > 0) {
    schema_.response.valueCount = responseSize;

    for (S32 index = 0; index < schema_.response.valueCount; ++index) {
      const LIB_TJsonMember &member = response.GetMember(index);
      ret =
          CreateResponseDataInternal(&schema_.response.values, 0, member, true);
      if (ret.IsSuccess() == false) {
        break;
      }
    }
    if (ret.IsSuccess() == false) {
      DisposeAllValue(&schema_.response.values);
    }
  }

  return ResultSuccess;
}
Result DMS_SchemaJson::ParseCommonResponse(const LIB_JsonValue &response,
                                           const LIB_TJsonMember &common) {
  Result ret = ResultSuccess;
  const LIB_TJsonMember &base_response = schema_.jdoc[KEY_RESPONSE];
  if (base_response.data.IsValid() == false) {
    CRITICAL_LOG("Invalid base response");
    ret = ADK_RESULT_PROGRAM(
        ResultUnknown, kDmsComponent,
        ERR_DMS_JSON_PARSE_COMMON_RESONSE_INVALID_BASE_RESPONSE, 0);
  } else {
    const LIB_TJsonMember &base_common = base_response.data[KEY_COMMON];
    if (base_common.data.IsValid() == false) {
      CRITICAL_LOG("Invalid base response");
      ret = ADK_RESULT_PROGRAM(
          ResultUnknown, kDmsComponent,
          ERR_DMS_JSON_PARSE_COMMON_RESONSE_INVALID_BASE_COMMON, 0);
    } else {
      Result ret = ParseResponseDataInternal(response, base_common, 0, true);
    }
  }

  return ret;
}
/* request managment functionally */
void *DMS_SchemaJson::CreateRequestData(void *common_header,
                                        void *request_data) {
  void *ret = 0;

  if (request_data == 0) {
    ret = CreateRequestDataForCommonHeader(common_header);
  } else {
    ret = CreateRequestDataForCommonHeaderAndRequestData(common_header,
                                                         request_data);
  }
  if (ret != 0) {
    INFO_LOG("Request binary info = \n");
#if 0
    ADK_BINARY_LOG((Char *)ret);
#endif
  }

  return ret;
}
void *DMS_SchemaJson::CreateRequestDataForCommonHeader(void *common_header) {
  const U32 common_header_length =
      static_cast<U32>(strlen(static_cast<const Char *>(common_header)));
#ifdef JSON_REQUEST_HEADER_WITH_JSON_EQUL
  const U32 data_length =
      5 + 1 + common_header_length + 1; /* 'json=' + '{' + 'common_header' + '}' */
#else
  const U32 data_length =
      1 + common_header_length + 1; /* '{' + 'common_header' + '}' */
#endif
  Byte *ret =
      (Byte *)ADK_Malloc(data_length + 1, "CreateRequestData"); /* + '\0' */
  if (ret != 0) {
#ifdef JSON_REQUEST_HEADER_WITH_JSON_EQUL
    sprintf((Char *)ret, "json={%s}", static_cast<Char *>(common_header));
#else
    sprintf((Char *)ret, "{%s}", static_cast<Char *>(common_header));
#endif
    ret[data_length] = '\0';
  }

  return ret;
}
void *DMS_SchemaJson::CreateRequestDataForCommonHeaderAndRequestData(
    void *common_header, void *request_data) {
  const LIB_JsonDocument &jdoc = schema_.jdoc;
  const LIB_JsonValue &request = jdoc["request"].data;
  ADK_ASSERT_MSG(request.IsValid(), "Not found \"request\"\n");

  const S32 member_count = request.Size();
  Byte *pos = static_cast<Byte *>(request_data);
  size_t alignment = 0;
  LIB_JsonDocument reqdata;
  reqdata.BeginObject();
  for (S32 index = 0; index < member_count; ++index) {
    const LIB_TJsonMember &member = request.GetMember(index);
    CreateRequestDataInternal((void **)&pos, &alignment, reqdata, member);
  }
  reqdata.EndObject(request.Size());
  reqdata.Assign();

  Char *jsonstr = 0;
  U32 jsonlen = 0;
  Char *ret = 0;
  Result result = reqdata.CreateStringWithoutTopObject(&jsonstr, &jsonlen);
  if (result.IsSuccess()) {
    const U32 common_header_length =
        static_cast<U32>(strlen((Char *)common_header));
    
#ifdef JSON_REQUEST_HEADER_WITH_JSON_EQUL
    const U32 request_data_length = 5 + 1 + common_header_length + 1 + jsonlen +
                                    1; /* 'json=' + '{' + 'common_header' + ',' + '}' */
#else
    const U32 request_data_length = 1 + common_header_length + 1 + jsonlen +
                                    1; /* '{' + 'common_header' + ',' + '}' */
#endif
    ret = (Char *)ADK_Malloc(request_data_length + 1,
                             "CreateRequestData"); /* + '\0' */
#ifdef JSON_REQUEST_HEADER_WITH_JSON_EQUL
    sprintf(ret, "json={%s,%s}", (Char *)common_header, jsonstr);
#else
    sprintf(ret, "{%s,%s}", (Char *)common_header, jsonstr);
#endif
    ADK_Free(jsonstr), jsonstr = 0;
  }
  reqdata.Release();

  return ret;
}
void DMS_SchemaJson::CreateRequestDataInternal(void **pos, size_t *alignment,
                                               LIB_JsonDocument &jdoc,
                                               const LIB_TJsonMember &member) {
  const LIB_JsonValue &name = member.name, &data = member.data;
  const LIB_TJsonType type = data.GetType();
  size_t type_binary_size = data.TypeToBinarySize();

  size_t sub_alignment = CheckAlignment(*alignment, type_binary_size);
  if (sub_alignment != 0) {
    *pos = (void *)((intptr_t) * pos + sub_alignment);
    *alignment += sub_alignment;
  }

  switch (type) {
    case LIB_kJsonNumber:
    case LIB_kJsonBool:
    case LIB_kJsonTrue:
    case LIB_kJsonFalse:
    case LIB_kJsonS32:
    case LIB_kJsonU32:
    case LIB_kJsonS64:
    case LIB_kJsonU64:
    case LIB_kJsonDouble: {
      CreateRequestDataInternalNumber(pos, name, data, jdoc);
      break;
    }
    case LIB_kJsonString: {
      CreateRequestDataInternalString(pos, name, jdoc);
      break;
    }
    case LIB_kJsonObject: {
      CreateRequestDataInternalObject(pos, name, data, alignment, jdoc);
      break;
    }
    case LIB_kJsonArray: {
      CreateRequestDataInternalArray(pos, name, data, alignment, jdoc);
      break;
    }
    case LIB_kJsonBinary:
    case LIB_kJsonNull:
    case LIB_kJsonDocument:
    case LIB_kJsonValue: {
      ADK_ASSERT_MSG(0, "Invalid type.\n");
      break;
    }
  }

  *alignment += type_binary_size;
}
void DMS_SchemaJson::CreateRequestDataInternalNumber(void **pos,
                                                     const LIB_JsonValue &name,
                                                     const LIB_JsonValue &data,
                                                     LIB_JsonDocument &jdoc) {
  switch (data.GetType()) {
    case LIB_kJsonBool:
    case LIB_kJsonTrue:
    case LIB_kJsonFalse: {
      const Bool *boolean = static_cast<Bool *>(*pos);
      jdoc.AddBool(name.GetString(), *boolean);
      break;
    }
    case LIB_kJsonS32: {
      const S32 *s32 = static_cast<S32 *>(*pos);
      jdoc.AddS32(name.GetString(), *s32);
      break;
    }
    case LIB_kJsonU32: {
      const U32 *u32 = static_cast<U32 *>(*pos);
      jdoc.AddU32(name.GetString(), *u32);
      break;
    }
    case LIB_kJsonS64: {
      const S64 *s64 = static_cast<S64 *>(*pos);
      jdoc.AddS64(name.GetString(), *s64);
      break;
    }
    case LIB_kJsonU64: {
      const U64 *u64 = static_cast<U64 *>(*pos);
      jdoc.AddU64(name.GetString(), *u64);
      break;
    }
    case LIB_kJsonDouble: {
      const Double *dbl = static_cast<Double *>(*pos);
      jdoc.AddDouble(name.GetString(), *dbl);
      break;
    }
    default: { ADK_ASSERT_MSG(0, "Invalid access.\n"); }
  }
  size_t binary_alignment = 0;
  const U32 binsize = data.GetTypeBinarySize(&binary_alignment);
  *pos = (void *)((intptr_t) * pos + binsize);
}
void DMS_SchemaJson::CreateRequestDataInternalString(void **pos,
                                                     const LIB_JsonValue &name,
                                                     LIB_JsonDocument &jdoc) {
  const Char *str = (const Char *)(*(reinterpret_cast<void **>(*pos)));
  jdoc.AddString(name.GetString(), str);
  *pos = (void *)((intptr_t) * pos + 4); /* アドレスサイズだけずらす */
}
void DMS_SchemaJson::CreateRequestDataInternalObject(void **pos,
                                                     const LIB_JsonValue &name,
                                                     const LIB_JsonValue &data,
                                                     size_t *alignment,
                                                     LIB_JsonDocument &jdoc) {
  const U32 size = data.Size();
  jdoc.BeginObject(name.GetString());
  for (U32 index = 0; index < size; ++index) {
    CreateRequestDataInternal(pos, alignment, jdoc, data.GetMember(index));
  }
  jdoc.EndObject(size);
}
void DMS_SchemaJson::CreateRequestDataInternalArray(void **pos,
                                                    const LIB_JsonValue &name,
                                                    const LIB_JsonValue &data,
                                                    size_t *alignment,
                                                    LIB_JsonDocument &jdoc) {
  const S32 max = *(static_cast<const S32 *>(*pos));
  *pos =
      (void *)((intptr_t) * pos +
               4); /* テーブルカウンタの参照が終わったのでアドレスサイズだけずらす
                      */

  void *array_addr = (U32 *)(*(reinterpret_cast<void **>(*pos)));
  jdoc.BeginArray(name.GetString());
  if (data[0].IsObject()) {
    for (S32 count = 0; count < max; ++count) {
      const LIB_JsonValue &value =
          data[0]; /* <- オブジェクトの内部を見るだけなので先頭のみでいい */
      size_t array_alignment = 0;
      jdoc.BeginObject();
      for (U32 array = 0; array < value.Size(); ++array) {
        CreateRequestDataInternal((void **)&array_addr, &array_alignment, jdoc,
                                  value.GetMember(array));
      }
      jdoc.EndObject(value.Size());
      const size_t check_alignment = CheckAlignment(array_alignment, 0x8);
      if (check_alignment != 0) {
        array_addr =
            (void *)((intptr_t)array_addr +
                     check_alignment); /* テーブルの参照が終わったのでアドレスサイズだけずらす
                                          */
      }
    }
  } else {
    for (S32 count = 0; count < max; ++count) {
      switch (data[0].GetType()) {
        case LIB_kJsonNumber:
        case LIB_kJsonBool:
        case LIB_kJsonTrue:
        case LIB_kJsonFalse:
        case LIB_kJsonS32:
        case LIB_kJsonU32:
        case LIB_kJsonS64:
        case LIB_kJsonU64:
        case LIB_kJsonDouble: {
          switch (data[0].GetType()) {
            case LIB_kJsonBool:
            case LIB_kJsonTrue:
            case LIB_kJsonFalse: {
              const Bool *boolean = static_cast<Bool *>(array_addr);
              jdoc.AddBool(*boolean);
              break;
            }
            case LIB_kJsonS32: {
              const S32 *s32 = static_cast<S32 *>(array_addr);
              jdoc.AddS32(*s32);
              break;
            }
            case LIB_kJsonU32: {
              const U32 *u32 = static_cast<U32 *>(array_addr);
              jdoc.AddU32(*u32);
              break;
            }
            case LIB_kJsonS64: {
              const S64 *s64 = static_cast<S64 *>(array_addr);
              jdoc.AddS64(*s64);
              break;
            }
            case LIB_kJsonU64: {
              const U64 *u64 = static_cast<U64 *>(array_addr);
              jdoc.AddU64(*u64);
              break;
            }
            case LIB_kJsonDouble: {
              const Double *dbl = static_cast<Double *>(array_addr);
              jdoc.AddDouble(*dbl);
              break;
            }
            default: { ADK_ASSERT_MSG(0, "Invalid access.\n"); }
          }
          size_t binary_alignment = 0;
          const U32 binsize = data[0].GetTypeBinarySize(&binary_alignment);
          array_addr = (void *)((intptr_t)array_addr + binsize);
          break;
        }
        default: { ADK_ASSERT_MSG(0, "Invalid access.\n"); }
      }
    }
  }
  jdoc.EndArray((U32)max);
  *pos = (void *)((intptr_t) * pos +
                  4); /* テーブルの参照が終わったのでアドレスサイズだけずらす */
}
#if 0
void DMS_SchemaJson::CreateRequestDataInternalArrayForObject(
    void **pos,
    const LIB_JsonValue &name,
    const LIB_JsonValue &data,
    void **array_addr,
    size_t *alignment,
    LIB_JsonDocument &jdoc) {
}
void DMS_SchemaJson::CreateRequestDataInternalArrayForNonObject(
    void **pos,
    const LIB_JsonValue &name,
    const LIB_JsonValue &data,
    void *array_addr,
    size_t *alignment,
    LIB_JsonDocument &jdoc) {
}
#endif
void DMS_SchemaJson::DisposeRequestData(void *request_data) {}
Result DMS_SchemaJson::CreateResponseDataInternal(LIB_TListBase *lb,
                                                  void *parent,
                                                  const LIB_TJsonMember &member,
                                                  Bool isTop) {
  Result ret = ResultSuccess;
  const LIB_JsonValue &name = member.name, &data = member.data;

  DMS_Value *value = CreateValue(lb, isTop);
  if (value == 0) {
    ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kDmsComponent,
                             ERR_DMS_JSON_CREATE_RESPONSE_DATA_INTERNAL, 0);
    goto errend;
  }

  if (data.IsString()) {
    ret = value->InitializeString(name.GetString(), name.GetStringLength(),
                                  data.GetString(), data.GetStringLength());
    if (ret.IsSuccess() == false) {
      goto errend;
    }
  } else if (data.IsNumber()) {
    if (data.IsS32()) {
      ret = value->InitializeS32(name.GetString(), name.GetStringLength(),
                                 data.GetS32());
      if (ret.IsSuccess() == false) {
        goto errend;
      }
    } else if (data.IsU32()) {
      ret = value->InitializeU32(name.GetString(), name.GetStringLength(),
                                 data.GetU32());
      if (ret.IsSuccess() == false) {
        goto errend;
      }
    } else if (data.IsS64()) {
      ret = value->InitializeS64(name.GetString(), name.GetStringLength(),
                                 data.GetS64());
      if (ret.IsSuccess() == false) {
        goto errend;
      }
    } else if (data.IsU64()) {
      ret = value->InitializeU64(name.GetString(), name.GetStringLength(),
                                 data.GetU64());
      if (ret.IsSuccess() == false) {
        goto errend;
      }
    } else if (data.IsDouble()) {
      ret = value->InitializeDouble(name.GetString(), name.GetStringLength(),
                                    data.GetDouble());
      if (ret.IsSuccess() == false) {
        goto errend;
      }
    }
  } else if (data.IsBool()) {
    ret = value->InitializeBool(name.GetString(), name.GetStringLength(),
                                data.GetBool());
    if (ret.IsSuccess() == false) {
      goto errend;
    }
  } else if (data.IsArray()) {
    size_t binary_alignment = 0;
    ret = value->InitializeArray(name.GetString(), name.GetStringLength(),
                                 data.Size(),
                                 data.GetTypeBinarySize(&binary_alignment));
    if (ret.IsSuccess() == false) {
      goto errend;
    }
    Byte *bin = (Byte *)value->GetArray();
    if (parent) {
      TParent *prnt = (TParent *)parent;
      prnt->child = bin;
    }

    size_t alignment = 0;
    for (S32 index = 0; index < data.Size(); ++index) {
      const LIB_JsonValue &jvalue = data[index];
      size_t type_binary_size = data.TypeToBinarySize();

      if (jvalue.IsObject()) {
        for (S32 pos = 0; pos < jvalue.Size(); ++pos) {
          const LIB_TJsonMember &submember = jvalue.GetMember(pos);
          ret = CreateResponseDataValue((void **)&bin, lb, 0, submember, false,
                                        &alignment);
        }
        size_t sub_alignment = CheckAlignment(alignment, 0x08);
        if (sub_alignment != 0) {
          *bin += sub_alignment;
          alignment += sub_alignment;
        }
      } else if (jvalue.IsBool()) {
        Bool *d = (Bool *)bin;
        *d = jvalue.GetBool();
        bin += sizeof(Bool);
      } else if (jvalue.IsNumber()) {
        if (jvalue.IsS32()) {
          S32 *d = (S32 *)bin;
          *d = jvalue.GetS32();
          bin += sizeof(S32);
        } else if (jvalue.IsU32()) {
          U32 *d = (U32 *)bin;
          *d = jvalue.GetU32();
          bin += sizeof(U32);
        } else if (jvalue.IsS64()) {
          S64 *d = (S64 *)bin;
          *d = jvalue.GetS64();
          bin += sizeof(S64);
        } else if (jvalue.IsU64()) {
          U64 *d = (U64 *)bin;
          *d = jvalue.GetU64();
          bin += sizeof(U64);
        }
      } else {
        ADK_ASSERT_MSG(0, "Invalid jvalue format.\n");
      }
      if (data.IsObject() == 0) {
        alignment += type_binary_size;
      }
    }
  } else if (data.IsObject()) {
    size_t binary_alignment = 0;
    ret = value->InitializeObject(name.GetString(), name.GetStringLength(),
                                  data.GetTypeBinarySize(&binary_alignment));
    if (ret.IsSuccess() == false) {
      goto errend;
    }
    Byte *bin = (Byte *)value->GetObject();
    if (parent) {
      TParent *prnt = (TParent *)parent;
      prnt->child = bin;
    }

    size_t alignment = 0;
    for (S32 pos = 0; pos < data.Size(); ++pos) {
      const LIB_TJsonMember &submember = data.GetMember(pos);
      ret = CreateResponseDataValue((void **)&bin, lb, 0, submember, false,
                                    &alignment);
    }
  } else {
    ADK_ASSERT_MSG(0, "Invalid type.\n");
  }

errend:

  return ret;
}
Result DMS_SchemaJson::CreateResponseDataValue(void **pos, LIB_TListBase *lb,
                                               void *parent,
                                               const LIB_TJsonMember &member,
                                               Bool isTop, size_t *alignment) {
  Result ret = ResultSuccess;
  const LIB_JsonValue &data = member.data;
  size_t type_binary_size = data.TypeToBinarySize();
  size_t sub_alignment = CheckAlignment(*alignment, type_binary_size);

  if (sub_alignment != 0) {
    *pos = (void *)((intptr_t) * pos + sub_alignment);
    *alignment += sub_alignment;
  }

  if (data.IsString()) {
    TStringPtr *str = static_cast<TStringPtr *>(*pos);
    str->str = data.GetString();
    *pos = (void *)((intptr_t) * pos + sizeof(void *));
  } else if (data.IsNumber()) {
    if (data.IsS32()) {
      S32 *i32 = static_cast<S32 *>(*pos);
      *i32 = data.GetS32();
      *pos = (void *)((intptr_t) * pos + sizeof(S32));
    } else if (data.IsU32()) {
      U32 *u32 = static_cast<U32 *>(*pos);
      *u32 = data.GetU32();
      *pos = (void *)((intptr_t) * pos + sizeof(U32));
    } else if (data.IsS64()) {
      S64 *i64 = static_cast<S64 *>(*pos);
      *i64 = data.GetS64();
      *pos = (void *)((intptr_t) * pos + sizeof(S64));
    } else if (data.IsU64()) {
      U64 *u64 = static_cast<U64 *>(*pos);
      *u64 = data.GetU64();
      *pos = (void *)((intptr_t) * pos + sizeof(U64));
    } else if (data.IsDouble()) {
      Double *d = static_cast<Double *>(*pos);
      *d = data.GetDouble();
      *pos = (void *)((intptr_t) * pos + sizeof(Double));
    }
  } else if (data.IsBool()) {
    Bool *d = static_cast<Bool *>(*pos);
    *d = data.GetBool();
    *pos = (void *)((intptr_t) * pos + sizeof(Bool));
  } else if (data.IsArray()) {
    S32 *count = static_cast<S32 *>(*pos);
    *count = data.Size();
    *pos = (void *)((intptr_t) * pos + sizeof(S32));
    *alignment += 4;

    void *addr = static_cast<void *>(*pos);
    ret = CreateResponseDataInternal(lb, addr, member, false);
    if (ret.IsSuccess() == false) {
      goto errend;
    }
    *pos = (void *)((intptr_t) * pos + sizeof(void *));
  } else if (data.IsObject()) {
    for (S32 index = 0; index < data.Size(); ++index) {
      const LIB_TJsonMember &submember = data.GetMember(index);
      ret = CreateResponseDataValue(pos, lb, 0, submember, false, alignment);
      if (ret.IsSuccess() == false) {
        goto errend;
      }
    }
    sub_alignment = CheckAlignment(*alignment, 0x08);
    if (sub_alignment != 0) {
      *pos = (void *)((intptr_t) * pos + sub_alignment);
      alignment += sub_alignment;
    }
  } else {
    ADK_ASSERT_MSG(0, "Invalid type.\n");
  }

  if (data.IsObject() == false) {
    *alignment += type_binary_size;
  }

errend:
  return ret;
}
Result DMS_SchemaJson::ParseResponseDataInternal2(const LIB_JsonValue &response,
                                                  const LIB_TJsonMember &base,
                                                  void *parent, Bool isTop) {
  Result ret = ResultSuccess;

  /*
   * スキーマの値が response に存在するかで処理を返る
   * 存在しない場合、何もせずスキーマ値を使用する。
   * 存在する場合、新たにレスポンスのバイナリを作成する
   */
  const LIB_TJsonMember &response_member = response[base.name.GetString()];
  if (response_member.name.IsValid() == false) {
    /* 存在しなかったので終了 */
  } else {
    DMS_Value *value = CreateValue(&response_.values, isTop);
    if (value == 0) {
      ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kDmsComponent,
                               ERR_DMS_JSON_PARSE_RESPONSE_DATA_INTERNAL, 0);
      goto errend;
    } else {
      if (base.data.IsString()) {
        JsonValueTypeEqual(response_member.data, base.data,
                           base.name.GetString(), true);
        ret = value->InitializeString(response_member.name.GetString(),
                                      response_member.name.GetStringLength(),
                                      response_member.data.GetString(),
                                      response_member.data.GetStringLength());
        if (ret.IsSuccess() == false) {
          goto errend;
        }
      } else if (response_member.data.IsNumber()) {
        if (base.data.IsS32()) {
          ret = value->InitializeS32(response_member.name.GetString(),
                                     response_member.name.GetStringLength(),
                                     response_member.data.GetS32());
          if (ret.IsSuccess() == false) {
            goto errend;
          }
        } else if (base.data.IsU32()) {
          ret = value->InitializeU32(response_member.name.GetString(),
                                     response_member.name.GetStringLength(),
                                     response_member.data.GetU32());
          if (ret.IsSuccess() == false) {
            goto errend;
          }
        } else if (base.data.IsS64()) {
          ret = value->InitializeS64(response_member.name.GetString(),
                                     response_member.name.GetStringLength(),
                                     response_member.data.GetS64());
          if (ret.IsSuccess() == false) {
            goto errend;
          }
        } else if (base.data.IsU64()) {
          ret = value->InitializeU64(response_member.name.GetString(),
                                     response_member.name.GetStringLength(),
                                     response_member.data.GetU64());
          if (ret.IsSuccess() == false) {
            goto errend;
          }
        } else if (base.data.IsDouble()) {
          ret = value->InitializeDouble(response_member.name.GetString(),
                                        response_member.name.GetStringLength(),
                                        response_member.data.GetDouble());
          if (ret.IsSuccess() == false) {
            goto errend;
          }
        }
      } else if (response_member.data.IsBool()) {
        JsonValueTypeEqual(response_member.data, base.data,
                           base.name.GetString(), true);
        ret = value->InitializeBool(response_member.name.GetString(),
                                    response_member.name.GetStringLength(),
                                    response_member.data.GetBool());
        if (ret.IsSuccess() == false) {
          goto errend;
        }
      } else if (response_member.data.IsArray()) {
        size_t binary_alignment = 0;
        ret = value->InitializeArray(
            response_member.name.GetString(),
            response_member.name.GetStringLength(), response_member.data.Size(),
            base.data.GetTypeBinarySize(&binary_alignment));
        if (ret.IsSuccess() == false) {
          goto errend;
        }
        void *bin = (Byte *)value->GetArray();
        if (parent) {
          TParent *prnt = (TParent *)parent;
          prnt->child = bin;
        }

        JsonBinary binary(bin);
        const LIB_JsonValue &basevalue = base.data[0];
        if (basevalue.IsObject()) {
          size_t alignment = 0;
          for (S32 index = 0; index < response_member.data.Size(); ++index) {
            const LIB_JsonValue &subdata = response_member.data[index];
            for (S32 pos = 0; pos < basevalue.Size(); ++pos) {
              const LIB_TJsonMember &basemember = basevalue.GetMember(pos);
              const LIB_TJsonMember &sub_value =
                  subdata[basemember.name.GetString()];
              if (sub_value.data.IsValid() == false) {
                ret = ParseResponseDataValue(&binary, 0, sub_value.data,
                                             basemember, basemember.data, false,
                                             true);
              } else {
                ret = ParseResponseDataValue(&binary, 0, sub_value.data,
                                             basemember, sub_value.data, false,
                                             true);
              }
            }
            size_t sub_alignment = CheckAlignment(alignment, 0x08);
            if (sub_alignment != 0) {
              bin = (void *)((intptr_t)bin + sub_alignment);
              alignment += sub_alignment;
            }
          }
        } else {
          for (S32 index = 0; index < response_member.data.Size(); ++index) {
            for (S32 pos = 0; pos < basevalue.Size(); ++pos) {
              const LIB_TJsonMember &basemember = basevalue.GetMember(0);
              const LIB_JsonValue &subdata = response_member.data[index];
              ret = ParseResponseDataValue(&binary, 0, subdata, basemember,
                                           subdata, false, true);
            }
          }
        }
      } else if (response_member.data.IsObject()) {
        size_t binary_alignment = 0;
        ret = value->InitializeObject(
            base.name.GetString(), base.name.GetStringLength(),
            base.data.GetTypeBinarySize(&binary_alignment));
        if (ret.IsSuccess() == false) {
          goto errend;
        }
        Byte *bin = (Byte *)value->GetObject();
        if (parent) {
          TParent *prnt = (TParent *)parent;
          prnt->child = bin;
        }
        /* 元であるスキーマを基準にオブジェクト内部を構築していく */
        JsonBinary binary(bin);
        for (S32 pos = 0; pos < base.data.Size(); ++pos) {
          const LIB_TJsonMember &basemember = base.data.GetMember(pos);
          const LIB_TJsonMember &subrespmbr =
              response_member.data[basemember.name.GetString()];
          if (subrespmbr.name.IsValid() == false) {
            ret =
                ParseResponseDataValue(&binary, 0, subrespmbr.data, basemember,
                                       basemember.data, false, true);
          } else {
            ret =
                ParseResponseDataValue(&binary, 0, subrespmbr.data, basemember,
                                       subrespmbr.data, false, true);
          }
        }
      } else {
        ADK_ASSERT_MSG(0, "Invalid type.\n");
      }
    }
  }

errend:
  return ret;
}
Result DMS_SchemaJson::ParseResponseDataValue(JsonBinary *bin, void *parent,
                                              const LIB_JsonValue &response,
                                              const LIB_TJsonMember &base,
                                              const LIB_JsonValue &write,
                                              Bool isTop, Bool isTypeCheck) {
  Result ret = ResultSuccess;
  size_t alignment_value = base.data.TypeToBinarySize();

  bin->Alignment(alignment_value);

  if (write.IsString()) {
    JsonValueTypeEqual(write, base.data, base.name.GetString(), isTypeCheck);
    bin->WriteString(write.GetString());
  } else if (write.IsNumber()) {
    JsonValueTypeEqual(write, base.data, base.name.GetString(), isTypeCheck);
    WriteResponseDataNumber(bin, base.data, write);
  } else if (write.IsBool()) {
    JsonValueTypeEqual(write, base.data, base.name.GetString(), isTypeCheck);
    bin->WriteBool(write.GetBool());
  } else if (write.IsArray()) {
    JsonValueTypeEqual(write, base.data, base.name.GetString(), isTypeCheck);

    DMS_Value *value = CreateValue(&response_.values, isTop);
    if (value == 0) {
      ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kDmsComponent,
                               ERR_DMS_JSON_PARSE_RESPONSE_DATA_VALUE, 0);
      goto errend;
    }
    size_t binary_alignment = 0;
    ret = value->InitializeArray(
        base.name.GetString(), base.name.GetStringLength(), write.Size(),
        base.data.GetTypeBinarySize(&binary_alignment));
    if (ret.IsSuccess() == false) {
      goto errend;
    }
    bin->WriteS32(write.Size());
    void *value_binary = (void *)value->GetArray();
    bin->WriteChild(value_binary);

    JsonBinary binary(value_binary);
    const LIB_JsonValue &baseobj = base.data[0];
    if (baseobj.IsObject()) {
      for (S32 index = 0; index < write.Size(); ++index) {
        for (S32 mbr = 0; mbr < baseobj.Size(); ++mbr) { /* なぜか０個になる */
          const LIB_TJsonMember &basemember = baseobj.GetMember(mbr);
          const LIB_TJsonMember &submember =
              response[index][basemember.name.GetString()];
          if (submember.name.IsValid() == false) {
            ret = ParseResponseDataValue(&binary, 0, submember.data, basemember,
                                         basemember.data, false, true);
          } else {
            ret = ParseResponseDataValue(&binary, 0, submember.data, basemember,
                                         submember.data, false, true);
          }
        }
      }
    } else if (baseobj.IsBool() || baseobj.IsNumber()) {
      for (S32 index = 0; index < write.Size(); ++index) {
        ret = ParseResponseDataValue(&binary, 0,
                                     response, /* 使用しないので適当な値 */
                                     base, /* 使用しないので適当な値 */
                                     write[index], false, false);
      }
    } else {
      ADK_ASSERT_MSG(0, "Not support string format.\n");
    }
  } else if (write.IsObject()) {
    JsonValueTypeEqual(write, base.data, base.name.GetString(), isTypeCheck);
    for (S32 index = 0; index < base.data.Size(); ++index) {
      const LIB_TJsonMember &basemember = base.data.GetMember(index);
      const LIB_TJsonMember &respmbr = response[basemember.name.GetString()];
      if (respmbr.name.IsValid() == false) {
        ret = ParseResponseDataValue(bin, 0, respmbr.data, basemember,
                                     basemember.data, false, true);
      } else {
        ret = ParseResponseDataValue(bin, 0, respmbr.data, basemember,
                                     respmbr.data, false, true);
      }
      if (ret.IsSuccess() == false) {
        goto errend;
      }
    }
    return ret;
  } else if (write.IsNull()) {
    /*
     * レスポンスデータにおいて null
     * が設定されていた値に対しては以下の対応を行う
     * |型|対応方法|
     * |:-|:-------|
     * |配列|配列要素を 0 個とし、配列要素が存在しないものとする。|
     * |オブジェクト|スキーマの情報を使う|
     * |その他|スキーマの情報を使う|
     */
    if (base.data.IsArray()) {
      /*
       * 配列の要素数を 0 にする。
       * 配列の参照先を null にする。
       */
      bin->WriteS32(0);
      bin->WriteChild(0);
    } else if (base.data.IsObject()) {
      for (S32 index = 0; index < base.data.Size(); ++index) {
        const LIB_TJsonMember &basemember = base.data.GetMember(index);
        ret = ParseResponseDataValue(bin, 0, basemember.data, basemember,
                                     basemember.data, false, true);
        if (ret.IsSuccess() == false) {
          goto errend;
        }
      }
      return ret;
    } else {
      ret = ParseResponseDataValue(bin, parent, response, base, base.data,
                                   false, false);
      if (ret.IsSuccess() == false) {
        goto errend;
      } else {
        return ret;
      }
    }
  } else {
    ADK_ASSERT_MSG(0, "Invalid type.\n");
  }

errend:
  return ret;
}
Result DMS_SchemaJson::ParseResponseDataInternal(const LIB_JsonValue &response,
                                                 const LIB_TJsonMember &base,
                                                 void *parent, Bool isTop) {
  Result ret = ResultSuccess;

  /*
   * スキーマの値がレスポンスに存在するか確かめる
   * オブジェクト内のパラメータが存在しない場合はベースの情報で作成する
   */
  const LIB_TJsonMember &respmbr = response[base.name.GetString()];
  Bool exist = respmbr.name.IsValid() == false ? false : true;
  if (exist == false) {
  } else {
    DMS_Value *value = CreateValue(&response_.values, isTop);
    if (value == 0) {
      ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kDmsComponent,
                               ERR_DMS_JSON_PARSE_RESPONSE_DATA_INTERNAL, 0);
      goto errend;
    }
    if (respmbr.data.IsString()) {
      JsonValueTypeEqual(respmbr.data, base.data, base.name.GetString(), true);
      ret = value->InitializeString(
          respmbr.name.GetString(), respmbr.name.GetStringLength(),
          respmbr.data.GetString(), respmbr.data.GetStringLength());
      if (ret.IsSuccess() == false) {
        goto errend;
      }
    } else if (respmbr.data.IsNumber()) {
      if (base.data.IsS32()) {
        ret = value->InitializeS32(respmbr.name.GetString(),
                                   respmbr.name.GetStringLength(),
                                   respmbr.data.GetS32());
        if (ret.IsSuccess() == false) {
          goto errend;
        }
      } else if (base.data.IsU32()) {
        ret = value->InitializeU32(respmbr.name.GetString(),
                                   respmbr.name.GetStringLength(),
                                   respmbr.data.GetU32());
        if (ret.IsSuccess() == false) {
          goto errend;
        }
      } else if (base.data.IsS64()) {
        ret = value->InitializeS64(respmbr.name.GetString(),
                                   respmbr.name.GetStringLength(),
                                   respmbr.data.GetS64());
        if (ret.IsSuccess() == false) {
          goto errend;
        }
      } else if (base.data.IsU64()) {
        ret = value->InitializeU64(respmbr.name.GetString(),
                                   respmbr.name.GetStringLength(),
                                   respmbr.data.GetU64());
        if (ret.IsSuccess() == false) {
          goto errend;
        }
      } else if (base.data.IsDouble()) {
        ret = value->InitializeDouble(respmbr.name.GetString(),
                                      respmbr.name.GetStringLength(),
                                      respmbr.data.GetDouble());
        if (ret.IsSuccess() == false) {
          goto errend;
        }
      }
    } else if (respmbr.data.IsBool()) {
      JsonValueTypeEqual(respmbr.data, base.data, base.name.GetString(), true);
      ret = value->InitializeBool(respmbr.name.GetString(),
                                  respmbr.name.GetStringLength(),
                                  respmbr.data.GetBool());
      if (ret.IsSuccess() == false) {
        goto errend;
      }
    } else if (respmbr.data.IsArray()) {
      size_t binary_alignment = 0;
      ret = value->InitializeArray(
          respmbr.name.GetString(), respmbr.name.GetStringLength(),
          respmbr.data.Size(), base.data.GetTypeBinarySize(&binary_alignment));
      if (ret.IsSuccess() == false) {
        goto errend;
      }
      void *bin = (Byte *)value->GetArray();
      if (parent) {
        TParent *prnt = (TParent *)parent;
        prnt->child = bin;
      }

      const LIB_JsonValue &basevalue = base.data[0];
      if (basevalue.IsObject()) {
        size_t alignment = 0;
        for (S32 index = 0; index < respmbr.data.Size(); ++index) {
          const LIB_JsonValue &subdata = respmbr.data[index];
          for (S32 pos = 0; pos < basevalue.Size(); ++pos) {
            const LIB_TJsonMember &basemember = basevalue.GetMember(pos);
            const LIB_TJsonMember &sub_value =
                subdata[basemember.name.GetString()];
            if (sub_value.data.IsValid() == false) {
              ret = ParseResponseDataValue((void **)&bin, &alignment, 0,
                                           sub_value.data, basemember,
                                           basemember.data, false, true);
            } else {
              ret = ParseResponseDataValue((void **)&bin, &alignment, 0,
                                           sub_value.data, basemember,
                                           sub_value.data, false, true);
            }
          }
          size_t sub_alignment = CheckAlignment(alignment, 0x08);
          if (sub_alignment != 0) {
            bin = (void *)((intptr_t)bin + sub_alignment);
            alignment += sub_alignment;
          }
        }
      } else {
        size_t alignment = 0;
        for (S32 index = 0; index < respmbr.data.Size(); ++index) {
          for (S32 pos = 0; pos < basevalue.Size(); ++pos) {
            const LIB_TJsonMember &basemember = basevalue.GetMember(0);
            const LIB_JsonValue &subdata = respmbr.data[index];
            ret = ParseResponseDataValue((void **)&bin, &alignment, 0, subdata,
                                         basemember, subdata, false, true);
          }
        }
      }
    } else if (respmbr.data.IsObject()) {
      size_t binary_alignment = 0;
      ret = value->InitializeObject(
          base.name.GetString(), base.name.GetStringLength(),
          base.data.GetTypeBinarySize(&binary_alignment));
      if (ret.IsSuccess() == false) {
        goto errend;
      }
      Byte *bin = (Byte *)value->GetObject();
      if (parent) {
        TParent *prnt = (TParent *)parent;
        prnt->child = bin;
      }
      /* 元であるスキーマを基準にオブジェクト内部を構築していく */
      size_t alignment = 0;
      for (S32 pos = 0; pos < base.data.Size(); ++pos) {
        const LIB_TJsonMember &basemember = base.data.GetMember(pos);
        const LIB_TJsonMember &subrespmbr =
            respmbr.data[basemember.name.GetString()];
        if (subrespmbr.name.IsValid() == false) {
          ret = ParseResponseDataValue((void **)&bin, &alignment, 0,
                                       subrespmbr.data, basemember,
                                       basemember.data, false, true);
        } else {
          ret = ParseResponseDataValue((void **)&bin, &alignment, 0,
                                       subrespmbr.data, basemember,
                                       subrespmbr.data, false, true);
        }
      }
    } else {
      if (respmbr.data.IsNull()) {
        return ResultUnknown;
      }
      ADK_ASSERT_MSG(0, "Invalid type.\n");
    }
  }

errend:
  return ret;
}
Result DMS_SchemaJson::ParseResponseDataValue(void **pos, size_t *alignment,
                                              void *parent,
                                              const LIB_JsonValue &response,
                                              const LIB_TJsonMember &base,
                                              const LIB_JsonValue &write,
                                              Bool isTop, Bool isTypeCheck) {
  Result ret = ResultSuccess;
  size_t type_binary_size = base.data.TypeToBinarySize();

  size_t sub_alignment = CheckAlignment(*alignment, type_binary_size);
  if (sub_alignment != 0) {
    *pos = (void *)((intptr_t) * pos + sub_alignment);
    *alignment += sub_alignment;
  }

  if (write.IsString()) {
    JsonValueTypeEqual(write, base.data, base.name.GetString(), isTypeCheck);
    WriteResponseDataString(pos, write);
  } else if (write.IsNumber()) {
    JsonValueTypeEqual(write, base.data, base.name.GetString(), isTypeCheck);
    WriteResponseDataNumber(pos, base.data, write);
  } else if (write.IsBool()) {
    JsonValueTypeEqual(write, base.data, base.name.GetString(), isTypeCheck);
    WriteResponseDataBool(pos, write);
  } else if (write.IsArray()) {
    JsonValueTypeEqual(write, base.data, base.name.GetString(), isTypeCheck);
    S32 *count = static_cast<S32 *>(*pos);
    *count = write.Size();
    *pos = (void *)((intptr_t) * pos + sizeof(S32));
    *alignment += sizeof(S32);

    DMS_Value *value = CreateValue(&response_.values, isTop);
    if (value == 0) {
      ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kDmsComponent,
                               ERR_DMS_JSON_PARSE_RESPONSE_DATA_VALUE, 0);
      goto errend;
    }
    size_t binary_alignment = 0;
    ret = value->InitializeArray(
        base.name.GetString(), base.name.GetStringLength(), write.Size(),
        base.data.GetTypeBinarySize(&binary_alignment));
    if (ret.IsSuccess() == false) {
      goto errend;
    }
    void *bin = (void *)value->GetArray();
    TParent *addr = static_cast<TParent *>(*pos);
    addr->child = (void *)bin;

    const LIB_JsonValue &baseobj = base.data[0];
    if (baseobj.IsObject()) {
      size_t array_alignment = 0;
      for (S32 index = 0; index < write.Size(); ++index) {
        for (S32 mbr = 0; mbr < baseobj.Size(); ++mbr) { /* なぜか０個になる */
          const LIB_TJsonMember &basemember = baseobj.GetMember(mbr);
          const LIB_TJsonMember &submember =
              response[index][basemember.name.GetString()];
          if (submember.name.IsValid() == false) {
            ret = ParseResponseDataValue((void **)&bin, &array_alignment, 0,
                                         submember.data, basemember,
                                         basemember.data, false, true);
          } else {
            ret = ParseResponseDataValue((void **)&bin, &array_alignment, 0,
                                         submember.data, basemember,
                                         submember.data, false, true);
          }
        }
        /* テーブル１要素のオブジェクトを入れ終わった後のパディングをチェックする
         */
        size_t sub_alignment = CheckAlignment(array_alignment, 0x08);
        if (sub_alignment != 0) {
          bin = (void *)((intptr_t)bin + sub_alignment);
          array_alignment += sub_alignment;
        }
      }
    } else if (baseobj.IsBool() || baseobj.IsNumber()) {
      size_t array_alignment = 0;
      for (S32 index = 0; index < write.Size(); ++index) {
        ret = ParseResponseDataValue((void **)&bin, &array_alignment, 0,
                                     response, /* 使用しないので適当な値 */
                                     base, /* 使用しないので適当な値 */
                                     write[index], false, false);
      }
    } else {
      ADK_ASSERT_MSG(0, "Not support string format.\n");
    }
    *pos = (void *)((intptr_t) * pos + sizeof(void *));
  } else if (write.IsObject()) {
    JsonValueTypeEqual(write, base.data, base.name.GetString(), isTypeCheck);
    for (S32 index = 0; index < base.data.Size(); ++index) {
      const LIB_TJsonMember &basemember = base.data.GetMember(index);
      const LIB_TJsonMember &respmbr = response[basemember.name.GetString()];
      if (respmbr.name.IsValid() == false) {
        ret = ParseResponseDataValue(pos, alignment, 0, respmbr.data,
                                     basemember, basemember.data, false, true);
      } else {
        ret = ParseResponseDataValue(pos, alignment, 0, respmbr.data,
                                     basemember, respmbr.data, false, true);
      }
      if (ret.IsSuccess() == false) {
        goto errend;
      }
    }
    return ret;
  } else if (write.IsNull()) {
    /*
     * レスポンスデータにおいて null
     * が設定されていた値に対しては以下の対応を行う
     * |型|対応方法|
     * |:-|:-------|
     * |配列|配列要素を 0 個とし、配列要素が存在しないものとする。|
     * |オブジェクト|スキーマの情報を使う|
     * |その他|スキーマの情報を使う|
     */
    if (base.data.IsArray()) {
      /*
       * 配列の要素数を 0 にする。
       * 配列の参照先を null にする。
       */
      S32 *count = static_cast<S32 *>(*pos);
      *count = 0;
      *pos = (void *)((intptr_t) * pos + sizeof(S32));
      *alignment += sizeof(S32);
      TParent *addr = static_cast<TParent *>(*pos);
      addr->child = 0;
      *pos = (void *)((intptr_t) * pos + sizeof(void *));
    } else if (base.data.IsObject()) {
      for (S32 index = 0; index < base.data.Size(); ++index) {
        const LIB_TJsonMember &basemember = base.data.GetMember(index);
        ret = ParseResponseDataValue(pos, alignment, 0, basemember.data,
                                     basemember, basemember.data, false, true);
        if (ret.IsSuccess() == false) {
          goto errend;
        }
      }
      return ret;
    } else {
      ret = ParseResponseDataValue(pos, alignment, parent, response, base,
                                   base.data, false, false);
      if (ret.IsSuccess() == false) {
        goto errend;
      } else {
        return ret;
      }
    }
  } else {
    ADK_ASSERT_MSG(0, "Invalid type.\n");
  }

  *alignment += type_binary_size;

errend:
  return ret;
}
Result DMS_SchemaJson::WriteResponseDataString(void **pos,
                                               const LIB_JsonValue &write) {
  TStringPtr *str = static_cast<TStringPtr *>(*pos);
  str->str = write.GetString();
  *pos = (void *)((intptr_t) * pos + sizeof(void *));
  return ResultSuccess;
}
Result DMS_SchemaJson::WriteResponseDataNumber(JsonBinary *bin,
                                               const LIB_JsonValue &base,
                                               const LIB_JsonValue &write) {
  if (base.IsS32()) {
    bin->WriteS32(write.GetS32());
  } else if (base.IsU32()) {
    bin->WriteU32(write.GetU32());
  } else if (base.IsS64()) {
    bin->WriteS64(write.GetS64());
  } else if (base.IsU64()) {
    bin->WriteS64(write.GetU64());
  } else if (base.IsDouble()) {
    bin->WriteDouble(write.GetDouble());
  }
  return ResultSuccess;
}
Result DMS_SchemaJson::WriteResponseDataNumber(void **pos,
                                               const LIB_JsonValue &base,
                                               const LIB_JsonValue &write) {
  if (base.IsS32()) {
    S32 *i32 = static_cast<S32 *>(*pos);
    *i32 = write.GetS32();
    *pos = (void *)((intptr_t) * pos + sizeof(S32));
  } else if (base.IsU32()) {
    U32 *u32 = static_cast<U32 *>(*pos);
    *u32 = write.GetU32();
    *pos = (void *)((intptr_t) * pos + sizeof(U32));
  } else if (base.IsS64()) {
    S64 *i64 = static_cast<S64 *>(*pos);
    *i64 = write.GetS64();
    *pos = (void *)((intptr_t) * pos + sizeof(S64));
  } else if (base.IsU64()) {
    U64 *u64 = static_cast<U64 *>(*pos);
    *u64 = write.GetU64();
    *pos = (void *)((intptr_t) * pos + sizeof(U64));
  } else if (base.IsDouble()) {
    Double *d = static_cast<Double *>(*pos);
    *d = write.GetDouble();
    *pos = (void *)((intptr_t) * pos + sizeof(Double));
  }
  return ResultSuccess;
}
Result DMS_SchemaJson::WriteResponseDataBool(void **pos,
                                             const LIB_JsonValue &write) {
  Bool *b = static_cast<Bool *>(*pos);
  *b = write.GetBool();
  *pos = (void *)((intptr_t) * pos + sizeof(Bool));
  return ResultSuccess;
}
Bool DMS_SchemaJson::JsonValueTypeEqual(const LIB_JsonValue &value1,
                                        const LIB_JsonValue &value2,
                                        const Char *name, Bool is_assertion) {
  /*
   * タイプが等しいかチェックする
   * 異なるタイプだったとしても数値同士なら等しいとする
   */
  if (value1.GetType() != value2.GetType()) {
    if (value1.IsNumber() && value2.IsNumber()) {
      return true;
    }
    if (is_assertion != false) {
      printf("Json value name = %s\n", name == 0 ? "none name" : name);
      ADK_ASSERT_MSG(0, "Detected a format different from the schema.\n");
    }
  }

  return false;
}
size_t DMS_SchemaJson::CheckAlignment(size_t now_alignment, size_t alignment) {
  size_t ret = 0;
  size_t check_alignment = ADK_ALIGN(now_alignment, alignment);
  ret = check_alignment - now_alignment;
  return ret;
}

/* value management functionally */
const DMS_Value *DMS_SchemaJson::FindValue(const LIB_TListBase *lb,
                                           const Char *key) const {
  const DMS_Value *ret = 0;
  if (key == 0) {
    ret = 0;
  } else {
    const U32 keylen = LIB_strlen(key);

    for (const TValue *value = (const TValue *)lb->first; value;
         value = value->next) {
      if (value->isTop == false || keylen != value->value.GetKeyLength() ||
          memcmp(key, value->value.GetKey(), keylen * sizeof(Char)) != 0) {
        continue;
      }
      ret = &value->value;
      break;
    }
  }

  return ret;
}
const DMS_Value *DMS_SchemaJson::FindValue(const Char *key,
                                           Result *result) const {
  const DMS_Value *value = FindValue(&response_.values, key);
  if (value == 0) {
    value = FindValue(&schema_.response.values, key);
  }
  if (result != 0) {
    *result = value == 0 ? ResultInvalidVariable : ResultSuccess;
  }

  return value;
}

ADK_END_NAMESPACE

