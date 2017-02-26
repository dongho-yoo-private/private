#ifndef __yiupnp_h__
#define __yiupnp_h__

#include "yistd.h"
#include "yisock.h"

#define MULTICAST_ADDRESS "239.255.255.250"
#define UPNP_PORT	1900
#define SEARCH_TIMEOUT  5000
#define REQUEST_TIMEOUT  1000

#ifdef __cplusplus
extern "C" {
#endif

typedef void* HandleUpnp;

// -------------------------------------
EXTERN Bool yiupnpGetRouterControlUrl(char* _addr, int* _port, char* _ctrlUrl, Bool* _bIsPPPConnection);


// -------------------------------------
EXTERN Bool yiupnpGetExternalAddress(const char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, char* _external_address);

// -------------------------------------
EXTERN Bool yiupnpAddPortMap(char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, 
					char* addr_ip_new, int port_new, Bool bIsTcp, const char* pszDescription);
// -------------------------------------
EXTERN Bool yiupnpDeletePortMap(const char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, int port_new, Bool bIsTcp);

// -------------------------------------
EXTERN HandleUpnp yiupnpCreatex();

// -------------------------------------
EXTERN HandleUpnp yiupnpCreateByAddrx(unsigned int addr, int port);

// -------------------------------------
EXTERN Bool yiupnpGetExternalAddressx(HandleUpnp h, char* _external_address);

// -------------------------------------
EXTERN Bool yiupnpAddPortMapx(HandleUpnp h, char* addr_ip_new, int port_new, Bool bIsTcp, const char* pszDescription);

// -------------------------------------
EXTERN Bool yiupnpDeletePortMapx(HandleUpnp h, int port_new, Bool bIsTcp);

// -------------------------------------
EXTERN void yiupnpSleep(HandleUpnp h);

// -------------------------------------
EXTERN const void* yiupnpExport(HandleUpnp h, int* size);

// -------------------------------------
EXTERN HandleUpnp yiupnpImport(void* p);

// -------------------------------------
EXTERN void yiupnpDestroyx(HandleUpnp h);

#ifdef __cplusplus
};
#endif

#endif
