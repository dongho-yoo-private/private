/**
 * \file err_request_manager_result.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief リクエストマネージャリザルト定義部
 */


#ifndef ERR_REQUEST_MANAGER_RESULT_H_
#define ERR_REQUEST_MANAGER_RESULT_H_


#include <debug_profiling/error/result.h>


ADK_BEGIN_NAMESPACE


/* extern function implementation */
/**
 * \~english
 * \~japanese
 *   \brief リクエストマネージャのリザルトメッセージを返します
 *   \return メッセージ
 */
extern const Char *GetRequestManagerResultMessage( const Result &result);


ADK_END_NAMESPACE


#endif  /* ERR_REQUEST_MANAGER_RESULT_H_ */


