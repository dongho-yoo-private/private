/**
 * \file lib_url_scheme.h
 * \ingroup adkgrp_library
 * \author ishii yasutaka
 * \~english
 * \~japanese
 *   \brief URLスキームに関する定義部です
 */


#ifndef LIB_URL_SCHEME_H_
#define LIB_URL_SCHEME_H_


#include <core/types.h>


ADK_BEGIN_NAMESPACE


/* extern function definition */
/**
 * \~english
 * \~japanese
 *   \brief アプリケーション・ストアへ遷移します
 */
extern void LIB_OpenUrlScheme(const Char* url);


ADK_END_NAMESPACE


#endif  /* LIB_URL_SCHEME_H_ */


