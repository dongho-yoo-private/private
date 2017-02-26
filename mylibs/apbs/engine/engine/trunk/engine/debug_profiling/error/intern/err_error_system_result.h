/**
 * \file err_error_system_result.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief エラーシステムリザルト定義部
 */


#ifndef ERR_ERROR_SYSTEM_RESULT_H_
#define ERR_ERROR_SYSTEM_RESULT_H_


#include <debug_profiling/error/result.h>


ADK_BEGIN_NAMESPACE


/* extern function implementation */
/**
 * \~english
 * \~japanese
 *   \brief エラーシステムのリザルトメッセージを返します
 *   \return メッセージ
 */
extern const Char *GetErrorSystemResultMessage( const Result &result);


ADK_END_NAMESPACE


#endif  /* ERR_ERROR_SYSTEM_RESULT_H_ */


