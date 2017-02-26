/**
 * \file lib_url_encode.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief URLエンコードライブラリ
 */


#ifndef LIB_URL_ENCODE_H_
#define LIB_URL_ENCODE_H_


#include <core/types.h>
#include <debug_profiling/error/result_types.h>


ADK_BEGIN_NAMESPACE


/* extern function definition */
/**
 * \~english
 * \~japanese
 *   \brief 文字列をURLエンコードフォーマットにエンコードします
 *   \param[out] dst エンコード後の文字列格納先
 *   \param[in] str エンコードする文字列
 *   \return エンコードの成功判定。以下の値が返ります
 *   \retval null以外  エンコードに成功したときに返ります\n
 *                       この値は \ref ADK_MemFree() で解放する必要があります
 *   \retval null    エンコードに失敗したときに返ります
 */
extern Result LIB_URLEncode( Char **dst, const Char *str);
/**
 * \~english
 * \~japanese
 *   \brief \ref LIB_URLEncode() で作成された文字列を削除します
 *   \param[in] str 削除する文字列
 */
extern Result LIB_URLDispose( Char *str);


ADK_END_NAMESPACE


#endif  /* LIB_URL_ENCODE_H_ */


