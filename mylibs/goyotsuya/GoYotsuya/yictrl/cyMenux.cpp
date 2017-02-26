#include "cyMenux.h"
#include "yiResource.h"
#include "cyctrlResource.h"

cyMenux::cyMenux(void)
{
	m_currSel=-1;
}

cyMenux::~cyMenux(void)
{
}

// --------------------------------
Bool cyMenux::Create(cyctrlbase* parent, int id, color_t bgColor, color_t fntColor)
{
	m_bgColor=bgColor;
	m_fntColor=fntColor;

	m_parent=parent;
	m_xx.Create(0, 0, 0, 0, 0, 0, True,(Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_DEFAULT_FONT) ,True);
	return cyctrlbase::Create(parent->m_hWnd, NULL, id, 0, 0, 10, 10, TRUE);
}

// -------------------------------
void cyMenux::AddItem(int id, wchar_t* pszItem)
{
	m_xx.Add(id, 0, pszItem, 5, 10, 10, 0);
}

// -------------------------------
void cyMenux::AddItem(int id, cyctrlbase* ctrl)
{
	return ;
}

// ------------------------------------
void cyMenux::Show(int x, int y)
{
	m_xx.Create(this, x, y);
	Move(x, y);
	ReSize(m_xx.m_w, m_xx.m_h);
	::SetWindowPos(m_hWnd, HWND_TOP,0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	UpdateWindow(m_hWnd);
}

// ------------------------------------
void cyMenux::Move(UINT x, UINT y)
{
	if (x==(int)m_x &&
			y==(int)m_y)
	{
		return;
	}
	cyctrlbase::Move(x, y);
	m_xx.Move(0,0);
	m_xx.Update(True);
}

// -------------------------------
LRESULT cyMenux::OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled)
{
	SendMessage(m_parent->m_hWnd, WM_COMMAND, MAKEWPARAM((wp)&0xFFFF, m_id), lp);
	bIsHandled=True;
	return 0;
}

// ---------------------------------
LRESULT cyMenux::OnSelectedItem(WPARAM wp, LPARAM lp, Bool& bIsHandled)
{
	bIsHandled=True;
	SendMessage(m_parent->m_hWnd, WM_COMMAND, MAKEWPARAM((wp)&0xFFFF, m_id), wp);
	m_currSel=wp;

	return 0;
}

// ---------------------------------
LRESULT cyMenux::OnKillFocus(WPARAM wp, LPARAM lp, Bool& bIsHandled)
{
	Hide();
	SendMessage(m_parent->m_hWnd, WM_COMMAND, MAKEWPARAM((m_currSel)&0xFFFF, m_id), lp);
	Destroy();
	return 0;
}

// ------------------------------------
BOOL  cyMenux::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	//Graphics g(hdc);
	//g.FillRectangle(&SolidBrush(Color::Black), 0, 0, m_w, m_h);
	return True;
}

// ---------------------------------
Bool cyMenux::OnCtrlxBgDraw(int id, Bitmap* srf, int x, int y)
{
	int w = srf->GetWidth();
	int h = srf->GetHeight();

	Graphics g(srf);

	g.FillRectangle(&SolidBrush(m_bgColor), 0, 0, w, h);
	g.DrawRectangle(&Pen(Color::Black), 0, 0, w, h);
	return True;
}

// ------------------------------------
Bool cyMenux::OnCtrlxDraw(int id, int subId, Bitmap* srf, Bitmap* bg, 
						   Font* fnt, sControlxII* s, eControlxEventII eventId)
{
	if (eventId==eCEII_NORMAL)
	{
		if (s->pCurrBmp==0)
		{
			eventId=eCEII_INIT;
		}
		else
		{
			cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, s, eventId);

			Graphics g(srf);
			g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
			g.DrawImage(s->pCurrBmp, s->rect.left, s->rect.right);
			return True;
		}
	}

	if (eventId==eCEII_INIT ||
			eventId==eCEII_MOUSE_LEAVE)
	{
		if (s->pBmp[5]==0)
		{
			Bitmap* bmp = new Bitmap(s->rect.right, s->rect.top, PixelFormat32bppARGB);
			Bitmap* str = cyiShape::BitmapString(0, s->name, fnt, &SolidBrush(m_fntColor));
			Graphics g(bmp);
			g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
			g.DrawImage(str, 0, (bmp->GetHeight()-str->GetHeight())>>1);
			s->pBmp[5]=bmp;
			s->pCurrBmp=s->pBmp[5];
			delete str;
		}

		//cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, s, eventId);
		cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, s, eventId);
		Graphics g(srf);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.DrawImage(s->pBmp[5], s->rect.left, s->rect.right);

		return True;
	}
	else if (eventId==eCEII_MOUSE_IN)
	{
		if (s->pBmp[6]==0)
		{
			Bitmap* bmp = cyiShape::FillRoundRect(&SolidBrush(Color::Orange&0x1F000000), RectF(0, 0, s->rect.right-4, s->rect.bottom-4), 2.0);
			Bitmap* bmp2 = cyiShape::DrawRoundRect(&Pen(Color::Orange), RectF(0, 0, s->rect.right-4, s->rect.bottom-4), 2.0);
			s->pBmp[6]= new Bitmap(s->rect.right, s->rect.top, PixelFormat32bppARGB);
			Graphics g(s->pBmp[6]);
			g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

			g.DrawImage(s->pBmp[5], 0, 0);
			g.DrawImage(bmp, 2, 2);
			g.DrawImage(bmp2, 2, 2);

			delete bmp;
			delete bmp2;

		}
		cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, s, eventId);
		Graphics g(srf);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.DrawImage(s->pBmp[6], s->rect.left, s->rect.right);

		return True;
	}
	else if (eventId==eCEII_DISABLE)
	{
		//if (s->bIsEnable==True)
		//{
		//	if (s->pCurrBmp)
		//	{
		//		cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, s, eventId);
		//		cyiBitmapBlender::RotateCopy(srf, s->pCurrBmp, 0, s->rect.left, s->rect.top, 0, 0, s->rect.right, s->rect.bottom);
		//		return True;
		//	}
		//}

	}

	return False;
}
