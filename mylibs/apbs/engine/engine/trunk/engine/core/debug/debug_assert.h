/**
 * \file assert.h
 * \ingroup adkgrp_debug
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief アサート機能
 */


#ifndef DEBUG_ASSERT_H_
#define DEBUG_ASSERT_H_


#include <assert.h>

#include <core/types.h>


/* assert support */
/**
 * \~english
 * \~japanese
 *   \brief Debug/Release 問わず必ず停止させる
 */
#define ADK_FATAL  assert
#ifdef ADK_ASSERT_SUPPORT
/**
 * \~english
 * \~japanese
 *   \brief Debug時のみ停止させる
 */
#  define ADK_ASSERT  assert
/**
 * \~english
 * \~japanese
 *   \brief Debug時のみメッセージ付きで停止させる
 */
#  define ADK_ASSERT_MSG(code,msg)\
  if(code==false)\
  {\
    printf(msg);\
    printf("\n");\
    ADK_ASSERT(code);\
  }\

#else
  /**
   * \~english
   * \~japanese
   *   \brief Debug時のみ停止させる
   */
#  define ADK_ASSERT
  /**
   * \~english
   * \~japanese
   *   \brief Debug時のみメッセージ付きで停止させる
   */
#  define ADK_ASSERT_MSG(code,msg)
#endif


#endif  /* DEBUG_ASSERT_H_ */


