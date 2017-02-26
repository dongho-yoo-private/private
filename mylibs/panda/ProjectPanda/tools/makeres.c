#include <stdio.h>
#include "yi.h"
extern const char* g_out;

main(int argc, char** argv)
{
	char* p;
	int size;
	int n;
	int i;

	if (argv[1]==0)
	{
		printf("%s", g_out);
		return 0;	
	}

	p = (char*)yiFileLoad(argv[1], &size);

	printf("---------------------------- start ---------------------------- \n");
	for (i=0; i<size; i++)
	{
		if (i%16==0)
		printf("\n");
		printf("0x%02x,", (int)((unsigned char)p[i]));
			
	}	
	printf("\n---------------------------- end ---------------------------- \n");
	printf("%d bytes \n", size);

	yiFileUnload(p);
}

