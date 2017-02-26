#include "yi.h"
#include "yistd.h"
#include "yifile.h"
#include "yiHashFunc.h"
#include "sycHashData.h"
#include "sycenv.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>


typedef struct xuserinfo {
	unsigned int id;
	char name[32];	
} suserinfo;


// --------------------------------------
void addwrite(const char* pszRootPath, int n, Bit32 layer, suserinfo* pInfo)
{
	char deffile[256];
	int cnt=0;
/*
	sprintf(deffile, "%s/%03d/%03d/%03d/defs", pszRootPath, __32to8__(layer, 3), __32to8__(layer, 2), __32to8__(layer, 1));
	ssycHashDefFile* p = (ssycHashDefFile* )yiFileCreditLoad(deffile, 0);
	ssycHashDefFile* pDef=0;

	if (p!=0)
	{
		assert(p->n==p->n2);

		cnt = p->n;
		pDef=p;
	}

	ssycHashDefFile init;
	if (pDef==0)
	{
		init.n=cnt;
		init.n2=cnt;

		pDef=&init;
	}
*/
	pInfo->id = (layer & 0xFFFFFF00) | n;

	char szFileName[256];
	sprintf(szFileName, "%s/%03d/%03d/%03d/%d", pszRootPath,  __32to8__(layer, 3), __32to8__(layer, 2), __32to8__(layer, 1), n);
	if (yiFileCreditWrite(szFileName, pInfo, sizeof(*pInfo), 1)==eFE_OK)
	{
		printf("write %s succcess\n", szFileName);
	}
/*
	pDef->n++;
	pDef->n2++;

	if (yiFileCreditWrite(deffile, pDef, 0, sizeof(*p), False)==True)
	{
		printf("write %s succcess\n", deffile);
	}

	if (p!=0)
	{
		yiFileUnload(p);
	}
*/
}

#define BIT_SHIFT 8

main()
{
	csycHashData data;
	ssycHashData* pHashData;
	char root[256];

	sprintf(root, "%s/root", csycEnv::GetServerDataDir());
	if ((pHashData = data.Load(root, BIT_SHIFT ))==0)
	{
		printf("load failure\n");
		return 0;
	}

	printf("+++++ Hash data+++++ \n");
	printf("total memory: %d bytes\n", pHashData->nTotalUsedMemory);
	printf("total data cnt : %d\n", pHashData->nTotalDataCnt);

	while(1)
	{
		suserinfo* info = (suserinfo* )yialloc(sizeof(suserinfo));

		char szFileName[256];
		
		memset(info, 0, sizeof(*info));
		printf("input name: ");
		scanf("%s", info->name);

		printf("name:%s>", info->name);

		if (strncmp(info->name, "quit", 4)==0)
		{
			break;
		}

		unsigned int n1 = yihf_encoded_additional(info->name, BIT_SHIFT );
		unsigned int n2 = yihf_additional(info->name, BIT_SHIFT );
		unsigned int n3 = yihf_general(info->name, BIT_SHIFT );
		Bit32 layer = __8to32__(n1, n2, n3, 0);

		cyiList* pList = data.GetList(layer);

		if (pList!=0)
		{
			Bool bIsDuplicated=False;
			for (cyiNode* pNode = pList->GetNodeTopFrom(0); pNode; pNode=pNode->np)
			{
				suserinfo* usr = (suserinfo*)pNode->ref;

				if (usr==0)
					continue;
				printf("usr->name:%s, info->name:%s\n", usr->name, info->name);
				if (strcmp(usr->name, info->name)==0)
				{
					printf("duplicate user....! %d(%x):%s\n", usr->id, usr->id, usr->name);
					bIsDuplicated=True;
					return 0;
					break;
				}
			}	
			data.ReleaseList(pList);
	
			if (bIsDuplicated==True)
			{
				continue;
			}
		}

		int nFileName;
		int cnt;
		if ((cnt = nFileName=data.AddRefData(layer, info))==-1)
		{
			assert(0);
		}
		addwrite(root, cnt, layer, info);

		printf("+++++ Hash data+++++ \n");
		printf("total memory: %d bytes\n", pHashData->nTotalUsedMemory);
		printf("total data cnt : %d\n", pHashData->nTotalDataCnt);
	}  

	data.Delete();
}
