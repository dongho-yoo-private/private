/**
* Copyright (c) 2015
*
* Authorized by dongho.yoo.
*/
#ifndef __IGX_ENCRIPT_H__
#define __IGX_ENCRIPT_H__

#include "igx_types.h"

#ifdef __cplusplus
#   define IGX_ENCRYPT_EXTERN extern "C"
#else
#   define IGX_ENCRYPT_EXTERN extern 
#endif

/**
* @brief 使用するエンコードのバージョン
*/
IGX_ENCRYPT_EXTERN void igx_encrypt_init(ubit32_t version);
/**
* @brief エンコードアルゴリズム、エンコードはソースの長さ×2+16バイト以下になります。
* @param dest 出力先バッファー
* @param dest_size 出力された長さ
* @param src 
* @param src_size
* @param key
*
* @return ゼロの場合はエラー
*
* 1. keyとのマスクしたビットを先頭から最後のビットを交換していく。
* 2. シフトさせて２倍にサイズを増やしておく
* 3. ランダムキー16ビットを生成する。
* 4. ランダムキーの上位8バイト、下位8バイトで埋めていく (これで、ゼロの場合も何かエンコードされる)
* 5. ヘッダーとチェックサムを埋め込む。
*/
IGX_ENCRYPT_EXTERN void* igx_encrypt(void* dest, size_t* dest_size, const void* src, size_t src_size, ubit32_t key);
/**
* @brief エンコードアルゴリズム
* @param dest
* @param src
* @param src_size
* @param key
*
* @return ゼロの場合はエラー
*/
IGX_ENCRYPT_EXTERN void* igx_decrypt(void* dest, size_t* dest_size, const void* src, size_t src_size, ubit32_t key);
/**
* @brief ビット単位でシャッフルします。
* @param dest
* @param src
* @param src_size
* @param key
*
* @return ゼロの場合はエラー
*/
IGX_ENCRYPT_EXTERN void igx_shuffle(void* dest, const void* src, size_t src_size, ubit32_t key);
/**
* @brief シャッフルされたデータを戻します。
* @param dest
* @param src
* @param src_size
* @param key
*
* @return ゼロの場合はエラー
*/
IGX_ENCRYPT_EXTERN void igx_restore(void* dest, const void* src, size_t src_size, ubit32_t key);
/**
* @brief 最後に発生したエラーコードを取得します。
*
* @param エラーコード（NULLの場合は無視されます。)
* @return エラーの文字列
*/
IGX_ENCRYPT_EXTERN const char* igx_encrypt_last_error(ubit32_t* error_no);
/**
* @brief エンコード後のサイズを取得します。
*
* @return サイズ
*/
IGX_ENCRYPT_EXTERN size_t igx_encrypted_size(size_t original_size);
/**
* @brief base64フォーマットに出力します。
*
* @param dest 格納するバッファー（NULLの場合はheapから割り当てられます。)
* @param out_size エンコード後のサイズ
* @param src エンコードを行うデータの先頭アドレス
* @param src_size データのサイズ
* 
* @return dest
*/
IGX_ENCRYPT_EXTERN void* igx_encrypt_base64(void* dest, size_t* out_size, const void* src, size_t src_size);
/**
* @brief base64形式のデータを戻します。
*
* @param dest 格納するバッファー（NULLの場合はheapから割り当てられます。)
* @param out_size でコード後のサイズ
* @param src でコードを行うデータの先頭アドレス
* @param src_size データのサイズ
* 
* @return dest
*/
IGX_ENCRYPT_EXTERN void* igx_decrypt_base64(void* dest, size_t* out_size, const void* src, size_t src_size);
/**
* @brief urlエンコードに圧縮します。(要らないかな。。。)
*/
IGX_ENCRYPT_EXTERN void* igx_url_encode(void* dest, size_t* out_size, const void* src, size_t src_size);
/**
* @brief ハッシュキーを生成します。
*
* @param str 文字列 (長さが正しければ、文字列である必要はない)
* @param len 長さ
*
* @return 32bitのハッシュキー
*/
IGX_ENCRYPT_EXTERN ubit32_t igx_hash_key_multiple_plus(const char* str, size_t len);
#endif
