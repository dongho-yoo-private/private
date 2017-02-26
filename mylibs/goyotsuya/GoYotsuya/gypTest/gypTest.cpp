// gypTest.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "yi.h"
#include "yistd.h"
#include "stdafx.h"
#include "gypTest.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#include "glsGobanContainer.h"
cglsGobanContainer* m_goban=0;

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

	//yictrl_initialize();

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GYPTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	//xTRACE("test");
	//cyiList* list = new cyiList();
	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GYPTEST));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	//m_goban->Destroy();
	//delete m_goban;
	yictrl_finalize();

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GYPTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GYPTEST);
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

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW|WS_VISIBLE,
      CW_USEDEFAULT, 0, 300, 400, NULL, NULL, hInstance, NULL);

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
//const CLSID CLSID_GYP = {0x112143a6, 0x62c1, 0x4478, {0x9e, 0x8f, 0x87, 0x26, 0x99, 0x25, 0x5e, 0x2e}};
static const CLSID  CLSID_GYP = { 0xe64b778, 0x416d, 0x458b, { 0xb7, 0x77, 0x70, 0xe, 0xba, 0x8d, 0x4, 0xc7 } };
//const CLSID IID_IPreviewHandler = {0x8895b1c6, 0xb41f, 0x4c1c, {0xa5, 0x62, 0x0d, 0x56, 0x42, 0x50, 0x83, 0x6f}};
#include <atlbase.h>
#include <Shlwapi.h>
#include <shlobj.h>
#include "gyPreview.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);

		ShowWindow(hWnd, SW_SHOW);

		//m_goban=new cglsGobanContainer();
		//m_goban->Create(hInst, 0, 500, 0, 300, 500, (HICON)hWnd, (HICON)5, &Font(L"Meiryo UI", 11.0), 0, 0, WS_CHILD|WS_VISIBLE);

		IPreviewHandler* ix=0;
		IStream* stream=0;
		HRESULT hr = SHCreateStreamOnFileA("c:\\yotsuya\\xxx.sgf", STGM_READ, &stream);

		if (FAILED(hr))
		{
			break;
		}

		//if (m_goban->SetStream(stream)==False)
		//{
		//	stream->Release();
		//	break;
		//}
		//stream->Release();

		//m_goban->ReSize(500, 600, False);


		::CoInitialize(0);
		if (CoCreateInstance(CLSID_GYP, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IPreviewHandler, (void**)&ix)!=S_OK)
		{
			break;
		}
		
		//ix->AddRef();

		IInitializeWithStream* iStream=0;
		if (ix->QueryInterface(IID_IInitializeWithStream, (void**)&iStream)!=S_OK)
		{
			break;
		}
		
		LARGE_INTEGER integer;
		memset(&integer, 0, sizeof(integer));

		stream->Seek(integer, STREAM_SEEK_SET, 0);
		if (iStream->Initialize(stream, STGM_READ)!=S_OK)
		{
			break;
		}



		if (ix->SetWindow(hWnd, &rect)!=S_OK)
		{
			break;
		}

		if (ix->DoPreview()!=S_OK)
		{
			break;
		}

		RECT xrect = {0, 0, 500, 800};
		ix->SetRect(&xrect);

		if (ix->Unload()!=S_OK)
		{
			break;
		}


		//ix->Release();

		memset(&integer, 0, sizeof(integer));

		stream->Seek(integer, STREAM_SEEK_SET, 0);
		if (iStream->Initialize(stream, STGM_READ)!=S_OK)
		{
			break;
		}
		
		ix->SetWindow(hWnd, &rect);

		ix->DoPreview();

		ix->SetFocus();

		stream->Release();

		m_goban->ReSize(300, 500);
		

		//m_goban->InvalidateRect(0, 0, False);

		//m_goban->ReSize(200, 200);
		//::CoInitialize(0);

		//::CoCreateInstance(
		break;
	}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			IStream* stream=0;
			HRESULT hr = SHCreateStreamOnFileA("c:\\yotsuya\\xxx.sgf", STGM_READ, &stream);

			if (FAILED(hr))
			{
				break;
			}

			if (m_goban->SetStream(stream)==False)
			{
				stream->Release();
				break;
			}
			stream->Release();

			break;
		}
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SIZE:
	{
		if (m_goban)
		{
			RECT rect;
			::GetClientRect(hWnd, &rect);
			m_goban->ReSize(rect.right, rect.bottom);
		}
		break;
	}
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
