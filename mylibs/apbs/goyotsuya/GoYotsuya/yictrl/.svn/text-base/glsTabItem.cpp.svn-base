#include "glsTabItem.h"
#include "glsTabCtrl.h"

#include "glsToolTip.h"
xcglsTabItem::xcglsTabItem(void)
: cglsStackPannel(),
m_nLength(0),
m_nOtherItemsLength(0),
m_nTextLength(0),
m_bIsDelete(False),
m_bIsTextReduced(False)
{
	__GLASSINTERFACE_NAME__;
}

xcglsTabItem::~xcglsTabItem(void)
{
}

#include "glsResourceBitmap.h"
// ------------------------------------------
Bool xcglsTabItem::Create(HWND hWndParent, int id, uIntPtr bmpId, wchar_t* pszCaption, eItemAlign align, cglsDrawObject* object/*=0*/, Bool bHasClosed/*=False*/, sGlsTabItem* detail)
{
	//m_bIsVertical=True;
	//m_nLength=80;

	if (detail)
	{
		m_detail=*detail;
	}
	else
	{
		xcglsTabCtrl::xDefaultItemDetail(&m_detail);
	}

	m_parent = (cGlassInterface*)hWndParent;
	Bool m_bIsVertical = xIS_GLS_FLAG(((cglsStackPannel*)m_parent)->m_eStackFlag, eSPF_VERTICAL);
	
	xSET_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL, m_bIsVertical);
	m_detail.bIsVertical=m_bIsVertical;//=((cglsStackPannel*)m_parent)->m_bIsVertical;

   	sGlsStackPannelDetail stackDetail;
	cglsStackPannel::xDefaultDetail(&stackDetail);
	stackDetail.bIsFixedMode=False;
	stackDetail.bIsShowScrollBar=False;
	stackDetail.bIsVertical=m_detail.bIsVertical;

	// m_nLength=m_parent->GetItemLength();

	//m_bIsMouseUpNothing=True;
	//m_eSelectedStatus=eDIS_MOUSE_DOWN;
	xON_GLS_FLAG(m_eStackFlag, eSPF_BG_SELECTED_MODE);
	RECT margin = {2, 3, 3, 3};

	stackDetail.pszToolTip=pszCaption;
	if (cglsStackPannel::Create(hWndParent, id, 0, &margin, this, &stackDetail)==False)
	{
		return False;
	}

	sGlsItemDetail xdetail;

	cglsStackPannel::xDefaultItemDetail(&xdetail);

	xdetail.eType=eSPT_STATIC;
	xdetail.eAlign=eSA_CENTER;
	if (bmpId!=0)
	{
		Add(1, (Bitmap*)bmpId, 0, 0, 0, &xdetail);// eSPT_STATIC, eSA_CENTER);
	}

	if (pszCaption!=0)
	{
		Add(2, (Bitmap*)0, pszCaption, 0, 0, &xdetail);
	}

	if (bHasClosed==True)
	{
		xdetail.eType=eSPT_NORMAL;
		Add(3, (Bitmap*)eGRID_CLOSE_BTN, 0, 0, 0, &xdetail);
	}

	//if (bHasMenu==True)
	//{
	//	Add(4, eGRID_MENU_BUTTON, 0, 0, eSPT_NORMAL, eSA_LEFT);
	//}

	RECT rect;
	cglsStackPannel::xCalculatePosition(&rect);

	m_nOriginLength = m_bIsVertical?rect.bottom:rect.right;

	m_nLength = ((xcglsTabCtrl*)m_parent)->GetItemLength();
	m_nOtherItemsLength = xGetOtherItemsLength();
	m_nTextLength = xGetTextLength();
	
	Hide();
	xSET_ITEM_SHOW((&m_item));
	//Show();
	//InvalidateRect(0, False);
	//Update(eGUM_FORCED);
	return True;
}

// ------------------------------------------
int xcglsTabItem::xGetOtherItemsLength()
{
	int sum=0;

	for (cyiNode* node= m_listItem->GetNodeTopFrom(0); node; node=node->np)
	{
		sDrawItem* xItem=(sDrawItem*)node->ref;

		if (xItem->id==2)
		{
			continue;
		}

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)==True)
			sum+=xItem->rect.h;
		else
			sum+=xItem->rect.w;
	}

	return sum;
}

// ------------------------------------------
int xcglsTabItem::xGetTextLength()
{
	if (m_nLength==0)
		return 0;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)==False)
		return m_nLength-m_nOtherItemsLength-(m_rectMargin.left+m_rectMargin.right);

	return m_nLength-m_nOtherItemsLength-(m_rectMargin.top+m_rectMargin.bottom);
}

// ------------------------------------------
int xcglsTabItem::GetItemLength()
{
	return m_nLength;
}

// ------------------------------------------
Bool xcglsTabItem::SetItemLength(int n)
{
	//if (n==m_nLength)
	//	return False;

	//m_nLength = ((xcglsTabCtrl*)m_parent)->GetItemLength();
	RECT rect;
	cglsStackPannel::xCalculatePosition(&rect);

	m_nOriginLength = xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)?rect.bottom:rect.right;

	m_nLength= n >m_nOriginLength? m_nOriginLength:n;

	m_nOtherItemsLength = xGetOtherItemsLength();
	m_nTextLength = xGetTextLength();

	//Update(eGUM_FORCED);

	return True;
}

// ------------------------------------------
int xcglsTabItem::SetText(const char* pszText)
{
	wchar_t* p = yiToUnicode(0, pszText, 0);
	return SetText(p);
}

// ------------------------------------------
int xcglsTabItem::SetText(const wchar_t* pszText)
{
	sDrawItem* item = GetItem(2);

	if (item==0)
	{
		sGlsItemDetail xdetail;

		cglsStackPannel::xDefaultItemDetail(&xdetail);

		xdetail.eType=eSPT_STATIC;
		xdetail.eAlign=eSA_CENTER;
		return Insert(2, 1, 0, pszText, 0, 0, &xdetail);
	}

	if (item->txt)
		yifree(item->txt);

	item->txt=yistrheapW(pszText);
	Update(eGUM_FORCED);

	return 0;
}

// ------------------------------------------
int xcglsTabItem::SetChanged(Bool bIsTrue)
{
	sDrawItem* item = GetItem(4);

	if (item==0)
	{
		Bitmap* bmp = cyiShape::BitmapString(0, L"*", GetFont(), &SolidBrush(Color::Black));

		Insert(4, 0, (Bitmap*)eGRID_CHANGED_MARK , 0, 0, 0, 0);
	}

	ShowItem(bIsTrue, 4);

	return 0;
}

// ------------------------------------------
int xcglsTabItem::SetButton(Bool bIsTrue)
{
	sDrawItem* item = GetItem(3);

	if (item==0)
	{
		sGlsItemDetail xdetail;

		cglsStackPannel::xDefaultItemDetail(&xdetail);

		xdetail.eType=eSPT_NORMAL;
		Add(3, (Bitmap*)eGRID_CLOSE_BTN, 0, 0, 0, &xdetail);

	}

	ShowItem(bIsTrue, 3);

	return 0;
}



// ------------------------------------
//Bool xcglsTabItem::OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect)
//{
//
//}


// ------------------------------------------
void xcglsTabItem::Select(Bool bIsSelected)
{
	if (bIsSelected==True)
		UpdateBg(eDIS_MOUSE_DOWN, False);
	else
		UpdateBg(eDIS_NORMAL, False);
	return ;
}

// ---------------------------------------
int xcglsTabItem::GetBgIndex(eDrawItemStatus status, Bool bIsHitted, Bool& bIsSkipAnimation)
{
	int ret=0;

	bIsSkipAnimation=False;

	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BG_SELECT_LOCK)==True)
	{
		eDrawItemStatus curr=xGET_ITEM_STATUS(((sDrawItem*)&m_item));

		if (curr!=eDIS_MOUSE_DOWN ||
				status!=eDIS_NORMAL)
		{
			return -1;
		}
	}

	//if (status==eDIS_MOUSE_LEAVE)
	//	status=eDIS_NORMAL;


	if (bIsHitted==True)
	{
		return -1;
	}

	if (status==eDIS_MOUSE_MOVE ||
			status==eDIS_MOUSE_ENTER)
	{
		ret=1;
		//bg=m_item.pBmp[1];
		//bgMask=m_item.pBmpMasked[1];
		if (SetItemStatus((sDrawItem*)&m_item, eDIS_MOUSE_MOVE)==False)
		{
			return -1;
		}
	}
	else if (status==eDIS_MOUSE_DOWN)
	{
		ret=2;
		//bg=m_item.pBmp[2];
		//bgMask=m_item.pBmpMasked[2];
		if (SetItemStatus((sDrawItem*)&m_item, eDIS_MOUSE_DOWN)==False)
		{
			//return -1;
		}
		xON_GLS_FLAG(m_eStackFlag, eSPF_BG_SELECT_LOCK);
		bIsSkipAnimation=True;
		return ret;
	}
	else
	{
		if (SetItemStatus((sDrawItem*)&m_item, eDIS_NORMAL)==False)
		{
			return -1;
		}
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_BG_SELECT_LOCK)==True)
		{
			xOFF_GLS_FLAG(m_eStackFlag, eSPF_BG_SELECT_LOCK);
			bIsSkipAnimation=True;
		}

	}

	return ret;
}

// ------------------------------------------
LRESULT xcglsTabItem::OnMouseLButtonDown(WPARAM wp, LPARAM lp, Bool& handled)
{
	int x=lp & 0xFFFF;
	int y=(lp >> 16) & 0xFFFF;


	__super::OnMouseLButtonDown(wp, lp, handled);


	sDrawItem* item = xGetItemIn(x, y);

	if (item)
	{
		if (xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE ||
			xGET_ITEM_TYPE(item)==eSPT_NORMAL)
		{
			return 0;
		}
	}
	xcglsTabCtrl* parent = (xcglsTabCtrl*)m_parent;

	handled=True;
	parent->SetCurSel(0, m_id);
	return 0;
}

// ------------------------------------------
LRESULT xcglsTabItem::OnMouseLButtonUp(WPARAM wp, LPARAM lp, Bool& handled)
{
	__super::OnMouseLButtonUp(wp, lp, handled);

	if (m_bIsDelete==True)
	{
		int id = m_id;
		HWND hNotifyWnd=((xcglsTabCtrl*)m_parent)->m_hNotifyWnd;
		int parentId=m_parent->m_id;
		((xcglsTabCtrl*)m_parent)->DeleteItem(0, m_id);

		NMHDR hdr;
		hdr.code=TCN_REMOVED;
		hdr.idFrom=id;
		//PostMessage(hNotifyWnd, WM_NOTIFY, parentId, (LPARAM)&hdr);
		SendMessage(hNotifyWnd, WM_NOTIFY, parentId, (LPARAM)&hdr);
		return 0;
	}

	int x=lp & 0xFFFF;
	int y=(lp >> 16) & 0xFFFF;
	sDrawItem* item = xGetItemIn(x, y);

	if (item)
	{
		if (xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE ||
			xGET_ITEM_TYPE(item)==eSPT_NORMAL)
		{
			return 0;
		}
	}

	handled=True;
}

// ---------------------------------------
LRESULT xcglsTabItem::OnItemClose(HWND hWnd, int Id)
{
	//((xcglsTabCtrl*)m_parent)->DeleteItem(0, m_id);
	//Destroy();
	NMHDR hdr;
	hdr.code=TCN_REMOVE_CONFIRM;
	hdr.idFrom=m_id;
	if (SendMessage(((xcglsTabCtrl*)m_parent)->m_hNotifyWnd, WM_NOTIFY, m_parent->m_id, (LPARAM)&hdr)==0)
	{
		m_bIsDelete=True;
	}
	return 0;
}

#include "glsPopup.h"

// -------------------------
LRESULT xcglsTabItem::OnMouseLeave(WPARAM wp, LPARAM lp)
{
/*	if (m_tooltip)
	{
		m_bIsToolTipLock=False;
		if (IsWindowVisible(m_tooltip->m_hWnd)==TRUE)
		{
			m_tooltip->Hide(1);
		}
	}*/

	if (m_pszToolTip)
	{
		cglsToolTip::GetInstance()->Hide(True);
	}

	return cglsStackPannel::OnMouseLeave(wp, lp);
}


// ---------------------------------------
LRESULT xcglsTabItem::OnMessageInterrupt(UINT msg, WPARAM wp, LPARAM lp, BOOL& IsHandled)
{
	IsHandled=False;
	if (msg==WM_LBUTTONDOWN)
	{
		int x=lp & 0xFFFF;
		int y=(lp >> 16) & 0xFFFF;

		sDrawItem* item = xGetItemIn(x, y);

		if (item)
		{
			if (xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE ||
				xGET_ITEM_TYPE(item)==eSPT_NORMAL)
			{
				return 0;
			}
		}
		xcglsTabCtrl* parent = (xcglsTabCtrl*)m_parent;

		IsHandled=True;
		parent->SetCurSel(0, m_id);
	}
	else if (msg==WM_LBUTTONUP)
	{
		int x=lp & 0xFFFF;
		int y=(lp >> 16) & 0xFFFF;
		sDrawItem* item = xGetItemIn(x, y);

		if (item)
		{
			if (xGET_ITEM_TYPE(item)==eSPT_GLASSINTERFACE ||
				xGET_ITEM_TYPE(item)==eSPT_NORMAL)
			{
				return 0;
			}
		}

			IsHandled=True;
	}

	return 0;
}

// ---------------------------------------
void xcglsTabItem::xCalculatePosition(RECT* rectAll)
{
	for (cyiNode* node= m_listItem->GetNodeTopFrom(0); node; node=node->np)
	{
		sDrawItem* xItem=(sDrawItem*)node->ref;

		if (xItem->id==2)
		{
			m_DrawObject->CreateBitmap(xItem, (void*)m_nTextLength);
			break;
		}
	}

	cglsStackPannel::xCalculatePosition(rectAll);
}

//// ------------------------------------------
Bool xcglsTabItem::UpdateBg(eDrawItemStatus status, Bool bIsHitted)
{
	__super::UpdateBg(status, bIsHitted);
	//if (m_bIsDelete==True)
	//{
	//	((xcglsTabCtrl*)m_parent)->DeleteItem(0, m_id);
	//}
	return True;
}

//// ---------------------------------------
//LRESULT xcglsTabItem::OnMouseLButtonDown(WPARAM wp, LPARAM lp, BOOL& handled)
//{
//	cglsStackPannel::OnMouseLButtonDown(wp, lp, handled);
//	return 0;
//}

#include "glsResourceBitmap.h"
// ------------------------------------------
void xcglsTabItem::DestroyBitmap(sDrawItem* item)
{
	if (item->id==2)
	{
		if (item->pBmp[0] && item->pBmp[0]->GetLastStatus()==0)
			delete item->pBmp[0];
		return ;
	}

	//if (item->id==4)
	//	return ;

	for (int i=0; i<3; i++)
	{
		if (item->id!=0 &&
			i==1)
			continue;

		if (item->txt)
		{
			if (item->pBmp[i])
				delete item->pBmp[i];
			item->pBmp[i]=0;
		}
		else
		{
			if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)==True)
			{
				if (item->pBmp[i])
					delete item->pBmp[i];
				item->pBmp[i]=0;
			}
		}
	}
}

// ------------------------------------------e
Bool xcglsTabItem::CreateBitmap(sDrawItem* item, void* pExtra)
{
	//cglsDrawObject::CreateBitmap(item);
	cglsResourceBitmap* resource = (cglsResourceBitmap*)m_xresource;

	if (resource==0)
		resource=(cglsResourceBitmap*)cGlassInterface::GetDefaultResource();

	if (item->id==0)
	{
		int w=m_w;
		int h=m_h;

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL)==True)
		{
			w=m_h;
			h=m_w;
		}

		for (int i=0; i<3; i++)
			item->pBmp[i]=resource->Draw(eDID_TAB_ITEM_BG, i, w, h, 0, 0, (void*)&m_detail);
		//item->pBmp[1]=resource->Draw(eDID_TAB_ITEM_BG, 1, w, h, (Bitmap*)&LinearGradientBrush(Point(0, 0), Point(0, h), xARGB(64, 0, 0, 255), Color::White), (Bitmap*)&Pen(Color::Blue, 1.0), (void*)m_bIsVertical);
		//item->pBmp[2]=resource->Draw(eDID_TAB_ITEM_BG, 2, w, h, (Bitmap*)&LinearGradientBrush(Point(0, 0), Point(0, h), xARGB(64, 0, 0, 255), Color::White), (Bitmap*)&Pen(Color::Orange, 2.0), (void*)m_bIsVertical);
		item->pCurrentBmp=item->pBmp[item->nLastItemIndex];
		return True;
	}

	if (item->id==4)
	{
		Bitmap* bmp = resource->GetBitmap(item->bitmapId, 0, (IntPtr)GetFont(), 0, 0);
		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL))
		{
			item->pCurrentBmp=item->pBmp[0]=cyiBitmapBlender::RotateCopy(0, bmp, 3);
		}
		else
		{
			item->pCurrentBmp=item->pBmp[0]=bmp;//cyiBitmapBlender::BitmapCopy(x;
		}
		item->rect.w=item->pBmp[0]->GetWidth();
		item->rect.h=item->pBmp[0]->GetHeight();

		return True;
	}

	//if (m_xfont==0)
	//{
	//	m_xfont=m_font;

	//	if (m_xfont==0)
	//	{
	//		if (m_parent)
	//		{
	//			m_xfont=m_parent->GetFont();
	//		}
	//	}
	//}

	//// デフォルト
	//if (m_xfont==0)
	//{
	//	m_xfont=new Font(L"Meiryo", 10.0);
	//}

	xcglsTabCtrl* parent = (xcglsTabCtrl*)m_parent;

	m_xfont=GetFont();//m_font=parent->m_font;

	int nFontHeight = cglsResourceObject::GetFontHeight(m_xfont);

	if (item->txt)
	{
		if (m_nTextLength==0)
		{
			if (item->pBmp[0])
				delete item->pBmp[0];

			Bitmap*x=cyiShape::BitmapString(0, item->txt, m_xfont, &SolidBrush(Color::Black),  eSTS_NONE);
			//Bitmap* xx = cBB::BitmapCopy(0, x);
			//Graphics g(x);
			//g.DrawImage(xx, 0, 0);
			//delete xx;

			if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL))
			{
				item->pCurrentBmp=item->pBmp[0]=cyiBitmapBlender::RotateCopy(0, x, 3);
				delete x;
			}
			else
			{
				item->pCurrentBmp=item->pBmp[0]=x;
			}
			//m_nTextLength=item->pCurrentBmp->GetWidth();
			item->rect.w=item->pCurrentBmp->GetWidth();
			item->rect.h=item->pCurrentBmp->GetHeight();
			return True;
		}

		Bitmap* x=cyiShape::BitmapString(0, item->txt, m_xfont, &SolidBrush(Color::Black), eSTS_FIXED, (color_t)&m_bIsTextReduced, (float)m_nTextLength);
		//Bitmap* xx = cBB::BitmapCopy(0, x);
		//Graphics g(x);
		//g.DrawImage(x, 0, 0);
		//delete xx;

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL))
		{
			item->pCurrentBmp=item->pBmp[0]=cyiBitmapBlender::RotateCopy(0, x, 3);
			delete x;
		}
		else
		{
			item->pCurrentBmp=item->pBmp[0]=x;
		}

		item->rect.w=item->pCurrentBmp->GetWidth();
		item->rect.h=item->pCurrentBmp->GetHeight();
		return True;
	}

	if (item->bitmapId)
	{
		Bitmap* x;
		
		if ((item->bitmapId&0xFFFF0000)==0)
			x= resource->GetBitmap(item->bitmapId, 0, 0, ((float)nFontHeight*0.7));
		else
			x=(Bitmap*)item->bitmapId;



		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL))
		{
			if (item->pBmp[0])
				delete item->pBmp[0];
			item->pCurrentBmp=item->pBmp[0]=cyiBitmapBlender::RotateCopy(0, x, 3);

			//if (item->bitmapId!=(int)x)
			//	delete x;
		}
		else
		{
			item->pCurrentBmp=item->pBmp[0]=x;
		}

		item->rect.w=item->pCurrentBmp->GetWidth();
		item->rect.h=item->pCurrentBmp->GetHeight();

		if (xGET_ITEM_TYPE(item)==eSPT_STATIC ||
				item->bitmapId==(IntPtr)item->pCurrentBmp)
		{
			return True;
		}

		x = resource->GetBitmap(item->bitmapId, 1, 0, ((float)nFontHeight*0.7));

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL))
			{
			if (item->pBmp[1])
				delete item->pBmp[1];
			item->pBmp[1]=cyiBitmapBlender::RotateCopy(0, x, 3);
			//delete x;
		}
		else
		{
			item->pBmp[1]=x;
		}

		x = resource->GetBitmap(item->bitmapId, 2, 0, ((float)nFontHeight*0.7));

		if (xIS_GLS_FLAG(m_eStackFlag, eSPF_VERTICAL))
		{
			if (item->pBmp[2])
				delete item->pBmp[2];
			item->pBmp[2]=cyiBitmapBlender::RotateCopy(0, x, 3);
			//delete x;
		}
		else
		{
			item->pBmp[2]=x;
		}

		return True;
	}

	return True;
}
