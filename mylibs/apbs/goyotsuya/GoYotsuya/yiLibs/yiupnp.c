#include "yiupnp.h"
#include <ws2tcpip.h>
#include <stdio.h>
//#include "yitrace.h"

// ----------------------------------
static char* GetLocation(const char* src, char* buffer, int* locationsize);
// ------------------------------
static void lowcase(char* buffer);
// ------------------------------
static void lowcasex(char* _buffer, char* src);
// ----------------------------------
static Bool GetAddressAnd(const char* pszPath, char* address, int* port, char* path);
// ----------------------------------
static char* strcpych(char* dest, const char* src, char ch);
// ----------------------------------
static Bool GetControlBase(char* pszAddress, int port, char* pszXmlPath, Bool bIsPPPConnection, char* _baseAddr, char* _controlUrl, int* _port);
// ------------------------------
static char* FindTag(char* str, char* value, int* _value_size, int n, const char* tag, ...);

// ------------------------------
static char* FineTagIndex(char* str, int n, char* tag, int* s, int*e);
// ------------------------------
static int HttpResult(char* p);

// ----------------------------------
// Requests
const char* g_pszSearchRequest = "M-SEARCH * HTTP/1.1\r\n\
HOST: %s:%u\r\n\
MAN: \"ssdp:discover\"\r\n\
MX: 3\r\n\
ST: urn:schemas-upnp-org:service:%s:1\r\n\r\n";

const char* g_pszSearchRequest2 = "M-SEARCH * HTTP/1.1\r\n\
HOST: 239.255.255.250:1900\r\n\
MAN: \"ssdp:discover\"\r\n\
MX: 3\r\n\
ST: upnp:rootdevice\r\n\r\n";

const char* g_pszHttpRequestHeaderx11="\
%s %s HTTP/1.1\r\n\
HOST: %s:%d\r\n\
CONTENT-LENGTH: %d\r\n\
CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n\
SOAPACTION: \"urn:schemas-upnp-org:service:%s:1#%s\"\r\n\
\r\n%s";

const char* g_pszHttpRequestHeaderx11m="\
M-%s %s HTTP/1.1\r\n\
HOST: %s:%d\r\n\
CONTENT-LENGTH: %d\r\n\
CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n\
MAN: \"http://schemas.xmlsoap.org/soap/envelope/\"; ns=01\r\n\
01-SOAPACTION: \"urn:schemas-upnp-org:service:%s:1\"\r\n\
\r\n%s";

const char* g_pszGlobalAddressRequest="\
<?xml version=\"1.0\"?>\r\n\
<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"\r\n\
  s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n\
  <s:Body>\r\n\
    <u:GetExternalIPAddress xmlns:u=\"urn:schemas-upnp-org:service:WANPPPConnection:1\">\r\n\
    </u:GetExternalIPAddress>\r\n\
  </s:Body>\r\n\
</s:Envelope>\r\n\r\n";

const char* g_pszAddPortMappingRequest ="\
<?xml version=\"1.0\"?>\r\n\
<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"\r\n\
  s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n\
  <s:Body>\r\n\
  <m:AddPortMapping xmlns:m=\"urn:schemas-upnp-org:service:%s:1\">\r\n\
<NewRemoteHost></NewRemoteHost>\r\n\
<NewExternalPort>%d</NewExternalPort>\r\n\
<NewProtocol>%s</NewProtocol>\r\n\
<NewInternalPort>%d</NewInternalPort>\r\n\
<NewInternalClient>%s</NewInternalClient>\r\n\
<NewEnabled>1</NewEnabled>\r\n\
<NewPortMappingDescription>%s</NewPortMappingDescription>\r\n\
<NewLeaseDuration>0</NewLeaseDuration>\r\n\
</m:AddPortMapping>\r\n\
  </s:Body>\r\n\
</s:Envelope>\r\n\r\n";

const char* g_pszDeletePortMappingRequest ="\
<?xml version=\"1.0\"?>\r\n\
<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"\r\n\
  s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n\
  <s:Body>\r\n\
  <m:DeletePortMapping xmlns:m=\"urn:schemas-upnp-org:service:%s:1\">\r\n\
<NewRemoteHost></NewRemoteHost>\r\n\
<NewExternalPort>%d</NewExternalPort>\r\n\
<NewProtocol>%s</NewProtocol>\r\n\
</m:DeletePortMapping>\r\n\
  </s:Body>\r\n\
</s:Envelope>\r\n\r\n";

// -------------------------------------
Bool yiupnpGetRouterControlUrl(char* _addr, int* _port, char* _ctrlUrl, Bool* _bIsPPPConnection)
{
	unsigned int tempaddr=0, templen=0;
	int sock = yiudp(0, -1);	
	int multicast_addr = yiitol(MULTICAST_ADDRESS);
	char send_buffer[1024];
	char url[128];
	int len=0;
	int  timeout=SEARCH_TIMEOUT;
	char szObjectName[64];
	unsigned char lTTL = 1;

	*_bIsPPPConnection=True;

	if (sock<0)
	{
		//yiTraceM2("c:\\yotsuya\\upnp.log", "yiupnpGetRouterInfo() sock failure\n");
		return False;
	}
	strcpy(szObjectName, "WANPPPConnection");

SEARCH:
	memset(send_buffer, 0, sizeof(send_buffer));
	sprintf(send_buffer, g_pszSearchRequest, MULTICAST_ADDRESS, UPNP_PORT, szObjectName);
	len = strlen(send_buffer)+1;	

	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (char*) &lTTL, sizeof(lTTL))<0)
	{
		yiclose(sock);
		return False;
	}

	if (yisendu(sock, (void*)send_buffer, len, multicast_addr, UPNP_PORT)!=len)
	{
		yiclose(sock);
		//yiTraceM2("c:\\yotsuya\\upnp.log", "yiupnpGetRouterInfo() send error\n");
		return 0;
	}

	while(1)
	{
		int res;
		int size;
		char* pLocationTop;
		char buffer2[4096];
		char buffer[4096];

		if ((res=yirecvu(sock, buffer, 4096, &timeout, 0, 0))<0)
		{
			if (timeout==0)
			{
				//yiTraceM2("c:\\yotsuya\\upnp.log", "yiupnpGetRouterInfo() time out\n");

				break;
			}
			yiclose(sock);
			//yiTraceM2("c:\\yotsuya\\upnp.log", "yiupnpGetRouterInfo() recv error\n");
			//yiTrace("yiupnpGetRouterInfo() recv error\n");
			return False;
		}
		buffer[res]=0;
		lowcasex(buffer2, buffer);

		yiTraceM2("upnp.log", "before location\n%s\n", buffer);
		if ((pLocationTop=GetLocation(buffer2, 0, &size))!=0)
		{
			char szPath[256];
			memcpy(url, &buffer[pLocationTop-buffer2], size);
			url[size]=0;
			if (GetAddressAnd(url, _addr, _port, szPath)==False)
			{
				continue;
			}
			//yiTraceM2("c:\\yotsuya\\upnp.log", "url:%s, _addr:%x, _port:%d, szPath:%s\n", url, _addr, *_port, szPath);
			if (GetControlBase(_addr, *_port, szPath, *_bIsPPPConnection, _addr, _ctrlUrl, _port)==True)
			{
				yiclose(sock);
				return True;
			}
			break;
			//return GetControlBase(_addr, *_port, szPath, *_bIsPPPConnection, _addr, _ctrlUrl, _port);
		}
	}

	if (*_bIsPPPConnection==True &&
			timeout==0)
	{
		*_bIsPPPConnection=False;
		strcpy(szObjectName, "WANIPConnection");
		timeout=SEARCH_TIMEOUT;
		goto SEARCH;
	}
	yiclose(sock);
	//yiTraceM2("c:\\yotsuya\\upnp.log", "yiupnpGetRouterInfo() not found!");
	//yiTrace("yiupnpGetRouterInfo() not found\n");

	return False;
}

typedef struct xsUpnpStruct {
	char szAddress[128];	
	int  access_port;
	char szControlUrl[128];
	Bool bIsPPPConnection;
	int  httpSock;
} sUpnpStruct;

// -------------------------------------
HandleUpnp yiupnpCreatex()
{
	sUpnpStruct s;

	if (yiupnpGetRouterControlUrl(s.szAddress, &s.access_port, s.szControlUrl, &s.bIsPPPConnection)==True)
	{
		s.httpSock=0;
		return (HandleUpnp)yitoheap(&s, sizeof(s));
	}
	return 0;
}

// -------------------------------------
HandleUpnp yiupnpCreateByAddrx(unsigned int addr, int port)
{
}

// -------------------------------------
static Bool _yiupnpGetExternalAddress(const char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, char* _external_address, int* sock)
{
	int sockHttp=0;
	char buffer[4096];
	int len = strlen(g_pszGlobalAddressRequest);
	int res;
	int cnt=0;
	int timeout=REQUEST_TIMEOUT;
	int nHttpResult;
	Bool bIsMpostSend=False;

	if (sock && *sock)
	{
		yiclose(*sock);
		*sock=0;
	}

	if (sock==0 ||
		*sock==0)
	{
		sockHttp = yiconnect(yiitol(addr_ip), port, 3);
		if (sockHttp<0)
		{
			return False;
		}

		if (sock)
			*sock=sockHttp;
	}
	else 
	{
		sockHttp = *sock;
	}

	sprintf(buffer, g_pszHttpRequestHeaderx11, "POST", controlUrl, addr_ip, port, len, 
					bIsPPPConnection? "WANPPPConnection":"WANIPConnection", 
						"GetExternalIPAddress",g_pszGlobalAddressRequest);
M_POST_SEND:

	len=strlen(buffer);

	if ((res=yisend(sockHttp, buffer, len, False))!=len)
	{
		yiTrace("send error....%d\n", res);
		return False;
	}

	buffer[0]=0;
	while(1)
	{
		int n=0;
		if ((n=yirecv(sockHttp, &buffer[cnt], 4096, &timeout))<1)
		{
			if (sock==0)
			{
				yiclose(sockHttp);
			}
			break;
		}
		timeout=30;	
		cnt+=n;
		//yiTrace("timeout:%d\n", timeout);
	}

	if (cnt==0)
	{
		return False;
	}

	buffer[cnt]=0;

	yiTraceM2("upnp.log", buffer);

	if ((nHttpResult=HttpResult(buffer))!=200)
	{
		if (nHttpResult==405)
		{
			if (bIsMpostSend==True)
			{
				return False;
			}

			len = strlen(g_pszGlobalAddressRequest);
			sprintf(buffer, g_pszHttpRequestHeaderx11m, "POST", controlUrl, addr_ip, port, len, 
						bIsPPPConnection? "WANPPPConnection":"WANIPConnection" ,g_pszGlobalAddressRequest);
			bIsMpostSend=True;
			if (sock==0)
			{
				sockHttp = yiconnect(yiitol(addr_ip), port, 3);
			}
			timeout=REQUEST_TIMEOUT;
			cnt=0;
			goto M_POST_SEND;
		}

		yiTrace("http result: %d\n", nHttpResult);

		return False;
	}

	//yiTrace(buffer);
	{
		char buffer_lowcase[4096];
		lowcasex(buffer_lowcase, buffer);

		if (FindTag(buffer_lowcase, _external_address, 0, 0, "newexternalipaddress", 0, 0)==0)
		{
			return False;
		}
	}

	return True;
}

// -------------------------------------
Bool yiupnpGetExternalAddress(const char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, char* _external_address)
{
	return _yiupnpGetExternalAddress(addr_ip, port, controlUrl, bIsPPPConnection, _external_address, 0);
}

// -------------------------------------
Bool yiupnpGetExternalAddressx(HandleUpnp h, char* _external_address)
{
	sUpnpStruct* s = (sUpnpStruct* )h;
	return _yiupnpGetExternalAddress(s->szAddress, s->access_port, s->szControlUrl, s->bIsPPPConnection, _external_address, &s->httpSock);
}

// -------------------------------------
Bool _yiupnpAddPortMap(char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, 
								const char* addr_ip_new, int port_new, 
										Bool bIsTcp, const char* pszDescription, int*sock)
{
	int sockHttp=0;
	char buffer[4096];
	int len = 0;
	int res;
	int cnt=0;
	int timeout=REQUEST_TIMEOUT;
	int nHttpResult;
	Bool bIsMpostSend=False;

	if (sock && *sock)
	{
		yiclose(*sock);
		*sock=0;
	}

	if (sock==0 ||
		*sock==0)
	{
		sockHttp = yiconnect(yiitol(addr_ip), port, 3);
		if (sockHttp<0)
		{
			return False;
		}

		if (sock)
			*sock=sockHttp;
	}
	else 
	{
		sockHttp = *sock;
	}

	{
		char buffer_portmap[1024];
		sprintf(buffer_portmap, g_pszAddPortMappingRequest, bIsPPPConnection? "WANPPPConnection":"WANIPConnection", 
								port_new, bIsTcp?"TCP":"UDP", port_new, addr_ip_new, pszDescription);

		sprintf(buffer, g_pszHttpRequestHeaderx11, "POST", controlUrl, addr_ip, port, strlen(buffer_portmap), 
						bIsPPPConnection? "WANPPPConnection":"WANIPConnection", 
							"AddPortMapping", buffer_portmap);
	}
M_POST_SEND:

	len=strlen(buffer);

	if ((res=yisend(sockHttp, buffer, len, False))!=len)
	{
		yiTrace("send error....%d\n", res);
		return False;
	}

	buffer[0]=0;
	while(1)
	{
		int n=0;
		if ((n=yirecv(sockHttp, &buffer[cnt], 4096, &timeout))<1)
		{
			if (sock==0)
			{
				yiclose(sockHttp);
			}
			break;
		}
		timeout=30;	
		cnt+=n;
		//yiTrace("timeout:%d\n", timeout);
	}

	if (cnt==0)
	{
		return False;
	}

	buffer[cnt]=0;
	yiTraceM2("upnp.log", buffer);


	if ((nHttpResult=HttpResult(buffer))!=200)
	{
		if (nHttpResult==405)
		{
			if (bIsMpostSend==True)
			{
				return False;
			}
			{
				char buffer_portmap[1024];
				sprintf(buffer_portmap, g_pszAddPortMappingRequest, bIsPPPConnection? "WANPPPConnection":"WANIPConnection", 
								port_new, bIsTcp?"TCP":"UDP", port_new, addr_ip_new, pszDescription);

				sprintf(buffer, g_pszHttpRequestHeaderx11, "POST", controlUrl, addr_ip, port, strlen(buffer_portmap), 
							bIsPPPConnection? "WANPPPConnection":"WANIPConnection", 
								"AddPortMapping", buffer_portmap);
			}

			bIsMpostSend=True;
			if (sock==0)
			{
				sockHttp = yiconnect(yiitol(addr_ip), port, 3);
			}
			timeout=REQUEST_TIMEOUT;
			cnt=0;
			goto M_POST_SEND;
		}

		yiTrace("HttpResult:%d\n", nHttpResult);
		return False;
	}

	return True;
}

// -------------------------------------
Bool yiupnpAddPortMap(char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, char* addr_ip_new, int port_new, Bool bIsTcp, const char* pszDescription)
{
	return _yiupnpAddPortMap(addr_ip, port, controlUrl, bIsPPPConnection, addr_ip_new, port_new, bIsTcp, pszDescription, 0);
}

// -------------------------------------
Bool yiupnpAddPortMapx(HandleUpnp h, char* addr_ip_new, int port_new, Bool bIsTcp, const char* pszDescription)
{
	sUpnpStruct* s = (sUpnpStruct* )h;
	return _yiupnpAddPortMap(s->szAddress, s->access_port, s->szControlUrl, s->bIsPPPConnection, addr_ip_new, port_new, bIsTcp, pszDescription, &s->httpSock);
}

// -------------------------------------
Bool _yiupnpDeletePortMap(const char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, int port_new, Bool bIsTcp, int*sock)
{
	int sockHttp=0;
	char buffer[4096];
	int len = 0;
	int res;
	int cnt=0;
	int timeout=REQUEST_TIMEOUT;
	int nHttpResult;
	Bool bIsMpostSend=False;

	if (sock && *sock)
	{
		yiclose(*sock);
		*sock=0;
	}

	if (sock==0 ||
		*sock==0)
	{
		sockHttp = yiconnect(yiitol(addr_ip), port, 3);
		if (sockHttp<0)
		{
			return False;
		}

		if (sock)
			*sock=sockHttp;
	}
	else 
	{
		sockHttp = *sock;
	}

	{
		char buffer_portmap[1024];
		sprintf(buffer_portmap, g_pszDeletePortMappingRequest, bIsPPPConnection? "WANPPPConnection":"WANIPConnection", 
								port_new, bIsTcp?"TCP":"UDP");

		sprintf(buffer, g_pszHttpRequestHeaderx11, "POST", controlUrl, addr_ip, port, strlen(buffer_portmap), 
						bIsPPPConnection? "WANPPPConnection":"WANIPConnection", 
							"DeletePortMapping", buffer_portmap);
	}
M_POST_SEND:

	len=strlen(buffer);

	if ((res=yisend(sockHttp, buffer, len, False))!=len)
	{
		yiTrace("send error....%d\n", res);
		return False;
	}

	buffer[0]=0;
	while(1)
	{
		int n=0;
		if ((n=yirecv(sockHttp, &buffer[cnt], 4096, &timeout))<1)
		{
			if (sock==0)
			{
				yiclose(sockHttp);
			}
			break;
		}
		timeout=30;	
		cnt+=n;
		//yiTrace("timeout:%d\n", timeout);
	}

	if (cnt==0)
	{
		return False;
	}

	buffer[cnt]=0;

	if ((nHttpResult=HttpResult(buffer))!=200)
	{
		if (nHttpResult==405)
		{
			if (bIsMpostSend==True)
			{
				return False;
			}
			{
				char buffer_portmap[1024];
				sprintf(buffer_portmap, g_pszDeletePortMappingRequest, bIsPPPConnection? "WANPPPConnection":"WANIPConnection", 
									port_new, bIsTcp?"TCP":"UDP");

				sprintf(buffer, g_pszHttpRequestHeaderx11, "POST", controlUrl, addr_ip, port, strlen(buffer_portmap), 
								bIsPPPConnection? "WANPPPConnection":"WANIPConnection", 
									"DeletePortMapping", buffer_portmap);
			}

			bIsMpostSend=True;
			if (sock==0)
			{
				sockHttp = yiconnect(yiitol(addr_ip), port, 3);
			}
			timeout=REQUEST_TIMEOUT;
			cnt=0;
			goto M_POST_SEND;
		}

		yiTrace("HttpResult:%d\n", nHttpResult);
		return False;
	}

	return True;
}

// -------------------------------------
Bool yiupnpDeletePortMap(const char* addr_ip, int port, const char* controlUrl, Bool bIsPPPConnection, int port_new, Bool bIsTcp)
{
	return _yiupnpDeletePortMap(addr_ip, port, controlUrl, bIsPPPConnection, port_new, bIsTcp, 0);//&s->httpSock);
}

// -------------------------------------
Bool yiupnpDeletePortMapx(HandleUpnp h, int port_new, Bool bIsTcp)
{
	sUpnpStruct* s = (sUpnpStruct* )h;
	return _yiupnpDeletePortMap(s->szAddress, s->access_port, s->szControlUrl, s->bIsPPPConnection, port_new, bIsTcp, &s->httpSock);
}

// -------------------------------------
void yiupnpSleep(HandleUpnp h)
{
	sUpnpStruct* s = (sUpnpStruct* )h;
	yiclose(s->httpSock);
	s->httpSock=0;
}

// -------------------------------------
void yiupnpDestroyx(HandleUpnp h)
{
	sUpnpStruct* s = (sUpnpStruct* )h;

	if (h==0)
	{
		return ;
	}

	if (s->httpSock!=0)
	{
		yiclose(s->httpSock);
	}

	yifree(h);
}




// ------------------------------
static void lowcase(char* buffer)
{
	char diff = 'a'-'A';

	while(*buffer)
	{
		if (*buffer>='A' && *buffer<='Z')
		{
			*buffer+=diff;
		}
		buffer++;
	}	
}

// ------------------------------
static void lowcasex(char* _buffer, char* src)
{
	char diff = 'a'-'A';

	while(*src)
	{
		if (*src>='A' && *src<='Z')
		{
			*_buffer=*src+diff;
		}
		else
		{
			*_buffer=*src;
		}
		_buffer++;
		src++;
	}

	*_buffer=0;
}


// ----------------------------------
static char* GetLocation(const char* src, char* buffer, int* locationsize)
{
	char* pIndex=(char*)src;
	int index=0;
	char* pValTop;
	int n=0;

	pIndex = strstr(pIndex, "location:");

	if (pIndex==0)
	{
		return FALSE;
	}

	pIndex = &pIndex[strlen("location:")];
	pValTop=pIndex;

	while(*pIndex)
	{
		if (*pIndex==' ')
		{
			pValTop++;
			pIndex++;
			continue;
		}
		if (*pIndex=='\r')
			break;
		if (*pIndex=='\n')
			break;
		if (buffer)
			buffer[index++]=*pIndex;
		pIndex++;
		n++;
	}

	if (buffer)
		buffer[index]=0;

	if (locationsize)
		*locationsize=n;

	return pValTop;
}

// ----------------------------------
static Bool GetAddressAnd(const char* pszPath, char* address, int* port, char* path)
{
	Bool bModePort=False;
	char* pIndex = strstr(pszPath, "http://");
	char port_buffer[6];

	if (pIndex==0)
	{
		pIndex=(char*)pszPath;
	}
	else
	{
		pIndex=&pIndex[strlen("http://")];	
	}

	
	while(*pIndex)
	{
		if (*pIndex==':')
		{
			pIndex++;
			bModePort=True;
			break;
		}

		if (*pIndex=='/')
		{
			pIndex++;
			break;
		}

		*address=*pIndex;
		address++;
		pIndex++;
	}

	*address=0;

	if (bModePort==True)
	{
		strcpych(port_buffer, pIndex, '/');
		*port = atoi(port_buffer);

		if (*port==0)
		{
			return False;
		}
	}

	pIndex = strstr(pIndex, "/");	

	if (pIndex==0)
	{
		return True;
	}
	pIndex++;

	if (path!=0)
	{
		strcpy(path, pIndex);
	}

	return True;
}

// ----------------------------------
static char* strcpych(char* dest, const char* src, char ch)
{
	char* org=dest;
	while(*src!=ch)
	{
		*dest++=*src++;
	}
	*dest=0;
	return org;
}

// ----------------------------------
static Bool GetControlBase(char* pszAddress, int port, char* pszXmlPath, Bool bIsPPPConnection, char* _baseAddr, char* _controlUrl, int* _port)
{
	int timeout = SEARCH_TIMEOUT;
	char buffer[4096];
	char buffer_lowcase[4096];
	char szMethodName[64];
	char urlBase[128];
	char command[256];
	int command_len;
	char* pServiceTagStart;
	int i=0, cnt=0;
	int sockHttp; 

	if (strlen(pszXmlPath)>200)
	{
		//yiTraceM2("c:\\yotsuya\\upnp.log", "too long path %s:%d\n", pszXmlPath);
		return False;
	}
	sockHttp = yiconnect(yiitol(pszAddress), port, 3);

	if (sockHttp<0)
	{
		//yiTraceM2("c:\\yotsuya\\upnp.log", "connection error %s:%d", pszAddress, port);
		//yiTrace("connection error %s:%d\n", pszAddress, port);
		return False;
	}


	sprintf(command, "GET /%s HTTP/1.0\r\n\r\n", pszXmlPath);
	command_len = strlen(command);
	if (yisend(sockHttp, command, command_len, False)!=command_len)
	{
		yiTrace("send error %s\n", command);
		return False;
	}

	if (bIsPPPConnection==TRUE)
	{
		strcpy(szMethodName, "wanpppconnection");
	}
	else
	{
		strcpy(szMethodName, "wanipconnection");
	}

	while(1)
	{
		int n=0;
		if ((n=yirecv(sockHttp, &buffer[cnt], 4096, &timeout))<1)
		{
			yiclose(sockHttp);
			break;
		}
		cnt+=n;
	}

	memcpy(buffer_lowcase, buffer, 4096);
	lowcase(buffer_lowcase);

	if (FindTag(buffer_lowcase, urlBase, 0, 0, "urlbase", 0, 0)==0)
	{
		if (FindTag(buffer_lowcase, urlBase, 0, 0, "device description", 0, 0)==0)
		{
			//yiTrace("urlbase or Device description tag not found\n");
			//bHasUrlBase=False;
			sprintf(urlBase, "http://%s/", pszAddress);//%d.%d.%d.%d", 
			if (_port)
				*_port=port;
			//return False;
		}
	}

	pServiceTagStart = FindTag(buffer_lowcase, 0, 0, 0, "root", 0, "servicelist",  0, 0);

	if (pServiceTagStart == 0)
	{
		//yiTraceM2("c:\\yotsuya\\upnp.log", "service start not found (%s)", buffer_lowcase);
		return False;
	}

	while(1)
	{
		char value[128];
		int  value_len;
		char* p = FindTag(pServiceTagStart, value, &value_len, i, "service", 0, "servicetype", 0, 0);

		if (p==0)
		{
			//yiTraceM2("c:\\yotsuya\\upnp.log", "tag not found\n");//, pszXmlPath);
			//yiTrace("tag not found\n");
			return False;
		}

		if (strstr(value, szMethodName)!=0)
		{
			int len;
			int offset;
			char* pControlUrl=FindTag(p, 0, &len, 0, "controlurl", 0, 0);
			if (pControlUrl==0)
			{
				//yiTraceM2("c:\\yotsuya\\upnp.log", "tag not found, contorlURL\n");
				return False;
			}
			offset = pControlUrl-buffer_lowcase;
			memcpy(_controlUrl, &buffer[offset], len);
			_controlUrl[len]=0;
			break;
		}
		i++;
	
	}

	if (GetAddressAnd(urlBase, _baseAddr, _port, 0)==False)
	{
		//yiTraceM2("c:\\yotsuya\\upnp.log", "invalid location\n");
		return False;
	}

	return True;
	//printf("controlUrl:%s\n", controlUrl);

}

// ------------------------------
static char* FindTag(char* str, char* value, int* _value_size, int n, const char* tag, ...)
{
	va_list ap;
	int i=0;
	int cnt;
	char* pIndex=str;
	void** pArgv=(void**)&n;
	int s, e;

	va_start(ap, tag);
	while(pArgv[i+1]!=0)
	{
		//yiTraceM2("c:\\yotsuya\\upnp.log", "tag:%s", tag);
		i++;
		i++;
	}

	cnt=i;
	//yiTraceM2("c:\\yotsuya\\upnp.log", "cnt:%d", cnt);

	if (cnt==0)
	{
		return 0;
	}

	for (i=0; i<cnt; i++)
	{
		int _n = (int)pArgv[i];

		//yiTraceM2("c:\\yotsuya\\upnp.log", "tag:%s", pArgv[i+1]);
		pIndex = FineTagIndex(pIndex, _n, (char*)pArgv[i+1], &s, &e);

		if (pIndex==0)
		{
			return 0;
		}
		i++;
	}

	if (value!=0)
	{ 
		strncpy(value, &pIndex[1], e-1);
		value[e-1]=0;
	}
	if (_value_size!=0)
	{
		*_value_size=e-1;
	}

	return &pIndex[1];
}

// ------------------------------
static char* FineTagIndex(char* str, int n, char* tag, int* s, int*e)
{
	char stag[128];
	char etag[128];
	int  taglen=strlen(tag);
	char* pIndex=str;
	char* pEnd=0;
	char* pStart=0;
	char* pStartEnd=0;
	int i=0;

	if (taglen>125)
		return 0;

	sprintf(stag, "<%s", tag);	
	sprintf(etag, "</%s", tag);	


	for (i=0; i<n+1; i++)
	{
		pIndex = strstr(pIndex, stag);
		if (pIndex==0)
			return 0;
		pIndex+=taglen;
	}
	pStart=pIndex;


	while(*pIndex)
	{
		if (strncmp(pIndex, "/>", 2)==0)
		{
			pStartEnd=pIndex+1;
			pEnd=pStartEnd;
			break;
		}
		else if (*pIndex=='>')
		{
			pStartEnd=pIndex;
			break;
		}

		pIndex++;
	}

	if (pStartEnd==0)
	{
		return 0;
	}

	pIndex = strstr(pIndex, etag);

	if (pIndex==0)
	{
		return 0;
	}

	pEnd=pIndex;

	*s = pStart-str;	
	*e = pEnd-pStartEnd;	

	return pStartEnd;
	
}

// ------------------------------
static int HttpResult(char* p)
{
	char  number[4];
	char* pIndex=strstr(p, "HTTP/1.1");
	int   cnt=0; 

	if (pIndex==0)
		return -1;

	pIndex=&pIndex[8];
	number[3]=0;

	while(*pIndex&&cnt!=3)
	{
		if (*pIndex<'0'||*pIndex>'9')
		{
			if (cnt!=0)
			{
				return -1;
			}
			pIndex++;
			continue;
		}
		number[cnt++]=*pIndex;
		pIndex++;
	}

	return atoi(number);
}

// -------------------------------------
const void* yiupnpExport(HandleUpnp h, int* size)
{
	if (size)
		*size = sizeof(sUpnpStruct);
	return (const void*)h;
}

// -------------------------------------
HandleUpnp yiupnpImport(void* p)
{
	char szExternalAddr[32];
	HandleUpnp h = (HandleUpnp)p;
	sUpnpStruct* s = (sUpnpStruct*)h;
	s->httpSock=0;

	if (yiupnpGetExternalAddressx(h, szExternalAddr)==False)
	{
		return 0;
	}

	return (HandleUpnp)yitoheap(p, sizeof(sUpnpStruct));
}
