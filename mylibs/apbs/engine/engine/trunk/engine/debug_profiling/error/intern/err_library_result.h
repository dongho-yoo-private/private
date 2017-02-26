/**
 * \file err_library_result.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief ライブラリリザルト定義部
 */


#ifndef ERR_LIBRARY_RESULT_H_
#define ERR_LIBRARY_RESULT_H_


#include <debug_profiling/error/result.h>


ADK_BEGIN_NAMESPACE


/* extern function implementation */
/**
 * \~english
 * \~japanese
 *   \brief ライブラリのリザルトメッセージを返します
 *   \return メッセージ
 */
extern const Char *GetLibraryResultMessage( const Result &result);


ADK_END_NAMESPACE


#endif  /* ERR_LIBRARY_RESULT_H_ */


