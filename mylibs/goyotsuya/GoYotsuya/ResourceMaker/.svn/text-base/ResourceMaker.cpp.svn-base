// ResourceMaker.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "yistd.h"
#include "ResourceMaker.h"

#define MAX_LOADSTRING 100

INT_PTR CALLBACK MyDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ---------------------------------
static int LoadPackageList();


// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	//// グローバル文字列を初期化しています。
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_RESOURCEMAKER, szWindowClass, MAX_LOADSTRING);
	//MyRegisterClass(hInstance);

	//// アプリケーションの初期化を実行します:
	//if (!InitInstance (hInstance, nCmdShow))
	//{
	//	return FALSE;
	//}

	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RESOURCEMAKER));

	char path[256];

	//if (yiFileOpenDialog(0, "", "", "", "", path)==True)
	//{
	//	// open.
	//}

	int res = DialogBoxA(hInstance, MAKEINTRESOURCEA(IDD_DIALOG_MAIN), 0, MyDialogProc);

	return res;

	// メイン メッセージ ループ:
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RESOURCEMAKER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RESOURCEMAKER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

#include <shellapi.h>

WNDPROC g_wndproc=0;
cyiList* g_listFile=0;



typedef struct {
	char szPath[64];
	char szFileName[128];
	unsigned int flag32;
	unsigned int size;
	unsigned int check_sum;
	char szShortCutName[64];
	void* p;
	char szFullPath[256];
} syiInstallFormatEx;

const char* g_pszBaseDirName[] = {
	"インストール先", 
	"%windir%",
	"%system%",
	"ユーザ",
	"AppData",
	0
};

HWND g_hDlg=0;
// ---------------------------------
static Bool xCreateSetupFile(const char* pszProjectName);

// ---------------------------------
static void LoadSoftwareData(const char* pszSoftName);

// ---------------------------------
static void MakeSoftwareHeader();

syiInstallFormatEx* g_curTarFormat=0;

LRESULT CALLBACK xxWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct {
	char szGroupName[32];
	char szGroupNameDisplay[32];
	char szSoftwareName[32];
	char szSoftwareNameDisplay[32];
}sSoftwareInfo;

sSoftwareInfo g_soft;

// バージョン情報ボックスのメッセージ ハンドラです。
INT_PTR CALLBACK MyDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
		{
			if (g_listFile==0)
				g_listFile = new cyiList();

			ZeroMemory(&g_soft, sizeof(g_soft));
			g_hDlg=hDlg;
			HWND hWndList = GetDlgItem(hDlg, IDC_LIST_FILE);
			g_wndproc=(WNDPROC)GetWindowLongA(hWndList, GWL_WNDPROC);
			SetWindowLongA(hWndList, GWL_WNDPROC, (LONG)xxWndProc);
			// ドラッグアンドドロップを有効
			DragAcceptFiles(hWndList, TRUE);

			HWND hWndCmb = GetDlgItem(hDlg, IDC_BASE_DIR);

			int cnt=0;
			while(g_pszBaseDirName[cnt])
			{
				SendMessage(hWndCmb, CB_ADDSTRING, 0, (LPARAM)g_pszBaseDirName[cnt]);
				cnt++;
			}

			LoadPackageList();

			return (INT_PTR)TRUE;
		}

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK2 || LOWORD(wParam) == IDCANCEL)
			{
				if (wParam==IDOK2)
				{
					char szProject[128];
					int n = GetWindowText(GetDlgItem(hDlg, IDC_PROJECT), szProject, 128);

					MakeSoftwareHeader();

					if (n!=0)
					{
						if (xCreateSetupFile(szProject)==True)
						{
							EndDialog(hDlg, LOWORD(wParam));
						}
					}
				}
				if (wParam==IDCANCEL)
				{
					EndDialog(hDlg, LOWORD(wParam));
				}
				return (INT_PTR)TRUE;
			}
			if (LOWORD(wParam)==IDC_LIST_FILE)
			{
				if (HIWORD(wParam)==LBN_SELCHANGE)
				{
					int index = SendMessage((HWND)lParam, LB_GETCURSEL, 0, 0);
					syiInstallFormatEx* p = (syiInstallFormatEx* )g_listFile->GetTopFromRef(index);
					g_curTarFormat = p;

					HWND hWndCmb = GetDlgItem(hDlg, IDC_BASE_DIR);

					SendMessage(hWndCmb, CB_SETCURSEL, (p->flag32&0x0F), 0);

					SetWindowText(GetDlgItem(hDlg, IDC_DIR), p->szPath);

					SetWindowText(GetDlgItem(hDlg, IDC_SHORT_CUT), p->szShortCutName);

					if (p->flag32&0x80000000)
					{
						SendMessage(GetDlgItem(hDlg, IDC_CHK_COMPRESS), BM_SETCHECK, BST_CHECKED, 0);
					}
					else
					{
						SendMessage(GetDlgItem(hDlg, IDC_CHK_COMPRESS), BM_SETCHECK, BST_UNCHECKED, 0);
					}

					if (p->flag32&0x40000000)
					{
						SendMessage(GetDlgItem(hDlg, IDC_CHK_EXEC), BM_SETCHECK, BST_CHECKED, 0);
					}
					else
					{
						SendMessage(GetDlgItem(hDlg, IDC_CHK_EXEC), BM_SETCHECK, BST_UNCHECKED, 0);
					}

					if (p->flag32&0x20000000)
					{
						SendMessage(GetDlgItem(hDlg, IDC_CHK_DESKTOP), BM_SETCHECK, BST_CHECKED, 0);
					}
					else
					{
						SendMessage(GetDlgItem(hDlg, IDC_CHK_DESKTOP), BM_SETCHECK, BST_UNCHECKED, 0);
					}

				
				
					
				}
			}
			else if (LOWORD(wParam)==IDC_BASE_DIR)
			{
				if (HIWORD(wParam)==CBN_SELCHANGE)
				{
					int n = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);

					if (g_curTarFormat)
					{
						g_curTarFormat->flag32 &= (0xF0);
						g_curTarFormat->flag32 |= (n&0x0F);
					}
				}
			}
			else if (LOWORD(wParam)==IDC_DIR)
			{
				if (HIWORD(wParam)==EN_CHANGE)
				{
					if (g_curTarFormat)
					{
						GetWindowText((HWND)lParam, g_curTarFormat->szPath, 128);
					}
				}
			}
			else if (LOWORD(wParam)==IDC_CHK_COMPRESS)
			{
				if (g_curTarFormat)
				{
					if (SendMessage((HWND)lParam, BM_GETCHECK, 0, 0)==BST_CHECKED)
					{
						g_curTarFormat->flag32 |=0x80000000;
					}
					else
					{
						g_curTarFormat->flag32 &=(~0x80000000);
					}
				}
			}
			else if (LOWORD(wParam)==IDC_CHK_EXEC)
			{
				if (g_curTarFormat)
				{
					if (SendMessage((HWND)lParam, BM_GETCHECK, 0, 0)==BST_CHECKED)
					{
						g_curTarFormat->flag32 |=0x40000000;
					}
					else
					{
						g_curTarFormat->flag32 &=(~0x40000000);
					}
				}
			}
			else if (LOWORD(wParam)==IDC_SHORT_CUT)
			{
				if (HIWORD(wParam)==EN_CHANGE)
				{
					if (g_curTarFormat)
					{
						GetWindowText((HWND)lParam, g_curTarFormat->szShortCutName, 64);
					}
				}
			}
			else if (LOWORD(wParam)==IDC_CHK_DESKTOP)
			{
				if (g_curTarFormat)
				{
					if (SendMessage((HWND)lParam, BM_GETCHECK, 0, 0)==BST_CHECKED)
					{
						g_curTarFormat->flag32 |=0x20000000;
					}
					else
					{
						g_curTarFormat->flag32 &=(~0x20000000);
					}
				}
			}
			else if (LOWORD(wParam)==IDC_CMB_PACKAGE)
			{
				if (HIWORD(wParam)==CBN_SELCHANGE)
				{
					char buffer[128];
					int n = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
					SendMessage((HWND)lParam, CB_GETLBTEXT, 0, (LPARAM)g_soft.szSoftwareName);//buffer, 0);
					//GetWindowText((HWND)lParam, g_soft.szSoftwareName, 32);

					LoadSoftwareData(g_soft.szSoftwareName);

				}
				else if (HIWORD(wParam)==CBN_EDITCHANGE)
				{
					GetWindowText((HWND)lParam, g_soft.szSoftwareName, 32);
				}
			}
			break;
		}
		default:
			break;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK xxWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg==WM_DROPFILES)
	{
		char buffer[256];
		HDROP hDrop=(HDROP)wParam;
		int n = DragQueryFile(hDrop, -1, (LPSTR)buffer, MAX_PATH);

		for (int i=0; i<n; i++)
		{
			DragQueryFile(hDrop, i, (LPSTR)buffer, MAX_PATH);
			syiInstallFormatEx* p = (syiInstallFormatEx*)yialloci(sizeof(syiInstallFormatEx));
			strcpy(p->szFileName, yiPathGetFileName(buffer));
			strcpy(p->szFullPath, buffer);


			p->p=yiFileLoad(p->szFullPath, (int*)&p->size);

			g_listFile->addref(p);
			SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)p->szFileName);
		}

		DragFinish(hDrop);
	}

	return ::CallWindowProc(g_wndproc, hWnd, msg, wParam, lParam);
}

#define MAX_PACKAGES  64
#define MAX_FILECOUNT 256
#define _INI_FILE_ ".\\ResourceMaker.ini"

static Bool xCreateSetupFile(const char* pszProjectName)
{
	int cnt=0;
	HandleArchive hArch=yiArchiveCreate();

	// まず、ｸﾘｱ
	for (int i=0; i<MAX_FILECOUNT; i++)
	{
		char xx[32];
		wsprintf(xx, "file%d", i);
		WritePrivateProfileStringA(g_soft.szSoftwareName, xx, NULL, _INI_FILE_);
	}

	yiArchiveAdd(hArch, &g_soft, sizeof(g_soft), 0, 0);
	for (cyiNode* node = g_listFile->GetNodeTopFrom(0); node; node=node->np)
	{
		void* bin=0;
		int   size=0;
		syiInstallFormatEx* p = (syiInstallFormatEx*)node->ref;

		bin=p->p;
		size=p->size;

		p->check_sum = yiGetCheckSum(p->p, p->size, p->size<1024?4:256);

		if (p->flag32&0x80000000)
		{
			cyiMemoryStream* stream = yiUtilCompress(p->p, p->size);
			bin = stream->ExportToBinary();
			size = xBIN_SIZE(bin);
			delete stream;
		}

		yiArchiveAdd(hArch, bin, size, p, sizeof(syiInstallFormat));

		char buffer[512];
		char xx[32];
		wsprintf(xx, "file%d", cnt++);
		wsprintf(buffer, "%s,%s,%s,%d", p->szFullPath, p->szPath, p->szShortCutName, p->flag32);
		WritePrivateProfileStringA(g_soft.szSoftwareName, xx, buffer, _INI_FILE_);
	}

	yiArchiveExport(hArch, pszProjectName);
	yiArchiveClose(hArch);

	WritePrivateProfileStringA(g_soft.szSoftwareName, "ImageName", pszProjectName, _INI_FILE_);

	return True;
}


// ---------------------------------
static int LoadPackageList()
{
	HWND hCmb = GetDlgItem(g_hDlg, IDC_CMB_PACKAGE);

	for (int i=0; i<MAX_PACKAGES; i++)
	{
		char buffer[64];
		char value[64];
		wsprintf(buffer, "package%d", i);
		DWORD n = GetPrivateProfileStringA("PACKAGES", buffer, "", value, 64, _INI_FILE_);

		if (n==0)
			continue;

		SendMessageA(hCmb, CB_ADDSTRING, 0, (LPARAM)value); 
	}

	return 0;
}

static void xGetXX(const char* p, char* filename, char* path, char* shortcut, int* n)
{
	int i=0;
	char* pIndex=(char*)p;

	filename[0]=0;
	while(pIndex[i]!=',')
	{
		filename[i]=pIndex[i];
		i++;
	}
	
	i++;
	int cnt=0;
	path[0]=0;
	while(pIndex[i]!=',')
	{
		path[cnt]=pIndex[i];
		cnt++;
		i++;
	}

	i++;
	cnt=0;
	shortcut[0]=0;
	while(pIndex[i]!=',')
	{
		shortcut[cnt]=pIndex[i];
		cnt++;
		i++;
	}

	i++;
	sscanf(&pIndex[i], "%d", n);

}

// ---------------------------------
static void LoadSoftwareData(const char* pszSoftName)
{
	GetPrivateProfileStringA(pszSoftName, "SoftwareNameDisplay", "", g_soft.szSoftwareNameDisplay, 32, _INI_FILE_);
	GetPrivateProfileStringA(pszSoftName, "GroupName", "", g_soft.szGroupName, 32, _INI_FILE_);
	GetPrivateProfileStringA(pszSoftName, "GroupNameDisplay", "", g_soft.szGroupNameDisplay, 32, _INI_FILE_);

	SetWindowText(GetDlgItem(g_hDlg, IDC_GROUP_NAME), g_soft.szGroupName);
	SetWindowText(GetDlgItem(g_hDlg, IDC_GROUP_NAME2), g_soft.szGroupNameDisplay);
	SetWindowText(GetDlgItem(g_hDlg, IDC_PACKAGE_NAME), g_soft.szSoftwareNameDisplay);

	char szImageName[128];
	GetPrivateProfileStringA(pszSoftName, "ImageName", "", szImageName, 128, _INI_FILE_);

	SetWindowText(GetDlgItem(g_hDlg, IDC_PROJECT), szImageName);


	HWND hWndList = GetDlgItem(g_hDlg, IDC_LIST_FILE);

	SendMessage(hWndList, LB_RESETCONTENT, 0, 0);
	g_listFile->clear();

	// Load File List.
	char buffer[256];

	for (int i=0; i<MAX_FILECOUNT; i++)
	{
		char key[32];
		char value[512];

		wsprintf(key, "file%d", i);
		if (GetPrivateProfileStringA(g_soft.szSoftwareName, key, "", value, 512, _INI_FILE_)==0)
		{
			continue;
		}

		syiInstallFormatEx* p = (syiInstallFormatEx*)yialloci(sizeof(syiInstallFormatEx));

		xGetXX(value, p->szFullPath, p->szPath, p->szShortCutName, (int*)&p->flag32);
		//sscanf(value, "%s,%s,%d", p->szFullPath, p->szPath, &p->flag32);
		strcpy(p->szFileName, yiPathGetFileName(p->szFullPath));

		
		//strcpy(p->szFileName, p->szFullPath);
		p->p=yiFileLoad(p->szFullPath, (int*)&p->size);

		g_listFile->addref(p);
		SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)p->szFileName);
	}

}

// ---------------------------------
static void MakeSoftwareHeader()
{
	int nBlankId=-1;
	Bool bIsFound=False;

	GetWindowText(GetDlgItem(g_hDlg, IDC_CMB_PACKAGE), g_soft.szSoftwareName, 32);
	GetWindowText(GetDlgItem(g_hDlg, IDC_GROUP_NAME), g_soft.szGroupName, 32);
	GetWindowText(GetDlgItem(g_hDlg, IDC_GROUP_NAME2), g_soft.szGroupNameDisplay, 32);
	GetWindowText(GetDlgItem(g_hDlg, IDC_PACKAGE_NAME), g_soft.szSoftwareNameDisplay, 32);

	for (int i=0; i<MAX_PACKAGES; i++)
	{
		char buffer[64];
		char value[64];
		wsprintf(buffer, "package%d", i);
		DWORD n = GetPrivateProfileStringA("PACKAGES", buffer, "", value, 64, _INI_FILE_);

		if (n==0)
		{
			if (nBlankId==-1)
				nBlankId=i;
			continue;
		}

		if (strcmp(value, g_soft.szSoftwareName)==0)
		{
			bIsFound=True;
			break;
		}
	}

	if (nBlankId==-1)
	{
		return ;
	}

	if (bIsFound==False)
	{
		char buffer[64];
		wsprintf(buffer, "package%d", nBlankId);
		WritePrivateProfileStringA("PACKAGES", buffer, g_soft.szSoftwareName, _INI_FILE_);
	}

	WritePrivateProfileStringA(g_soft.szSoftwareName, "SoftwareNameDisplay", g_soft.szSoftwareNameDisplay, _INI_FILE_);
	WritePrivateProfileStringA(g_soft.szSoftwareName, "GroupName", g_soft.szGroupName, _INI_FILE_);
	WritePrivateProfileStringA(g_soft.szSoftwareName, "GroupNameDisplay", g_soft.szGroupNameDisplay, _INI_FILE_);

}
