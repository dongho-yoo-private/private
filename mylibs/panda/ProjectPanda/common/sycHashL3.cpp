#include "sycHashL3.h"
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
ssycHashL3* csycHashL3::Load(const char* root_path, int array_shift, ...)
{
	Bool bIsDir=True;
	void** pargv = ((void**)&array_shift);

	if (pargv[0]!=0)
	{
		m_ClassCallBack = (class_callback)pargv[1];
		m_pParentObject = (void*)pargv[0];
	}
	else
	{
		m_ClassCallBack=0;
		m_pParentObject=0;
	}

	if (yiIsExist(root_path, &bIsDir)==False)
	{
		return 0;	
	}

	if (bIsDir==False)
	{
		return 0;
	}

	m_hMutex=yiMutexCreate(0);

	int nAllocSize = sizeof(ssycHashL3);
	ssycHashL3* p = (ssycHashL3*)yialloci(sizeof(ssycHashL3));
	assert(p);


	p->nTotalUsedMemory = nAllocSize;
	strcpy(p->szRootPath, root_path);
	p->nArrayCnt=1<<array_shift;
	p->nLayerCnt=2;

	nAllocSize = sizeof(cyiList*)<<array_shift;
	p->pHashTableEntryPoint = (cyiList****)yialloci(nAllocSize);

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
		p->pHashTableEntryPoint[i] = (cyiList***)yialloci(nAllocSize);
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
			
			p->pHashTableEntryPoint[i][j] = (cyiList**)yialloci(nAllocSize);
			p->nTotalUsedMemory+=nAllocSize;

			for (int k=0; k<len; k++)
			{
				char pathL3[256];
				sprintf(pathL3, "%s/%03d", pathL2, k);
				if (yiIsExist(pathL3, &bIsDir)==False)
				{
					p->pHashTableEntryPoint[i][j][k]=0;	
					p->nFreeLayerCnt++;
					continue;
				}
				if (bIsDir==False)
				{
					assert(0);
				}
		
				// 削除?	
				//p->pHashTableEntryPoint[i][j][k] = (cyiList*)yialloci(nAllocSize);
				//p->nTotalUsedMemory+=nAllocSize;

				char filename[256];
				sprintf(filename, "%s/defs", pathL3);

				ssycHashDefFile def;
				if (yiFileCreditRead(filename, &def)!=eFE_OK)
				{
					SYSLOG(, "%s> read failure\n", filename);
					Delete();
					return 0;
				}

				if (def.n!=def.n2)
				{
					SYSLOG(, "%s> invalid format\n", filename);
					Delete();
					return 0;
				}

				if (def.n==0)
				{
					p->pHashTableEntryPoint[i][j][k]=0;	
					p->nFreeLayerCnt++;
					continue;
				}

				cyiList* pList = new cyiList(yifree, True);	
				assert(pList);
				p->pHashTableEntryPoint[i][j][k]=pList;
				p->nTotalUsedMemory+=sizeof(cyiList);
				p->nTotalDataCnt++;

				for (int h=0; h<def.n; h++)
				{
					int size;
					sprintf(filename, "%s/%d", pathL3, h+1);
					void* pData = yiFileCreditLoad(filename, &size);

					if (pData==0)
					{
						yiTrace("load failure (%s)\n", filename);
					}

					p->nTotalUsedMemory+=size;
					p->nTotalDataCnt++;
					pList->addref(pData);

					//if (m_ClassCallBack!=0)
					//	m_ClassCallBack(m_pParentObject, __8to32__(i, j, k, h), pData);
				}// for h
			}// for k
		} // for j

	}// for i

	return m_p = p;
}


// ---------------------------------
void csycHashL3::Delete()
{
	ssycHashL3* p = m_p;

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
			if (p->pHashTableEntryPoint[i][j]==0)
			{
				continue;
			}
			for (int k=0; k<p->nArrayCnt; k++)
			{
				if (p->pHashTableEntryPoint[i][j][k]!=0)
				{
					p->pHashTableEntryPoint[i][j][k]->clear(yifree);
					delete p->pHashTableEntryPoint[i][j][k];
				}
			}

			yifree(p->pHashTableEntryPoint[i][j]);
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
int csycHashL3::AddRefData(Bit32 layer, void* pRef)
{
	char layer1Path[256];
	char layer2Path[256];
	char layer3Path[256];

	int layer1 = __32to8__(layer, 3);
	int layer2 = __32to8__(layer, 2);
	int layer3 = __32to8__(layer, 1);

	yiMutexLock(m_hMutex);

	ssycHashL3*p = m_p;

	yiTrace("csycHashL3::AddRefData() %d,%d,%d\n", layer1, layer2, layer3);
	sprintf(layer1Path, "%s/%03d", p->szRootPath, layer1);
	if (p->pHashTableEntryPoint[layer1]==0)
	{
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
		p->pHashTableEntryPoint[layer1] = (cyiList***)yialloci(nAllocSize);
		assert(p->pHashTableEntryPoint[layer1]); 
		p->nTotalUsedMemory+=nAllocSize ;
	}

	sprintf(layer2Path, "%s/%03d", layer1Path, layer2);
	if (p->pHashTableEntryPoint[layer1][layer2]==0)
	{
		Bool bIsDir=True;
		if (yiIsExist(layer2Path, &bIsDir)==False)
		{
			yiMakeDir(layer2Path);
		}

		if (bIsDir==False)
		{
			assert(0);
		}

		int nAllocSize = sizeof(cyiList**)*p->nArrayCnt;
		p->pHashTableEntryPoint[layer1][layer2] = (cyiList**)yialloci(nAllocSize);
		assert(p->pHashTableEntryPoint[layer1][layer2]); 
		p->nTotalUsedMemory+=nAllocSize ;
	}

	cyiList* pList = p->pHashTableEntryPoint[layer1][layer2][layer3]; 
	sprintf(layer3Path, "%s/%03d", layer2Path, layer3);
	if (pList==0)
	{
		Bool bIsDir=True;
		if (yiIsExist(layer3Path, &bIsDir)==False)
		{
			yiMakeDir(layer3Path);
		}

		if (bIsDir==False)
		{
			assert(0);
		}

		pList = new cyiList(yifree, True);
		assert(pList);
		p->nTotalDataCnt++;
		p->pHashTableEntryPoint[layer1][layer2][layer3]=pList; 	
	}
	yiMutexUnlock(m_hMutex);

	yiTrace("cyiList lock %x\n", pList);
	pList->lock();
	pList->addref(pRef);

	char filename[256];
	sprintf(filename, "%s/defs", layer3Path);
	ssycHashDefFile def;

	def.n = pList->count();
	def.n2 = def.n;

	pList->unlock();


	yiFileCreditWrite(filename, &def, sizeof(def), 1);
	//if (m_ClassCallBack!=0)
	//	m_ClassCallBack(m_pParentObject, __8to32__(layer1, layer2, layer3, def.n), pRef);

	// ??
	return pList->count();
}

// ---------------------------------
cyiList* csycHashL3::GetList(Bit32 layer)
{
	int layer1 = __32to8__(layer, 3);
	int layer2 = __32to8__(layer, 2);
	int layer3 = __32to8__(layer, 1);
	ssycHashL3* p = m_p;

	if (p->pHashTableEntryPoint[layer1]==0)
	{
		return 0;
	}
	
	if (p->pHashTableEntryPoint[layer1][layer2]==0)
	{
		return 0;
	}

	if (p->pHashTableEntryPoint[layer1][layer2][layer3]==0)
	{
		return 0;
	}

	cyiList* pList = p->pHashTableEntryPoint[layer1][layer2][layer3];
	pList->lock();
	return pList;
}

// ---------------------------------
void csycHashL3::ReleaseList(cyiList* list)
{
	if (list)
	{
		list->unlock();
	}
}

// ---------------------------------
const void* csycHashL3::GetData(Bit32 layer, int _index)
{
	int index = (_index==0) ? (layer&0xFF):_index;
	cyiList* p = GetList(layer);

	cyiNode* node = p->GetNodeTopFrom(index);

	ReleaseList(p);

	return node->ref;
}


