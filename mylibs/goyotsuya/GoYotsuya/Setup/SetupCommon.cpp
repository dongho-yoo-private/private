
#include "yi.h"
#include "yistd.h"

char g_szGroupName[32]={0,};


// --------------------------------
HKEY xGetRootKey()
{
	char szKey[128];
	char szDisplayNameEn[128];
	HKEY hKeyYotsuya=0;
	
	wsprintf(szKey, "%s\\%s", "SOFTWARE", g_szGroupName);

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_ALL_ACCESS, &hKeyYotsuya)!=ERROR_SUCCESS)
	{
		return 0;
	}

	return hKeyYotsuya;
}

// --------------------------------
//Bool xIsProcessExecute(char* szSoftwareName)
//{
//	cyiList* list = xGetSoftwareList();
//
//	if (list==0)
//	{
//		return False;
//	}
//
//	for (cyiNode* node = list->GetNodeTopFrom(); node; node=node->np)
//	{
//		const char* p = (const char*)node->ref;
//		cyiList* list2 = ::xGetSoftwareFileList(p);
//
//		for (cyiNode* node=list2->GetNodeTopFrom(); node; node=node->np)
//		{
//			::EnumProcesses(
//		}
//	}
//}


// --------------------------------
HKEY xGetSoftwareKey(const char* pszSoftware)
{
	char szKey[128];
	char szDisplayNameEn[128];
	HKEY hKeyYotsuya=0;
	
	wsprintf(szKey, "%s\\%s\\%s", "SOFTWARE", g_szGroupName, pszSoftware);

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_ALL_ACCESS, &hKeyYotsuya)!=ERROR_SUCCESS)
	{
		return 0;
	}

	return hKeyYotsuya;
}

// --------------------------------
cyiList* xGetSoftwareList()
{
	HKEY hKeyYotsuya=xGetRootKey();
	cyiList* list = new cyiList();
	int cnt=0;

	while(1)
	{
		char key[64];
		DWORD dwKey=64;
		
		if (RegEnumKeyEx(hKeyYotsuya, cnt, key, &dwKey, 0, 0, 0, 0)!=ERROR_SUCCESS)
			break;

		xTRACE("xGetSoftwareList.... %s", key);
		list->addref(yistrheap(key));
		cnt++;
	}

	RegCloseKey(hKeyYotsuya);

	if (list->count()==0)
	{
		delete list;

		return 0;
	}

	return list;
}

// --------------------------------
Bool xIsUninstall(const char* pszFileName)
{
	const char* name = ::yiPathGetFileName(pszFileName);

	if (stricmp(name, "Uninstall.exe")==0)
	{
		return True;
	}

	return False;
}

// --------------------------------
const char* xEnumSoftware(int index)
{
	static char key[64];
	HKEY hKeyYotsuya=xGetRootKey();
	int cnt=0;
	DWORD dwKey=64;
	
RETRY:
	if (RegEnumKeyEx(hKeyYotsuya, index, key, &dwKey, 0, 0, 0, 0)!=ERROR_SUCCESS)
	{
		RegCloseKey(hKeyYotsuya);
		return 0;
	}

	if (stricmp(key, "Common")==0)
	{
		index++;
		goto RETRY;
	}

	RegCloseKey(hKeyYotsuya);


	return (const char*)key;
}


// --------------------------------
cyiList* xGetSoftwareShortcutList(const char* pszSoftware, cyiList* _list)
{
	HKEY hKeyYotsuya=xGetSoftwareKey(pszSoftware);
	cyiList* list = _list==0? new cyiList():_list;
	int cnt=0;

	while(1)
	{
		char szValue[32];
		DWORD dwData=256;
		char szData[256];
		DWORD dwValue=32;
		DWORD type=REG_SZ;

		if (RegEnumValue(hKeyYotsuya, cnt, szValue, &dwValue, 0, &type, (LPBYTE)szData, &dwData)!=ERROR_SUCCESS)
		{
			break;
		}
		cnt++;

		if (strncmp(szValue, "shortcut", 8)==0)
		{
			list->addref(yistrheap(szData));
		}
	}

	RegCloseKey(hKeyYotsuya);

	if (_list==0 &&
			list->count()==0)
	{
		delete list;

		return 0;
	}

	return list;
}

// --------------------------------
cyiList* xGetSoftwareFileList(const char* pszSoftware)
{
	HKEY hKeyYotsuya=xGetSoftwareKey(pszSoftware);
	cyiList* list = new cyiList();	
	int cnt=0;

	while(1)
	{
		char szValue[32];
		DWORD dwData=32;
		char szData[256];
		DWORD dwValue=256;
		DWORD type=REG_SZ;

		if (RegEnumValue(hKeyYotsuya, cnt, szValue, &dwValue, 0, &type, (LPBYTE)szData, &dwData)!=ERROR_SUCCESS)
		{
			break;
		}
		cnt++;

		if (strncmp(szValue, "file", 4)==0)
		{
			list->addref(yistrheap(szData));
		}
	}

	RegCloseKey(hKeyYotsuya);

	if (list->count()==0)
	{
		delete list;

		return 0;
	}

	return list;
}

// --------------------------------
Bool xGetSoftwareDisplayName(const char* pszSoftware, char* display)
{
	HKEY hKeyYotsuya=xGetSoftwareKey(pszSoftware);
	DWORD type = REG_SZ;
	DWORD xx=256;
	display[0]=0;

	if (RegGetValueA(hKeyYotsuya, "", "DisplayName", RRF_RT_ANY, &type, display, &xx)==ERROR_SUCCESS)
	{
		RegCloseKey(hKeyYotsuya);
		return True;
	}

	RegCloseKey(hKeyYotsuya);
	return False;
}


Bool xGetGroupDisplayName(char* display)
{
	HKEY hKeyYotsuya=xGetRootKey();
	DWORD type = REG_SZ;
	DWORD xx=256;
	display[0]=0;

	if (RegGetValueA(hKeyYotsuya, "", "DisplayName", RRF_RT_ANY, &type, display, &xx)==ERROR_SUCCESS)
	{
		RegCloseKey(hKeyYotsuya);
		return True;
	}
	RegCloseKey(hKeyYotsuya);
	return False;
}

// --------------------------------
Bool xGetSoftwareInstallDir(const char* pszSoftware, char* path)
{
	HKEY hKeyYotsuya=xGetSoftwareKey(pszSoftware);
	cyiList* list = new cyiList();	

	DWORD type = REG_SZ;
	DWORD xx=256;
	path[0]=0;

	if (RegGetValueA(hKeyYotsuya, "", "InstDir", RRF_RT_ANY, &type, path, &xx)==ERROR_SUCCESS)
	{
		RegCloseKey(hKeyYotsuya);
		return True;
	}

	RegCloseKey(hKeyYotsuya);
	return False;
}

// --------------------------------
Bool xIsCommonDelete()
{
	Bool bHasCommon=False;
	cyiList* list = xGetSoftwareList();
	int cnt=0;

	for (cyiNode* node = list->GetNodeTopFrom(); node; node=node->np)
	{
		if (stricmp((const char*)node->ref, "Common")==0)
		{
			bHasCommon=True;
			break;
		}
	}

	if (bHasCommon==True)
	{
		if (list->count()==1)
		{
			delete list;
			return True;
		}

		if (list->count()==2)
		{
			delete list;
			return True;
		}
	}

	delete list;
	return False;
}