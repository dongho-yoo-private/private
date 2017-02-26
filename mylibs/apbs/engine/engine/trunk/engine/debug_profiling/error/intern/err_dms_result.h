/**
 * \file err_dms_result.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief データマネージメントシステムエラー定義部
 */


#ifndef ERR_DMS_RESULT_H_
#define ERR_DMS_RESULT_H_


#include <debug_profiling/error/result.h>


ADK_BEGIN_NAMESPACE


/* extern function implementation */
/**
 * \~english
 * \~japanese
 *   \brief ローカルDBのリザルトメッセージを返します
 *   \return メッセージ
 */
extern const Char *GetDmsResultMessage( const Result &result);


ADK_END_NAMESPACE


#endif  /* _ERR_DMS_RESULT_H_ */


