#ifndef __yit_listex_h__
#define __yit_listex_h__

#include "yistd.h"
#include "yimutex.h"

#include <iostream>
#include <assert.h>

using namespace std;

#ifndef xtNODELOOP
#define xtNODELOOP(node, list) \
	for (cyitNode<T>* node=list->GetNodeTopFrom(); node; node=node->np)
#endif

// -------------------------------
template <class T> class cyitNode
{
public:
	cyitNode<T>* np;
	cyitNode<T>* pp;

	//void*	ref;
	T* ref;
};

// -----------------------------------------------
typedef void (*fyiDeleteCallbackx)(void* val);

// -------------------------------
template <class T> class cyitList
{
public:
	// -----------------------
	cyitList(Bool bIsCreateMutex=False);

	// -----------------------
	cyitList(fyiDeleteCallbackx callback, Bool bIsCreateMutex=False);

	// -----------------------
	~cyitList();

	// -----------------------
	Bool add(T* data);

	// -----------------------
	Bool insert(int nTopFrom, T* data);

	// -----------------------
	void remove(int nTopFrom, fyiDeleteCallback f=0);

	// -----------------------
	T* removeEx(int nTopFrom);

	// -----------------------
	cyitNode<T>* removenode(cyitNode<T>* node);

	// -----------------------
	Bool insertnode(int nTopFrom, cyitNode<T>* node);

	// -----------------------
	void remove(cyitNode<T>* node, fyiDeleteCallbackx f=0);

	// -----------------------
	void clear(fyiDeleteCallbackx f=0);

	// -----------------------
	unsigned long count();

	// -----------------------
	cyitNode<T>* GetNodeTopFrom(int n=0);

	// -----------------------
	T* GetTopFrom(int n=0);

	// -----------------------
	cyitNode<T>* GetNodeTailFrom(int n);

	// -----------------------
	T* REF(cyitNode<T>* node);

	// -----------------------
	void destroyer(fyiDeleteCallbackx callback);

	// -----------------------
	void lock();

	// -----------------------
	Bool islocked();
	
	// -----------------------
	void unlock();

	// -----------------------
	T** ExportArray();

	// -----------------------
	void DeleteArray(T** pArray);

protected:
	unsigned long m_cnt;
	cyitNode<T>* m_pTop;
	cyitNode<T>* m_pTail;
	fyiDeleteCallback m_deletef;
	Handle m_hMutex;

private: // member functions.
	// -----------------------------
	inline cyitNode<T>* __get__(int n);

	// -----------------------
	inline cyitNode<T>* __getr__(int n);
	
	// -----------------------------
	Bool __create_node__(int n, T* data);

};


// ------------------------
template<class T> cyitList<T>::cyitList(Bool bUseMutex)
{
	m_pTail=0;
	m_pTop=0;	
	m_cnt=0;
	m_deletef=0;
	m_hMutex=0;

	if (bUseMutex==True)
	{
		m_hMutex = yiMutexCreateEx(3);
		assert(m_hMutex);
	}
}

// ------------------------
template<class T> cyitList<T>::cyitList(fyiDeleteCallbackx callback, Bool bUseMutex)
{
	m_pTail=0;
	m_pTop=0;	
	m_cnt=0;
	m_deletef=callback;
	m_hMutex=0;

	if (bUseMutex==True)
	{
		m_hMutex = yiMutexCreateEx(3);
		assert(m_hMutex);
	}
}

// ------------------------
template<class T> cyitList<T>::~cyitList()
{
	//yiTrace("cyitList<T>::~cyitList() called\n");
	clear();
	if (m_hMutex!=0)
	{
		yiMutexDestroyEx(m_hMutex);
		m_hMutex=0;
	}
}

// -----------------------
 template<class T> void cyitList<T>::clear(fyiDeleteCallbackx callback)
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

	//yiTrace("cyitList<T>::clear() exit\n");
}

// -----------------------
 template<class T> Bool cyitList<T>::add(T* data)
{
	return __create_node__(m_cnt, data);
}


// -----------------------
 template<class T> Bool cyitList<T>::insert(int nTopFrom, T* data)
{
	return	__create_node__(nTopFrom, data);
}


// -----------------------
template<class T> void cyitList<T>::remove(int nTopFrom, fyiDeleteCallbackx f)
{
	cyitNode<T>* p = __get__(nTopFrom);

	if (p==0)
		return ;

	remove(p, f);
}

// -----------------------
template<class T> T* cyitList<T>::removeEx(int nTopFrom)
{
	cyitNode<T>* p = __get__(nTopFrom);
	T* ret=0;

	if (p==0)
		return 0;

	ret = p->ref;
	remove(p, 0);

	return ret;
}

// -----------------------
template<class T> cyitNode<T>* cyitList<T>::removenode(cyitNode<T>* node)
{
	return 0;
}

// -----------------------
template<class T> Bool cyitList<T>::insertnode(int nTopFrom, cyitNode<T>* node)
{
	return True;
}


// -----------------------
template<class T> void cyitList<T>::remove(cyitNode<T>* p, fyiDeleteCallbackx f)
{
	if (p==0)
	{
		return ;
	}

	cyitNode<T>* prev = p->pp;
	cyitNode<T>* next = p->np;

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
 template<class T> unsigned long cyitList<T>::count()
{
	return m_cnt;
}

// -----------------------
 template<class T> cyitNode<T>* cyitList<T>::GetNodeTopFrom(int n)
{
	return __get__(n);
}

// -----------------------
template<class T> T* cyitList<T>::GetTopFrom(int n)
{
	cyitNode<T>* p;
	if ((p=__get__(n))!=0)
	{
		return p->ref;
	}
	return 0;
}


// -----------------------
 template<class T> cyitNode<T>* cyitList<T>::GetNodeTailFrom(int n)
{
	return __getr__(n);
}

// -----------------------
template<class T> 	T* cyitList<T>::REF(cyitNode<T>* node)
{
	return (T*)node->ref;
}

//
// private functions.
//
//
//
//
//

// -----------------------
 template<class T> inline cyitNode<T>* cyitList<T>::__getr__(int n)
{
	cyitNode<T>* p = m_pTail;

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
template<class T>  inline cyitNode<T>* cyitList<T>::__get__(int n)
{
	cyitNode<T>* p = m_pTop;

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
 template<class T> Bool cyitList<T>::__create_node__(int n, T* data)
{
	cyitNode<T>* p = new cyitNode<T>();

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
		cyitNode<T>* next = __get__(n); 
		cyitNode<T>* prev; 

		if (next==0) 
			return false;

		prev = next->pp;

		p->pp = prev;
		p->np = next;
		next->pp = p;
		prev->np = p; 
	}

	p->ref = data;
	m_cnt++;

	return true;
}

// -----------------------
template<class T> void cyitList<T>::destroyer(fyiDeleteCallbackx f)
{
	m_deletef=f;
}

// -----------------------
template<class T> void cyitList<T>::lock()
{
	//xTRACE("list lock %x", m_hMutex);

	if (m_hMutex)
	{
		yiMutexLockEx(m_hMutex);
	}
}

// -----------------------
template<class T> void cyitList<T>::unlock()
{
	//xTRACE("list unlock %x", m_hMutex);
	//assert(m_hMutex);
	if (m_hMutex)
	{
		yiMutexUnlockEx(m_hMutex);
	}
}

// -----------------------
template<class T> Bool cyitList<T>::islocked()
{
	return yiMutexTryEx(m_hMutex)==False?True:False;
}

#include "yistd.h"

// -----------------------
template<class T> T** cyitList<T>::ExportArray()
{
    T** arrayref;
    int i=0;

    if (m_cnt==0)
    {
            return 0;
    }

    arrayref =  (T**)yialloc(sizeof(T*)*(m_cnt+1));

    for (cyitNode<T>* p=m_pTop;p!=0; p=p->np, i++)
    {
            arrayref[i]=p->ref;
    }
    arrayref[i]=0;

    return arrayref;
}

// -----------------------
template<class T> void cyitList<T>::DeleteArray(T** pArray)
{
    yifree(pArray);
}


#endif
