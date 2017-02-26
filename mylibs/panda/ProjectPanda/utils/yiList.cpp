#include "yistd.h"
#include "yiList.h"
//#include "yitrace.h"

#include <assert.h>
#include <iostream>
using namespace std;

// ------------------------
cyiList::cyiList(Bool bUseMutex)
{
	m_pTail=0;
	m_pTop=0;	
	m_cnt=0;
	m_deletef=0;
	m_hMutex=0;
	m_nElementSize=0;

	if (bUseMutex==True)
	{
		m_hMutex = yiMutexCreateEx(3);
		assert(m_hMutex);
	}
}

// ------------------------
 cyiList::cyiList(fyiDeleteCallback callback, Bool bUseMutex)
{
	m_pTail=0;
	m_pTop=0;	
	m_cnt=0;
	m_deletef=callback;
	m_hMutex=0;
	m_nElementSize=0;
	m_nIdOffset=0;
	m_nIdSize=0;


	if (bUseMutex==True)
	{
		m_hMutex = yiMutexCreateEx(3);
		assert(m_hMutex);
	}

}

// -----------------------
cyiList::cyiList(cyiList* list)
{
	//cyiList::cyiList(list->m_hMutex?True:False);

	m_pTail=0;
	m_pTop=0;	
	m_cnt=0;
	m_deletef=0;
	m_hMutex=0;
	m_nElementSize=0;
	m_nIdOffset=0;
	m_nIdSize=0;

	if (list->m_hMutex!=0)
	{
		m_hMutex = yiMutexCreateEx(3);
		assert(m_hMutex);
	}

	for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
	{
		addref(node->ref);
	}

}


// ------------------------
cyiList::~cyiList()
{
	//yiTrace("cyiList::~cyiList() called\n");
	clear();
	if (m_hMutex!=0)
	{
		yiMutexDestroyEx(m_hMutex);
		m_hMutex=0;
	}
}

// -----------------------
 void cyiList::clear(fyiDeleteCallback callback)
{
	int n=m_cnt;
	for (int i=0; i<n; i++)
	{
		remove(0, callback);
	}

	m_pTail=0;
	m_pTop=0;	
	m_cnt=0;

	if (m_deletef!=0)
		m_deletef=callback;

	//if (m_hMutex!=0)
	//{
	//	yiMutexDestroyEx(m_hMutex);
	//	m_hMutex=0;
	//}

	//yiTrace("cyiList::clear() exit\n");
}

// -----------------------
void cyiList::SetIdOffset(int offset, int size)
{
	m_nIdOffset=offset;
	m_nIdSize=size;
}


// -----------------------
 Bool cyiList::add(yiAnyValue data)
{
	return __create_node__(m_cnt, (void*)data);
}

// -----------------------
 Bool cyiList::addref(void* data)
{
	return	__create_node__(m_cnt, data);
}

// -----------------------
 Bool cyiList::insert(int nTopFrom, yiAnyValue data)
{
	return	__create_node__(nTopFrom, (void*)data);
}

// -----------------------
Bool cyiList::insertref(int nTopFrom, void* data)
{
	return	__create_node__(nTopFrom, data);
}

// -----------------------
void cyiList::remove(int nTopFrom, fyiDeleteCallback f)
{
	cyiNode* p = __get__(nTopFrom);

	if (p==0)
		return ;

	remove(p, f);
}

// -----------------------
void* cyiList::removeEx(int nTopFrom)
{
	cyiNode* p = __get__(nTopFrom);
	void* ret=0;

	if (p==0)
		return 0;

	ret = p->ref;
	remove(p, 0);

	return ret;
}

// -----------------------
cyiNode* cyiList::removenode(cyiNode* node)
{
	return 0;
}

// -----------------------
Bool cyiList::insertnode(int nTopFrom, cyiNode* node)
{
	return True;
}


// -----------------------
void cyiList::remove(cyiNode* p, fyiDeleteCallback f)
{
	if (p==0)
	{
		return ;
	}

	cyiNode* prev = p->pp;
	cyiNode* next = p->np;

	if (prev==0)
	{
		m_pTop=next;
	}
	else
	{
		prev->np=next;
	}

	if (next==0)
	{
		m_pTail=prev;
	}
	else
	{
		next->pp = prev;
	}

	if (f) 
	{
		if (p->ref)
			f((void*)p->ref);
	}
	else if (m_deletef)
	{
		if (p->ref)
			m_deletef((void*)p->ref);
	}

	//yifree(p);
	delete p;
	m_cnt--;

	return ;
}

// -----------------------
 unsigned long cyiList::count()
{
	return m_cnt;
}

// -----------------------
cyiNode* cyiList::GetNodeTopFrom(int n)
{
	return __get__(n);
}

// -----------------------
void* cyiList::GetFromId(void* pId)
{
	if (m_pTop==0 || m_cnt==0)
		return 0;

	if (m_nIdSize==0)
		return 0;

	if (m_nIdSize==1)
	{
		cyiNode* p = m_pTop;
		ubit8_t id=*((ubit8_t*)pId);

		for (cyiNode*p = m_pTop; p; p=p->np)
		{
			ubit8_t* x = (ubit8_t* )p->ref;

			if (id==x[this->m_nIdOffset])
				return p->ref;
		}
		return 0;
	}

	if (m_nIdSize==2)
	{
		cyiNode* p = m_pTop;
		ubit16_t id=*((ubit16_t *)pId);

		for (cyiNode*p = m_pTop; p; p=p->np)
		{
			ubit8_t* x = (ubit8_t* )p->ref;

			if (id==*((ubit16_t*)(&x[this->m_nIdOffset])))
				return p->ref;
		}
		return 0;
	}

	if (m_nIdSize==4)
	{
		cyiNode* p = m_pTop;
		ubit32_t id=*((ubit32_t *)pId);

		for (cyiNode*p = m_pTop; p; p=p->np)
		{
			ubit8_t* x = (ubit8_t* )p->ref;

			if (id==*((ubit32_t*)(&x[this->m_nIdOffset])))
				return p->ref;
		}
		return 0;
	}


	cyiNode* p = m_pTop;
	for (cyiNode*p = m_pTop; p; p=p->np)
	{
		ubit8_t* x = (ubit8_t* )p->ref;

		if (memcmp(pId, &x[this->m_nIdOffset], m_nIdSize)==0)
			return p->ref;
	}

	return 0;
	//if (m_pTop==0 || m_cnt==0)
	//	return 0;

	//cyiNode* p = m_pTop;

	//for (cyiNode*p = m_pTop; p; p=p->np)
	//{
	//	unsigned int* xid = (unsigned int*)p->ref;

	//	if (*xid==id)
	//	{
	//		return p->ref;
	//	}
	//}

	return 0;
}

// -----------------------
void* cyiList::GetTopFromRef(int n)
{
	cyiNode* node=GetNodeTopFrom(n);

	return node?node->ref:0;
}


// -----------------------
 cyiNode* cyiList::GetNodeTailFrom(int n)
{
	return __getr__(n);
}

//
// private functions.
//
//
//
//
//

// -----------------------
 inline cyiNode* cyiList::__getr__(int n)
{
	cyiNode* p = m_pTail;

	if (p==0)
		return 0;

	for (int i=0; i<n; i++)
	{
		p=p->pp;

		if (p==0)
			return false;	
	}

	return p;
}

// -----------------------
 inline cyiNode* cyiList::__get__(int n)
{
	cyiNode* p = m_pTop;

	if (p==0)
		return 0;

	for (int i=0; i<n; i++)
	{
		p=p->np;

		if (p==0)
		{
			return 0;	
		}
	}

	return p;
}

// -----------------------
 Bool cyiList::__create_node__(int n, /*yiAnyValue data, */void* ref)
{
	cyiNode* p = /*(cyiNode*)yialloci(sizeof(cyiNode));// */new cyiNode();

	if (p==0)
		return false;

	if (m_pTop==0)
	{
		p->np=0;
		p->pp=0;
		m_pTop=p;
		m_pTail=p;
	}
	else if (n==m_cnt)
	{
		p->np=0;
		p->pp=m_pTail;
		m_pTail->np=p;
		m_pTail=p;
	}
	else if (n==0)
	{
		p->pp=0;
		p->np=m_pTop;
		m_pTop->pp = p;	
		m_pTop=p;
	}
	else
	{
		cyiNode* next = __get__(n); 
		cyiNode* prev; 

		if (next==0) 
			return false;

		prev = next->pp;

		p->pp = prev;
		p->np = next;
		next->pp = p;
		prev->np = p; 
	}

	p->ref = ref;
	m_cnt++;

	return true;
}

// -----------------------
void cyiList::destroyer(fyiDeleteCallback f)
{
	m_deletef=f;
}

// -----------------------
void cyiList::lock()
{
	//xTRACE("list lock %x", m_hMutex);
	//assert(m_hMutex);
	yiMutexLockEx(m_hMutex);
}

// -----------------------
void cyiList::unlock()
{
	//xTRACE("list unlock %x", m_hMutex);
	//assert(m_hMutex);
	yiMutexUnlockEx(m_hMutex);
}

// -----------------------
Bool cyiList::islocked()
{
	return yiMutexTryEx(m_hMutex)==False?True:False;
}

#include "yistd.h"
#include "yiencodex.h"

// -----------------------
void** cyiList::ExportArray()
{
    void** arrayref;
    int i=0;

    if (m_cnt==0)
    {
            return 0;
    }

    arrayref =  (void**)yialloc(sizeof(void*)*(m_cnt+1));

    for (cyiNode* p=m_pTop;p!=0; p=p->np, i++)
    {
            arrayref[i]=p->ref;
    }
    arrayref[i]=0;

    return arrayref;
}

// -----------------------
Binary cyiList::Export(const char* pszFileName, int nElementSize, void* header, int headerSize, unsigned int security, Bool bBackup)
{
	void* p = yialloc(m_cnt*nElementSize + headerSize+sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int));
	char* pIndex=(char*)p;
	int size=0;

	//pIndex+=sizeof(unsigned int);
	//pIndex+=((security&0xFFFF0000)!=0)?sizeof(unsigned int):0;
	//pIndex+=(security!=0)?sizeof(unsigned int):0;

	if (header)
	{
		memcpy(pIndex, header, headerSize);
		pIndex+=headerSize;
	}

	if (pszFileName!=0 &&
		security!=0)
	{
		*((unsigned int*)pIndex)=security;
		pIndex+=sizeof(unsigned int);
	}

	*((int*)pIndex)=nElementSize;
	pIndex+=sizeof(int);

    for (cyiNode* p=m_pTop;p!=0; p=p->np)
    {
		if (p->ref)
		{
			memcpy(pIndex, p->ref, nElementSize);
			pIndex+=nElementSize;
			size+=nElementSize;
		}
    }

	if (pszFileName==0)
	{
		*((unsigned int*)p)=size+headerSize+sizeof(unsigned int);
		return &((unsigned int*)p)[1];
	}

	if (security&0xFFFF0000)
	{
		//*((unsigned int*)pIndex)=security;
		char* pp = (char*)p;
		::yiencode(&pp[headerSize+sizeof(unsigned int)], size+sizeof(unsigned int), security);

		if (bBackup)
		{
			yiFileWriteOnceEx(pszFileName, &((unsigned int*)p)[0], 0, size+headerSize+sizeof(unsigned int)+sizeof(unsigned int), True);
		}
		else
		{
			yiFileWriteOnce(pszFileName, &((unsigned int*)p)[0], 0, size+headerSize+sizeof(unsigned int)+sizeof(unsigned int));
		}
		return 0;
	}

	if (bBackup)
	{
		yiFileWriteOnceEx(pszFileName, &((unsigned int*)p)[0], 0, size+headerSize+sizeof(unsigned int), True);
	}
	else
	{
		yiFileWriteOnce(pszFileName, &((unsigned int*)p)[0], 0, size+headerSize+sizeof(unsigned int));
	}

	return 0;
}

// -----------------------
Binary cyiList::Export(const char* pszFileName, fyiGetSizeCallback callback, void* header, int headerSize, unsigned int security, Bool bBackup)
{
	void* p = 0;//yialloc(m_cnt*nElementSize + headerSize);
	char* pIndex=(char*)p;
	int size=0;
	int index=0;

    for (cyiNode* px=m_pTop;px!=0; px=px->np)
    {
		if (px->ref)
		{
			size+=callback(index, px->ref);
			size+=sizeof(unsigned int);
		}
		index++;
	}

	p=yialloc(size+headerSize+sizeof(unsigned int)+sizeof(unsigned int));
	pIndex=(char*)p;

	if (pszFileName==0)
	{
		pIndex+=4;
	}

	//*((unsigned int*)pIndex)=;

	if (header)
	{
		memcpy(pIndex, header, headerSize);
		pIndex+=headerSize;
	}

	if (pszFileName!=0 &&
			security!=0)
	{
		*((unsigned int*)pIndex)=security;
		pIndex+=4;
	}

	*((unsigned int*)pIndex)=0;
	pIndex+=sizeof(unsigned int);
	

	index=0;
    for (cyiNode* px=m_pTop;px!=0; px=px->np)
    {
		if (px->ref)
		{
			int n = callback(index, px->ref);
			memcpy(pIndex, &n, sizeof(int));
			memcpy(&pIndex[4], px->ref, n);
			pIndex+=(n+sizeof(int));
		}
		index++;

    }

	if (pszFileName==0)
	{
		*((unsigned int*)p)=size+headerSize+sizeof(unsigned int);
		return &((unsigned int*)p)[1];
	}
	if (security&0xFFFF0000)
	{
		//((unsigned int*)p)[1]=security;
		::yiencode(&((unsigned char*)p)[headerSize+4], size+sizeof(unsigned int), security);

		if (bBackup)
		{
			yiFileWriteOnceEx(pszFileName, &((unsigned int*)p)[0], 0, size+headerSize+sizeof(unsigned int)+sizeof(unsigned int), True);
		}
		else
		{
			yiFileWriteOnce(pszFileName, &((unsigned int*)p)[0], 0, size+headerSize+sizeof(unsigned int)+sizeof(unsigned int));
		}
		return 0;
	}

	if (bBackup)
	{
		yiFileWriteOnceEx(pszFileName, &((unsigned int*)p)[0], 0, size+headerSize, True);
	}
	else
	{
		yiFileWriteOnce(pszFileName, &((unsigned int*)p)[0], 0, size+headerSize+sizeof(unsigned int));
	}

	return 0;
}

// -----------------------
int	cyiList::Import(const char* pszFileName, void* header, int headerSize, Bool bIsEncoded, Bool bBackup)
{
	int size=0;
	void* p = bBackup==False?yiFileLoad(pszFileName, &size):yiFileLoadEx(pszFileName, &size);
	
	if (p==0)
		return 0;

	int res = Import(p, size, header, headerSize, bIsEncoded, bBackup);

	yifree(p);

	return res;
}


// -----------------------
int	cyiList::Import(void* p, int n, void* header, int headerSize, Bool bIsEncoded, Bool bBackup)
{
	char* pIndex=(char*)p;
	int nElementSize;
	unsigned int security=0;

	if (n<headerSize)
		return 0;

	if (headerSize)
	{
		if (header)
			memcpy(header, pIndex, headerSize);
		pIndex+=headerSize;
		n-=headerSize;
	}

	if (bIsEncoded==True)
	{
		security = *((unsigned int*)pIndex);//[1];
		pIndex+=sizeof(unsigned int);
		n-=sizeof(unsigned int);
		::yidecode(pIndex, n, security);

		if (n==0)
			return 0;
	}

	m_nElementSize=nElementSize = *((int*)pIndex);
	pIndex+=sizeof(int);
	n-=sizeof(int);

	if (n==0 ||
			n<0)
		return 0;


	if (nElementSize)
	{
		while(n)
		{
			if (n<nElementSize)
				return 0;
			__create_node__(m_cnt, yitoheap(pIndex, nElementSize));
			n-=nElementSize;
			pIndex+=nElementSize;
		}
	}
	else
	{
		while(n)
		{
			int* size = (int*)pIndex;
			if (n<*size)
				return 0;
			__create_node__(m_cnt, yiBinary(&pIndex[sizeof(int)], *size, 0));
			n-=(*size+sizeof(int));
			pIndex+=(*size+sizeof(int));
		}
	}

	return m_cnt;
}


// -----------------------
void cyiList::DeleteArray(void** pArray)
{
    yifree(pArray);
}


// -----------------------
int cyiList::GetReferenceIndex(void* ref)
{
	int i=0;
    for (cyiNode* p=m_pTop;p!=0; p=p->np, i++)
    {
		if (p->ref==ref)
		{
			return i;
		}
    }

	return -1;
}

// -----------------------
void cyiList::SetElementSize(int size)
{
	m_nElementSize=size;
}

// -----------------------
int cyiList::GetElementSize()
{
	return m_nElementSize;
}
