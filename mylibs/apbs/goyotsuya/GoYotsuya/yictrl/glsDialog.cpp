#include "glsDialog.h"



cglsDialog::cglsDialog(void)
: m_bIsExitLoop(False)
, m_returnValue(-1)
, m_nContentsHeight(0)
, m_nContentsWidth(0)
, m_bIsSizeFixedMode(True)
, m_eButtonAlign(eGIA_CENTER)
, m_fMarginX(1.5)
, m_fMarginY(2.0)
, m_nButtonzId(0)

{
	__GLASSINTERFACE_NAME__;
	m_szCaption[0]=0;
	m_bIsModal=False;
}

cglsDialog::~cglsDialog(void)
{

	//cglsGaussPannel::~cglsGaussPannel();
}

// --------------------------------
void cglsDialog::xDefaultDetail(sGlsDialogDetail* detail)
{
	detail->bHasCaption=True;
	detail->bIsMovable=False;
	detail->bHasClosebutton=True;
	detail->font=0;
	cglsGaussPannel::xDefaultDetail(&detail->gauss);
	detail->bIsAutoDestroy=True;
	detail->bIsAnimationShow=True;
	detail->nShowAnimationTime=200;
	detail->nHideAnimationTime=300;
}

// -----------------------------------
void cglsDialog::xOnMessageLoop()
{
	MSG msg;
	int res=0;
	while((res=GetMessage(&msg, NULL, 0, 0))!=0)
	{
		if (msg.message==WM_MOUSEMOVE ||
			msg.message==WM_LBUTTONDOWN)
		{
			RECT rect;
			POINT pt={msg.wParam&0xFFFF, (msg.wParam>>16)&0xFFFF};
			ClientToScreen(msg.hwnd, &pt);
			GetWindowRect(m_hWnd, &rect);

			if (yiUtilPointIsInRect(&rect, pt.x, pt.y)==False)
			{
				continue;
			}
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (m_bIsExitLoop==True)
		{
			Sleep(100);
			while (PeekMessage(&msg,NULL, 0, 0, PM_REMOVE)==True)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return ;
		}

	}


	if (res==0)
	{
		PostQuitMessage(0);
	}
}

// -----------------------------------
int cglsDialog::DoModal(HWND hWndParent, int id, const wchar_t* pszCaption, int x, int y, int w, int h, sGlsDialogDetail* detail/*=0*/)
{
	m_bIsExitLoop=False;
	m_item.fTransparency=0.0;
	xON_GLS_FLAG(m_flag, eGF_PARENT_CENTER);

	m_bIsModal=True;

	if (Create(hWndParent, id, pszCaption, x, y, w, h, detail)==True)
	{
		//ZOrder(eZO_TOP);

		//if (this->m_bIsSizeFixedMode==False)
		//{
		//	ReArrangeItemz();
		//}

		//Show();
		if (xIS_GLS_FLAG(m_flag, eGF_ANIMATION_SHOW)==True)
		{
			sGlsAnimationEx ani;
			glsInitAnimationEx(&ani, &m_item);
			glsSetTrAnimationEx(&ani, 0.0, 1.0, eGAP_FISO);
			glsSetSzAnimationEx(&ani, 0.0, 1.0, eGAP_FISO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));
			AnimationShow(True, &ani, 15, m_nShowAnimationTime);

		}
		else
		{
			Show();
		}

		xOnMessageLoop();

		OnExitLoop();

		//if (detail->bIsAutoDestroy==True)
		//{
		//	if (xIS_GLS_FLAG(m_flag, eGF_ANIMATION_SHOW)==True)
		//	{
		//		m_bIsRequestDestroy=True;
		//		AnimationShow(False, m_nHideAnimationTime);

		//		m_bIsExitLoop=False;
		//		xOnMessageLoop();
		//	}
		//	Destroy();
		//}
		//else
		//{
		//	AnimationShow(False, m_nHideAnimationTime);
		//	//Hide();
		//}

		return m_returnValue;
	}

	return -1;
}

// ------------------------------------
void cglsDialog::OnReqDestroy()
{
	m_bIsExitLoop=True;
}

// -----------------------------
void cglsDialog::OnShowAnimationEnd(Bool bIsShow, Bool bIsCanceled)
{
	if (m_bIsModal==True)
	{
		if (bIsShow==False)
		{
		//	//MessageBox(0, "xxxx", "XXXXXX", MB_OK);
			m_bIsExitLoop=True;
		//	InvalidateRect(0, 0, True);
		//	return ;
		}
	}
	else
	{
		if (bIsShow==False)
		{
			Hide();
	
			if (m_bIsRequestDestroy==True)
			{
				Destroy();
			}
		}
	}
}

// -----------------------------------
Bool cglsDialog::Create(HWND _hWndParent, int id, const wchar_t* pszCaption, int x, int y, int w, int h, sGlsDialogDetail* detail/*=0*/)
{
	HWND hWndParent = IsWindow(_hWndParent)?_hWndParent:((cGlassInterface*)_hWndParent)->m_hWnd;
	sGlsDialogDetail xdetail;
	xSET_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW, True);
	
	cGlassInterface* parent = (cGlassInterface*)_hWndParent;

	if (detail)
	{
		xdetail=*detail;
	}
	else
	{
		xDefaultDetail(&xdetail);
	}

	if (parent)
	{
		cglsSystem* system = parent->GetSystem();
		if (system)
		{
			int n = system->GetPerformance();

			if (n==0)
			{
				xdetail.gauss.fBlureRad=2.0;
			}
			else if (n==1)
			{
				xdetail.gauss.fBlureRad=3.0;
			}
			else if (n==2)
			{
				xdetail.gauss.fBlureRad=4.0;
			}
		}
	}

	SetGaussPannelDetail(&xdetail.gauss);

	if (m_hShadow==0)
	{
		m_hShadow=cyiShape::CreateShadow((int)xdetail.gauss.fShadowWidth, xdetail.gauss.fEdge, 0.7, 0xFF000000);
	}

	m_id=id;
	sGlsDetail zdetail;
	cGlassInterface::xDefaultDetail(&zdetail);
	zdetail.font=xdetail.font;
	zdetail.bIsFocusManageInterface=True;
	
	m_szCaption[0]=0;
	
	if (m_font==0 &&
			xdetail.font!=0)
	{
		m_font = (Font*)cyiShape::Clone(xdetail.font);
	}

	m_ptItemDistance.x=GetFontHeight(True)/2;
	m_ptItemDistance.y=GetFontHeight(True)/7;

	if (pszCaption)
		wcscpy(m_szCaption, pszCaption);

	cglsSystem* system=parent->GetSystem();

	zdetail.bIsAnimationShow=xdetail.bIsAnimationShow;
	zdetail.nShowAnimationTime=xdetail.nShowAnimationTime;
	zdetail.nHideAnimationTime=xdetail.nHideAnimationTime;

	if (system)
	{
		int n = system->GetPerformance();
		if (n<3)
		{
			zdetail.bIsAnimationShow=False;
		}
	}

	if (cGlassInterface::Create(_hWndParent, id, x, y, w, h, 0, /*WS_CLIPCHILDREN|*/WS_VISIBLE|WS_CHILD, &zdetail)==False)
	{
		return False;
	}


	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOREDRAW|SWP_NOMOVE|SWP_NOSIZE);

	if (m_glsDrawInterface)
		xON_GLS_FLAG(m_glsDrawInterface->m_flag, eGF_REQ_UPDATE_BUFFER);

	return True;
}

// -----------------------------------
void cglsDialog::Destroy()
{
	Hide();
	if (m_glsDrawInterface)
		xOFF_GLS_FLAG(m_glsDrawInterface->m_flag, eGF_REQ_UPDATE_BUFFER);

	__super::Destroy();

}
#include "glsImageBox.h"
#define xTYPEIS resources[cnt].type
#define xTAG resources[cnt]
// ------------------------------------
Bool cglsDialog::CreateChildFromResource(sGlsDialogResources* resources)
{
	int cnt=0;

	cGlassInterface* obj;
	while(resources[cnt].id!=0)
	{
		eGlassInterfaceType type = xTYPEIS;
		if (type==eGIT_BUTTON)
		{
			cglsButton* btn = new cglsButton();
			if (resources[cnt].param1==0)
			{
				btn->Create((HWND)this, resources[cnt].id, 0, 0, (Bitmap*)resources[cnt].bitmapId, resources[cnt].text);
			}
			else
			{
				sGlsButtonDetail detail;
				cglsButton::xDefaultDetail(&detail);
				detail.colorHilight=resources[cnt].param1;
				btn->Create((HWND)this, resources[cnt].id, 0, 0, (Bitmap*)resources[cnt].bitmapId, resources[cnt].text, 0, &detail);
			}
			btn->Update(eGUM_FORCED);
		}
		else if (type==eGIT_LABEL)
		{
			sGlsStaticDetail detail;
			cglsStatic::xDefaultDetail(&detail);
			detail.id=resources[cnt].id;

			cglsStatic* label = new cglsStatic();
			label->Create((HWND)this, 0, 0, 0, 0, 0, resources[cnt].text, &detail);
		}
		else if (type==eGIT_SPLIT_BUTTON)
		{
			sGlsSplitButtonDetail splitDetail;
			cglsSplitButton::xDefaultDetail(&splitDetail);

			cglsSplitButton* btn= new cglsSplitButton();

			if (xTAG.param1)
				splitDetail.colorHilight=Color::LightBlue;//xTAG.param1;
			btn->Create((HWND)this, xTAG.id, 0, 0, GetFontHeight(True)*xTAG.param2, 0, 0, &splitDetail);
			
			const wchar_t** pStrings = (const wchar_t**)xTAG.text;
			const wchar_t** pStringComments = (const wchar_t**)xTAG.bitmapId;
			int i=0;
			while(pStrings!=0 &&
					pStrings[i]!=0)
			{
				btn->AddItem(i+1, 0, pStrings[i], pStringComments==0?0:pStringComments[i]);
				i++;
			}
			btn->SelectIndex(0);
			btn->Update(eGUM_FORCED);
		}
		else if (type==eGIT_EDIT_BOX)
		{
			cglsEditBox* edit = new cglsEditBox();
			edit->Create((HWND)this, xTAG.id, 0, 0, GetFontHeight(True)*xTAG.param2, 0);

			if (xTAG.param1)
				edit->SetMaxLength(xTAG.param1);
			if (xTAG.bitmapId!=0)
			{
				edit->SetImeMode(eGTIM_IME_ALWAYS_OFF);
			}

			if (xTAG.text!=0)
			{
				edit->SetNumeric(True);
			}

			edit->Hide();
		}
		else if (type==eGIT_LISTBOX)
		{
			cglsListBox* listBox = new cglsListBox();
			listBox->Create((HWND)this, xTAG.id, 0, 0, GetFontHeight(True)*xTAG.param1, GetFontHeight(True)*xTAG.param2);

			if (xTAG.text)
			{
				int i=0;
				const wchar_t** p=(const wchar_t**)xTAG.text;

				while(p[i])
				{
					listBox->AddItem(i+1, 0, p[i]);
					i++;
				}
				listBox->Update(eGUM_FORCED);
			}
		}
		else if (type==eGIT_PICTURE)
		{
			cglsImageBox* picture = new cglsImageBox();
			picture->Create(this, xTAG.id, 0, 0, 0, 0, 0, 0);// False, &Pen(Color::Black));
		}
		else if (type==eGIT_RADIO)
		{
			cglsRadioGroup* radio = new cglsRadioGroup();
			radio->Create(this, xTAG.id, 0, 0);

			if (xTAG.text)
			{
				int i=0;
				const wchar_t** p=(const wchar_t**)xTAG.text;

				while(p[i])
				{
					radio->AddItem(i+1, p[i]);
					i++;
				}
			}
			radio->SetChecked(xTAG.param1);
			radio->Update(eGUM_FORCED);
			radio->Hide();
		}
		else if (type==eGIT_CHECK)
		{
			
			cglsCheckGroup* radio = new cglsCheckGroup();
			radio->Create(this, xTAG.id, 0, 0);

			if (xTAG.text)
			{
				int i=0;
				const wchar_t** p=(const wchar_t**)xTAG.text;

				while(p[i])
				{
					radio->AddItem(i+1, p[i]);
					i++;
				}
			}
			//radio->SetChecked(xTAG.param1);
			radio->Update(eGUM_FORCED);
			radio->Hide();
		}

		cGlassInterface* obj = GetChild(xTAG.id);

		if (obj==0)
			return False;

		if (xTAG.posFrom)
		{
			int x=0;
			int y=0;
			if (xTAG.margin&eIM_SMALL_X)
				x=GetFontHeight(True)/2;
			else if (xTAG.margin&eIM_X)
				x=GetFontHeight();
			else if (xTAG.margin&eIM_LARGE_X)
				x=GetFontHeight(True)<<1;

			if (xTAG.margin&eIM_SMALL_Y)
			{
				y=GetFontHeight(True)/5;
			}
			else if (xTAG.margin&eIM_Y)
			{
				y=GetFontHeight(True)/2;
			}
			else if (xTAG.margin&eIM_LARGE_Y)
			{
				y=GetFontHeight(True);
			}

			GetChild(resources[cnt].posFrom)->AddPositionLink(obj, resources[cnt].position, x, y);
		}
		obj->Hide();
		//obj->Hide();
		cnt++;
	}

	return True;
}


// -----------------------------------
void cglsDialog::Show()
{
	__super::Show();
	return ;
}

// -----------------------------------
void cglsDialog::Hide()
{
	__super::Hide();
	return ;
}

// -----------------------------------
LRESULT cglsDialog::OnMouseMove(WPARAM wp, LPARAM lp, BOOL& handled)
{
	return 0;
}

#include "glsButton.h"
// ---------------------------------------------------
//LRESULT cglsDialog::OnCreate(WPARAM wParam, LPARAM lParam)
//{
//	//cglsButton* btn = new cglsButton();
//
//	//btn->Create((HWND)this, 1, 50, 50, 0, L"ƒeƒXƒg");
//	//btn->Show();
//	//btn->Update(eGUM_FORCED);
//	//btn->Move(200, 10);
//	OnItemReArranged();
//	return 0;
//}

// ------------------------------------
Bool cglsDialog::ReArrangeItemz()
{
	OnItemReArranged();
	return True;
}

// ------------------------------------
cglsStatic* cglsDialog::CreateLabel(const wchar_t* str, sGlsStaticDetail* detail)
{
	cglsStatic* label = new cglsStatic();
	if (label->Create((HWND)this, 0, 0, 0, 0, 0, str, detail)==True)
	{
		return label;
	}
	return 0;
}


// ------------------------------------
int cglsDialog::SetFont(Font* font)
{
	int res = cGlassInterface::SetFont(font);

	//cGlassInterface::Update(eGUM_FORCED);
	ReArrangeItemz();

	return res;
}


#define DIALOG_ITEM_ID_BUTTON_PANEL 128
#define DIALOG_ITEM_ID_TOOL_BUTTON  129

// ------------------------------------
void cglsDialog::OnItemReArranged()
{
	//if (m_bIsSizeFixedMode==False)
	//{
	//	RECT rect;
	//	GetClientRect(&rect);

	//	cglsStackPannel* m_btnPannel = (cglsStackPannel*)GetChild(DIALOG_ITEM_ID_BUTTON_PANEL);
	//	m_btnPannel->Show();
	//	m_btnPannel->Update(eGUM_FORCED);
	//	m_btnPannel->Move((m_w-pannel->m_w)>>1, m_h-rect.top-pannel->m_h-(rect.top<<1));

	//	cglsToolButton* btn = (cglsToolButton*)GetChild(DIALOG_ITEM_ID_TOOL_BUTTON);
	//	btn->Move(m_w-rect.left, rect.top);

	//}
}

// ------------------------------------
LRESULT cglsDialog::OnSizeChanged(WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	//ReArrangeItemz();
	return 0;
}

// ------------------------------------
void cglsDialog::SetButtonText(int id, const wchar_t* text)
{
	cglsStackPannel* panel = (cglsStackPannel*)GetChild(DIALOG_ITEM_ID_BUTTON_PANEL);
	cglsButton* btn = (cglsButton* )panel->GetItem(id)->gls;

	btn->SetItem(0, text);
	btn->Update(eGUM_FORCED);
	
	panel->Update(eGUM_FORCED);
}


// ------------------------------------
void cglsDialog::EnableButton(int id, Bool bEnable)
{
	cglsStackPannel* panel = (cglsStackPannel*)GetChild(DIALOG_ITEM_ID_BUTTON_PANEL);
	panel->EnableItem(id, bEnable);
}

// ------------------------------------
int cglsDialog::AddButton(const wchar_t* name, int id)
{
	cglsStackPannel* panel = (cglsStackPannel*)GetChild(DIALOG_ITEM_ID_BUTTON_PANEL);
	if (panel==0)
	{
		panel = new cglsStackPannel();
		panel->Create((HWND)this, DIALOG_ITEM_ID_BUTTON_PANEL, 0, 0, 0);
	}

	cglsButton* btnOk = new cglsButton();
	btnOk->Create((HWND)panel, id, 0, 0, 0, name);
	btnOk->Show();
	panel->Add(btnOk, 0);

	return panel->GetItemCount();
}

// ------------------------------------
void cglsDialog::UpdateButton(eGlsItemAlign align, int distance)
{
	cglsStackPannel* panel = (cglsStackPannel*)GetChild(DIALOG_ITEM_ID_BUTTON_PANEL);
	if (panel==0)
		return ;

	RECT rect;
	GetClientRect(&rect);

	cglsToolButton* btn = new cglsToolButton();
	btn->Create(this,DIALOG_ITEM_ID_TOOL_BUTTON , m_w-rect.left, rect.top, 0, 0);
	btn->Add(eGTBT_CROSS, eGTBSS_FILL);

	if (m_bIsSizeFixedMode==True)
	{
		panel->Show();
		panel->Update(eGUM_FORCED);
		int x=(m_w-panel->m_w)>>1;

		if (align==eGIA_RIGHT)
		{
			x=(m_w-panel->m_w-rect.left-distance);
		}
		else if (align==eGIA_LEFT)
		{
			x=(rect.left+distance);
		}

		panel->Move(x, m_h-rect.top-panel->m_h-(rect.top<<1));
		m_eButtonAlign=align;
	}
}

// ------------------------------------
void cglsDialog::CreateButton(int id, eGlsItemAlign align, int distance)
{
	RECT rect;
	GetClientRect(&rect);

	m_nButtonzId=id;

	cglsStackPannel* m_btnPannel = new cglsStackPannel();
	m_btnPannel->Create((HWND)this, DIALOG_ITEM_ID_BUTTON_PANEL, 0, 0, 0);

	cglsToolButton* btn = new cglsToolButton();
	btn->Create(this,DIALOG_ITEM_ID_TOOL_BUTTON , m_w-rect.left, rect.top, 0, 0);
	btn->Add(eGTBT_CROSS, eGTBSS_FILL);

	{
		cglsButton* btnOk = new cglsButton();
		btnOk->Create((HWND)m_btnPannel, IDOK, 0, 0, 0, L"OK");
		btnOk->Show();
		m_btnPannel->Add(btnOk, 0);
	}

	if (id==MB_OKCANCEL)
	{
		cglsButton* btnCancel = new cglsButton();
		btnCancel->Create((HWND)m_btnPannel, IDCANCEL, 0, 0, 0, L"Cancel");
		btnCancel->Show();
		m_btnPannel->Add(btnCancel, 0);
	}

	if (m_bIsSizeFixedMode==True)
	{
		m_eButtonAlign=eGIA_CENTER;
		m_btnPannel->Show();
		m_btnPannel->Update(eGUM_FORCED);
		m_btnPannel->Move((m_w-m_btnPannel->m_w)>>1, m_h-rect.top-m_btnPannel->m_h-(rect.top<<1));
	}

}

// -----------------------------------
Bool cglsDialog::IsButtonOnlyOK()
{
	return m_nButtonzId==MB_OK;
}


// ---------------------------------------------------
LRESULT cglsDialog::OnCreate(WPARAM wParam, LPARAM lParam)
{
	if (m_bIsSizeFixedMode==False)
	{
		sGlsStaticDetail detail;
		cglsStatic::xDefaultDetail(&detail);
		detail.eStyle=eSTS_DROP_SHADOW;
		cglsStatic* caption = new cglsStatic();
		caption->Create((HWND)this, 0, 0, 0, 0, 0, m_szCaption, &detail);
		caption->Update(eGUM_FORCED);
		caption->Show();

		caption->Move((m_w-caption->m_w)>>1, GetFontHeight(True)*0.5);
	}
	return 0;
}

// ---------------------------------------------------
LRESULT cglsDialog::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	bIsHandled=True;

	if ((wParam&0xFFFF0000)==0)
	{
		m_returnValue=(wParam&0xFFFF);
	}

	if (m_returnValue==DIALOG_ITEM_ID_TOOL_BUTTON)
	{
		m_returnValue=IDCANCEL;
	}
	
	if (m_returnValue==IDOK ||
			m_returnValue==IDCANCEL)
	{
		if (m_returnValue==IDCANCEL)
		{
			m_returnValue=0;
		}
		if (m_bIsExitLoop==False)
		{
			sGlsAnimationEx ani;

			glsInitAnimationEx(&ani, &m_item);
			glsSetTrAnimationEx(&ani, 1.0, 0.0, eGAP_SIFO);
			glsSetSzAnimationEx(&ani, 1.0, 0.0, eGAP_SIFO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));

			if (AnimationShow(False, &ani, 15, m_nShowAnimationTime)==False)
			{
				m_bIsExitLoop=True;
			}

			return 0;
		}
		Destroy();
	}
	return 0;
}



// ------------------------------------
Bool cglsDialog::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	//HBITMAP hBmp=(HBITMAP)GetCurrentObject(hdc, OBJ_BITMAP);

	//if (hBmp==0)
	//{
	//	::ReleaseDC(m_hWnd, hdc);
	//	return False;
	//}

	//if (m_glsDrawInterface)
	//{
	//	GetAbsoluteCoordination(m_nAbsX, m_nAbsY);
	//}

	//Bitmap tempBmp((HBITMAP)hBmp, 0);

	//RECT rect = {m_nAbsX, m_nAbsY, m_w, m_h};
	//OnDrawMyself(&tempBmp, &rect, &rect);

	//Graphics g(hdc);
	//g.DrawImage(&tempBmp, m_nAbsX,m_nAbsY, m_nAbsX,m_nAbsY, m_w, m_h, UnitPixel);
	//::ReleaseDC(m_hWnd, hdc);
	return True;
}

// ------------------------------------
void cglsDialog::OnExitLoop()
{
	Destroy();
	return ;
}

// ------------------------------------
void cglsDialog::GetContentsOffset(int&x, int&y)
{
	RECT rect;
	GetClientRect(&rect);

	x=rect.left+GetFontHeight(True)*m_fMarginX;
	y=rect.top+GetFontHeight(True)*m_fMarginY;
}

// ------------------------------------
void cglsDialog::SetContentsSize(int w, int h)
{
	RECT rect;
	GetClientRect(&rect);

	int x=rect.left+GetFontHeight(True)*m_fMarginX;
	int y=rect.top+GetFontHeight(True)*m_fMarginY;

	cGlassInterface* child=GetChild(DIALOG_ITEM_ID_BUTTON_PANEL);

	if (child)
	{
		child->Show();
		child->Update(eGUM_FORCED);

		w+=(x<<1);
		h+=child->m_h+GetFontHeight(True)*2+y;

		if (m_eButtonAlign==eGIA_CENTER)
		{
			child->Move( (w-child->m_w)>>1, h-(child->m_h+rect.top+GetFontHeight(True)/2));
		}
		else if (m_eButtonAlign==eGIA_LEFT)
		{
			child->Move( x, h-(child->m_h+rect.top+GetFontHeight(True)/2));
		}
		else
		{
			child->Move( m_w-x-child->m_w, h-(child->m_h+rect.top+GetFontHeight(True)/2));
		}
	}
	else
	{
		w+=(x<<1);
		h+=GetFontHeight(True)*2+y;
	}


	cglsToolButton* btn = (cglsToolButton*)GetChild(DIALOG_ITEM_ID_TOOL_BUTTON);

	if (btn)
	{
		btn->Show();
		btn->Update(eGUM_FORCED);
		btn->Move(w-rect.left-btn->m_w*1.5, rect.top+btn->m_h/2);
	}

	if (GetChild(133)==0)
	{
		if (m_szCaption[0]!=0)
		{
			sGlsStaticDetail detail;

			cglsStatic::xDefaultDetail(&detail);
			detail.eStyle=eSTS_DROP_SHADOW;

			detail.id=133;
			cglsStatic* caption = new cglsStatic();
			caption->Create((HWND)this, 0, 0, 0, 0, 0, m_szCaption, &detail);
			caption->Update(eGUM_FORCED);
			caption->Show();
			caption->Move((w-caption->m_w)>>1, btn->m_y);
		}
	}
	else
	{
		cGlassInterface* caption = GetChild(133);
		caption->Move((w-caption->m_w)>>1, btn->m_y);
	}


	ReSize(w, h);

	if (xIS_GLS_FLAG(m_flag, eGF_PARENT_CENTER)==True)
	{
		Move((m_parent->m_w-m_w)>>1, (m_parent->m_h-m_h)>>1);
	}
}

// ------------------------------------
void cglsDialog::ShowChildz(int from, int to, Bool bIsShow)
{
	for (int i=from; i<to; i++)
	{
		cGlassInterface* child = GetChild(i);

		if (child==0)
			continue;

		if (bIsShow)
			child->Show();
		else
			child->Hide();
	}
}



// -----------------------------------
void cglsDialog::GetMaxWidthHeight(int from, int to, int&w, int& h, Bool bIsVisibleOnly)
{
	w=0;
	h=0;
	for (int i=from; i<to; i++)
	{
		cGlassInterface* child = GetChild(i);

		if (child==0)
			continue;

		if (bIsVisibleOnly==True)
		{
			if (IsWindowVisible(*child)==FALSE)
				continue;
		}

		w= w<(child->m_x+child->m_w)?(child->m_x+child->m_w):w;
		h= h<(child->m_y+child->m_h)?(child->m_y+child->m_h):h;
	}
}



// -----------------------------------
Bool cglsDialog::CreateBitmap(sDrawItem* item, void* pExtra)
{
	return True;
}

// ------------------------------------
//Bool cglsDialog::AddLayout(cGlassInterface* obj, int x, int y, POINT* pt/*=0*/)
//{
//	if (m_listItemPos==0)
//	{
//		m_listItemPos = new cyiStaticList<sGlsChildLayout>();
//	}
//	sGlsChildLayout layout={obj, x, y};
//
//	m_listItemPos->Add(yitoheap(&layout, sizeof(layout)));
//
//	return True;
//}

// ------------------------------------
//Bool cglsDialog::ReArrangeItemz(RECT* _rect)
//{
//	int w=0, h=0;
//
//	int n=m_listItemPos->count();
//
//	for (int i=0; i<n; i++)
//	{
//		sGlsChildLayout* item = m_listItemPos->GetTopFrom(i);
//
//		int x, y;
//		if (item->x>0x0000FFFF)
//		{
//			x=m_nLayoutPoint[item->x>>16];
//		}
//		else
//		{
//			x=m_nLayoutX[item->x];
//			if (==-1)
//			{
//			}
//		}
//
//		int x = OnSetLayout(item->x, False);
//		int y = OnSetLayout(item->y, True);
//		item->obj->Move(x, y);
//	}
//
//	return True;
//}
//
//// ------------------------------------
//int cglsDialog::OnSetLayout(int id, Bool bIsVertical)
//{
//	return -1;
//}