#include "StdAfx.h"
#include "MyMainFrame.h"

// ---------------------------------------
static ATOM xMyRegisterClass(HINSTANCE hInstance, const char* pszClassName);

// ---------------------------------------
static HWND xInitInstance(HINSTANCE hInstance, const char* pszClassName, const char* pszTitle, int nCmdShow, void* param);

// ---------------------------------------
cMyMainFrame::cMyMainFrame()
{
}

// ---------------------------------------
cMyMainFrame::~cMyMainFrame()
{
}

// ---------------------------------------
Bool cMyMainFrame::Create(HINSTANCE hInstance, const char* pszClassName, const char* pszTitle)
{
	if (xMyRegisterClass(hInstance, pszClassName)==0)
	{
		return False;
	}

	m_hWnd = xInitInstance(hInstance, pszClassName, pszTitle, SW_SHOW, this);
	m_bIsCreated=True;

	return m_hWnd ? True:False;
}



// ---------------------------------------
BOOL cMyMainFrame::OnPaint(HDC hdc, PAINTSTRUCT* ps/*=0*/)
{
	return False;
}

// ---------------------------------------------------------------
LRESULT cMyMainFrame::OnCreate(WPARAM wp, LPARAM lp)
{
	return 0;
}

// ------------------------------------
LRESULT cMyMainFrame::OnDestroy(WPARAM wp, LPARAM lp)
{
	PostQuitMessage(0);
	return 0;
}

// ---------------------------------------------------------------
LRESULT cMyMainFrame::OnAbout(HWND hWnd, int code)
{
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE);

	cyctrlbase m_dlg;
	m_dlg.CreateFromReosurce(m_hWnd, hInst, IDD_ABOUTBOX, True);
	return 0;
}

// ---------------------------------------------------------------
LRESULT cMyMainFrame::OnExit(HWND hWnd, int code)
{
	DestroyWindow(m_hWnd);
	return 0;
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
static ATOM xMyRegisterClass(HINSTANCE hInstance, const char* pszClassName)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, pszClassName);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_YIGEDITOR);
	wcex.lpszClassName	= pszClassName;
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
static HWND xInitInstance(HINSTANCE hInstance, const char* pszClassName, const char* psztitle, int nCmdShow, void* param)
{
   HWND hWnd;

   hWnd = CreateWindow(pszClassName, psztitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, param);

   if (!hWnd)
   {
      return 0;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
}

