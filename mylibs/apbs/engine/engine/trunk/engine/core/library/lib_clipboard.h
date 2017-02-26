/**
 * \file lib_clipboard.h
 * \ingroup adkgrp_library
 * \author ishii yasutaka
 * \~english
 * \~japanese
 *   \brief クリップボード情報に関する定義部です
 */


#ifndef LIB_CLIPBOARD_H_
#define LIB_CLIPBOARD_H_


#include <core/types.h>


ADK_BEGIN_NAMESPACE


/* extern function definition */
/**
 * \~english
 * \~japanese
 *   \brief クリップボードを設定します
 */
extern void LIB_SetClipBoard(const Char* text);


ADK_END_NAMESPACE


#endif  /* LIB_CLIPBOARD_H_ */


