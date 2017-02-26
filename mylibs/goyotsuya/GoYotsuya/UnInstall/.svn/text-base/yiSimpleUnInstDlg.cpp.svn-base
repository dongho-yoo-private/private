#include "StdAfx.h"

#include "..\\Setup\\SetupCommon.h"
#include "yiSimpleUnInstDlg.h"





// ------------------------------------
cyiSimpleUnInstDlg::cyiSimpleUnInstDlg(void)
{
	m_bIsQuit=FALSE;
	m_AppList=0;
}

// ------------------------------------
cyiSimpleUnInstDlg::~cyiSimpleUnInstDlg(void)
{
	if (m_AppList)
		delete m_AppList;
}

// ------------------------------------
void cyiSimpleUnInstDlg::SetResourceList(sResourcesInfo* info)
{
	int cnt=0;
	while(info[cnt++].pszFileName);

	m_nInstallFileCount=cnt;
	install_files= cnt ? info:0;
}

// --------------------------------
LRESULT cyiSimpleUnInstDlg::OnThreadEnd(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	handled=True;
	m_bIsQuit=True;

	if (wParam==0)
	{
		MessageBox(m_hWnd, "アプリケーションを削除しました。", "お知らせ", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(m_hWnd, "アプリケーションを削除に失敗しました。\r\nGo!よつやのアプリケーションが実行中でないことをご確認の上、\r\nもう一度やり直してください。", "失敗", MB_OK|MB_ICONERROR);
	}
	EndDialog(m_hWnd, 0);
	return 0;
}


// ------------------------------------
LRESULT cyiSimpleUnInstDlg::OnOk()
{
	if (m_bIsQuit==True)
	{
		EndDialog(m_hWnd, 0);
		return 0;
	}

	ShowWindow(GetDlgItem(m_hWnd, IDC_CMB_APPLICATION), SW_HIDE);
	ShowWindow(GetDlgItem(m_hWnd, IDC_PROGRESS), SW_SHOW);
	
	EnableWindow(GetDlgItem(m_hWnd, IDOK), FALSE);
	EnableWindow(GetDlgItem(m_hWnd, IDCANCEL), FALSE);


	SetWindowText(GetDlgItem(m_hWnd, IDC_COMMENT), "削除中・・・");

	StartThread(1, 0, 0);
	return 0;
}


#include <Shlobj.h>
#pragma comment(lib, "Shell32.lib")

typedef struct xsApplicationName {
	char szDisplayName[64];
	char szApplicationName[64];
} sAppName;


// ------------------------------------
LRESULT cyiSimpleUnInstDlg::OnCreate(WPARAM wp, LPARAM lp)
{
	// レジストリがあるかどうかをチェック
	// レジストリからディレクトリ取得
	HKEY hKeyYotsuya = xGetRootKey();
	int cnt=0;

	xTRACE("cyiSimpleUnInstDlg::OnCreate() called");
	if (hKeyYotsuya==0)
	{
		MessageBox(m_hWnd, "インストールされてないか、既に削除されています。", "お知らせ", MB_OK|MB_ICONINFORMATION);
		EndDialog(m_hWnd, 0);
		return 1;
	}

	RegCloseKey(hKeyYotsuya);
	
	// 1. まずは。。。
	// コンボボックスに削除対象を表示させる。

	HWND hCmbApp=GetDlgItem(m_hWnd, IDC_CMB_APPLICATION);
	SendMessage(hCmbApp, CB_ADDSTRING, 0, (LPARAM)"全て");

	cyiList* list = ::xGetSoftwareList();

	if (list==0 ||
			list->count()==0)
	{
		MessageBox(m_hWnd, "削除対象のアプリケーションがありません。", "お知らせ", MB_OK|MB_ICONINFORMATION);
		EndDialog(m_hWnd, 0);
		return 1;
	}

	cnt=0;
	for (cyiNode* node = list->GetNodeTopFrom(); node; node=node->np)
	{
		const char* p = (const char*)node->ref;

		if (stricmp(p, "Common")==0)
			continue;
		SendMessage(GetDlgItem(m_hWnd, IDC_CMB_APPLICATION), CB_ADDSTRING, 0, (LPARAM)p);
		cnt++;

	}

/*	while(1)
	{
		char szSoftwareName[64];
		DWORD keySize=64;

		if (RegEnumKeyEx(hKeyYotsuya, cnt, szSoftwareName, &keySize, 0, 0, 0, 0)!=ERROR_SUCCESS)
		{
			break;
		}

		if (stricmp(szSoftwareName, "Common")==0)
			continue;

		SendMessage(GetDlgItem(m_hWnd, IDC_CMB_APPLICATION), CB_ADDSTRING, 0, (LPARAM)szSoftwareName);
		cnt++;
	}
*/
	SendMessage(GetDlgItem(m_hWnd, IDC_PROGRESS), PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	SendMessage(GetDlgItem(m_hWnd, IDC_PROGRESS), PBM_SETSTEP, 1, 0);
	
	SendMessage(::GetDlgItem(m_hWnd, IDC_CMB_APPLICATION), CB_SETCURSEL, 0, 0);
	xTRACE("cyiSimpleUnInstDlg::OnCreate() exit");

	return 1;
}


// --------------------------------
LRESULT cyiSimpleUnInstDlg::OnCancel()
{
	EndDialog(m_hWnd, 0);
	return 0;
}

// --------------------------------
LRESULT cyiSimpleUnInstDlg::OnDeleteAll()
{
	ShowWindow(GetDlgItem(m_hWnd, IDC_CMB_APPLICATION), SW_HIDE);
	ShowWindow(GetDlgItem(m_hWnd, IDC_PROGRESS), SW_SHOW);
	
	EnableWindow(GetDlgItem(m_hWnd, IDOK), FALSE);
	EnableWindow(GetDlgItem(m_hWnd, IDCANCEL), FALSE);


	SetWindowText(GetDlgItem(m_hWnd, IDC_COMMENT), "削除中・・・");

	StartThread(1, 1, 0);
	return 0;
}

// --------------------------------
//static int xGetDeleteFiles(cyiList* list, const char* pszSoftware)
//{
//	// 全て
//	HKEY hKeyYotsuya = xGetRootKey();
//	if (pszSoftware==0)
//	{
//		for (int i=0; i<m_nSoftwareCount; i++)
//		{
//			char szSoftwareName[32];
//			DWORD n=32;
//			RegEnumKeyEx(hKeyYotsuya, i, szSoftwareName, &n, 0, 0, 0, 0); 
//			xGetSoftwareFiles(hKeyYotsuya, szSoftwareName, list);
//		}
//	}
//	else
//	{
//		xGetSoftwareFiles(hKeyYotsuya, pszSoftware, list);
//	}
//	RegCloseKey(hKeyYotsuya);
//	
//	return list->count();
//}






// --------------------------------
void cyiSimpleUnInstDlg::OnThreadStart(int id, int param1, int param2)
{
	if (id==1)
	{
		int n=0;
		char szInstallDir[256];
		char szSoftware[64];
		Bool bIsCommonNotInstalled=False;

		cyiList listDeleteFile;
		int nInstalledApplicationCount=0;
		Bool bIsAllRemove=False;
		cyiList* listSoftware;
		cyiList* listShortcut=0;
		
		xTRACE("cyiSimpleUnInstDlg::OnThreadStart() called");
		//int index = SendMessage(GetDlgItem(m_hWnd, IDC_CMB_APPLICATION), CB_GETCURSEL, 0, 0);
		//char szDisplayNameEn[256];
		
		SendMessage(GetDlgItem(m_hWnd, IDC_PROGRESS), PBM_SETPOS, 5, 0);

		// 1. レジストリからインストールされたファイルのリストを取得。
		HWND hCmbApp = GetDlgItem(m_hWnd, IDC_CMB_APPLICATION);
		n = SendMessage(hCmbApp, CB_GETCURSEL, 0, 0);
		bIsAllRemove = n==0?True:False;

		SendMessage(hCmbApp, CB_GETLBTEXT, n, (LPARAM)szSoftware);

		nInstalledApplicationCount = SendMessage(hCmbApp, CB_GETCOUNT, 0, 0);

		szSoftware[0]=0;
		listSoftware = xGetSoftwareList();

		if (listSoftware==0)
		{
			::PostMessage(m_hWnd, WM_THREAD_QUIT, -1, 0);
			return ;
		}

		// 2. インストールされたソフトのでxれくとり毎削除。
		//HKEY hRoot = xGetRootKey();
		if (n==0)
		{
			listShortcut= new cyiList();
			for (cyiNode* node = listSoftware->GetNodeTopFrom();node;node=node->np)
			{
				if (stricmp((const char*)node->ref, "Common")==0)
				{
					continue;
				}

				char szInstallDir[128];
				xGetSoftwareInstallDir((const char*)node->ref, szInstallDir);
				yiRemoveForced(szInstallDir, True);

				listShortcut = ::xGetSoftwareShortcutList((const char*)node->ref, listShortcut);
			}

			if (listShortcut->count()==0)
			{
				delete listShortcut;
				listShortcut=0;
			}
		}
		else
		{
			char szInstallDir[128];
			SendMessage(hCmbApp, CB_GETLBTEXT, n, (LPARAM)szSoftware);

			if (xGetSoftwareInstallDir(szSoftware, szInstallDir)==True)
			{
				yiRemoveForced(szInstallDir, True);
			}

			listShortcut = ::xGetSoftwareShortcutList(szSoftware);
			//xGetShortCutKey(hRoot, szSoftware, &listShortCut);
		}

		SendMessage(GetDlgItem(m_hWnd, IDC_PROGRESS), PBM_SETPOS, 20, 0);

		HKEY hCommon=0;
		HKEY hKeyRoot = xGetRootKey();
		if (RegOpenKeyEx(hKeyRoot, "Common", 0, KEY_READ, &hCommon)!=ERROR_SUCCESS)
		{
			bIsCommonNotInstalled=True;
		}

		::RegCloseKey(hKeyRoot);


		Bool bDeleteCommon=False;
		// 2. もし、インストールされているのが自分のみなら。
		if (bIsCommonNotInstalled==False)
		{
			if (bIsAllRemove==True)
			{
				bDeleteCommon=True;
			}
			else if (nInstalledApplicationCount==2 ||
						nInstalledApplicationCount==1)
			{
				bDeleteCommon=True;
			}
		}

		if (bDeleteCommon==True)
		{
			cyiList* list = ::xGetSoftwareFileList("Common");

			if (list)
			{
				for (cyiNode* node = list->GetNodeTopFrom(); node; node=node->np)
				{
					yiRemoveForced((const char*)node->ref, True);
				}
			}

			::RegCloseKey(hCommon);
			hCommon=0;
			char szSubKey[64];
			wsprintf(szSubKey, "SOFTWARE\\%s\\%s", g_szGroupName, "Common");
			::RegDeleteKey(HKEY_LOCAL_MACHINE, szSubKey);
		}

		if (listShortcut)
		{
			for (cyiNode* node = listShortcut->GetNodeTopFrom(); node; node=node->np)
			{
				yiRemoveForced((const char*)node->ref, True);
				yifree(node->ref);
			}
			delete listShortcut;
		}

		char szStartupApp[128];
		char szStartUp[128];
		char szGroupDisplayName[64];

		if (xGetGroupDisplayName(szGroupDisplayName)==False)
		{
			strcpy(szGroupDisplayName, g_szGroupName);
		}

		SHGetSpecialFolderPath(m_hWnd, szStartUp, CSIDL_COMMON_PROGRAMS,True);
		wsprintf(szStartupApp, "%s\\%s", szStartUp, szGroupDisplayName);

		SendMessage(GetDlgItem(m_hWnd, IDC_PROGRESS), PBM_SETPOS, 50, 0);

		// スタートアップディレクトリ全て削除。
		if (bDeleteCommon==True)
		{
			char szSubKey[32];
			// スタートアップディレクトリ削除。
			if (g_szGroupName[0]!=0)
			{
				yiRemoveForced(szStartupApp, True);
			}

			cyiList* list = xGetSoftwareList();

			if (list && list->count())
			{
				for (cyiNode* node = list->GetNodeTopFrom(); node; node=node->np)
				{
					char szSubKey[64];
					wsprintf(szSubKey, "SOFTWARE\\%s\\%s", g_szGroupName, node->ref);
					::RegDeleteKey(HKEY_LOCAL_MACHINE, szSubKey);
					yifree(node->ref);
				}
				delete list;
			}

			// グループそのものを削除。
			wsprintf(szSubKey, "SOFTWARE\\%s", g_szGroupName);
			RegDeleteKey(HKEY_LOCAL_MACHINE, szSubKey);
		}
		else
		{
			char szSubKey[32];
			// レジストリ削除。
			wsprintf(szSubKey, "SOFTWARE\\%s\\%s", g_szGroupName, szSoftware);
			RegDeleteKey(HKEY_LOCAL_MACHINE, szSubKey);

			if (listSoftware)
				delete listSoftware;

			listSoftware = xGetSoftwareList();

			if (listSoftware)
			{
				for (cyiNode* node=listSoftware->GetNodeTopFrom(); node; node=node->np)
				{
					if (stricmp((const char*)node->ref, "Common")==0)
						continue;

					Bool bIsExit=False;
					cyiList* listFile = xGetSoftwareFileList((const char*)node->ref);
					for (cyiNode* node = listFile->GetNodeTopFrom(); node; node=node->np)
					{
						const char* pszFileName = ::yiPathGetFileName((const char*)node->ref);
						if (stricmp(pszFileName, "UnInstall.exe")==0)
						{
							char szShortCut[256];
							wsprintf(szShortCut, "%s\\%s\\UnInstall.lnk", szStartUp, g_szGroupName);

							yiUtilCreateShortCutKey(szShortCut, (char*)node->ref, g_szGroupName);//==FALSE)
							bIsExit=True;
							break;
						}
						yifree(node->ref);
					}
					delete listFile;

					yifree(node->ref);

					if (bIsExit==True)
						break;
				}
				delete listSoftware;
			}
		}
		
		SendMessage(GetDlgItem(m_hWnd, IDC_PROGRESS), PBM_SETPOS, 100, 0);

		PostMessage(m_hWnd, WM_THREAD_QUIT, 0, 0);
		//EnableWindow(GetDlgItem(m_hWnd, IDOK), True);
		//SetWindowText(GetDlgItem(m_hWnd, IDOK), "終了");
		m_bIsQuit=True;
	}
}





