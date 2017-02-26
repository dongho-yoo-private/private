#include "yctrlToolTip.h"

cyctrlToolTip::cyctrlToolTip(void)
:cyctrlbase()
{
}

cyctrlToolTip::~cyctrlToolTip(void)
{
}

// -----------------------------------
BOOL cyctrlToolTip::Create(HWND hWnd)
{
	HWND hTopParent=hWnd;
	while(1)
	{
		HWND h = ::GetParent(hTopParent);

		if (h==0)
		{
			break;
		}
		hTopParent=h;
	}

	m_hFromWnd=hWnd;
	return cyctrlbase::Create(hTopParent, NULL, TOOL_TIP_ID, 0, 0, 100, 100, TRUE);
}

// -----------------------------------
BOOL cyctrlToolTip::Show(const char* pszComment)
{
	return TRUE;
}

#define __CURSOR_SIZE__ 16

#define DEFAULT_TOOL_TIP_HEIGHT 20

// -----------------------------------
BOOL cyctrlToolTip::Show(const wchar_t* pszComment)
{
	RectF bound;
	POINT pos;

	GetCursorPos(&pos);
	
	int x = pos.x;
	int y = pos.y;
	HDC hdc = ::GetDC(*this);
	Graphics g(hdc);

	Status ret = g.MeasureString(pszComment, wcslen(pszComment), &m_font, RectF(0.0, 0.0, 128, 20), &bound);
	::ReleaseDC(*this, hdc);

	UINT w = (UINT)(bound.Width)+1;

	RECT rect;
	RECT crect;
	::GetWindowRect(m_hParentWnd, &rect);
	::GetClientRect(m_hParentWnd, &crect);

	UINT top = rect.top+(rect.bottom-rect.top-crect.bottom);
	UINT left = rect.left+(rect.right-rect.left-crect.right);

	wcsncpy(m_tooltip, pszComment, MAX_TOOL_TIP_STRING);

	m_x = x-left+__CURSOR_SIZE__;
	m_y = y-top+__CURSOR_SIZE__;
	m_w = w;
	m_h = DEFAULT_TOOL_TIP_HEIGHT;
	::SetWindowPos(*this, m_hFromWnd,m_x , m_y, m_w, m_h, SWP_SHOWWINDOW);



	::InvalidateRect(*this, NULL, FALSE);
	//::UpdateWindow(*this);
	//::ShowWindow(*this, SW_SHOW);
	return TRUE;
}

// -----------------------------------
void cyctrlToolTip::Hide()
{
	::ShowWindow(*this, SW_HIDE);
}

// -----------------------------------
BOOL cyctrlToolTip::OnPaint(HDC hdc)
{
	Graphics g2(hdc);

	RECT rect;
	::GetClientRect(*this, &rect);
	g2.FillRectangle(&SolidBrush(Color::Yellow), RectF(0.0, 0.0, (float)rect.right, (float)rect.bottom));
	g2.DrawRectangle(&Pen(Color::Black, 0.5), RectF(0.0, 0.0, (float)(rect.right-1), (float)(rect.bottom-1)));
	g2.DrawString(m_tooltip, wcslen(m_tooltip), &m_font, RectF(0, 0, (float)rect.right, (float)rect.bottom), NULL, &SolidBrush(Color::Black));

	return TRUE;
}
