/** !Attention.
*   This header must be included by only ADK_Http.cpp.
*/

#ifdef _HTTP_INTERN_

#include <intern/network/intern/np_cmn.h>

typedef struct {
	THttpError 	error;
	address_t 	addr;
	char		*host;

	int recivedChunkDataSize; // chunk dataのためのもの
	THttpDetail detail;
	THttpResponse response;
	HandleSocket hSock;
} sHttps;

#define xLOCALHOST (address_t)0x0100007F

static int xstrlenx(const char* p, const char* until);
static int xGetStatusFromHeader(const char* p, int* status, int* nContentLen, int headerSize);
static THttpError xRecvHttpHeader4SSL(sHttps*s, THttpResponse* response, int nTimeOut, char** body, int* bodyLen);
static THttpError xHttpRequest(sHttps* s, const char* request, const void* data, int size);
static void xSetProgressStatus(sHttps* sock, const char* msg, float fProgress);
static const char* xAdrrStr(address_t addr);
static address_t xGetAddress(const char* hostname);

static char* xstrcpyx(char* p, const char* pp, const char* until);
static char* xstrncpyx(char* p, const char* pp, const char* until, int len);
static char *strstri( const char *str, const char *key);

// -------------------------------
static THttpError xReadChunkData(sHttps* s);

// -----------------------------------------------------------
static THttpError xHttpResponse(sHttps* s);


#define xGET_PROGRESS(s) (s->detail.progress?&s->detail.progress->fprocess:0)

#define xHEADER_READ_ONEBYTE

#endif /*_HTTP_INTERN_ */
