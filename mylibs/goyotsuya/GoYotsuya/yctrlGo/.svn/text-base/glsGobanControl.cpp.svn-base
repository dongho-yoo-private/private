#include "glsGobanControl.h"
#include "glsSplitButton.h"
#include "yiGoCommandz.h"

cglsGobanControl::cglsGobanControl(void)
: m_scale(1.0)
{
}

cglsGobanControl::~cglsGobanControl(void)
{
}

// -----------------------------------------
Bool cglsGobanControl::Create(HWND hWnd, int id, float scale, int x, int y, eGobanControlSet set, sGlsGobanControlDetail* detail)
{
	sGlsStackPannelDetail panelDetail;
	cglsStackPannel::xDefaultDetail(&panelDetail);

	panelDetail.bUseGaussPannel=True;
	panelDetail.gls.font=detail->font;

	panelDetail.gauss.colorTop=__COLORp__(127, 0, 0, 0);
	panelDetail.gauss.colorBottom=__COLORp__(200, 0, 0, 0);

	RECT rect = {x, y, 0, 0};
	RECT margin = {3, 3, 3, 3};
	if (cglsStackPannel::Create(hWnd, id, &rect, &margin, 0, &panelDetail)==False)
	{
		return False;
	}

	m_eControlSet=set;
	m_bIsCreated=False;
	CreateButtonz(scale);
	m_bIsCreated=True;

	InvalidateRect(0, 0, 0, True);

	return True;
}

// -----------------------------------------
Bool cglsGobanControl::ChangeControlSet(eGobanControlSet set)
{
	if (m_eControlSet==set)
	{
		return False;
	}

	m_eControlSet=set;

	InvalidateRect(0);
	CreateButtonz(m_scale);
	return True;
}

// -----------------------------------------
Bool cglsGobanControl::SetScale(float scale)
{
	m_scale=scale;
	return True;
}

#include "glsButton.h"
#include "glsResourceBitmap.h"

// -----------------------------------------
void cglsGobanControl::xCreateButton(int id, int bitmapId, wchar_t* pszText, wchar_t* comment, int systemId)
{
	sDrawItem* child=GetItem(id);
	sGlsButtonDetail detail;
	cglsButton::xDefaultDetail(&detail);

	detail.gls.style=eGGS_NEON;
	if (child==0)
	{
		cglsButton* btn = new cglsButton();
		btn->Create((HWND)this, id, 0, 0, (Bitmap*)bitmapId, pszText, comment, &detail);		
		Add(btn, 0);

		if (systemId)
		{
			cglsSystem* system = GetSystem();

			if (system)
			{
				system->RegisterCommand(btn, systemId);
			}
		}
	}

	if (m_eControlSet&id)
	{
		ShowItem(True, id);
		//EnableItem(id, True);
	}
	else
	{
		ShowItem(False, id);
	}

	return ;
}


// -----------------------------------------
void cglsGobanControl::CreateButtonz(float scale)
{
	int nButtonHeight=cglsButton::GetMeasureHeight(GetFont());

	xCreateButton(eGCS_PASS, 0,  L"パス", 0, eGCID_PASS);
	xCreateButton(eGCS_GIVEUP, 0,  L"投了", 0, eGCID_GIVEUP);
	xCreateButton(eGCS_PREV_BRANCH, 0, L"前の枝へ", L"前の枝へ", eGCID_PREV_BRANCH);
	xCreateButton(eGCS_NEXT_BRANCH, 0,  L"次の枝へ", L"次の枝へ", eGCID_NEXT_BRANCH);
	xCreateButton(eGCS_STUDY_END, 0,  L"検討終了", 0, eGCID_STUDY_END);




	xCreateButton(eGCS_ONE_MORE_TIME, 0,  L"もう一局", 0, eGCID_REQ_PLAY_DIALOG);


	sDrawItem* child=GetItem(eGCS_LABEL);
	sGlsSplitButtonDetail detail;
	cglsSplitButton::xDefaultDetail(&detail);
	detail.gls.style=eGGS_NEON;
	if (m_eControlSet&eGCS_LABEL)
	{
		if (child==0)
		{
			cglsSplitButton* btn = new cglsSplitButton();
			btn->Create((HWND)this, eGCS_LABEL, 0, 0, GetFontHeight()*5/*(Bitmap*)eGRID_NOT_SHOW_NO*/, nButtonHeight, L"ラベル", &detail);
			
			btn->AddItem(eLT_LABLE_CLOSE, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"ラベル", L"ラベル入力終了");//L"ABC...", L"ABC...");
			btn->AddItem(eLT_ROMA_LARGE, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"ABC...", L"ABC...");
			btn->AddItem(eLT_ROMA_SMALL, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"abc...", L"abc...");
			btn->AddItem(eLT_NUMERIC, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"123...", L"123...");
			btn->AddItem(eLS_CIRCLE, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"○", L"○");
			btn->AddItem(eLS_SQURE, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"□", L"□");
			btn->AddItem(eLS_TRIANGLE, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"△", L"△");
			Add(btn, 0);
			btn->Update(eGUM_FORCED);
			btn->Show();

			cglsSystem* system = GetSystem();

			if (system)
			{
				system->RegisterCommand(btn, eGCID_LABEL, eGCS_LABEL);
			}
		}
		ShowItem(True, eGCS_LABEL);
	}
	else
		ShowItem(False, eGCS_LABEL);

	xCreateButton(eGCS_DELETE_NODE, 0,  L"枝削除", L"現在の枝を削除します。", eGCID_DELETE_NODE);
	child=GetItem(eGCS_ADDSTONE);
	if (m_eControlSet&eGCS_ADDSTONE)
	{
		if (child==0)
		{
			cglsSplitButton* btn = new cglsSplitButton();
			btn->Create((HWND)this, eGCS_ADDSTONE, 0, 0, GetFontHeight()*4/*(Bitmap*)eGRID_NOT_SHOW_NO*/, nButtonHeight, L"右クリック：黒、左クリック：白");
			btn->AddItem(1, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"置石", L"置石入力終了");
			btn->AddItem(2, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"置石入力", L"置石入力開始");
			btn->Show();
			btn->Update(eGUM_FORCED);
			Add(btn, 0);
		}
		ShowItem(True, eGCS_ADDSTONE);
		//EnableItem(eGCS_LABEL, False);

	}
	else
		ShowItem(False, eGCS_ADDSTONE);

	// 対局モード時

	xCreateButton(eGCS_GIVEUP, 0, L"投了", 0, eGCID_GIVEUP);

	xCreateButton(eGCS_WAIT,	0, L"待った", 0/*L"待った(3回まで)"*/, eGCID_WAIT);
	xCreateButton(eGCS_HUU,		0, L"封じ手",0/* L"封じ手"*/, eGCID_HUU);
	xCreateButton(eGCS_PAUSE,	0, L"打ち掛け", 0/*L"一時中断"*/, eGCID_PAUSE);
	xCreateButton(eGCS_CANCEL,	0, L"対局取り消し", L"15手以下までは相手の許可なしで対局を中断できます。", eGCID_CANCEL);
	xCreateButton(eGCS_CLOSE,	0, L"閉じる", L"閉じる", eGCID_CLOSE);
	xCreateButton(eGCS_COPY,	0, L"碁盤を複製", L"現在のノードまでを別のタブにコピーします。(Ctrl+V)", eGCID_COPY);

	
	if (m_eControlSet&eGCS_PASS)
	{
		AddLF(0);
	}

	xCreateButton(eGCS_TOP,		eGRID_TOP,  0,  L"先頭へ(Ctrl+Shift+右クリック)", eGCID_TOP);
	xCreateButton(eGCS_PP,		eGRID_PP,	0,  L"数手前へ(Ctrl+右ダブルクリック)", eGCID_PP);
	xCreateButton(eGCS_PREV,	eGRID_PREV, 0,	L"一手前へ(Ctrl+右クリック)", eGCID_PREV);
	xCreateButton(eGCS_NEXT,	eGRID_NEXT, 0,  L"次へ(Ctrl+右ダブルクリック)", eGCID_NEXT);
	xCreateButton(eGCS_NN,		eGRID_NN,	0,  L"数手前へ(Ctrl+右ダブルクリック)", eGCID_NN);
	xCreateButton(eGCS_END,		eGRID_END,  0,  L"数手前へ(Ctrl+右ダブルクリック)", eGCID_END);
	//xCreateButton(eGCS_NN, eGRID_NN,  0,  L"数手前へ(Ctrl+右ダブルクリック)");


	
	child=GetItem(eGCS_SHOW_NO);
	if (m_eControlSet&eGCS_SHOW_NO)
	{
		if (child==0)
		{
			cglsSplitButton* btn = new cglsSplitButton();
			btn->Create((HWND)this, eGCS_SHOW_NO, 0, 0, GetFontHeight()*5/*(Bitmap*)eGRID_NOT_SHOW_NO*/, nButtonHeight,  L"手順表示・非表示");
			btn->AddItem(1, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"手順×", L"手順を隠す");
			btn->AddItem(2, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"手順表示", L"手順を表示");
			btn->AddItem(3, 0/*(Bitmap*)eGRID_SHOW_NO_FROM*/, L"～手順", L"今の手から手順を表示");
			btn->Update(eGUM_FORCED);

			cglsSystem* system = GetSystem();

			if (system)
			{
				system->RegisterCommand(btn, eGCID_SHOW_NO, eGCS_SHOW_NO);
			}

			btn->Show();
			Add(btn, 0);
		}
		ShowItem(True, eGCS_SHOW_NO);
	}
	else
		ShowItem(False, eGCS_SHOW_NO);


	child=GetItem(eGCS_AUTOPLAY);
	if (m_eControlSet&eGCS_AUTOPLAY)
	{
		if (child==0)
		{
			cglsSplitButton* btn = new cglsSplitButton();
			btn->Create((HWND)this, eGCS_AUTOPLAY, 0, 0, GetFontHeight()*6/*(Bitmap*)eGRID_NOT_SHOW_NO*/, nButtonHeight, L"自動再生");
			btn->AddItem(1, 0, L"自動再生", L"自動再生終了");
			btn->AddItem(2, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"1秒", L"1秒間隔");
			btn->AddItem(3, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"3秒", L"3秒間隔");
			btn->AddItem(4, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"5秒", L"5秒間隔");
			btn->AddItem(5, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"10秒", L"10秒間隔");
			btn->AddItem(6, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"1分", L"1分間隔");
			btn->AddItem(7, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"5分", L"5分間隔");
			btn->Update(eGUM_FORCED);

			btn->Show();
			Add(btn, 0);
			cglsSystem* system = GetSystem();

			if (system)
			{
				system->RegisterCommand(btn, eGCID_AUTOPLAY, eGCS_AUTOPLAY);
			}
		}
		ShowItem(True, eGCS_AUTOPLAY);
	}
	else
		ShowItem(False, eGCS_AUTOPLAY);

	xCreateButton(eCGS_CALCULATE_END, 0,  L"計算終了",  L"計算終了", eGCID_CALCULATE_CONFIRM);//END);
	xCreateButton(eCGS_CALCULATE_STOP, 0,  L"キャンセル",  L"キャンセル", eGCID_CALCULATE_STOP);


	Update(eGUM_FORCED);
}
