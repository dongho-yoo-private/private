#include "cyctrlButtons.h"
#include "yiBitmap.h"

cyctrlButtons::cyctrlButtons(void)
: cyctrlbase()
{
}

cyctrlButtons::~cyctrlButtons(void)
{
}



// ---------------------------------------
BOOL cyctrlButtons::OnPaint(HDC hdc)
{
	Graphics g(hdc);
	g.DrawImage(m_bmp, 0, 0);

	return TRUE;
}

// ---------------------------------------
static BOOL xIsInRect(RECT* rect, int x, int y)
{
	if (x<rect->left)
		return FALSE;
	if (x>rect->right)
		return FALSE;
	if (y>rect->bottom)
		return FALSE;
	if (y<rect->top)
		return FALSE;

	return TRUE;
}

// ---------------------------------------
LRESULT cyctrlButtons::OnClicked(WPARAM wp, LPARAM lp)
{
	POINT pos;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標

	for (cyiNode* p = m_buttons.GetNodeTopFrom(0); p!=0; p = p->np)
	{
		if (p->ref==0)
		{
			continue;
		}
		sButtonsStruct* btn = (sButtonsStruct*)p->ref;

		if (xIsInRect(&btn->rect, xPos, yPos)==TRUE)
		{
			SendMessage(this->m_hParentWnd, WM_COMMAND, btn->id, 0);
			break;
		}
	}
	return 0;
}

// ---------------------------------------
LRESULT cyctrlButtons::OnMouseMove(WPARAM wp, LPARAM lp, BOOL& handled)
{
	POINT pos;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標

	for (cyiNode* p = m_buttons.GetNodeTopFrom(0); p!=0; p = p->np)
	{
		if (p->ref==0)
		{
			continue;
		}
		sButtonsStruct* btn = (sButtonsStruct*)p->ref;

		if (xIsInRect(&btn->rect, xPos, yPos)==TRUE)
		{
			if (m_selected_btn)
			{
				DrawButtons(0, m_selected_btn, 0);
				InvalidateRect(m_hWnd, &m_selected_btn->rect, FALSE);
			}
			DrawButtons(0, btn, 2);
			InvalidateRect(m_hWnd, &btn->rect, FALSE);
			m_selected_btn = btn;
			break;
		}

	}
	return 0;

}

// --------------------------------------------
BOOL cyctrlButtons::Create(HWND hWndParent, int x, int y, Brush* bgNormal, Brush* fntNormal, Pen* pen,  BOOL IsHide)
{
	m_fntBrush = fntNormal;
	m_bgBrush  = bgNormal;
	m_selected_btn=0;
	m_pen=pen;
	m_bmp=0;
	return cyctrlbase::Create(hWndParent, 0, 0, x, y, 10, 10, IsHide);
}


// -----------------------------
BOOL cyctrlButtons::Add(int id, cyiBitmap* bmp, const wchar_t* pszText/*, unsigned int fgColor, unsigned int bgColor*/)
{
	sButtonsStruct data;
	data.id=id;
	data.bmp=bmp;
	memset(&data.rect, 0, sizeof(data.rect));
	wcscpy(data.name, pszText);

	m_buttons.addref(yitoheap(&data, sizeof(data)));

	return TRUE;
}

// -----------------------------
void cyctrlButtons::Update()
{
	int width=0;
	int height=0;
	HDC hdc = ::GetDC(m_hWnd);
	Graphics g(hdc);

	for (cyiNode* p = m_buttons.GetNodeTopFrom(0); p!=0; p = p->np)
	{
		if (p->ref==0)
		{
			continue;
		}
		sButtonsStruct* btn = (sButtonsStruct*)p->ref;
		btn->rect.right=0;

		//if (btn->bmp)
		//{
		//	unsigned int n = btn->bmp->GetWidth();
		//	width+=n;
		//	btn->rect.right+=n;
		//}

		if (btn->name[0]!=0)
		{
			RectF bound;
			int len = wcslen(btn->name);
			unsigned int bwidth=0;
			Status ret = g.MeasureString(btn->name, len, &m_font, RectF(0, (float)0, (float)1024, (float)30), &bound);
			btn->rect.left=width;
			height=height<bound.Height?bound.Height:height;

			bwidth=(bound.Width+2);
			width+=bwidth;
			btn->rect.right+=(bwidth+btn->rect.left);
			btn->rect.top=0;
			btn->rect.bottom=30;
		}
	}

	height+=2;
	if (m_bmp==0)
	{
		delete m_bmp;
	}

	m_bmp = new Bitmap(width, 30, PixelFormat32bppARGB);
	Graphics _g(m_bmp);
	_g.FillRectangle(m_bgBrush, RectF(0, 0, width, height));

	for (cyiNode* p = m_buttons.GetNodeTopFrom(0); p!=0; p = p->np)
	{
		if (p->ref==0)
		{
			continue;
		}
		sButtonsStruct* btn = (sButtonsStruct*)p->ref;

		btn->rect.bottom=height;
		DrawButtons(&_g, btn, 0);
	}

	SetWindowPos(m_hWnd, 0, 0, 0, width, height, SWP_NOMOVE|SWP_NOZORDER);
	this->m_h=height;
	this->m_w=width;
	ShowWindow(m_hWnd, SW_SHOW);
	InvalidateRect(m_hWnd, 0, FALSE);
}

// -----------------------------
void cyctrlButtons::DrawButtons(Graphics* g, sButtonsStruct* btn, int nMode)
{
	if (btn==0)
		return ;

	Graphics _g(m_bmp);
	if (g==0)
	{
		g=&_g;
	}
	g->FillRectangle(m_bgBrush, RectF(btn->rect.left, btn->rect.top, btn->rect.right, btn->rect.bottom));

	if (nMode==2)
	{
		g->DrawRectangle(m_pen, RectF(btn->rect.left+1, 1, btn->rect.right-2,  btn->rect.bottom-2));
	}

	g->DrawString(btn->name, wcslen(btn->name), &m_font, RectF(btn->rect.left, btn->rect.top, btn->rect.right, btn->rect.bottom), 0, m_fntBrush);
}

LRESULT cyctrlButtons::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	sButtonsStruct* btn = m_selected_btn;
	DrawButtons(0, btn, 0);
	m_selected_btn = 0;
	InvalidateRect(m_hWnd, &btn->rect, FALSE);
	return 0;

}