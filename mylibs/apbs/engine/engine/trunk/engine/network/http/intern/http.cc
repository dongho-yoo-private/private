#include <network/http/intern/http.h>

#include <core/debug/debug_assert.h>
#include <core/debug/logging.h>
#include <core/memory_allocation/guarded_alloc.h>
#ifdef HTTP_USE_LINKED_LIST
#include <core/library/lib_list.h>
#endif
#include <core/library/lib_string.h>
#include <network/http/intern/np_cmn.h>
#include <network/http/intern/np_types.h>


#ifdef HTTP_USING_C_PLUS_PLUS
#define xRESPONSE_FREE(a) delete a, a=0
#else
#define xRESPONSE_FREE(a) xHTTP_FREE((void*)a), a=0
#endif

#define xHANDLE(sock) ((HandleSocket)&(sock))

#ifdef DEBUG
extern Bool __print_http_request__;
#endif

ADK_BEGIN_NAMESPACE
typedef struct {
	S32 status;
	S32 size;
	Char contentType[HTTP_MAX_CONTENT_TYPE_LEN+1];
	int contentTypeLen;
} THttpResponse__;

typedef struct {
	THttpError 	error;
	address_t 	addr;
	char		host[HTTP_MAX_HOST_NAME_LEN+1];
	Bool		bIsUseBuffer; // ハンドルに指定されたバッファーを使うかどうか
	int 		recivedChunkDataSize; // chunk dataのためのもの
	THttpDetail 	detail;
	sockssl_t 	sock;
	THttpResponse__ __response;

#ifdef HTTP_USING_C_PLUS_PLUS
	HttpResponse*   response;
#else
	THttpResponse*  response;
#endif
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


//#define _HTTP_INTERN_
//#include <http.intern.h>

const char* gpszxHttpContentTypeUrlEncode="application/x-www-form-urlencoded";
const char* gpszxHttpContentTypeTextHtml="text/html";
const char* gpszxHttpContentTypeTextPlane="text/plain";

const char* gpszxHttpGetRequestHeaderOnce="GET %s HTTP/1.1\r\nHost:%s\r\nPort:%d\r\nConnection:close\r\nAccept-Charset:%s\r\n\r\n";
const char* gpszxHttpPostRequestHeaderOnce="POST %s HTTP/1.1\r\nHost:%s\r\nPort:%d\r\nAccept-Charset: %s\r\nContent-Type: %s\r\nContent-Length:%d\r\nConnection:close\r\n\r\n";

const char* gpszxHttpGetRequestHeader="GET %s HTTP/1.1\r\nHost:%s\r\nPort:%d\r\nKeep-Alive:%d\r\nConnection:Keep-Alive\r\nAccept-Charset:%s\r\n\r\n";
const char* gpszxHttpPostRequestHeader="POST %s HTTP/1.1\r\nHost:%s\r\nPort:%d\r\nKeep-Alive:%d\r\nConnection:Keep-Alive\r\nAccept-Charset:%s\r\nContent-Type: %s\r\nContent-Length:%d\r\n\r\n";

#define xHTTP_MALLOC(a) ADK_Malloc(a, "Http")
#define xHTTP_FREE(a) ADK_Free(a)

#define xHEADER_READ_ONEBYTE

#define xHTTP_DEBUG printf //DEBUG_LOG
#define xHTTP_DEBUG_LONG printf



// ++++++++++++++++++++++++++++++++ exported functions. 
// -----------------------------------------------------------
#if 0
const char* HttpGetErrorMsg(THttpError code)
{
	switch(code)
	{
		case kNPE_SUCCESS:
			return "エラー無し";
		case kNPE_CONNECT:
			return "コネクト失敗";
		case kNPE_CONNECT_SSL:
			return "SSL negotiation失敗";
		case kNPE_CONNECT_TIMEOUT:
			return "コネクトタイムアウト";
		case kNPE_UNKNOWN_HOST:
			return "ホストが見つからない";
		case kNPE_READ:
			return "読み込みエラー";
		case kNPE_READ_TIMEOUT:
			return "読み込みタイムアウト";
		case kNPE_WRITE:
			return "書き込みエラー";
		case kNPE_PARAM:
			return "不正なパラメータ";
		case kNPE_PEER:
			return "リモートが原因で接続が切れた";
		case kNPE_NO_MEMORY:
			return "メモリ不足";
		case kNPE_UNKNOWN:
			return "原因不明なエラー";
		default:
			break;
	}

	return 0;
}
#endif

// -----------------------------------------------------------
S32 HttpGetHandleSize()
{
	return sizeof(sHttps);
}

// -----------------------------------------------------------
HandleHttp HttpCreate(address_t addr, const char* hostname, THttpDetail* detail, THttpError* error)
{
	return HttpCreateInBuffer(0, addr, hostname, detail, error);
}

// -----------------------------------------------------------
HandleHttp HttpCreateInBuffer(void* buffer, address_t addr, const char* hostname, THttpDetail* detail, THttpError* error)
{
	sHttps* s = (sHttps*)buffer;
	int len = strlen(hostname);
	TNPError _error;
	Bool bIsUseBuffer=true;
  S32 _port=0;
  Char _protocol[32]={0, };

	if (hostname==0)
	{
		*error=kNPE_PARAM;
		return 0;
	}

	if (buffer==0)
	{
		s = (sHttps*)xHTTP_MALLOC(sizeof(sHttps));
		bIsUseBuffer=false;
	}

	memset(s, 0, sizeof(*s));

	if (addr==0)
	{
    Char _hostname[HTTP_MAX_HOST_NAME_LEN+1];
    NPCmnUrlParse(hostname, _protocol, _hostname, &_port);
		s->addr = NPCmnGetHostAdr(_hostname);
	}
	else
	{
		s->addr=addr;
	}
	//s->detail.isAutoFree=false;

	if (s->addr==0)
	{
		xHTTP_DEBUG("%s was not found\n", hostname);
		*error = kNPE_UNKNOWN_HOST;
		return 0;
	}

#ifdef _DEBUG	
	{
		xHTTP_DEBUG("ip:%s\n", NPCmnAdr2Ip(0, s->addr));
	}
#endif

	ADK_ASSERT(len<HTTP_MAX_HOST_NAME_LEN);
	memcpy(s->host, hostname, len+1);

	if (detail)
		s->detail=*detail;
	else
		HttpSetDefaultDetail(&s->detail);

  if (_port)
  {
    s->detail.port=_port;
  }

  if (_protocol[0]!=0)
  {
    if (memcmp(_protocol, HTTP_STR_PROTOCOL_HTTP, HTTP_STR_PROTOCOL_HTTP_LEN+1)==0)
    {
      s->detail.useSSL=false;
      if (s->detail.port==0)
      {
        s->detail.port=HTTP_DEFAULT_PORT;
      }
    }
    else if (memcmp(_protocol, HTTP_STR_PROTOCOL_HTTPS, HTTP_STR_PROTOCOL_HTTPS_LEN+1)==0)
    {
      s->detail.useSSL=true;
      if (s->detail.port==0)
      {
        s->detail.port=HTTPS_DEFAULT_PORT;
      }
    }
  }
  
	s->bIsUseBuffer = bIsUseBuffer;

	HandleSocket hSock = NPCmnConnectInBuffer(&s->sock, s->addr, s->detail.port, s->detail.useSSL, s->detail.connectTimeout, &_error);

	if (hSock==0)
	{
		*error = _error;
		return 0;
	}

	return (HandleHttp)s;
}

// -----------------------------------------------------------
THttpError HttpMessage(HandleHttp hHttps, const Char* header, const void* data, int size)
{
	sHttps* s = (sHttps*)hHttps;
	ADK_ASSERT(s);

	s->error = xHttpRequest(s, header, data, size);

	if (s->error!=kNPE_SUCCESS)
	{
		return s->error;
	}

	if (s->detail.isReceiveAfter==0)
	{
		s->error = xHttpResponse(s);
	}

#ifdef DEBUG
  if (__print_http_request__)
  {
    xHTTP_DEBUG_LONG("++++++++++++++++++++ response body\n");
    xHTTP_DEBUG_LONG("%s\n", s->response?s->response->GetData():"null");
  }
#endif
	return s->error;
}


// -----------------------------------------------------------
THttpError HttpPost(HandleHttp hHttps, const char* path, const void* data, int dataSize)
{
	sHttps* s = (sHttps*)hHttps;
	char szRequestHeader[1024];

	if (s==0)
	{
		return kNPE_PARAM;
	}

	s->error = kNPE_SUCCESS;

	if (s->detail.keepAliveTimeout!=0)
	{
		sprintf(szRequestHeader, gpszxHttpPostRequestHeader, path, s->host, 
				s->detail.port, s->detail.keepAliveTimeout, s->detail.charset, s->detail.contentType, dataSize);
	}
	else
	{
		sprintf(szRequestHeader, gpszxHttpPostRequestHeaderOnce, path, s->host, 
				s->detail.port, s->detail.charset, s->detail.contentType, dataSize);
	}
	return HttpMessage(hHttps, szRequestHeader, data, dataSize);
}

// -----------------------------------------------------------
THttpError HttpGet(HandleHttp hHttps, const char* path)
{
	sHttps* s = (sHttps*)hHttps;
	//char szRequestHeader[256];
  char* pszRequestHeader;
  int  pathLen = strlen(path);
  char host_name[HTTP_MAX_HOST_NAME_LEN+1];
  char protocol[32];
  S32 port=0;

  pszRequestHeader = (char*)xHTTP_MALLOC(pathLen+1024);
	if (s==0)
	{
		return kNPE_PARAM;
	}
  
  // case) http://xxxx.xxx.xxxxx:nn/xxxxxxx.jpg
  protocol[0]=0;
  host_name[0]=0;
  const Char* _path = NPCmnUrlParse(path, protocol, host_name, &port);
  
  if (_path!=0)
  {
    path=_path;
  }
  
  if (protocol[0])
  {
    if (memcmp(protocol, "http", 5)==0)
    {
      s->detail.useSSL=false;
    }
    else if (memcmp(protocol, "https", 6)==0)
    {
      s->detail.useSSL=true;
    }
  }

	s->error = kNPE_SUCCESS;

	// status sending... 0%
	xSetProgressStatus(s, "sending...", 0.0);
  
  
	if (s->detail.keepAliveTimeout!=0)
	{
		sprintf(pszRequestHeader, gpszxHttpGetRequestHeader, path, protocol[0]?host_name:s->host,
					port?port:s->detail.port, s->detail.keepAliveTimeout, s->detail.charset);
	}
	else
	{
		sprintf(pszRequestHeader, gpszxHttpGetRequestHeaderOnce, path, protocol[0]?host_name:s->host,
													port?port:s->detail.port, s->detail.charset);
	}

	s->error = HttpMessage(hHttps, pszRequestHeader, 0, 0);
    xHTTP_FREE(pszRequestHeader);
	return s->error;
}

#ifdef HTTP_USING_C_PLUS_PLUS
// --------------------------------------------------------
const HttpResponse* HttpGetResponse(HandleHttp hHttps)
{
	sHttps* s = (sHttps*)hHttps;

	if (s==0)
	{
		return 0;
	}	

	if (s->detail.isReceiveAfter!=0)
	{
		s->error = xHttpResponse(s);
		if (s->error!=kNPE_SUCCESS)
		{
			return 0;
		}
	}

	return s->response;
}
#else
// --------------------------------------------------------
const THttpResponse* HttpGetResponse(HandleHttp hHttps)
{
	sHttps* s = (sHttps*)hHttps;

	if (s==0)
	{
		return 0;
	}	

	if (s->detail.isReceiveAfter!=0)
	{
		s->error = xHttpResponse(s);
		if (s->error!=kNPE_SUCCESS)
		{
			return 0;
		}
	}

	return s->response;
}
#endif

// --------------------------------------------------------
void HttpClose(HandleHttp hHttps)
{
	sHttps* s = (sHttps*)hHttps;
	
	if (s==0)
		return ;

	NPCmnClose(xHANDLE(s->sock));

	if (s->detail.isAutoFree==true)
	{
		HttpDeleteResponse(s->response);
	}

	if (s->bIsUseBuffer==false)
	{
		xHTTP_FREE(s);
	}
}

#ifdef HTTP_USING_C_PLUS_PLUS
// -------------------------------------------------
void HttpDeleteResponse(const HttpResponse* response)
{
	if (response==0)
		return ;

	xRESPONSE_FREE(response);
}
#else
// -------------------------------------------------
void HttpDeleteResponse(THttpResponse* response)
{
	if (response==0)
		return ;

	xRESPONSE_FREE(response);
}
#endif


// -------------------------------------------------
THttpError HttpGetLastError(HandleHttp hHttps)
{
	sHttps* s = (sHttps*)hHttps;
	if (s)
		return s->error;
	return kNPE_PARAM;
}


// -------------------------------------------------
THttpDetail* HttpSetDefaultDetail(THttpDetail* detail)
{
	memset(detail, 0, sizeof(*detail));
	detail->port=80;
	detail->connectTimeout=HTTP_DEFAULT_CONNECT_TIMEOUT;
	detail->readTimeout=HTTP_DEFAULT_READ_TIMEOUT;
	detail->charset="utf-8";
	detail->useSSL=0;
	detail->isAutoFree=false;
	detail->contentType=gpszxHttpContentTypeUrlEncode;
  detail->charset = HTTP_DEFAULT_CHAR_SET;
  detail->contentType = HTTP_DEFAULT_CONTENT_TYPE;

	return detail;
}


// -------------------------------------------------
Bool HttpIsConnected(HandleHttp hHttp)
{
	sHttps* s = (sHttps*)hHttp;
	//ADK_ASSERT(s);

	if (s==0)
	{
		return false;
	}

	return NPCmnIsConnected(xHANDLE(s->sock));
}

const char* g_pszGenerlHeaderPost = "POST %s HTTP/1.1\r\nHost:%s\r\nPort:%d\r\nContent-Length:%d\r\n";
const char* g_pszGenerlHeaderGet  = "GET  %s HTTP/1.1\r\nHost:%s\r\nPort:%d\r\n";

Char* HttpMakeGeneralHeader(char* buffer, int bufferLen, const Char* kHostName, S32 dataSize, S32 port, const char* path, const TRequestHttpHeader* headers, int headerCount)
{
	int sum=0;
	int len, i;
	const U16 kLFCR = *((U16*)"\r\n");
	char* p;
	path=path==0?"/":path;

	if (dataSize)
	{
		len = snprintf(buffer, bufferLen, g_pszGenerlHeaderPost, path, kHostName, port, dataSize);
	}
	else
	{
		len = snprintf(buffer, bufferLen, g_pszGenerlHeaderPost, path, kHostName, port, dataSize);
	}

	ADK_ASSERT(bufferLen!=len-1);

	p=buffer+len;

	for (i=0; i<headerCount; i++)
	{
		int keyLen = strlen(headers[i].key);
		int valLen = strlen(headers[i].value);

		ADK_ASSERT(len+sum<bufferLen);				
		memcpy(p+sum, headers[i].key, keyLen); 	// key
		sum+=keyLen;
		
		ADK_ASSERT(len+sum<bufferLen);				
		p[sum++]=':';				// key:

		ADK_ASSERT(len+sum<bufferLen);				
		memcpy(p+sum, headers[i].value, valLen);	// key:value
		sum+=valLen;

		ADK_ASSERT(len+sum<bufferLen);				
		*((U16*)&p[sum])=kLFCR;			// key:value\r\n
		sum+=2;
	}

	
	ADK_ASSERT(len+sum<bufferLen);				
	*((U16*)&p[sum])=kLFCR;
	sum+=2;

	ADK_ASSERT(len+sum<bufferLen);				
	p[sum]=0;

	return buffer;
}

Char* HttpMakeHeader(const Char* kHostName, S32 dataSize, S32 port,
			const Char* path, U32 keepAliveTime, const Char* contentType, const Char* charSet)
{
  return HttpMakeHeaderInBuffer(0, 0, kHostName, dataSize, port, path, keepAliveTime, contentType, charSet);
}

void HttpHeaderDepose(Char* header)
{
	if (header)
		xHTTP_FREE(header);
}

Char* HttpMakeHeaderInBuffer(Char* buffer, int bufferSize, const Char* kHostName,
                             S32 dataSize, S32 port,
                             const Char* path, U32 keepAliveTime,
                             const Char* contentType, const Char* charSet)
{
	int n;
  Char _buffer[HTTP_MAX_HEADER_LEN+1];
  Char _host_name[HTTP_MAX_HOST_NAME_LEN+1];
  Char _protocol[16];
  Char* _path=0;
  
  S32 _port=0;
 
  _buffer[0]=0;
  if (buffer==0)
  {
    buffer=_buffer;
  }
  
	path=path==0?"/":path;
 
  if (kHostName==0)
  {
    return 0;
  }
  
  if (charSet==0)
  {
    charSet= HTTP_DEFAULT_CHAR_SET;
  }
  
  if (contentType==0)
  {
    contentType= HTTP_DEFAULT_CONTENT_TYPE;
  }

  _host_name[0]=0;
  _protocol[0]=0;
  _path = (char*)NPCmnUrlParse(kHostName, _protocol, _host_name, &_port);
  
  if (_path)
  {
    path = path==0?_path:path;
  }

	if (dataSize==0)
	{
		goto ___GET;
	}

	// POST
	{
		if (keepAliveTime!=0)
		{
			n = snprintf(buffer, bufferSize, gpszxHttpPostRequestHeader, path, _protocol[0]?_host_name:kHostName,
					port==0?_port:port, keepAliveTime, charSet, contentType, dataSize);
		}
		else
		{
			n = snprintf(buffer, bufferSize, gpszxHttpPostRequestHeaderOnce, path, _protocol[0]?_host_name:kHostName,
					port==0?_port:port, charSet, contentType, dataSize);
		}
		ADK_ASSERT(n!=bufferSize);
    
    if (_buffer[0])
    {
      return LIB_strdup(buffer);
    }
    else
    {
      return buffer;
    }
	}

___GET:
	{
 	   if (keepAliveTime!=0)
 	   {
 	   		n = snprintf(buffer, bufferSize, gpszxHttpGetRequestHeader, path, _protocol[0]?_host_name:kHostName,
 	   				port==0?_port:port, keepAliveTime, charSet);
 	   }
 	   else
 	   {
 	   		n = snprintf(buffer, bufferSize, gpszxHttpGetRequestHeaderOnce, path, _protocol[0]?_host_name:kHostName, port==0?_port:port, charSet);
 	   }
		ADK_ASSERT(n!=bufferSize);
    
    if (_buffer[0])
    {
      return LIB_strdup(buffer);
    }
    else
    {
      return buffer;
    }
	}
	return 0;
}




// ++++++++++++++++++++++++++++++++ static functions. 


// -----------------------------------------------------------
static int xWaitSock(int sock, int* timeout)
{
	if (timeout==0)
	{
		return 1;
	}
	else
	{
		long sec=0, usec=0;
		int res=0;
		fd_set fds;
		struct timeval tv;

		FD_ZERO(&fds);
		FD_SET(sock, &fds);

		if (*timeout==-1)
		{
			return select(0, &fds, 0, 0, 0);
		}

		// とりあえず秒単位のタイムアウトという事にしよう
		sec = *timeout/1000;
		usec = (*timeout%1000)*1000;
		tv.tv_sec=sec;
		tv.tv_usec=usec;

		res = select(sock+1, &fds, 0, 0, &tv);

		if (FD_ISSET(sock, &fds)!=0)
		{
			return 1;
		}

		*timeout = tv.tv_sec*1000+tv.tv_usec/1000;
		return res;
	}

	return -1;
}

// -----------------------------------------------------------
static char* xstrcpyx(char* p, const char* pp, const char* until)
{
	char* x = (char*)strstr(pp, until);

	if (x==0)
		return 0;

	strncpy(p, pp, (x-pp));
	p[x-pp]=0;

	return p;
}

// -----------------------------------------------------------
static char* xstrncpyx(char* p, const char* pp, const char* until, int len)
{
	char* x = (char*)strstr(pp, until);
	int c;

	if (x==0)
		return 0;

	c = len>(x-pp)?(x-pp):len;
	strncpy(p, pp, c);//len>(x-pp)?(x-pp):len);
	p[c]=0;

	return p;
}

// -----------------------------------------------------------
static int xstrlenx(const char* p, const char* until)
{
	char* x = (char*)strstr(p, until);

	if (x==0)
		return 0;

	return x-p;
}

#define xCHAR_DIFF ('a'-'A')
#define xUPCASE(a) ((a>'Z'&&a<='z') ? a-xCHAR_DIFF:a)
#define xCHAR_CMP(a, b) ((a==b)||(xUPCASE(a)==xUPCASE(b)))
#define xIS_ALPHABET(a) (a>='A'&&a=>'z')

// -----------------------------------------
static char *strstri( const char *str, const char *key)
{
	int keyLength=strlen(key);
	int len = strlen(str);
	int i;
	char* p = (char*)str;

	if (keyLength>len)
		return 0;

	for (i=0; i<=len-keyLength; i++)
	{
		if (xCHAR_CMP(p[i], *key))
		{
			int isSame=1;
			int j;
			for (j=1; j<keyLength; j++)
			{
				if (xCHAR_CMP(p[i+j], key[j])==0)
				{
					isSame=0;
					break;
				}
			}

			if (isSame==1)
			{
				return &p[i];
			}
		}
	}
	return 0;	
}

const char* xSkipSpaceTab(const char* p)
{
	char* str= (char*)p;
	while(*str==' '||*str=='\t') str++;
	return (const char*)str;
}

// -----------------------------------------------------------
static int xGetStatusFromHeader(const char* p, THttpResponse__* response, int headerSize)
{
	int n;
	char temp[64];
	const char* pContentLength 	= strstri(p, "Content-Length");
	const char* pContentType	= strstri(p, "Content-Type");

	if (p==0)
		return 0;

	response->contentType[0]=0;

	if (pContentType>p+headerSize)
	{
		xHTTP_DEBUG("invalid format content-type is too late\n");
		return -1;
	}

	if (pContentType)
	{
		int n, n2;
		char* p = strstr(pContentType, ":");
		if (p==0)
		{
			return 0;
		}

		p = (char*)xSkipSpaceTab(++p);
		//n = xstrlen(p,"\r\n");
		n = LIB_strlenu(p,"\r\n");
		n2 = LIB_strlenu(p,";");

		n = n>n2?n2:n;

		if (n)
		{
			ADK_ASSERT(n<HTTP_MAX_CONTENT_TYPE_LEN);
			memcpy((char*)response->contentType, p, n);
			response->contentType[n]=0;
			response->contentTypeLen=n;
		}
	}

	n = sscanf(p, "%s %d", temp, &response->status);
	//xHTTP_DEBUG("xGetStatusFromHeader scanf ret:%d\n", n);	

	if (n!=2)
		return -1;

	// この場合はエラー
	if (response->status!=200)
	{
		xHTTP_DEBUG("status:%d!=200\n", response->status);
		//return 0;
	}

	response->size=0;
	//*nContentLen = 0;

	if (pContentLength>p+headerSize)
	{
		xHTTP_DEBUG("invalid format content-length is too late\n");
		return -1;
	}
	
	if (pContentLength!=0)
	{
		char buffer[32];
		pContentLength+=strlen("Content-Length");
		pContentLength=strstr(pContentLength, ":");
		
		if (pContentLength==0)
		{
			
			xHTTP_DEBUG("content-length is zero (%s)\n", pContentLength);
			return 0;
		}
		pContentLength++;

		if (xstrlenx(pContentLength, "\r\n")>31)
		{
			return 0;
		}
		xstrcpyx(buffer, pContentLength, "\r\n");
		response->size = atoi(buffer);

	}
	else
	{
		response->size=0;
		// chunkedがあるかどうか
		if (pContentType!=0)
		{
			char* pTransfer = strstri(p, "transfer-encoding"); // まず、ヘッダーを全て小文字か大文字にする処理を入れないとまずい。
			if (pTransfer)
			{
				char temp[32];
				pTransfer+=strlen("Transfer-Encoding");
				xstrncpyx(temp, pTransfer, "\r\n", 32);
				if (strstri(temp, "chunked"))
				{
					//xHTTP_DEBUG("chunked found!\n");
					response->size=-1;
				}
			}
			else
			{
				xHTTP_DEBUG("Transfer-Encoding is not found\n");
			}
		}
		else
		{
			xHTTP_DEBUG("no content-type\n");
		}
	}

	return 0;
}

// -----------------------------------------------------------
// 今のところは
// status、Content-Lengthのみ必要かな。。。
static THttpError xRecvHttpHeader4SSL(sHttps*s, THttpResponse__* response, int nTimeOut, char** body, int* bodyLen)
{
	//sockssl_t* sock = s->sock;
	char  header[HTTP_MAX_HEADER_LEN+1];
	//char* p=header;
	int sum=0;
	int headerSize;
	char* pHeaderEnd=0;
	const unsigned int kLFCRLFCR = *((unsigned int*)"\r\n\r\n");

#ifdef xHEADER_READ_ONEBYTE
	while(sum!=HTTP_MAX_HEADER_LEN)
	{
		int nReadByte;
		TNPError error = NPCmnReadOnce(xHANDLE(s->sock), &header[sum], 1, &nReadByte, nTimeOut);
		if (error!=kNPE_SUCCESS ||
					nReadByte!=1)
		{
      xHTTP_DEBUG_LONG("read error (%d)\n", error);
			return kNPE_READ;
		}
		sum++;
		if (sum>4)
		{
			unsigned int x = *((unsigned int*)&header[sum-4]);
			if (x==kLFCRLFCR)
			{
				// complete.
				header[sum]=0;
				break;
			}
		}
	}

#ifdef DEBUG
  if (__print_http_request__)
  {
    xHTTP_DEBUG_LONG("++++++++++++++++++++ response header\n");
  }
#endif
	if (sum==HTTP_MAX_HEADER_LEN)
	{
		header[sum]=0;
          
#ifdef DEBUG
          
            if (__print_http_request__)
            {
                xHTTP_DEBUG_LONG("%s\n", header);
            }
#endif
		return kNPE_PEER;
	}

#ifdef DEBUG
            if (__print_http_request__)
            {
              xHTTP_DEBUG_LONG("%s\n", header);
            }
#endif
	*bodyLen=0;
	*body=0;
	headerSize=sum;
	pHeaderEnd=&header[sum];
#else
	while(sum!=HTTP_MAX_HEADER_LEN && pHeaderEnd==0)
	{
		int nReadByte = s->pfnReadOnce(s->sock, &header[sum], HTTP_MAX_HEADER_LEN-sum, nTimeOut);

		if (nReadByte==0)
		{
			// timeout.
			return kNPE_PEER;
		}
		if (nReadByte<0)
		{
			return kNPE_READ;
		}

		sum+=nReadByte;
		header[sum]=0;

		pHeaderEnd = strstr(header, "\r\n\r\n");
	} /* while */

	if (pHeaderEnd==0)
	{
		// まず、ないとは思うが、httpのヘッダーが異常に長いかhttpヘッダーじゃないか。。。
		//xHTTP_DEBUG_LONG("%s\n", header);
		return kNPE_PEER;
	}
	//xHTTP_DEBUG_LONG("%s\n", header);

 	pHeaderEnd+=4; // \r\n\r\n
	headerSize = pHeaderEnd-header;
	*bodyLen = sum-headerSize;
#endif

	if (xGetStatusFromHeader(header, response, headerSize)==0)
	{
		/*if (response->size)
		{
			response->data = (char*)xHTTP_MALLOC(response->size+1);
			(*body)[*bodyLen]=0;
		}*/
		return kNPE_SUCCESS;	
	}

	xHTTP_DEBUG("get header error\n");
	return kNPE_PEER;
}


// -----------------------------------------------------------
THttpError xHttpRequest(sHttps* s, const char* request, const void* data, int size)
{
	int nHeaderSize=strlen(request);

	// このタイミングで以前のレスポンスは削除しておく
#ifdef DEBUG
  if (__print_http_request__)
  {
    xHTTP_DEBUG_LONG("++++++++++++++++++++ request header\n");
    xHTTP_DEBUG_LONG("%s", request);
    xHTTP_DEBUG_LONG("++++++++++++++++++++ request body\n");
    xHTTP_DEBUG_LONG("%s\n", data);
  }
#endif
	if (s->detail.isAutoFree==true)
	{
		HttpDeleteResponse(s->response);
	}

	// ヘッダー送信
	if ((s->error=NPCmnWrite(xHANDLE(s->sock), (const Char*)request, nHeaderSize, xGET_PROGRESS(s)))!=kNPE_SUCCESS)
	{
		return s->error;
	}

	if (data!=0 &&
		size!=0)
	{
		// Body送信
		if ((s->error=NPCmnWrite(xHANDLE(s->sock), (const char*)data, size, xGET_PROGRESS(s))) != kNPE_SUCCESS)
		{
			return s->error;
		}
	}
   
	if (s->detail.progress)
	{ 
		// ここからは受け取り
		// status receiving... 0%
		xSetProgressStatus(s, "receiving...", 0.0);
	}

	return s->error;
}

#ifdef HTTP_USING_C_PLUS_PLUS 
static HttpResponse* xCreateResponse(int status, const char* contentType, int contentTypeLen, int size)
#else
static THttpResponse* xCreateResponse(int status, const char* contentType, int contentTypeLen, int size)
#endif
{

#ifdef HTTP_USING_C_PLUS_PLUS
	HttpResponse* ret = (HttpResponse*)xHTTP_MALLOC(sizeof(HttpResponse)+contentTypeLen+size+2);
	THttpResponse* response;

	if (ret==0)
	{
		return 0;
	}

	response = &ret->response;
#else
	THttpResponse* response = (THttpResponse*)xHTTP_MALLOC(sizeof(THttpResponse)+contentTypeLen+size+2);
	if (response==0)
	{
		return 0;
	}
#endif

	response->contentType = (char*)&response[1];
	memcpy(response->contentType, contentType, contentTypeLen+1);
	response->data = (char*)&response->contentType[contentTypeLen+1];
	response->data[size]=0;
	response->status=status;
	response->size=size;


	response->data = response->size?response->data:0;

#ifdef HTTP_USING_C_PLUS_PLUS
	return ret;
#else
	return response;
#endif
}

// -----------------------------------------------------------
static THttpError xHttpResponse(sHttps* s)
{
	int recvedBodySize=0;
	char* recvedBody=0;

	// ヘッダー受信
	if ((s->error=xRecvHttpHeader4SSL(s, &s->__response, s->detail.readTimeout, 
						&recvedBody, &recvedBodySize))!=kNPE_SUCCESS)
	{
		return s->error;
	}

	// ここで整理してみよう。
	// case 1. status=200 + data
	// case 2. status!=200 + data
	// case 3. status=200 + data size -1
	//xHTTP_DEBUG("status:%d, body:%d\n", s->__response.status, s->__response.size);

	if (s->__response.size==0)
	{
		xSetProgressStatus(s, "received.", 100.0);
		xHTTP_DEBUG("content-length is zero\n");
		// この場合も一応、データ作っておかなきゃ
		s->response = xCreateResponse(s->__response.status, s->__response.contentType, 
												s->__response.contentTypeLen, s->__response.size);
		return kNPE_SUCCESS;
	}

	// chunk形式でない場合 case 1,2
    if (s->__response.size!=-1)
    {
		// ヘッダー読み込み時にBodyも読み込まれていない場合
		// 読み込まれている場合は、既にxRecvHttpHeader4SSLの中でメモリを割り当ててコピーしている。
		if (recvedBodySize==0)
		{
			s->response = xCreateResponse(s->__response.status, s->__response.contentType, 
												s->__response.contentTypeLen, s->__response.size);
			if (s->response==0)
			{
			    // これ以上、進めても仕方ない。
                // デバッグ時のみ？
				xHTTP_DEBUG("Out of Memory\n");
                return s->error=kNPE_NO_MEMORY;
			}
		} /*recvedBodySize==0*/
    } 
    else /* s->response.size == -1*/
    {
		// chunk dataの場合
		// この場合は色々とややこしい。。。。
		s->recivedChunkDataSize= recvedBodySize;
		return xReadChunkData(s);
	}
	
	//既にbodyも全て読み込んだ場合
	if (recvedBodySize==s->__response.size)
	{
		if (s->detail.progress)
		{
			xSetProgressStatus(s, "received.", 100.0);
		}
    	//xHTTP_DEBUG_LONG("%s??r??n", s->response.data);
		return kNPE_SUCCESS;
	}
	else // bodyが完全に受信されていなかった場合は、以下のように読み込む。
	{
    //xHTTP_DEBUG_LONG("reading.... %dbytes\n", s->response->response.size);
#ifdef HTTP_USING_C_PLUS_PLUS
		s->error = NPCmnRead(xHANDLE(s->sock), (char*)s->response->response.data, s->response->response.size, 
										s->detail.readTimeout, xGET_PROGRESS(s));
#else
		s->error = NPCmnRead(xHANDLE(s->sock), (char*)s->response->data, s->response->size, 
										s->detail.readTimeout, xGET_PROGRESS(s));
#endif
		if (s->error!=kNPE_SUCCESS)
		{
      xHTTP_DEBUG_LONG("reading.... error!!! %d\n", s->error);
			xHTTP_FREE(s->response);
			s->response=0;
			return s->error;
		}
	}

	if (s->detail.progress)
	{
		xSetProgressStatus(s, "received.", 100.0);
	}
  //xHTTP_DEBUG_LONG("%s\r\n", s->response->response.data?s->response->response.data:"null");
	return s->error;
}


// ------------------------------------------------------------------------------
static void xSetProgressStatus(sHttps* s, const char* msg, float fProgress)
{
	if (s==0)
		return ;

	if (s->detail.progress)
	{
		s->detail.progress->msg = msg;
		s->detail.progress->fprocess=fProgress;
	}
}

// ------------------------------------------------------------------------------
// デバッグ用
static const char* xAdrrStr(address_t addr)
{
	static char buffer[32];
	unsigned char* p = (unsigned char*)&addr;	

	sprintf(buffer, "%d.%d.%d.%d", (int)p[0], (int)p[1], (int)p[2], (int)p[3]);
	return (const char*)buffer;
}


// ------------------------------------------------
static address_t xGetAddress(const char* hostname)
{
	struct hostent* host;
	host = gethostbyname(hostname);
	address_t addr;

	if (host)
	{
		addr = *((long*)host->h_addr_list[0]);
	}
	else
	{
		addr = inet_addr(hostname);
	}
	
	{
		//xHTTP_DEBUG("ipaddress:%s\n", xAdrrStr(addr));
	}

	return addr;
}


// --------------------------------------------
// chunkデータ読み込みようの巨大関数
#define MAX_NUM_LEN 32
static THttpError xReadChunkData(sHttps* s)
{
	char strNo[MAX_NUM_LEN +1] = {0, };
	const char* strLfCr = "\r\n";
	const unsigned short LINE_FEED = *((unsigned short*)(strLfCr));
#if 0
	int chunkSize=0;
#endif
	int chunkSizeSum=0;
	//int len;
#ifdef HTTP_USE_LINKED_LIST
	HandleList hList=LIB_ListCreate();
#else
	void *chunk_data_list[HTTP_MAX_CHUNK_COUNT];
	int  chunk_data_list_count=0;
#endif
#if 0
	char* tempChunkBuffer;
#endif
#if 0
	char* pChunkSizeEnd;
	THttpError error;
#endif
	int isQuit=1;

	s->__response.size=0;	

#ifdef xHEADER_READ_ONEBYTE
	goto __NORMAL_PROCESS;
#endif

// どこかで削除予定
#if 0
__READ_CHUNK_SIZE:
	pChunkSizeEnd = strstr(p, strLfCr);

	// この場合は、chunkデータの途中までしか読み込まれてないケース
	if (pChunkSizeEnd==0)
	{
		int i, n;
		int sum = strlen(s->response.data);
		strcpy(strNo, s->response.data);
		// chunk data のサイズが最後まで読み込まれていない。
		for (i=0; i<MAX_NUM_LEN; i++)
		{
			n = s->pfnReadOnce(s->sock, &strNo[sum++], 1, s->detail.readTimeout);

			if (n!=1)
			{
				s->error = kNPE_READ;//xConvertHttpError(error);
				goto __ERROR_EXIT;
			}

			strNo[sum]=0;
			if (*((unsigned short*)&strNo[sum-2])!=LINE_FEED)
			{
				continue;
			}

		}

		if (i==MAX_NUM_LEN)
		{
			s->error=kNPE_PEER;
			goto __ERROR_EXIT;
		}

		if (sscanf(strNo, "%x", &chunkSize)!=1)
		{
			s->error=kNPE_PEER;
			goto __ERROR_EXIT;
		}

		if (chunkSize==0)
		{
			// 終了
			goto __SUCCESS_EXIT;
		}

		// とりあえず、ここからはchunk dataを普通に読み込めばよい。
		tempChunkBuffer = (char*)xHTTP_MALLOC(chunkSize+sizeof(int)+1);
		chunkSizeSum+=chunkSize;

		if (tempChunkBuffer==0)
		{
			s->error=kNPE_NO_MEMORY;
			goto __ERROR_EXIT;
		}
		error = s->pfnRead(s->sock, &tempChunkBuffer[sizeof(int)], chunkSize, s->detail.readTimeout, xGET_PROGRESS(s));

		if (error!=kNPE_SUCCESS)
		{
			xHTTP_FREE(tempChunkBuffer);
			s->error = error;//xConvertHttpError(error);
			goto __ERROR_EXIT;
		}
		*((int*)tempChunkBuffer)=chunkSize;
#ifdef HTTP_USE_LINKED_LIST
		LIB_ListAdd(hList, &tempChunkBuffer[sizeof(int)]);
#else
		ADK_ASSERT(chunk_data_list_count!=HTTP_MAX_CHUNK_COUNT);
		chunk_data_list[chunk_data_list_count++] = tempChunkBuffer;//&tempChunkBuffer[sizeof(int)];
#endif
		s->response.size+=chunkSize;

		goto __NORMAL_PROCESS;
	}

	memcpy(strNo, p, pChunkSizeEnd-p);
	strNo[pChunkSizeEnd-p]=0;

	if (sscanf(strNo, "%x", &chunkSize)!=1)
	{
		s->error=kNPE_PEER;
		goto __ERROR_EXIT;
	}

	if (chunkSize==0)
	{
		// 終了
		goto __SUCCESS_EXIT;
	}

	p=pChunkSizeEnd;
	p++;
	p++;

	tempChunkBuffer = (char*)xHTTP_MALLOC(chunkSize+sizeof(int)+1);
	if (tempChunkBuffer==0)
	{
		s->error=kNPE_NO_MEMORY;
		goto __ERROR_EXIT;
	}
	tempChunkBuffer[chunkSize+sizeof(int)]=0;

	if ((s->recivedChunkDataSize-(p-s->response.data))>chunkSize)
	{
		// chunk dataがメモリ上に全てのっている。
		memcpy(tempChunkBuffer, p, chunkSize);
		*((int*)tempChunkBuffer)=chunkSize;
#ifdef HTTP_USE_LINKED_LIST
		LIB_ListAdd(hList, &tempChunkBuffer[sizeof(int)]);
#else
		ADK_ASSERT(chunk_data_list_count!=HTTP_MAX_CHUNK_COUNT);
		chunk_data_list[chunk_data_list_count++] = &tempChunkBuffer[sizeof(int)];
#endif
		s->response.size+=chunkSize;
		p+=chunkSize;
		p++; // \r\n

		goto __READ_CHUNK_SIZE;
	}
	else
	{
		int remain = chunkSize-(s->recivedChunkDataSize-(p-s->response.data));
		// メモリにコピーして
		if (remain==0)
		{
			// ぴったんこ
			memcpy(&tempChunkBuffer[sizeof(int)], p, chunkSize);
			*((int*)tempChunkBuffer)=chunkSize;
#ifdef HTTP_USE_LINKED_LIST
			LIB_ListAdd(hList, &tempChunkBuffer[sizeof(int)]);
#else
			ADK_ASSERT(chunk_data_list_count!=HTTP_MAX_CHUNK_COUNT);
			chunk_data_list[chunk_data_list_count++] = &tempChunkBuffer[sizeof(int)];
#endif
			s->response.size+=chunkSize;
		}
		else
		{
			// 残りを読み込む
			memcpy(&tempChunkBuffer[sizeof(int)], p, chunkSize-remain);
			error = s->pfnRead(s->sock, &tempChunkBuffer[sizeof(int)+chunkSize-remain], remain, s->detail.readTimeout, xGET_PROGRESS(s));

			if (error!=kNPE_SUCCESS)
			{
				xHTTP_FREE(tempChunkBuffer);
				s->error = error;//xConvertHttpError(error);
				goto __ERROR_EXIT;
			}
		}
		goto __NORMAL_PROCESS;
	}
#endif // ここまで削除予定　

__NORMAL_PROCESS:
	while(isQuit)
	{
		int i;

		memset(strNo, 0, sizeof(strNo));
		for (i=0; i<MAX_NUM_LEN; i++)
		{
			int nRecved=0;
			TNPError error = NPCmnReadOnce(xHANDLE(s->sock), &strNo[i], 1, &nRecved, s->detail.readTimeout);
			int chunkSize;
			char* data;
			if (error!=kNPE_SUCCESS ||
					nRecved!=1)
			{
				xHTTP_DEBUG("Read Error!(chunk size)\n");
				//return s->error = kNPE_READ;//xConvertHttpError(error);
				goto __ERROR_EXIT;
			}

			if (*((unsigned short*)&strNo[i-1])!=LINE_FEED)
			{
				continue;
			}
			
			strNo[i+1]=0;
			if (sscanf(strNo, "%x", &chunkSize)!=1)
			{
				xHTTP_DEBUG("invalid chunk size %s\n", strNo);
				s->error=kNPE_READ;
				goto __ERROR_EXIT;
			}
			//xHTTP_DEBUG("get chucnk size %d ++++++++++++++++++++++++++++++++++++++++\n", chunkSize);

			if ((isQuit=chunkSize)==0)
			{
				char buffer[4];
				// あと２バイト読み込んで置く必要があるよね？
				if (NPCmnRead(xHANDLE(s->sock), buffer, 2, s->detail.readTimeout, 0)!=kNPE_SUCCESS)
				{
					// TODO. connectionを切る?
					NPCmnClose(xHANDLE(s->sock)), s->sock.sock=0;
				}
				goto __SUCCESS_EXIT;
			}	

			data = (char*)xHTTP_MALLOC(chunkSize+1+sizeof(int)+2); // +2 は\r\n
			if (data==0)
			{
				s->error=kNPE_NO_MEMORY;
				goto __ERROR_EXIT;
			}
#ifdef HTTP_USE_LINKED_LIST
			LIB_ListAdd(hList, &data[sizeof(int)]);
#else
			ADK_ASSERT(chunk_data_list_count!=HTTP_MAX_CHUNK_COUNT);
			chunk_data_list[chunk_data_list_count++] = &data[sizeof(int)];
#endif
			chunkSizeSum+=chunkSize;

			*((int*)data)=chunkSize;
			if ((error = NPCmnRead(xHANDLE(s->sock), &data[sizeof(int)], chunkSize+2, 
								s->detail.readTimeout, xGET_PROGRESS(s)))!=kNPE_SUCCESS)
			{
				xHTTP_DEBUG("read chunk data.... error!!!\n");
				s->error=kNPE_READ;
				goto __ERROR_EXIT;
			}
			data[sizeof(int)+chunkSize]=0;
			xHTTP_DEBUG_LONG("%s\n", &data[sizeof(int)]);
			break;
		} // for

		if (i==MAX_NUM_LEN)
		{
			s->error = kNPE_PEER;
			goto __ERROR_EXIT;
		}
	} // while
__SUCCESS_EXIT:
#ifdef HTTP_USE_LINKED_LIST
	if (hList)
	{
		int sum=0;
		char* x;
		TListNode* node;

		if (s->response.data)
		{
			xRESPONSE_FREE(s->response);
		}

		s->response.data = (char*)xHTTP_MALLOC(s->response.size+1);
		x = (char*)s->response.data;
        x[sum]=0;

		for (node=LIB_ListGetTopFrom(hList, 0); node; node=node->np)
		{
			char* x = (char*)node->ref;
			int size = *((int*)&x[-4]);
			memcpy((char*)&s->response.data[sum], x, size);
			sum+=size;
		}

		while((x=(char*)LIB_ListRemoveEx(hList, 0))!=0)
		{
			xHTTP_FREE(&x[-sizeof(int)]);
		} 
		LIB_ListDestroy(hList);
	}
#else
	{
		int i;
		int offset=0;

		if (s->detail.isAutoFree &&
				s->response)
		{
			xRESPONSE_FREE(s->response);
		}

		s->response = xCreateResponse(s->__response.status, s->__response.contentType, 
												s->__response.contentTypeLen, chunkSizeSum);
		if (s->response==0)
		{
		    // これ以上、進めても仕方ない。
            // デバッグ時のみ？
            return s->error=kNPE_NO_MEMORY;
		}


#ifdef HTTP_USING_C_PLUS_PLUS
		char* p = s->response->response.data;
#else
		char* p = s->response->data;
#endif
		for (i=0; i<chunk_data_list_count; i++)
		{
			char* pChunk = (char*)chunk_data_list[i];
			int   size = *((int*)&pChunk[-sizeof(int)]);

			memcpy(p+offset, pChunk, size);
			offset+=size;
			xHTTP_FREE(((char*)chunk_data_list[i])-sizeof(int));
		}

		ADK_ASSERT(offset==chunkSizeSum);
		p[chunkSizeSum]=0;
	}

#endif
	return s->error=kNPE_SUCCESS;

__ERROR_EXIT:
#ifdef HTTP_USE_LINKED_LIST
	// hListの削除　
	if (hList)
	{
		char* x;
		while((x=(char*)LIB_ListRemoveEx(hList, 0))!=0)
		{
			xHTTP_FREE(&x[-sizeof(int)]);
		} 
		LIB_ListDestroy(hList);
	}
#else
	{
		int i;
		for (i=0; i<chunk_data_list_count; i++)
		{
			xHTTP_FREE(((char*)chunk_data_list[i])-sizeof(int));
		}
	}
#endif
	if (s->response)
	{
		xRESPONSE_FREE(s->response);
	}
	return s->error;
}

ADK_END_NAMESPACE

#if 0
#ifdef __UNIT_TEST__
int main(int argc, char** argv)
{
	adk::THttpResponse response;
	adk::THttpDetail detail;
	adk::THttpError  error;

	HttpSetDefaultDetail(&detail);
	detail.useSSL=0;
	detail.connectTimeout=30;
	error = HttpGetOnce(argv[1], &response, &detail);

	if (error!=kNPE_SUCCESS)
	{
		ADK_DEBUG_LOG("error\n");
		exit(0);
	}
	HttpDeleteResponse(response);
	return 0;
}
#endif
#endif


