#include "yiHttpClient.h"


// -------------------------
char* yiHttpRequest(unsigned int server_addr, int port, const char* pszRequest, int* nRecvedSize)
{
	int sock = yiconnect(server_addr, port, 3);
	char len = strlen(pszRequest);
	char _sendBuffer[256];
	char* sendBuffer=_sendBuffer;
	BOOL bIsAllocated=False;

	if (len>(256-2))
	{
		bIsAllocated=True;
		sendBuffer = (char*)yialloc(len+3);
	}

	memcpy(sendBuffer, pszRequest, len);
	sendBuffer[len+1]='\n';
	sendBuffer[len+2]='\n';
	sendBuffer[len+3]=(char)0;

	if (yisend(sock, pszRequest, len+3, False)!=len+3)
	{
		goto ERROR;
	}

	if (yirecv(sock, 
ERROR:
	if (bIsAllocated==True)
	{
		yifree(sendBuffer);
	}
	return 0;
}

