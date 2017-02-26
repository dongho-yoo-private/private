/**
* Copyright (C) 2015 igotomo
* Authorized by dongho.yoo.
*
* @brief (itp://) igotomo protocol
*/
#ifndef _IGX_GTP_
#define _IGX_GTP_

#include "igx_go_types.h"

#ifdef WIN32
#   pragma pack(push 1)
#   define PACKED 
#else
#   define PACKED __attribute__((__packed__))
#endif
namespace go { namespace itp {

/**
* @brief メールアドレスから変換された64ビットのハッシュキー\n
* よほど出ない限り重ならない。
*/
typedef ubit64_t user_id_t;

typedef struct {
    /**@brief 囲碁の申し込んだ時間*/
    igx_time_t  time;
    /**@brief メールアドレスから変換された64ビットのハッシュキー*/
    user_id_t user_id;
} uuid_t;

/**@brief イベントID\n
* 基本的に送受信兼用
*/
typedef enum {
    /**@brief 相手からの対局中のアクション*/
    kAddStone = 1,
    /**@brief 対局申し込み*/
    kRequestPlay,
    /**@brief フレンド申請*/
    kARequestFriend,
    /**@brief フレンドの承諾*/
    kAcceptFriend,
    /**@brief フレンド情報更新*/
    kChangedFriendInfo,
} event_id_t;
/**
* @brief itp共通ヘッダー\n
* 共通ヘッダーはタイトル名に従って暗号化されます。\n
*/
struct header_t
{
    /** @brief イベントID*/
    event_id_t id;
    /** @brief ユーザID*/
    user_id_t user_id;
    /** @brief ユーザの表示名*/
    char display_name[MAX_USER_DISPLAY_LEN+1]
    /** @brief ボディのサイズ*/
    bit32_t body_size;
} PACKED;
typedef struct header_t header_t; 

/**
* @brief AddStoneイベントボディ
*/
struct __add_stone__ {
    /**@brief uuid。このIDで碁盤を検索し、碁盤が開かれていたら更新します。*/
    uuid_t uuid;
    /**@brief 更新のあった石、stoneがゼロの場合はメッセージのみ*/
    stone_t stone;
    /**@brief ラベル*/
    label_t label;
    /**@brief 更新のあったコメントの長さ, コメントがない場合はゼロ*/
    size_t  comment_len;
    /**@brief */
    char   comment[16];
} PACKED;
typedef struct __add_stone__ add_stone_t;
/**
* @brief RequestPlayイベントボディ
*/
struct __request_play__ {
    /**@brief イベント情報 \ref go::event_t */
    event_t event;
    /**@brief string_poolのサイズ*/
    bit32_t size;
    /**@brief event_tで参照している文字列のバッファー*/
    char string_pool[16];
} PACKED;
typedef struct __request_play__ request_play_t;

extern request_play_t* make_request_play_body(char* buffer, size_t buffer_size, 
                                                const char* event_name, const char* place, const char* count, const char* comment, 
                                                    const rules_t& rules, const igx_time_t* time=0);


/**@brief このサイズを超えるメールアドレスは登録そのものが出来ない様にする\n
現実的にはどれくらいにすべきだろうか。。。*/
#define MAX_EMAIL_ADDRESS_SIZE (63)
/**
@brief まさか31バイトは超えないよね。
*/
#define MAX_EMAIL_PASSWORD_SIZE(31)
/**
* @brief フレンドの構造\n
* ユーザ情報\n
* writable/path/friends/email_domain/email_address.dat\n
* ユーザサムネール\n
* writable/path/friends/emali_domain/email_address.png\n
* ユーザリスト\n
* writable/path/friends/friend_list.dat\n
* これらのファイルは暗号化する。\n
* 
* フレンド意外のユーザも同じ構造でキャッシュを残す。
* しかし、パスワードは残さないのと、ユーザ情報は対局時のみ更新する。
*/
struct __friend__ {
    user_id_t user_id;
    char email_address[MAX_EMAIL_ADDRESS_SIZE+1];
    char email_passwd[MAX_EMAIL_PASSWORD_SIZE+1];
    bit32_t rate;
    bit32_t total;
    bit32_t win;
    bit32_t lose;
    bit32_t draw;
} PACKED;
typedef struct __friend__ friend_t;
/**
* @brief AcceptFriendイベントボディ
*/
struct __accept_friend__ {
    /**@brief user id */
    user_id_t user_id;
    /**@brief email_address */
    char email_address[MAX_EMAIL_ADDRESS_SIZE+1];
    /**@brief email_address */
    char password[MAX_PASSWORD_SIZE+1];
    /**@brief 0:OK, 1:NG*/
    bit32_t status;
} PACKED;
typedef struct __accept_friend__ accept_friend_t;
struct __request_friend__ {
    /**@brief user id */
    user_id_t user_id;
    /**@brief email_address */
    char email_address[MAX_EMAIL_ADDRESS_SIZE+1];
    /**@brief email_address */
    char password[MAX_PASSWORD_SIZE+1];
    /**@brief 0:OK, 1:NG*/
    bit32_t status;
} PACKED;
typedef struct __request_friend__ request_friend_t;
/**
* @brief ChangeFriendInfoイベントボディ
*/
struct __accept_friend__{
    /**@brief user id */
    user_id_t user_id;
    /**@brief email_address */
    char email_address[MAX_EMAIL_ADDRESS_SIZE+1];
} PACKED;
typedef struct __accept_friend__ accept_friend_t;

/**
* @brief uuidを生成します。
* @param email_address (メールアドレス)
* @return 128bitのuuid
*/
extern uuid_t generate_uuid(const char* email_address);
/**
* @brief uuidを生成します。
* @param user_id \ref user_id_t 
* @return 128bitのuuid
*/
extern uuid_t generate_uuid(user_id_t user_id);
/**
* @brief userIdを取得します。
* @param email_address メールアドレス。
* @return 64bitのハッシュキー
*/
extern user_id_t get_user_id(const char* email_address);

}}
#ifdef WIN32
#   pragma pack(pop)
#endif
#endif
