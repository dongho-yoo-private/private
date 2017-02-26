#include "yistd.h"
#include "yitime.h"
#include <string.h>


main(int argc, char** argv)
{
	char buffer[4096];
	char* p = (char*)yialloc(4096);
	int n = atoi(argv[1]);
	int i=0;
	syiSecs start = yiGetCurrentSec();

	if ((int)&buffer[4]%16)
	{
		return 0;
	}
	yimemzero(buffer, n);

	if (argv[2]==0)
	{	
		printf("yimemzero\n");
		for (i=0; i<1000000; i++)
		{
			yimemzero(&buffer[4], n);
			//yimemcpy(p, &buffer[4], n);
		}
	}
	else
	{
		printf("memzero\n");
		for (i=0; i<1000000; i++)
		{
			memset(&buffer[4], 0, n);
			//memcpy(p, &buffer[4], n);
		}
	}

	start = yiCompareSecs(start, yiGetCurrentSec());
	yiTrace("total %d.%06d sec\n", start.sec, start.usec);
	//yiTraceDump(p, n, "result yimemzero");	
}

