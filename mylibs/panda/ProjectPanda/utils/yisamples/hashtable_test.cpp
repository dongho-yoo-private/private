#include "../yi.h"
#include "../yistd.h"
#include "../yiHashTable.h"
#include "../yitime.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void xOnRemoved(void* p)
{
	//printf("xOnRemoved called\n");
}

// ----------------------------------
static void xCreateTemporaryName(char* buffer)
{
	int n = (rand()%12) + 4;
	int interval = 'Z'-'A';

	for (int i=0; i<n; i++)
	{
		int k = rand()%2;
		*buffer = (rand()%interval)+(k ?'A':'a'); 
		buffer++;
	}
	*buffer=0;
}

// ----------------------------------
main(int argc, char** argv)
{
	int maxDupCnt=0;
	int dupcnt=0;
	int dupsum=0;
	int nLayer=atoi(argv[1]);
	unsigned int nIndexMask;
	char xname[32];
	cyiHashTable table(nLayer, 8, -1, xOnRemoved, False);

	nIndexMask=table.GetIndexMask();
	srand(time(0));

	memset(xname, 0, sizeof(xname));
	int n = atoi(argv[2]);
	syiSecs sec = yiGetCurrentSec();
	for (int i=0; i<n; i++)
	{
		char name[32];
		unsigned char* p;
		//printf("Enter Name: ");
		//scanf("%s", name);
		//

		memset(name, 0, sizeof(name));
		xCreateTemporaryName(name);
		unsigned int n = table.Add(name, 0);
		p=(unsigned char*)&n;

		if (n==0)
		{
			continue;
		}

		//printf("(%d) name:%s id>%u:%u:%u:%u\n", i, name, 0xFF&p[0], 0xFF&p[1], 0xFF&p[2], 0xFF&p[3]);
		int k = n&nIndexMask;

		if (k!=1)
			dupcnt++;

		if (i==100)
		{
			strcpy(xname, name);
		}

		/*if (i==101)
		{
			n = table.GetKey(xname);
			p=(unsigned char*)&n;
			printf("GetKey(%s) result %u:%u:%u:%u\n", xname, 0xFF&p[0], 0xFF&p[1], 0xFF&p[2], 0xFF&p[3]);
		}*/

		maxDupCnt = (maxDupCnt > k) ? maxDupCnt:k;
/*

		n = table.GetKey(name);
		p=(unsigned char*)&n;
		printf("GetKey(%s) result %u:%u:%u:%u\n", name, 0xFF&p[0], 0xFF&p[1], 0xFF&p[2], 0xFF&p[3]);

		char* a;
		table.GetValue(n, &a);
		printf("GetValue(%u) result %s\n", n, a);
*/
	}	

	syiSecs secResult = yiCompareSecs(sec, yiGetCurrentSec());
	printf("total used memory: %u Kbytes, total element:%d, dup:%d, maxDup:%d Total:%d.%06dseconds\n", (table.GetUsedMemorySize()+table.GetCount()*(16+12))/1024, table.GetCount(), dupcnt, maxDupCnt, secResult.sec, secResult.usec);

	sec = yiGetCurrentSec();
	n = table.GetKey(xname);
	unsigned char* p=(unsigned char*)&n;
	secResult = yiCompareSecs(sec, yiGetCurrentSec());
	printf("get name(%s) id: %u.%u.%u.%u, total:%d.%06dseconds\n", xname, 0xFF&p[0], 0xFF&p[1], 0xFF&p[2], 0xFF&p[3], 
						secResult.sec, secResult.usec);

}

