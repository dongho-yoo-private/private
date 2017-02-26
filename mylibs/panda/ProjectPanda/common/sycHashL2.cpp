#include "sycHashL2.h"
#include "yistd.h"
#include "yifile.h"
#include "yitrace.h"

#include <assert.h>
#include <string.h>

// ---------------------------------
typedef struct xssycHashDefFile
{
	int	n;
	int	n2;
}ssycHashDefFile;

// ---------------------------------
ssycHashL2* csycHashL2::Load(const char* root_path, int array_shift)
{

	Bool bIsDir=True;
	if (yiIsExist(root_path, &bIsDir)==False)
	{
		return 0;	
	}

	if (bIsDir==False)
	{
		return 0;
	}

	m_hMutex=yiMutexCreate(0);

	int nAllocSize = sizeof(ssycHashL2);
	ssycHashL2* p = (ssycHashL2*)yialloci(sizeof(ssycHashL2));
	assert(p);


	p->nTotalUsedMemory = nAllocSize;
	strcpy(p->szRootPath, root_path);
	p->nArrayCnt=1<<array_shift;
	p->nLayerCnt=2;

	nAllocSize = sizeof(cyiList*)<<array_shift;
	p->pHashTableEntryPoint = (cyiList***)yialloci(nAllocSize);

	int len = 1<<array_shift;
	for (int i=0; i<len; i++)
	{
		char path[256];
		sprintf(path, "%s/%03d", root_path, i);
		Bool bIsDir = True;
		if (yiIsExist(path, &bIsDir)==False)
		{
			p->pHashTableEntryPoint[i]=0;	
			p->nFreeLayerCnt+=len;
			continue;
		}
	
		if (bIsDir==False)
		{
			assert(0);
		}

		nAllocSize = sizeof(cyiList*)<<array_shift;
		p->pHashTableEntryPoint[i] = (cyiList**)yialloci(nAllocSize);
		p->nTotalUsedMemory+=nAllocSize;

		for (int j=0; j<len; j++)
		{
			char pathL2[256];
			sprintf(pathL2, "%s/%03d", path, j);
			if (yiIsExist(pathL2, &bIsDir)==False)
			{
				p->pHashTableEntryPoint[i][j]=0;	
				p->nFreeLayerCnt++;
				continue;
			}
			if (bIsDir==False)
			{
				assert(0);
			}

			char filename[256];
			sprintf(filename, "%s/defs", pathL2);

			ssycHashDefFile def;
			if (yiFileCreditRead(filename, &def)!=eFE_OK)
			{
				SYSLOG(, "%s > read failure\n", filename);
				Delete();
				return 0;
			}

			if (def.n!=def.n2)
			{
				SYSLOG(, "%s > invalid format (%d!=%d)\n", filename, def.n, def.n2);
				Delete();
				return 0;
			}

			if (def.n==0)
			{
				p->pHashTableEntryPoint[i][j]=0;	
				p->nFreeLayerCnt++;
				continue;
			}

			cyiList* pList = new cyiList(yifree, True);	
			assert(pList);
			p->pHashTableEntryPoint[i][j]=pList;
			p->nTotalUsedMemory+=sizeof(cyiList);
			p->nTotalDataCnt++;

			for (int k=0; k<def.n; k++)
			{
				int size;
				sprintf(filename, "%s/%d", pathL2, k+1);
				void* pData = yiFileCreditLoad(filename, &size);

				if (pData==0)
				{
					yiTrace("load failure (%s)\n", filename);
				}

				p->nTotalUsedMemory+=size;
				p->nTotalDataCnt++;
				pList->addref(pData);
			}	
			
		}

	}	

	return m_p = p;
}


// ---------------------------------
void csycHashL2::Delete()
{
	ssycHashL2* p = m_p;

	if (p->pHashTableEntryPoint==0)
	{
		if (m_hMutex)
			yiMutexDestroy(m_hMutex);	
		if (m_p)
			yifree(m_p);
		return ;
	}

	if (!m_p)
	{
		if (m_hMutex)
			yiMutexDestroy(m_hMutex);	
		return ;
	}
	
	for (int i=0; i<p->nArrayCnt; i++)
	{
		if (p->pHashTableEntryPoint[i]==0)
		{
			continue;
		}

		for (int j=0; j<p->nArrayCnt; j++)
		{
			if (p->pHashTableEntryPoint[i][j]!=0)
			{
				p->pHashTableEntryPoint[i][j]->clear(yifree);
				delete p->pHashTableEntryPoint[i][j];
			}
		}

		yifree(p->pHashTableEntryPoint[i]);
	}

	if (m_hMutex)
		yiMutexDestroy(m_hMutex);	
	yifree(m_p);

	m_p=0;
	m_hMutex=0;
}

// ---------------------------------
int csycHashL2::AddRefData(Bit32 layer, void* pRef)
{
	char layer1Path[256];
	char layer2Path[256];

	int layer1 = __32to8__(layer, 3);
	int layer2 = __32to8__(layer, 2);

	yiMutexLock(m_hMutex);

	ssycHashL2*p = m_p;

	yiTrace("csycHashL2::AddRefData() %d,%d\n", layer1, layer2);
	if (p->pHashTableEntryPoint[layer1]==0)
	{
		sprintf(layer1Path, "%s/%03d", p->szRootPath, layer1);
		Bool bIsDir=True;
		if (yiIsExist(layer1Path, &bIsDir)==False)
		{
			yiMakeDir(layer1Path);
		}

		if (bIsDir==False)
		{
			assert(0);
		}

		int nAllocSize = sizeof(cyiList**)*p->nArrayCnt;
		p->pHashTableEntryPoint[layer1] = (cyiList**)yialloci(nAllocSize);
		assert(p->pHashTableEntryPoint[layer1]); 
		p->nTotalUsedMemory+=nAllocSize ;
	}

	cyiList* pList = p->pHashTableEntryPoint[layer1][layer2]; 
	if (pList==0)
	{
		sprintf(layer2Path, "%s/%03d", layer1Path, layer2);
		Bool bIsDir=True;
		if (yiIsExist(layer2Path, &bIsDir)==False)
		{
			yiMakeDir(layer2Path);
		}

		if (bIsDir==False)
		{
			assert(0);
		}

		int nAllocSize = sizeof(cyiList*)*p->nArrayCnt;
		p->pHashTableEntryPoint[layer1][layer2] = (cyiList*)yialloci(nAllocSize);
		p->nTotalUsedMemory+=nAllocSize;

		yiTrace("cyiList new\n");
		pList = new cyiList(yifree, True);
		assert(pList);
		yiTrace("new:%x\n", pList);
		p->nTotalDataCnt++;
		p->pHashTableEntryPoint[layer1][layer2]=pList; 	
	}

	yiTrace("cyiList lock %x\n", pList);
	pList->lock();
	pList->addref(pRef);


	char filename[256];
	sprintf(filename, "%s/defs", layer2Path);
	ssycHashDefFile def;

	def.n = pList->count();
	def.n2 = def.n;

	pList->unlock();

	yiFileCreditWrite(filename, &def, sizeof(def), 1);

	yiMutexUnlock(m_hMutex);
	return pList->count();
	
}

// ---------------------------------
cyiList* csycHashL2::GetList(Bit32 layer)
{
	ssycHashL2* p = m_p;
	int layer1 = __32to8__(layer, 3);
	int layer2 = __32to8__(layer, 2);

	if (p->pHashTableEntryPoint[layer1]==0)
	{
		return 0;
	}
	
	if (p->pHashTableEntryPoint[layer1][layer2]==0)
	{
		return 0;
	}

	cyiList* pList = p->pHashTableEntryPoint[layer1][layer2];
	pList->lock();
	return pList;
}

// ---------------------------------
void csycHashL2::ReleaseList(cyiList* list)
{
	if (list)
	{
		list->unlock();
	}
}

// ---------------------------------
const void* csycHashL2::GetData(Bit32 layer, int index)
{
	cyiList* p = GetList(layer);

	cyiNode* node = p->GetNodeTopFrom(index);

	ReleaseList(p);

	return node->ref;
}


