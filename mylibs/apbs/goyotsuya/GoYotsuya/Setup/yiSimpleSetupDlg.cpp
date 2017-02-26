#include "StdAfx.h"
#include "yiSimpleSetupDlg.h"



#pragma comment(lib, "shell32.lib")

HINSTANCE hExtraModule=0;

cyiSimpleSetupDlg::cyiSimpleSetupDlg(void)
{
	m_bResult=False;
}

cyiSimpleSetupDlg::~cyiSimpleSetupDlg(void)
{
}

Bool cyiSimpleSetupDlg::IsSuccess()
{
	return m_bResult;//m_result==0?True:False;
}

void cyiSimpleSetupDlg::SetResourceList(sResourcesInfo* info)
{
	int cnt=0;
	while(info[cnt++].pszFileName);

	m_nInstallFileCount=cnt;
	install_files= cnt ? info:0;
}

LRESULT cyiSimpleSetupDlg::OnOK()
{
	//cyctrlbase::OnOK();
	char fullpath[256];

	if (m_bInstallQuit==True)
	{
		return ::EndDialog(m_hWnd, 0);
	}

	GetWindowText(GetDlgItem(m_hWnd, IDC_EDIT_PATH), fullpath, 256);
	
	if (yiMakeDir(fullpath)==False)
	{
		DWORD error = GetLastError();

		if (error!=183)
		{
			MessageBox(m_hWnd, "ディレクトリの作成に失敗しました。", "エラー", MB_OK|MB_ICONERROR);
			return 0;
		}
	}

	strcpy(m_szInstallPath, fullpath);

	int progress=0;
	Bool canceled=False;

	EnableWindow(GetDlgItem(m_hWnd, IDC_PROGRESS), TRUE);
	EnableWindow(GetDlgItem(m_hWnd, IDOK), FALSE);
	EnableWindow(GetDlgItem(m_hWnd, IDCANCEL), FALSE);

	StartThread(1, 0, 0);

	return 0;

	//OnThreadStart(1, 0, 0);



	//MessageBox(m_hWnd, "インストールが終了しました。", "お知らせ", MB_OK|MB_ICONINFORMATION);
	//cyctrlbase::OnOK();
	
	// ショートカット作成


	return 0;
}

// -----------------------
LRESULT cyiSimpleSetupDlg::OnThreadQuit(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	if (wParam!=0)
	{
		m_bResult=False;
		if (wParam==1)
		{
			MessageBox(m_hWnd, "すでにインストールされています。\r\n先にアンインストールしてから行ってください。", "お知らせ", MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(m_hWnd, "インストールに失敗しました。", "エラー", MB_OK|MB_ICONERROR);
		}
		::EndDialog(m_hWnd, 0);
		return 0;
	}
	else
	{
		m_bResult=True;
		MessageBox(m_hWnd, "インストールが完了しました。", "お知らせ", MB_OK|MB_ICONINFORMATION);
		EndDialog(m_hWnd, 0);
	}

	//EnableWindow(GetDlgItem(m_hWnd, IDOK), TRUE);
	//SetWindowText(GetDlgItem(m_hWnd, IDOK), "確認");
	//SetWindowText(GetDlgItem(m_hWnd, IDC_COMMENT), "インストールが終了しました。");
	m_bInstallQuit=True;

	return 0;
}

LRESULT cyiSimpleSetupDlg::OnCancel()
{
	//return cyctrlbase::OnCancel();
	EndDialog(m_hWnd, 0);
	return 0;
}

LRESULT cyiSimpleSetupDlg::OnOpenDir(HWND hWnd, int code)
{
	char szInstallDir[256];

	GetWindowText(GetDlgItem(m_hWnd, IDC_EDIT_PATH), szInstallDir, 256);
	if (yiFileOpenPath(m_hWnd, "インストール先", szInstallDir)==True)
	{
		SetWindowText(GetDlgItem(m_hWnd, IDC_EDIT_PATH), szInstallDir);
	}

	return 0;
}

#include <Shlobj.h>
#pragma comment(lib, "Shell32.lib")
#include "setup.h"

extern sSoftwareInfo* g_pSoftwareInfo;

extern int g_nArchiveCount;
extern HandleArchive g_hArchive;

LRESULT cyiSimpleSetupDlg::OnCreate(WPARAM wp, LPARAM lp)
{
	char path[256];
	char fullpath[256];
	char name[128];
	int extraStep=0;

	SHGetSpecialFolderPathA(m_hWnd, path, CSIDL_PROGRAM_FILESX86, False);
	//LoadStringA(m_hInstance, IDS_ROOT_NAME, name, 256);

	wsprintf(fullpath, "%s\\%s", path, g_pSoftwareInfo->szSoftwareName);//g_pSoftwareInfo->szGroupName);
	SetWindowText(GetDlgItem(m_hWnd, IDC_EDIT_PATH), fullpath);

	for (int i=1; i<g_nArchiveCount; i++)
	{
		Binary bin = yiArchiveEnum(g_hArchive, i);
		syiInstallFormat* tar = (syiInstallFormat*)bin;

		if (stricmp(tar->szFileName, "_setup.dll")==0)
		{
			char szTempDir[256];
			char szTempFileName[128];
			::GetTempPath(256, szTempDir);
			::GetTempFileName(szTempDir, 0, 0, szTempFileName);

			if (tar->flag32&0x80000000)
			{
				cyiMemoryStream* st = yiUtilDeCompress(&tar[1], xBIN_SIZE(bin)-sizeof(*tar));

				if (st==0)
				{
					goto ABORT_INSTALL;
				}
				st->ExportToFile(szTempFileName);
			}
			else
			{
				unsigned int check_sum = yiGetCheckSum(&tar[1], tar->size, tar->size<1024?4:256);

				if (check_sum!=tar->check_sum)
				{
					goto ABORT_INSTALL;
				}

				if (yiFileWrite(szTempFileName, &tar[1], 0, tar->size)==False)
				{
					goto ABORT_INSTALL;
				}
				hExtraModule = ::LoadLibrary(szTempFileName);

				if (hExtraModule==0)
					goto ABORT_INSTALL;
			}
			break;
		}
	}

	if (hExtraModule)
	{
		FARPROC fnc = GetProcAddress(hExtraModule, "_xGetTotalStep");

		if (fnc)
		{
			extraStep = fnc();
		}
	}

	SendMessage(GetDlgItem(m_hWnd, IDC_PROGRESS), PBM_SETRANGE, 0, MAKELPARAM(0, extraStep+g_nArchiveCount-1));
	SendMessage(GetDlgItem(m_hWnd, IDC_PROGRESS), PBM_SETSTEP, 1, 0);

	EnableWindow(GetDlgItem(m_hWnd, IDC_PROGRESS), FALSE);
	m_bInstallQuit=FALSE;

	//StartThread(1, 0, 0);
	return 1;

ABORT_INSTALL:
	::MessageBox(m_hWnd, "インストールに失敗しました。", "エラー", MB_OK|MB_ICONERROR);
	::EndDialog(m_hWnd, 0);
	return 0;
}

typedef Bool (*OptionalSetupFunc)(sSoftwareInfo* info, const char* pszInstallPath, HWND hWndProgress);

#include "SetupCommon.h"
// --------------------------------
void cyiSimpleSetupDlg::OnThreadStart(int id, int param1, int param2)
{
	if (id==1)
	{
		//char szAppName[32];
		char szStartUp[256];
		char szStartUpApp[256];
		char szDesktopDir[256];
		//char szDisplayName[128];
		char szRootName[128];
		char szIniFileName[256];
		//char szAppDisplayName[128];
		int nInstallFileCnt=0;
		char szKey[128];
		HKEY hKeyYotsuya;
		HKEY hKeyApp=0;
		HKEY hCommon=0;
		int nSoftCnt=0;
		int nCommonCnt=0;
		int cnt=0;
		Bool bIsIamFirst=False;
		char szBeforeUninstallPath[256];
		cyiList listInstalledFile;
		cyiList listInstalledCommonFile;

		//wsprintf(szIniFileName, "%s\\%s.ini", m_szInstallPath, "all");
		
		m_bResult=TRUE;
		szBeforeUninstallPath[0]=0;

		strcpy(g_szGroupName, g_pSoftwareInfo->szGroupName);

		// この段階でレジストリに書き込む
		// HKEY_LOCAL_MACHINE\\SOFTWARE\YotsuyaSoft
		//goto ABORT_INSTALL;


		wsprintf(szKey, "%s\\%s", "SOFTWARE", g_pSoftwareInfo->szGroupName);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_ALL_ACCESS, &hKeyYotsuya)!=ERROR_SUCCESS)
		{
			if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, szKey, 0,0 , 0, KEY_ALL_ACCESS, 0, &hKeyYotsuya, 0)!=ERROR_SUCCESS)
			{
				DWORD dwError =GetLastError();
				PostMessage(m_hWnd, WM_THREAD_QUIT, -1, 0);
				m_bResult=FALSE;
				return ;
			}
		}

		RegSetValueEx(hKeyYotsuya, "DisplayName", 0, REG_SZ, (const BYTE*)g_pSoftwareInfo->szGroupNameDisplay, strlen(g_pSoftwareInfo->szGroupNameDisplay)+1);

		// まず、ここで自分以外にすでにインストールされているかどうかを見極める。
		{
			cyiList* list = ::xGetSoftwareList();

			if (list==0 /*||
					list->count()==0*/)
			{
				bIsIamFirst=True;
			}
			else if (list->count()==1)
			{
				const char* p = (const char*)list->GetTopFromRef(0);

				if (stricmp(p, "Common")==0)
				{
					bIsIamFirst=True;
				}
			}

			if (list)
				delete list;
		}

		if (RegOpenKeyEx(hKeyYotsuya, g_pSoftwareInfo->szSoftwareName, 0, KEY_ALL_ACCESS, &hKeyApp)==ERROR_SUCCESS)
		{
			PostMessage(m_hWnd, WM_THREAD_QUIT, 1, 0);
			::RegCloseKey(hKeyYotsuya);
			return ;
		}

		if (RegCreateKeyEx(hKeyYotsuya, g_pSoftwareInfo->szSoftwareName, 0,0 , 0, KEY_ALL_ACCESS, 0, &hKeyApp, 0)!=ERROR_SUCCESS)
		{
				DWORD dwError =GetLastError();
				PostMessage(m_hWnd, WM_THREAD_QUIT, -1, 0);
				m_bResult=FALSE;
				::RegCloseKey(hKeyYotsuya);
				return ;
		}

		RegCreateKeyEx(hKeyYotsuya, "Common", 0,0 , 0, KEY_ALL_ACCESS, 0, &hCommon, 0);

		DWORD b=strlen(m_szInstallPath)+1;
		RegSetValueEx(hKeyApp, "InstDir", 0, REG_SZ, (const BYTE*)m_szInstallPath, b);
		
		b=strlen(g_pSoftwareInfo->szSoftwareNameDisplay)+1;
		RegSetValueEx(hKeyApp, "DisplayName", 0, REG_SZ, (const BYTE*)g_pSoftwareInfo->szSoftwareNameDisplay, b);

		RegCloseKey(hKeyYotsuya);
		//RegCloseKey(hKeyApp);

		SHGetSpecialFolderPath(m_hWnd, szStartUp, CSIDL_COMMON_PROGRAMS, True);
		wsprintf(szStartUpApp, "%s\\%s", szStartUp, g_pSoftwareInfo->szGroupNameDisplay);
		yiMakePath(szStartUpApp, False);

		if (hExtraModule)
		{
			OptionalSetupFunc fnc = (OptionalSetupFunc)GetProcAddress(hExtraModule, "_xBeforeExtract");

			if (fnc)
			{
				HWND hProgress = GetDlgItem(m_hWnd, IDC_PROGRESS);
				if (fnc(g_pSoftwareInfo, m_szInstallPath, hProgress)==False)
				{
					goto ABORT_INSTALL;
				}
			}
		}

		for (int i=1; i<g_nArchiveCount; i++)
		{
			Binary bin = yiArchiveEnum(g_hArchive, i);
			syiInstallFormat* tar = (syiInstallFormat*)bin;
			char szFileName[256];
			char szInstallDir[256];

			if (tar==0)
				break;

			if (tar->flag32&0x40000000)
				continue;

			if (stricmp(tar->szFileName, "_setup.dll")==0)
				continue;

			int nDir = tar->flag32&0x0F;

			//"インストール先", 0
			//"%windir%", 1
			//"%system%", 2
			//"ユーザ", 3
			//"AppData", 4
			if (nDir==0)
			{
				strcpy(szInstallDir, m_szInstallPath);
				if (tar->szPath[0]!=0)
				{
					strcat(szInstallDir, "\\");
					strcat(szInstallDir, tar->szPath);
					yiMakePath(szInstallDir, False);
				}
				strcat(szInstallDir, "\\");
			}
			else if (nDir==1) // common
			{
				::GetWindowsDirectoryA(szInstallDir, 256);
				strcat(szInstallDir, "\\");
				//wsprintf(szInstallDir, "%s\\%s", m_szInstallPath, tar->szFileName);
			}
			else if (nDir==2)
			{
				::GetSystemDirectoryA(szInstallDir, 256);
				strcat(szInstallDir, "\\");
			}
			else if (nDir==3 ||
						nDir==4)
			{
				if (SHGetSpecialFolderPath(NULL, szInstallDir, CSIDL_APPDATA, TRUE)==FALSE)
				{
					goto ABORT_INSTALL;
				}
				
				strcat(szInstallDir, g_pSoftwareInfo->szGroupName);
				strcat(szInstallDir, "\\");
				strcat(szInstallDir, g_pSoftwareInfo->szSoftwareName);
				
				if (tar->szPath[0]!=0)
				{
					strcat(szInstallDir, "\\");
					strcat(szInstallDir, tar->szPath);
				}
				yiMakePath(szInstallDir, False);

				strcat(szInstallDir, "\\");
			}

			strcat(szInstallDir, tar->szFileName);


			if (tar->flag32&0x80000000)
			{
				cyiMemoryStream* st = yiUtilDeCompress(&tar[1], xBIN_SIZE(bin)-sizeof(*tar));

				if (st==0)
				{
					goto ABORT_INSTALL;
				}

				if (st->ExportToFile(szInstallDir)==False)
				{
					goto ABORT_INSTALL;
				}

				delete st;

			}
			else
			{
				unsigned int check_sum = yiGetCheckSum(&tar[1], tar->size, tar->size<1024?4:256);

				if (check_sum!=tar->check_sum)
				{
					goto ABORT_INSTALL;
				}

				if (yiFileWrite(szInstallDir, &tar[1], 0, tar->size)==False)
				{
					goto ABORT_INSTALL;
				}
			}

			if (nDir==1 ||
					nDir==2)
			{
				char szKey[32];
				wsprintf(szKey, "file%d", nCommonCnt++);

				RegSetValueEx(hCommon, szKey, 0, REG_SZ, (const BYTE*)szInstallDir, strlen(szInstallDir)+1);
				listInstalledCommonFile.addref(yistrheap(szInstallDir));
			}
			else
			{
				char szKey[32];
				wsprintf(szKey, "file%d", nSoftCnt++);

				RegSetValueEx(hKeyApp, szKey, 0, REG_SZ, (const BYTE*)szInstallDir, strlen(szInstallDir)+1);
				listInstalledFile.addref(yistrheap(szInstallDir));
			}

			if (tar->szShortCutName[0]!=0)//flag32&0x20000000)
			{
				char szLinkName[256];

				wsprintf(szLinkName, "%s\\%s.lnk", szStartUpApp, tar->szShortCutName);

				if (xIsUninstall(tar->szFileName)==True)
				{
					if (::yiIsExist(szLinkName, 0)==True)
					{
						::GetTempPath(255, szBeforeUninstallPath);
						strcat(szBeforeUninstallPath, "\\Uninstall.lnk");
						CopyFile(szBeforeUninstallPath, szLinkName, False);
					}

					if (yiUtilCreateShortCutKey(szLinkName, szInstallDir, g_pSoftwareInfo->szGroupName)==FALSE)
					{
						goto ABORT_INSTALL;
					}
				}
				else
				{
					if (yiUtilCreateShortCutKey(szLinkName, szInstallDir, 0)==FALSE)
					{
						goto ABORT_INSTALL;
					}
				}

				{
					char szKey[32];
					wsprintf(szKey, "shortcut%d", cnt);

					RegSetValueEx(hKeyApp, szKey, 0, REG_SZ, (const BYTE*)szLinkName, strlen(szLinkName)+1);

					if (nDir==1 ||
							nDir==2)
					{
						listInstalledCommonFile.addref(yistrheap(szLinkName));
					}
					else
					{
						listInstalledFile.addref(yistrheap(szLinkName));
					}
				}
				cnt++;

				if (tar->flag32&0x20000000)
				{
					if (SHGetSpecialFolderPath(m_hWnd, szDesktopDir, CSIDL_COMMON_DESKTOPDIRECTORY, TRUE)==FALSE)
					{
						goto ABORT_INSTALL;
					}

					wsprintf(szLinkName, "%s\\%s.lnk", szDesktopDir, tar->szShortCutName);
					if (yiUtilCreateShortCutKey(szLinkName, szInstallDir, 0)==FALSE)
					{
						goto ABORT_INSTALL;
					}
					{
						char szKey[32];
						wsprintf(szKey, "shortcut%d", cnt);

						RegSetValueEx(hKeyApp, szKey, 0, REG_SZ, (const BYTE*)szLinkName, strlen(szLinkName)+1);
						listInstalledFile.addref(yistrheap(szLinkName));

					}
					cnt++;
				}

			}
			SendMessage(GetDlgItem(m_hWnd, IDC_PROGRESS), PBM_STEPIT, 0, 0);
		}

		//SHGetSpecialFolderPathA(m_hWnd, szDesktopDir, CSIDL_DESKTOPDIRECTORY, False);
		//SHGetSpecialFolderPathA(m_hWnd, szStartUp, CSIDL_COMMON_PROGRAMS, False);

		//wsprintf(szStartUpApp, "%s\\%s", szStartUp, g_pSoftwareInfo->szGroupName);
		//yiMakeDir(szStartUpApp);

		RegCloseKey(hKeyApp);

		//goto ABORT_INSTALL;

		{
			int cnt=listInstalledFile.count();

			for (int i=0; i<cnt; i++)
			{
				yifree(listInstalledFile.removeEx(0));
			}

			cnt = listInstalledCommonFile.count();
			for (int i=0; i<cnt; i++)
			{
				yifree(listInstalledCommonFile.removeEx(0));
			}

		}

		if (szBeforeUninstallPath[0]!=0)
		{
			DeleteFile(szBeforeUninstallPath);
		}

		if (hExtraModule)
		{
			OptionalSetupFunc fnc = (OptionalSetupFunc)GetProcAddress(hExtraModule, "_xAfterExtract");

			if (fnc)
			{
				HWND hProgress = GetDlgItem(m_hWnd, IDC_PROGRESS);
				if (fnc(g_pSoftwareInfo, m_szInstallPath, hProgress)==False)
				{
					goto ABORT_INSTALL;
				}
			}
		}

		SendMessage(GetDlgItem(m_hWnd, IDC_PROGRESS), PBM_STEPIT, 0, 0);

		PostMessage(m_hWnd, WM_THREAD_QUIT, 0, 0);
		return ;
ABORT_INSTALL:
		// 1. まず、ディレクトリ削除。
		::yiRemoveForced(m_szInstallPath, True);

		if (bIsIamFirst==True)
		{
			// 全て削除。
			// 2. start up ディレクトリ削除。
			::yiRemoveForced(szStartUpApp, True);

			// 3. Commonファイル削除。
			cyiList* list = xGetSoftwareFileList("Common");
			if (list && list->count())
			{
				for (cyiNode* node = list->GetNodeTopFrom(); node; node=node->np)
				{
					DeleteFile((LPCSTR)node->ref);
					yifree(node->ref);
				}
				delete list;
			}

			list = xGetSoftwareList();

			if (list && list->count())
			{
				for (cyiNode* node = list->GetNodeTopFrom(); node; node=node->np)
				{
					char szSubKey[64];
					wsprintf(szSubKey, "SOFTWARE\\%s\\%s", g_pSoftwareInfo->szGroupName, node->ref);
					::RegDeleteKey(HKEY_LOCAL_MACHINE, szSubKey);
					yifree(node->ref);
				}
				delete list;
			}

			// 4. レジストリ削除。
			char szSubKey[64];
			wsprintf(szSubKey, "SOFTWARE\\%s", g_pSoftwareInfo->szGroupName);
			if (::RegDeleteKey(HKEY_LOCAL_MACHINE, szSubKey)!=ERROR_SUCCESS)
			{
				DWORD dwError = GetLastError();
				Sleep(1);
			}

			
		}
		else
		{
			// 2. startupのリンク削除。
			cyiList* list = ::xGetSoftwareShortcutList(g_pSoftwareInfo->szSoftwareName);

			if (list && list->count())
			{
				for (cyiNode* node = list->GetNodeTopFrom(); node; node=node->np)
				{
					DeleteFile((LPCSTR)node->ref);
					yifree(node->ref);
				}
				delete list;
			}

			// 3. レジストリ削除
			char szSubKey[64];
			wsprintf(szSubKey, "SOFTWARE\\%s\\%s", g_pSoftwareInfo->szGroupName, g_pSoftwareInfo->szSoftwareName);
			::RegDeleteKey(HKEY_LOCAL_MACHINE, szSubKey);

			// 4. Uninstall復元

			if (szBeforeUninstallPath[0]!=0)
			{
				char szLinkName[256];
				wsprintf(szLinkName, "%s\\%UnInstall.lnk", szStartUpApp);
				MoveFile(szBeforeUninstallPath, szLinkName);
			}
		}

		PostMessage(m_hWnd, WM_THREAD_QUIT, -1, 0);

	} // thread id is 1.

	return ;

}