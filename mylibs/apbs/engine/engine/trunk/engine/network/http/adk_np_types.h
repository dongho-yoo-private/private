#ifndef __ADK_NPTYPES_H__
#define __ADK_NPTYPES_H__

#include <core/types.h>

typedef enum {
	kNPE_SUCCESS=0,
	kNPE_CONNECT,
	kNPE_CONNECT_TIMEOUT,
	kNPE_CONNECT_SSL,
	kNPE_UNKNOWN_HOST,
	kNPE_WRITE,
	kNPE_READ,
	kNPE_READ_TIMEOUT,
	kNPE_PEER_CONNECTION,
	kNPE_PEER,
	KNPE_INVALID_SSL_VERSION,
	kNPE_SSL,
	kNPE_PARAM,
	kNPE_NO_MEMORY,
	kNPE_UNKNOWN
} TNPError;

typedef U32 address_t;

/* macros *****************************************************/
#define MAX_URL_LEN	 (512)
#define MAX_URL_PATH_LEN (512)

#undef MAX_HOSTNAME_LEN 
#define MAX_HOSTNAME_LEN (512)
#define MAX_PROTOCOL_LEN (16)
#define NO_TIMEOUT (-1)

/* @brief インジゲーターの構造体
 */
typedef struct {
	/**@brief 進行中のメッセージ*/
	const char* msg;
	/**@brief 0.0〜100.0までのプロセス*/
	float fProcess;
} TNpIndigator;

// 廃止
#if 0
/* @brief 細かい設定
 */
typedef struct {
	/**@brief ポーと番号 default:443*/
	S32 port; 			
	/**@brief コネクト時のタイムアウト(秒単位) default:10*/
	S32 connectTimeout;
	/**@brief Read時のタイムアウト(秒単位) default:10*/
	S32 readTimeout;
	/**@brief コネクションを保つ際のタイムアウト値(秒単位) default:0*/
	S32	keepAliveTimeout;
	/**@brief 自動リトライ数*/
	S32 autoRetryCount;
	/**@brief SSL通信をする場合は1層でない場合は0 default:1*/
	Bool useSSL;
	/**@brief 文字コード default:"utf-8"*/
	const char*	charset;
	/**@brief 通人状態のインジゲータ*/
	TNpIndigator* indigator;


	/**@brief ContentType(http option)default:"ContentType"*/
	const char* 	contentType;
} TNpOption;
#endif

#endif
