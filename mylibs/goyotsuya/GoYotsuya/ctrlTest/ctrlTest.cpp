// ctrlTest.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "yistd.h"
//#include "yiBitmapBlender.h"
//#include "stdafx.h"
#include "ctrlTest.h"
#include "glsStackPannel.h"
#include "ButtonSet.h"

#include <atlbase.h>
#include <gdiplus.h>
#include <windows.h>

#ifndef __GDI_PLUS__
using namespace Gdiplus;
#pragma comment(lib, "GdiPlus.lib") 
#define __GID_PLUS__
#endif

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
char szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

GdiplusStartupInput __g_gdiSI__;
ULONG_PTR           __g_gdiToken__;
Image* m_bg;
Image* m_pt;
Bitmap* m_ptb;
int m_ptx;
int m_pty;
Bitmap* m_ptSample;
Bitmap* m_bgBlure;
Bitmap* m_pannel;
Bitmap* m_blackpannel;

#define xARGB(a, r, g, b) ((a<<24)|(r<<16)|(g<<8)|b)

// ---------------------------
Bitmap* xGetBg(Bitmap* bg, int x, int y, int w, int h)
{
	Bitmap* bmp = new Bitmap(w, h, PixelFormat24bppRGB);
	Bitmap* bmpTemp;
	Graphics g(bmp);
	//g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	// Blureのかかったイメージ
	int bgw=bg->GetWidth();
	int bgh=bg->GetHeight();
	int xStartPos = x%bgw;
	int yStartPos = y&bgh;
	int wCnt=(xStartPos+w)/bgw;
	int hCnt=(yStartPos+h)/bgh;

	bmpTemp = new Bitmap(w+xStartPos, h+yStartPos, PixelFormat32bppPARGB);
	Graphics gTemp(bmpTemp);

	gTemp.SetSmoothingMode(SmoothingMode::SmoothingModeHighSpeed);
	gTemp.FillRectangle(&TextureBrush(bg, WrapMode::WrapModeTile), Rect(0, 0, w+xStartPos, h+yStartPos));

	//int xx=0; 
	//int yy=0;
	//for (int i=0; i<hCnt+1; i++)
	//{
	//	xx=0;
	//	for (int j=0; j<wCnt+1; j++)
	//	{
	//		gTemp.DrawImage(bg, xx, yy);
	//		xx+=bgw;
	//	}
	//	yy+=bgh;
	//}

	g.DrawImage(bmpTemp, 0, 0, xStartPos, yStartPos, w, h, Unit::UnitPixel);

	delete bmpTemp;

	return bmp;
}


// ---------------------------
Bitmap* xMakePannel(Bitmap* bg, int x, int y, int w, int h)
{
	
	Bitmap* bmpFrame = cyiShape::DrawRoundRect3D(xARGB(128, 200,200,200), xARGB(128, 0, 0, 0), RectF(0, 0, w, h), 2.0);
	Bitmap* bmpPannel = cyiShape::FillRoundRect(&LinearGradientBrush(PointF(0, 0), PointF(0, h), Color(0, 0,0,0), Color(64, 255,255,0)), RectF(0, 0, w, h), 2.0);
	Bitmap* bmpMask = cyiShape::FillRoundRect(&SolidBrush(Color(255, 0,0, 0)), RectF(0, 0, w, h), 2.0);
	//Bitmap* bg = new Bitmap(w, h);
	HandleShadow hShadow = cyiShape::CreateShadow(4, 2.0, 0.5);
	Bitmap* shadow = cyiShape::MakeShadowBitmap(hShadow, w+8, h+8);
	Graphics g(bg);

	//g.DrawImage(shadow, 0, 0);
	//g.DrawImage(bmp, 0, 0, x, y, w, h, Unit::UnitPixel);
	g.DrawImage(bmpPannel, 0, 0);
	g.DrawImage(bmpFrame, 0, 0);

	bg = cyiBitmapBlender::SetMask(bg, bmpMask, 0, 0, False);


	//cyiShapeMask mask(0, 0, bmpMask);
	//Bitmap* img = cyiBitmapBlender::DropShadow(bg, 1, 1, 5.0, 0.3, 0);//&mask);

	Graphics gShadow(shadow);
	gShadow.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	Bitmap* string = cyiShape::BitmapString(0, L"保存", &Font(L"メイリオ", 12.0), &SolidBrush(Color::Black));//, eShapeTextStyle::eSTS_DROP_SHADOW);
	g.DrawImage(string, (INT)(w-string->GetWidth())/2, (INT)(h-string->GetHeight())/2);

	gShadow.DrawImage(bg, 3, 3);
	cyiShape::DestroyShadow(hShadow);



	//delete img;
	delete bmpFrame;
	delete bmpPannel;
	delete bmpMask;
	return shadow;
}

// ---------------------------
Bitmap* xMakeBlackPannel(Bitmap* bg, int x, int y, int w, int h)
{
	
	Bitmap* bmpFrame = cyiShape::DrawRoundRect3D(xARGB(128, 255,255,255), xARGB(128, 0, 0, 0), RectF(0, 0, w, h), 2.0);
	Bitmap* bmpPannel = cyiShape::FillRoundRect(&LinearGradientBrush(PointF(0, 0), PointF(0, h), Color(0, 0,0,0), Color(64, 0,0,0)), RectF(0, 0, w, h), 2.0);
	Bitmap* bmpMask = cyiShape::FillRoundRect(&SolidBrush(Color(255, 0,0, 0)), RectF(0, 0, w, h), 2.0);
	//Bitmap* bg = new Bitmap(w, h);
	HandleShadow hShadow = cyiShape::CreateShadow(8, 2.0, 0.5);
	Bitmap* shadow = cyiShape::MakeShadowBitmap(hShadow, w+16, h+16);
	Graphics g(bg);

	//g.DrawImage(shadow, 0, 0);
	//g.DrawImage(bmp, 0, 0, x, y, w, h, Unit::UnitPixel);
	g.DrawImage(bmpPannel, 0, 0);
	g.DrawImage(bmpFrame, 0, 0);

	bg = cyiBitmapBlender::SetMask(bg, bmpMask, 0, 0, False);


	//cyiShapeMask mask(0, 0, bmpMask);
	//Bitmap* img = cyiBitmapBlender::DropShadow(bg, 1, 1, 5.0, 0.3, 0);//&mask);

	Graphics gShadow(shadow);
	gShadow.DrawImage(bg, 5, 5);
	cyiShape::DestroyShadow(hShadow);

	Bitmap* string = cyiShape::BitmapString(0, L"対局情報", &Font(L"メイリオ", 14.0), &SolidBrush(Color::Black), eShapeTextStyle::eSTS_DROP_SHADOW);

	gShadow.DrawImage(string, (INT)(w-string->GetWidth())/2, (INT)(h-string->GetHeight())/2);

	//delete img;
	delete bmpFrame;
	delete bmpPannel;
	delete bmpMask;
	return shadow;
}

Bitmap* xMakePannelII(int x, int y, int w, int h, color_t buttoncolor)
{
	Bitmap* bmp = new Bitmap(w, h, PixelFormat32bppARGB);
	Graphics g(bmp);

	//Bitmap* bmpFrame = cyiShape::DrawRoundRect(

	return bmp;
}

//static Bitmap* xMakeButton(Bitmap* bmp, color_t pannel_color, UINT w, UINT h)
//{
//	Bitmap* bmp = new Bitmap(w, h, PixelFormat32bppPARGB);
//
//
//}

// ---------------------------
//Bitmap* xMakeBlackPannel(Bitmap* bmp, int x, int y, int w, int h)
//{
//	
//	Bitmap* bmpFrame = cyiShape::DrawRoundRect3D(xARGB(128, 200,200,200), xARGB(128, 0, 0, 0), RectF(0, 0, w, h), 2.0);
//	Bitmap* bmpPannel = cyiShape::FillRoundRect(&LinearGradientBrush(PointF(0, 0), PointF(0, h), Color(64, 0,0,0), Color(127, 0,0,0)), RectF(0, 0, w, h), 2.0);
//	Bitmap* bmpMask = cyiShape::FillRoundRect(&SolidBrush(Color(255, 0,0, 0)), RectF(0, 0, w, h), 2.0);
//	Bitmap* bg = new Bitmap(w, h);
//	//HandleShadow hShadow = cyiShape::CreateShadow(8, 2.0, 0.5);
//	//Bitmap* shadow = cyiShape::MakeShadowBitmap(hShadow, w, h);
//	Graphics g(bg);
//
//	//g.DrawImage(shadow, 0, 0);
//	g.DrawImage(bmp, 0, 0, x, y, w, h, Unit::UnitPixel);
//	g.DrawImage(bmpPannel, 0, 0);
//	g.DrawImage(bmpFrame, 0, 0);
//
//	bg = cyiBitmapBlender::SetMask(bg, bmpMask, 0, 0, False);
//
//	cyiShapeMask mask(0, 0, bmpMask);
//	Bitmap* img = cyiBitmapBlender::DropShadow(bg, 1, 1, 5.0, 0.3, 0);//&mask);
//
//	//Graphics gShadow(shadow);
//	//gShadow.DrawImage(bg, 4, 4);
//	//cyiShape::DestroyShadow(hShadow);
//
//	delete bg;
//	delete bmpFrame;
//	delete bmpPannel;
//	delete bmpMask;
//	return img;
//}

Bitmap* m_xxxx;

#include "MainWindow.h"
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
	MainWindow wnd;

	// グローバル文字列を初期化しています。
	LoadStringA(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CTRLTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	GdiplusStartup(&__g_gdiToken__, &__g_gdiSI__, NULL);

	//if (wnd.Create(hInstance, szTitle, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
	//	LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CTRLTEST)),
	//	LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)),
	//	new Font(L"Meiryo", 11.0),
	//	IDC_CTRLTEST)==False)
	//{
	if (wnd.Create(hInstance, szTitle, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CTRLTEST)),
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)),
		new Font(L"Meiryo", 11.0),
		0)==False)
	{
		return False;
	}

	// アプリケーションの初期化を実行します:
	//if (!InitInstance (hInstance, nCmdShow))
	//{
	//	return FALSE;
	//}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CTRLTEST));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//GdiplusShutdown(__g_gdiToken__);
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CTRLTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CTRLTEST);
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
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   HWND hWnd;
//
//   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。
//
//   hWnd = ::CreateWindowEx(0/*WS_EX_TOOLWINDOW*/, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}



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
		//case 1024:
		//case 1025:
		//case 1026:
		//{
		//	wchar_t szMessage[256];
		//	wsprintfW(szMessage, L"Button %d Clicked", wmId);
		//	//MessageBoxA(hWnd, szMessage, "information", MB_OK|MB_ICONINFORMATION);
		//	//btn.SetItem(0, szMessage, True);
		//	break;
		//}
		//case 1029:
		//{
		//	panelH->SetItem(1025, 0, L"テスト");
		//	break;
		//}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		//break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		Graphics g(hdc);

		g.FillRectangle(&SolidBrush(Color::Yellow), ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.left+ps.rcPaint.right, ps.rcPaint.top+ps.rcPaint.bottom);
		// TODO: 描画コードをここに追加してください...

		//Image *img = new Image(L"c:\\vfrwin\\bg.jpg");
		//Bitmap* b=cyiBitmapBlender::GaussBlureXX((Bitmap*)img, 5.0, 100, 0);
		//Graphics _g(hdc);

		//_g.DrawImage(b, 0, 0);

		//Bitmap* xx = new Bitmap(ps.rcPaint.right, ps.rcPaint.bottom, PixelFormat32bppPARGB);
		//Graphics g(xx);
		//g.FillRectangle(&TextureBrush(m_pt), 0, 0, ps.rcPaint.right, ps.rcPaint.bottom);
		////g.DrawImage(m_bg, 0, 0, m_bg->GetWidth(), m_bg->GetHeight());
		//g.DrawImage(m_pannel, 100, 300, m_pannel->GetWidth(), m_pannel->GetHeight());
		////g.DrawImage(m_blackpannel, 450, 300, m_blackpannel->GetWidth(), m_blackpannel->GetHeight());

		////g.DrawImage(m_xxxx, 100, 500, m_xxxx->GetWidth(), m_xxxx->GetHeight());

		//int xxxx=0, yyyy=0;

		//Bitmap* x = xGetBg(m_ptb, xxxx, yyyy, 300, 300);

		//g.DrawImage(x, xxxx, yyyy);

		//_g.DrawImage(xx, 0, 0);
		
		EndPaint(hWnd, &ps);
		break;
	}
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
