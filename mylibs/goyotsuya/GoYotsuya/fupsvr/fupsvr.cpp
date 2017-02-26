// fupsvr.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "yistd.h"
#include "yisock.h"

#define xDEFAULT_PORT_NO ((short)1532)


// ---------------------------
int _tmain(int argc, _TCHAR* argv[])
{

	yiInitWinsock(2,2);

	int sock = yiudp(0, xDEFAULT_PORT_NO);

	if (sock<0)
	{
		printf("socket error \n");
	}

	// ---------------------------
	while(1)
	{
		syipacketu pack;
		address_t addr=0;
		int port=0;
		char buffer[MAX_UDP_PACKET_SIZE];

		if (yirecvxu(sock, &pack, buffer,  0, -1, &addr, &port)<0)
		{
			printf("receive packet falure.....\n");
			continue;
		}

		printf("receive packet..... from %d,%d\n", addr, port);

		int size=0;
		void* p = yiFileLoad("c:\\vfrwin\\a.jpg", &size);

		if (p==0)
		{
			printf("file not found!!");
			yiclose(sock);
			break;
		}

		//yisendxu(sock, 0, 0,0, 0, 0, addr, port,False, 0);

		//size=1024;
		printf("sendfile %d bytes\n", size);
		if (yisendxul(sock, 0, 0, 0, p, size, addr, port, 0)!=size)
		{
			printf("send error %d\n", size);
		}

		yiFileUnload(p);

	}
	
	return 0;
}	

