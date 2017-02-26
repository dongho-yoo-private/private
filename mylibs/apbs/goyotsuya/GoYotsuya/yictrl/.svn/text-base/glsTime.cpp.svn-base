#include "glsTime.h"

cglsTime::cglsTime(eGlsTimeFlag eFlag, unsigned int initTime)
:m_eFlag(eFlag)
,m_initTime(initTime)
{
	if (m_initTime==0)
	{
		SYSTEMTIME t;
		GetLocalTime(&t);
		m_initTime=MAKETIME(2009, 0, 0, t.wHour, t.wMinute, t.wSecond);
	}
}

cglsTime::~cglsTime(void)
{
}

// ------------------------------------
Bool cglsTime::DoModal(HWND hWndParent, char* pszTitle, int x, int y, 
								unsigned int day, eGlsTimeFlag mode, Font* font)
{
	m_eFlag=mode;
	if (day)
	{
		m_initTime=day;
	}
	return cglsWindow::DoModal(hWndParent, 0, pszTitle, x, y, 100, 100, font);
}

// ------------------------------------
Bool cglsTime::TrackPopup(HWND hWndParent, int x, int y, 
								   unsigned int day, eGlsTimeFlag mode, Font* font)
{
	m_eFlag=mode;
	
	if (day)
	{
		m_time=m_initTime=day;
	}

	Bool res = CreatePopup(hWndParent, x, y, 100, 100, font, 0);

	DestroyWindow(m_hWnd);


	//for (int i=1; i<6; i++)
	//{
	//	cGlassInterface* itf = GetChild(i);
	//	delete itf;
	//}

	m_children->clear();
	return res;
}

// ------------------------------------
Bool cglsTime::OnOk(HWND hWnd, int code)
{
	Bool bIsPM = False;
	int h=0, m=0, s=0;

	cglsSpinBox* spin=0;

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
	m_bIsExitLoop=True;

	return True;
}

// ------------------------------------
time64_t cglsTime::GetSelectedTime()
{
	return m_time;
}

// ------------------------------------
Bool cglsTime::OnCancel(HWND hWnd, int code)
{
	EndDialog(m_hWnd, 0);
	return True;
}


// ˆÈ‰º‚ðŽQÆ
// http://homepage2.nifty.com/c_lang/sdk2/sdk_178.htm
// ------------------------------------
Bool cglsTime::OnInitDialog(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	return True;
}


#include "glsSpinBox.h"

// ---------------------------------------------------
LRESULT cglsTime::OnCreate(WPARAM wParam, LPARAM lParam)
{
	int x=0;
	int h=0;

	if (m_eFlag&eGTF_AMPM)
	{
		cglsSpinBox* ampm = new cglsSpinBox();
		ampm->Create(this, 2, x, 0, 1);
		ampm->AddString(0, L"AM", 0, True);
		ampm->AddString(0, L"PM", 1);

		//ampm->Show();
		x+=ampm->m_w;

		h = h>ampm->m_h?h:ampm->m_h;

	}

	if (m_eFlag&eGTF_HOUR)
	{
		cglsSpinBox* spinHour = new cglsSpinBox();
		spinHour->Create(this, 3, x, 0, 1);
		spinHour->SetNumer(0, 0, 12, xHOUR(m_initTime));
		x+=spinHour->m_w;
		h = h>spinHour->m_h?h:spinHour->m_h;

	}

	if (m_eFlag&eGTF_MINUTE)
	{
		cglsSpinBox* spinMin = new cglsSpinBox();
		spinMin->Create(this, 4, x, 0, 2);
		spinMin->SetNumer(0, 0, 9, xMINUTE(m_initTime)%10);
		spinMin->SetNumer(1, 0, 5, xMINUTE(m_initTime)/10);
		x+=spinMin->m_w;
		h = h>spinMin->m_h?h:spinMin->m_h;
	}

	if (m_eFlag&eGTF_SECOND)
	{
		cglsSpinBox* spinSec = new cglsSpinBox();
		spinSec->Create(this, 5, x, 0, 2);
		spinSec->SetNumer(0, 0, 9, xSECOND(m_initTime)%10);
		spinSec->SetNumer(1, 0, 5, xSECOND(m_initTime)/10);
		x+=spinSec->m_w;
		h = h>spinSec->m_h?h:spinSec->m_h;
	}
	
	cglsButton* btn = new cglsButton();
	sGlsButtonDetail s;
	cglsButton::xDefaultDetail(&s);

	s.rect.right=x;
	s.rect.bottom=0;

	//wchar_t buffer[128];

	//cyiTime::TimeToString(m_initTime, buffer, "YYYY-MM-DD");

	btn->Create((HWND)this, IDOK, 0, h, 0, L"OK", 0, &s);
	btn->Update(eGUM_FORCED);
	btn->Show();

	ReSizeClientRect(x, h+btn->m_h);
	
	return 0;
}

// ------------------------------------
Bool cglsTime::OnClose(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	EndDialog(m_hWnd, 0);
	return True;
}

#define BOLDDAY(ds, iDay)  \
    if (iDay > 0 && iDay < 32)(ds) |= (0x00000001 << (iDay - 1))


// ------------------------------------
LRESULT cglsTime::OnNotify(WPARAM wParam, LPARAM lParam)
{
	return True;
}

// assitant interface
// -----------------------------------------------------
Bool cglsTime::ShowAssistant(HWND hWnd, int x, int y)
{
	return TrackPopup(hWnd, x, y, m_initTime);
	//return True;
}

// -----------------------------------------------------
Bool cglsTime::GetResult(wchar_t* buffer)
{
	wsprintfW(buffer, L"%02d:%02d:%02d",xHOUR(m_time), xMINUTE(m_time), xSECOND(m_time));
	return True;
}

// -----------------------------------------------------
Bool cglsTime::GetResult(unsigned int& result1, unsigned int& result2)
{
	return False;
}

// -----------------------------------------------------
Bool cglsTime::GetDefaultValue(wchar_t* buffer)
{
	wsprintfW(buffer, L"%02d:%02d:%02d", xHOUR(m_initTime), xMINUTE(m_initTime), xSECOND(m_initTime));//xYEAR64(m_initTime), xMONTH64(m_initTime), xDAY64(m_initTime));
	return True;
}

// -----------------------------------------------------
Bool cglsTime::GetDefaultValue(unsigned int& result1, unsigned int& result2)
{
	return False;
}

// -----------------------------------------------------
Bool cglsTime::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	//Graphics g(hdc);

	//LinearGradientBrush brush(Point(0, 0), Point(0, m_h), Color::Beige, Color(255,255,255,255));
	//g.FillRectangle(&brush, 0, 0, m_w, m_h);
	//g.DrawRectangle(&Pen(Color::Black), 1, 1, m_w-1, m_h-1);

	return True;
}

// -----------------------------------------------------
Bool cglsTime::OnBgDraw()
{
	::InvalidateRect(m_hWnd, 0, FALSE);
	return False;
}