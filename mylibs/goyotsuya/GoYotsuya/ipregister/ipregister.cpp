#include "yi.h"
#include "yistd.h"
#include "yisock.h"
#include "yiEncoder.h"
//#include "sycenv.h"

#include <string.h>
//#include <unistd.h>
#include <stdlib.h>


static Bool PutFtpCommand(int sock, char* szCommand, char* buffer)
{
	int timeout=1000;
	int n=0;
	printf("%s", szCommand);
	yisend(sock, szCommand, strlen(szCommand), 0);
	if ((n=yirecv(sock, buffer, 1024, &timeout))<0)
	{
		printf("recv error (%s)\n", szCommand);
		yiclose(sock);
		return False;
	}
	buffer[n]=0;
	printf(buffer);
	return True;
}
#include "yiupnp.h"

int main(int argc, char** argv)
{
	HandleUpnp hUpnp;
	char szExternalAddress[32];
	char szServerAddress[256];
	char szId[64];
	char szPassword[64];
	char szIpAddress[32];
	int size;
	int portNo=10003;

	char* p = (char*)yiFileLoad(".\\ipregister.dat", &size);	

	if (p==0)
	{
		printf("file not found\n");
	}

	sscanf(p, "addr:%s\nport:%d\nid:%s\npass:%s", szServerAddress, &portNo, szId, szPassword);
	printf("%s:%d, %s, %s\n", szServerAddress, portNo, szId, szPassword);

	yifree(p);

	::yiInitWinsock(2, 2);
	hUpnp=yiupnpCreatex();
	if (hUpnp==0)
		return 0;
	if (yiupnpGetExternalAddressx(hUpnp, szExternalAddress)==False)
	{
		return 0;
	}

	//yiupnpDestroyx(hUpnp);

	printf("External Address: %s\n", szExternalAddress);

	address_t addr = yihtol(szServerAddress, 0);
	int sock=yiconnect(addr, 21, 0);
	
	if (sock<0)
	{
		printf("connect failure(%s,%d)\n", szServerAddress);
	}

	char szRecv[1024];
	char szCmd[128];
	int timeout=1000;

	//usleep(500);
	if (yirecv(sock, szRecv, 1024, &timeout)<0)
	{
		printf("recv error \n");
		yiclose(sock);
		return False;
	}
	printf(szRecv);

	sprintf(szCmd, "USER %s\n", szId);

	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		yiclose(sock);
		return 0;
	}

	sprintf(szCmd, "PASS %s\n", szPassword);
	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		yiclose(sock);
		return 0;
	}


	sprintf(szCmd, "TYPE I\n");
	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		yiclose(sock);
		return 0;
	}

		
	sprintf(szCmd, "PASV\n");
	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		yiclose(sock);
		return 0;
	}

	char* pRemoteAddr=strstr(szRecv, "(");
	if (pRemoteAddr==0)
	{
		yiclose(sock);
		return 0;
	}
	int ipaddr[4];
	int port[2]={0,0};
	pRemoteAddr++;
	//for (int i=0; i<4; i++)
	//	pRemoteAddr=strstr(pRemoteAddr, ",")+1;

	//printf(pRemoteAddr);
	
	
	sscanf(pRemoteAddr, "%d,%d,%d,%d,%d,%d)", &ipaddr[0],
				&ipaddr[1],
				&ipaddr[2],
				&ipaddr[3],
						&port[0],
						&port[1]);

	printf("port:%d, %d\n", port[0], port[1]);
	short nPutPort = (port[0]<<8)|port[1];
	address_t* putAddr=(address_t*)ipaddr;	


	int sockPutPort=yiconnect(addr, nPutPort, 0);

	if (sockPutPort<0)
	{
		printf("put port error %d,%d, %d\n", (int)port[0], (int)port[1], (int)nPutPort);
		yiclose(sock);
	}

	sprintf(szCmd, "STOR goyotsuya\n");
	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		yiclose(sock);
		return 0;
	}

	char szSendTextBuffer[128];

	sprintf(szSendTextBuffer, "%s:%d", szExternalAddress, portNo);

	int nSended=yisend(sockPutPort, (void*)szSendTextBuffer, strlen(szSendTextBuffer), 0);
	yiclose(sockPutPort);

	printf("send %d bytes success\n", nSended);

	if (yirecv(sock, szRecv, 1024, &timeout)<0)
	{
		printf("recv error \n");
		yiclose(sock);
		return False;
	}
	printf(szRecv);

	sprintf(szCmd, "QUIT\n");
	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		yiclose(sock);
		return 0;
	}
	//getchar();
	yiclose(sock);

	p = (char*)yiFileLoad(".\\ipregister.port", &size);	

	if (p==0)
		return 0;
	p[size]=0;

	char* pIndex=p;
	char* pStartIndex=p;
	while(*pIndex)
	{
		if (*pIndex=='\n' ||
			*pIndex=='\r')
		{
			*pIndex=0;
			int port = atoi(pStartIndex);
			
			if (port>0)
			{
				
				printf("open port:%d .....\n", port);
				if (yiupnpAddPortMapx(hUpnp, "192.168.1.7", port, True, "GoYotsuya!")==True)
				{
					printf("open port:%d success\n", port);
				}
			}
			pStartIndex=pIndex+1;	
		}
		pIndex++;	
	}
	yiupnpDestroyx(hUpnp);
	yifree(p);
	
	getchar();

	return 0;
}

