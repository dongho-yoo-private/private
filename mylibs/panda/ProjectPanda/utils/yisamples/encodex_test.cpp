#include "../yiencodex.h"
#include "../yitrace.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

main(int argc, char** argv)
{
	char buffer[4096];
	srand(time(0));
	int randx = rand();
	int n = atoi(argv[1]);

	for (int i=0; i<4096; i++)
	{
		buffer[i]=i;//1;//(char)i;
	}

	printf("randx:%x\n", randx);
	yiTraceDump(buffer, 4096, 32, "before encode.");

	yiencode(buffer, n, randx);

	yiTraceDump(buffer, 4096, 32, "after encode.");

	yidecode(buffer, n, randx);

	yiTraceDump(buffer, 4096, 32, "after decode.");


}

