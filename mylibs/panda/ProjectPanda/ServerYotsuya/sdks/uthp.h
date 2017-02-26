#ifndef __uthp_h__
#define __uthp_h__

#include "yistd.h"
#include "yisock.h"
#include "svry/svrydefs.h"
#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
#	ifdef __cplusplus
#		define _EXTERN extern "C" __declspec(dllexport)
#	else
#		define _EXTERN extern __declspec(dllexport)
#	endif
#else
#	define _EXTERN extern
#endif


typedef void* HandlePrivateServer;


typedef struct xsyiAddress {
	address_t address;
	int port;
} syiAddress;

// ------------------------------------
enum eUthpSocketType {
	eUST_MAIN_SOCK,
	eUST_ACCEPT_SOCK,
	eUST_ACCEPT_UDP_SOCK,
	eUST_NORMAL,
	eUST_WAKEUP_UDP,
	eUST_UDP,
	eUST_TCP,
	eUST_BROADCAST_UDP,
	eUST_TCP_PROXY
};

#ifdef __POSIX__
#define HANDLE Handle
#endif

// ------------------------------------
typedef struct xsyiSocket {
	eUthpSocketType type;
	int		sock;
	int		security;
	Bool		bIsConnected; // added.
	sNetworkInfo 	remote;
	HANDLE		hMutex;
	unsigned int 	event;
	int		w_id;
	void*		wbuffer;
	int		wsize;
	int		seq;
	char		kabuff[4]; // kabuff[0]: recived, kabuff[1]: send, kabuff[2]:recieved other, kabuff[3]:send other
	void*		pExtraData;
} syiSocket;

// ------------------------------------
typedef syiSocket* socket_t;

// ------------------------------------
_EXTERN socket_t uthpSock(int sock, eUthpSocketType type, int security, sNetworkInfo* remote, void* p);

// ------------------------------------
_EXTERN BOOL uthpSend(socket_t sock, int id, void* buffer, int size);

// ------------------------------------
_EXTERN BOOL uthpRecv(socket_t sock, int* id, void* buffer, int* size, int timeout);

// ------------------------------------
_EXTERN socket_t uthpTcpConnect(address_t server, int port, unsigned long security);

// ------------------------------------
_EXTERN void uthpClose(socket_t sock);

// ------------------------------------
_EXTERN int uthpTcpLogin(socket_t sock, unsigned long id, const char* pszName, const char* pszPassword, sNetworkInfo* network, socket_t acceptSock);

// ------------------------------------
_EXTERN BOOL uthpSendRCU(socket_t tcpBloker, int myIndex, int remoteIndex);

// ------------------------------------
_EXTERN BOOL uthpSendRCUOk(socket_t sock, int myIndex, int remoteIndex);

// ------------------------------------
_EXTERN BOOL uthpCheckMyNetwork(socket_t sock, int sockUdp, unsigned int in_address, int in_port, eNatType* type, unsigned int* out_address, int* out_port);

// ------------------------------------
_EXTERN BOOL uthpRegisterMyNetwork(socket_t sock, eNatType type, unsigned int address, int port);

// ------------------------------------
_EXTERN socket_t uthpUdpHolePunching(socket_t serverSock, int server_udpport, int nRandNo, int nRetry);


//// ------------------------------------
//socket_t uthpUdpConnect(socket_t server, int remoteIndex);
//
//// ------------------------------------
//socket_t uthpTcpProxyConnect(int sockServer, int remoteSock, unsigned long security, );
//
//// ------------------------------------
//void uthpClose(socket_t sock);
//
//// ------------------------------------
//int uthpSend(socket_t sock, const void* buffer, int size);
//
//// ------------------------------------
//int uthpReply(socket_t sock, const void* buffer, int size);
//
//// ------------------------------------
//int uthpRecv(socket_t sock, void* buffer, int buffersize, int timeout);



#endif
