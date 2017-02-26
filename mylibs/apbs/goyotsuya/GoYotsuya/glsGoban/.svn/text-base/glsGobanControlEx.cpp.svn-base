#include "glsGobanControlEx.h"
#include "glsButton.h"
#include "glsSplitButton.h"
#include "yiGoCommandz.h"
#include "glsResourceBitmap.h"

typedef struct {
	int id;
	unsigned int bmpId;
	const wchar_t* pszDisplay;
	const wchar_t* pszDefault;
} sSplitButtonItem;

sSplitButtonItem g_AutoplaySplit[] = {
	{1, 0, L"éËìÆ", L"éËìÆçƒê∂"},
	{2, 0, L"1ïb", L"1ïbä‘äu"},
	{3, 0, L"3ïb", L"3ïbä‘äu"},
	{5, 0, L"5ïb", L"5ïbä‘äu"},
	{6, 0, L"10ïb", L"10ïbä‘äu"},
	{7, 0, L"30ïb", L"30ïbä‘äu"},
	{8, 0, L"1ï™", L"1ï™ä‘äu"},
	{9, 0, L"3ï™", L"3ï™ä‘äu"},
	{0, 0, 0, 0}

};

cglsGobanControl::cglsGobanControl(void)
{
}

cglsGobanControl::~cglsGobanControl(void)
{
}



// -------------------------------------------------
Bool cglsGobanControl::Create(cGlassInterface* parent, int id, int w, int h, eGobanControlSet set, Font* font)
{
	sGlsStackPannelDetail panelDetail;
	cglsStackPannel::xDefaultDetail(&panelDetail);

	panelDetail.bUseGaussPannel=False;
	panelDetail.gls.font=font;
	panelDetail.bIsFixedMode=True;
	//panelDetail.gauss.colorTop=__COLORp__(127, 0, 0, 0);
	//panelDetail.gauss.colorBottom=__COLORp__(200, 0, 0, 0);
	m_set=set;
	RECT rect = {0, 0, w, h};
	RECT margin = {3, 3, 3, 3};
	if (cglsStackPannel::Create((HWND)parent, id, &rect, &margin, 0, &panelDetail)==False)
	{
		return False;
	}

	m_bIsCreated=True;
	return True;
}

// -----------------------------------------
void cglsGobanControl::xAddButton(int id, int bitmapId, wchar_t* pszText, wchar_t* comment, int systemId, Bool bIsOnOff, int style)
{
	sDrawItem* child=GetItem(id);
	sGlsButtonDetail detail;
	cglsButton::xDefaultDetail(&detail);

	//detail.gls.style=(eGlsGuiStyle)style;
	detail.bIsOnOff=bIsOnOff;
	//detail.gls.font = cyiShape::GetFontByHeight((int)((float)GetFontHeight(True)*1.0), L"Meiryo");
	if (child==0)
	{
		cglsButton* btn = new cglsButton();
		btn->Create((HWND)this, id, 0, 0, (Bitmap*)bitmapId, pszText, comment, &detail);		
		Add(btn, 0);

		if (systemId)
		{
			cglsSystem* system = GetSystem();

			if (system)
			{
				system->RegisterCommand(btn, systemId);
			}
		}
	}

	ShowItem(True, id);
	return ;
}

// -------------------------------------------------
void cglsGobanControl::xAddSplitButton(int id, int systemId, void* p, int width, const wchar_t* pszToolTip)
{
	sSplitButtonItem* s = (sSplitButtonItem*)p;
	
	sGlsSplitButtonDetail detail;

	cglsSplitButton::xDefaultDetail(&detail);

	cglsSplitButton* btn = new cglsSplitButton();
	if (btn->Create((HWND)this, id, 0, 0, width, GetFontHeight(), pszToolTip)==False)
		return ;
	
	int cnt=0;
	while(s[cnt].id)
	{
		btn->AddItem(s[cnt].id, (Bitmap*)s[cnt].bmpId, s[cnt].pszDisplay, s[cnt].pszDefault);
		cnt++;
	}

	btn->Show();
	btn->Update(eGUM_FORCED);
	Add(btn, 0);
	cglsSystem* system = GetSystem();

	if (system)
	{
		system->RegisterCommand(btn, systemId, id);
	}
	ShowItem(True, id);

	return ;
}



// -------------------------------------------------
// top.
// pprev
// prev
// next
// nnext
// end
// pass
// show no.
// auto play.
Bool cglsGobanControl::xCreateBase()
{
	//eGCC_TOP = 0x1000,
	//eGCC_PP,
	//eGCC_PREV,
	//eGCC_NEXT,
	//eGCC_NN,
	//eGCC_END,
	//eGCC_SHOW_NO,
	//eGCC_NEXT_BRANCH,
	//eGCC_PREV_BRANCH,
	int margin = GetFontHeight()>>1;

	xAddButton(eGCC_TOP, eGRID_TOP, 0, L"êÊì™Ç÷(HomeÉ{É^Éì)", eGCID_TOP);
	xAddButton(eGCC_PP, eGRID_PP, 0, L"êîéËëOÇ÷", eGCID_PP);
	xAddButton(eGCC_PREV, eGRID_PREV, 0, L"àÍéËëOÇ÷(Å©É{É^Éì)", eGCID_PREV);
	xAddButton(eGCC_NEXT, eGRID_NEXT, 0, L"éüÇ÷(Å®É{É^Éì)", eGCID_NEXT);
	xAddButton(eGCC_NN, eGRID_NN, 0, L"êîéËêÊÇ÷", eGCID_NN);
	xAddButton(eGCC_END, eGRID_END, 0, L"ç≈èIéËÇ÷(EndÉ{É^Éì)", eGCID_NN);

	AppSpace(margin, GetFontHeight());
	xAddButton(eGCC_PASS, eGRID_PASS, 0, L"àÍéËÉpÉX", eGCID_PASS);
	xAddButton(eGCC_PREV_BRANCH, eGRID_PREV_BRANCH, 0, L"ëOÇÃé}Ç÷", eGCID_PREV_BRANCH);
	xAddButton(eGCC_NEXT_BRANCH, eGRID_NEXT_BRANCH, 0, L"éüÇÃé}Ç÷", eGCID_NEXT_BRANCH);

	AppSpace(margin, GetFontHeight());
	xAddButton(eGCC_SHOW_NO, eGRID_SHOW_NO, 0, L"éËèáï\é¶ÅEîÒï\é¶", eGCID_SHOW_NO, True);
	AppSpace(margin, GetFontHeight()>>1);
	xAddSplitButton(eGCC_AUTOPLAY, eGCID_AUTOPLAY, g_AutoplaySplit,GetFontHeight()*3, L"é©ìÆÅEéËìÆçƒê∂");

	return True;
}

Bool cglsGobanControl::xCreateEditor()
{
	//eGCC_LABEL,
	//eGCC_ADDSTONE,
	//eGCC_AUTOPLAY,
	//eGCC_DELETE_NODE,
	//eCGS_CALCULATE_END,
	//eCGS_CALCULATE_STOP,
	//eCGS_SAVE,

	return True;
}

// -------------------------------------------------
LRESULT cglsGobanControl::OnCreate(WPARAM wParam, LPARAM lParam)
{
	if (m_set==eGCS_BASE)
	{
		xCreateBase();
		Update(eGUM_FORCED);
		InvalidateRect(0);
		return True;
	}

	if (m_set==eGCS_EDITOR)
	{
		return xCreateEditor();
	}

	//if (m_set==eGCS_PLAY_INFO)
	//{
	//	return xCreatePlayInfo();
	//}

	//if (m_set==	eGCS_PLAY)
	//{
	//	return xCreatePlay();
	//}
	//
	//if (m_set==eGCS_CALCULATE)
	//{
	//	return xCreateCalculate();
	//}

	//if (m_set==eGCS_GOMSG)
	//{
	//	return xCreateGoMsg();
	//}

	return False;
}