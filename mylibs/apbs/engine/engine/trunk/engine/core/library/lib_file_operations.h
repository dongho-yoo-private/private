/**
 * \file lib_file_operations.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief ファイル/ディレクトリ操作
 */


#ifndef LIB_FILE_OPERATIONS_H_
#define LIB_FILE_OPERATIONS_H_


#include <dirent.h>

#include <core/types.h>
#include <core/library/lib_file_types.h>


ADK_BEGIN_NAMESPACE


/* extern function definition */
/* common functionally */
/**
 * \~english
 * \~japanese
 *   \brief ファイル/ディレクトリが存在するかを返します。
 *   \param name [in] ファイル/ディレクトリ名
 *   \return 存在するかどうか。以下の値が返ります。
 *   \retval true  存在するときに返ります。
 *   \retval false  存在しないときに返ります。
 */
extern Bool LIB_FileExists( const Char *name);
/**
 * \~english
 * \~japanese
 *   \brief ファイル/ディレクトリをコピーします。
 *   \param from [in] コピー元のファイル/ディレクトリ名
 *   \param to [in] コピー先のファイル/ディレクトリ名
 *   \return コピーの成功判定。以下の値が返ります。
 *   \retval true  コピーに成功したときに返ります。
 *   \retval false  コピーに失敗したときに返ります。
 */
extern Bool LIB_FileCopy( const Char *from, const Char *to);
/**
 * \~english
 * \~japanese
 *   \brief ファイル/ディレクトリの名前を変更します。
 *   \param from [in] 名前を変更するファイル/ディレクトリ名
 *   \param to [in] 変更するファイル/ディレクトリ名
 *   \return 変更の成功判定。以下の値が返ります。
 *   \retval true  変更に成功したときに返ります。
 *   \retval false  変更に失敗したときに返ります。
 */
extern Bool LIB_FileRename( const Char *from, const Char *to);
/**
 * \~english
 * \~japanese
 *   \brief ファイル/ディレクトリを削除します。
 *   \param name [in] 削除するファイル/ディレクトリ名
 *   \param dir [in] ディレクトリなら 'true' を設定します。\n
 *                   内部でファイル状態を見ればファイル/ディレクトリを判断できるが処理が長くなるのでココで設定してもらうようにする。
 *   \param recursive [in] ディレクトリの場合、下位ディレクトリも削除するなら 'true' を設定します。
 *   \return 削除の成功判定。以下の値が返ります。
 *   \retval true  削除に成功したときに返ります。
 *   \retval false  削除に失敗したときに返ります。
 */
extern Bool LIB_FileDelete( const Char *name, Bool dir, Bool recursive);
/**
 * \~english
 * \~japanese
 *   \brief ファイル/ディレクトリを移動します。
 *   \param from [in] 移動元のファイル/ディレクトリ名
 *   \param to [in] 移動先のファイル/ディレクトリ名
 *   \return 移動の成功判定。以下の値が返ります。
 *   \retval true  移動に成功したときに返ります。
 *   \retval false  移動に失敗したときに返ります。
 */
extern Bool LIB_FileMove( const Char *from, const Char *to);
/**
 * \~english
 * \~japanese
 *   \brief ファイル/ディレクトリの状態を取得します。
 *   \param stat [out] 状態格納先
 *   \param name [in] 取得するファイル名
 *   \return 取得の成功判定。以下の値が返ります。
 *   \retval true  取得に成功したときに返ります。
 *   \retval false  取得に失敗したときに返ります。
 */
extern Bool LIB_FileStat( LIB_TFileStatus *out, const Char *name);

/* directories management functionally */
/**
 * \~english
 * \~japanese
 *   \brief パスがディレクトリかを返します。
 *   \param path [in] パス
 *   \return ディレクトリかどうか。以下の値が返ります。
 *   \retval true  パスがディレクトリのときに返ります。
 *   \retval false  パスがディレクトリでないときに返ります。
 */
extern Bool LIB_IsDir( const Char *path);
/**
 * \~english
 * \~japanese
 *   \brief ディレクトリを作成します。\n
 *          存在しない階層を指定した場合それも作成します。
 *   \param dir [in] ディレクトリパス
 *   \return 作成の成功判定。以下の値が返ります。
 *   \retval true  作成に成功したときに返ります。
 *   \retval false  作成に失敗したときに返ります。
 */
extern Bool LIB_FileCreateDir( const Char *dir);
/**
 * \~english
 * \~japanese
 *   \brief ディレクトリの空き領域を返します。
 *   \param dir [in] ディレクトリパス
 *   \return 空き領域
 */
extern U64 LIB_FileDirFreeSpace( const Char *dir);
/**
 * \~english
 * \~japanese
 *   \brief アクセス許可を行って生成した LIB_TFileKey 構造体を破棄します。
 *   \param key[in]  破棄する認証済みのファイル情報
 *   \retval ktrue  破棄に成功したときに返ります。
 *   \retval kfalse  破棄に失敗したときに返ります。
 */
extern Bool LIB_FileDisposeKey( LIB_TFileKey *key);
/**
 * \~english
 * \~japanese
 *   \brief 
 */
extern S32 LIB_DirContents( struct LIB_TDirEntry **filelist, const Char *dirname);
/**
 * \~english
 * \~japanese
 *   \brief 
 */
extern void LIB_FreeDirContents( struct LIB_TDirEntry *filelist, U32 contentCount);

/* files management functionally */
/**
 * \~english
 * \~japanese
 *   \brief パスがファイルかを返します。
 *   \param path [in] パス
 *   \return ファイルかどうか。以下の値が返ります。
 *   \retval true  パスがファイルのときに返ります。
 *   \retval false  パスがファイルでないときに返ります。
 */
extern Bool LIB_IsFile( const Char *path);
/**
 * \~english
 * \~japanese
 *   \brief ファイルを開きます。\n
 *          ダウンロードコンテンツファイルを開く場合は引数に LIB_TFileKey を指定する LIB_FileOpen() を使用してください。
 *   \param filename [in] ファイル名
 *   \param type [in] 開く際のファイルタイプ \ref LIB_TFileType を設定します。
 *   \return ファイルオブジェクトが返ります。
 *   \retval null  ファイルを開くのに失敗したときに返ります。
 */
extern LIB_TFileObject *LIB_FileOpen( const Char *filename, LIB_TFileType type);
/**
 * \~english
 * \~japanese
 *   \brief ファイルを閉じます
 *   \param fileobj [in] ファイルオブジェクト
 *   \return 閉じれたかどううかの成功判定。以下の値が返ります。
 *   \retval true  閉じるのに成功したときに返ります。
 *   \retval false  閉じるのに失敗したときに返ります。
 */
extern Bool LIB_FileClose( LIB_TFileObject *fileobj);
/**
 * \~english
 * \~japanese
 *    \brief ファイルを読み込みます。
 *   \param buf [out] 読み込み先のバッファ
 *   \param fileobj [in] ファイルオブジェクト
 *   \param size [in] 読み込みサイズ
 *   \return 読み込んだサイズが返ります。
 */
extern U64 LIB_FileRead( void *buf, LIB_TFileObject *fileobj, U64 size);
/**
 * \~english
 * \~japanese
 *   \brief ファイルに書き込みます。
 *   \param fileobj [in] ファイルオブジェクト
 *   \param buf [in] 書き込み元のバッファ
 *   \param size [in] 書き込むサイズ
 *   \return 書き込みの成功判定。以下の値が返ります。
 *   \retval true  書き込みに成功したときに返ります。
 *   \retval false  書き込みに失敗したときに返ります。
 */
extern Bool LIB_FileWrite( LIB_TFileObject *fileobj, const void *buf, U64 size);
/**
 * \~english
 * \~japanese
 *   \brief ファイル参照位置をシークします。
 *   \param fileobj [in] ファイルオブジェクト
 *   \param seek [in] シーク位置
 *   \return シークの成功判定。以下の値が返ります。
 *   \retval true  シークに成功したときに返ります。
 *   \retval false  シークに失敗したときに返ります。
 */
extern Bool LIB_FileSeek( LIB_TFileObject *fileobj, S64 seek, LIB_TFileWhence whence);
/**
 * \~english
 * \~japanese
 *   \brief ファイル最後尾の位置を返します
 *   \param fileobj [in] ファイルオブジェクト
 *   \return 位置
 */
extern U64 LIB_FileTell( LIB_TFileObject *fileobj);
/**
 * \~english
 * \~japanese
 *   \brief ファイルを切り詰めます
 *   \param fileobj [in] ファイルオブジェクト
 *   \param size [in] サイズ
 *   \return 切り詰めの成功判定。以下の値が返ります。
 *   \retval true  切り詰めに成功したときに返ります。
 *   \retval false  切り詰めに失敗したときに返ります。
 */
extern Bool LIB_FileTruncate( LIB_TFileObject *fileobj, U64 size);
/**
 * \~english
 * \~japanese
 *   \brief ファイルサイズを返します。
 *   \param fileobj [in] ファイルオブジェクト
 *   \return バイト単位のサイズ
 */
extern U64 LIB_FileSize( LIB_TFileObject *fileobj);

ADK_END_NAMESPACE

#endif  /* LIB_FILE_OPS_H_ */
