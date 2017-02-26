//#include "StdAfx.h"
#include "yiResourceManager.h"


// ----------------------------------------
yiResourceManager* yiResourceManager::m_me = NULL;

// ----------------------------------------
yiResourceManager::yiResourceManager(void)
{
	::ZeroMemory(m_bIsObjectList, sizeof(m_bIsObjectList));
//	::ZeroMemory(m_list, sizeof(m_list));
	::ZeroMemory(m_range, sizeof(m_range));
	::ZeroMemory(m_array, sizeof(m_array));
}

// ----------------------------------------
yiResourceManager::~yiResourceManager(void)
{
	for (int i=0; i<eRT_MAX_RESOURCE_TYPE; i++)
	{
		if (m_array[i]!=NULL)
		{
			HeapFree(::GetProcessHeap(), HEAP_NO_SERIALIZE, m_array[i]);
		}
	}
}

// ----------------------------------------
yiResourceManager* yiResourceManager::GetInstance()
{
	if (m_me==0)
	{
		m_me = new yiResourceManager();
	}

	return m_me;
}

// ----------------------------------------
char* yiResourceManager::LoadString(HINSTANCE hResourceModule, UINT id)
{
	char buffer[256];
	int n = ::LoadString(hResourceModule, id, buffer, 256);
	char* str = (char*)HeapAlloc(::GetProcessHeap(), HEAP_NO_SERIALIZE|HEAP_ZERO_MEMORY, n+1);
	::memcpy(str, buffer, n);

	return str;
}


// ----------------------------------------
BOOL yiResourceManager::DefineResource(eResourceType type, eResourceSystem system, UINT nMinimumId, UINT nMaxCnt)
{
#ifdef _DEBUG
	if (type>=eRT_MAX_RESOURCE_TYPE)
	{
		return FALSE;
	}
#endif

	if (system==eRS_LIST)
	{
		return FALSE;
		//m_list[type] = new cyiList();
	}
	else
	{
		//m_array[type] = (void**)yialloci((sizeof(void*)*(nMaxCnt));
		m_array[type] = (void**)HeapAlloc(::GetProcessHeap(), HEAP_NO_SERIALIZE|HEAP_ZERO_MEMORY, sizeof(void*)*(nMaxCnt));
		m_range[type] = MAKELONG(nMinimumId, nMaxCnt);
	}

	return TRUE;
}

// ----------------------------------------
BOOL yiResourceManager::Add(eResourceType type, UINT resId, void* p)
{
#ifdef _DEBUG
	if (type>=eRT_MAX_RESOURCE_TYPE)
	{
		return FALSE;
	}
#endif

	if (m_array[type]!=0)
	{
		WORD min = LOWORD(m_range[type]);
		m_array[type][resId-min]=p;
		return TRUE;
	}

	return FALSE;
}

// ----------------------------------------
void* yiResourceManager::Get(eResourceType type, UINT resId)
{
#ifdef _DEBUG
	if (type>=eRT_MAX_RESOURCE_TYPE)
	{
		return FALSE;
	}
#endif
	if (m_array[type]!=0)
	{
		WORD min = LOWORD(m_range[type]);
		return m_array[type][resId-min];
	}

	return 0;
}

// ----------------------------------------
void* yiResourceManager::Set(eResourceType type, UINT resId, void* data)
{
#ifdef _DEBUG
	if (type>=eRT_MAX_RESOURCE_TYPE)
	{
		return FALSE;
	}
#endif
	if (m_array[type]!=0)
	{
		WORD min = HIWORD(m_range[type]);
		void* pRet = m_array[type][resId-min];
		m_array[type][resId-min]=data;
		return pRet;
	}

}

