#include "MainWindow.h"
#include "resource.h"
#include "GlassControlz.h"

extern INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

MainWindow::MainWindow(void)
{
	m_TextureImg=0;
}

MainWindow::~MainWindow(void)
{
}



#include "glsButton.h"
#include "glsStackPannel.h"
#include "glsResourceBitmap.h"
#include "ButtonSet.h"
#include "glsTabCtrl.h"
#include "glsPopupMenu.h"
#include "glsEditBox.h"
#include "glsComboBox.h"
#include "glsSplitButton.h"
#include "glsStatic.h"
#include "glsMonthCalender.h"
#include "glsToolButton.h"
#include "glsSpinBox.h"
#include "glsScrollBar.h"
#include "glsRadioGroup.h"
#include "glsCheckGroup.h"
#include "glsImageBox.h"

cglsStackPannel* panelH;
cglsStackPannel* panelV;
cglsStackPannel* panelX;

cglsPopupMenu* g_menu=0;
cglsButton btn;
cglsButton btn2;
cglsButton btn3;
cglsButton btn4;
cButtonSet btnSet(1);
cButtonSet btnSet2(2);
cButtonSet btnSet3(3);
cglsEditBox* edit;

#include <iostream>
using namespace std;


//cglsTabItem tabitem;
cglsTabCtrl tabctrl;
#include "yisock.h"
#include "glsDialog.h"
#include "glsListView.h"
#include "glsGoban.h"

#include "glsPageScroll.h"

// ---------------------------------------------------
//LRESULT MainWindow::OnCreate(WPARAM wParam, LPARAM lParam)
//{
//	RECT rect;
//	::GetClientRect(*this, &rect);
//
//	m_TextureImg = BITMAP32(32, 32);//new Image(L".\\data\\131.jpg");
//	cyiShape::FillRadialGradiation(m_TextureImg, 25, 25, 48.0, Color::Orange, Color::Yellow);
//
//	cglsGoban* goban = new cglsGoban();
//	
//	sGobanDetail detail;
//	cglsGoban::xDefaultDetail(&detail);
//	
//	if (goban->Create((HWND)this, 0, 0, rect.right, rect.bottom, 0, &detail)==False)
//	{
//		Destroy();
//		PostQuitMessage(0);
//		return 0;
//	}
//	goban->Show();
//	InvalidateRect(0);
//	return 0;
//}

#define __XXXX
#ifdef __XXXX
// ---------------------------------------------------
LRESULT MainWindow::OnCreate(WPARAM wParam, LPARAM lParam)
{
	POINT pt = {50, 100};
	RECT margine={10, 10, 10, 10};
	HWND hWnd=m_hWnd;

	//yiInitWinsock(2,2);
	//yiconnect(yihtol("www.yahoo.co.jp", 0), 43, 0);

	sGlsItemDetail itemdetail;

	cglsStackPannel::xDefaultItemDetail(&itemdetail);

	cglsWindow::OnCreate(wParam, lParam);

	//m_TextureImg = new Image(L".\\data\\logo.gif");

	btnSet.SetBgColor(__COLORp__(128,0,255,0), Color::Aqua);
	panelH = new cglsStackPannel();
	panelH->Add(1024, 0, 0, 0, 0, &itemdetail);
	panelH->Add(1025, 0, 0, 2,  0, &itemdetail);
	panelH->Add(1026, 0, 0, 2,  0, &itemdetail);

	//panelH->CreateDynamicSize((HWND)this, 1, &pt, &margine, &btnSet, False, True, eSA_LEFT);
	Font btnFont(L"Meiryo", 12.0);
	sGlsButtonDetail xbtnDetail;
	cglsButton::xDefaultDetail(&xbtnDetail);
	xbtnDetail.gls.font=&btnFont;
	xbtnDetail.gls.nDragOnTimer=1000;
	xbtnDetail.colorHilight=Color::Orange;
	xbtnDetail.edge=2.0;
	xbtnDetail.gls.bIsNotOwnDraw=True;

	sGlsStackPannelDetail detailx;
	
	cglsStackPannel::xDefaultDetail(&detailx);
	detailx.bIsVertical=False;
	detailx.bUseGaussPannel=True;
	detailx.gauss.colorTop=__COLORp__(127, 0, 0, 0);
	detailx.gauss.colorBottom=__COLORp__(200, 0, 0, 0);
	RECT xrect={pt.x, pt.y, 0, 0};
	panelH->Create((HWND)this, 1, &xrect, &margine, &btnSet, &detailx);
	panelH->Show();
	cglsButton* btnx = new cglsButton();
	btnx->Initialize(0, new Font(L"Meiryo", 11.0));

	btnx->Create((HWND)panelH, 1028, 0, 0, (Bitmap*)eGRID_NEXT, 0, 0, &xbtnDetail);
	btnx->Show();

	detailx.bUseGaussPannel=False;

	itemdetail.fEnterTransparency=1.0;
	itemdetail.fLeaveTransparency=0.5;
	panelH->AddFreeItem(222, 70, 100, 0, 0, 0, &itemdetail);


	panelH->Add(btnx, 2);


	pt.x=0;
	pt.y=0;
	btnSet2.SetBgColor(Color::White, Color::Red);
	panelV = new cglsStackPannel();
	detailx.bIsVertical=True;
	detailx.fEnterTransparency=1.0;
	detailx.fLeaveTransparency=0.5;
	xrect.left=pt.x;
	xrect.top=pt.y;
	panelV->Create((HWND)panelH, 2, &xrect, &margine, &btnSet2, &detailx);
	//panelV->Show();
	panelV->Add(1024, 0, 0, 0);

	for (int i=0; i<10; i++)
	{
		panelV->Add(1025+i, 0, 0, 2);
	}

	panelH->Insert(1, panelV, 2);

	panelH->Show();

	//ShowWindow(panelH->m_hWnd, SW_SHOW);
	panelH->ShowItem(True, 1027);
	//ShowWindow(panelV->m_hWnd, SW_SHOW);

	panelH->Update(eGUM_FORCED);

	btnSet3.SetBgColor(__COLORp__(64, 255,0,0)/*::Yellow*/, Color::Beige);
	RECT rect={0, 0, 0, 200};
	panelX = new cglsStackPannel();
	detailx.bIsVertical=True;
	detailx.eAlign=eSA_LEFT;
	detailx.bIsFixedMode=True;
	detailx.fEnterTransparency=-1.0;
	detailx.fLeaveTransparency=-1.0;
	detailx.bIsShowScrollBar=True;
	panelX->Create((HWND)panelH, 1030, &rect, &margine, &btnSet3, &detailx);
	panelX->Show();
	RECT xRect={0, 0, 60, 1000};
	//panelX->SetBufferSize(xRect);
	pt.x=0;
	pt.y=0;
	//panelX->CreateDynamicSize((HWND)panelH, 2, &pt, &margine, &btnSet3, True, True, eSA_LEFT);

	for (int i=0; i<15; i++)
	{
		cglsButton* btn = new cglsButton();
		btn->Initialize(0, new Font(L"Meiryo", 11.0));
		btn->Create((HWND)panelX, 1031+i, 0, 0, (Bitmap*)eGRID_NEXT, 0, 0, &xbtnDetail);
		btn->Update(eGUM_FORCED);
		//btn->Show();
		panelX->Add(btn, 1);
		panelX->Add(2048+i, 0, 0, 2);
	}
	panelX->Show();
	panelH->Add(panelX, 2);
	panelH->Update(eGUM_FORCED);

	panelH->Add(1031, 0, 0, 10);
	panelH->Update(eGUM_SKIP_REDRAW);
	

	btn.Initialize(0, new Font(L"Meiryo", 12.0));
	btn.Create((HWND)this, 1028, 500, 200, (Bitmap*)eGRID_NEXT, L"テスト");		
	btn.Show();
	btn.Update(eGUM_FORCED);

	btn2.Initialize(0, new Font(L"Meiryo", 12.0));
	btn2.Create((HWND)this, 1029, 500+btn.m_w+2, 200, (Bitmap*)eGRID_NEXT, 0);
	btn2.Show();
	btn2.Update(eGUM_FORCED);

	sGlsButtonDetail btnDetail;
	cglsButton::xDefaultDetail(&btnDetail);
	btnDetail.gls.font= new Font(L"Meiryo", 12.0);
	btnDetail.gls.nDragOnTimer=500;
	btnDetail.colorHilight=Color::LightPink;
	btnDetail.gls.fEnterTransparency=1.0;
	btnDetail.gls.fLeaveTransparency=0.3;

	btn3.Initialize(0, new Font(L"Meiryo", 12.0));
	btn3.Create((HWND)this, 1030, 500+btn.m_w+2+btn2.m_w, 200, (Bitmap*)0, L"壁紙変更", 0, &btnDetail);
	btn3.Show();
	btn3.Update(eGUM_FORCED);

	btnDetail.colorHilight=Color::LightBlue;
	btnDetail.gls.nShowAnimationInterval=300;
	btnDetail.edge=3.0;
	btn4.Initialize(0, new Font(L"Meiryo", 12.0));
	btn4.Create((HWND)this, 1031, 500+btn.m_w+2+btn2.m_w+2+btn3.m_w, 200, (Bitmap*)0, L"Selected", L"何か選んでください", &btnDetail);
	btn4.Show();
	btn4.Update(eGUM_FORCED);


	//tabitem.Create((HWND)this, 1, 0, L"適当な名前", eIA_CENTER, 0, True);
	sGlsTabDetail detail;

	cglsTabCtrl::xDefaultDetail(&detail);

	detail.stack.bIsVertical=True;
	detail.wMaxShowCount=3;
	detail.wMinShowCount=6;

	tabctrl.Create((HWND)this, 1, 0, 10, 500, &detail);
	tabctrl.Show();

	for (int i=0; i<5; i++)
	{
		wchar_t szCaption[32];
		wsprintfW(szCaption, L"テストアイテム%d", i);
		tabctrl.AddItem(10+i, 0, szCaption, True);
		tabctrl.SetCurSel(0, 10+i, False);
	}

	tabctrl.Update(eGUM_FORCED);


	//tab->Update(eGUM_FORCED);

	//tab->Destroy();
	//delete tab;
	//tabitem.Move(30, 30);	
	//tabctrl.Update(eGUM_FORCED);

	m_TextureImg = BITMAP32(32, 32);//new Image(L".\\data\\131.jpg");

	cyiShape::FillRadialGradiation(m_TextureImg, 25, 25, 48.0, Color::Orange, Color::Yellow);
	//Graphics gt(m_TextureImg);

	//gt.FillRectangle(&SolidBrush(Color::Beige), 0, 0, 32, 32);
	//gt.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	//gt.FillEllipse(&SolidBrush(Color::YellowGreen), Rect(8, 8, 16, 16));

	m_img = new Image(L"c:\\aaa.jpg");

	m_img2 = BITMAP32(300, 300);


	Graphics g(m_img2);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	//g.FillRectangle(&SolidBrush(Color::White), Rect(0, 0, 300, 300));
	g.DrawEllipse(&Pen(Color::Green), Rect(10, 10, 280, 280));
	g.FillEllipse(&SolidBrush(Color(255, 0, 255, 0)), Rect(20, 20, 260, 260));

	//cglsDialog* dlg=new cglsDialog();

	//dlg->Create((HWND)this, 777, 0, 200, 50, 500, 200, 0);

	//InvalidateRect(0, False);
	cglsPopupMenu* menu = new cglsPopupMenu();

	menu->Create(m_hWnd, 737);
	menu->AddItem(15, 0, L"選択１");
	menu->AddItem(16, 0, L"選択２");
	menu->AddItem(17, 0, L"選択３");
	menu->AddItem(18, 0, L"選択４");
	menu->AddItem(19, 0, L"選択６");
	menu->SetCheck(2, True, True);

	sGlsEditBoxDetail editDetail;

	cglsEditBox::xDefaultDetail(&editDetail);

	cglsMonthCalender* cal = new cglsMonthCalender();
	//editDetail.bIsStaticEditorMode=True;
	//editDetail.bIsFontSizeVariable=True;
	//editDetail.assistant= cal;//menu;
	//editDetail.gls.font=new Font(L"Meiryo", 11.0);
	
	edit = new cglsEditBox();
	edit->Create((HWND)this, 778, 500, 300, 200, 0, &editDetail);
	edit->SetImeMode(eGTIM_IME_ALWAYS_OFF);
	edit->SetMaxLength(5);
	edit->SetPasswordMode(True);
	//edit->SetText(L"まだ、なんか問題だらけ。");
	edit->Show();

	cglsComboBox* combo;
	sGlsComboBoxDetail comboDetail;

	cglsComboBox::xDefaultDetail(&comboDetail);
	
	comboDetail.edit.gls.font= new Font(L"Meriyo", 10.0);
	combo = new cglsComboBox();
	combo->Create((HWND)this, 788, 500, 400, 200, 0, &comboDetail);
	combo->Show();

	cglsSplitButton* sBtn = new cglsSplitButton();
	sBtn->Create((HWND)this, 780, 500, 350, GetFontHeight()*5, 0, 0);

	int xxxcnt=1;
	for (int i=0; i<5; i++)
	{
	sBtn->AddItem(xxxcnt++, 0, L"手順表示X", L"手順を表示しない", True);
	sBtn->AddItem(xxxcnt++, 0, L"手順表示", L"手順表示");
	sBtn->AddItem(xxxcnt++, 0, L"手順表示+", L"ここから手順表示");
	sBtn->AddItem(xxxcnt++, 0, L"Pico", L"PicoPico");
	}

	sBtn->Show();
	sBtn->Update(eGUM_FORCED);

	sGlsSplitButtonDetail splitDetail;
	cglsSplitButton::xDefaultDetail(&splitDetail);
	splitDetail.colorHilight=Color::LightBlue;
	cglsSplitButton* sBtn2 = new cglsSplitButton();

	splitDetail.eStyle=eSBS_RIGHT_FULLDOWN;
	sBtn2->Create((HWND)this, 781, 650, 350, GetFontHeight()*5, 0, L"右左別", &splitDetail);

	sBtn2->AddItem(1, 0, L"NHK方式", L"NHK方式: 制限時間なし、1手30秒、1分秒読み10回", True);
	sBtn2->AddItem(2, 0, L"棋聖戦方式", L"棋聖戦方式");
	sBtn2->AddItem(3, 0, L"富士通杯方式", L"富士通杯方式");
	sBtn2->AddItem(4, 0, L"Pico", L"PicoPico");

	sBtn2->Show();
	sBtn2->Update(eGUM_FORCED);

	sBtn->AddPositionLink(sBtn2, (eGlsPosition)(eGP_RIGHTSIDE|eGP_BOTTOM));


	cglsStatic* stat= new cglsStatic();
	sGlsStaticDetail statx;

	cglsStatic::xDefaultDetail(&statx);
	statx.bIsAutoFontSize=True;
	stat->Create((HWND)this, 400, 450, 120, 30, 0, L"私の名前は何でしょうか？", &statx);
	stat->Show();


	cglsToolButton* btn = new cglsToolButton();
	btn->Create(this, 737, 500, 100, 0, 0);
	btn->Add(eGTBT_ALEFT, eGTBSS_FILL);
	btn->Add(eGTBT_ARIGHT, eGTBSS_FILL);
	btn->Show();
	btn->Update(eGUM_FORCED);

	cglsToolButton* btn2 = new cglsToolButton();
	sGlsToolButtonDetail toolDetail;
	cglsToolButton::xDefaultDetail(&toolDetail);
	toolDetail.stack.bIsVertical=True;
	btn2->Create(this, 738, 600, 100, 0, 0, &toolDetail);
	btn2->Add(eGTBT_AUP, eGTBSS_FILL);
	btn2->Add(eGTBT_ADOWN, eGTBSS_FILL);
	btn2->Show();
	btn2->Update(eGUM_FORCED);

	cglsSpinBox* spin = new cglsSpinBox();
	spin->Create(this, 877, 600, 150, 3);
	//spin->SetNumer(0, 0, 9, 6);
	spin->AddString(0, L"0", 0, True);
	spin->AddString(0, L"5", 1, False);
	spin->SetNumer(1, 0, 9, 0);
	spin->SetNumer(2, 0, 1, 0);
	spin->SetLimited(1, 16);
	//spin->InsertLabel(L":", 1, Color::Black);
	sBtn2->AddPositionLink(spin, (eGlsPosition)(eGP_BOTTOMSIDE|eGP_RIGHT));

	//cglsSpinBox* spin2 = new cglsSpinBox();
	//spin2->Create(this, 875, 600, 150, 1);
	//spin2->AddString(0, L"AM", 0, True);
	//spin2->AddString(0, L"PM", 1);
	//spin->AddPositionLink(spin2, (eGlsPosition)(eGP_RIGHTSIDE|eGP_BOTTOM));

	sGlsSpinBoxDetail spinDetail;
	cglsSpinBox::xDefaultDetail(&spinDetail);
	//spinDetail.gls.nKeyLButtonDownTimer=0;
	//cglsSpinBox* spinHour = new cglsSpinBox();
	//spinHour->Create(this, 874, 600, 150, 1, &spinDetail);
	//spinHour->SetNumer(0, 1, 9, 0);
	//spin2->AddPositionLink(spinHour, (eGlsPosition)(eGP_RIGHTSIDE|eGP_BOTTOM));

	//cglsSpinBox* spinMin = new cglsSpinBox();
	//spinMin->Create(this, 873, 600, 150, 2);
	//spinMin->SetNumer(0, 1, 10, 0, 1);
	//spinMin->SetNumer(1, 0, 1, 0);
	//spinMin->SetLimited(0, 12);
	//spinHour->AddPositionLink(spinMin, (eGlsPosition)(eGP_RIGHTSIDE|eGP_BOTTOM), -10);
	//cglsSpinBox* spinSec = new cglsSpinBox();
	//spinSec->Create(this, 872, 600, 150, 2);
	//spinSec->SetNumer(0, 0, 9, 2);
	//spinSec->SetNumer(1, 0, 5, 3);
	//spinMin->AddPositionLink(spinSec, (eGlsPosition)(eGP_RIGHTSIDE|eGP_BOTTOM), -10);


	spinDetail.bIsShowButton=False;
	cglsSpinBox* spinTime = new cglsSpinBox();
	spinTime->Create(this, 871, 600, 150, 6, &spinDetail);
	spinTime->SetNumer(0, 0, 9, 0);
	spinTime->SetNumer(1, 0, 5, 0);
	spinTime->SetNumer(2, 0, 9, 0);
	spinTime->SetNumer(3, 0, 5, 0);
	spinTime->SetNumer(4, 0, 9, 1);
	spinTime->SetNumer(5, 0, 1, 0);
	spinTime->SetLimited(4, 16);

	spinTime->InsertLabel(L":", 2, Color::Black);
	spinTime->InsertLabel(L":", 4, Color::Black);
	spin->AddPositionLink(spinTime, (eGlsPosition)(eGP_RIGHTSIDE|eGP_BOTTOM), 10);


	//spin2->SetNumer(0, 0, 5, 0);
	//spin2->SetNumer(1, 0, 6, 0);
	//spin2->SetNumer(2, 0, 9, 0);


	// ---------------------------------------------------
	cglsScrollBar* scroll = new cglsScrollBar();
	scroll->Create(this);
	scroll->SetRange(0, m_h*2, m_h);

	cglsEditBox* editMulti = new cglsEditBox();
	cglsEditBox::xDefaultDetail(&editDetail);
	editDetail.bIsMutipleLine=True;
	editDetail.gls.font=new Font(L"Meiryo", 10.0);

	editMulti->Create((HWND)this, 833, 500, 500, 300, 100, &editDetail);
	editMulti->Show();

	cglsEditBox* editStatic = new cglsEditBox();
	editDetail.bIsMutipleLine=False;
	editDetail.bIsStaticEditorMode=True;
	editDetail.bIsFontSizeVariable=True;
	editStatic->Create((HWND)this, 834, 900, 500, 100, 30, &editDetail);
	editStatic->SetText(L"Hellow");
	editStatic->Show();

	cglsRadioGroup* radio = new cglsRadioGroup();
	radio->Create(this, 1201, 700, 300);
	radio->AddItem(1, L"選択1");
	radio->AddItem(2, L"Selection 2");
	radio->AddItem(3, L"選択3");
	radio->AddItem(4, L"あいうえお！！");
	radio->SetChecked(0);
	radio->Show();
	radio->Update(eGUM_FORCED);

	cglsCheckGroup* check = new cglsCheckGroup();
	sCheckGroupDetail checkDetail;
	cglsCheckGroup::xDefaultDetail(&checkDetail);

	checkDetail.gls.font=new Font(L"Meiryo", 11.0);
	check->Create(this, 1202, 700, 250);// 0);//&checkDetail);
	check->AddItem(1, L"選択1");
	check->AddItem(2, L"Selection 2");
	check->AddItem(3, L"選択3");
	check->AddItem(4, L"あいうえお！！");
	//radio->SetChecked(0);
	check->Show();
	check->Update(eGUM_FORCED);

	SetTimer(m_hWnd, 1, 1000, NULL);

	sGlsListBoxDetail listDetail;

	Font listFont(L"Meiryo UI", 11.0);
	cglsListBox::xDefaultDetail(&listDetail);
	listDetail.bIsSelectionCheck=True;
	listDetail.bIsMultiSelect=True;
	listDetail.stack.gls.font=&listFont;
	cglsListBox* listbox=new cglsListBox();

	listbox->Create((HWND)this, 3300, 1000, 50, 100, 100, &listDetail);
	listbox->Show();

	int cnt=1;
	for (int i=0; i<10; i++)
	{
		listbox->AddItem(cnt++, 0, L"選択1");
		listbox->AddItem(cnt++, 0, L"Selection 2");
		listbox->AddItem(cnt++, 0, L"選択3");
		listbox->AddItem(cnt++, 0, L"あいうえお！！");
	}

	listbox->Update(eGUM_FORCED);

	listbox->SelectItem(0);

	//listbox->Clear();

	//listbox->ReSize(200, 300);

	cglsPageScroll* zzz = new cglsPageScroll();
	zzz->Create(this, 222, 0, 0, &Font(L"Meiryo", 10.0), 5, True, 0);
	zzz->Update(eGUM_FORCED);
	zzz->Show();

	editMulti->AddPositionLink(zzz, eGP_BOTTOMSIDE|eGP_LEFT);

	Bitmap* bmpx = cyiShape::BitmapString(0, L"黒3目半勝ち", &Font(L"ＭＳ Ｐ明朝", 13), &SolidBrush(Color::Black), eSTS_GROW, Color::White, 5.0, 0.0);
	cglsImageBox* img = new cglsImageBox();
	img->Create(this, 3301, 1300, 50, 100, 100, 0, False);

	listbox->AddPositionLink(img, eGP_BOTTOMSIDE|eGP_LEFT);

	//cglsListView* listView = new cglsListView();
	//listView->Create(this, 3311, 0, 0, 350, 100);

	//listView->CreateColumn(3, 4, L"名前", eGIA_LEFT, 3, L"段位", eGIA_LEFT, 5, L"日時", eGIA_LEFT);
	//listView->Show();

	//listView->AddItem(1, 0, L"宮下", 0, L"3段", 0, L"2011/10/11");
	//listView->AddItem(2, 0, L"村田", 0, L"6段", 0, L"2011/09/11");
	//listView->AddItem(3, 0, L"ぎゃおー", 0, L"1段", 0, L"2011/10/10");
	//listView->AddItem(4, 0, L"どんぱぱき", 0, L"1段", 0, L"2011/10/10");
	//listView->AddItem(5, 0, L"みやみやココスバーキー", 0, L"1段", 0, L"2011/10/10");
	//listView->AddItem(6, 0, L"ままちゃん", 0, L"1段", 0, L"2011/10/10");

	//listView->Update(eGUM_FORCED);
	//spinTime->AddPositionLink(listView, eGP_RIGHTSIDE|eGP_TOP, 10);

	cglsPageScroll* xxx = new cglsPageScroll();
	xxx->Create(this, 222, 0, 0, &Font(L"Meiryo", 10.0), 5, True, 0);
	xxx->Update(eGUM_FORCED);
	xxx->Show();

	editMulti->AddPositionLink(xxx, eGP_BOTTOMSIDE|eGP_LEFT);

	return 0;
}
#endif

// ---------------------------------------------------
LRESULT MainWindow::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}

#include "glsMessageBox.h"

// ---------------------------------------------------
//LRESULT MainWindow::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
//{
//	return 0;
//}
int g_addcnt=0;

#ifdef __XXXX
// ---------------------------------------------------
LRESULT MainWindow::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	WORD wmId    = LOWORD(wParam);
	WORD wmEvent = HIWORD(wParam);

	bIsHandled=True;
	// 選択されたメニューの解析:
	switch (wmId)
	{
	case IDM_ABOUT:
		DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), m_hWnd, About);
		break;
	case IDM_EXIT:
	{
		cglsMessageBox box;
		if (box.Show(this, L"本当に終了しちゃうの？\r\n2行目も？", L"caption", MB_OKCANCEL)==IDOK)
		{
			DestroyWindow(m_hWnd);
			//PostQuitMessage(0);
		}
		break;
	}
	case 3301:
	{
		wchar_t* pwszFileName = (wchar_t*)wParam;
		cglsImageBox* img = (cglsImageBox*)lParam;

		Bitmap* bmp = img->ExportBitmap(200, 200);

		if (bmp)
		{
			cyiShape::SaveBitmap(L"c:\\yotsuya\\abcd.jpg", bmp, L"image/jpeg");

			int size;
			void* p = yiFileLoad("c:\\yotsuya\\abcd.jpg", &size);

			yiFileWrite("c:\\yotsuya\\1111.jpg", p, 0, size);
		}


		break;
	}
	case 737:
	{
		int code = wParam>>16;

		if (code==eGTBT_ALEFT)
		{
			cglsSplitButton* p = (cglsSplitButton*)GetChild(780);
			p->Move(p->m_x-10, p->m_y+10);

			//wchar_t szCaption[128];
			//wsprintfW(szCaption, L"eGTBT_ALEFT", g_addcnt++);
			//tabctrl.InsertItem(0, 100+g_addcnt, 0, szCaption, True);
			//tabctrl.Update(::eGUM_FORCED);
		}
		else if (code==eGTBT_ARIGHT)
		{
			cglsSplitButton* p = (cglsSplitButton*)GetChild(780);
			p->Move(p->m_x+10, p->m_y-10);

			//wchar_t szCaption[128];
			//wsprintfW(szCaption, L"eGTBT_ARIGHT", g_addcnt++);
			//tabctrl.InsertItem(0, 100+g_addcnt, 0, szCaption, True);
			//tabctrl.Update(::eGUM_FORCED);
		}
		break;
	}
	case 781:
	{
		wchar_t szMsg[256];
		wsprintfW(szMsg, L"%d Selected", wmEvent);
		MessageBox(m_hWnd, szMsg, L"info", MB_OK);
		break;
	}
	case 738:
	{
		cglsSpinBox* sb = (cglsSpinBox*)GetChild(877);

		int code = wParam>>16;
		int val[3];
		if (code==eGTBT_AUP)
		{
			for (int i=0;i<3; i++)
				val[i] = sb->GetValue(i);
		}
		else if (code==eGTBT_ADOWN)
		{
			//sb->ScrollTo(300, 1000);
		}
		break;
	}
	case 1024:
	{
		cglsMonthCalender cal;

		//cal.DoModal(m_hWnd, m_hInstance, "日付", 333,  0, 300, 300);
		cal.TrackPopup(m_hWnd, 100, 100, MAKETIME64(1974, 8, 30, 0, 0, 0));

		//cglsWindow* win = new cglsWindow();
		//win->CreatePopup(m_hWnd, 100, 100, 100, 100);
		//Graphics g(m_bg);
		//g.FillRectangle(&SolidBrush(Color::Beige), 0, 0, m_w, m_h);

		//int t1 = cyiTime::CurrentmSec();

		////Bitmap* bmp = cyiBitmapBlender::GaussBlureXX((Bitmap*)m_img, 7.0, 100, 0);
		////Bitmap* bmp = cyiBitmapBlender::DropShadowEx((Bitmap*)m_img2, 5, 5, 5.0, 1.0);
		////Bitmap* bmp = cyiBitmapBlender::DropShadow((Bitmap*)m_img2, 5, 5, 5.0, 0.5);
		//int t2 = cyiTime::GetSecDiff(t1, cyiTime::CurrentmSec());


		////g.DrawImage(bmp, 500, 100);
		////delete bmp;
		//cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);
		//Update(m_bmpCurrent);
		//InvalidateRect(0, False);
		break;
	}
	case 1025:
	{
		OnBgDraw();
		break;
		Graphics g(m_bg);
		g.FillRectangle(&SolidBrush(Color::White), 0, 0, m_w, m_h);
		cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);
		Update(m_bmpCurrent);
		InvalidateRect(0, False);
		break;
	}
	case 1030:
	{
		cglsMessageBox msgBox;

		int n = msgBox.Show(this, L"背景を変更しますか？\r\nBitmap* bmp = cyiBitmapBlender::DropShadowEx((Bitmap*)m_img2, 0, 0, 5.0, 1.0, Color::White);\r\nBitmap* bmp = cyiBitmapBlender::DropShadowEx((Bitmap*)m_img2, 0, 0, 5.0, 1.0, Color::White);", 0, MB_OKCANCEL, &Font(L"メイリオ", 13.0));

		if (n==IDCANCEL)
			break;

		Graphics g(m_bg);
		g.FillRectangle(&SolidBrush(Color::Black), 0, 0, m_w, m_h);
		int t1 = cyiTime::CurrentmSec();
		//Bitmap* bmp = cyiBitmapBlender::Blure((Bitmap*)m_img, 5);
		Bitmap* bmp = cyiBitmapBlender::DropShadowEx((Bitmap*)m_img2, 0, 0, 5.0, 1.0, Color::White);
		int t2 = cyiTime::GetSecDiff(t1, cyiTime::CurrentmSec());
		g.DrawImage(bmp, 500, 150);
		delete bmp;
		cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);
		m_eUpdateReason=eGUR_BG_CHANGED;
		Update(m_bmpCurrent);
		InvalidateRect(0, False);
		break;
	}
	case 1031:
	{
		wchar_t szCaption[128];
		wsprintfW(szCaption, L"Item Added %d", g_addcnt++);
		tabctrl.InsertItem(0, 100+g_addcnt, 0, szCaption, True);
		tabctrl.SetCurSel(0);
		//tabctrl.Update(::eGUM_FORCED);
		//static Bool bIsTrue=True;

		//btn3.AnimationShow(bIsTrue, bIsTrue?0.0:1.0, bIsTrue?1.0:0.0);

		//bIsTrue=1-bIsTrue;

		break;
	}
	case 1028:
	{
		tabctrl.ReSize(300, new Font(L"MS P明朝", 10.0));
		break;
	}
	case 1029:
	{
		tabctrl.ReSize(500, new Font(L"Meiryo", 11.0));
		break;
	}
	//case 1025:
	//case 1026:
	//{
	//	wchar_t szMessage[256];
	//	wsprintfW(szMessage, L"Button %d Clicked", wmId);
	//	//MessageBoxA(hWnd, szMessage, "information", MB_OK|MB_ICONINFORMATION);
	//	btn.SetItem(0, szMessage, True);
	//	break;
	//}
	//case 1029:
	//{
	//	panelH->SetItem(1025, 0, L"テスト");
	//	break;
	//}
	default:
		bIsHandled=False;
		break;
	}

	return 0;
}
#endif

// ---------------------------------------
LRESULT MainWindow::OnTabItemNotify(int id, int subId, int code)
{
	// タブコントロール
	if (id==1)
	{
		if (code==TCN_SELCHANGE)
		{
			wchar_t szText[128];
			wsprintfW(szText, L"%d Selected", subId);
			btn4.SetItemChange((Bitmap*)ITEM_NOCHANGED, szText);
			return 0;
		}
	}

	return 0;
}
// ---------------------------------------
LRESULT MainWindow::OnTimer(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	//return 0;
	cglsSpinBox* spinTime = (cglsSpinBox* )GetChild(871);

	int n = spinTime->GetValue();
	spinTime->SetValue(-1, eSBVF_CURRENT, True);
	return 0;
}


// ---------------------------------------
LRESULT MainWindow::OnRButtonUp(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	handled=True;
	int y = lParam>>16;
	int x = (lParam)&0xFFFF;

	cglsPopupMenu* menu = new cglsPopupMenu();

	sGlsPopupMenuDetail detail;
	cglsPopupMenu::xDefaultDetail(&detail);

	menu->Create(m_hWnd, 737);
	menu->AddItem(15, 0, L"選択１");
	menu->AddItem(16, 0, L"選択２");
	menu->AddItem(17, 0, L"選択３");
	menu->AddItem(18, 0, L"選択４");
	menu->AddItem(19, 0, L"選択６");

	menu->ShowAndWait(x, y);

	wchar_t buffer[128];
	menu->GetResult(buffer);

	menu->Destroy();
	//delete menu;
	return 0;
	//if (g_menu==0)
	//{
	//	cglsPopupMenu* submenu = new cglsPopupMenu();

	//	g_menu= new cglsPopupMenu();
	//	sGlsPopupMenuDetail detail;
	//	cglsPopupMenu::xDefaultDetail(&detail);

	//	submenu->Create((HWND)this, 11, x, y, &detail);
	//	submenu->AddItem(15, 0, L"joseki.dat");
	//	submenu->AddItem(16, 0, L"untitled.sgf");
	//	submenu->AddItem(17, 0, L"abc.sgf");
	//	g_menu->Create((HWND)this, 11, x, y, &detail);

	//	g_menu->AddItem(12, 0, L"開く", 0);
	//	g_menu->AddItem(13, 0, L"保存", submenu);
	//	g_menu->AddItem(14, 0, L"再生", 0);
	//}

	//g_menu->Move(x, y);
	//g_menu->Show();

	return 0;
}
// ---------------------------------------------------
LRESULT MainWindow::OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	cglsGoban* goban = (cglsGoban*)GetChild(1);

#ifdef __XXXX
	return 0;
#endif

	if (wParam==SIZE_MINIMIZED)
	{
		handled=False;
		return 0;
	}

	RECT rect;
	::GetClientRect(m_hWnd, &rect);

	__super::OnSizeChanged(wParam, lParam, handled);

	if (goban->m_h==m_h &&
			goban->GetRealWidth()==m_w)
	{
		return 0;
	}

	if (goban)
	{
		goban->ReSize(rect.right, rect.bottom);
		int n = goban->GetRealWidth();
		ReSizeClientRect(n, m_h);

	}

	handled=True;
	return 0;
}

// ---------------------------------------
int nPos=0;
LRESULT MainWindow::OnVScroll(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	handled=True;
	cglsScrollBar* bar = (cglsScrollBar*)lParam;
	int cur = bar->GetPos();

	switch(LOWORD(wParam)) {
	case SB_LINEUP:
		cur--;
		break;
	case SB_LINEDOWN:
		cur++;
		break;
	case SB_PAGEUP:
		cur-=m_h;
		break;
	case SB_PAGEDOWN:
		cur+=m_h;
		break;
	case SB_THUMBPOSITION:
		break;
	}
	bar->SetPos(cur);
	return 1;
}


// ---------------------------------------
Bool MainWindow::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	__super::OnPaint(hdc, ps);

	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	//DrawText(hdc, L"Hellow", -1, &rect, DT_CENTER|DT_VCENTER);
	//RECT rect={0, 0, m_w, m_h};
	//SetTextColor(hdc, Color::White);
	//DrawText(hdc, L"asldfjasdlkf", -1, &rect, DT_CENTER|DT_VCENTER);



	return True;
}


// ---------------------------------------
Bool MainWindow::OnBgDraw()
{

	if (m_TextureImg==0)
		return True;

	if (m_bmpCurrent==0)
	{
		m_bmpCurrent=BITMAP32(m_w, m_h);
	}
	if (m_bg==0)
	{
		m_bg=BITMAP32(m_w, m_h);
	}

	//cyiBitmapBlender::FillColor(m_bmpCurrent, 0);
	//Update(m_bmpCurrent);
	//InvalidateRect(0, False, True);

	//return 0;

	int w=m_bmpCurrent->GetWidth();
	int h=m_bmpCurrent->GetHeight();

	if (w!=m_w || h!=m_h)
	{
		delete m_bmpCurrent;
		delete m_bg;
		m_bmpCurrent=BITMAP32(m_w, m_h);
		m_bg=BITMAP32(m_w, m_h);
	}

	Graphics g(m_bg);

	TextureBrush br(m_TextureImg);

	g.FillRectangle(&br, 0, 0, m_w, m_h);

	//g.FillRectangle(&SolidBrush(Color::White), 0, 0, m_w, m_h);

	//Graphics g22(m_bg);
	//g22.DrawImage(cyiShape::DrawGlassCover(100, 30, 2.0), m_w/2, 100);

	//cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);

	////Graphics g22(m_bg);
	////g22.DrawImage(cyiShape::DrawGlassCover(100, 30, 2.0), m_w/2, 0);

	////NotifyBgChanged();
	//Bitmap* bmp = cyiShape::BitmapString(0, L"黒3目半勝ち", &Font(L"ＭＳ Ｐ明朝", 48), &SolidBrush(Color::Black), eSTS_GROW, Color::White, 5.0, 0.0);
	//g22.DrawImage(bmp, m_w/2, 0);

	//wchar_t blackResult[128];
	//wsprintfW(blackResult, L"黒  アゲハマ: 10 地: 55");
	//Bitmap* bmpBlack=cyiShape::BitmapString(0, blackResult, &Font(L"Meiryo", 12), &SolidBrush(Color::Black), eSTS_GROW, Color::White, 3.0, 0.5);
	//g22.DrawImage(bmpBlack, m_w/2, bmp->GetHeight());

	//wchar_t whiteResult[128];
	//wsprintfW(whiteResult, L"白  アゲハマ: 12 地: 44 コミ：6目半");
	//Bitmap* bmpWhite=cyiShape::BitmapString(0, whiteResult, &Font(L"Meiryo", 12), &SolidBrush(Color::White), eSTS_GROW, Color::Black, 3.0, 0.1);
	//g22.DrawImage(bmpWhite, m_w/2, bmpBlack->GetHeight()+bmp->GetHeight());

	m_eUpdateReason=eGUR_BG_CHANGED;
	m_nRequestUpdateOrder=0;
	Update(m_bmpCurrent);
	InvalidateRect(0, False, True);
	return True;
}