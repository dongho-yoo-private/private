#include "../yistd.h"

int main(int argc, char** argv)
{
	int size;
	void* p = yiFileLoad(argv[1], &size);
	char szFileName[256]={0,};

	if (p==0)
	{
		printf("File not found....%s\n", argv[1]);
		return 0;
	}

	if (argv[2])
	{
		if (strcmp(argv[2], "-x")==0)
		{
			yifree(p);
			p=0;
			size=0;
		}
	}


	strcpy(szFileName, argv[1]);
	syipDetailForBinaryMode mode;
	yipSetDefaultParam(&mode);

	mode.nExtraIdSize=256;
	mode.bIsDeleteAtTime=False;
	mode.nTotalSizeLimitted=1024*1024;
	mode.nElementCount=10;

	Handle hBin = yipOpen("sample.bin", &mode);
	yipWriteBinaryById(hBin, 0, szFileName, p, size);	
	yipClose(hBin);

	yifree(p);

	return 0;
}
