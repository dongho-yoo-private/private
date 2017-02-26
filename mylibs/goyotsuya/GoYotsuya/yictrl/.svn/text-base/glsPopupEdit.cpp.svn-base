#include "glsPopupEdit.h"

cglsPopupEdit::cglsPopupEdit(eglsPopupEditMode mode, int n, Font* font)
: m_mode(mode)
, m_n(n)
{
	if (font)
		m_font = (Font*)cyiShape::Clone(font);
}

cglsPopupEdit::~cglsPopupEdit(void)
{
	delete m_font;
	m_font=0;
}

// ------------------------------------
Bool cglsPopupEdit::TrackPopup(HWND hWndParent, int x, int y, int n, Font* font)
{
	if (n!=0)
		m_n=n;

	if (m_n==0)
		m_n=10;


	m_bIsExitLoop=False;
	Bool res = CreatePopup(hWndParent, x, y, 10, 10, font, 0);
	DestroyWindow(m_hWnd);

	cGlassInterface* p=	GetChild(2);

	p->Destroy();
	delete p;


	m_children->clear();

	return True;
}

#include "glsEditBox.h"
const char* g_numericFilter="0123456789";

// ---------------------------------------------------
LRESULT cglsPopupEdit::OnCreate(WPARAM wParam, LPARAM lParam)
{
	cglsEditBox* edit = new cglsEditBox();
	sGlsEditBoxDetail editDetail;
	cglsEditBox::xDefaultDetail(&editDetail);

	editDetail.nEnterCommand=IDOK;
	
	edit->Create((HWND)this, 2, 0, 0, m_n*GetFontHeight(True), 0, &editDetail);
	edit->SetPasswordMode((m_mode&ePEM_PASSWORD)!=0);

	if (m_mode&ePEM_NUMERIC)
	{
		char* pszFilter=(char*)yialloci(strlen(g_numericFilter)+2);
		pszFilter[0]=0;
		strcpy(&pszFilter[1], g_numericFilter);
		edit->SetNoImeMode(True, pszFilter);
	}
	edit->Update(eGUM_FORCED);

	ReSize(edit->m_w, edit->m_h);
	InvalidateRect(0);

	SetFocus(*edit);
	SetTimer(*this, 1, 100, NULL);
	return True;
}

// -----------------------------------------------------
LRESULT cglsPopupEdit::OnTimer(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	cGlassInterface* child=GetChild(2);
	SetFocus(*child);

	KillTimer(*this, 1);
	handled=True;
	return 0;
}


// ---------------------------------------------------
Bool cglsPopupEdit::GetText(wchar_t* pszText, int nBufferSize)
{
	wcsncpy(pszText, m_szResult, nBufferSize);

	return True;
}

// ---------------------------------------------------
Bool cglsPopupEdit::GetText(char* pszText, int nBufferSize)
{
	yiToMultiByteCode(pszText, m_szResult, 0);
	return True;
}

// ---------------------------------------------------
LRESULT cglsPopupEdit::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (wParam==IDOK)
	{
		cglsEditBox* edit = (cglsEditBox*)GetChild(2);
		edit->GetText(m_szResult, 1024);

		m_bIsExitLoop=True;
	}

	return 0;
}


// assitant interface
// -----------------------------------------------------
Bool cglsPopupEdit::ShowAssistant(HWND hWnd, int x, int y)
{
	return TrackPopup(hWnd, x, y);
}

// -----------------------------------------------------
Bool cglsPopupEdit::GetResult(wchar_t* buffer)
{
	return True;
}

// -----------------------------------------------------
Bool cglsPopupEdit::GetResult(unsigned int& result1, unsigned int& result2)
{
	return True;
}

// -----------------------------------------------------
Bool cglsPopupEdit::GetDefaultValue(wchar_t* buffer)
{
	return True;
}

// -----------------------------------------------------
Bool cglsPopupEdit::GetDefaultValue(unsigned int& result1, unsigned int& result2)
{
	return True;
}

// -----------------------------------------------------
Bool cglsPopupEdit::SetDefaultValue(wchar_t* str)
{
	return True;
}