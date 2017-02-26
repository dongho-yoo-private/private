/**
 * \file lib_string.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief 文字列操作ライブラリ
 */


#ifndef LIB_STRING_H_
#define LIB_STRING_H_


#include <core/types.h>


ADK_BEGIN_NAMESPACE


/* extern function definition */
/* decode management functionally */
/**
 * \~english
 * \~japanese
 *   \brief 文字列を複製して返します
 *   \param[in] str 複製元の文字列
 *   \return 文字列
 */
extern Char *LIB_strdup( const Char *str);
/**
 * \~english
 * \~japanese
 *   \brief 文字列を複製して返します
 *   \param[in] str 複製元の文字列
 *   \return 文字列
 */
extern Char *LIB_strdupcat( const Char *str1, const char *str2);
/**
 * \~english
 * \~japanese
 *   \brief 文字列長を返します
 *   \param[in] str 計測する文字列
 *   \return 文字列長
 */
extern S32 LIB_strlen( const Char *str);
/**
 * \~english
 * \~japanese
 *   \brief 文字列長を返します
 *   \param[in] str 計測する文字列
 *   \return 文字列長
 */
extern S32 LIB_strnlen( const Char *str, S32 maxlen);
/**
 * \~english
 * \~japanese
 *   \brief 文字列のコピーを行います
 *   \param[out] dst コピー先の文字列
 *   \param[in] dstlen コピー先の最大長
 *   \param[in] src コピー元の文字列
 *   \return コピーの成功判定
 */
extern Char *LIB_strncpy( Char *dst, const Char *src, S32 dstlen);

/**
 * \~english
 * \~japanese
 *   \brief strstrの大小文字区別なしバージョン
 *   \param[in] source   検索元文字列
 *   \param[in] key    検索文字列
 *   \return keyが見つかった所のアドレス
 *  \retval null時はkeyが見つかっていない。
 */
extern Char *LIB_strstri( const Char *source, const Char *key);
/**
 * \~english
 * \~japanese
 *   \brief strcmp
 *   \param[in] str1 比較文字列1
 *   \param[in] str2 比較文字列2
 *   \return 比較差分
 *  \retval 0 同じ
 *  \retval str1-str2の差分
 */
extern S32 LIB_strcmp( const Char *str1, const Char *str2);
/**
 * \~english
 * \~japanese
 *   \brief strcmpの大小文字区別なしバージョン
 *   \param[in] str1 比較文字列1
 *   \param[in] str2 比較文字列2
 *   \return 比較差分
 *  \retval 0 同じ
 *  \retval str1-str2の差分
 */
extern S32 LIB_strcmpi( const Char *str1, const Char *str2);
/**
 * \~english
 * \~japanese
 *   \brief strcmp
 */
extern S32 LIB_natstrcmp( const Char *str1, const Char *str2);
/**
 * \~english
 * \~japanese
 *   \brief 文字列のendの前までコピーするか0までコピーする関数
 *   \param[in] str1 比較文字列1
 *   \param[in] str2 比較文字列2
 *   \return dest
 */
extern Char *LIB_strcpyu( Char *dest, const Char *source, const Char* end);

/**
 * \~english
 * \~japanese
 *   \brief 文字列のendの前までの文字列の長さを返します。
 *   \param[in] source 文字列
 *   \return 文字列の長さ
 */
extern U32 LIB_strlenu( const Char *source, const Char* end);



ADK_END_NAMESPACE


#endif  /* LIB_STRING_H_ */


