#ifndef __http_client_h__
#define __http_client_h__
#include "yistd.h"
#include "yisock.h"

// -------------------------
char* yiHttpRequest(unsigned int server_addr, int port, const char* pszRequest, int* nRecvedSize);

// -------------------------
void yiHttpFree(char* p);



#endif
