#include "yctrlDlgPreviewDetail.h"
#include "resource.h"

cyctrlDlgPreviewDetail::cyctrlDlgPreviewDetail(void)
{
}

cyctrlDlgPreviewDetail::~cyctrlDlgPreviewDetail(void)
{
}

// -------------------------------
Bool cyctrlDlgPreviewDetail::CreateFromReosurce(HWND hWnd, HINSTANCE hInstance, int id, sSearchFilter* filter)
{
	m_filter=filter;
	return cyctrlbase::CreateFromReosurce(hWnd, hInstance, id);
}


// -------------------------------
LRESULT cyctrlDlgPreviewDetail::OnDrawItem(WPARAM wp, LPARAM lp, Bool& Handled)
{
	LPDRAWITEMSTRUCT lpDraw = (LPDRAWITEMSTRUCT)lp;

	Handled=True;
	return 0;
}

// -------------------------------
void cyctrlDlgPreviewDetail::xMakeSearchCondition()
{
	int check = SendMessage(GetDlgItem(m_hWnd, IDC_CHK_ALL), BM_GETCHECK, (WPARAM)0, 0L);

	if (check==1)
	{
		m_filter->first_condition=0;
	}
	else
	{
		if (SendMessage(GetDlgItem(m_hWnd, IDC_CHK_JOSEKI), BM_GETCHECK, (WPARAM)0, 0L))
			m_filter->first_condition|=eFC_JOSEKI;
		if (SendMessage(GetDlgItem(m_hWnd, IDC_CHK_HAMETE), BM_GETCHECK, (WPARAM)0, 0L))
			m_filter->first_condition|=eFC_HAMETE;
		if (SendMessage(GetDlgItem(m_hWnd, IDC_CHK_AFTER_JOSEKI), BM_GETCHECK, (WPARAM)0, 0L))
			m_filter->first_condition|=eFC_JOSEKI_AFTER;
		if (SendMessage(GetDlgItem(m_hWnd, IDC_CHK_JOSEKI_TENUKI), BM_GETCHECK, (WPARAM)0, 0L))
			m_filter->first_condition|=eFC_JOSEKI_TENUKI;
		if (SendMessage(GetDlgItem(m_hWnd, IDC_CHK_CASE_BY_JOSEKI), BM_GETCHECK, (WPARAM)0, 0L))
			m_filter->first_condition|=eFC_CASE_BY;
		if (SendMessage(GetDlgItem(m_hWnd, IDC_CHK_OTHER), BM_GETCHECK, (WPARAM)0, 0L))
			m_filter->first_condition|=eFC_OTHER;
	}

	m_filter->second_condition[0] = SendMessage(GetDlgItem(m_hWnd, IDC_CMB_VALUE), CB_GETCURSEL, 0, 0);
	m_filter->second_condition[1] = SendMessage(GetDlgItem(m_hWnd, IDC_CMB_EDIT), CB_GETCURSEL, 0, 0);
	m_filter->second_condition[2] = SendMessage(GetDlgItem(m_hWnd, IDC_CMB_PLAYED), CB_GETCURSEL, 0, 0);
	m_filter->second_condition[10] = SendMessage(GetDlgItem(m_hWnd, IDC_CMB_OKINIIRI), CB_GETCURSEL, 0, 0);
	m_filter->second_condition[3] = SendMessage(GetDlgItem(m_hWnd, IDC_CMB_SICHO), CB_GETCURSEL, 0, 0);
	m_filter->second_condition[4] = SendMessage(GetDlgItem(m_hWnd, IDC_CMB_OKIGO), CB_GETCURSEL, 0, 0);
	m_filter->second_condition[5] = SendMessage(GetDlgItem(m_hWnd, IDC_CMB_SAME_SHAPE), CB_GETCURSEL, 0, 0);
	m_filter->second_condition[6] = (SendMessage(GetDlgItem(m_hWnd, IDC_CMB_NANIDO), CB_GETCURSEL, 0, 0) <<4) |
										(SendMessage(GetDlgItem(m_hWnd, IDC_CMB_NANIDO_J), CB_GETCURSEL, 0, 0));
	m_filter->second_condition[7] = (SendMessage(GetDlgItem(m_hWnd, IDC_CMB_HINDO), CB_GETCURSEL, 0, 0) <<4) |
										(SendMessage(GetDlgItem(m_hWnd, IDC_CMB_HINDO_J), CB_GETCURSEL, 0, 0));

	m_filter->second_condition[8] = (SendMessage(GetDlgItem(m_hWnd, IDC_CMB_JOSEKI_HIGH), CB_GETCURSEL, 0, 0) <<4) |
										(SendMessage(GetDlgItem(m_hWnd, IDC_CMB_JOSEKI_LOW), CB_GETCURSEL, 0, 0));
	m_filter->second_condition[9] = SendMessage(GetDlgItem(m_hWnd, IDC_CMB_JOSEKI_DETAIL), CB_GETCURSEL, 0, 0);
	return;
}

// ------------------------------------
void cyctrlDlgPreviewDetail::xCheckAllEnable(Bool bIsEnable)
{
	EnableWindow(GetDlgItem(m_hWnd, IDC_CHK_JOSEKI) , bIsEnable);
	EnableWindow(GetDlgItem(m_hWnd, IDC_CHK_HAMETE) , bIsEnable);
	EnableWindow(GetDlgItem(m_hWnd, IDC_CHK_AFTER_JOSEKI) , bIsEnable);
	EnableWindow(GetDlgItem(m_hWnd, IDC_CHK_JOSEKI_TENUKI) , bIsEnable);
	EnableWindow(GetDlgItem(m_hWnd, IDC_CHK_CASE_BY_JOSEKI) , bIsEnable);
	EnableWindow(GetDlgItem(m_hWnd, IDC_CHK_OTHER) , bIsEnable);
}

// ------------------------------------
LRESULT cyctrlDlgPreviewDetail::OnAllCheck(HWND hWnd, int code)
{
	if(BST_CHECKED == SendMessage(hWnd , BM_GETCHECK , 0 , 0))
	{
		xCheckAllEnable(False);
	}
	else
	{
		xCheckAllEnable(True);
	}

	return 0;
}

// -------------------------------
LRESULT cyctrlDlgPreviewDetail::OnOK(HWND hWnd, int code)
{
	//EndDialog(m_hWnd, 0);

	xMakeSearchCondition();
	PostMessage(m_hParentWnd, WM_CLOSED_PREVIEW_DETAIL, 0, 0); 
	DestroyWindow(m_hWnd);
	return 0;
}

// -------------------------------
LRESULT cyctrlDlgPreviewDetail::OnCancel(HWND hWnd, int code)
{
	//EndDialog(m_hWnd, 0);
	DestroyWindow(m_hWnd);
	return 0;
}


// -------------------------------
LRESULT cyctrlDlgPreviewDetail::OnCommand(WPARAM wp, LPARAM lp, Bool& Handled)
{
	int id=LOWORD(wp);
	int code=HIWORD(wp);
	HWND hWnd=(HWND)lp;
	Handled=True;

	switch(id)
	{
		case IDC_OK:
			return OnOK(hWnd, code);
		case IDC_CANCEL:
			return OnCancel(hWnd, code);
		case IDC_CHK_ALL:
			return OnAllCheck(hWnd, code);
	}

	Handled=False;
	return 0;
}


// ---------------------------------------
BOOL cyctrlDlgPreviewDetail::OnPaint(HDC hdc, PAINTSTRUCT* ps/*=0*/)
{
	//if (m_bmpBgBlure==0)
	//{
	//	if (m_bmpBg==0)
	//	{
	//		m_bmpBg = cyctrlbase::MakeBmpBackground(0, 2.0, m_hShdow);
	//	}
	//	Graphics g(hdc);
	//	g.DrawImage(m_bmpBg, 0, 0);
	//	return False;
	//}

	//Graphics g(hdc);
	//g.DrawImage(m_bmpBgBlure, 0, 0);
	//return False;

	return True;
}

// ------------------------------------
void cyctrlDlgPreviewDetail::OnThreadStart(int id)
{
	//if (id==256)
	//{
	//	//Bitmap* bmp = cyctrlbase::GetBehindBitmap();
	//	Bitmap* bmpBluer = cyiBitmapBlender::GaussBlureXX(m_bmpBg, 5.0, 255, 0);

	//	m_bmpBgBlure = cyctrlbase::MakeBmpBackground(bmpBluer, 2.0, m_hShdow);
	//	delete bmpBluer;

	//	//HDC hdc = GetDC(m_hWnd);
	//	//Graphics g(hdc);
	//	//g.DrawImage(m_bmpBgBlure, 0, 0);
	//	//ReleaseDC(m_hWnd, hdc);

	//	InvalidateRect(m_hWnd, 0, True);
	//}
}

#include "yctrlGobanResource.h"

// ----------------------------------------
const char* g_pszJosekiNameHigh[] = {
	"‚·‚×‚Ä",
	"‘I‘ð‚È‚µ",
	"¬–Ú",
	"–Ú‚Í‚¸‚µ",
	"‚–Ú",
	"¯",
	"ŽOX",
	0
};

// ----------------------------------------
const char* g_pszJosekiNameMiddle[] = {
	"‚·‚×‚Ä",
	"‚È‚µ",
	"¬Œj”n‚ª‚©‚è",
	"ˆêŠÔ‚‚ª‚©‚è",
	"“ñŠÔ‚‚©‚©‚è",
	"‘åŒj”n‚©‚©‚è",
	"‚‚©‚©‚è",
	"‹÷‚ª‚©‚è",
	"‘Å‚¿ž‚Ý",
	"‚»‚Ì‘¼",
	0
};

// ----------------------------------------
const char* g_pszJosekiNameDetail[] = {
	"‚·‚×‚Ä",
	"‚È‚µ",
	"ˆêŠÔ‚Î‚³‚Ý",
	"ˆêŠÔ‚‚Î‚³‚Ý",
	"“ñŠÔ‚Î‚³‚Ý",
	"“ñŠÔ‚‚Î‚³‚Ý",
	"ŽOŠÔ‚Î‚³‚Ý",
	"ŽOŠÔ‚‚Î‚³‚Ý",
	"‚±‚·‚Ý",
	"‰º‚Â‚¯",
	"ã‚Â‚¯",
	"ˆêŠÔ‚Æ‚Ñ",
	"¬Œj”n",
	"‘åŒj”n",
	"—¼‚ª‚©‚è"
	"‚»‚Ì‘¼",
	0
};

// ----------------------------------------
const char* g_pszValue[] = {
	"‚·‚×‚Ä",
	"•]‰¿‚È‚µ",
	"ŒÝŠp",
	"•—D¨",
	"”’—D¨",
	"•‚â‚â—L—˜",
	"”’‚â‚â—L—˜",
	"•‚Â‚Ô‚ê",
	"”’‚Â‚Ô‚ê",
	"ó‹µŽŸ‘æ",
	0
};

// ----------------------------------------
const char* g_pszEdited[] = {
	"‚·‚×‚Ä",
	"•ÒWÏ‚Ý",
	"•ÒW‚µ‚Ä‚¢‚È‚¢",
	0
};

// ----------------------------------------
const char* g_pszPlayed[] = {
	"‚·‚×‚Ä",
	"Ä¶Ï‚Ý",
	"Ä¶‚µ‚Ä‚¢‚È‚¢",
	0
};

// ----------------------------------------
const char* g_pszOkiniiri[] = {
	"‚·‚×‚Ä",
	"‚¨‹C‚É“ü‚è",
	"‚¨‹C‚É“ü‚è‚Å‚È‚¢",
	0
};

// ----------------------------------------
const char* g_pszSicho[] = {
	"‚·‚×‚Ä",
	"ƒVƒ`ƒ‡ƒEŽŸ‘æ",
	"ƒVƒ`ƒ‡ƒE‚Æ–³ŠÖŒW",
	0
};

// ----------------------------------------
const char* g_pszOkigo[] = {
	"‚·‚×‚Ä",
	"’u‚«Œé‚¨‚·‚·‚ß",
	"’u‚«Œé‚Æ–³ŠÖŒW",
	0
};

// ----------------------------------------
const char* g_pszSame[] = {
	"‚·‚×‚Ä",
	"“¯ˆêŒ^",
	"“¯ˆêŒ^‚Æ–³ŠÖŒW",
	0
};

// ----------------------------------------
const char* g_pszStar[] = {
	"‚·‚×‚Ä",
	"š",
	"šš",
	"ššš",
	"šššš",
	"ššššš",
	0
};

// ----------------------------------------
const char* g_pszJoken[] = {
	"“¯‚¶",
	"ˆÈã",
	"ˆÈ‰º"
};

// ------------------------------------
static void xComboString(HWND hWnd, int id, const char** str)
{
	HWND hCombo=GetDlgItem(hWnd, id);
	int cnt=0;
	while(str[cnt])
	{
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)str[cnt++]);
	}
	SendMessage(hCombo, CB_SETCURSEL, 0, 0);
}

// ------------------------------------
void cyctrlDlgPreviewDetail::xInitInterface()
{
	int condition = m_filter->first_condition;

	if (condition==0)
	{
		SendMessage(GetDlgItem(m_hWnd, IDC_CHK_ALL), BM_SETCHECK, 1, 0);
		xCheckAllEnable(False);
		goto SET_SECOND_CONDITION;
	}

	xCheckAllEnable(True);

	SendMessage(GetDlgItem(m_hWnd, IDC_CHK_JOSEKI), BM_SETCHECK, (WPARAM)0, 0L);
	SendMessage(GetDlgItem(m_hWnd, IDC_CHK_HAMETE), BM_SETCHECK, (WPARAM)0, 0L);
	SendMessage(GetDlgItem(m_hWnd, IDC_CHK_AFTER_JOSEKI), BM_SETCHECK, (WPARAM)0, 0L);
	SendMessage(GetDlgItem(m_hWnd, IDC_CHK_JOSEKI_TENUKI), BM_SETCHECK, (WPARAM)0, 0L);
	SendMessage(GetDlgItem(m_hWnd, IDC_CHK_CASE_BY_JOSEKI), BM_SETCHECK, (WPARAM)0, 0L);
	SendMessage(GetDlgItem(m_hWnd, IDC_CHK_OTHER), BM_SETCHECK, (WPARAM)0, 0L);


	if (condition&eFC_JOSEKI)
		SendMessage(GetDlgItem(m_hWnd, IDC_CHK_JOSEKI), BM_SETCHECK, (WPARAM)1, 0L);

	if (condition&eFC_HAMETE)
		SendMessage(GetDlgItem(m_hWnd, IDC_CHK_HAMETE), BM_SETCHECK, (WPARAM)1, 0L);

	if (condition&eFC_JOSEKI_AFTER)
		SendMessage(GetDlgItem(m_hWnd, IDC_CHK_AFTER_JOSEKI), BM_SETCHECK, (WPARAM)1, 0L);

	if (condition&eFC_JOSEKI_TENUKI)
		SendMessage(GetDlgItem(m_hWnd, IDC_CHK_JOSEKI_TENUKI), BM_SETCHECK, (WPARAM)1, 0L);

	if (condition&eFC_CASE_BY)
		SendMessage(GetDlgItem(m_hWnd, IDC_CHK_CASE_BY_JOSEKI), BM_SETCHECK, (WPARAM)1, 0L);

	if (condition&eFC_OTHER)
		SendMessage(GetDlgItem(m_hWnd, IDC_CHK_OTHER), BM_SETCHECK, (WPARAM)1, 0L);


SET_SECOND_CONDITION:
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_VALUE), CB_SETCURSEL, m_filter->second_condition[0], 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_EDIT), CB_SETCURSEL, m_filter->second_condition[1] , 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_PLAYED), CB_SETCURSEL, m_filter->second_condition[2] , 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_OKINIIRI), CB_SETCURSEL, m_filter->second_condition[10] , 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_SICHO), CB_SETCURSEL, m_filter->second_condition[3] , 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_OKIGO), CB_SETCURSEL, m_filter->second_condition[4] , 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_SAME_SHAPE), CB_SETCURSEL, m_filter->second_condition[5] , 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_NANIDO), CB_SETCURSEL, m_filter->second_condition[6]>>4, 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_NANIDO_J), CB_SETCURSEL, m_filter->second_condition[6]&0x0F, 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_HINDO), CB_SETCURSEL, m_filter->second_condition[7] >>4, 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_HINDO_J), CB_SETCURSEL, m_filter->second_condition[7] & 0x0F, 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_JOSEKI_HIGH), CB_SETCURSEL, m_filter->second_condition[8]>>4, 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_JOSEKI_LOW), CB_SETCURSEL, m_filter->second_condition[8]&0x0F, 0);
	SendMessage(GetDlgItem(m_hWnd, IDC_CMB_JOSEKI_DETAIL), CB_SETCURSEL, m_filter->second_condition[9] , 0);
}

// ------------------------------------
LRESULT cyctrlDlgPreviewDetail::OnCreate(WPARAM wp, LPARAM lp)
{
	m_bmpBg=0;
	m_bmpBgBlure=0;

	m_hShdow = cyiShape::CreateShadow(5, 2.0, 0.5);
	m_bmpBg = cyctrlbase::MakeBmpBackground(0, 2.0, m_hShdow);

	Font* font = cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL);
	
	HDC hdc = GetDC();
	Graphics g(hdc);
	LOGFONTA logfnt;

	font->GetLogFontA(&g, &logfnt);
	ReleaseDC(hdc);

	HFONT hFont = CreateFontIndirect(&logfnt);

	SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hFont ,True);
	StartThread(256);


	xComboString(m_hWnd, IDC_CMB_VALUE, g_pszValue);
	xComboString(m_hWnd, IDC_CMB_JOSEKI_HIGH, g_pszJosekiNameHigh);
	xComboString(m_hWnd, IDC_CMB_JOSEKI_LOW, g_pszJosekiNameMiddle);
	xComboString(m_hWnd, IDC_CMB_JOSEKI_DETAIL, g_pszJosekiNameDetail);
	xComboString(m_hWnd, IDC_CMB_EDIT, g_pszEdited);
	xComboString(m_hWnd, IDC_CMB_PLAYED, g_pszPlayed);
	xComboString(m_hWnd, IDC_CMB_OKINIIRI, g_pszOkiniiri);
	
	xComboString(m_hWnd, IDC_CMB_SICHO, g_pszSicho);
	xComboString(m_hWnd, IDC_CMB_OKIGO, g_pszOkigo);
	xComboString(m_hWnd, IDC_CMB_HINDO, g_pszStar);
	xComboString(m_hWnd, IDC_CMB_HINDO_J, g_pszJoken);
	xComboString(m_hWnd, IDC_CMB_NANIDO, g_pszStar);
	xComboString(m_hWnd, IDC_CMB_NANIDO_J, g_pszJoken);
	xComboString(m_hWnd, IDC_CMB_SAME_SHAPE, g_pszSame);

	xInitInterface();

	
	return 0;
}