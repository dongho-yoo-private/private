#include "yigRegisterDialog.h"
#include "GlassControlz.h"
#include "ypngoNetwork.h"

#define xID 16

static const wchar_t* g_pszArea[] = {
	L"-",
	L"北海道",
	L"青森県",
	L"岩手県",
	L"宮城県",
	L"秋田県",
	L"山梨県",
	L"福島県",
	L"茨城県",
	L"栃木県",
	L"群馬県",
	L"埼玉県",
	L"千葉県",
	L"東京都",
	L"神奈川県",
	L"新潟県",
	L"富山県",
	L"石川県",
	L"福井県",
	L"山梨県",
	L"長野県",
	L"岐阜県",
	L"静岡県",
	L"愛知県",
	L"三重県",
	L"滋賀県",
	L"京都府",
	L"大阪府",
	L"兵庫県",
	L"奈良県",
	L"和歌山県",
	L"鳥取県",
	L"島根県",
	L"岡山県",
	L"広島県",
	L"山口県",
	L"徳島県",
	L"香川県",
	L"愛媛県",
	L"高知県",
	L"福島県",
	L"佐賀県",
	L"長崎県",
	L"熊本県",
	L"大分県",
	L"宮崎県",
	L"鹿児島県",
	L"沖縄県",
	0
};

const wchar_t* g_pszLevel[] = {
	L"アマ七段",
	L"アマ五段",
	L"アマ六段",
	L"アマ四段",
	L"アマ三段",
	L"アマ二段",
	L"アマ初段",
	L"1級",
	L"2級",
	L"3級",
	L"4級",
	L"5級",
	L"6級",
	L"7級",
	L"8級",
	L"9級",
	L"10級",
	L"11級",
	L"12級",
	L"13級",
	L"14級",
	L"15級",
	L"16級",
	L"17級",
	L"18級",
	L"プロ",
	L"セミプロ",
	0
};

const wchar_t* g_pszNo[] = {
	L"公開",
	0
};

const wchar_t* g_pszSex[] = {
	L"男",
	L"女",
	0
};

DEFINE_RESOURCE_BEGIN(_gRESOURCE)
	// ページ1
	DEFINE_RESOURCE(xID+1, eGIT_LABEL, L"Ｅメールアドレス", 0, 0)
	DEFINE_EDIT_BOX(xID+2, 10, True, False, 80, xID+1, eGP_BOTTOMSIDE|eGP_LEFT, 0)
	//ページ2
	DEFINE_RESOURCE(xID+3, eGIT_LABEL, L"認証コード", 0, 0)
	DEFINE_EDIT_BOX(xID+4, 10, True, False, 0, xID+3, eGP_BOTTOMSIDE|eGP_LEFT, 0)
	// ページ3
	DEFINE_RESOURCE(xID+5, eGIT_LABEL, L"パスワード", 0, 0)
	DEFINE_RESOURCE(xID+6, eGIT_LABEL, L"ＩＤ", xID+5, eGP_TOPSIDE|eGP_RIGHT, eIM_Y)
	DEFINE_EDIT_BOX(xID+7, 9, False, False, 16, xID+6, eGP_RIGHTSIDE|eGP_BOTTOM, eIM_SMALL_X)
	DEFINE_EDIT_BOX(xID+8, 9, True, False, 8, xID+5, eGP_RIGHTSIDE|eGP_BOTTOM, eIM_SMALL_X)
	DEFINE_RESOURCE(xID+9, eGIT_LABEL, L"再確認", xID+5, eGP_BOTTOMSIDE|eGP_RIGHT, eIM_SMALL_Y)
	DEFINE_EDIT_BOX(xID+10, 9, True, False, 8, xID+9, eGP_RIGHTSIDE|eGP_BOTTOM,eIM_SMALL_X|eIM_SMALL_Y)
	DEFINE_RESOURCE(xID+11, eGIT_LABEL, L"段位", xID+9, eGP_BOTTOMSIDE|eGP_RIGHT, eIM_LARGE_Y)
	DEFINE_SPILIT_BUTTON(xID+12, g_pszLevel, 0, xID+11, eGP_RIGHTSIDE|eGP_BOTTOM, 0, 5, eIM_SMALL_X)
	DEFINE_RESOURCE(xID+13, eGIT_LABEL, L"お住まい", xID+11, eGP_BOTTOMSIDE|eGP_RIGHT, eIM_SMALL_Y)
	DEFINE_SPILIT_BUTTON(xID+14, g_pszArea, 0, xID+13, eGP_RIGHTSIDE|eGP_BOTTOM, 0, 5, eIM_SMALL_X)
	DEFINE_RESOURCE(xID+15, eGIT_CHECK, (const wchar_t*)g_pszNo, xID+14, eGP_RIGHTSIDE|eGP_BOTTOM, eIM_X)
	DEFINE_RESOURCE(xID+16, eGIT_LABEL, L"性別", xID+13, eGP_BOTTOMSIDE|eGP_RIGHT, eIM_SMALL_Y)
	DEFINE_RESOURCE(xID+17, eGIT_RADIO, (const wchar_t*)g_pszSex, xID+16, eGP_RIGHTSIDE|eGP_BOTTOM, eIM_X)
	DEFINE_RESOURCE(xID+18, eGIT_LABEL, L"生年月日", xID+16, eGP_BOTTOMSIDE|eGP_RIGHT, eIM_SMALL_Y)
	DEFINE_RESOURCE(xID+19, eGIT_CHECK, (const wchar_t*)g_pszNo, xID+15, eGP_BOTTOMSIDE|eGP_LEFT, eIM_SMALL_Y)
	DEFINE_RESOURCE(xID+21, eGIT_CHECK, (const wchar_t*)g_pszNo, xID+19, eGP_BOTTOMSIDE|eGP_LEFT, eIM_SMALL_Y)
	//DEFINE_RESOURCE(xID+22, eGIT_BUTTON, L"重複チェック", xID+7, eGP_RIGHTSIDE|eGP_BOTTOM, eIM_SMALL_X)

DEFINE_RESOURCE_END()


cyigRegisterDialog::cyigRegisterDialog(void)
:m_userid(0)
,m_page(0)
{
	m_szId[0]=0;
	m_szPassword[0]=0;
}

cyigRegisterDialog::~cyigRegisterDialog(void)
{
}


// ------------------------------------------------------
Bool cyigRegisterDialog::RegisterUser(cGlassInterface* parent, socket_t sock, Font* font)
{
	//Bool res = Create(parent, font);
	sGlsDialogDetail xdetail;

	cglsDialog::xDefaultDetail(&xdetail);
	xdetail.font=font;
	xdetail.gauss.fBlureRad=5.0;
	xdetail.gauss.fShadowWidth=7.0;

	m_bIsExitLoop=False;
	m_bIsSizeFixedMode=False;

	int w=10;
	int h=10;
	int x=(parent->m_w-w)>>1;
	int y=(parent->m_h-h)>>1;

	xdetail.bIsAnimationShow=True;
	m_sock=sock;
	Bool res = cglsDialog::DoModal((HWND)parent, GetBlankId(), L"ユーザ登録", x, y, w, h, &xdetail);

	if (res)
		return m_userid;
	return 0;
}

// ------------------------------------------------------
Bool cyigRegisterDialog::GetValue(char* szUserName, char* password)
{
	strcpy(szUserName, m_szId);
	strcpy(password, m_szPassword);
	return True;
}


#define CONTROL_OFFSET 16
#include "glsMonthCalender.h"
// --------------------------------------------
LRESULT cyigRegisterDialog::OnCreate(WPARAM wParam, LPARAM lParam)
{
	//m_ptItemDistance.x=GetFontHeight(True)/2;
	//m_ptItemDistance.y=GetFontHeight(True)/7;

	if (cglsDialog::CreateChildFromResource(_gRESOURCE)==False)
	{
		m_bIsExitLoop=True;
		return 0;
	}

	sGlsEditBoxDetail detail;
	cglsEditBox::xDefaultDetail(&detail);
	cglsEditBox* edit = new cglsEditBox();

	detail.assistant=new cglsMonthCalender();
	detail.bIsStaticEditorMode=True;
	detail.bIsMouseDownShow=True;
	edit->Create((HWND)this,  xID+20, 0, 0, GetFontHeight(True)*4, 0, &detail);

	GetChild(xID+18)->AddPositionLink(edit, eGP_RIGHTSIDE|eGP_BOTTOM, GetFontHeight(True)/2, 0);

	edit = (cglsEditBox*)GetChild(xID+8);
	edit->SetPasswordMode(True);
	edit = (cglsEditBox*)GetChild(xID+10);
	edit->SetPasswordMode(True);

	/*
	sGlsStaticDetail detail;
	cglsStatic::xDefaultDetail(&detail);
	cglsStatic* label = new cglsStatic();

	// メールアドレス登録画面
	detail.id=CONTROL_OFFSET+1;
	label->Create((HWND)this, 0, 0, 0, 0, 0, L"メールアドレス：", &detail);

	cglsEditBox* edit = new cglsEditBox();
	edit->Create((HWND)this, CONTROL_OFFSET+2, 0, 0, GetFontHeight(True)*10, 0);
	edit->SetImeMode(eGlsTextImeMode::eGTIM_IME_ALWAYS_OFF);
	edit->SetMaxLength(80);
	label->AddPositionLink(edit, eGlsPosition::eGP_RIGHTSIDE|eGlsPosition::eGP_BOTTOM);

	// Help用のラベル
	//m_font->GetSize()
	//label = new cglsStatic();
	//detail.id=CONTROL_OFFSET+3;
	//label->Create((HWND)this, 0, 0, 0, 0, 0, L"メールアドレスを入力してください。", &detail);
	

	detail.id=CONTROL_OFFSET+3;
	label = new cglsStatic();
	label->Create((HWND)this, 0, 0, 0, 0, 0, L"認証コード", &detail);

	cglsEditBox* edit = new cglsEditBox();
	edit->Create((HWND)this, CONTROL_OFFSET+4, 0, 0, GetFontHeight(True)*10, 0);
	edit->SetImeMode(eGlsTextImeMode::eGTIM_IME_ALWAYS_OFF);
	edit->SetMaxLength(80);
	label->AddPositionLink(edit, eGlsPosition::eGP_BOTTOM_SIDE|eGlsPosition::eGP_LEFT);


	detail.id=CONTROL_OFFSET+5;
	labe = new cglsStatic();
	label->Create((HWND)this, 0, 0, 0, 0, 0, L"ID", &detail);

	edit = new cglsEditBox();
	edit->Create((HWND)this, CONTROL_OFFSET+6, 0, 0, GetFontHeight(True)*10, 0);
	edit->SetMaxLength(16);
	label->AddPositionLink(edit, eGlsPosition::eGP_RIGHTSIDE|eGlsPosition::eGP_BOTTOM);

	detail.id=CONTROL_OFFSET+7;
	labe = new cglsStatic();
	label->Create((HWND)this, 0, 0, 0, 0, 0, L"パスワード", &detail);

	edit = new cglsEditBox();
	edit->Create((HWND)this, CONTROL_OFFSET+6, 0, 0, GetFontHeight(True)*8, 0);
	edit->SetMaxLength(8);
	label->AddPositionLink(edit, eGlsPosition::eGP_RIGHTSIDE|eGlsPosition::eGP_BOTTOM);


	*/



	AddButton(L"<前へ", 3);
	AddButton(L"次へ>", 4);
	AddButton(L"取り消し", IDCANCEL);
	EnableButton(3, False);
	
	cglsDialog::UpdateButton(eGIA_RIGHT, 0);

	ReArrangeItemz();

	return 0;
}

// ------------------------------------
void cyigRegisterDialog::OnItemReArranged()
{
	int w=0;
	int h=0;
	int xOffset, yOffset;
	const int distance=GetFontHeight(True)/7;
	cGlassInterface* child;

	GetContentsOffset(xOffset, yOffset);

	// メールアドレス登録画面
	if (m_page!=0)
	{
		GetChild(xID+1)->Hide();
		GetChild(xID+2)->Hide();
	}
	else
	{
		child=GetChild(xID+1);
		child->Move(xOffset, yOffset);
		child->Show();

		child=GetChild(xID+2);
		child->Show();
		w=child->m_w+child->m_x-xOffset;
		h=child->m_h+child->m_y-yOffset;
		SetFocus(*child);
	}

	if (m_page==1)
	{
		child=GetChild(xID+3);
		child->Move(xOffset, yOffset);
		child->Show();

		child=GetChild(xID+4);
		child->Show();
		w=child->m_w+child->m_x-xOffset;
		h=child->m_h+child->m_y-yOffset;
		SetFocus(*child);
	}
	else
	{
		for (int i=xID+3; i<xID+5; i++)
			GetChild(i)->Hide();
	}

	if (m_page==2)
	{
		cGlassInterface* child2 = GetChild(xID+6);

		child=GetChild(xID+5);
		child->Move(xOffset, child2->m_h+yOffset+GetFontHeight(True)/2);

		child = GetChild(xID+18);
		h=child->m_y+child->m_h-yOffset;
		//child2= GetChild(xID+9);
		//child->Move((child2->m_y+child2->m_w)-child->m_w, child2->m_h+child2->m_y+GetFontHeight(True)/2);

		for (int i=xID+5; i<xID+22; i++)
			GetChild(i)->Show();

		child = GetChild(xID+8);
		w=child->m_w+child->m_x-xOffset;
		SetFocus(GetChild(xID+7)->m_hWnd);

	}
	else
	{
		for (int i=xID+5; i<xID+22; i++)
			GetChild(i)->Hide();
	}

	SetContentsSize(w, h);
	InvalidateRect(0, 0, True);
}

#include "glsMessageBox.h"
// ------------------------------------
Bool cyigRegisterDialog::ShowHelp(const wchar_t* help)
{
	cglsMessageBox msg;
	msg.Show(this, help, L"", MB_OK, &Font(L"Meiryo", 12.0));
	return True;
}


// ------------------------------------
void cyigRegisterDialog::OnExitLoop()
{
	__super::OnExitLoop();
}

// ------------------------------------
static Bool xIsEmailAddress(const char* pszEmailAddress)
{
	if (pszEmailAddress==0)
		return False;

	if (pszEmailAddress[0]==0)
		return False;

	if (strstr(pszEmailAddress, "@")==0)
		return False;
	if (strstr(pszEmailAddress, ".")==0)
		return False;

	return True;
}
#include "yiHashFunc.h"

// ------------------------------------
LRESULT cyigRegisterDialog::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIshandled)
{
	int id = wParam&0xFFFF;
	switch(id)
	{
		// NEXT
		case 4:
		{
			if (m_page==0)
			{
				xSendEmail();
			}
			else if (m_page==1)
			{
				xSendCode();
			}
			else if (m_page==2)
			{
				wchar_t szReason[256];
				if (xCheckValue(szReason)==True)
				{
					xRegisterUser();
				}
				else
				{
					//wchar_t* uni=::yiToUnicode(0, szReason, 0);
					ShowHelp(szReason);
					//yifree(uni);
				}
			}
			break;
		}
		case IDCANCEL:
		{
			//m_bIsExitLoop=True;

			if (m_page!=0)
			{
				char buffer[128];
				sGeneralParam* p=(sGeneralParam*)buffer;
				strncpy((char*)p, m_szEmailAddress, 80);

				cypnGoNetwork* net = (cypnGoNetwork*)cypnGoNetwork::GetInstance();
				if (net->Send(m_sock, eUSPID_RELEASE_EMAIL, p, 80)==False)
				{
					// 再接続？
					//ShowHelp(L"不明なエラー");
					//return 0;
				}
			}
			cypnGoNetwork* net = (cypnGoNetwork* )cypnGoNetwork::GetInstance();
			net->DisconnectFromServer(m_sock);
			m_szId[0]=0;
			m_szPassword[0]=0;
			m_szEmailAddress[0]=0;

			m_bIsRequestDestroy=True;
			AnimationShow(False, 300);
			break;
		}
	}

	return 0;
}

// --------------------------------------------
void cyigRegisterDialog::xSendCode()
{
	//m_page++;
	//cglsDialog::ReArrangeItemz();
	//return ;

	char szCode[64];
	unsigned int r1, r2;
	cglsEditBox* edit = (cglsEditBox*)GetChild(CONTROL_OFFSET+4);
	edit->GetText(szCode, 64);

	if (sscanf(szCode, "%u-%u", &r1, &r2)!=2)
	{
		ShowHelp(L"認証コードが間違っています。");
		return ;
	}

	sGeneralParam param;

	param.param[0]=r1;
	param.param[1]=r2;
	cypnGoNetwork* net = (cypnGoNetwork*)cypnGoNetwork::GetInstance();
	if (net->Send(m_sock, eUSPID_REQ_RECONIZED_CODE, &param, GP_SIZE(2))==False)
	{
		// 再接続？
		ShowHelp(L"不明なエラー");
		return ;
	}

	int id, size;
	if (net->Receive(m_sock, &id, &param, &size)==False)
	{
		// 再接続？
		ShowHelp(L"不明なエラー");
		return ;
	}

	if (param.param[0]==0)
	{
		// 続行
		m_page++;
		//EnableButton(4, False);
		SetButtonText(4, L"登録");

		//cglsDialog::ReArrangeItemz();
		
	}
	else
	{
		ShowHelp(L"認証コードが間違っています。");
		return ;
	}

	cglsDialog::ReArrangeItemz();

}

// --------------------------------------------
void cyigRegisterDialog::xSendEmail()
{
	//m_page++;
	//cglsDialog::ReArrangeItemz();
	//return ;

	char szEmailAddress[256];
	cglsEditBox* edit = (cglsEditBox*)GetChild(CONTROL_OFFSET+2);
	edit->GetText(szEmailAddress, 256);

	if (xIsEmailAddress(szEmailAddress)==True)
	{
		char buffer[128];
		sGeneralParam* param=(sGeneralParam*)buffer;
		cypnGoNetwork* network = (cypnGoNetwork*)cypnGoNetwork::GetInstance();

		unsigned int n1 = yihf_additional((char*)szEmailAddress, 8);
		unsigned int n2 = yihf_encoded_additional((char*)szEmailAddress, 8);

		param->param[0]=(n1<<24)|(n2<<16);
		strncpy((char*)&param->param[1], szEmailAddress, 80);
		
		if (network->Send(m_sock, eUSPID_REQ_REGISTER_EMAIL, param, sizeof(sizeof(unsigned int))+80)==True)
		{
			sGeneralParam param;
			int size;
			int id;
			if (network->Receive(m_sock, &id, &param, &size)==False)
			{
				ShowHelp(L"不明なエラー");
				return ;
			}

			if (id==eUSPID_REQ_REGISTER_EMAIL)
			{
				if (param.param[0]==0)
				{
					// 次の画面へ
					m_page++;
					cglsDialog::ReArrangeItemz();
					strncpy(m_szEmailAddress, szEmailAddress, 80);
					return ;
				}
				else if (param.param[0]==2 ||
							param.param[0]==3)
				{
					ShowHelp(L"このメールアドレスは既に登録済みです。");
				}
				else
				{
					ShowHelp(L"現在サーバが込み合っているので、しばらくしてからもう一度行ってください");
				}
			}
		}
	}
	return ;
}

#include "svry/svrydefs.h"
#include "svry/svrydefs.register.h"
// --------------------------------------------
void cyigRegisterDialog::xRegisterUser()
{
	char buffer[256];
	sRegisterUserInfo info;
	memset(&info, 0, sizeof(info));

	strcpy(info.szEmailAddress, m_szEmailAddress);

	// ユーザ名設定
	cglsEditBox* edit = (cglsEditBox*)GetChild(xID+7);
	edit->GetText(buffer, 256);
	strcpy(info.szUserName, buffer);

	// パスワード設定
	edit=(cglsEditBox*)GetChild(xID+8);
	edit->GetText(buffer, 256);
	strcpy(info.szPassword, buffer);

	// 段位設定
	cglsSplitButton* btn = (cglsSplitButton*)GetChild(xID+12);
	int nLevel = btn->GetSelectIndex();
	info.flag = nLevel;

	// お住まい
	btn = (cglsSplitButton*)GetChild(xID+14);
	info.nAreaCode=btn->GetSelectIndex();

	// 生年月日
	edit = (cglsEditBox*)GetChild(xID+20);
	edit->GetText(buffer, 256);

	int y, m, d;
	sscanf(buffer, "%d/%d/%d", &y, &m, &d);
	
	info.birthday= (y<<9)|(m<<5)|d;

	info.nContryCode=0; // japan.

	// 性別
	cglsRadioGroup* radio = (cglsRadioGroup*)GetChild(xID+17);
	int n = radio->GetChecked();
	info.flag |= n==0?(0x80000000):0;

	// 公開
	cglsCheckGroup* check = (cglsCheckGroup*)GetChild(xID+15);
	
	if (check->GetChecked(0)==True)
	{
		info.flag|=0x00000001;
	}

	check = (cglsCheckGroup*)GetChild(xID+19);
	if (check->GetChecked(0)==True)
	{
		info.flag|=0x00000002;
	}

	check = (cglsCheckGroup*)GetChild(xID+21);
	if (check->GetChecked(0)==True)
	{
		info.flag|=0x00000004;
	}

	unsigned int r1 = ::yihf_additional(info.szUserName, 8);
	unsigned int r2 = ::yihf_encoded_additional(info.szUserName, 8);
	unsigned int r3 = ::yihf_general(info.szUserName, 8);

	info.id=(r1<<24)|(r2<<16)|(r3<<8);

	r1 = ::yihf_additional(info.szEmailAddress, 8);
	r2 = ::yihf_encoded_additional(info.szEmailAddress, 8);
	info.emailId=(r1<<24)|(r2<<16);

	m_userid=0;
	cypnGoNetwork* net = (cypnGoNetwork*)cypnGoNetwork::GetInstance();
	if (net->Send(m_sock, eUSPID_REQ_REGISTER_USER, &info, sizeof(info))==True)
	{
		int id, size;
		sGeneralParam param;
		if (net->Receive(m_sock, &id, &param, &size)==True)
		{
			if (param.param[0]==0)
			{
				cglsMessageBox msg;
				msg.Show(this, L"登録されました。", L"", MB_OK, &Font(L"Meiryo UI", 12.0));
				m_userid=param.param[1];
				this->m_bIsExitLoop=True;

				net->DisconnectFromServer(m_sock);
				strcpy(m_szId, info.szUserName);
				strcpy(m_szPassword, info.szPassword);
				AnimationShow(False, 300);


				return ;
			}
			if (param.param[0]==0xFFFFFFFF)
			{
				cglsMessageBox msg;
				msg.Show(this, L"すでに登録されているIDです。", L"", MB_OK, &Font(L"Meiryo UI", 12.0));
			}
			else
			{
				cglsMessageBox msg;
				msg.Show(this, L"システムエラー\r\nもう一度、登録ボタンを押してください。", L"", MB_OK, &Font(L"Meiryo UI", 12.0));
			}

			return ;
		}
	}

	cglsMessageBox msg;
	msg.Show(this, L"登録に失敗しました。", L"", MB_OK, &Font(L"Meiryo UI", 12.0));

}

// --------------------------------------------
Bool cyigRegisterDialog::xCheckValue(wchar_t* reason)
{
	char buffer[256];
	// IDの長さチェック・重複チェック
	cglsEditBox* edit = (cglsEditBox* )GetChild(xID+7);

	edit->GetText(buffer, 256);

	int n = strlen(buffer);

	if (n>16)
	{
		wcscpy(reason, L"IDが長すぎます。(半角16文字、全角8文字まで)");
		return False;
	}

//	char buffer[32];
	unsigned int r1=yihf_additional(buffer, 8);
	unsigned int r2=yihf_encoded_additional(buffer, 8);
	unsigned int r3=yihf_general(buffer, 8);

	char qbff[32];
	sGeneralParam* s = (sGeneralParam*)qbff;
	s->param[0]=(r1<<24)|(r2<<16)|(r3<<8);
	strcpy((char*)&s->param[1], buffer);
	cypnGoNetwork* net = (cypnGoNetwork*)cypnGoNetwork::GetInstance();
	if (net->Send(m_sock, eUSPID_REQ_QUERY_USER, s, 16+sizeof(unsigned int))==True)
	{
		int id, size;
		if (net->Receive(m_sock, &id, s, &size)==False)
		{
			wcscpy(reason, L"登録失敗");
			return False;
		}

		if (s->param[0]!=0)
		{
			wcscpy(reason, L"すでに登録されているIDです。");
			return False;
		}
	}

	char buffer2[32];
	// パスワードチェック
	edit=(cglsEditBox*)GetChild(xID+8);
	edit->GetText(buffer, 256);
	edit=(cglsEditBox*)GetChild(xID+10);
	edit->GetText(buffer2, 32);

	if (strcmp(buffer, buffer2)!=0)
	{
		wcscpy(reason, L"パスワードが同じでないです。");
		return False;
	}

	if (strlen(buffer)>9)
	{
		wcscpy(reason, L"パスワードが長すぎます。(8文字まで)");
		return False;
	}

	// 生年月日
	edit=(cglsEditBox*)GetChild(xID+20);
	edit->GetText(buffer, 256);

	int y, m, d;
	sscanf(buffer, "%d/%d/%d", &y, &m, &d);

	//time64_t timex = cyiTime::GetLocalTime64();
	unsigned int timex = cyiTime::GetLocalTime();
	int cur = xYEAR(timex);//(timex);

	if (cur<y ||
			cur-3<y)
	{
		wcscpy(reason, L"生年月日の値が不正です");
		return False;
	}

	return True;
}