/**
 * \file err_api_result.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief APIリザルト定義部
 */


#ifndef ERR_API_RESULT_H_
#define ERR_API_RESULT_H_


#include <debug_profiling/error/result.h>


ADK_BEGIN_NAMESPACE


/* function */
#define ERRFUNC_API_INITIALIZE  (0)
#define ERRFUNC_API_TERMINATE  (1)
#define ERRFUNC_API_LOGIN  (2)
#define ERRFUNC_API_DISPATCHPROCESS  (3)


/* extern function implementation */
/**
 * \~english
 * \~japanese
 *   \brief APIのリザルトメッセージを返します
 *   \return メッセージ
 */
extern const Char *GetApiResultMessage( const Result &result);


ADK_END_NAMESPACE


#endif  /* _ADK_API_RESULT_H_ */


