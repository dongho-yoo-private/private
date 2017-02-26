#include "../yi.h"
#include "../yisock.h"
#include <string.h>

main(int argc, char** argv)
{
	int sock = yiudp(0, 0);
	int port = yigetport(sock, 0);
	char buffer[16];

	printf("port is %d\n", port);

	if (argv[1]==0)
	{
		address_t addr=0;
		int port=0;
		int n;

		n = yirecvu(sock, buffer, 16, WAIT_INFINITE, &addr, &port);

		if (n>0)
		{
			/*if (yiconnectudp(sock, addr, port)==-1)
			{
				printf("connect error !!");
				close(sock);
				return 0;
			}*/
			printf("connect %u:%d\n", addr, port);
		}
		else
		{
			printf("recv error !!");
			close(sock);
			return 0;
		}
		printf("recv %s\n", buffer);

		while(1)
		{
			address_t addr=0;
			int port=0;
			n = yirecvu(sock, buffer, 16, WAIT_INFINITE, &addr, &port);
			//n = yirecv(sock, buffer, 16, WAIT_INFINITE);
			printf("recv %s\n", buffer);

			if (buffer[0]=='q')
			{
				printf("quit....\n");
				break;
			}
		}
		close(sock);
		return 0;
	}
	else
	{
		int port = atoi(argv[1]);
		if (argv[2]==0)
		{
			strcpy(buffer, "hellow");
		}
		else
		{
			strcpy(buffer, argv[2]);
		}
		if (yiconnectudp(sock, yiitol("127.0.0.1"), port)==-1)
		{
			printf("yiconnectudp fialure....\n");
			return 0;
		}
		yisend(sock, buffer, 16, False);
	}

	{
		int sock2 = yiudp(0, 0);
		int port = atoi(argv[1]);

		strcpy(buffer, "xxxxxx");
		if (yiconnectudp(sock2, yiitol("127.0.0.1"), port+1)==-1)
		{
			printf("yiconnectudp fialure....2\n");
			return 0;
		}
		yisend(sock2, buffer, 16, False);
		yiclose(sock2);
	}

	strcpy(buffer, "quit....");
	yisend(sock, buffer, 16, False);
	
	close(sock);
}

