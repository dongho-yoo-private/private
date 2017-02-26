#include "yiHashTable.h"
#include "yiHashFunc.h"
#include "yistd.h"
#include "yiList.h"
#include <string.h>


// ------------------------------
typedef struct xsHashTable{
	union _table {
		struct xsHashTable** table;	
		cyiList* list;
	} table;
	int 	id;
	Bool 	bIsDataLayer;
	HandleMutexx hMutex;
	syiHashKeyValue** pExportedList;
} sHashTable;

#define m_pLayerEntryPoint ((sHashTable*)m_pLayerEntryPointx)

// ------------------------------
static sHashTable* xCreateHashTable(int layerNo, int maxlayer, int maxshift_cnt, int id, int* memory_size, fyiDeleteCallback callback)
{
	sHashTable* p = (sHashTable*)yialloci(sizeof(sHashTable));
	*memory_size=sizeof(sHashTable);

	if (layerNo==maxlayer)
	{
		p->bIsDataLayer=True;
		p->table.list = new cyiList(callback, False);
		//p->parent=parent;
		p->id = id;
		*memory_size+=sizeof(cyiList);
		return p;
	}

	int n = (sizeof(sHashTable*)<<maxshift_cnt);

	p->table.table = (sHashTable**) yialloci(n);
	//p->parent=parent;
	p->id = id;

	*memory_size+=n;
	return p;
}

// ------------------------------
static sHashTable* xGetHashTable(sHashTable* pTop, unsigned int key)
{
	sHashTable* tbl;
	unsigned int n1=(key&0xFF000000)>>24;
	unsigned int n2=(key&0x00FF0000)>>16;
	unsigned int n3=(key&0x0000FF00)>>8;

	if ((tbl=pTop->table.table[n1])==0)
	{
		return 0;
	}

	if (tbl->bIsDataLayer==True)
	{
		return tbl;
	}

	if ((tbl=tbl->table.table[n2])==0)
	{
		return 0;
	}

	if (tbl->bIsDataLayer==True)
	{
		return tbl;
	}

	if ((tbl=tbl->table.table[n3])==0)
	{
		return 0;
	}

	return tbl;
}

// ------------------------------
static int xFindKeyIndex(syiHashKeyValue** pExportedList, const char* pszString, void** value)
{
	int i=0;
	if (pExportedList==0)
		return 0;
	while(pExportedList[i])
	{
		if (strcmp(pExportedList[i]->pszKey, pszString)==0)
		{
			if (value)
			{
				*value=pExportedList[i]->value;
			}
			return i+1;
		}
		i++;
	}	
	return 0;
}

// ------------------------------
static int xFindKeyIndex(syiHashKeyValue** pExportedList, const char* pszString, int n, void** value)
{
	int i=0;

	if (pExportedList==0)
		return 0;

	if (n==-1)
	{
		return xFindKeyIndex(pExportedList, pszString, value);
	}

	while(pExportedList[i])
	{
		if (memcmp(pExportedList[i]->pszKey, pszString, n)==0)
		{
			if (value)
			{
				*value=pExportedList[i]->value;
			}
			return i+1;
		}
		i++;
	}
	return 0;
}

// ------------------------------
static syiHashKeyValue** xAddKeyAndValue(cyiList* list, int* index, const char* pszStringKey, const void* value, int n, yiDestroyer _df)
{
	syiHashKeyValue* p = struct_alloc(syiHashKeyValue);
	if (n!=-1)
	{

		p->pszKey=(char*)yitoheapi((void*)pszStringKey, n);
	}
	else
	{
		p->pszKey=yistrheap(pszStringKey);
	}

	p->value=(void*)value;
	p->deletef=_df;
	list->addref(p);
	*index=list->count();
	return (syiHashKeyValue**)list->ExportArray();
}

// ------------------------------
static void xOnDelete(void* _p)
{
	syiHashKeyValue*p = (syiHashKeyValue*)_p;

	if (p==0)
		return ;

	if (p->deletef)
	{
		p->deletef(p->value);
	}

	if (p->pszKey)
	{
		yifree(p->pszKey);
	}

	yifree(p);
}

// ------------------------------
static void xDeleteAll(sHashTable* tbl, int shift_bit)
{
	if (tbl->bIsDataLayer==True)
	{
		tbl->table.list->clear(xOnDelete);
		if (tbl->pExportedList)
		{
			tbl->table.list->DeleteArray((void**)tbl->pExportedList);
			if (tbl->hMutex)
			{
				yiMutexDestroyx(tbl->hMutex);
			}
		}
		delete tbl->table.list;
		return ;
	}

	if (tbl->table.table!=0)
	{
		int n = (1<<shift_bit);
		for (int i=0; i<n; i++)
		{
			if (tbl->table.table[i]==0)
				continue;
			xDeleteAll(tbl->table.table[i], shift_bit);
		}
	}

	yifree(tbl);
}


// ------------------------------
cyiHashTable::cyiHashTable(int nLayer, int nLayerBitCnt, int nKeyLength, yiDestroyer callback, Bool bUseMutex)
{
	m_nLayerBitCnt=nLayerBitCnt;
	m_nLayer=nLayer;
	m_nTotalUsedMemorySize=0;
	m_nKeyLength=nKeyLength;
	m_Destroyer=callback;
	m_nElementCount=0;
	m_pLayerEntryPointx = (sHashTable*)xCreateHashTable(0, m_nLayer, nLayerBitCnt, 0, &m_nTotalUsedMemorySize, callback);
	m_max_id_mask=~(0xFFFFFFFF<<((4-m_nLayer)<<3));

}

// ------------------------------
cyiHashTable::~cyiHashTable()
{
	xDeleteAll(m_pLayerEntryPoint, m_nLayerBitCnt);

}

// ------------------------------
unsigned int cyiHashTable::GetHashLayer(const char* pszString, int nLayer, int nLayerBitCnt)
{
	if (nLayer==1)
	{
		return (yihf_additional(pszString, nLayerBitCnt)<<24);
	}

	if (nLayer==2)
	{
		return ((yihf_additional(pszString, nLayerBitCnt)<<24) | (yihf_encoded_additional(pszString, nLayerBitCnt)<<16));
	}

	if (nLayer==3)
	{
		return ((yihf_additional(pszString, nLayerBitCnt)<<24) | (yihf_encoded_additional(pszString, nLayerBitCnt)<<16)|(yihf_general(pszString, nLayerBitCnt)<<8));
	}
}

// ------------------------------
void cyiHashTable::Remove(unsigned int key)
{
	sHashTable*tbl = xGetHashTable(m_pLayerEntryPoint, key);
	
	if (tbl==0)
	{	
		return ;
	}

	if (tbl->table.list==0)
	{
		return ;
	}

	yiMutexLx(tbl->hMutex);
	int n=(key&m_max_id_mask)-1;

	if (tbl->table.list)
	{
		tbl->table.list->remove(n, m_Destroyer);
		tbl->table.list->DeleteArray((void**)tbl->pExportedList);
		tbl->pExportedList=(syiHashKeyValue**)tbl->table.list->ExportArray();
	}
	yiMutexUnLx(tbl->hMutex);
	
}

// ------------------------------
unsigned int cyiHashTable::Add(const char* pszString, void* value)
{
	unsigned int layer = GetHashLayer(pszString, m_nLayer, m_nLayerBitCnt);
	return Add(layer, pszString, value);
}

// ------------------------------
unsigned int cyiHashTable::Add(unsigned int layer, const char* pszStringKey, void* value)
{
	sHashTable* tbl;
	int nLayerDepth=1;
	unsigned int n1=(layer&0xFF000000)>>24;
	unsigned int n2=(layer&0x00FF0000)>>16;
	unsigned int n3=(layer&0x0000FF00)>>8;

	tbl=m_pLayerEntryPoint->table.table[n1];
	if (tbl==0)
	{
		int n;
		tbl = m_pLayerEntryPoint->table.table[n1] = xCreateHashTable(nLayerDepth, m_nLayer, m_nLayerBitCnt, n1, &n, m_Destroyer);
		m_nTotalUsedMemorySize+=n;
	}

ADD_KEY_AND_VALUE:
	if (nLayerDepth==m_nLayer)
	{
		if (tbl->hMutex==0)
		{
			tbl->hMutex=yiMutexCreatex();
		}

		yiMutexLx(tbl->hMutex);
		unsigned int nCurrentCnt = tbl->table.list->count();

		if (nCurrentCnt >= m_max_id_mask)
		{
			unsigned int x = ~(0xFFFFFFFF<<(m_nLayer<<3));
			//yiTrace("nCurrentCnt:%d, capacity:%d, %s\n", nCurrentCnt, x);
			yiMutexUnLx(tbl->hMutex);
			return 0;
		}

		if (xFindKeyIndex(tbl->pExportedList, pszStringKey, m_nKeyLength, 0)!=0)
		{
			//yiTrace("name is duplicated %s\n", pszStringKey);
			yiMutexUnLx(tbl->hMutex);
			return 0;
		}
	
		syiHashKeyValue** prev= tbl->pExportedList;
		syiHashKeyValue** x;

		int n;
		if ((x=xAddKeyAndValue(tbl->table.list, &n, pszStringKey, value, m_nKeyLength, m_Destroyer))==0)
		{
			//yiTrace("xAddKeyAndValue() error, %s\n", pszStringKey);
			yiMutexUnLx(tbl->hMutex);
			return 0;
		}
		//tbl->table.destroyer=callback;
		tbl->pExportedList = x;
		if (prev!=0)
		{
			yifree(prev);
		}

		yiMutexUnLx(tbl->hMutex);
		m_nElementCount++;
		return (layer|n);
	}

	if (tbl->table.table[n2]==0)
	{
		int n;
		tbl->table.table[n2]=xCreateHashTable(2, m_nLayer, m_nLayerBitCnt, ((n1<<24)|(n2<<16)), &n, m_Destroyer);
		m_nTotalUsedMemorySize+=n;
	}
	tbl = tbl->table.table[n2];

	if (m_nLayer==2)
	{
		nLayerDepth=2;
		goto ADD_KEY_AND_VALUE;
	}

		
	if (tbl->table.table[n3]==0)
	{
		int n;
		tbl->table.table[n3]=xCreateHashTable(3, m_nLayer, m_nLayerBitCnt, ((n1<<24)|(n2<<16)|(n3<<8)), &n, m_Destroyer);
		m_nTotalUsedMemorySize+=n;
	}

	nLayerDepth=3;
	tbl = tbl->table.table[n3];

	goto ADD_KEY_AND_VALUE;
}

// ------------------------------
const void* cyiHashTable::GetValue(const char* pszString)
{
	unsigned int layer = GetHashLayer(pszString, m_nLayer, m_nLayerBitCnt);
	return GetValue(layer, pszString);
}

#define IsKeyZero(key) ((key&(~(0xFFFFFFFF<<((3-m_nLayer+1)<<3))))==0)

// ------------------------------
const void* cyiHashTable::GetValue(unsigned int key, const char* pszStringKey)
{
	int nKey=key;
	void* ret;
	YIDEBUG("cyiHashTable::GetValue() 2 called\n");
	if (IsKeyZero(key)==True)
	{
		sHashTable* tbl = xGetHashTable(m_pLayerEntryPoint, key);

		if (tbl==0)
		{	
			YIDEBUG("cyiHashTable::GetValue() xGetHashTable is null\n");
			return 0;
		}
		yiMutexLx(tbl->hMutex);

		int n = xFindKeyIndex(tbl->pExportedList, pszStringKey, m_nKeyLength, &ret);
		yiMutexUnLx(tbl->hMutex);

		if (n==0)
		{
			YIDEBUG("cyiHashTable::GetValue() xFindKeyIndex is null\n");
			return 0;
		}

		return ret;
	}

	return GetValue(nKey);	
}

// ------------------------------
const void* cyiHashTable::GetValue(unsigned int key, char** pszStringKey)
{
	sHashTable* tbl;
	int n1=(key&0xFF000000)>>24;
	int n2=(key&0x00FF0000)>>16;
	int n3=(key&0x0000FF00)>>8;
	int n4=(key&0x000000FF);
	unsigned int subId;

	YIDEBUG("cyiHashTable::GetValue() m_nLayer:%d\n", m_nLayer);
	if (m_nLayer==1)
	{
		subId=(key&0x00FFFFFF);
	}
	else if (m_nLayer==2)
	{
		subId=(key&0xFFFF);
	}
	else if (m_nLayer==3)
	{
		subId=key&0xFF;
	}
	else
	{
		return 0;
	}

	YIDEBUG("cyiHashTable::GetValue() subId==%x\n", subId);

	if (subId==0)
	{
		if (pszStringKey!=0)
		{
			*pszStringKey=0;
		}
		return 0;
	}

	tbl = xGetHashTable(m_pLayerEntryPoint, key);
	YIDEBUG("cyiHashTable::GetValue() xGetHashTable() tbl:%x, key:%x\n", tbl, key);

	if (tbl==0)
	{
		if (pszStringKey!=0)
		{
			*pszStringKey=0;
		}
		return 0;
	}

	yiMutexLx(tbl->hMutex);
	syiHashKeyValue* ret = tbl->pExportedList[subId-1];

	if (pszStringKey!=0)
	{
		*pszStringKey=ret->pszKey;
	}
	yiMutexUnLx(tbl->hMutex);

	return ret->value;
/*
	tbl=m_pLayerEntryPoint->table.table[n1];

	if (tbl==0)
	{
		return 0;
	}

	if (tbl->bIsDataLayer==True)
	{
GET_VALUE:
		if (n4==0)
		{
			if (pszStringKey!=0)
			{
				*pszStringKey=0;
			}
			return 0;
		}
		syiHashKeyValue* ret = tbl->pExportedList[n4-1];

		if (pszStringKey!=0)
		{
			*pszStringKey=ret->pszKey;
		}

		return ret->value;
	}

	if ((tbl=tbl->table.table[n2])==0);
	{
		if (pszStringKey!=0)
		{
			*pszStringKey=0;
		}
		return 0;
	}
	if (tbl->bIsDataLayer==True)
	{
		n4=(key&0x0000FFFF);
		goto GET_VALUE;
	}

	if ((tbl=tbl->table.table[n3])==0)
	{
		if (pszStringKey!=0)
		{
			*pszStringKey=0;
		}
		return 0;
	}
	n4=(key&0x000000FF);

	goto GET_VALUE;
*/
}

// ------------------------------
Bool cyiHashTable::SetValue(unsigned int id, void* p)
{
	unsigned int subId;
	if (m_nLayer==1)
	{
		subId=(id&0x00FFFFFF);
	}
	else if (m_nLayer==2)
	{
		subId=(id&0xFFFF);
	}
	else if (m_nLayer==3)
	{
		subId=id&0xFF;
	}
	else
	{
		return False;
	}

	sHashTable*tbl = xGetHashTable(m_pLayerEntryPoint, id);

	if (tbl==0)
		return False;
	
	yiMutexLx(tbl->hMutex);
	syiHashKeyValue* ret = tbl->pExportedList[subId-1];

	if (ret->value)
	{
		if (ret->deletef)
			ret->deletef(ret->value);
	}
	ret->value=p;
	yiMutexUnLx(tbl->hMutex);

	return True;
}

// ------------------------------
unsigned int cyiHashTable::GetKey(unsigned int layer, const char* pszString)
{
	sHashTable*tbl = xGetHashTable(m_pLayerEntryPoint, layer);

	if (tbl==0)
	{
		//printf("tbl is zero\n");
		return 0;
	}

	int index = xFindKeyIndex(tbl->pExportedList, pszString, m_nKeyLength, 0);
	return index==0?0:(layer|index);
}


// ------------------------------
unsigned int cyiHashTable::GetKey(const char* pszString)
{
	unsigned int layer=GetHashLayer(pszString, m_nLayer, m_nLayerBitCnt);
	return GetKey(layer, pszString);
}


// ------------------------------
unsigned int cyiHashTable::GetUsedMemorySize()
{
	return m_nTotalUsedMemorySize;
}

// ------------------------------
int cyiHashTable::GetCount()
{
	return m_nElementCount;
}

unsigned int cyiHashTable::GetIndexMask()
{
	return m_max_id_mask;
}


// ------------------------------
int yixSearchPath(const char* root, unsigned int key, int level, unsigned int max_layer, 
					int shift_bit, unsigned int index_mask, zxOnFoundPath f)
{
	int sum=0;
	int level_shift = 1<<((4-level)<<3);

//	YIDEBUG("yixSearchPath() %s, key:%d, level:%d, max_layer:%d, shift_bit:%d, index_mask:%d\n", root, key, level, max_layer, shift_bit, index_mask);
	if (level<max_layer)
	{
		int n = 1<<shift_bit;
		for (int i=0; i<n; i++)
		{
			Bool bIsDir=False;
			char path[256];
			sprintf(path, "%s/%03d",  root, i);

			if (yiIsExist(path, &bIsDir)==True)
			{
				YIDEBUG("yixSearchPath() found path %s\n", path);
				if (bIsDir==True)
				{
					//YIDEBUG("dir found %s\n", path);
					unsigned int xid = (key | (i<<level_shift));
					int res =yixSearchPath(path, xid, level+1, max_layer, shift_bit, index_mask, f);

					if (res==-1)
					{
						return -1;
					}
					sum+=res;
				}
				else
				{
					YIERROR("%s is not dir\n", path);
					return -1;
				}
			}
		}
		return sum;
	}

	for (int i=1; i<index_mask+1; i++)
	{
		Bool bIsDir=False;
		char path[256];
		sprintf(path, "%s/%d",  root, i);

		if (yiIsExist(path, &bIsDir)==True)
		{
			YIDEBUG("yixSearchPath() found path %s\n", path);
			if (bIsDir==True)
			{
				unsigned int xid = (key | i);
				f(path, xid);
				sum++;
				continue;
			}
			else
			{
				YIERROR("%s is not dir\n", path);
				return -1;
			}
		}
		break;	
	}

	return sum;
}

// ------------------------------
int yixSearchPath(const char* root, unsigned int key, const char* pszStr, int level, unsigned int max_layer, zxOnFoundPath2 f)
{
	int Level1 = ((key>>24)&0xFF);	
	int Level2 = 0;//((key>>16)&0xFF);
	int Level3 = 0;//((key>>8)&0xFF);
	int value=0;
	char path[256];

	if (max_layer==0)
		return 0;
	if (max_layer==1)
	{
		Level2=0;
		Level3=0;
		value = key&0x00FFFFFF;
		sprintf(path, "%s/%03d",  root, Level1);
	}
	else if (max_layer==2)
	{
		Level2=((key>>16)&0xFF);
		value = key&0x0000FFFF;
		sprintf(path, "%s/%03d/%03d",  root, Level1, Level2);
	}
	else
	{
		Level3=((key>>8)&0xFF);
		value = key&0x000000FF;
		sprintf(path, "%s/%03d/%03d",  root, Level1, Level2, Level3);
	}
	
	if (value==0)
	{
		Bool isDir=True;
		if (yiIsExist(path, &isDir)==True && 
			isDir==True)
		{
			return f(path, key, pszStr);	
		}
	}
	else
	{
		Bool isDir=True;
		char filepath[256];
		sprintf(filepath, "%s/%d", value);

		if (yiIsExist(path, &isDir)==True 
				&& isDir==True)
		{
			return key;
		}
	}
	
	return 0;
}

