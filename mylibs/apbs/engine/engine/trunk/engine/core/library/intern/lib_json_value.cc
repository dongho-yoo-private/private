/**
 * \file lib_json_value.cc
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief JSON値実装部
 */

#include <core/library/lib_json_value.h>

#include <debug_profiling/error/err_library_result_types.h>
#include <core/library/lib_json_document.h>
#include <core/library/lib_string.h>

ADK_BEGIN_NAMESPACE

static LIB_JsonValue nullval;
static LIB_TJsonMember nullmember = {nullval, nullval};

/* class implementation */
/* constructor & destructor */
LIB_JsonValue::LIB_JsonValue() : type_(kUnknown) {}
LIB_JsonValue::LIB_JsonValue(LIB_TJsonType type) {
  static const TType kTypes[17] = {
      kNull, kBool,   kTrue,   kFalse,  kNumber, kS32,    kU32,      kS64,
      kU64,  kDouble, kString, kObject, kArray,  kBinary, kDocument, kValue};

  type_ = kTypes[type];
}
LIB_JsonValue::LIB_JsonValue(bool b) {
  type_ = static_cast<TType>(b == false ? kBool | kFalse : kBool | kTrue);
}
LIB_JsonValue::LIB_JsonValue(const Char *str, U32 length, Bool isStrDup)
    : type_(kString) {
  if (isStrDup != false) {
    data_.s.length = length;
    data_.s.str = static_cast<Char *>(
        ADK_Malloc(data_.s.length + 1, "LIB_JsonValue::String"));
    ADK_ASSERT(data_.s.str);
    memcpy(data_.s.str, str, (size_t)length);
    data_.s.str[length] = '\0';
  } else {
    data_.s.length = length;
    data_.s.str = (Char *)(str);
  }
}
LIB_JsonValue::LIB_JsonValue(S32 i)
    : type_(static_cast<TType>(kNumber | kS32)) {
  data_.n.i.i = i;
}
LIB_JsonValue::LIB_JsonValue(U32 u)
    : type_(static_cast<TType>(kNumber | kU32)) {
  data_.n.u.u = u;
}
LIB_JsonValue::LIB_JsonValue(S64 i)
    : type_(static_cast<TType>(kNumber | kS64)) {
  data_.n.i64 = i;
}
LIB_JsonValue::LIB_JsonValue(U64 u)
    : type_(static_cast<TType>(kNumber | kU64)) {
  data_.n.u64 = u;
}
LIB_JsonValue::LIB_JsonValue(Double d)
    : type_(static_cast<TType>(kNumber | kDouble)) {
  data_.n.d = d;
}
LIB_JsonValue::LIB_JsonValue(const void *b, U32 size) : type_(kBinary) {
  data_.b.binary = b;
  data_.b.size = size;
}
LIB_JsonValue::LIB_JsonValue(const LIB_JsonDocument *jdoc) : type_(kDocument) {
  data_.d.doc = jdoc;
}
LIB_JsonValue::LIB_JsonValue(const LIB_JsonValue *jval) : type_(kValue) {
  data_.v.value = jval;
}
LIB_JsonValue::~LIB_JsonValue() {
  if (type_ & kArray) {
    ADK_ASSERT(data_.a.elements == 0);
  } else if (type_ & kObject) {
    ADK_ASSERT(data_.o.members == 0);
  }
}
void LIB_JsonValue::Dispose(Bool isStrDup) {
  if (type_ & kString && isStrDup != false) {
    ADK_Free(data_.s.str), data_.s.str = 0;
  } else if (type_ & kArray) {
    if (data_.a.elements != 0) {
      const LIB_JsonValue *end = data_.a.elements + data_.a.size;
      for (LIB_JsonValue *v = data_.a.elements; v != end; ++v) {
        v->Dispose(isStrDup);
      }
      ADK_Free(data_.a.elements), data_.a.elements = 0;
    }
  } else if (type_ & kObject) {
    if (data_.o.members != 0) {
      const LIB_TJsonMember *end = data_.o.members + data_.o.size;
      for (LIB_TJsonMember *m = data_.o.members; m != end; ++m) {
        m->name.Dispose(isStrDup);
        m->data.Dispose(isStrDup);
      }
      ADK_Free(data_.o.members), data_.o.members = 0;
    }
  } else if (type_ & kDocument) {
    if (data_.d.doc != 0) {
      adk::LIB_JsonDocument *jdoc = (adk::LIB_JsonDocument *)data_.d.doc;
      jdoc->Release();
      delete jdoc, jdoc = 0;
    }
  }
}

Bool LIB_JsonValue::IsNull() const {
  if (IsValue()) {
    return data_.v.value->IsNull();
  } else {
    return type_ == kNull;
  }
}
Bool LIB_JsonValue::IsFalse() const {
  if (IsValue()) {
    return data_.v.value->IsFalse();
  } else {
    return type_ & kFalse ? true : false;
  }
}
Bool LIB_JsonValue::IsTrue() const {
  if (IsValue()) {
    return data_.v.value->IsTrue();
  } else {
    return type_ & kTrue ? true : false;
  }
}
Bool LIB_JsonValue::IsBool() const {
  if (IsValue()) {
    return data_.v.value->IsBool();
  } else {
    return type_ & kBool ? true : false;
  }
}
Bool LIB_JsonValue::GetBool() const {
  if (IsValue()) {
    return data_.v.value->GetBool();
  } else {
    ADK_ASSERT(IsBool());
    return IsFalse() != false ? false : true;
  }
}
Bool LIB_JsonValue::GetBool(Bool &out) const {
    Bool ret = true;
    if (IsValid() == false || IsNull() != false || IsBool() == false) {
        ret = false;
    } else {
        out = GetBool();
    }
    return ret;
}
const LIB_JsonValue *LIB_JsonValue::BeginArray() const {
  if (IsValue()) {
    return data_.v.value->BeginArray();
  } else {
    ADK_ASSERT(IsArray());
    return data_.a.elements;
  }
}
const LIB_JsonValue *LIB_JsonValue::EndArray() const {
  if (IsValue()) {
    return data_.v.value->EndArray();
  } else {
    ADK_ASSERT(IsArray());
    return data_.a.elements + data_.a.size - sizeof(LIB_JsonValue);
  }
}
Bool LIB_JsonValue::IsNumber() const {
  if (IsValue()) {
    return data_.v.value->IsNumber();
  } else {
    return type_ & kNumber ? true : false;
  }
}
Bool LIB_JsonValue::IsS32() const {
  if (IsValue()) {
    return data_.v.value->IsS32();
  } else {
    return type_ & kS32 ? true : false;
  }
}
S32 LIB_JsonValue::GetS32() const {
  if (IsValue()) {
    return data_.v.value->GetS32();
  } else {
    ADK_ASSERT(IsNumber());
    if (IsS32()) {
      return data_.n.i.i;
    } else if (IsU32()) {
      return (S32)data_.n.u.u;
    } else if (IsS64()) {
      return (S32)data_.n.i64;
    } else if (IsU64()) {
      return (S32)data_.n.u64;
    } else if (IsDouble()) {
      return (S32)data_.n.d;
    } else {
      return data_.n.i.i;
    }
  }
}
Bool LIB_JsonValue::GetS32(S32 &out) const {
    Bool ret = true;
    if (IsValid() == false || IsNull() != false || IsNumber() == false) {
        ret = false;
    } else {
        out = GetS32();
    }
    return ret;
}
Bool LIB_JsonValue::IsU32() const {
  if (IsValue()) {
    return data_.v.value->IsU32();
  } else {
    return type_ & kU32 ? true : false;
  }
}
U32 LIB_JsonValue::GetU32() const {
  if (IsValue()) {
    return data_.v.value->GetU32();
  } else {
    ADK_ASSERT(IsNumber());
    if (IsS32()) {
      return (U32)data_.n.i.i;
    } else if (IsU32()) {
      return data_.n.u.u;
    } else if (IsS64()) {
      return (U32)data_.n.i64;
    } else if (IsU64()) {
      return (U32)data_.n.u64;
    } else if (IsDouble()) {
      return (U32)data_.n.d;
    } else {
      return data_.n.u.u;
    }
  }
}
Bool LIB_JsonValue::GetU32(U32 &out) const {
    Bool ret = true;
    if (IsValid() == false || IsNull() != false || IsNumber() == false) {
        ret = false;
    } else {
        out = GetU32();
    }
    return ret;
}
Bool LIB_JsonValue::IsS64() const {
  if (IsValue()) {
    return data_.v.value->IsS64();
  } else {
    return type_ & kS64 ? true : false;
  }
}
S64 LIB_JsonValue::GetS64() const {
  if (IsValue()) {
    return data_.v.value->GetS64();
  } else {
    ADK_ASSERT(IsNumber());
    if (IsS32()) {
      return (S64)data_.n.i.i;
    } else if (IsU32()) {
      return (S64)data_.n.u.u;
    } else if (IsS64()) {
      return data_.n.i64;
    } else if (IsU64()) {
      return (S64)data_.n.u64;
    } else if (IsDouble()) {
      return (S64)data_.n.d;
    } else {
      return data_.n.i64;
    }
  }
}
Bool LIB_JsonValue::GetS64(S64 &out) const {
    Bool ret = true;
    if (IsValid() == false || IsNull() != false || IsNumber() == false) {
        ret = false;
    } else {
        out = GetS64();
    }
    return ret;
}
Bool LIB_JsonValue::IsU64() const {
  if (IsValue()) {
    return data_.v.value->IsU64();
  } else {
    return type_ & kU64 ? true : false;
  }
}
U64 LIB_JsonValue::GetU64() const {
  if (IsValue()) {
    return data_.v.value->GetU64();
  } else {
    ADK_ASSERT(IsNumber());
    if (IsS32()) {
      return (U64)data_.n.i.i;
    } else if (IsU32()) {
      return (U64)data_.n.u.u;
    } else if (IsS64()) {
      return (U64)data_.n.i64;
    } else if (IsU64()) {
      return data_.n.u64;
    } else if (IsDouble()) {
      return (U64)data_.n.d;
    } else {
      return data_.n.u64;
    }
  }
}
Bool LIB_JsonValue::GetU64(U64 &out) const {
    Bool ret = true;
    if (IsValid() == false || IsNull() != false || IsNumber() == false) {
        ret = false;
    } else {
        out = GetU64();
    }
    return ret;
}
Bool LIB_JsonValue::IsDouble() const {
  if (IsValue()) {
    return data_.v.value->IsDouble();
  } else {
    return type_ & kDouble ? true : false;
  }
}
Double LIB_JsonValue::GetDouble() const {
  if (IsValue()) {
    return data_.v.value->GetDouble();
  } else {
    ADK_ASSERT(IsNumber());
    if (IsS32()) {
      return (Double)data_.n.i.i;
    } else if (IsU32()) {
      return (Double)data_.n.u.u;
    } else if (IsS64()) {
      return (Double)data_.n.i64;
    } else if (IsU64()) {
      return (Double)data_.n.u64;
    } else if (IsDouble()) {
      return data_.n.d;
    } else {
      return data_.n.d;
    }
  }
}
Bool LIB_JsonValue::GetDouble(Double &out) const {
    Bool ret = true;
    if (IsValid() == false || IsNull() != false || IsNumber() == false) {
        ret = false;
    } else {
        out = GetDouble();
    }
    return ret;
}
Bool LIB_JsonValue::IsString() const {
  if (IsValue()) {
    return data_.v.value->IsString();
  } else {
    return type_ & kString ? true : false;
  }
}
const Char *LIB_JsonValue::GetString() const {
  if (IsValue()) {
    return this->data_.v.value->GetString();
  } else {
    ADK_ASSERT(IsString());
    return data_.s.str;
  }
}
Bool LIB_JsonValue::GetString(S32 out_length, char *out) const {
    Bool ret = true;
    if (IsValid() == false || IsNull() != false || IsString() == false) {
        ret = false;
    } else {
        adk::LIB_strncpy(out, GetString(), out_length);
    }
    return ret;
}
U32 LIB_JsonValue::GetStringLength() const {
  if (IsValue()) {
    return data_.v.value->GetStringLength();
  } else {
    ADK_ASSERT(IsString());
    return data_.s.length;
  }
}
Bool LIB_JsonValue::IsBinary() const {
  if (IsValue()) {
    return data_.v.value->IsBinary();
  } else {
    return type_ & kBinary ? true : false;
  }
}
const void *LIB_JsonValue::GetBinary() const {
  if (IsValue()) {
    return data_.v.value->GetBinary();
  } else {
    ADK_ASSERT(IsBinary());
    return data_.b.binary;
  }
}
Bool LIB_JsonValue::IsDocument() const {
  if (IsValue()) {
    return data_.v.value->IsDocument();
  } else {
    return type_ & kDocument ? true : false;
  }
}
const LIB_JsonDocument *LIB_JsonValue::GetDocument() const {
  if (IsValue()) {
    return data_.v.value->GetDocument();
  } else {
    ADK_ASSERT(IsDocument());
    return data_.d.doc;
  }
}

/* value managment functionally */
void LIB_JsonValue::AddMember(LIB_TJsonMember *members, U32 membercount) {
  ADK_ASSERT(type_ == kObject);

  data_.o.members = static_cast<LIB_TJsonMember *>(ADK_Malloc(
      sizeof(LIB_TJsonMember) * membercount, "Lib_JsonValue::Member"));
  ADK_ASSERT(data_.o.members);
  memcpy(data_.o.members, members, sizeof(LIB_TJsonMember) * membercount);
  data_.o.size = membercount;
}
LIB_TJsonMember *LIB_JsonValue::FindMember(const Char *name) {
  ADK_ASSERT_MSG(name, "LIB_JsonValue::FindMember(): Invalid variable(name).");
  ADK_ASSERT_MSG(
      IsObject(),
      "LIB_JsonValue::FindMember(): Not kObject. I've tried the search key.");

  const U32 namelen = LIB_strlen(name);

  TObject &o = data_.o;
  void *end = data_.o.members + data_.o.size;
  for (LIB_TJsonMember *member = o.members; member != end; ++member) {
    if (namelen == (member->name.data_.s.length) &&
        memcmp(member->name.data_.s.str, name, namelen * sizeof(Char)) == 0) {
      return member;
    }
  }

  return 0;
}
const LIB_TJsonMember &LIB_JsonValue::GetMember(S32 index) const {
  if (IsDocument()) {
    return data_.d.doc->GetMember(index);
  } else {
    ADK_ASSERT(type_ == kObject);
    ADK_ASSERT(index >= 0 && index < Size());
    return *(&data_.o.members[index]);
  }
}
void LIB_JsonValue::AddElements(LIB_JsonValue *elements, U32 elementcount) {
  ADK_ASSERT(type_ == kArray);

  data_.a.elements = static_cast<LIB_JsonValue *>(ADK_Malloc(
      sizeof(LIB_JsonValue) * elementcount, "Lib_JsonValue::LIB_JsonValue"));
  ADK_ASSERT(data_.a.elements);
  memcpy(static_cast<void *>(data_.a.elements), static_cast<void *>(elements),
         sizeof(LIB_JsonValue) * elementcount);
  data_.a.size = elementcount;
}
Bool LIB_JsonValue::IsObject() const {
  if (IsValue()) {
    return data_.v.value->IsObject();
  } else if (IsDocument()) {
    return data_.d.doc->IsArray();
  } else {
    return type_ == kObject;
  }
}
Bool LIB_JsonValue::IsArray() const {
  if (IsValue()) {
    return data_.v.value->IsArray();
  } else if (IsDocument()) {
    return data_.d.doc->IsArray();
  } else {
    return type_ == kArray;
  }
}
U32 LIB_JsonValue::Size() const {
  U32 ret;

  if (IsArray()) {
    ret = data_.a.size;
  } else if (IsObject()) {
    ret = data_.o.size;
  } else {
    ret = 0;
  }

  return ret;
}
Result LIB_JsonValue::GetNumber(void *dst, LIB_TJsonType type) const {
  Result ret = ResultSuccess;

  ADK_ASSERT(IsNumber());

  if (dst == 0) {
    ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kLibraryComponent,
                             kLibJsonValue,
                             ERR_LIB_JVALUE_GET_NUMBER_INVALID_VARIABLE);
  } else {
    switch (type) {
      case LIB_kJsonS32: {
        S32 *value = static_cast<S32 *>(dst);
        if (IsS32()) {
          *value = static_cast<S32>(GetS32());
        } else if (IsU32()) {
          *value = static_cast<U32>(GetU32());
        } else if (IsS64()) {
          *value = static_cast<S64>(GetS64());
        } else if (IsU64()) {
          *value = static_cast<U64>(GetU64());
        }
        break;
      }
      case LIB_kJsonU32: {
        U32 *value = static_cast<U32 *>(dst);
        if (IsS32()) {
          *value = static_cast<S32>(GetS32());
        } else if (IsU32()) {
          *value = static_cast<U32>(GetU32());
        } else if (IsS64()) {
          *value = static_cast<S64>(GetS64());
        } else if (IsU64()) {
          *value = static_cast<U64>(GetU64());
        }
      }
      case LIB_kJsonS64: {
        S64 *value = static_cast<S64 *>(dst);
        if (IsS32()) {
          *value = static_cast<S32>(GetS32());
        } else if (IsU32()) {
          *value = static_cast<U32>(GetU32());
        } else if (IsS64()) {
          *value = static_cast<S64>(GetS64());
        } else if (IsU64()) {
          *value = static_cast<U64>(GetU64());
        }
        break;
      }
      case LIB_kJsonU64: {
        U64 *value = static_cast<U64 *>(dst);
        if (IsS32()) {
          *value = static_cast<S32>(GetS32());
        } else if (IsU32()) {
          *value = static_cast<U32>(GetU32());
        } else if (IsS64()) {
          *value = static_cast<S64>(GetS64());
        } else if (IsU64()) {
          *value = static_cast<U64>(GetU64());
        }
        break;
      }
      default: {
        ret = ADK_RESULT_PROGRAM(
            ResultInvalidVariable, kLibraryComponent, kLibJsonValue,
            ERR_LIB_JVALUE_GET_NUMBER_INVALID_VARIABLE_TYPE);
      }
    }
  }

  return ret;
}
LIB_TJsonType LIB_JsonValue::GetType() const {
  LIB_TJsonType ret = LIB_kJsonNull;

  if (IsString()) {
    ret = LIB_kJsonString;
  } else if (IsBool()) {
    ret = LIB_kJsonBool;
  } else if (IsNumber()) {
    if (IsS32()) {
      ret = LIB_kJsonS32;
    } else if (IsU32()) {
      ret = LIB_kJsonU32;
    } else if (IsS64()) {
      ret = LIB_kJsonS64;
    } else if (IsU64()) {
      ret = LIB_kJsonU64;
    } else if (IsDouble()) {
      ret = LIB_kJsonDouble;
    }
  } else if (IsObject()) {
    ret = LIB_kJsonObject;
  } else if (IsArray()) {
    ret = LIB_kJsonArray;
  } else if (IsBinary()) {
    ret = LIB_kJsonBinary;
  } else if (IsDocument()) {
    ret = LIB_kJsonDocument;
  } else if (IsValue()) {
    ret = LIB_kJsonValue;
  }

  return ret;
}
U32 LIB_JsonValue::GetTypeBinarySize(size_t *alignment) const {
  U32 ret = 0;
  size_t type_binary_size = TypeToBinarySize();
  size_t sub_alignment = ADK_ALIGN(*alignment, type_binary_size) - *alignment;
  if (sub_alignment != 0) {
    ret += sub_alignment;
  }

  if (IsString()) {
    ret += sizeof(const Char *);
  } else if (IsBool()) {
    ret += 4;
  } else if (IsNumber()) {
    if (IsS32() || IsU32()) {
      ret += 4;
    } else if (IsS64() || IsU64() || IsDouble()) {
      ret += 8;
    }
  } else if (IsArray()) {
    if (Size() == 0) {
      ret += 0;
    } else {
      /*
       * 配列が見つかったら配列の手前に配列数格納用のサイズを用意する(S32)
       */
      ret += 4;
      *alignment += 4;
      /*
       * 配列の持つ一要素の合計サイズを求める
       * ただJSONフォーマットは配列内に要素の違うオブジェクトを組み込めてしまうが、
       * 現状は全て同じ要素とし先頭オブジェクトの要素サイズのみ参照するようにする
       *
       * また、配列はアドレス参照するためサイズを'sizeof(void*)'と固定にする
       */
      for (S32 index = 0; index < Size(); ++index) {
        const LIB_JsonValue &top = (*this)[index];
        if (top.IsObject()) {
          for (S32 pos = 0; pos < top.Size(); ++pos) {
            const LIB_TJsonMember &member = top.GetMember(pos);
            ret += member.data.GetTypeBinarySize(alignment);
          }
        } else if (top.IsBool() || top.IsNumber()) {
          ret += top.GetTypeBinarySize(alignment);
        } else {
          ADK_ASSERT(0);
        }
      }
    }
  } else if (IsObject()) {
    if (Size() == 0) {
      ret += 0;
    } else {
      /*
       * オブジェクトが持つ要素の合計サイズを求める。
       * ただし、配列はアドレス参照するためサイズを'sizeof(void*)'と固定にする
       */
      for (S32 pos = 0; pos < this->Size(); ++pos) {
        const LIB_TJsonMember &member = this->GetMember(pos);
        if (member.data.IsArray()) {
          ret += sizeof(S32);
          ret += sizeof(void *);
        } else {
          U32 binary_size = member.data.GetTypeBinarySize(alignment);
          ret += binary_size;
          *alignment += binary_size;
        }
      }
    }
  } else if (IsValue()) {
    ADK_ASSERT_MSG(0, "Unauthorized access.\n");
  }

  return ret;
}
size_t LIB_JsonValue::TypeToBinarySize() const {
  size_t ret = 0;

  if (IsString()) {
    ret += 4; /* pointer size */
  } else if (IsNumber()) {
    if (IsS32() || IsU32()) {
      ret += 4;
    } else if (IsS64() || IsU64() || IsDouble()) {
      ret += 8;
    }
  } else if (IsBool()) {
    ret += 4;
  } else if (IsArray()) {
    ret += 4; /* pointer size */
  } else if (IsObject()) {
    ret += 8;
  } else {
    ADK_ASSERT(false);
  }

  return ret;
}
LIB_TJsonMember &LIB_JsonValue::operator[](const Char *name) {
  if (IsValue()) {
    return (const_cast<LIB_JsonValue &>(*this->data_.v.value))[name];
  } else {
    if (LIB_TJsonMember *member = FindMember(name)) {
      return *member;
    } else {
      return nullmember;
    }
  }
}
const LIB_TJsonMember &LIB_JsonValue::operator[](const Char *name) const {
  if (IsValue()) {
    return (const_cast<LIB_JsonValue &>(*this->data_.v.value))[name];
  } else {
    return const_cast<LIB_JsonValue &>(*this)[name];
  }
}
LIB_JsonValue &LIB_JsonValue::operator[](S32 index) {
  if (IsValue()) {
    return (const_cast<LIB_JsonValue &>(*this->data_.v.value))[index];
  } else if (IsDocument()) {
    return const_cast<LIB_JsonValue &>((*this->data_.d.doc)[index]);
  } else {
    ADK_ASSERT(IsArray());
    ADK_ASSERT(0 <= index && index < Size());

    if (IsArray() == false || index < 0 || index >= Size()) {
      return nullval;
    } else {
      return data_.a.elements[index];
    }
  }
}
const LIB_JsonValue &LIB_JsonValue::operator[](S32 index) const {
  if (IsValue()) {
    return (const_cast<LIB_JsonValue &>(*this->data_.v.value))[index];
  } else if (IsDocument()) {
    return const_cast<LIB_JsonValue &>((*this->data_.d.doc)[index]);
  } else {
    return const_cast<LIB_JsonValue &>(*this)[index];
  }
}
void LIB_JsonValue::Copy(LIB_JsonValue *value) {
  this->data_ = value->data_;
  this->type_ = value->type_;
}

ADK_END_NAMESPACE

