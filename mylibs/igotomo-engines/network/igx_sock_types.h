/**
 * Copyright 2015 (c) igotomo.
 * @author dongho yoo
 * @file igx_sock_types.h
*/

#ifndef _IGX_SOCK_TYPES_H_
#define _IGX_SOCK_TYPES_H_
#include "igx_types.h"

/* define macros for environment. */

/** @brief sslにはopensslを使う */
#define _USE_OPEN_SSL_
/** @brief トリガーはselectを使う */
#define _USE_SELECT_

#ifdef _USE_OPEN_SSL_
#   include <openssl/ssl.h>
#endif

#ifdef __POSIX__
#   include <arpa/inet.h>
#   include <netdb.h>
#   include <netinet/in.h>
#   include <sys/ioctl.h>
#   include <sys/socket.h>
#   include <sys/types.h>
#endif
#ifdef WIN32
#   define _USE_WINSOCK_
#endif

#ifndef STORE
#   define IGX_SOCK_DEBUG
#endif

/**@brief エラータイプ定義 */
typedef enum {
    /**@brief 成功 */
    kSockErrorSuccess=0,
    /**@brief 接続エラー */
    kSockErrorConnect,
    /**@brief タイムアウト */
    kSockErrorTimeout,
    /**@brief writeエラー */
    kSockErrorWrite,
    /**@brief readエラー */
    kSockErrorRead,
    /**@brief SSL関連エラー */
    kSockErrorSSL,
    /**@brief リモートからの接続が切れた*/
    kSockErrorClosed,
    /**@brief 不明なエラー */
    kSockErrorUnknown
} igx_sock_error_t;

/** @brief ソケットハンドル */
typedef struct {
    char buffer[64];
} igx_sock_t;

/** @brief 32bitアドレス */
typedef ubit32_t igx_addrv4_t;
/** @brief 接続先のアドレス */
struct igx_addr_t;

/**@brief 一度に読み・書きするサイズ */
#define kIGX_SOCK_OPTIMIZED_PACKET_SIZE (4096)
/**@brief 32bitのローカルホストの値 */
#define kLOCALHOST (igx_addrv4_t)0x0100007F


#endif
