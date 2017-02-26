/**
* Copyright (c) 2015 igotomo
* authorized dongho.yoo.
*
* @brief smtpプロトコル
* @startcode
* 
* igx_smtp_t smtp;
* igx_smtp_detail_t detail;
* igx_smtp_user_t user1, user2, user3;
* 
* igx_smtp_default_detail(&detail);
* addr = igx_get_host_name("servername");
* if (igx_smtp_create(&smtp, addr, port, &detail, &error)==0)
* {
    // TODO. error.
  }
* 
* // ユーザの設定
* igx_mime_user_t users[] = {
*    {"any-email@address.com", "displayname", 0, {1, 0, 0, 0}},
*    {"any-email2@address.com", "displayname2", {1, 0, 0, 0}},
*    {"any-email@address3.com", "displayname3", {0, 1, 0, 0}}
* };
*
* // 送るユーザ設定
* igx_smtp_add_user(&smtp, users, 3);
*
* // 送り主の設定
* igx_smtp_sender(&smtp, "my-email@address.net", "displayname", "id_for_smtp", "id_for_password");
* // タイトル
* igx_smtp_subject(&smtp, "subject", strlen("subject"));
* // メッセージ
* igx_smtp_message(&smtp, "body", strlen("body"));
* // 添付ファイル
* igx_smtp_add_file(&smtp, "filename", data, data_size);
* igx_smtp_add_file(&smtp, "filename2", data2, data_size2);
*
* if (igx_smtp_login(&smtp, "id", "password")!=kSmtpErrorSuccess)
* {
*     // TODO. error
* }
*
* 送信
* if (igx_smtp_send(&smtp)!=kSmtpErrorSuccess)
  {
    // TODO. error
  }
* igx_smtp_close(&smtp);
*
* @endcode
*/

#ifndef _IGX_SMTP_H_
#define _IGX_SMTP_H_

#include "igx_types.h"
#include "network/igx_sock_types.h"
#include "smtp_types.h"

#ifdef __cplusplus
#   define IGX_SMTP_EXTERN extern "C"
#else
#   define IGX_SMTP_EXTERN extern
#endif

/**
* @brief エラーメッセージを取得します。
* @param error_no エラー番号　
*/
IGX_SMTP_EXTERN const char* igx_smtp_error_message(igx_smtp_error_t error_no);
/**
* @brief smtpの詳細情報のデフォルトを設定します。
* @param detail @ref igx_smtp_detail_t
* @return detailのアドレス。
*/
IGX_SMTP_EXTERN igx_smtp_detail_t* igx_smtp_default_detail(igx_smtp_detail_t* detail);
/**
* @brief smtpを生成します。\n
* 内部的にはサーバへ接続を行います。\n
*
* @param [out] smtp ハンドル
* @param addr  接続先のアドレス
* @param detail 0の場合はデフォルト設定 @ref igx_stmp_detail_t
*
* @return @ref igx_smtp_error_t
*/
IGX_SMTP_EXTERN igx_smtp_error_t igx_smtp_create(igx_smtp_t* smtp, igx_addrv4_t addr, igx_smtp_detail_t* detail);
/**
* @brief ログインを行います。
* @param smtp smtpハンドル
* @param id ID
* @param password パスワード　
* 
* @return @ref igx_smtp_error_t
* @retval kSmtpErrorSuccess 成功
* @retval kSmtpErrorLogin   'AUTH LOGIN'がサポートされていない。
* @retval kSmtpErrorID       IDが間違っている
* @retval kSmtpErrorPassword PASSWORDが間違っている
* @retval kSmtpErrorOther    不明
*/
IGX_SMTP_EXTERN igx_smtp_error_t igx_smtp_login(igx_smtp_t* smtp, const char* id, const char* password);
/**
* @brief TOにユーザを追加します。
* @param smtp ハンドル
* @param user ユーザー情報の配列
* @param user_count 配列の数
* @return 追加されたユーザの数
*/
IGX_SMTP_EXTERN bit32_t igx_smtp_add_user(igx_smtp_t* smtp, igx_mime_user_t* user, const bit32_t user_count);
/*
* @brief 送信者の情報を設定します。
* @param smtp ハンドル
* @param email_address 送信者のメールアドレス
* @param email_address 送信者の表示名
*/
IGX_SMTP_EXTERN void igx_smtp_sender(igx_smtp_t* smtp, const char* email_address, const char* display_name);
/**
* @brief タイトルを設定します。
* @param smtp ハンドル
* @param subject タイトル
* @param subject_size タイトルの長さ
*/
IGX_SMTP_EXTERN void igx_smtp_subject(igx_smtp_t* smtp, const char* subject, const size_t subject_size);
/**
* @brief 本文を設定します。
* @param smtp ハンドル
* @param body 本文
* @param body_size 本文のサイズ
*/
IGX_SMTP_EXTERN void igx_smtp_message(igx_smtp_t* smtp, const char* body, const size_t body_size);
/**
* @brief ファイルを追加します。内部的にはbase64にエンコードされた後、添付されます。　
* @param filename 添付した際に表示されるファイル名（ロードするファイル名ではない)
* @param data データの戦闘ポインター
* @param data_size データのサイズ
* 
* @return 現在のファイルの数を返します。　
*/
IGX_SMTP_EXTERN bit32_t igx_smtp_add_file(igx_smtp_t* smtp, const char* filename, const void* data, const size_t size);
/**
* @brief メールを送信します。
* @param smtp ハンドル
* 
* @return @brief igx_smtp_error_t
*/
IGX_SMTP_EXTERN igx_smtp_error_t igx_smtp_send(igx_smtp_t* smtp);
/*
* @brief ハンドルを解放します。
* @param smtp ハンドル
*/
IGX_SMTP_EXTERN void igx_smtp_close(igx_smtp_t* smtp);

#endif /* _IGX_SMTP_H_ */
