#include "syEncoder.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>

typedef struct __secs__
{
	long sec;
	long usec;
} swsxSecs;

swsxSecs wsxGetCurrentSec()
{
	swsxSecs sec;
	struct timeval tv;/* = {0,0};*/

	if (gettimeofday(&tv, 0)==-1)
	{
		return sec;
	}

	sec.sec=tv.tv_sec;
	sec.usec=tv.tv_usec;

	return sec;
}

swsxSecs wsxDiffSec(swsxSecs e, swsxSecs s)
{
	swsxSecs sec = {e.sec-s.sec, e.usec-s.usec};

	sec.sec = sec.usec < 0 ? --sec.sec : sec.sec;
	sec.usec = sec.usec < 0 ? 1000000+sec.usec : sec.usec;
/*	printf("%d.%d - %d.%d = %d.%d\n", );*/

	return sec;	
}


/**/
void wsxTraceDump(void* p, int dumpSize, const char* name)
{
	char* pChar = (char*)p;
	int i=0;

	printf("------------ dump(%s) %d bytes ------------ \n", name, dumpSize);
	for (i=0; i<dumpSize; i++) 
	{
		if (i&&i%16==0) putchar('\n');
		printf("%02x", 0x000000FF&(int)*pChar++);
	}

	putchar('\n');
}


main()
{
	srand(time(NULL));
	int n = rand();
	int mask = rand();

	int a, b, c, d;
	csyEncoder::securityCode("Yotsuya Igo Center", n, mask, &a, &b, &c, &d);
	printf("randNo: %x, mask: %x\n", n, mask);
	printf("%x %x %x %x\n", a, b, c, d);


	printf("randNo: %x, mask: %x\n", a, b);
	csyEncoder* en = new csyEncoder(a, b);

	char buffer[256];
	char outbuffer[512];
	char decbuffer[256];

	const int nDecSize=128;

	printf("input string (rnd:%x)\n", n);
	scanf("%s", buffer);
	wsxTraceDump(buffer, nDecSize, "input");

	swsxSecs sec = wsxGetCurrentSec();
	csyEncoder::estr(buffer);

	wsxTraceDump(buffer, 32, "estr");

	csyEncoder::dstr(buffer);

	wsxTraceDump(buffer, 32, "dstr");



//for (int i=0; i<1000000; i++)
//{
	en->Encode(outbuffer, buffer, nDecSize);

	wsxTraceDump(outbuffer, nDecSize<<1, "encode");

	en->Decode(decbuffer, outbuffer, nDecSize);

	wsxTraceDump(decbuffer, nDecSize, "decode");
//}
	sec = wsxDiffSec(wsxGetCurrentSec(), sec);

	if (memcmp(decbuffer, buffer, nDecSize)==0)
	{
		printf("time.... %d.%06d\n", sec.sec, sec.usec);
		printf("decode same (%s)\n", decbuffer);
	}
	else
	{
		printf("time.... %d.%06d\n", sec.sec, sec.usec);
		printf("decode not same (%s)\n", decbuffer);
	}
}

