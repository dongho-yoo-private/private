/**
* Copyright (c) 2015 igotomo
* authorized dongho.yoo.
*
* @brief smtpプロトコル
*/
#ifndef _IGX_SMTP_TYPES_H_
#define _IGX_SMTP_TYPES_H_

/* 詳細オプション */
typedef struct {
    /**@brief コネクトタイムアウト */
    bit16_t connect_timeout;
    /**@brief レシーブのタイムアウト*/
    bit16_t read_timeout;
    /**@brief 文字コード*/
    const char* charset;
    struct {
        /**@brief id (エンコードしたID)*/
        char* id;
        /**@brief paswword (エンコードしたパスワード)*/
        char* password;
    } login;
    struct {
        /*@brief sslを使うかどうか、デフォルト1*/
        unsigned use_ssl:1;
        unsigned use_mem_page:1;
        unsigned reserved:14;
        /*@brief ポート番号、デフォルト587、SSLでない場合は25*/
        unsigned port:16;
    };
} igx_smtp_detail_t;

/** mime形式のユーザ名　'any@email.com<dislya_name>' */
typedef struct {
    /** @brief メールアドレス */
    const char* mail_address;
    /** @brief 表示名 */
    const char* display_name;
    /** @brief receiverのタイプ */
    struct {
        unsigned to:1;
        unsigned cc:1;
        unsigned bcc:1;
        unsigned reserved:29;
    } type;
} igx_mime_user_t;
/**
* @brief smtpのエラー一覧*/
typedef enum {
    /**@brief 成功 */
    kSmtpErrorSuccess = 0,
    /**@brief コネクトエラー*/
    kSmtpErrorConnect,
    /**@brief 送信エラー*/
    kSmtpErrorWrite,
    /**@brief 受信エラー*/
    kSmtpErrorRead,
    /**@brief サーバ側エラー*/
    kSmtpErrorServer,
    /**@brief ログインエラー*/
    kSmtpErrorLogin,
    /**@brief ID不正*/
    kSmtpErrorId,
    /**@brief パスワード不正*/
    kSmtpErrorPassword,
    /**@brief TOエラー*/
    kSmtpErrorRcptTo,
    /**@brief MAIL FROMエラー*/
    kSmtpErrorMailFrom,
    /**@brief ボディ送信エラー*/
    kSmtpErrorData,
    /**@brief 終了エラー*/
    kSmtpErrorQuit,
    /**@brief その他エラー*/
    kSmtpErrorOther=-1,
} igx_smtp_error_t;

typedef struct {
    ubit8_t __unused__[256];
} igx_smtp_t;

#endif
