#include "StdAfx.h"
#include "yiGoApp.h"

cyiGoApp::cyiGoApp(const char* pszSoftwareName, const char* pszApplicationName, Bool bIsMultiple)
{
	strcpy(m_szAppName, pszApplicationName);
	m_bIsMultiple=bIsMultiple;
}

cyiGoApp::~cyiGoApp(void)
{
}

// ------------------------
Bool cyiGoApp::Initialize(const char* pszCommandLine)
{
	HWND hWnd;
	Bool res=True;

	if (cyiGoApp::GetApplicationDirectory(m_szSoftwareName, m_szInstDir)==False)
	{
		//res = False;
	}

	if (m_bIsMultiple==False)
	{
		char szMutexName[256];
		wsprintf(szMutexName, "Global\\%s", m_szAppName);

		m_hMutex=OpenMutex(MUTEX_ALL_ACCESS , FALSE, szMutexName);

		if (m_hMutex==0)
		{
			m_hMutex = CreateMutex(0, FALSE, szMutexName);
		}
		else
		{
			m_hMutex=0;
		}

		if (m_hMutex==0)
		{
			if (cyiGoApp::GetAppAttributeInt(this->m_szInstDir, this->m_szAppName, "Handle", (int*)&hWnd, -1)==True)
			{
				if (IsWindow(hWnd)==True)
				{
					OnApplicationDuplicated(hWnd);

					if (pszCommandLine)
					{
						COPYDATASTRUCT s;
						s.cbData=0;
						s.dwData=strlen(pszCommandLine+1);
						s.lpData=(LPVOID)pszCommandLine;
						res = False;
						SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&s);

						//MessageBox(0, pszCommandLine, "command line", MB_OK);
					}
				}
			}
			return False;
		}

		//MessageBox(NULL, szMutexName, "Mutex success", MB_OK);

		//if (cyiGoApp::GetAppAttributeInt(this->m_szInstDir, this->m_szAppName, "Handle", (int*)&hWnd, -1)==True)
		//{
		//	if (IsWindow(hWnd)==True)
		//	{
		//		OnApplicationDuplicated(hWnd);

		//		if (pszCommandLine)
		//		{
		//			COPYDATASTRUCT s;
		//			s.cbData=0;
		//			s.dwData=strlen(pszCommandLine+1);
		//			s.lpData=(LPVOID)pszCommandLine;
		//			res = False;
		//			SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&s);
		//		}
		//	}
		//}
	}

	return res;
}

// ------------------------
void cyiGoApp::OnMainWindowCreateAfter(const char* pszCommandLine)
{
	return ;
}

// ------------------------
Bool cyiGoApp::Finalize()
{
	cyiGoApp::SetAppAttributeInt(m_szInstDir, m_szAppName, "Handle", 0);//==True)
	CloseHandle(m_hMutex);
	return True;
}

// ------------------------
void cyiGoApp::OnApplicationDuplicated(HWND hWnd)
{
	return ;
}

// ------------------------
void cyiGoApp::SetMainWindow(HWND hWnd, void* param)
{
	m_hMainWnd=hWnd;
	cyiGoApp::SetAppAttributeInt(m_szInstDir, m_szAppName, "Handle", (int)hWnd);
	m_param=param;
	return ;
}


// for static

// ------------------------
Bool cyiGoApp::GetApplicationDirectory(const char* pszSoftwareName, char* szApplicationDir)
{
		char szKey[128];
		HKEY hKeyApp;
		DWORD type, size;

		wsprintf(szKey, "%s\\%s", "SOFTWARE", pszSoftwareName);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_READ, &hKeyApp)!=ERROR_SUCCESS)
		{
			//GetCurrentDirectory(256, szApplicationDir);
			goto THEN_ERROR;
		}

		if (RegQueryValueEx(hKeyApp, "InstDir", 0, &type, (BYTE*)szApplicationDir, &size)!=ERROR_SUCCESS)
		{
			//GetCurrentDirectory(256, szApplicationDir);
			RegCloseKey(hKeyApp);
			goto THEN_ERROR;
		}

		RegCloseKey(hKeyApp);
		return True;

THEN_ERROR:
		{
			char szModuleName[256];
			char szAppDir[256];
			GetModuleFileNameA(0, szModuleName, 256);
			yiFileGetNameAndExt(szModuleName, szAppDir, 0, 0);
			SetCurrentDirectoryA(szAppDir);
			strcpy(szApplicationDir, szAppDir);
		}
		return False;
}

// ------------------------
Bool cyiGoApp::SetAppAttributeStr(const char* pszAppDir, const char* pszAppName, const char* key, char* value)
{
	char szIniFileName[256];
	wsprintf(szIniFileName, "%s\\all.ini", pszAppDir);

	return WritePrivateProfileStringA(pszAppName, key, value, szIniFileName);
}

// ------------------------
Bool cyiGoApp::SetAppAttributeInt(const char* pszAppDir, const char* pszAppName, const char* key, int value)
{
	char szIniFileName[256];
	char szValue[16];
	wsprintf(szIniFileName, "%s\\all.ini", pszAppDir);

	return WritePrivateProfileStringA(pszAppName, key, itoa(value, szValue, 10), szIniFileName);
}

// ------------------------
Bool cyiGoApp::GetAppAttributeStr(const char* pszAppDir, const char* pszAppName, const char* key, char* value)
{
	char szIniFileName[256];
	wsprintf(szIniFileName, "%s\\all.ini", pszAppDir);

	if (GetPrivateProfileStringA(pszAppName, key, "", value, 256, szIniFileName)==0)
	{
		return False;
	}

	return True;
}

// ------------------------
Bool cyiGoApp::GetAppAttributeInt(const char* pszAppDir, const char* pszAppName, const char* key, int* value, int nDefault)
{
	char szValue[16];
	char szIniFileName[256];
	wsprintf(szIniFileName, "%s\\all.ini", pszAppDir);

	*value = GetPrivateProfileInt(pszAppName, key, nDefault, szIniFileName);

	return (*value!=nDefault)?True:False;
}

// ------------------------
Bool cyiGoApp::GetProperty(const char* pszSection, const char* Key, char* value)
{
	if (pszSection==0)
	{
		pszSection=(const char*)m_szAppName;
	}
	return cyiGoApp::GetAppAttributeStr(m_szInstDir, pszSection, Key, value);
}

// ------------------------
Bool cyiGoApp::GetPropertyInt(const char* pszSection, const char* Key, int& value, int defaultValue)
{
	if (pszSection==0)
	{
		pszSection=(const char*)m_szAppName;
	}
	return cyiGoApp::GetAppAttributeInt(m_szInstDir, pszSection, Key, &value, defaultValue);
}

// ------------------------
Bool cyiGoApp::SetPropertyInt(const char* pszSection, const char* Key, int n)
{
	if (pszSection==0)
	{
		pszSection=(const char*)m_szAppName;
	}
	return cyiGoApp::SetAppAttributeInt(m_szInstDir, pszSection, Key, n);
}

// ------------------------
Bool cyiGoApp::SetProperty(const char* pszSection, const char* Key, char* value)
{
	if (pszSection==0)
	{
		pszSection=(const char*)m_szAppName;
	}
	return cyiGoApp::SetAppAttributeStr(m_szInstDir, pszSection, Key, value);
}

/*
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;
	HWND hWnd;
	cyiGoApp app("YotsuyaSoft", "YotsuyaSample", False);

	app.Initialize();

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GOYOTSUYA, szWindowClass, MAX_LOADSTRING);

	// TODO: CreateWindow.
	// app.SetMainWindow(m_hWnd);

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	app.Finalize();
}
*/
