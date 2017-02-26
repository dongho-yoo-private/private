#include "yigRoomNoPopup.h"
#include "glsEditBox.h"
#include "glsButton.h"
#include "glsStatic.h"

cyigRoomNoPopup::cyigRoomNoPopup(void)
{
}

cyigRoomNoPopup::~cyigRoomNoPopup(void)
{
}

// ------------------------------------
Bool cyigRoomNoPopup::TrackPopup(HWND hWndParent, int x, int y, int n, Font* font)
{
	if (n!=0)
		m_n=n;

	if (m_n==0)
		m_n=10;
	m_bIsExitLoop=False;
	Bool res = CreatePopup(hWndParent, x, y, 10, 10, font, 0);

	//for (cyiNode* node = m_children->GetNodeTopFrom(); node; node=node->np)
	//{
	//	cGlassInterface* p = (cGlassInterface* )node->ref;
	//	p->Destroy();
	//	//delete p;
	//}

	m_children->clear();
	//delete m_children;
	m_children=0;

	Destroy();
	//DestroyWindow(m_hWnd);

	//cGlassInterface* p=	GetChild(2);

	//p->Destroy();
	//delete p;

	//cGlassInterface* p=	GetChild(3);

	//p->Destroy();
	//delete p;

	//m_children->clear();

	return res;
}

const char* g_numericFilter="0123456789";
// ---------------------------------------------------
LRESULT cyigRoomNoPopup::OnCreate(WPARAM wParam, LPARAM lParam)
{
	cglsEditBox* edit = new cglsEditBox();
	sGlsEditBoxDetail editDetail;
	cglsEditBox::xDefaultDetail(&editDetail);
	int x=0;
	int y=0;
	int w=0;
	int xOffset=0;
	int margin=3;
	//editDetail.bIsEnterCommand=True;

	cglsStatic* label = new cglsStatic();
	label->Create((HWND)this, margin, margin, 0, 0, 0, L"接続先の部屋番号：");
	label->Show();

	xOffset=label->m_w+margin;
	
	
	edit->Create((HWND)this, 2, xOffset+GetFontHeight(True)*0.5, margin, m_n*GetFontHeight(True), 0, &editDetail);
	//edit->SetImeMode(eGTIM_IME_OFF);

	{
		char* pszFilter=(char*)yialloci(strlen(g_numericFilter)+2);
		pszFilter[0]=0;
		strcpy(&pszFilter[1], g_numericFilter);
		edit->SetNoImeMode(True, pszFilter);
	}
	edit->SetImeMode(eGTIM_IME_ALWAYS_OFF);
	edit->Update(eGUM_FORCED);
	edit->Show();

	w=edit->m_x+edit->m_w+margin+margin;

	y+=edit->m_h+margin;

	label = new cglsStatic();
	label->Create((HWND)this, margin, 0, 0, 0, 0, L"部屋のパスワード：");
	label->Show();
	label->Move(xOffset-label->m_w, y);

	cglsEditBox* edit2 = new cglsEditBox();
	edit2->Create((HWND)this, 3, xOffset+GetFontHeight(True)*0.5, y, m_n*GetFontHeight(True), 0, &editDetail);
	edit2->SetImeMode(eGTIM_IME_ALWAYS_OFF);
	edit2->Update(eGUM_FORCED);
	edit2->Show();

	y+=edit2->m_h;
	
	cglsButton* btn = new cglsButton();
	sGlsButtonDetail detail;
	cglsButton::xDefaultDetail(&detail);
	detail.colorHilight=Color::LightGoldenrodYellow;
	detail.rect.right=w;
	btn->Create((HWND)this, IDOK, 0, y, 0, L"OK", 0, &detail);
	btn->Update(eGUM_FORCED);
	btn->Show();

	ReSize(w, y+btn->m_h+margin+margin);


	InvalidateRect(0, 0, True);

	SetFocus(*GetChild(2));
	SetTimer(*this, 1, 100, NULL);
	return True;
}

// ---------------------------------------------------
LRESULT cyigRoomNoPopup::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	bIsHandled=True;
	if ((wParam&0xFFFF)==IDOK)
	{
		char no[16];
		cglsEditBox* edit = (cglsEditBox*)GetChild(2);
		edit->GetText(no, 16);
		m_roomNo=atoi(no);

		edit = (cglsEditBox*)GetChild(3);
		edit->GetText(m_szPassword, 16);

		m_bIsExitLoop=True;
		m_bIsExitCode=1;
	}

	return 0;
}
// -----------------------------------------------------
LRESULT cyigRoomNoPopup::OnTimer(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	cGlassInterface* child=GetChild(2);
	SetFocus(*child);

	KillTimer(*this, 1);
	handled=True;
	return 0;
}

// ---------------------------------------
Bool cyigRoomNoPopup::OnBgDraw()
{
	return __super::OnBgDraw();
}