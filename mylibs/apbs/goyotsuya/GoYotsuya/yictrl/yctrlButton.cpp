#include "yctrlButton.h"
#include "yiShape.h"
#include "yiBitmap.h"
#include "yiBitmapBlender.h"


cyctrlButton::cyctrlButton(void)
{
	m_style=eBS_NONE;
}

cyctrlButton::~cyctrlButton(void)
{
	if (m_style!=eBS_NONE)
	{
		for (int i=0; i<3; i++)
		{
			if (m_dobjs[i]!=NULL)
			{
				delete m_dobjs[i];
				m_dobjs[i]=0;
			}
		}
	}
}

// ---------------------------------------
BOOL cyctrlButton::Create(HWND hWnd, int id, int x, int y, int w, int h)
{
	if (w==0)
	{
		w = normal->GetWidth();
		h = normal->GetHeight();
	}

	if (cyctrlbase::Create(hWnd, NULL, id, x, y, w, h, FALSE)==FALSE)
	{
		return FALSE;
	}

	m_mode=0;

	m_dobjs[0]=normal;
	m_dobjs[1]=over;
	m_dobjs[2]=pushed;

	if (over!=0)
	{
		MouseTrackMode(TRUE);
	}
	return TRUE;
}

#include <math.h>

#define PI (float)3.141592


// ---------------------------------------
BOOL cyctrlButton::Create(HWND hWnd, int id, int x, int y, int w, int h, eButtonStyle style, UINT color, ...)
{
	m_mode=0;
	m_style=style;

	m_w=w;
	m_h=h;

	if (m_style==eBS_CLOSE)
	{
		Bitmap bmp(m_w, m_h);
		Graphics g(&bmp);
		g.Clear(Color(0,0,0,0));
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		UINT normal_color=cyiBitmapBlender::GetGrayScaleColor(color, 0.5);

		g.FillEllipse(&SolidBrush(Color((ARGB)normal_color)), RectF((float)1, (float)1, (float)m_w-2, (float)m_h-2));
		
		cyiShape::FillX(g, (float)3, (float)3, (float)(m_w-6), Color(Color::Black), (float)2);

		m_dobjs[0] = new cyiDIBitmap(&bmp);

		g.Clear(Color::Transparent);

		g.FillEllipse(&SolidBrush(Color((ARGB)(color))), RectF((float)1, (float)1, (float)m_w-2, (float)m_h-2));
		cyiShape::FillX(g, 3, 3, m_w-6, Color(Color::Black), 2.0);

		m_dobjs[1] = new cyiDIBitmap(&bmp);
		m_dobjs[2]=NULL;
	}
	//else if (m_style==eBS_PLUS)
	//{
	//	Bitmap* bmp = new Bitmap(m_w, m_h);
	//	Graphics g(bmp);
	//	g.Clear(Color(0,0,0,0));
	//	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	//	
	//}


	if (cyctrlbase::Create(hWnd, NULL, id, x, y, w, h, FALSE)==FALSE)
	{
		return FALSE;
	}
	MouseTrackMode(TRUE);
	return TRUE;//cyctrlbase::Create(hWnd, NULL, id, x, y, w, h, FALSE);
}

// ---------------------------------------
BOOL CreateVistaStyle(HWND hWnd, int id, int x, int y, int w, int h, cyiBitmap* bmp)
{
	//m_mode=0;
	//m_style=style;

	//m_w=w;
	//m_h=h;

	//Bitmap bmp(m_w, m_h, PixelFormat32bppARGB);
	return TRUE;
	
}

// ---------------------------------------
BOOL CreateVistaStyle(HWND hWnd, int id, int x, int y, int w, int h, const wchar_t* txt)
{
	return TRUE;
}

// ---------------------------------------
BOOL cyctrlButton::OnPaint(HDC hdc)
{
	Graphics g(hdc);

	if (m_dobjs[m_mode]!=0)
		m_dobjs[m_mode]->Draw(&g, 0, 0);
	return TRUE;
}


// ---------------------------------------
LRESULT cyctrlButton::OnMouseEnter(WPARAM wp, LPARAM lp)
{
	m_mode=1;

	::InvalidateRect(m_hWnd, NULL, TRUE);
	return 0;
}

// ---------------------------------------
LRESULT cyctrlButton::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	m_mode=0;
	::InvalidateRect(m_hWnd, NULL, TRUE);
	return 0;
}

// ---------------------------------------
LRESULT cyctrlButton::OnLButtonDown(WPARAM wp, LPARAM lp, BOOL& IsHandled)
{
	m_mode=2;
	::InvalidateRect(m_hWnd, NULL, TRUE);
	return 0;
}

// ---------------------------------------
LRESULT cyctrlButton::OnLButtonUp(WPARAM wp, LPARAM lp, BOOL& IsHandled)
{
	::SendMessage(m_hParentWnd, WM_COMMAND, ((m_id)&0xFFFF), (LPARAM)m_hWnd);
	m_mode=1;
	::InvalidateRect(m_hWnd, NULL, TRUE);
	return 0;
}

