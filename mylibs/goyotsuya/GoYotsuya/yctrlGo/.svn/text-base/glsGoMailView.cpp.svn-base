#include "glsGoMailView.h"

cglsGoMailView::cglsGoMailView(void)
: m_nLastSelctedTabId(0)
{
}

cglsGoMailView::~cglsGoMailView(void)
{
}

const wchar_t* g_interval[] = {
	L"15秒",
	L"30秒",
	L"1分",
	L"5分",
	L"10分",
	L"30分",
	L"1時間",
	0
};


#define xID 16

DEFINE_RESOURCE_BEGIN(_gRESOURCE)
	DEFINE_RESOURCE(xID, eGIT_PICTURE, 0, 0, 9)//xID+3, eGP_RIGHTSIDE|eGP_TOP, eIM_X) // resize必要
	// 自分の情報
	DEFINE_RESOURCE(xID+1, eGIT_LABEL, L"", xID+6, eGP_RIGHTSIDE|eGP_TOP, 0)
	DEFINE_RESOURCE(xID+2, eGIT_LABEL, L"", xID+1, eGP_BOTTOMSIDE|eGP_LEFT, 0)

	// Pop3サーバ
	DEFINE_RESOURCE(xID+16, eGIT_LABEL, L"pop3サーバ：", 0, 0, 0)//xID+3, eGP_RIGHTSIDE|eGP_TOP, eIM_X) // resize必要
	DEFINE_EDIT_BOX(xID+17, 10, True, False, 80, xID+16, eGP_BOTTOMSIDE|eGP_LEFT, 0)
	DEFINE_RESOURCE(xID+18, eGIT_LABEL, L"ポート：", xID+17, eGP_BOTTOMSIDE|eGP_LEFT, 0)//xID+3, eGP_RIGHTSIDE|eGP_TOP, eIM_X) // resize必要
	DEFINE_EDIT_BOX(xID+19, 5, True, False, 80, xID+18, eGP_BOTTOMSIDE|eGP_LEFT, 0)
	DEFINE_RESOURCE(xID+24, eGIT_CHECK, L"SSLを使う", xID+16, eGP_RIGHTSIDE|eGP_BOTTOM, eIM_X)

	// ID, password.
	DEFINE_RESOURCE(xID+20, eGIT_LABEL, L"ID：", xID+24, eGP_LEFTSIDE|eGP_BOTTOM, 0)//xID+3, eGP_RIGHTSIDE|eGP_TOP, eIM_X) // resize必要
	DEFINE_EDIT_BOX(xID+21, 10, True, False, 80, xID+20, eGP_BOTTOMSIDE|eGP_LEFT, 0)
	DEFINE_RESOURCE(xID+22, eGIT_LABEL, L"パスワード：", xID+20, eGP_LEFTSIDE|eGP_BOTTOM, 0)//xID+3, eGP_RIGHTSIDE|eGP_TOP, eIM_X) // resize必要
	DEFINE_EDIT_BOX(xID+23, 10, True, False, 80, xID+22, eGP_BOTTOMSIDE|eGP_LEFT, 0)

	// 表示名
	DEFINE_RESOURCE(xID+25, eGIT_LABEL, L"表示名：", xID+22, eGP_LEFTSIDE|eGP_BOTTOM, 0)//xID+3, eGP_RIGHTSIDE|eGP_TOP, eIM_X) // resize必要
	DEFINE_EDIT_BOX(xID+26, 10, False, False, 80, xID+25, eGP_BOTTOMSIDE|eGP_LEFT, 0)
	// メールアドレス
	DEFINE_RESOURCE(xID+27, eGIT_LABEL, L"メールアドレス：", xID+25, eGP_LEFTSIDE|eGP_BOTTOM, 0)//xID+3, eGP_RIGHTSIDE|eGP_TOP, eIM_X) // resize必要
	DEFINE_EDIT_BOX(xID+28, 10, False, False, 80, xID+27, eGP_BOTTOMSIDE|eGP_LEFT, 0)
	// 受信間隔
	DEFINE_RESOURCE(xID+29, eGIT_LABEL, L"受信間隔", xID+27, eGP_LEFTSIDE|eGP_BOTTOM, 0)//xID+3, eGP_RIGHTSIDE|eGP_TOP, eIM_X) // resize必要
	DEFINE_SPILIT_BUTTON(xID+30, g_interval, 0, xID+29, eGP_RIGHTSIDE|eGP_BOTTOM, 0, 5, 0)

DEFINE_RESOURCE_END()

// ----------------------------
Bool cglsGoMailView::Create(cGlassInterface* parent, int x, int y, int w, int h, cyiList* servers, Font* font)
{
	sGlsDialogDetail xdetail;
	cglsDialog::xDefaultDetail(&xdetail);

	xdetail.font=_font?_font:&font;
	xdetail.gauss.fBlureRad=5.0;
	xdetail.gauss.fShadowWidth=5.0;
	xdetail.gauss.colorTop=__COLORp__(128, 233,255,200);

	m_bIsSizeFixedMode=False;
	m_fMarginX=1.0;
	m_fMarginY=1.0;
	xdetail.bIsAnimationShow=True;
	xdetail.nShowAnimationTime=200;
	xdetail.nHideAnimationTime=300;

	m_servers=servers;

	m_eStyle=style;
	Bool res = cglsDialog::Create((HWND)parent, 256 , 0, x, y, w, h, &xdetail);

	if (res==False)
		return False;

	AnimationShow(True, 1000);

	return True;
}

// ------------------------------------
LRESULT cglsGoMailView::OnCreate(WPARAM wParam, LPARAM lParam)
{
	sGlsStaticDetail detail;
	cglsStatic* label;

	// 3. タブ生成
	cglsTabCtrl* tab = new cglsTabCtrl();
	{
		sGlsTabDetail detail;
		int tabWidth=nSmallFontHeight*10;//GetFontHeight(True)*10;
		cglsTabCtrl::xDefaultDetail(&detail);
		detail.font=fontSmall;

		//detail.stack.bIsVertical=True;
		detail.wMaxShowCount=1;
		detail.wMinShowCount=1;

		tab->Create((HWND)this,xID+3, 0, 0, tabWidth, &detail);
		//tab->AddItem(1, 0, L"友人", False);

		sGlsTabItem item;

		cglsTabCtrl::xDefaultItemDetail(&item, Color::LightBlue);
		tab->AddItem(1, 0, L"メッセージ", False, &item);
		tab->AddItem(2, 0, L"対局", False, &item);

		cglsTabCtrl::xDefaultItemDetail(&item, Color::LightGray);
		tab->AddItem(2, 0, L"設定", False, &item);

		tab->Show();

		GetChild(xID)->AddPositionLink(tab, eGP_BOTTOMSIDE|eGP_LEFT);
	}

	// 4. リストビュー(メッセージ生成)
	cglsListView* listView = new cglsListView();
	listView->Create(this, xID+4, 0, 0, 10, nSmallFontHeight*10, fontSmall);
	listView->CreateColumn(7, &sListViewColumn(12, L"差出人", eGIA_LEFT, eGCT_STRING), 
									&sListViewColumn(12, L"タイトル", eGIA_LEFT, eGCT_STRING), 
										&sListViewColumn(5, L"受信日時", eGIA_LEFT));

	listView->Hide();

	GetChild(xID+3)->AddPositionLink(listView, eGP_BOTTOMSIDE|eGP_LEFT);

	// 5. リストビュー(棋譜生成)
	listView = new cglsListView();
	listView->Create(this, xID+8, 0, 0, 10, nSmallFontHeight*10, fontSmall);
	listView->CreateColumn(7, &sListViewColumn(12, L"黒", eGIA_LEFT, eGCT_STRING), 
									&sListViewColumn(12, L"白", eGIA_LEFT, eGCT_STRING), 
										&sListViewColumn(5, L"手合", eGIA_LEFT),
											&sListViewColumn(5, L"手数", eGIA_LEFT, eGCT_NUMERIC), 
												&sListViewColumn(3, L"勝"),
													&sListViewColumn(6, L""),
														&sListViewColumn(11, L"日時"));

	listView->Hide();
	GetChild(xID+3)->AddPositionLink(listView, eGP_BOTTOMSIDE|eGP_LEFT);

	cglsSplitButton* btn = new cglsSplitButton();
	sGlsSplitButtonDetail split;
	cglsSplitButton::xDefaultDetail(&split);
	btn->Create((HWND)this, xID+9, 0, 0, GetFontHeight(True)*4, 0, 0, &split);

	GetChild(xID+3)->AddPositionLink(GetChild(xID+16), eGP_BOTTOMSIDE|eGP_LEFT);

	// 5. 設定コントロール生成
	for (int i=xID+16; i<xID+31; i++)
	{
		cGlassInterface* child = GetChild(i);
		if (child==0)
			continue;
		child->Hide();
	}

	UpdateMyView(1); // -> update forced.
	return ReArrangeItemz();
}

// ------------------------------------
void cglsGoMailView::OnItemReArranged()
{
}

typedef struct {
	unsigned int id;
	char displayName[32];
	char email[80];
	char popserver[32];
	char user[16];
	char pass[16];
	char charset[16];
	int  port;
	Bool bUseSsl;
	HandleMutexx hMutex;
} sPopServer;

// ----------------------------
Bool cglsGoMailView::UpdateMyView(int tabIndex)
{
	int cnt=0;
	// 1. まず、splitボタンを更新
	cglsSplitButton* btn = (cglsSplitButton*)GetChild(xID+9);
	btn->Clear();
	
	if (m_servers)
	{
		for (cyiNode* node=m_servers->GetNodeTopFrom(); node; node=node->np)
		{
			sPopServer* svr = (sPopServer*)node->ref;
			btn->AddItem(svr->id, 0, svr->displayName, svr->id==m_nLastSelectServer);
		}
		cnt = m_servers->count();
	}

	if (cnt==0)
	{
		// 登録されたサーバなし
		cGlassInterface::Update(eGUM_FORCED);
		return True;
	}

	int index = btn->GetSelectIndex();
	sPopServer* svr = m_servers->GetTopFromRef(index);

	// 表示名
	cglsStatic* label = (cglsStatic*)GetChild(xID+1);
	label->SetText(0, svr->displayName);
	label = (cglsStatic*)GetChild(xId+2);
	label->SetText(0, svr->email);
	
	// イメージ
	cglsImageBox* img = (cglsStatic*)GetChild(xID);
	img->SetImage(svr->img, eGIBVS_FIX);

	cGlassInterface::Update(eGUM_FORCED);

	tabIndex=tabIndex?tabIndex:m_nLastSelctedTabId;
	return True;
}
