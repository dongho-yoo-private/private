#include "glsNameLevel.h"
#include "glsEditBox.h"
#include "glsSplitButton.h"
#include "glsCheckGroup.h"

cglsNameLevel::cglsNameLevel(const char* pszName, int level, const char* pszLevel)
{
	yiToUnicode(m_szName, pszName, 0);
	m_level=level;
	yiToUnicode(m_szLevel, pszLevel, 0);
	//cglsNameLevel(yiToUnicode(m_szName, pszName, 0), level, yiToUnicode(m_szLevel, pszLevel, 0));
}

cglsNameLevel::cglsNameLevel(const wchar_t* pszName, int level, const wchar_t* pszLevel)
{
	wcscpy(m_szName, pszName);

	//((wchar_t*)pszLevel)[0]=0;
	m_szLevel[0]=0;

	if (pszLevel)
		wcscpy(m_szLevel, pszLevel);
	m_level=level;
}

cglsNameLevel::~cglsNameLevel(void)
{
}

// ------------------------------------
void cglsNameLevel::SetNameLevel(const char* pszName, int level, const char* pszLevel)
{
	yiToUnicode(m_szName, pszName, 0);
	m_level=level;
	yiToUnicode(m_szLevel, pszLevel, 0);
	//cglsNameLevel(yiToUnicode(m_szName, pszName, 0), level, yiToUnicode(m_szLevel, pszLevel, 0));
}

// ------------------------------------
void cglsNameLevel::SetNameLevel(const wchar_t* pszName, int level, const wchar_t* pszLevel)
{
	if (pszName)
		wcscpy(m_szName, pszName);
	else
		m_szName[0]=0;

	//((wchar_t*)pszLevel)[0]=0;
	m_szLevel[0]=0;

	if (pszLevel)
	{
		wcscpy(m_szLevel, pszLevel);
	}
	else
	{
		m_szLevel[0]=0;
	}

	m_level=level;
}

// ------------------------------------
Bool cglsNameLevel::TrackPopup(HWND hWndParent, int x, int y,Font* font)
{
	m_bHasPopup=True;
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


	//m_initTime=m_time;

	if (m_children)
		m_children->clear();
	return res;
}

#include "glsSplitButton.h"

// ------------------------------------
Bool cglsNameLevel::OnCheckAmature(HWND hWnd, int code)
{
	cglsCheckGroup* check=(cglsCheckGroup*)hWnd;
	//cglsSplitButton* btn = (cglsSplitButton* )GetChild(4);
	//btn->Clear();

	//InitLevelList(check->GetChecked(-1, code));
//	btn->Update(eGUM_FORCED);

	return False;
}

// ------------------------------------
Bool cglsNameLevel::OnOk(HWND hWnd, int code)
{
	m_bIsChanged=False;
	wchar_t szName[64];

	cglsEditBox* edit = (cglsEditBox*)GetChild(2);
	edit->GetText(szName, 256);


	if (wcscmp(szName, m_szName)!=0)
	{
		wcscpy(m_szName, szName);
		m_bIsChanged=True;
	}


	cglsEditBox* edit2 = (cglsEditBox*)GetChild(5);
	edit2->GetText(szName, 256);

	if (wcscmp(szName, m_szLevel)!=0)
	{
		wcscpy(m_szLevel, szName);
		m_bIsChanged=True;
	}

	//cglsCheckGroup* check = (cglsCheckGroup*)GetChild(3);
	//Bool bIsAmature = check->GetChecked(0, 1);

	cglsSplitButton* btn = (cglsSplitButton*)GetChild(4);
	int n = btn->GetSelectIndex();

	if (n!=m_level)
	{
		m_level=n;
		m_bIsChanged=True;
	}

	
	//int nSelectedIndex = btn->GetSelectIndex();

	//if (bIsAmature==True &&
	//		nSelectedIndex== ??)
	//{

	//}

	m_bIsExitLoop=True;

	return True;
}


// ------------------------------------
Bool cglsNameLevel::OnCancel(HWND hWnd, int code)
{
	EndDialog(m_hWnd, 0);
	return True;
}


// 以下を参照
// http://homepage2.nifty.com/c_lang/sdk2/sdk_178.htm
// ------------------------------------
Bool cglsNameLevel::OnInitDialog(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	return True;
}


#include "glsSpinBox.h"
#include "glsSplitButton.h"
#include "glsCheckGroup.h"
#include "yctrlGobanResource.h"

// -----------------------------------------------------
void cglsNameLevel::InitLevelList(Bool bIsAmatuare)
{
	int i=0;
	int start=1, end=9;

	return ;
	cglsSplitButton* sBtn  = (cglsSplitButton*)GetChild(4);
	cglsCheckGroup* check = (cglsCheckGroup*)GetChild(3);
	int x = check->m_w+check->m_x;

	if (sBtn==0)
	{
		sBtn=new cglsSplitButton();
		sBtn->Create((HWND)this, 4, check->m_x+check->m_w, 0, GetFontHeight(True)*5, 0,  0);
	}

	sBtn->Clear();

	if (bIsAmatuare==False)
	{
		for (i=start; i<end+1; i++)
		{
			wchar_t buffer[16];
			cyctrlGobanResource::GetInstance().GetLevelString(((1<<6) | i), buffer);
			sBtn->AddItem(i, 0, buffer);
		}
	}
	else
	{
		start=3;
		end=28;
		for (i=start; i<end+1; i++)
		{
			wchar_t buffer[16];
			cyctrlGobanResource::GetInstance().GetLevelString(i, buffer);
			sBtn->AddItem(i, 0, buffer);
		}
	}

	//sBtn->AddItem(i, 0, L"その他");
	sBtn->Show();
	sBtn->Update(eGUM_FORCED);
}


// ---------------------------------------------------
LRESULT cglsNameLevel::OnCreate(WPARAM wParam, LPARAM lParam)
{
	int x=0;
	int h=0;
	int nMargin = GetFontHeight(True)>>1;

	cglsStatic* label = new cglsStatic();
	sGlsStaticDetail staticDetail;

	cglsStatic::xDefaultDetail(&staticDetail);
	staticDetail.id=11;
	label->Create((HWND)this, nMargin/2, nMargin/2, 0, 0, 0, L"名前: ", &staticDetail);
	label->Update(eGUM_FORCED);
	label->Show();


	cglsEditBox* edit = new cglsEditBox();
	edit->Create((HWND)this, 2, label->m_x+label->m_w, nMargin/2, GetFontHeight(True)*7, 0);
	edit->SetText(m_szName);

	x+=edit->m_w;
	h = edit->m_y+edit->m_h;//h>edit->m_h?h:edit->m_h;

	{
		//cglsCheckGroup* check = new cglsCheckGroup();
		//check->Create(this, 3, nMargin, h+(nMargin/2));
		//check->AddItem(1, L"アマ");
		//check->Show();
		//check->Update(eGUM_FORCED);
		//check->SetCheckedById(1);

		//x=check->m_x+check->m_w+(nMargin/4);
		//h = h>check->m_h?h:check->m_h;

		cglsSplitButton* xxx = new cglsSplitButton();
		xxx->Create((HWND)this, 4, nMargin, h+(nMargin/2), GetFontHeight(True)*3, 0);
		//cglsSplitButton* xxx = (cglsSplitButton* )GetChild(4);

		{
			int start=1;
			int end = 29;
			xxx->AddItem(1, 0, L"-", L"不明");
			for (int i=start; i<end+1; i++)
			{
				wchar_t buffer[16];
				cyctrlGobanResource::GetInstance().GetLevelString(((1<<6) | i), buffer);
				xxx->AddItem(i+1, 0, buffer);
			}
		}

		xxx->Update(eGUM_FORCED);
		xxx->Show();

		xxx->SelectIndex(m_level);

		//xxx->Move(x, check->m_y);
		x=xxx->m_w+xxx->m_x+nMargin/2;
		h= xxx->m_y+xxx->m_h;//h>xxx->m_h?h:xxx->m_h;

		//InitLevelList(check->GetChecked(0, 1));

		cglsEditBox* edit2 = new cglsEditBox();
		edit2->Create((HWND)this, 5, x, xxx->m_y, 80, 0);
		edit2->SetText(m_szLevel);
		//x+=edit2->m_w;
		//edit2->Enable(False);

		x=edit2->m_x+edit2->m_w;
	//edit->SetText(m_szName);
	}
	
	cglsButton* btn = new cglsButton();
	sGlsButtonDetail s;
	cglsButton::xDefaultDetail(&s);

	s.rect.right=x;
	s.rect.bottom=0;

	btn->Create((HWND)this, IDOK, nMargin/2, h+nMargin/2, 0, L"OK", 0, &s);
	btn->Update(eGUM_FORCED);
	btn->Show();

	ReSizeClientRect(x+nMargin, btn->m_y+btn->m_h+nMargin/2);

	SetFocus(edit->m_hWnd);
	
	return 0;
}

// ------------------------------------
LRESULT cglsNameLevel::OnNotHandledCommand(WPARAM wParam, LPARAM lParam)
{
	int id = (wParam&0xFFFF);

	if (id==5)
	{
		char buffer[16];
		cglsEditBox* edit = (cglsEditBox*)GetChild(id);
		int n = edit->GetText(buffer, 16);

		if (n!=0)
		{
			//GetChild(3)->Enable(False);
			GetChild(4)->Enable(False);
		}
		else
		{
			//GetChild(3)->Enable(True);
			GetChild(4)->Enable(True);
		}
	}
	return 0;
}


// ------------------------------------
Bool cglsNameLevel::OnClose(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	EndDialog(m_hWnd, 0);
	return True;
}

#define BOLDDAY(ds, iDay)  \
    if (iDay > 0 && iDay < 32)(ds) |= (0x00000001 << (iDay - 1))


// ------------------------------------
LRESULT cglsNameLevel::OnNotify(WPARAM wParam, LPARAM lParam)
{
	return True;
}

// assitant interface
// -----------------------------------------------------
Bool cglsNameLevel::ShowAssistant(HWND hWnd, int x, int y)
{
	return TrackPopup(hWnd, x, y);
	//return True;
}

const wchar_t* _level[] = {
	L"九段",
	L"八段",
	L"七段",
	L"六段",
	L"五段",
	L"四段",
	L"三段",
	L"二段",
	L"初段",
	0
};

// -----------------------------------------------------
static void GetLevelToString(int n, wchar_t* pszIn, int locale)
{
	Bool bIsAmature = IS_AMATURE(n);
	n			= GET_LEVEL_ONLY(n);

	//if (n==0)
	//{
	//	pszIn[0]=0;
	//	pszIn[1]=0;
	//	return ;
	//}

	bIsAmature=False;
	pszIn[0]=0;
SETX:
	//if (bIsAmature)
	{
		wchar_t xxx[16];
		//wcscpy(pszIn, L"アマ");
		pszIn[0]=0;
		if (n<9)
		{
			wcscat(pszIn, _level[n]);
		}
		else if (n==28)
		{
			wcscat(pszIn, L"セミプロ");
		}
		else if (n==29)
		{
			wcscat(pszIn, L"レッスンプロ");
		}
		else
		{
			wsprintfW(xxx, L"%d級", n-9+1);
			wcscat(pszIn, xxx);
		}
		return ;
	}
	//else
	//{
	//	pszIn[0]=0;
	//	if (n<10)
	//	{
	//		wcscat(pszIn, _level[n]);
	//	}
	//	else if (n==29)
	//	{
	//		wcscat(pszIn, L"セミプロ");
	//	}
	//	else if (n==30)
	//	{
	//		wcscat(pszIn, L"レッスンプロ");
	//	}
	//	else
	//	{
	//		bIsAmature=True;
	//		goto SETX;
	//	}
	//}

	return ;

}

// ------------------------------------
char* cglsNameLevel::GetName()
{
	if (m_szName[0]==0)
		return 0;
	return ::yiToMultiByteCode(0, m_szName, 0);
}

// ------------------------------------
char* cglsNameLevel::GetLevel(unsigned int& level)
{
	level=m_level;

	if (m_szLevel[0]==0)
	{
		return 0;
	}
	return ::yiToMultiByteCode(0, m_szLevel, 0);
}


// -----------------------------------------------------
Bool cglsNameLevel::GetResult(wchar_t* buffer)
{
	wchar_t _buffer[16];
	wchar_t* p = m_szLevel;
	
	if (p[0]==0)
	{
		if (m_level!=0)
		{
			GetLevelToString(m_level-1, _buffer, 0);
			p=_buffer;
		}
	}

	wsprintfW(buffer, L"%s %s", m_szName, p);//p?p:m_szLevel);
	return True;
	//eGoResult eResult = (eGoResult)((m_result>>24)&0xFF);

	//if (eResult==eGR_CANCELED)
	//{
	//	wsprintfW(buffer, L"  -  ");
	//}
	//else if (eResult==eGR_DRAW)
	//{
	//	wsprintfW(buffer, L"ジゴ");
	//}
	//else
	//{
	//	if ((m_result>>9)&0x01!=0)
	//	{
	//		wsprintfW(buffer, L"%s%s", eResult==eGR_BLACK_WIN?L"黒":L"白", L"中押し勝ち");
	//	}
	//	else
	//	{
	//		wsprintfW(buffer, L"%s %d目%s", eResult==eGR_BLACK_WIN?L"黒":L"白", (int)m_result&0xFF, ((m_result>>8)&0xFF)?L"勝ち":L"半勝ち");
	//	}
	//}
	return True;
}

// -----------------------------------------------------
Bool cglsNameLevel::GetResult(unsigned int& result1, unsigned int& result2)
{
	return False;
}

// -----------------------------------------------------
Bool cglsNameLevel::GetDefaultValue(wchar_t* buffer)
{
	buffer[0]=0;

	wsprintfW(buffer, L"%s %s", m_szName, m_szLevel);
	return GetResult(buffer);
	//eGoResult eResult = (m_result>>24)&0xFF;

	//if (eResult==eGR_CANCELED)
	//{
	//	wsprintfW(buffer, L"  -  ");
	//}
	//else if (eResult==eGR_DRAW)
	//{
	//	wsprintfW(buffer, L"ジゴ");
	//}
	//else
	//{
	//	if (m_result&0xFF)
	//	{
	//		wsprintfW(buffer, L"%s %d目%s", eResult==eGR_BLACK_WIN?L"黒":L"白", (int)m_result&0xFF, ((m_result>>8)&0xFF)?L"勝ち":L"半勝ち"));
	//	}
	//	else
	//	{
	//		wsprintfW(buffer, L"%s%s", eResult==eGR_BLACK_WIN?L"黒":L"白", L"中押し勝ち");
	//	}
	//}
	return GetResult(buffer);
}

// -----------------------------------------------------
Bool cglsNameLevel::GetDefaultValue(unsigned int& result1, unsigned int& result2)
{
	return False;
}

// -----------------------------------------------------
Bool cglsNameLevel::SetDefaultValue(wchar_t* _str)
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
Bool cglsNameLevel::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	//Graphics g(hdc);

	//LinearGradientBrush brush(Point(0, 0), Point(0, m_h), Color::Beige, Color(255,255,255,255));
	//g.FillRectangle(&brush, 0, 0, m_w, m_h);
	//g.DrawRectangle(&Pen(Color::Black), 1, 1, m_w-1, m_h-1);

	return True;
}

// -----------------------------------------------------
Bool cglsNameLevel::OnBgDraw()
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