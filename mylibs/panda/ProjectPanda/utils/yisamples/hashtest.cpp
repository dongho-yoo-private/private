#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "../yiHashFunc.h"
#include "../yifile.h"
#include "../yiList.h"
#include "../yipsr.h"

#define BIT_SHIFT 8

main(int argc, const char** argv)
{

	char name[256];
	int array1[1<<BIT_SHIFT ];
	int array2[1<<BIT_SHIFT];
	int array3[1<<BIT_SHIFT];
	int array_size=1<<BIT_SHIFT;
	cyiList list;

	char* p = (char*)yiFileLoad("namelist",0);
	yiPsrSplit(p, "\n", list);
	

	for (int i=0; i<array_size; i++)
	{
		array1[i]=0;
		array2[i]=0;
		array3[i]=0;
	}

	int gusuu1=0;
	int gusuu2=0;
	int gusuu3=0;
	int cnt=0;
	for (cyiNode* pNode=list.GetNodeTopFrom(0); pNode!=0; pNode=pNode->np)
	{
		//printf("(%03d) input name: ", cnt++);
		//scanf("%s", name);
		int nRand1 = rand();
		int n1= yihf_additional((char*)pNode->ref, BIT_SHIFT);
		int n2= yihf_encoded_additional((char*)pNode->ref, BIT_SHIFT);
		int n3= yihf_general((char*)pNode->ref, BIT_SHIFT);

		//int n1=yihf_additional((char*)pNode->ref, 8);
		//int n2=yihf_encoded_additional((char*)pNode->ref, 8);
		//int n2=yihf_sub((char*)pNode->ref, 8);

		array1[n1]++;
		array2[n2]++;
		array3[n3]++;

		if (n1%2==0)
			gusuu1++;

		if (n2%2==0)
			gusuu2++;

		if (n3%2==0)
			gusuu3++;

		printf("%s (%d, %d, %d), dup:(%d, %d, %d)\n", pNode->ref, n1, n2, n3, array1[n1], array2[n2], array3[n3]);

		if (cnt++==array_size)
			break;
	}

	int dup1=0;	
	int dup2=0;
	int dup3=0;
	int zero1=0;
	int zero2=0;
	int zero3=0;
	int maxdup1=1;
	int maxdup2=1;
	int maxdup3=1;
	int maxdupId1=0;
	int maxdupId2=0;
	int maxdupId3=0;

	for (int i=0; i<array_size; i++)
	{
		if (array1[i]==0)
		{
			zero1++;
		}
		if (array2[i]==0)
		{
			zero2++;
		}
		if (array3[i]==0)
		{
			zero3++;
		}
		if (array1[i]>1)
		{
			dup1+=(array1[i]-1);
			if (maxdup1<array1[i])
			{
				maxdup1=array1[i];
				maxdupId1=i;
			}
		}
		if (array2[i]>1)
		{
			dup2+=(array2[i]-1);
			if (maxdup2<array2[i])
			{
				maxdup2=array2[i];
				maxdupId2=i;
			}
		}
		if (array3[i]>1)
		{
			dup3+=(array3[i]-1);
			if (maxdup3<array3[i])
			{
				maxdup3=array3[i];
				maxdupId3=i;
			}
		}
	}

	float total = array_size;//(float)list.count();
	printf("+++++++ result (%d) +++++++ \n", list.count());
	printf("type1: maxdup: %d, maxdupId:%d, dupcnt: %d(%.2f%%) zerocnt: %d, gu:%.2f\n", maxdup1, maxdupId1, dup1, (float)(100.0*(float)dup1/total), zero1, (float)((float)gusuu1*100.0/total));
	printf("type2: maxdup: %d, maxdupId:%d, dupcnt: %d(%.2f%%) zerocnt: %d, gu:%.2f\n", maxdup2, maxdupId2, dup2, (float)(100.0*(float)dup2/total), zero2, (float)((float)gusuu2*100.0/total));
	printf("type3: maxdup: %d, maxdupId:%d, dupcnt: %d(%.2f%%) zerocnt: %d, gu:%.2f\n", maxdup3, maxdupId3, dup3, (float)(100.0*(float)dup3/total), zero3, (float)((float)gusuu3*100.0/total));
}

