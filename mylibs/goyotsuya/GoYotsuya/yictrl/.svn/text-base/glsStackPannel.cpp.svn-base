#include "glsStackPannel.h"
#include "yiBitmap.h"

#define __xITEM_LOCK__ \
MutexLock();
//m_listItem->lock();\
//m_listFreeItem.lock()

#define __xITEM_UNLOCK__ \
MutexUnlock();
//m_listItem->unlock();\
//m_listFreeItem.unlock()


cglsStackPannel::cglsStackPannel(void)
: cglsGaussPannel(),
m_eSelectedStatus(eDIS_INVALID),
m_itemSelected(0),
m_itemMouseMove(0),
m_itemMouseDown(0),
m_eStackFlag(0),
m_bg(0),
m_nItemzLength(0),
m_nItemzWidth(0),
m_listFreeItem(True),
m_bIsRequestCheckItem(False), 
m_nLastSelectedParts(0),
m_nBeforeScrollPos(0),
m_wMargin(0),
m_hMargin(0),
m_eScrollBarState(eSPSBS_MOUSE_LEAVE),
m_nSelectedScrollBarParts(-1),
m_fScrollLastFactor(1.0),
m_hScrollNotifyWnd(0),
m_listItem(0)
{
	m_hWnd=0;
	m_DrawObject=0;
	m_bIsBeforeAdded=False;
	memset(&m_item, 0, sizeof(m_item));

	m_item.fTransparency=1.0;
	memset(&m_fgItem, 0, sizeof(m_fgItem));

	memset(&m_itemScrollArea, 0, sizeof(sDrawItem));
	memset(&m_itemScrollBarPrev, 0, sizeof(sDrawItem));
	memset(&m_itemScrollBarNext, 0, sizeof(sDrawItem));
	memset(&m_itemScrollBar, 0, sizeof(sDrawItem));
	memset(&m_itemScrollBarPage, 0, sizeof(sDrawItem));
	memset(&m_scrollInfo, 0, sizeof(m_scrollInfo));
	memset(&m_rectSystem, 0, sizeof(RECT));

	//m_rectSystem.bottom=200;
	m_listItem = new cyiList(True);
	xSET_ITEM_HIDE((&m_itemScrollArea));
	__GLASSINTERFACE_NAME__;

	//m_bg=0;
}

cglsStackPannel::~cglsStackPannel(void)
{
}

// ---------------------------------------
//static BOOL xIsInRect(yiRect* rect, int x, int y, /*Bool bIsCoordChanged=False, */int posX=0, int posY=0)
//{
//	x+=posX;
//	y+=posY;
//
//	if (x<rect->x)	
//		return FALSE;
//	if (x>(rect->w+rect->x))
//		return FALSE;
//	if (y>(rect->h+rect->y))
//		return FALSE;
//	if (y<rect->y)
//		return FALSE;
//
//	return TRUE;
//}

// ---------------------------------------
sDrawItem* cglsStackPannel::xGetItemIn(int x, int y)
{
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);
	x+=m_bIsVertical?0:m_nScrollPos;
	y+=m_bIsVertical?m_nScrollPos:0;

	for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item = (sDrawItem*)node->ref;

		if (xIsInRect(&item->rect, x, y)==TRUE)
		{
			return item;
		}
	}

	return 0;
}

// --------------------------------------------
void cglsStackPannel::xDefaultDetail(sGlsStackPannelDetail* detail)
{
	cGlassInterface::xDefaultDetail(&detail->gls);
	cglsGaussPannel::xDefaultDetail(&detail->gauss);

	detail->bUseGaussPannel=False;
	detail->gls.nKeyLButtonDownTimer=500;
	detail->bIsFixedMode=False;
	detail->bIsVertical=False;
	detail->eAlign=eSA_CENTER;
	detail->fEnterTransparency=1.0;
	detail->fLeaveTransparency=1.0;
	detail->pszToolTip=0;
	detail->bIsInsertAndCreate=True;
	detail->fScrollBarEnterTransparency=0.8;
	detail->fScrollBarLeaveTransparency=0.3;
	detail->bIsScrollDrawDirect=True;
	detail->bIsNomalScrollMode=False;
	detail->bEnableResize=False;
	detail->bIsReSizeManual=False;

	if (detail->bIsFixedMode==True)
	{
		detail->bIsShowScrollBar=True;
	}
	else
		detail->bIsShowScrollBar=False;
}

// --------------------------------
void cglsStackPannel::xDefaultItemDetail(sGlsItemDetail* detail)
{
	detail->eType=eSPT_NORMAL;
	detail->eAlign=eSA_CENTER;
	detail->param=0;
	detail->bIsDragAble=False;
	detail->bIsTransparant=True;
	detail->bInitVisible=True;
	detail->fEnterTransparency=1.0;
	detail->fLeaveTransparency=1.0;
	detail->colorBmp=0;
	detail->colorFont=0;
}

// --------------------------------
//void cglsStackPannel::InitDetail()
//{
//	__super::InitDetail(m_detail.gls);
//
//	if (m_fLeaveTransparency!=-1.0)
//	{
//		m_item.fTransparency=m_fLeaveTransparency;
//	}
//
//	if (m_detail.gls.bIsNoAnimation==False)
//	{
//		if(m_item.hAnit==0)
//		{
//			m_item.hAnit=yiAnitCreate();
//		}
//	}
//	xSET_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL, m_detail.bIsVertical);
//
//	//m_bIsVertical = m_detail.bIsVertical;
//}

// --------------------------------------------
Bool cglsStackPannel::Create(HWND hWnd, int id, RECT* rect, RECT* margin, cglsDrawObject* obj, sGlsStackPannelDetail* detail/*=0*/)
{
	sGlsStackPannelDetail m_detail;

	if (detail)
		m_detail=*detail;
	else
		xDefaultDetail(&m_detail);

	xSET_GLS_FLAG(m_eStackFlag, eSPF_ENABLE_RESIZE, m_detail.bEnableResize);
	xSET_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE, m_detail.bIsFixedMode);
	xSET_GLS_FLAG(m_eStackFlag, eSPF_INSERT_AND_CREATE, m_detail.bIsInsertAndCreate);
	xSET_GLS_FLAG(m_eStackFlag, eSPF_NORMAL_SCROLLMODE, m_detail.bIsNomalScrollMode);
	xSET_GLS_FLAG(m_eStackFlag, eSPF_SCROLL_DIRECT, m_detail.bIsScrollDrawDirect);
	xSET_GLS_FLAG(m_eStackFlag, eSPF_SHOW_SCROLLBAR, m_detail.bIsShowScrollBar);
	xSET_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL, m_detail.bIsVertical);
	xSET_GLS_FLAG(m_eStackFlag, eSPF_USE_GAUSS_PANNEL, m_detail.bUseGaussPannel);
	xSET_GLS_FLAG(m_eStackFlag, eSPF_RESIZE_IS_MANUAL, m_detail.bIsReSizeManual);

	m_fScrollBarLeaveTransparency = m_detail.fScrollBarLeaveTransparency;
	m_fScrollBarEnterTransparency = m_detail.fScrollBarEnterTransparency;

	m_item.fLeaveTransparency=m_detail.fLeaveTransparency;
	m_item.fEnterTransparency=m_detail.fEnterTransparency;
	m_item.fTransparency=m_item.fLeaveTransparency;
	

	if (m_detail.bUseGaussPannel==True)
	{
		if (IsWindow(hWnd)==FALSE)
		{
			cGlassInterface* parent = (cGlassInterface*)hWnd;
			if (parent)
			{
				cglsSystem* system = parent->GetSystem();
				if (system)
				{
					int n = system->GetPerformance();

					if (n==0)
					{
						m_detail.gauss.fBlureRad=2.0;
					}
					else if (n==1)
					{
						m_detail.gauss.fBlureRad=3.0;
					}
					else if (n==2)
					{
						m_detail.gauss.fBlureRad=4.0;
					}
				}
			}
		}

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True)
		{
			if (xIS_GLS_FLAG(m_eStackFlag, eSPF_USE_GAUSS_PANNEL)==True)
			{
				m_rectMargin.left+=m_detail.gauss.fShadowWidth;
				m_rectMargin.top+=m_detail.gauss.fShadowWidth;
				m_rectMargin.right+=m_detail.gauss.fShadowWidth;
				m_rectMargin.bottom+=m_detail.gauss.fShadowWidth;
			}
		}
		SetGaussPannelDetail(&m_detail.gauss);
	}
	

	//m_detail.gls.nDragOnTimer=1000;
	if (m_detail.bIsFixedMode==True)
	{
		if (CreateFixed(hWnd, id, rect, margin, obj, m_detail.bIsVertical, True, detail->eAlign, m_detail)==False)
		{
			return False;
		}

		if (detail->pszToolTip)
			cGlassInterface::CreateToolTip(detail->pszToolTip, 500, 0);
		return True;
	}

	//InitDetail();
	
	POINT pt={0, 0};

	if (rect)
	{
		pt.x=rect->left;
		pt.y=rect->top;
	}


	if (CreateDynamicSize(hWnd, id, &pt, margin, obj, m_detail.bIsVertical, True, m_detail.eAlign, m_detail)==True)
	{
		if (m_detail.pszToolTip)
			cGlassInterface::CreateToolTip(m_detail.pszToolTip, 500, 0);
		m_hScrollNotifyWnd=m_hNotifyWnd;
		return True;
	}

	return False;
}


// --------------------------------------------
Bool cglsStackPannel::CreateFixed(HWND hWnd, int id, RECT* rect, RECT* margin, cglsDrawObject* obj, Bool bIsVertical, Bool bIsNotOwnDraw, eStackAlign align, sGlsStackPannelDetail& m_detail)
{
	xSET_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL, bIsVertical);

	m_eAlign=align;
	m_itemMouseDown=0;
	m_itemMouseMove=0;
	m_hScrollAnit=0;

	m_nScrollPos=0;

	m_DrawObject=obj;

	xSET_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW, bIsNotOwnDraw);

	if (m_DrawObject &&
			m_DrawObject->m_bIsTypeUserDraw==True)
	{
		cglsUserDrawObject* x = (cglsUserDrawObject* )m_DrawObject;
		x->m_Itemz = m_listItem;
	}

	cGlassInterface::InitResource();

	memset(&m_rectMargin, 0, sizeof(RECT));

	if (margin)
		m_rectMargin=*margin;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_USE_GAUSS_PANNEL)==True)
	{
		m_rectMargin.left+=m_detail.gauss.fShadowWidth+1;//rect.left;
		m_rectMargin.right+=m_detail.gauss.fShadowWidth+1;
		m_rectMargin.top+=m_detail.gauss.fShadowWidth+1;
		m_rectMargin.bottom+=m_detail.gauss.fShadowWidth+1;
	}
	
	if (cGlassInterface::Create(hWnd, 0, id, rect, &m_detail.gls)==False)
	{
		return False;
	}

	if (rect)
	{
		ReSize(rect->right, rect->bottom);
		Move(rect->left, rect->top);
	}

	return True;
}

// --------------------------------------------
Bool cglsStackPannel::CreateDynamicSize(HWND hWnd, int id, POINT* pt, RECT* margin, cglsDrawObject* obj, Bool bIsVertical, Bool bIsNotOwnDraw, eStackAlign align, sGlsStackPannelDetail& m_detail)
{
	RECT rect={pt?pt->x:0, pt?pt->y:0, 10, 10};
	xOFF_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE);
	return CreateFixed(hWnd, id, &rect, margin, obj,  bIsVertical, bIsNotOwnDraw, align, m_detail);
}

// -----------------------------
static void xInitItem(sDrawItem* item, sGlsItemDetail* detail)
{
	if (detail->bInitVisible)
		xSET_ITEM_SHOW(item);
	else
		xSET_ITEM_HIDE(item);

	if (detail->bIsDragAble)
		xSET_ITEM_DRAG_ON(item);
	else
		xSET_ITEM_DRAG_OFF(item);

	xSET_ITEM_ALIGN(item, detail->eAlign);
	xSET_ITEM_TYPE(item, detail->eType);

	
//	item->fEnterTransparency=detail->fEnterTransparency;
//	item->fLeaveTransparency=detail->fLeaveTransparency;
	item->param=detail->param;

	item->fTransparency=detail->fLeaveTransparency;
	item->fEnterTransparency=detail->fEnterTransparency;
	item->fLeaveTransparency=detail->fLeaveTransparency;

	
}

// -----------------------------
int cglsStackPannel::Insert(int id, int index, Bitmap* bmp, const wchar_t* pszText, int distance, 
							const wchar_t* pszToolTip/*=0*/, sGlsItemDetail* detail)
{
	sGlsItemDetail xdetail;
	sDrawItem item;
	memset(&item, 0, sizeof(item));

	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	xDefaultItemDetail(&xdetail);

	unsigned int t1 = cyiTime::CurrentmSec();
	//YIDEBUG("trace..............1 cnt:(%d)", m_listItem->count());

	if (detail)
	{
		xdetail=*detail;
	}

	xInitItem(&item, &xdetail);

	item.id=id;
	item.bitmapId=(IntPtr)bmp;
	item.colorFont=xdetail.colorFont;
	item.colorBmp = xdetail.colorBmp;

	if (pszText)
	{
		item.txt = yistrheapW(pszText);
	}

	item.distance=distance;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_INSERT_AND_CREATE)==True)
	{
		if (m_DrawObject && 
			m_DrawObject->CreateBitmap(&item, item.param)==False)
		{
			return False;
		}
	}
	//}
	//YITRACE(5, "trace....2 %d msec", cyiTime::GetSecDiff(t1, cyiTime::CurrentmSec()));
	if (xIS_GLS_FLAG(m_flag, eGF_NO_ANIMATION)==False &&
			xdetail.eType!=eSPT_STATIC &&
				xdetail.eType!=eSPT_SPACE &&
					xdetail.eType!=eSPT_GLASSINTERFACE)
	{
		item.hAnit = yiAnitCreate();
	}

	//YITRACE(5, "trace....3 %d msec", cyiTime::GetSecDiff(t1, cyiTime::CurrentmSec()));

	if (xdetail.eType==eSPT_SPACE)
	{
		item.rect.x=xdetail.rect.left;
		item.rect.y=xdetail.rect.top;
		item.rect.w=xdetail.rect.right;
		item.rect.h=xdetail.rect.bottom;
	}

	if (index==-1)
		m_listItem->addref(yitoheap(&item, sizeof(item)));
	else
		m_listItem->insertref(index, yitoheap(&item, sizeof(item)));
	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_INSERT_AND_CREATE)==False)
	{
		return True;
	}
	//YITRACE(5, "trace....4 %d msec", cyiTime::GetSecDiff(t1, cyiTime::CurrentmSec()));
	if (m_hWnd==0)
	{
		m_bIsBeforeAdded=True;
	}

	if (index==-1 && xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED)==True)
	{
		RECT rect;
		xCalculatePosition(&rect, True);

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)==True)
		{
			ReSize(m_bmpCurrent->GetWidth(), m_h);
		}
		else
		{
			ReSize(m_w, m_bmpCurrent->GetHeight());
		}
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED)==True)
		{
			Update(eGUM_UPDATE_LASTITEM);
			return True;
		}


		Update(eGUM_UPDATE_LASTITEM);

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_SHOW_SCROLLBAR)==True)
		{
			if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False)
			{
				SetScrollRange(0, m_bIsVertical?m_bmpCurrent->GetHeight():m_bmpCurrent->GetWidth(), m_bIsVertical?m_h:m_w);
			}
		}

		return True;
	}
	//YITRACE(5, "trace....5 %d msec", cyiTime::GetSecDiff(t1, cyiTime::CurrentmSec()));
	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True || 
			IsWindowVisible(m_hWnd)==TRUE)
	{
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_SHOW_SCROLLBAR)==True)
		{
			if (m_bmpCurrent)
			{
				SetScrollRange(0, m_bIsVertical?m_bmpCurrent->GetHeight():m_bmpCurrent->GetWidth(), m_bIsVertical?m_h:m_w);
			}
			else
			{
				SetScrollRange(0, m_bIsVertical?m_h:m_w, m_bIsVertical?m_h:m_w);
			}
		}
	}
	//YITRACE(5, "trace....6 %d msec", cyiTime::GetSecDiff(t1, cyiTime::CurrentmSec()));

	return True;

}

// -----------------------------
int cglsStackPannel::Insert(int index, cGlassInterface* pannel, int distance, eStackAlign align)
{
	//sDrawItem item;
	//memset(&item, 0, sizeof(item));

	//item.id=id==-1?pannel->m_id:id;
	//item.gls=pannel;
	//item.distance=distance;
	
	//xSET_ITEM_TYPE((&item), eSPT_GLASSINTERFACE);

	//if (::IsWindowVisible(*pannel)==FALSE)
	//{
	//	xSET_ITEM_HIDE((&item));
	//}
	sDrawItem* item=&pannel->m_item;
	xSET_ITEM_ALIGN(item, align);
	xSET_ITEM_TYPE(item, eSPT_GLASSINTERFACE);
	item->gls=pannel;
	item->distance=distance;

	if (index==-1)
		m_listItem->addref(item);//yitoheap(&item, sizeof(item)));
	else
		m_listItem->insertref(index, item);//yitoheap(&item, sizeof(item)));


	if (index==-1 && xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED)==True)
	{
		RECT rect;
		xCalculatePosition(&rect, True);
		Update();

		return True;
	}

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True || 
			IsWindowVisible(m_hWnd)==TRUE)
	{
		Update(eGUM_SKIP_REDRAW);
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_SHOW_SCROLLBAR)==True)
		{
			SetScrollRange(0, m_nItemzLength, xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?m_h:m_w);
			//SetScrollRange(0, m_bIsVertical?m_bmpCurrent->GetHeight():m_bmpCurrent->GetWidth(), m_bIsVertical?m_h:m_w);
		}
	}

	return 1;
}

// -----------------------------
int cglsStackPannel::InsertLF(int index, int distance)
{
	sDrawItem item;
	memset(&item, 0, sizeof(item));
	xSET_ITEM_TYPE((&item), eSPT_LF);
	item.distance=distance;

	if (index==-1)
	{
		cyiNode* node = m_listItem->GetNodeTailFrom(0);
		sDrawItem* xx = (sDrawItem*)node->ref;

		if (xx && xGET_ITEM_TYPE(xx)==eSPT_LF)
		{
			return -1;
		}
		m_listItem->addref(yitoheap(&item, sizeof(item)));
	}
	else
	{
		cyiNode* node = m_listItem->GetNodeTopFrom(index);

		if (node)
		{
			sDrawItem* xx = (sDrawItem*)node->ref;
			if (xx && xGET_ITEM_TYPE(xx)==eSPT_LF)
			{
				return -1;
			}
		}

		node=m_listItem->GetNodeTopFrom(index+1);
		if (node)
		{
			sDrawItem* xx = (sDrawItem*)node->ref;
			if (xx && xGET_ITEM_TYPE(xx)==eSPT_LF)
			{
				return -1;
			}
		}

		m_listItem->insertref(index, yitoheap(&item, sizeof(item)));
	}

	if (index!=-1)
	{
		RECT rect;
		xCalculatePosition(&rect, True);
		Update();
	}

	return m_listItem->count();
}

// -----------------------------
int cglsStackPannel::AddLF(int distance)
{
	return InsertLF(-1, distance);
}

// -----------------------------
int cglsStackPannel::InsertSpace(int index, int width, int height)
{
	sDrawItem item;
	memset(&item, 0, sizeof(item));
	xSET_ITEM_TYPE((&item), ::eSPT_SPACE);
	item.rect.w=width;
	item.rect.h=height;
	item.distance=0;

	if (index==-1)
		m_listItem->addref(yitoheap(&item, sizeof(item)));
	else
		m_listItem->insertref(index, yitoheap(&item, sizeof(item)));

	if (index!=-1)
	{
		RECT rect;
		xCalculatePosition(&rect, True);
		Update();
	}

	return m_listItem->count();
}

// -----------------------------
int cglsStackPannel::AppSpace(int width, int height)
{
	return InsertSpace(-1, width, height);
}

// -----------------------------
void cglsStackPannel::SetScrollNotifyWindow(HWND hWnd)
{
	m_hScrollNotifyWnd=hWnd;
}


// -----------------------------
int cglsStackPannel::InsertFreeItem(int id, int index, int x, int y, Bitmap* bmp, const wchar_t* pszText, 
									const wchar_t* pszToolTip/*=0*/, sGlsItemDetail* detail)
{
	sGlsItemDetail xdetail;
	sDrawItem item;
	memset(&item, 0, sizeof(item));

	xDefaultItemDetail(&xdetail);

	if (detail)
	{
		xdetail=*detail;
	}

	xInitItem(&item, &xdetail);

	xSET_ITEM_FREE((&item));

	item.rect.x=x;
	item.rect.y=y;

	item.id=id;
	item.bitmapId=(IntPtr)bmp;

	if (pszText)
	{
		item.txt = yistrheapW(pszText);
	}

	xSET_ITEM_TYPE((&item), xdetail.eType);

	if (m_DrawObject && 
		m_DrawObject->CreateBitmap(&item, item.param)==False)
	{
		return False;
	}

	if (xIS_GLS_FLAG(m_flag, eGF_NO_ANIMATION)==False &&
			xdetail.eType!=eSPT_STATIC &&
				xdetail.eType!=::eSPT_GLASSINTERFACE)
	{
		item.hAnit = yiAnitCreate();
	}


	if (index==-1)
		m_listFreeItem.addref(yitoheap(&item, sizeof(item)));
	else
		m_listFreeItem.insertref(index, yitoheap(&item, sizeof(item)));


	if (m_hWnd==0)
	{
		m_bIsBeforeAdded=True;
	}
	else
	{
		RECT rect = {item.rect.x, item.rect.y, item.rect.w+item.rect.x, item.rect.h+item.rect.y};
		InvalidateRect(&rect, False);
	}

	return m_listFreeItem.count();

}

// -----------------------------
int cglsStackPannel::InsertFreeGlsItem(int id, int index, int x, int y, cGlassInterface* obj)
{
	//sDrawItem item;
	//memset(&item, 0, sizeof(item));

	//item.id=id;
	sDrawItem* item = &obj->m_item;
	item->gls=obj;
	//item.param=(void*)obj;
	
	xSET_ITEM_TYPE(item, eSPT_GLASSINTERFACE);
	xSET_ITEM_FREE(item);


	if (::IsWindowVisible(*obj)==FALSE)
	{
		xSET_ITEM_HIDE(item);
	}

	if (index==-1)
		m_listFreeItem.addref(item);//yitoheap(&item, sizeof(item)));
	else
		m_listFreeItem.insertref(index, item);//index, yitoheap(&item, sizeof(item)));


	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True || 
			IsWindowVisible(m_hWnd)==TRUE)
	{
		RECT rect = {item->rect.x, item->rect.y, item->rect.w+item->rect.x, item->rect.h+item->rect.y};
		InvalidateRect(&rect, False);

	}

	return m_listFreeItem.count();
}



// -----------------------------
int cglsStackPannel::Add(int id, Bitmap* bmp, const wchar_t* pszText, int distance, 
						 const wchar_t* pszToolTip/*=0*/, sGlsItemDetail* detail)
{
	return Insert(id, -1, bmp, pszText, distance, pszToolTip, detail);
}

// -----------------------------
int cglsStackPannel::Add(cGlassInterface* pannel, int distance, eStackAlign align)
{
	return Insert(-1, pannel, distance, align);
}

// -----------------------------
int cglsStackPannel::AddFreeItem(int id, int x, int y, Bitmap* bmp, const wchar_t* pszText, 
								  const wchar_t* pszToolTip/*=0*/, sGlsItemDetail* detail)
{
	return InsertFreeItem(id, -1, x, y, bmp, pszText, pszToolTip, detail);
}

// -----------------------------
int cglsStackPannel::AddFreeGlsItem(int id, int x, int y, cGlassInterface* obj)
{
	return InsertFreeGlsItem(id, -1, x, y, obj);
}

// -----------------------------
int cglsStackPannel::IndexToId(int index)
{
	sDrawItem* item = (sDrawItem*)m_listItem->GetTopFromRef(index);
	if (item==0)
		return 0;
	return item->id;
}

// -----------------------------
int cglsStackPannel::IdToIndex(int id)
{
	int cnt=0;
	for (cyiNode* node = m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item = (sDrawItem*)node->ref;

		if (item->id==id)
		{
			return cnt;
		}
		cnt++;
	}

	return -1;
}


// -----------------------------
sDrawItem** cglsStackPannel::GetExportItemz(Bool bIsRemoveFromList)
{
	int n = m_listItem->count();
	int cnt=0;
	sDrawItem** p = (sDrawItem**)yialloc(n*sizeof(sDrawItem*));

	for (cyiNode* node = m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		p[cnt++]=(sDrawItem*)node->ref;
	}
	
	if (bIsRemoveFromList==True)
		m_listItem->clear();

	return p;
}

// -----------------------------
Bool cglsStackPannel::ImportItemz(sDrawItem** p, int n)
{
	for (int i=0; i<n; i++)
	{
		m_listItem->addref(p[i]);
	}

	RECT all;
	xCalculatePosition(&all);
	InvalidateRect(0);
	return True;
}

// -----------------------------
Bool cglsStackPannel::OnSelfBgDraw(Bitmap* bmp, RECT* rect, RECT* rectChild)
{
	return False;
}

// -----------------------------
Bool cglsStackPannel::OnSelfFgDraw(Bitmap* bmp, RECT* rect, RECT* rectChild)
{
	return False;
}

// -----------------------------
void cglsStackPannel::SetBufferSize(RECT& rect, Bool bIsVertual)
{
	//int nVirtualLength=0;
	MutexLock();
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);
	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==False)
	{
		xOFF_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED);
		//m_bIsBufferSizeFixed=False;
		MutexUnlock();
		return ;
	}
	
	//m_bIsBufferSizeFixed=True;
	xON_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED);
	if (bIsVertual==True)
	{
		xON_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL);

		int nVirtualLength = m_bIsVertical?rect.bottom:rect.right;
		rect.left=0;
		rect.top=0;
		rect.right=m_w;
		rect.bottom=m_h;


		if (m_bmpCurrent==0)
		{
			m_bmpCurrent=BITMAP32(m_w, m_h);
		}

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_SHOW_SCROLLBAR)==True)
		{
			if (m_bIsVertical)
				SetScrollRange(0, nVirtualLength, m_h);
			else
				SetScrollRange(0, nVirtualLength, m_w);

			SetScrollPos(0);
		}

		cyiBitmapBlender::FillColor(m_bmpCurrent, 0);
		
	}
	else
	{
		if (m_bmpCurrent)
		{
			if (m_bmpCurrent->GetWidth()==rect.right &&
					m_bmpCurrent->GetHeight()==rect.bottom)
			{
				MutexUnlock();

				return ;
			}

			delete m_bmpCurrent;
			m_bmpCurrent=0;
		}
	}

	if (m_bg)
	{
		delete m_bg;
		m_bg=0;
	}
	
	m_item.rect.w=rect.right;
	m_item.rect.h=rect.bottom;

	if (m_DrawObject)
	{
		if (m_DrawObject->CreateBitmap((sDrawItem*)&m_item, 0)==True)
		{
			m_bg = cyiBitmapBlender::BitmapCopy(0, m_item.pCurrentBmp);
		}
		else
		{
			if (bIsVertual==False)
			{
				m_bg=0;
				m_bmpCurrent=BITMAP32(rect.right, rect.bottom);
			}
		}
	}

	if (m_bg)
		m_bmpCurrent = cyiBitmapBlender::BitmapCopy(0, m_bg);

	xON_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED);

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_SHOW_SCROLLBAR)==True)
	{
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False)
		{
			if (m_bIsVertical)
				SetScrollRange(0, m_item.rect.h, m_h);
			else
				SetScrollRange(0, m_item.rect.w, m_w);

			SetScrollPos(0);
		}

	}
	
	MutexUnlock();

}

// -----------------------------
sDrawItem* cglsStackPannel::GetItem(int id)
{
	Bool bIsQuit=False;
	cyiList* list=m_listItem;

TOLOOP:
	list->lock();
	for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item=(sDrawItem* )node->ref;

		if (item->gls)
		{
			if (item->gls->m_id==id)
			{
				return item;
			}
		}
		if (item->id==id)
		{
			list->unlock();
			return item;
		}
	}

	list->unlock();

	if (bIsQuit==False)
	{
		list=&m_listFreeItem;
		bIsQuit=True;
		goto TOLOOP;
	}

	return 0;
}

// -----------------------------
int cglsStackPannel::GetItemCount()
{
	return m_listItem->count();
}

// -----------------------------
int cglsStackPannel::GetCurrentScrolledPos(float* ratio)
{
	if (ratio)
	{
		int nLength = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?m_bmpCurrent->GetHeight():m_bmpCurrent->GetWidth();
		nLength -= xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?m_h:m_w;

		*ratio=(float)((float)m_nScrollPos/(float)nLength);
	}

	return m_nScrollPos;
}


// -----------------------------
static sDrawItem* xGetItem(int id, cyiList* list)
{
	for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item=(sDrawItem*)node->ref;

		if (id==item->id)
		{
			return item;
		}
	}

	return 0;
}

// -----------------------------
void cglsStackPannel::SetItem(int id, int bitmapId, const wchar_t* pszText, Bool bIsUpdate)
{
	sDrawItem* item = xGetItem(id, m_listItem); //m_listItem->GetNodeTopFrom(index)->ref;

	if (item)
	{
		if (pszText!=(const wchar_t*)-1)
		{
			if (item->txt)
				yifree(item->txt);
			item->txt=yistrheapW(pszText);
		}

		if (bitmapId!=-1)
		{
			item->bitmapId=bitmapId;
		}

		m_DrawObject->CreateBitmap(item);
		
		if (bIsUpdate==True)
		{
			Update(eGUM_FORCED);
			InvalidateRect(0);
		}
	}
	return ;
}

// -----------------------------
int cglsStackPannel::GetId(int index)
{
	sDrawItem* s = (sDrawItem*)m_listItem->GetNodeTopFrom(index)->ref;

	if (s->gls)
		return s->gls->m_id;
	return s->id;

}

// -----------------------------
int cglsStackPannel::GetIndex(int id)
{
	int cnt=0;
	for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item=(sDrawItem*)node->ref;

		if (item->gls)
		{
			if (item->gls->m_id==id)
			{
				return cnt;
			}
		}
		else if (item->id==id)
			return cnt;

		cnt++;
	}

	return -1;
}

// ------------------------------------
void cglsStackPannel::GetMargin(RECT& rect)
{
	rect=m_rectMargin;
}

// -----------------------------
void cglsStackPannel::xDestroyItem(sDrawItem * x, Bool bIsFree)
{
	// scroll bar?
	if (x->id==-2)
	{
		return ;
	}

	if (xGET_ITEM_TYPE(x)==eSPT_GLASSINTERFACE)
	{
		cGlassInterface* obj = (cGlassInterface*)x->gls;

		if (m_children)
		{
			for (cyiNode* node=m_children->GetNodeTopFrom(); node;node=node->np)
			{
				if (node->ref==x->gls)
				{
					m_children->remove(node);
					break;
				}
			}
		}
		if (bIsFree)
		{
			obj->Destroy();
			delete obj;
			//yifree(x); 
		}
		return ;
	}

	if (m_DrawObject)
		m_DrawObject->DestroyBitmap(x);

	if (x->txt)
		yifree(x->txt);

	if (bIsFree==True)
	{
		// どうする？
		if (x->hAnit)
		{
			if (yiAnitIsAlived(x->hAnit)==True)
			{
				yiAnitStop(x->hAnit, True);
				yiAnitDestroy(x->hAnit);
				x->hAnit=0;
			}
		}
		yifree(x);
	}
}

// -----------------------------
void cglsStackPannel::Remove(unsigned int id, Bool bIsFree)
{

	for (cyiNode* p = m_listItem->GetNodeTopFrom(0); p!=0; p=p->np)
	{
		sDrawItem* x = (sDrawItem*)p->ref;

		if (x->gls && x->gls->m_id==id)
		{
			m_bIsRequestCheckItem=True;
			if (x->gls)
			{
				UnRegisterChild(x->gls);
				m_listItem->remove(p);
			}

			// 削除
			if (bIsFree)
			{
				x->gls->Destroy();
				delete x;
			}
			break;
		}
		if (id==x->id)
		{
			m_bIsRequestCheckItem=True;
			m_listItem->remove(p);
			xDestroyItem(x, bIsFree);
			break;
		}
	}

}



// -----------------------------
Bool cglsStackPannel::ReSize(UINT x, UINT y, Bool bIsRepaint)
{
	if (cGlassInterface::ReSize(x, y, bIsRepaint)==True)
	{
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED)==True)
		{
			
		}
		if (m_bmpCurrent)
			delete m_bmpCurrent;
		m_bmpCurrent = BITMAP32(x, y);

		// スクロールバーの位置やサイズも変更する。

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_SHOW_SCROLLBAR)==True)
		{
			xCreateScrollBar(True);
		}
		return True;
	}
	return False;
	//m_bmpCurrent
	//return cGlassInterface::ReSize(x, y, bIsRepaint);
	//return cyctrlbase::ReSize(x, y, bIsRepaint);

	//if (m_bIsSizeFixed==True)
	//{
	//	if (cyctrlbase::ReSize(x, y, bIsRepaint)==True)
	//	{
	//		Update(eGUM_FORCED);
	//	}
	//}
	//return True;
}


// -----------------------------
sDrawItem* cglsStackPannel::RemoveFromList(unsigned int id)
{
	for (cyiNode* p = m_listItem->GetNodeTopFrom(0); p!=0; p=p->np)
	{
		sDrawItem* x = (sDrawItem*)p->ref;
		if (id==x->id)
		{
			m_bIsRequestCheckItem=True;
			m_listItem->remove(p);
			return x;
		}
	}

	return 0;
}

// -----------------------------
sDrawItem* cglsStackPannel::GetDrawItem(unsigned int id)
{
	for (cyiNode* p = m_listItem->GetNodeTopFrom(0); p!=0; p=p->np)
	{
		sDrawItem* x = (sDrawItem*)p->ref;
		if (id==x->id)
		{
			return x;
		}
	}
	
	return 0;
}

void OnDeleteItem(void* p)
{
	cyiList* x = (cyiList*)p;
	cglsStackPannel* pClass = (cglsStackPannel*)x->GetNodeTopFrom(0)->ref;

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);

	// クラスのポインターを持つ最後のノードは削除。
	x->remove(x->GetNodeTopFrom(0));

	// アニメーションと競合が起きる可能性がある。
	for (cyiNode* p = x->GetNodeTopFrom(0); p!=0; p=p->np)
	{
		sDrawItem* z = (sDrawItem*)p->ref;

		if (z->hAnit &&
			::yiAnitIsAlived(z->hAnit)==True)
		{
			Sleep(50);
		}

		pClass->xDestroyItem(z);
	}
	delete x;
}

// -----------------------------
void cglsStackPannel::Clear()
{
	m_bIsRequestCheckItem=True;

	m_NowAnimationList.clear();

	if (m_item.hAnit!=0)
	{
		yiAnitStop(m_item.hAnit, True);
	}

	if (m_fgItem.hAnit)
	{
		yiAnitStop(m_fgItem.hAnit, True);
	}

	if (m_listItem!=0 &&
			m_listItem->count()!=0)
	{
		m_listItem->insertref(0, this);
		_beginthread(OnDeleteItem, 0, m_listItem);
		m_listItem = new cyiList(True);
	}
	/*for (cyiNode* p = m_listItem->GetNodeTopFrom(0); p!=0; p=p->np)
	{
		sDrawItem* x = (sDrawItem*)p->ref;
		xDestroyItem(x);
	}*/

MutexLock();
	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==False)
	{
		xDestroyItem(&m_item, False);
		xDestroyItem(&m_fgItem, False);

		if (m_bmpCurrent)
		{
			cyiBitmapBlender::FillColor(m_bmpCurrent, 0);
		}
	}
MutexUnlock();

	m_nItemzLength=0;
	m_nItemzWidth=0;
	//m_listItem->clear();
	m_itemMouseMove=0;
	m_itemMouseDown=0;
	
	if (m_children &&
			m_children->count()!=0)
			m_children->clear();

	m_nScrollPos=0;

}


// -----------------------------
void cglsStackPannel::Update(sDrawItem* item, eDrawItemStatus status)
{
	RECT rect = {(LONG)item->rect.x, (LONG)item->rect.y, (LONG)(item->rect.x+item->rect.w), (LONG)(item->rect.y+item->rect.h)};

	if (xDrawItem(item, status)==True)
	{
		if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==True)
		{
			int sw, sh;
			GetScrollPos(sw, sh);
			rect.left-=sw;
			rect.right-=sw;
			rect.top-=sh;
			rect.bottom-=sh;
		}
		InvalidateRect(&rect, FALSE);
	}
}

// ------------------------------------
Bool cglsStackPannel::InvalidateRect(RECT* lpRect, Bool bErase, Bool bIsDirect, Bool bIsForced)
{
	return cGlassInterface::InvalidateRect(lpRect, bErase, bIsDirect, bIsForced);
}

// -----------------------------
// Updateが呼ばれるタイミングは、
// 生成時、UIの追加、削除、スケールの変更がある場合

void cglsStackPannel::Update(eGlsUpdateMode mode, eDrawItemStatus _status)
{
	int width=m_wMargin;
	int height=m_hMargin;
	int xHeight=0;
	int xWidth=0;
	RECT rect;

	if (mode==eGUM_SKIP_REDRAW ||
			mode==eGUM_BG_FORCED)
	{
		goto SKIP_REDRAW;
	}

	if (mode==eGUM_UPDATE ||
		mode==eGUM_UPDATE_LASTITEM ||
		mode==eGUM_UPDATE_BGCHANGED)
	{
		goto UPDATE_ONLY;
	}

	if (m_bIsBeforeAdded==True)
	{
		m_bIsBeforeAdded=False;
		mode=eGUM_FORCED;
	}

	// スケールの変化がある場合
	// 新たなビットマップを生成する。
	if (mode==eGUM_FORCED)
	{
//#ifdef _DEBUG
//		::YITRACE(7, "cglsStackPannel::Update(eGUM_FORCED) classname:%s", m_szClassName);
//#endif
		xReDrawObject(); // itemのビットマップ生成
	}

SKIP_REDRAW:
	if (mode==eGUM_BG_FORCED)
	{
		mode=eGUM_FORCED;
	}

	// 座標計算
	xCalculatePosition(&rect);


	int w=rect.right, h=rect.bottom;
	// 固定サイズでない場合、ここでWindowのサイズを変更する。
	// バッファーも初期化
	// 固定サイズの場合
	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True)
	{
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED)==False)
		{
			if (xIS_GLS_FLAG(m_eStackFlag,eSPF_RESIZE_IS_MANUAL)==False)
			{
				if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL))
				{
					ReSize(rect.right, m_h);
					//h=m_item.rect.h?m_item.rect.h:rect.bottom;
				}
				else
				{
					ReSize(m_w, rect.bottom);
					//w=m_item.rect.w?m_item.rect.w:rect.right;
				}
			}
		}
		else
		{
			if (xIS_GLS_FLAG(m_eStackFlag,eSPF_RESIZE_IS_MANUAL)==False)
			{
				if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL))
				{
					ReSize(m_bmpCurrent->GetWidth(), m_h);
				}
				else
				{
					ReSize(m_w, m_bmpCurrent->GetHeight());
				}
			}
		}
	}
	else
	{
		if (m_w!=rect.right || m_h!=rect.bottom)
		{
			ReSize(rect.right, rect.bottom);
			m_w=rect.right;
			m_h=rect.bottom;
		}
	}

	//if (xCreateBg(w, h, mode==eGUM_FORCED)==False)
	if (xCreateBg(w, h, mode==eGUM_FORCED)==False)
	{
		if (mode==eGUM_FORCED)
		{
			//if (m_bg)
			//	cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);
		}
	}

UPDATE_ONLY:

	// すべてのアイテムの描画
	if (mode==eGUM_UPDATE_LASTITEM)
	{
		sDrawItem* item = (sDrawItem*)m_listItem->GetNodeTailFrom(0)->ref;
		xDrawItem(item, eDIS_FORCED);
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True || 
			IsWindowVisible(m_hWnd)==TRUE)
		{
			RECT rect = {item->rect.x, item->rect.y, item->rect.x+item->rect.w, item->rect.h+item->rect.y};

			// TODO.
			//int sx, sy;
			//GetScrollPos(sx, sy);

			if ((item->rect.x+item->rect.w<=m_w) &&
					(item->rect.y+item->rect.h<=m_h))
			{
				//if (m_bIsBufferSizeFixedVirtualMode==False)
				InvalidateRect(&rect, FALSE);
			}
			else
			{
				return ;
			}
		}
	}
	else
	{
		xDrawItem((mode==eGUM_UPDATE_BGCHANGED)? (sDrawItem*)-1: 0);

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True || 
			IsWindowVisible(m_hWnd)==TRUE)
		{
			InvalidateRect(0, FALSE);
		}
	}

	return ;

}

// -----------------------------
void cglsStackPannel::GetScrollPos(int&x,int&y)
{
	x=0;
	y=0;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL))
		y=m_nScrollPos;
	else
		x=m_nScrollPos;
}


// ---------------------------------------
Bool cglsStackPannel::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	Bitmap* bmp=m_bmpCurrent;

	if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==True)
	{
		// 何もしない
		return True;
	}

	yiMutexLockEx(m_hPaintMutex);

	Graphics g(hdc);

	if (bmp)
	{
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True) // 固定サイズの場合
		{
			if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)==True)
			{
				g.DrawImage(bmp, 0, 0, 0, m_nScrollPos, m_w, m_h, UnitPixel);
			}
			else
			{
				g.DrawImage(bmp, 0, 0, m_nScrollPos, 0, m_w, m_h, UnitPixel);
			}
		}
		else
		{
			g.DrawImage(bmp, 0, 0);
		}

		//if (m_item.pCurrentMaskBmp)
		//{
		//	g->DrawImage(m_item.pCurrentMaskBmp, 0, 0);
		//}
	}

	yiMutexUnlockEx(m_hPaintMutex);

	return TRUE;
}

// -----------------------------
LRESULT cglsStackPannel::OnMouseWheel(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int w = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?m_h:m_w;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==False)
	{
		return 0;
	}

	if (w>=m_nItemzLength)
		return 0;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_SHOW_SCROLLBAR)==False)
		return 0;

	__xITEM_LOCK__;


	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam); 
	int n=zDelta/120;
	int nOneElement = (float)(m_scrollInfo.max-m_scrollInfo.min)/(float)m_listItem->count();

	if (abs(zDelta/120)==1)
	{
		if (n==-1)
		{
			int mod=m_scrollInfo.pos%nOneElement;

			if (mod)
				Scroll(nOneElement+(nOneElement-mod), 300);
			else
				Scroll(nOneElement, 300);
		}
		else
		{
			int mod = 0;
			mod=m_scrollInfo.pos%nOneElement;
			Scroll(-nOneElement-mod, 300);
		}
	}
	else
	{
		if (n>0) // PageUP
		{
			int mod=m_scrollInfo.pos%nOneElement;
			Scroll(-m_scrollInfo.page-mod, 500, -nOneElement, 1000);
		}
		else 
		{
			int mod=m_scrollInfo.pos%nOneElement;

			if (mod)
				Scroll(m_scrollInfo.page+(nOneElement-mod), 500, nOneElement, 1000);
			else
				Scroll(m_scrollInfo.page, 500, nOneElement, 1000);
		}
	}

	//int nOneElement = (float)(m_scrollInfo.max-m_scrollInfo.min)/(float)m_listItem->count();

	//int pos=-nOneElement*zDelta/120;

	//Scroll(pos, 300);//, pos>0?nOneElement:-nOneElement, 1000); 


	__xITEM_UNLOCK__;
	return 0;
}

// ---------------------------------------
LRESULT cglsStackPannel::OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	
	//if (xIS_ITEM_HIDE((&m_itemScrollArea))==0)
	//{

	//if (m_bIsSizeFixed==True)
	//{
	//	if (m_detail.bIsShowScrollBar==True)
	//	{
	//		xCreateScrollBar();
	//		//SetScrollRange(0, m_nItemzLength, m_bIsVertical?m_h:m_w);
	//	}
	//}
	//}
	return 0;
}

// ------------------------------------
LRESULT cglsStackPannel::OnLButtonRepeated(WPARAM wp, LPARAM lp)
{
	int x=wp, y=lp;

	if (xIS_ITEM_HIDE((&m_itemScrollArea))==0)
	{
		int index;
		x-=m_itemScrollArea.rect.x;
		y-=m_itemScrollArea.rect.y;
		index = xGetScrollParts(x, y);

		if (index==-1)
			return 0;

		m_bPushed=True;
		xOnLButtonDownScrollBar(wp, lp);
		m_bPushed=False;
		return 0;
	}

	//else
	//{
	//	EndLButtonRepeat();
	//}

	return 0;
}

// ---------------------------------------
LRESULT cglsStackPannel::OnClicked(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// ---------------------------------------
LRESULT cglsStackPannel::OnClicked(sDrawItem* item)
{

	if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)==False)
	{
		if (xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE)
		{
			SendMessage(m_hNotifyWnd, WM_COMMAND, item->id, 0);
		}
		else
		{
			SendMessage(m_hWnd, WM_COMMAND, item->id, 0);
		}
	}
	
	return 0;
}

// ---------------------------------------
LRESULT cglsStackPannel::OnMouseLButtonDblClick(WPARAM wp, LPARAM lp, Bool& handled)
{
	m_bIsDblClicked=True;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標


	sDrawItem* item = xFindItem(xPos, yPos, &m_listFreeItem);

	if (item)
	{
		m_bIsDblClicked=False;
		return 0;
	}

	if (item==0)
	{
		item=xFindItem(xPos, yPos, m_listItem);
	}

	if (item==0)
	{
		m_bIsDblClicked=False;
		return 0;
	}

	//if (item!=m_itemMouseDown)
	//{
	//	m_bIsDblClicked=False;
	//	return 0;
	//}

	if (xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE)
	{
		SendMessage(m_hNotifyWnd, WM_COMMAND, item->id, 0);
	}
	else
	{
		//SendMessage(m_hWnd, WM_COMMAND, item->id, 0);
		SendMessage(m_hWnd, WM_COMMAND, item->id, m_bIsDblClicked);
	}
	m_bIsDblClicked=False;
	return 0;
}

// ---------------------------------------
sDrawItem* cglsStackPannel::xFindItem(int x, int y, cyiList* list)
{
	int sw=0, sh=0;

	if (list==0)
		list = m_listItem;
	GetScrollPos(sw, sh);

	for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item = (sDrawItem*)node->ref;

		if (xIS_ITEM_HIDE(item))
			continue;

		if (xIS_ITEM_ABS_COORD(item))
		{
			if (xIsInRect(&item->rect, x-sw, y-sh)==TRUE)
			{
				return item;
			}
		}
		else
		{
			if (xIsInRect(&item->rect, x, y)==TRUE)
			{
				return item;
			}
		}
	}

	return 0;
}

// ---------------------------------------
LRESULT cglsStackPannel::OnMouseLButtonDown(WPARAM wp, LPARAM lp, Bool& handled)
{
	int sx=0, sy=0;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	Bool bIsHitted=False;
	Bool bIsGlassInterface=False;
	eStackPannelType type;

	if (::GetFocus()!=m_hWnd)
		SetFocus(m_hWnd);

	YIDEBUG("cglsStackPannel::OnMouseLButtonDown() called");
	// マウスキャプチャーのとき全体の領域に入っているかどうかをチェックする。
	yiRect xrect = m_item.rect;
	xrect.x=0;
	xrect.y=0;

	if (xIsInRect(&xrect, xPos, yPos)==False)
	{
		return -1;
	}

	GetScrollPos(sx,sy);
	xPos+=sx;
	yPos+=sy;
	//xPos+=m_bIsVertical?0:m_nScrollPos;
	//yPos+=m_bIsVertical?m_nScrollPos:0;

	if (MutexLock(False)==False)
		return 0;

	// for thread safe
	//__xITEM_LOCK__;


	sDrawItem* item = xFindItem(xPos, yPos, &m_listFreeItem);

	if (item==0)
	{
		item=xFindItem(xPos, yPos, m_listItem);
	}

	if (item==0)
		goto NO_HIT_ITEM;

	if (xGET_ITEM_TYPE(item)==eSPT_SCROLLBAR &&
		xIS_ITEM_HIDE(item)==0 )
	{
		// ここでスクロールを行う
		if (xOnLButtonDownScrollBar(xPos-sx, yPos-sy)==True)
		{
			xScrollBarUpdate(xPos, yPos);
		}
		__xITEM_UNLOCK__;
		return 0;
	}

	bIsGlassInterface=(xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE);

	// アイテムが無効化されているかスタティックの場合は
	// 処理しない
	type = xGET_ITEM_TYPE(item);
	if (xIS_ITEM_DISABLE(item) ||
		type ==eSPT_STATIC ||
			type==eSPT_SPACE ||
				xIS_ITEM_HIDE(item)!=0)
		goto NO_HIT_ITEM;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==False &&
			bIsGlassInterface==True)
	{
		goto NO_HIT_ITEM;
	}

	// サイズ固定モード時のみ
	if (bIsGlassInterface)
	{
		// すでに押されている場合は処理を省く
		if (m_itemMouseDown==item)
			goto NO_HIT_ITEM;

		xPos-=item->rect.x;
		yPos-=item->rect.y;
		lp = (yPos<<16)|xPos;

		// 既存の押されていたアイテムに対して
		if (m_itemMouseDown)
		{
			if (xGET_ITEM_TYPE(m_itemMouseDown)==eSPT_GLASSINTERFACE)
			{
				cGlassInterface* obj=(cGlassInterface*)m_itemMouseDown->gls;
				obj->OnMouseLeave(wp, lp);
			}
			else
			{
				Update(m_itemMouseDown, eDIS_MOUSE_LEAVE);
			}
		}
		cGlassInterface* obj = (cGlassInterface*)item->gls;

		obj->OnMouseLButtonDown(wp, lp, handled);
		m_itemMouseDown=item;
		goto HIT_ITEM;
	}

	// eSPT_NORMAL時の処理
	if (xOnItemMouseDown(item)==True)
	{
		__xITEM_UNLOCK__;
		return 0;
	}

	// 既に押されている。
	if (m_itemMouseDown==item)
	{
		if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)==False)
		{
			goto NO_HIT_ITEM;
		}
		else
		{
			Update(m_itemMouseDown, eDIS_FREE2);
			SendMessage(m_hWnd, WM_COMMAND, item->id, (LPARAM)this);
			//m_itemMouseDown=0;
			goto NO_HIT_ITEM;
		}
	}

	if (m_eSelectedStatus==eDIS_MOUSE_DOWN)
	{
		if (m_itemSelected && m_itemSelected!=item)
		{
			sDrawItem* xItem=m_itemSelected;
			m_itemSelected=0;
			Update(xItem, eDIS_MOUSE_DOWN);
		}

		m_itemSelected=item;
	}

	if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)==True)
	{
		if (m_itemMouseDown)
		{
			Update(m_itemMouseDown, eDIS_FREE);
			m_itemMouseDown=0;
		}
	}

	m_itemMouseDown=item;
	Update(item, eDIS_MOUSE_DOWN);

HIT_ITEM:
	bIsHitted=True;

NO_HIT_ITEM:
	UpdateBg(eDIS_MOUSE_DOWN, bIsHitted);
	UpdateFg(eDIS_MOUSE_DOWN, bIsHitted);

	__xITEM_UNLOCK__;

	if (bIsHitted==True)
	{
		if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)==True)
		{
			SendMessage(m_hWnd, WM_COMMAND, item->id, (LPARAM)this);
		}
	}

	return 0;
}

// ---------------------------------------
LRESULT cglsStackPannel::OnMouseLButtonUp(WPARAM wp, LPARAM lp, Bool& handled)
{
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	Bool bIsHitted=False;
	Bool bIsGlassInterface;
	eStackPannelType type;

	// マウスキャプチャーのとき全体の領域に入っているかどうかをチェックする。
	yiRect xrect = m_item.rect;
	xrect.x=0;
	xrect.y=0;

	if (xIsInRect(&xrect, xPos, yPos)==False)
	{
		return -1;
	}

	xPos+=xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?0:m_nScrollPos;
	yPos+=xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?m_nScrollPos:0;



	// for thread safe
	__xITEM_LOCK__;



	sDrawItem* item = xFindItem(xPos, yPos, &m_listFreeItem);

	if (item==0)
	{
		item=xFindItem(xPos, yPos, m_listItem);
	}

	if (item==0)
		goto NO_HIT_ITEM;

	// 
	if (xGET_ITEM_TYPE(item)==eSPT_SCROLLBAR &&
		xIS_ITEM_HIDE(item)==0)
	{
		xScrollBarUpdate(xPos, yPos, False);
		__xITEM_UNLOCK__;
		return 0;
	}


	bIsGlassInterface=(xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE);

	// アイテムが無効化されているかスタティックの場合は
	// 処理しない
	type = xGET_ITEM_TYPE(item);
	if (xIS_ITEM_DISABLE(item) ||
		type ==eSPT_STATIC  ||
			type==eSPT_SPACE ||
				xIS_ITEM_HIDE(item)!=0)
		goto NO_HIT_ITEM;

	// サイズ固定じゃない場合、GlassInterfaceは自分でメッセージを処理する。
	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==False &&
			bIsGlassInterface==True)
	{
		goto NO_HIT_ITEM;
	}

	// 固定サイズの場合
	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True &&
			bIsGlassInterface==True)
	{
		if (m_itemMouseDown!=item)
		{
			goto NO_HIT_ITEM;
		}

		xPos-=item->rect.x;
		yPos-=item->rect.y;
		lp = (yPos<<16)|xPos;

		if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)==False)
		{
			cGlassInterface* obj=(cGlassInterface*)m_itemMouseDown->gls;
			obj->OnMouseLButtonUp(wp, lp, handled);

			m_itemMouseDown=0;
			goto HIT_ITEM;
		}
	}

	if (bIsGlassInterface)
		goto NO_HIT_ITEM;

	if (xOnItemMouseUp(item)==True)
	{
		__xITEM_UNLOCK__;
		return 0;
	}

	if (m_itemMouseDown==0)
	{
		goto NO_HIT_ITEM;
	}

	Update(m_itemMouseDown, eDIS_MOUSE_UP);

	if (m_itemMouseDown!=item)
	{
		if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)==False)
		{		
			m_itemMouseDown=0;
		}
		goto NO_HIT_ITEM;
	}
	if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)==False)
	{
		m_itemMouseDown=0;
	}

	//m_itemMouseDown=0;
	OnClicked(item);

HIT_ITEM:
	bIsHitted=True;

NO_HIT_ITEM:
	UpdateBg(eDIS_MOUSE_UP, bIsHitted);
	UpdateFg(eDIS_MOUSE_UP, bIsHitted);

	__xITEM_UNLOCK__;
	return 0;
}



// ---------------------------------------
LRESULT cglsStackPannel::OnMouseMove(WPARAM wp, LPARAM lp, Bool& handled)
{
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	Bool bIsHitted=False;
	Bool bIsNoItem=False;
	Bool bIsGlassInterface;
	eStackPannelType type;
	int ret=-1;
//#ifdef _DEBUG
//	YIDEBUG("cglsStackPannel::OnMouseMove(%s)", m_szClassName);
//#endif
	// マウスキャプチャーのとき全体の領域に入っているかどうかをチェックする。

	yiRect xrect = m_item.rect;
	xrect.x=0;
	xrect.y=0;

	if (xIsInRect(&xrect, xPos, yPos)==False)
	{
		return -1;
	}

	xPos+=xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?0:m_nScrollPos;
	yPos+=xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?m_nScrollPos:0;

	// for thread safe

	if (MutexLock(False)==False)
		return ret;

	//if (GetFocus()!=m_hWnd)
	//{
	//	SetFocus(m_hWnd);
	//}

	//__xITEM_LOCK__;

	sDrawItem* item = xFindItem(xPos, yPos, &m_listFreeItem);

	if (item && xGET_ITEM_TYPE(item)==eSPT_SCROLLBAR)
	{
		//m_eScrollBarState=eSPSB_MOUSE_ENTER;
		LRESULT res = xOnMouseMoveScrollBar(wp, lp);

		MutexUnlock();
		return res;
	}
	else
	{
		if (m_eScrollBarState!=eSPSBS_DRAG)
		{
			xOnMouseLeaveScrollBar(wp, lp);
		}
	}

	if (item==0)
	{
		item=xFindItem(xPos, yPos, m_listItem);
	}

	if (item==0)
	{
		bIsNoItem=True;
		goto NO_HIT_ITEM;
	}

	// 
	// 
	//if (xGET_ITEM_TYPE(item)==eSPT_SCROLLBAR &&
	//	xIS_ITEM_HIDE(item)==0)
	//{
	//	xOnMouseMoveScrollBar(xPos, yPos);
	//	//__xITEM_UNLOCK__;
	//	//return 0;
	//}

	// アイテムが無効化されているかスタティックの場合は
	// 処理しない

	type = xGET_ITEM_TYPE(item);
	if (xIS_ITEM_DISABLE(item) ||
			type==eSPT_STATIC ||
				type==eSPT_SPACE ||
					xIS_ITEM_HIDE(item)!=0)
	{
		// ただし、既存のものは戻す。
		if (m_itemMouseMove)
		{
			Update(m_itemMouseMove, eDIS_MOUSE_LEAVE);
			m_itemMouseMove=0;
		}

		goto NO_HIT_ITEM;
	}


	bIsGlassInterface=(type==eSPT_GLASSINTERFACE);

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True && bIsGlassInterface)
	{
		cGlassInterface* obj = (cGlassInterface*)item->gls;

		xPos-=item->rect.x;
		yPos-=item->rect.y;
		lp = (yPos<<16)|xPos;

		if (m_itemMouseMove==item)
		{
			obj->OnMouseMove(wp, lp, handled);
			goto NO_HIT_ITEM;
		}

		if (m_itemMouseMove)
		{
			if (xGET_ITEM_TYPE(m_itemMouseMove)==eSPT_GLASSINTERFACE)
			{
				cGlassInterface* obj=(cGlassInterface*)m_itemMouseMove->gls;
				obj->OnMouseLeave(wp, lp);
			}
			else
			{
				Update(m_itemMouseMove, eDIS_MOUSE_LEAVE);
			}
		}

		if (obj->OnMouseMove(wp, lp, handled)==-1)
		{
			obj->OnMouseEnter(wp, lp);
		}
		m_itemMouseMove=item;
		goto HIT_ITEM;
	}
	
	if (bIsGlassInterface)
	{
		goto NO_HIT_ITEM;
	}

	if (xOnItemMouseMove(item)==True)
	{
		__xITEM_UNLOCK__;
		return 0;
	}

	if (item == m_itemMouseMove)
	{
		if (xGET_ITEM_TYPE(item)==eSPT_SCROLLBAR)
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(m_hWnd, &pt);
			pt.x-=item->rect.x;
			pt.y-=item->rect.y;
			
			int n = xGetScrollParts(pt.x, pt.y);
			if (m_nLastSelectedParts == n)
			{
				//xTRACE("MouseMove..... Same item selected(%d)", n);

				goto NO_HIT_ITEM;
			}
			//xTRACE("MouseMove..... item selected(%d)", n);
			m_nLastSelectedParts=n;

			xSET_ITEM_STATUS(item, eDIS_NORMAL);
		}
		else
		{
			goto NO_HIT_ITEM;
		}

		
	}



	// 削除していいよね？通らないよね？
	//if (xGET_ITEM_TYPE(item)==eSPT_SCROLLBAR)
	//{
	//	m_itemScrollArea.animation->fAnimationRate=0.0;
	//}

	if (m_itemMouseMove)
	{
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True &&
			xGET_ITEM_TYPE(m_itemMouseMove)==eSPT_GLASSINTERFACE)
		{
			cGlassInterface* obj=(cGlassInterface*)m_itemMouseMove->gls;
			obj->OnMouseLeave(wp, lp);
		}
		else
		{
			if (m_itemMouseMove!=item ||
				xGET_ITEM_TYPE(m_itemMouseMove)!=eSPT_SCROLLBAR)
			{
				m_nLastSelectedParts=-1;
				Update(m_itemMouseMove, eDIS_MOUSE_LEAVE);
			}
		}

	}

	m_itemMouseMove=item;
	Update(item, eDIS_MOUSE_MOVE);

HIT_ITEM:
	bIsHitted=True;
	ret=item->id;


NO_HIT_ITEM:
	if (bIsNoItem==True)
	{
		if (m_itemMouseMove)
		{
			if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True &&
				xGET_ITEM_TYPE(m_itemMouseMove)==eSPT_GLASSINTERFACE)
			{
				cGlassInterface* obj=(cGlassInterface*)m_itemMouseMove->gls;
				obj->OnMouseLeave(wp, lp);
			}
			else
			{
				Update(m_itemMouseMove, eDIS_MOUSE_LEAVE);
			}
		}

		if (m_itemMouseDown)
		{
			if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True &&
				xGET_ITEM_TYPE(m_itemMouseDown)==eSPT_GLASSINTERFACE)
			{
				cGlassInterface* obj=(cGlassInterface*)m_itemMouseDown->gls;
				obj->OnMouseLeave(wp, lp);
			}
			else
			{
				Update(m_itemMouseDown, eDIS_MOUSE_LEAVE);
			}
		}
		m_itemMouseMove=0;

		if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)==False)
		{
			m_itemMouseDown=0;
		}
	}

	UpdateBg(eDIS_MOUSE_MOVE, bIsHitted);
	UpdateFg(eDIS_MOUSE_MOVE, bIsHitted);

	__xITEM_UNLOCK__;
	return ret;

}

// -------------------------
LRESULT cglsStackPannel::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	int res=-1;
	// for thread safe
	//xTRACE("cglsStackPannel::OnMouseLeave %x", GetThreadId(GetCurrentThread()));
	//__xITEM_LOCK__;
	
	if (MutexLock(False)==False)
		return res;

	if (m_itemMouseMove)
	{
		//SetItemStatus(m_itemMouseMove, eDIS_NORMAL);

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True)
		{
			if (xGET_ITEM_TYPE(m_itemMouseMove)==eSPT_GLASSINTERFACE)
			{
				cGlassInterface* obj = (cGlassInterface* )m_itemMouseMove->gls;
				obj->OnMouseLeave(wp, lp);
			}
			else
			{
				Update(m_itemMouseMove, eDIS_MOUSE_LEAVE);
			}
		}
		else
			Update(m_itemMouseMove, eDIS_MOUSE_LEAVE);

		res = m_itemMouseMove->id;
	}

	if (m_itemMouseDown)
	{
		//SetItemStatus(m_itemMouseMove, eDIS_NORMAL);
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True)
		{
			if (xGET_ITEM_TYPE(m_itemMouseDown)==eSPT_GLASSINTERFACE)
			{
				cGlassInterface* obj = (cGlassInterface* )m_itemMouseDown->gls;
				obj->OnMouseLeave(wp, lp);
			}
		}
		else
			Update(m_itemMouseDown, eDIS_MOUSE_LEAVE);
	}

	m_itemMouseMove=0;

	if (xIS_GLS_FLAG(m_flag, eGF_MOUSEDOWN_COMMAND)==False)
	{
		m_itemMouseDown=0;
	}

	xOnMouseLeaveScrollBar(wp, lp);

	//m_eScrollBarState=eSPSB_MOUSE_LEAVE;

	UpdateBg(eDIS_MOUSE_LEAVE, False);
	UpdateFg(eDIS_MOUSE_LEAVE, False);


	__xITEM_UNLOCK__;

	return res;

}

// ---------------------------------------
LRESULT cglsStackPannel::OnMouseEnter(WPARAM wp, LPARAM lp)
{
	Bool handled=True;
	return OnMouseMove(wp, lp, handled);
	//__xITEM_LOCK__;
	//UpdateBg(eDIS_MOUSE_ENTER, False);
	//UpdateFg(eDIS_MOUSE_ENTER, False);
	//__xITEM_UNLOCK__;

	return 0;
}

// -----------------------------
void cglsStackPannel::EnableItem(int id, Bool bIsEnable)
{
	Bool bIsQuit=False;
	cyiList* list=m_listItem;

TOLOOP:
	for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item = (sDrawItem*)node->ref;

		int xId=item->id;

		if (item->gls)
		{
			xId=item->gls->m_id;
		}

		if (xId==id)
		{
			if (bIsEnable)
				xITEM_ENABLE(item);
			else
				xITEM_DISABLE(item);

			if (xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE)
			{
				cGlassInterface* obj = (cGlassInterface*)item->gls;
				obj->Enable(bIsEnable);
				obj->InvalidateRect(0);
				break;
			}

			if (bIsEnable==False)
			{
				if (m_itemMouseMove==item)
				{
					m_itemMouseMove=0;
				}

				if (m_itemMouseDown==item)
				{
					m_itemMouseDown=0;
				}
			}
			Update(item);
			break;
		}
	}

	if (bIsQuit==False)
	{
		list=&m_listFreeItem;
		bIsQuit=True;
		goto TOLOOP;
	}
}

// ------------------------------------
Bool cglsStackPannel::IsItemVisible(int id)
{
	sDrawItem* item = (sDrawItem*)xGetItemById(id);

	if (item)
	{
		return xIS_ITEM_HIDE(item)==0?False:True;
	}
	return False;
}

// ------------------------------------
Bool cglsStackPannel::ShowItem(Bool bIsShow, int id)
{
	Bool bIsQuit=False;
	cyiList* list=m_listItem;

TOLOOP:
	for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item = (sDrawItem*)node->ref;
		int xId=item->id;

		if (item->gls)
		{
			xId=item->gls->m_id;
		}

		if (xId==id)
		{
			Bool bIsBeforeState = xIS_ITEM_HIDE(item)==0;

			//if (bIsBeforeState==bIsShow)
			//	return False;

			if (bIsShow)
				xSET_ITEM_SHOW(item);
			else
				xSET_ITEM_HIDE(item);


			if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==False)
			{
				if (xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE)
				{
					cGlassInterface* obj = (cGlassInterface*)item->gls;

					if (bIsShow)
						obj->Show();
					else
						obj->Hide();
				}
			}

			//xCalculatePosition(0, 0);
			return True;
		}
	}

	if (bIsQuit==False)
	{
		list=&m_listFreeItem;
		bIsQuit=True;
		goto TOLOOP;
	}

	return False;
}


// ------------------------------------
LRESULT cglsStackPannel::OnNotHandledCommand(WPARAM wp, LPARAM lp)
{
	SendMessage(m_hNotifyWnd/*m_hParentWnd*/, WM_COMMAND, wp, lp);
	return 0;
}

// ---------------------------------------
void* cglsStackPannel::xGetItemById(int id)
{
	for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item = (sDrawItem*)node->ref;
		
		if (item->gls==0)
		{
			if (id==item->id)
			{
				return item;
				//return xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE ? item->param:(void*)item;
			}
		}
		else
		{
			if (id==item->gls->m_id)
			{
				return item;
			}
		}
	}
	return 0;
}

// ---------------------------------------
void* cglsStackPannel::xGetItemByIndex(int index)
{
	cyiNode* node=m_listItem->GetNodeTopFrom(index);

	if (node)
	{
		return node->ref;
	}

	return 0;
}

// ---------------------------------------
int cglsStackPannel::xGetItemIndex(int id)
{
	int cnt=0;
	for ( cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np, cnt++)
	{
		sDrawItem* item = (sDrawItem*)node->ref;

		if (id==item->id)
		{
			return cnt;
		}
	}
	return -1;
}


// -----------------------------
void cglsStackPannel::xReDrawObject()
{
	int cnt=0;
	int length=0;

	for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item = (sDrawItem*)node->ref;
		
		eStackPannelType type = xGET_ITEM_TYPE(item);

		if (type==eSPT_LF || type==eSPT_SPACE)
			continue;

		if (xIS_ITEM_HIDE(item)!=0)
		{
			continue;
		}

		if (xGET_ITEM_TYPE(item)!=eSPT_GLASSINTERFACE)
		{
			m_DrawObject->CreateBitmap(item);
			continue;
		}

		cGlassInterface* obj = (cGlassInterface*)item->gls;
		obj->Update(eGUM_FORCED);
	}
}


// ---------------------------------------
int cglsStackPannel::GetBgIndex(eDrawItemStatus status, Bool bIsHitted, Bool& bIsSkipAnimation)
{
	int ret=0;

	bIsSkipAnimation=False;

	if (bIsHitted==True)
	{
		return -1;
	}

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BG_SELECTED_MODE)==True &&
			xIS_GLS_FLAG(m_eStackFlag, ::eSPF_BG_SELECT_LOCK)==True)
	{
		return -1;
	}

	if (status==eDIS_MOUSE_MOVE ||
			status==eDIS_MOUSE_ENTER)
	{
		ret=1;

		//if (m_item.fEnterTransparency!=-1.0)
		//{
		//	return -2;
		//}
		//else
		{
			if (SetItemStatus((sDrawItem*)&m_item, eDIS_MOUSE_MOVE)==False)
			{
				return -1;
			}
		}
	}
	else if (status==eDIS_MOUSE_DOWN)
	{
		ret=2;
		//bg=m_item.pBmp[2];
		//bgMask=m_item.pBmpMasked[2];
		if (SetItemStatus((sDrawItem*)&m_item, eDIS_MOUSE_DOWN)==False)
		{
			return -1;
		}
		xON_GLS_FLAG(m_eStackFlag, eSPF_BG_SELECT_LOCK);
		//m_bIsBgSelectedLock=True;
		bIsSkipAnimation=True;
		return ret;
	}
	else
	{
		//if (m_fLeaveTransparency!=-1.0)
		//{
		//	return -3;
		//}
		//else
		{
			if (SetItemStatus((sDrawItem*)&m_item, eDIS_NORMAL)==False)
			{
				return -1;
			}
		}
	}

	return ret;
}



// ---------------------------------------
Bool cglsStackPannel::UpdateBg(eDrawItemStatus status, Bool bIsHitted)
{
	int nBmpIndex=0;
	Bool bIsSkipAnimation;
	
	// 背景描画なし
	if (m_item.pBmp[0]==0 ||
			m_item.pCurrentBmp==0)
	{
		return False;
	}

	//if (m_item.pBmp[0]==m_item.pBmp[1])
	//{
	//	return False;
	//}

	//xTRACE("UpdateBg status:%d", status);
	if ((nBmpIndex=GetBgIndex(status, bIsHitted, bIsSkipAnimation))==-1)
	{
		//xTRACE("UpdateBg status:%d exit", status);
		return False;
	}

	//if (m_item.pBmp[nBmpIndex]==m_item.pBmp[m_item.nLastItemIndex])
	//	return False;
	//else if (nBmpIndex==-2 ||
	//			nBmpIndex==-3) // Transparent Animation.
	//{
	//	m_item.pStartBmp=0;
	//	m_item.pEndBmp=0;
	//	m_item.to=nBmpIndex==-2?m_fEnterTransparency:m_fLeaveTransparency;
	//	m_item.from=m_item.fTransparency;

	//	if (m_item.to==m_item.from)
	//	{
	//		return False;
	//	}
	//	m_item.pCurrentBmp=m_item.pBmp[0];

	//	goto START_ANIMATION;
	//}



	if (xIS_GLS_FLAG(m_flag, eGF_NO_ANIMATION)==True ||
			bIsSkipAnimation==True)
	{
		StopAnimation(&m_item, True);
		m_item.nLastItemIndex=nBmpIndex;
		goto SKIP_ANIMATION;
	}

	int sx, sy;

	GetScrollPos(sx, sy);

	//if (m_item.pStartBmp)
	//	delete m_item.pStartBmp;

	//if (m_item.pAnimationBmp)
	//{
	//	m_item.pStartBmp=cyiBitmapBlender::BitmapCopy(0, m_item.pAnimationBmp, 0, 0, sx, sy, m_w, m_h);
	//	delete m_item.pAnimationBmp;
	//	m_item.pAnimationBmp=0;
	//}
	//else
	//{
	//	m_item.pStartBmp=cyiBitmapBlender::BitmapCopy(0, m_item.pCurrentBmp, 0, 0, sx, sy, m_w, m_h);
	//}

	//if (m_item.pEndBmp)
	//	delete m_item.pEndBmp;

	//m_item.pEndBmp=cyiBitmapBlender::BitmapCopy(0, m_item.pBmp[nBmpIndex], 0, 0, sx, sy, m_w, m_h);

	//m_item.pCurrentBmp=m_item.pBmp[nBmpIndex];

START_ANIMATION:
	//yiAnitStart(m_item.hAnit, m_hWnd, WM_GLS_BG_ANIMT, 250, 15, eGAP_FISO, &m_item);

	if (m_item.pBmp[0]==m_item.pBmp[1])
		return False;

	sGlsAnimationEx ani;
	::glsInitAnimationEx(&ani, &m_item);
	::glsSetDvAnimationEx(&ani, m_item.nLastItemIndex, nBmpIndex, eGAP_FISO);
	m_item.nLastItemIndex=nBmpIndex;

	StartAnimation(&m_item, &ani, 15, 250);
	
	return True;

SKIP_ANIMATION:
	{
		//m_item.pAnimationBmp=0;
		m_item.pCurrentBmp=m_item.pBmp[nBmpIndex];
		InvalidateRect(0, False);
	}
	return True;
}

// ---------------------------------------
int cglsStackPannel::GetFgIndex(eDrawItemStatus status, Bool bIsHitted, Bool& bIsSkipAnimation)
{
	int ret=0;

	bIsSkipAnimation=False;

	//if (bIsHitted==True)
	//{
	//	return -1;
	//}

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BG_SELECTED_MODE)==True &&
		xIS_GLS_FLAG(m_eStackFlag, eSPF_BG_SELECT_LOCK)==True)
	{
		return -1;
	}

	if (status==eDIS_MOUSE_MOVE ||
			status==eDIS_MOUSE_ENTER)
	{
		ret=1;

		if (SetItemStatus((sDrawItem*)&m_fgItem, eDIS_MOUSE_MOVE)==False)
		{
			return -1;
		}
	}
	else if (status==eDIS_MOUSE_DOWN)
	{
		return -1;
		ret=2;
		if (SetItemStatus((sDrawItem*)&m_fgItem, eDIS_MOUSE_DOWN)==False)
		{
			return -1;
		}
		xON_GLS_FLAG(m_eStackFlag, eSPF_BG_SELECT_LOCK);
		bIsSkipAnimation=True;
		return ret;
	}
	else
	{
		if (SetItemStatus((sDrawItem*)&m_fgItem, eDIS_NORMAL)==False)
		{
			return -1;
		}
	}

	return ret;
}

// ---------------------------------------
Bool cglsStackPannel::UpdateFg(eDrawItemStatus status/*=eDIS_NORMAL*/, Bool bIsHitted/*=True*/)
{
	//return False;
	int nIndex;
	Bool bIsSkipAnimation=False;
	Bitmap* bmpTo;
	Bitmap* bmpFrom;

	if (m_fgItem.pBmp[0]==0 &&
			m_fgItem.pCurrentBmp==0)
		return False;

	if ((nIndex=GetFgIndex(status, bIsHitted, bIsSkipAnimation))==-1)
	{
		return False;
	}

	//if (m_fgItem.pBmp[nIndex]==0 &&
	//		m_fgItem.pCurrentBmp==0)
	//{
	//	if (m_parent)
	//	{
	//		RECT rect = {0, 0, m_w, m_h};
	//		bmpTo=m_parent->GetChildFgImage(m_item.id, nIndex, &rect);
	//	}

	//	if (bmpTo==0)
	//	{
	//		return False;
	//	}
	//}
	//else
	//{
	//	bmpTo=m_fgItem.pBmp[nIndex];
	//	bmpFrom=m_fgItem.pAnimationBmp?m_fgItem.pAnimationBmp:m_fgItem.pCurrentBmp;
	//}

	if (bIsSkipAnimation==True)
	{
		m_fgItem.pCurrentBmp=bmpTo;
		//if (m_fgItem.pAnimationBmp)
		//{
			//delete m_fgItem.pAnimationBmp;
			//m_fgItem.pAnimationBmp=0;
		//}
		StopAnimation(&m_fgItem, True);
		m_fgItem.nLastItemIndex=nIndex;
		goto SKIP_ANIMATION;
	}

	sGlsAnimationEx ani;
	glsInitAnimationEx(&ani, &m_fgItem);
	::glsSetDvAnimationEx(&ani, m_fgItem.nLastItemIndex, nIndex, nIndex==1?::eGAP_FISO:eGAP_SIFO);
	m_fgItem.nLastItemIndex=nIndex;

	StartAnimation(&m_fgItem, &ani, this->m_nAnimationMilliSec, nIndex==1?m_nMouseMoveAnimationInterval:m_nMouseLeaveAnimationInterval);
	
	return True;

SKIP_ANIMATION:

	InvalidateRect(0, False);

	return True;
}

// -----------------------------
// 見える画面だけを描画する
void cglsStackPannel::UpdateFixedMode(Bitmap* bmp, RECT* rect, RECT* rectChild,int offsetX, int offsetY)
{
	RECT _rect = {0, 0, m_w, m_h};
	RECT _rectChild;
	RECT localRect;
	int sw=(xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?0:m_nScrollPos);
	int sh=(xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?m_nScrollPos:0);
	Bool bIsBmpIsLocalBuffer=False;



	if (m_item.fTransparency==0.0)
		return ;

	if (m_bmpCurrent==0)
	{
		return ;
	}

	if (MutexLock(False)==False)
		return ;

	if (rect==0)
	{
		rect=&_rect;
	}

	if (rectChild==0)
	{
		rectChild=&_rectChild;
		_rectChild=_rect;
	}

	if (bmp==0)
	{
		bmp=m_bmpCurrent;
		bIsBmpIsLocalBuffer=True;
	}

	if (bmp!=m_bmpCurrent)
	{
		localRect=*rectChild;
		localRect.left+=sw;
		localRect.top+=sh;

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False)
		{
			//if (localRect.left+localRect.right > m_bmpCurrent->GetWidth())
			//{
			//	localRect.right = m_bmpCurrent->GetWidth()-localRect.left;
			//}

			//if (localRect.top+localRect.bottom > m_bmpCurrent->GetHeight())
			//{
			//	localRect.bottom = m_bmpCurrent->GetHeight()-localRect.top;
			//}

			if (localRect.right==0 ||
					localRect.bottom==0)
			{
				MutexUnlock();
				return ;
			}
		}
		else
		{
			if (localRect.left+localRect.right-sw > m_bmpCurrent->GetWidth())
			{
				localRect.right = m_bmpCurrent->GetWidth()-localRect.left;
			}

			if (localRect.top+localRect.bottom-sh > m_bmpCurrent->GetHeight())
			{
				localRect.bottom = m_bmpCurrent->GetHeight()-localRect.top;
			}
		}
	}
	else
	{
		bIsBmpIsLocalBuffer=True;
		localRect=*rect;
	}

	if (m_item.pCurrentBmp!=0)
	{
		//Graphics g(bmp);
		if (bmp!=m_bmpCurrent)
		{
			Bitmap* pBmpCurrent = GetCurrentBitmap(&m_item);
			Graphics gg(m_bmpCurrent);
			//gg.Clear(0);
			//gg.DrawImage(pBmpCurrent, rectChild->left, rectChild->top, localRect.left, localRect.top, localRect.right, localRect.bottom, Unit::UnitPixel);
			cyiBitmapBlender::FillColor(m_bmpCurrent, 0);
			cyiBitmapBlender::BitmapCopy(m_bmpCurrent, pBmpCurrent, rectChild->left, rectChild->top,
															localRect.left, localRect.top, localRect.right, localRect.bottom);
			//YIDEBUG("localRect(%d, %d, %d, %d), to (%d, %d)", localRect.left, localRect.top, localRect.right, localRect.bottom, rectChild->left, rectChild->top);

		}
		else
		{
			if (m_item.pCurrentBmp)
				cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_item.pCurrentBmp);//rectChild->left, rectChild->top,
																//rectChild->left, rectChild->top, rectChild->right, rectChild->bottom);
			else
				cyiBitmapBlender::FillColor(m_bmpCurrent, 0);
		}
		//cyiBitmapBlender::FillColor(m_bmpCurrent, 0);
	}
	else
	{

		if (OnDrawMyself(bmp, rect, rectChild)==False)
		{
			//goto SKIP_DRAW;
			Bitmap* bg=GetBgBitmap();

			if (bg!=0)
			{
				Graphics g(bmp);//bmp);
				Status status = g.DrawImage(bg, rect->left, rect->top, rectChild->left, rectChild->top, rectChild->right, rectChild->bottom, Unit::UnitPixel);
				cyiBitmapBlender::FillColor(m_bmpCurrent, 0);
			}
			else
			{
				cyiBitmapBlender::FillColor(m_bmpCurrent, 0x00FFFFFF);
				OnSelfBgDraw(m_bmpCurrent/*bmp*/, rect, rectChild);
			}
		}


		if (bIsBmpIsLocalBuffer==True)
			bmp=m_bmpCurrent;
	}


	//if (MutexLock(False)==False)
	//{
	//	xTRACE("Mutex is locked .... no draw");
	//	//__xITEM_LOCK__;
	//	MutexUnlock();

	//	return ;
	//}



	Graphics g(m_bmpCurrent);
	cyiList* pList=m_listItem;

	{
LOOP:
		for (cyiNode* node = pList->GetNodeTopFrom(); node; node=node->np)
		{
			sDrawItem* s=(sDrawItem*)node->ref;

			int x=s->rect.x;
			int y=s->rect.y;
			int w=s->rect.w;
			int h=s->rect.h;

			if (xIS_ITEM_ABS_COORD(s))
			{
				x+=sw;
				y+=sh;
			}

			RECT crect = {x, y, w, h};
			RECT retRect;
			if (xIsInRect(&crect, &localRect, retRect)==False)
			{
				continue;
			}

			//YIDEBUG("crect(%d, %d, %d, %d), localRect(%d, %d, %d, %d), retRect(%d, %d, %d, %d)",
			//		crect.left, crect.top, crect.right, crect.bottom,
			//			localRect.left, localRect.top, localRect.right, localRect.bottom,
			//					retRect.left, retRect.top, retRect.right, retRect.bottom);

			if (xIS_ITEM_ANIMATION_NOTIFY_ONLY(s))
			{
				RECT clipRect=retRect;

				if (xIS_ITEM_ABS_COORD(s))
				{
					x-=sw;
					y-=sh;
				}

				OnItemOwnerDraw(&g, s, 0);//&clipRect);
				continue;
			}

			if (xIS_ITEM_HIDE(s)!=0)
				continue;

			if (s->fTransparency==0.0)
				continue;

			if (xGET_ITEM_TYPE(s)!=eSPT_GLASSINTERFACE)
			{
				Bitmap* pItemBmp=s->pCurrentBmp;

				if (pItemBmp==0)
				{
					if (m_DrawObject->OnNullBitmapDraw(s, 0)==False)
						continue;

					pItemBmp=s->pCurrentBmp;
				}

				if (s->animation &&
						s->animation->pBmpAnimation)
				{
					pItemBmp=s->animation->pBmpAnimation;
				}

				if (s->fTransparency!=1.0)
				{
					pItemBmp = cyiBitmapBlender::Transparent(pItemBmp, s->fTransparency);
				}

				if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==True)
				{
					g.DrawImage(pItemBmp, 
						retRect.left-sw, retRect.top-sh, retRect.left-s->rect.x, retRect.top-s->rect.y, retRect.right, retRect.bottom, Unit::UnitPixel);
				}
				else
				{
					g.DrawImage(pItemBmp, 
						retRect.left, retRect.top, retRect.left-s->rect.x, retRect.top-s->rect.y, retRect.right, retRect.bottom, Unit::UnitPixel);
				}

				if (s->fTransparency!=1.0)
				{
					delete pItemBmp;
				}

				continue;
			}
			else
			{
				RECT xrect = retRect;
				retRect.left-=x;//s->rect.x;
				retRect.top-=y;//s->rect.y;
				xrect.left-=sw;
				xrect.top-=sh;

				if (xIS_GLS_FLAG(m_eStackFlag, eSPF_NORMAL_SCROLLMODE)==True)
				{
					xrect.left-=sw;
					xrect.top-=sh;
				}
				cGlassInterface* obj = (cGlassInterface*)s->gls;
				obj->Update(m_bmpCurrent, &xrect, &retRect, m_nAbsX/*-sw*/, m_nAbsY/*-sh*/);
				
				if (xIS_GLS_FLAG(obj->m_flag, eGF_REQUEST_UPDATE)==True)
				{
					//m_glsDrawInterface->m_nRequestUpdateCount++;
					xOFF_GLS_FLAG(obj->m_flag, eGF_REQUEST_UPDATE);
				}
			}
		}

		if (pList!=&m_listFreeItem)
		{
			pList=&m_listFreeItem;
			goto LOOP;
		}
	} // End of LOOP


	OnSelfFgDraw(m_bmpCurrent/*bmp*/, rect, rectChild);
	// スクロールバーの描画



	// 子供の描画が終わったらForegroundを描画

	if (m_fgItem.pCurrentBmp)
	{
		g.DrawImage(m_fgItem.pCurrentBmp, localRect.left, localRect.top, 
			localRect.left-sw, localRect.top-sh, localRect.right, localRect.bottom, UnitPixel);
	}

	if (bmp!=m_bmpCurrent)
	{
		int nSystemOffsetX = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?0:m_rectSystem.right;
		int nSystemOffsetY = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?m_rectSystem.bottom:0;


		// システム領域がある場合の動作
		if (offsetX==0 &&
				offsetY==0)
		{
			rect->left+=nSystemOffsetX;
			rect->top+=nSystemOffsetY;

			int w=localRect.right;
			int h=localRect.bottom;
			if (w+nSystemOffsetX>m_w)
			{
				w=m_w-nSystemOffsetX;
			}
			if (h+nSystemOffsetY>m_h)
			{
				h=m_h-nSystemOffsetY;
			}
			localRect.right=w;
			localRect.bottom=h;
		}

		Graphics gx(bmp);

		if (m_item.fTransparency==1.0 ||
				m_item.fTransparency==-1.0)
		{
			if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==True)
			{
				gx.DrawImage(m_bmpCurrent, rect->left, rect->top, 
					localRect.left-sw, localRect.top-sh, localRect.right, localRect.bottom, UnitPixel);
			}
			else
			{
				//YIDEBUG("UpdateFixedMode(%s) draw(%d, %d, %d, %d) m_bmpCurrent:(%d, %d)", m_szClassName, localRect.left, localRect.top, localRect.right, localRect.bottom, m_bmpCurrent->GetWidth(), m_bmpCurrent->GetHeight());
				gx.DrawImage(m_bmpCurrent, rect->left, rect->top,
					rectChild->left, rectChild->top, rectChild->right, rectChild->bottom, UnitPixel);
					//rectChild->l, localRect.top, localRect.right, localRect.bottom, UnitPixel);
			}
		}
		else
		{
			Bitmap* xx;
			Bitmap* temp;
			if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==True)
			{
				temp = cyiBitmapBlender::BitmapCopy(0, m_bmpCurrent, 0, 0, localRect.left-sw, localRect.top-sh, localRect.right, localRect.bottom);
			}
			else
			{
				temp = cyiBitmapBlender::BitmapCopy(0, m_bmpCurrent, 0, 0, localRect.left, localRect.top, localRect.right, localRect.bottom);
			}

			xx = cyiBitmapBlender::Transparent(temp, m_item.fTransparency);
			delete temp;

			gx.DrawImage(xx, rect->left, rect->top, 0, 0, localRect.right, localRect.bottom, UnitPixel);
			delete xx;
		}


		// スクロールバーは、m_bmpCurrentには描画しない。（汚さない)
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False &&
			xIS_ITEM_HIDE((&m_itemScrollArea))==False)
		{
			Rect cbr;
			gx.GetClipBounds(&cbr);
			gx.SetClip(Rect(rect->left, rect->top, rect->right, rect->bottom));

			RECT xrect = {rect->left-rectChild->left+m_itemScrollArea.rect.x/*-offsetX*/, rect->top-rectChild->top+m_itemScrollArea.rect.y/*-offsetY*/, 0, 0};//{m_itemScrollArea.rect.x+m_x, m_itemScrollArea.rect.y+m_y, 0, 0};

			xDrawScrollBar(&gx, &xrect, True);//&localRect);
			gx.SetClip(cbr);
		}
	}
__xITEM_UNLOCK__;

	return ;
}

// ---------------------------------------
Bool cglsStackPannel::OnItemOwnerDraw(Graphics* g, sDrawItem* item, RECT* clipRect)
{
	Rect rect;
	//Rect xclipRect(clipRect->left, clipRect->top, clipRect->right, clipRect->bottom);

	//if (clipRect)
	//{
	//	g->GetClipBounds(&rect);
	//	g->SetClip(xclipRect);
	//}

	if (xGET_ITEM_TYPE(item)==eSPT_SCROLLBAR)
	{
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==True)
				xDrawScrollBar(g, clipRect);
	}
	
	//if (clipRect)
	//{
	//	g->SetClip(rect);
	//}

	return True;
}


// -----------------------------
// _rectは絶対座標の描画領域
// rectChildは相対座標の描画領域
void cglsStackPannel::Update(Bitmap* bmp, RECT* _rect, RECT* rectChild/*=0*/, int offsetX, int offsetY,  Bool bIsLocalBufferUpdate)//, cGlassInterface* sender)
{
	RECT rect = {0, 0, m_w, m_h};
	RECT _rectChild;

	//YIDEBUG("cglsStackPannel::Update(%s)", m_szClassName);
	if (bmp!=0 &&
		xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True)
	{
		UpdateFixedMode(bmp, _rect, rectChild, offsetX, offsetY);
			return ;
	}

	if (_rect)
	{
		rect=*_rect;
	}

	if (rectChild==0)
	{
		rectChild=&_rectChild;
		_rectChild=rect;
	}


	int x=rectChild?rectChild->left:rect.left;
	int y=rectChild?rectChild->top:rect.top;
	int w=rectChild?rectChild->right:rect.right;
	int h=rectChild?rectChild->bottom:rect.bottom;

	if (m_item.fTransparency!=1.0 && bIsLocalBufferUpdate==False)
	{
		if (m_item.fTransparency==0.0)
			return ;

		Bitmap* xbmp = BITMAP32(rect.right, rect.bottom);

		RECT xrect={0, 0, rect.right, rect.bottom};
		Update(xbmp, &xrect, rectChild, m_nAbsX+rectChild->left, m_nAbsY+rectChild->top, True);

		//if (m_bmpCurrent)
		//	delete m_bmpCurrent;

		Bitmap* bmp2=cyiBitmapBlender::Transparent(xbmp, m_item.fTransparency);
		delete xbmp;
		Graphics g(bmp);
		g.DrawImage(bmp2, rect.left, rect.top, 0, 0, w, h, Unit::UnitPixel);
		delete bmp2;
		return ;
	}

	Graphics g(bmp);
	Bitmap* pBmpCurrent = GetCurrentBitmap(&m_item);

	if (pBmpCurrent)
	{
		g.DrawImage(pBmpCurrent, rect.left, rect.top, x, y, w, h, Unit::UnitPixel);
	}
	else
	{
		if (OnDrawMyself(bmp, &rect, rectChild)==False)
		{
			//goto SKIP_DRAW;
			Bitmap* bg=GetBgBitmap();

			if (bg!=0)
			{
				Graphics g(bmp);
				Status status = g.DrawImage(bg, rect.left, rect.top, x, y, w, h, Unit::UnitPixel);
			}
		}
	}

	int sx=0, sy=0;
	if (m_parent)
	{
		m_parent->GetScrollPos(sx, sy);
	}

//__xITEM_LOCK__;	
	if (MutexLock(False)==False)
	{
		return ;
	}

	cyiList* pList=m_listItem;
{
LOOP:
	for (cyiNode* node = pList->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* s=(sDrawItem*)node->ref;

		RECT retRect;
		RECT crect = {s->rect.x, s->rect.y, s->rect.w, s->rect.h};


		if (xIsInRect(&crect, rectChild, retRect)==False)
		{
			continue;
		}
		RECT xrect = {retRect.left+m_nAbsX-offsetX-sx, retRect.top+m_nAbsY-offsetY-sy, retRect.right, retRect.bottom};

		retRect.left-=s->rect.x;
		retRect.top-=s->rect.y;

		if (xIS_ITEM_HIDE(s)!=0)
			continue;

		if (xGET_ITEM_TYPE(s)!=eSPT_GLASSINTERFACE)
		{
			if (s->fTransparency==0.0)
				continue;

			Bitmap* bmpCur = GetCurrentBitmap(s);
			if (s->fTransparency==1.0)
			{
				if (bmpCur->GetLastStatus()==Status::Ok)
				{
					g.DrawImage(bmpCur, (int)xrect.left, xrect.top, retRect.left, retRect.top, retRect.right, retRect.bottom, UnitPixel);
				}
				else
				{
					Sleep(1);
				}
			}
			else
			{
				Bitmap* pBmp = cyiBitmapBlender::Transparent(bmpCur, s->fTransparency);
				g.DrawImage(pBmp, (int)xrect.left, xrect.top,
					retRect.left, retRect.top, retRect.right, retRect.bottom, UnitPixel);
				delete pBmp;
			}
			continue;
		}
		else
		{
			cGlassInterface* obj = (cGlassInterface*)s->gls;
			obj->Update(bmp, &xrect, &retRect, offsetX, offsetY);

			if (xIS_GLS_FLAG(obj->m_flag, eGF_REQUEST_UPDATE)==True)
			{
				xOFF_GLS_FLAG(obj->m_flag, eGF_REQUEST_UPDATE);
				//m_glsDrawInterface->m_nRequestUpdateCount++;
			}

		}
	}

	if (pList!=&m_listFreeItem)
	{
		pList=&m_listFreeItem;
		if (pList->count()!=0)
		{
			goto LOOP;
		}
	}
}
__xITEM_UNLOCK__;

	if (m_fgItem.pCurrentBmp)
	{
		Bitmap* bmpCur = GetCurrentBitmap(&m_fgItem);
		g.DrawImage(bmpCur, rect.left, rect.top, x, y, w, h, Unit::UnitPixel);
	}

	return ;
}


// ---------------------------------------
// m_bg -> ???
LRESULT cglsStackPannel::OnBgAnimation(WPARAM wp, LPARAM lp, Bool& handled)
{
/*	int cnt=wp;
	float factor;
	sDrawItem* item = (sDrawItem*)lp;
	Bool bIsQuit=False;
	int sw, sh;


	// アニメーションが既にきゃんせるされている
	if (item==0)
	{
		//何もせず。
		return 0;
	}

	__xITEM_LOCK__;

	if (m_bg==0 || yiAnitIsAlived(item->hAnit)==False)
	{
		yiAnitStop(item->hAnit, False);
		__xITEM_UNLOCK__;
		return 0;
	}

	GetScrollPos(sw, sh);

	if ((factor=yiAnitGetFactor(item->hAnit, cnt))<0.0)
	{
		yiAnitStop(item->hAnit, False);
		bIsQuit=True;

		if (item->pStartBmp==0 &&
				item->pEndBmp==0)
		{
			item->fTransparency=item->to;
		}
		else
		{
			if (item->animation->pBmpAnimation)
				delete item->animation->pBmpAnimation;
		}
	}
	else
	{
		//xTRACE("OnBgAnimation() factor:%f, cnt:%d", factor, cnt);
		if (item->animation->pBmpAnimation)
			delete item->animation->pBmpAnimation;

		if (item->pStartBmp==0 &&
				item->pEndBmp==0)
		{
			item->fTransparency=factor*item->to+(1-factor)*item->from;
			//xTRACE("cglsStackPannel::OnBgAnimation() fTransparency:%f, factor:%f", item->fTransparency, factor);
		}
		else
		{
			item->animation->pBmpAnimation = cyiBitmapBlender::Desolve(item->pStartBmp, item->pEndBmp, factor);
		}
	}

	//if (m_parent)
	//{
	//	if (m_parent->IsBgAnimation()==True)
	//	{
	//		if (bIsQuit==True)
	//		{
	//			item->animation->pBmpAnimation=0;
	//		}
	//		__xITEM_UNLOCK__;

	//		return 0;
	//	}
	//}

	if (bIsQuit==False)
	{
		InvalidateRect(0, False);
	}
	else
	{
		item->animation->pBmpAnimation=0;
		InvalidateRect(0, False);
	}

	__xITEM_UNLOCK__;
*/
	return 0;
}

// ---------------------------------------
// m_bg -> ???
LRESULT cglsStackPannel::OnFgAnimation(WPARAM wp, LPARAM lp, Bool& handled)
{
/*	int cnt=wp;
	float factor;
	sDrawItem* item = (sDrawItem*)lp;
	Bool bIsQuit=False;
	int sw, sh;

	// アニメーションが既にきゃんせるされている
	if (item==0)
	{
		//何もせず。
		return 0;
	}

	__xITEM_LOCK__;

	if (yiAnitIsAlived(item->hAnit)==False)
	{
		yiAnitStop(item->hAnit, False);
		__xITEM_UNLOCK__;
		return 0;
	}

	GetScrollPos(sw, sh);

	if ((factor=yiAnitGetFactor(item->hAnit, cnt))<0.0)
	{
		yiAnitStop(item->hAnit, False);

		bIsQuit=True;

		if (item->animation->pBmpAnimation)
			delete item->animation->pBmpAnimation;
		item->animation->pBmpAnimation=0;
	}
	else
	{
		item->animation->pBmpAnimation = cyiBitmapBlender::Desolve(item->pStartBmp, item->pEndBmp, factor);
	}

	// BgAnimationの方が優先
	//if (IsBgAnimation()==True)
	//{
	//	if (bIsQuit==True)
	//	{
	//		item->animation->pBmpAnimation=0;
	//	}
	//	__xITEM_UNLOCK__;


	//	return 0;
	//}

	if (bIsQuit==False)
	{
		if (m_glsDrawInterface && xIS_GLS_FLAG(m_glsDrawInterface->m_flag, eGF_REQ_UPDATE_BUFFER))
		{
			InvalidateRect(0, False);
		}
		else
			DirectDraw(item->animation->pBmpAnimation);
		//InvalidateRect(0, False);
	}
	else
	{
		item->animation->pBmpAnimation=0;
		InvalidateRect(0, False);
	}

	__xITEM_UNLOCK__;
*/
	return 0;
}



// ------------------------------------
// Default
int cglsStackPannel::GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation, Bitmap** ret)
{
	eDrawItemStatus _status = xGET_ITEM_STATUS(item);
	eDrawItemStatus _preStatus = xGET_ITEM_PRE_STATUS(item);
	bIsNoAnimation=True;
	Bool bIsTransparentAnimation=(item->fEnterTransparency!=item->fLeaveTransparency);

	if (ret)
		*ret=0;

	if (status==eDIS_MOUSE_ENTER)
	{
		status=eDIS_MOUSE_MOVE;
	}

	if (status==_status)
	{
		return -1;
	}

	if (status==eDIS_FORCED)
	{
		SetItemStatus(item, eDIS_NORMAL);
		return 0;
	}

	if (status==eDIS_MOUSE_MOVE)
	{
		if (_status==eDIS_NORMAL)
		{
			//if (xIS_ITEM_ANIMATION_NOTIFY_ONLY(item))
			//{
			//	bIsNoAnimation=False;
			//	SetItemStatus(item, status);
			//	return -4;
			//}

			//if (bIsTransparentAnimation &&
			//		item->fTransparency!=item->fEnterTransparency)
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
		//if (_status==eDIS_MOUSE_MOVE)
		//{
		SetItemStatus(item, status);
		return 2;
		//}
		//return -1;
	}

	if (status==eDIS_MOUSE_LEAVE)
	{
		if (_status==eDIS_MOUSE_MOVE)
		{
			if (xIS_ITEM_ANIMATION_NOTIFY_ONLY(item))
			{
				bIsNoAnimation=False;
				SetItemStatus(item, eDIS_NORMAL);
				return -4;
			}

			if (bIsTransparentAnimation &&
					item->fTransparency!=item->fLeaveTransparency)
			{
				bIsNoAnimation=False;
				SetItemStatus(item, eDIS_NORMAL);
				return -3;
			}

			bIsNoAnimation=False;
			SetItemStatus(item, eDIS_NORMAL);
			return 0;
		}
		else if (_status==eDIS_MOUSE_DOWN)
		{
			SetItemStatus(item, eDIS_NORMAL);
			return 0;
		}

		return -1;
	}

	if (status==eDIS_MOUSE_UP)
	{
		if (_status==eDIS_MOUSE_DOWN)
		{
			if (bIsTransparentAnimation &&
					item->fTransparency!=item->fLeaveTransparency)
			{
				bIsNoAnimation=False;
				SetItemStatus(item, eDIS_MOUSE_MOVE);
				return -3;
			}

			SetItemStatus(item, eDIS_MOUSE_MOVE);
			return 1;
		}

		return -1;
	}

	return -1;

}

// ---------------------------------------
Bool cglsStackPannel::xCreateBg(int w, int h,Bool bIsForced)
{
	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED)==True)
	{
		return False;
	}

	if (w==0 ||
			h==0)
	{
		return False;
	}

	if (bIsForced==False &&
		m_item.rect.w==w &&
			m_item.rect.h==h)
	{
		return False;
	}

	// size changed.
	//w=w<m_w?m_w:w;
	//h=h<m_h?m_h:h;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True)
	{
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)==True)
		{
			w=m_w;
			//h=m_h;
		}
		else
		{
			h=m_h;
			//w=m_w;
		}
	}

	if (m_bmpCurrent!=0)
	{
		delete m_bmpCurrent;
	}
	
	if (m_w!=0 &&
			m_h!=0)
	{
		m_bmpCurrent = new Bitmap(m_w, m_h, PixelFormat32bppARGB);
	}
	else
	{
		if (m_bmpCurrent)
			delete m_bmpCurrent;
		m_bmpCurrent=0;
	}

	//if (m_bg)
	//{
	//	delete m_bg;
	//	m_bg=0;
	//}

	RECT rect = {0, 0, w, h};
	
	m_item.rect.w=w;
	m_item.rect.h=h;

	if (m_DrawObject)
	{
		if (m_DrawObject->CreateBitmap((sDrawItem*)&m_item, 0)==True)
		{
			//m_bg = cyiBitmapBlender::BitmapCopy(0, m_item.pCurrentBmp);
		}
		else
		{
			//m_bg = 0;
		}


		// fg
		m_fgItem.id=-1;
		m_fgItem.rect.w=w;
		m_fgItem.rect.h=h;
		
		if (m_DrawObject->CreateBitmap((sDrawItem*)&m_fgItem, 0)==True)
		{
			if (xIS_GLS_FLAG(m_flag, eGF_NO_ANIMATION)==False &&
				m_fgItem.hAnit==0)
			{
				m_fgItem.hAnit=yiAnitCreate();
			}
		}
	}

	//if (m_bg)
	//	cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);
	//else
	//	cyiBitmapBlender::FillColor(m_bmpCurrent, 0);

	return True;
}

// ---------------------------------------
Bool cglsStackPannel::xDrawItem(sDrawItem* item, eDrawItemStatus status)
{
	Bool bSkipControlUpdate=False;
	Bool bIsNoAnimation=False;
	int nBitmapIndex=0;
	int nStartIndex=0;
	int nEndIndex=0;

	if (m_bmpCurrent==0)
	{
		return False;
	}

	// Draw All
	if (item==0 || (IntPtr)(item)==-1)
	{
		goto DRAW_ALL;
	}

	nStartIndex=item->nLastItemIndex;

	if (status==eDIS_FORCED)
	{
		status=eDIS_NORMAL;
		nBitmapIndex=0;
		bIsNoAnimation=True;

		if (item->pBmp[nBitmapIndex]==0)
			return True;

		goto SKIP_BMP_INDEX;
	}

	if ((nBitmapIndex=GetStatusToBitmapIndex(item, status, bIsNoAnimation))==-1)
	{
		return False;
	}

	if (nStartIndex==nBitmapIndex)
		return False;

	nEndIndex=nBitmapIndex;

	item->nLastItemIndex=nBitmapIndex;

SKIP_BMP_INDEX:
	if (m_DrawObject)
	{
		eDrawItemStatus status = xGET_ITEM_STATUS(item);

		if (item->pBmp[nBitmapIndex]==0)
		{
			if (m_DrawObject->OnNullBitmapDraw(item, (void*)nBitmapIndex)==False)
			{
				return False;
			}
		}
	}

	if (xIS_GLS_FLAG(m_flag, eGF_NO_ANIMATION)==True)
	{
		bIsNoAnimation=True;
	}

	if (bIsNoAnimation==True)
	{
		goto SKIP_ANIMATION;
	}

	if (OnItemAnimation(item, nStartIndex, nEndIndex)==False)
	{
		sGlsAnimationEx ani;
		glsInitAnimationEx(&ani, item);
		glsSetDvAnimationEx(&ani, nStartIndex, nEndIndex, nEndIndex==1?eGAP_FISO:eGAP_SIFO);
		StartAnimation(item, &ani, m_nAnimationMilliSec, nEndIndex==1?m_nMouseMoveAnimationInterval:m_nMouseLeaveAnimationInterval);
	}

	return True;


SKIP_ANIMATION:
	StopAnimation(item, True);
	InvalidateRect(0);

	item->pCurrentBmp=item->pBmp[nBitmapIndex];

/*	if (bmpItem==0)
	{
		return True;
	}
*/
	if (m_bg)
	{
		//cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg, item->rect.x, item->rect.y, item->rect.x, item->rect.y, item->rect.w, item->rect.h);
	}

	{
		//Graphics g(m_bmpCurrent);

		//if (item->pCurrentBmp)
		//{
		//	g.DrawImage(item->pCurrentBmp, item->rect.x, item->rect.y, 0, 0, item->pCurrentBmp->GetWidth(), item->pCurrentBmp->GetHeight(), UnitPixel);
		//}
	}

	return True;

DRAW_ALL:

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True)
	{
		RECT rect = {0, 0, m_bmpCurrent->GetWidth(), m_bmpCurrent->GetHeight()};
		//cyiBitmapBlender::FillColor(m_bmpCurrent, 0);
		Update(m_bmpCurrent,  &rect, 0, m_nAbsX, m_nAbsY);
	}

	return True;
}

// -----------------------------
Bool cglsStackPannel::OnItemAnimation(sDrawItem* item, int start, int end)
{
	return False;
	/*if (item->id
	return True;*/
}


static int xGetPosEx(eStackAlign align, int from, int end, int width)
{
	if (align==eSA_LEFT)
	{
		return from;
	}
	if (align==eSA_CENTER)
	{
		return (((end-from)-width)>>1)+from;
	}

	else
	{
		return (end-width);
	}
}


// ---------------------------------------
static int xGetPos(eStackAlign align, RECT* rect, RECT* margine, int w, int h)
{
	if (w==0)
	{
		if (align==eSA_CENTER)
		{
			int _h = (rect->bottom-margine->top-margine->bottom);
			return ((_h-h)>>1)+margine->top;
		}
		else if (align==eSA_LEFT)
		{
			return margine->top;
		}
		else
		{
			return rect->bottom-h-margine->bottom;
		}
	}

	if (align==eSA_CENTER)
	{
		int _w = (rect->right-margine->left-margine->right);
		return ((_w-w)>>1)+margine->left;
	}
	else if (align==eSA_LEFT)
	{
		return margine->left;
	}
	else
	{
		return rect->right-w-margine->right;
	}

	return 0;
}

// ---------------------------------------
void cglsStackPannel::xCalculatePosition(RECT* rectAll, Bool bIsCalculateEnd/*=False*/)
{
	int sum=0;
	cyiStack stackItemLength;
	cyiQueue queueItemWidth;
	int nTotalHeight=0;
	int nLineTop=0;
	//RECT rect;

	if (m_nItemzLength!=0 &&
			bIsCalculateEnd==True)
	{
		goto CALCULATE_LAST_ITEM;
	}

	m_nItemzLength=0;
	m_nItemzWidth=0;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)==FALSE)
	{	
		goto NOT_VERTICAL;
	}
	m_nItemzLength=m_rectMargin.top;

	for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item = (sDrawItem*)node->ref;
		eStackPannelType type=xGET_ITEM_TYPE(item);

		if (type==eSPT_LF)
		{
			if (node->np!=0)
			{
				stackItemLength.push(m_nItemzLength);
				queueItemWidth.enq(m_nItemzWidth);
				m_nItemzLength=m_rectMargin.top;
				nTotalHeight+=(item->distance+m_nItemzWidth);
				m_nItemzWidth=0;
			}
			continue;
		}
		if (xIS_ITEM_HIDE(item)!=0)
		{
			continue;
		}

		item->rect.y = m_nItemzLength+item->distance;
		//item->rect.y = nTotalHeight; // とりあえずは、eSA_LEFTにする。
		if (type==eSPT_GLASSINTERFACE)
		{
			cGlassInterface* gls=(cGlassInterface*)item->gls;

			item->rect.w=gls->m_w;
			item->rect.h=gls->m_h;
		}

		m_nItemzLength+=(item->rect.h+item->distance);
		m_nItemzWidth = m_nItemzWidth>item->rect.w? m_nItemzWidth:item->rect.w;
	}

	if (stackItemLength.count()!=0)
	{
		yiAnyValue data;
		while(stackItemLength.pop(data))
		{
			m_nItemzLength=m_nItemzLength>data?m_nItemzLength:data;
		}
		m_nItemzWidth+=nTotalHeight;
	}

	m_nItemzLength+=m_rectMargin.bottom;

	if (rectAll)
	{
		rectAll->left=0;
		rectAll->top=0;
		rectAll->right=m_nItemzWidth+m_rectMargin.left+m_rectMargin.right;
		rectAll->bottom=m_nItemzLength;
	}

	if (queueItemWidth.count()!=0)
	{
		nLineTop=m_rectMargin.top; // eSA_TOP時の座標
		queueItemWidth.deq((yiAnyValue&)nTotalHeight); // 幅
	}
	else
	{
		nTotalHeight=m_nItemzWidth;
		nLineTop=m_rectMargin.left;
	}


	for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item = (sDrawItem*)node->ref;

		eStackPannelType type=xGET_ITEM_TYPE(item);

		if (type==eSPT_LF) // 次のLine
		{
			if (node->np!=0)
			{
				int val;
				queueItemWidth.deq((yiAnyValue&)val);
				nLineTop=(nTotalHeight+nLineTop+item->distance);
				nTotalHeight=val;
			}
			continue;
		}
		if (xIS_ITEM_HIDE(item)!=0)
		{
			continue;
		}

		item->rect.x = ::xGetPosEx(xGET_ITEM_ALIGN(item), nLineTop, m_nItemzWidth+m_rectMargin.left+m_rectMargin.right,item->rect.w);
		//item->rect.x = ::xGetPos(xGET_ITEM_ALIGN(item), rectAll, &m_rectMargin, item->rect.w, 0);
		
		if (xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE)
		{
			cGlassInterface* gls=(cGlassInterface*)item->gls;
			gls->Move(item->rect.x, item->rect.y);
		}
	}


	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_SHOW_SCROLLBAR)==True &&
			xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True &&
				xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False)
	{
		SetScrollRange(0, m_nItemzLength, m_h);
	}
	return ;
NOT_VERTICAL:

	m_nItemzLength=m_rectMargin.left;



	for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item = (sDrawItem*)node->ref;
		eStackPannelType type=xGET_ITEM_TYPE(item);

		if (type==eSPT_LF)
		{
			stackItemLength.push(m_nItemzLength);
			queueItemWidth.enq(m_nItemzWidth);
			m_nItemzLength=m_rectMargin.left;
			nTotalHeight+=(item->distance+m_nItemzWidth);
			m_nItemzWidth=0;
			continue;
		}


		if (xIS_ITEM_HIDE(item)!=0)
		{
			continue;
		}

		item->rect.x = m_nItemzLength+item->distance;
		//item->rect.y = nTotalHeight; // とりあえずは、eSA_TOPにする。
		
		if (xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE)
		{
			cGlassInterface* gls=(cGlassInterface*)item->gls;

			item->rect.w=gls->m_w;
			item->rect.h=gls->m_h;
		}

		m_nItemzLength+=(item->rect.w+item->distance);
		m_nItemzWidth = m_nItemzWidth>item->rect.h? m_nItemzWidth:item->rect.h;
	}

	if (stackItemLength.count()!=0)
	{
		yiAnyValue data;
		while(stackItemLength.pop(data))
		{
			m_nItemzLength=m_nItemzLength>data?m_nItemzLength:data;
		}
		m_nItemzWidth+=nTotalHeight;
	}

	m_nItemzLength+=m_rectMargin.right;
	if (rectAll)
	{
		rectAll->left=0;
		rectAll->top=0;
		rectAll->right=m_nItemzLength;
		rectAll->bottom=m_nItemzWidth+m_rectMargin.top+m_rectMargin.bottom;
	}

	if (queueItemWidth.count()!=0)
	{
		nLineTop=m_rectMargin.top; // eSA_TOP時の座標
		queueItemWidth.deq((yiAnyValue&)nTotalHeight); // 幅
	}
	else
	{
		nTotalHeight=m_nItemzWidth;
		nLineTop=m_rectMargin.top;
	}
	
	int offset=0;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True)
	{
		if (m_eAlign==eSA_CENTER)
		{
			offset = ((m_w-m_nItemzLength)>>1);
		}
		else if (m_eAlign==eSA_RIGHT)
		{
			offset = (m_w-m_nItemzLength);
		}
	}

	for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item = (sDrawItem*)node->ref;
		eStackPannelType type=xGET_ITEM_TYPE(item);

		if (type==eSPT_LF) // 次のLine
		{
			int val;
			queueItemWidth.deq((yiAnyValue&)val);
			nLineTop=(nTotalHeight+nLineTop+item->distance);
			nTotalHeight=val;
			continue;

		}

		if (xIS_ITEM_HIDE(item)!=0)
		{
			continue;
		}

		//item->rect.y = ::xGetPos(xGET_ITEM_ALIGN(item), rectAll, &m_rectMargin, 0, item->rect.h);

		item->rect.y = ::xGetPosEx(xGET_ITEM_ALIGN(item), nLineTop, nLineTop+nTotalHeight,item->rect.h);
		item->rect.x += offset;

		if (type==eSPT_GLASSINTERFACE)
		{
			cGlassInterface* gls=(cGlassInterface*)item->gls;
			gls->Move(item->rect.x, item->rect.y);
		}
	}



	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_SHOW_SCROLLBAR)==True &&
			xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True &&
				xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False)
	{
		SetScrollRange(0, m_nItemzLength, m_h);
	}

	return ;

CALCULATE_LAST_ITEM:
	{
		cyiNode* node=m_listItem->GetNodeTailFrom(0);
		cyiNode* prevNode=node->pp;

		if (prevNode==0)
		{
			return ;
		}

		sDrawItem* item = (sDrawItem*)node->ref;
		sDrawItem* prevItem = (sDrawItem*)prevNode->ref;

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL))
		{
			m_nItemzLength+=item->distance+item->rect.h;
			m_nItemzWidth=m_nItemzWidth>item->rect.w?m_nItemzWidth:item->rect.w;
			item->rect.y=prevItem->rect.y+prevItem->distance+prevItem->rect.h;

			RECT xrect = {0, 0, m_nItemzWidth+m_rectMargin.left+m_rectMargin.right, m_nItemzLength};
			item->rect.x = ::xGetPos(xGET_ITEM_ALIGN(item), &xrect, &m_rectMargin, item->rect.w, 0);

			if (rectAll)
			{
				*rectAll=xrect;
			}
		}
		else
		{
			m_nItemzLength+=item->distance+item->rect.w;
			m_nItemzWidth=m_nItemzWidth>item->rect.h?m_nItemzWidth:item->rect.h;
			item->rect.x=prevItem->rect.x+prevItem->distance+prevItem->rect.w;

			RECT xrect = {0, 0, m_nItemzLength, m_nItemzWidth+m_rectMargin.top+m_rectMargin.bottom};
			item->rect.y = ::xGetPos(xGET_ITEM_ALIGN(item), &xrect, &m_rectMargin, 0, item->rect.h);
			
			if (rectAll)
			{
				*rectAll=xrect;
			}
		}
	}

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_SHOW_SCROLLBAR)==True &&
			xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True &&
				xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False)
	{
		SetScrollRange(0, m_nItemzLength, m_h);
	}

	return ;
}

// -----------------------------
typedef struct xsScrollAnit {
	unsigned int from;
	unsigned int to;
	int nStopScrollPos;
	int nStopMillisec;
	Bitmap* bg;
} sScrollAnit;

// ------------------------------------
Bool cglsStackPannel::xIsItemIsValid(sDrawItem* item)
{
	m_NowAnimationList.lock();

	for (cyiNode* node=m_NowAnimationList.GetNodeTopFrom(); node; node=node->np)
	{
		if (item==(sDrawItem*)node->ref)
		{
			m_NowAnimationList.unlock();
			return True;
		}
	}

	m_NowAnimationList.unlock();
	return False;
}

// ------------------------------------
LRESULT cglsStackPannel::OnGlsAnimationEx(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{

	return 	__super::OnGlsAnimationEx(wParam, lParam, bIsHandled);
	bIsHandled=True;

	if (MutexLock(False)==False)
	{
		xTRACE("cglsStackPannel::OnGlsAnimation() Locked ... skip animation.");
		MutexLock();
		// この段階でitemが削除されたかどうかをチェック。
		if (xIsItemIsValid((sDrawItem*)lParam)==False)
		{
			xTRACE("cglsStackPannel::OnGlsAnimation() item invalid");
			//MutexUnlock();
			//return 0;
		}

		MutexUnlock();
		return 0;
	}

	//if (m_bIsRequestCheckItem==True)
	//{
	// この段階でitemが削除されたかどうかをチェック。
	if (xIsItemIsValid((sDrawItem*)lParam)==False)
	{
		xTRACE("cglsStackPannel::OnGlsAnimation() item invalid(%x)", lParam);
		MutexUnlock();
		return 0;
	}
	//xTRACE("cglsStackPannel::OnGlsAnimation() item Valid");
	m_bIsRequestCheckItem=False;
	//}

	//else
	//{
	//	__xITEM_LOCK__;
	//}

	//if (xGET_ITEM_TYPE(((sDrawItem*)lParam))==eSPT_SCROLLBAR)
	//{
	//	return xOnScrollBarAnimation(wParam, lParam, bIsHandled);
	//}

	LRESULT res = __super::OnGlsAnimationEx(wParam, lParam, bIsHandled);
	//__xITEM_UNLOCK__;

	MutexUnlock();

	return res;
}

// ---------------------------------------
LRESULT cglsStackPannel::OnScrolled(WPARAM wp, LPARAM lp, Bool& handled)
{
	int cnt=wp;
	sScrollAnit* s = (sScrollAnit*)lp;
	float factor;
	int range;
	int delta=0;
	Bool bUseDirectDraw=False;//xIS_GLS_FLAG(m_eStackFlag, eSPF_SCROLL_DIRECT);

	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);
	if (s==0)
	{
		// 中止された状態
		return 0;
	}

	//if (m_glsDrawInterface && xIS_GLS_FLAG(m_glsDrawInterface->m_flag, eGF_REQ_UPDATE_BUFFER))
	//{
	//	bUseDirectDraw=False;
	//}

	if ((int)s==-1)
	{
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False)
		{
			//Bitmap* bg = cyiBitmapBlender::BitmapCopy(0, s->bg);
			////Bitmap* bg = cyiBitmapBlender::BitmapCopy(0, m_bmpCurrent, 0, 0, m_bIsVertical?0:m_nScrollPos, m_bIsVertical?m_nScrollPos:0, m_w, m_h);
			//Graphics g(bg);
			//g.DrawImage(m_bmpCurrent, 0, 0, m_bIsVertical?0:m_nScrollPos, m_bIsVertical?m_nScrollPos:0, m_w, m_h, UnitPixel);
			//
			//if (xIS_ITEM_HIDE((&m_itemScrollArea))==0)
			//{
			//	SetScrollPos(m_nScrollPos);
			//	xDrawScrollBar(&g);
			//}

			//DirectDraw(bg);
			//delete bg;
		}
		else
		{
			if (bUseDirectDraw)
			{
				RECT crect = {xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?0:m_nScrollPos, xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?m_nScrollPos:0, m_w, m_h};
				Update(m_bmpCurrent, &crect, 0, m_nAbsX, m_nAbsY);
				DirectDraw(m_bmpCurrent);
			}
			else
			{
				InvalidateRect(0);
			}
		}

		return 0;
	}

	__xITEM_LOCK__;

	range= s->to-s->from;

	if ((factor=yiAnitGetFactor(m_hScrollAnit, cnt))<0.0 ||
			factor>1.0)
	{
		// 終了
		yiAnitStop(m_hScrollAnit, False);

		m_nScrollPos=s->to;
		SetScrollPos(m_nScrollPos);

		InvalidateRect(0);

		if (s->nStopScrollPos!=0)
			Scroll(s->nStopScrollPos, s->nStopMillisec);
		m_listItem->unlock();

		if (s->nStopScrollPos==0)
		{
			PostMessage(m_hNotifyWnd, WM_STACKPANNEL_SCROLL_END, 0, 0);
		}
		__xITEM_UNLOCK__;
		return 0;
	}

	delta = (int)(factor*(float)range);


	if (m_itemScrollArea.fTransparency==0.0)
			m_itemScrollArea.fTransparency=0.3;

	//m_nScrollPos
	int nScrollPos=s->from+delta;
	
	Bool bScrollResult=True;
	if (xIS_ITEM_HIDE((&m_itemScrollArea))==0)
	{
		bScrollResult=SetScrollPos(nScrollPos);//)==True)
		nScrollPos=m_scrollInfo.pos;
	}

	m_nScrollPos=nScrollPos;

	int nSystemOffsetX = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?0:m_rectSystem.right;
	int nSystemOffsetY = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?m_rectSystem.bottom:0;

	if (bUseDirectDraw)
	{
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False)
		{
			Bitmap* bg = cyiBitmapBlender::BitmapCopy(0, s->bg);



			Graphics g(bg);

			if (m_bg)
			{
				g.DrawImage(m_item.pCurrentBmp, nSystemOffsetX, nSystemOffsetY, xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?0:nScrollPos, xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?nScrollPos:0, 
								m_w-nSystemOffsetX, m_h-nSystemOffsetY, UnitPixel);
			}

			g.DrawImage(m_bmpCurrent, nSystemOffsetX, nSystemOffsetY, m_bIsVertical?0:nScrollPos, m_bIsVertical?nScrollPos:0, 
							m_w-nSystemOffsetX, m_h-nSystemOffsetY, UnitPixel);
			
			xDrawScrollBar(&g, 0, True);
			DirectDraw(bg);
			delete bg;
		}
		else
		{
			RECT crect = {m_bIsVertical?0:nScrollPos, m_bIsVertical?nScrollPos:0, m_w, m_h};
			Update(m_bmpCurrent, &crect, 0, m_nAbsX, m_nAbsY);

			DirectDraw(m_bmpCurrent);
		}
	}
	else
	{
		//RECT rect={m_itemScrollArea.rect.x, m_itemScrollArea.rect.y, m_itemScrollArea.rect.w+m_itemScrollArea.rect.x, m_itemScrollArea.rect.h+m_itemScrollArea.rect.y};
		//InvalidateRect(&rect, FALSE);

		InvalidateRect(0);
	}

	//DirectDraw(m_bmpCurrent);
	//InvalidateRect(0, FALSE);
	__xITEM_UNLOCK__;

	return 0;
}

sScrollAnit g_scrollAnit;

// -----------------------------
int cglsStackPannel::Scroll(int pos, int nMilliSec, int StopPos, int nStopPosMilliSec)
{
	unsigned int prev = m_nScrollPos;
	int range;
	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==False)
		return -1;



	if (nMilliSec==0)
	{
		Bool handled=False;
		m_nScrollPos=pos;
		OnScrolled(0, -1, handled);
		return 0;
	}

	pos-=StopPos;
	if (xIS_GLS_FLAG(m_flag, eGF_NO_ANIMATION))
	{
		m_nScrollPos=pos;
		InvalidateRect(0, FALSE);
	}

	if (m_hScrollAnit==0)
	{
		m_hScrollAnit = yiAnitCreate();
	}

	if (yiAnitIsAlived(m_hScrollAnit)==True)
	{
		yiAnitStop(m_hScrollAnit, True);
	}

	pos+=m_nScrollPos;
	if (pos<0)
	{
		pos=0;
	}
	//else if (pos>(int)(m_bmpCurrent->GetHeight()-m_h))
	else if (pos>(m_nItemzLength-m_h))
	{
		pos = m_nItemzLength-m_h;//m_bmpCurrent->GetHeight()-m_h;
	}

	if (nMilliSec==0)
	{
		m_nScrollPos=pos;
		//InvalidateRect(0, FALSE);

		return prev;
	}

	Bitmap* bg = GetParentBg(0, 0, 0);


	sScrollAnit s = {m_nScrollPos, pos, StopPos, nStopPosMilliSec, bg};
	g_scrollAnit=s;
	yiAnitStart(m_hScrollAnit, m_hWnd, WM_STACKPANNEL_SCROLLED, nMilliSec, 15, eGAP_FISO, &g_scrollAnit/*xTOHEAP(&s)*/);

	return prev;
}


// ---------------------------------------
void cglsStackPannel::Destroy()
{
	if (m_bIsCreated==False)
		return ;

	m_bIsCreated=False;

	if (m_parent)
	{
		m_parent->UnRegisterChild(this);
	}

	cyctrlbase::Destroy();

	for (cyiNode* node = m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* item=(sDrawItem*)node->ref;
		xDestroyItem(item);
	}

	m_listItem->clear();

	if (m_bg)
		delete m_bg;

	if (m_bmpCurrent)
		delete m_bmpCurrent;

	yiAnitDestroy(m_hScrollAnit);
	yiAnitDestroy(m_item.hAnit);

	delete m_listItem;
	m_listItem=0;
}

// ------------------------------------
Bool cglsStackPannel::IsBgAnimation()
{
	if (m_item.hAnit==0)
		return False;

	if (yiAnitIsAlived(m_item.hAnit)==False)
	{
		if (m_parent)
		{
			return m_parent->IsBgAnimation();
		}
		return False;
	}

	return False;
}

// ------------------------------------
LRESULT cglsStackPannel::OnLButtonLongPressed(WPARAM wParam, LPARAM lParam)
{
	if (m_nDragOnTimer!=-1)
	{
		StartDrag(wParam, lParam);
	}
	else if (m_nKeyRepeatInterval!=0)
	{
		int x=wParam-m_itemScrollArea.rect.x;
		int y=lParam-m_itemScrollArea.rect.y;
		if (xGetScrollParts(wParam, lParam)!=-1)
		{
			StartLButtonRepeat(x, y, m_nKeyRepeatInterval);
			return 0;
		}
	}
	return StartLButtonRepeat(wParam, lParam, m_nKeyRepeatInterval);
}

// ------------------------------------
LRESULT cglsStackPannel::OnDragStart(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int sw=0, sh=0;
	handled=True;
	GetScrollPos(sw, sh);

	sDrawItem* item = xFindItem(wParam+sw, lParam+sh, &m_listFreeItem);
	m_itemDraged=item;

	if (item==0)
	{
		// 全体が動くモード
		return __super::OnDragStart(wParam, lParam, handled);
	}

	// スクロールバーの場合
	if (xGET_ITEM_TYPE(item)==eSPT_SCROLLBAR)
	{
		Bool handled=True;

		return xOnScrollBarDragStart(wParam, lParam, handled);
	}

	if (item->animation==0)
	{
		item->animation = (sGlsAnimationEx*)yialloci(sizeof(sGlsAnimationEx));
	}


	if (item->animation->pBmpAnimation)
	{
		delete item->animation->pBmpAnimation;
	}

	item->animation->pBmpAnimation=cyiBitmapBlender::Transparent(item->pCurrentBmp, 0.5);
	//YIDEBUG("cglsButton::OnDragStart() called");
	m_itemMoveOrgX=item->rect.x;
	m_itemMoveOrgY=item->rect.y;
	xMoveItem(item,m_itemMoveOrgX+1, m_itemMoveOrgY+1);
	return 1;
}

// ------------------------------------
LRESULT cglsStackPannel::OnDrag(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int x,y;
	handled=True;
	GetDragStartPos(x, y);
	sDrawItem* item = m_itemDraged;//xFindItem(x, y, &m_listFreeItem);

	//xTRACE("cglsStackPannel::OnDrag(%d, %d) called", wParam,lParam);
	if (item==0)
	{
		// 全体が動くモード
		m_itemDraged=0;
		return __super::OnDrag(wParam, lParam, handled);
	}

	// スクロールバーの場合
	if (xGET_ITEM_TYPE(item)==eSPT_SCROLLBAR)
	{
		Bool handled=True;
		return xOnScrollBarDrag(wParam, lParam, handled);
	}


	xMoveItem(item, m_itemMoveOrgX+((int)wParam-x), m_itemMoveOrgY+((int)lParam-y));
	//xTRACE("cglsButton::OnDragStart() called (%d,%d), wParam:%d, lParam:%d, x:%d, y:%d", item->rect.x, item->rect.y, wParam, lParam, x, y);

	handled=True;
	return 1;
}

// ------------------------------------
LRESULT cglsStackPannel::OnDragEnd(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int x, y;
	handled=True;

	GetDragStartPos(x, y);
	sDrawItem* item = m_itemDraged;//xFindItem(x, y, &m_listFreeItem);

	if (item==0)
	{
		// 全体が動くモード
		return __super::OnDragEnd(wParam, lParam, handled);
	}

	// スクロールバーの場合
	if (item->id==-2)
	{
		Bool handled=True;
		return xOnScrollBarDragEnd(wParam, lParam, handled);
	}

	if (item->animation &&
		item->animation->pBmpAnimation)
	{
		delete item->animation->pBmpAnimation;
		item->animation->pBmpAnimation=0;
	}

	xMoveItem(item, m_itemMoveOrgX+(wParam-x), m_itemMoveOrgY+(lParam-y));
	return 0;
}

// ------------------------------------
void static xRectOr(RECT& result, RECT& rect1, RECT& rect2)
{
	int x[4] = {rect1.left, rect1.right, rect2.left, rect2.right};
	int y[4] = {rect1.top, rect1.bottom, rect2.top, rect2.bottom};
	int min=0x7FFFFFFF, max=80000000;

	for (int i=0; i<4; i++)
	{
		min=min>x[i]?x[i]:min;
		max=min<x[i]?x[i]:max;
	}

	result.left=min;
	result.right=max;

	min=0x7FFFFFFF;
	max=80000000;
	for (int i=0; i<4; i++)
	{
		min=min>y[i]?y[i]:min;
		max=min<y[i]?y[i]:max;
	}
	result.top=min;
	result.bottom=max;

	return;
}

// ------------------------------------
Bool cglsStackPannel::xMoveItem(sDrawItem* item, int x, int y)
{
	if (item->rect.x==x &&
			item->rect.y==y)
	{
			return False;
	}

	RECT rectBefore = {item->rect.x, item->rect.y, item->rect.w+item->rect.x, item->rect.h+item->rect.y};
	RECT rectAfter = {x, y, item->rect.w+x, item->rect.h+y};
	RECT rectResult;
	xRectOr(rectResult, rectBefore, rectAfter);

	//xTRACE("cglsButton::xMoveItem() called (%d,%d)->(%d, %d)", item->rect.x, item->rect.y, x, y);
	item->rect.x=x;
	item->rect.y=y;
	InvalidateRect(&rectResult, False);
}
#include "glsResourceBitmap.h"
#include "glsButton.h"

#define __SCROLL_BAR_RATIO__ (0.7)
//#define __SCROLL_BAR_MARGIN__ 2
// ---------------------------------------------
Bool cglsStackPannel::xCreateScrollBar(Bool bIsRedrawOnly/*=False*/)
{
	if (m_bmpCurrent==0)
		return False;

	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	Graphics g(m_bmpCurrent);

	int w=(int)((float)cglsResourceObject::GetFontHeight(m_font)*__SCROLL_BAR_RATIO__);

	memset(&m_itemScrollArea, 0, sizeof(m_itemScrollArea));
	memset(&m_itemScrollBar, 0, sizeof(m_itemScrollArea));
	memset(&m_itemScrollBarPrev, 0, sizeof(m_itemScrollArea));
	memset(&m_itemScrollBarNext, 0, sizeof(m_itemScrollArea));
	memset(&m_itemScrollBarPage, 0, sizeof(m_itemScrollBarPage));

	m_itemScrollArea.rect.x=m_bIsVertical?(m_w-w-5):1;
	m_itemScrollArea.rect.y=m_bIsVertical?1:(m_h-w-5);
	m_itemScrollArea.rect.w=m_bIsVertical?w:(m_w-2);
	m_itemScrollArea.rect.h=m_bIsVertical?m_h-2:w;

	m_itemScrollArea.fLeaveTransparency=m_fScrollBarLeaveTransparency;//0.3;
	m_itemScrollArea.fEnterTransparency=m_fScrollBarEnterTransparency;//0.8;

	if (m_itemScrollArea.hAnit)
	{
		yiAnitClear(m_itemScrollArea.hAnit);
	}
	m_itemScrollArea.fTransparency=m_itemScrollArea.fLeaveTransparency;


	xSET_ITEM_TYPE((&m_itemScrollArea), eSPT_SCROLLBAR);
	xSET_ITEM_ANIMATION_NOTIFY_ONLY_ON((&(m_itemScrollArea)));
	xSET_xIS_ITEM_ABS_COORD_ON((&m_itemScrollArea));


	m_itemScrollArea.id=-1;


	cglsResourceBitmap* resource = (cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
	
	if (m_bIsVertical)
	{
		sGlsButtonDetail detail;
		memset(&detail, 0, sizeof(detail));
		detail.colorHilight=Color::LightBlue;
		detail.edge=2.0;
		int ArrowSize=w*0.6;
		for (int i=0; i<3; i++)
		{
			if (m_itemScrollBarPrev.pBmp[i])
				delete m_itemScrollBarPrev.pBmp[i];

			if (m_itemScrollBarNext.pBmp[i])
				delete m_itemScrollBarNext.pBmp[i];

			Bitmap* bmpArrow=resource->GetBitmap(eGRID_ARROW_UP, __COLORp__(255, 0, 0, 0), ArrowSize, ArrowSize);
			m_itemScrollBarPrev.pBmp[i]=resource->Draw(eGDID_GLASS_BTN_PANNEL, i, 0, w, bmpArrow, 0, &detail);
			
			bmpArrow=resource->GetBitmap(eGRID_ARROW_DOWN, __COLORp__(255, 0, 0, 0), ArrowSize, ArrowSize);
			//m_itemScrollBarNext.pBmp[i]=resource->GetBitmap(eGRID_SCROLL_DOWN, i, w, w);
			m_itemScrollBarNext.pBmp[i]=resource->Draw(eGDID_GLASS_BTN_PANNEL, i, 0, w, bmpArrow, 0, &detail);
		}

		w = m_itemScrollBarPrev.pBmp[2]->GetHeight();

		m_itemScrollBarPrev.rect.x=0;
		m_itemScrollBarPrev.rect.y=0;
		m_itemScrollBarPrev.rect.w=m_itemScrollBarPrev.pBmp[2]->GetWidth();
		m_itemScrollBarPrev.rect.h=m_itemScrollBarPrev.pBmp[2]->GetHeight();
		m_itemScrollBarNext.rect.x=0;
		m_itemScrollBarNext.rect.y=m_itemScrollArea.rect.y+m_itemScrollArea.rect.h-w;
		m_itemScrollBarNext.rect.w=m_itemScrollBarNext.pBmp[2]->GetWidth();
		m_itemScrollBarNext.rect.h=m_itemScrollBarNext.pBmp[2]->GetHeight();

	}
	else
	{
		for (int i=0; i<3; i++)
		{
			m_itemScrollBarNext.pBmp[i]=resource->GetBitmap(eGRID_SCROLL_UP, i, w, w);
			m_itemScrollBarNext.pBmp[i]=resource->GetBitmap(eGRID_SCROLL_DOWN, i, w, w);
		}
		m_itemScrollBarNext.rect.x=m_itemScrollArea.rect.w-w;
		m_itemScrollBarNext.rect.y=0;
		m_itemScrollBarNext.rect.w=w;
		m_itemScrollBarNext.rect.h=w;
		m_itemScrollBarPrev.rect.x=0;
		m_itemScrollBarPrev.rect.y=0;
		m_itemScrollBarPrev.rect.w=w;
		m_itemScrollBarPrev.rect.h=w;
	}

	if (m_bIsVertical)
	{
		if ((m_w-w-1)<0)
		{
			ReSize(w+1, m_h);
		}
	}
	// ビットマップサイズに合わせて修正
	m_itemScrollArea.rect.x=m_bIsVertical?(m_w-w-1):1;
	m_itemScrollArea.rect.y=m_bIsVertical?1:(m_h-w-1);
	m_itemScrollArea.rect.w=m_bIsVertical?w:(m_w-2);
	m_itemScrollArea.rect.h=m_bIsVertical?m_h-2:w;

	int nSystemOffsetX = m_bIsVertical?0:m_rectSystem.right;
	int nSystemOffsetY = m_bIsVertical?m_rectSystem.bottom:0;

	m_itemScrollArea.rect.x+=nSystemOffsetX;
	m_itemScrollArea.rect.y+=nSystemOffsetY;
	m_itemScrollArea.rect.h-=nSystemOffsetY;
	m_itemScrollArea.rect.w-=nSystemOffsetX;


	if(m_itemScrollBarPage.pBmp[1])
		delete m_itemScrollBarPage.pBmp[1];
	m_itemScrollBarPage.pBmp[1]=resource->Draw(eDID_SCROLL_PAGE, 0, w, w, 0, 0);
	m_itemScrollBarPage.rect.w=w;
	m_itemScrollBarPage.rect.h=w;
	
	m_scrollInfo.m_nScrollBarWidth=w;
	m_scrollInfo.m_nScrollBarStartPos=m_bIsVertical?m_itemScrollBarPrev.rect.h+1:m_itemScrollBarPrev.rect.w+1;
	m_scrollInfo.m_nScrollBarRange=m_bIsVertical?m_itemScrollBarNext.rect.y-1:m_itemScrollBarNext.rect.x-1;
	m_scrollInfo.m_nScrollBarRange-=m_scrollInfo.m_nScrollBarStartPos;
	m_scrollInfo.m_nScrollBarLength=m_scrollInfo.m_nScrollBarRange;

	//SetScrollRange(0, 1, 1, True);

	m_itemScrollBar.id=-2;

	if (m_bIsVertical)
	{
		m_itemScrollBar.rect.x=0;
		m_itemScrollBar.rect.y=m_itemScrollBarPrev.rect.h+1;
	}
	else
	{
		m_itemScrollBar.rect.x=m_itemScrollBarPrev.rect.w+1;
		m_itemScrollBar.rect.y=0;
	}

	// 一応、設定しておく
	xSET_ITEM_TYPE((&m_itemScrollBar), eSPT_SCROLLBAR);
	xSET_ITEM_ANIMATION_NOTIFY_ONLY_ON((&(m_itemScrollBar)));
	xSET_xIS_ITEM_ABS_COORD_ON((&m_itemScrollBar));
	

	if (bIsRedrawOnly==False)
	{
		//m_listFreeItem.insertref(0, &m_itemScrollBar);
		m_listFreeItem.insertref(0, &m_itemScrollArea);
	}

	return True;
}

// ------------------------------------
Bool cglsStackPannel::OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX, int offsetY)
{
	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_USE_GAUSS_PANNEL)==True &&
			m_item.pCurrentBmp==0)
	{
		return cglsGaussPannel::OnDrawMyself(bmp, rect, childRect);
	}
	return False;
}

// ---------------------------------------
Bool cglsStackPannel::SetScrollPos(int pos)
{
	int range = m_scrollInfo.max-m_scrollInfo.min;
	int nRealRange = range-m_scrollInfo.page;

	if (pos>m_scrollInfo.max)
		pos=m_scrollInfo.max;
	if (pos<m_scrollInfo.min)
		pos=m_scrollInfo.min;

	if (range==0)
	{
		return False;
	}

	if (pos < 0)
		pos=0;

	if (pos > nRealRange)
		pos=nRealRange;

	if (pos==m_scrollInfo.pos)
	{
		//return False;
	}
	m_scrollInfo.pos=pos;

	float rate = (float)pos/(float)nRealRange;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)==True)
		m_itemScrollBar.rect.y=(int)(rate*(float)(m_scrollInfo.m_nScrollBarRange-m_scrollInfo.m_nScrollBarLength))+m_scrollInfo.m_nScrollBarStartPos;//m_itemScrollBarPrev.rect.h+1;
	else
		m_itemScrollBar.rect.x=(int)(rate*(float)(m_scrollInfo.m_nScrollBarRange-m_scrollInfo.m_nScrollBarLength))+m_scrollInfo.m_nScrollBarStartPos;

	return True;
}



// ---------------------------------------
Bool cglsStackPannel::SetScrollRange(int min, int max, int pagesize, Bool bIsForced)
{
	cglsResourceBitmap* resource = (cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
	int range=max-min;
	float rate;
	int length;

	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	int nSystemOffsetX = m_bIsVertical?0:m_rectSystem.right;
	int nSystemOffsetY = m_bIsVertical?m_rectSystem.bottom:0;
	int clen=m_bIsVertical?m_h:m_w;

	if (range==0)
	{
		xSET_ITEM_HIDE((&(m_itemScrollArea)));
		return False;
	}

	if (range<=clen)
	{
		xSET_ITEM_HIDE((&(m_itemScrollArea)));
		return False;
	}

	if (bIsForced==True)
	{
		if (xCreateScrollBar()==False)
		{
			xSET_ITEM_HIDE((&(m_itemScrollArea)));
			return False;
		}
	}
	else if (m_itemScrollArea.rect.w==0 &&
			m_itemScrollArea.rect.h==0)
	{
		if (xCreateScrollBar()==False)
		{
			xSET_ITEM_HIDE((&(m_itemScrollArea)));
			return False;
		}
	}

	// ここまで来たらShow

	xSET_ITEM_SHOW((&(m_itemScrollArea)));

	// サイズなどの変更があった場合を含めて。。。
	m_itemScrollArea.rect.x=m_bIsVertical?(m_w-m_scrollInfo.m_nScrollBarWidth-1):1;
	m_itemScrollArea.rect.y=m_bIsVertical?1:(m_h-m_scrollInfo.m_nScrollBarWidth-1);

	if (m_bIsVertical)
	{
		m_itemScrollArea.rect.h=m_h-2;
		m_itemScrollArea.rect.w=m_scrollInfo.m_nScrollBarWidth;
	}
	else
	{
		m_itemScrollArea.rect.h=m_scrollInfo.m_nScrollBarWidth;
		m_itemScrollArea.rect.w=m_w-2;
	}

	m_itemScrollArea.rect.x+=nSystemOffsetX;
	m_itemScrollArea.rect.y+=nSystemOffsetY;
	m_itemScrollArea.rect.h-=nSystemOffsetY;
	m_itemScrollArea.rect.w-=nSystemOffsetX;

	if (m_bIsVertical)
	{
		m_itemScrollBarNext.rect.y=m_itemScrollArea.rect.h-m_itemScrollBarNext.rect.h-1;
	}
	else
	{
		m_itemScrollBarNext.rect.x=m_itemScrollArea.rect.w-m_itemScrollBarNext.rect.w-1;
	}

	if (m_scrollInfo.min==min &&
		m_scrollInfo.max==max)
	{
		if (pagesize==-1)
		{
			return False;
		}
		
		if (m_scrollInfo.page==pagesize)
		{
			return False;
		}
	}

//SKIP_CHECK:
	m_scrollInfo.m_nScrollBarRange=m_bIsVertical?m_itemScrollBarNext.rect.y-1:m_itemScrollBarNext.rect.x-1;
	m_scrollInfo.m_nScrollBarRange-=(m_scrollInfo.m_nScrollBarStartPos);//+(m_bIsVertical?m_itemScrollBar.rect.h:m_itemScrollBar.rect.w));

	m_scrollInfo.min=min;
	m_scrollInfo.max=max;

	if (pagesize!=-1)
		m_scrollInfo.page=pagesize;

	rate = (float)m_scrollInfo.page/(float)range;
	rate=rate>1.0? 1.0: rate;

	length = (int)((float)m_scrollInfo.m_nScrollBarRange*rate);
	length =(length<m_scrollInfo.m_nScrollBarWidth)?m_scrollInfo.m_nScrollBarWidth:length;

	//m_scrollInfo.m_nScrollBarRange-=length;

	if (bIsForced)
		goto SKIP_CHECK2;

	if (length==m_scrollInfo.m_nScrollBarLength)
	{
		//xSET_ITEM_HIDE((&(m_itemScrollArea)));
		return False;
	}

SKIP_CHECK2:

	m_scrollInfo.m_nScrollBarLength=length;


	if (m_scrollInfo.m_nScrollBarLength>=m_scrollInfo.m_nScrollBarRange)
	{
		xSET_ITEM_HIDE((&m_itemScrollArea));
	}
	else
	{
		xSET_ITEM_SHOW((&m_itemScrollArea));
	}

	sGlsButtonDetail detail;
	memset(&detail, 0, sizeof(detail));
	detail.edge=5.0;
	for (int i=0; i<2; i++)
	{
		if (m_itemScrollBar.pBmp[i])
			delete m_itemScrollBar.pBmp[i];
		if (m_bIsVertical)
		{
			detail.rect.right=m_scrollInfo.m_nScrollBarWidth;
			detail.rect.bottom=m_scrollInfo.m_nScrollBarLength;
			detail.colorHilight=Color::LightBlue;
			m_itemScrollBar.pBmp[i] = resource->Draw(eGDID_GLASS_BTN_PANNEL, i, 0, 0, 0, 0, &detail);
			//m_itemScrollBar.pBmp[i]=resource->Draw(eDID_SCROLL_BAR, i, m_scrollInfo.m_nScrollBarWidth, m_scrollInfo.m_nScrollBarLength, 0, 0);
			//m_itemScrollBar.pBmp[i]=resource->Draw(eDID_SCROLL_BAR, i, m_scrollInfo.m_nScrollBarWidth, m_scrollInfo.m_nScrollBarLength, 0, 0);
		}
		else
		{
			m_itemScrollBar.pBmp[i]=resource->Draw(eDID_SCROLL_BAR, i, m_scrollInfo.m_nScrollBarLength, m_scrollInfo.m_nScrollBarWidth, 0, 0);
			//m_itemScrollBar.pBmp[i]=resource->Draw(eDID_SCROLL_BAR, i, m_scrollInfo.m_nScrollBarLength, m_scrollInfo.m_nScrollBarWidth, 0, 0);
		}
	}
	m_itemScrollBar.pBmp[2]=m_itemScrollBar.pBmp[1];
	//m_itemScrollBar.pCurrentBmp=m_itemScrollBar.pBmp[0];

	m_itemScrollBar.rect.w=m_itemScrollBar.pBmp[0]->GetWidth();
	m_itemScrollBar.rect.h=m_itemScrollBar.pBmp[0]->GetHeight();

	if (m_itemScrollBar.pCurrentBmp)
		delete m_itemScrollBar.pCurrentBmp;

	m_itemScrollBar.pCurrentBmp=cyiBitmapBlender::Transparent(m_itemScrollBar.pBmp[0], m_itemScrollArea.fLeaveTransparency);

	SetScrollPos(0);

	//for (int i=0; i<2; i++)
	//{
	//	if (m_itemScrollArea.pBmp[i])
	//		delete m_itemScrollArea.pBmp[i];

	//	m_itemScrollArea.pBmp[i]=BITMAP32(m_itemScrollArea.rect.w, m_itemScrollArea.rect.h);
	//	Graphics g(m_itemScrollArea.pBmp[i]);

	//	g.DrawImage(m_itemScrollBarPrev.pBmp[i], m_itemScrollBarPrev.rect.x, m_itemScrollBarPrev.rect.y, 
	//		0, 0, m_itemScrollBarPrev.rect.w, m_itemScrollBarPrev.rect.h, UnitPixel);

	//	g.DrawImage(m_itemScrollBarPrev.pBmp[i], m_itemScrollBarPrev.rect.x, m_itemScrollBarPrev.rect.y, 
	//		0, 0, m_itemScrollBarPrev.rect.w, m_itemScrollBarPrev.rect.h, UnitPixel);
	//}

	//xMakeScrollBarImage();

	xScrollBarUpdate(0, 0, True);

	return True;
}


// ---------------------------------------------
int cglsStackPannel::xGetScrollParts(int x, int y)
{
	RECT all = {0, 0, m_itemScrollArea.rect.w, m_itemScrollArea.rect.h};
	if (all.left > x ||
			all.left+all.right<x ||
				all.top > y ||
					all.top+all.bottom < y)
	{
		return -1;
	}

	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	if (xIsInRect(&m_itemScrollBarPrev.rect, x, y)==True)
	{
		return 0;
	}

	if (xIsInRect(&m_itemScrollBar.rect, x, y)==True)
	{
		return 1;
	}
	if (xIsInRect(&m_itemScrollBarNext.rect, x, y)==True)
	{
		return 2;
	}

	if (m_bIsVertical)
	{
		if (m_itemScrollBar.rect.y > y)
		{
			return 4;
		}
		else if (m_itemScrollBar.rect.y+m_itemScrollBar.rect.h<y)
		{
			return 5;
		}
	}
	else
	{
		if (m_itemScrollBar.rect.x > x)
		{
			return 4;
		}
		else if (m_itemScrollBar.rect.x+m_itemScrollBar.rect.w<x)
		{
			return 5;
		}
	}

	return -1;
}

// ---------------------------------------------
//LRESULT cglsStackPannel::OnScrollBarMouseUp(WPARAM wParam, LPARAM lParam)
//{
//	int mousex=lParam&0xFFFF;
//	int mousey=(lParam>>16)&0xFFFF;
//	int rex=mousex-m_itemScrollArea.rect.x;
//	int rey=mousey-m_itemScrollArea.rect.y;
//
//
//	Update();
//	return 0;
//}


// ---------------------------------------------
// Mouse Enter.
LRESULT cglsStackPannel::xOnMouseMoveScrollBar(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	sDrawItem* itemz[] = {&m_itemScrollBarPrev, &m_itemScrollBar, &m_itemScrollBarNext, &m_itemScrollArea, &m_itemScrollBarPage, &m_itemScrollBarPage};

	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	pt.x-=m_itemScrollArea.rect.x;
	pt.y-=m_itemScrollArea.rect.y;
	
	int nSelected = xGetScrollParts(pt.x, pt.y);


	if (nSelected==-1)
		return 0;

	// 何もしない。
	if (m_eScrollBarState==eSPSBS_MOUSE_ENTER &&
			m_nSelectedScrollBarParts==nSelected)
	{
		return 0;
	}

	m_nSelectedScrollBarParts=nSelected;
	m_eScrollBarState=eSPSBS_MOUSE_ENTER;

	m_fScrollLastFactor=m_itemScrollArea.fTransparency;
	//m_itemScrollArea.animation->fAnimationRate=0.0;

	sGlsAnimationEx ani;
	glsInitAnimationEx(&ani, &m_itemScrollArea);
	glsSetTrAnimationEx(&ani, m_itemScrollArea.fLeaveTransparency, m_itemScrollArea.fEnterTransparency, eGAP_FISO);
	//glsSetDvAnimationEx(&ani, 0, 1, eGAP_FISO);
	StartAnimation(&m_itemScrollArea, &ani, 20, 500);

	return 0;
}


// ---------------------------------------------
LRESULT cglsStackPannel::xOnMouseLeaveScrollBar(WPARAM wParam, LPARAM lParam)
{
	//xTRACE("xDrawScrollBar(): xOnMouseLeaveScrollBar:called");

	if (m_eScrollBarState!=eSPSBS_MOUSE_LEAVE)
	{
		sGlsAnimationEx ani;
		glsInitAnimationEx(&ani, &m_itemScrollArea);
		glsSetTrAnimationEx(&ani, m_itemScrollArea.fEnterTransparency, m_itemScrollArea.fLeaveTransparency, eGAP_SIFO);
		glsSetDvAnimationEx(&ani, 1, 0, eGAP_SIFO);

		StartAnimation(&m_itemScrollArea, &ani, 20, 500);

		//xTRACE("xDrawScrollBar(): xOnMouseLeaveScrollBar: start leave animation");
		//m_itemScrollArea.animation->fAnimationRate=0.0;
		//m_fScrollLastFactor=m_itemScrollArea.fTransparency;
		//Update(&m_itemScrollArea, eDIS_MOUSE_LEAVE);
	}
	m_eScrollBarState = eSPSBS_MOUSE_LEAVE;
	m_nSelectedScrollBarParts=-1;
	return 0;
}

// ---------------------------------------------
Bool cglsStackPannel::xOnLButtonDownScrollBar(int mousex, int mousey)// wParam, LPARAM lParam)
{
	//int mousex=lParam&0xFFFF;
	//int mousey=(lParam>>16)&0xFFFF;
	int rex=mousex-m_itemScrollArea.rect.x;
	int rey=mousey-m_itemScrollArea.rect.y;
	sDrawItem* item=0;
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);
	UINT msg = m_bIsVertical?WM_VSCROLL:WM_HSCROLL;
	int nParts;
	int nOneElement = (float)(m_scrollInfo.max-m_scrollInfo.min)/(float)m_listItem->count();

	nParts=m_nSelectedScrollBarParts=xGetScrollParts(rex, rey);

	if (this->m_itemScrollArea.hAnit)
	{
		if (yiAnitIsAlived(m_itemScrollArea.hAnit)==True)
		{
			yiAnitStop(m_itemScrollArea.hAnit, True);
		}
		yiAnitClear(m_itemScrollArea.hAnit);

		m_itemScrollArea.fTransparency = m_itemScrollArea.fEnterTransparency;
		
	}

	//m_nLastSelectedParts=nParts;
	if (nParts==0)
	{
		item=&m_itemScrollBarPrev;
		if (m_hScrollNotifyWnd!=0 && 
				SendMessage(m_hScrollNotifyWnd, msg, SB_LINEUP, (LPARAM)this)!=0)
		{
			InvalidateRect(0);
			return False;
		}

		int mod = 0;
		mod=m_scrollInfo.pos%nOneElement;
		Scroll(-nOneElement-mod, 300);
	}
	else if (nParts==1)
	{
		Bool handled=True;

		//m_bNowDrag=True;
		//m_ptDragOn.x=mousex;
		//m_ptDragOn.y=mousey;

		m_itemMoveOrgX=m_itemScrollBar.rect.x+m_itemScrollArea.rect.x;
		m_itemMoveOrgY=m_itemScrollBar.rect.y+m_itemScrollArea.rect.y;
		m_itemDraged=&m_itemScrollBar;

		StartDrag(mousex, mousey);

		//SetCapture(m_hWnd);
		return True;//xOnScrollBarDragStart(mousex, mousey, handled);
		//return OnDragStart(mousex, mousey, handled);
	}
	else if (nParts==2)
	{
		item=&m_itemScrollBarPrev;
		if (m_hScrollNotifyWnd!=0 && 
			SendMessage(m_hScrollNotifyWnd, msg, SB_LINEDOWN,(LPARAM)this)!=0)
		{
			InvalidateRect(0);
			return False;
		}
		int mod=m_scrollInfo.pos%nOneElement;

		if (mod)
			Scroll(nOneElement+(nOneElement-mod), 300);
		else
			Scroll(nOneElement, 300);

	}
	else if (nParts==4) // PageUP
	{
		if (m_hScrollNotifyWnd!=0 && 
			SendMessage(m_hScrollNotifyWnd, msg, SB_PAGEUP,(LPARAM)this)!=0)
		{
			InvalidateRect(0);
			return False;
		}

		item=&m_itemScrollBarPage;

		if (m_bIsVertical)
			item->rect.y=mousey;
		else
			item->rect.x=mousex;
		int mod=m_scrollInfo.pos%nOneElement;

		Scroll(-m_scrollInfo.page-mod, 500);//, -nOneElement, 1000);
	}
	else if (nParts==5) // PageDown
	{
		if (m_hScrollNotifyWnd!=0 && 
			SendMessage(m_hScrollNotifyWnd, msg, SB_PAGEDOWN,(LPARAM)this)!=0)
		{
			InvalidateRect(0);
			return False;
		}

		item=&m_itemScrollBarPage;
		if (m_bIsVertical)
			item->rect.y=mousey;
		else
			item->rect.x=mousex;

		int mod=m_scrollInfo.pos%nOneElement;

		if (mod)
			Scroll(m_scrollInfo.page+(nOneElement-mod), 500);//nOneElement, 1000);
		else
			Scroll(m_scrollInfo.page, 500);// nOneElement, 1000);


	}

	//if (nParts!=-1)
	//{
	//	item->pCurrentBmp=item->pBmp[1];
	//}

	return 0;

}



// ------------------------------------
LRESULT cglsStackPannel::xOnScrollBarDragStart(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	m_itemDraged=&m_itemScrollBar;
	m_itemMoveOrgX=m_itemScrollBar.rect.x;//item->rect.x;
	m_itemMoveOrgY=m_itemScrollBar.rect.y;//item->rect.y;

	m_eScrollBarState=eSPSBS_DRAG;

	if (m_itemScrollBar.pCurrentBmp)
		delete m_itemScrollBar.pCurrentBmp;

	m_itemScrollBar.pCurrentBmp=cyiBitmapBlender::BitmapCopy(0, m_itemScrollBar.pBmp[2]);
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);
	UINT msg = m_bIsVertical?WM_VSCROLL:WM_HSCROLL;
	if (SendMessage(m_hNotifyWnd, msg, SB_THUMBPOSITION, (LPARAM)this)!=0)
	{
		//InvalidateRect(0);
		return 1;
	}

	
	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False)
	{
		if (m_itemScrollArea.param)
			delete (Bitmap*)m_itemScrollArea.param;

		m_itemScrollArea.param=(void*)GetParentBg(0, 0, 0);
	}

	return 1;
}

// ------------------------------------
LRESULT cglsStackPannel::xOnScrollBarDrag(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int x,y;
	int newPos;
	int range=m_scrollInfo.max-m_scrollInfo.min;

	m_eScrollBarState=eSPSBS_DRAG;

	handled=True;
	GetDragStartPos(x, y);
	sDrawItem* item = m_itemDraged;//xFindItem(x, y, &m_listFreeItem);

	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);
	newPos = m_bIsVertical?m_itemMoveOrgY+((int)lParam-y):m_itemMoveOrgX+((int)wParam-x);
	
	if (newPos<m_scrollInfo.m_nScrollBarStartPos)
	{
		newPos=m_scrollInfo.m_nScrollBarStartPos;
		m_scrollInfo.pos=0;
	}
	else if (newPos>(m_scrollInfo.m_nScrollBarRange+m_scrollInfo.m_nScrollBarStartPos-m_scrollInfo.m_nScrollBarLength))
	{
		newPos=m_scrollInfo.m_nScrollBarRange+m_scrollInfo.m_nScrollBarStartPos-m_scrollInfo.m_nScrollBarLength;
		m_scrollInfo.pos=m_scrollInfo.max-m_scrollInfo.page;
	}
	else
	{
		float rate = (float)(newPos-m_scrollInfo.m_nScrollBarStartPos)/(float)m_scrollInfo.m_nScrollBarRange;
		int pos = (int)(rate*(float)range);

		m_scrollInfo.pos=pos;
	}

	if (m_bIsVertical)
		m_itemScrollBar.rect.y=newPos;
	else
		m_itemScrollBar.rect.x=newPos;
	//SetScrollPos(pos);

	UINT msg = m_bIsVertical?WM_VSCROLL:WM_HSCROLL;
	if (m_hScrollNotifyWnd!=0 &&
			SendMessage(m_hScrollNotifyWnd, msg, SB_THUMBPOSITION, (LPARAM)this)!=0)
	{
		InvalidateRect(0);
		return 1;
	}

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False)
	{
		Bitmap* ret = cyiBitmapBlender::BitmapCopy(0, (Bitmap*)m_itemScrollArea.param);

		int x=0, y=0;

		m_nScrollPos=m_scrollInfo.pos;
		GetScrollPos(x,y);
		Graphics g(ret);
		g.DrawImage(m_bmpCurrent, 0, 0, x, y, m_w, m_h, UnitPixel);
		xDrawScrollBar(&g, 0, True);

		DirectDraw(ret);
		delete ret;
	}
	else
	{
		Scroll(m_scrollInfo.pos, 0);
	}
	

	return 1;
}

// ------------------------------------
LRESULT cglsStackPannel::xOnScrollBarDragEnd(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int x=wParam;
	int y=lParam;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);


	xOnScrollBarDrag(wParam, lParam, handled);

	if (xIsInRect(&m_itemScrollArea.rect, pt.x, pt.y)==True)
	{
		
		m_eScrollBarState=eSPSBS_MOUSE_ENTER;
	}
	else
	{
		m_eScrollBarState=eSPSBS_MOUSE_LEAVE;
		Update(&m_itemScrollArea);
	}
	return 0;
}


// ---------------------------------------------
void cglsStackPannel::xScrollBarUpdate(int xPos, int yPos, Bool bIsAllRange)
{
	bIsAllRange=True;

	if (bIsAllRange==True)
	{
		RECT rect = {m_itemScrollArea.rect.x, m_itemScrollArea.rect.y, m_itemScrollArea.rect.w+m_itemScrollArea.rect.x, m_itemScrollArea.rect.h+m_itemScrollArea.rect.y};
		InvalidateRect(&rect);
		return ;
	}

	//int n = xGetScrollParts();
	//yiRect* pRect;

	//if (n==0)
	//{
	//	pRect = &m_itemScrollBarPrev.rect;
	//}
	//else if (n==1)
	//{
	//	pRect = &m_itemScrollBar.rect;
	//}

}

// ---------------------------------------------
Bool cglsStackPannel::xSetScrollItemStatus(int nIndex, bit32_t* mask, float factor)
{
	*mask=0;
	sDrawItem* item[] = {&m_itemScrollBarPrev, &m_itemScrollBar, &m_itemScrollBarNext, &m_itemScrollArea, &m_itemScrollBarPage, &m_itemScrollBarPage};
	// PrevButton時


	// 基本的に3番目に変化があれば、透明度をかえる。
	// 変化がなければ買えない。

	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);
	if (m_nBeforeScrollPos!=(m_bIsVertical?item[1]->rect.y:item[1]->rect.x))
	{
		*mask=(1<<1);
		m_nBeforeScrollPos=(m_bIsVertical?item[1]->rect.y:item[1]->rect.x);
	}

	// mouse leave
	if (nIndex==-1)
	{
		for (int i=0; i<3; i++)
		{
			eDrawItemStatus status = xGET_ITEM_STATUS(item[i]);

			if (status==eDIS_NORMAL)
				continue;

			if (factor==1.0 ||
					factor==-1.0)
			{
				xSET_ITEM_STATUS(item[i], eDIS_NORMAL);
			}
			*mask|=(1<<i);
		}
		if (factor==1.0 ||
				factor==-1.0)
		{
			xSET_ITEM_STATUS(item[3], eDIS_NORMAL);
		}

		if (item[3]->fTransparency!=item[3]->fLeaveTransparency)//xGET_ITEM_STATUS(item[3])!=eDIS_NORMAL)
		{
			*mask|=(1<<3);
		}

		if (*mask)
			return True;

		if (factor!=1.0)
		{
			return True;
		}
		return False;
	}

	// mouse down 状態時
	if (m_bPushed)
	{
		eDrawItemStatus status = xGET_ITEM_STATUS(item[nIndex]);
		*mask|=(1<<nIndex);
		if (factor==1.0 ||
				factor==-1.0)
		{
			xSET_ITEM_STATUS(item[3], eDIS_MOUSE_DOWN);
		}
		return True;
	}
	// mouse move 状態時
	else
	{
		eDrawItemStatus status = xGET_ITEM_STATUS(item[3]);

		if (item[3]->fTransparency!=item[3]->fEnterTransparency)//status!=eDIS_MOUSE_MOVE)
		{
			if (factor==1.0 ||
					factor==-1.0)
			{
				xSET_ITEM_STATUS(item[3], eDIS_MOUSE_MOVE);
			}

			*mask=(1<<3);//0x0F;
			return True;
		}


		//status = xGET_ITEM_STATUS(item[nIndex]);

		// あり得る？
		//if (status==eDIS_MOUSE_MOVE)
		//{
		//	xTRACE("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
		//	*mask=0;
		//	return False;
		//}

		for (int i=0; i<3; i++)
		{
			status = xGET_ITEM_STATUS(item[nIndex]);

			if (status==eDIS_MOUSE_MOVE)
			{
				*mask|=(1<<i);
				if (factor==1.0 ||
						factor==-1.0)
				{
					xSET_ITEM_STATUS(item[i], eDIS_NORMAL);
				}
			}
		}

		*mask|=(1<<nIndex);
		if (factor==1.0 ||
				factor==-1.0)
		{
			xSET_ITEM_STATUS(item[nIndex], eDIS_MOUSE_MOVE);
		}
	}

	if (*mask)
		return True;

	if (factor!=1.0)
	{
		return True;
	}


	return True;
}


// ---------------------------------------------
//void cglsStackPannel::xDestroyScrollBar()
//{
//	// TODO.
//	return ;
//}

// ---------------------------------------------
void cglsStackPannel::xDrawScrollBar(Graphics* g, RECT* clipRect, Bool bIsAbsCoord/*=False*/)
{
	// 描画しない
	if (xIS_ITEM_HIDE((&m_itemScrollArea)))
	{
		return ;
	}

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	Bool bIsAnimationStopped=False;
	
	int x=m_itemScrollArea.rect.x;
	int y=m_itemScrollArea.rect.y;

	int sw=0, sh=0;
	float fAbortFactor=0.0;

	if (m_itemScrollArea.animation)
	{
		if (m_itemScrollArea.hAnit==0)
		{
			m_itemScrollArea.hAnit = ::yiAnitCreate();
		}

		if (yiAnitIsCleared(m_itemScrollArea.hAnit, &fAbortFactor)==False)
		{
			bIsAnimationStopped=True;
		}
	}

	if (clipRect)
	{
		x=clipRect->left;
		y=clipRect->top;
	}

	if (bIsAbsCoord==False &&
			xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL)==False)
	{
		GetScrollPos(sw, sh);
	}

	x+=sw;
	y+=sh;

	pt.x-=m_itemScrollArea.rect.x;
	pt.y-=m_itemScrollArea.rect.y;
	int nIndex = this->m_nSelectedScrollBarParts;//xGetScrollParts(pt.x, pt.y);


	//xTRACE("xDrawScrollBar(): m_nSelectedScrollBarParts:%d, animation rate:%f, from:%f, to:%f, t:%f", nIndex, factor, fTraFrom, fTraTo, m_itemScrollArea.fTransparency);

	//if (nIndex==-1 &&
	//	this->m_eScrollBarState!=eSPSBS_MOUSE_LEAVE)
	//{
	//	nIndex
	//}

	//xTRACE("cglsStackPannel::xDrawScrollBar() index:%d", nIndex);
	bit32_t mask;
	sDrawItem* itemz[] = {&m_itemScrollBarPrev, &m_itemScrollBar, &m_itemScrollBarNext, &m_itemScrollArea, &m_itemScrollBarPage, &m_itemScrollBarPage};

	if (m_itemScrollBar.pBmp[0]==0)
		return ;

	for (int i=0; i<5; i++)
	{
		if (itemz[i]->animation==0)
			itemz[i]->animation = (sGlsAnimationEx*)yialloci(sizeof(sGlsAnimationEx));
	}

	// 変化なし時
	//if (xSetScrollItemStatus(nIndex, &mask, bIsAnimationStopped?1.0:factor)==False)
	//{
	//	//xTRACE("cglsStackPannel::xDrawScrollBar() no changed!! %x, %f, index:%d", mask, factor, nIndex);
	//	// same:
	//	for (int i=0; i<3; i++)
	//	{
	//		g->DrawImage(itemz[i]->pCurrentBmp, itemz[i]->rect.x+x, itemz[i]->rect.y+y, 0, 0, 
	//			itemz[i]->rect.w, itemz[i]->rect.h, UnitPixel);
	//	}

	//	if (m_itemScrollBarPage.pCurrentBmp)
	//	{
	//		g->DrawImage(m_itemScrollBarPage.pCurrentBmp, m_itemScrollBarPage.rect.x+x, m_itemScrollBarPage.rect.y+y, 
	//			0, 0, m_itemScrollBarPage.rect.w, m_itemScrollBarPage.rect.h, UnitPixel);
	//	}
	//}
	// 変化がある場合
	/*else*/
	{
		//xTRACE("cglsStackPannel::xDrawScrollBar() change state !! %x, %f, index:%d", mask, factor, nIndex);

		// この場合はアニメーションしなのと一か所だけ描画をする？
		if (m_bPushed && nIndex!=-1)
		{
			//xTRACE("cglsStackPannel::xDrawScrollBar() push index:%d", nIndex);
			for (int i=0; i<3; i++)
			{
				if (i==nIndex)
					continue;
				g->DrawImage(itemz[i]->pBmp[0]/*pCurrentBmp*/, itemz[i]->rect.x+x, itemz[i]->rect.y+y, 0, 0, 
					itemz[i]->rect.w, itemz[i]->rect.h, UnitPixel);
			}

			if (nIndex>3)
			{
				int x=xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?itemz[3]->rect.x:pt.x+itemz[3]->rect.x;
				int y=xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?pt.y+itemz[3]->rect.y:itemz[3]->rect.y;
				g->DrawImage(itemz[nIndex]->pBmp[1], x, y, //pt.x+itemz[3]->rect.x, pt.y+itemz[3]->rect.y, 
					0, 0, itemz[nIndex]->rect.w, itemz[nIndex]->rect.h, UnitPixel);
				
				if (itemz[nIndex]->pCurrentBmp)
					delete itemz[nIndex]->pCurrentBmp;
				itemz[nIndex]->pCurrentBmp=cyiBitmapBlender::BitmapCopy(0, itemz[nIndex]->pBmp[1]);
			}
			else
			{
				Bitmap* bmpPushed = cyiBitmapBlender::Transparent(itemz[nIndex]->pBmp[1], itemz[3]->fTransparency);
				itemz[nIndex]->nLastItemIndex=1;

				g->DrawImage(bmpPushed, itemz[nIndex]->rect.x+x, itemz[nIndex]->rect.y+y, 0, 0, 
					itemz[nIndex]->rect.w, itemz[nIndex]->rect.h, UnitPixel);

				if (itemz[nIndex]->pCurrentBmp)
					delete itemz[nIndex]->pCurrentBmp;

				//itemz[nIndex]->pCurrentBmp=cyiBitmapBlender::BitmapCopy(0, itemz[nIndex]->pBmp[1]);

				itemz[nIndex]->pCurrentBmp= bmpPushed;

				itemz[nIndex]->animation->pBmpAnimation= cyiBitmapBlender::BitmapCopy(0, bmpPushed);

			}

			return ;

		}
		else if (nIndex==-1) // Mouse Leave時
		{
			float f = m_itemScrollArea.fTransparency;//(1.0-factor)*(fTraFrom)/*itemz[3]->fTransparency*/+(factor)*itemz[3]->fLeaveTransparency;
			f = (fAbortFactor==1.0)?itemz[3]->fLeaveTransparency:f;

			itemz[3]->fTransparency=f;
			if (fAbortFactor!=1.0)
			{
				m_fScrollLastFactor=f;
			}

			//m_itemScrollArea.fTransparency=f;

			//xTRACE("xDrawScrollBar() fTransparency:%f, factor:%f, flag:%x, nIndex:%d f:%f", itemz[3]->fTransparency, factor, mask, nIndex, f);
			for (int i=0; i<3; i++)
			{
				Bitmap* p = cyiBitmapBlender::Transparent(itemz[i]->pBmp[0], f);
				g->DrawImage(p, itemz[i]->rect.x+x, itemz[i]->rect.y+y, 0, 0, 
					itemz[i]->rect.w, itemz[i]->rect.h, UnitPixel);

				if (fAbortFactor==0.0)
				{
					// 最後のアニメーションの場合。
					if (itemz[i]->pCurrentBmp)
						delete itemz[i]->pCurrentBmp;
					itemz[i]->pCurrentBmp=p;

				}
				else
				{
					delete p;
				}
			}

			if (fAbortFactor==1.0)
			{
				itemz[3]->fTransparency=itemz[3]->fLeaveTransparency;
			}
			if (itemz[4]->pCurrentBmp)
				delete itemz[4]->pCurrentBmp;
			itemz[4]->pCurrentBmp=0;
		}
		else // Mouse Enter時
		{
			Bool bIsTransparencyDraw=(mask&(1<<3)!=0);
			Bitmap* pBmp=0;
			float f = m_itemScrollArea.fTransparency;//(1.0-factor)*fTraFrom/*itemz[3]->fTransparency*/+(factor)*itemz[3]->fEnterTransparency;
			float factor = fAbortFactor;//==0.0?1.0:fAbortFactor;

			if (itemz[4]->pCurrentBmp)
				delete itemz[4]->pCurrentBmp;
			itemz[4]->pCurrentBmp=0;

			itemz[3]->fTransparency=f;

			if (fAbortFactor!=0.0)
			{
				m_fScrollLastFactor=f;
			}

			g->FillRectangle(&SolidBrush(Color(32, 255, 255, 255)), Rect(itemz[3]->rect.x, itemz[3]->rect.y, itemz[3]->rect.w, itemz[3]->rect.h));
			for (int i=0; i<3; i++)
			{
				Bool bIsSkipTransparent=False;
				/*
				if ((mask&(1<<i)))
				{
					if (i==nIndex)
					{
						if (itemz[i]->animation)
						{
							pBmp = cyiBitmapBlender::Desolve(itemz[i]->animation->pBmpAnimation?itemz[i]->animation->pBmpAnimation:itemz[i]->pBmp[0], itemz[i]->pBmp[1], factor);
						}
						else
						{
							pBmp=0;
						}

						if (pBmp==0)
						{
							pBmp = cyiBitmapBlender::Desolve(itemz[i]->pBmp[0], itemz[i]->pBmp[1], factor);
						}
					}
					else
					{
						if (itemz[i]->animation)
						{
							pBmp = cyiBitmapBlender::Desolve(itemz[i]->animation->pBmpAnimation?itemz[i]->animation->pBmpAnimation:itemz[i]->pBmp[1], itemz[i]->pBmp[0], factor);
						}
						else
						{
							pBmp=0;
						}

						if (pBmp==0)
						{
							pBmp = cyiBitmapBlender::Desolve(itemz[i]->pBmp[1], itemz[i]->pBmp[0], factor);
						}

					}

					//g->DrawImage(pBmp, itemz[i]->rect.x+x, itemz[i]->rect.y+y, 0, 0, 
					//	itemz[i]->rect.w, itemz[i]->rect.h, UnitPixel);
				}
				else
				{
					//pBmp=itemz[i]->pBmp[0];
					// 無駄な処理だけど
					eDrawItemStatus status = xGET_ITEM_STATUS(itemz[i]);
					pBmp=cyiBitmapBlender::BitmapCopy(0, itemz[i]->pBmp[1]);


					//if (pBmp==0)
					//{
					//	eDrawItemStatus status = xGET_ITEM_STATUS(itemz[i]);
					//	pBmp=cyiBitmapBlender::Transparent(itemz[i]->pBmp[status], itemz[3]->fTransparency);
					//}
				}
				*/
				//if (bIsSkipTransparent)
				//{
				//	g->DrawImage(pBmp, itemz[i]->rect.x+x, itemz[i]->rect.y+y, 0, 0, 
				//		itemz[nIndex]->rect.w, itemz[i]->rect.h, UnitPixel);
				//	delete pBmp;
				//}
				//else
				{
					Bitmap* p = cyiBitmapBlender::Transparent(itemz[i]->pBmp[0]/*pBmp*/, f);
					g->DrawImage(p, itemz[i]->rect.x+x, itemz[i]->rect.y+y, 0, 0, 
						itemz[i]->rect.w, itemz[i]->rect.h, UnitPixel);

					if (fAbortFactor==1.0 || 
							bIsAnimationStopped==True)
					{
						if (itemz[i]->pCurrentBmp)
						{
							delete itemz[i]->pCurrentBmp;
						}
						itemz[i]->pCurrentBmp=p;
					}
					else
					{
						delete p;
					}

					//delete pBmp;
					//pBmp=0;
				}
			} // for

			if (fAbortFactor==1.0)
			{
				itemz[3]->fTransparency=itemz[3]->fEnterTransparency;
			}
		} // if Move
	}

}

// -----------------------------
Bool cglsStackPannel::xOnItemMouseDown(sDrawItem* item)
{
	return False;
}

// -----------------------------
Bool cglsStackPannel::xOnItemMouseUp(sDrawItem* item)
{
	return False;
}

// -----------------------------
Bool cglsStackPannel::xOnItemMouseMove(sDrawItem* item)
{
	return False;
}

// -----------------------------
Bool cglsStackPannel::xOnItemMouseLeave(sDrawItem* item)
{
	return False;
}
