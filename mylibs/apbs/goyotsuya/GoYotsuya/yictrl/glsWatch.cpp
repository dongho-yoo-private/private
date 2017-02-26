#include "glsWatch.h"

cglsWatch::cglsWatch(eglsWatchFlag eFlag, unsigned int initTime, unsigned int remainSec)
:m_eFlag(eFlag)
,m_initTime(initTime)
,m_remainSec(remainSec)
{
	if (m_initTime==0)
	{
		SYSTEMTIME t;
		GetLocalTime(&t);
		m_initTime=MAKETIME(2009, 0, 0, t.wHour, t.wMinute, t.wSecond);
	}
}

cglsWatch::~cglsWatch(void)
{
}

void cglsWatch::Set(unsigned int time, unsigned int sec)
{
	m_remainSec=sec;
	m_initTime=time;
	m_time=m_initTime;
}

// ------------------------------------
Bool cglsWatch::DoModal(HWND hWndParent, char* pszTitle, int x, int y, 
								unsigned int day, eglsWatchFlag mode, Font* font)
{
	m_eFlag=mode;
	if (day)
	{
		m_initTime=day;
	}
	return cglsWindow::DoModal(hWndParent, 0, pszTitle, x, y, 100, 100, font);
}

// ------------------------------------
Bool cglsWatch::TrackPopup(HWND hWndParent, int x, int y, 
								   unsigned int day, eglsWatchFlag mode, Font* font)
{
	m_eFlag=mode;
	
	if (day)
	{
		m_time=m_initTime=day;
	}

	Bool res = CreatePopup(hWndParent, x, y, 500, 500, font, 0);

	DestroyWindow(m_hWnd);

	m_initTime=m_time;

	m_children->clear();
	return res;
}

// ------------------------------------
Bool cglsWatch::OnOk(HWND hWnd, int code)
{
	Bool bIsPM = False;
	int h=0, m=0, s=0;
	unsigned int r1=m_time;
	unsigned int r2=m_remainSec;
	cglsSpinBox* spin=0;

	m_time=0;
	m_remainSec=0;
	m_bIsChanged=True;

	if (spin=(cglsSpinBox*)GetChild(2))
	{
		 bIsPM = spin->GetValue();
	}

	if (spin=(cglsSpinBox*)GetChild(3))
	{
		 h = spin->GetValue();
		 h+=bIsPM?12:0;
	}

	if (spin=(cglsSpinBox*)GetChild(4))
	{
		m = spin->GetValue();
	}

	if (spin=(cglsSpinBox*)GetChild(5))
	{
		s = spin->GetValue();
	}

	m_time=MAKETIME(0, 0, 0, h, m, s);
	//SetFocus(GetParent(m_hWnd));

	// ’·‚³
	if (spin=(cglsSpinBox*)GetChild(6))
	{
		int n = spin->GetValue();
		m_remainSec=n;
	}

	// ‰ñ”
	if (spin=(cglsSpinBox*)GetChild(7))
	{
		int n = spin->GetValue();
		m_remainSec|=(n<<24);
	}

	m_bIsExitLoop=True;

	if (r1==m_time &&
			r2==m_remainSec)
	{
		m_bIsChanged=False;
	}

	return True;
}

// ------------------------------------
time64_t cglsWatch::GetSelectedTime()
{
	return m_time;
}

// ------------------------------------
Bool cglsWatch::OnCancel(HWND hWnd, int code)
{
	EndDialog(m_hWnd, 0);
	return True;
}


// ˆÈ‰º‚ðŽQÆ
// http://homepage2.nifty.com/c_lang/sdk2/sdk_178.htm
// ------------------------------------
Bool cglsWatch::OnInitDialog(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	return True;
}


#include "glsSpinBox.h"

// ---------------------------------------------------
LRESULT cglsWatch::OnCreate(WPARAM wParam, LPARAM lParam)
{
	int x=3;
	int h=3;

	int nSpinOffset=0;
	int nMaxWidth=0;

	/*if (m_eFlag&eGTF_AMPM)
	{
		cglsSpinBox* ampm = new cglsSpinBox();
		ampm->Create(this, 2, x, 0, 1);
		ampm->AddString(0, L"AM", 0, True);
		ampm->AddString(0, L"PM", 1);

		//ampm->Show();
		x+=ampm->m_w;

		h = h>ampm->m_h?h:ampm->m_h;

	}*/

	cglsStatic* label = new cglsStatic();
	label->Create((HWND)this, x, h, 0, 0, 0, L"Ž‚¿ŽžŠÔ");
	label->Show();

	x+=label->m_w+3;

	if (m_eFlag&eGTF_HOUR)
	{
		cglsSpinBox* spinHour = new cglsSpinBox();
		spinHour->Create(this, 3, x, h, 2);
		spinHour->SetNumer(0, 0, 9, xHOUR(m_initTime)%10);
		spinHour->SetNumer(1, 0, 4, xHOUR(m_initTime)/10);
		spinHour->InsertLabel(L"Žž", -1, Color::Black);
		x+=spinHour->m_w;
		nSpinOffset=spinHour->m_x;
		//h = h>spinHour->m_h?h:spinHour->m_h;


	}

	if (m_eFlag&eGTF_MINUTE)
	{
		cglsSpinBox* spinMin = new cglsSpinBox();
		spinMin->Create(this, 4, x, h, 2);
		spinMin->SetNumer(0, 0, 9, xMINUTE(m_initTime)%10);
		spinMin->SetNumer(1, 0, 5, xMINUTE(m_initTime)/10);
		spinMin->InsertLabel(L"•ª", -1, Color::Black);
		x+=spinMin->m_w;
		//h = h>spinMin->m_h?h:spinMin->m_h;
	}

	if (m_eFlag&eGTF_SECOND)
	{
		cglsSpinBox* spinSec = new cglsSpinBox();
		spinSec->Create(this, 5, x, h, 2);
		spinSec->SetNumer(0, 0, 9, xSECOND(m_initTime)%10);
		spinSec->SetNumer(1, 0, 5, xSECOND(m_initTime)/10);
		x+=spinSec->m_w;
		//h = h>spinSec->m_h?h:spinSec->m_h;
		nMaxWidth=spinSec->m_x+spinSec->m_w;

	}
	h+=label->m_h+3;

	sGlsButtonDetail s;	
	s.rect.right=x;
	x=3;
	label = new cglsStatic();
	label->Create((HWND)this, x, h, 0, 0, 0, L"•b“Ç‚Ý");
	label->Show();
	x+=label->m_w+3;

	// •b“Ç‚Ý’·‚³
	{
		cglsSpinBox* spinSecRead = new cglsSpinBox();
		spinSecRead->Create(this, 6, nSpinOffset/*x*/, h, 3);

		int n = m_remainSec&0x00FFFFFF;
		spinSecRead->SetNumer(0, 0, 9, xSECOND(n)%10);
		spinSecRead->SetNumer(1, 0, 9, (xSECOND(n)%100)/10);
		spinSecRead->SetNumer(2, 0, 3, xSECOND(n)/100);
		x=nSpinOffset+spinSecRead->m_w;
		//h = h>spinSecRead->m_h?h:spinSecRead->m_h;

	}

	label = new cglsStatic();
	label->Create((HWND)this, x, h, 0, 0, 0, L"•b");
	label->Show();
	x+=label->m_w+3;

	// •b“Ç‚Ý‰ñ”
	{
		int n = m_remainSec>>24;
		cglsSpinBox* spinSecReadCount = new cglsSpinBox();
		spinSecReadCount->Create(this, 7, x, h, 2);
		spinSecReadCount->SetNumer(0, 0, 9, xSECOND(n)%10);
		spinSecReadCount->SetNumer(1, 0, 9, xSECOND(n)/10);
		x+=spinSecReadCount->m_w;
		//h = h>spinSecReadCount->m_h?h:spinSecReadCount->m_h;

	}
	label = new cglsStatic();
	label->Create((HWND)this, x, h, 0, 0, 0, L"‰ñ");
	label->Show();
	x+=label->m_w+3;
	h+=label->m_h;

	cglsButton* btn = new cglsButton();

	cglsButton::xDefaultDetail(&s);


	s.rect.right=x>s.rect.right?x:s.rect.right;
	s.rect.bottom=0;

	//wchar_t buffer[128];

	//cyiTime::TimeToString(m_initTime, buffer, "YYYY-MM-DD");

	btn->Create((HWND)this, IDOK, 3, h+3, 0, L"OK", 0, &s);
	btn->Update(eGUM_FORCED);
	btn->Show();

	ReSizeClientRect(nMaxWidth+6, h+btn->m_h+6);
	
	return 0;
}

// ------------------------------------
Bool cglsWatch::OnClose(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	EndDialog(m_hWnd, 0);
	return True;
}

#define BOLDDAY(ds, iDay)  \
    if (iDay > 0 && iDay < 32)(ds) |= (0x00000001 << (iDay - 1))


// ------------------------------------
LRESULT cglsWatch::OnNotify(WPARAM wParam, LPARAM lParam)
{
	return True;
}

// assitant interface
// -----------------------------------------------------
Bool cglsWatch::ShowAssistant(HWND hWnd, int x, int y)
{
	return TrackPopup(hWnd, x, y, m_initTime);
	//return True;
}

// -----------------------------------------------------
Bool cglsWatch::GetResult(wchar_t* buffer)
{
	//wsprintfW(buffer, L"%02d:%02d:%02d",xHOUR(m_time), xMINUTE(m_time), xSECOND(m_time));
	if (m_remainSec)
	{
		wsprintfW(buffer, L"%02d:%02d:%02d   %d•b •b“Ç‚Ý%d‰ñ", xHOUR(m_time), xMINUTE(m_time), xSECOND(m_time),
				m_remainSec&0x00FFFFFF, (m_remainSec>>24)&0xFF);
	}
	else
	{
		wsprintfW(buffer, L"%02d:%02d:%02d", xHOUR(m_time), xMINUTE(m_time), xSECOND(m_time));
	}
	return True;
}

// -----------------------------------------------------
Bool cglsWatch::GetResult(unsigned int& result1, unsigned int& result2)
{
	result1=m_time;
	result2=m_remainSec;
	return False;
}

// -----------------------------------------------------
Bool cglsWatch::GetDefaultValue(wchar_t* buffer)
{
	if (m_remainSec)
	{
		wsprintfW(buffer, L"%02d:%02d:%02d   %d•b •b“Ç‚Ý%d‰ñ", xHOUR(m_initTime), xMINUTE(m_initTime), xSECOND(m_initTime),
				m_remainSec&0x00FFFFFFFF, (m_remainSec>>24)&0xFF);
	}
	else
	{
		wsprintfW(buffer, L"%02d:%02d:%02d", xHOUR(m_initTime), xMINUTE(m_initTime), xSECOND(m_initTime));
	}
	return True;
}

// -----------------------------------------------------
Bool cglsWatch::GetDefaultValue(unsigned int& result1, unsigned int& result2)
{
	return False;
}

// -----------------------------------------------------
Bool cglsWatch::SetDefaultValue(wchar_t* _str)
{
	int h, m, s;
	char* szstr=yiToMultiByteCode(0, _str, 0);

	if (szstr==0)
		return False;

	sscanf(szstr, "%d:%d:%d", &h, &m, &s);
	m_initTime=MAKETIME(0, 0, 0, h, m, s);

	yifree(szstr);
	return True;
}

// -----------------------------------------------------
Bool cglsWatch::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	//Graphics g(hdc);

	//LinearGradientBrush brush(Point(0, 0), Point(0, m_h), Color::Beige, Color(255,255,255,255));
	//g.FillRectangle(&brush, 0, 0, m_w, m_h);
	//g.DrawRectangle(&Pen(Color::Black), 1, 1, m_w-1, m_h-1);

	return True;
}

// -----------------------------------------------------
//Bool cglsWatch::OnBgDraw()
//{
//	//::InvalidateRect(m_hWnd, 0, FALSE);
//	return __super::OnBgDraw();
//	return False;
//}

// ---------------------------------------
Bool cglsWatch::OnBgDraw()
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