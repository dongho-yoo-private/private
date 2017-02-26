// GoYotsuya.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "yistd.h"
#include "stdafx.h"
#include "GoYotsuya.h"

#define MAX_LOADSTRING 100

Image* m_bg;

GdiplusStartupInput __g_gdiSI__;
ULONG_PTR           __g_gdiToken__;

// ------------------------------------------
extern HWND xCreateRemocon(HWND hWnd);

// ----------------------------------
extern HWND xCreateJosekiControl(HWND hWnd, int x, int y);


HWND g_hRemocon=0;
HWND g_hJosekiControl=0;

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

#include "AppGoYotsuya.h"
// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
HWND				InitInstance(HINSTANCE, int);
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
	HWND hWnd;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GOYOTSUYA, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	GdiplusStartup(&__g_gdiToken__, &__g_gdiSI__, NULL);

	cAppGoYotsuya::GetInstance().Initialize();

	// 背景イメージロード
	//m_bg = new Image(L"..\\Data\\bg.jpg");
	//int w=m_bg->GetWidth();
	//int h = m_bg->GetHeight();

	// アプリケーションの初期化を実行します:
	if (!(hWnd=InitInstance(hInstance, nCmdShow)))
	{
		return FALSE;
	}

	if (strcmp(lpCmdLine, "ROTATE")==0)
	{
		__xxSetRotateModeEnable();
	}

	//if (strcmp(lpCmdLine, "USE_REMOCON")==0)
	//{
	//	g_hRemocon = xCreateRemocon(hWnd);
	//	cAppGoYotsuya::GetInstance().m_hControlWnd = g_hRemocon;
	//}
	//else if (strcmp(lpCmdLine, "JOSEKI_EDITOR")==0)
	//{
	cAppGoYotsuya& app = cAppGoYotsuya::GetInstance();

	int w = cAppGoYotsuya::GetInstance().m_goban->GetSize();
	
	g_hRemocon = xCreateJosekiControl(hWnd, w, 0);
	app.SetMode(::eAM_JOSEKI_EDITOR);
	app.m_goban->SetBrancheViewMode(eBVM_SHOW_ONE_NODE);
	goid_t id = app.m_goban->AddForEditByFile(".\\data\\joseki.dat", eGoFileType::eGFT_SGF);

	if (id)
	{
		char szBackupFileName[256];
		SYSTEMTIME time;
		GetLocalTime(&time);
		yiMakeDir(".\\data\\backups");
		wsprintf(szBackupFileName, ".\\data\\backups\\Joseki.dat.%04d.%02d.%02d.%02d.%02d.%02d", 
			(int)time.wYear, (int)time.wMonth, (int)time.wDay,
				(int)time.wHour, (int)time.wMinute, (int)time.wSecond);
		CopyFile(".\\data\\joseki.dat", szBackupFileName, False);
	}

	app.m_goban->SearchResult();
	app.m_hControlWnd = g_hRemocon;

	RECT rect;
	RECT rectp;

	GetWindowRect(g_hRemocon, &rect);

	SetWindowPos(hWnd, 0, 0, 0, rect.right+10, (rect.bottom-rect.top)+50 , SWP_NOZORDER|SWP_NOMOVE);



	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GOYOTSUYA));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	GdiplusShutdown(__g_gdiToken__);
	return (int) msg.wParam;
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GOYOTSUYA));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GOYOTSUYA);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

#define WS_OVERLAPPEDWINDOWX (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MAXIMIZEBOX)
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
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(szWindowClass, szTitle, WS_CLIPCHILDREN|/*WS_CLIPSIBLINGS|*/WS_OVERLAPPEDWINDOWX,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return 0;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
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
	case WM_CREATE:
	{
		cAppGoYotsuya::GetInstance().OnCreate(hWnd);
		break;
	}
	case WM_GOBAN_ADDED:
	{
		const char* pszTitle = cAppGoYotsuya::GetInstance().m_goban->GetTitle();
		SetWindowTextA(hWnd, pszTitle); 
		break;
	}
	case WM_SIZE:
	{
		cAppGoYotsuya& app = cAppGoYotsuya::GetInstance();
		RECT rect;
		GetClientRect(hWnd, &rect);
		app.m_goban->Resize(rect.right, rect.bottom);
		int w = app.m_goban->GetSize();
		SetWindowPos(app.m_hControlWnd, 0, w, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);

		break;
	}
	case WM_PUT_STONED:
	{
		cAppGoYotsuya::GetInstance().OnPutStoned(wParam, lParam);
		break;
	}
	case WM_PREVIEW_SELECTED:
	{
		cAppGoYotsuya::GetInstance().OnPreViewSelected(wParam, lParam);
		break;
	}
	case WM_PREVIEW_PLAY:
	{
		cAppGoYotsuya::GetInstance().OnPreViewSelected(-1, lParam);
		break;
	}
	case WM_QUIT_OPTIMIZE:
	{
		char buffer[128];
		wsprintf(buffer, "%d件の同一型を統合しました。", wParam);
		cAppGoYotsuya::GetInstance().m_ProgressDlg.SetProgressOK(buffer);
		break;
	}
	case WM_QUIT_SEARCHRESULT:
	{
		cAppGoYotsuya& app = cAppGoYotsuya::GetInstance();
		app.m_prev->UpdateTree((cyiTreeNode*)lParam);
		break;
	}
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
	// 廃止
	//case WM_COMMENT_CHANGED:
	//{
	//	eAppMode mode = cAppGoYotsuya::GetInstance().GetMode();

	//	if (mode==eAM_REMOCON)
	//	{
	//		SetDlgItemText(g_hRemocon, IDC_COMMENT, (LPCSTR)(lParam==0?(LPARAM)"":lParam));
	//	}
	//	else if (mode==eAM_JOSEKI_EDITOR)
	//	{
	//		SetDlgItemText(g_hJosekiControl, IDC_COMMENT, (LPCSTR)(lParam==0?(LPARAM)"":lParam));
	//	}
	//	break;
	//}
	case WM_NODE_CHANGED:
	{
		cAppGoYotsuya::GetInstance().OnNodeChanged((cyiTreeNode*)lParam, wParam);
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		Graphics g(hdc);

		//g.DrawImage(m_bg, 0, 0, m_bg->GetWidth(), m_bg->GetHeight());
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_CLOSE:
	{
		if (cAppGoYotsuya::GetInstance().GetMode()==eAppMode::eAM_JOSEKI_EDITOR)
		{
			if (MessageBox(hWnd, "変更を保存しますか？", "お知らせ", MB_YESNO|MB_ICONINFORMATION)==IDYES)
			{
				cAppGoYotsuya::GetInstance().m_goban->Save(0, eGoFileType::eGFT_SGF);
			}
		}
		break;
	}
	case WM_DESTROY:
	{

		cAppGoYotsuya::GetInstance().m_goban->Destroy();
		delete cAppGoYotsuya::GetInstance().m_goban;
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
	//return 0;
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
