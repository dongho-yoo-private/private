// ctrlTest.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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

// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
char szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

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
	// Blure�̂��������C���[�W
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

	Bitmap* string = cyiShape::BitmapString(0, L"�ۑ�", &Font(L"���C���I", 12.0), &SolidBrush(Color::Black));//, eShapeTextStyle::eSTS_DROP_SHADOW);
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

	Bitmap* string = cyiShape::BitmapString(0, L"�΋Ǐ��", &Font(L"���C���I", 14.0), &SolidBrush(Color::Black), eShapeTextStyle::eSTS_DROP_SHADOW);

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
// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
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

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;
	MainWindow wnd;

	// �O���[�o������������������Ă��܂��B
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

	// �A�v���P�[�V�����̏����������s���܂�:
	//if (!InitInstance (hInstance, nCmdShow))
	//{
	//	return FALSE;
	//}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CTRLTEST));

	// ���C�� ���b�Z�[�W ���[�v:
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
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
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
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   HWND hWnd;
//
//   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B
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
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
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
		// �I�����ꂽ���j���[�̉��:
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
		//	panelH->SetItem(1025, 0, L"�e�X�g");
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
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...

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

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���ł��B
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
