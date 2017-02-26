#include "svryuser.h"
#include "yiList.h"
#include "sycTypes.h"
#include <string.h>

// --------------------------
int csvryuser::IsExistName(Bit32 id, const char* pszName)
{
	cyiList* pList = m_EmailHash.GetList(id);
	int _id = (id & 0x000000FF);

	if (pList==0)
	{
		return -1;
	}

	if (_id != 0)
	{
		cyiNode* pNode = pList->GetNodeTopFrom(_id-1);
	
		ssycUserBasicInfo* pInfo = (ssycUserBasicInfo* )pNode->ref;

		if (strncmp(pInfo->name, pszName, YI_MAX_NAME_LENGHT)!=0)
		{
			m_EmailHash.ReleaseList(pList);
			return 0;
		}

		m_EmailHash.ReleaseList(pList);
		return _id;
	}
	

	int cnt=0;
	for (cyiNode* pNode=pList->GetNodeTopFrom(0); pNode!=0; pNode=pNode->np)
	{
		ssycUserBasicInfo* pInfo = (ssycUserBasicInfo* )pNode->ref;
		cnt++;
		if (strncmp(pInfo->name, pszName, YI_MAX_NAME_LENGHT)==0)
		{
			m_EmailHash.ReleaseList(pList);
			return cnt;
		}
	}

	m_EmailHash.ReleaseList(pList);
	return 0;
}


// --------------------------
int csvryuser::IsExistMail(Bit32 id, const char* pszMail)
{
	cyiList* pList = m_EmailHash.GetList(id);
	int _id = (id & 0x0000FFFF);

	if (pList==0)
	{
		return -1;
	}

	if (_id!=0)
	{
		cyiNode* pNode = pList->GetNodeTopFrom(_id-1);
		if (memcmp(pNode->ref, pszMail, YI_MAX_EMAIL_LENGHT)!=0)
		{
			m_EmailHash.ReleaseList(pList);
			return 0;
		}

		m_EmailHash.ReleaseList(pList);
		return _id;
	}
	

	int cnt=0;
	for (cyiNode* pNode=pList->GetNodeTopFrom(0); pNode!=0; pNode=pNode->np)
	{
		cyiNode* pNode = pList->GetNodeTopFrom(_id-1);
		cnt++;
		if (memcmp(pNode->ref, pszMail, YI_MAX_EMAIL_LENGHT)==0)
		{
			m_EmailHash.ReleaseList(pList);
			return cnt;
		}
	}

	return 0;
}

