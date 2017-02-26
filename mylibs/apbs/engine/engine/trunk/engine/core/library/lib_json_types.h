/**
 * \file lib_json_types.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief JSON定義部
 */

#ifndef LIB_JSON_TYPES_H_
#define LIB_JSON_TYPES_H_

#include <core/types.h>

ADK_BEGIN_NAMESPACE

/* value member type */
typedef enum {
  LIB_kJsonNull,
  LIB_kJsonBool,
  LIB_kJsonTrue,
  LIB_kJsonFalse,
  LIB_kJsonNumber,
  LIB_kJsonS32,
  LIB_kJsonU32,
  LIB_kJsonS64,
  LIB_kJsonU64,
  LIB_kJsonDouble,
  LIB_kJsonString,
  LIB_kJsonObject,
  LIB_kJsonArray,
  LIB_kJsonBinary,
  LIB_kJsonDocument,
  LIB_kJsonValue
} LIB_TJsonType;

ADK_END_NAMESPACE

#endif /* LIB_JSON_TYPES_H_ */

