#include "glsKomi.h"

cglsKomi::cglsKomi(float fDefaultKomi)
:m_fKomi(fDefaultKomi)
{
}

cglsKomi::~cglsKomi(void)
{
}

// ------------------------------------
void cglsKomi::SetKomi(float fKomi)
{
	m_fKomi=fKomi;
}


// ------------------------------------
Bool cglsKomi::TrackPopup(HWND hWndParent, int x, int y, 
								   float komi, Font* font)
{
	m_fKomi=komi;

	m_bIsChanged=False;
	Bool res = CreatePopup(hWndParent, x, y, 300, 300, font, 0);

	if (m_children)
	{
		cGlassInterface* p=0;
		while (p=(cGlassInterface*)m_children->removeEx(0))
		{
			p->Destroy();
			//delete p;
		}
		//cglsButton* btn = (cglsButton* )m_children->removeEx(0);
		//delete btn;
	}
	DestroyWindow(m_hWnd);
	m_hWnd=0;
	//m_initTime=m_time;

	m_children->clear();
	return res;
}

// ------------------------------------
Bool cglsKomi::OnOk(HWND hWnd, int code)
{
	Bool bIsWhite=True;
	int nKomi=0;
	Bool bIsHalf=True;
	float fKomi=m_fKomi;

	cglsSpinBox* spin=0;

	if (spin=(cglsSpinBox*)GetChild(2))
	{
		bIsWhite = spin->GetValue() ? True:False;
	}

	if (spin=(cglsSpinBox*)GetChild(3))
	{
		 nKomi = spin->GetValue();
	}

	if (spin=(cglsSpinBox*)GetChild(4))
	{
		bIsHalf = spin->GetValue() ? True:False;
	}

	m_fKomi=(float)nKomi;

	m_fKomi+=bIsHalf?0.5:0.0;
	m_fKomi=bIsWhite?m_fKomi:-m_fKomi;

	if (m_fKomi!=fKomi)
	{
		m_bIsChanged=True;
	}
	m_bIsExitLoop=True;

	return True;
}

// ------------------------------------
float cglsKomi::GetKomi()
{
	return m_fKomi;
}

// ------------------------------------
Bool cglsKomi::OnCancel(HWND hWnd, int code)
{
	EndDialog(m_hWnd, 0);
	return True;
}


// 以下を参照
// http://homepage2.nifty.com/c_lang/sdk2/sdk_178.htm
// ------------------------------------
Bool cglsKomi::OnInitDialog(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	return True;
}


#include "glsSpinBox.h"

// ---------------------------------------------------
LRESULT cglsKomi::OnCreate(WPARAM wParam, LPARAM lParam)
{
	int x=0;
	int y=0;
	int h=0;
	int nMargin = GetFontHeight(True)/4;

	y=x=nMargin;

	{
		cglsSpinBox* spinKomi = new cglsSpinBox();
		spinKomi->Create(this, 2, x, y, 1);
		spinKomi->AddString(0, L"黒", 0, m_fKomi<0.0?True:False);
		spinKomi->AddString(0, L"白", 1, m_fKomi<0.0?False:True);
		x+=spinKomi->m_w+nMargin;
		h = h>spinKomi->m_h?h:spinKomi->m_h;
	}

	{
		cglsSpinBox* spinKomi = new cglsSpinBox();
		spinKomi->Create(this, 3, x, y, 3);
		spinKomi->SetNumer(0, 0, 9, (int)m_fKomi%10);
		spinKomi->SetNumer(1, 0, 9, (int)((int)m_fKomi%100)/10);
		spinKomi->SetNumer(2, 0, 1, (int)m_fKomi/100);
		spinKomi->SetLimited(1, 16);

		x+=spinKomi->m_w+nMargin;
		h = h>spinKomi->m_h?h:spinKomi->m_h;
	}

	{
		cglsSpinBox* spinKomi = new cglsSpinBox();
		spinKomi->Create(this, 4, x, y, 1);
		spinKomi->AddString(0, L"　", 0, (m_fKomi-(float)((int)m_fKomi))==0.0?True:False);
		spinKomi->AddString(0, L"半", 1, (m_fKomi-(float)((int)m_fKomi))==0.0?False:True);

		x+=spinKomi->m_w;
		h = h>spinKomi->m_h?h:spinKomi->m_h;
	}

	cglsButton* btn = new cglsButton();
	sGlsButtonDetail s;
	cglsButton::xDefaultDetail(&s);

	s.rect.right=x-nMargin;
	s.rect.bottom=0;

	h+=nMargin;
	btn->Create((HWND)this, IDOK, nMargin, h+(nMargin/2), 0, L"OK", 0, &s);
	btn->Update(eGUM_FORCED);
	btn->Show();

	ReSizeClientRect(x+nMargin, h+btn->m_h+nMargin);
	::InvalidateRect(m_hWnd, 0, False);
	InvalidateRect(0);
	
	return 0;
}

// ------------------------------------
Bool cglsKomi::OnClose(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	EndDialog(m_hWnd, 0);
	return True;
}

#define BOLDDAY(ds, iDay)  \
    if (iDay > 0 && iDay < 32)(ds) |= (0x00000001 << (iDay - 1))


// ------------------------------------
LRESULT cglsKomi::OnNotify(WPARAM wParam, LPARAM lParam)
{
	return True;
}

// assitant interface
// -----------------------------------------------------
Bool cglsKomi::ShowAssistant(HWND hWnd, int x, int y)
{
	m_bIsChanged=False;
	return TrackPopup(hWnd, x, y, m_fKomi);
	//return True;
}

// -----------------------------------------------------
Bool cglsKomi::GetResult(wchar_t* buffer)
{
	if (m_fKomi==0.0)
	{
		wsprintfW(buffer, L"コミなし");
	}
	else
	{
		wsprintfW(buffer, L"%s %d目%s", m_fKomi<0?L"黒":L"白", (int)(m_fKomi<0.0?-m_fKomi:m_fKomi), ((float)(m_fKomi-(int)m_fKomi)==0.0?L"　":L"半"));
	}
	return True;
}

// -----------------------------------------------------
Bool cglsKomi::GetResult(unsigned int& result1, unsigned int& result2)
{
	result1 = (unsigned int)m_fKomi;

	result2 = ((m_fKomi-(float)result1)==0.0)?0:1;
	return True;
}

// -----------------------------------------------------
Bool cglsKomi::GetDefaultValue(wchar_t* buffer)
{
	if (m_fKomi==0.0)
	{
		wsprintfW(buffer, L"コミなし");
	}
	else
	{
		wsprintfW(buffer, L"%s %d目%s", m_fKomi<0?L"黒":L"白", (int)(m_fKomi<0.0?-m_fKomi:m_fKomi), ((float)(m_fKomi-(int)m_fKomi)==0.0?L"　":L"半"));
	}
	return True;
}

// -----------------------------------------------------
Bool cglsKomi::GetDefaultValue(unsigned int& result1, unsigned int& result2)
{
	return False;
}

// -----------------------------------------------------
Bool cglsKomi::SetDefaultValue(wchar_t* _str)
{
/*	int h, m, s;
	char* szstr=yiToMultiByteCode(0, _str, 0);

	if (szstr==0)
		return False;

	sscanf(szstr, "%d:%d:%d", &h, &m, &s);
	m_initTime=MAKETIME(0, 0, 0, h, m, s);

	yifree(szstr);*/
	return True;
}

// -----------------------------------------------------
Bool cglsKomi::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	//Graphics g(hdc);

	//LinearGradientBrush brush(Point(0, 0), Point(0, m_h), Color::Beige, Color(255,255,255,255));
	//g.FillRectangle(&brush, 0, 0, m_w, m_h);
	//g.DrawRectangle(&Pen(Color::Black), 1, 1, m_w-1, m_h-1);

	return True;
}

// -----------------------------------------------------
//Bool cglsKomi::OnBgDraw()
//{
//	//::InvalidateRect(m_hWnd, 0, FALSE);
//	return __super::OnBgDraw();
//	return False;
//}

// ---------------------------------------
Bool cglsKomi::OnBgDraw()
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
	g.FillRectangle(&LinearGradientBrush(Point(0, 0), Point(0, m_h-1), Color::White, Color::Beige), 0, 0, m_w, m_h);
	g.DrawRectangle(&Pen(Color::Black), 0, 0, m_w-1, m_h-1);

	cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);

	//NotifyBgChanged();

	m_eUpdateReason=eGUR_BG_CHANGED;
	Update(m_bmpCurrent, 0);
	InvalidateRect(0);//, True);
	return True;
}