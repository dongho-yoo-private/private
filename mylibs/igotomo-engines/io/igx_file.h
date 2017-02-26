/**
* Copyright (c) 2015
* Authorized by dongho.yoo.
*/
#ifndef __IGX_FILE_H__
#define __IGX_FILE_H__

#include "igx_types.h"
#include "igx_encrypt.h"

#ifdef __cplusplus
#   define IGX_FILE_EXTERN extern "C"
#else
#   define IGX_FILE_EXTERN extern
#endif

#ifdef __POSIX__
#include <sys/stat.h>
#include <sys/types.h>
typedef enum 
{
	kFileMode777 = S_IRWXU|S_IRWXG|S_IRWXO,
	kFileMode755 = (kFileMode777&~S_IWOTH)&~S_IWGRP,
	kFileMode666 = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH,
	kFileMode444 = S_IRUSR|S_IRGRP|S_IROTH, /* ReadOnly*/
	kFileMode400 = S_IRUSR, 
	kFileMode644 = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH, 
	kFileMode664 = S_IRUSR|S_IWUSR|S_IWGRP|S_IRGRP|S_IROTH|S_IWOTH,
	kFileMode600 = S_IRUSR|S_IWUSR
} igx_file_mode_t;
#endif

#ifdef WIN32
typedef enum {
    kFileMode777 = (GENERIC_READ|GENERIC_WRITE),
	kFileMode755 = kFileMode777, 
	kFileMode666 = kFileMode777,
	kFileMode444 = (GENERIC_READ),
	kFileMode400 = kFileMode444,
	kFileMode644 = kFileMode777,
	kFileMode664 = kFileMode777,
	kFileMode600 = kFileMode777
} igx_file_mode_t;

#endif
/**
* @brief 読み書き時にエンコードおよびデコードが必要な場合使います。
*/
typedef struct {
    void* (*encrypt_func)(void* ,size_t* ,const void* ,size_t ,ubit32_t);
    void* (*decrypt_func)(void* ,size_t* ,const void* ,size_t ,ubit32_t);
    int   (*checksum_func)(void* ,size_t);
    ubit32_t key;
} igx_file_security_t;

/**
* @brief ファイルを読み込みます。
*
* @param _file_name ファイル名
* @param buffer バッファー
* @param index 読み込むファイルポインター
* @param size  読み込むサイズ（バッファーサイズ), ゼロの場合は全体を読み込みます。
*
* @return 読み込んだファイルのバイト数
* @retval -1 エラー
*/
IGX_FILE_EXTERN bit32_t igx_file_read(const char* file_name, void* buffer, int index, size_t size);
/**
* @brief ファイルを書き込みます。
*
* @param _file_name ファイル名
* @param buffer バッファー
* @param index 書き込むファイルポインター
* @param size  書き込むサイズ（バッファーサイズ)
*
* @return 書き込んだファイルのバイト数
* @retval -1 エラー
*/
IGX_FILE_EXTERN bit32_t igx_file_write(const char* file_name, const void* buffer, int index, size_t size);
/**
* @brief ファイルの後ろに追加します。
*
* @param _file_name ファイル名
* @param buffer バッファー
* @param size  書き込むサイズ（バッファーサイズ)
*
* @return 書き込んだファイルのバイト数
* @retval -1 エラー
*/
IGX_FILE_EXTERN bit32_t igx_file_append(const char* file_name, void* buffer, size_t size);
/**
* @brief ファイルを書き込みます。
*
* @param _file_name ファイル名
* @param [out] buffer バッファー(NULLの場合はヒープ上に割り当てられます)
* @param [out] size  読み込んだファイルのサイズ
* @param [in] secur  エンコード・デコードするメソッド(NULLの場合は無視される)
*
* @return ファイルの内容
* @retval NULL エラー
*/
IGX_FILE_EXTERN void* igx_file_load(const char* file_name, void* buffer, size_t* size, igx_file_security_t* secur);
/**
* @brief ファイルをmmapでロードします。\n
* 速度面では遅いのだが、ファイルが巨大な場合などはこちらを使用します。
* @param [in] file_name ファイル名
* @param [out] size  読み込んだファイルのサイズ
* @param [in] secur  エンコード・デコードするメソッド(NULLの場合は無視される)
* @return ファイルの内容
*/
IGX_FILE_EXTERN void* igx_file_vload(const char* file_name, size_t* size, igx_file_security_t* secur);
/**
* @brief ファイルを書き込みます。
*
* @param _file_name ファイル名
* @param buffer バッファー
* @param size  書き込むサイズ（バッファーサイズ)
* @param [in] secur  エンコード・デコードするメソッド(NULLの場合は無視される)
*
* @return 書き込んだファイルのバイト数
* @retval -1 エラー
*/
IGX_FILE_EXTERN size_t igx_file_write_s(const char* file_name, const void* buffer, size_t size, igx_file_security_t* secur);
/**
* @brief デフォルトのencryptorやdecryptorが設定された構造体を返します。
*/
IGX_FILE_EXTERN void igx_get_default_security(igx_file_security_t* security, ubit32_t key);
/**
* @brief igx_file_loadで読み込んだメモリを削除します。
*/
IGX_FILE_EXTERN void igx_file_unload(void*);
/**
* @brief igx_file_vloadでロードしたメモリを解放します。
*/
IGX_FILE_EXTERN void igx_file_unvload(void* p);
/**
* @brief 最後のエラーを取得します。
*/
IGX_FILE_EXTERN const char* igx_file_get_last_error(ubit32_t* error_no);
/**
* @brief ディレクトリを作成します。
*
* @param path パス
* @param mode 権限
*
* @return true成功、false失敗
*/
IGX_FILE_EXTERN bool_t igx_file_mkdir(const char* path, igx_file_mode_t mode);
/**
* @brief ファイルを複製します。
*
* @param path dest_path コピー先
* @param mode src_path  コピーもと
* @return true成功、false失敗
*/
IGX_FILE_EXTERN bool_t igx_file_copy(const char* dest_path, const char* src_path);
/**
* @brief ファイル名を変更します。
*
* @param path dest_path 変更時のパス
* @param mode src_path  変更前のパス
* @return true成功、false失敗
*/
IGX_FILE_EXTERN bool_t igx_file_move(const char* dest_path, const char* src_path);
/**
* @brief ファイルを削除します。
*
* @param path ファイル名
* @return true成功、false失敗
*/
IGX_FILE_EXTERN bool_t igx_file_remove(const char* path);
/**
* @brief 書き込み可能なパスを取得します。
*
* @return path パス
*/
IGX_FILE_EXTERN const char* igx_file_writable_path();
/**
* @brief ファイルサイズを取得します。
*
* @param filename フィアル名
* @return ファイルサイズ
*/
IGX_FILE_EXTERN size_t igx_file_size(const char* filesize);
/**
* @brief 特定のディスクリプターから文字列を読み込みます。
* @param from_fd ディスクリプター
* @param buffer 受け取るバッファー
* @param buffer_size バッファーサイズ
* @param size       受け取った文字列のサイズ(0)で無視
* @param break_ch この文字列が入っている場合にはブレイクします。
*/
IGX_FILE_EXTERN char* igx_get_string_from_io(int from_fd, char* buffer, size_t buffer_size, size_t* size, char break_ch);
#endif
