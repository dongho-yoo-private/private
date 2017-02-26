#include "glsWindow.h"
#include <windows.h>


#include <DWMApi.h>

//#pragma comment(lib, "DWMApi.lib")

// ---------------------------
typedef HRESULT (*lpfnDwmEnableBlurBehindWindow)(HWND hWnd, const DWM_BLURBEHIND* data);

// ---------------------------
typedef HRESULT (*lpfnDwmExtendFrameIntoClientArea)(HWND hWnd, const MARGINS* margin);

typedef HRESULT (*lpfnDwmIsCompositionEnabled)(BOOL* a);


static HRESULT EnableBlurBehind(HWND hwnd)
{
   HRESULT hr = S_OK;

   HMODULE hModule = LoadLibrary("DWMApi.dll");

   if (hModule==0)
   {
	   return S_FALSE;
   }

   lpfnDwmEnableBlurBehindWindow proc = (lpfnDwmEnableBlurBehindWindow)GetProcAddress(hModule, "DwmEnableBlurBehindWindow");


   //Create and populate the BlurBehind structre
   DWM_BLURBEHIND bb = {0};
   //Enable Blur Behind and Blur Region;
   bb.dwFlags = DWM_BB_ENABLE;
   bb.fEnable = true;
   bb.hRgnBlur = NULL;

   //Enable Blur Behind
   hr = proc(hwnd, &bb);
   if (SUCCEEDED(hr))
   {
		MARGINS margin = { -1 };
		lpfnDwmExtendFrameIntoClientArea DwmExtendFrameIntoClientArea = (lpfnDwmExtendFrameIntoClientArea)GetProcAddress(hModule, "DwmExtendFrameIntoClientArea");
		DwmExtendFrameIntoClientArea( hwnd, &margin );

      //do more things
   }

   return hr;
}


cglsWindow::cglsWindow(void)
:m_bg(0)
, m_bIsModal(0)
, m_bIsPopup(0)
, m_bIsExitLoop(0)
, m_bHasPopup(False)
, m_bIsExitCode(0)
, m_exceptionObject(0)
{
}

cglsWindow::~cglsWindow(void)
{
}

//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//
//	switch (message)
//	{
//	case WM_COMMAND:
//		wmId    = LOWORD(wParam);
//		wmEvent = HIWORD(wParam);
//		// 選択されたメニューの解析:
//		switch (wmId)
//		{
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//		break;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		// TODO: 描画コードをここに追加してください...
//		EndPaint(hWnd, &ps);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}

extern HINSTANCE g_hInstance;

// ---------------------------------------------------------
Bool cglsWindow::CreateForAero(HINSTANCE hInstance, char* pszTitle, int x, int y, int w, int h, HICON hIcon, HICON hIconSmall, Font* font, int menuId/*=0*/, DWORD styleEx/*=0*/, DWORD style/*=0*/)
{
	// バージョンチェック
	if (yiIsOverXP()==False)
	{
		return Create(hInstance, pszTitle, x, y, w, h, hIcon, hIconSmall, font, menuId/*=0*/, styleEx/*=0*/, style/*=0*/);
	}

	m_DrawObject=0;
	WNDCLASSEX wcex;

	m_hInstance=hInstance;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= (style&WS_CHILD)?0:hIcon;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)CreateSolidBrush(0xff000000);
	wcex.lpszMenuName	= MAKEINTRESOURCE(menuId);
	wcex.lpszClassName	= "GLS_WINDOW_FORAERO";
	wcex.hIconSm		= hIconSmall;

	RegisterClassEx(&wcex);

	m_parent=0;
	m_hParentWnd=0;

	sGlsDetail xdetail;
	cGlassInterface::xDefaultDetail(&xdetail);
	cGlassInterface::InitDetail(xdetail);

	if (font)
		m_font=(Font*)cyiShape::Clone(font);//font->Clone();
	//m_font=font;

	if (m_font==0)
	{
		m_font = new Font(L"Meiryo", 10.5);
	}
	m_nFontHeight = m_font->GetHeight(__DEFAULTGRAPHICS__);
	unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, L"1", m_font, 0, 0);
	m_nFontHeightGDI = (pos&0xFFFF);

	xSET_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW, False);
	xSET_GLS_FLAG(m_flag, eGF_FONT_FROM_PARENT, False);
	xSET_GLS_FLAG(m_flag, eGF_FOCUS_MANAGE, True);

	m_x=x;
	m_y=y;
	m_w=w;
	m_h=h;

	m_hWnd = CreateWindowEx(styleEx, "GLS_WINDOW_FORAERO", pszTitle, style==0?WS_OVERLAPPEDWINDOW:style,
		x, y, w, h, (style&WS_CHILD)?(HWND)hIcon:0, (style&WS_CHILD)?(HMENU)hIconSmall:0, hInstance, this);

	if (m_hWnd==0)
		return False;

   HMODULE hModule = LoadLibrary("DWMApi.dll");

   if (hModule==0)
   {
	   return False;
   }

	BOOL dwmEnable;
	lpfnDwmIsCompositionEnabled DwmIsCompositionEnabled=(lpfnDwmIsCompositionEnabled)GetProcAddress(hModule, "DwmIsCompositionEnabled");

	if (DwmIsCompositionEnabled)
		DwmIsCompositionEnabled (&dwmEnable); 
	if (dwmEnable) EnableBlurBehind(m_hWnd);

	xSET_GLS_FLAG(m_flag, eGF_BUFFERED_PAINT ,True);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	::InvalidateRect(m_hWnd, 0, FALSE);
	UpdateWindow(m_hWnd);
	return True;
}

// -------------------------------------
Bool cglsWindow::Create(HINSTANCE hInstance, char* pszTitle, int x, int y, int w, int h, HICON hIcon, HICON hIconSmall, Font* font, int menuId/*=0*/, DWORD styleEx/*=0*/, DWORD style/*=0*/, Bool bHide/*=False*/)
{
	m_DrawObject=0;
	WNDCLASSEX wcex;

	m_hInstance=hInstance;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= (style&WS_CHILD)?0:hIcon;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(GetStockObject(NULL_BRUSH));
	wcex.lpszMenuName	= MAKEINTRESOURCE(menuId);
	wcex.lpszClassName	= "GLS_WINDOW";
	wcex.hIconSm		= hIconSmall;

	RegisterClassEx(&wcex);

	m_parent=0;
	m_hParentWnd=0;

	sGlsDetail xdetail;
	cGlassInterface::xDefaultDetail(&xdetail);
	cGlassInterface::InitDetail(xdetail);

	if (font)
		m_font=(Font*)cyiShape::Clone(font);//font->Clone();
	//m_font=font;

	if (m_font==0)
	{
		m_font = new Font(L"Meiryo", 10.5);
	}
	m_nFontHeight = m_font->GetHeight(__DEFAULTGRAPHICS__);
	unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, L"1", m_font, 0, 0);
	m_nFontHeightGDI = (pos&0xFFFF);

	xSET_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW, False);
	xSET_GLS_FLAG(m_flag, eGF_FONT_FROM_PARENT, False);
	xSET_GLS_FLAG(m_flag, eGF_FOCUS_MANAGE, True);

	m_x=x;
	m_y=y;
	m_w=w;
	m_h=h;

	m_hWnd = CreateWindowEx(styleEx, "GLS_WINDOW", pszTitle, style==0?WS_OVERLAPPEDWINDOW:style,
		x, y, w, h, (style&WS_CHILD)?(HWND)hIcon:0, (style&WS_CHILD)?(HMENU)hIconSmall:0, hInstance, this);

	if (m_hWnd==0)
		return False;

	if (bHide==False)
	{
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		InvalidateRect(0, FALSE);
		UpdateWindow(m_hWnd);
	}
	return True;
}

// -------------------------------------
Bool cglsWindow::ReSize(UINT x, UINT y, Bool bIsRepaint)
{
	return __super::ReSize(x, y, bIsRepaint);
	if (m_bIsPopup==False)
		return __super::ReSize(x, y, bIsRepaint);

	return __super::ReSize(x+3, y+3, bIsRepaint);
}


// -------------------------------------
Bool cglsWindow::CreatePopup(HWND hWndParent, int x, int y, int w, int h, Font* font, int style)
{
	HINSTANCE hInstance=(HINSTANCE)GetWindowLong(hWndParent, GWL_HINSTANCE);
	m_DrawObject=0;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= 0;//CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(GetStockObject(NULL_BRUSH));
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(menuId);
	wcex.lpszClassName	= "GLS_POPUP";
	wcex.hIconSm		= 0;

	RegisterClassEx(&wcex);

	m_parent=0;
	m_hParentWnd=hWndParent;

	sGlsDetail xdetail;
	cGlassInterface::xDefaultDetail(&xdetail);
	cGlassInterface::InitDetail(xdetail);

	if (m_font==0)
	{
		if (font)
			m_font=(Font*)cyiShape::Clone(font);//font->Clone();

		if (m_font==0)
		{
			Font font(L"Meiryo", 10.5);
			SetFont(&font);
		}
	}

	xSET_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW, False);
	xSET_GLS_FLAG(m_flag, eGF_FONT_FROM_PARENT, False);
	xSET_GLS_FLAG(m_flag, eGF_FOCUS_MANAGE, True);

	if (m_font)
	{
		m_nFontHeight = m_font->GetHeight(__DEFAULTGRAPHICS__);
		unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, L"1", m_font, 0, 0);
		m_nFontHeightGDI = (pos&0xFFFF);
	}
	m_bIsPopup=True;
	m_bIsExitLoop=False;
	//POINT pt = {x, y};
	//ClientToScreen(hWndParent, &pt);
	m_hWnd = CreateWindowEx(/*WS_EX_LAYERED|*/WS_EX_TOPMOST, "GLS_POPUP", 0, WS_POPUP|WS_VISIBLE,
				  x, y, w, h, NULL, NULL, hInstance, this);

	if (m_hWnd==0)
	{
		xTRACE("cglsWindow::CreatePopup() CreateWindow Error %d", GetLastError());
		return False;
	}

	m_x=x;
	m_y=y;
	//m_w=w;
	//m_h=h;

	//ShowWindow(m_hWnd, SW_SHOWNORMAL);
	Show();
	InvalidateRect(0, FALSE);
	UpdateWindow(m_hWnd);
	SetFocus(m_hWnd);

	
	return MessageLoop(True);

}

// ------------------------------------
Bool cglsWindow::OnUpdateAfter(HDC hdc)
{
/*	if (m_bIsPopup==False)
		return False;

	if (m_hShadow)
	{
		cyiShape::DestroyShadow(m_hShadow);
	}




	m_hShadow = cyiShape::CreateShadow(m_fShadowWidth, m_fShadowEdge, m_fShadowLevel);

	Bitmap* base = cyiShape::DrawRoundRectEx(0, m_colorBorder?&Pen(Color(m_colorBorder)):0, 
												m_brush?m_brush:&SolidBrush(Color(m_colorBg)),
														RectF(0, 0, w+margin, h), m_fShadowEdge);

	cyiShape::BitmapStringEx(base, (margin>>1), 0, m_pszText, GetFont(), m_colorFont);

	m_bmpCurrent = cyiShape::MakeShadowBitmap(m_hShadow, m_w, m_h);

	Graphics g(m_bmpCurrent);



	g.DrawImage(base, k>>1, k>>1);

	delete base;

	if (m_hBmp)
	{
		DeleteObject(m_hBmp);
	}

	m_hBmp = cyiBitmapBlender::CreateHBitmapV5(m_bmpCurrent->GetWidth(), m_bmpCurrent->GetHeight(), m_bmpCurrent);


	::InvalidateRect(m_hWnd, 0, FALSE);

    HDC hmemdc, hsdc;
	hsdc   = ::GetDC(0);                      // デスクトップのデバイスコンテキスト（色情報指定用）
    hmemdc = CreateCompatibleDC(hdc);       // hdcの互換デバイスコンテキスト

    BLENDFUNCTION blend;
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = (int)((float)255.0*m_fTransparency); // 不透明度（レイヤードウィンドウ全体のアルファ値）
    blend.AlphaFormat = AC_SRC_ALPHA;

	HGDIOBJ hObj = SelectObject(hmemdc, m_hBmp);
	BitBlt(hdc, 0, 0, m_w, m_h, hmemdc, 0, 0, SRCCOPY|CAPTUREBLT); 

	POINT pt={0, 0};
	SIZE size = {m_w, m_h};
	RECT rectWindow;
	GetWindowRect(m_hWnd, &rectWindow);

    // レイヤードウィンドウの位置、サイズ、形、内容、透明度を更新
    if (0 == UpdateLayeredWindow(m_hWnd, hsdc, (LPPOINT)&rectWindow, &size, hmemdc, &pt, 0, &blend, ULW_ALPHA)) {
        //TCHAR strErrMes[80];
        //DWORD err = GetLastError();

        //wsprintf(strErrMes, _T("UpdateLayeredWindow失敗：エラーコード＝%d"), err);
        //MessageBox(hWnd, strErrMes, _T("エラー"), MB_OK | MB_ICONSTOP);
        //DestroyWindow(hWnd);
		YIERROR("cglsToolTip::OnPaint() UpdateLayeredWindow() failure!");

		SelectObject(hmemdc, hObj);
		DeleteDC(hmemdc);
		ReleaseDC(hsdc);
		return False;
    }

	SelectObject(hmemdc, hObj);
	DeleteDC(hmemdc);
	ReleaseDC(hsdc);

	return True;*/

	return False;
}


// -------------------------------------
Bool cglsWindow::MessageLoop(Bool UnfocusExit)
{
	Bool res=True;
	MSG msg;
	int quit=0;

	//SetCapture(m_hWnd);
	while((quit=GetMessage(&msg, NULL, 0, 0))!=0)
	{
		if (msg.message==WM_MOUSEMOVE)
		{
			RECT rect;
			POINT pt={msg.wParam&0xFFFF, (msg.wParam>>16)&0xFFFF};
			ClientToScreen(msg.hwnd, &pt);
			GetWindowRect(m_hWnd, &rect);

			if (yiUtilPointIsInRect(&rect, pt.x, pt.y)==False)
			{
				if (m_exceptionObject==0)
					continue;

				if (msg.hwnd!=m_exceptionObject->m_hWnd &&
						GetParent(msg.hwnd)!=m_exceptionObject->m_hWnd &&
							GetParent(GetParent(msg.hwnd))!=m_exceptionObject->m_hWnd)
				{
					continue;
				}
			}
		}

		//if (m_exceptionObject!=0)
		//{
			if (msg.message==WM_LBUTTONDOWN ||
					msg.message==WM_RBUTTONDOWN /*||
						msg.message==WM_LBUTTONUP ||
							msg.message==WM_RBUTTONUP*/)
			{
				RECT rect;
				POINT pt={msg.wParam&0xFFFF, (msg.wParam>>16)&0xFFFF};
				ClientToScreen(msg.hwnd, &pt);
				GetWindowRect(m_hWnd, &rect);

				if (yiUtilPointIsInRect(&rect, pt.x, pt.y)==False)
				{

					if (m_exceptionObject!=0 &&
						(msg.hwnd==m_exceptionObject->m_hWnd ||
							GetParent(msg.hwnd)==m_exceptionObject->m_hWnd ||
								GetParent(GetParent(msg.hwnd))==m_exceptionObject->m_hWnd))
					{
					}
					else
					{
						if (UnfocusExit)
						{
							m_bIsExitLoop=True;
							m_bIsExitCode=False;
						}
						else
							continue;
					}
				}
			}
		//}

		if (m_bIsExitLoop==False)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (m_bIsExitLoop==True)
		{
			Sleep(10);
			while (PeekMessage(&msg,NULL, 0, 0, PM_REMOVE)==True)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				//xTRACE("Peek Message..... %d", msg.message);
			}
			//xTRACE("m_bIsExitLoop is True..... exit");
			break;
		}
	}

	//ReleaseCapture();

	if (quit==0)
	{
		PostQuitMessage(0);
		return 0;
	}
	//return True;
	return m_bIsExitCode;
}

// -------------------------------------
Bool cglsWindow::DoModal(HWND hWndParent, HINSTANCE hInstance,  char* pszTitle, int x, int y, int w, int h, Font* font, int style)
{
	m_bIsModal=True;
	m_hParentWnd=hWndParent;


	sGlsDetail xdetail;
	cGlassInterface::xDefaultDetail(&xdetail);
	cGlassInterface::InitDetail(xdetail);

	if (font)
		m_font=(Font*)cyiShape::Clone(font);//font->Clone();
	//m_font=font;
	if (m_font==0)
	{
		m_font = new Font(L"Meiryo", 10.0);
	}

	xSET_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW, False);
	xSET_GLS_FLAG(m_flag, eGF_FONT_FROM_PARENT, False);
	xSET_GLS_FLAG(m_flag, eGF_FOCUS_MANAGE, True);

	m_w=w;
	m_h=h;
	m_parent=0;

	LPDLGTEMPLATE p=cyctrlbase::CreateDialogTemplate(x, y, w, h, yiToUnicode(0, pszTitle, 0));
	int res = ::DialogBoxIndirectParam((HINSTANCE)GetWindowLong(hWndParent, GWL_HINSTANCE), p, hWndParent, (DLGPROC)cyctrlbase::__dlgproc__, (LPARAM)this);
	yifree(p);

	return res;
}

// ---------------------------------------------------
LRESULT cglsWindow::OnCreate(WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	GetWindowRect(m_hWnd, &rect);

	if (GetParent(m_hWnd)==0)
	{
		m_x=rect.left;
		m_y=rect.top;
	}

	::GetClientRect(m_hWnd, &rect);
	m_w=rect.right;
	m_h=rect.bottom;

	if (m_w==0 ||
			m_h==0)
	{
		ReSize(100, 100);
	}

	if (m_x<0 ||
			m_y<0)
	{
		Move(0, 0);
	}

	return 0;
}

// -------------------------------------
Bool cglsWindow::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	__super::OnPaint(hdc, ps);
	return True;
}

// -----------------------------
Bitmap* cglsWindow::GetBgBitmap()
{
	if (m_bg==0)
	{
		OnBgDraw();
	}

	return m_bg;
}

// ---------------------------------------
Bool cglsWindow::OnBgDraw()
{
	if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==True)
	{
		return False;
	}

	if (m_bmpCurrent==0)
	{
		m_bmpCurrent=BITMAP32(m_w, m_h);
	}
	if (m_bg==0)
	{
		m_bg=BITMAP32(m_w, m_h);
	}

	int w=m_bmpCurrent->GetWidth();
	int h=m_bmpCurrent->GetHeight();

	if (w!=m_w || h!=m_h)
	{
		delete m_bmpCurrent;
		delete m_bg;
		m_bmpCurrent=BITMAP32(m_w, m_h);
		m_bg=BITMAP32(m_w, m_h);
	}

	Graphics g(m_bg);
	g.FillRectangle(&SolidBrush(Color::White), 0, 0, m_w, m_h);
	g.DrawRectangle(&Pen(Color::Black), 0, 0, m_w-1, m_h-1);

	cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);

	//NotifyBgChanged();

	//m_eUpdateReason=eGUR_BG_CHANGED;
	//Update(m_bmpCurrent, 0);
	InvalidateRect(0);
	return True;
}

// ---------------------------------------
LRESULT cglsWindow::OnMouseLButtonDown(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	SetFocus(m_hWnd);

	//if (m_bIsPopup)
	//{
	//	RECT rect;
	//	POINT pt={lParam&0xFFFF, (lParam>>16)&0xFFFF};
	//	GetWindowRect(m_hWnd, &rect);
	//	
	//	if (pt.x <0 || 
	//			pt.y<0)
	//	{
	//		m_bIsExitLoop=True;
	//		return 0;
	//	}

	//	ClientToScreen(m_hWnd, &pt);

	//	if (pt.x > rect.right ||
	//			pt.y > rect.bottom)
	//	{
	//		m_bIsExitLoop=True;
	//		return 0;
	//	}
	//	return 1;

	//}

	return 0;
}

// ---------------------------------------
LRESULT cglsWindow::OnMouseRButtonDown(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	//if (m_bIsPopup)
	//{
	//	RECT rect;
	//	POINT pt={lParam&0xFFFF, (lParam>>16)&0xFFFF};
	//	GetWindowRect(m_hWnd, &rect);
	//	
	//	if (pt.x <0 || 
	//			pt.y<0)
	//	{
	//		m_bIsExitLoop=True;
	//		return 0;
	//	}

	//	ClientToScreen(m_hWnd, &pt);

	//	if (pt.x > rect.right ||
	//			pt.y > rect.bottom)
	//	{
	//		m_bIsExitLoop=True;
	//		return 0;
	//	}
	//	return 1;
	//}
	return 0;
}

// ---------------------------------------
LRESULT cglsWindow::OnActivate(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	return 0;
	if (m_bIsPopup)
	{
		if ((wParam & 0xFFFF)==0)
		{
			if (m_exceptionObject==0)
			{
				m_bIsExitLoop=True;
				m_bIsExitCode=0;
			}
		}
	}
	return 0;
}

// ---------------------------------------
LRESULT cglsWindow::OnMouseMove(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	//if (m_bIsPopup)
	//{
	//	RECT rect;
	//	POINT pt={lParam&0xFFFF, (lParam>>16)&0xFFFF};
	//	GetWindowRect(m_hWnd, &rect);
	//	
	//	if (pt.x <0 || 
	//			pt.y<0)
	//	{
	//		//m_bIsExitLoop=True;
	//		return 0;
	//	}

	//	ClientToScreen(m_hWnd, &pt);

	//	if (pt.x > rect.right ||
	//			pt.y > rect.bottom)
	//	{
	//		//m_bIsExitLoop=True;
	//		return 0;
	//	}
	//	return 1;

	//}
	return 0;
}

// ---------------------------------------
LRESULT cglsWindow::OnKillFocus(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	//if (m_bIsPopup)
	//{
	//	m_bIsExitLoop=True;
	//	bIsHandled=True;
	//}
	return 0;
}

// ---------------------------------------
LRESULT cglsWindow::OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (wParam==0 ||
		wParam==2)
	{
		RECT rect;
		::GetClientRect(m_hWnd, &rect);
		m_w=rect.right;
		m_h=rect.bottom;

		if (this->m_bmpCurrent)
			delete m_bmpCurrent;
		if (m_bg)
			delete m_bg;
		m_bmpCurrent=0;
		m_bg=0;
		OnBgDraw();
	}

	//InvalidateRect(0, False);

	return 0;
}
