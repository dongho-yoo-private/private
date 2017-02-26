#include "svryuser.h"
#include "yiHashTable.h"
#include "base/svryenv.h"


// --------------------------
static void xOnUserHashDelete(void* data)
{
	if (data)
	{
		syiHashKeyValue* keyAndValue=(syiHashKeyValue*)data;

		if (keyAndValue->pszKey)
			yifree(keyAndValue->pszKey);
		
		void** datas = (void**)keyAndValue->value;
		if (datas)
		{
			for (int i=0; i<USER_DATA_COUNT; i++)
			{
				if (datas[i])
					yifree(datas[i]);
			}
		}
		yifree(data);
	}
}

// --------------------------
static void xOnEmailHashDelete(void* data)
{
	yifree(data);
}

// --------------------------
void xOnLoadUserData(const char* pszPath, unsigned int id)
{
	//yiTrace("xOnLoadUserData.... Found path:%s\n", pszPath);
	/*char buffer[256];
	strcpy(buffer, pszPath);
	strcat(buffer,"/a");*/
	csvryuser::GetInstance()->OnLoadUserData(pszPath, id);
}

// --------------------------
Bool csvryuser::OnLoadUserData(const char* pszPath, unsigned int id, Bool bIsReLoad)
{
	int   n;
	char buffer[256];
	void** p = (void**)yialloci(sizeof(void*)*(USER_DATA_COUNT+2));
	sRegisterUserInfo* zero;
	sBaseUserInfo* one;
	//sATypeData* pATypeData = (sATypeData* )yiFileSafeLoad(pszPath, &n);
	//
	YIDEBUG("csvryuser::OnLoadUserData(%s)\n", pszPath);
	
	unsigned int sizes[USER_DATA_COUNT]={0, };
	for (int i=0; i<USER_DATA_COUNT; i++)
	{
		sprintf(buffer, "%s/%d", pszPath, i);
		//YIDEBUG("csvryuser::OnLoadUserData() load file %s\n", buffer);
		//sRegisterUserInfo* zero = (sRegisterUserInfo*)syiFileLoadEx(buffer, &n);
		//
		p[i]=yiFileLoadEx(buffer, &n);
		sizes[i]=n;

		//yiTraceDump(p[i], n, 32, buffer);
		

		if (p[i]==0)
		{
			sizes[i]=0;
			YIDEBUG("csvryuser::OnLoadUserData() load file %s %s\n", buffer, "NG");
		}
		else
		{
			YIDEBUG("csvryuser::OnLoadUserData() load file %s size:%d bytes\n", buffer, sizes[i]);
		}
	}

	zero=(sRegisterUserInfo*)p[0];
	if (zero==0)
	{
		YIERROR("csvryuser::OnLoadUserData(%s) Zero Data Load Error\n", buffer);
		return False;
	}

	// unused user.
	if (zero->id==0)
	{
		YIDEBUG("csvryuser::OnLoadUserData() zero->id is zero !\n");
		m_nUnUsedUser++;
		yiFileUnLoadEx(zero);
		yiFileUnLoadEx(one);
		return False;
	}

	zero=(sRegisterUserInfo*)p[0];
	one=(sBaseUserInfo*)p[1];

	for (int i=0; i<USER_DATA_COUNT; i++)
	{
		one->sizes[i]=sizes[i];
	}

	sprintf(buffer, "%s/wmq8", pszPath);
	p[USER_DATA_COUNT+1]=yiFileLoadEx(buffer, 0);

	if (p[USER_DATA_COUNT+1]==0)
	{
		YIDEBUG("csvryuser::OnLoadUserData() load file %s %s\n", buffer, "NG");
	}

	p[4] = (void*)yialloci(sizeof(sSimpleGoHeader)<<5);
	sprintf(buffer, "%s/go", pszPath);
	if (yiFileReadEx(buffer, -1, p[4], sizeof(sSimpleGoHeader)<<5, True)==False)
	{
		YIDEBUG("csvryuser::OnLoadUserData(%s) NGs\n");
		yifree(p[4]);
		p[4]=0;
	}
	else
	{
		one->sizes[4]=sizeof(sSimpleGoHeader)<<4;
	}

	// check login time.

	if (bIsReLoad==True)
	{
		if (m_UserHash->SetValue(id, p)==False)
		{
			YIERROR("SetUserData Failure!! %s:%x\n", zero->szUserName, zero->id);
		}
	}
	else
	{
		m_UserHash->Add(zero->id, zero->szUserName, p);
	}
	m_nLoadedUserCnt++;
	m_nUsedMemory+=(sizeof(*one)+sizeof(*zero));

/*	
	if ((userinfo->id&0xFF)!=1)
	{
		YIDEBUG("csvryuser::OnLoadUserData(%s) (%s, %x)A-Type Data success\n", pszPath, userinfo->szUserName, userinfo->id);
	}

	yiFileSafeUnload(pATypeData);
*/ 		
	YIDEBUG("csvryuser::OnLoadUserData(%s) (%s, %x) user data load success\n", pszPath, zero->szUserName, zero->id);
	return True;
}


// --------------------------
Bool csvryuser::LoadUserData()
{
	int n=0;
	m_UserHash = new cyiHashTable(3, 8, -1, xOnUserHashDelete, True);

	syiSecs sec = yiGetCurrentSec();
	
	if (m_UserHash==0)
	{
		return False;
	}

	YIDEBUG("csvryuser::LoadUserData() called \n");
	if ((n=yixSearchPath(m_szUserRoot/*csvryEnv::GetUserRoot()*/, 0, 0, 3, 8, m_UserHash->GetIndexMask(), xOnLoadUserData))<0)
	{
		return False;
	}
	syiSecs sec2 = yiGetCurrentSec();
	syiSecs sec3 = yiCompareSecs(sec, sec2);

	yiTrace("load %d.%06d secs.....\n", sec3.sec, sec3.usec);

	return True;
}

// --------------------------
void xOnLoadGroupData(const char* pszPath, unsigned int id)
{
	csvryuser::GetInstance()->OnLoadGroupData(pszPath, id);
}

// --------------------------
Bool csvryuser::OnLoadGroupData(const char* pszPath, unsigned int id)
{
	return True;
}

// --------------------------
Bool csvryuser::LoadGroupData()
{
	return True;
}

// --------------------------
void xOnLoadEmailAddress(const char* pszPath, unsigned int id)
{
	YIDEBUG("xOnLoadEmailAddress(%s, %x)\n", pszPath, id);
	csvryuser::GetInstance()->OnLoadEmailAddress(pszPath, id);
}

// --------------------------
Bool csvryuser::OnLoadEmailAddress(const char* pszPath, unsigned int id, Bool bIsReLoad)
{
	char buffer[256];

	sprintf(buffer, "%s/0", pszPath);

	sRegisterEmailInfo* zero = (sRegisterEmailInfo*)yiFileLoadEx(buffer, 0);

	if (zero==0)
	{
		YIERROR("csvryuser::OnLoadEmailAddress(%s) zero file load error\n", buffer);
		return False;
	}

	if (bIsReLoad==True)
	{
		if (m_EmailHash->SetValue(id, zero)==False)
		{
			YIERROR("csvryuser::OnLoadEmailAddress() SetValue Failure!! (%s:%x)\n", zero->szEmailAddress, zero->id);
		}

	}
	else
	{
		m_EmailHash->Add(zero->id, zero->szEmailAddress, zero);
	}
	//m_UserHash->Add(zero->id, zero->szEmailAddress, zero);

	m_nUsedMemory+=sizeof(sRegisterEmailInfo);
	m_nRegisteredEmailCount++;
	YIDEBUG("csvryuser::OnLoadEmailAddress(%s) success.....\n", zero->szEmailAddress);
	/*
	int   n;
	Bool isDir=False;
	if (yiIsExist(pszPath, &isDir)==True)
	{
		if (isDir==True)
		{
			return True;
		}
	}
	return False;

	sATypeData* pATypeData = (sATypeData* )yiFileSafeLoad(pszPath, &n);

	if (pATypeData==0)
	{
		YIERROR("csvryuser::OnLoadUserData(%s) A-Type Data Load Error\n", pszPath);
		return False;
	}
	
	syiLoginUserInfo* userinfo = (syiLoginUserInfo* )yialloci(sizeof(syiLoginUserInfo));

	memcpy(userinfo->szPassword, pATypeData->password, 8);
	memcpy(userinfo->szUserName, pATypeData->name, 16);
	userinfo->id = pATypeData->id;	

	m_UserHash->Add(userinfo->id, userinfo->szUserName, userinfo);
	
	if ((userinfo->id&0xFF)!=1)
	{
		YIDEBUG("csvryuser::OnLoadUserData(%s) (%s, %x)A-Type Data success\n", pszPath, userinfo->szUserName, userinfo->id);
	}

	yiFileSafeUnload(pATypeData);
*/		
	return True;
}

// --------------------------
Bool csvryuser::LoadEmailAddress()
{
	int n=0;
	m_EmailHash = new cyiHashTable(2, 8, -1, xOnEmailHashDelete, True);

	syiSecs sec = yiGetCurrentSec();
	
	if (m_EmailHash==0)
	{
		return False;
	}

	if ((n=yixSearchPath(csvryEnv::GetEmailRoot(), 0, 0, 2, 8, m_UserHash->GetIndexMask(), xOnLoadEmailAddress))<0)
	{
		return False;
	}
	syiSecs sec2 = yiGetCurrentSec();
	syiSecs sec3 = yiCompareSecs(sec, sec2);

	YIDEBUG("email list load %d.%06d secs.....\n", sec3.sec, sec3.usec);

	return True;
}

// --------------------------
Bool csvryuser::OnReqDelMsg32(int sock, HandleUserFile hFile, int index)
{
	DeleteMsg32(hFile, index);
	return True;
}

