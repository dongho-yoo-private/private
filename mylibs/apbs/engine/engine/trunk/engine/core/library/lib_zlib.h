/**
 * \author dongho yoo
 * \file LIB_ZLib.h
 * \~english
 * \~japanese
 * 	\brief Linked List Library.
 */

#ifndef __LIB_ZLIB_H__
#define __LIB_ZLIB_H__

#include <core/types.h>

#ifndef __cplusplus
#define EXTERN_Z extern "C"
#else
#define EXTERN_Z extern
#endif

// 何故この値なのか忘れちゃった。
// スピードと関係あったような。。
/**@brief 内部で一度に処理するバッファーサイズ
*/
#define _ZLIB_CHUNK_ (1024)

ADK_BEGIN_NAMESPACE

/**@brief zlibで圧縮します。
*
*@param [in] p 元データ
*@param [in] size 元データサイズ
*@param [out] compressedSize 変換後のサイズ
*@param [out] fProcess 処理のインジケーター、0を指定すると無視されます。
*
*@return 圧縮されたデータが返ります。
*@retval 0の場合は、圧縮失敗
*/
void* LIBZ_Compress(const void* source, S32 size, S32* compressedSize, float* fProcess);

/**@brief 圧縮したデータを解凍します。
*
*@param [in] source 圧縮されたデータ
*@param [in] size 圧縮されたデータサイズ
*@param [out] extractedSize変換後のサイズ
*@param [out] fProcess 処理のインジケーター、0を指定すると無視されます。
*
*@return 圧縮されたデータが返ります。
*@retval 0の場合は、解凍失敗
*/
void* LIBZ_DeCompress(const void* source, U32 size, U32* extractedSize, float* fProcess);

/**@brief zipデータがzipやgzip形式でなく、raw形式の場合の圧縮したデータを解凍します。
*
*@param [in] source 圧縮されたデータ
*@param [in] size 圧縮されたデータサイズ
*@param [out] extractedSize変換後のサイズ
*@param [out] fProcess 処理のインジケーター、0を指定すると無視されます。
*
*@return 圧縮されたデータが返ります。
*@retval 0の場合は、解凍失敗
*/
void* LIBZ_DeCompress4Raw(const void* source, U32 size, U32* extractedSize, float* fProcess);

/** @brief LIBZから割り当てられたメモリを解放します。
*
*@param p 割り当てられたデータ
*/
void LIBZ_Delete(void* p);

/**\brief ハンドル*/
ADK_HANDLE(HZIP);

/**@brief 圧縮したデータをオープンします。
 * \param data 解凍するバイナリデータ
 * \param size バイナリデータサイズ
 */
//TODO ファイル保存ではなく、メモリ上への確保に変更する
//HZIP LIBZ_UnzOpen(const void* data, S32 size);
HZIP LIBZ_UnzOpen(const char *path);
//unzFile ZEXPORT unzOpen (const char *path);

/**\brief LIBZ_UnzOpenで生成されたハンドルをクローズします。
 * \param hZip LIBZ_UnzOpenで生成されたハンドル
 */
void LIBZ_UnzClose(HZIP hZip);
//int ZEXPORT unzClose (unzFile file)

Bool LIBZ_UnzGoToFirstFile(HZIP hZip);
//int ZEXPORT unzGoToFirstFile (unzFile file);

/**\brief 現在のzipファイルの情報を取得します。
 *\param [in] hZip LIBZ_UnzipOpenから取得したハンドル
 *\param [out] filename ファイル名
 *\param [in] len ファイル名の収納バッファー
 *\param [out] 解凍後のファイルサイズ
 */
Bool LIBZ_UnzGetCurrentFileInfo(HZIP hZip, Char* filename, S32 len, S32* file_size);

Bool LIBZ_UnzGetCurrentFileInfo64(HZIP hZip,
                                 //                                unz_file_info64 * pfile_info,
                                 Char *szFileName, U32 fileNameBufferSize,
                                 void *extraField, U32 extraFieldBufferSize,
                                 Char *szComment,  U32 commentBufferSize);
//int ZEXPORT unzGetCurrentFileInfo64 (unzFile file,
//                                     unz_file_info64 * pfile_info,
//                                     char * szFileName, uLong fileNameBufferSize,
//                                     void *extraField, uLong extraFieldBufferSize,
//                                     char* szComment,  uLong commentBufferSize);

Bool LIBZ_UnzLocateFile(HZIP hZip, const char* szFileName, S32 iCaseSensitivity);
//int ZEXPORT unzLocateFile (unzFile file, const char *szFileName, int iCaseSensitivity)

Bool LIBZ_UnzOpenCurrentFile (HZIP hZip);
//int ZEXPORT unzOpenCurrentFile (unzFile file)

Bool LIBZ_UnzReadCurrentFile(HZIP hZip, void* buffer, U32 len);
//int ZEXPORT unzReadCurrentFile  (unzFile file, voidp buf, unsigned len)

Bool LIBZ_UnzCloseCurrentFile (HZIP hZip);
//int ZEXPORT unzCloseCurrentFile (unzFile file)

Bool LIBZ_UnzGoToNextFile (HZIP hZip);
//int ZEXPORT unzGoToNextFile (unzFile  file)

ADK_END_NAMESPACE

#endif
