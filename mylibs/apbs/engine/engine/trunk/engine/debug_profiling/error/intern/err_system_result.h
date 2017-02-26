/**
 * \file err_system_result.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief システムリザルト定義部
 */

#ifndef ERR_SYSTEM_RESULT_H_
#define ERR_SYSTEM_RESULT_H_

#include <debug_profiling/error/result.h>

ADK_BEGIN_NAMESPACE

/* extern function implementation */
/**
 * \~english
 * \~japanese
 *   \brief システムのリザルトメッセージを返します
 *   \return メッセージ
 */
extern const Char *GetSystemResultMessage(const Result &result);

ADK_END_NAMESPACE

#endif /* ERR_SYSTEM_RESULT_H_ */

