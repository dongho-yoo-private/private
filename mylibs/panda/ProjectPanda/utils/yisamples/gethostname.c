#include <stdio.h>
#include <netdb.h>


main(int argc, char** argv)
{
	int i=0;
	struct hostent* host = gethostbyname(argv[1]);

	if (host==0)
	{
		printf("gethostbyname error\n");
		return 0;
	}

	for(i= 0 ; host->h_addr_list[i] ; i++) 
	{
		unsigned char* p = (unsigned char*)host->h_addr_list[i];
		printf("ip= %d.%d.%d.%d (%x)\n", p[0], p[1], p[2], p[3], *((long*)p));
	}
		
}

