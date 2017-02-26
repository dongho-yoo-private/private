#include "glsResult.h"

cglsResult::cglsResult(unsigned int result, Bool bIsHalfScore)
:m_result(result)
,m_bIsHalfScore(bIsHalfScore)
{
}

cglsResult::~cglsResult(void)
{
}

// ------------------------------------
Bool cglsResult::TrackPopup(HWND hWndParent, int x, int y, 
								   float komi, Font* font)
{

	//m_result=komi;

	m_bHasPopup=True;
	Bool res = CreatePopup(hWndParent, x, y, 300, 300, font, 0);

	DestroyWindow(m_hWnd);

	//m_initTime=m_time;

	m_children->clear();

	return res;
}

#include "glsSplitButton.h"

// ------------------------------------
Bool cglsResult::OnOk(HWND hWnd, int code)
{
	Bool bIsWhite=True;
	int nScore=0;
	//Bool bIsHalf=(m_result>>8)&0xFF!=0;
	unsigned int beforResult=m_result;

	cglsSpinBox* spin=0;
	m_result=0;
	//m_result=bIsHalf==True?(1<<8):0;

	//{
		cglsSplitButton* s = (cglsSplitButton*)GetChild(2);
		int n = s->GetSelectIndex();

		if (n==5)
			n=eGR_DRAW;
		else if (n==3)
		{
			//n=eGR_BLACK_WIN;
			//m_result|=(1<<9);

			m_result|=(1<<24);
		}
		else if (n==4)
		{
			//n=eGR_WHITE_WIN;
			//m_result|=(1<<9);
			m_result|=(2<<24);
		}
		else
		{
			m_result|=(n<<24);
		}
	//}
	//if (spin=)
	//{
	//	eGoResult n = (eGoResult)spin->GetValue();// ? True:False;
	//	m_result |= n<<24;
	//}

	//if (((m_result>>9)&0x01)!=0)
	//{
	//	m_bIsExitLoop=True;
	//	return True;
	//}

	if (n==3 ||
			n==4)
	{
		m_result |=0xFF;
	}
	else if (spin=(cglsSpinBox*)GetChild(3))
	{
		 nScore = spin->GetValue();
		 m_result |= (nScore&0xFF);
	}

	//if (spin=(cglsSpinBox*)GetChild(4))
	//{
	//	bIsHalf = spin->GetValue() ? True:False;
	//}

	//m_fKomi=(float)nKomi;

	//m_fKomi+=bIsHalf?0.5:0.0;
	//m_fKomi=bIsWhite?m_fKomi:-m_fKomi;

	if (m_result!=beforResult)
	{
		m_bIsChanged=True;
	}

	m_bIsExitLoop=True;

	return True;
}

// ------------------------------------
unsigned int cglsResult::GetGoResult()
{
	return m_result;
}

// ------------------------------------
Bool cglsResult::OnCancel(HWND hWnd, int code)
{
	EndDialog(m_hWnd, 0);
	return True;
}


// ˆÈ‰º‚ðŽQÆ
// http://homepage2.nifty.com/c_lang/sdk2/sdk_178.htm
// ------------------------------------
Bool cglsResult::OnInitDialog(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	return True;
}


#include "glsSpinBox.h"
#include "glsSplitButton.h"

// ---------------------------------------------------
LRESULT cglsResult::OnCreate(WPARAM wParam, LPARAM lParam)
{
	int nMargin=GetFontHeight(True)/4;
	int x=nMargin;
	int h=x;
	int splitOffset=0;
	
	{
		cglsSplitButton* sBtn  = new cglsSplitButton();

		sBtn->Create((HWND)this, 2, x, nMargin, GetFontHeight(True)*5, 0,  0);

		sBtn->AddItem(1, 0, L"-", L"Œ‹‰Ê–³‚µ", m_result==0);
		sBtn->AddItem(2, 0, L"•Ÿ‚¿", 0, (m_result>>24)==1);
		sBtn->AddItem(3, 0, L"”’Ÿ‚¿", 0, (m_result>>24)==2);
		sBtn->AddItem(4, 0, L"•’†‰Ÿ‚µŸ‚¿", 0, ((m_result>>24)==1 && (m_result&0xFF)==0xFF));
		sBtn->AddItem(5, 0, L"”’’†‰Ÿ‚µŸ‚¿", 0, ((m_result>>24)==2 && (m_result&0xFF)==0xFF));
		sBtn->AddItem(6, 0, L"ƒWƒS", 0, (m_result>>24)==3);

		sBtn->Show();
		sBtn->Update(eGUM_FORCED);

		//cglsSpinBox* spinKomi = new cglsSpinBox();
		//eGoResult eResult = (eGoResult)(m_result>>24);
		//spinKomi->Create(this, 2, x, 0, 1);
		//spinKomi->AddString(0, L"-", 0, eResult==eGR_CANCELED);
		//spinKomi->AddString(0, L"•Ÿ", 1, eResult==eGR_BLACK_WIN);
		//spinKomi->AddString(0, L"”’Ÿ", 2, eResult==eGR_WHITE_WIN);
		//spinKomi->AddString(0, L"ƒWƒS", 3, eResult==eGR_DRAW);
		x+=sBtn->m_w;
		h = h>sBtn->m_h?h:sBtn->m_h;
		splitOffset = h+sBtn->m_y;
	}

	{
		cglsSpinBox* spinKomi = new cglsSpinBox();
		int score=0xFF&m_result;

		if (score==0xFF)
			score=0;

		spinKomi->Create(this, 3, x, splitOffset-GetFontHeight(True)-2, 3);
		spinKomi->SetNumer(0, 0, 9, (int)score%10);
		spinKomi->SetNumer(1, 0, 9, (int)((int)score%100)/10);
		spinKomi->SetNumer(2, 0, 2, (int)score/100);
		spinKomi->SetLimited(1, 25);
		spinKomi->SetLimited(0, 256);
		spinKomi->InsertLabel(L"–Ú", 0, Color::Black); 

		x+=spinKomi->m_w;
		h = h>spinKomi->m_h?h:spinKomi->m_h;
	}

	//{
	//	cglsSpinBox* spinKomi = new cglsSpinBox();
	//	spinKomi->Create(this, 4, x, 0, 1);
	//	spinKomi->AddString(0, L"@", 0, (m_fKomi-(float)((int)m_fKomi))==0.0?True:False);
	//	spinKomi->AddString(0, L"”¼", 1, (m_fKomi-(float)((int)m_fKomi))==0.0?False:True);

	//	x+=spinKomi->m_w;
	//	h = h>spinKomi->m_h?h:spinKomi->m_h;
	//}

	cglsButton* btn = new cglsButton();
	sGlsButtonDetail s;
	cglsButton::xDefaultDetail(&s);

	s.rect.right=x;
	s.rect.bottom=0;

	btn->Create((HWND)this, IDOK, nMargin, splitOffset+(nMargin/2), 0, L"OK", 0, &s);
	btn->Update(eGUM_FORCED);
	btn->Show();

	ReSizeClientRect(x+(nMargin<<1), btn->m_y+btn->m_h+nMargin);
	
	return 0;
}

// ------------------------------------
Bool cglsResult::OnClose(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	EndDialog(m_hWnd, 0);
	return True;
}

#define BOLDDAY(ds, iDay)  \
    if (iDay > 0 && iDay < 32)(ds) |= (0x00000001 << (iDay - 1))


// ------------------------------------
LRESULT cglsResult::OnNotify(WPARAM wParam, LPARAM lParam)
{
	return True;
}

// assitant interface
// -----------------------------------------------------
Bool cglsResult::ShowAssistant(HWND hWnd, int x, int y)
{
	m_bIsChanged=False;
	return TrackPopup(hWnd, x, y, m_result);
	//return True;
}

// ------------------------------------
void cglsResult::Set(unsigned int result, Bool bIsHalfScore)
{
	m_result=result;
	m_bIsHalfScore=bIsHalfScore;
}

// ------------------------------------
unsigned int cglsResult::Get()
{
	return m_result;
}


// -----------------------------------------------------
Bool cglsResult::GetResult(wchar_t* buffer)
{
	eGoResult eResult = (eGoResult)((m_result>>24)&0xFF);

	if (eResult==eGR_CANCELED)
	{
		wsprintfW(buffer, L"  -  ");
	}
	else if (eResult==eGR_DRAW || (m_bIsHalfScore==0 && (m_result&0xFF)==0))
	{
		wsprintfW(buffer, L"ƒWƒS");
	}
	else
	{
		if ((m_result&0xFF)==0xFF)
		{
			wsprintfW(buffer, L"%s%s", eResult==eGR_BLACK_WIN?L"•":L"”’", L"’†‰Ÿ‚µŸ‚¿");
		}
		else if ((m_result&0xFF)==0)
		{
			wsprintfW(buffer, L"%s %s", eResult==eGR_BLACK_WIN?L"•":L"”’", m_bIsHalfScore?L"”¼–ÚŸ‚¿":L"ƒWƒS");
		}
		else if ((m_result&0xFF)==0xFE)
		{
			wsprintfW(buffer, L"%s %s", eResult==eGR_BLACK_WIN?L"•":L"”’", L"ŽžŠÔŸ‚¿");
		}
		else
		{
			//if ((m_result&0xFF)==0)
			//{
			//	wsprintfW(buffer, L"%s ”¼–ÚŸ‚¿", eResult==eGR_BLACK_WIN?L"•":L"”’");//, (int)m_result&0xFF, m_bIsHalfScore?L"Ÿ‚¿":L"”¼Ÿ‚¿");
			//}
			//else
			//{
				wsprintfW(buffer, L"%s %d–Ú%s", eResult==eGR_BLACK_WIN?L"•":L"”’", (int)m_result&0xFF, m_bIsHalfScore?L"”¼Ÿ‚¿":L"Ÿ‚¿");
			//}
		}
	}
	return True;
}

// -----------------------------------------------------
Bool cglsResult::GetResult(unsigned int& result1, unsigned int& result2)
{
	return False;
}

// -----------------------------------------------------
Bool cglsResult::GetDefaultValue(wchar_t* buffer)
{
	//eGoResult eResult = (m_result>>24)&0xFF;

	//if (eResult==eGR_CANCELED)
	//{
	//	wsprintfW(buffer, L"  -  ");
	//}
	//else if (eResult==eGR_DRAW)
	//{
	//	wsprintfW(buffer, L"ƒWƒS");
	//}
	//else
	//{
	//	if (m_result&0xFF)
	//	{
	//		wsprintfW(buffer, L"%s %d–Ú%s", eResult==eGR_BLACK_WIN?L"•":L"”’", (int)m_result&0xFF, ((m_result>>8)&0xFF)?L"Ÿ‚¿":L"”¼Ÿ‚¿"));
	//	}
	//	else
	//	{
	//		wsprintfW(buffer, L"%s%s", eResult==eGR_BLACK_WIN?L"•":L"”’", L"’†‰Ÿ‚µŸ‚¿");
	//	}
	//}
	return GetResult(buffer);
}

// -----------------------------------------------------
Bool cglsResult::GetDefaultValue(unsigned int& result1, unsigned int& result2)
{
	return False;
}

// -----------------------------------------------------
Bool cglsResult::SetDefaultValue(wchar_t* _str)
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
Bool cglsResult::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	//Graphics g(hdc);

	//LinearGradientBrush brush(Point(0, 0), Point(0, m_h), Color::Beige, Color(255,255,255,255));
	//g.FillRectangle(&brush, 0, 0, m_w, m_h);
	//g.DrawRectangle(&Pen(Color::Black), 1, 1, m_w-1, m_h-1);

	return True;
}

// -----------------------------------------------------
Bool cglsResult::OnBgDraw()
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