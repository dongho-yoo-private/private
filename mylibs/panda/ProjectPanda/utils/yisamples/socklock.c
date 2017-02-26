#include <stdio.h>
#include "yisock.h"
#include "yithread.h"
#include "yimutex.h"
#include <unistd.h>
#include <pthread.h>

int g_sock=0;
HandleMutexx g_hMutex;

// ----------------------------------
void* OnClientThread(void* param)
{
	int sock = yiconnect(yiitol("127.0.0.1"), 12345, 3);
	int nRead;
	char buffer[8192];

	while(g_sock==0);

	yiMutexLx(0);
	if ((nRead=yisend(g_sock, buffer, 8192, False))!=8192)
	{
		printf("send error (%d), %d\n", param, nRead);
		yiMutexUnLx(g_hMutex );
		return 0;
	}
	printf("send success(%d) \n", param);
	usleep(1000000);
	yiMutexUnLx(g_hMutex );
	return 0;
}


main()
{
	int listenSock;
	int i=0, port;
	unsigned char* pAddress;
	address_t addr;
	g_hMutex = yiMutexCreatex();

	for (i=0; i<2; i++)
	{
		char addr[64];
		address_t n =yigetlocaladdress(addr, i);

		if (n==0)
			break;
		//printf("localhost %u, %s\n", n, addr);//0xFF&pAddress[0], 0xFF&pAddress[1], 0xFF&pAddress[2], 0xFF&pAddress[3]);
		pAddress = &n;
		printf("localhost %u, %u.%u.%u.%u\n", n, 0xFF&pAddress[0], 0xFF&pAddress[1], 0xFF&pAddress[2], 0xFF&pAddress[3]);
		printf("last is %d\n", (0xFF000000&n)>>24);//, 0xFF&pAddress[0], 0xFF&pAddress[1], 0xFF&pAddress[2], 0xFF&pAddress[3]);
	}


	for (i=0; i<10; i++)
	{
		yiThreadStartx(OnClientThread, (void*)i, 0);
	}
	
	listenSock= yilisten(0, 12345, 10);

	port = yigetport(listenSock, &addr);
	pAddress = (unsigned char*)&addr;
	printf("localhost %d, %u, %u.%u.%u.%u\n", port, addr, 0xFF&pAddress[0], 0xFF&pAddress[1], 0xFF&pAddress[2], 0xFF&pAddress[3]);

	while(1)
	{
		int sockClient = yiaccept(listenSock, 0, 0);

		if (g_sock==0)
		{
			g_sock=sockClient;
		}
		printf("accepted\n");
	}	
}

