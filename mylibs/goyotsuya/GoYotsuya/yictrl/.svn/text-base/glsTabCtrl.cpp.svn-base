#include "glsTabCtrl.h"
#include "glsTabItem.h"



xcglsTabCtrl::xcglsTabCtrl(void)
{
	__GLASSINTERFACE_NAME__;
}

xcglsTabCtrl::~xcglsTabCtrl(void)
{
}

// ----------------------------------------------
void xcglsTabCtrl::xDefaultDetail(sGlsTabDetail* detail)
{
	cglsStackPannel::xDefaultDetail(&detail->stack);

	detail->stack.bIsFixedMode=True;
	detail->stack.bIsShowScrollBar=False;

	detail->font=0;//new Font(L"Meiryo", 10.5);
	detail->wMaxShowCount=3;
	detail->wMinShowCount=10;
	detail->wMargin=5;
	detail->hMargin=5;
	detail->brush = new SolidBrush(Color(200, 255, 255, 255));
	detail->pTopPen = new Pen(Color::Black);
	detail->pUnderPen = new Pen(Color::Black);
}

// ----------------------------------------------
void xcglsTabCtrl::xDefaultItemDetail(sGlsTabItem* detail, color_t color)
{
	detail->radius = 3.0; // default 3
	detail->NormalTopColor=color;
	detail->NormalTopColorMask=0x1FFFFFFF;
	detail->NormalBaseColor=0x7FFFFFFF;
	detail->DownTopColor=color;
	detail->DownTopColorMask=0x7FFFFFFF;
	detail->DownBaseColor=0x7FFFFFFF;
	detail->MoveTopColor=cyiBitmapBlender::ColorShift(color, 0, 50, 50, 50);
	detail->MoveTopColorMask=0x1FFFFFFF;
	detail->MoveBaseColor=__COLORp__(64, 255, 255, 255);
	detail->HiLightColor=cyiBitmapBlender::ColorShift(color, 0, 128, 128, 128);
	detail->bHasChangedMark=True;
	detail->bIsNoLock=False;
}

// ----------------------------------------------
Bool xcglsTabCtrl::Create(HWND hWndParent, int id, int x, int y, int w, sGlsTabDetail* detail/*=0*/)
{
	if (detail==0)
	{
		xDefaultDetail(&m_detail);
	}
	else
	{
		m_detail=*detail;
	}

	//m_bIsNotOwnDraw=True;
	m_DrawObject=this;

	RECT rect = {x, y, m_detail.stack.bIsVertical?10:w, m_detail.stack.bIsVertical?w:10};
	RECT marginRect = {m_detail.wMargin, 0, m_detail.wMargin, m_detail.hMargin};

	if (m_detail.stack.bIsVertical==True)
	{
		marginRect.top=m_detail.wMargin;
		marginRect.bottom=m_detail.wMargin;
		marginRect.left=m_detail.hMargin;
		marginRect.right=0;
	}

	//sGlsStackPannelDetail stackDetail;
	//cglsStackPannel::xDefaultDetail(&stackDetail);
	//stackDetail.bIsFixedMode=True;
	//stackDetail.bIsShowScrollBar=False;
	//stackDetail.bIsVertical=m_detail.stack.bIsVertical;
	//stackDetail.bIsShowScrollBar=True;
	
	m_detail.stack.gls.font=detail->font;
	m_detail.stack.pszToolTip=L"";
	if (cglsStackPannel::Create(hWndParent, id, &rect, &marginRect, this, &m_detail.stack)==False)
	{
		return False;
	}

	//cglsTabCtrl* ctrl = (cglsTabCtrl*)m_parent;
	//m_font = ctrl->m_font;

	Move(x, y);
	Show();

	return True;
}

// ----------------------------------------------
int xcglsTabCtrl::AddItem(int id, Bitmap* icon, const wchar_t* pszTab, Bool bHasClosedButton/*=False*/, sGlsTabItem* item/*=0*/)
{
	return InsertItem(-1, id, icon, pszTab, bHasClosedButton, item);
}

// ----------------------------------------------
Bool xcglsTabCtrl::xUpdateItemzLength(Bool bIsRedraw)
{
	int nLength=0;
	Bool ret=False;

	if (bIsRedraw==True)
	{
		for (cyiNode* node = m_listItem->GetNodeTopFrom(); node; node=node->np)
		{
			sDrawItem* s=(sDrawItem*)node->ref;

			xcglsTabItem* item = (xcglsTabItem*)s->gls;
			item->m_nOriginLength=0;
			item->m_nTextLength=0;
			item->m_nLength=0;
			item->xReDrawObject();
			//item->CreateBitmap(s);
		}
	}
	//cglsStackPannel::xCalculatePosition(&rect);

	//m_nOriginLength = m_bIsVertical?rect.bottom:rect.right;

	//m_nLength = ((xcglsTabCtrl*)m_parent)->GetItemLength();
	//m_nOtherItemsLength = xGetOtherItemsLength();
	//m_nTextLength = xGetTextLength();

	
	if (nLength=GetItemLength())
	{
		for (cyiNode* node = m_listItem->GetNodeTopFrom(); node; node=node->np)
		{
			sDrawItem* s=(sDrawItem*)node->ref;

			xcglsTabItem* item = (xcglsTabItem*)s->gls;
			if (item->SetItemLength(nLength)==True)
			{
				ret=True;
			}
		}
	}
	return ret;
}


// ----------------------------------------------
int xcglsTabCtrl::InsertItem(int index, int id, Bitmap* icon, const wchar_t* pszTab, Bool bHasClosedButton/*=False*/, sGlsTabItem* item/*=0*/)
{
	xcglsTabItem* xItem = new xcglsTabItem();

	if (xItem->Create((HWND)this, id, (IntPtr)icon, (wchar_t*)pszTab, eIA_CENTER, 0, bHasClosedButton, item)==False)
	{
		return -1;
	}

	int res = Insert(index, xItem, 1);

	if (res==-1)
		return -1;

	xUpdateItemzLength();

	//m_bIsNoLock=xItem.bIsNoLock;


	//if (m_listItem->count()==1)
	//{
	//	sDrawItem* s=(sDrawItem*)m_listItem->GetNodeTopFrom()->ref;
	//	xcglsTabItem* item=(xcglsTabItem*)s->gls;
	//	item->Select(True);
	//	m_itemSelected=s;
	//}
	//Update(eGUM_FORCED);

	//if (m_listItem->count()==1)
	//	SetCurSel(0, id);


	return res;
}

// ----------------------------------------------
int xcglsTabCtrl::SetItem(int id, int bitmapId, const wchar_t* pszText, Bool bIsUpdate)
{
	for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		sDrawItem* x = (sDrawItem*)node->ref;

		if (x->id==id)
		{
			xcglsTabItem* item = (xcglsTabItem*)x->gls;

			if (bitmapId!=-1)
			{
				sDrawItem* xx = item->GetItem(1);
				xx->bitmapId=bitmapId;
			}

			if (pszText!=(const wchar_t*)-1)
			{
				sDrawItem* xx = item->GetItem(2);
				if (xx->txt)
					yifree(xx->txt);

				xx->txt=0;

				if (pszText)
					xx->txt=yistrheapW(pszText);
			}
			
			Update(eGUM_FORCED);
			InvalidateRect(0);
			return id;
		}
	}

	return 0;
}

// ----------------------------------------------
int xcglsTabCtrl::DeleteItem(int index, int id/*=-1*/, int FocusId/*=-1*/, Bool bUpdate/*=True*/)
{
	sDrawItem* item=0;

	if (id!=-1)
	{
		index=0;
		for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
		{
			sDrawItem* item = (sDrawItem*)node->ref;
			if (item->gls->m_id==id)
			{
				break;
			}
			index++;
		}
	}

	item = (sDrawItem*)m_listItem->removeEx(index);

	if (item==0)
		return -1;

	if (xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE)
	{
		if (m_children)
		{
			for (cyiNode* node=m_children->GetNodeTopFrom(); node;node=node->np)
			{
				if (node->ref==(void*)item->gls)
				{
					m_children->remove(node);
					break;
				}
			}
		}
	}

	m_hdr.code=TCN_SELCHANGE;
	m_hdr.idFrom=item->gls->m_id;//m_id;
	m_hdr.hwndFrom=0;

	if (m_itemSelected==item)
	{
		m_itemSelected=0;
	}
	if (m_itemMouseMove==item)
	{
		m_itemMouseMove=0;
	}

	if (m_itemMouseDown==item)
	{
		m_itemMouseDown=0;
	}

	cGlassInterface* obj = (cGlassInterface*)item->gls;
	
	if (item->id!=0)
		yifree(item);

	if (obj)
	{
		obj->Destroy();
		delete obj;
	}
	


	int res= m_listItem->count();

	RECT rect;
	xCalculatePosition(&rect);

	Bool bIsSizeChanged=False;
	//int nLength=0;
	//if (nLength=GetItemLength())
	//{
	//	for (cyiNode* node = m_listItem->GetNodeTopFrom(); node; node=node->np)
	//	{
	//		sDrawItem* s=(sDrawItem*)node->ref;

	//		xcglsTabItem* item = (xcglsTabItem*)s->gls;
	//		if (item->SetItemLength(nLength)==True)
	//		{
	//			bIsSizeChanged=True;
	//		}
	//	}
	//}

	bIsSizeChanged=xUpdateItemzLength();

	// Update(eGUM_FORCED);

	if (m_itemSelected==0)
	{
		if (FocusId!=-1)
		{
			SetCurSel(0, FocusId, 1, bUpdate);
		}
		else
		{
			SetCurSel(-1, -1, 1, bUpdate);
		}
	}
	else
	{
		int id=m_itemSelected->gls->m_id;
		xcglsTabItem* x=(xcglsTabItem*)m_itemSelected->gls;
		xOFF_GLS_FLAG(x->m_eStackFlag, eSPF_BG_SELECT_LOCK);
		m_itemSelected=0;
		SetCurSel(0, id, False, bUpdate);
	}


	//bIsSizeChanged?eGUM_FORCED:eGUM_SKIP_REDRAW);
	//InvalidateRect(NULL);

	PostMessage(m_hNotifyWnd/*m_hParentWnd*/, WM_NOTIFY, m_id, (LPARAM)&m_hdr);
	cglsTabCtrl* ctrl = (cglsTabCtrl*)m_parent;
	ctrl->OnNotifyDeleted();
	return res;
}

// ----------------------------------------------
void xcglsTabCtrl::SetCurSel(int index, int id/*=-1*/, Bool bIsNotify/*=True*/, Bool bUpdate/*=True*/)
{
	sDrawItem* item;
	
	if (m_listItem->count()==0)
	{
		return ;
	}

	if (id!=-1)
	{
		item = (sDrawItem*)xGetItemById(id);
	}
	else if (index==-1)
	{
		item = (sDrawItem*)(m_listItem->GetNodeTailFrom(0)->ref);
	}
	else
	{
		item = (sDrawItem*)(m_listItem->GetNodeTopFrom(index)->ref);
	}

	if (item==0)
	{
		return ;
	}


	if (item==m_itemSelected)
	{
		xcglsTabItem* tabItem = (xcglsTabItem*)item->gls;
		if (tabItem->m_detail.bIsNoLock==False)
		{
			return ;
		}
	}

	if (m_itemSelected!=0)
	{
		xcglsTabItem* x = (xcglsTabItem*)m_itemSelected->gls;
		x->Select(False);
	}

	m_itemSelected=item;
	
	//if (CreateBitmap((sDrawItem*)&m_item, 0)==True)
	{

		xcglsTabItem* x = (xcglsTabItem*)item->gls;
		x->Select();

		if (bUpdate==True)
		{
			xUpdateItemzLength(True);
			Update(eGUM_FORCED);//BG_FORCED);
			//Update(m_itemSelected, eDIS_MOUSE_DOWN);
			InvalidateRect(0, False);
		}


		m_hdr.code=TCN_SELCHANGE;
		m_hdr.idFrom=x->m_id;
		m_hdr.hwndFrom=x->m_hWnd;

		if (bIsNotify==True)
			PostMessage(m_hNotifyWnd/*m_hParentWnd*/, WM_NOTIFY, m_id, (LPARAM)&m_hdr);
	}
	return ;
}


// ----------------------------------------------
int xcglsTabCtrl::GetMaxItemLength()
{
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);
	int w = m_bIsVertical?m_h:m_w;
	w -= m_bIsVertical?(m_detail.hMargin<<1):(m_detail.wMargin<<1);
	//w-=m_listItem->count();
	return w/m_detail.wMaxShowCount;
}

// ----------------------------------------------
int xcglsTabCtrl::GetMinItemLength()
{
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);
	int w = m_bIsVertical?m_h:m_w;
	return w/m_detail.wMinShowCount;
}

// ----------------------------------------------
Bool xcglsTabCtrl::IsItemVisible(int id)
{
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		xcglsTabItem* item = (xcglsTabItem* )((sDrawItem*)node->ref)->gls;
		
		if (item->m_id==id)
		{
			if (m_bIsVertical)
			{
				if (item->m_y>=m_nScrollPos && (item->m_y+item->m_h)<=(m_nScrollPos+m_h))
				{
					return 0;
				}
				else
				{
					if (item->m_y<m_nScrollPos)
						return item->m_y;
					return item->m_y+item->m_h-m_h;
				}
			}
			else
			{
				if (item->m_x>=m_nScrollPos && (item->m_x+item->m_w)<=(m_nScrollPos+m_w))
				{
					return 0;
				}
				else
				{
					if (item->m_x<m_nScrollPos)
						return item->m_x;
					return item->m_x+item->m_w-m_w;
				}
			}
		}
	}
	return -1;
}


// ----------------------------------------------
int xcglsTabCtrl::GetItemLength()
{
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	int max = GetMaxItemLength();
	int w = m_bIsVertical?m_h:m_w;
	int nOriginItemzLength=0;

	w-= m_bIsVertical?(m_detail.hMargin<<1):(m_detail.wMargin<<1);

	for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
	{
		xcglsTabItem* item = (xcglsTabItem* )((sDrawItem*)node->ref)->gls;
		nOriginItemzLength+=item->m_nOriginLength;
	}

	if (nOriginItemzLength>w)
	{
		int n = m_listItem->count();
		int min = GetMinItemLength();
		
		if ((w/n)>min)
		{
			return (w/n)-1;
		}
		else
		{
			return min;
		}
	}

	return max;
}

// ----------------------------------------------
int xcglsTabCtrl::GetCurSel()
{
	if (m_itemSelected)
	{
		return m_itemSelected->gls->m_id;//id;
	}

	return -1;
}

// ---------------------------------------
int xcglsTabCtrl::xGetItemzHeight()
{
	if (m_listItem->count()==0)
	{
		return 0;
	}


	sDrawItem* item = (sDrawItem* )m_listItem->GetNodeTopFrom()->ref;

	if (item)
	{
		xcglsTabItem* x = (xcglsTabItem*)item->gls;
		return x->m_nItemzWidth;
	}

	return 0;
}

// ---------------------------------------
int xcglsTabCtrl::GetBgIndex(eDrawItemStatus status, Bool bIsHitted, Bool& bIsSkipAnimation)
{
	bIsSkipAnimation=True;

	if (status==eDIS_FORCED)
	{
		return 0;
	}
	return -1;
}


//// ----------------------------------------------
//void* xcglsTabCtrl::GetTabParam(int index)
//{
//	sGlassUi* ui = m_listBtn.GetNodeTopFrom(index);
//
//	return ui->gls;
//}
//
//// ----------------------------------------------
//void xcglsTabCtrl::SetTabParam(int index, void* param)
//{
//	sGlassUi* ui = m_listBtn.GetNodeTopFrom(index);
//	ui->gls=param;
//}

// ------------------------------------
LRESULT xcglsTabCtrl::OnToolTipTimer(WPARAM wp, LPARAM lp)
{
	int sx=0, sy=0;

	GetScrollPos(sx,sy);
	wp+=sx;
	lp+=sy;

	sDrawItem* item = cglsStackPannel::xFindItem(wp, lp, m_listItem);

	if (item==0)
		return 0;

	xcglsTabItem* obj = (xcglsTabItem*)item->gls;

	if (obj->m_bIsTextReduced==True)
	{
		obj->OnToolTipTimer(wp, lp);
	}
	return 0;
}


// ------------------------------------
LRESULT xcglsTabCtrl::OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled)
{

	//for (cyiNode* node=m_listItem->GetNodeTopFrom();node; node=node->np)
	//{
	//	sDrawItem* item = (sDrawItem*)node->ref;

	//	if (item->id==LOWORD(wp) )
	//	{
	//		if (m_itemSelected==item)
	//		{
	//			break;
	//		}

	//		if (HIWORD(wp)==0)
	//		{
	//			sDrawItem* prev=m_itemSelected;
	//			m_itemSelected=item;
	//			CreateBitmap((sDrawItem*)&m_item, (void*)item);
	//			UpdateBg(eDIS_NORMAL, True);
	//			bIsHandled=True;
	//			PostMessage(m_hNotifyWnd, WM_TAB_SEL_CHANGED, m_itemSelected->id, prev->id);
	//			break;
	//		}
	//		else
	//		{
	//			sDrawItem* prev=m_itemSelected;
	//			m_listItem->remove(node);

	//			if (item==m_itemSelected)
	//			{
	//				cyiNode* node=m_listItem->GetNodeTopFrom();

	//				if (node==0)
	//				{
	//					m_itemSelected=0;
	//				}
	//				else
	//				{
	//					m_itemSelected=(sDrawItem*)node->ref;
	//				}

	//			}
	//			UpdateBg(eDIS_NORMAL, True);
	//			PostMessage(m_hNotifyWnd, WM_TAB_SEL_CHANGED, m_itemSelected?m_itemSelected->id:0, prev->id);
	//			xDestroyItem(item);
	//			PostMessage(m_hNotifyWnd, WM_TAB_ITEM_REMOVED, m_itemSelected->id, prev->id);
	//		}
	//	}
	//}

	return 0;
}
// ----------------------------------------------
Bool xcglsTabCtrl::SetItemChanged(int id, Bool bIsTrue)
{
	sDrawItem* item = GetItem(id);
	if (item==0)
		return False;

	xcglsTabItem* x = (xcglsTabItem* )item->gls;
	x->SetChanged(bIsTrue);//SetButton(bIsTrue);
	Update(eGUM_FORCED);
	return True;
}

// ----------------------------------------------
Bool xcglsTabCtrl::SetItemShowButton(int id, Bool bIsTrue)
{
	sDrawItem* item = GetItem(id);
	if (item==0)
		return False;

	xcglsTabItem* x = (xcglsTabItem* )item->gls;
	x->SetButton(bIsTrue);
	Update(eGUM_FORCED);

	return True;
}

// ----------------------------------------------
void xcglsTabCtrl::GetVisibleLastItemPos(int& x, int& y)
{
	int sw, sh;
	GetScrollPos(sw, sh);
	
	x=0;
	y=0;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)==False)
	{
		for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
		{
			sDrawItem* item = (sDrawItem*)node->ref;

			if ((item->rect.x+item->rect.w)>(sw+m_w))
			{
				x=item->rect.x-sw;
				y=item->rect.y;
				break;
			}
		}
	}
	else
	{
		for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
		{
			sDrawItem* item = (sDrawItem*)node->ref;

			if ((item->rect.y+item->rect.h)>(sh+m_h))
			{
				y=item->rect.y-sh;
				x=item->rect.x;
				break;
			}
		}
	}

}

// ----------------------------------------------
void xcglsTabCtrl::GetVisibleFirstItemPos(int& x, int& y)
{
	int sw, sh;
	GetScrollPos(sw, sh);

	x=0;
	y=0;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)==False)
	{
		for (cyiNode* node=m_listItem->GetNodeTailFrom(0); node; node=node->pp)
		{
			sDrawItem* item = (sDrawItem*)node->ref;

			if ((item->rect.x)<(sw))
			{
				x=m_w-(item->rect.x+item->rect.w-sw);
				y=item->rect.y;
				break;
			}
		}
	}
	else
	{
		for (cyiNode* node=m_listItem->GetNodeTailFrom(0); node; node=node->pp)
		{
			sDrawItem* item = (sDrawItem*)node->ref;

			if ((item->rect.y)<(sh))
			{
				y=m_h-(item->rect.y+item->rect.h-sh);
				x=item->rect.x;
				break;
			}
		}
	}
}

// -----------------------------
void xcglsTabCtrl::OnUpdateBufferBefore(Graphics*g, Bitmap* bmpBuffer, int x, int y, int w, int h)
{
	Bitmap* bmp = cyiBitmapBlender::BitmapCopy(m_bg, bmpBuffer, 0, 0, x, y, w, h);
	CreateBitmap((sDrawItem*)&m_item, bmp);
}

#include "glsResourceBitmap.h"

// ----------------------------------------------
Bool xcglsTabCtrl::CreateBitmap(sDrawItem* item, void* pExtra)
{
	Bitmap* bmpParent = (Bitmap*)pExtra;
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	if (item->id!=0)
		return False;
	if (m_listItem->count()==0)
	{
		if (item->pBmp[0])
		{
			delete item->pBmp[0];
		}
		item->pBmp[0]=0;
		item->pCurrentBmp=0;
		return False;
	}

	if (m_itemSelected==0)
	{
		//cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);
		return False;
	}

	cGlassInterface* obj = (cGlassInterface*)m_itemSelected->gls;

	int nSelectedX=obj->m_x;
	int nSelectedY=obj->m_y;


	cglsResourceBitmap* resource = (cglsResourceBitmap* )cGlassInterface::GetDefaultResource();

	sGeneralDrawItem data;
	memset(&data, 0, sizeof(sGeneralDrawItem));

	// data set.

	data.pBrush[0]=m_detail.brush;
	data.pPen[0]=m_detail.pUnderPen;
	data.pPen[1]=m_detail.pTopPen;

	if (m_bIsVertical)
	{
		data.rect[0].right= m_item.rect.h;
		data.rect[0].bottom=m_item.rect.w;

		data.rect[1].left = m_itemSelected->rect.y;
		data.rect[1].top = m_itemSelected->rect.x;
		data.rect[1].right = m_itemSelected->rect.h;
		data.rect[1].bottom = m_itemSelected->rect.w;
	}
	else
	{
		data.rect[0].right= m_item.rect.w;
		data.rect[0].bottom=m_item.rect.h;

		data.rect[1].left = m_itemSelected->rect.x;
		data.rect[1].top = m_itemSelected->rect.y;
		data.rect[1].right = m_itemSelected->rect.w;
		data.rect[1].bottom = m_itemSelected->rect.h;
	}


	Bitmap* bmp = resource->Draw(eDID_TAB_BG, 0, item->rect.w, item->rect.h, 0, 0, &data); 

	if (m_bIsVertical)
	{
		Bitmap* ret = cyiBitmapBlender::RotateCopy(0, bmp,3);
		delete bmp;
		bmp=ret;
	}

	if (bmp!=0)
	{
		if (item->pBmp[0])
		{
			delete item->pBmp[0];
		}
		int sx, sy;
		GetScrollPos(sx, sy);
		item->pCurrentBmp=item->pBmp[0]=cyiBitmapBlender::BitmapCopy(0, bmp, 0, 0);//sx, sy, 0, 0, bmp->GetWidth(), m_h);
		item->pBmp[1]=item->pBmp[0];
		delete bmp;
	}

	return True;
}

cglsTabCtrl::cglsTabCtrl(void)
:m_itemArrowNext(0),
m_itemArrowPrev(0),
m_btnScroll(0)
{
	__GLASSINTERFACE_NAME__;
}

cglsTabCtrl::~cglsTabCtrl(void)
{
}

// ----------------------------------------------
void cglsTabCtrl::xDefaultDetail(sGlsTabDetail* detail)
{
	xcglsTabCtrl::xDefaultDetail(detail);
}

// ----------------------------------------------
void cglsTabCtrl::xDefaultItemDetail(sGlsTabItem* detail, color_t color)
{
	xcglsTabCtrl::xDefaultItemDetail(detail, color);
}


// ----------------------------------------------
Bool cglsTabCtrl::Create(HWND hWndParent, int id, int x, int y, int w, sGlsTabDetail* pDetail)
{
	sGlsStackPannelDetail detail;
	sGlsTabDetail tab_detail;

	if (pDetail!=0)
	{
		detail=pDetail->stack;
		tab_detail=*pDetail;
	}
	else
	{
		xcglsTabCtrl::xDefaultDetail(&tab_detail);
		cglsStackPannel::xDefaultDetail(&detail);
	}

	detail.bIsFixedMode=False;
	detail.gls.nKeyLButtonDownTimer=500;
	detail.bIsVertical=tab_detail.stack.bIsVertical;
	detail.gls.nKeyLButtonDownTimer=500;
	detail.gls.nKeyRepeatInterval=100;

	m_nLength=w;
	RECT rect = {x, y, detail.bIsVertical?1:w, detail.bIsVertical?w:1};
	if (cglsStackPannel::Create(hWndParent, id, &rect, 0, this, &detail)==False)
	{
		return False;
	}

	m_tab= new xcglsTabCtrl();

	if (m_tab->Create((HWND)this, id, 0, 0, w, &tab_detail)==False)
	{
		Destroy();
		return False;
	}

	m_tab->SetNotifyWindow(m_parent->m_hWnd);

	if (Add(m_tab, 0)==False)
	{
		m_tab->Destroy();
		delete m_tab;
		Destroy();
		return False;
	}

	ZOrder(0);

	m_tab->Show();
	Show();

	return True;
}

// ----------------------------------------------
int cglsTabCtrl::AddItem(int id, Bitmap* icon, const wchar_t* pszTab, Bool bHasClosedButton, sGlsTabItem* item)
{
	return InsertItem(-1, id, icon, pszTab,bHasClosedButton, item);

}

// ----------------------------------------------
int cglsTabCtrl::InsertItem(int index, int id, Bitmap* icon, const wchar_t* pszTab, Bool bHasClosedButton, sGlsTabItem* item)
{
	int res = m_tab->InsertItem(index, id, icon, pszTab,bHasClosedButton, item);

	if (m_tab->m_listItem->count()==1)
	{
		Update(eGUM_FORCED);
	}
	
	if (m_tab->m_nItemzLength>m_nLength)
	{
		xShowScrollButton();
	}
	else
	{
		xHideScrollButton();
	}
	//InvalidateRect(0);

	return res;
}

// ----------------------------------------------
int cglsTabCtrl::SetItem(int id, int bitmapId, const wchar_t* pszText, Bool bIsUpdate)
{
	return m_tab->SetItem(id, bitmapId, pszText, bIsUpdate);
}


// ----------------------------------------------
int cglsTabCtrl::DeleteItem(int index, int id, int FocusId, Bool bUpdateAfter)
{
	int res=m_tab->DeleteItem(index, id, FocusId, bUpdateAfter);
	//Update(eGUM_FORCED);

	//if (m_tab->m_nItemzLength>m_nLength)
	//{
	//	xCreateScrollButton();
	//}
	//else
	//{
	//	xDestroyScrollButton();
	//}
	//InvalidateRect(0);
	return res;
}

// ----------------------------------------------
int cglsTabCtrl::GetItemCount()
{
	return m_tab->GetItemCount();
}

// -----------------------------
int cglsTabCtrl::GetId(int index)
{
	return m_tab->GetId(index);
}

// ----------------------------------------------
int cglsTabCtrl::GetIndex(int id)
{
	return m_tab->GetIndex(id);
}

// ----------------------------------------------
void cglsTabCtrl::SetCurSel(int index, int id, Bool bIsNotify, Bool bUpdate)
{
	m_tab->SetCurSel(index, id, bIsNotify, bUpdate);
}

// ----------------------------------------------
int cglsTabCtrl::GetCurSel()
{
	return m_tab->GetCurSel();
}

// ----------------------------------------------
Bool cglsTabCtrl::ReSize(int length, Font* font)
{
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	if (font)
	{
		SetFont(font, True);
		//m_font=font;
		m_tab->SetFont(font);
		//m_tab->m_font=font;
		//m_tab->xUpdateItemzLength(False);
	}

	if (m_bIsVertical)
	{
		if (length==m_h)
		{
			if (font)
				Update(eGUM_FORCED);
			InvalidateRect(0, False);
			return False;
		}

		int h=m_tab->m_h;
		__super::ReSize(m_w, length);
		m_nLength=m_h;
		m_tab->ReSize(m_tab->m_w, length);//m_tab->m_h+(length-m_h));
	}
	else
	{
		if (length==m_w)
		{
			if (font)
				Update(eGUM_FORCED);
			InvalidateRect(0, False);
			return False;
		}

		int w=m_tab->m_w;

		int wButton=0;

		__super::ReSize(length, m_h);
		m_nLength=m_w;
		m_tab->ReSize(length-wButton, m_tab->m_h);//m_tab->m_w+(length-m_w));
	}

	//m_tab->xUpdateItemzLength();
	m_tab->Update(eGUM_FORCED);

	if (m_tab->m_nItemzLength>m_nLength)
	{
		//CreateBitmap();

		xShowScrollButton(False, True);
	}
	else
	{
		xHideScrollButton(True, True);
	}


	Update(eGUM_FORCED);
	InvalidateRect(0, False);
	return True;
}

// ----------------------------------------------
int cglsTabCtrl::GetMeasureHeight()
{
	return GetFont()->GetHeight(__DEFAULTGRAPHICS__)+13;
}

// ----------------------------------------------
Bool cglsTabCtrl::ShowItem(int id, Bool bIsShow)
{
	return m_tab->ShowItem(bIsShow, id);
}

Bool cglsTabCtrl::IsItemVisible(int id)
{
	return m_tab->IsItemVisible(id);
}

// ----------------------------------------------
Bool cglsTabCtrl::SetItemChanged(int id, Bool bIsTrue)
{
	return m_tab->SetItemChanged(id, bIsTrue);
}

// ----------------------------------------------
Bool cglsTabCtrl::SetItemShowButton(int id, Bool bIsTrue)
{
	return m_tab->SetItemShowButton(id, bIsTrue);
}


// ------------------------------------
//LRESULT cglsTabCtrl::OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled)
//{
//	return 0;
//}

#include "glsButton.h"
#define __SCROLL_BAR_RATIO__ (0.7)
// ----------------------------------------------
Bool cglsTabCtrl::CreateBitmap(sDrawItem* item, void* pExtra)
{
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	if (item->id==0 ||
			item->id==-1)
	{
		return False;
	}

	cglsResourceBitmap* resource = (cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
	//int w=(int)((float)cglsResourceObject::GetFontHeight(m_font)*__SCROLL_BAR_RATIO__);

	int w = m_tab->xGetItemzHeight()*0.7;

	if (w==0)
		return False;

	if (w<12)
	{
		w = 12;
	}

	sGlsButtonDetail detail;
	memset(&detail, 0, sizeof(detail));
	detail.colorHilight=Color::LightSeaGreen;
	detail.edge=2.0;
	int ArrowSize=w*0.6;

	if (item->id==-2)
	{
		for (int i=0; i<3; i++)
		{
			if (item->pBmp[i])
				delete item->pBmp[i];
			Bitmap* bmpArrow = resource->GetBitmap(m_bIsVertical?eGRID_ARROW_UP:eGRID_ARROW_LEFT, __COLORp__(255, 0, 0, 0), ArrowSize, ArrowSize);
			item->pBmp[i]	 = resource->Draw(eGDID_GLASS_BTN_PANNEL, i, 0, w, bmpArrow, 0, &detail);
		}

	}
	else if (item->id==-3)
	{
		for (int i=0; i<3; i++)
		{
			if (item->pBmp[i])
				delete item->pBmp[i];
			Bitmap* bmpArrow = resource->GetBitmap(m_bIsVertical?eGRID_ARROW_DOWN:eGRID_ARROW_RIGHT, __COLORp__(255, 0, 0, 0), ArrowSize, ArrowSize);
			item->pBmp[i]	 = resource->Draw(eGDID_GLASS_BTN_PANNEL, i, 0, w, bmpArrow, 0, &detail);
		}
	}
	else
		return False;

	item->rect.w=item->pBmp[0]->GetWidth();
	item->rect.h=item->pBmp[0]->GetHeight();
	item->pCurrentBmp=item->pBmp[0];

	return True;
}

// --------------------------------
void cglsTabCtrl::SetNotifyWindow(HWND hWnd)
{
	m_tab->SetNotifyWindow(hWnd);
}

// --------------------------------
Bool cglsTabCtrl::xShowScrollButton(Bool bContainSize, Bool bForced)
{
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	if (m_btnScroll==0)
	{
		sGlsToolButtonDetail toolDetail;
		cglsToolButton::xDefaultDetail(&toolDetail);
		toolDetail.stack.bIsVertical=True;
		m_btnScroll = new cglsToolButton();
		
		m_btnScroll->Create(this, -2, 600, 100, 0, 0, &toolDetail);
		m_btnScroll->Add(eGTBT_AUP, eGTBSS_FILL);
		m_btnScroll->Add(eGTBT_ADOWN, eGTBSS_FILL);
		m_btnScroll->Show();
		m_btnScroll->Update(eGUM_FORCED);
		m_btnScroll->Hide();

	}

	if (bForced==False)
	{
		if (IsWindowVisible(m_btnScroll->m_hWnd)==True)
		{
			return False;
		}
	}

	if (this->GetItem(m_btnScroll->m_id)==0)
	{
		if (Insert(0, m_btnScroll, 0, eSA_CENTER)==False)
		{
			return False;
		}
	}

	m_btnScroll->Show();

	int w = m_bIsVertical?0:m_btnScroll->m_w;
	int h = m_bIsVertical?m_btnScroll->m_h:0;

	if (bContainSize==False)
		m_tab->ReSize(m_tab->m_w-w, m_tab->m_h-h, True);

	//Update(eGUM_FORCED);
	xCalculatePosition(0, 0);
	InvalidateRect(0);

	return True;



	//int w, h;
	sDrawItem* item = GetDrawItem((unsigned int)-2);

	if (item)
		return True;

	if (m_itemArrowPrev!=0)
	{
		m_listItem->insertref(0, m_itemArrowPrev);
		m_listItem->insertref(1, m_itemArrowNext);

	}
	else
	{
		sGlsItemDetail detail;

		cglsStackPannel::xDefaultItemDetail(&detail);
		//detail.eAlign=eSA_LEFT;
		if (Insert(-2, 0, 0, 0, 0, 0, &detail)==False)
		{
			return False;
		}
		if (Insert(-3, 1, 0, 0, 0, 0, &detail)==False)
		{
			return False;
		}
		m_itemArrowPrev = GetDrawItem(-2);
		m_itemArrowNext = GetDrawItem(-3);
	}
	

	w = m_bIsVertical?0:m_itemArrowPrev->rect.w;
	h = m_bIsVertical?m_itemArrowPrev->rect.h:0;

	m_tab->ReSize(m_tab->m_w-(w<<1), m_tab->m_h-(h<<1), True);

	Update(eGUM_FORCED);
	InvalidateRect(0);

	return True;
}

// --------------------------------
Bool cglsTabCtrl::xHideScrollButton(Bool bContainSize, Bool bForced)
{
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	if (m_btnScroll==0)
		return False;

	if (bForced==False)
	{
		if (IsWindowVisible(m_btnScroll->m_hWnd)==False)
		{
			return False;
		}
	}

	m_btnScroll->Hide();

	RemoveFromList(-2);
	int w = m_bIsVertical?0:m_btnScroll->m_w;
	int h = m_bIsVertical?m_btnScroll->m_h:0;

	if (bContainSize==False)
		m_tab->ReSize(m_tab->m_w+w, m_tab->m_h+h, True);

	//Update(eGUM_FORCED);
	xCalculatePosition(0, 0);
	InvalidateRect(0);
	return True;
	sDrawItem* item = GetDrawItem((unsigned int)-2);

	if (item==0)
		return True;

	//int w = m_bIsVertical?0:m_itemArrowPrev->rect.w;
	//int h = m_bIsVertical?m_itemArrowPrev->rect.h:0;

	//RemoveFromList(-2);
	//RemoveFromList(-3);

	m_tab->m_nScrollPos=0;
	m_tab->ReSize(m_tab->m_w+(w<<1), m_tab->m_h+(h<<1), True);

	//Update(eGUM_FORCED);
	xCalculatePosition(0, 0);
	InvalidateRect(0);

	return True;
}

// ------------------------------------
LRESULT cglsTabCtrl::OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled)
{
	short id = (wp&0xFFFF);

	if (id==-2)
	{
		unsigned int code=(wp>>16);
		if (code==eGTBT_ALEFT ||
				code==eGTBT_AUP)
			return OnScrollPrev(0, 0);
		if (code==eGTBT_ARIGHT ||
				code==eGTBT_ADOWN)
			return OnScrollNext(0,0);
	}

	if (wp==-2)
		return OnScrollPrev(0, 0);
	if (wp==-3)
		return OnScrollNext(0,0);

	return 0;
}

// ------------------------------------
LRESULT cglsTabCtrl::OnLButtonRepeated(WPARAM wParam, LPARAM lParam)
{
	if (m_listItem->count()!=3)
	{
		EndLButtonRepeat();
		return 0;
	}

	if (xIsInRect(&m_itemArrowNext->rect, wParam, lParam)==True)
	{
		OnScrollNext(0, 0);
		return 0;
	}

	if (xIsInRect(&m_itemArrowPrev->rect, wParam, lParam)==True)
	{
		OnScrollPrev(0, 0);
		return 0;
	}

	EndLButtonRepeat();

	return 0;
}

// ------------------------------------
LRESULT cglsTabCtrl::OnLButtonLongPressed(WPARAM wParam, LPARAM lParam)
{
	if (m_listItem->count()!=3)
		return 0;

	if (xIsInRect(&m_itemArrowNext->rect, wParam, lParam)==False &&
		xIsInRect(&m_itemArrowPrev->rect, wParam, lParam)==False)
		return 0;

	StartLButtonRepeat(wParam, lParam, 100);
	return 1;
}

// ------------------------------------
LRESULT cglsTabCtrl::OnScrollPrev(HWND hWnd, int code)
{
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);

	int nOneElement = (float)(m_tab->m_nItemzLength)/(float)(m_tab->m_listItem->count());
	int mod = 0;
	int x,y;

	m_tab->GetVisibleFirstItemPos(x, y);


	mod=m_nScrollPos%nOneElement;
	//m_tab->Scroll(-nOneElement-mod, 300);

	int len = m_bIsVertical?y:x;//m_tab->m_h:m_tab->m_w;
	m_tab->Scroll(-len-mod, 500, 0, 0);//-nOneElement, 1000);
	return 0;
}

// ------------------------------------
LRESULT cglsTabCtrl::OnScrollNext(HWND hWnd, int code)
{
	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL);
	int x,y;

	int nOneElement = (float)(m_tab->m_nItemzLength)/(float)(m_tab->m_listItem->count());
	int mod=m_nScrollPos%nOneElement;
	
	m_tab->GetVisibleLastItemPos(x, y);

	int len = m_bIsVertical?y:x;

	//if (mod)
	//	m_tab->Scroll(nOneElement+(nOneElement-mod), 300);
	//else
	//	m_tab->Scroll(nOneElement, 300);


	if (mod)
		m_tab->Scroll(len+(nOneElement-mod), 500, 0, 0);
	else
		m_tab->Scroll(len, 500, 0/*nOneElement*/, 0);

	return 0;
}


// ------------------------------------
void cglsTabCtrl::OnNotifyDeleted()
{
	if (m_tab->m_nItemzLength<=m_nLength)
	{
		xHideScrollButton();
		InvalidateRect(0);
	}
}

// -----------------------------
void cglsTabCtrl::Update(eGlsUpdateMode mode, eDrawItemStatus status)
{
	if (mode==eGUM_FORCED)
	{
		int n = 0;
		if (m_tab->m_itemSelected)
		{
			n = IsItemVisible(m_tab->m_itemSelected->id);
			if (n!=0 && 
					n!=-1)
			{
				m_tab->m_nScrollPos=n;
			}
		}

		if (n==-1)
		{
			m_tab->m_nScrollPos=0;
		}
	}
	
	m_tab->xUpdateItemzLength(True);
	__super::Update(mode, status);

	if (mode==eGUM_FORCED)
	{
		if (m_tab->m_nItemzLength<=m_nLength)
		{
			if (xHideScrollButton()==True)
			{
				InvalidateRect(0);
			}
		}
		else
		{
			if (xShowScrollButton()==True)
			{
				InvalidateRect(0);
			}
		}
	}
}
