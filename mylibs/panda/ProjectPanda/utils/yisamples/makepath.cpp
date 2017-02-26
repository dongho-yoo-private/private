#include <stdio.h>
#include "yi.h"
#include "yistd.h"

main(int n, char** argv)
{
	if (yiMakePath(argv[1], False)==True)
	{
		printf("yiMakePath success (%s)\n", argv[1]);
	}
}

