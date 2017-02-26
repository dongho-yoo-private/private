#include "glsMyView.h"

#define xID 16

DEFINE_RESOURCE_BEGIN(_gRESOURCE)
	// ピクチャー
	DEFINE_RESOURCE(xID+6, eGIT_PICTURE, 0, 0, 9)//xID+3, eGP_RIGHTSIDE|eGP_TOP, eIM_X) // resize必要
	// 自分の情報
	DEFINE_RESOURCE(xID+1, eGIT_LABEL, L"", xID+6, eGP_RIGHTSIDE|eGP_TOP, eIM_SMALL_X)
	// 状態
	DEFINE_SPILIT_BUTTON(xID+2, 0, 0, xID+1, eGP_RIGHTSIDE|eGP_BOTTOM, 0, 5, eIM_SMALL_X)
	// 最終ログイン時刻
	DEFINE_RESOURCE(xID+3, eGIT_LABEL, L"", xID+1, eGP_BOTTOMSIDE|eGP_LEFT, eIM_Y)
	// 戦績
	DEFINE_RESOURCE(xID+4, eGIT_LABEL, L"", xID+3, eGP_BOTTOMSIDE|eGP_LEFT, eIM_SMALL_Y)
	// マイポイント
	DEFINE_RESOURCE(xID+5, eGIT_LABEL, L"", xID+4, eGP_BOTTOMSIDE|eGP_LEFT, eIM_SMALL_Y)

	// ID無しログイン
	DEFINE_RESOURCE(xID+16, eGIT_LABEL, L"", 0, 0, 0)

	// 部屋番号
	DEFINE_RESOURCE(xID+17, eGIT_LABEL, L"接続先の部屋番号", 0, 0, 0)

	// 部屋番号（Edit Box)
	DEFINE_EDIT_BOX(xID+18, 5, True, True, 5, xID+17, eGP_RIGHTSIDE|eGP_BOTTOM, eIM_SMALL_X)

	// パスワード
	DEFINE_RESOURCE(xID+19, eGIT_LABEL, L"パスワード", xID+17, eGP_BOTTOMSIDE|eGP_LEFT, eIM_SMALL_Y)

	// パスワード（Edit Box)
	DEFINE_EDIT_BOX(xID+20, 5, True, True, 5, xID+18, eGP_BOTTOMSIDE|eGP_LEFT, eIM_SMALL_Y)

	// グレード
	//DEFINE_RESOURCE(xID+7, eGIT_LABEL, L"", xID+6, eGP_BOTTOMSIDE|eGP_LEFT, eIM_SMALL_Y)
	// グループへログイン
	//DEFINE_RESOURCE(xID+10, eGIT_LABEL, L"ログイン", xID+6, eGP_BOTTOMSIDE|eGP_LEFT,  eIM_LARGE_Y)
	// グループ一覧
	//DEFINE_SPILIT_BUTTON(xID+8, 0, 0, xID+10, eGP_RIGHTSIDE|eGP_VCENTER, 0, 7, eIM_SMALL_X)

	//// ユーザ表示
	//DEFINE_SPILIT_BUTTON(xID+11, 0, 0, xID+10, eGP_BOTTOMSIDE|eGP_LEFT, 0, 5, eIM_LARGE_Y)
	//// ユーザ一覧
	//DEFINE_RESOURCE_PARAM(xID+12, eGIT_LISTBOX, L"", xID+11, eGP_BOTTOMSIDE|eGP_LEFT, 8, 10, 0)

	//// 追加ボタン
	//DEFINE_RESOURCE_PARAM(xID+13, eGIT_BUTTON, L"追加", xID+11, eGP_RIGHTSIDE|eGP_BOTTOM, 0, 0, 0)

	//// グループ登録
	//DEFINE_RESOURCE_PARAM(xID+14, eGIT_BUTTON, L"グループ登録", xID+8, eGP_RIGHTSIDE|eGP_BOTTOM, 0, 0, 0)

	// ユーザ一覧
	//DEFINE_RESOURCE_PARAM(xID+13, eGIT_LISTBOX, L"", xID+11, eGP_RIGHTSIDE|eGP_TOP, 10, 6, 0)


DEFINE_RESOURCE_END()

cglsMyView::cglsMyView(void)
: m_nGroupChangedCommand(0)
, m_nPlayCommand(0)
, m_nWatchCommand(0)
, m_ygf_header(0)

{
}

cglsMyView::~cglsMyView(void)
{
}


// ------------------------------------
Bool cglsMyView::Create(cGlassInterface* parent, int x, int y, int w, int h, CUserManager* mgr, Font* _font, eMyViewStyle style)
{
	sGlsDialogDetail xdetail;
	cglsDialog::xDefaultDetail(&xdetail);

	m_mgr=mgr;
	float fFontSize = 12.0;
	Font font(L"Meiryo UI", fFontSize);


	xdetail.font=_font?_font:&font;
	xdetail.gauss.fBlureRad=5.0;
	xdetail.gauss.fShadowWidth=5.0;
	xdetail.gauss.colorTop=__COLORp__(128, 200,255,200);
	//xdetail.gauss.colorTop=;
	
	//m_myinfo=*info;
	m_bIsSizeFixedMode=False;
	m_fMarginX=1.0;
	m_fMarginY=1.0;
	xdetail.bIsAnimationShow=True;
	xdetail.nShowAnimationTime=200;
	xdetail.nHideAnimationTime=300;

	m_listGo[0]=new cyiList(True);
	m_listGo[1]=new cyiList(True);

	memset(m_filter, 0, sizeof(m_filter));

	m_eStyle=style;
	Bool res = cglsDialog::Create((HWND)parent, 256 , 0, x, y, w, h, &xdetail);

	sGlsAnimationEx ani;
	glsInitAnimationEx(&ani, &m_item);
	glsSetTrAnimationEx(&ani, 0.0, 1.0, eGAP_FISO);
	glsSetSzAnimationEx(&ani, 0.0, 1.0, eGAP_FISO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));
	AnimationShow(True, &ani, 20, 1000);


	//if (_font)
	//	delete _font;

	return res;
}

// ------------------------------------
void cglsMyView::SetGroupChangedId(int id)
{
	m_nGroupChangedCommand=id;
	return ;
}

// ------------------------------------
void cglsMyView::SetPlayCommandId(int id)
{
	m_nPlayCommand=id;
	return;
}

// ------------------------------------
void cglsMyView::SetWatchCommandId(int id)
{
	m_nWatchCommand=id;
	return;
}
#include "glsListView.h"

// ----------------------------	--------
void cglsMyView::UpdateMyView(eMyViewStyle style)
{
	wchar_t buffer[256];
	cglsStatic* label=0;
	m_eStyle=style;

	if (style!=eMVS_NORMAL)
		goto SKIP_NORMAL;

	unsigned int seq;
	cglsImageBox* img = (cglsImageBox*)GetChild(xID+6);
	img->ReSize(GetFontHeight(True)*5, GetFontHeight(True)*5);
	img->SetImage(m_mgr->GetPicture(0, seq), eGIBVS_TRIM_BORDER_FIXED);

	label = (cglsStatic*)GetChild(xID+1);
	label->SetText(0, m_mgr->GetMyName());

	unsigned int lastLoginDay=m_mgr->GetMyBaseInfo()->lastLoginDate;
	unsigned int lastLoginTime=m_mgr->GetMyBaseInfo()->lastLoginTime;
	if (lastLoginDay ==0&&
			lastLoginTime==0)
	{
		wsprintfW(buffer, L"%s 初めてのログイン", L"最終ログイン：");
	}
	else
	{
		wsprintfW(buffer, L"%s %d/%2d/%2d %2d:%2d:%2d", L"最終ログイン：",
			DAY_YEAR(lastLoginDay), DAY_MON(lastLoginDay), DAY_DAY(lastLoginDay),
			xHOUR(lastLoginTime), xMINUTE(lastLoginTime), xSECOND(lastLoginTime));
	}
	label=(cglsStatic*)GetChild(xID+3);
	label->SetText(0, buffer);

	wsprintfW(buffer, L"戦績： %d勝%d負%d分け", (int)m_mgr->GetMyBaseInfo()->wins, (int)m_mgr->GetMyBaseInfo()->loses, (int)m_mgr->GetMyBaseInfo()->draws);
	label=(cglsStatic*)GetChild(xID+4);
	label->SetText(0, buffer);

	wsprintfW(buffer, L"マイポイント： %dpt", (int)m_mgr->GetMyBaseInfo()->point);//>win, (int)info->draws, (int)info->draws);
	label=(cglsStatic*)GetChild(xID+5);
	label->SetText(0, buffer);



	//wsprintfW(buffer, L"マイポイント： %dpt", (int)info->point);//>win, (int)info->draws, (int)info->draws);
	//label=(cglsStatic*)GetChild(xID+7);
	//label->SetText(0, L"-");

	//cglsSplitButton* btn = (cglsSplitButton*)GetChild(xID+8);
	//btn->AddItem(1, 0, L"-", L"ログインなし");
	//btn->AddItem(2, 0, L"一般対局室", L"一般対局室");
	////btn->AddItem(2, 0, L"", L"友人ののみ表示");

	cglsSplitButton* btn = (cglsSplitButton*)GetChild(xID+2);
	btn->AddItem(1, 0, L"対局OK", L"対局を受け付ける");
	btn->AddItem(2, 0, L"対局NG", L"対局を受け付けない");
	btn->AddItem(3, 0, L"対局-", L"友人のみ対局を受け付ける");

	//btn = (cglsSplitButton*)GetChild(xID+11);
	//btn->AddItem(1, 0, L"友人", L"友人のみ表示");
	//btn->AddItem(2, 0, L"グループ", L"グループユーザ表示");
	//btn->AddItem(3, 0, L"お気に入り", L"お気に入り表示");

	for (int i=xID+1; i<xID+15; i++)
	{
		if (i==xID+9)
			continue;
		cGlassInterface* child=GetChild(i);
		if (child)
		{
			//child->Update(eGUM_FORCED);
			child->Show();
		}
	}

	for (int i=xID+32; i<xID+42; i++)
	{
		cGlassInterface* child=GetChild(i);
		if (child)
		{
			//child->Update(eGUM_FORCED);
			child->Show();
		}
	}

	for (int i=xID+16; i<xID+32; i++)
	{
		cGlassInterface* child=GetChild(i);
		if (child)
		{
			//child->Update(eGUM_FORCED);
			child->Hide();
		}
	}

	return ;

SKIP_NORMAL:
	label = (cglsStatic*)GetChild(xID+16);
	label->SetText(0, m_mgr->GetMyName());	

	for (int i=xID+1; i<xID+15; i++)
	{
		cGlassInterface* child=GetChild(i);
		if (child)
		{
			//child->Update(eGUM_FORCED);
			child->Hide();
		}
	}

	for (int i=xID+16; i<xID+32; i++)
	{
		cGlassInterface* child=GetChild(i);
		if (child)
		{
			//child->Update(eGUM_FORCED);
			child->Show();
		}
	}

	GetChild(xID+9)->Show();

}

// ------------------------------------
void cglsMyView::AddGo(int index, sSimpleGoHeader* header, Bool bIsUpdate)
{
	char szBlackName[16];
	char szWhiteName[16];
	wchar_t* pwszWiner;
	wchar_t szResult[8]={0,};
	wchar_t szDay[32];
	wchar_t szTeai[5];
	Bool bIsIamBlack=(header->result&0x80000000)!=0;


	m_listGo[0]->insertref(0,yitoheap(header, sizeof(sSimpleGoHeader)));
	int cnt = m_listGo[0]->count();
	
	const char* pszUserName = m_mgr->GetUserName(header->remoteId);
	if (bIsIamBlack)
	{
		strcpy(szBlackName, m_mgr->GetMyName());

		if (pszUserName)
			strcpy(szWhiteName, pszUserName);
		else
			strcpy(szWhiteName, "?????");
	}
	else
	{
		strcpy(szWhiteName, m_mgr->GetMyName());

		if (pszUserName)
			strcpy(szBlackName, pszUserName);
		else
			strcpy(szBlackName, "?????");
	}

	int score = (header->result>>19)&0xFF;
	int n = (header->result>>15)&0x0F;
	Bool bIsHalfScore = (header->result&(1<<28))!=0; //HALF_SCORE(header->condition);

	int winer = (header->result>>29)&0x03;
	
	if (winer==1)
		pwszWiner=L"黒";
	else if (winer==2)
		pwszWiner=L"白";
	else if (winer==3)
		pwszWiner=L"ジ";
	else
		pwszWiner=L"--";

	if (n==0)
	{
		if (bIsHalfScore==True)
		{
			wcscpy(szTeai, L"互先");
		}
		else
		{
			wcscpy(szTeai, L"定先");
		}
	}
	else
	{
		wsprintfW(szTeai, L"%d子", n);
	}


	if (winer==2 || winer==1)
	{
		if (score==0xFF)
		{
			wsprintfW(szResult, L"中");
		}
		else if (score==0xFE)
		{
			wsprintfW(szResult, L"時");
		}
		else if (bIsHalfScore==True)
		{
			wsprintfW(szResult, L"%d.5", score);
		}
		else
		{
			wsprintfW(szResult, L"%d", score);
		}
	}
	wsprintfW(szDay, L"%d/%02d/%02d", DAY_YEAR(header->day), DAY_MON(header->day), DAY_DAY(header->day));

	cglsListView* listView = (cglsListView*)GetChild(xID+8);
	listView->InsertItem(index+1, 0,
		 Color::Black,
		0, (wchar_t*)szBlackName,
			0, szWhiteName,
				0, szTeai, 
					0, (header->result>>6)&0x1FF,
						0, pwszWiner,
							0, szResult,
								0, szDay);

	if (bIsUpdate)
		listView->Update(eGUM_FORCED);

	listView->InvalidateRect(0);
}

// ------------------------------------
void cglsMyView::UpdateGo(int index, int i, sSimpleYgfHeader* header, Bool bIsUpdate)
{
	wchar_t szDay[32];
	wchar_t* pwszWiner;
	wchar_t szResult[8]={0,};

	cyiNode* node = m_listGo[1]->GetNodeTailFrom(i);
	yifree(node->ref);

	node->ref=yitoheap(header, sizeof(*header));
	//m_listGo[index]->lock();
	//m_listGo[1]->insertref(0, yitoheap(header, sizeof(sSimpleYgfHeader)));

	//for (cyiNode* node = m_listGo[1]->GetNodeTopFrom(); node; node=node->np)
	//{
	//	sSimpleYgfHeader* header = (sSimpleYgfHeader*)node->ref;

	//}

	int score = GET_SCORE(header->flag);
	Bool bIsHalfScore = HALF_SCORE(header->condition);

	int winer = GET_WINER(header->flag);
	int n = GET_INIT_STONE(header->condition);
	wchar_t szTeai[5];

	if (winer==2)
		pwszWiner=L"黒";
	else if (winer==1)
		pwszWiner=L"白";
	else if (winer==3)
		pwszWiner=L"ジ";
	else
		pwszWiner=L"--";


	if (n==0)
	{
		if (bIsHalfScore==True)
		{
			wcscpy(szTeai, L"互先");
		}
		else
		{
			wcscpy(szTeai, L"定先");
		}
	}
	else
	{
		wsprintfW(szTeai, L"%d子", n);
	}

	if (winer==2 ||
			winer==1)
	{
		if (score==0xFF)
		{
			wsprintfW(szResult, L"中");
		}
		else if (bIsHalfScore==True)
		{
			wsprintfW(szResult, L"%d.5", score);
		}
		else
		{
			wsprintfW(szResult, L"%d", score);
		}
	}

	wsprintfW(szDay, L"%d/%02d/%02d", DAY_YEAR(header->day), DAY_MON(header->day), DAY_DAY(header->day));

	cglsListView* listView = (cglsListView*)GetChild(xID+9);
	listView->UpdateItem(header->id, 0,
		Color::Black, 0, (wchar_t*)header->szBlackName,
			0, header->szWhiteName,
				0, szTeai,
					0, header->last_order,
						0, pwszWiner,
							0, szResult,
								0, szDay);
	listView->InvalidateRect(0);
}


// ------------------------------------
void cglsMyView::AddGo(int index, sSimpleYgfHeader* header, Bool bIsUpdate)
{
	wchar_t szDay[32];
	wchar_t* pwszWiner;
	wchar_t szResult[8]={0,};

	//m_listGo[index]->lock();
	m_listGo[1]->insertref(0, yitoheap(header, sizeof(sSimpleYgfHeader)));

	int score = GET_SCORE(header->flag);
	Bool bIsHalfScore = HALF_SCORE(header->condition);

	int winer = GET_WINER(header->flag);
	int n = GET_INIT_STONE(header->condition);
	wchar_t szTeai[5];

	if (winer==2)
		pwszWiner=L"黒";
	else if (winer==1)
		pwszWiner=L"白";
	else if (winer==3)
		pwszWiner=L"ジ";
	else
		pwszWiner=L"--";


	if (n==0)
	{
		if (bIsHalfScore==True)
		{
			wcscpy(szTeai, L"互先");
		}
		else
		{
			wcscpy(szTeai, L"定先");
		}
	}
	else
	{
		wsprintfW(szTeai, L"%d子", n);
	}

	if (winer==2 ||
			winer==1)
	{
		if (score==0xFF)
		{
			wsprintfW(szResult, L"中");
		}
		else if (bIsHalfScore==True)
		{
			wsprintfW(szResult, L"%d.5", score);
		}
		else
		{
			wsprintfW(szResult, L"%d", score);
		}
	}

	wsprintfW(szDay, L"%d/%02d/%02d", DAY_YEAR(header->day), DAY_MON(header->day), DAY_DAY(header->day));

	cglsListView* listView = (cglsListView*)GetChild(xID+9);//+index);
	listView->InsertItem(header->id, 0,
		Color::Black, 0, (wchar_t*)header->szBlackName,
			0, header->szWhiteName,
				0, szTeai,
					0, header->last_order,
						0, pwszWiner,
							0, szResult,
								0, szDay);
	m_listGo[1]->unlock();

	if (bIsUpdate==True)
	{
		listView->Update(eGUM_FORCED);
	}
	listView->InvalidateRect(0);
}


// ------------------------------------
int cglsMyView::SetFont(Font* font, Bool bIsReference)
{
	if (m_font)
		delete m_font;

	
	m_font=(Font*)cyiShape::Clone(font);//font->Clone();
	m_nFontHeight = cGlassInterface::CalculateFontHeight(m_font, False);//m_font->GetHeight(__DEFAULTGRAPHICS__);
	m_nFontHeightGDI = cGlassInterface::CalculateFontHeight(m_font, True);

	Font* fontSmall = (Font*)cyiShape::Clone(GetFont(), 0.9);
	int nSmallFontHeight=cGlassInterface::CalculateFontHeight(fontSmall, True);;
	
	cglsTabCtrl* tab = (cglsTabCtrl*)GetChild(xID+7);

	//GetChild(xID+7)->SetFont(fontSmall);//ReSize();
	GetChild(xID+8)->SetFont(fontSmall);//ReSize();
	GetChild(xID+9)->SetFont(fontSmall);
	tab->ReSize(GetChild(xID+8)->m_w, fontSmall);
	delete fontSmall;

	ReArrangeItemz();

	return 0;
}

#define xIDSEARCH (xID+32)

// ------------------------------------
LRESULT cglsMyView::OnCreate(WPARAM wParam, LPARAM lParam)
{
	if (cglsDialog::CreateChildFromResource(_gRESOURCE)==False)
		return False;

	Font* fontSmall = (Font*)cyiShape::Clone(GetFont(), 0.9);
	int nSmallFontHeight;
	
	m_nFontHeight	 = fontSmall->GetHeight(__DEFAULTGRAPHICS__);
	unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, L"1", m_font, 0, 0);
	nSmallFontHeight = (pos&0xFFFF);

	// タブ
	cglsTabCtrl* tab = new cglsTabCtrl();
	{
		sGlsTabDetail detail;
		int tabWidth=nSmallFontHeight*10;//GetFontHeight(True)*10;
		cglsTabCtrl::xDefaultDetail(&detail);
		detail.font=fontSmall;

		//detail.stack.bIsVertical=True;
		detail.wMaxShowCount=1;
		detail.wMinShowCount=1;

		tab->Create((HWND)this,xID+7, 0, 0, tabWidth, &detail);
		//tab->AddItem(1, 0, L"友人", False);
		tab->AddItem(1, 0, L"対戦棋譜", False);
		tab->AddItem(2, 0, L"その他", False);

		tab->Show();
		tab->ReSize(tabWidth);
		tab->Update(eGUM_FORCED);

		if (m_eStyle==eMVS_NORMAL)
		{
			tab->SetCurSel(0);
		}

		GetChild(xID+6)->AddPositionLink(tab, eGP_BOTTOMSIDE|eGP_LEFT, 0, GetFontHeight(True)/2);
	}



	sGlsEditBoxDetail editDetail;
	cglsEditBox::xDefaultDetail(&editDetail);
	editDetail.gls.font=fontSmall;
	cglsEditBox* edit = new cglsEditBox();
	edit->Create((HWND)this, xIDSEARCH+1, 0, 0, GetFontHeight(True)*10, GetFontHeight(True), &editDetail);
	tab->AddPositionLink(edit, eGP_BOTTOMSIDE|eGP_LEFT, 0, 2);

	sGlsSplitButtonDetail splitDetail;
	cglsSplitButton::xDefaultDetail(&splitDetail);

	splitDetail.gls.font=fontSmall;

	cglsSplitButton* s1 = new cglsSplitButton();
	s1->Create((HWND)this, xIDSEARCH+2, 0, 0, GetFontHeight(True)*3, GetFontHeight(True), 0, &splitDetail);
	s1->AddItem(1, 0, L"手合", L"全て表示", True);
	s1->AddItem(2, 0, L"互先", L"互い先", False);
	s1->AddItem(3, 0, L"定先", L"定先", False);
	
	edit->AddPositionLink(s1, eGP_RIGHTSIDE|eGP_BOTTOM);
	for (int i=2; i<10; i++)
	{
		wchar_t xx[5];
		wchar_t xxx[5];
		
		//wsprintfW(xx, L"%d", i);
		wsprintfW(xxx, L"%d子", i);
		
		s1->AddItem(2+i, 0, xxx, xxx, False);
	}

	cglsSplitButton* s2 = new cglsSplitButton();
	s2->Create((HWND)this, xIDSEARCH+3, 0, 0, GetFontHeight(True)*4, GetFontHeight(True), 0, &splitDetail);
	s2->AddItem(1, 0, L"手数", L"全て表示", True);
	s2->AddItem(2, 0, L"30↓", L"30手未満のみ表示", False);
	s2->AddItem(3, 0, L"150↓", L"30手以上150手未満のみ表示", False);
	s2->AddItem(4, 0, L"250↓", L"150手以上250手未満のみ表示", False);
	s2->AddItem(5, 0, L"250↑", L"250手以上のみ表示", False);
	s2->AddItem(6, 0, L"300↓", L"300手以上のみ表示", False);

	s1->AddPositionLink(s2, eGP_RIGHTSIDE|eGP_TOP);

	cglsSplitButton* s3 = new cglsSplitButton();
	s3->Create((HWND)this, xIDSEARCH+4, 0, 0, GetFontHeight(True)*3, GetFontHeight(True), 0, &splitDetail);
	s3->AddItem(1, 0, L"結果", L"全て表示", True);
	s3->AddItem(2, 0, L"勝ち", L"自分の勝ち碁すべて", False);
	s3->AddItem(3, 0, L"負け", L"自分の負け碁すべて", False);
	s3->AddItem(4, 0, L"ジゴ", L"ジゴの全て", False);
	s2->AddPositionLink(s3, eGP_RIGHTSIDE|eGP_BOTTOM);

	cglsSplitButton* s4 = new cglsSplitButton();
	s4->Create((HWND)this, xIDSEARCH+5, 0, 0, GetFontHeight(True)*3, GetFontHeight(True), 0, &splitDetail);
	s4->AddItem(1, 0, L"詳細", L"全て表示", True);
	s4->AddItem(2, 0, L"中押", L"中押し", False);
	s4->AddItem(3, 0, L"時間", L"時間切れ", False);
	s4->AddItem(4, 0, L"1目", L"1目以内", False);
	s4->AddItem(5, 0, L"3目", L"3目以内", False);
	s4->AddItem(6, 0, L"10目", L"10目以内", False);
	s4->AddItem(7, 0, L"30目", L"30目以上", False);
	s3->AddPositionLink(s4, eGP_RIGHTSIDE|eGP_BOTTOM);

	// 棋譜
	{
		cglsListView* listView = new cglsListView();
		listView->Create(this, xID+8, 0, 0, 10, nSmallFontHeight*10, fontSmall);
		listView->CreateColumn(7, &sListViewColumn(12, L"黒", eGIA_LEFT, eGCT_STRING), 
										&sListViewColumn(12, L"白", eGIA_LEFT, eGCT_STRING), 
											&sListViewColumn(5, L"手合", eGIA_LEFT),
												&sListViewColumn(5, L"手数", eGIA_LEFT, eGCT_NUMERIC), 
													&sListViewColumn(3, L"勝"),
														&sListViewColumn(6, L""),
															&sListViewColumn(11, L"日時"));

		listView->Update(eGUM_FORCED);
		listView->Hide();

		//tab->AddPositionLink(listView, eGP_BOTTOMSIDE|eGP_LEFT,  0, 2);
		edit->AddPositionLink(listView, eGP_BOTTOMSIDE|eGP_LEFT, 0, 2);

		listView = new cglsListView();
		listView->Create(this, xID+9, 0, 0, 10, GetFontHeight(True)*10, fontSmall);

		listView->CreateColumn(7, &sListViewColumn(12, L"黒", eGIA_LEFT, eGCT_STRING), 
										&sListViewColumn(12, L"白", eGIA_LEFT, eGCT_STRING), 
											&sListViewColumn(5, L"手合", eGIA_LEFT),
												&sListViewColumn(5, L"手数", eGIA_LEFT, eGCT_NUMERIC), 
													&sListViewColumn(3, L"勝"),
														&sListViewColumn(6, L""),
															&sListViewColumn(11, L"日時"));

		listView->Update(eGUM_FORCED);
		edit->AddPositionLink(listView, eGP_BOTTOMSIDE|eGP_LEFT, 0, 2);
		//tab->AddPositionLink(listView, eGP_BOTTOMSIDE|eGP_LEFT, 0, 2);
		listView->Hide();


		delete fontSmall;
	}
	cglsButton* btn = new cglsButton();
	btn->Create((HWND)this, xID+21, 0, 0, 0, L"対局申し込み");
	btn->Update(eGUM_FORCED);
	btn->Enable(False);
	btn->Hide();



	UpdateMyView(m_mgr->GetMyId()!=0?eMVS_NORMAL:eMVS_NOID);
	Bool res = ReArrangeItemz();
	
	m_bIsQuitThread[0]=False;
	m_bIsQuitThread[1]=False;

	StartThread(-1, 0);
	StartThread(-1, 1);
	return res;
}

#include "glsPopupEdit.h"
#include "glsMessageBox.h"
#include "yiGoCommandz.h"
// ------------------------------------
LRESULT cglsMyView::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	int id = (wParam&0xFFFF);

	switch(id)
	{
		case (xID+8):
		{
			if (lParam==1)
			{
				unsigned int id = wParam>>16;
				cyiList* list = m_listGo[0];

				int size;
				sSimpleGoHeader* hd = m_mgr->GetGoHeader(0, &size);
				
				if (hd==0)
					return 0;
				
				sSimpleGoHeader*header = &hd[id-1];
				//if (node)
				{
					char szFilePath[256];
					unsigned int myId=m_mgr->GetMyId();
					//sSimpleGoHeader* header = (sSimpleGoHeader*)node->ref;

					Bool bIsImBlack=((header->result&0x80000000)!=0);

					wsprintf(szFilePath, "%08x%08x%08x%08x.sgf", bIsImBlack?myId:header->remoteId, bIsImBlack?header->remoteId:myId,
																			header->day, header->time);
					GetSystem()->GetCommandCenter()->OnCommand(eGCID_ADD_GOBAN, this, (IntPtr)szFilePath, (IntPtr)header);
				}
			}
			else
			{
				// プレビュー
				// 詳細情報表示
			}
			break;
		}
		case (xID+6):
		{
			wchar_t* pwszFileName = (wchar_t*)wParam;
			cglsImageBox* img = (cglsImageBox*)lParam;

			Bitmap* exbmp = img->ExportBitmap(200, 200);

			unsigned int seq;
			Bitmap* bmp = m_mgr->GetPicture(0, seq);
			m_mgr->SetPicture(0, exbmp, ++seq);
			bmp = m_mgr->GetPicture(0, seq);
			img->SetImage(bmp, eGIBVS_TRIM_BORDER_FIXED);


			//if (bmp)
			//{
			//	cyiShape::SaveBitmap(L"c:\\yotsuya\\abcd.jpg", bmp, L"image/jpeg");
			//}
			break;
		}
		case (xID+9):
		{
			if (lParam==1)
			{
				unsigned goId = wParam>>16;
				cglsTabCtrl* tab = (cglsTabCtrl*)GetChild(xID+7);
				cyiList* list = m_listGo[1];

				list->lock();
				for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
				{
					sSimpleYgfHeader* hd = (sSimpleYgfHeader*)node->ref;
					if (hd->id==goId)
					{
						GetSystem()->GetCommandCenter()->OnCommand(eGCID_ADD_GOBAN, this, (IntPtr)hd->szFileName, 0);
					}
				}
				list->unlock();
			}
			else
			{
				// プレビュー
				// 詳細情報表示
			}
			break;
		}
		case (xID+18):
		{
			if ((wParam>>16)==EN_CHANGE)
			{
				cglsEditBox* edit = (cglsEditBox*)GetChild(id);

				int n = edit->GetText((wchar_t*)0, 0);

				if (n!=0)
				{
					GetChild(xID+21)->Enable(True);
				}
				else
				{
					GetChild(xID+21)->Enable(False);
				}

			}
			break;
		}
		case (xID+21):
		{
			cglsEditBox* edit = (cglsEditBox*)GetChild(xID+18);
			char szNo[16];
			char szPassword[16];

			edit->GetText(szNo, 16);
			edit = (cglsEditBox*)GetChild(xID+20);
			edit->GetText(szPassword, 16);
			int  n = atoi(szNo);

			GetSystem()->GetCommandCenter()->OnCommand(eGCID_REQ_PLAY_DIALOG, this, n, (IntPtr)szPassword);

			break;
		}
		case (xIDSEARCH+1):
		case (xIDSEARCH+2):
		case (xIDSEARCH+3):
		case (xIDSEARCH+4):
		case (xIDSEARCH+5):
		{
			cglsTabCtrl* tab = (cglsTabCtrl*)GetChild(xID+7);
			char szText[16]={0, };
			int tabIndex = tab->GetCurSel()-1;

			cglsEditBox* edit = (cglsEditBox*)GetChild(xIDSEARCH+1);

			edit->GetText(m_filter[tabIndex].szUserName, 16);

			for (int i=0; i<4; i++)
			{
				cglsSplitButton* btn = (cglsSplitButton*)GetChild(i+xIDSEARCH+2);
				m_filter[tabIndex].filter[i]=btn->GetSelectIndex();
			}
			

			m_bIsQuitThread[tabIndex]=True;
			if (WaitThread(tabIndex+1, 1000)==False)
				break;

			m_bIsQuitThread[tabIndex]=False;
			cglsListView* listView=(cglsListView*)GetChild(xID+8+tabIndex);
			listView->Clear();
			StartThread(tabIndex+1, tabIndex);
			//OnAdjustFilter(szText, filter);
			//this->UpdateMyView(
			break;
		}
		//case (xID+11):
		//{
		//	cglsSplitButton* btn = (cglsSplitButton*)GetChild(id);
		//	int n = btn->GetSelectIndex();

		//	if (n!=0)
		//		GetChild(xID+13)->Hide();
		//	else
		//		GetChild(xID+13)->Show();
		//	break;
		//}
		//case (xID+13):
		//{
		//	cGlassInterface* child=GetChild(xID+13);
		//	POINT pt={0, child->m_h};
		//	cglsPopupEdit edit(ePEM_NORMAL, 8, GetFont());
		//	ClientToScreen(child->m_hWnd, &pt);
		//	if (edit.TrackPopup(*this, pt.x, pt.y, 8, GetFont())==True)
		//	{
		//		char szFriendName[17]={0, };
		//		edit.GetText(szFriendName, 16);
		//		if (GetSystem()->GetCommandCenter()->OnCommand(eGCID_ADD_FRIEND, this, (unsigned int)yistrheap(szFriendName), 0)==False)
		//		{
		//			cglsMessageBox msg;
		//			msg.Show(this, L"失敗しました!\r\nしばらくしてからもう一度行ってください", L"", MB_OK, GetFont()); 
		//		}
		//	}
		//	break;
		//}
		default:
			break;
	}

	return 0;
}

// ------------------------------------
LRESULT cglsMyView::OnNotify(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	NMHDR* hdr=(NMHDR*)lParam;

	// タブ
	if (wParam==(xID+7))
	{
		if (hdr->idFrom==1)
		{
			GetChild(xID+8)->Show();
			GetChild(xID+9)->Hide();

			cglsEditBox* edit = (cglsEditBox*)GetChild(xIDSEARCH+1);
			edit->SetText(m_filter[0].szUserName);

			for (int i=0; i<4;i++)
			{
				cglsSplitButton* btn = (cglsSplitButton*)GetChild(xIDSEARCH+2+i);
				btn->SelectIndex(m_filter[0].filter[i]);
				btn->Update(eGUM_FORCED);
			}
			//GetChild(xID+10)->Hide();
		}
		else if (hdr->idFrom==2)
		{
			GetChild(xID+9)->Show();
			GetChild(xID+8)->Hide();
			cglsEditBox* edit = (cglsEditBox*)GetChild(xIDSEARCH+1);
			edit->SetText(m_filter[1].szUserName);

			for (int i=0; i<4;i++)
			{
				cglsSplitButton* btn = (cglsSplitButton*)GetChild(xIDSEARCH+2+i);
				btn->SelectIndex(m_filter[1].filter[i]);
				btn->Update(eGUM_FORCED);
			}
			//GetChild(xID+10)->Hide();
		}
		//else if (hdr->idFrom==1)
		//{
		//	GetChild(xID+9)->Hide();
		//	GetChild(xID+8)->Hide();
		//	GetChild(xID+10)->Show();
		//}
	}
	return 0;
}



// ------------------------------------
void cglsMyView::OnItemReArranged()
{
	int xOffset, yOffset;
	int w, w2;
	int maxWidth=0;
	int maxHeight=0;
	Font* fontLarge = (Font*)cyiShape::Clone(GetFont(), 2.0);
	cGlassInterface* child;

	GetContentsOffset(xOffset, yOffset);
	
	if (m_eStyle==eMVS_NOID)
		goto NOID;


	cglsStatic* label = (cglsStatic*)GetChild(xID+1);
	//label->SetText(0, info->szMyId);
	label->SetFont(fontLarge);

	delete fontLarge;

	cGlassInterface::Update(eGUM_FORCED);


	GetChild(xID+6)->Move(xOffset, yOffset);
	w=GetChild(xID+1)->m_w+GetChild(xID+1)->m_x;

	cGlassInterface* xx=GetChild(xID+5);

	int picWidth = (xx->m_y+xx->m_h)-GetChild(xID+6)->m_y;
	GetChild(xID+6)->ReSize(picWidth, picWidth);//GetChild(xID+6)->m_w ,(xx->m_y+xx->m_h)-GetChild(xID+6)->m_y);

	unsigned int seq;
	Bitmap* bmp = m_mgr->GetPicture(0, seq);

	if (bmp)
	{
		cglsImageBox* img = (cglsImageBox* )GetChild(xID+6);
		img->SetImage(bmp, eGIBVS_TRIM_BORDER_FIXED);
	}

	cGlassInterface* tab=GetChild(xID+7);
	tab->ReSize(GetChild(xID+8)->m_w, tab->m_h);

	for (int i=xID+1; i<xID+29; i++)
	{
		cGlassInterface* obj = GetChild(i);
		if (obj==0)
			continue;

		maxWidth = (obj->m_w+obj->m_x)>maxWidth?(obj->m_w+obj->m_x):maxWidth;
		maxHeight = (obj->m_h+obj->m_y)>maxHeight?(obj->m_h+obj->m_y):maxHeight;
	}

	SetContentsSize(maxWidth-xOffset, maxHeight-yOffset);
	Move((m_parent->m_w-m_w)>>1, (m_parent->m_h-m_h)>>1);
	InvalidateRect(0, 0, True);

	return ;
NOID:
	label = (cglsStatic*)GetChild(xID+16);
	label->SetFont(fontLarge);

	cGlassInterface::Update(eGUM_FORCED);

	delete fontLarge;
	child=GetChild(xID+9);

	maxWidth = label->m_w > child->m_w ? label->m_w:child->m_w;
	label->Move((maxWidth+(xOffset<<1)-label->m_w)>>1, yOffset);

	child->Move(xOffset, label->m_y+label->m_h+GetFontHeight(True));

	GetChild(xID+17)->Move(xOffset, child->m_y+child->m_h+GetFontHeight(True));
	maxHeight=GetChild(xID+20)->m_y+GetChild(xID+18)->m_h;

	GetChild(xID+21)->Move(maxWidth+xOffset-GetChild(xID+21)->m_w, maxHeight-GetChild(xID+21)->m_h);
	SetContentsSize(maxWidth, maxHeight-yOffset);
	Move((m_parent->m_w-m_w)>>1, (m_parent->m_h-m_h)>>1);
	InvalidateRect(0, 0, True);

	return ;
}

// ------------------------------------
LRESULT cglsMyView::OnAddGo(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	int id		= (wParam>>16);
	int index	= wParam&0xFFFF;

	if (id==xID+8)
	{
		if (lParam==0xFFFFFFFF)
		{
			GetChild(xID+8)->Update(eGUM_FORCED);
		}
		else
		{
			AddGo(index, (sSimpleGoHeader*)lParam, False);
		}
	}
	else
	{
		if (lParam==0xFFFFFFFF)
		{
			GetChild(xID+9)->Update(eGUM_FORCED);
		}
		else
		{
			AddGo(index, (sSimpleYgfHeader*)lParam, False);
		}
	}
	return 0;
}

// ------------------------------------
void cglsMyView::OnLoadMyGoData()
{
	int size;
	cglsListView* listView = (cglsListView*)GetChild(xID+8);
	sSimpleGoHeader* header = m_mgr->GetGoHeader(0, &size);
	int n = size/sizeof(sSimpleGoHeader);
	int nTextLen = strlen(m_filter[0].szUserName);
	
	for (int i=0; i<n; i++)
	{
		if (m_bIsQuitThread[0]==True)
		{
			return ;
		}

		if (header[i].remoteId==0)
			continue;

		if (m_filter[0].szUserName[0]!=0)
		{
			const char* pszUserName = m_mgr->GetUserName(header[i].remoteId);

			if (pszUserName==0)
				continue;

			if (strncmp(pszUserName, m_filter[0].szUserName, nTextLen)!=0)
				continue;
		}

		if (m_filter[0].filter[0]!=0)
		{
			int nInitStone = (header[i].result>>15)&0x0F;
			Bool bIsHalfScore = (header[i].result&(1<<28))!=0; //HALF_SCORE(header->condition);

			if ((nInitStone==0 && bIsHalfScore==True) && (m_filter[0].filter[0]!=1))
			{
				continue;
			}
			else if ((nInitStone==0 && bIsHalfScore==False) && m_filter[0].filter[0]!=2)
			{
				continue;
			}
			else if (nInitStone!=0)
			{
				if (nInitStone!=(m_filter[0].filter[0]-1))
				{
					continue;
				}
			}
		}

		if (m_filter[0].filter[1]!=0)
		{
			int nLastOrder = (header[i].result>>6)&0x1FF;

			if (m_filter[0].filter[1]==1)
			{
				if (nLastOrder>30)
				{
					continue;
				}
			}
			else if (m_filter[0].filter[1]==2)
			{
				if (nLastOrder<30 ||
						nLastOrder>150)
				{
					continue;
				}
			}
			else if (m_filter[0].filter[1]==3)
			{
				if (nLastOrder<150 ||
						nLastOrder>250)
				{
					continue;
				}
			}
			else if (m_filter[0].filter[1]==4)
			{
				if (nLastOrder<250)
				{
					continue;
				}
			}
			else if (m_filter[0].filter[1]==5)
			{
				if (nLastOrder<300)
				{
					continue;
				}
			}
		}


		if (m_filter[0].filter[2]!=0)
		{
			Bool IamBlack = (header[i].result&0x80000000)!=0;
			int winer = (header[i].result>>29)&0x03;

			if (m_filter[0].filter[2]==1)
			{
				if (IamBlack==True &&
						winer==1)
				{
				}
				else if (IamBlack==False &&
							winer==2)
				{
				}
				else
					continue;
			}
			else if (m_filter[0].filter[2]==2)
			{
				if (IamBlack==True &&
						winer==1)
				{
					continue;
				}
				else if (IamBlack==False &&
							winer==2)
				{
					continue;
				}
				else if (winer==3)
				{
					continue;
				}
			}
			else if (m_filter[0].filter[2]==3)
			{
				if (winer!=3)
					continue;
			}
		}

		if (m_filter[0].filter[3]!=0)
		{
			int score = (header[i].result>>19)&0xFF;

			if (m_filter[0].filter[3]==1)
			{
				if (score!=0xFF)
					continue;
			}
			else if (m_filter[0].filter[3]==2)
			{
				if (score!=0xFE)
					continue;
			}
			else if (m_filter[0].filter[3]==3)
			{
				if (score!=0)
					continue;
			}
			else if (m_filter[0].filter[3]==4)
			{
				if (score>4)
					continue;
			}
			else if (m_filter[0].filter[3]==5)
			{
				if (score>10)
					continue;
			}
			else if (m_filter[0].filter[3]==6)
			{
				if (score<30)
					continue;
				if (score==0xFF ||
						score==0xFE)
					continue;
			}

		}

		::PostMessage(*this, WM_ADD_GO, ((xID+8)<<16)|i, (LPARAM)&header[i]);
		//AddGo(&header[i]);
	}
	
	PostMessage(*this, WM_ADD_GO, ((xID+8)<<16)|0, 0xFFFFFFFF);
}

// ------------------------------------
void cglsMyView::OnThreadStart(int id, int param1, int param2)
{
	int size;
	int n;

	if (param1==0)
	{
		OnLoadMyGoData();
		return ;
	}

	if (m_ygf_header)
		yiFileUnLoadEx(m_ygf_header);

	m_ygf_header = (sSimpleYgfHeader* )yiFileLoadEx(m_mgr->GetGoFilePath(param1), &size);
	n= size/sizeof(sSimpleYgfHeader);

	sSimpleYgfHeader* header=m_ygf_header;
	if (header==0)
		return ;

	cglsListView* listView = (cglsListView*)GetChild(xID+8+param1);

	const char* pszMyName=m_mgr->GetMyName();
	int nTextLen = 0;

	if (m_filter[1].szUserName[0]!=0)
		pszMyName=m_filter[1].szUserName;

	nTextLen=strlen(pszMyName);

	//m_listGo[1]->lock();
	for (int i=0; i<n; i++)
	{
		
		if (m_filter[1].szUserName[0]!=0)
		{
			if (header[i].szBlackName[0]==0 &&
					header[i].szWhiteName[0]==0)
			{
				continue;
			}

			if (strncmp(header[i].szBlackName, m_filter[1].szUserName, nTextLen)!=0)
			{
				if (strncmp(header[i].szWhiteName, m_filter[1].szUserName, nTextLen)!=0)
				{
					continue;
				}
			}

			//pszMyName=m_filter[1].szUserName;
		}

		if (m_filter[1].filter[0]!=0)
		{
			int nInitStone = GET_INIT_STONE(header[i].condition);//(header[i].result>>15)&0x0F;
			Bool bIsHalfScore = HALF_SCORE(header[i].condition);

			if ((nInitStone==0 && bIsHalfScore==True) && (m_filter[1].filter[0]!=1))
			{
				continue;
			}
			else if ((nInitStone==0 && bIsHalfScore==False) && m_filter[1].filter[0]!=2)
			{
				continue;
			}
			else if (nInitStone!=0)
			{
				if (nInitStone!=(m_filter[1].filter[0]-1))
				{
					continue;
				}
			}
		}

		if (m_filter[1].filter[1]!=0)
		{
			int nLastOrder = header[i].last_order;//.result>>6)&0x1FF;

			if (m_filter[1].filter[1]==1)
			{
				if (nLastOrder>30)
				{
					continue;
				}
			}
			else if (m_filter[1].filter[1]==2)
			{
				if (nLastOrder<30 ||
						nLastOrder>150)
				{
					continue;
				}
			}
			else if (m_filter[1].filter[1]==3)
			{
				if (nLastOrder<150 ||
						nLastOrder>250)
				{
					continue;
				}
			}
			else if (m_filter[1].filter[1]==4)
			{
				if (nLastOrder<250)
				{
					continue;
				}
			}
			else if (m_filter[1].filter[1]==5)
			{
				if (nLastOrder<300)
				{
					continue;
				}
			}
		}


		if (m_filter[1].filter[2]!=0)
		{
			int winer = GET_WINER(header[i].flag);//(header[i].result>>29)&0x03;
			int len = strlen(pszMyName);
			Bool IamBlack=False;

			if (strncmp(header[i].szBlackName, pszMyName, len)==0)
			{
				IamBlack=True;
			}
			else if (strncmp(header[i].szWhiteName, pszMyName, len)==0)
			{
				IamBlack=False;
			}
			else
			{
				continue;
			}

			if (m_filter[1].filter[2]==1)
			{
				if (IamBlack==True &&
						winer==1)
				{
				}
				else if (IamBlack==False &&
							winer==2)
				{
				}
				else
					continue;
			}
			else if (m_filter[1].filter[2]==2)
			{
				if (IamBlack==True &&
						winer==1)
				{
					continue;
				}
				else if (IamBlack==False &&
							winer==2)
				{
					continue;
				}
				else if (winer==3)
				{
					continue;
				}
			}
			else if (m_filter[1].filter[2]==3)
			{
				if (winer!=3)
					continue;
			}
		}

		if (m_filter[1].filter[3]!=0)
		{
			int score = GET_SCORE(header[i].flag);//.result>>19)&0xFF;

			if (m_filter[1].filter[3]==1)
			{
				if (score!=0xFF)
					continue;
			}
			else if (m_filter[1].filter[3]==2)
			{
				if (score!=0xFE)
					continue;
			}
			else if (m_filter[1].filter[3]==3)
			{
				if (score!=0)
					continue;
			}
			else if (m_filter[1].filter[3]==4)
			{
				if (score>4)
					continue;
			}
			else if (m_filter[1].filter[3]==5)
			{
				if (score>10)
					continue;
			}
			else if (m_filter[1].filter[3]==6)
			{
				if (score<30)
					continue;
				if (score==0xFF ||
						score==0xFE)
					continue;
			}

		}

		//m_listGo[1]->addref(yitoheap(&header[i], sizeof(sSimpleYgfHeader)));
		::PostMessage(*this, WM_ADD_GO, ((xID+9)<<16)|i, (LPARAM)&header[i]);
		//AddGo(1, &header[i]);
	}
	PostMessage(*this, WM_ADD_GO, ((xID+9)<<16)|0, 0xFFFFFFFF);

	//m_listGo[1]->unlock();

	//yiFileUnLoadEx(header);
	
}
