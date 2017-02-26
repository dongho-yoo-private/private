/**
 * \file dbg_stack_trace.h
 * \ingroup adkgrp_debug
 * \author dongho.yoo
 * \~english
 * \~japanese
 *   \brief コールスタックに関する定義部
 */


#ifndef DBG_STACK_TRACE_H_
#define DBG_STACK_TRACE_H_

#include <core/types.h>

//ADK_BEGIN_NAMESPACE

#ifdef __cplusplus
extern "C" {
#endif

/* extern function definition */
/**
 * \~english
 * \~japanese
 *   \brief 現在位置のバックトレース情報を取得します。
 *   \param[out] simbols シンボルアドレス格納先
 *   \param[in] max 最大バックトレース数
 *   \return 取得できたコールスタック数。以下の値が返ります
 *   \retval ret>0  取得できたコールスタック数
 *   \retval 0    取得できなかったときに返ります
 */
extern S32 DBG_GetBackTrace( const void *simbols[], S32 max);
/**
 * \~english
 * \~japanese
 *   \brief シンボルアドレスからシンボル名を返します
 *   \param[in] addr シンボルアドレス
 *   \return 文字列
 *   \retval ret!=0  シンボル名が取得できたときに返ります
 *   \retval 0    シンボル名が取得できなかったときに返ります
 */
extern const Char *DBG_GetSymbolName( const void *addr);

#ifdef __cplusplus
}
#endif  /* __cplusplus */


//ADK_END_NAMESPACE


#endif  /* DBG_STACK_TRACE_H_ */


