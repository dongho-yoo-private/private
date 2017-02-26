/**
 * \file adk_request_types.h
 * \ingroup adkgrp_request
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief リクエストに関する定義部
 */


#ifndef ADK_REQUEST_TYPES_H_
#define ADK_REQUEST_TYPES_H_

#include <core/types.h>

ADK_BEGIN_NAMESPACE


/**
 * \~english
 * \~japanese
 *  \brief JSON共通パラメータ最大数
 */
#define ADK_JSON_COMMON_PARAMETER_MAX (16)
/**
 * \~english
 * \~japanese
 *  \brief JSON共通パラメータに使用するキーの文字列長
 */
#define ADK_JSON_COMMON_PARAMETER_KEY_LEN (32)
/**
 * \~english
 * \~japanese
 *  \brief JSON共通パラメータに使用する値の文字列長
 */
#define ADK_JSON_COMMON_PARAMETER_VALUE_LEN (32)
/**
 * \~english
 * \~japanese
 *  \brief JSON共通パラメータ
 */
typedef struct {
  /**
   * \~english
   * \~japanese
   *  \brief パラメータのキー文字列
   */
  Char  key[ADK_JSON_COMMON_PARAMETER_KEY_LEN];
  /**
   * \~english
   * \~japanese
   *  \brief パラメータデータ
   */
  struct {
    /**
     * \~english
     * \~japanese
     *  \brief パラメータの値文字列
     */
    Char string[ADK_JSON_COMMON_PARAMETER_KEY_LEN];
    /**
     * \~english
     * \~japanese
     *  \brief パラメータのS32値
     */
    S32 i32;
    /**
     * \~english
     * \~japanese
     *  \brief パラメータのU64値
     */
    U64 u64;
    /**
     * \~english
     * \~japanese
     *  \brief データタイプ\n
     *         0: 文字列
     *         1: S32
     *         2: U64
     */
    S32 type;
  }data;
}TJsonCommonParameter;


ADK_END_NAMESPACE


#endif  /* ADK_REQUEST_TYPES_H_ */


