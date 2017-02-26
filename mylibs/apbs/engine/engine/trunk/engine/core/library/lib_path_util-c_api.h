/**
 * \file lib_path_util_c-api.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief パスユーティリティ for CApi
 */


#ifndef LIB_PATH_UTIL_CAPI_H_
#define LIB_PATH_UTIL_CAPI_H_


#include <core/types.h>


#ifdef __cplusplus
extern "C" {
#endif


/* extern function definition */
/* utility */
/**
 * \~english
 * \~japanese
 *   \brief 文字列をディレクトリとファイル名に分割する
 *   \param dir [out] ディレクトリ格納先
 *   \param file [out] ファイル名格納先
 *   \param dirlen [in] dir の最大文字列長
 *   \param filelen [in] file の最大文字列長
 *   \param str [in] 分割する文字列
 *   \return 分割の成功判定。以下の値が返ります。
 *   \retval Success  分割に成功したときに返ります。
 *   \retval Failure  分割に失敗したときに返ります。
 */
extern Bool ADKLIB_SplitPathDirFile(
    Char *dir,
    Char *file,
    U32 dirlen,
    U32 filelen,
    const Char *str);
/**
 * \~english
 * \~japanese
 *   \brief 文字列をディレクトリとファイル名と拡張子に分割する
 *   \param dir [out] ディレクトリ格納先
 *   \param file [out] ファイル名格納先
 *   \param ext [out] 拡張子格納先
 *   \param dirlen [in] dir の最大文字列長
 *   \param filelen [in] file の最大文字列長
 *   \param extlen [in] ext の最大文字列長
 *   \param str [in] 分割する文字列
 *   \return 分割の成功判定。以下の値が返ります。
 *   \retval Success  分割に成功したときに返ります。
 *   \retval Failure  分割に失敗したときに返ります。
 */
extern Bool ADKLIB_SplitPathDirFileExt(
    Char *dir,
    Char *file,
    Char *ext,
    U32 dirlen,
    U32 filelen,
    U32 extlen,
    const Char *str);
/**
 * \~english
 * \~japanese
 */
extern void ADKLIB_JoinDirFile(
    Char *dst,
    const U32 maxlen,
    const Char *dir,
    const Char *file);

/* device path information management functionally */
/**
 * \~english
 * \~japanese
 *   \brief 共有リソース格納先のディレクトリパスを返します
 *   \param[out] dst パスの格納先
 *   \param[in] dstlen 格納先の最大長
 *   \return 取得の成功判定。以下の値が返ります
 *   \retval true  取得に成功したときに返ります
 *   \retval false  取得に失敗したときに返ります
 */
extern Bool ADKLIB_GetSystemDir( Char *dst, U32 dstlen);
/**
 * \~english
 * \~japanese
 *   \brief ユーザー情報格納先のディレクトリパスを返します
 *   \param[out] dst パスの格納先
 *   \param[in] dstlen 格納先の最大長
 *   \return 取得の成功判定。以下の値が返ります
 *   \retval true  取得に成功したときに返ります
 *   \retval false  取得に失敗したときに返ります
 */
extern Bool ADKLIB_GetUserDir( Char *dst, U32 dstlen);
/**
 * \~english
 * \~japanese
 *   \brief セーブデータ格納先のディレクトリパスを返します
 *   \param[out] dst パスの格納先
 *   \param[in] dstlen 格納先の最大長
 *   \return 取得の成功判定。以下の値が返ります
 *   \retval true  取得に成功したときに返ります
 *   \retval false  取得に失敗したときに返ります
 */
extern Bool ADKLIB_GetSaveDataDir( Char *dst, U32 dstlen);
/**
 * \~english
 * \~japanese
 *   \brief リソースファイル格納先のディレクトリパスを返します
 *   \param[out] dst パスの格納先
 *   \param[in] dstlen 格納先の最大長
 *   \return 取得の成功判定。以下の値が返ります
 *   \retval true  取得に成功したときに返ります
 *   \retval false  取得に失敗したときに返ります
 */
extern Bool ADKLIB_GetResourceDir( Char *dst, U32 dstlen);
/**
 * \~english
 * \~japanese
 *   \brief 一時ファイル格納先のディレクトリパスを返します
 *   \param[out] dst パスの格納先
 *   \param[in] dstlen 格納先の最大長
 *   \return 取得の成功判定。以下の値が返ります
 *   \retval true  取得に成功したときに返ります
 *   \retval false  取得に失敗したときに返ります
 */
extern Bool ADKLIB_GetTemporaryDir( Char *dst, U32 dstlen);
/**
 * \~english
 * \~japanese
 *   \brief デスクトップパスのディレクトリパスを取得します
 *   \param[out] dst パスの格納先
 *   \param[in] dstlen 格納先の最大長
 *   \return 取得の成功判定。以下の値が返ります
 *   \retval true  取得に成功したときに返ります
 *   \retval false  取得に失敗したときに返ります
 */
extern Bool ADKLIB_GetDesktopDir( Char *dst, U32 dstlen);


#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif  /* LIB_PATH_UTIL_CAPI_H_ */


