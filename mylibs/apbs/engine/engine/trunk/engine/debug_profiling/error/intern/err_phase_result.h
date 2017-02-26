/**
 * \file err_phase_result.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief フェーズエラー定義部
 */


#ifndef ERR_PHASE_RESULT_H_
#define ERR_PHASE_RESULT_H_


#include <debug_profiling/error/result.h>


ADK_BEGIN_NAMESPACE


/* extern function implementation */
/**
 * \~english
 * \~japanese
 *   \brief フェーズのリザルトメッセージを返します
 *   \return メッセージ
 */
extern const Char *GetPhaseResultMessage( const Result &result);


ADK_END_NAMESPACE


#endif  /* ERR_PHASE_RESULT_H_ */


