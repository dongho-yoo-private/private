#include "cyRadiox.h"


cyRadiox::cyRadiox(void)
{
	m_nRadioCnt=0;
	m_cx=0;
	m_prevSelected=0;
}

cyRadiox::~cyRadiox(void)
{
}

// ------------------------------
Bool cyRadiox::AddItem(const wchar_t* pszText, int distance)
{
	if (m_cx==0)
	{
		m_cx = new cyctrlxII();
		if (m_cx->Create(0, 0, 0, 0, 10, 10, True, (Font*)cyiResource::GetInstance(eResourceTypex::eRTX_TEMPORARY)->Get(eRTID_DEFAULT_FONT))!=True)
		{
			return False;
		}
	}

	Bitmap* bmp = new Bitmap(16,16, PixelFormat32bppARGB);
	m_cx->Add(m_nRadioCnt++, bmp, pszText, distance, 5, 5, 0);

	return True;
}

// ------------------------------
Bool cyRadiox::Create(cyctrlbase* ctrl, int id, int x, int y, color_t color, color_t txtcol, int* retval)
{
	m_radio_color=color;
	m_txtColor=txtcol;
	m_retval=retval;
	
	if (m_retval)
		*m_retval=0;

	//if (m_cx->Create(0, 0, 0, 0, 10, 10, True, (Font*)cyiResource::GetInstance(eResourceTypex::eRTX_TEMPORARY)->Get(eRTID_DEFAULT_FONT))!=True)
	//{
	//	return False;
	//}

	m_id=id;
	//int w, h;
	//m_cx->ArrangeItems(w, h);

	//m_w=w;
	//m_h=h;

	m_w=10;
	m_h=10;
	if (ctrl!=0)
	{
		m_parent=ctrl;
		if (cyctrlbase::Create(ctrl->m_hWnd, 0, 0, x, y, m_w, m_h, True)==False)
		{
			return False;
		}

		m_cx->Create(this, 0, 0);
		//m_cx->Update(True);
		return True;
	}

	return True;
}

// ------------------------------
Bool cyRadiox::Create(cyctrlbase* base, int x, int y)
{
	if (cyctrlbase::Create(base->m_hWnd, 0, 0, x, y, m_w, m_h, True)==False)
	{
		return False;
	}

	m_parent=base;


	Move(x, y);
	//m_OffBmp=new Bitmap(m_w, m_h, PixelFormat32bppARGB);
	//m_OffBmp = base->GetBackGround(x, y, m_w,m_h);


	m_cx->Create(this, 0, 0);
	m_cx->Update(True);

	m_cx->SelectionChange(0);
	Show();
	UpdateWindow(m_hWnd);
	return True;
}

// ------------------------------
//Bool cyRadiox::OnPaint(HDC hdc, PAINTSTRUCT* ps)
//{
//	Graphics g(hdc);
//	g.DrawImage(m_OffBmp);
//}

// ------------------------------
void cyRadiox::Destroy()
{
	if (m_cx)
	{
		m_cx->Destroy();
		delete m_cx;
	}
	cyctrlbase::Destroy();
}

Bitmap* g_bmp_radio_on=0;
Bitmap* g_bmp_radio_off=0;

#define __REVERSE_COLOR__(col) ((0xFF000000&col) |((~col)&0x00FFFFFF))

// ------------------------------------
Bool cyRadiox::OnMakeItem(int id, int subId, Font* fnt, sControlxII* s)
{
	if (g_bmp_radio_on==0)
	{
		if (s->bmp!=0)
		{
			delete s->bmp;
			s->bmp=0;
		}
		g_bmp_radio_on=(Bitmap*)cyiResource::GetInstance(eResourceTypex::eRTX_IMAGE)->Get(eRBID_RADIO_ON);
		g_bmp_radio_off=(Bitmap*)cyiResource::GetInstance(eResourceTypex::eRTX_IMAGE)->Get(eRBID_RADIO_OFF);
		if (g_bmp_radio_on==0)
		{
			g_bmp_radio_on = new Bitmap(16, 16, PixelFormat32bppARGB);
			g_bmp_radio_off = new Bitmap(16, 16, PixelFormat32bppARGB);

			Graphics gx(g_bmp_radio_on);
			gx.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

			gx.DrawEllipse(&Pen(Color((ARGB)m_radio_color), 2.0), 2, 2, 10, 10);
			gx.FillEllipse(&SolidBrush(Color((ARGB)m_radio_color)), 4, 4, 6, 6);

			Graphics gxx(g_bmp_radio_off);
			gxx.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
			gxx.DrawEllipse(&Pen(Color((ARGB)m_radio_color), 2.0), 2, 2, 10, 10);
		}
	}

	if (s->pBmp[5]==0)
	{
		Bitmap* str =cyiShape::BitmapString(0, s->name, fnt, &SolidBrush(Color((ARGB)m_txtColor)), eSTS_GROW, __REVERSE_COLOR__(m_txtColor), 3.0, 4.0);
		s->rect.right+=str->GetWidth();
		s->rect.bottom+=str->GetHeight();
		s->rect.right+=g_bmp_radio_off->GetWidth();

		s->pBmp[5]= new Bitmap(s->rect.right, s->rect.bottom);
		
		int bmpy = ((s->rect.bottom-g_bmp_radio_off->GetHeight())>>1);
		cyiBitmapBlender::RotateCopy(s->pBmp[5], g_bmp_radio_off, 0, 0, bmpy, 0, 0, 16, 16);

		int stry = ((s->rect.bottom-str->GetHeight())>>1);
		cyiBitmapBlender::RotateCopy(s->pBmp[5], str, 0, 
										g_bmp_radio_off->GetWidth(), stry, 
											0, 0, str->GetWidth(), str->GetHeight());
		delete str;

	}

	if (s->pBmp[6]==0)
	{
		Bitmap* str =cyiShape::BitmapString(0, s->name, fnt, &SolidBrush(Color((ARGB)m_txtColor)), eSTS_GROW, __REVERSE_COLOR__(m_txtColor), 3.0, 4.0);

		s->pBmp[6]= new Bitmap(s->rect.right, s->rect.bottom);
		
		int bmpy = ((s->rect.bottom-g_bmp_radio_on->GetHeight())>>1);
		cyiBitmapBlender::RotateCopy(s->pBmp[6], g_bmp_radio_on, 0, 0, bmpy, 0, 0, 16, 16);

		int stry = ((s->rect.bottom-str->GetHeight())>>1);
		cyiBitmapBlender::RotateCopy(s->pBmp[6], str, 0, 
										g_bmp_radio_on->GetWidth(), stry, 
											0, 0, str->GetWidth(), str->GetHeight());
		delete str;

	}

	return True;
}

// ------------------------------------
Bool cyRadiox::OnCtrlxDraw(int id, int subId, Bitmap* srf, Bitmap* bg, 
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
			cyiBitmapBlender::RotateCopy(srf, s->pCurrBmp, 0, s->rect.left, s->rect.top, 0, 0, s->rect.right, s->rect.bottom);
		}
	}

	if (eventId==eCEII_INIT ||
			eventId==eCEII_UNSELECTED)
	{
		cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, s, eventId);
		cyiBitmapBlender::RotateCopy(srf, s->pBmp[5], 0, s->rect.left, s->rect.top, 0, 0, s->rect.right, s->rect.bottom);
		s->pCurrBmp=s->pBmp[5];

		return True;
	}
	else if (eventId==eCEII_MOUSE_IN)
	{
		Graphics gxx(srf);
		gxx.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		//gxx.DrawRectangle(&Pen(Color((ARGB)m_txtColor)), Rect(s->rect.left, s->rect.top, s->rect.right-1, s->rect.bottom-1));
		gxx.DrawLine(&Pen(Color((ARGB)m_radio_color)), s->rect.left+2, s->rect.top+s->rect.bottom-2, s->rect.left+s->rect.right-2, s->rect.top+s->rect.bottom-2);
		return True;
	}
	else if (eventId==eCEII_MOUSE_LEAVE)
	{
		cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, s, eventId);
		cyiBitmapBlender::RotateCopy(srf, s->pCurrBmp, 0, s->rect.left, s->rect.top, 0, 0, s->rect.right, s->rect.bottom);
	}
	else if (eventId==eCEII_MOUSE_LDOWN)
	{
		if (m_prevSelected)
		{
			cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, m_prevSelected, eventId);
			cyiBitmapBlender::RotateCopy(srf, m_prevSelected->pBmp[5], 0, m_prevSelected->rect.left, m_prevSelected->rect.top, 0, 0, m_prevSelected->rect.right, m_prevSelected->rect.bottom);
			m_prevSelected->pCurrBmp=m_prevSelected->pBmp[5];
		}

		cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, s, eventId);
		cyiBitmapBlender::RotateCopy(srf, s->pBmp[6], 0, s->rect.left, s->rect.top, 0, 0, s->rect.right, s->rect.bottom);
		s->pCurrBmp=s->pBmp[6];
		m_prevSelected=s;

		return True;
	}
	else if (eventId==eCEII_SELECTED)
	{
		if (m_prevSelected)
		{
			cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, m_prevSelected, eventId);
			cyiBitmapBlender::RotateCopy(srf, m_prevSelected->pBmp[5], 0, m_prevSelected->rect.left, m_prevSelected->rect.top, 0, 0, m_prevSelected->rect.right, m_prevSelected->rect.bottom);
			m_prevSelected->pCurrBmp=m_prevSelected->pBmp[5];
		}
		cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, s, eventId);
		cyiBitmapBlender::RotateCopy(srf, s->pBmp[6], 0, s->rect.left, s->rect.top, 0, 0, s->rect.right, s->rect.bottom);
		s->pCurrBmp=s->pBmp[6];
		m_prevSelected=s;

		return True;
	}
	else if (eventId==eCEII_DISABLE)
	{
		if (s->bIsEnable==True)
		{
			if (s->pCurrBmp)
			{
				cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, s, eventId);
				cyiBitmapBlender::RotateCopy(srf, s->pCurrBmp, 0, s->rect.left, s->rect.top, 0, 0, s->rect.right, s->rect.bottom);
				return True;
			}
		}

	}

	return False;
}

// ------------------------------------
Bool cyRadiox::OnCtrlxBgDraw(int id, Bitmap* srf, int x, int y)
{
	return cyctrlbase::OnCtrlxBgDraw(id, srf, x, y);
}

// ------------------------------------
Bool cyRadiox::EnableItem(Bool bIsEnable)
{
	m_cx->EnableItem(-1, bIsEnable);
	return cyctrlbase::EnableItem(bIsEnable);
}

// ------------------------------
void cyRadiox::SetValue(int n)
{
	if (m_retval)
		*m_retval=n;

	m_cx->SelectionChange(n);
}

Bool cyRadiox::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	Graphics g(hdc);

	//g.FillRectangle(&SolidBrush(Color::Black), 0, 0, m_w, m_h);
	return True;
}

LRESULT cyRadiox::OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled)
{
	*m_retval=wp;

	SendMessage(m_parent->m_hWnd, WM_COMMAND, (wp<<16|m_id), (LPARAM)m_hWnd);
	bIsHandled=True;
	return 0;
}

// ------------------------------------
void cyRadiox::Move(UINT x, UINT y)
{
	if (x==m_x &&
			y==m_y)
	{
		return;
	}
	cyctrlbase::Move(x, y);
	m_cx->Update(True, True);
	ReSize(m_cx->m_w, m_cx->m_h);
}