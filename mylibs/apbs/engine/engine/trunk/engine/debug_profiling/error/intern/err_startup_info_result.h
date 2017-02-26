/**
 * \file err_startup_info_result.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief 起動設定リザルト定義部
 */


#ifndef ERR_STARTUP_INFO_RESULT_H_
#define ERR_STARTUP_INFO_RESULT_H_


#include <debug_profiling/error/result.h>


ADK_BEGIN_NAMESPACE


/* extern function implementation */
/**
 * \~english
 * \~japanese
 *   \brief 起動設定のリザルトメッセージを返します
 *   \return メッセージ
 */
extern const Char *GetStartupInfoResultMessage( const Result &result);


ADK_END_NAMESPACE


#endif  /* ERR_STARTUP_INFO_RESULT_H_ */


