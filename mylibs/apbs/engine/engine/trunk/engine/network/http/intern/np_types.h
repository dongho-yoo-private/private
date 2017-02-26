/**
 * \author dongho yoo
 * \file nptypes.h
 * \~english
 * \~japanese
 * 	\brief this file must be included by npCmn.c 
*/

#ifndef __NPTYPES_H__
#define __NPTYPES_H__

#ifdef _USE_OPEN_SSL_
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#endif

#define _POSIX
#ifdef _POSIX
#include <arpa/inet.h>
#include <debug_assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

#include <core/debug/logging.h>

typedef S32* SockPtr;

typedef TNPError (*NP_READ_FUNC)(SockPtr sock, char* buffer, int size, int timeout, float* progress);
typedef TNPError (*NP_WRITE_FUNC)(SockPtr sock, const char* buffer, int size, float* progress);
typedef TNPError (*NP_READ_ONCE_FUNC )(SockPtr sock, char* buffer, int size, int* recved, int timeout);
/**
*/
typedef struct {
	int sock;
	int isUseBuffer;
	NP_READ_FUNC pfnRead;
	NP_READ_ONCE_FUNC pfnReadOnce;
	NP_WRITE_FUNC pfnWrite;
//#ifdef _USE_KQUEUE_
	int fd;
//#endif
#ifdef _USE_OPEN_SSL_
	SSL* ssl;
	SSL_CTX* ctx;
#endif
} sockssl_t;

#define xNP_DEBUG DEBUG_LOG

#define xNP_MALLOC 	malloc
#define xNP_FREE	free
#define xNP_ASSERT  assert
#define xNP_ZERO(a) memset(a, 0, sizeof(*(a)))

#define xNP_OPTIMIZE_PACKET_SIZE 4096

#ifdef _DEBUG
#	define xPRINT_SSL_ERROR() ERR_print_errors_fp(stdin)
#	define xSSL_DEBUG printf
#else
#	define xPRINT_SSL_ERROR() 
#	define xSSL_DEBUG DEBUG_LOG
#endif


#define xLOCALHOST (address_t)0x0100007F

// it used by npCmn.c.
#if 0
static int xConnect(address_t address, int port, int timeout, int* fd);
static TNPError xRead(SockPtr sock, char* buffer, int size, int timeout, float* fprogress);
static TNPError xRead4SSL(SockPtr sock, char* buffer, int size, int timeout, float* fprogress);
static TNPError xWrite(SockPtr sock, char* buffer, int size, float* fprogress);
static TNPError xWrite4SSL(SockPtr sock, char* buffer, int size, float* fprogress);
static int xWaitSock(int sock, int timeout/*second*/, int* descriptor);
#endif

#endif
