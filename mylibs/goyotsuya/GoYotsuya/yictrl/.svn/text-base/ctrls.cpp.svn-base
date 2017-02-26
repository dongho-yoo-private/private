// ctrls.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ctrls.h"
#include "DrawObject.h"
#include "cDrawObjectButton.h"
#include "yiResourceManager.h"
#include "yctrlTab.h"

#define MAX_LOADSTRING 100

GdiplusStartupInput m_gdiSI;
ULONG_PTR           m_gdiToken;

LRESULT CALLBACK ChildDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CTRLS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	GdiplusStartup(&m_gdiToken, &m_gdiSI, NULL);


	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CTRLS));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
		GdiplusShutdown(m_gdiToken);

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CTRLS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CTRLS);
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

#define REGISTER_TEXT_RESOURCE(hInstance) RegisterTextResources(hInstance)

#define BEGIN_RESOURCE_TEXT(min, max) \
void RegisterTextResources(HINSTANCE hInstance)\
{\
yiResourceManager* mgr = yiResourceManager::GetInstance();\
mgr->DefineResource(eRT_TEXT, eRS_ARRAY, min, max);

#define TEXT_RESOURCE(id)\
{\
char* str = (char*)yiResourceManager::LoadString(hInstance, id);\
mgr->Add(eRT_TEXT, id, str);\
}

//HRSRC hRes = ::FindResource((HMODULE)hInstance, MAKEINTRESOURCE(id), RT_STRING);\
//HGLOBAL hg = ::LoadResource((HMODULE)hInstance, hRes);\
//void* p = ::LockResource(hg);\


#define END_RESOURCE_TEXT \
}

#define BEGIN_RESOURCE_DRAW_OBJECT(min, max) \
void RegisterDrawObjectResource(HINSTANCE hInstance)\
{\
yiResourceManager* mgr = yiResourceManager::GetInstance();\
mgr->DefineResource(RT_DRAW_OBJECT, eRS_ARRAY, min, max);

#define IMAGE_OBJECT_RESOURCE(id1, type, sclaed) \
{\
ImageObject* i = new ImageObject();\
i->Create(id1, type, hInstance, scaled);\
mgr->Add(RT_DRAW_OBJECT, IDS_STRING104);\
}

#define TEXT_OBJECT_RESOURCE(id1, type, sclaed) \
{\
TextObject* i = new ImageObject();\
i->Create(id1, type, hInstance, scaled);\
mgr->Add(RT_DRAW_OBJECT, IDS_STRING104);\
}


#define END_RESOURCE \
}

BEGIN_RESOURCE_TEXT(IDS_STRING104, 100)
	TEXT_RESOURCE(IDS_STRING104)
	TEXT_RESOURCE(IDS_STRING105)
	TEXT_RESOURCE(IDS_STRING106)
END_RESOURCE_TEXT

cyctrlTab* tabctrl;
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
		case 120:
			{
				tabctrl->Add(NULL, "Buttonxxxxxx", RGB(127, 127, 127)|0xFF000000);
			break;
			}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		//ImageObject img;

		Graphics g(hdc);

		g.SetSmoothingMode(SmoothingModeHighQuality);

		//BOOL res = img.Create("c:\\logo.gif", TRUE);

		//img.Draw(&g, 100, 100);
		//img.SetScale(0.5f);
		//img.Draw(&g, 100, 200);

		//for (int i=0; i<3; i++)
		//{
		//	char* pStr = (char*)yiResourceManager::GetInstance()->GetText(IDS_STRING104+i);
		//
		//	TextObject txt;
		//	txt.Create(pStr, L"メイリオ", 12, Color( 0,0, 0), TRUE);
		//	txt.Draw(&g, 100, 300+i*20);
		//}
		EndPaint(hWnd, &ps);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
	{
//		HWND hWndDlg = ::CreateDialogW(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ChildDlg);

		REGISTER_TEXT_RESOURCE(hInst);
		cyctrlButton* btn = new cyctrlButton();
		btn->Create(hWnd, 120, 0, 0, 100, 30, 
			new ImageObject("c:\\logo.gif"), new ImageObject("c:\\a.png"), new ImageObject("c:\\c.PNG"));

		cyctrlButton* btn2 = new cyctrlButton();
		btn2->Create(hWnd, 121, 0, 0, 100, 30, 
				new ImageObject("c:\\a.png"), new ImageObject("c:\\logo.gif"), new ImageObject("c:\\c.PNG"));

		cyctrlButton* btn3 = new cyctrlButton();
		btn3->Create(hWnd, 122, 0, 0, 100, 30, 
				new ImageObject("c:\\c.PNG"), new ImageObject("c:\\logo.gif"), new ImageObject("c:\\a.png"));

		tabctrl = new cyctrlTab();
		
		tabctrl->Add(btn, "Button1 TextBox", RGB(255, 0, 0)|0xFF000000);
		tabctrl->Add(btn2, "Button2", RGB(0, 255, 0)|0xFF000000);
		tabctrl->Add(btn3, "Button3", RGB(0, 0, 255)|0xFF000000);
		tabctrl->Add(btn, "Button4", RGB(0, 128, 128)|0xFF000000);
		tabctrl->Add(btn2, "Button5", RGB(128, 0, 255)|0xFF000000);


		tabctrl->Create(hWnd, 125, 200, 0, 20, 500, 300, 30, eTabDirection::eTD_VERTICAL, TRUE);

		
		break;
	}
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

// バージョン情報ボックスのメッセージ ハンドラです。
LRESULT CALLBACK ChildDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (LRESULT)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (LRESULT)TRUE;
		}
		break;
	}
	return DefDlgProc(hDlg, message, wParam, lParam);
}
