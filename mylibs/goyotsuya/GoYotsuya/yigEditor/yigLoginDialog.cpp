#include "StdAfx.h"
#include "yigLoginDialog.h"
#include "GlassControlz.h"

cyigLoginDialog::cyigLoginDialog(void)
: m_bIsNormalLogin(True)
{
	m_szId[0]=0;
	m_szPass[0]=0;
}

cyigLoginDialog::~cyigLoginDialog(void)
{
}


// -----------------------------------
Bool cyigLoginDialog::Login(cGlassInterface* parent, wchar_t* pszName, wchar_t* pszPassword, int mode, Font* font)
{
	::wcscpy(m_szId, pszName);
	::wcscpy(m_szPass, pszPassword);

	Bool res = Create(parent, font);
	*pszName=0;
	*pszPassword=0;
	if (res)
	{
		::wcscpy(pszName, m_szId);
		::wcscpy(pszPassword, m_szPass);
	}

	return res;
}

// -----------------------------------
Bool cyigLoginDialog::Login(cGlassInterface* parent, char* pszName, char* pszPassword, int mode, Font* font)
{
	Bool res = Create(parent, font);

	*pszName=0;
	*pszPassword=0;
	if (res)
	{
		::yiToMultiByteCode(pszName, m_szId, 0);
		::yiToMultiByteCode(pszPassword, m_szPass, 0);
	}

	return res;

}

// -----------------------------------
Bool cyigLoginDialog::IsNormalLogin()
{
	return m_bIsNormalLogin;
	//cglsTabCtrl* tab = (cglsTabCtrl*)GetChild(16);

	//if (tab==0)
	//	return 0;

	//int n = tab->GetCurSel();
	//
	//if (n==1)
	//	return True;
	//return False;
}


// -----------------------------------
Bool cyigLoginDialog::Create(cGlassInterface* parent, Font* font)
{
	sGlsDialogDetail xdetail;

	cglsDialog::xDefaultDetail(&xdetail);
	xdetail.font=font;
	xdetail.gauss.fBlureRad=5.0;
	xdetail.gauss.fShadowWidth=7.0;
	xdetail.bIsAnimationShow=True;

	//cyiShape::MeasureTextSize(pszMessage, font?font:parent->GetFont(), (parent->m_w<<1)/2-20, m_ptMessage); 

	int w=300;
	int h=200;
	int x=(parent->m_w-w)>>1;
	int y=(parent->m_h-h)>>1;

	m_bIsExitLoop=False;
	m_bIsSizeFixedMode=False;
	return cglsDialog::DoModal((HWND)parent, GetBlankId(), L"ログイン", x, y, w, h, &xdetail);
}

// --------------------------------------------
LRESULT cyigLoginDialog::OnCreate(WPARAM wParam, LPARAM lParam)
{
	//cglsTabCtrl* tab = new cglsTabCtrl();
	//tab->Create((HWND)this,16, 0, 0, 100);
	//tab->AddItem(1, 0, L"ログイン", False);
	//tab->AddItem(2, 0, L"ＩＤなしログイン", False);
	//tab->Update(eGUM_FORCED);
	//tab->SetCurSel(0);
	//tab->Hide();

	sGlsStaticDetail staticDetail;
	cglsStatic::xDefaultDetail(&staticDetail);
	cglsStatic* label;

	// id
	{
		staticDetail.id=21;
		label = new cglsStatic();
		label->Create((HWND)this, 0, 0, 0, 0, 0, L"ID", &staticDetail);
		label->Hide();
	}

	{
		staticDetail.id=22;
		label = new cglsStatic();
		label->Create((HWND)this, 0, 0, 0, 0, 0, L"パスワード", &staticDetail);
		label->Hide();

	}

	// id
	{
		staticDetail.id=23;
		label = new cglsStatic();
		label->Create((HWND)this, 0, 0, 0, 0, 0, L"表示名", &staticDetail);
		label->Hide();
	}

	{
		staticDetail.id=24;
		label = new cglsStatic();
		label->Create((HWND)this, 0, 0, 0, 0, 0, L"部屋のパスワード", &staticDetail);
		label->Hide();
	}

	// id
	{
		cglsEditBox* edit = new cglsEditBox();
		edit->Create((HWND)this, 17, 0, 0, GetFontHeight(True)*10, 0, 0, L"ID入力");
		edit->SetMaxLength(15);
		edit->Hide();

		if (m_szId[0]!=0)
		{
			edit->SetText(m_szId);
			edit->SetCarotPos(-1);
			edit->SetCurSel(0);
		}


	}

	// password
	{
		cglsEditBox* edit = new cglsEditBox();
		edit->Create((HWND)this, 18, 0, 0, GetFontHeight(True)*10, 0, 0, L"パスワード入力");
		edit->SetMaxLength(8);
		edit->SetImeMode(eGTIM_IME_ALWAYS_OFF);
		edit->SetPasswordMode(True);
		edit->Hide();
	}

	// id
	{
		cglsEditBox* edit = new cglsEditBox();
		edit->Create((HWND)this, 19, 0, 0, GetFontHeight(True)*10, 0);
		edit->SetMaxLength(14);
		edit->Hide();
	}

	// password
	{
		cglsEditBox* edit = new cglsEditBox();
		edit->Create((HWND)this, 20, 0, 0, GetFontHeight(True)*10, 0);
		edit->SetMaxLength(8);
		edit->SetImeMode(eGTIM_IME_ALWAYS_OFF);
		edit->Hide();
	}

	// ------------------------------------
	cglsDialog::CreateButton(MB_OKCANCEL);

	ReArrangeItemz();
	return 0;
}

// ------------------------------------
void cyigLoginDialog::OnItemReArranged()
{
	cGlassInterface* child;
	int xOffset;
	int yOffset;
	int xOffset2;

	int y=0;
	int x=0;
	int h=0;
	const int distance=GetFontHeight(True)/7;

	GetContentsOffset(xOffset, yOffset);

	int tabWidth=GetChild(24)->m_w+GetChild(19)->m_w;//-xOffset;

	// tab control.
	child=GetChild(16);

	if (child==0)
	{
		sGlsTabDetail detail;

		cglsTabCtrl::xDefaultDetail(&detail);

		//detail.stack.bIsVertical=True;
		detail.wMaxShowCount=1;
		detail.wMinShowCount=1;

		cglsTabCtrl* tab = new cglsTabCtrl();
		tab->Create((HWND)this,16, 0, 0, tabWidth, &detail);
		tab->AddItem(1, 0, L"ログイン", False);
		tab->AddItem(2, 0, L"ＩＤなしログイン", False);

		tab->Show();
		tab->ReSize(tabWidth);
		tab->Update(eGUM_FORCED);
		tab->SetCurSel(0);
		//tab->Hide();
	}
	child=GetChild(16);
	child->Move(xOffset, yOffset);
	y=yOffset;
	y+=child->m_h+distance;

	xOffset2=GetChild(22)->m_w+xOffset;

	// ID
	child=GetChild(21);
	child->Move(xOffset2-child->m_w, y);
	x=child->m_x+child->m_w+GetFontHeight(True)/2;
	
	child=GetChild(17);
	child->Move(x, y);
	y+=child->m_h+distance;

	// パスワード
	child=GetChild(18);
	child->Move(x, y);
	child=GetChild(22);
	child->Move(xOffset, y);

	//y=yOffset+GetChild(16)->m_h+distance;
	y=yOffset+GetChild(16)->m_h+distance;
	// 表示名
	xOffset2=GetChild(24)->m_w+xOffset;

	child=GetChild(23);
	child->Move(xOffset2-child->m_w, y);
	x=child->m_x+child->m_w+GetFontHeight(True)/2;

	child=GetChild(19);
	child->Move(x, y);
	y+=child->m_h+distance;

	// 部屋の番号
	child=GetChild(20);
	child->Move(x, y);
	child=GetChild(24);
	child->Move(xOffset, y);

	y+=child->m_h;

	// タブのサイズ調整
	//child=GetChild(16);
	//child->Show();
	//child->ReSize(tabWidth, child->m_h);//Move(xOffset, yOffset);
	//child->Update(eGUM_FORCED);

	SetContentsSize(tabWidth, y-yOffset);

	xShowContents();
	//InvalidateRect(0);
	InvalidateRect(0, 0, True);
}

// ------------------------------------
void cyigLoginDialog::OnExitLoop()
{
	
	if (m_returnValue==IDOK)
	{
		cglsTabCtrl* tab = (cglsTabCtrl*)GetChild(16);
		int n = tab->GetCurSel();

		if (n==1)
		{
			cglsEditBox* editId = (cglsEditBox*)GetChild(17);
			cglsEditBox* editPass = (cglsEditBox*)GetChild(18);

			editId->GetText(m_szId, 32);
			editPass->GetText(m_szPass, 16);
			m_bIsNormalLogin=True;
		}
		else
		{
			cglsEditBox* editId = (cglsEditBox*)GetChild(19);
			cglsEditBox* editPass = (cglsEditBox*)GetChild(20);

			editId->GetText(m_szId, 32);
			editPass->GetText(m_szPass, 16);
			m_bIsNormalLogin=False;

		}
	}

	Destroy();
}

// ---------------------------------------------------
//LRESULT cyigLoginDialog::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
//{
//	m_returnValue=wParam;
//
//	if (m_returnValue==64)
//	{
//		m_returnValue=IDCANCEL;
//	}
//	
//	m_bIsExitLoop=True;
//	bIsHandled=True;
//
//	return 0;
//}

// ---------------------------------------------------
LRESULT cyigLoginDialog::OnNotifyFromTab(int id, int subId, int code)
{
	if (code==TCN_SELCHANGE)
	{
		xShowContents();
	}
	return 0;
}

// ---------------------------------------------------
void cyigLoginDialog::xShowContents()
{
	cglsTabCtrl* tab = (cglsTabCtrl*)GetChild(16);

	if (tab->GetCurSel()==1)
	{
		GetChild(17)->Show();
		GetChild(18)->Show();
		GetChild(19)->Hide();
		GetChild(20)->Hide();
		GetChild(21)->Show();
		GetChild(22)->Show();
		GetChild(23)->Hide();
		GetChild(24)->Hide();

		SetFocus(GetChild(17)->m_hWnd);
	}
	else
	{
		GetChild(17)->Hide();
		GetChild(18)->Hide();
		GetChild(19)->Show();
		GetChild(20)->Show();
		GetChild(21)->Hide();
		GetChild(22)->Hide();
		GetChild(23)->Show();
		GetChild(24)->Show();
		SetFocus(GetChild(19)->m_hWnd);
	}

	InvalidateRect(0);
}



