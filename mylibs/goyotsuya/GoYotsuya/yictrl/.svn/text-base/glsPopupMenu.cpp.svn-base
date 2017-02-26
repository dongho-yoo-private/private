#include "glsPopupMenu.h"
#include "glsResourceBitmap.h"

cglsPopupMenu::cglsPopupMenu(void)
:m_bmpItemForeGround(0),
m_ShowChild(0),
m_parentMenu(0),
m_hMenu(0),
m_bIsExit(False)
{
}

cglsPopupMenu::~cglsPopupMenu(void)
{
}

void cglsPopupMenu::xDefaultDetail(sGlsPopupMenuDetail* detail)
{
	cglsStackPannel::xDefaultDetail(&detail->stack);
	detail->stack.bUseGaussPannel=True;
	detail->stack.bIsFixedMode=False;
	detail->stack.bIsVertical=True;
	detail->stack.eAlign=eSA_LEFT;
	
	detail->bIsBlureInterface=True;
	detail->fBlureRadius=5.0;
	detail->fEdgeRadius=3.0;
	detail->fShadowWidth=7.0;
	detail->colorFont=Color::Black;
	detail->colorSelectionLine=(color_t)Color::DarkBlue;
	detail->colorSelectionTop=(color_t)__COLORp__(64, 255, 255, 255);
	detail->colorSelectionBottom=(color_t)__COLORp__(127, 255, 255, 255);
	detail->fSelectionEdgeRadius=2.0;
}

// -----------------------------------------------------
Bool cglsPopupMenu::CreateFromResource(HWND hWnd, int id)
{
	HINSTANCE hInstance=(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	return CreateFromResource(hWnd, hInstance, id);
	//m_hMenu = LoadMenu(hInstance , MAKEINTRESOURCE(id));
	//m_hMenuWnd=m_hNotifyWnd=m_hParentWnd=hWnd;
	//m_hMenu=f
	return (m_hMenu!=0);
}

// -----------------------------------------------------
Bool cglsPopupMenu::CreateFromResource(HWND hWnd, HINSTANCE hInstance, int id)
{
	m_hMenu = LoadMenu(hInstance , MAKEINTRESOURCE(id));
	m_hMenuWnd=m_hNotifyWnd=m_hParentWnd=hWnd;
	//m_hMenu=f
	return (m_hMenu!=0);
}


// -----------------------------------------------------
//int cglsPopupMenu::OnCreate(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}
//
//// -----------------------------------------------------
//LRESULT cglsPopupMenu::OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled)
//{
//	//PostQuitMessage(0);
//	return 0;
//}

// ------------------------------------
LRESULT cglsPopupMenu::OnNotify(WPARAM wp, LPARAM lp, Bool& bIsHandled)
{
	return 0;
}

// -----------------------------------------------------
void cglsPopupMenu::OnThreadStart(int id, int param1, int param2)
{
	int x=param1>>16;
	int y=param1&0xFFFF;
	int align=param2;

	cGlassInterface::Create(0, 0, 10, 10, 10, 10);
	ShowWindow(m_hWnd, SW_HIDE);
	SetForegroundWindow(m_hWnd);

	BOOL res= TrackPopupMenu(GetSubMenu(m_hMenu, 0) , align|TPM_RETURNCMD, x, y, 0 , m_hWnd/*m_hParentWnd */, NULL);

	m_systemId=0;
	//DWORD dwError = GetLastError();

	//MSG msg;
	//while(GetMessage(&msg, 0, 0, 0))
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}

	//if (res!=0)
	//{
	if (!m_bIsNoNotify)
	{
		cGlassInterface* obj = (cGlassInterface*)GetWindowLong(m_hMenuWnd, GWL_USERDATA);
		cglsSystem* system = obj->GetSystem();

		if (system!=0)
		{
			int id = system->GetCommandId(this, res);

			if (id)
			{
				m_systemId=id;
				//system->GetCommandCenter()->OnCommand(system->GetCommandId(this, res), this);
				PostMessage(m_hMenuWnd, WM_GLS_SYSTEM_COMMAND, id, (LPARAM)this);
			}
			else
			{
				PostMessage(m_hMenuWnd, WM_COMMAND, res, 0);
			}
		}
		else
		{
			PostMessage(m_hMenuWnd, WM_COMMAND, res, 0);
		}

	}

	DestroyWindow(m_hWnd);

	//}

	if (res)
	{
		int nIndex;
		int n=::GetMenuItemCount(GetSubMenu(m_hMenu,0));
		for(int i=0; i<n; i++)
		{
			if (GetMenuItemID(GetSubMenu(m_hMenu,0), i)==res)
			{
				nIndex=i;
			}
			SetCheck(i, False, True);
		}

		SetCheck(nIndex, True, True);
		//CheckMenuItem(nIndex, 
	}

	m_nResultId=res;
	m_bIsExit=True;

}

// -----------------------------------------------------
Bool cglsPopupMenu::Show(int x, int y, int align)
{
	m_bIsNoNotify=False;

	//BOOL res= TrackPopupMenu(GetSubMenu(m_hMenu, 0) , align, x, y, 0 , m_hParentWnd , NULL);
	return StartThread(1, ((x<<16)|y), align);
	//return res;
}

// -----------------------------------------------------
Bool cglsPopupMenu::ShowAndWait(int x, int y, int align)
{
	//BOOL res= TrackPopupMenu(GetSubMenu(m_hMenu, 0) , align, x, y, 0 , m_hParentWnd , NULL);

	POINT pt = {x, y};
	ClientToScreen(m_hMenuWnd, &pt);

	m_bIsExit=False;
	Bool res = StartThread(1, ((pt.x<<16)|pt.y), align);

	//WaitThread(1, INFINITE);
	if (res==False)
		return False;

	MSG msg;
	while(::GetMessage(&msg, NULL, 0, 0))
	{	
		if (msg.message==WM_MOUSEMOVE)
		{
			//RECT rect;
			//POINT pt={msg.wParam&0xFFFF, (msg.wParam>>16)&0xFFFF};
			//ClientToScreen(msg.hwnd, &pt);
			//GetWindowRect(m_hWnd, &rect);

			//if (yiUtilPointIsInRect(&rect, pt.x, pt.y)==False)
			//{
			//	continue;
			//}
			continue;
		}
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);

		if (m_bIsExit==True)
		{
			Sleep(100);
			while (PeekMessage(&msg,NULL, 0, 0, PM_REMOVE)==True)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			break;
		}
	}


	if (m_systemId)
		GetSystem()->GetCommandCenter()->OnCommand(m_systemId, this);

	return res;
}

// -----------------------------------------------------
void cglsPopupMenu::SetCheck(int index, Bool bCheck, Bool bIsTypeRadio)
{

	if (bCheck)
	{
		CheckMenuItem(GetSubMenu(m_hMenu, 0), index, MF_BYPOSITION|MF_CHECKED);
		m_nResultId=GetMenuItemID(GetSubMenu(m_hMenu, 0), index);
	}
	else
	{
		CheckMenuItem(GetSubMenu(m_hMenu, 0), index, MF_UNCHECKED|MF_BYPOSITION);
	}

	//if (bIsTypeRadio)
	//{
	//}
	//else
	//{
	//	CheckMenuRadioItem(GetSubMenu(m_hMenu, 0), index, bCheck);
	//}
}


// -----------------------------------------------------
Bool cglsPopupMenu::Create(HWND hWnd, int id, int x, int y, sGlsPopupMenuDetail* detail)
{
	if (detail==0)
	{
		xDefaultDetail(&m_detail);
	}
	else
	{
		m_detail=*detail;
	}

	RECT rect={x, y, 0, 0};
	int  nFontWidth = 0;//cglsResourceObject::GetFontHeight(0);
	RECT rectMargin = {m_detail.stack.gauss.fShadowWidth+nFontWidth+3, m_detail.stack.gauss.fShadowWidth, m_detail.stack.gauss.fShadowWidth+nFontWidth+3, m_detail.stack.gauss.fShadowWidth};

	if (cglsStackPannel::Create(hWnd, id, &rect, &rectMargin, this, &m_detail.stack)==False)
	{
		return False;
	}
	Hide();
	return True;
}

// -----------------------------------------------------
Bool cglsPopupMenu::Create(HWND hWnd, int id, Bool bIsMenu)
{
	m_hMenuWnd=hWnd;
	if ((m_hMenu=CreateMenu())==0)
		return False;

	HMENU hSubMenu = CreatePopupMenu();
	return AppendMenu(m_hMenu, MF_ENABLED | MF_POPUP , (UINT)hSubMenu, 0);
}

// -----------------------------------------------------
void cglsPopupMenu::Destroy()
{
	if (m_hMenu)
	{
		DestroyMenu(m_hMenu);
	}
	else
	{
		cglsStackPannel::Destroy();
	}
}

// -----------------------------------------------------
Bool cglsPopupMenu::AddItem(int id, Bitmap* icon, wchar_t* pszItemString, cglsPopupMenu* pChildMenu/*=0*/)
{
	if (m_hMenu)
	{
		HMENU hPopup = GetSubMenu(m_hMenu, 0);
		MENUITEMINFOW info;

		memset(&info, 0, sizeof(info));

		//info.cbSize=sizeof(info);
		//info.fMask=MIIM_TYPE;
		//info.fType=MFT_STRING;
		//info.dwTypeData=pszItemString;

		//BOOL res = InsertMenuItemW(m_hMenu, id, FALSE, &info);

		AppendMenuW(hPopup, MF_ENABLED|MF_STRING, id, pszItemString);

		return True;
	}
	return InsertItem(-1, id, icon, pszItemString, pChildMenu);
}

// -----------------------------------------------------
Bool cglsPopupMenu::InsertItem(int index, int id, Bitmap* icon, wchar_t* pszItemString, cglsPopupMenu* pChildMenu/*=0*/)
{
	cglsStackPannel* panel = new cglsStackPannel();
	sGlsStackPannelDetail detail;

	cglsStackPannel::xDefaultDetail(&detail);
	detail.bIsVertical=False;
	detail.bIsFixedMode=False;

	RECT rect={0, 0, 0, 0};
	if (panel->Create((HWND)this, id, &rect, 0, this, &detail)==False)
	{
		delete panel;
		return False;
	}

	if (icon)
	{
		panel->Add(1, icon, 0, 0);
	}

	if (pszItemString)
	{
		panel->Add(2, 0, pszItemString, 0, 0);
	}

	if (pChildMenu)
	{
		sGlsItemDetail item;
		memset(&item, 0, sizeof(item));
		cglsStackPannel::xDefaultItemDetail(&item);

		item.param=(void*)pChildMenu;

		panel->Add(4, (Bitmap*)eGRID_ARROW_RIGHT, 0, 0, 0, &item);
		pChildMenu->SetParentMenu(this);
	}

	panel->Show();
	return Insert(index, panel, 0);
}

// -----------------------------------------------------
LRESULT cglsPopupMenu::OnShow(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	handled=False;
	//SetCapture(m_hWnd);
	Update(eGUM_FORCED);
	//m_glsDrawInterface->m_nRequestUpdateCount=1;
	InvalidateRect(0);
	return 0;
}

// -----------------------------------------------------
// Hide cascade.
void cglsPopupMenu::Hide()
{
	//if (m_hMenu)
	//{

	//}

	if (m_ShowChild)
	{
		m_ShowChild->Hide();
	}

	cyctrlbase::Hide();
	//Hide();

	if (m_parentMenu==0)
	{
		//ReleaseCapture();
	}
}


// ---------------------------------------
void cglsPopupMenu::SetParentMenu(cglsPopupMenu* parent)
{
	m_parentMenu=parent;
}


// ---------------------------------------
LRESULT cglsPopupMenu::OnMouseLButtonDown(WPARAM wp, LPARAM lp, BOOL& handled)
{
	int id = cglsStackPannel::OnMouseLButtonDown(wp, lp, handled);

	if (id==-1)
	{
		if (m_parentMenu)
		{
			m_parentMenu->Hide();
		}
		else
			Hide();

		return 0;
	}

	Hide();
	PostMessage(m_hNotifyWnd, WM_COMMAND, id, m_id);

	return 0;
}


// ---------------------------------------
LRESULT cglsPopupMenu::OnMouseMove(WPARAM wp, LPARAM lp, BOOL& handled)
{
	int id = cglsStackPannel::OnMouseMove(wp, lp, handled);

	if (id==-1)
		return -1;

	sDrawItem* item = (sDrawItem*)xGetItemById(id);
	cglsStackPannel* panel = (cglsStackPannel*)item->gls;

	if (panel==0)
	{
		if (m_ShowChild)
		{
			m_ShowChild->Hide();
			m_ShowChild=0;
		}
		return 0;
	}

	item=(sDrawItem*)panel->GetItem(4);

	cglsPopupMenu* child=(cglsPopupMenu*)item->gls;

	if (child)
	{
		int x = panel->m_x;
		int y = panel->m_y;

		if (m_ShowChild)
		{
			m_ShowChild->Hide();
		}
		child->Move(x+m_x, y+m_y);
		//child->Show();

		m_ShowChild=child;
	}
	else
	{
		m_ShowChild->Hide();
		m_ShowChild=0;
	}

	//PostMessage(m_hNotifyWnd, WM_GLS_TAB_SELCHANGED, id, m_id);
	return 0;
}



// ---------------------------------------
Bitmap* cglsPopupMenu::GetChildFgImage(int childId, int nImageIndex, RECT* rect)
{
	if (nImageIndex==1)
	{
		if (m_bmpItemForeGround==0)
		{
			m_bmpItemForeGround=
				cyiShape::DrawRoundRectEx(0, &Pen(Color(m_detail.colorSelectionLine)), 
				&LinearGradientBrush(Point(0, 0), Point(0, rect->bottom), Color(m_detail.colorSelectionTop), Color(m_detail.colorSelectionBottom)), 
					RectF(0, 0, rect->right, rect->bottom), m_detail.fSelectionEdgeRadius);
		}

		return m_bmpItemForeGround;
	}

	return 0;
}

// -----------------------------------------------------
Bool cglsPopupMenu::CreateBitmap(sDrawItem* item, void* pExtra)
{
	if (item->id==0)
	{
		return False;
	}

	cglsResourceBitmap* resource = (cglsResourceBitmap*)m_xresource;
	
	if (resource==0)
	{
		resource=(cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
	}


	Bitmap* bmp=0;
	Bitmap* bmpStr=0;


	//if (m_font==0)
	//{
	//	if (m_xfont==0)
	//	{
	//		m_xfont=m_font;

	//		if (m_xfont==0)
	//		{
	//			//m_xfont=m_parent->GetFont();
	//		}
	//	}

	//	// デフォルト
	//	if (m_xfont==0)
	//	{
	//		m_xfont=new Font(L"メイリオ", 11.0);
	//	}
	//}
	//else
	//{
	//	m_xfont=m_font;
	//}

	m_xfont=GetFont();

	int nFontHeight = cglsResourceObject::GetFontHeight(m_xfont);

	// サブメニューの場合
	if (item->id==4)
	{
		item->pBmp[0]=resource->GetBitmap(eGRID_ARROW_RIGHT, m_detail.colorFont, nFontHeight*0.5, nFontHeight*0.5);
		item->pCurrentBmp=item->pBmp[0];
	}
	else if (item->txt)
	{
		bmpStr = cyiShape::BitmapString(0, item->txt, m_xfont, &SolidBrush(Color(m_detail.colorFont)));

		item->pBmp[0]=bmpStr;
		item->pCurrentBmp=bmpStr;
		//return True;
	}

	else if (item->bitmapId)
	{
		if ((item->bitmapId&0xFFFF0000)==0)
			bmp = resource->GetBitmap(item->bitmapId, 0, 0, ((float)nFontHeight*0.7));
		else
			bmp=(Bitmap*)item->bitmapId;

		item->pBmp[0]=bmp;
		item->pCurrentBmp=bmp;

		//return True;
	}
	else
	{
		return False;
	}
	
	item->rect.x=0;
	item->rect.y=0;
	item->rect.w=item->pCurrentBmp->GetWidth();
	item->rect.h=item->pCurrentBmp->GetHeight();

	return True;
}

// --------------------------------
void cglsPopupMenu::Enable(Bool bIsEnable, int id)
{
	MENUITEMINFO info;
	info.cbSize = sizeof( info );
	info.fMask = MIIM_STATE;
	info.fState = MFS_DISABLED;
	SetMenuItemInfo(m_hMenu, id, FALSE, &info);
	DrawMenuBar(m_hParentWnd);
}

// -----------------------------------------------------
void cglsPopupMenu::DestroyBitmap(sDrawItem* item)
{
	if (item->pBmp[0])
	{
		delete item->pBmp[0];
		item->pBmp[0]=0;
	}

	return ;
}

// -----------------------------------------------------
Bool cglsPopupMenu::ShowAssistant(HWND hWnd, int x, int y)
{
	m_bIsNoNotify=True;
	ShowAndWait(x, y);
	return True;
}

// -----------------------------------------------------
Bool cglsPopupMenu::GetResult(wchar_t* buffer)
{
	char cBuffer[1024];

	if (m_nResultId==0)
	{
		buffer[0]=0;
		return False;
	}

	GetMenuString(m_hMenu, m_nResultId, cBuffer, 1024, MF_BYCOMMAND);
	::yiToUnicode(buffer, cBuffer, 0);
	return True;
}

// -----------------------------------------------------
Bool cglsPopupMenu::GetResult(unsigned int& result1, unsigned int& result2)
{
	result1=m_nResultId;
	result2=0;
	return True;
}

// -----------------------------------------------------
Bool cglsPopupMenu::GetDefaultValue(wchar_t* buffer)
{
	if (m_nResultId)
	{
		char cBuffer[1024];
		GetMenuString(m_hMenu, m_nResultId, cBuffer, 1024, MF_BYCOMMAND);
		::yiToUnicode(buffer, cBuffer, 0);
		return True;
	}
	buffer[0]=0;
	return False;
}

// -----------------------------------------------------
Bool cglsPopupMenu::GetDefaultValue(unsigned int& result1, unsigned int& result2)
{
	result1=m_nResultId;
	return True;
}

// -----------------------------------------------------
Bool cglsPopupMenu::SetDefaultValue(wchar_t* buffer)
{
	return True;
}
