#include "GlassInterface.h"
#include "glsToolTip.h"

// -----------------------------------------
cGlassInterface::cGlassInterface(void)
: m_bmpCurrent(0),
  m_font(0),
  m_resource(0),
  m_children(0),
  m_parent(0),
  m_glsDrawInterface(0),
  m_nAbsX(0),
  m_nAbsY(0),
  m_hPaintMutex(0),
  m_hNotifyWnd(0),
  m_hAnitShow(0),
  m_pItemRects(0),
  m_NowAnimationList(True),
  m_flag(0),
  m_childwindow(0),
  m_nRequestUpdateOrder(0),
  //m_tooltip(0),
  m_DrawObject(0),
  m_nFontHeight(0),
  m_listpos(0), 
  m_nFontHeightGDI(0),
  m_eFontType(eFT_PARENT),
  m_eUpdateReason(eGUR_FORCED),
  //m_fLeaveTransparency(1.0),
  //m_fEnterTransparency(1.0),
  m_nAnimationMilliSec(15),
  m_nMouseMoveAnimationInterval(200),
  m_nShowAnimationInterval(200),
  m_eMouseLeavePattern(eGAP_FISO),
  m_eMouseMovePattern(eGAP_SIFO),
  m_eShowPattern(eGAP_SIFO),
  m_nDragOnTimer(0),
  m_nKeyLButtonDownTimer(0),
  m_nKeyRepeatInterval(0),
  m_nTooltipTimer(0),
  m_nMouseLeaveAnimationInterval(200),
  m_pUserParam(0),
  m_system(0),
  m_focusChildren(0),
  m_bIsRequestDestroy(False),
  m_bIsDblClicked(False),
  m_menu(0),
  m_bmpBg(0),
  m_posX(0),
  m_posY(0),
  m_pszToolTip(0)
{
	memset(&m_item, 0, sizeof(m_item));
	//memset(&m_fgItem, 0, sizeof(m_fgItem));
	m_item.fTransparency=1.0;
	m_item.fSizeRate=1.0;

	m_hPaintMutex = 0;//yiMutexCreateEx(10);
	//xDefaultDetail(&m_detail);

	m_hMutex=yiMutexCreateEx(3);
	m_hPaintMutex=yiMutexCreateEx(3);
	m_glsDrawInterface=0;

	__GLASSINTERFACE_NAME__;
	
}

cGlassInterface::~cGlassInterface(void)
{
	//if (m_bIsCreated==True)
	//{
	//	Destroy();
	//}
	//yiMutexDestroyEx(m_hPaintMutex);

	//yiMutexDestroyEx(m_hMutex);

	//if (m_bIsCreated==True)
	//{
	//	Destroy();
	//}
}

// --------------------------------
void cGlassInterface::xDefaultDetail(sGlsDetail* detail)
{
	memset(detail, 0, sizeof(sGlsDetail));

	detail->font= 0;//new Font(L"Meiryo", 10.0);
	detail->bIsNoAnimation=False;
	detail->bIsNotTransparent=False;
	detail->bIsNotOwnDraw=True;
	detail->eMouseLeavePattern=eGAP_SIFO;
	detail->eMouseMovePattern=eGAP_FISO;
	detail->nAnimationMilliSec=10;
	detail->nMouseLeaveAnimationInterval=300;
	detail->nMouseMoveAnimationInterval=250;
	detail->nShowAnimationInterval=250;
	detail->eShowPattern=eGAP_FISO;
	detail->bIsTransparentWindow=True;
	detail->fEnterTransparency=-1.0;
	detail->fLeaveTransparency=-1.0;
	detail->nTooltipTimer=0;
	detail->nDragOnTimer=-1;
	detail->nKeyLButtonDownTimer=0;
	detail->nKeyRepeatInterval=100;
	detail->bRecieveDblClick=False;
	detail->bMouseDownCommand=False;
	detail->bIsNoSendCommand=False;
	detail->bUpdateIndependent=False;
	detail->bFocusStop=False;
	detail->bIsFocusManageInterface=False;
	detail->bIsAnimationShow=False;
	detail->bDisableDisplayOnly=False;
	detail->bDrawMyway=False;
	detail->fEnterTransparency=1.0;
	detail->fLeaveTransparency=1.0;

	detail->nShowAnimationTime=200;
	detail->nHideAnimationTime=300;
	detail->style=eGGS_DEFAULT;
	detail->menu=0;


	return;
}

// ---------------------------------------
// 時間がある時に続行
//Bitmap* cGlassInterface::GetCurrentBmp(sDrawItem* item/*=0*/)
//{
//	if (item)
//	{
//		Bitmap* ret=item->pCurrentBmp;
//
//		if (item->pTransparencyBmp)
//			return item->pTransparencyBmp;
//
//		if (item->pAnimationBmp)
//			return item->pAnimationBmp;
//
//		return ret;
//	}
//
//	return 0;
//}

// --------------------------------
void cGlassInterface::InitDetail(sGlsDetail& m_detail)
{
	//if (m_detail.fLeaveTransparency!=-1.0)
	//	m_item.fTransparency=m_detail.fLeaveTransparency;
	//else
	
	m_item.fTransparency=1.0;

	//m_item.fLeaveTransparency=m_detail.fLeaveTransparency;
	//m_item.fEnterTransparency=m_detail.fEnterTransparency;
	//m_item.fScale=1.0;

	xSET_GLS_FLAG(m_flag, eGF_TRANSPARENT, m_detail.bIsTransparentWindow);
	xSET_GLS_FLAG(m_flag, eGF_NO_ANIMATION, m_detail.bIsNoAnimation);
	xSET_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW, m_detail.bIsNotOwnDraw);
	xSET_GLS_FLAG(m_flag, eGF_NO_MOUSEMOVE, m_detail.bIsNoMouseMove);
	xSET_GLS_FLAG(m_flag, eGF_NO_INPUT, m_detail.bIsNoInput);
	xSET_GLS_FLAG(m_flag, eGF_DBL_CLICK, m_detail.bRecieveDblClick);
	xSET_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND, m_detail.bMouseDownCommand);
	xSET_GLS_FLAG(m_flag, eGF_TRANSPARENT_WINDOW, m_detail.bIsTransparentWindow);
	xSET_GLS_FLAG(m_flag, eGF_NOCOMMAND, m_detail.bIsNoSendCommand);
	xSET_GLS_FLAG(m_flag, eGF_UPDATE_INDEPENDENT, m_detail.bUpdateIndependent);
	xSET_GLS_FLAG(m_flag, eGF_FOCUS_STOP, m_detail.bFocusStop);
	xSET_GLS_FLAG(m_flag, eGF_FOCUS_MANAGE, m_detail.bIsFocusManageInterface);
	xSET_GLS_FLAG(m_flag, eGF_ANIMATION_SHOW, m_detail.bIsAnimationShow);
	xSET_GLS_FLAG(m_flag, eGF_DISABLE_DISPLAY_ONLY, m_detail.bDisableDisplayOnly);
	xSET_GLS_FLAG(m_flag, eGF_DRAW_MYWAY, m_detail.bDrawMyway);
	
	m_font=m_detail.font?(Font*)cyiShape::Clone(m_detail.font):0;

	if (xIS_GLS_FLAG(m_flag, eGF_NO_ANIMATION)==False)
	{
		m_item.hAnit=yiAnitCreate();
	}

	m_nToolTipTimer=m_detail.nTooltipTimer;
	//m_nDragTimer=m_detail.nDragOnTimer;
	m_nLButtonLongTimer=m_detail.nKeyLButtonDownTimer;

	m_nAnimationMilliSec=m_detail.nAnimationMilliSec;
	m_nMouseLeaveAnimationInterval=m_detail.nMouseLeaveAnimationInterval;
	m_nMouseMoveAnimationInterval=m_detail.nMouseMoveAnimationInterval;
	m_nShowAnimationInterval=m_detail.nShowAnimationInterval;
	m_eMouseLeavePattern=m_detail.eMouseLeavePattern;
	m_eMouseMovePattern=m_detail.eMouseMovePattern;
	m_eShowPattern=m_detail.eShowPattern;


	m_item.fEnterTransparency=m_detail.fEnterTransparency;
	m_item.fLeaveTransparency=m_detail.fLeaveTransparency;
	m_item.fTransparency=m_item.fLeaveTransparency;

	m_item.fSizeRate=1.0;

	m_nDragOnTimer=m_detail.nDragOnTimer;
	m_nKeyLButtonDownTimer=m_detail.nKeyLButtonDownTimer;
	m_nKeyRepeatInterval=m_detail.nKeyRepeatInterval;
	m_nTooltipTimer=m_detail.nTooltipTimer;

	if (m_detail.bIsAnimationShow==True)
		m_item.fTransparency=0.0;

	m_nShowAnimationTime=m_detail.nShowAnimationTime;
	m_nHideAnimationTime=m_detail.nHideAnimationTime;

	m_guiStyle=m_detail.style;

	xSET_ITEM_TYPE((&m_item), eSPT_GLASSINTERFACE);

	if (m_guiStyle==eGGS_DEFAULT)
	{
		if (m_parent)
		{
			if (IsWindow((HWND)m_parent)==False)
				m_guiStyle = m_parent->GetGuiStyle();
		}
	}

}

// -------------------------------------
Bool cGlassInterface::IsCreated()
{
	return 	m_bIsCreated;
}

// -------------------------------------
Bool cGlassInterface::SetItemPos(int x, int y, eGlsItemAlign align)
{
	int _x=x;
	int _y=y;

	if (m_parent)
	{
		if (align&eGIA_CENTER)
		{
			_x+=((m_parent->m_w-m_w)>>1);
		}
		else if (align&eGIA_RIGHT)
		{
			_x+=(m_parent->m_w-m_w);
		}

		if (align&eGIA_VCENTER)
		{
			_y+=((m_parent->m_h-m_h)>>1);
		}
		else if (align&eGIA_BOTTOM)
		{
			_y+=(m_parent->m_h-m_h);
		}

		m_item.rect.x=_x;
		m_item.rect.y=_y;
		return Move(_x, _y);
	}
	return True;
}


// --------------------------------
void cGlassInterface::SetParent(HWND hWndParent)
{
	::SetParent(m_hWnd, hWndParent);

	if (m_hParentWnd==m_hNotifyWnd)
	{
		m_hNotifyWnd=m_hParentWnd=hWndParent;
	}
	else
	{
		m_hParentWnd=hWndParent;
	}

}

// --------------------------------
void cGlassInterface::SetParent(cGlassInterface* parent)
{
	if (m_parent==parent)
		return ;

	if (m_parent==0)
	{
		LONG_PTR n = GetWindowLong(*this, GWL_STYLE);
		n&=(~WS_POPUP);
		n|=WS_CHILD;
		SetWindowLong(*this, GWL_STYLE, n);
	}
	else
	{
		m_parent->UnRegisterChild(this);
	}
	
	if (parent==0)
	{
		LONG_PTR n = GetWindowLong(*this, GWL_STYLE);
		n&=(~WS_CHILD);
		n|=WS_POPUP;
		SetWindowLong(*this, GWL_STYLE, n);
	}
	else
	{
		HWND hWnd;
		m_parent=parent;
		m_parent->RegisterChild(this);

		m_glsDrawInterface=GetDrawWindow(hWnd);

		if (m_glsDrawInterface)
		{
			GetAbsoluteCoordination(m_nAbsX, m_nAbsY);
		}
	}
	SetParent(parent?parent->m_hWnd:0);
}


// --------------------------------
Bool cGlassInterface::Create(HWND hWndParent, HINSTANCE hInstance, int id, RECT*lpRect, sGlsDetail* detail)
{
	WNDCLASSEX wcex;
	sGlsDetail m_detail;

	int x=CW_USEDEFAULT, y=CW_USEDEFAULT, w=CW_USEDEFAULT, h=CW_USEDEFAULT;
	
	if (detail)
	{
		m_detail=*detail;
	}
	else
	{
		cGlassInterface::xDefaultDetail(&m_detail);
	}

	if (IsWindow(hWndParent)==False)
		m_parent = (cGlassInterface*)hWndParent;

	InitDetail(m_detail);

	
	m_bIsRequestDestroy=False;

	if (hWndParent==0)
	{
		m_parent=0;
		m_hParentWnd=0;
	}
	if (IsWindow(hWndParent)==False)
	{
		m_parent=(cGlassInterface*)hWndParent;
		hWndParent=m_parent->m_hWnd;

		cglsSystem* system = m_parent->GetSystem();

		if (system)
		{
			if (system->GetPerformance()<3)
			{
				xON_GLS_FLAG(m_flag, eGF_NO_ANIMATION);
				xOFF_GLS_FLAG(m_flag, eGF_ANIMATION_SHOW);

				if (m_detail.fLeaveTransparency!=-1.0)
					m_item.fTransparency = m_detail.fLeaveTransparency;
				else
					m_item.fTransparency = 1.0;
			}
		}

		//if (m_font==0)
		//{
		//	m_font=m_parent->m_font;

		//	if (m_font==0)
		//	{
		//		m_font = new Font(L"Meiryo", 10.0);
		//		m_eFontType=eFT_SELF;
		//	}
		//	else
		//	{
		//		m_eFontType=eFT_PARENT;
		//	}
		//	xSET_GLS_FLAG(m_flag, eGF_FONT_FROM_PARENT, True);
		//}
	}
	else
	{
		m_parent=0;
	}

	m_item.gls=this;
	
	if (hInstance!=0)
		m_hInstance=hInstance;
	else
	{
		if (hWndParent)
			m_hInstance=(HINSTANCE)::GetWindowLong(hWndParent, GWL_HINSTANCE);
		else
			m_hInstance=0;
	}


	if (m_font)
	{
		m_nFontHeight = m_font->GetHeight(__DEFAULTGRAPHICS__);
		IntPtr pos = (IntPtr)cyiShape::BitmapStringEx(0, L"1", m_font, 0, 0);
		m_nFontHeightGDI = (pos&0xFFFF);
	}

	if (lpRect)
	{
		if (lpRect->bottom==0)
			lpRect->bottom=m_nFontHeightGDI+3;

		if (lpRect->right==0)
			lpRect->right=m_nFontHeightGDI+3;
	}



	m_hNotifyWnd = m_hParentWnd = hWndParent;
	m_bMouseTrackMode = True;
	m_bPushed=FALSE;
	m_id=id;

	if (lpRect!=0)
	{
		x=lpRect->left;
		y=lpRect->top;
		w=lpRect->right;
		h=lpRect->bottom;
	}

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW|CS_VREDRAW|(m_detail.bRecieveDblClick ? CS_DBLCLKS:0);
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CTRLS));
	wcex.hCursor		= NULL;//LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)::GetStockObject(NULL_BRUSH);//(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= m_detail.bRecieveDblClick?"NULL_WINDOW_CLASS_DBL":"NULL_WINDOW_CLASS";
	wcex.hIconSm		= NULL;

	if (RegisterClassEx(&wcex)==0)
	{
		//return FALSE;
	}

	m_id = id;
	m_hWnd = CreateWindow(m_detail.bRecieveDblClick?"NULL_WINDOW_CLASS_DBL":"NULL_WINDOW_CLASS",
							NULL,//タイトルバーにこの名前が表示されます
							hWndParent==0?WS_POPUP:WS_CHILD /*|WS_CLIPSIBLINGS | WS_CLIPCHILDREN*/,    //ウィンドウの種類
							x,    //Ｘ座標
							y,    //Ｙ座標
							w,    //幅
							h,    //高さ
							hWndParent,            //親ウィンドウのハンドル、親を作るときはNULL
							(HMENU)id, //メニューハンドル、子供のID
							m_hInstance,            //インスタンスハンドル
							this);

	if (m_hWnd)
	{
		RECT rect;
		::GetClientRect(m_hWnd, &rect);

		m_bIsCreated=True;

		m_w=rect.right;
		m_h=rect.bottom;

		GetWindowRect(m_hWnd, &rect);

		int deltaX = rect.right-rect.left-m_w;
		int deltaY = rect.bottom-rect.top-m_h;

		if (m_hParentWnd)
		{
			ScreenToClient(m_hParentWnd, (LPPOINT)&rect);
			m_x=rect.left-deltaX;
			m_y=rect.top-deltaY;
		}
		else
		{
			m_x = rect.left-deltaX;
			m_y = rect.top-deltaY;
		}

		if (m_parent)
		{
			HWND hWndParent;
			m_parent->RegisterChild(this);

			if (m_detail.bFocusStop==True)
			{
				RegisterFocusStopObject(this);
			}
			
			m_glsDrawInterface=GetDrawWindow(hWndParent);

			if (m_glsDrawInterface)
			{
				GetAbsoluteCoordination(m_nAbsX, m_nAbsY);
			}
		}
		else
		{
			m_glsDrawInterface=this;
		}

	}

	return m_hWnd ? True:False;
}

// -------------------------------------
Bool cGlassInterface::Create(HWND _hWndParent, int id, int x, int y, int w, int h, 
							 DWORD styleEx/*=0*/, DWORD style/*=0*/, sGlsDetail* detail/*=0*/)
{
	HWND hWndParent;
	if (IsWindow(_hWndParent)==FALSE)
	{
		m_parent=(cGlassInterface*)_hWndParent;
		if (m_parent)
		{
			hWndParent=m_hNotifyWnd=m_hParentWnd=m_parent->m_hWnd;
		}
		else
		{
			hWndParent=0;
			m_hNotifyWnd=0;
			m_hParentWnd=0;
		}
	}
	else
	{
		m_parent=0;
		hWndParent=m_hNotifyWnd=m_hParentWnd=_hWndParent;
	}

	sGlsDetail m_detail;

	if (detail)
	{
		m_detail=*detail;
	}
	else
	{
		xDefaultDetail(&m_detail);
	}

	//m_parent = (cGlassInterface*)hWndParent;

	InitDetail(m_detail);
	m_bIsRequestDestroy=False;


	//if (m_font==0)
	//{
	//	if (m_parent)
	//		m_font=m_parent->m_font;
	//}

	if (m_font)
	{
		m_nFontHeight=m_font->GetHeight(__DEFAULTGRAPHICS__);
		IntPtr pos = (IntPtr)cyiShape::BitmapStringEx(0, L"1", m_font, 0, 0);
		m_nFontHeightGDI = (pos&0xFFFF);
	}




	m_DrawObject=0;

	m_hInstance=(HINSTANCE)GetWindowLong(hWndParent, GWL_HINSTANCE);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(GetStockObject(NULL_BRUSH));
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= "GLS_WINDOW_CHILD";
	wcex.hIconSm		= 0;

	RegisterClassEx(&wcex);


	//m_hParentWnd=0;
	
	//if (style&WS_POPUP)
	//{
	//	xGLS_FLAG_ON(m_flag, eGF_POPUP);
	//}

	m_x=-1;
	m_y=-1;
	m_w=-1;
	m_h=-1;

	//if (m_parent)
	//{
	//	if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==True)
	//	{
	//		m_parent->RegisterChild(this);
	//		m_glsDrawInterface=GetDrawWindow(hWndParent);
	//	}
	//}

	m_x=0;
	m_y=0;

	m_x=x;
	m_y=y;
	
	if (m_parent)
	{
		if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==True)
		{
			m_glsDrawInterface=GetDrawWindow(hWndParent);
			GetAbsoluteCoordination(m_nAbsX, m_nAbsY);
		}
		else
		{
			m_glsDrawInterface=this;
			m_nAbsX=x;
			m_nAbsY=y;
		}
	}

	m_item.gls=this;

	m_hWnd = CreateWindowEx(styleEx, "GLS_WINDOW_CHILD", 0, style==0?WS_OVERLAPPEDWINDOW:style,
				  x, y, w, h, hWndParent, NULL, m_hInstance, this);



	if (m_hWnd==0)
	{
		if (m_parent)
			m_parent->UnRegisterChild(this);
		return False;
	}

	m_bIsCreated=True;

	if (m_w==-1 
		&&	m_h==-1)
	{
		m_w=w;
		m_h=h;
	}

	if (m_x==0 && 
			m_y==0)
	{
		m_x=x;
		m_y=y;
	}


	if (m_parent)
	{
		if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==True)
		{
			m_parent->RegisterChild(this);
			//m_glsDrawInterface=GetDrawWindow(hWndParent);
			//GetAbsoluteCoordination(m_nAbsX, m_nAbsY);
		}
		/*else
		{
			m_glsDrawInterface=this;
		}*/


		cglsSystem* system = m_parent->GetSystem();

		if (system)
		{
			if (system->GetPerformance()<3)
			{
				xON_GLS_FLAG(m_flag, eGF_NO_ANIMATION);
				xOFF_GLS_FLAG(m_flag, eGF_ANIMATION_SHOW);

				if (m_detail.fLeaveTransparency!=-1.0)
					m_item.fTransparency = m_detail.fLeaveTransparency;
				else
					m_item.fTransparency = 1.0;

			}
		}

	}
	else
	{
		m_glsDrawInterface=this;
		xOFF_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW);
	}
	//OnBgDraw();

	//ShowWindow(m_hWnd, SW_SHOWNORMAL);
	InvalidateRect(0, FALSE);
	UpdateWindow(m_hWnd);

	return True;
}

#include "glsPopup.h"
// -------------------------------------
Bool cGlassInterface::CreateToolTip(const wchar_t* string, int nToolTipTimer, void*/*sGlsPopupDetail**/ _detail)
{
	m_pszToolTip=yistrheapW(string);
	//sGlsPopupDetail* detail=(sGlsPopupDetail*)_detail;
	//sGlsPopupDetail xdetail;

	//if (detail)
	//	xdetail=*detail;
	//else
	//{
	//	cglsPopup::xDefaultDetail(&xdetail);
	//}

	//Font tempFont(L"MS P明朝", GetFont()->GetSize()*0.8); 
	//xdetail.gls.font = &tempFont;

	m_nToolTipTimer=nToolTipTimer;
	//m_tooltip=new cglsPopup();
	//RECT rect;

	//GetWindowRect(m_hWnd, &rect);
	//ScreenToClient(m_glsDrawInterface->m_hWnd, (LPPOINT)&rect);
	//xdetail.gls.bIsNoInput=True;
	//xdetail.gls.bIsNoMouseMove=True;
	//if (m_tooltip->Create((HWND)m_glsDrawInterface, rect.left, rect.right, -1/*m_id-256*/, 0, string, &xdetail)==True)
	//{
	//	return True;
	//}

	//return False;
	return True;
}

// -------------------------------------
Bool cGlassInterface::SetHiLight(Bool bIsHilight)
{
	if (bIsHilight)
		xON_GLS_FLAG(m_flag, eGF_HILITE);
	else
		xOFF_GLS_FLAG(m_flag, eGF_HILITE);

	Update(eGUM_UPDATE);
	InvalidateRect(0);

	return True;
}


// --------------------------------
//int cGlassInterface::GetValidChildId()
//{
//	if (m_children==0)
//		return 1;
//
//	for (cyiNode* node=m_children->GetNodeTopFrom(); node; node=node->np)
//	{
//		cGlassInterface* node->ref;
//	}
//}

// --------------------------------
void cGlassInterface::SetNotifyWindow(HWND hWnd)
{
	m_hNotifyWnd=hWnd;
}

// --------------------------------
void cGlassInterface::Enable(Bool bIsEnable, int id)
{

	if (id!=0)
	{
		cGlassInterface* child = GetChild(id);
		child->Enable(bIsEnable, id);
		return ;
	}

	if (IsWindowEnabled(m_hWnd)==bIsEnable)
	{
		return ;
	}

	EnableWindow(m_hWnd, bIsEnable);

	if (bIsEnable==False)
	{
		m_item.fTransparency=0.5;
	}
	else
	{
		m_item.fTransparency=m_item.fEnterTransparency==-1.0?1.0:m_item.fEnterTransparency;
	}

	InvalidateRect(0);
}

// ------------------------------------
Bitmap* cGlassInterface::GetCurrentBitmap(sDrawItem* item)
{
	item = item==0?&m_item:item;

	//if (m_bmpShowAnimation)
	//	return m_bmpShowAnimation;
	Bitmap* pBmpAnimation = item->animation==0?0:item->animation->pBmpAnimation;
	return pBmpAnimation?pBmpAnimation:item->pCurrentBmp;
}

// ------------------------------------
Bool cGlassInterface::OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX, int offsetY)
{
	return False;
}

// -----------------------------
void cGlassInterface::Update(Bitmap* bmp, RECT* rect, RECT* childRect,int offsetX, int offsetY, Bool bIsLocalBufferUpdate)//, cGlassInterface* sender/*GLS_IGNORE_SENDER*/)
{
	RECT _rect = {0, 0, m_w, m_h};
	RECT _childRect;
	Bitmap* pCurrentBmp=0;

	if (m_bIsCreated==False)
		return ;

	if (IsWindowVisible(*this)==False)
	{
		if (xIS_ITEM_HIDE((&m_item))!=0)
		{
			return ;
		}
	}

	if (childRect)
	{
//#ifdef YIDEBUG
//		YIDEBUG("Update(%s, %d, %d, %d, %d):%d", m_szClassName, childRect->left, childRect->top, childRect->right, childRect->bottom, GetDrawOrder());
//#endif
	}

	if (rect==0)
	{
		rect=&_rect;
	}

	if (childRect==0)
	{
		childRect=&_childRect;
		_childRect=*rect;
	}

	if (bmp==0)
	{
		bmp=m_bmpCurrent;
	}

	if (bmp==0)
		return ;

	//if (m_item.fTransparency==1.0)
	//{
	//	Sleep(1);
	//}

	//if (m_item.fSizeRate==1.0)
	//{
	//	Sleep(1);
	//}

	if ((bIsLocalBufferUpdate==False) &&
		((m_item.fTransparency!=1.0) || (m_item.fSizeRate!=1.0)))
	{
		if (m_item.fTransparency==0.0||
				m_item.fSizeRate==0.0)
		{
			m_eUpdateReason=eGUR_NO_REASON;
			if (xIS_GLS_FLAG(m_flag, eGF_ANIMATION_SHOW)==True)
			{
				goto SKIP_CHECK_TRANSPARENCY;
			}
			return ;
		}
		else 
		{
			Bitmap* xbmp = BITMAP32(childRect->right, childRect->bottom);

			// ここで子供を含めたイメージをバッファーに描画させる。
			Update(xbmp, childRect, childRect, m_nAbsX, m_nAbsY, True);


			float fTransparency=GetMyTransparency();

			Bitmap* bmp2=cyiBitmapBlender::Transparent(xbmp, fTransparency/*m_item.fTransparency*/);
			
			delete xbmp;
			
			int x=rect->left;
			int y=rect->top;
			int w=rect->right;
			int h=rect->bottom;
			int xoff=0;
			int yoff=0;

			Graphics g(bmp);

			if (m_item.fSizeRate!=1.0)
			{
				float nw = (float)(m_item.fSizeRate*(float)w);
				float nh = (float)(m_item.fSizeRate*(float)h);

				if (m_item.animation->align&eGIA_CENTER)
				{
					x+=((w-(INT)nw)>>1);
				}
				else if (m_item.animation->align&eGIA_RIGHT)
				{
					x+=(w-(INT)nw);
				}

				if (m_item.animation->align&eGIA_VCENTER)
				{
					y+=((h-(INT)nh)>>1);
				}
				else if (m_item.animation->align&eGIA_BOTTOM)
				{
					y+=(h-(INT)nh);
				}
				

				g.DrawImage(bmp2, x, y, (INT)nw, (INT)nh);
				//g.DrawImage(bmp2, x, y, xoff, yoff, w, y, Unit::UnitPixel);
				delete bmp2;
				return ;
			}

			g.DrawImage(bmp2, x, y, xoff, yoff, w, y, Unit::UnitPixel);

			delete bmp2;
			return ;
			//goto SKIP_DRAW;

		}
	}

SKIP_CHECK_TRANSPARENCY:
	{
		Bool bIsDrawed=False;

		if ((bIsDrawed=OnDrawMyself(bmp, rect, childRect, offsetX, offsetY))==False)
		{
			if (m_item.fTransparency==0.0)
				return ;

			if (GetBgBitmap()!=0)
			{
				Bitmap* bg=GetBgBitmap();

				if (bg)
				{
					Graphics g(bmp);
					Status status = g.DrawImage(bg, 
								rect->left, rect->top, childRect->left,childRect->top,
								rect->right, rect->bottom, Unit::UnitPixel);
					bIsDrawed=True;
				}
			}
		}
		else
		{
			if (m_item.fTransparency==0.0)
				return ;
		}

		// 上記の手法で描画できなかった場合は、通常の描画
		//if (bIsDrawed==False)
		//{
		pCurrentBmp=GetCurrentBitmap();
		if (pCurrentBmp!=0)
		{
			// hdcに直接描画をする？
			Graphics g(bmp);
			Status status = g.DrawImage(pCurrentBmp, 
						rect->left, rect->top, childRect->left,childRect->top,
						rect->right, rect->bottom, Unit::UnitPixel);
							
		}
		//}
	}

	if (m_childwindow==0 ||
			m_childwindow->count()==0)
	{
		goto SKIP_WINDOW_DRAW;
	}

	for (cyiNode* node=m_childwindow->GetNodeTopFrom(); node; node=node->np)
	{
		HWND hWnd = (HWND)node->ref;

		if (IsWindowVisible(hWnd)==False)
			continue;

		RECT rectWindow, retRect;
		int x=0;
		int y=0;
		GetWindowRect(hWnd, &rectWindow);


		rectWindow.right-=rectWindow.left;
		rectWindow.bottom-=rectWindow.top;

		color_t* windowImage;
		HDC hdc=cyiBitmapBlender::CreateMemoryDC(rectWindow.right, rectWindow.bottom, &windowImage);
		SendMessage(hWnd, WM_PRINT, (WPARAM)hdc, (LPARAM)PRF_CLIENT|PRF_CHILDREN|PRF_ERASEBKGND);

		ScreenToClient(m_hWnd, (POINT*)&rectWindow);
		x=rectWindow.left;
		y=rectWindow.top;

		if (xIsInRect(&rectWindow, childRect, retRect)==False)
		{
			cyiBitmapBlender::DestroyMemoryDC(hdc);
			continue;
		}


		HBITMAP hBmp=(HBITMAP)GetCurrentObject(hdc, OBJ_BITMAP);
		Bitmap tempBmp((HBITMAP)hBmp, 0);
		Graphics g(bmp);

		g.DrawImage(&tempBmp, retRect.left, retRect.top, 
				retRect.left-x, retRect.top-y, retRect.right, retRect.bottom, UnitPixel);

		cyiBitmapBlender::DestroyMemoryDC(hdc);
	}

SKIP_WINDOW_DRAW:
	if (m_children==0 ||
			m_children->count()==0)
	{
		m_eUpdateReason=eGUR_NO_REASON;
//#ifdef _DEBUG
//		xTRACE("Update(%s, %d, %d, %d, %d):%d Exit", m_szClassName, childRect->left, childRect->top, childRect->right, childRect->bottom, GetDrawOrder());
//#endif
		return ;
	}

	for (cyiNode* node=m_children->GetNodeTopFrom(); node; node=node->np)
	{
		cGlassInterface* obj = (cGlassInterface*)node->ref;


		//if (obj->m_bIsNotOwnDraw==False)
		//{
		//	// WM_PAINTメッセージで処理するようにさせる。

		//	continue;
		//}

		// これで良いかどうか。。。
		if (IsWindowVisible(obj->m_hWnd)==False)
		{
			continue;
		}

		RECT crect = {obj->m_x, obj->m_y, obj->m_w, obj->m_h};
		RECT retRect;


		if (xIsInRect(&crect, childRect, retRect)==False)
		{
			continue;
		}
		int sw=0, sh=0;

		RECT xrect = {retRect.left+m_nAbsX-offsetX, retRect.top+m_nAbsY-offsetY, retRect.right, retRect.bottom};
		retRect.left-=obj->m_x;
		retRect.top-=obj->m_y;
#ifdef _DEBUG
		//xTRACE("iam:%s(%d), obj:%s(%d) Enter", m_szClassName, GetDrawOrder(), obj->m_szClassName, obj->GetDrawOrder());
#endif
		obj->Update(bmp, &xrect, &retRect, offsetX, offsetY);
		/*if (obj->m_item.fScale==1.0)
		{
			obj->Update(bmp, &xrect, &retRect, offsetX, offsetY);
		}
		else if (obj->m_item.fScale==0.0)
		{
			continue;
		}
		else
		{
			int w = (int)(obj->m_item.fScale*(float)xrect.right);
			int h = (int)(obj->m_item.fScale*(float)xrect.bottom);

			if (w==0 ||
					h==0)
					continue;

			RECT xxrect = {0, 0, xrect.right, xrect.bottom};
			Bitmap* xbmp = BITMAP32(xrect.right, xrect.bottom);
			int x = (xrect.right-w)>>1;
			int y = (xrect.bottom-h)>>1;
			obj->Update(xbmp, &xxrect, &retRect, offsetX, offsetY);
			Graphics g(bmp);
			
			g.DrawImage(xbmp, x, y, w, h);
			delete xbmp;
		}*/
#ifdef _DEBUG
		//xTRACE("iam:%s(%d), obj:%s(%d) Exit", m_szClassName, GetDrawOrder(), obj->m_szClassName, obj->GetDrawOrder());
#endif

		if (xIS_GLS_FLAG(obj->m_flag, eGF_REQUEST_UPDATE)==True)
		{
			//if (m_glsDrawInterface)
			//	m_glsDrawInterface->m_nRequestUpdateCount++;
			//else
			//	m_nRequestUpdateCount++;
			xOFF_GLS_FLAG(obj->m_flag, eGF_REQUEST_UPDATE);
		}
	}

//#ifdef _DEBUG
//		xTRACE("Update(%s, %d, %d, %d, %d):%d Exit", m_szClassName, childRect->left, childRect->top, childRect->right, childRect->bottom, GetDrawOrder());
//#endif

	m_eUpdateReason=eGUR_NO_REASON;//eGUR_BG_CHANGED
	return ;
}

// -----------------------------
Bool cGlassInterface::GetMeasureSize(int&w, int&h)
{
	w=m_item.rect.w;
	h=m_item.rect.h;

	if (w==0 ||
			h==0)
	{
		return False;
	}

	return True;
}

// -----------------------------
cglsSystem* cGlassInterface::GetSystem()
{
	if (m_system!=0)
		return m_system;

	if (m_parent==0)
		return 0;

	return m_parent->GetSystem();
}

// -----------------------------
void cGlassInterface::RegisterSystem()
{
	//m_system=(cglsSystem*)this;
	m_system = new cglsSystem();
}


// -----------------------------
Bool cGlassInterface::GetEffectedBg(Bitmap* dest, int x, int y, int sx, int sy, int w, int h)
{
	return False;
}


// -----------------------------
void cGlassInterface::SetGuiStyle(eGlsGuiStyle style, Bool bSetIamOnly)
{
	m_guiStyle=style;

	if (bSetIamOnly==False)
	{
		xNODELOOP(node, m_children)
		{
			cGlsp child = (cGlsp)node->ref;
			child->SetGuiStyle(style, False);
		}
	}
}

// -----------------------------
int cGlassInterface::GetChildCount()
{
	if (m_children==0)
		return 0;

	return m_children->count();
}


// -----------------------------
eGlsGuiStyle cGlassInterface::GetGuiStyle()
{
	if (m_guiStyle==eGGS_DEFAULT &&
			m_parent!=0)
		return m_parent->m_guiStyle;

	return m_guiStyle;
}

// -----------------------------
cGlassInterface* cGlassInterface::GetDrawWindow(HWND& out_hWnd)
{
	if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==False)
	{
		return this;
		//cGlassInterface* ret;
		//if (m_parent==0)
		//{
		//	return this;
		//}

		//if ((ret=m_parent->GetDrawWindow(out_hWnd))==0)
		//{
		//	return this;
		//}
		/*return ret;*/
	}

	if (m_parent==0)
	{
		HWND hWnd = GetParent(m_hWnd);

		out_hWnd=hWnd;
		return 0;
	}

	return m_parent->GetDrawWindow(out_hWnd);
}

typedef struct {
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
	int		nDrawNo;
	Bool	bIndependent;
} xRECT;

// ------------------------------------
Bool cGlassInterface::InvalidateRect(RECT* lpRect, Bool bIsBg, Bool bIsDirect, Bool bIsForced)
{
	int x, y;
	xRECT _rect={0, 0, m_w, m_h, bIsForced==True?0:GetDrawOrder(), xIS_GLS_FLAG(m_flag,eGF_UPDATE_INDEPENDENT)};

//#ifdef YIDEBUG
//		YIDEBUG("InvalidateRect(%s)", m_szClassName);//, childRect->left, childRect->top, childRect->right, childRect->bottom, GetDrawOrder());
//#endif

	if (m_parent==0)
	{
		if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==True)
		{
			return ::InvalidateRect(m_hWnd, lpRect, False);
		}
	}

	//if (IsWindowVisible(*this)==False)
	//{
	//	return False;
	//}

	if (_rect.nDrawNo==-1)
	{
		return False;
	}

	if (lpRect!=0)
	{
		memcpy(&_rect, lpRect, sizeof(RECT));
	}

	if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==False)
	{
		//if (bErase==0)
		//{
		MutexLock(True);
		if (m_pItemRects==0)
		{
			m_pItemRects=new cyiQueue();
		}
		//xTRACE("InvalidateRect (%d, %d, %d, %d, %d) cnt:%d", _rect.left, _rect.top, _rect.right, _rect.bottom, _rect.nDrawNo, m_pItemRects->count());

		xRECT* pRect=(xRECT* )yitoheap((void*)&_rect, sizeof(_rect));

		
		if (bIsDirect==True)
		{
			m_pItemRects->insert(0, (yiAnyValue)pRect);
			SendMessage(m_hWnd, WM_GLS_PAINT, bIsBg, (LPARAM)pRect);
		}
		else
		{
			m_pItemRects->enq((yiAnyValue)pRect);
			PostMessage(m_hWnd, WM_GLS_PAINT, bIsBg, (LPARAM)pRect);
		}
		MutexUnlock();

		return True;
		//}
		//else
		//{
		//	cyctrlbase::InvalidateRect(lpRect, bErase);
		//	return True;
		//}
	
	}

	int sx=0, sy=0;
	if (m_parent)
	{
		m_parent->GetScrollPos(sx, sy);
	}

	_rect.left+=(x=(m_nAbsX-sx));
	_rect.top+=(y=(m_nAbsY-sy));
	_rect.right+=x;
	_rect.bottom+=y;

	if (m_glsDrawInterface)
	{
		xSET_GLS_FLAG(m_flag, eGF_REQUEST_UPDATE, True);
		if (m_glsDrawInterface->m_pItemRects==0)
		{
			m_glsDrawInterface->m_pItemRects=new cyiQueue();
		}
#ifdef _DEBUG
		xTRACE("InvalidateRect2 (%s:%s)", m_parent->m_szClassName, m_szClassName);
#endif
		//xTRACE("InvalidateRect2 (%d, %d, %d, %d, %d) cnt:%d", _rect.left, _rect.top, _rect.right, _rect.bottom, _rect.nDrawNo, m_glsDrawInterface->m_pItemRects->count());
		xRECT* pRect=(xRECT* )yitoheap((void*)&_rect, sizeof(_rect));
		

		if (bIsDirect==True)
		{
			m_glsDrawInterface->m_pItemRects->insert(0,(yiAnyValue)pRect);
			SendMessage(*m_glsDrawInterface, WM_GLS_PAINT, bIsBg, (LPARAM)pRect);
		}
		else
		{
			m_glsDrawInterface->m_pItemRects->enq((yiAnyValue)pRect);
			PostMessage(*m_glsDrawInterface, WM_GLS_PAINT, bIsBg, (LPARAM)pRect);
		}
		//m_glsDrawInterface->InvalidateRect(&rect, bErase, bIsDirect);
	}

	return True;
}

// --------------------------------
void cGlassInterface::Hide()
{
	InvalidateRect(0);
	//xSET_ITEM_HIDE((&m_item));
	cyctrlbase::Hide();
}

// --------------------------------
void cGlassInterface::Show()
{
	cyctrlbase::Show();
	//xSET_ITEM_SHOW((&m_item));
	InvalidateRect(0);
}


// --------------------------------
void cGlassInterface::Destroy()
{
	if (m_bIsCreated==False)
		return ;

	if (this->m_hAnitShow)
	{
		if (::yiAnitIsAlived(m_hAnitShow)==True)
		{
			yiAnitWait(m_hAnitShow);
			//m_bIsRequestDestroy=True;
			//return ;
		}
	}

	if (m_hAnitShow)
	{
		yiAnitDestroy(m_hAnitShow);
		m_hAnitShow=0;
	}
	
	m_bIsCreated=False;

	if (m_parent)
	{
		m_parent->UnRegisterChild(this);
	}

	if (m_children)
	{
		for (cGlassInterface* obj = (cGlassInterface*)m_children->removeEx(0); obj; obj=(cGlassInterface*)m_children->removeEx(0))
		{
			obj->Destroy();
			// TODO. 
			//delete obj;
		}

		//for (cyiNode* node=m_children->GetNodeTopFrom(0); node; node=node->np)
		//{
		//	
		//	cGlassInterface* obj = (cGlassInterface* )node->ref;
		//	obj->Destroy();
		//	delete obj;
		//}
		m_children->clear();
		delete m_children;
		m_children=0;
	}

	cyctrlbase::Destroy();

	if (m_DrawObject)
	{
		m_DrawObject->DestroyBitmap(&m_item);
	}
	else
	{
		for (int i=0; i<3; i++)
			if (m_item.pBmp[i])
			{
				m_item.pBmp[i]=0;
				delete m_item.pBmp[i];
			}
	}

	memset(&m_item, 0, sizeof(m_item));


	//if (m_bg)
	//	delete m_bg;

	if (m_bmpCurrent)
		delete m_bmpCurrent;

	m_bmpCurrent=0;

	if (m_bmpBg)
	{
		delete m_bmpBg;
	}
	m_bmpBg=0;

	if (m_hMutex)
		yiMutexDestroyEx(m_hMutex);

	m_hMutex=0;
	//if (GetBgBitmap()!=0)
	//	delete GetBgBitmap();
}

// ------------------------------------
Bool cGlassInterface::IsBgAnimation()
{
	return False;
}

// -----------------------------
void cGlassInterface::Update(eGlsUpdateMode mode, eDrawItemStatus status)
{
	Bitmap* prevBmp=0;

	if (m_bIsCreated==False)
		return ;



	// cglsDrawObjectを設定してない場合は、無効。
	if (m_DrawObject==0 &&  mode==eGUM_FORCED)
	{
		//m_eUpdateReason=eGUR_NO_REASON;
		//InvalidateRect(0);

		if (m_children)
		{
			for (cyiNode* node=m_children->GetNodeTopFrom(); node; node=node->np)
			{
				cGlassInterface* obj = (cGlassInterface*)node->ref;
				obj->Update(mode, status);
			}
		}

		return ;
	}

	if (mode==eGUM_UPDATE)
	{
		if (m_DrawObject &&
				m_item.pBmp[0]==0)
		{
			mode=eGUM_FORCED;
		}
	}


	m_eUpdateReason=eGUR_NO_REASON;
	// スケールの変化がある場合
	// 新たなビットマップを生成する。
	if (mode==eGUM_FORCED)
	{
		m_eUpdateReason=eGUR_FORCED;
		// redraw
		if (m_DrawObject==0)
			return ;

		if (m_DrawObject->CreateBitmap(&m_item)==False)
		{
			return ;
		}

		if (ReSize(m_item.pCurrentBmp->GetWidth(), m_item.pCurrentBmp->GetHeight())==True)
		{
			if (xIS_GLS_FLAG(m_flag, eGF_DRAW_MYWAY)==False)
			{
				/*if (m_bmpCurrent)
					delete m_bmpCurrent;
				m_bmpCurrent = new Bitmap(m_w, m_h, PixelFormat32bppARGB);*/
			}
		}
		else
		{
			/*if (xIS_GLS_FLAG(m_flag, eGF_DRAW_MYWAY)==False)
			{
				if (m_bmpCurrent==0)
				{
					m_bmpCurrent = new Bitmap(m_w, m_h, PixelFormat32bppARGB);
				}
				cyiBitmapBlender::FillColor(m_bmpCurrent, 0);
			}*/
		}

		status=eDIS_FORCED;
	}
	else
	{
		if (m_item.pBmp[0]==0)
			return ;
	}

	Bool bIsNoAnimation=False;
	Bitmap* pBmpSpecial=0;
	int nIndex=GetStatusToBitmapIndex(&m_item, status, bIsNoAnimation);

	if (nIndex==-1)
	{
		return ;
	}

	if (bIsNoAnimation==False &&
		xIS_GLS_FLAG(m_flag, eGF_NO_ANIMATION)==False)
	{
		sGlsAnimationEx anit;
		glsInitAnimationEx(&anit, &m_item);
		eGlsAnimationPattern pattern = nIndex==1?eGAP_FISO:eGAP_SIFO;

		
		glsSetDvAnimationEx(&anit, m_item.nLastItemIndex, nIndex, pattern);

		if (m_item.fEnterTransparency!=m_item.fLeaveTransparency)
		{

			glsSetTrAnimationEx(&anit, nIndex==1?m_item.fLeaveTransparency:m_item.fEnterTransparency,
				nIndex==1?m_item.fEnterTransparency:m_item.fLeaveTransparency, pattern);
		}

		m_item.nLastItemIndex=nIndex;
		
		StartAnimation(&m_item, &anit, m_nAnimationMilliSec, nIndex==1?this->m_nMouseMoveAnimationInterval:m_nMouseLeaveAnimationInterval);

		return ;
	}


	// no animation.
	{
		int sx=0, sy=0;

		StopAnimation(&m_item, True);
		m_item.pCurrentBmp=m_item.pBmp[nIndex];
		m_item.nLastItemIndex=nIndex;
		//cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_item.pCurrentBmp);

		RECT rect = {sx, sy, m_w, m_h};
		InvalidateRect(&rect, False);
		return;
	}
	
	//{
	//	if (m_item.hAnit==0)
	//	{
	//		m_item.hAnit=yiAnitCreate();
	//	}


	//	CrossDraw(&m_item, m_item.animation->pBmpAnimation?m_item.animation->pBmpAnimation:prevBmp, m_item.pCurrentBmp);
	//}

	return ;
}



// ------------------------------------
static Bool xGetCombineRect(RECT* result, RECT* rect1, RECT* rect2)
{
	RECT rectResult=*result;
	int nRectSize = (rect1->right-rect1->left)*(rect1->bottom-rect1->top)+(rect2->right-rect2->left)*(rect2->bottom-rect2->top);
	
	result->left=rect1->left<rect2->left?rect1->left:rect2->left;
	result->top=rect1->top<rect2->top?rect1->top:rect2->top;
	result->right=rect1->right<rect2->right?rect2->right:rect1->right;
	result->bottom=rect1->bottom<rect2->bottom?rect2->bottom:rect1->bottom;

	int nCombinedSize = (result->right-result->left)*(result->bottom-result->top);

	if (nCombinedSize<=nRectSize)
	{
		return True;
	}

	//xTRACE("size rate is.... %f", (float)nCombinedSize/(float)nRectSize);

	if ((float)nCombinedSize<=(float)nRectSize*1.5)
	{
		return True;
	}
	*result=rectResult;
	return False;
}


// ------------------------------------
Bool cGlassInterface::xGetDrawRect(RECT* result)
{
	int nRectCount = m_pItemRects->count();
	int nMinZorder=1024;
	Bool bIsMaxInvalidateRect=False;

	if (nRectCount==1)
	{
		yiAnyValue val;
		m_pItemRects->deq(val);
		return False;
	}

	for (cyiNode* node=m_pItemRects->GetNodeTopFrom(); node; node=node->np)
	{
		xRECT* pRect=(xRECT*)node->data;

		if (pRect->right==m_w &&
				pRect->bottom==m_h)
		{
			// この場合は統合して一度のみでよい。
			// 起動時やサイズ変更など頻繁に呼び出される場合、有効な手法である。
			bIsMaxInvalidateRect=True;
		}
		nMinZorder=nMinZorder>pRect->nDrawNo?pRect->nDrawNo:nMinZorder;
	}

	if (bIsMaxInvalidateRect)
	{
		xRECT* pRect=0;
		yiAnyValue val;
		//while((pRect=(xRECT*)m_pItemRects->removeEx(0))!=0) yifree(pRect);
		m_pItemRects->deq(val);
		while(m_pItemRects->deq(val)!=False) yifree((void*)val);
		result->left=0;
		result->top=0;
		result->right=m_w;
		result->bottom=m_h;
		m_nRequestUpdateOrder=nMinZorder;
		return True;
	}

	yiAnyValue val;
	m_pItemRects->deq(val);
	xRECT* pRectTop = (xRECT*)val;

	{
		xRECT* pRect=pRectTop;
		xRECT* pRectAfter=(xRECT*)m_pItemRects->GetNodeTopFrom(0)->data;
		Bool bIsResult=False;

		m_nRequestUpdateOrder = pRect->nDrawNo;

		//if (pRectAfter->bIndependent==True)
		//{
		//}

		while(pRectAfter!=0 && pRectAfter->bIndependent==False)
		{
			if (xGetCombineRect(result, (LPRECT)pRect, (LPRECT)pRectAfter)==True)
			{
				bIsResult=True;
				//*pRect=*result;

				memcpy(pRect, result, sizeof(RECT));
				
				//xTRACE("cGlassInterface::xGetDrawRect() duplicate area.... (%d,%d,%d,%d):%d+(%d,%d,%d,%d)=(%d,%d,%d,%d)",
				//				pRect->left, pRect->top, pRect->right, pRect->bottom, pRect->nDrawNo, 
				//					pRectAfter->left, pRectAfter->top, pRectAfter->right, pRectAfter->bottom, result->left, result->top, result->right, result->bottom);
				m_nRequestUpdateOrder=m_nRequestUpdateOrder<pRectAfter->nDrawNo?m_nRequestUpdateOrder:pRectAfter->nDrawNo;

				yiAnyValue val;
				m_pItemRects->deq(val);
				yifree((void*)val);

				cyiNode* node=m_pItemRects->GetNodeTopFrom(0);

				if (node==0)
				{
					return True;
				}

				pRectAfter=(xRECT*)node->data;
			}
			else
			{
				return bIsResult;
			}

		} // while
	}

	return False;
}

// ------------------------------------
void OnPaintExBg(void* p)
{
	unsigned int* x = (unsigned int*)p;
	cGlassInterface* pClass = (cGlassInterface*)x[0];

	Bool handled=False;
	pClass->OnPaintEx(0, x[1], handled);

	yiBinaryDelete(p);
}

// ------------------------------------
LRESULT cGlassInterface::OnPaintEx(WPARAM wp, LPARAM lp, Bool& handled)
{
	handled=True;

	
	//if (wp==1) // call from bg.
	//{
	//	cGlassInterface* obj = this;
	//	Binary p = yiBinary(&obj, 4, &lp, 4, 0);
	//	_beginthread(OnPaintExBg, 0, p);
	//	return 0;
	//}

	//MutexLock(True);
	yiMutexLockEx(m_hPaintMutex);

	xRECT* lpRect=(xRECT*)lp;

	RECT rectResult;

	if (m_pItemRects->count()==0)
		return True;

	// おかしくなった場合
	if (m_pItemRects->GetNodeTopFrom()==0)
	{
		m_pItemRects->clear();
		yiMutexUnlockEx(m_hPaintMutex);
		return 0;
	}

	if (lpRect!=(xRECT*)m_pItemRects->GetNodeTopFrom()->data)
	{
		yiMutexUnlockEx(m_hPaintMutex);

		return True;
	}

	m_nRequestUpdateOrder=lpRect->nDrawNo;

	if (lpRect->bIndependent==False &&
			xGetDrawRect(&rectResult)==True)
	{
		//*lpRect=rectResult;
		memcpy(lpRect, &rectResult, sizeof(RECT));
	}

	if (lpRect->bIndependent==True)
	{
		yiAnyValue val;
		m_pItemRects->deq(val);
	}

	HDC hdc = GetDC();
	// ここからは自己描画機能を持つWindow
	int x=lpRect->left;
	int y=lpRect->top;
	int w=lpRect->right-lpRect->left;
	int h=lpRect->bottom-lpRect->top;


	Bitmap* bg = GetBgBitmap();

	//if (bg)
	//{
	//	cyiBitmapBlender::BitmapCopy(m_bmpCurrent, bg, x, y, x, y, w, h);//==0)
	//}

	RECT rect = {x, y, w, h};
	//m_nRequestUpdateCount=0;


	// ここでもm_bmpCurrentがゼロの場合は、
	// バッファーのための役割として残す。
	if (m_bmpCurrent==0)
	{
		m_bmpCurrent=BITMAP32(m_w, m_h);
	}

	if (m_bmpCurrent->GetWidth()!=m_w ||
			m_bmpCurrent->GetHeight()!=m_h)
	{
		m_bmpCurrent=BITMAP32(m_w, m_h);
	}

	//if (m_glsDrawInterface==this)
	//{
	//	Update(m_bmpCurrent, &rect);
	//}
	//else
	//{
	//xTRACE("OnPaintEx Update(%d, %d, %d, %d)......... from:%d", rect.left, rect.top, rect.right, rect.bottom, m_nRequestUpdateOrder);
	Update(m_bmpCurrent, &rect, 0, m_nAbsX, m_nAbsY);
	m_nRequestUpdateOrder=0;
	//}

	//m_nRequestUpdateCount=0;

	if (OnUpdateAfter(hdc)==True)
	{
		yifree(lpRect);
		yiMutexUnlockEx(m_hPaintMutex);
		return True;
	}

	Graphics g(hdc);

	RECT rectChanged;
	if (GetChangedDrawArea(&rectChanged)==True)
	{
		yiUtilGetAddRect2(&rect, &rectChanged, &rect);
		x=rect.left;
		y=rect.top;
		w=rect.right;
		h=rect.bottom;
	}

	g.DrawImage(m_bmpCurrent, x, y, x, y, w, h, Unit::UnitPixel);
	//g.DrawImage(m_bmpCurrent, 0, 0, 0, 0, m_w, m_h, UnitPixel);//, x, y, w, h, Unit::UnitPixel);
	ReleaseDC(hdc);

	yifree(lpRect);
	yiMutexUnlockEx(m_hPaintMutex);


	return True;
}

// ------------------------------------
Bool cGlassInterface::OnUpdateAfter(HDC hdc)
{
	return False;
}


// ------------------------------------
Bool cGlassInterface::GetChangedDrawArea(RECT* rect)
{
	if (m_listChangeDrawArea.count()==0)
	{
		return False;
	}

	yiAnyValue val;
	RECT rectSum={0, 0,0, 0};
	while(m_listChangeDrawArea.deq(val)==True)
	{
		RECT* pRect=(RECT*)val;
		yiUtilGetAddRect(&rectSum, 0, pRect);
	}
	*rect=rectSum;
	return True;
}

// ------------------------------------
Bool cGlassInterface::SetChangedDrawArea(RECT* rect)
{
	m_listChangeDrawArea.enq((yiAnyValue)yitoheap(rect, sizeof(RECT)));
	return True;
}

// --------------------------------------------
Bool cGlassInterface::PrevFocus()
{
	cGlassInterface* parent = this;

	while(parent!=0 &&
			xIS_GLS_FLAG(parent->m_flag, eGF_FOCUS_MANAGE)==False)
		parent=parent->m_parent;

	if (parent==0)
	{
		return False;
	}

	if (parent->m_focusChildren==0)
		return False;

	cGlassInterface* prev=0;

	Bool bImFound=False;
	for (cyiNode* node=parent->m_focusChildren->GetNodeTailFrom(0); node; node=node->pp)
	{
		cGlassInterface* obj = (cGlassInterface*)node->ref;
		if (IsWindowVisible(*obj)==FALSE ||
			IsWindowEnabled(*obj)==FALSE)
		{
			continue;
		}

		if (bImFound==True)
		{
			prev=obj;
			break;
		}

		if (obj==this)
		{
			bImFound=True;
		}
	}

	if (prev==0)
	{
		for (cyiNode* node=parent->m_focusChildren->GetNodeTailFrom(0); node; node=node->pp)
		{
			cGlassInterface* obj = (cGlassInterface*)node->ref;
			if (IsWindowVisible(*obj)==FALSE ||
				IsWindowEnabled(*obj)==FALSE)
			{
				continue;
			}

			if (obj==this)
			{
				prev=this;
				break;
			}

			prev=obj;
			break;
		}
	}

	if (prev==this)
		return False;

	SetFocus(*prev);
	prev->OnMouseEnter(0, 0);
	OnMouseLeave(0, 0);
	return True;
}

// --------------------------------------------
Bool cGlassInterface::IsDblClicked()
{
	return m_bIsDblClicked;
}

// --------------------------------------------
Bool cGlassInterface::NextFocus()
{
	cGlassInterface* parent = this;

	while(parent!=0 &&
			xIS_GLS_FLAG(parent->m_flag, eGF_FOCUS_MANAGE)==False)
		parent=parent->m_parent;

	if (parent==0)
	{
		return False;
	}

	if (parent->m_focusChildren==0)
		return False;

	cGlassInterface* next=0;

	Bool bImFound=False;
	for (cyiNode* node=parent->m_focusChildren->GetNodeTopFrom(); node; node=node->np)
	{
		cGlassInterface* obj = (cGlassInterface*)node->ref;
		if (IsWindowVisible(*obj)==FALSE ||
			IsWindowEnabled(*obj)==FALSE)
		{
			continue;
		}

		if (bImFound==True)
		{
			next=obj;
			break;
		}

		if (obj==this)
		{
			bImFound=True;
		}
	}

	if (next==0)
	{
		for (cyiNode* node=parent->m_focusChildren->GetNodeTopFrom(); node; node=node->np)
		{
			cGlassInterface* obj = (cGlassInterface*)node->ref;
			if (IsWindowVisible(*obj)==FALSE ||
				IsWindowEnabled(*obj)==FALSE)
			{
				continue;
			}

			if (obj==this)
			{
				next=this;
				break;
			}

			next=obj;
			break;
		}
	}

	if (next==this)
		return False;

	SetFocus(*next);
	next->OnMouseEnter(0, 0);
	OnMouseLeave(0, 0);
	return True;

/*	if (xIS_GLS_FLAG(flag, eGF_FOCUS_MANAGE)==True)
	{
		return False;
	}

	Bool bIsFoundMe=False;
	for (cyiNode* node=m_parent->m_children->GetNodeTopFrom();node; node=node->np)
	{
		cGlassInterface* obj = (cGlassInterface*)node->ref;
		if (IsWindowVisible(*obj)==FALSE ||
			IsWindowEnabled(*obj)==FALSE)
		{
			continue;
		}

		if (bIsFoundMe==True)
		{
			if (xIS_GLS_FLAG(obj->m_flag, eGF_FOCUS_STOP)==True)
			{
				SetFocus(obj->m_hWnd);
				obj->OnMouseEnter(0, 0);
				OnMouseLeave(0, 0);
				return True;
			}

			if (m_children)
			{
				for (cyiNode* node=m_children->GetNodeTopFrom(); node; node=node->np)
				{
					cGlassInterface* obj = (cGlassInterface*)node->ref;
					if (IsWindowVisible(*obj)==FALSE ||
						IsWindowEnabled(*obj)==FALSE)
					{
						continue;
					}

					if (xIS_GLS_FLAG(obj->m_flag, eGF_FOCUS_STOP)==True)
					{
						SetFocus(obj->m_hWnd);
						obj->OnMouseEnter(0, 0);
						OnMouseLeave(0, 0);
						return True;
					}
				}
			}

		}

		if (node->ref==(void*)this)
		{
			bIsFoundMe=True;
		}
	}

	if (m_parent->NextFocus()==True)
	{
		return True;
	}



	//if (bIsFoundMe==True)
	//{
	//	for (cyiNode* node=m_parent->m_children->GetNodeTopFrom();node; node=node->np)
	//	{
	//		cGlassInterface* obj = (cGlassInterface*)node->ref;
	//		if (IsWindowVisible(*obj)==FALSE ||
	//			IsWindowEnabled(*obj)==FALSE)
	//		{
	//			continue;
	//		}
	//		if (bIsFoundMe==True)
	//		{
	//			if (xIS_GLS_FLAG(obj->m_flag, eGF_FOCUS_STOP)==True)
	//			{
	//				SetFocus(obj->m_hWnd);
	//				obj->OnMouseEnter(0, 0);

	//				return ;
	//			}
	//		}

	//		if (node->ref==(void*)this)
	//		{
	//			SetFocus(m_hWnd);
	//			return ;
	//		}
	//	}
	//}*/
}

#include <Uxtheme.h>
#pragma comment(lib, "UxTheme.lib")

// ------------------------------------
Bool cGlassInterface::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	//RECT zrect = {ps->rcPaint.left, ps->rcPaint.top, ps->rcPaint.right, ps->rcPaint.bottom};
	//InvalidateRect(&zrect);
	//return True;
	if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==True)
	{
		// 何も行わない。
		// これが意味するのは子供はWM_PAINTの処理を行わない。

		return True;
	}


	//if (xIS_GLS_FLAG(m_flag, eGF_BUFFERED_PAINT)==True)
	//{
	//	// XPはここには入らないはず。
	//	 HDC targetDC = hdc;
	//	 HDC bufferedDC = NULL;
	//	 RECT rect;
	//	 GetClientRect(&rect);
	//	 HPAINTBUFFER pb = BeginBufferedPaint(targetDC, &rect, 
	//		 BPBF_TOPDOWNDIB, 0,&bufferedDC);

	//	 if (pb==0)
	//		 return 0;

	//	 SendMessage(m_hWnd, WM_PRINTCLIENT, 
 //        reinterpret_cast<WPARAM>(bufferedDC), PRF_CLIENT);
	//	 //ちょっと内側の矩形を作成
	//	 RECT rr;
	//	 rr.left = rect.left + 3;
	//	 rr.right = rect.right -3;
	//	 rr.top = rect.top + 3;
	//	 rr.bottom = rect.bottom -3;
	//	 //Alpha値を書き込む
	//	 HRESULT hr = BufferedPaintSetAlpha(pb, &rr, 255);
	//	 if (hr != S_OK){
	//		 //error
	//	 }

	//	Graphics g(bufferedDC);
	//	g.DrawImage(m_bmpCurrent, 0, 0, 0, 9, m_w, m_h, Unit::UnitPixel);
	//	 hr = EndBufferedPaint(pb, TRUE);
	//	 if (hr != S_OK){
	//		 //error
	//	 }
	//	
	//	 return 0;

	//}

	//return True;

	//yiMutexLockEx(m_hPaintMutex);
	// ここからは自己描画機能を持つWindow
	int x=ps->rcPaint.left;
	int y=ps->rcPaint.top;
	int w=ps->rcPaint.right-ps->rcPaint.left;
	int h=ps->rcPaint.bottom-ps->rcPaint.top;

	{
		Graphics g(hdc);
		if (m_bmpCurrent)
		{
			g.DrawImage(m_bmpCurrent, x, y, x, y, w, h, Unit::UnitPixel);
		}
		return True;
	}

	Bitmap* bg = GetBgBitmap();

	if (bg)
	{
		cyiBitmapBlender::BitmapCopy(m_bmpCurrent, bg, x, y, x, y, w, h);
	}

	RECT rect = {x, y, w, h};
	//m_nRequestUpdateCount=0;
	m_nRequestUpdateOrder=1024;
	//xTRACE("cGlassInterface::OnPaint(%d,%d, %d, %d)", rect.left, rect.top, rect.right, rect.bottom);
	Update(m_bmpCurrent, &rect);
	//m_nRequestUpdateOrder=0;


	Graphics g(hdc);
	g.DrawImage(m_bmpCurrent, x, y, x, y, w, h, Unit::UnitPixel);

	//yiMutexUnlockEx(m_hPaintMutex);
	return True;
}

// ------------------------------------
Font* cGlassInterface::GetFont()
{
	//static Font font(L"Meyrio", 11.0); // glass interface default font.

	if (m_font)
		return m_font;

	if (m_parent)
		return m_parent->GetFont();


	return 0;//&font;
}

// ------------------------------------
cglsResourceObject* cGlassInterface::GetResource()
{
	if (m_resource)
		return m_resource;

	if (m_parent)
		return m_parent->GetResource();

	return 0;
}

// ------------------------------------
void cGlassInterface::InitResource()
{
	if (m_DrawObject)
	{
		//m_font = m_DrawObject->m_xfont;
		m_resource = m_DrawObject->m_xresource;

		if (m_font==0)
		{
			m_DrawObject->m_xfont=GetFont();
		}

		if (m_resource==0)
		{
			m_DrawObject->m_xresource=GetResource();
		}
	}
}


// ------------------------------------
Bool cGlassInterface::SetItemStatus(sDrawItem* item, eDrawItemStatus status)
{
	eDrawItemStatus itemStatus = xGET_ITEM_STATUS(item);
	eDrawItemStatus prevItemStatus = xGET_ITEM_PRE_STATUS(item);

	if (itemStatus==status)
	{
		return False;
	}

	if (status==eDIS_PREV)
	{
		xSET_ITEM_STATUS(item, prevItemStatus);
		xSET_ITEM_PRE_STATUS(item, 0);
		return True;
	}

	xSET_ITEM_PRE_STATUS(item, itemStatus);
	xSET_ITEM_STATUS(item, status);

	return True;
}

// ------------------------------------
int cGlassInterface::RegisterChild(cGlassInterface* child)
{
	if (IsWindow((HWND)child)==TRUE)
	{
		if (m_childwindow==0)
			m_childwindow=new cyiList();
		m_childwindow->addref(child);
		return m_childwindow->count();
	}

	if (m_children==0)
		m_children=new cyiList();

	m_children->addref(child);
	return m_children->count();
}

// ------------------------------------
void cGlassInterface::UnRegisterChild(cGlassInterface* child)
{
	if (IsWindow((HWND)child)==TRUE)
	{
		if (m_childwindow==0)
			return ;
		for (cyiNode* node=m_childwindow->GetNodeTopFrom(); node; node=node->np)
		{
			if (node->ref==(void*)child)
			{
				m_childwindow->remove(node);
				break;
			}
		}

		return ;
	}

	if (m_children==0)
		return ;

	for (cyiNode* node=m_children->GetNodeTopFrom(); node; node=node->np)
	{
		if (node->ref==(void*)child)
		{
			m_children->remove(node);
			break;
		}
	}

}

// ------------------------------------
int cGlassInterface::CalculateFontHeight(Font* font, Bool bIsGDI)
{
	if (bIsGDI==False)
		return font->GetHeight(__DEFAULTGRAPHICS__);

	unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, L"1", font, 0, 0);
	return (pos&0xFFFF);
}

// ------------------------------------
int cGlassInterface::SetFont(Font* font, Bool bIsFontReference)
{
	if (m_font)
		delete m_font;

	if (font==0)
	{
		m_font=0;
		return 0;
	}
	
	m_font=(Font*)cyiShape::Clone(font);//font->Clone();


	m_nFontHeight = CalculateFontHeight(m_font, False);//m_font->GetHeight(__DEFAULTGRAPHICS__);

	//unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, L"1", m_font, 0, 0);
	m_nFontHeightGDI = CalculateFontHeight(m_font, True);//(pos&0xFFFF);

	//if (bIsFontReference==True)
	//{
	//	Update(eGUM_FORCED);
	//	InvalidateRect(0);
	//	return 0;
	//}


	if (m_children==0)
	{
		Update(eGUM_FORCED);
		InvalidateRect(0);
		return 0;
	}

	for (cyiNode* node=m_children->GetNodeTopFrom();node;node=node->np)
	{
		cGlassInterface* obj=(cGlassInterface*)node->ref;
		obj->OnFontChanged();
	}

	Update(eGUM_FORCED);

	if (xIS_GLS_FLAG(m_flag, eGF_FONT_FROM_PARENT)==False)
	{
		InvalidateRect(0);
	}

	return 0;
}

// -----------------------------
void cGlassInterface::OnFontChanged()
{
	// SetFont(m_parent->GetFont());
}

// -----------------------------
int cGlassInterface::GetDrawOrder()
{
	int sum=0;
	cGlassInterface* child	= this;
	cGlassInterface* parent = m_parent;
	while(parent)
	{
		if (parent->m_children==0)
		{
			return sum;
		}

		int index = parent->m_children->GetReferenceIndex(child);

		if (index==-1)
		{
			return -1;
		}
		sum+=(index+1);
		child=parent;
		parent=parent->m_parent;
	}

	return sum;
}

// -----------------------------
Bool cGlassInterface::GetMeasureRect(RECT* rect)
{
	return False;
}


// -----------------------------
//Bool cGlassInterface::AddPoint(int id)
//{
//	if (m_listPoint==0)
//	{
//		m_listPoint = new cyiStaticList<unsigned long>(False);
//	}
//	
//	unsigned long val = (id<<16);
//	m_listPoint->Add(&val);
//
//	return True;
//}
//
//
//// -----------------------------
//Bool cGlassInterface::SetItemPositionLine(cGlassInterface* obj, int xid, int yid)
//{
//}
//
//// -----------------------------
//int cGlassInterface::OnSetPoint(int id)
//{
//	return 0;
//}
//
//// -----------------------------
//void cGlassInterface::ReArrangeItem()
//{
//	int n=m_listPoint->count();
//
//	for (int i=0; i<n; i++)
//	{
//		unsigned long* pVal = m_listPoint->GetTopFrom(i);
//		int x = OnSetPoint(((*pVal)>>16));
//		*pVal |= x;
//	}
//
//	while(m_ArrangedItem[cnt].obj)
//	{
//		m_ArrangedItem[cnt].obj->Move();
//	}
//}


// -----------------------------
sDrawItem* cGlassInterface::GetItem(int id)
{
	return &m_item;
}

// -----------------------------
void cGlassInterface::SetItem(int bitmapId, const wchar_t* pszText, Bool bIsUpdate)
{
	m_item.bitmapId=bitmapId;
	
	if (m_item.txt)
	{
		yifree(m_item.txt);
		m_item.txt=yistrheapW(pszText);
	}

	if (bIsUpdate==True)
	{
		Update(eGUM_FORCED);
	}
}

// --------------------------------
Bool cGlassInterface::ImEnabled()
{
	if (::IsWindowEnabled(*this)==FALSE)
		return False;

	if (m_parent)
		return m_parent->ImEnabled();

	return True;
}

// --------------------------------
void cGlassInterface::SetTransparency(float f)
{
	m_item.fTransparency=f;
	InvalidateRect(0);
}


// --------------------------------
float cGlassInterface::GetMyTransparency()
{
	float f=1.0;

	if (m_parent)
	{
		f = m_item.fTransparency*m_parent->GetMyTransparency();
		return f;
	}

	return m_item.fTransparency;
}


// --------------------------------
void cGlassInterface::SetAnimation(int nAnimationMilisec, int nAnimationInterval, eGlsAnimationPattern eAnimationPartern)
{
	//m_nAnimationMilisec=nAnimationMilisec;
	//m_nAnimationInterval = nAnimationInterval;
	//m_eAnimationPattern = eAnimationPartern;
}

// ------------------------------------
Bool cGlassInterface::MutexLock(Bool bIsWait)
{
	if (bIsWait==True)
	{
		yiMutexLockEx(m_hMutex);
		return True;
	}

	return yiMutexTryEx(m_hMutex);
}

// ------------------------------------
void cGlassInterface::MutexUnlock()
{
	//int id = GetThreadId(GetCurrentThread());
	yiMutexUnlockEx(m_hMutex);
	//xTRACE("cGlassInterface::MutexLock(%d) Leave", id);
}

typedef struct {
	float from;
	float to;
} sAnitShow;

// ------------------------------------
void cGlassInterface::OnReqDestroy()
{
}

// ------------------------------------
/*LRESULT cGlassInterface::OnAnimationShow(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	int cnt=wParam;
	sAnitShow* item = (sAnitShow*)lParam;
	float factor;

	if (item==0)
	{
		return 0;
	}

	if (m_hAnitShow==0)
		return 0;

	// 中止された場合
	if (yiAnitIsAlived(m_hAnitShow)==False)
	{
		//OnShowAnimationEnd(False, True);
		return 0;
	}

	if ((factor=yiAnitGetFactor(m_hAnitShow, cnt)) < 0.0 ||
			factor >= 1.0)
	{
		if (item->to==0.0)
		{
			Hide();
			OnShowAnimationEnd(False, False);
		}
		else
		{
			OnShowAnimationEnd(True, False);
		}


		m_item.fTransparency = item->to;
		
		m_item.fScale=1.0;

		yiAnitStop(m_hAnitShow, True);
		yifree(item);

		if (m_bIsRequestDestroy==True)
		{
			yiAnitDestroy(m_hAnitShow);
			m_hAnitShow=0;
			//Destroy();
			OnReqDestroy();
			return 0;
		}

	}
	else
	{
		m_item.fTransparency = factor*item->to+(1.0-factor)*item->from;
		m_item.fScale=factor;
	}
	InvalidateRect(0);

	return 0;
}
*/

// ------------------------------------
Bool cGlassInterface::AnimationShow(Bool bIsShow, int msec)
{
	sGlsAnimationEx ani;

	glsInitAnimationEx(&ani, &m_item);

	if (bIsShow)
	{
		m_item.fSizeRate=0.0;
		glsSetTrAnimationEx(&ani, 0.0, 1.0, eGAP_FISO);
		glsSetSzAnimationEx(&ani, 0.0, 1.0, eGAP_FISO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));
	}
	else
	{
		glsSetTrAnimationEx(&ani, 1.0, 0.0, eGAP_SIFO);
		glsSetSzAnimationEx(&ani, 1.0, 0.0, eGAP_SIFO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));
	}

	return AnimationShow(True, &ani, 15, msec);
}

// ------------------------------------
Bool cGlassInterface::AnimationShow(Bool bIsShow, sGlsAnimationEx* animation, int term, int msec)
{
	if (xIS_GLS_FLAG(m_flag, eGF_NO_ANIMATION))
	{
		if (bIsShow)
			Show();
		else
			Hide();

		OnShowAnimationEnd(bIsShow, False);
		return False;
	}

	if (xIS_GLS_FLAG(m_flag, eGF_ANIMATION_SHOW)==False)
	{
		if (bIsShow)
			Show();
		else
			Hide();
		return False;
	}

	if (bIsShow)
		Show();
	return StartAnimation(&m_item, animation, term, msec);
}

// ------------------------------------
/*void cGlassInterface::CrossDraw(sDrawItem* item, Bitmap* bmpPrev, Bitmap* bmpNext)
{
	int nAnimationTime=300;

	if (bmpPrev!=0 &&
			bmpPrev==bmpNext)
	{
		return;
	}

	if (item->hAnit==0)
	{
		item->hAnit=yiAnitCreate();
	}

	if (yiAnitIsAlived(item->hAnit)==True)
	{
		if (bmpNext&&
			(item->pStartBmp==0 && item->pEndBmp==0))
		{
			return ;
		}
		nAnimationTime = yiAnitStop(item->hAnit, True);
	}
	else
	{
		item->pAnimationBmp=0;
	}

	if (item->pStartBmp)
		delete item->pStartBmp;

	item->pStartBmp=0;
	if (bmpPrev)
	{
		item->pStartBmp=cyiBitmapBlender::BitmapCopy(0, bmpPrev);
	}

	if (item->pEndBmp)
		delete item->pEndBmp;

	item->pEndBmp=0;
	if (bmpNext)
	{
		item->pEndBmp= cyiBitmapBlender::BitmapCopy(0, bmpNext);
	}

	//m_queueAnit.push((yiAnyValue)item);
	m_NowAnimationList.lock();
	m_NowAnimationList.addref(item);
	m_NowAnimationList.unlock();

	if (item->pEndBmp==0 &&
			item->pStartBmp==0)
	{
		yiAnitStart(item->hAnit, m_hWnd, WM_GLS_ANIMATION, m_nShowAnimationInterval, 
			m_nAnimationMilliSec, m_eShowPattern, item);
	}
	else
	{
		if (xGET_ITEM_STATUS(item)==eDIS_MOUSE_MOVE)
		{
			yiAnitStart(item->hAnit, m_hWnd, WM_GLS_ANIMATION, m_nMouseMoveAnimationInterval, 
				m_nAnimationMilliSec, m_eMouseMovePattern,item);
		}
		else
		{
			yiAnitStart(item->hAnit, m_hWnd, WM_GLS_ANIMATION, m_nMouseLeaveAnimationInterval, 
				m_nAnimationMilliSec, m_eMouseLeavePattern,item);
		}
	}
			
	return ;
}*/

// ------------------------------------
void cGlassInterface::OnThreadStart(int id, int param1, int param2)
{
	return ;
}

// ------------------------------------
void cGlassInterface::xDeleteItemFromList(sDrawItem* item)
{
	for (cyiNode* node=m_NowAnimationList.GetNodeTopFrom(); node; node=node->np)
	{
		if (item==(sDrawItem*)node->ref)
		{
			m_NowAnimationList.remove(node);
			break;
		}
	}
}

// --------------------------------
point_t cGlassInterface::GetMeasureSize(Font* font)
{
	return (m_w|(m_h<<16));
}
typedef struct {
	cGlassInterface* obj;
	eGlsPosition ePosition;
	int dx;
	int dy;
} sGlsObjectPos;


// ------------------------------------
static void xMoveByPosition(cGlassInterface* obj, sGlsObjectPos* s)
{
	int x=0;
	int y=0;

	if (s->ePosition & eGP_LEFT)
	{
		x=obj->m_x;
		x+=s->dx;
	}
	else if (s->ePosition & eGP_RIGHT)
	{
		x=obj->m_x+obj->m_w-s->obj->m_w;
		x-=s->dx;
	}
	else if (s->ePosition & eGP_CENTER)
	{
		x=obj->m_x+((obj->m_w-s->obj->m_w)>>1);
		x+=s->dx;
	}
	else if (s->ePosition & eGP_LEFTSIDE)
	{
		x=obj->m_x-s->obj->m_w;
		x-=s->dx;
	}
	else if (s->ePosition & eGP_RIGHTSIDE)
	{
		x=obj->m_x+obj->m_w;
		x+=s->dx;
	}

	if (s->ePosition & eGP_TOP)
	{
		y=obj->m_y;
		y+=s->dy;
	}
	else if (s->ePosition & eGP_BOTTOM)
	{
		y=obj->m_y+obj->m_h-s->obj->m_h;
		y+=s->dy;
	}
	else if (s->ePosition & eGP_VCENTER)
	{
		y=obj->m_y+((obj->m_h-s->obj->m_h)>>1);
		y+=s->dy;
	}
	else if (s->ePosition & eGP_TOPSIDE)
	{
		y=obj->m_y-s->obj->m_h;
		y-=s->dy;
	}
	else if (s->ePosition & eGP_BOTTOMSIDE)
	{
		y=obj->m_y+obj->m_h;
		y+=s->dy;
	}

	s->obj->Move(x, y);
}

// ------------------------------------
Bool cGlassInterface::ReSizeNoActivate(UINT x, UINT y)
{
	return ReSize(x, y, True);
}

// ------------------------------------
void cGlassInterface::GetMargin(RECT& rect)
{
	rect.left=0;
	rect.top=0;
	rect.right=0;
	rect.bottom=0;
	return ;
}


// ------------------------------------
Bool cGlassInterface::ReSize(UINT x, UINT y, Bool bIsRepaint)
{
	if (x==0 &&
			y==0)
	{
		return False;
	}

	m_item.rect.w=x;
	m_item.rect.h=y;

	int before_w=m_w;
	int before_h=m_h;

	Bool res = cyctrlbase::ReSize(x, y, bIsRepaint);

	if (res==True)
	{
		m_item.rect.w=m_w;
		m_item.rect.h=m_h;

		if (m_listpos)
		{
			xNODELOOP(node, m_listpos)
			{
				sGlsObjectPos* s = (sGlsObjectPos*)node->ref;
				xMoveByPosition(this, s);
			}
		}
		RECT rect={0, 0, before_w, before_h};
		InvalidateRect(&rect);
		InvalidateRect(0);
	}

	return res;
}




// ------------------------------------
void cGlassInterface::AddPositionLink(cGlassInterface* obj, int ePosition, int xDistance, int yDistance)
{
	if (m_listpos==0)
		m_listpos = new cyiList();

	sGlsObjectPos s = { obj, (eGlsPosition)ePosition, xDistance, yDistance};

	m_listpos->addref(yitoheap(&s, sizeof(s)));
	xMoveByPosition(this, &s);
}

// ------------------------------------
Bool cGlassInterface::StartAnimation(sDrawItem* item, sGlsAnimationEx* animation, int term, int msec)
{
	//if (strcmp(m_szClassName, "xcglsTabCtrl")==0)
	//{
	//	Sleep(1);
	//}

	if (item==0)
		item=&m_item;

	if (item->animation==0)
	{
		item->animation = (sGlsAnimationEx*)yialloc(sizeof(*animation));
	}

	if (item->hAnit==0)
	{
		item->hAnit = yiAnitCreate();
	}


	if (yiAnitIsAlived(item->hAnit)==True)
	{
		yiAnitStop(item->hAnit, True);
	}

	//int term = item->animation->term;
	//int msec = item->animation->msec;

	float fAbortedFactor=0.0;
	if (yiAnitIsCleared(item->hAnit, &fAbortedFactor)==False)
	{
		// この場合は途中からのスタートとなるので。。。
		float fTr	= item->fTransparency;
		float fSz	= item->fSizeRate;
		Bitmap* pBmp = item->animation->pBmpAnimation;

		memcpy(item->animation, animation, sizeof(*animation));

		item->animation->fFromSizeRate=fSz;
		item->animation->fFromTransparency=fTr;
		item->animation->ptStart.x=item->rect.x;
		item->animation->ptStart.y=item->rect.y;

		if (item->animation->pBmpAborted)
			delete item->animation->pBmpAborted;

		item->animation->pBmpAborted=pBmp;

		msec = msec*fAbortedFactor;

	}
	else
	{
		memcpy(item->animation, animation, sizeof(*animation));
	}

	item->animation->nLastIndex=0;
	
	m_NowAnimationList.lock();
	m_NowAnimationList.addref(item);
	m_NowAnimationList.unlock();

	//YIDEBUG("Start Animation (%s)", m_szClassName);
	return yiAnitStart(item->hAnit, *this, WM_GLS_ANIMATIONEX, msec, term, eGAP_LINEAR, item);
}

// ------------------------------------
void cGlassInterface::StopAnimation(sDrawItem* item, Bool bClear)
{
	if (yiAnitIsAlived(item->hAnit)==True)
	{
		yiAnitStop(item->hAnit, True);
	}

	if (bClear)
	{
		yiAnitClear(item->hAnit);

		if (item->animation!=0 &&
				item->animation->pBmpAnimation)
		{
			delete item->animation->pBmpAnimation;
			item->animation->pBmpAnimation=0;
		}

		if (item->animation!=0 &&
				item->animation->pBmpAborted)
		{
			delete item->animation->pBmpAborted;
			item->animation->pBmpAborted=0;
		}
	}
}

// ------------------------------------
LRESULT cGlassInterface::OnGlsAnimationEx(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	//YIDEBUG("Animation(%s) Enter", m_szClassName);	
	if (m_bIsCreated==False)
		return 0;

	int  cnt=wParam;
	Bool bIsLastAnimation=False;
	float factor;
	static float xfactor;
	Bool bMoveAnimation=False;
	Bool bIsQuit=False;
	Bool bIsChanged=False;

	sDrawItem* item = (sDrawItem*)lParam;

	if (item==0)
	{
		return 0;
	}

	if (cnt==-1)
		return 0;

	if (cnt==-2)
		return 0;

	sGlsAnimationEx* s=(sGlsAnimationEx*)item->animation;


	if (s==0)
	{
		return 0;
	}

	if (cnt!=s->nLastIndex)
	{
		return 0;
	}

	if (yiAnitIsCleared(item->hAnit, 0)==True)
	{
		return 0;
	}

	s->nLastIndex++;

	// 中止される場合などを考えて、Linearタイプのファクターも取っておく。
	//s->fAnimationRate = yiAnitGetFactor(m_hAnimation, cnt, eGAP_LINEAR);

	//if (s->fAnimationRate<0.0 ||
	//		s->fAnimationRate>=1.0)
	//{
	//	s->fAnimationRate=1.0;
	//}

	// 1. まず、Desolveから
	if (s->nBmpStartIndex!=s->nBmpEndIndex)
	{
		factor = yiAnitGetFactorEx(item->hAnit, cnt, s->eDvPattern);

		if (factor==-4.0)
		{
			goto SKIP_DESOLVE;
		}
		bIsChanged=True;
		//YIDEBUG("OnGlsAnimationEx(%s) factor:%.3f", m_szClassName, factor);

		if (factor<0.0 ||
				factor>=1.0)
		{
			// 終了時の処理。
			// もうpAnimationBmpは要らない。
			if (s->pBmpAnimation)
				delete s->pBmpAnimation;

			if (s->pBmpAborted)
			{
				delete s->pBmpAborted;
				s->pBmpAborted=0;
			}

			s->pBmpAnimation=0;
			item->pCurrentBmp=s->nBmpEndIndex==-1?0:item->pBmp[s->nBmpEndIndex];

			bIsQuit=True;
			//YIDEBUG("OnGlsAnimationEx(%s) quit ......");
		}
		else
		{
			Bitmap* bmpStart=s->nBmpStartIndex==-1?0:item->pBmp[s->nBmpStartIndex];
			Bitmap* bmpEnd=s->nBmpEndIndex==-1?0:item->pBmp[s->nBmpEndIndex];

			if (s->pBmpAborted)
				bmpStart=s->pBmpAborted;

			s->pBmpAnimation=cyiBitmapBlender::Desolve(bmpStart, bmpEnd, factor);
			//YIDEBUG("OnGlsAnimationEx(%s) desolve ......");
		}
	}

SKIP_DESOLVE:
	//YIDEBUG("Animation(%s) TR:%.3f", m_szClassName, item->fTransparency);	

	if (s->fToTransparency!=s->fFromTransparency)
	{
		factor = yiAnitGetFactorEx(item->hAnit, cnt, s->eTrPattern);
		//YIDEBUG("OnGlsAnimationEx(%s) tr factor:%.3f", m_szClassName, factor);
		if (factor==-4.0)
		{
			goto SKIP_TRANS;
		}
		bIsChanged=True;
		if (factor<0.0 ||
				factor>=1.0)
		{
			item->fTransparency=s->fToTransparency;
			bIsQuit=True;

			if (cnt==0)
			{
				if (item->gls)
				{
					Show();
				}

				if (xIS_GLS_FLAG(m_flag, eGF_ANIMATION_SHOW)==True)
				{
					xSET_ITEM_SHOW(item);
				}
			}

			if (s->fToTransparency==0.0)
			{
				if (xIS_GLS_FLAG(m_flag, eGF_ANIMATION_SHOW)==True)
				{
					xSET_ITEM_HIDE(item);
				}

				if (item->gls)
				{
					Hide();
					OnShowAnimationEnd(False, False);
				}
			}
			else
			{
				if (item->gls)
				{
					OnShowAnimationEnd(True, False);
				}
			}
		}
		else
			item->fTransparency=s->fToTransparency*(factor)+s->fFromTransparency*(1.0-factor);

	}

SKIP_TRANS:

	if (s->fFromSizeRate!=s->fToSizeRate)
	{
		factor = yiAnitGetFactorEx(item->hAnit, cnt, s->eSzPattern);

		if (factor==-4.0)
		{
			goto SKIP_SIZE;
		}
		bIsChanged=True;

		//YIDEBUG("OnGlsAnimationEx(%s) sz factor:%.3f", m_szClassName, factor);
		if (factor<0.0 ||
				factor>=1.0)
		{
			item->fSizeRate=s->fToSizeRate;
			bIsQuit=True;
		}
		else
			item->fSizeRate=s->fToSizeRate*(factor)+s->fFromSizeRate*(1.0-factor);

	}

SKIP_SIZE:

	if (s->ptStart.x!=s->ptEnd.x)
	{
		factor = yiAnitGetFactorEx(item->hAnit, cnt, s->eMvPatternX);

		if (factor==-4.0)
		{
			goto SKIP_MOVE_X;
		}

		bIsChanged=True;
		//YIDEBUG("OnGlsAnimationEx(%s) ptx factor:%.3f", m_szClassName, factor);
		if (factor<0.0 ||
				factor>=1.0)
		{
			bMoveAnimation=True;
			bIsQuit=True;
		}
		else
		{
			int x=(int)((float)s->ptStart.x*(factor)+(float)s->ptEnd.x*(1.0-factor));
			item->rect.x=x;
			InvalidateRect(0, 0, False);
		}
	}

SKIP_MOVE_X:
	if (s->ptStart.y!=s->ptEnd.y)
	{
		factor = yiAnitGetFactorEx(item->hAnit, cnt, s->eMvPatternY);

		if (factor==-4.0)
		{
			goto SKIP_MOVE_Y;
		}

		bIsChanged=True;
		//YIDEBUG("OnGlsAnimationEx(%s) pty factor:%.3f", m_szClassName, factor);

		if (factor<0.0 ||
				factor>=1.0)
		{
			bMoveAnimation=True;
			bIsQuit=True;
		}
		else
		{
			int y =(int)((float)s->ptStart.y*(factor)+(float)s->ptEnd.y*(1.0-factor));
			item->rect.y=y;

			//InvalidateRect(0, 0, False);
		}
	}

SKIP_MOVE_Y:
	if (bMoveAnimation==True &&
		(factor<0.0 || factor>=1.0))
	{
		Move(s->ptEnd.x, s->ptEnd.y);
	}

	if (bIsQuit==True)
	{
		::yiAnitStop(item->hAnit, True);
		::yiAnitClear(item->hAnit);
		OnAnimationEnd(s);
	}

	if (bIsChanged==0)
	{
		YIDEBUG("Animation no changed!");
		return 0;
	}
	
	InvalidateRect(0);//, 0, False);
	
	return 0;
}

// ------------------------------------
void cGlassInterface::OnAnimationEnd(sGlsAnimationEx* animation)
{
	return ;
}

// ------------------------------------
/*
LRESULT cGlassInterface::OnGlsAnimation(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (m_bIsCreated==False)
		return 0;

	int cnt=wParam;
	sDrawItem* item = (sDrawItem*)lParam;
	int maxcnt;
	Bool bIsLastAnimation=False;
	float factor;
	static float xfactor;

	if (item==0)
	{
		return 0;
	}

	// 中止された場合
	if (yiAnitIsAlived(item->hAnit)==False)
	{
		if (item->pAnimationBmp)
		{
			delete item->pAnimationBmp;
		}
		item->pAnimationBmp=0;
		
		xDeleteItemFromList(item);

		if (xIS_ITEM_ANIMATION_NOTIFY_ONLY(item))
		{

			RECT rect = {(LONG)item->rect.x, (LONG)item->rect.y, (LONG)(item->rect.w+item->rect.x), (LONG)(item->rect.h+item->rect.y)};
			int x, y;

			//GetScrollPos(x, y);
			//item->from=-1;
			//item->to=-1;

			//rect.left-=x;
			//rect.top-=y;
			InvalidateRect(&rect, False);
		}

		return 0;
	}

	if ((factor=yiAnitGetFactor(item->hAnit, cnt)) < 0.0 ||
			factor >= 1.0)
	{
		yiAnitStop(item->hAnit, False);
		xDeleteItemFromList(item);

		item->fAnimationRate=1.00;

		// 何も行わない。
		if (xIS_ITEM_ANIMATION_NOTIFY_ONLY(item))
		{
			if (m_parent && m_parent->IsBgAnimation()==False)
			{
				RECT rect = {(LONG)item->rect.x, (LONG)item->rect.y, (LONG)(item->rect.w+item->rect.x), (LONG)(item->rect.h+item->rect.y)};
				int y;
				
				//GetScrollPos(x, y);
				//rect.left-=x;
				//rect.top-=y;
				InvalidateRect(&rect, False);
			}
			return 0;
		}

		if (item->pAnimationBmp)
		{
			delete item->pAnimationBmp;
		}

		// 最終
		bIsLastAnimation=True;


		if (item->pEndBmp)
		{
			item->pAnimationBmp=item->pEndBmp;

		}
		else
		{
			//item->pAnimationBmp=cyiBitmapBlender::Transparent(item->pStartBmp, item->fTransparency=item->to);//(item->fTransparency=(factor*item->to+(1-factor)*item->from)));
			//cyiBitmapBlender::BitmapCopy(m_bmpCurrent, item->pAnimationBmp);
			//item->pCurrentBmp=item->pAnimationBmp;

			//if (item->fTransparency==0.0)
			//{
			//	Hide();
			//}
			item->fTransparency=item->to;
			//xTRACE("OnglsAnimation fTransparency:%f, from:%f, to:%f", item->fTransparency, item->from, item->to);
		}

	}
	else
	{
		item->fAnimationRate=factor;

		// 何も行わない。
		if (xIS_ITEM_ANIMATION_NOTIFY_ONLY(item))
		{
			RECT rect = {(LONG)item->rect.x, (LONG)item->rect.y, (LONG)(item->rect.w+item->rect.x), (LONG)(item->rect.h+item->rect.y)};
			int y;

			//xTRACE("OnGlsAnimation animation rate:%f", item->fAnimationRate);
			//GetScrollPos(x, y);
			//rect.left-=x;
			//rect.top-=y;
			InvalidateRect(&rect, False);

			return 0;
		}

		if (item->pAnimationBmp)
		{
			delete item->pAnimationBmp;
		}

		if (item->pEndBmp)
		{
			item->pAnimationBmp=cyiBitmapBlender::Desolve(item->pStartBmp, item->pEndBmp, factor);
		}
		else
		{
			item->fTransparency=(factor*item->to+(1.0-factor)*item->from);

			//xTRACE("OnglsAnimation fTransparency:%f, from:%f, to:%f", item->fTransparency, item->from, item->to);
			//item->pAnimationBmp=cyiBitmapBlender::Transparent(item->pStartBmp, (item->fTransparency=(factor*item->to+(1.0-factor)*item->from)));
		}
		xfactor=factor;
	}

	//Graphics g(m_bmpCurrent);
	//g.DrawImage(item->pAnimationBmp, item->rect.x, item->rect.y, 0, 0, item->pAnimationBmp->GetWidth(), item->pAnimationBmp->GetHeight(), UnitPixel);

	//if (m_parent && m_parent->IsBgAnimation()==False)
	{
		RECT rect = {(LONG)item->rect.x, (LONG)item->rect.y, (LONG)(item->rect.w+item->rect.x), (LONG)(item->rect.h+item->rect.y)};
		int x, y;

		GetScrollPos(x, y);
		rect.left-=x;
		rect.top-=y;
		//DirectDraw(item->pAnimationBmp, &rect);
		InvalidateRect(&rect, False);
	}
	
	if (bIsLastAnimation)
	{
		//if (item->pEnd==0)
		//{
		//	delete item->pAnimationBmp;
		//}
		item->pAnimationBmp=0;
	}

	bIsHandled=True;
	return 0;

}*/

// ------------------------------------
// Default
int cGlassInterface::GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation)
{
	eDrawItemStatus _status = xGET_ITEM_STATUS(item);
	eDrawItemStatus _preStatus = xGET_ITEM_PRE_STATUS(item);
	Bool bIsNoTransparentAnimation=(item->fEnterTransparency==item->fLeaveTransparency);
	bIsNoAnimation=True;

	if (status==eDIS_FORCED)
	{
		SetItemStatus(item, eDIS_NORMAL);
		if (xIS_GLS_FLAG(m_flag, eGF_HILITE)==True)
		{
			return 3;
		}
		return 0;
	}



	if (status==eDIS_MOUSE_ENTER)
	{
		status=eDIS_MOUSE_MOVE;
	}

	if (status==_status)
	{
		//SetItemStatus(item, eDIS_NORMAL);
		return -1;
	}

	if (status==eDIS_FORCED)
	{
		SetItemStatus(item, eDIS_NORMAL);

		if (xIS_GLS_FLAG(m_flag, eGF_HILITE)==True)
		{
			return 3;
		}
		return 0;
	}

	if (status==eDIS_MOUSE_MOVE)
	{
		if (_status==eDIS_NORMAL)
		{
			//if (bIsNoTransparentAnimation==False &&
			//	m_item.fEnterTransparency!=-1.0)
			//{
			//	bIsNoAnimation=False;
			//	SetItemStatus(item, status);
			//	return -2;
			//}
			bIsNoAnimation=False;
			SetItemStatus(item, status);

			return 1;
		}

		return -1;
	}

	if (status==eDIS_MOUSE_DOWN)
	{
		if (_status==eDIS_MOUSE_MOVE)
		{
			SetItemStatus(item, status);
			return 2;
		}
		return -1;
	}

	if (status==eDIS_MOUSE_LEAVE)
	{
		if (_status==eDIS_MOUSE_MOVE)
		{
			//if (bIsNoTransparentAnimation==False &&
			//	m_item.fLeaveTransparency!=-1.0)
			//{
			//	bIsNoAnimation=False;
			//	SetItemStatus(item, eDIS_NORMAL);
			//	return -3;
			//}
			bIsNoAnimation=False;
			SetItemStatus(item, eDIS_NORMAL);

			if (xIS_GLS_FLAG(m_flag, eGF_HILITE)==True)
			{
				return 3;
			}

			return 0;
		}
		else if (_status==eDIS_MOUSE_DOWN)
		{
			//if (bIsNoTransparentAnimation==False &&
			//		m_item.fLeaveTransparency!=-1.0)
			//{
			//	SetItemStatus(item, eDIS_NORMAL);
			//	return -3;
			//}
			SetItemStatus(item, eDIS_NORMAL);

			if (xIS_GLS_FLAG(m_flag, eGF_HILITE)==True)
			{
				return 3;
			}

			return 0;
		}

		return -1;
	}

	if (status==eDIS_MOUSE_UP)
	{
		if (_status==eDIS_MOUSE_DOWN)
		{
			SetItemStatus(item, eDIS_MOUSE_MOVE);
			return 1;
		}

		return -1;
	}

	return -1;

}

// -----------------------------
void cGlassInterface::GetScrollPos(int&x,int&y)
{
	x=0;
	y=0;
}


// ---------------------------------------
LRESULT cGlassInterface::OnClicked(WPARAM wp, LPARAM lp)
{
	//SendMessage

	if (xIS_GLS_FLAG(m_flag, eGF_NOCOMMAND)!=0)
		return 0;

	if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)==0)
	{
		cglsSystem* system=GetSystem();

		if (system)
		{
			int id = system->GetCommandId(this, 0);

			if (id)
			{
				system->GetCommandCenter()->OnCommand(id, this);
				return 0;
			}
		}

		SendMessage(m_hNotifyWnd/*m_hParentWnd*/, WM_COMMAND, m_id/*m_item.id*/, (LPARAM)this);
	}
	return 0;
}

#include "glsPopupMenu.h"
// ---------------------------------------
LRESULT cGlassInterface::OnMouseRButtonDown(WPARAM wp, LPARAM lp, Bool& handled)
{
	if (m_menu)
	{
		POINT pt;
		GetCursorPos(&pt);
		handled=True;
		m_menu->Show(pt.x, pt.y);
	}

	return 0;
}

// ---------------------------------------
LRESULT cGlassInterface::OnMouseLButtonDown(WPARAM wp, LPARAM lp, Bool& handled)
{
	sDrawItem* item = &m_item;
	eDrawItemStatus status=xGET_ITEM_STATUS(item);

	if (m_pszToolTip)
	{
		cglsToolTip::GetInstance()->Hide(False);
	}

	if (xIS_GLS_FLAG(m_flag, eGF_NO_INPUT)==True)
	{
		return 0;
	}


	// Disable
	if (xIS_ITEM_DISABLE(item))
	{
		return 0;
	}

	//if (status==eDIS_MOUSE_DOWN)
	//	return 0;

	//if (item->hAnit)
	//{
	//	if (yiAnitIsAlived(item->hAnit)==True)
	//	{
	//		yiAnitStop(item->hAnit, False);
	//	}
	//}

	if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)!=0)
	{
		// PostMessageにすべき？
		cglsSystem* system=GetSystem();

		if (system)
		{
			int id = system->GetCommandId(this, m_id);

			if (id)
			{
				system->GetCommandCenter()->OnCommand(id, this);
				return 0;
			}
		}

		if (xIS_GLS_FLAG(m_flag, eGF_NOCOMMAND)==0)
			SendMessage(m_hNotifyWnd/*m_hParentWnd*/, WM_COMMAND, m_id/*m_item.id*/, (LPARAM)this);
	}

	//SetItemStatus(item, eDIS_MOUSE_DOWN);
	Update(eGUM_UPDATE, eDIS_MOUSE_DOWN);

	return 0;
}

// ---------------------------------------
LRESULT cGlassInterface::OnMouseLButtonUp(WPARAM wp, LPARAM lp, Bool& handled)
{
	sDrawItem* item = &m_item;
	eDrawItemStatus status=xGET_ITEM_STATUS(item);

	if (xIS_GLS_FLAG(m_flag, eGF_NO_INPUT)==True)
	{
		return 0;
	}

	// これで良い？
	if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)!=0)
	{
		SetItemStatus(item, eDIS_NORMAL);
		return 0;
		//return 0;
	}

	// Disable
	if (xIS_ITEM_DISABLE(item))
	{
		return 0;
	}

	if (status==eDIS_NORMAL ||
			status == eDIS_MOUSE_MOVE)
		return 0;

	//SetItemStatus(item, eDIS_MOUSE_MOVE);
	//Update();
	Update(eGUM_UPDATE, eDIS_MOUSE_UP);

	return 0;
}

// ---------------------------------------
LRESULT cGlassInterface::OnMouseLButtonDblClick(WPARAM wp, LPARAM lp, Bool& handled)
{
	m_bIsDblClicked=True;
	LRESULT res = OnClicked(wp, lp);

	m_bIsDblClicked=False;
	return res;
}


// ---------------------------------------
LRESULT cGlassInterface::OnMouseMove(WPARAM wp, LPARAM lp, Bool& handled)
{
	handled=False;

	//if (IsWindowVisible(*cglsToolTip::GetInstance())==True)
	//{
	//	cglsToolTip::GetInstance()->Hide(False);
	//}

	//if (m_tooltip)
	//{
	//	m_bIsToolTipLock=False;
	//	if (IsWindowVisible(m_tooltip->m_hWnd)==TRUE)
	//	{
	//		m_tooltip->Hide(1);
	//	}
	//}
	//if (m_tooltip)
	//{
	//	if (IsWindowVisible(m_tooltip->m_hWnd)==TRUE)
	//	{
	//		m_tooltip->Hide(1);
	//	}
	//}
	return -1;
}

// ------------------------------------
LRESULT cGlassInterface::OnChar(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	if (wParam==0x09)
	{
		handled=True;
		if (GetKeyState(VK_SHIFT) & 0x8000)
			PrevFocus();
		else
			NextFocus();
		return 0;
	}
	else if (wParam== 0x0d)
	{
		if (xIS_GLS_FLAG(m_flag, eGF_FOCUS_STOP)==True)
		{
			handled=True;
			//OnMouseLButtonDown(0, 0, handled);
			//OnMouseLButtonUp(0, 0, handled);
			if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)==True)
			{
				OnMouseLButtonDown(0, 0, handled);
				OnMouseLButtonUp(0, 0, handled);
			}
			else
			{
				OnClicked(0, 0);
			}
			return 0;
		}
	}

	cglsSystem* system=GetSystem();
	//handled=True;
	if (system==0)
	{
		handled=False;
		return 0;
	}


	int nCommand;
	if ((nCommand=system->GetKeyCommandId(wParam))==0)
	{
		if (system->GetKeyCenter()==0)
		{
			handled=False;
			return 0;
		}

		if (handled==False)
		{
			handled=True;
			system->GetKeyCenter()->OnChar(wParam, this);
		}
		handled=False;
		return 0;
	}

	handled=True;
	system->GetCommandCenter()->OnCommand(nCommand, this);
	
	return 0;
}

// ------------------------------------
LRESULT cGlassInterface::OnImeChar(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	cglsSystem* system=GetSystem();
	handled=True;
	if (system==0)
	{
		handled=False;
		return 0;
	}

	system->GetKeyCenter()->OnImeChar(wParam, this);
	
	return 0;
}

// ------------------------------------
LRESULT cGlassInterface::OnSysKeyDown(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	cglsSystem* system=GetSystem();
	//handled=True;
	if (system==0)
	{
		handled=False;
		return 0;
	}

	int nCommand;
	if ((nCommand=system->GetSysKeyCommandId(wParam))==0)
	{
		if (handled==False)
		{

			if (system->GetKeyCenter())
			{
				handled=True;
				system->GetKeyCenter()->OnSysKey(wParam, this);
			}
			handled=False;
			return 0;
		}
		handled=False;
		return 0;
	}

	system->GetCommandCenter()->OnCommand(nCommand, this);
	return 0;
}

// ------------------------------------
LRESULT cGlassInterface::OnSysKeyUp(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	//cglsSystem* system=GetSystem();
	//handled=True;
	//if (system==0)
	//{
	//	handled=False;
	//	return 0;
	//}

	//system->GetKeyCenter()->OnImeChar(wParam, this);
	return 0;
}

// ------------------------------------
LRESULT cGlassInterface::OnStartComposition(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	cglsSystem* system=GetSystem();
	handled=True;
	if (system==0)
	{
		handled=False;
		return 0;
	}

	if (system->GetKeyCenter()==0)
	{
		return 0;
	}

	system->GetKeyCenter()->OnStartComposition(wParam, this);
	return 0;
}

// ------------------------------------
LRESULT cGlassInterface::OnComposition(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	cglsSystem* system=GetSystem();
	handled=True;
	if (system==0)
	{
		handled=False;
		return 0;
	}

	if (system->GetKeyCenter()==0)
		return 0;

	system->GetKeyCenter()->OnComposition(wParam, this);
	return 0;
}

// ------------------------------------
LRESULT cGlassInterface::OnEndComposition(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	cglsSystem* system=GetSystem();
	handled=True;
	if (system==0)
	{
		handled=False;
		return 0;
	}
	if (system->GetKeyCenter()==0)
	{
		handled=False;
		return 0;
	}

	system->GetKeyCenter()->OnEndComposition(wParam, this);
	return 0;
}

// ------------------------------------
LRESULT cGlassInterface::OnGlsSystemCommand(WPARAM wParam, LPARAM lParam, Bool& handed)
{
	GetSystem()->GetCommandCenter()->OnCommand(wParam, (cGlassInterface*)lParam);
	return 0;
}



// ---------------------------------------
LRESULT cGlassInterface::OnMouseEnter(WPARAM wp, LPARAM lp)
{
	sDrawItem* item = &m_item;
	eDrawItemStatus status=xGET_ITEM_STATUS(item);

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	if (xIS_GLS_FLAG(m_flag, eGF_NO_MOUSEMOVE)!=0)
	{
		SetItemStatus(item, eDIS_MOUSE_MOVE);
		return 0;
	}

	// Disable
	if (xIS_ITEM_DISABLE(item))
	{
		return 0;
	}

	if (status==eDIS_MOUSE_MOVE)
		return 0;



	Update(eGUM_UPDATE, eDIS_MOUSE_ENTER);

	//SetItemStatus(item, eDIS_MOUSE_MOVE);
	//Update();
	return 0;
}

// ---------------------------------------
LRESULT cGlassInterface::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	sDrawItem* item = &m_item;
	eDrawItemStatus status=xGET_ITEM_STATUS(item);

	if (xIS_GLS_FLAG(m_flag, eGF_NO_MOUSEMOVE)!=0)
	{
		SetItemStatus(item, eDIS_NORMAL);
		return 0;
		//return 0;
	}

	if (m_pszToolTip)
	{
		cglsToolTip::GetInstance()->Hide(True);
	}

/*	if (m_tooltip)
	{
		m_bIsToolTipLock=False;
		if (IsWindowVisible(m_tooltip->m_hWnd)==TRUE)
		{
			m_tooltip->Hide(1);
		}
	}
*/
	if (xIS_GLS_FLAG(m_flag, eGF_NO_MOUSEMOVE)==True)
	{
		return 0;
	}

	// Disable
	if (xIS_ITEM_DISABLE(item))
	{
		return 0;
	}

	if (status==eDIS_NORMAL)
		return 0;

	Update(eGUM_UPDATE, eDIS_MOUSE_LEAVE);
	//SetItemStatus(item, eDIS_NORMAL);
	//Update();
	return 0;
}


// ---------------------------------------
int cGlassInterface::xGetPos(eItemAlign align, int wAll, int margin, int w)
{
	if (align==eIA_CENTER)
	{
		int _w = (wAll-(margin<<1));
		return ((_w-w)>>1)+margin;
	}
	else if (align==eSA_LEFT)
	{
		return margin;
	}
	else
	{
		return wAll-w-margin;
	}

	return 0;
}


// ---------------------------------------
BOOL cGlassInterface::xIsInRect(yiRect* rect, int x, int y, int posX, int posY)
{
	x+=posX;
	y+=posY;

	yiRect rc;

	if (x<rect->x)	
		return FALSE;
	if (x>(rect->w+rect->x))
		return FALSE;
	if (y>(rect->h+rect->y))
		return FALSE;
	if (y<rect->y)
		return FALSE;

	return TRUE;
}

// -----------------------------
Bitmap* cGlassInterface::GetBgBitmap()
{
	return 0;
	//return m_bmpBg;
}



// -----------------------------
void cGlassInterface::SetBgBitmap(Bitmap* bmp)
{
	//m_bmpBg=bmp;
	//InvalidateRect(0);
}

// -----------------------------
void cGlassInterface::SetUserParam(void* param)
{
	m_pUserParam=param;
}


// -----------------------------
void* cGlassInterface::GetUserParam()
{
	return m_pUserParam;
}

// -----------------------------
void cGlassInterface::RegisterFocusStopObject(cGlassInterface* obj)
{
	if (xIS_GLS_FLAG(m_flag, eGF_FOCUS_MANAGE)==True)
	{
		if (m_focusChildren==0)
			m_focusChildren = new cyiList();

		m_focusChildren->addref(obj);
	}

	if (m_parent)
		m_parent->RegisterFocusStopObject(obj);

	return ;
}



// ---------------------------------------
BOOL cGlassInterface::xIsInRect(RECT* rect1, RECT* rect2, RECT& ret)
{
	int x,y,w,h;

	if (rect1->left > rect2->left)
	{
		if (rect2->left+rect2->right < rect1->left)
		{
			return False;
		}
		x=rect1->left;
		w=rect1->left+rect1->right;
		w=w>rect2->left+rect2->right?rect2->left+rect2->right:w;
	}
	else
	{
		if (rect1->left+rect1->right < rect2->left)
		{
			return False;
		}
		x=rect2->left;
		w=rect2->left+rect2->right;
		w=w>rect1->left+rect1->right?rect1->left+rect1->right:w;
	}
	w-=x;

	if (rect1->top > rect2->top)
	{
		if (rect2->top+rect2->bottom < rect1->top)
		{
			return False;
		}
		y=rect1->top;
		h=rect1->top+rect1->bottom;
		h=h>rect2->top+rect2->bottom?rect2->top+rect2->bottom:h;
	}
	else
	{
		if (rect1->top+rect1->bottom < rect2->top)
		{
			return False;
		}
		y=rect2->top;
		h=rect2->top+rect2->bottom;
		h=h>rect1->top+rect1->bottom?rect1->top+rect1->bottom:h;
	}

	h-=y;
	ret.left=x;
	ret.right=w;
	ret.top=y;
	ret.bottom=h;

	if (h==0 || w==0)
	{
		return False;
	}
	return True;
}

// ---------------------------------------
Bitmap* cGlassInterface::GetParentBg(Bitmap* bmp, RECT* rect, cGlassInterface* sender)
{
	RECT _rect;

	if (rect==0)
	{
		_rect.left=0;
		_rect.top=0;
		_rect.right=m_w;
		_rect.bottom=m_h;
		rect = &_rect;
	}
	if (bmp==0)
	{
		bmp = BITMAP32(rect->right, rect->bottom);
	}
	if (sender==0)
	{
		sender=this;
	}

	if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==True)
	{
		int x=rect->left+m_x;
		int y=rect->top+m_y;
		
		RECT xrect = {x, y, rect->right, rect->bottom};
		m_parent->GetParentBg(bmp, &xrect, sender);

		if (this!=sender)
		{
			Graphics g(bmp);

			Bitmap* pCurrentBmp = GetCurrentBitmap();

			if (pCurrentBmp==0)
				pCurrentBmp = GetBgBitmap();

			if (pCurrentBmp)
				g.DrawImage(pCurrentBmp/*m_item.pCurrentBmp*/, 0, 0, rect->left, rect->top, rect->right, rect->bottom, UnitPixel);

			/*if (m_item.pCurrentBmp)
			{
				g.DrawImage(m_item.pCurrentBmp, 0, 0, rect->left, rect->top, rect->right, rect->bottom, UnitPixel);
			}
			else
			{
				this->Get
			}*/

/*			if (m_bmpCurrent)
				g.DrawImage(m_bmpCurrent, 0, 0, rect->left, rect->top, rect->right, rect->bottom, UnitPixel);
			else
			{
				Bitmap* bmpBg=GetBgBitmap();
				if (bmpBg)
				{
					g.DrawImage(bmpBg, 0, 0, rect->left, rect->top, rect->right, rect->bottom, UnitPixel);
				}
			}*/

		}
		return bmp;
	}
	else
	{
		Bitmap* bmpBg = GetBgBitmap();
		if (bmp!=0)
		{
			cyiBitmapBlender::BitmapCopy(bmp, bmpBg, 0, 0, rect->left, rect->top, rect->right, rect->bottom);
		}
		else
		{
			cyiBitmapBlender::FillColor(bmp, 0);
		}

		//Graphics g(bmp);
		//g.DrawImage(m_bmpCurrent, 0, 0, rect->left, rect->top, rect->right, rect->bottom, UnitPixel);
	}

	return bmp;
}

// -----------------------------
int cGlassInterface::ZOrder(int n)
{
	int cnt=0;

	if (m_parent==0)
		return -1;

	cyiList* parentList = m_parent->m_children;
	
	if (parentList==0)
		return -1;

	for (cyiNode* node=parentList->GetNodeTopFrom(); node; node=node->np)
	{
		cGlassInterface* obj = (cGlassInterface* )node->ref;

		if (obj==this)
		{
			parentList->remove(node);

			if (n==eZO_TOP)
			{
				::SetWindowPos(m_hWnd, HWND_TOPMOST, 0 , 0 , 0 , 0 , SWP_NOMOVE | SWP_NOSIZE);

				parentList->addref((void*)this);
			}
			else
			{
				parentList->insertref(n, (void*)this);
			}
			return cnt;
		}
		cnt++;
	}

	return -1;
}

// -----------------------------
cGlassInterface* cGlassInterface::GetChild(int id)
{
	if (m_children==0)
		return 0;

	for (cyiNode* node=m_children->GetNodeTopFrom(); node; node=node->np)
	{
		cGlassInterface* obj = (cGlassInterface*)node->ref;

		if (obj==0)
			continue;

		if (obj->m_id==id)
		{
			return obj;
		}
	}
	return 0;
}


// -----------------------------
Bool cGlassInterface::GetAbsoluteCoordination(int&x, int&y)
{
	x=0;
	y=0;

	if (m_parent==0)
	{
		x=m_x;
		y=m_y;
		return True;
	}

	if (this!=m_glsDrawInterface)
	{
		m_nAbsX=m_x;
		m_nAbsY=m_y;
		cGlassInterface* parent=m_parent;

		while(parent)
		{
			if (parent==m_glsDrawInterface)
			{
				break;
			}
			m_nAbsX+=parent->m_x;
			m_nAbsY+=parent->m_y;
			parent=parent->m_parent;
		}

		if (m_children==0)
		{
			return parent?True:False;
		}

		for (cyiNode* node=m_children->GetNodeTopFrom(); node; node=node->np)
		{
			cGlassInterface* obj = (cGlassInterface* )node->ref;
			
			obj->GetAbsoluteCoordination(obj->m_nAbsX, obj->m_nAbsY);

		}

		return parent?True:False;
	}

	return True;
}

// ------------------------------------
Bool cGlassInterface::Move(UINT x, UINT y, Bool bIsActivate)
{
	//InvalidateRect(0);
	if (m_x!=x ||
			m_y!=y)
	{
		InvalidateRect(0);
	}

	if (cyctrlbase::Move(x, y, bIsActivate)==True)
	{
		m_item.rect.x=m_x;
		m_item.rect.y=m_y;

		GetAbsoluteCoordination(m_nAbsX, m_nAbsY);
		InvalidateRect(0);

		if (m_children)
		{
			xNODELOOP(node, m_children)
			{
				cGls* gls = (cGls*)node->ref;
				gls->m_nAbsX=gls->m_x+m_nAbsX;
				gls->m_nAbsY=gls->m_y+m_nAbsY;
			}
		}

		if (m_listpos)
		{
			xNODELOOP(node, m_listpos)
			{
				sGlsObjectPos* s = (sGlsObjectPos*)node->ref;
				xMoveByPosition(this, s);
			}
		}
		InvalidateRect(0);
		return True;
	}

	return False;
}

// ------------------------------------
Bool cGlassInterface::MoveByPosition(sGlsItemPosition* x, sGlsItemPosition* y)
{
	if (x==0)
		x=m_posX;
	if (y==0)
		y=m_posY;

	m_posX=x;
	m_posY=y;
	return MoveByPosition(*x, *y);
}

// ------------------------------------
Bool cGlassInterface::MoveByPosition(sGlsItemPosition& _x, sGlsItemPosition& _y)
{
	int x, y;
	int stX=0;
	int stY=0;

	if (_x.from==0)
	{
		if (m_parent==0)
		{
			stX=_x.pos;
		}
		else
		{
			if (_x.align&eGP_CENTER)
			{
				stX=(m_parent->m_w)>>1;
			}
			else if (_x.align&eGP_RIGHT)
			{
				stX=(m_parent->m_w);
			}
			else
			{
				stX=_x.pos;
			}
		}
	}
	else
	{

		stX = _x.from->m_x+_x.pos;
		if ((_x.align&eGP_CENTER))
		{
			stX = (_x.from->m_w>>1) + _x.from->m_x;
		}
		else if ((_x.align&eGP_RIGHT) ||
					(_x.align&eGP_RIGHTSIDE))
		{
			stX = _x.from->m_x+_x.from->m_w;
		}
	}

	if (_x.align&eGP_CENTER)
	{
		x=stX-(m_w>>1)+_x.pos;
	}
	else if (_x.align&eGP_LEFTSIDE)
	{
		x-=m_w;
	}
	else if (_x.align&eGP_RIGHT)
		x-=m_w;
	else if (_x.align&eGP_RIGHTSIDE)
		x-=_x.pos;

	// y座標
	if (_y.from==0)
	{
		if (m_parent==0)
		{
			stY=_y.pos;
		}
		else
		{
			if (_y.align&eGP_VCENTER)
			{
				stY=(m_parent->m_h)>>1;
			}
			else if (_y.align&eGP_RIGHT)
			{
				stY=(m_parent->m_h);
			}
			else
			{
				stY=_y.pos;
			}
		}
	}
	else
	{

		stY = _y.from->m_y+_y.pos;
		if ((_y.align&eGP_VCENTER))
		{
			stY = (_y.from->m_h>>1) + _y.from->m_y;
		}
		else if ((_y.align&eGP_BOTTOM) ||
					(_y.align&eGP_BOTTOMSIDE))
		{
			stY = _y.from->m_y+_y.from->m_h;
		}
	}

	if (_y.align&eGP_VCENTER)
	{
		y=stY-(m_h>>1)+_y.pos;
	}
	else if (_y.align&eGP_TOPSIDE)
	{
		y-=m_h;
	}
	else if (_y.align&eGP_BOTTOM)
		y-=m_h;
	else if (_y.align&eGP_BOTTOMSIDE)
		y+=_y.pos;

	return Move(x, y);
}


// ------------------------------------
Bool cGlassInterface::Move(UINT _x, UINT _y, eGlsItemAlign eAlign)
{
	int x=_x, y=_y;
	if (eAlign&eGIA_CENTER)
	{
		x=_x-(m_w>>1);
	}
	else if (eAlign&eGIA_RIGHT)
	{
		x=_x-m_w;
	}

	if (eAlign&eGIA_VCENTER)
	{
		y=_y-(m_h>>1);
	}
	else if (eAlign&eGIA_BOTTOM)
	{
		y=_y-m_h;
	}

	return Move(x=x<0?0:x, y=y<0?0:y);
}

// ---------------------------------------
Bool cGlassInterface::OnDirectDrawBefore(Graphics* g, RECT* rect)
{
	return True;
}

// ---------------------------------------
Bool cGlassInterface::OnDirectDrawAfter(Graphics* g, RECT* rect)
{
	return True;
}

// ---------------------------------------
Bitmap* cGlassInterface::GetChildFgImage(int childId, int nImageIndex, RECT* rect)
{
	return 0;
}


// ---------------------------------------
Bool cGlassInterface::DirectDraw(Bitmap* bmp, RECT* rect)
{
	RECT _rect;

	if (rect==0)
	{
		_rect.left=0;
		_rect.top=0;
		_rect.right=m_w;
		_rect.bottom=m_h;

		rect=&_rect;
	}

	if (bmp==0)
	{
		bmp = m_bmpCurrent;
	}


	yiMutexLockEx(m_hPaintMutex);
	HDC hdc = m_glsDrawInterface->GetDC();
	
	Graphics g(hdc);
	int sx, sy;

	GetScrollPos(sx, sy);

	//xTRACE("cGlassInterface::DirectDraw(%d, %d, %d, %d) called", sx, sy, rect->right, rect->bottom);

	//OnDirectDrawBefore(&g, rect);
	g.DrawImage(bmp, m_nAbsX+rect->left, m_nAbsY+rect->top, rect->left, rect->top, rect->right, rect->bottom, Unit::UnitPixel);
	//OnDirectDrawAfter(&g, rect);
	m_glsDrawInterface->ReleaseDC(hdc);
	yiMutexUnlockEx(m_hPaintMutex);

	return True;
}

// -----------------------------
void cGlassInterface::OnShowAnimationEnd(Bool bIsShow, Bool bIsCanceled)
{
	
	return ;
}


// ------------------------------------
LRESULT cGlassInterface::OnDragStart(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	handled=True;
	m_item.fTransparency=0.5;
	xTRACE("cglsButton::OnDragStart() called");
	Move(m_x+1, m_y+1);

	InvalidateRect(0);
	return 1;
}

// ------------------------------------
LRESULT cGlassInterface::OnDrag(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int x, y;


	GetDragStartPos(x, y);

	Move(m_x+((int)wParam-x), m_y+((int)lParam-y));
	//xTRACE("cglsButton::OnDragStart() called (%d,%d)->(%d,%d), cx:%d, cy:%d, pt.x:%d, pt.y:%d", m_x, m_y, m_x+(cx+x), m_y+(cy-y), cx, cy, pt.x, pt.y);

	handled=True;
	InvalidateRect(0);
	return 1;
}

// ------------------------------------
LRESULT cGlassInterface::OnDragEnd(WPARAM wParam, LPARAM lParam, Bool& handeled)
{
	int cx=lParam&0xFFFF;
	int cy=(lParam>>16)&0xFFFF;
	int x, y;

	handeled=True;
	GetDragStartPos(x, y);

	Move(m_x+(wParam-x), m_y+(lParam-y));
	//xTRACE("cglsButton::OnDragEnd() called");

	m_item.fTransparency=m_item.fLeaveTransparency;
	InvalidateRect(0);
	return 0;
}

// ------------------------------------
LRESULT cGlassInterface::OnToolTipTimer(WPARAM wParam, LPARAM lParam)
{

	if (m_pszToolTip!=0)
	{
		POINT pt;

		if (IsWindowVisible(*cglsToolTip::GetInstance())==TRUE)
		{
			return 0;
		}

		//if (m_bIsToolTipLock==True)
		//{
		//	return 0;
		//}

		cglsToolTip* m_tooltip = cglsToolTip::GetInstance();

		GetCursorPos(&pt);
		//ClientTo
		//ScreenToClient(m_tooltip->m_hWnd, &pt);
	
		if (pt.x+m_tooltip->m_w > m_tooltip->m_parent->m_w)
		{
		
			//pt.x-=(m_tooltip->m_parent->m_w-m_tooltip->m_w);
		}
		
		if	(pt.y+m_tooltip->m_h > m_tooltip->m_parent->m_h)
		{
			//pt.y-=m_tooltip->m_h;
		}

		if (m_tooltip->SetText(m_pszToolTip, this)==True)
		{
			cglsToolTip::GetInstance()->Move(pt.x+8, pt.y+8);
			m_tooltip->Show();
		}

		//m_tooltip->Move(pt.x, pt.y);
		//m_tooltip->Show(1);
		//m_bIsToolTipLock=True;

	}

	return 0;
}

// --------------------------------
extern cglsResourceObject* g_resourceObject;

// --------------------------------
cglsResourceObject* cGlassInterface::GetDefaultResource()
{
	return g_resourceObject;
}

// ---------------------------------------
cGlassInterface* cGlassInterface::xNEW()
{
	return new cGlassInterface();
}

// ---------------------------------------
void cGlassInterface::xDELETE(cGlassInterface* obj)
{
	delete obj;
}

// --------------------------------
//void cGlassInterface::CreateGlassBitmap(sDrawItem* item, Bitmap* bmp, Bitmap* bmpStr, eGlassInterfaceStyle style)
//{
//	if (style==eGlassInterfaceStyle::eGIS_BUTTON)
//	{
//		cGlassInterface::xCreateGlassBitmapForBtn(item, bmp, bmpStr);
//	}
//}

// --------------------------------
//void cGlassInterface::xCreateGlassBitmapForBtn(sDrawItem* item, Bitmap* bmp, Bitmap* bmpStr)
//{
//	// 共通
//	int w_item = (bmpStr?bmpStr->GetWidth():0) + (bmp?bmp->GetWidth():0);
//	int h_item = (bmpStr?bmpStr->GetHeight():0); 
//	float fMargin = (float)h_item/6.0;
//	int nhMargin = (int)fMargin;
//	int nwMargin;
//	yiRect rect;
//
//	fMargin=(float)w_item/4.0;
//	nwMargin = (int)fMargin;
//
//	if (bmp)
//	{
//		h_item = bmp->GetHeight()>h_item ? bmp->GetHeight():h_item;
//	}
//
//	item->rect.x=0;
//	item->rect.y=0;
//	item->rect.w=w_item+(nwMargin<<1)+1;
//	item->rect.h=h_item+(nhMargin<<1)+1;
//
//	rect=item->rect;
//
//	// ノーマル
//	Bitmap* bmp2 = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, item->rect.w-2, item->rect.h-2), 1.5);
//	Graphics g(bmp2);
//	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
//
//	if (bmp)
//	{
//		int x = xGetPos(eIA_CENTER, item->rect.w, nwMargin, w_item);
//		int y = xGetPos(eIA_CENTER, item->rect.h, nhMargin, bmp->GetHeight());
//		g.DrawImage(bmp, x, y);
//	}
//
//	if (bmpStr)
//	{
//		int x = xGetPos(eIA_CENTER, item->rect.w, nwMargin, w_item) + (bmp?bmp->GetWidth():0);
//		int y = xGetPos(eIA_CENTER, item->rect.h, nhMargin, bmpStr->GetHeight());
//		g.DrawImage(bmpStr, x, y);
//	}
//
//	Bitmap* bmp3=cyiShape::FillRoundRect(&SolidBrush(Color(32, 32, 32, 32)), RectF(2, 2, item->rect.w-2, item->rect.h-2), 1.5);
//	g.DrawImage(bmp3, 0, 0);
//	item->pBmp[0] = cyiBitmapBlender::DropShadow(bmp2, 1, 1, 1, 0.3);
//
//	item->rect.w = item->pBmp[0]->GetWidth()+1;
//	item->rect.h = item->pBmp[0]->GetHeight()+1;
//
//	delete bmp3;
//	delete bmp2;
//
//	item->pBmp[1]=item->pBmp[0];
//	item->pBmp[2]=item->pBmp[0];
//	item->pCurrentBmp=item->pBmp[0];
//
//	// Mouse Enter
//	Bitmap* bmpx = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, rect.w-2, rect.h-2), 1.5);
//
//	Graphics g2(bmpx);
//
//	g2.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
//
//	bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(8, 16, 64, 16)), RectF(0, 0, rect.w-2, rect.h-2), 1.5);
//	Bitmap temp(rect.w-2, rect.h-2, PixelFormat32bppARGB);
//	Graphics g3(&temp);
//	g3.FillEllipse(&SolidBrush(Color(127, 127, 255, 127)), RectF(5, 5, rect.w-2-10, rect.h-2-10)); 
//	Bitmap* bmp5 = cyiBitmapBlender::Blure(&temp, 10.0, 100);
//
//	g2.DrawImage(bmp5, 0, 0);
//	g2.DrawImage(bmp3, 0, 0);
//
//	if (bmp)
//	{
//		int x = xGetPos(eIA_CENTER, rect.w, nwMargin, w_item);
//		int y = xGetPos(eIA_CENTER, rect.h, nhMargin, bmp->GetHeight());
//		g2.DrawImage(bmp, x, y);
//	}
//
//	if (bmpStr)
//	{
//		int x = xGetPos(eIA_CENTER, rect.w, nwMargin, w_item) + (bmp?bmp->GetWidth():0);
//		int y = xGetPos(eIA_CENTER, rect.h, nhMargin, bmpStr->GetHeight());
//		g2.DrawImage(bmpStr, x, y);
//	}
//
//	item->pBmp[1] = cyiBitmapBlender::DropShadow(bmpx, 1, 1, 1, 0.3);
//	delete bmpx;
//	delete bmp5;
//	delete bmp3;
//
//	//// Mouse Down
//	bmpx = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, rect.w-2, rect.h-2), 1.5);
//
//	Graphics g7(bmpx);
//	g7.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
//
//	bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(32, 32, 32, 32)), RectF(0, 0, rect.w-2, rect.h-2), 1.5);
//	g7.DrawImage(bmp3, 0, 0);
//
//	if (bmp!=0)
//	{
//		int x = xGetPos(eIA_CENTER, rect.w, nwMargin, w_item);
//		int y = xGetPos(eIA_CENTER, rect.h, nhMargin, bmp->GetHeight());
//		g7.DrawImage(bmp, x, y);
//	}
//
//	if (bmpStr)
//	{
//		int x = xGetPos(eIA_CENTER, rect.w, nwMargin, w_item) + (bmp?bmp->GetWidth():0);
//		int y = xGetPos(eIA_CENTER, rect.h, nhMargin, bmpStr->GetHeight());
//		g7.DrawImage(bmpStr, x, y);
//	}
//
//	bmp5 = cyiBitmapBlender::DropShadow(bmpx, 0, 0, 1.0, 0.7);
//
//	Bitmap* ret=BITMAP32(item->rect.w, item->rect.h);
//	Graphics gx(ret);
//
//	gx.DrawImage(bmp5, 1, 1);
//	
//	delete(bmp3);
//	delete bmpx;
//	delete bmp5;
//
//}

#include "glsResourceBitmap.h"

// ---------------------------------------
Bitmap* cGlassInterface::xCreateItemBitmap(sDrawItem* item, Font* font)
{
	cglsResourceBitmap* resource = (cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
	Bitmap* bmp=0;
	Bitmap* bmpStr=0;
	int nFontHeight = font->GetHeight(__DEFAULTGRAPHICS__);
	int w=0, h=0;

	if (item->bitmapId)
	{
		if (((int)item->bitmapId&0xFFFF0000)==0)
		{
			bmp = cyiBitmapBlender::BitmapCopy(0, (Bitmap*)item->bitmapId);
		}
		else
		{
			int w=nFontHeight*0.8;
			bmp=resource->GetBitmap((int)item->bitmapId, 0, w, w);
		}

		w=bmp->GetWidth();
		h=bmp->GetHeight();
	}

	if (item->txt)
	{
		bmpStr=cyiShape::BitmapString(0, item->txt, font, &SolidBrush(Color::Black)); 

		w+=bmpStr->GetWidth();
		h=h>bmpStr->GetHeight()?h:bmpStr->GetHeight();
	}

	Bitmap* ret = BITMAP32(w, h);
	int xStringOffset=0;
	
	if (bmp)
	{
		cyiBitmapBlender::BitmapCopy(ret, bmp, 0, (h-bmp->GetHeight())>>1, 0, 0, bmp->GetWidth(), bmp->GetHeight());
		xStringOffset=bmp->GetWidth();
		delete bmp;
	}
	
	if (bmpStr)
	{
		cyiBitmapBlender::BitmapCopy(ret, bmpStr, xStringOffset, (h-bmpStr->GetHeight())>>1, 0, 0, bmpStr->GetWidth(), bmpStr->GetHeight());
		delete bmpStr;
	}


	return ret;

	//if (item->pItemBmp)
	//	delete item->pItemBmp;

	//item->pItemBmp=ret;
}

// ---------------------------------------
Bool cGlassInterface::xMesureBitmap(Bitmap* bmp, wchar_t* txt, Font* font, RECT* rect, int nFixedWidth)
{
	cglsResourceBitmap* resource = (cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
	int w=0, h=0;
	int wIcon=0;
	int hIcon=0;
	int nFontHeight = font->GetHeight(__DEFAULTGRAPHICS__);

	if (bmp)
	{
		if (((IntPtr)bmp&0xFFFF0000)==0)
		{
			//bmp = cyiBitmapBlender::BitmapCopy(0, icon);
			wIcon=w=bmp->GetWidth();
			hIcon=h=bmp->GetHeight();
		}
		else
		{
			int w=nFontHeight*0.8;
			Bitmap* bmpTemp=resource->GetBitmap((IntPtr)bmp, 0, w, w);
			wIcon=w=bmpTemp->GetWidth();
			hIcon=h=bmpTemp->GetHeight();
			delete bmpTemp;
		}
	}

	if (txt)
	{
		unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, txt, font, 0, 0, eSTS_NONE, 0, nFixedWidth); 
		w+=(pos>>16);
		h=hIcon>(pos&0xFFFF)?hIcon:(pos&0xFFFF);

	}

	rect->left=wIcon;
	rect->top=hIcon;
	rect->right=w;
	rect->bottom=h;

	return True;

}

//// ---------------------------------------
//Bitmap* cGlassInterface::xCreateBitmapEx(Bitmap* bg, Bitmap* bmp, wchar_t* txt, Font* font, color_t colorFont)
//{
//	cglsResourceBitmap* resource = (cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
//	Bitmap* bmp=0;
//	Bitmap* bmpStr=0;
//	int nFontHeight = font->GetHeight(__DEFAULTGRAPHICS__);
//	int w=0, h=0;
//	Graphics g(bg);
//
//	if (icon)
//	{
//		if (((int)icon&0xFFFF0000)==0)
//		{
//			bmp = cyiBitmapBlender::BitmapCopy(0, icon);
//		}
//		else
//		{
//			int w=nFontHeight*0.8;
//			bmp=resource->GetBitmap((int)icon, 0, w, w);
//		}
//
//		w=bmp->GetWidth();
//		h=bmp->GetHeight();
//	}
//
//	if (txt)
//	{
//		bmpStr=cyiShape::BitmapStringEx(bg, txt, font, colorFont, 0, 0); 
//		w+=bmpStr->GetWidth();
//		h=h>bmpStr->GetHeight()?h:bmpStr->GetHeight();
//	}
//
//	Bitmap* ret = BITMAP32(w, h);
//	int xStringOffset=0;
//	
//	if (bmp)
//	{
//		cyiBitmapBlender::BitmapCopy(ret, bmp, 0, (h-bmp->GetHeight())>>1, 0, 0, bmp->GetWidth(), bmp->GetHeight());
//		xStringOffset=bmp->GetWidth();
//		delete bmp;
//	}
//	
//	if (bmpStr)
//	{
//		cyiBitmapBlender::BitmapCopy(ret, bmpStr, xStringOffset, (h-bmpStr->GetHeight())>>1, 0, 0, bmpStr->GetWidth(), bmpStr->GetHeight());
//		delete bmpStr;
//	}
//}


// ---------------------------------------
Bitmap* cGlassInterface::xCreateBitmap(Bitmap* icon, wchar_t* txt, Font* font, int nLength, color_t color)
{
	cglsResourceBitmap* resource = (cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
	Bitmap* bmp=0;
	Bitmap* bmpStr=0;
	int nFontHeight = font->GetHeight(__DEFAULTGRAPHICS__);
	int w=0, h=0;

	if (icon)
	{
		if (((IntPtr)icon&0xFFFF0000)!=0)
		{
			if (icon->GetLastStatus()==Status::Ok)
			{
				bmp = cyiBitmapBlender::BitmapCopy(0, icon);
			}
		}
		else
		{
			int w=nFontHeight*0.8;
			bmp=resource->GetBitmap((IntPtr)icon, 0, w, w);
		}

		if (bmp)
		{
			w=bmp->GetWidth();
			h=bmp->GetHeight();
		}
	}

	if (txt)
	{
		if (nLength==0)
		{
			bmpStr=cyiShape::BitmapString(0, txt, font, &SolidBrush(color)); 
		}
		else 
		{
			nLength-=w;
			bmpStr=cyiShape::BitmapString(0, txt, font, &SolidBrush(color), eSTS_FIXED_NORMAL, 0, (float)nLength);
		}

		w+=bmpStr->GetWidth();
		h=h>bmpStr->GetHeight()?h:bmpStr->GetHeight();
	}

	Bitmap* ret = BITMAP32(w, h);
	int xStringOffset=0;
	
	if (bmp)
	{
		cyiBitmapBlender::BitmapCopy(ret, bmp, 0, (h-bmp->GetHeight())>>1, 0, 0, bmp->GetWidth(), bmp->GetHeight());
		xStringOffset=bmp->GetWidth();
		delete bmp;
	}
	
	if (bmpStr)
	{
		cyiBitmapBlender::BitmapCopy(ret, bmpStr, xStringOffset, (h-bmpStr->GetHeight())>>1, 0, 0, bmpStr->GetWidth(), bmpStr->GetHeight());
		delete bmpStr;
	}

	return ret;
}


// ---------------------------------------
int cGlassInterface::GetFontHeight(Bool bIsGDI)
{
	if (m_font)
	{
		if (bIsGDI==0)
		{
			return m_nFontHeight;
		}

		return m_nFontHeightGDI;
	}
	if (m_parent)
		return m_parent->GetFontHeight(bIsGDI);

	return 0;
}

// -----------------------------
int cGlassInterface::GetBlankId()
{
	int id=32;
	if (m_children==0)
	{
		return 32;
	}

	for (cyiNode* node=m_children->GetNodeTopFrom(); node; node=node->np)
	{
		cGlassInterface* obj = (cGlassInterface*)node->ref;

		if (obj->m_id==id)
		{
			node=m_children->GetNodeTopFrom();
			id++;
		}
	}

	return id;
}

// -----------------------------
Bool cGlassInterface::IsBgChanged()
{
	if (m_eUpdateReason==eGUR_BG_CHANGED)
	{
		return True;
	}
	if (m_parent)
		return m_parent->IsBgChanged();

	return False;
}


// --------------------------------------------
cglsImageList::cglsImageList(int nElementCount, Bool bIsVertical, eGlsItemAlign align, int distance)
: m_ElementCount(nElementCount),
m_bIsVertical(bIsVertical),
m_eAlign(align),
m_distance(distance),
m_list(0),
m_listFixed(0)
, m_min(0)
, m_max(0)
, m_nLimited(0)
, m_w(0)
, m_h(0)
,m_colorBg(0)
, m_listString(0)
{
	if (nElementCount==0)
		m_list = new cyiBitmapList();
	else
		m_listFixed = new cyiBitmapRb(nElementCount, True);
}

// --------------------------------------------
cglsImageList::~cglsImageList()
{
	if (m_list)
		delete m_list;

	if (m_listFixed)
		delete m_listFixed;
}

// --------------------------------------------
inline Bitmap* cglsImageList::xGetBitmap(int index)
{
	if (m_ElementCount==0)
		return 0;

	if (m_list==0 &&
			m_listFixed==0)
	{
		return 0;
	}

	if (m_list)
	{
		int n;
		if ((n=m_list->count())==0)
			return 0;

		index = index%n;
		Bitmap* bmp = m_list->GetTopFrom(index);
		Bitmap* ret;

		if (n==1)
			return cyiBitmapBlender::BitmapCopy(0, bmp);
		
		if (m_bIsVertical==True)
		{
			ret = BITMAP32(m_w, bmp->GetHeight());
			Graphics g(ret);
			g.Clear(Color(m_colorBg));
			cyiBitmapBlender::BitmapCopy(ret, bmp, (m_w-bmp->GetWidth())>>1, 0);
		}
		else
		{
			ret = BITMAP32(bmp->GetWidth(), m_h);
			Graphics g(ret);
			g.Clear(Color(m_colorBg));
			cyiBitmapBlender::BitmapCopy(ret, bmp, 0, (m_h-bmp->GetHeight())>>1);
		}
		return ret;
	}

	index = index%m_ElementCount;

	if (m_listFixed->count()==0)
		return 0;

	Bitmap* bmp = m_listFixed->getp(index);

	Bitmap* ret;
	
	if (m_bIsVertical==True)
	{
		ret = BITMAP32(m_w, bmp->GetHeight());
		Graphics g(ret);
		g.Clear(Color(m_colorBg));
		cyiBitmapBlender::BitmapCopy(ret, bmp, (m_w-bmp->GetWidth())>>1, 0);
	}
	else
	{
		ret = BITMAP32(bmp->GetWidth(), m_h);
		Graphics g(ret);
		g.Clear(Color(m_colorBg));
		cyiBitmapBlender::BitmapCopy(ret, bmp, 0, (m_h-bmp->GetHeight())>>1);
	}

	return ret;
}


// --------------------------------------------
int cglsImageList::Add(Bitmap* bmp)
{
	m_w = bmp->GetWidth()>m_w?bmp->GetWidth():m_w;
	m_h = bmp->GetHeight()>m_h?bmp->GetHeight():m_h;

	if (m_list)
	{
		int res = m_list->add(bmp);
		m_ElementCount=m_list->count();
		return res;
	}

	
	int res = m_listFixed->enq(bmp);
	m_ElementCount=m_listFixed->count();
	return res;
}

// --------------------------------------------
cglsImageList* cglsImageList::Import(Bitmap* bmp, int nCount, Bool bIsVertical, eGlsItemAlign align, int distance)
{
	int w = bmp->GetWidth();
	int h = bmp->GetHeight();
	int remain = bIsVertical?(h%nCount):(w%nCount);
	int nSplitSize = bIsVertical?(h/nCount):(w/nCount);

	if (remain)
		return 0;

	cglsImageList* img = new cglsImageList(nCount, bIsVertical, align, distance);

	if (bIsVertical)
	{
		int y=0;
		for (int i=0; i<nCount; i++)
		{
			img->Add(cyiBitmapBlender::BitmapCopy(0, bmp, 0, 0, 0, y, w, nSplitSize));
			y+=nSplitSize;
		}
	}
	else
	{
		int x=0;
		for (int i=0; i<nCount; i++)
		{
			img->Add(cyiBitmapBlender::BitmapCopy(0, bmp, 0, 0, x, 0, nSplitSize, h));
			x+=nSplitSize;
		}
	}

	return img;
}

// --------------------------------------------
Bitmap* cglsImageList::Export()
{
	int count= m_list?m_list->count():m_listFixed->count();
	int nMaxLength=0;
	int nMaxWidth=0;
	int y=0;

	Bitmap* bmpExported=0;

	if (m_bIsVertical)
	{
		goto VERTICAL;
	}

VERTICAL:
	if (m_list)
	{
		for (cyiBitmapNode* node=m_list->GetNodeTopFrom(); node; node=node->np)
		{
			Bitmap* bmp = node->ref;
			nMaxWidth = nMaxWidth<bmp->GetWidth()?bmp->GetWidth():nMaxWidth;
			nMaxLength+= bmp->GetHeight();
		}
	}
	else
	{
		for (int i=0; i<count; i++)
		{
			Bitmap* bmp = m_listFixed->getp(i);//node->ref;
			nMaxWidth = nMaxWidth<bmp->GetWidth()?bmp->GetWidth():nMaxWidth;
			nMaxLength+= bmp->GetHeight();
		}
	}

	bmpExported=BITMAP32(nMaxWidth, nMaxLength);

	if (m_list)
	{
		int y=m_distance;
		for (cyiBitmapNode* node=m_list->GetNodeTopFrom(); node; node=node->np)
		{
			Bitmap* bmp = node->ref;
			cyiBitmapBlender::BitmapCopy(bmpExported, bmp, 0, y, 0, 0, bmp->GetWidth(), bmp->GetHeight());
			y+=(m_distance+bmp->GetHeight());
			nMaxWidth = nMaxWidth<bmp->GetWidth()?bmp->GetWidth():nMaxWidth;
			nMaxLength+= bmp->GetHeight();
		}
	}
	else
	{
		for (int i=0; i<count; i++)
		{
			Bitmap* bmp = m_listFixed->getp(i);//node->ref;
			cyiBitmapBlender::BitmapCopy(bmpExported, bmp, 0, y, 0, 0, bmp->GetWidth(), bmp->GetHeight());
			y+=(m_distance+bmp->GetHeight());
			nMaxWidth = nMaxWidth<bmp->GetWidth()?bmp->GetWidth():nMaxWidth;
			nMaxLength+= bmp->GetHeight();
		}
	}

	return bmpExported;

}

#include <math.h>

// --------------------------------------------
Bitmap* cglsImageList::Get(float from, int length)
{
	int nCount=(m_list?m_list->count():m_listFixed->count());

	if (m_min==0 &&
			m_max==0)
	{
		goto NORMALRANGE;
	}

NORMALRANGE:
	if (from > (float)nCount)
	{
		if (m_nLimited)
		{
			from = from - (int)(from/(float)m_nLimited)*m_nLimited;
		}

		from = from - (int)(from/(float)nCount)*nCount;
	}
	else if (from < 0)
	{
		if (m_nLimited)
		{
			from = from - (int)(from/(float)m_nLimited)*m_nLimited;
			from = (float)m_nLimited+from;
		}
		else
		{
			from = from - (int)(from/(float)nCount)*nCount;
			from = (float)nCount+from;
		}
	}

	if (from<0)
	{
		float val, val2;
		int nFrom = (int)from;
		int nFromRemain = nFrom%nCount;
		val2 = modf(from, &val);

		if (m_nLimited)
		{
			nFromRemain = nFrom%m_nLimited;

			float x = (float)nFromRemain+val2;
			from = (float)nFromRemain+x;
		}
		else
		{
			float x = (float)nFromRemain+val2;
			from = (float)nCount+x;
		}
	}

	int nStartImageIndex=(int)from, nEndIndex;
	int nIndex=nStartImageIndex%nCount;//:(nCount+(nStartImageIndex%nCount)-1);

	if (length==-1)
	{
		return (Bitmap*)nIndex;
	}


	Bitmap* bmp = xGetBitmap(nIndex);
	int nPos = (int)((float)(from-(int)from)*(float)(m_bIsVertical?bmp->GetHeight():bmp->GetWidth()));
	int nRemain=length;


	int w = bmp->GetWidth();
	int h = bmp->GetHeight();
	Bitmap* ret=BITMAP32(w, h);

	
	//Graphics g(ret);

	//g.Clear(Color(m_colorBg));
	int nStart=0 , nEnd=0;


	if (m_bIsVertical)
	{
		goto VERTICAL;
	}


VERTICAL:
	//nPos=nStartImageIndex;
	int nDrawSum=0;
	while(nRemain)
	{
		cyiBitmapBlender::BitmapCopy(ret, bmp, 0, nDrawSum, 0, nPos, w, h-nPos);

		nRemain-=(h-nPos);
		nDrawSum+=(h-nPos);

		if (nRemain==0)
			break;

		nIndex++;

		bmp = xGetBitmap(nIndex);
		w = bmp->GetWidth();
		h = bmp->GetHeight();
		nPos=0;

		if (h>nRemain)
		{
			h=nRemain;
		}

	}
	delete bmp;
	return ret;
}

// --------------------------------------------
//Bitmap* cglsImageList::Get(Bitmap* bmp, float from, int length, eGlsItemAlign align)
//{
//}

// --------------------------------------------
Bitmap* cglsImageList::Get(int index)
{
	return xGetBitmap(index);
}

// --------------------------------------------
//Bitmap* cglsImageList::Get(Bitmap* bmp, int index, eGlsItemAlign align)
//{
//}


// --------------------------------------------
int cglsImageList::GetIndex(float from)
{
	int nCount=(m_list?m_list->count():m_listFixed->count());

	if (nCount==0 || nCount==1)
		return 0;

	int nStartImageIndex=(int)from;

	if (from<0 &&
			nStartImageIndex==0)
	{
		nStartImageIndex=-1;
	}

	return nStartImageIndex%nCount;
}



// --------------------------------------------
int cglsImageList::count()
{
	if (m_list)
		return m_list->count();

	return m_listFixed->count();
}

// --------------------------------------------
void cglsImageList::SetNumericRange(int min, int max)
{
	m_min=min;
	m_max=max;
}

// --------------------------------------------
void cglsImageList::SetLimited(int limited)
{
	m_nLimited=limited;
}

// --------------------------------------------
int cglsImageList::GetWidth()
{
	return m_w;
}

// --------------------------------------------
int cglsImageList::GetHeight()
{
	return m_h;
}

// --------------------------------------------
void cglsImageList::SetBitmapSize(int w, int h, color_t bgColor)
{
	m_w=w;
	m_h=h;

	m_colorBg=bgColor;
}

// --------------------------------------------
typedef struct {
	wchar_t* text;
	color_t colorText;
	color_t colorBg;
} sTextInfo;

// --------------------------------------------
int cglsImageList::Add(wchar_t* text, Font* font, color_t colorFont, color_t colorBg)
{
	sTextInfo info = {yistrheapW(text), colorFont, colorBg};
	
	Bitmap* bmp = cyiShape::BitmapStringEx(0, text, font, colorFont, colorBg);

	if (this->m_listString==0)
		m_listString = new cyiList();
	m_listString->addref(yitoheap(&info, sizeof(info)));

	return Add(bmp);
}

// --------------------------------------------
Bool cglsImageList::Update(Font* font)
{
	int count=0;
	if (m_listString==0)
		return False;
	//for (cyiNode* node=m_listString->GetNodeTopFrom(); node; node=m_listString->GetNodeTopFrom(0))
	//{
	//	sTextInfo info* info = (sTextInfo*)node->ref;
	//	yifree(info->text);
	//	yifree(node->ref);
	//	m_list->remove(node);
	//}

	count=m_listString->count();
	if (m_list)
	{
		for (cyitNode<Bitmap>* node=m_list->GetNodeTopFrom(); node; node=m_list->GetNodeTopFrom(0))
		{
			delete node->ref;
			m_list->remove(node);
		}
	}

	if (m_listFixed)
	{
		int cnt=m_listFixed->count();

		for (int i=0; i<cnt; i++)
			delete m_listFixed->getp(i);
		m_listFixed->clear();
	}

	if (m_listFixed)
		delete m_listFixed;

	if (m_list)
		delete m_list;
	m_list=0;

	m_listFixed = new cyiBitmapRb(count, True);

	m_w=0;
	m_h=0;
	for (cyiNode* node=m_listString->GetNodeTopFrom(); node; node=node->np)
	{
		sTextInfo* info = (sTextInfo*)node->ref;
		Bitmap* bmp = cyiShape::BitmapStringEx(0, info->text, font, info->colorText, info->colorBg);
		Add(bmp);
	}

	return True;
}

void glsInitAnimationEx(sGlsAnimationEx* ani, sDrawItem* item)
{
	memset(ani, 0, sizeof(*ani));

	ani->fFromTransparency=item->fTransparency;
	ani->fToTransparency=item->fTransparency;
	
	ani->fToSizeRate=item->fSizeRate;
	ani->fFromSizeRate=item->fSizeRate;
	
	ani->ptEnd.x = ani->ptStart.x = item->rect.x;
	ani->ptEnd.y = ani->ptStart.y = item->rect.y;

//	ani->term=term;
//	ani->msec=msec;
}

void glsSetTrAnimationEx(sGlsAnimationEx* ani, float fFrom, float fTo, eGlsAnimationPattern pattern)
{
	ani->fFromTransparency=fFrom;
	ani->fToTransparency=fTo;
	ani->eTrPattern=pattern;
}

void glsSetSzAnimationEx(sGlsAnimationEx* ani, float fFrom, float fTo, eGlsAnimationPattern pattern, eGlsItemAlign align)
{
	ani->fFromSizeRate=fFrom;
	ani->fToSizeRate=fTo;
	ani->eSzPattern=pattern;
	ani->align=align;
}

void glsSetDvAnimationEx(sGlsAnimationEx* ani, int from, int to, eGlsAnimationPattern pattern)
{
	ani->nBmpStartIndex=from;
	ani->nBmpEndIndex=to;
	ani->eDvPattern=pattern;
}

void glsSetPtAnimationEx(sGlsAnimationEx* ani, POINT* from, POINT* to, eGlsAnimationPattern patternX, eGlsAnimationPattern patternY)
{
	ani->ptStart=*from;
	ani->ptEnd=*to;
	ani->eMvPatternX=patternX;
	ani->eMvPatternY=patternY;
}
