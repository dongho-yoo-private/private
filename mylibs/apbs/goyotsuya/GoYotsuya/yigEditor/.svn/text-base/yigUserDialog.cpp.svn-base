#include "StdAfx.h"
#include "yigUserDialog.h"
#include "glsRadioGroup.h"
#include "glsSpinbox.h"
#include "glsCheckGroup.h"
#include "glsMessageBox.h"
#include "ypnGoNetwork.h"

#include "resource.h"
// --------------------------------------------
static wchar_t* xGetString(HINSTANCE hInstance, int id)
{
	static wchar_t buffer[256];

	if (LoadStringW(hInstance, id, buffer, 256)!=0)
	{
		return buffer;
	}

	return 0;
}
#define DIALOG_ITEM_ID_BUTTON_PANEL 128
#define CONTROL_OFFSET (16)

cyigUserDialog::cyigUserDialog(void)
: m_condition1(0)
, m_condition2(0)
, m_rules(0)
, m_userz(0)
, m_nSelectedRule(0)
, m_rulez(0)
, m_hSelectedUser(0)
{
	memset(&m_remoteRule, 0, sizeof(m_remoteRule));
}

cyigUserDialog::~cyigUserDialog(void)
{
	if (m_rulez)
		delete m_rulez;
}

static const wchar_t* rules_name[] = {
	L"GO!!よつや",
	L"ゆったり",
	L"NHK杯",
	L"本因坊戦",
	0
};

static const wchar_t* rules_comment[] = {
	L"GO!!よつやルール（アマチュアに無難なルール）",
	L"ゆったり（持ち時間無制限、中断あり）",
	L"NHK杯ルール（持ち時間なし、一手30秒、1分秒読み10回",
	L"本因坊戦ルール（持ち時間8時間）",
	0
};


// -----------------------------------
Bool cyigUserDialog::xDefaultCondition(int id, unsigned int& condition, unsigned int& condition2)
{
	// NHK
	if (id==2)
	{
		condition=0;
		condition2=0;

		// 持ち時間0
		SET_PLAY_TIME(condition, 0); 

		// こみ 6目半
		SET_KOMI(condition, 6);
		SET_HALF_SCORE(condition);

		// 秒読み 60秒10回
		SET_SECOND_READ2(condition, 60);
		SET_SECOND_READ_COUNT(condition2, 10);		

		// 秒読み30秒
		SET_SECOND_READ(condition, 30);
	}
	// 本因坊戦
	else if (id==3)
	{
		condition=0;
		condition2=0;

		// こみ 6目半
		SET_KOMI(condition, 6);
		SET_HALF_SCORE(condition);

		// 持ち時間20分
		SET_PLAY_TIME(condition, 8*60); 

		// 秒読み 30秒3回
		SET_SECOND_READ2(condition, 60);
		SET_SECOND_READ_COUNT(condition2, 3);

		SET_HUU_COUNT(condition2, 1);

		// 秒読み10秒
		SET_SECOND_READ(condition, 10);
	}
	else if (id==1)
	{
		condition=0;
		condition2=0;

		// こみ 6目半
		SET_KOMI(condition, 6);
		SET_HALF_SCORE(condition);

		// 持ち時間20分
		//SET_PLAY_TIME(condition, 0xFF); // -> 無限 

		SET_PLAYPAUSE(condition2);
	}
	// Yotsuya rule
	else if (id==0)
	{
		condition=0;
		condition2=0;

		// こみ 6目半
		SET_KOMI(condition, 6);
		SET_HALF_SCORE(condition);

		// 持ち時間20分
		SET_PLAY_TIME(condition, 20); 

		// 秒読み 30秒3回
		SET_SECOND_READ2(condition, 30);
		SET_SECOND_READ_COUNT(condition2, 3);

		// 秒読み10秒
		SET_SECOND_READ(condition, 10);
	}
	else
	{
		condition=0;
		condition2=0;
		return False;
	}

	return True;
}

// -----------------------------------
void xSetCondition(int index, sGoYotsuyaGoRules* rules, unsigned int& con1, unsigned int&con2)
{
	if (cyigUserDialog::xDefaultCondition(index, con1, con2)==False)
	{
		if (rules)
		{
			con1=rules[index].condition1;
			con2=rules[index].condition2;
		}
	}
}

// -----------------------------------
Bool cyigUserDialog::Show(cGlassInterface* parent, int groupId, int userId, 
								sGoGroupInfo* loginedGroup, CUserList* listUser, sGoGroupInfo* generalGroup, 
									int nSelectedRule, sGoYotsuyaGoRules* ruleList, Font* font)
{
	m_mode=eUDM_FULL;
	m_nSelectedRule=nSelectedRule;
	m_userz=listUser;
	m_generalGroup=generalGroup;
	m_loginedGroup=loginedGroup;
	xSetCondition(m_nSelectedRule, ruleList, m_condition1, m_condition2);

	return Create(parent, 0, font);
}

// -----------------------------------
Bool cyigUserDialog::Show(cGlassInterface* parent, const char* pszFrom, unsigned int con1, unsigned con2, cyiList* rules, Font* font)
{
	m_mode=eUDM_RPY;
	//m_mode=eUDM_FULL;
	m_condition1=con1;
	m_condition2=con2;
	m_userz=0;
	m_generalGroup=0;
	m_loginedGroup=0;
	m_nSelectedRule=-1;

	if (m_condition1==0 &&
			m_condition2==0)
	{
		xDefaultCondition(0, m_condition1, m_condition2);
	}

	m_remoteRule.id=-1;
	m_remoteRule.condition1=con1;
	m_remoteRule.condition2=con2;
	yiToUnicode(m_remoteRule.szName, pszFrom, 0);
	wsprintfW(m_remoteRule.szComment, L"%sさんのルール", m_remoteRule.szName);

	wchar_t szCaption[128];
	wsprintfW(szCaption, L"%sさんからの申し込み", m_remoteRule.szName);

	
	return Create(parent, szCaption, font);

	
}

// -----------------------------------
Bool cyigUserDialog::Show(cGlassInterface* parent, unsigned int remoteId, const char* pszRemoteUserName, unsigned int con1, unsigned int con2, Font* font)
{
	m_mode=eUDM_NORMAL;

	m_condition1=con1;
	m_condition2=con2;
	m_userz=0;
	m_generalGroup=0;
	m_loginedGroup=0;
	m_nSelectedRule=-1;

	if (con1==0 &&
			con2==0)
	{
		xSetCondition(m_nSelectedRule, 0, m_condition1, m_condition2);
	}

	wchar_t wszRemoteUser[16];
	wchar_t szCaption[128];

	::yiToUnicode(wszRemoteUser, pszRemoteUserName, 0);
	wsprintfW(szCaption, L"%sさんへの申し込み", wszRemoteUser);

	return Create(parent, szCaption, font);
}



// -----------------------------------
HandleUser cyigUserDialog::GetValue(unsigned int& condition1, unsigned int& condition2)
{
	condition1=m_condition1;
	condition2=m_condition2;
	return m_hSelectedUser;


	//return (HandleUser)info;
}


// -----------------------------------
Bool cyigUserDialog::Create(cGlassInterface* parent, const wchar_t* pszCaption, Font* font)
{
	sGlsDialogDetail xdetail;

	cglsDialog::xDefaultDetail(&xdetail);
	xdetail.font=font;
	xdetail.gauss.fBlureRad=5.0;
	xdetail.gauss.fShadowWidth=7.0;
	//xdetail.bIsAutoDestroy=False;

	//cyiShape::MeasureTextSize(pszMessage, font?font:parent->GetFont(), (parent->m_w<<1)/2-20, m_ptMessage); 

	int w=10;
	int h=10;
	int x=(parent->m_w-w)>>1;
	int y=(parent->m_h-h)>>1;

	m_bIsExitLoop=False;
	m_bIsSizeFixedMode=False;
	//xdetail.bIsAnimationShow=True;
	//xdetail.nShowAnimationTime=500;
	//xdetail.nHideAnimationTime=500;
	return cglsDialog::DoModal((HWND)parent, GetBlankId(), pszCaption?pszCaption:xGetString((HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE), IDS_CONFIG_PLAY), x, y, w, h, &xdetail);
}



const wchar_t* g_pszNoKan[]={L"二", L"三", L"四", L"五", L"六", L"七", L"八", L"九"};


// --------------------------------------------
LRESULT cyigUserDialog::OnCreate(WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE);
	sGlsStaticDetail staticDetail;
	cglsStatic::xDefaultDetail(&staticDetail);
	cglsStatic* label;
	int nFontHeight=GetFontHeight(True);
	const int nDestance=nFontHeight/7;
	sGlsSplitButtonDetail splitDetail;
	cglsSplitButton::xDefaultDetail(&splitDetail);
	splitDetail.colorHilight=Color::LightBlue;

	if (m_mode==eUDM_FULL)
	{
		// 左側
		staticDetail.id=CONTROL_OFFSET+1;
		label = new cglsStatic();
		label->Create((HWND)this, 0, 0, 0, 0, 0, xGetString(hInstance, IDS_USERLIST), &staticDetail);
		label->Hide();


		cglsSplitButton* btn= new cglsSplitButton();


		// ユーザリスト: 1
		// グループリスト: 2 SplitButton
		int itemId=0;
		btn->Create((HWND)this, CONTROL_OFFSET+2, 0, 0, GetFontHeight(True)*6, 0, 0, &splitDetail);
		btn->Hide();
		btn->AddItem(1, 0, xGetString(hInstance, IDS_FRIEND), 0, 0);
		btn->AddItem(2, 0, xGetString(hInstance, IDS_NOMAL_PLAY_ROOM), 0, 0);
		itemId=2;
		// TODO. グループ追加


		// リストボックス

		

		btn->AddItem(++itemId, 0, xGetString(hInstance, IDS_ROOM_NO), xGetString(hInstance, IDS_ROOM_NO_DIRECT));
		btn->SelectIndex(m_nSelectedRule);
		btn->Update(eGUM_FORCED);

		label->AddPositionLink(btn, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT, 0, nDestance);

		cglsListBox* listBox = new cglsListBox();
		listBox->Create((HWND)this,CONTROL_OFFSET+3 , GetFontHeight(True)*6, 0, GetFontHeight(True)*6, 100);
		listBox->Hide();

		btn->AddPositionLink(listBox, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT, 0, nDestance);


		staticDetail.id=CONTROL_OFFSET+4;
		label = new cglsStatic();
		label->Create((HWND)this, 0, 0, 0, 0, 0, xGetString(hInstance, IDS_SCORE), &staticDetail);
		label->Hide();

		listBox->AddPositionLink(label, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT, 0, nDestance);

		staticDetail.id=CONTROL_OFFSET+5;
		label = new cglsStatic();
		label->Create((HWND)this, 0, 0, 0, 0, 0, xGetString(hInstance, IDS_SCORE_IN_GROUP), &staticDetail);
		label->Hide();

		GetChild(CONTROL_OFFSET+4)->AddPositionLink(label, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT);
	}

	// 対局ルール
	staticDetail.id=CONTROL_OFFSET+6;
	label = new cglsStatic();
	label->Create((HWND)this, 0, 0, 0, 0, 0, xGetString(hInstance, IDS_PLAY_RULE), &staticDetail);
	label->Hide();

	// 対局ルール
	cglsSplitButton* btn= new cglsSplitButton();
	splitDetail.colorHilight=Color::LightBlue;
	btn->Create((HWND)this, CONTROL_OFFSET+7, 0, 0, GetFontHeight(True)*6, 0, 0, &splitDetail);
	btn->Hide();

	int cnt=0;
	while(rules_name[cnt])
	{
		btn->AddItem(cnt+1, 0, rules_name[cnt], rules_comment[cnt++], 0);
	}
	btn->Update(eGUM_FORCED);
	label->AddPositionLink(btn, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT, 0);

	// TODO.

	sGlsButtonDetail buttonDetail;
	cglsButton::xDefaultDetail(&buttonDetail);

	//buttonDetail.colorHilight=Color::LightPink;
	cglsButton* savebtn = new cglsButton();
	savebtn->Create((HWND)this, CONTROL_OFFSET+8, 0, 0, 0, xGetString(hInstance, IDS_SAVE), xGetString(hInstance, IDS_SAVE_RULE), &buttonDetail);
	savebtn->Hide();
	savebtn->Update(eGUM_FORCED);
	btn->AddPositionLink(savebtn, eGlsPosition::eGP_RIGHTSIDE|eGlsPosition::eGP_TOP, nFontHeight);


	buttonDetail.colorHilight=Color::LightPink;
	savebtn = new cglsButton();
	savebtn->Create((HWND)this, CONTROL_OFFSET+9, 0, 0, 0, xGetString(hInstance, IDS_DELETE), xGetString(hInstance, IDS_SAVE_RULE), &buttonDetail);
	savebtn->Hide();
	savebtn->Update(eGUM_FORCED);
	GetChild(CONTROL_OFFSET+8)->AddPositionLink(savebtn, eGlsPosition::eGP_RIGHTSIDE|eGlsPosition::eGP_TOP);


	// 手合い
	staticDetail.id=CONTROL_OFFSET+22;
	label = new cglsStatic();
	label->Create((HWND)this, 0, 0, 0, 0, 0, xGetString(hInstance, IDS_TEAI), &staticDetail);
	label->Hide();

	GetChild(CONTROL_OFFSET+7)->AddPositionLink(label, eGP_LEFT|eGP_BOTTOMSIDE, 0, nDestance);

	btn= new cglsSplitButton();
	splitDetail.colorHilight=Color::LightBlue;
	btn->Create((HWND)this, CONTROL_OFFSET+10, 0, 0, GetFontHeight(True)*4, 0, 0, &splitDetail);
	btn->AddItem(1, 0, xGetString(hInstance, IDS_TAGAISEN), 0, True);
	btn->AddItem(2, 0, xGetString(hInstance, IDS_JOSEN));
	for (int i=0; i<8; i++)
	{
		wchar_t buffer[16];
		//wchar_t no[2]={L'一'+i,0}; 
		//buffer[0]=g_pszNoKan[i][0];
		//wcscpy(&buffer[1], xGetString(hInstance, IDS_OKIGOX));
		wsprintfW(buffer, xGetString(hInstance, IDS_OKIGOX), g_pszNoKan[i]);
		btn->AddItem(3+i, 0, buffer);
	}
	btn->Hide();
	btn->Update(eGUM_FORCED);
	label->AddPositionLink(btn, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT);

	// 手合い（たがいせんの場合）
	cglsRadioGroup* radio = new cglsRadioGroup();
	radio->Create(this, CONTROL_OFFSET+11, 0, 0);
	radio->AddItem(1, xGetString(hInstance, IDS_BLACK));
	radio->AddItem(2, xGetString(hInstance, IDS_WHITE));
	radio->AddItem(3, xGetString(hInstance, IDS_NIGIRI));
	radio->SetChecked(2);
	radio->Update(eGUM_FORCED);
	radio->Hide();

	btn->AddPositionLink(radio, eGlsPosition::eGP_RIGHTSIDE|eGlsPosition::eGP_TOP, nFontHeight);

	// コミ
	staticDetail.id=CONTROL_OFFSET+12;
	label = new cglsStatic();
	label->Create((HWND)this, 0, 0, 0, 0, 0, xGetString(hInstance, IDS_KOMI), &staticDetail);
	label->Hide();

	btn->AddPositionLink(label, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT, 0, nDestance*1.5);

	cglsSpinBox* spin = new cglsSpinBox();
	spin->Create(this, CONTROL_OFFSET+13, 0, 0, 3);
	spin->SetNumer(0, 0, 9, 6);
	spin->SetNumer(1, 0, 9, 0);
	spin->SetLimited(1, 12);
	spin->SetNumer(2, 0, 1);
	//spin->Update((eGlsUpdateMode)eGUM_FORCED);
	spin->Hide();
//	spin->Update(eGUM_FORCED);

	label->AddPositionLink(spin, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT);

	cglsCheckGroup* check = new cglsCheckGroup();
	check->Create(this, CONTROL_OFFSET+14, 0, 0);
	check->AddItem(1, xGetString(hInstance, IDS_HALFSCORE));
	check->AddItem(2, xGetString(hInstance, IDS_REVERSE));
	check->SetChecked(0);
	check->SetChecked(1);
	check->Update(eGUM_FORCED);
	check->Hide();

	spin->AddPositionLink(check, eGlsPosition::eGP_RIGHTSIDE|eGlsPosition::eGP_TOP, nFontHeight);

	// 持ち時間
	check = new cglsCheckGroup();
	check->Create(this, CONTROL_OFFSET+15, 0, 0);
	check->AddItem(1, xGetString(hInstance, IDS_LIMITED_TIME));
	check->SetChecked(0);
	check->Update(eGUM_FORCED);

	check->Hide();
	GetChild(CONTROL_OFFSET+13)->AddPositionLink(check, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT, 0, nDestance*2);

	sGlsSpinBoxDetail detail;
	cglsSpinBox::xDefaultDetail(&detail);
	detail.fScrollTerm=5.0;
	spin = new cglsSpinBox();
	spin->Create(this, CONTROL_OFFSET+16, 0, 0, 2);
	spin->SetNumer(0, 0, 9, 0);
	spin->SetNumer(1, 0, 2, 0);
	spin->SetTotalLimited(0, 20);
	spin->InsertLabel(xGetString(hInstance, IDS_HOUR), -1, Color::Black);
	spin->Hide();
	check->AddPositionLink(spin, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT);

	spin = new cglsSpinBox();
	spin->Create(this, CONTROL_OFFSET+25, 0, 0, 2);
	spin->AddString(0, L"0", 0, True);
	spin->AddString(0, L"5", 1);
	//spin->SetNumer(0, 0, 9, 0);
	spin->SetNumer(1, 0, 5, 0);
	spin->InsertLabel(xGetString(hInstance, IDS_MINUTE), 0, Color::Black);
//	spin->Update(eGUM_FORCED);
	spin->Hide();
	GetChild(CONTROL_OFFSET+16)->AddPositionLink(spin, eGlsPosition::eGP_RIGHTSIDE|eGlsPosition::eGP_TOP, nFontHeight/2);

//	spin->Update(eGUM_FORCED);

	// 秒読み
	check = new cglsCheckGroup();
	check->Create(this, CONTROL_OFFSET+17, 0, 0);
	check->AddItem(1, xGetString(hInstance, IDS_SECOND_READ));
	check->SetChecked(0);
	check->Update(eGUM_FORCED);

	check->Hide();
	GetChild(CONTROL_OFFSET+16)->AddPositionLink(check, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT, 0, nDestance);

	// 300秒まで(10秒単位)
	spin = new cglsSpinBox();
	spin->Create(this, CONTROL_OFFSET+18, 0, 0, 3);
	spin->SetNumer(0, 0, 0, 0);
	spin->SetNumer(1, 0, 5, 0);
	spin->SetNumer(2, 0, 5, 0);	
	//spin->SetLimited(1, 12);
	spin->SetTotalLimited(0, 30);
	spin->InsertLabel(xGetString(hInstance, IDS_SECOND), 0, Color::Black);
	spin->InsertLabel(xGetString(hInstance, IDS_MINUTE), 2, Color::Black);
//	spin->Update(eGUM_FORCED);
	spin->Hide();
//	spin->Update(eGUM_FORCED);
	check->AddPositionLink(spin, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT);
	

	// 150秒まで
	spin = new cglsSpinBox();
	spin->Create(this, CONTROL_OFFSET+24, 0, 0, 3);
	spin->SetNumer(0, 0, 0, 0);
	spin->AddString(1, L"0", 0, True);
	spin->AddString(1, L"3", 1);
	//spin->SetNumer(1, 0, 9, 0);
	spin->SetNumer(2, 0, 5, 0);
	spin->SetTotalLimited(0, 10);
	spin->InsertLabel(xGetString(hInstance, IDS_MINUTE), 2, Color::Black);

	//spin->SetLimited(1, 18);
	spin->InsertLabel(xGetString(hInstance, IDS_SECOND), 0, Color::Black);
//	spin->Update(eGUM_FORCED);
	spin->Hide();
//	spin->Update(eGUM_FORCED);
	GetChild(CONTROL_OFFSET+18)->AddPositionLink(spin, eGlsPosition::eGP_RIGHTSIDE|eGlsPosition::eGP_TOP, nFontHeight);

	// 何回=50回まで
	spin = new cglsSpinBox();
	spin->Create(this, CONTROL_OFFSET+19, 0, 0, 2);
	spin->SetNumer(0, 0, 9, 0);
	spin->SetNumer(1, 0, 5, 0);
	spin->SetTotalLimited(0, 50);
	spin->InsertLabel(xGetString(hInstance, IDS_KAI), -1, Color::Black);
//	spin->Update(eGUM_FORCED);
	spin->Hide();
//	spin->Update(eGUM_FORCED);
	GetChild(CONTROL_OFFSET+24)->AddPositionLink(spin, eGlsPosition::eGP_RIGHTSIDE|eGlsPosition::eGP_TOP);


	// オプション
	staticDetail.id=CONTROL_OFFSET+20;
	label = new cglsStatic();
	label->Create((HWND)this, 0, 0, 0, 0, 0, xGetString(hInstance, IDS_OPTION), &staticDetail);
	label->Hide();
	GetChild(CONTROL_OFFSET+18)->AddPositionLink(label, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT, 0, nDestance);

	check = new cglsCheckGroup();
	check->Create(this, CONTROL_OFFSET+21, 0, 0);
	check->AddItem(1, xGetString(hInstance, IDS_WAIT));
	check->AddItem(2, xGetString(hInstance, IDS_HUU));
	check->AddItem(3, xGetString(hInstance, IDS_PAUSE));
	check->SetChecked(0);
	check->Update(eGUM_FORCED);

	check->Hide();

	label->AddPositionLink(check, eGlsPosition::eGP_BOTTOMSIDE|eGlsPosition::eGP_LEFT);
	//cglsStackPannel pannel=GetButtonPannel();
	
	//cglsButton* btn;

	// 申し込み
	// キャンセル
	// 条件変更

	//ReArrangeItemz();
	// ------------------------------------
	//cglsDialog::CreateButton(MB_OKCANCEL);

	if (m_mode==eUDM_FULL)
	{
		AddButton(L"対局開始", IDOK);
		AddButton(L"取り消し", IDCANCEL);
	}
	else
	{
		AddButton(L"対局開始", IDOK);
		AddButton(L"条件変更", 3);
		AddButton(L"取り消し", IDCANCEL);
	}
	cglsDialog::UpdateButton(eGIA_RIGHT, 0);

	//if (m_mode==eUDM_FULL)
	//{
	//	SetTimer(*this, 1, 500, 0);
	//}
	//AnimationShow(True, 1000);

	return ReArrangeItemz();
}

// ------------------------------------

// グループリスト: 2 SplitButton
// ユーザリスト: 3 listbox
// ユーザリスト: 4 大戦戦績
// ユーザリスト: 5 グループ内戦績
// 対局ルール：6
// 対局ルール：7 SplitButton
// 対局ルール: 8 button (保存)
// 対局ルール: 9 button (削除)
// 手合い: 22
// 手合い：10 split button
// 手合い：11 radio
// コミ: 12
// コミ：13 spinbox
// コミ：14 チェック
// 持ち時間：15 チェック
// 持ち時間：16 spin
// 秒読み：17 チェック
// 秒読み：18 spin
// 秒読み：19 spin
// 秒読み：24 spin
// オプション:20
// オプション：21 チェック
// タブ：23
// ------------------------------------
void cyigUserDialog::OnItemReArranged()
{
	cGlassInterface* child;
	int xOffset;
	int yOffset;
	int nLeftSideWidth;
	int maxWidth;
	int maxHeight;

	const int distance=GetFontHeight(True)/7;

	GetContentsOffset(xOffset, yOffset);

	nLeftSideWidth=0;
	if (m_mode==eUDM_FULL)
	{
		nLeftSideWidth=GetChild(CONTROL_OFFSET+2)->m_w;
	}

	child=GetChild(CONTROL_OFFSET+6);
	child->Move(xOffset+nLeftSideWidth+GetFontHeight(True)*2, yOffset);

	child=GetChild(CONTROL_OFFSET+21);
	maxWidth = child->m_w+child->m_x-xOffset;

	child=GetChild(CONTROL_OFFSET+21);
	maxHeight =child->m_h+child->m_y-yOffset;

	if (m_mode==eUDM_FULL)
	{
		child=GetChild(CONTROL_OFFSET+1);
		child->Move(xOffset, yOffset);

		child=GetChild(CONTROL_OFFSET+3);
		child->ReSize(nLeftSideWidth, GetChild(CONTROL_OFFSET+20)->m_y-child->m_y);
		child->Update(eGUM_FORCED);
	}


	SetContentsSize(maxWidth, maxHeight);

	xSetValue(m_condition1, m_condition2);

	if (m_mode==eUDM_RPY)
	{
		cglsStackPannel* panel = (cglsStackPannel*)GetChild(DIALOG_ITEM_ID_BUTTON_PANEL);
		if (panel!=0)
		{
			panel->EnableItem(1, True);
			panel->EnableItem(3, False);
		}
	}

	xShowContents();
	InvalidateRect(0, 0, True);
}

#include "glsPopupEdit.h"
#include "yigRoomNoPopup.h"
// ------------------------------------
LRESULT cyigUserDialog::OnCommand(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int id=wParam&0xFFFF;
	switch(id)
	{
		// 手合い
		case CONTROL_OFFSET+10:
		{
			xShowNigiri();

			break;
		}
		// 対局条件
		case CONTROL_OFFSET+7:
		{
			unsigned int con1, con2;
			cglsSplitButton* btn = (cglsSplitButton*)GetChild(id);
			if (cyigUserDialog::xDefaultCondition(btn->GetSelectIndex(), con1, con2)==False)
			{
				return 0;
			}
			//m_condition1=con1;
			//m_condition2=con2;
			xSetValue(con1, con2);
			break;
		}
		// 条件保存
		case CONTROL_OFFSET+8:
		{
			cGlassInterface* child=GetChild(CONTROL_OFFSET+8);
			int x=child->m_x;
			int y=child->m_y+child->m_h;
			POINT pt={x, y};
			ClientToScreen(*this, &pt);
			cglsPopupEdit edit(ePEM_NORMAL, 7, m_font);
			edit.TrackPopup(*this, pt.x, pt.y);
			wchar_t name[32];
			edit.GetText(name, 32);

			cglsSplitButton* split=(cglsSplitButton* )GetChild(CONTROL_OFFSET+7);
			split->AddItem(16, 0, name, 0, True);
			split->Update(eGUM_FORCED);

			break;
		}
		case CONTROL_OFFSET+2:
		{
			cglsSplitButton* btn = (cglsSplitButton*)GetChild(id);
			int n = btn->GetSelectIndex();

			// ID指定
			if (n==(btn->GetItemCount()-1))
			{
				POINT pt={btn->m_x, btn->m_y};
				ClientToScreen(*btn, &pt);
				cyigRoomNoPopup edit;
				//cglsPopupEdit edit(eglsPopupEditMode::ePEM_NUMERIC, 5, &Font(L"Meiryo", 13.0));
				if (edit.TrackPopup(*this, pt.x, pt.y, 5, m_font)==True)
				{
					cglsSystem* system = GetSystem();
					
					cypnGoNetwork* network =  (cypnGoNetwork*)cypnGoNetwork::GetInstance();
					sypnUserInfo info;
					if (network->Connect(0, 0, edit.m_roomNo, edit.m_szPassword, &info)==-1)
					{
						cglsMessageBox msg;
						msg.Show(this, L"相手の部屋に接続できません。\r\n部屋番号や部屋のパスワードを確認してください", L"エラー", MB_OK, m_font);
						break;
					}

					//HandleUser hUser = network->IndexToUser(info.index);
					//sGoUserInfo* info = cypnGoNetwork::GetGoUserInfo();

					//cglsListBox* listbox = (clgsListBox*)GetChild(CONTROL_OFFSET+3);
					//listbox->Clear();
					//listbox->AddItem(1, info->szName, info);
					//listbox->SelectItem(0, True);
				}
				break;
			}
		}
		case CONTROL_OFFSET+15:
		{
			xEnalbeLimitedTime();
			break;
		}
		case CONTROL_OFFSET+17:
		{
			xEnalbeReadSecond();
			break;
		}
		case IDOK:
		{
			xGetValue(m_condition1, m_condition2);
			break;
		}
		//case IDCANCEL
		//{
		//	break;
		//}
		case 3:
		{
			xGetValue(m_condition1, m_condition2);
			m_bIsExitLoop=True;
			m_returnValue=3;
			break;
		}
		default:
			break;
	}

	if (m_mode==eUDM_RPY)
	{
		cglsStackPannel* panel = (cglsStackPannel*)GetChild(DIALOG_ITEM_ID_BUTTON_PANEL);

		cGlassInterface* child = GetChild(1);

		if (panel!=0)
		{
			unsigned int con1, con2;
			xGetValue(con1, con2);

			if (IS_NIGIRI(con1)==False)
			{
				if (con1==m_condition1 &&
						con2==m_condition2)
				{
					panel->EnableItem(1, True);
					panel->EnableItem(3, False);
				}
				else
				{
					panel->EnableItem(1, False);
					panel->EnableItem(3, True);
				}
			}
			else
			{
				if ((0x7FFFFFFF&con1)==(0x7FFFFFFF&m_condition1) &&
						con2==m_condition2)
				{
					panel->EnableItem(1, True);
					panel->EnableItem(3, False);
				}
				else
				{
					panel->EnableItem(1, False);
					panel->EnableItem(3, True);
				}

			}
		}
	}
	return __super::OnCommand(wParam, lParam, handled);
}

// -----------------------------------
void cyigUserDialog::xEnalbeLimitedTime()
{
	cglsCheckGroup* check=(cglsCheckGroup*)GetChild(CONTROL_OFFSET+15);
	Bool bIsEnalbe = !check->GetChecked(0);
	if (check==0)
		return ;

	cGlassInterface* child=GetChild(CONTROL_OFFSET+16);

	if (child==0)
		return ;

	GetChild(CONTROL_OFFSET+16)->Enable(bIsEnalbe);
	GetChild(CONTROL_OFFSET+25)->Enable(bIsEnalbe);

	GetChild(CONTROL_OFFSET+17)->Enable(bIsEnalbe);

	xEnalbeReadSecond();
	//GetChild(CONTROL_OFFSET+18)->Enable(bIsEnalbe);
	//GetChild(CONTROL_OFFSET+19)->Enable(bIsEnalbe);
	//GetChild(CONTROL_OFFSET+24)->Enable(bIsEnalbe);
}


// ------------------------------------
void cyigUserDialog::UpdateUserListForNoId()
{
	//Bool bIsChanged=False;
	//cglsListBox* listbox = (cglsListBox*)GetChild(CONTROL_OFFSET+3);
	//int cnt=listbox->GetItemCount();
	//int nFoundCount=0;

	//if (m_userz==0)
	//	return ;

	//for (CUserInfo* info =m_userz->GetNodeTopFrom(); info; info=info->np)
	//{
	//	sGoUserInfo* user = info->ref;
	//	
	//	if (user->id==0)
	//	{
	//		sSimpleUserInfo* x = (sSimpleUserInfo*)listbox->GetItemParam(nFoundCount++);

	//		if (x==0)
	//			bIsChanged=True;
	//		else if (x->id!=0)
	//			bIsChanged=True;
	//		else if (x->room!=user->room)
	//			bIsChanged=True;


	//	}
	//	if (bIsChanged==True)
	//		break;
	//}

	//if (bIsChanged==True)
	//{
	//	xTRACE("ListBox is changed!!");
	//	listbox->Clear();
	//	int id=1;
	//	for (CUserInfo* info =m_userz->GetNodeTopFrom(); info; info=info->np)
	//	{
	//		sGoUserInfo* user = info->ref;
	//		
	//		if (user->id==0)
	//		{
	//			listbox->AddItem(id++, 0, user->szName, user);
	//		}
	//	}

	//	if (listbox->GetItemCount()==1)
	//	{
	//		listbox->SelectItem(0);
	//	}
	//	listbox->Update(eGUM_FORCED);
	//	listbox->InvalidateRect(0);
	//}
}

// ------------------------------------
//void cyigUserDialog::UpdateUserForMyFriends()
//{
//	Bool bIsChanged=False;
//	cglsListBox* listbox = (cglsListBox*)GetChild(CONTROL_OFFSET+3);
//	int cnt=listbox->GetItemCount();
//	int nFoundCount=0;
//
//	if (m_userz==0)
//		return ;
//		
//	for (CUserInfo* info =m_userz->GetNodeTopFrom(); info; info=info->np)
//	{
//		sGoUserInfo* user = info->ref;
//		
//		if ((user->state&eGIUS_MYFRIEND))
//		{
//			sSimpleUserInfo* x = (sSimpleUserInfo*)listbox->GetItemParam(nFoundCount++);
//
//			if (x==0)
//				bIsChanged=True;
//			
//			if (x->id!=user->id)
//				bIsChanged=True;
//
//			if (x->room!=user->room)
//				bIsChanged=True;
//		}
//		if (bIsChanged==True)
//			break;
//	}
//
//	if (bIsChanged==True)
//	{
//		listbox->Clear();
//		int id=1;
//		for (CUserInfo* info =m_userz->GetNodeTopFrom(); info; info=info->np)
//		{
//			sGoUserInfo* user = info->ref;
//			
//			if ((user->state&eGIUS_MYFRIEND))
//			{
//				listbox->AddItem(id++, 0, user->szName, user);
//			}
//		}
//		listbox->Update(eGUM_FORCED);
//		listbox->InvalidateRect(0);
//		
//	}
//}

// ------------------------------------
//LRESULT cyigUserDialog::OnTimer(WPARAM wParam, LPARAM lParam, Bool& handled)
//{
//
//	// ここで値を更新
//	cglsSplitButton* btn = (cglsSplitButton*)GetChild(CONTROL_OFFSET+2);
//
//	int nIndex = btn->GetSelectIndex();
//
//	// myfriends
//	if (nIndex==0)
//	{
//		UpdateUserForMyFriends();
//	}
//	// IDなしログイン
//	else if (nIndex==btn->GetItemCount()-1)
//	{
//		UpdateUserListForNoId();
//	}
//	// 一般対局室
//	if (nIndex==1)
//	{
//		// Connect Group (0)
//
//		// RequestPlayableUserList 
//
//
//	}
//	// ログイン済みのグループ
//	else
//	{
//		
//	}
//
//	return 0;
//}



// -----------------------------------
void cyigUserDialog::xShowNigiri()
{
	cglsSplitButton* btn = (cglsSplitButton*)GetChild(CONTROL_OFFSET+10);
	int n = btn->GetSelectIndex();

	cglsStackPannel* panel = (cglsStackPannel* )GetChild(CONTROL_OFFSET+11);
	cglsRadioGroup* radi=(cglsRadioGroup*)panel;
	int nChecked=radi->GetChecked();

	if (n!=0)
	{
		if (nChecked==2)
			radi->SetChecked(0);

		panel->ShowItem(False, 3);
		panel->Update(eGUM_FORCED);

		cglsSpinBox* spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+13);
		cglsCheckGroup* check = (cglsCheckGroup*)GetChild(CONTROL_OFFSET+14);

		spin->SetValue(0, eSBVF_ZERO, False);
		check->SetChecked(0, False);
		check->SetChecked(1, False);

	}
	else
	{
		radi->SetChecked(2);
		panel->ShowItem(True, 3);
		panel->Update(eGUM_FORCED);

		// コミをゼロにする。
		cglsSpinBox* spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+13);
		cglsCheckGroup* check = (cglsCheckGroup*)GetChild(CONTROL_OFFSET+14);

		spin->SetValue(6, eSBVF_ZERO, False);
		check->SetChecked(0, True);
		check->SetChecked(1, False);

	}
}

// -----------------------------------
void cyigUserDialog::xEnalbeReadSecond()
{
	cglsCheckGroup* check = (cglsCheckGroup* )GetChild(CONTROL_OFFSET+17);

	if (check==0)
	{
		return ;
	}

	if (::IsWindowEnabled(*check)==FALSE)
	{
		GetChild(CONTROL_OFFSET+18)->Enable(False);
		GetChild(CONTROL_OFFSET+19)->Enable(False);
		GetChild(CONTROL_OFFSET+24)->Enable(False);
		return ;
	}

	if (GetChild(CONTROL_OFFSET+18)==0)
	{
		return ;
	}
	
	Bool bIsEnalbe = check->GetChecked(0);
	//GetChild(CONTROL_OFFSET+17)->Enable(bIsEnalbe);
	GetChild(CONTROL_OFFSET+18)->Enable(bIsEnalbe);
	GetChild(CONTROL_OFFSET+19)->Enable(bIsEnalbe);
	GetChild(CONTROL_OFFSET+24)->Enable(bIsEnalbe);


}

// -----------------------------------
void cyigUserDialog::xResetControlState()
{
	xShowNigiri();
	xEnalbeLimitedTime();
	xEnalbeReadSecond();
}


void cyigUserDialog::xShowContents()
{
	for (int i=1; i<26; i++)
	{
		cGlassInterface* obj = GetChild(CONTROL_OFFSET+i);

		if (obj)
			obj->Show();
	}

}

// -----------------------------------
void cyigUserDialog::xGetValue(unsigned int& condition1, unsigned int& condition2)
{
	//int condition1=0, condition2=0;
	condition1=0;
	condition2=0;

	// 持ち時間
	{
		cglsCheckGroup* check = (cglsCheckGroup*)GetChild(CONTROL_OFFSET+15);
		
		// 持ち時間あり
		if (check->GetChecked(0)==False)
		{
			int hour=0, minute=0;
			cglsSpinBox* spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+16);
			hour = spin->GetValue();
			spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+25);
			minute = spin->GetValue()*5;

			minute+=(hour*60);

			SET_PLAY_TIME(condition1, minute);

		}
	}

	// 秒読み設定
	{
		cglsCheckGroup* check = (cglsCheckGroup* )GetChild(CONTROL_OFFSET+17);
		Bool bIsSecondRead=check->GetChecked(0);//, False);

		if (bIsSecondRead)
		{
			// 秒読み：18 spin
			cglsSpinBox* spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+18);
			SET_SECOND_READ(condition1, spin->GetValue()*10);
			

			// 秒読み：19 spin
			spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+24);
			SET_SECOND_READ2(condition1, spin->GetValue()*30);


			// 秒読み：24 spin
			spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+19);
			SET_SECOND_READ_COUNT(condition2, spin->GetValue());//nSecondReadCount, eSBVF_ZERO, False);
		}
		//spin->InvalidateRect(0);
	}

	// 手合い
	{
		cglsSplitButton* btn = (cglsSplitButton*)GetChild(CONTROL_OFFSET+10);
		int n = btn->GetSelectIndex();

		cglsRadioGroup* radio = (cglsRadioGroup*)GetChild(CONTROL_OFFSET+11);

		int bIBlack=radio->GetChecked();

		if (n==0)
		{
			if (bIBlack==2)
			{
				srand(cyiTime::CurrentmSec());
				bIBlack=(rand()%2);
			}

		}

		if (bIBlack==0)
			SET_IAM_BLACK(condition1);
		else
			SET_IAM_WHITE(condition1);

		if (n>0)
		{
			SET_HANDICAP(condition1);

			if (n>1)
				SET_INIT_STONE(condition1, n);
		}
	}

	// コミ設定
	{
		cglsSpinBox* spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+13);
		cglsCheckGroup* check = (cglsCheckGroup*)GetChild(CONTROL_OFFSET+14);

		SET_KOMI(condition1, spin->GetValue());

		if (check->GetChecked(0)==True)
			SET_HALF_SCORE(condition1);

		if (check->GetChecked(1)==True)
			SET_REVERSE_KOMI(condition1);
	}

	// オプション
	{
		cglsCheckGroup* check = (cglsCheckGroup*)GetChild(CONTROL_OFFSET+21);
		Bool bIsWaitable = check->GetChecked(0);
		Bool bIsHuu = check->GetChecked(1);
		Bool bIsPause = check->GetChecked(2);

		SET_WAITABLE_COUNT(condition2, bIsWaitable);
		SET_HUU_COUNT(condition2, bIsHuu);

		if (bIsPause)
			SET_PLAYPAUSE(condition2);
		else
			OFF_PLAYPAUSE(condition2);

		check->InvalidateRect(0);

	}

	//m_condition1=condition1;
	//m_condition2=condition2;
}

// -----------------------------------
void cyigUserDialog::xSetValue(unsigned int m_condition1, unsigned int m_condition2)
{
	// 持ち時間設定
	{
		if (IS_LIMITED_PLAY_TIME(m_condition1)==False)
		{
			cglsCheckGroup* check = (cglsCheckGroup*)GetChild(CONTROL_OFFSET+15);
			check->SetChecked(0, True);

			cglsSpinBox* spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+16);
			spin->SetValue(0, eSBVF_ZERO, False);
			spin->InvalidateRect(0);

			spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+25);
			spin->SetValue(0, eSBVF_ZERO, False);
			spin->InvalidateRect(0);
		}
		else
		{
			int nTime=GET_PLAY_TIME(m_condition1);
	
			cglsSpinBox* spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+16);
			spin->SetValue(nTime/60, eSBVF_ZERO, False);
			spin->InvalidateRect(0);

			spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+25);
			spin->SetValue((nTime%60)/5, eSBVF_ZERO, False);
			spin->InvalidateRect(0);

			cglsCheckGroup* check = (cglsCheckGroup*)GetChild(CONTROL_OFFSET+15);
			check->SetChecked(0, False);
			//check->InvalidateRect(0);
		}
	}

	// 秒読み設定
	{
		int nSecondRead = GET_SECOND_READ(m_condition1);
		int nSecondRead2 = GET_SECOND_READ2(m_condition1);
		int nSecondReadCount=GET_SECOND_READ_COUNT(m_condition2);

		if (nSecondRead==0 &&
				nSecondRead2==0 &&
					nSecondReadCount==0)
		{
			cglsCheckGroup* check = (cglsCheckGroup* )GetChild(CONTROL_OFFSET+17);
			check->SetChecked(0, False);
		}
		else
		{
			cglsCheckGroup* check = (cglsCheckGroup* )GetChild(CONTROL_OFFSET+17);
			check->SetChecked(0, True);
		}

		// 秒読み：18 spin
		cglsSpinBox* spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+18);
		spin->SetValue(nSecondRead/10, eSBVF_ZERO, False);
		spin->InvalidateRect(0);


		// 秒読み：19 spin
		spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+24);
		spin->SetValue(nSecondRead2/30, eSBVF_ZERO, False);
		spin->InvalidateRect(0);

		// 秒読み：24 spin
		spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+19);
		spin->SetValue(nSecondReadCount, eSBVF_ZERO, False);
		spin->InvalidateRect(0);
	}

	// コミ設定
	{
		int nKomi		= GET_KOMI(m_condition1);
		Bool bIsHalf	= HALF_SCORE(m_condition1);
		Bool bIsReverse = REVERSE_KOMI(m_condition1);

		cglsSpinBox* spin = (cglsSpinBox*)GetChild(CONTROL_OFFSET+13);
		spin->SetValue(nKomi, eSBVF_ZERO, False);
		spin->InvalidateRect(0);

		cglsCheckGroup* check = (cglsCheckGroup*)GetChild(CONTROL_OFFSET+14);
		
		check->SetChecked(0, bIsHalf);
		check->SetChecked(1, bIsReverse);
	}

	// 手合い
	{
		int nCheckedIndex = IAM_BLACK(m_condition1)==True?0:1;
		Bool bIsNigiri = IS_NIGIRI(m_condition1);
		int  n = GET_INIT_STONE(m_condition1);

		int nSelectedIndex=0;

		if (bIsNigiri==False &&
				n==1)
		{
			nSelectedIndex=1;
		}
		else
		{
			nSelectedIndex=n;
		}

		cglsSplitButton* btn = (cglsSplitButton*)GetChild(CONTROL_OFFSET+10);

		btn->SelectIndex(nSelectedIndex);

		cglsRadioGroup* radio = (cglsRadioGroup*)GetChild(CONTROL_OFFSET+11);

		if (nSelectedIndex==0)
			radio->SetChecked(2);
		else
			radio->SetChecked(nCheckedIndex);
	}

	// オプション
	{
		Bool bIsWaitable = GET_WAITABLE(m_condition2);
		Bool bIsHuu = GET_HUU_COUNT(m_condition2);
		Bool bIsPause = IS_PLAYPAUSE(m_condition2);

		cglsCheckGroup* check = (cglsCheckGroup*)GetChild(CONTROL_OFFSET+21);
		
		check->SetChecked(0, bIsWaitable);
		check->SetChecked(1, bIsHuu!=0);
		check->SetChecked(2, bIsPause);

		check->InvalidateRect(0);
	}
}

// ------------------------------------
void cyigUserDialog::OnExitLoop()
{
	//xGetValue(m_condition1, m_condition2);

	cglsListBox* listbox = (cglsListBox*)GetChild(CONTROL_OFFSET+3);

	m_hSelectedUser=0;
	if (listbox)
	{
		m_hSelectedUser = (HandleUser)listbox->GetSelectedItemParam();
	}

	Destroy();

}