#include "glsInformationDlg.h"

cglsInformationDlg::cglsInformationDlg(void)
: m_listLabel(eGIDID_COUNT),
  m_listValue(eGIDID_COUNT)
{
	__GLASSINTERFACE_NAME__;
}

cglsInformationDlg::~cglsInformationDlg(void)
{
}

#define xID_KOMI 7

// ------------------------------------------
Bool cglsInformationDlg::Create(HWND hwnd, int id, int x, int y, int w, int h, sGoHeader* header, eGlsInformationDlgItemz eItemzFlag, Font* font)
{
	sGlsDialogDetail xdetail;

	cglsDialog::xDefaultDetail(&xdetail);
	//xdetail.gauss.colorTop=__COLORp__(127, 0, 0, 0);
	//xdetail.gauss.colorBottom=__COLORp__(200, 0, 0, 0);
	xdetail.gauss.fShadowWidth=5.0;
	xdetail.gauss.fBlureRad=4.0;
	xdetail.font=0;
	m_header=header;
	SetItemzFlag(eItemzFlag);
	//m_static.C

	xdetail.bIsAnimationShow=False;
	Bool res = cglsDialog::Create(hwnd, id , 0, x, y, w, h, &xdetail);

	if (res==True)
	{
		SetFont(font);
		return True;
	}


	return False;
}

#include "yctrlGobanResource.h"
// ----------------------------------------
Bool cglsInformationDlg::UpdateHeader(sGoHeader* header)
{
	if (header!=(sGoHeader*)0xFFFFFFFF)
	{
		m_header=header;
	}
	//int nKomi = GET_KOMI(header->condition);
	//Bool bIsHalfScore=HALF_SCORE(header->condition);

	//wchar_t buffer[32];
	//wsprintfW(buffer, L"%d%s%s", nKomi, cyctrlGobanResource::GetInstance().GetStringW(eGTRID_MOKU), cyctrlGobanResource::GetInstance().GetStringW(eGTRID_HALF));
	//m_txtGameHandicap2.SetText(buffer);

	SetHeaderValue();
	return True;
}

// ----------------------------------------
void cglsInformationDlg::SetItemzFlag(eGlsInformationDlgItemz eItemzFlag)
{
	for (int i=0; i<eGIDID_COUNT; i++)
	{
		m_fItemzFlagz[i]=((eItemzFlag>>(4+i))&1)!=0;
	}
	m_eItemzFlag=eItemzFlag;
}


#include "glsMonthCalender.h"
#include "glsWatch.h"
#include "glsKomi.h"
#include "glsResult.h"
#include "glsPopupMenu.h"
#include "glsTeai.h"
// ------------------------------------
void cglsInformationDlg::SetHeaderValue()
{
	cglsEditBox* edit;
	
	if (m_header==0)
		return ;

	if (m_header->pszEventName)
	{
		edit = (cglsEditBox*)GetChild(eGIDID_TXT_GAME);
		edit->SetText(yiToUnicode(NULL, m_header->pszEventName, 0));
	}

	if (m_header->playDate)
	{
		cglsEditBox* edit = (cglsEditBox*)GetChild(eGIDID_TXT_GAMETIME);
		if (edit)
		{
			wchar_t buffer[32];
			cglsMonthCalender* cal = (cglsMonthCalender*)edit->GetAssistant();
			cal->SetDay(MAKETIME64(DAY_YEAR(m_header->playDate), DAY_MON(m_header->playDate), DAY_DAY(m_header->playDate), 0, 0, 0));
			
			cal->GetResult(buffer);

			edit->SetText(buffer);
			
		}
	}

	//if (m_header->pszPlace)
	{
		wchar_t buffer[256];
		cglsEditBox* edit = (cglsEditBox*)GetChild(eGIDID_TXT_PLACE);
		if (edit)
			edit->SetText(yiToUnicode(buffer, m_header->pszPlace, 0));
	}

	// 手合い
	{
		Bool bIsNigiri = IS_NIGIRI(m_header->condition);
		int  nHandicap;
		if (bIsNigiri==False)
		{
			 nHandicap = GET_INIT_STONE(m_header->condition);
		}

		cglsEditBox* edit = (cglsEditBox*)GetChild(eGTRID_TXT_TEAI);

		if (edit)
		{
			cglsPopupMenu* menu = (cglsPopupMenu*)edit->GetAssistant();


			if (bIsNigiri==True)
				menu->SetCheck(0, True, True);
			else
			{
				menu->SetCheck(nHandicap==0?1:nHandicap, True, True);
			}
			wchar_t buffer[32];
			menu->GetResult(buffer);
			edit->SetText(buffer);
		}

	}
	
	// コミ
	{
		float fKomi = GET_KOMI(m_header->condition)+(HALF_SCORE(m_header->condition)?0.5:0.0);
		Bool bIsInverseKomi = REVERSE_KOMI(m_header->condition);
		cglsEditBox* edit = (cglsEditBox*)GetChild(eGIDID_TXT_HANDICAP);

		if (bIsInverseKomi)
			fKomi=-fKomi;

		if (edit)
		{
			wchar_t buffer[32];
			cglsKomi* komi= (cglsKomi*)edit->GetAssistant();
			komi->SetKomi(fKomi);//->Set((winer<<24)|(bIsHalfScore<<16)|(score));
			//cal->SetDay(MAKETIME64((m_header->playDate>>12), ((m_header->playDate>>6)&0x3F), (m_header->playDate&0x3F), 0, 0, 0));
			
			komi->GetResult(buffer);
			edit->SetText(buffer);
		}		
	}
	// 結果
	{
		int winer = GET_WINER(m_header->flag32);
		int score = GET_SCORE(m_header->flag32);
		Bool bIsHalfScore = HALF_SCORE(m_header->condition);

		cglsEditBox* edit = (cglsEditBox*)GetChild(eGIDID_TXT_RESULT);
		if (edit)
		{
			wchar_t buffer[32];
			cglsResult* res = (cglsResult*)edit->GetAssistant();
			res->Set((winer<<24)|(score), bIsHalfScore);
			
			res->GetResult(buffer);
			edit->SetText(buffer);
		}
	}

	// 持ち時間・秒読み
	{
		unsigned int time = GET_PLAY_TIME(m_header->condition);//MAKETIME(0, 0, 0, m_header->remainTime/3600, (m_header->remainTime%3600)/60, (m_header->remainTime%60));
		unsigned int xx = ((GET_SECOND_READ_COUNT(m_header->condition2))<<24)|((GET_SECOND_READ2(m_header->condition)));
		cglsEditBox* edit = (cglsEditBox*)GetChild(eGIDID_TXT_LIMITTIME);
		if (edit)
		{
			wchar_t buffer[32];
			cglsWatch* wat = (cglsWatch*)edit->GetAssistant();

			time = MAKETIME(0, 0, 0, time/60, time%60, 0);
			wat->Set(time, xx);//m_header->remainSec);
			
			wat->GetResult(buffer);
			edit->SetText(buffer);
		}
	}

	//if (GET_SCORE(header->flag32)

	InvalidateRect(0);
}

// ----------------------------------------
static void xMoveRightAlign(cGlassInterface* obj, int pos)
{
	obj->Move((pos-obj->m_w), obj->m_y);
}



// ------------------------------------
Bool cglsInformationDlg::xCreateChild(sGoHeader* header)
{
	RECT rect;
	sGlsStaticDetail detail;
	sGlsEditBoxDetail editDetail;

	int height=0;
	int maxWidth=0;

	cglsStatic::xDefaultDetail(&detail);
	cglsEditBox::xDefaultDetail(&editDetail);

	editDetail.bIsStaticEditorMode=True;
	GetClientRect(&rect);

	//// 大会名
	{
		wchar_t buffer[64];
		cglsStatic* label=0;

		wsprintfW(buffer, L"%s:", cyctrlGobanResource::GetInstance().GetStringW(eGTRID_GAMENAME));
		m_listLabel.enq((label=CreateLabel(buffer, &detail)));


		cglsEditBox* edit = new cglsEditBox();
		edit->Create((HWND)this,  eGIDID_TXT_GAME, 0, 0, 0, label->m_h, &editDetail);
		m_listValue.enq(edit);

	}

	// 日時
	{
		wchar_t buffer[64];
		cglsStatic* label=0;

		wsprintfW(buffer, L"%s:", cyctrlGobanResource::GetInstance().GetStringW(eGTRID_DATE));
		m_listLabel.enq((label=CreateLabel(buffer, &detail)));

		cglsEditBox* edit = new cglsEditBox();
		editDetail.assistant=new cglsMonthCalender();
		editDetail.bIsStaticEditorMode=True;
		edit->Create((HWND)this,  eGIDID_TXT_GAMETIME, 0, 0, 0, 0, &editDetail);
		m_listValue.enq(edit);
	}

	// 場所
	{
		wchar_t buffer[64];
		cglsStatic* label=0;

		wsprintfW(buffer, L"%s:", cyctrlGobanResource::GetInstance().GetStringW(eGTRID_PLACE));
		m_listLabel.enq((label=CreateLabel(buffer, &detail)));

		cglsEditBox* edit = new cglsEditBox();
		editDetail.assistant=0;//new cglsMonthCalender();
		editDetail.bIsStaticEditorMode=True;
		editDetail.bIsMouseDownShow=True;
		edit->Create((HWND)this,  eGIDID_TXT_PLACE, 0, 0, 0, 0, &editDetail);
		m_listValue.enq(edit);
	}

	// 持ち時間
	{
		wchar_t buffer[64];
		cglsStatic* label=0;

		wsprintfW(buffer, L"%s:", cyctrlGobanResource::GetInstance().GetStringW(eGTRID_LIMIT_TIME));
		m_listLabel.enq((label=CreateLabel(buffer, &detail)));
		cglsEditBox* edit = new cglsEditBox();
		editDetail.assistant= new cglsWatch(eGTF_ALL, MAKETIME(2009, 0, 0, 0, 20, 0));
		editDetail.bIsStaticEditorMode=True;
		edit->Create((HWND)this,  eGIDID_TXT_LIMITTIME, 0, 0, 0, 0, &editDetail);
		m_listValue.enq(edit);
	}

	// 手合い
	{


		wchar_t buffer[64];
		cglsStatic* label=0;
		wsprintfW(buffer, L"%s:", cyctrlGobanResource::GetInstance().GetStringW(eGTRID_TEAI));
		m_listLabel.enq((label=CreateLabel(buffer, &detail)));


		//cglsPopupMenu* menu = new cglsPopupMenu();
		cglsListMenu* menu = new cglsListMenu(GetFont());
		//menu->Create((HWND)this, 2);
		menu->AddItem(1, L"互い先");
		menu->AddItem(2, L"定先");
		menu->SetCheck(0, 1, True);

		for (int i=2; i<10; i++)
		{
			wchar_t buffer[16];
			wsprintfW(buffer, L"%d子局", i);
			menu->AddItem(i+1, buffer);

		}

		cglsEditBox* edit = new cglsEditBox();

		editDetail.assistant= menu;//new cglsWatch(eGTF_ALL, MAKETIME(2009, 0, 0, 0, 20, 0));
		editDetail.bIsStaticEditorMode=True;
		edit->Create((HWND)this,  eGTRID_TXT_TEAI, 0, 0, 0, 0, &editDetail);
		m_listValue.enq(edit);
	}

	// コミ
	{
		wchar_t buffer[64];
		cglsStatic* label=0;
		wsprintfW(buffer, L"%s:", cyctrlGobanResource::GetInstance().GetStringW(eGTRID_KOMI));
		m_listLabel.enq((label=CreateLabel(buffer, &detail)));

		cglsEditBox* edit = new cglsEditBox();
		editDetail.assistant= new cglsKomi(6.5);//(eGTF_ALL, MAKETIME(2009, 0, 0, 0, 20, 0));
		editDetail.bIsStaticEditorMode=True;
		edit->Create((HWND)this,  eGIDID_TXT_HANDICAP, 0, 0, 0, 0, &editDetail);//eGIDID_TXT_LIMITTIME, 0, 0, 0, label->m_h, &editDetail);
		m_listValue.enq(edit);
	}

	// 対局結果
	{
		wchar_t buffer[64];
		int n = ((GET_WINER(m_header->flag32)<<24)|(GET_SCORE(m_header->flag32)));
		cglsStatic* label=0;
		wsprintfW(buffer, L"%s:", cyctrlGobanResource::GetInstance().GetStringW(eGTRID_PLAYRESULT));
		m_listLabel.enq((label=CreateLabel(buffer, &detail)));

		cglsEditBox* edit = new cglsEditBox();
		editDetail.assistant= new cglsResult(n, HALF_SCORE(m_header->flag32));//(eGTF_ALL, MAKETIME(2009, 0, 0, 0, 20, 0));
		editDetail.bIsStaticEditorMode=True;
		edit->Create((HWND)this,  eGIDID_TXT_RESULT, 0, 0, 0, 0, &editDetail);//eGIDID_TXT_LIMITTIME, 0, 0, 0, label->m_h, &editDetail);
		m_listValue.enq(edit);
	}
	
	return True;
}

// ----------------------------------------
LRESULT cglsInformationDlg::OnCreate(WPARAM wParam, LPARAM lParam)
{
	if (xCreateChild(m_header))
	{
		UpdateHeader((sGoHeader*)-1);
		//__super::Update(eGUM_FORCED);
		return ReArrangeItemz();
	}
	return 0;
}


// ------------------------------------
void cglsInformationDlg::OnItemReArranged()
{
	int maxw=0;
	cyitrb<int> lineq(m_listLabel.count());
	cyitrb<cglsStatic> queueTemp(m_listLabel.count());
	int xOffset = GetFontHeight(True)/2;

	RECT rect;

	GetClientRect(&rect);

	//xtNODELOOP(node, (&m_listLabel))

	cGlassInterface::Update(eGUM_FORCED);

	for (int i=0; i<eGIDID_COUNT; i++)
	{
		if (m_fItemzFlagz[i]==0)
		{
			continue;
		}

		cglsStatic* label=m_listLabel.getp(i);//.REF(node);

		if (m_fItemzFlagz[i]==0)
		{
			label->Hide();
			continue;
		}

		// 生成時失敗？
		if (label==0)
		{
			return ;
		}

		maxw=label->m_w>maxw?label->m_w:maxw;
		lineq.enq(label->m_h);
	}

	int sum=rect.top+m_fEdge+1;
	int nmargin = GetFontHeight(True)/2;
	//xtNODELOOP(node, (&m_listLabel))
	for (int i=0; i<eGIDID_COUNT; i++)
	{
		int h=0;
		cglsStatic* label=m_listLabel.getp(i);//.REF(node);

		if (m_fItemzFlagz[i]==0)
		{
			label->Hide();
			m_listValue.getp(i)->Hide();
			continue;
		}

		// 生成時失敗？
		if (label==0)
		{
			return ;
		}
		if (lineq.deq(h)==RINGBUFFER_EMPTY)
		{
			return ;
		}

		sum+=(h+1);

		label->Move(xOffset+maxw+m_fEdge+rect.left, sum, (eGlsItemAlign)(eGIA_RIGHT|eGIA_BOTTOM));
		label->Show();


		cglsEditBox* edit = (cglsEditBox* )m_listValue.getp(i);

		if (edit==0)
			continue;

		edit->Move((xOffset>>1)+m_fEdge+rect.left+maxw+nmargin, sum+2, (eGlsItemAlign)(eGIA_LEFT|eGIA_BOTTOM));
		edit->ReSize(rect.right-maxw-GetFontHeight(True)*2, edit->m_h);
		edit->Show();


	}
	
	// 大会名
	SetHeaderValue();

	//m_h=sum+rect.top+m_fEdge;

	ReSize(m_w, sum+(rect.top+m_fEdge));
	cGlassInterface::Update(eGUM_FORCED);

	//ReSize(m_w, sum+rect.top);
	InvalidateRect(0, 0, True);
}


void cglsInformationDlg::Update(Bitmap* bmp, RECT* rect, RECT* _childRect, int offsetX, int offsetY, Bool bIsLocalBufferUpdate)
{
	xTRACE("cglsInformationDlg::Update(%x) Enter", bmp);

	__super::Update(bmp, rect, _childRect, offsetX, offsetY, bIsLocalBufferUpdate);
	xTRACE("cglsInformationDlg::Update(%x) Leave", bmp);
}

// ------------------------------------
LRESULT cglsInformationDlg::OnCommand(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	if (wParam==eGIDID_TXT_RESULT)
	{
		cglsResult* result = (cglsResult*)((cGlsAssistant*)lParam);
		unsigned int n = result->Get();
		unsigned int winer=(n>>24);

		if (winer==0x01)
			SET_BLACK_WIN(m_header->flag32);
		else if (winer==0x02)
			SET_WHITE_WIN(m_header->flag32);
		else if (winer==0x03)
			SET_DRAW(m_header->flag32);
		else 
			SET_CANCELED(m_header->flag32);

		SET_SCORE(m_header->flag32, 0xFF&n);
	}
	else if (wParam==eGIDID_TXT_LIMITTIME)
	{
		cglsWatch* wat = (cglsWatch*)((cGlsAssistant*)lParam);
		
		unsigned int time;
		unsigned int sec;
		wat->GetResult(time, sec);

		SET_PLAY_TIME(m_header->condition, ((xHOUR(time)*60)+(xMINUTE(time))));
		SET_SECOND_READ_COUNT(m_header->condition2, (sec>>24));
		SET_SECOND_READ2(m_header->condition, (sec&0xFF));

	}
	else if (wParam==eGIDID_TXT_PLACE)
	{
		char buffer[256];
		if (m_header->pszPlace)
			yifree(m_header->pszPlace);

		cglsEditBox* edit = (cglsEditBox*)GetChild(eGIDID_TXT_PLACE);

		buffer[0]=0;
		edit->GetText(buffer, 256);
		m_header->pszPlace = yistrheap(buffer);
	}
	else if (wParam==eGIDID_TXT_HANDICAP)
	{
		unsigned int result=0, result2=0;
		cglsKomi* komi = (cglsKomi*)((cGlsAssistant*)lParam);

		KOMI_CLR(m_header->condition);
		REVERSE_KOMI_CLR(m_header->condition);

		komi->GetResult(result, result2);

		int score = abs(result);
		SET_KOMI(m_header->condition, score);

		if (result2)
		{
			SET_HALF_SCORE(m_header->condition);
		}

		if (result<0)
		{
			SET_REVERSE_KOMI(m_header->condition);
		}

		// コミが変更されたら、結果も更新する。
		cglsEditBox* edit = (cglsEditBox*)GetChild(eGIDID_TXT_RESULT);
		cglsResult* x = (cglsResult*)edit->GetAssistant();
		
		int n = x->Get();
		x->Set(n, HALF_SCORE(m_header->condition));
		
		wchar_t xxxxx[32];
		x->GetResult(xxxxx);
		edit->SetText(xxxxx);
	}
	else if (wParam==eGTRID_TXT_TEAI)
	{
		unsigned int result=0, result2=0;
		cglsListMenu* menu = (cglsListMenu*)((cGlsAssistant*)lParam);
		menu->GetResult(result, result2);

		if (result==1)
		{
			SET_NIGIRI(m_header->condition);
		}
		else if (result!=1)
		{
			SET_HANDICAP(m_header->condition);
			SET_INIT_STONE_CLR(m_header->condition);
			if (result>3)
			{
				SET_INIT_STONE(m_header->condition, (result-1));
			}
		}
	}
	else
	{
		return 0;
	}
	
	PostMessage(m_hNotifyWnd, WM_GO_HEADER_CHANGED, 0, 0);
	return 0;
}