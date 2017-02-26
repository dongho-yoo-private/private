#include "cydlgx.h"
#include "cyctrlxII.h"
#include "yiList.h"
#include "yiResource.h"
#include "cyctrlResource.h"
#include "yiGlassInterfaceDraw.h"

Bitmap* xMakeDialogShadow(int _w, int _h);


cyiList* g_DialogBoxList=0;
HandleMutex g_hMutex=0;

// ----------------------------------
static void xRegisterDailog(int id, cydlgx* dlg)
{
	int param[2] = {id, (int)dlg};

	if (g_DialogBoxList==0)
	{
		g_DialogBoxList = new cyiList(True);
	}

	g_DialogBoxList->lock();
	g_DialogBoxList->addref(yitoheap(param, sizeof(param)));
	g_DialogBoxList->unlock();
}

// ----------------------------------
static cydlgx* xGetDialog(int id)
{
	if (g_DialogBoxList==0)
		return 0;
	g_DialogBoxList->lock();

	for (cyiNode* pNode = g_DialogBoxList->GetNodeTopFrom(0); pNode!=0; pNode=pNode->np)
	{
		int* param = (int*)pNode->ref;
		
		if (param[0]==id)
		{
			g_DialogBoxList->unlock();
			return (cydlgx*)param[1];
		}
	}
	g_DialogBoxList->unlock();
	return 0;
}

// ----------------------------------
int yiMessageBox(cyctrlbase* base, const wchar_t* pszMessage, int buttons, cydlgx** out_dlg, HWND hWndLoop)
{
	cydlgx* dlg = new cydlgx();

	*out_dlg = dlg;
	int n = dlg->DoModal(base, pszMessage, buttons,hWndLoop);

	delete dlg;

	return n;
}

// ----------------------------------
int yiMessageBox(HWND hWnd /*cyctrlbase* base*/, int x, int y, const wchar_t* pszMessage, int buttons)
{
	cydlgx* dlg = new cydlgx();

	int n = dlg->DoModal(hWnd/*base*/, x, y, 0, 0, pszMessage, buttons);

	delete dlg;

	return n;
}


#define WM_YOTSUYA_DLG_END WM_USER+1

cydlgx::cydlgx(void)
: cyctrlbase()
{
	m_hWnd=0;
	m_pBg=0;
	m_pMask=0;
	m_pShadow=0;
	m_pBtns=0;
	m_pStr=0;
	m_bIsModal=False;
	m_bIsMessageBox=False;
	memset(m_uiz, 0,sizeof(m_uiz));
}

cydlgx::~cydlgx(void)
{
	if (m_pBg)
		delete m_pBg;

	if (m_pMask)
		delete m_pMask;

	if (m_pShadow)
		delete m_pShadow;

	if (m_pBtns)
		delete m_pBtns;

	if (m_pStr)
		delete m_pStr;
}

#define DEFAULT_DLG_WIDTH 1
#define DEFAULT_DLG_HEIGHT 1

// ----------------------------------
Bool cydlgx::Create(HWND hWndParent, int wMargin, int hMargin)
{
	if (cyctrlbase::Create(hWndParent, 0, 0, 0, 0, DEFAULT_DLG_WIDTH, DEFAULT_DLG_HEIGHT, False)==False)
	{
		return False;
	}

	m_wMargin=wMargin;
	m_hMargin=hMargin;
	return True;
}

// ----------------------------------
//Bool cydlgx::AddCtrlx(cyctrlx* ctrl, int x, int y, eCtrlxItemOffset offset/*eTOP_LEFT*/)
//{
//	ctrl->Move(x, y);
//	m_ctrlxs.addref(ctrl);
//
//	return True;
//}



#define DEFAULT_DLG_WMARGIN 10
#define DEFAULT_DLG_HMARGIN 10

//// ----------------------------------
//Bool xDrawDefaultButton(int id, Graphics* g, Font* fnt, sControlx* x, eControlxEvent xevent)
//{
//	return True;
//}

// ----------------------------------
int cydlgx::DoModal()
{
	MSG msg;
	HWND hWnd = m_hWndParent==0 ? m_hWnd: m_hWndParent;

	::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW );
	Show();
	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);

	while(GetMessage(&msg,/*m_bIsModal==True?m_hWnd:*/NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message==WM_YOTSUYA_DLG_END)
		{
			break;
		}
	}

	if (msg.message==WM_QUIT)
	{
		PostQuitMessage(0);
	}

	Hide();

	if (m_id!=0)
	{
		//xRegisterDialog(id, this);
	}

	//yiMutexUnlock(g_hMutex);

	return m_nResult;
}


#define DEFAULT_BUTTON_OFFSET 60
#define MESSAGE_BOX_MARGIN_X 20
#define MESSAGE_BOX_MARGIN_Y 20

// --------------------------------------------
void cydlgx::ChangeBackground(Bitmap* bg)
{
	if (m_bIsMessageBox)
	{
		OnDrawMessageBox(bg);
		m_pBtns->Update(True);
	}
	else
	{
		OnDrawDialog(bg);
		UpdateUiz();
	}
	InvalidateRect(m_hWnd, 0, FALSE);
}

#include "yiTime.h"
// --------------------------------------------
void cydlgx::OnDrawMessageBox(Bitmap* bg)
{
	int w=bg->GetWidth();
	int h=bg->GetHeight();
	int txtw=m_pStr->GetWidth();
	int txth=m_pStr->GetHeight();

	Bitmap* pBlurBg = cyiBitmapBlender::GaussBlureXX(bg, 5.0, 1.0, &cyiShapeMask(DIALOG_SHADOW_WIDTH-3, DIALOG_SHADOW_WIDTH-3, m_pMask));
	Graphics g(m_OffBmp);

	g.DrawImage(bg, 0, 0);
	g.DrawImage(m_pShadow, 0, 0);
	g.DrawImage(pBlurBg, 0, 0);

	g.DrawImage(m_pBg,DIALOG_SHADOW_WIDTH-3, DIALOG_SHADOW_WIDTH-3);
	g.DrawImage(m_pStr, (w-txtw)/2, (h+DIALOG_SHADOW_WIDTH-DEFAULT_BUTTON_OFFSET-txth)/2);
}

#define DIALOG_DEFAULT_MARGIN 10

// --------------------------------------------
void cydlgx::OnDrawDialog(Bitmap* bg)
{
	int w=bg->GetWidth();
	int h=bg->GetHeight();
	int txtw=m_pStr->GetWidth();
	int txth=m_pStr->GetHeight();

	Bitmap* pBlurBg = cyiBitmapBlender::GaussBlureXX(bg, 5.0, 1.0, &cyiShapeMask(DIALOG_SHADOW_WIDTH-3, DIALOG_SHADOW_WIDTH-3, m_pMask));
	Graphics g(m_OffBmp);

	g.DrawImage(bg, 0, 0);
	g.DrawImage(m_pShadow, 0, 0);
	g.DrawImage(pBlurBg, 0, 0);

	g.DrawImage(m_pBg,DIALOG_SHADOW_WIDTH-3, DIALOG_SHADOW_WIDTH-3);
	g.DrawImage(m_pStr, DIALOG_DEFAULT_MARGIN, DIALOG_DEFAULT_MARGIN);

	m_nItemStartY = m_pStr->GetHeight()+DIALOG_DEFAULT_MARGIN;

}

// --------------------------------------------
int cydlgx::DoModal(cyctrlbase* base, const wchar_t* pszCaption, int w, int h, Bool bIsModal)
{
	if (Create(base->m_hWnd, w, h)==False)
	{
		return -1;
	}

	m_bIsModal=bIsModal;
	ReSize(w, h);

	int x = (base->m_w-w)>>1;
	int y = (base->m_h-h)>>1;

	Move(x, y);


	m_hWndParent = bIsModal==True? m_hWnd:base->m_hWnd;

	// caption.
	m_pStr	= cyiShape::BitmapString(0, pszCaption, (Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_DIALOG_CAPTION_FONT), &SolidBrush(Color::White), eSTS_NONE);//, Color::White, 4.0, 5.0);
	m_pBg	= cyiGlassInterfaceDraw::DrawBackground(w, h, DIALOG_SHADOW_WIDTH, 3, 5.0, 1.0);
	m_pMask = cyiShape::FillRoundRect(&SolidBrush(Color::Black), RectF(0, 0, w-(DIALOG_SHADOW_WIDTH<<1), h-(DIALOG_SHADOW_WIDTH<<1)), 5.0);
	m_pShadow = cyiGlassInterfaceDraw::GetShadowBitmap(eRBID_SHADOW_10, w, h, &cyiShapeMask(DIALOG_SHADOW_WIDTH+3, DIALOG_SHADOW_WIDTH+3, m_pMask, True));

	if (m_OffBmp)
	{
		delete m_OffBmp;
	}
	m_OffBmp= new Bitmap(m_w, m_h);
	Bitmap*bg = base->GetBackGround(x, y, w, h);
	OnDrawDialog(bg);
	delete bg;

	//m_uiz.Create(this, 0, DIALOG_DEFAULT_MARGIN, m_nItemStartY, 5, 5, True, 0, True);
	OnDefineUiz(0, w, h);

	//int uizw, uizh;
	//m_uiz.ArrangeItems(uizw, uizh);
	UpdateUiz();

	OnUiInitializeEnd();

	base->m_bShowDialog=True;
	int res = DoModal();
	base->m_bShowDialog=False;

	return res;
}

// ----------------------------------
void cydlgx::SetUiz(int index, cyctrlbase* ui)
{
	if (m_uiz[index])
	{
		if (m_uiz[index]==ui)
		{
			return ;
		}
		cyctrlbase* prev=m_uiz[index];
		prev->Hide();
		m_uiz[index]=ui;
		m_uiz[index]->Move(prev->m_x, prev->m_y);
		m_uiz[index]->Show();
		return ;
	}
	
	m_uiz[index]=ui;
}

// --------------------------------------------
void  cydlgx::UpdateUiz(int index)
{
	int x = DIALOG_DEFAULT_MARGIN;
	int y = m_nItemStartY;
	//m_uiz.Move(DIALOG_DEFAULT_MARGIN, m_nItemStartY);
	//m_uiz.Update(True);
	//for (cyiNode* p = m_ctrlxs.GetNodeTopFrom(0); p!=0; p=p->np)
	//{
	//	cyctrlxII* ctrlx = (cyctrlxII*)p->ref;
	//	ctrlx->Update(True);
	//}

	if (index==-1)
	{
		for (int i=0; i<MAX_UI_COUNT; i++)
		{
			if (m_uiz[i]==0)
				break;
			m_uiz[i]->Move(x, y);
			//::InvalidateRect(m_uiz[i]->m_hWnd, NULL, False);
			m_uiz[i]->Show();

			//UpdateWindow(m_uiz[i]->m_hWnd);
			y+=m_uiz[i]->m_h;
		}
		return ;
	}
}

// --------------------------------------------
int cydlgx::DoModal(cyctrlbase* base, const wchar_t* pszMessage, int DefaultButton, HWND hWndLoop)
{
	MSG msg;
	int x, y, w, h;

	if (Create(hWndLoop, DEFAULT_DLG_WMARGIN, DEFAULT_DLG_HMARGIN)==False)
	{
		return -1;
	}

	m_hWndParent=hWndLoop;

	// message box
	if (pszMessage!=0)
	{
		m_bIsMessageBox=True;

		int txtw=0, txth=0;
		int btnw, btnh;
		
		//cyiShape::GetTextSize((Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_DIALOG_FONT), pszMessage, txtw, txth);
		m_pStr = cyiShape::BitmapString(0, pszMessage, (Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_DIALOG_FONT), &SolidBrush(Color::Black), eSTS_GROW, 0xFFFFFFFF, 3.0, 5.0);

		txtw=m_pStr->GetWidth();
		txth=m_pStr->GetHeight();

		w = txtw+(MESSAGE_BOX_MARGIN_X<<1);
		h = txth + (MESSAGE_BOX_MARGIN_Y<<1)+DEFAULT_BUTTON_OFFSET;

		// ここでサイズが分かる。

		int x = ((base->m_w-w)>>1)+base->m_x;
		int y = ((base->m_h-h)>>1)+base->m_y;

		Move(x, y);
		ReSize(w, h);

		m_pBg = cyiGlassInterfaceDraw::DrawBackground(w, h, DIALOG_SHADOW_WIDTH, 3, 5.0, 1.0);
		m_pMask = cyiShape::FillRoundRect(&SolidBrush(Color::Black), RectF(0, 0, w-(DIALOG_SHADOW_WIDTH<<1), h-(DIALOG_SHADOW_WIDTH<<1)), 5.0);
		m_pShadow = cyiGlassInterfaceDraw::GetShadowBitmap(eRBID_SHADOW_10, w, h, &cyiShapeMask(DIALOG_SHADOW_WIDTH+3, DIALOG_SHADOW_WIDTH+3, m_pMask, True));
		Bitmap* pBlurBg;

		if (m_OffBmp)
		{
			delete m_OffBmp;
		}
		m_OffBmp= new Bitmap(m_w, m_h);


		Bitmap* bg;
		if (base!=0)
		{
			bg = base->GetBackGround(x-base->m_x, y-base->m_y, w, h);
		}
		else
		{
			HDC hBgDC = GetDC(::GetDesktopWindow());

			POINT pt = {x, y};
			ClientToScreen(m_hWnd, &pt);
			bg = cyiBitmapBlender::CreateBitmap(hBgDC, pt.x, pt.y, w, h);

			ReleaseDC(::GetDesktopWindow(), hBgDC);
		}

		OnDrawMessageBox(bg);
		
		delete bg;
		//delete pBlureBg;

		//m_OffBmp->blt(DIALOG_SHADOW_WIDTH-3, DIALOG_SHADOW_WIDTH-3, pBg);
		//m_OffBmp->blt((w-txtw)/2, (h-DEFAULT_BUTTON_OFFSET-txth)/2, pStr);

		cyctrlxII* ctrl = new cyctrlxII();
		ctrl->Create(this, 0, 0, 0, 5, 5, True, (Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_BUTTON_FONT));

		if (DefaultButton==MB_OK)
		{
			ctrl->Add(IDOK, 0, L"確認", 0, 10, 5, 0);
		}
		else
		{
			ctrl->Add(IDYES, 0, L"はい", 0, 10, 5, 0);
			ctrl->Add(IDNO, 0, L"いいえ", 0, 10, 5, 0);
		}

		ctrl->Update(False);
		btnw = ctrl->m_w;
		btnh = ctrl->m_h;

		ctrl->Move((m_w-btnw)>>1, m_h-DEFAULT_BUTTON_OFFSET);
		ctrl->Update(True);
		m_pBtns=ctrl;
	}

	//Move(x,y);
	//ReSize(w, h);

	//m_id = id;

	return DoModal();
}

// ----------------------------------
void cydlgx::UseDefaultButton(eDlgButtonSet buttonSet)
{
	cyctrlxII* ctrl = new cyctrlxII();
	ctrl->Create(this, 0, 0, 0, 5, 5, True, (Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_BUTTON_FONT));
	int btnw, btnh;

	if (buttonSet==eDlgButtonSet::eDBS_OK)
	{
		ctrl->Add(IDOK, 0, L"確認", 0, 10, 5, 0);
	}
	else if (buttonSet==eDlgButtonSet::eDBS_OKCANCEL)
	{
		ctrl->Add(IDOK, 0, L"確認", 0, 10, 5, 0);
		ctrl->Add(IDCANCEL, 0, L"取消", 0, 10, 5, 0);
	}
	else if (buttonSet==eDlgButtonSet::eDBS_YESNO)
	{
		ctrl->Add(IDYES, 0, L"はい", 0, 10, 5, 0);
		ctrl->Add(IDNO, 0, L"いいえ", 0, 10, 5, 0);
	}

	ctrl->ArrangeItems(btnw, btnh);
	/*btnw = ctrl->m_w;
	btnh = ctrl->m_h;*/

	ctrl->Move((m_w-btnw)>>1, m_h-DEFAULT_BUTTON_OFFSET);
	ctrl->Update(True);
	m_pBtns=ctrl;
}

// ----------------------------------
//int cydlgx::DoModal(cyctrlbase* base, int x, int y, int w, int h, const wchar_t* pszMessage, int DefaultButton)
int cydlgx::DoModal(HWND hWndParent, int x, int y, int w, int h, const wchar_t* pszMessage, int DefaultButton)
{
	MSG msg;

	if (Create(hWndParent, DEFAULT_DLG_WMARGIN, DEFAULT_DLG_HMARGIN)==False)
	{
		return -1;
	}

	m_hWndParent=hWndParent;

	// message box
	if (pszMessage!=0)
	{
		int txtw=0, txth=0;
		int btnw, btnh;
		
		//cyiShape::GetTextSize((Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_DIALOG_FONT), pszMessage, txtw, txth);
		Bitmap* pStr = cyiShape::BitmapString(0, pszMessage, (Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_DIALOG_FONT), &SolidBrush(Color::Black), eSTS_GROW, 0xFFFFFFFF, 3.0, 5.0);

		txtw=pStr->GetWidth();
		txth=pStr->GetHeight();

		w = txtw+(MESSAGE_BOX_MARGIN_X<<1);
		h = txth + (MESSAGE_BOX_MARGIN_Y<<1)+DEFAULT_BUTTON_OFFSET;
		//m_OffBmp = new cyiSurface(w, h);

		Bitmap* pBg = cyiGlassInterfaceDraw::DrawBackground(w, h, DIALOG_SHADOW_WIDTH, 3, 5.0, 1.0);
		Bitmap* pMask = cyiShape::FillRoundRect(&SolidBrush(Color::Black), RectF(0, 0, w-(DIALOG_SHADOW_WIDTH<<1), h-(DIALOG_SHADOW_WIDTH<<1)), 5.0);
		Bitmap* pShadow = cyiGlassInterfaceDraw::GetShadowBitmap(eRBID_SHADOW_10, w, h, &cyiShapeMask(DIALOG_SHADOW_WIDTH+3, DIALOG_SHADOW_WIDTH+3, pMask, True));
		Bitmap* pBlurBg;

		ReSize(w, h);

		if (m_OffBmp)
		{
			delete m_OffBmp;
		}
		m_OffBmp= new Bitmap(m_w, m_h);


		//if (base!=0)
		//{
		//	//base->GetBackGround(
		//}
		//else
		//{
			HDC hBgDC = GetDC(::GetDesktopWindow());

			POINT pt = {x, y};
			ClientToScreen(m_hWnd, &pt);
			Bitmap* bg = cyiBitmapBlender::CreateBitmap(hBgDC, pt.x, pt.y, w, h);
			pBlurBg = cyiBitmapBlender::GaussBlureXX(bg, 5.0, 1.0, &cyiShapeMask(DIALOG_SHADOW_WIDTH-3, DIALOG_SHADOW_WIDTH-3, pMask));

			Graphics g(m_OffBmp);

			g.DrawImage(bg, 0, 0);
			g.DrawImage(pShadow, 0, 0);
			g.DrawImage(pBlurBg, 0, 0);

			//m_OffBmp->blt(0, 0, bg);
			//m_OffBmp->blt(0, 0, pShadow);
			//m_OffBmp->blt(0, 0, pBlurBg);
		//}

		g.DrawImage(pBg,DIALOG_SHADOW_WIDTH-3, DIALOG_SHADOW_WIDTH-3);
		g.DrawImage(pStr, (w-txtw)/2, (h+DIALOG_SHADOW_WIDTH-DEFAULT_BUTTON_OFFSET-txth)/2);
		//m_OffBmp->blt(DIALOG_SHADOW_WIDTH-3, DIALOG_SHADOW_WIDTH-3, pBg);
		//m_OffBmp->blt((w-txtw)/2, (h-DEFAULT_BUTTON_OFFSET-txth)/2, pStr);

		cyctrlxII* ctrl = new cyctrlxII();
		ctrl->Create(this, 0, 0, 0, 5, 5, True, (Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_BUTTON_FONT));

		if (DefaultButton==MB_OK)
		{
			ctrl->Add(IDOK, 0, L"確認", 0, 10, 5, 0);
		}
		else
		{
			ctrl->Add(IDYES, 0, L"はい", 0, 10, 5, 0);
			ctrl->Add(IDNO, 0, L"いいえ", 0, 10, 5, 0);
		}

		ctrl->Update(False);
		btnw = ctrl->m_w;
		btnh = ctrl->m_h;

		ctrl->Move((m_w-btnw)>>1, m_h-DEFAULT_BUTTON_OFFSET);
		ctrl->Update(True);
	}

	Move(x,y);
	//ReSize(w, h);

	//m_id = id;

	return DoModal();
}

// ---------------------------------------
BOOL cydlgx::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	//cyctrlbase::OnPaint(hdc, ps);
	if (m_OffBmp)
	{
		Graphics g(hdc);
		g.DrawImage(m_OffBmp, 0, 0);//ps->rcPaint.left, ps->rcPaint.top, ps->rcPaint.right, ps->rcPaint.bottom);
		//m_OffBmp->Draw(hdc, ps->rcPaint.left, ps->rcPaint.top, ps->rcPaint.right, ps->rcPaint.bottom);
	}
	return TRUE;
}

// ---------------------------------------
LRESULT cydlgx::OnSize(WPARAM wp, LPARAM lp, BOOL& handled)
{
	return 0;
}

// ---------------------------------------
void cydlgx::OnIDOK()
{
	PostMessage(m_hWndParent, WM_YOTSUYA_DLG_END, 0, 0);
	m_nResult=IDOK;
}

// ---------------------------------------
void cydlgx::OnIDCancel()
{
	PostMessage(m_hWndParent, WM_YOTSUYA_DLG_END, 0, 0);
	m_nResult=IDCANCEL;
}

// ---------------------------------------
LRESULT cydlgx::OnIdOk(HWND hWnd, WPARAM wp)
{
	OnIDOK();
	return 0;
}

// ---------------------------------------
LRESULT cydlgx::OnIdCancel(HWND hWnd, WPARAM wp)
{
	OnIDCancel();
	return 0;
}

// ------------------------------------
Bool cydlgx::OnDefButtonDraw(Bitmap* srf, Bitmap* bg, Font* fnt, sControlxII* s, eControlxEventII eventId, int resId)
{
	if (eventId==eCEII_NORMAL ||
			eventId==eCEII_INIT ||
				eventId==eCEII_MOUSE_LUP ||
					eventId==eCEII_MOUSE_LEAVE)
	{
		if (s->pBmp[0]==0)
		{
			s->pBmp[0] = (Bitmap*)cyiResource::GetInstance(eResourceTypex::eRTX_IMAGE_CACHED)->Get(resId);
		}

		if (s->pBmp[0]==0)
		{
			s->pBmp[0] = cyiGlassInterfaceDraw::DrawDefaultButton(s->bmp, s->name, &s->rect, fnt);
			cyiResource::GetInstance(eResourceTypex::eRTX_IMAGE_CACHED)->Register(resId, s->pBmp[0], 0);
		}

		cyctrlbase::OnCtrlxDraw(0, 0, srf, bg, fnt, s, eventId);
		cyiBitmapBlender::RotateCopy(srf, s->pBmp[0], 0, s->rect.left, s->rect.top,0, 0, s->pBmp[0]->GetWidth(), s->pBmp[0]->GetHeight());
		//srf->blt(s->rect.left, s->rect.top, s->pBmp[0], 0, 0, s->pBmp[0]->GetWidth(), s->pBmp[0]->GetHeight());
		return True;
	}
	else if (eventId==eCEII_DISABLE)
	{
		if (s->pBmp[1]==0)
		{
			s->pBmp[1] = (Bitmap*)cyiResource::GetInstance(eResourceTypex::eRTX_IMAGE_CACHED)->Get(resId+1);
		}

		if (s->pBmp[1]==0)
		{
			s->pBmp[1] = cyiGlassInterfaceDraw::DrawDefaultButtonDisable(s->bmp, s->name, &s->rect, fnt);
			cyiResource::GetInstance(eResourceTypex::eRTX_IMAGE_CACHED)->Register(resId+1, s->pBmp[1], 0);
		}

		cyctrlbase::OnCtrlxDraw(0, 0, srf, bg, fnt, s, eventId);
		if (s->bIsEnable==True)
		{
			cyiBitmapBlender::RotateCopy(srf,  s->pBmp[0], 0, s->rect.left, s->rect.top,0, 0, s->pBmp[0]->GetWidth(), s->pBmp[0]->GetHeight());
			//srf->blt(s->rect.left, s->rect.top, s->pBmp[0], 0, 0, s->rect.right, s->rect.bottom);
			return True;
		}
		cyiBitmapBlender::RotateCopy(srf,s->pBmp[1], 0, s->rect.left, s->rect.top,0, 0, s->pBmp[1]->GetWidth(), s->pBmp[1]->GetHeight());
		//srf->blt(s->rect.left, s->rect.top, s->pBmp[1], 0, 0, s->rect.right, s->rect.bottom);
		return True;
	}
	else if (eventId==eCEII_MOUSE_LDOWN)
	{
		if (s->pBmp[2]==0)
		{
			s->pBmp[2] = (Bitmap*)cyiResource::GetInstance(eResourceTypex::eRTX_IMAGE_CACHED)->Get(resId+2);
		}

		if (s->pBmp[2]==0)
		{
			s->pBmp[2] = cyiGlassInterfaceDraw::DrawDefaultButton(s->bmp, s->name, &s->rect, fnt, Color::Gray);
			cyiResource::GetInstance(eResourceTypex::eRTX_IMAGE_CACHED)->Register(resId+2, s->pBmp[2], 0);
		}
		cyctrlbase::OnCtrlxDraw(0, 0, srf, bg, fnt, s, eventId);
		cyiBitmapBlender::RotateCopy(srf, s->pBmp[2], 0, s->rect.left, s->rect.top,0, 0, s->pBmp[2]->GetWidth(), s->pBmp[2]->GetHeight());
		//srf->blt(s->rect.left, s->rect.top, s->pBmp[2], 0, 0, s->rect.right, s->rect.bottom);
		return True;
	}
	else if (eventId==eCEII_MOUSE_IN)
	{
		if (s->pBmp[3]==0)
		{
			s->pBmp[3] = (Bitmap*)cyiResource::GetInstance(eResourceTypex::eRTX_IMAGE_CACHED)->Get(resId+3);
		}

		if (s->pBmp[3]==0)
		{
			s->pBmp[3] = cyiGlassInterfaceDraw::DrawDefaultButton(s->bmp, s->name, &s->rect, fnt, Color::Blue);
			cyiResource::GetInstance(eResourceTypex::eRTX_IMAGE_CACHED)->Register(resId+3, s->pBmp[3], 0);
		}
		cyctrlbase::OnCtrlxDraw(0, 0, srf, bg, fnt, s, eventId);
		cyiBitmapBlender::RotateCopy(srf,s->pBmp[3], 0, s->rect.left, s->rect.top,0, 0, s->pBmp[3]->GetWidth(), s->pBmp[3]->GetHeight());
		//srf->blt(s->rect.left, s->rect.top, s->pBmp[3], 0, 0, s->rect.right, s->rect.bottom);
		return True;
	}

	return False;
}

// ------------------------------------
Bool cydlgx::OnCtrlxDraw(int id, int subId, Bitmap* srf, Bitmap* bg, Font* fnt, sControlxII* s, eControlxEventII eventId)
{
	if (id==0) // message box
	{
		if (subId==IDOK)
		{
			return OnDefButtonDraw(srf, bg, fnt, s, eventId, eRBID_BTN_OK_INIT);
		}
		else if (subId==IDCANCEL)
		{
			return OnDefButtonDraw(srf, bg, fnt, s, eventId, eRBID_BTN_CANCEL_INIT);
		}
		else if (subId==IDYES)
		{
			return OnDefButtonDraw(srf, bg, fnt, s, eventId, eRBID_BTN_YES_INIT);
		}
		else if (subId==IDNO)
		{
			return OnDefButtonDraw(srf, bg, fnt, s, eventId, eRBID_BTN_NO_INIT);
		}

		return False;
	}
	return False;
}

// ------------------------------------
Bool cydlgx::OnCtrlxBgDraw(int id, Bitmap* srf, int x, int y)
{
	return cyctrlbase::OnCtrlxBgDraw(id, srf, x, y);
}