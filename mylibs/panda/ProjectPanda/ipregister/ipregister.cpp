#include "yi.h"
#include "yistd.h"
#include "yisock.h"
#include "yiEncoder.h"
//#include "sycenv.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>


static Bool PutFtpCommand(int sock, char* szCommand, char* buffer)
{
	int timeout=1000;
	int n=0;
	int* pTimeout=&timeout;
	printf("%s", szCommand);
	yisend(sock, szCommand, strlen(szCommand), 0);
RECEIVE:
	if ((n=yirecv(sock, buffer, 1024, pTimeout))<0)
	{
		YIERROR("ftp: %s error\n", szCommand);
		close(sock);
		return False;
	}

	if (n==0)
	{
		close(sock);
		printf("closed socket....\n");
		return True;
	}

	buffer[n]=0;
	printf(buffer);

	timeout=200;
	if ((n=yiwaitsock(sock, &timeout))>0)
	{
		printf("yiwaitsock... ret=%d\n", n);
		pTimeout=0;
		goto RECEIVE;
	}
	printf("success\n", szCommand);
	return True;
}
#include "yiupnp.h"

main(int argc, char** argv)
{
	HandleUpnp hUpnp=0;
	char szExternalAddress[32];
	char szServerAddress[256];
	char szId[64];
	char szPassword[64];
	char szIpAddress[32];
	int size;
	int portNo=10003;
	int upnpSize=0;
	Bool bUpnpImportSuccess=False;
	address_t local;
	char szLocalAddress[32];
	Bool bIsForced=False;

	szLocalAddress[0]=0;
	yiLogStart(0, "ipregister");

	if (argv[1]==0)
		local = yigetlocaladdress(szLocalAddress, 1);
	else
		strcpy(szLocalAddress, argv[1]);

	if (argv[1]!=0 && strncmp(argv[1], "-f", 2)==0)
		bIsForced=True;
	else if (argv[2]!=0 && strncmp(argv[2], "-f", 2)==0)
		bIsForced=True;

	YIINFO("start ipregister! local address(%s)\n", szLocalAddress);
	char* p = (char*)yiFileLoad("/usr/local/goyotsuya/bin/ipregister.dat", &size);	

	if (p==0)
	{
		YIERROR("ipregister! file not found!");
		return 0;
	}

	sscanf(p, "addr:%s\nport:%d\nid:%s\npass:%s", szServerAddress, &portNo, szId, szPassword);
	yifree(p);

	//YIDEBUG("%s:%d, %s, %s\n", szServerAddress, portNo, szId, szPassword);

	void* pUpnp = yiFileLoad("/usr/local/goyotsuya/bin/ipregister.upnp", &size);

	if (pUpnp)
	{
		hUpnp=yiupnpImport(pUpnp);
		yifree(pUpnp);
	}

	if (hUpnp==0)	
		hUpnp=yiupnpCreatex();
	else
		bUpnpImportSuccess=True;

	if (hUpnp==0)
	{
		YIERROR("yiupnpCreate() failure!\n");
		return 0;
	}

	//YIDEBUG("yiupnpCreate() success\n");
	if (yiupnpGetExternalAddressx(hUpnp, szExternalAddress)==False)
	{
		YIERROR("yiupnpGetExternalAddressx() failure!\n");
		return 0;
	}

	char* pExAddr=(char*)yiFileLoad("/usr/local/goyotsuya/bin/ipregister.exa", 0);

	if (bIsForced==False &&
			pExAddr)
	{
		if (strcmp(pExAddr, szExternalAddress)==0)
		{
			YIINFO("address is not changed (%s).... exit\n", szExternalAddress);
			return 0;
		}
	}

	yiFileWrite("/usr/local/goyotsuya/bin/ipregister.exa", szExternalAddress, 0, strlen(szExternalAddress)+1);


	if (bUpnpImportSuccess==False)
	{
		pUpnp = (void*)yiupnpExport(hUpnp, &upnpSize);
		yiFileWrite("/usr/local/goyotsuya/bin/ipregister.upnp", pUpnp, 0, upnpSize);
	}

	//yiupnpDestroyx(hUpnp);

	YIDEBUG("External Address: %s\n", szExternalAddress);

	address_t addr = yihtol(szServerAddress, 0);
	int sock=yiconnect(addr, 21, 0);
	
	if (sock<0)
	{
		YIERROR("ftp connect failure(%s,%d)\n", szServerAddress);
		remove("/usr/local/goyotsuya/bin/ipregister.exa");
		return 0;
	}

	char szRecv[1024];
	char szCmd[128];
	int timeout=1000;

	//usleep(500);
	if (yirecv(sock, szRecv, 1024, &timeout)<0)
	{
		YIERROR("ftp: recv error\n");
		close(sock);
		remove("/usr/local/goyotsuya/bin/ipregister.exa");
		return False;
	}
	printf(szRecv);

	sprintf(szCmd, "USER %s\n", szId);

	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		close(sock);
		remove("/usr/local/goyotsuya/bin/ipregister.exa");
		return 0;
	}

	sprintf(szCmd, "PASS %s\n", szPassword);
	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		close(sock);
		remove("/usr/local/goyotsuya/bin/ipregister.exa");
		return 0;
	}


	sprintf(szCmd, "TYPE I\n");
	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		close(sock);
		remove("/usr/local/goyotsuya/bin/ipregister.exa");
		return 0;
	}

		
	sprintf(szCmd, "PASV\n");
	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		close(sock);
		remove("/usr/local/goyotsuya/bin/ipregister.exa");
		return 0;
	}

	printf("pasv success!");
	char* pRemoteAddr=strstr(szRecv, "(");
	if (pRemoteAddr==0)
	{
		close(sock);
		remove("/usr/local/goyotsuya/bin/ipregister.exa");
		return 0;
	}
	int ipaddr[4];
	int port[2]={0,0};
	pRemoteAddr++;
	//for (int i=0; i<4; i++)
	//	pRemoteAddr=strstr(pRemoteAddr, ",")+1;

	printf(pRemoteAddr);

	
	
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
		YIERROR("put port error %d,%d, %d\n", (int)port[0], (int)port[1], (int)nPutPort);
		close(sock);
	}

	sprintf(szCmd, "STOR goyotsuya.txt\n");
	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		close(sock);
		return 0;
	}

	char szSendTextBuffer[128];

	sprintf(szSendTextBuffer, "%s:%d", szExternalAddress, portNo);

	int nSended=yisend(sockPutPort, (void*)szSendTextBuffer, strlen(szSendTextBuffer), 0);
	close(sockPutPort);

	printf("send %d bytes success\n", nSended);

	if (yirecv(sock, szRecv, 1024, &timeout)<0)
	{
		YIERROR("ftp: recv error!\n");
		close(sock);
		//return False;
	}
	printf(szRecv);

	sprintf(szCmd, "QUIT\n");
	if (PutFtpCommand(sock, szCmd, szRecv)==False)
	{
		close(sock);
		return 0;
	}
	close(sock);

	p = (char*)yiFileLoad("/usr/local/goyotsuya/bin/ipregister.port", &size);	

	if (p==0)
	{
		printf("file not found! ipregister.port\n");
		return 0;
	}
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
				if (yiupnpAddPortMapx(hUpnp, szLocalAddress, port, True, "GoYotsuya!")==True)
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

	YIINFO("ipregister success!\n");

	return 0;
}

