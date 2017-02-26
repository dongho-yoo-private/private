/**
 * \author dongho yoo
 * \file npCmn.h
 * \~english
 * \~japanese
 * 	\brief network protocol common socket+ssl library.
*/

#ifndef __NETWORK_COMMON_H__
#define __NETWORK_COMMON_H__

#include <core/types.h>
#include <network/http/adk_np_types.h>

#ifdef __cplusplus
#define NPC_EXTERN extern "C"
#else
#define NPC_EXTERN extern 
#endif

// define macros for environment.
#define _USE_OPEN_SSL_
#if defined(WITH_ANDROID)
# define _USE_SELECT_ 1
#elif defined(WITH_IOS)
# define _USE_KQUEUE_ 1
#endif

#ifndef __POSIX__
#define __POSIX__
#endif

//typedef U32 address_t;
// ----------------------
/**\breif 32bit address.(IPV4)
*/
/*typedef struct {
	union {
		unsigned char char_addr[4];
		unsigned long long_addr;
	};
} address_t;*/

typedef void* HandleSocket;

ADK_BEGIN_NAMESPACE

/** @brief
*/
NPC_EXTERN address_t NPCmnIp2Adr(const Char* ip);

/** @brief
*/
NPC_EXTERN Char* NPCmnAdr2Ip(address_t addr, Char* ip);

/** @brief
* @param index -1 -> random
*/
NPC_EXTERN address_t NPCmnGetHostAdr(const Char* hostname);//, S32 index);

/** @brief
*/
NPC_EXTERN HandleSocket NPCmnConnect(address_t addr, S32 port, Bool useSSL, S32 timeout/*secs*/, TNPError* err);

/** @brief
*/
NPC_EXTERN int NPCmnGetHandleSize();

/** @brief
*/
NPC_EXTERN HandleSocket NPCmnConnectInBuffer(void* buffer, address_t addr, S32 port, Bool useSSL, S32 timeout/*secs*/, TNPError* err);

/** @brief
*/
NPC_EXTERN void NPCmnClose(HandleSocket hSock);

/** @brief
*/
NPC_EXTERN TNPError NPCmnRead(HandleSocket hSock, Char*buffer, S32 size, S32 timeout, float* progress);

/** @brief
*/
NPC_EXTERN TNPError NPCmnReadOnce(HandleSocket hSock, Char*buffer, S32 size, S32 * nRecved, S32 timeout);

/** @brief
*/
NPC_EXTERN TNPError NPCmnWrite(HandleSocket hSock, const Char*buffer, S32 size, float* progress);

/** @brief
*/
NPC_EXTERN Bool NPCmnIsConnected(HandleSocket hSock);

/** @brief
*/
NPC_EXTERN Bool NPCmnWait(HandleSocket hSock, S32 timeout);

/** \brief urlをパースします。
* 
* \param [in] url (ex "http://www.yahoo.co.jp:80/abcde/kkkkk?id=xxxx, pass=????")
* \param [out] protocol プロトコル(ex http, ftp, svn ...)
* \param [out] hostname ホスト名
* \param [out] port	ポーと番号(ない場合はゼロ)
*
* \retval ホスト名の後の文字列のアドレスを返します。
*/
NPC_EXTERN const Char* NPCmnUrlParse(const Char* url, Char* protocol, Char* hostname, S32* port);

ADK_END_NAMESPACE

#endif
