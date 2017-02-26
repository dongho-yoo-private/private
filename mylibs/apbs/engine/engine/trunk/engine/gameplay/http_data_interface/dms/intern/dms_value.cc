/**
 * \file dms_value.cc
 * \ingroup adkgrp_dms
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief DMSで扱うスキーマの値クラス実装部
 */

#include <gameplay/http_data_interface/dms/intern/dms_value.h>

#include <core/debug/debug_assert.h>
#include <core/library/lib_string.h>
#include <debug_profiling/error/result_types.h>

ADK_BEGIN_NAMESPACE

/* class implementation */
/* constructor & destructor */
DMS_Value::DMS_Value() {
  memset(&key_, 0, sizeof(key_));
  memset(&value_, 0, sizeof(value_));
}
DMS_Value::~DMS_Value() {}

/* initialize & terminate */
Result DMS_Value::InitializeBool(const Char *key, U32 keylen, Bool b) {
  Result ret = ResultSuccess;

  if (key == 0) {
    ret = ResultInvalidVariable;
  } else {
    type_ = DMS_kBoolean;
    key_.str = key;
    key_.len = keylen;
    value_.size = 4;
    value_.count = 1;
    value_.v.b.b = b;
  }

  return ret;
}
Result DMS_Value::InitializeString(const Char *key, U32 keylen, const Char *str,
                                   U32 len) {
  Result ret = ResultSuccess;

  if (key == 0) {
    ret = ResultInvalidVariable;
  } else {
    type_ = DMS_kString;
    key_.str = key;
    key_.len = keylen;
    value_.size = len;
    value_.count = 1;
    value_.v.s.addr = (U64)str;
  }

  return ret;
}
Result DMS_Value::InitializeS32(const Char *key, U32 keylen, S32 value) {
  Result ret = ResultSuccess;

  if (key == 0) {
    ret = ResultInvalidVariable;
  } else {
    type_ = DMS_kS32;
    key_.str = key;
    key_.len = keylen;
    value_.size = 4;
    value_.count = 1;
    value_.v.n.i.i = value;
  }

  return ret;
}
Result DMS_Value::InitializeU32(const Char *key, U32 keylen, U32 value) {
  Result ret = ResultSuccess;

  if (key == 0) {
    ret = ResultInvalidVariable;
  } else {
    type_ = DMS_kU32;
    key_.str = key;
    key_.len = keylen;
    value_.size = 4;
    value_.count = 1;
    value_.v.n.u.u = value;
  }

  return ret;
}
Result DMS_Value::InitializeS64(const Char *key, U32 keylen, S64 value) {
  Result ret = ResultSuccess;

  if (key == 0) {
    ret = ResultInvalidVariable;
  } else {
    type_ = DMS_kS64;
    key_.str = key;
    key_.len = keylen;
    value_.size = 8;
    value_.count = 1;
    value_.v.n.i64 = value;
  }

  return ret;
}
Result DMS_Value::InitializeU64(const Char *key, U32 keylen, U64 value) {
  Result ret = ResultSuccess;

  if (key == 0) {
    ret = ResultInvalidVariable;
  } else {
    type_ = DMS_kU64;
    key_.str = key;
    key_.len = keylen;
    value_.size = 8;
    value_.count = 1;
    value_.v.n.u64 = value;
  }

  return ret;
}
Result DMS_Value::InitializeDouble(const Char *key, U32 keylen, Double value) {
  Result ret = ResultSuccess;

  if (key == 0) {
    ret = ResultInvalidVariable;
  } else {
    type_ = DMS_kDouble;
    key_.str = key;
    key_.len = keylen;
    value_.size = 8;
    value_.count = 1;
    value_.v.n.d = value;
  }

  return ret;
}
Result DMS_Value::InitializeArray(const Char *key, U32 keylen, S32 count,
                                  U32 size) {
  Result ret = ResultSuccess;

  if (key == 0) {
    ret = ResultInvalidVariable;
  } else {
    type_ = DMS_kArray;
    key_.str = key;
    key_.len = keylen;
    value_.size = size;
    value_.count = count;
    value_.v.a.addr = (U64)ADK_Malloc(size * count, "DMS_Value::Array");
    if (value_.v.a.addr == 0) {
      ret = ResultOutOfResource;
    }
  }

  return ret;
}
Result DMS_Value::InitializeObject(const Char *key, U32 keylen, U32 size) {
  Result ret = ResultSuccess;

  if (key == 0) {
    ret = ResultInvalidVariable;
  } else {
    type_ = DMS_kObject;
    key_.str = key;
    key_.len = keylen;
    value_.size = size + 4;
    value_.count = 1;
    value_.v.o.addr = (U64)ADK_Malloc(value_.size, "DMS_Value::Object");
    if (value_.v.o.addr == 0) {
      ret = ResultOutOfResource;
    }
  }

  return ret;
}
Result DMS_Value::Terminate() {
  if (type_ == DMS_kArray) {
    ADK_Free((void *)value_.v.a.addr), value_.v.a.addr = 0;
  } else if (type_ == DMS_kObject) {
    ADK_Free((void *)value_.v.o.addr), value_.v.o.addr = 0;
  }

  return ResultSuccess;
}

/* value */
Bool DMS_Value::GetBoolean() const {
  ADK_ASSERT(type_ == DMS_kBoolean);
  return value_.v.b.b;
}
const Char *DMS_Value::GetString() const {
  ADK_ASSERT(type_ == DMS_kString);
  return (const Char *)value_.v.s.addr;
}
S32 DMS_Value::GetS32() const {
  ADK_ASSERT(type_ == DMS_kS32);
  return value_.v.n.i.i;
}
U32 DMS_Value::GetU32() const {
  ADK_ASSERT(type_ == DMS_kU32);
  return value_.v.n.u.u;
}
S64 DMS_Value::GetS64() const {
  ADK_ASSERT(type_ == DMS_kS64);
  return value_.v.n.i64;
}
U64 DMS_Value::GetU64() const {
  ADK_ASSERT(type_ == DMS_kU64);
  return value_.v.n.u64;
}
Double DMS_Value::GetDouble() const {
  ADK_ASSERT(type_ == DMS_kDouble);
  return value_.v.n.d;
}
const void *DMS_Value::GetArray() const {
  ADK_ASSERT(type_ == DMS_kArray);
  return (void *)(value_.v.a.addr);
}
const void *DMS_Value::GetObject() const {
  ADK_ASSERT(type_ == DMS_kObject);
  return (void *)value_.v.o.addr;
}

ADK_END_NAMESPACE

