#include "glsToolTip.h"


// -----------------------------------------
void cglsToolTip::xDefaultDetail(sGlsToolTipDetail* detail)
{
	detail->brush=0;
	detail->colorBg=Color::Beige;
	detail->colorBorer=Color::Black;
	detail->colorFont=Color::Black;
	detail->fShadowEdge=2.0;
	detail->fShadowLevel=0.5;
	detail->fShadowWidth=2.0;
}

cglsToolTip* cglsToolTip::m_me = 0;

// -----------------------------------------
cglsToolTip* cglsToolTip::GetInstance(HINSTANCE hInstance, Font* font, sGlsToolTipDetail* detail)
{
	if (m_me==0)
	{
		m_me = new cglsToolTip();
		m_me->Create(hInstance, font, detail);
	}

	return m_me;
}

cglsToolTip::cglsToolTip(void)
: m_pszText(0)
, m_hShadow(0)
, m_hBmp(0)
, m_fShadowWidth(2.0)
, m_fShadowEdge(2.0)
, m_fShadowLevel(0.5)
, m_brush(0)
, m_colorBorder(Color::Black)
, m_colorFont(Color::Black)
, m_colorBg(Color::Beige)
, m_fTransparency(1.0)
, m_from(0)
{
}

cglsToolTip::~cglsToolTip(void)
{
}


// -----------------------------------------
Bool cglsToolTip::Create(HINSTANCE hInstance, Font* font, sGlsToolTipDetail* detail)
{
	sGlsToolTipDetail xdetail;

	if (detail==0)
	{
		xDefaultDetail(&xdetail);
		detail=&xdetail;
	}

	m_brush=detail->brush;
	m_colorBg=detail->colorBg;
	m_colorBorder=detail->colorBorer;
	m_colorFont=detail->colorFont;
	m_fShadowEdge=detail->fShadowEdge;
	this->m_fShadowLevel=detail->fShadowLevel;
	this->m_fShadowWidth=detail->fShadowWidth;
	m_fTransparency=1.0;

	return cglsWindow::Create(hInstance, 0, 0, 0, 10, 10, 0, 0, font, 0, WS_EX_LAYERED | WS_EX_TOPMOST, WS_POPUP|WS_DISABLED, True);
}
// -----------------------------------------
LRESULT cglsToolTip::OnCreate(WPARAM wParam, LPARAM lParam)
{
	EnableWindow(m_hWnd, False);
	return 0;
}

// ------------------------------------
Bool cglsToolTip::xUpdate()
{
	unsigned int n = (unsigned int)cyiShape::BitmapStringEx(0, m_pszText, GetFont(), 0, 0);
	int w = n>>16;
	int h = (n&0xFFFF);
	const int margin = GetFontHeight(True)>>1;
	int k = (int)m_fShadowWidth;

	if (m_bmpCurrent)
	{
		delete m_bmpCurrent;
		m_bmpCurrent=0;
	}

	ReSize(w+(int)(k<<1)+margin, h+(int)(k<<1), True);
	
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
	return True;
}


// ---------------------------------------
Bool cglsToolTip::SetText(const char* pszText, void* from)
{
	if (m_from==from)
		return False;

	if (pszText==0)
	{
		return SetText((const wchar_t*)0, from);
	}

	if (m_pszText!=0)
		yifree(m_pszText);

	m_pszText = yiToUnicode(0, pszText, 0);
	m_from=from;
	xUpdate();


	return True;
}

// ---------------------------------------
Bool cglsToolTip::SetText(const wchar_t* pszText, void* from)
{
	if (m_from==from)
		return False;

	if (m_pszText!=0)
		yifree(m_pszText);

	if (pszText==0)
	{
		m_pszText=0;
		from=0;
		return False;
	}
	else
		m_pszText = yistrheapW(pszText);
	m_from=from;


	xUpdate();

	return True;
}

// ---------------------------------------
LRESULT cglsToolTip::OnActivate(WPARAM wParam, LPARAM lParam, Bool&bIshandled)
{
	bIshandled=True;
	::SetActiveWindow((HWND)lParam);
	return 0;
}

// ---------------------------------------
LRESULT cglsToolTip::OnFocus(WPARAM wParam, LPARAM lParam, Bool&bIshandled)
{
	if (wParam)
		::SetFocus((HWND)wParam);
	bIshandled=True;
	return 0;
}

// ---------------------------------------
Bool cglsToolTip::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
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

	//Graphics g(hdc);
	//g.FillRectangle(&SolidBrush(Color::Red), 0, 0, m_w, m_h);

	return True;
}

// ---------------------------------------
void cglsToolTip::Show()
{

	SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);
	//ShowWindow(m_hWnd, SW_SHOW);
	::InvalidateRect(m_hWnd, 0, False);
}

// ---------------------------------------
void cglsToolTip::Hide(Bool bClearFrom)
{
	ShowWindow(m_hWnd, SW_HIDE);

	if (bClearFrom)
		m_from=0;
}