#ifndef __yisock_h__
#define __yisock_h__

#ifdef __cplusplus
#define EXTERN extern "C" 
#else
#define EXTERN extern 
#endif

#ifdef WIN32
//#include <windows.h>
#define FD_SETSIZE 256
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

typedef int socklen_t;
#else
#include "yi.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#endif
#include <string.h>

#ifndef MSG_NOSIGNAL 
#define MSG_NOSIGNAL 0
#endif


#define MAX_PACKET_SIZE 4096
#define MAX_UDP_PACKET_SIZE 512
#ifndef UDP_WAIT_TIMEOUT
#define UDP_WAIT_TIMEOUT 200 
#endif

#ifndef UDP_RESEND_COUNT
#define UDP_RESEND_COUNT 3
#endif

#ifndef WIN32
#define BOOL int
#define FALSE 0
#define TRUE  1
#endif

typedef  unsigned int address_t;

enum eyiReservedPacketId {
	eRPID_UDP_KEEP_ALIVE = 127
};

enum ePacketFlag16 {
	ePF16_PACKET_NORMAL = 1<<15,
	ePF16_REQ_CON		= 1<<14,
	ePF16_REQ_CON_SUCESS = 1<<13,
	ePF16_REQ_UDP_CONFIRM = 1<<12,
	ePF16_RESEND = 1<<11,
	ePF16_REQ_UDP_FIN = 1<<10,
	ePF16_UDP_REPLY = 1<<9,
	ePF16_UDP_KEEP_ALIVE= 1<<8
};

enum eSocketError {
	eSE_THREAD_CREATE = -1,
	eSE_INVALID_PACKET = -2,
	eSE_SELECT_ERROR = -3,
	eSE_IGNORED_PACKET=-4,
	eSE_TIMEOUT = -5,
	eSE_NO_REPLY=-6,
	eSE_DIFFERENT_SIZE=-7,
	eSE_SEND_ERROR=-8,
	eSE_CLOSED=-8,
};

// ----------------------------
typedef struct xsyipacket {
	unsigned int encode_key;
	unsigned int security;
	unsigned short sizex; 
	unsigned short check_sum; 
	unsigned short id;
	unsigned short flag16;
//} syipacket;
 } __attribute__((aligned(1))) syipacket;
//__attribute__ ((packed)) syipacket ; //16 byte.

// ----------------------------
typedef struct xsyipacketu {
	unsigned int encode_key;
	unsigned int security;
	unsigned int flag32;/*9bit:size, 7bit:ID, 16bit:check_sum*/
	unsigned short seq;
	unsigned short flag16;
//} syipacketu;
} __attribute__ ((packed)) syipacketu; //16 byte.

#define __UF32_SIZE__(f) ((0xFF100000&f)>>23)
#define __UF32_ID__(f) ((0x007F0000&f)>>16)
#define __UF32_CHECKSUM__(f) (0x0000FFFF&f)

#define __UF32_SET_SIZE__(f, v) f|=(v<<23)
#define __UF32_SET_ID__(f, v) f|=v<<16
#define __UF32_SET_CHECKSUM__(f, v) f|=(unsigned int)v

#define NO_TIMEOUT -1

// ----------------------------
typedef struct xsyipacketex {
	unsigned int check_sum;
	char data[1];
}syipacketex;

// ----------------------------
typedef struct xsyipacket32 {
	unsigned int check_sum;
	char buffer[24];
}syipacket32;

// ----------------------------
typedef struct xsyipacket16 {
	unsigned int check_sum;
	char buffer[12];
}syipacket16;

#define WAIT_INFINITE ((int*)-1)
#define NOWAIT	      ((int*)0)
#ifdef WIN32
// --------------------------------
EXTERN int yiInitWinsock(int l, int h);

// --------------------------------
EXTERN void yiCloseWinsock();
#endif


// -------------------------------
// udp.
EXTERN int yiudp(address_t address, int port);

// -------------------------------
typedef unsigned short address6_t[8];

// -------------------------------
EXTERN int yiudpV6(address6_t address, int port);

// -------------------------------
EXTERN int yisenduV6(int sock, const void* buffer, int size, address6_t address, int port);

// -------------------------------
EXTERN int yirecvuV6(int sock, void* buffer, int size, address6_t address, int* port, int timeout);

// -------------------------------
// udp.
//EXTERN int yiudp_unix(const char* name);

// -------------------------------
// tcp
EXTERN int yiconnectudp(int sock, address_t address, int port);

// -------------------------------
// tcp
EXTERN int yiconnect(address_t address, int port, int nRetry);

// -------------------------------
// tcp
EXTERN int yiconnect_unix(const char* pszPort, int nRetry);

// -------------------------------
// tcp
EXTERN int yilisten(address_t address, int port, int nBuffCnt);

// -------------------------------
// tcp
EXTERN int yiaccept(int sock, address_t* address, int* port, int* timeout);

// --------------------------------
EXTERN address_t yihtol(const char* pszHostName, int index);

// --------------------------------
EXTERN address_t yiitol(const char* pszIpAddress);

// -------------------------
EXTERN int yigetport(int sock, address_t* address);

// -------------------------
EXTERN int yigetportV6(int sock, address6_t address);

// -------------------------
EXTERN void yiclose(int sock);

// -------------------------
EXTERN int yisend(int sock, void* buffer, int n, int bUseThread);

// -------------------------
EXTERN int yisendu(int sock, void* buffer, int n, address_t address, int port);

// -------------------------
EXTERN int yisendx(int sock, int id, int security1, int security2, void* data, int n, int bBackground);

// -------------------------
EXTERN int yisendxu(int sock, int id, int security1, int security2, void* data, int n, address_t address, int port, BOOL bConfirm, int seq);

// -------------------------
EXTERN int yirecv(int sock, void* buffer, int n, int* timeout);

// -------------------------
EXTERN int yirecvu(int sock, void* buffer, int n, int* timeout, address_t* address, int* port);

// -------------------------
EXTERN int yirecvx(int sock, syipacket* packet, void* buffer,  unsigned int security, int timeout);

// -------------------------
EXTERN int yirecvxu(int sock, syipacketu* packet, void* data,  unsigned int security, int timeout, address_t* addr, int* port);

// -------------------------
EXTERN int yiwaitsock(int sock, int* timeout);

// -------------------------
EXTERN int yisockclear(int sock);

// -------------------------
EXTERN unsigned int yigetlocaladdress(char* pszAddress, int n);


// -------------------------
EXTERN int yigetlocaladdressV6(address6_t addr, const char* pszServiceName);

// -------------------------
EXTERN const char* yiltostr(unsigned int addr);

// -------------------------
EXTERN void yisockblk(int sock, int bIsBlock);

// -------------------------
EXTERN int yisocklock(int sock);

// -------------------------
EXTERN int yisockunlock(int sock);

// -------------------------
EXTERN Bool yisockIsWritable(int sock);

// -------------------------
//EXTERN address_t yigetlocaladdress(char* szIpAddress);

// for yotsuya server groups.
enum eYotuyaPacketId {
	ePID_RESERVED= 0x8000,
	ePID_TERMINATE = 0x4000,
	ePID_TERMINATE_FORCED=0x01 | ePID_TERMINATE,
	ePID_REQUEST_TERMINATE=0x02 | ePID_TERMINATE,
	ePID_ECHO = 1 | ePID_RESERVED,
	ePID_GET_STATUS,
	ePID_ADD_CONNECTION
};

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

typedef struct {
	int sock;
	SSL* ssl;
	SSL_CTX* ctx;
}_sockssl_t, * sockssl_t;

// -------------------------
EXTERN sockssl_t yiconnect_ssl(address_t address, int port, int nRetry);

// -------------------------
EXTERN int yisend_ssl(sockssl_t sock, const void* p, int size);

// -------------------------
EXTERN int yirecv_ssl(sockssl_t sock, const void* p, int size, int nTimeout);

// -------------------------
EXTERN void yiclose_ssl(sockssl_t sock);

#endif // __cplusplus

