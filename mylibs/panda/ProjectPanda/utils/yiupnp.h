#ifndef __yiupnp_h__
#define __yiupnp_h__

#include "yistd.h"
#include "yisock.h"

#define MULTICAST_ADDRESS "239.255.255.250"
#define UPNP_PORT	1900
#define SEARCH_TIMEOUT  10000
#define REQUEST_TIMEOUT  1000

#ifdef __cplusplus
extern "C" {
#endif

typedef void* HandleUpnp;

// -------------------------------------
extern Bool yiupnpGetRouterControlUrl(char* _addr, int* _port, char* _ctrlUrl, Bool* _bIsPPPConnection);


// -------------------------------------
extern Bool yiupnpGetExternalAddress(const char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, char* _external_address);

// -------------------------------------
extern Bool yiupnpAddPortMap(char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, 
					char* addr_ip_new, int port_new, Bool bIsTcp, const char* pszDescription);
// -------------------------------------
extern Bool yiupnpDeletePortMap(const char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, int port_new, Bool bIsTcp);

// -------------------------------------
extern HandleUpnp yiupnpCreatex();

// -------------------------------------
extern Bool yiupnpGetExternalAddressx(HandleUpnp h, char* _external_address);

// -------------------------------------
extern Bool yiupnpAddPortMapx(HandleUpnp h, char* addr_ip_new, int port_new, Bool bIsTcp, const char* pszDescription);

// -------------------------------------
extern Bool yiupnpDeletePortMapx(HandleUpnp h, int port_new, Bool bIsTcp);

// -------------------------------------
extern void yiupnpSleep(HandleUpnp h);

// -------------------------------------
extern const void* yiupnpExport(HandleUpnp h, int* size);

// -------------------------------------
extern HandleUpnp yiupnpImport(void* p);

// -------------------------------------
extern void yiupnpDestroyx(HandleUpnp h);

#ifdef __cplusplus
};
#endif

#endif
