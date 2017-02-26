/**
 * \file dms_schema_types.h
 * \ingroup adkgrp_dms
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief DMSのスキーマで扱う定義群
 */


#ifndef DMS_SCHEMA_TYPES_H_
#define DMS_SCHEMA_TYPES_H_


#include <core/types.h>


ADK_BEGIN_NAMESPACE


/* schema type */
/**
 * \~english
 * \~japanese
 *   \brief スキーマタイプ
 */
typedef enum{
  DMS_kStatic,
  DMS_kDynamic
}DMS_TSchemaType;
/**
 * \~english
 * \~japanese
 *   \brief 値タイプ
 */
typedef enum{
  DMS_kBoolean,
  DMS_kString,
  DMS_kS32,
  DMS_kU32,
  DMS_kS64,
  DMS_kU64,
  DMS_kDouble,
  DMS_kArray,
  DMS_kObject
}DMS_TValueType;


ADK_END_NAMESPACE


#endif  /* DMS_SCHEMA_H_ */


