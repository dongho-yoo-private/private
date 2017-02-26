#include "yi.h"
#include "yistd.h"
#include "yisock.h"
#include "yiEncoder.h"
#include "sycenv.h"

#include <string.h>
#include <unistd.h>
#include "yi.h"

main(int argc, char** argv)
{
	char szGetCommand[1024];
	char buffer[512];
	address_t address=yihtol(argv[1], 0);
	int sock = yiconnect(address, 80, 0);

	if (sock<0)
	{
		printf("connect failure %s\n", argv[1]);
		return 0;
	}


	sprintf(szGetCommand, "GET %s HTTP/1.0\n\n", argv[2]);

	if (send(sock, szGetCommand, strlen(szGetCommand), 0)<=0)
	{
		printf("send failure %s\n", argv[1]);
		close(sock);
		return 0;
	}

	if (yirecv(sock, buffer, 512, 0)<=0)
	{
		printf("recv failure %s\n", argv[1]);
		close(sock);
		return 0;
	}

	buffer[511]=0;
	printf(buffer);

	close(sock);

}

