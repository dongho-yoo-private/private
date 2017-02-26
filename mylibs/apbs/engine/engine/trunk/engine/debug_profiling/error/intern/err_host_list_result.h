/**
 * \file err_host_list_result.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief ホストリストリザルト定義部
 */


#ifndef ERR_HOST_LIST_RESULT_H_
#define ERR_HOST_LIST_RESULT_H_


#include <debug_profiling/error/result.h>


ADK_BEGIN_NAMESPACE


/* extern function implementation */
/**
 * \~english
 * \~japanese
 *   \brief ホストリストのリザルトメッセージを返します
 *   \return メッセージ
 */
extern const Char *GetHostListResultMessage( const Result &result);


ADK_END_NAMESPACE


#endif  /* ERR_HOST_LIST_RESULT_H_ */


