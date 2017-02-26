/**
 * \file adk_network_types.h
 * \ingroup adkgroup_network
 * \~english
 * \~japanese
 *  \brief ネットワーク定義群
 */


#ifndef ADK_NETWORK_TYPES_H_
#define ADK_NETWORK_TYPES_H_


#include <core/types.h>


ADK_BEGIN_NAMESPACE


/**
 * \~english
 * \~japanese
 *  \brief HTTPリクエストヘッダ最大数
 */
#define ADK_HTTP_REQUEST_HEADER_MAX (16)
/*
 * バッファを可変する事により削除
 */
#if 0
/**
 * \~english
 * \~japanese
 *  \brief HTTPリクエストヘッダキー文字列長
 */
#define ADK_HTTP_REQUEST_HEADER_KEY_LEN (64)
/**
 * \~english
 * \~japanese
 *  \brief HTTPリクエストヘッダキー文字列長
 */
#define ADK_HTTP_REQUEST_HEADER_VALUE_LEN (128)
#endif
/**
 * \~english
 * \~japanese
 *  \brief HTTPリクエストヘッダ
 */
typedef struct {
  /**
   * \~english
   * \~japanese
   *  \brief リクエストヘッダのキー文字列
   */
  Char *key;
  /**
   * \~english
   * \~japanese
   *  \brief リクエストヘッダの値文字列
   */
  Char *value;
}TRequestHttpHeader;


ADK_END_NAMESPACE


#endif  /* ADK_NETWORK_TYPES_H_ */


