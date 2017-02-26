#include "glsPageScroll.h"
#include "glsButton.h"

cglsPageScroll::cglsPageScroll(void)
{
}

cglsPageScroll::~cglsPageScroll(void)
{
}


// ---------------------------------------------
void cglsPageScroll::xDefaultDetail(sGlsPageScrollDetail* detail)
{
	detail->color=__COLORp__(255, 125, 255, 125);
	detail->style=eGGS_NEON;
}

// ---------------------------------------------
Bool cglsPageScroll::Create(cGlassInterface* parent, int id, int x, int y, Font* font, int nPageCount, Bool bHasArrowButton, sGlsPageScrollDetail* detail)
{
	sGlsPageScrollDetail xdetail;

	if (detail==0)
	{
		detail=&xdetail;
		xDefaultDetail(detail);
	}

	sGlsStackPannelDetail stack;

	cglsStackPannel::xDefaultDetail(&stack);
	stack.gls.font=font;
	stack.bIsFixedMode=False;
	stack.bIsShowScrollBar=False;
	stack.gls.style=detail->style;
	m_color=detail->color;

	m_nPageCount=nPageCount;
	m_bHasArrowButton=bHasArrowButton;
	return __super::Create((HWND)parent, id, 0, 0, 0, &stack);
}

// ---------------------------------------------
int cglsPageScroll::SetPageCount(int n)
{
	m_nPageCount=n;
	xCreateButtonz();
	Update(eGUM_FORCED);
	return 0;
}

// ---------------------------------------------
LRESULT cglsPageScroll::OnCommand(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int n = GetPage();
	cGlassInterface* gls = (cGlassInterface*)lParam;

	if (wParam==1)
	{
	}
	else if (wParam==2)
	{
	}
	else if (wParam>7)
	{
		return SendMessage(this->m_hNotifyWnd, WM_COMMAND, (gls->m_id<<16)|(m_id), (LPARAM)gls);
	}

	return 0;
}

#include "glsRadioGroup.h"
#include "glsResourceBitmap.h"
// ---------------------------------------------
Bool cglsPageScroll::xCreateButtonz()
{
	Clear();

	cglsButton* btn;

	sGlsButtonDetail detail;
	cglsButton::xDefaultDetail(&detail);

	if (m_bHasArrowButton)
	{
		detail.gls.style=m_guiStyle;
		detail.bHasBorder=False;

		btn = new cglsButton();
		btn->Create((HWND)this, 1, 0, 0, (Bitmap*)eGRID_ARROW_LEFT, 0, 0, &detail);
		btn->Show();

		Add(btn, 0);
		
		btn = new cglsButton();
		btn->Create((HWND)this, 2, 0, 0, (Bitmap*)eGRID_ARROW_RIGHT, 0, 0, &detail);
		btn->Update(eGUM_FORCED);
		btn->Show();

		AppSpace(GetFontHeight(), GetFontHeight());
	}

	sRadioGroupDetail radioDetail;
	cglsRadioGroup::xDefaultDetail(&radioDetail);
	
	radioDetail.bIsAutoArrange=True;
	radioDetail.colorRadio=m_color;
	radioDetail.gls.style=m_guiStyle;

	cglsRadioGroup* radio = new cglsRadioGroup();
	radio->Create(this, 3, 0, 0, &radioDetail);
	radio->Show();

	for (int i=0; i<m_nPageCount; i++)
	{
		radio->AddItem(i+1, 0);
	}

	Add(radio, eSA_CENTER);

	if (m_bHasArrowButton)
	{
		AppSpace(GetFontHeight(), GetFontHeight());
		Add(GetChild(2), 0);
	}


	return True;
}

// ---------------------------------------------
LRESULT cglsPageScroll::OnCreate(WPARAM wParam, LPARAM lParam)
{
	xCreateButtonz();
	Update(eGUM_FORCED);

	return 0;
}


// ---------------------------------------------
int cglsPageScroll::GetPage()
{
	for (int i=8; i<64; i++)
	{
		cglsButton* btn = (cglsButton*)GetChild(i);

		// ÅŒã‚Ü‚Å’B‚µ‚½
		if (btn==0)
			break;

		if (btn->IsOn()==True)
			return i-8;
	}

	return -1;
}

// ---------------------------------------------
int cglsPageScroll::SetPage(int n)
{
	for (int i=8; i<64; i++)
	{
		cglsButton* btn = (cglsButton*)GetChild(i);

		// ÅŒã‚Ü‚Å’B‚µ‚½
		if (btn==0)
			break;

		if (n==i-8)
		{
			btn->SetOnOff(True);
		}
		else
			btn->SetOnOff(False);
	}

	InvalidateRect(0);
	return 0;
}

