#include <stdio.h>
#include "yistd.h"
#include "yisock.h"

main()
{
	int sock = yilisten(0, 32333, 5);
	int sockClient;
	address_t address;
	int port;
	char buffer[128];

	if (sock<0)
	{
		printf("bind error\n");
		return 0;
	}
	
	sockClient = yiaccept(sock, &address, &port);

	if (sockClient<0)
	{
		printf("accept error\n");
		return 0;
	}

	int timeout=-1;
	if (yirecv(sockClient, buffer, 128, &timeout)<1)
	{
		printf("read failure \n");
		close(sockClient);
		return 0;
	}

	printf("%s\n", buffer);

	close(sock);

}

