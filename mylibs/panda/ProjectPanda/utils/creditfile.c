#include <stdio.h>
#include <string.h>
#include "yi.h"
#include "yifile.h"

typedef struct __temp__ {
	int id;
	int abc;
	char szName[32];
	Int32 reserved;
} sTemp;

#define TEST_FILE_NAME "/var/soft_yotsuya/temp/credittest.dat"
#define TEST_FILE_NAME2 "/var/soft_yotsuya/temp/credittest2.dat"

int main()
{
	char buffer[256];
	sTemp data, *pData;
	char* p;
	int i;

	data.id=10;
	data.abc = 20;
	strcpy(data.szName, "dongho");

	if (yiFileCreditWrite(TEST_FILE_NAME, &data, credit_sizeof(data), 8)!=eFE_OK)
	{
		printf("write error\n");
		return 0;
	}	

	p = yiFileCreditLoad(TEST_FILE_NAME, 0);

	if (p==0)
	{
		printf("load error\n");
		return 0;
	}

	{
		sTemp* pTemp = (sTemp*)p;

		printf("%d %d %s\n", pTemp->id, pTemp->abc, pTemp->szName);
	}

	for (i=0; i<10; i++)
	{
		data.id = i;
		data.abc +=i;
		if (yiFileCreditWriteRecord(TEST_FILE_NAME2, &data, -1, credit_sizeof(data), 8)!=eFE_OK)
		{
			printf("write error %s\n", TEST_FILE_NAME2);
			return 0;
		}
	}

	for (i=0; i<10; i++)
	{
		if (yiFileCreditReadRecord(TEST_FILE_NAME2, &data, 9-i)!=eFE_OK)
		{
			printf("read error %s\n", TEST_FILE_NAME2);
			return 0;
		}
		printf("id:%d, abc:%d, name:%s\n", data.id, data.abc, data.szName);
	}

	if (yiFileCreditReadRecords(TEST_FILE_NAME2, buffer, 3, 5)!=eFE_OK)
	{
		printf("read error %s\n", TEST_FILE_NAME2);
		return 0;
	}

	pData=(sTemp*)buffer;
	for (i=0; i<5; i++)
	{
		printf("id:%d, abc:%d, name:%s\n", pData[i].id, pData[i].abc, pData[i].szName);
	}

	return 0;

}

