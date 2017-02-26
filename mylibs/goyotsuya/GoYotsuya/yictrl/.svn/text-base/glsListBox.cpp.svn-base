#include "glsListBox.h"
#include "glsImageBox.h"

enum eGlsListBoxFlag {
	eGLBF_SORTABLE=1,
	eGLBF_MULTI=2,
	eGLBF_CHECK=4,
	eGLBF_MOUSE_ENTER_NOTIFY=8,
	eGLBF_NO_LBUTTON_DOWN=0x10,
	eGLBF_LIST_VIEW=0x20,
	eGLBF_MSG_LIKE_MENU=0x40
};


cglsListBox::cglsListBox(void)
: m_bmpListBg(0)
, m_bmpCover(0)
, m_bmpCheck(0)
, m_nColumn(0)
, m_columns(0)
, m_nBeforeFontHeight(0)

{
}

cglsListBox::~cglsListBox(void)
{
	if (m_bmpCheck)
		delete m_bmpCheck;
	if (m_bmpCover)
		delete m_bmpCover;
}


// ---------------------------------------
Bitmap* xDrawCheck(int subId, int w, int h, color_t color);

// ------------------------------------------------------------
void cglsListBox::xDefaultDetail(sGlsListBoxDetail* detail)
{
	cglsStackPannel::xDefaultDetail(&detail->stack);
	detail->stack.gls.nMouseLeaveAnimationInterval=200;
	detail->stack.gls.nMouseMoveAnimationInterval=250;
	detail->stack.bIsShowScrollBar=True;
	detail->colorBg1=Color::White;
	detail->colorBg2=Color::Beige;
	detail->colorFont1=Color::Black;
	detail->colorFont2=Color::Black;
	detail->bIsMultiSelect=False;
	detail->bIsAutoSort=False;
	detail->colorSelection=Color::White;
	detail->colorSelectionBg=__COLORp__(255, 32, 128, 255);
	detail->bIsSelectionCheck=False;
	detail->bIsMouseEnterNotify=False;
	detail->bIsNoClicked=False;
	detail->bIsListView=False;
	detail->bIsMsgLikeMenu=False;
	detail->colorCheck=Color::Black;
}

// ------------------------------------------------------------
Bool cglsListBox::Create(HWND hWnd, int id, int x, int y, int w, int h, sGlsListBoxDetail* pDetail)
{
	sGlsListBoxDetail detail;

	if (pDetail)
	{
		detail=*pDetail;
	}
	else
		xDefaultDetail(&detail);

	detail.stack.bIsFixedMode=True;
	//detail.stack.bIsShowScrollBar=True;
	detail.stack.eAlign=eSA_LEFT;
	detail.stack.bIsVertical=True;
	detail.stack.bIsScrollDrawDirect=False;
	detail.stack.gls.bMouseDownCommand=True;
	detail.stack.bIsNomalScrollMode=True;

	m_colorBg1=detail.colorBg1;
	m_colorBg2=detail.colorBg2;
	m_colorFont1=detail.colorFont1;
	m_colorFont2=detail.colorFont2;
	m_colorSelection=detail.colorSelection;
	m_colorSelectionBg=detail.colorSelectionBg;
	m_colorCheck = detail.colorCheck;
	//detail.stack.gls.nDragOnTimer=500;

	xSET_GLS_FLAG(m_ctrlFlag, eGLBF_SORTABLE, detail.bIsAutoSort);
	xSET_GLS_FLAG(m_ctrlFlag, eGLBF_MULTI, detail.bIsMultiSelect);
	xSET_GLS_FLAG(m_ctrlFlag, eGLBF_CHECK, detail.bIsSelectionCheck);
	xSET_GLS_FLAG(m_ctrlFlag, eGLBF_MOUSE_ENTER_NOTIFY, detail.bIsMouseEnterNotify);
	xSET_GLS_FLAG(m_ctrlFlag, eGLBF_NO_LBUTTON_DOWN, detail.bIsNoClicked);
	xSET_GLS_FLAG(m_ctrlFlag, eGLBF_LIST_VIEW, detail.bIsListView);
	xSET_GLS_FLAG(m_ctrlFlag, eGLBF_MSG_LIKE_MENU, detail.bIsMsgLikeMenu);

	RECT rect={x, y, w, h};

	m_parent=(cGlassInterface*)hWnd;
	m_font=detail.stack.gls.font;
	int nMargin=GetFontHeight(True)/2;



	RECT rectMargin={1, 1, 1, 1};
	if (cglsStackPannel::Create(hWnd, id, &rect, &rectMargin, this, &detail.stack)==False)
	{
		return False;
	}

	RECT bufferRect={0, 0, 0, 0};
	SetBufferSize(bufferRect, True);

	//SetNotifyWindow(*this);
	SetScrollNotifyWindow(0);

	Show();
	InvalidateRect(0);
	return True;
}

// ------------------------------------------------------------
Bool cglsListBox::AddItem(int id, Bitmap* bmp, const wchar_t* pszString, color_t colorFont, void* param)
{
	return InsertItem(id, -1, bmp, pszString, colorFont, param);
}

// ------------------------------------------------------------
Bool cglsListBox::AddItem(int id, Bitmap* bmp, const char* pszString, color_t colorFont, void* param)
{
	wchar_t* p=yiToUnicode(0, pszString, 0);
	Bool res = InsertItem(id, -1, bmp, p, colorFont, param);
	yifree(p);
	return res;
}

// ------------------------------------------------------------
int cglsListBox::MeasureItemWidth(wchar_t* pszText, Font* font, Bool bIsChecked, int* height)
{
	wchar_t buffer[1024];
	if (bIsChecked==True)
	{
		wsprintfW(buffer, L"   %s", pszText);
	}
	else
	{
		wsprintfW(buffer, L"  %s", pszText);
	}

	IntPtr size = (IntPtr)cyiShape::BitmapStringEx(0, buffer, font, 0, 0); 
	if (height)
		*height=(size&0xFFFF);
	return (size>>16);
}

// ------------------------------------------------------------
Bool cglsListBox::InsertItem(int id, int index, Bitmap* bmp, const wchar_t* pszString, color_t colorFont, void* param)
{
	wchar_t buffer[1024];

	if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_CHECK)==True)
	{
		wsprintfW(buffer, L"    %s", pszString);
	}
	else
	{
		wsprintfW(buffer, L"  %s", pszString);
	}


	sGlsItemDetail itemDetail;
	cglsStackPannel::xDefaultItemDetail(&itemDetail);
	itemDetail.colorFont=colorFont;
	Bool res= cglsStackPannel::Insert(id, index, bmp, buffer, 0, 0, &itemDetail);

	if (res==False)
		return False;

	cglsStackPannel::GetItem(id)->param=param;
	int n = m_listItem->count();

	if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_CHECK)==True)
	{
		if (m_bmpCheck==0)
		{
			int h=GetFontHeight(True);
			m_bmpCheck=xDrawCheck(0, h, h, m_colorCheck);
		}

		sGlsImageBoxDetail detail;
		cglsImageBox::xDefaultDetail(&detail);
		detail.bAutoResize=True;
		cglsImageBox* img = new cglsImageBox();
		img->Create(this, 0x7FFF+id, 0, 0, 0, 0, m_bmpCheck, &detail);
		img->Hide();
		InsertFreeGlsItem(0x7FFF+id, index, 0, 0, img);
		//ShowItem(True, 0x7FFF+id);
	}

	RECT rect={0, 0, 0, n*GetFontHeight(True)+2};
	SetBufferSize(rect, True);

	return res;
}

// ------------------------------------------------------------
Bool cglsListBox::AddItem(cGlassInterface* object, void* param)
{
	return InsertItem(-1, object, param);
}

// ------------------------------------------------------------
Bool cglsListBox::InsertItem(int index, cGlassInterface* object, void* param)
{
	return cglsStackPannel::Insert(index, object, 0);
}

// ------------------------------------------------------------
//Bool cglsListBox::AddItemEx(int id, Bitmap* bmp, color_t colorFont, const wchar_t* pszString, ...)
//{
//	if (m_nColumn==0)
//		return False;
//	sListViewItem* p = (sListViewItem*)&bmp;
//	
//	sListViewItem* px = (sListViewItem*)yialloc(m_nColumn*sizeof(sListViewItem));
//
//	for (int i=0; i<m_nColumn; i++)
//	{
//		sListViewItem s = {p[i].bmp, yistrheapW(p[i].pszText)};
//		px[i]= s;//(sListViewItem*)yitoheap(&s, sizeof(s));
//	}
//
//	return AddItemEx(id, px);
//}

// ------------------------------------------------------------
Bool cglsListBox::AddItemEx(int id, sListViewItem* list, color_t colorFont)
{
	sGlsItemDetail detail;
	cglsStackPannel::xDefaultItemDetail(&detail);
	detail.param=(void*)list;
	detail.colorFont=colorFont;
	Bool res = cglsStackPannel::Insert(id, -1, 0, 0, 0, 0, &detail);

	int n = m_listItem->count();
	RECT rect={0, 0, 0, n*GetFontHeight(True)+2};
	SetBufferSize(rect, True);

	return res;
}

// ------------------------------------------------------------
Bool cglsListBox::InsertItemEx(int id, int index, Bitmap* bmp, const wchar_t* pszString, ...)
{
	if (m_nColumn==0)
		return False;
	sListViewItem** p = (sListViewItem**)&bmp;
	
	sListViewItem* px = (sListViewItem*)yialloc(m_nColumn*sizeof(sListViewItem));

	for (int i=0; i<m_nColumn; i++)
	{
		sListViewItem s = {p[i]->bmp, yistrheapW(p[i]->pszText)};
		px[i]= s;//(sListViewItem*)yitoheap(&s, sizeof(s));
	}

	return InsertItemEx(id, index, px);
}

// ------------------------------------------------------------
Bool cglsListBox::UpdateItemEx(int id, int index, sListViewItem* list, color_t colorFont)
{
	sDrawItem* item = GetItem(id);
	if (item==0)
		return False;

	sListViewItem* param = (sListViewItem*)item->param;
	yifree(item->param);
	item->param=(void*)list;

	for (int i=0; i<3; i++)
	{
		if (item->pBmp[i])
			delete item->pBmp[i];
		item->pBmp[i]=0;
	}
	//item->nLastItemIndex=0;
	//item->pAnimationBmp=0;

	OnNullBitmapDrawListView(item, 0);
	OnNullBitmapDrawListView(item, (void*)2);
	item->pCurrentBmp=item->pBmp[item->nLastItemIndex];
	//m_itemMouseMove=0;
	//m_itemMouseDown=0;

	InvalidateRect(0);
	return True;
}

// ------------------------------------------------------------
Bool cglsListBox::InsertItemEx(int id, int index, sListViewItem* list, color_t colorFont)
{
	sGlsItemDetail detail;
	cglsStackPannel::xDefaultItemDetail(&detail);
	detail.param=(void*)list;
	detail.colorFont=colorFont;
	Bool res = cglsStackPannel::Insert(id, index, 0, 0, 0, 0, &detail);

	int n = m_listItem->count();
	RECT rect={0, 0, 0, n*GetFontHeight(True)+2};
	SetBufferSize(rect, True);

	return res;
}


// ------------------------------------------------------------
Bool cglsListBox::SetColumn(int nColumnCount, int n, ...)
{
	sListViewColumn* p = (sListViewColumn*)&n;
	int w=0;

	//sColumnInfo** columns = (sColumnInfo**)yialloc(m_nColumn*sizeof(sColumnInfo));

	//for (int i=0; i<m_nColumn; i++)
	//{
	//	m_columns[i]=(sColumnInfo*)yitoheap(&p[i], sizeof(sColumnInfo));
	//	w+=m_columns[i]->width;
	//}

	return SetColumn(nColumnCount, p);

}

// ------------------------------------------------------------
Bool cglsListBox::SetColumn(int nColumnCount, sListViewColumn* column)
{
	m_nColumn=nColumnCount;

	if (m_columns)
		yifree(m_columns);

	m_columns=column;

	//m_columns = (sColumnInfo*)yialloc(sizeof(sColumnInfo)*nColumnCount);
	//memcpy(m_columns, column, sizeof(sColumnInfo)*nColumnCount);

	int w=0;
	for (int i=0; i<m_nColumn; i++)
	{
		w+=m_columns[i].m_width;
	}
	ReSize(w, m_h);
	Update(eGUM_FORCED);
	return True;
}

// ------------------------------------------------------------
int cglsListBox::SelectItem(int index, int id, Bool bIsSelected)
{
	sDrawItem* item = id!=0?GetItem(id):(sDrawItem*)m_listItem->GetTopFromRef(index);
	
	if (item==0)
	{
		return -1;
	}
	
	//int sx, sy;
	//Bool bIsHandled=True;
	//GetScrollPos(sx, sy);

	//OnMouseLButtonDown(0, (item->rect.x+1-sx)|((item->rect.y+1-sy)<<16), bIsHandled);
	////OnMouseLButtonUp(0, (item->rect.x+1-sx)|((item->rect.y+1-sy)<<16), bIsHandled);

	//return 0;

	if (item==m_itemMouseDown && bIsSelected==True)
	{
		return 0;
	}

	ShowItem(bIsSelected, id);

	if (m_itemMouseDown)
	{
		int nCheckedId=m_itemMouseDown->id+0x7FFF;
		ShowItem(False, nCheckedId);
		SetItemStatus(item, eDIS_NORMAL);
		item->pCurrentBmp=item->pBmp[0];
		InvalidateRect(0);
		return 0;
	}

	int sx, sy;
	Bool bIsHandled=True;
	GetScrollPos(sx, sy);

	//OnMouseLButtonDown(0, (item->rect.x+1-sx)|((item->rect.y+1-sy)<<16), bIsHandled);
	//OnMouseLButtonUp(0, (item->rect.x+1-sx)|((item->rect.y+1-sy)<<16), bIsHandled);
	//Update(eGUM_UPDATE, eDIS_MOUSE_DOWN);
	cglsStackPannel::Update(item, eDIS_MOUSE_DOWN);
	m_itemMouseDown=item;

	InvalidateRect(0);

	if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_MSG_LIKE_MENU)==True)
	{
		//PostMessage(m_hNotifyWnd, WM_COMMAND, (LBN_SELCHANGE<<16)|item->id, 0);
	}
	else
	{
		PostMessage(m_hNotifyWnd, WM_COMMAND, (LBN_SELCHANGE<<16)|m_id, 0);
	}

	//{
	//	int nCheckedId=m_itemMouseDown->id+0x7FFF;
	//	ShowItem(True, nCheckedId);
	//	SetItemStatus(item, eDIS_NORMAL);
	//	item->pCurrentBmp=item->pBmp[0];
	//	InvalidateRect(0);
	//	return 0;
	//}

	return 0;
}
#include "glsResourceBitmap.h"
// --------------------------
Bitmap* cglsListBox::MakeListViewImage(sDrawItem* item, Bool reverse)
{
	sListViewItem* s = (sListViewItem*)item->param;
	int index=cglsStackPannel::GetIndex(item->id);
	int nMargin = m_rectMargin.left;

	cglsResourceBitmap* resource = (cglsResourceBitmap* )cGlassInterface::GetDefaultResource();
	Bitmap* bmp = BITMAP32(m_w-(nMargin<<1), GetFontHeight(True));
	int start=nMargin;
	int columnMargin=GetFontHeight(True)>>2;
	int end=0;

	if (reverse==True)
	{
		cyiBitmapBlender::FillColor(bmp, m_colorSelectionBg);
	}
	else
		cyiBitmapBlender::FillColor(bmp, index%2?m_colorBg2:m_colorBg1);

	color_t textColor = item->colorFont;
	textColor=textColor==0?(index%2?m_colorFont1:m_colorFont2):textColor;

	for (int i=0; i<m_nColumn; i++)
	{
		int x=start;
		Bitmap* bmpItem=0;
		Bitmap* bmpStr=0;

		x+=columnMargin;
		int width = m_columns[i].m_width - columnMargin;//(i==0?GetFontHeight(True)/2:0);

		end+=m_columns[i].m_width;

		if (s[i].bmp!=0)
		{
			Bitmap* bmp=s[0].bmp;
			if ((((IntPtr)bmp)&0xFFFF0000)==0)
			{
				bmpItem = resource->GetBitmap((IntPtr)s[i].bmp, 0, GetFontHeight(True), GetFontHeight(True));
			}
			//x+=bmp->GetWidth();	
		}

		if (s[i].pszText)
		{
			wchar_t buffer[32];
			wchar_t* pwszText = s[i].pszText;

			if (m_columns[i].m_type==eGCT_NUMERIC)
			{
				if ((int)s[i].pszText==-1)
				{
					wsprintfW(buffer, L"--");
				}
				else
				{
					wsprintfW(buffer, L"%d", (int)s[i].pszText);
				}
				pwszText=buffer;
			}
			else if (m_columns[i].m_type==eGCT_STRING)
			{
				pwszText=yiToUnicode(0, (const char*)s[i].pszText, 0);
			}

			if (reverse==False)
			{
				bmpStr = cyiShape::BitmapStringEx(0, pwszText, GetFont(), textColor, index%2?m_colorBg2:m_colorBg1, eSTS_NONE, DT_SINGLELINE|DT_MODIFYSTRING|DT_END_ELLIPSIS, width);
			}
			else
			{
				bmpStr = cyiShape::BitmapStringEx(0, pwszText, GetFont(), m_colorSelection, m_colorSelectionBg, eSTS_NONE, DT_SINGLELINE|DT_END_ELLIPSIS|DT_MODIFYSTRING, width);
			}

			if (m_columns[i].m_type==eGCT_STRING)
				yifree(pwszText);
		}

		if (m_columns[i].m_align==eGIA_LEFT)
		{
			if (bmpItem)
			{
				cyiBitmapBlender::BitmapCopy(bmp, bmpItem, x, (bmp->GetHeight()-bmpItem->GetHeight())>>1, 0, 0, bmpItem->GetWidth(), bmpItem->GetHeight());
				x+=bmpItem->GetWidth();
				delete bmpItem;
			}

			if (bmpStr)
			{
				//Graphics g(bmp);
				//g.DrawImage(bmpStr, x, (bmp->GetHeight()-bmpStr->GetHeight())>>1, 0, 0, bmpStr->GetWidth(), bmpStr->GetHeight(), Unit::UnitPixel);
				cyiBitmapBlender::BitmapCopy(bmp, bmpStr, x, (bmp->GetHeight()-bmpStr->GetHeight())>>1, 0, 0, bmpStr->GetWidth(), bmpStr->GetHeight());
				delete bmpStr;
				
			}
		}
		else if (m_columns[i].m_align==::eGIA_RIGHT)
		{
		}
		else
		{
		}

		start = end;
	}

	return bmp;
}

// ---------------------------------------
int cglsListBox::SetFont(Font* font, Bool bIsFontReference)
{
	int n = m_nFontHeightGDI;

	__super::SetFont(font, bIsFontReference);



	Update(eGUM_FORCED);

	return 0;
}

// -------------------------------
void cglsListBox::Clear()
{
	int n=m_listItem->count();
	for (int i=0; i<n; i++)
	{
		sDrawItem* item = (sDrawItem* )m_listItem->removeEx(0);

		for (int j=0; j<3; j++)
		{
			if (item->pBmp[j])
			{
				delete item->pBmp[j];
				item->pBmp[j]=0;
			}
		}

		if (item->txt)
			yifree(item->txt);

		if (item->bitmapId)
		{
			if ((item->bitmapId&0xFFFF0000)!=0)
			{
				Bitmap* bmp=(Bitmap*)item->bitmapId;
				delete bmp;
			}
		}

		sListViewItem* s = (sListViewItem*)item->param;
		// リストビュー
		if (s)
		{
			for (int i=0; i<m_nColumn; i++)
			{
				if (s[i].bmp)
					delete s[i].bmp;

				
				if (m_columns[i].m_type==eGCT_STRING ||
					m_columns[i].m_type==eGCT_TEXT)
				{
					if (s[i].pszText)
						yifree(s[i].pszText);
				}
				//{p[i]->bmp, yistrheapW(p[i]->pszText)};
				//px[i]= s;//(sListViewItem*)yitoheap(&s, sizeof(s));
			}
			yifree(s);
			item->param=0;
		}

		if (item->hAnit)
		{
			::yiAnitStop(item->hAnit, True);
			::yiAnitDestroy(item->hAnit);
			item->hAnit=0;
		}

		item->pCurrentBmp=0;
		yifree(item);
	
	}

	m_itemMouseDown=0;
	m_itemMouseMove=0;
	__super::Clear();
	m_nItemzLength=0;
	m_nItemzWidth=0;
	m_NowAnimationList.clear();
	m_nScrollPos=0;
}

// ---------------------------------------
Bool cglsListBox::ReSizeByItemz()
{
	if (m_listItem->count()==0)
	{
		return ReSize(0, 0);
	}

	int h = m_nItemzLength+m_rectMargin.top+m_rectMargin.bottom;
	int w = m_nItemzWidth+m_rectMargin.left+m_rectMargin.right;
	return ReSize(w, h);
}

// ---------------------------------------
void cglsListBox::Update(eGlsUpdateMode mode, eDrawItemStatus status)
{
	if (mode==eGUM_FORCED)
	{
		if (m_nBeforeFontHeight!=0 &&
			m_nBeforeFontHeight!=GetFontHeight(True))
		{
			float factor=(float)GetFontHeight(True)/(float)m_nBeforeFontHeight;
			float sum=0.0;
			if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_LIST_VIEW)==True)
			{
				for (int i=0; i<m_nColumn; i++)
				{
					m_columns[i].m_width*= factor;
					m_columns[i].m_width++;
					sum+=m_columns[i].m_width;
				}

				ReSize(sum, (float)m_h*factor+1);
			}
			else
			{
				
				ReSize((float)m_w*factor+1, (float)m_h*factor+1);
			}
		}

		m_nBeforeFontHeight=GetFontHeight(True);
		__super::Update(mode, status);

		for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
		{
			sDrawItem* item = (sDrawItem*)node->ref;
			
			for (int i=0; i<3; i++)
			{
				if (item->pBmp[i])
					delete item->pBmp[i];
				item->pBmp[i]=0;
			}
			item->pCurrentBmp=0;
		}
	}
	else
	{
		__super::Update(mode, status);
	}
	InvalidateRect(0);
}

// --------------------------
Bool cglsListBox::OnNullBitmapDrawListView(sDrawItem* item, void* pExtra)
{
	if (item->pBmp[0]==0)
	{
		item->pBmp[0]=MakeListViewImage(item);
		item->pCurrentBmp=item->pBmp[0];

		item->rect.w=item->pCurrentBmp->GetWidth();
		item->rect.h=item->pCurrentBmp->GetHeight();
	}

	if ((int)pExtra==2)
	{
		item->pBmp[2]=MakeListViewImage(item, True);
	}

	return True;
}


// --------------------------
Bool cglsListBox::OnNullBitmapDraw(sDrawItem* item, void* pExtra)
{
	int index=cglsStackPannel::GetIndex(item->id);
	int nDrawIndex=(int)pExtra;
	int nMargin = m_rectMargin.left;

	if (item->pBmp[0]==0 &&
			xIS_GLS_FLAG(m_ctrlFlag, eGLBF_LIST_VIEW)==True)
	{
		OnNullBitmapDrawListView(item, pExtra);
	}
	else if (item->pBmp[0]==0)
	{
		color_t color = item->colorFont;
		color=color==0?(index%2?m_colorFont1:m_colorFont2):color;

		Bitmap* bmpString = cyiShape::BitmapStringEx(0, item->txt, GetFont(), color, index%2?m_colorBg2:m_colorBg1);

		Bitmap* bmpItem = BITMAP32(m_w-(nMargin<<1), GetFontHeight(True));
		cyiBitmapBlender::FillColor(bmpItem, index%2?m_colorBg2:m_colorBg1);

		if (item->bitmapId!=0)
		{
			Bitmap* bmp = (Bitmap*)item->bitmapId;
			cyiBitmapBlender::BitmapCopy(bmpItem, bmp, GetFontHeight(True), (GetFontHeight(True)-bmp->GetHeight())>>1);
			cyiBitmapBlender::BitmapCopy(bmpItem, bmpString, bmp->GetWidth()+GetFontHeight(True), 0, 0, 0, bmpString->GetWidth(), bmpString->GetHeight());
		}
		else
		{
			cyiBitmapBlender::BitmapCopy(bmpItem, bmpString);
		}
		item->pBmp[0]=bmpItem;
		item->pCurrentBmp=item->pBmp[0];
		delete bmpString;

		item->rect.w=item->pCurrentBmp->GetWidth();
		item->rect.h=item->pCurrentBmp->GetHeight();
	}

	if (nDrawIndex==1)
	{
		if (m_bmpCover==0)
		{
			m_bmpCover=cyiShape::DrawRoundRectEx(0, &Pen(Color::Orange), &LinearGradientBrush(PointF(0, 0), PointF(0, GetFontHeight(True)), Color::Orange&0x3FFFFFFF/*Color(64, 0,0,0)*/, Color::Orange&0x1FFFFFFF), RectF(0, 0, m_w-2, GetFontHeight(True)), 2.0);
		}

		item->pBmp[1]=cyiBitmapBlender::BitmapCopy(0, item->pBmp[0]);
		Graphics g(item->pBmp[1]);
		g.DrawImage(m_bmpCover, 0, 0);
	}
	else if (nDrawIndex==2)
	{
		if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_LIST_VIEW)==True)
		{
			OnNullBitmapDrawListView(item, pExtra);
			return True;
		}

		Bitmap* bmpString = cyiShape::BitmapStringEx(0, item->txt, GetFont(), m_colorSelection,
			m_colorSelectionBg);
		Bitmap* bmpItem = BITMAP32(m_w-(nMargin<<1), GetFontHeight(True));
		cyiBitmapBlender::FillColor(bmpItem, m_colorSelectionBg);
		cyiBitmapBlender::BitmapCopy(bmpItem, bmpString);
		item->pBmp[2]=bmpItem;
		delete bmpString;
	}
	

	return True;
}


// ------------------------------------------------------------
int cglsListBox::GetSelectedIndex()
{
	if (m_itemMouseDown==0)
		return -1;
	return GetIndex(m_itemMouseDown->id);
}


// ------------------------------------------------------------
int cglsListBox::GetSelectedItem(wchar_t* pszText)
{
	if (m_itemMouseDown==0)
		return -1;

	if (pszText)
	{
		wcscpy(pszText, &m_itemMouseDown->txt[2]);
	}
	return m_itemMouseDown->id;
}

// ------------------------------------------------------------
void* cglsListBox::GetItemParam(int index)
{
	sDrawItem* item = (sDrawItem*)m_listItem->GetTopFromRef(index);//->GetTopFrom(index);

	if (item==0)
		return 0;
	return item->param;
}


// ------------------------------------------------------------
void* cglsListBox::GetSelectedItemParam()
{

	if (m_itemMouseDown)
	{
		return m_itemMouseDown->param;
	}

	return 0;
}

// ------------------------------------------------------------
int cglsListBox::SelectItem(sDrawItem* item)
{
	m_itemMouseDown=item;
	return 0;
}

// ------------------------------------------------------------
int cglsListBox::SelectItem(int index)
{
	return SelectItem(index, 0, True);
}

// -----------------------------
Bool cglsListBox::OnSelfBgDraw(Bitmap* bmp, RECT* rect, RECT* rectChild)
{
	int nFontHeight=GetFontHeight(True);

	int h = m_h;//((m_h/GetFontHeight())+4)*nFontHeight;
	int w = m_w;
	Bool bIsNew=False;
NEW:
	if (m_bmpListBg==0)
	{
		m_bmpListBg = BITMAP32(w, h);
		bIsNew=True;
	}
	
	if (m_bmpListBg->GetWidth()!=w || m_bmpListBg->GetHeight()!=h)
	{

		delete m_bmpListBg;
		m_bmpListBg=0;
		goto NEW;
	}

	if (m_bmpCurrent->GetWidth()!=m_w ||
			m_bmpCurrent->GetHeight()!=m_h)
	{
		delete m_bmpCurrent;
		m_bmpCurrent=BITMAP32(m_w, m_h);
	}

	if (bIsNew==True)
	{
		Graphics g(m_bmpListBg);
		Bitmap* bmp = BITMAP32(nFontHeight, (nFontHeight<<1));
		cyiBitmapBlender::FillColor(bmp, m_colorBg1, &Rect(0, 0, nFontHeight, nFontHeight));
		cyiBitmapBlender::FillColor(bmp, m_colorBg2, &Rect(0, nFontHeight, nFontHeight, nFontHeight));

		TextureBrush brush(bmp);
		g.FillRectangle(&brush, 1, 1, w-2, h-2);

		delete bmp;
	}

	if (m_nItemzLength>=m_h)
	{
		return False;
	}

	Graphics g(m_bmpCurrent);
	g.DrawImage(m_bmpListBg, 0, 0);//(&Pen(Color::Black), Rect(1, 0, m_w-1, m_h-1));

	return True;
}


// -----------------------------
Bool cglsListBox::OnSelfFgDraw(Bitmap* bmp, RECT* rect, RECT* rectChild)
{
	Graphics g(m_bmpCurrent);
	g.DrawRectangle(&Pen(Color::Black), Rect(0, 0, m_w-1, m_h-1));

	return True;
}

// --------------------------
Bool cglsListBox::CreateBitmap(sDrawItem* item, void* pExtra)
{
	if (item->id==0 ||
			item->id==-1)
	{
		//if (m_bmpCurrent==0)
		//	m_bmpCurrent=BITMAP32(m_w, m_h);

		//if (m_bmpCurrent->GetWidth()!=m_w || m_bmpCurrent->GetHeight()!=m_h)
		//{
		//	delete m_bmpCurrent;
		//	m_bmpCurrent=BITMAP32(m_w, m_h);
		//	return False;
		//}
		item->rect.w=m_w;
		item->rect.h=m_h;
		return False;
	}

	item->rect.w=m_w-2;
	item->rect.h=GetFontHeight(True);
	return True;
}

#define xITEM_DOWN 0x8000

// -----------------------------
Bool cglsListBox::xOnItemMouseUp(sDrawItem* item)
{
	return True;
}

// -----------------------------
Bool cglsListBox::xOnItemMouseDown(sDrawItem* item)
{
	if(xIS_GLS_FLAG(m_ctrlFlag, eGLBF_MULTI)==False)
	{
		return False;
	}

	if (item->flag&xITEM_DOWN)
	{
		cglsStackPannel::Update(item, eDIS_MOUSE_DOWN);
		item->flag&=~(xITEM_DOWN);
		SendMessage(m_hNotifyWnd, WM_COMMAND, (LBN_SELCHANGE<<16)|m_id, (0x80000000|item->id));
	}
	else
	{
		cglsStackPannel::Update(item, eDIS_MOUSE_DOWN);
		item->flag|=xITEM_DOWN;
		SendMessage(m_hNotifyWnd, WM_COMMAND, (LBN_SELCHANGE<<16)|m_id, item->id);
	}

	return True;
}

// -----------------------------
Bool cglsListBox::xOnItemMouseMove(sDrawItem* item)
{
	if(xIS_GLS_FLAG(m_ctrlFlag, eGLBF_MULTI)==False)
	{
		return False;
	}

	if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_CHECK)==True)
	{
		return False;
	}

	if (item->flag&xITEM_DOWN)
	{
		if (m_itemMouseMove)
			cglsStackPannel::Update(m_itemMouseMove, eDIS_MOUSE_LEAVE);

		m_itemMouseMove=0;
		return False;
	}
	
	if (m_itemMouseMove)
		cglsStackPannel::Update(m_itemMouseMove, eDIS_MOUSE_LEAVE);

	cglsStackPannel::Update(item, eDIS_MOUSE_MOVE);
	m_itemMouseMove=item;
	return True;
}

// ------------------------------------
int cglsListBox::GetStatusToBitmapIndexForMulti(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation, Bitmap** ret)
{
	eDrawItemStatus _status = xGET_ITEM_STATUS(item);
	eDrawItemStatus _preStatus = xGET_ITEM_PRE_STATUS(item);

	if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_CHECK)==True)
	{
		sDrawItem* xitem=GetItem(item->id+0x7FFF);

		if (xitem==0)
		{
			int index= __super::GetStatusToBitmapIndex(item, status, bIsNoAnimation, ret);

			if (index==1)
			{
				if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_MOUSE_ENTER_NOTIFY)==True)
				{
					int sx, sy;
					GetScrollPos(sx, sy);
					POINT pt = {item->rect.x-sx, item->rect.y-sy};
					ClientToScreen(*this, &pt);

					PostMessage(this->m_hNotifyWnd, WM_COMMAND, (0x04000000|(item->id&0xFFFF)), (pt.x<<16)|pt.y);
				}
			}

			return index;
		}

		// チェックされたアイテムが見えている状態ならば、
		if (xIS_ITEM_HIDE(xitem)==0)
		{
			// 何もしない。
			if (status==eDIS_MOUSE_DOWN)
			{
				int nCheckedId=item->id+0x7FFF;
				ShowItem(False, nCheckedId);
				//SetItemStatus(item, eDIS_NORMAL);
				bIsNoAnimation=True;
				InvalidateRect(0);
				return -1;
			}
		}
		else
		{
			// 何もしない。
			if (status==eDIS_MOUSE_DOWN)
			{
				int nCheckedId=item->id+0x7FFF;
				ShowItem(True, nCheckedId);
				bIsNoAnimation=True;

				status=eDIS_NORMAL;
				//SetItemStatus(item, eDIS_NORMAL);
				InvalidateRect(0);
				return -1;
			}
		}

		return __super::GetStatusToBitmapIndex(item, status, bIsNoAnimation, ret);
	}

	YIDEBUG("item status:%d, is down:%d", status, (item->flag&xITEM_DOWN));
	if (item->flag&xITEM_DOWN)
	{
		if (status==eDIS_MOUSE_DOWN)
		{
			bIsNoAnimation=True;
			return 0;
		}
		else
		{
			//m_itemMouseMove=0;
			return -1;
		}
	}
	else
	{
		if (status==eDIS_MOUSE_DOWN)
		{
			bIsNoAnimation=True;
			return 2;
		}
		else
		{
			//m_itemMouseMove=0;
			return __super::GetStatusToBitmapIndex(item, status, bIsNoAnimation, ret);
		}
	}

	return 0;
}

// ------------------------------------
int cglsListBox::GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation, Bitmap** ret)
{
	eDrawItemStatus _status = xGET_ITEM_STATUS(item);
	eDrawItemStatus _preStatus = xGET_ITEM_PRE_STATUS(item);

	if(xIS_GLS_FLAG(m_ctrlFlag, eGLBF_MULTI)==True)
	{
		return cglsListBox::GetStatusToBitmapIndexForMulti(item, status, bIsNoAnimation, ret);
	}

	if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_CHECK)==True)
	{
		sDrawItem* xitem=GetItem(item->id+0x7FFF);
		
		if (xitem==0)
		{
			int index= __super::GetStatusToBitmapIndex(item, status, bIsNoAnimation, ret);

			if (index==1)
			{
				if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_MOUSE_ENTER_NOTIFY)==True)
				{
					int sx, sy;
					GetScrollPos(sx, sy);
					POINT pt = {item->rect.x-sx, item->rect.y-sy};
					ClientToScreen(*this, &pt);

					PostMessage(this->m_hNotifyWnd, WM_COMMAND, (0x04000000|(item->id&0xFFFF)), (pt.x<<16)|pt.y);
				}
			}

			return index;
		}

		// チェックされたアイテムが見えている状態ならば、
		if (xIS_ITEM_HIDE(xitem)==0)
		{
			// 何もしない。
			if (status==eDIS_MOUSE_DOWN)
				status=eDIS_FREE;

			if (status==eDIS_FREE)
			{
				int nCheckedId=item->id+0x7FFF;
				ShowItem(False, nCheckedId);
				SetItemStatus(item, eDIS_NORMAL);
				InvalidateRect(0);
				return -1;
			}
		}


		if(status==eDIS_MOUSE_DOWN)
		{
			int nCheckedId=item->id+0x7FFF;
			ShowItem(True, nCheckedId);
			status=eDIS_MOUSE_LEAVE;
			bIsNoAnimation=True;
			InvalidateRect(0);
			return -1;
		}

	}

	// 詮索されていない状態にする。。
	if (_status==eDIS_MOUSE_DOWN)
	{
		if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_CHECK)==False)
		{
			if (status==eDIS_FREE2)
			{
				return -1;
			}
		}

		if (status==eDIS_FREE2)
			m_itemMouseDown=0;

		if (status==eDIS_FREE)
		{
			status=eDIS_MOUSE_LEAVE;
			int index=__super::GetStatusToBitmapIndex(item, status, bIsNoAnimation, ret);

			if (index==1)
			{
				if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_MOUSE_ENTER_NOTIFY)==True)
				{
					int sx, sy;
					GetScrollPos(sx, sy);
					POINT pt = {item->rect.x-sx, item->rect.y-sy};
					ClientToScreen(*this, &pt);
					PostMessage(this->m_hNotifyWnd, WM_COMMAND, (0x04000000|(item->id&0xFFFF)), (pt.x<<16)|pt.y);
				}
			}
			return index;
		}
		
		return -1;
	}

	// 何もしない。
	if (status==eDIS_MOUSE_UP)
	{
		return -1;
	}

	{
		int index=__super::GetStatusToBitmapIndex(item, status, bIsNoAnimation, ret);
		
		if (index==1)
		{
			if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_MOUSE_ENTER_NOTIFY)==True)
			{
				int sx, sy;
				GetScrollPos(sx, sy);
				POINT pt = {item->rect.x-sx, item->rect.y-sy};
				ClientToScreen(*this, &pt);
				PostMessage(this->m_hNotifyWnd, WM_COMMAND, (0x04000000|(item->id&0xFFFF)), (pt.x<<16)|pt.y);
			}
		}

		return index;
	}

	return 0;
}

// ------------------------------------
LRESULT cglsListBox::OnNotHandledCommand(WPARAM wp, LPARAM lp)
{
	if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_MSG_LIKE_MENU)==True)
	{
		SendMessage(m_hNotifyWnd/*m_hParentWnd*/, WM_COMMAND, (LBN_SELCHANGE<<16)|wp, lp);
	}
	else
	{
		SendMessage(m_hNotifyWnd/*m_hParentWnd*/, WM_COMMAND, (LBN_SELCHANGE<<16)|m_id, lp);
	}
	return 0;
}

// ---------------------------------------
LRESULT cglsListBox::OnMouseLButtonDown(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_NO_LBUTTON_DOWN)==True)
	{
		handled=True;
		return 0;
	}

	handled=False;
	return __super::OnMouseLButtonDown(wParam, lParam, handled);
}



// ---------------------------------------
void cglsListBox::xCalculatePosition(RECT* rectAll, Bool bIsLastItemOnly)
{
	__super::xCalculatePosition(rectAll, bIsLastItemOnly);

	if (xIS_GLS_FLAG(m_ctrlFlag, eGLBF_CHECK)==True)
	{
		for (cyiNode* node=m_listItem->GetNodeTopFrom(); node; node=node->np)
		{
			sDrawItem* item = (sDrawItem*)node->ref;
			int id = item->id+0x7FFF;
			int y=item->rect.y;
			int x=item->rect.x;

			for (cyiNode* node=m_listFreeItem.GetNodeTopFrom(); node; node=node->np)
			{
				sDrawItem* item = (sDrawItem*)node->ref;

				if (item->gls==0)
					continue;

				if (item->gls->m_id==id)
				{
					cGlassInterface* obj = item->gls;
					item->rect.x=x;
					item->rect.y=y;
					item->rect.w=obj->m_w;
					item->rect.h=obj->m_h;
					obj->Move(x, y);
					break;
				}
			}
		}
	}
}

// ---------------------------------------
static Bitmap* xDrawCheck(int subId, int w, int h, color_t color)
{
	Bitmap* bg = BITMAP32(h, h);
	Graphics g(bg);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	
	//int n=(h>>3);
	//g.DrawEllipse(&Pen(Color::Red), n, (h>>2), h>>1, h>>1);
	int nMargine=(int)((float)h*0.6);
	h-=nMargine;
	nMargine=nMargine>>1;

	g.DrawLine(&Pen(color,2), nMargine, nMargine+h/2, nMargine+h/2, nMargine+h);
	g.DrawLine(&Pen(color,2), nMargine+h/2, nMargine+h, nMargine+h-2, nMargine);
	return bg;
}