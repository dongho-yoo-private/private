#include "yigNoIdDialog.h"

cyigNoIdDialog::cyigNoIdDialog(void)
{
	m_szRemoteName[0]=0;
	m_nRoomNo=0;
}

cyigNoIdDialog::~cyigNoIdDialog(void)
{
}

// -----------------------------------
Bool cyigNoIdDialog::ConnectRoom(cGlassInterface* parent, unsigned int* remoteRoomNo, char* pszRemoteName, Font* font)
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
	m_szRemoteName[0]=0;
	m_nRoomNo=0;

	
	Bool res = cglsDialog::DoModal((HWND)parent, GetBlankId(), L"相手の部屋に接続", x, y, w, h, &xdetail);

	if (res==True)
	{
		*remoteRoomNo=m_nRoomNo;
		strncpy(pszRemoteName, m_szRemoteName, 16);
	}
	
	return res;
}

// --------------------------------------------
LRESULT cyigNoIdDialog::OnCreate(WPARAM wParam, LPARAM lParam)
{

	sGlsStaticDetail staticDetail;
	cglsStatic::xDefaultDetail(&staticDetail);
	cglsStatic* label;

	// id
	{
		staticDetail.id=21;
		label = new cglsStatic();
		label->Create((HWND)this, 0, 0, 0, 0, 0, L"相手の部屋番号", &staticDetail);
		label->Hide();
	}

	{
		staticDetail.id=22;
		label = new cglsStatic();
		label->Create((HWND)this, 0, 0, 0, 0, 0, L"部屋のパスワード", &staticDetail);
		label->Hide();
		label->AddPositionLink(GetChild(21), eGP_TOPSIDE|eGP_RIGHT, 0, GetFontHeight(True)/4);
	}



	// id
	{
		cglsEditBox* edit = new cglsEditBox();
		edit->Create((HWND)this, 17, 0, 0, GetFontHeight(True)*4, 0);
		edit->SetMaxLength(4);
		edit->SetImeMode(eGTIM_IME_ALWAYS_OFF);
		edit->Hide();
		GetChild(21)->AddPositionLink(edit, eGP_RIGHTSIDE|eGP_BOTTOM, GetFontHeight(True)/2, 0);
	}

	// password
	{
		cglsEditBox* edit = new cglsEditBox();
		edit->Create((HWND)this, 18, 0, 0, GetFontHeight(True)*4, 0);
		edit->SetMaxLength(8);
		edit->SetImeMode(eGTIM_IME_ALWAYS_OFF);
		edit->Hide();
		GetChild(22)->AddPositionLink(edit, eGP_RIGHTSIDE|eGP_BOTTOM, GetFontHeight(True)/2, 0);
	}

	// for help.
	{
		staticDetail.id=23;
		label = new cglsStatic();
		label->Create((HWND)this, 0, 0, 0, 0, 0, L"接続する相手の部屋番号や部屋のパスワードを入力してください。", &staticDetail);
		label->Hide();
		GetChild(22)->AddPositionLink(label, eGP_BOTTOMSIDE|eGP_LEFT, GetFontHeight(True), 0);
	}

	// ------------------------------------
	cglsDialog::CreateButton(MB_OKCANCEL);

	ReArrangeItemz();
	return 0;
}

// ------------------------------------
void cyigNoIdDialog::OnItemReArranged()
{
	int xOffset;
	int yOffset;
	int width, height;

	this->GetContentsOffset(xOffset, yOffset);
	GetChild(22)->Move(xOffset, yOffset+GetChild(21)->m_h+GetFontHeight(True)/4);

	GetMaxWidthHeight(16, 30, width, height);
	ShowChildz(16, 30, True);

	//GetChild(23)->AnimationShow(True, 300);

	SetContentsSize(width-xOffset, height-yOffset);

	this->ShowChildz(16, 30, True);
	//InvalidateRect(0);
	InvalidateRect(0, 0, True);
}

// ---------------------------------------------------
LRESULT cyigNoIdDialog::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	m_returnValue=wParam;

	if (m_returnValue==64)
	{
		m_returnValue=IDCANCEL;
	}

	if (m_returnValue==IDCANCEL)
	{
		m_bIsExitLoop=True;
		bIsHandled=True;
		m_returnValue=0;
		AnimationShow(False, m_nShowAnimationTime);
		Destroy();

		return 0;
	}

	if (wParam==IDOK)
	{
		cglsSystem* system = GetSystem();
		
		cypnGoNetwork* network =  (cypnGoNetwork*)cypnGoNetwork::GetInstance();
		sypnUserInfo info;
		
		char szRoomNo[32];
		char szPassword[16];

		cglsEditBox* edit = (cglsEditBox*)GetChild(17);
		
		if (edit->GetText(szRoomNo, 32)==False)
		{
			cglsStatic* help = (cglsStatic*)GetChild(23);
			help->SetText(0, L"相手の部屋番号を入力してください。");
			return 0;
		}

		edit = (cglsEditBox*)GetChild(18);
		if (edit->GetText(szPassword, 32)==False)
		{
			//cglsStatic* help = (cglsStatic*)GetChild(23);
			//help->SetText(0, L"相手の部屋番号を入力してください。");
		}


		//sypnUserInfo info;
		if (network->Connect(0, 0, atoi(szRoomNo), szPassword, &info)==-1)
		{
			cglsStatic* help = (cglsStatic*)GetChild(23);
			help->SetText(0, L"相手の部屋に接続できません。\r\n部屋番号や部屋のパスワードを確認してください。");
			return 0;
		}

		m_bIsExitLoop=True;
		bIsHandled=True;
		m_returnValue=IDOK;

		AnimationShow(False, m_nShowAnimationTime);
		Destroy();

		/*EnableButton(IDOK, False);*/
		
	}

	return 0;
}