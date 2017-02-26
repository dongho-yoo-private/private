/**
 * \file lib_json_value-inl.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief JSON値インライン実装部
 */

#ifndef LIB_JSON_VALUE_INL_H_
#define LIB_JSON_VALUE_INL_H_

ADK_BEGIN_NAMESPACE

ADK_INLINE Bool LIB_JsonValue::IsValid() const {
  return type_ == kUnknown ? false : true;
}
ADK_INLINE Bool LIB_JsonValue::IsValue() const {
  return type_ & kValue ? true : false;
}
ADK_INLINE const LIB_JsonValue *LIB_JsonValue::GetValue() const {
  ADK_ASSERT(IsValue());
  return data_.v.value;
}

ADK_END_NAMESPACE

#endif /* LIB_JSON_VALUE_INL_H_ */

