#include "glsListView.h"

cglsListView::cglsListView(void)
: m_listbox(0)
, m_pannelHeader(0)
, m_nColumn(0)
, m_columns(0)
, m_bIsUp(False)
, m_nSelectIndex(-1)
{
}

cglsListView::~cglsListView(void)
{
}

//typedef struct {
//	int width;
//	wchar_t* pwszColumnName;
//	int align;
//} xxxTemp;

// -------------------------------
Bool cglsListView::CreateColumn(int n, sListViewColumn* column, ...)
{
	va_list ap;
	m_nColumn=n;
	int total_width=0;//GetFontHeight(True)>>1;

	va_start(ap, column);

	if (m_pannelHeader==0)
	{
		RECT rect = {0, 0, m_w, GetFontHeight(True)};
		sGlsStackPannelDetail detail;
		cglsStackPannel::xDefaultDetail(&detail);

		//detail.bIsFixedMode=True;
		m_pannelHeader = new cglsStackPannel();
		m_pannelHeader->Create((HWND)this, 1, &rect, 0, this, &detail);

		sListViewColumn** columns = (sListViewColumn**)&column;
		m_columns = (sListViewColumn*)yialloc(sizeof(sListViewColumn)*n);
		int distance = GetFontHeight(True)>>1;
		//sGlsItemDetail detail;

		int nFontHeight = GetFontHeight(True)>>1;
		for (int i=0; i<n; i++)
		{
			m_columns[i].m_width = columns[i]->m_width*nFontHeight;
			m_columns[i].m_align = (eGlsItemAlign)columns[i]->m_align;
			m_columns[i].m_type = columns[i]->m_type;
			m_pannelHeader->Add(i+1, 0, columns[i]->m_pszText, 0);
			total_width+=m_columns[i].m_width;
		}

		m_pannelHeader->AddFreeItem(n+1, 0, 0, 0, 0); 
		m_pannelHeader->AddFreeItem(n+2, 0, 0, 0, 0); 

		m_pannelHeader->ShowItem(False, n+1);
		m_pannelHeader->ShowItem(False, n+2);
		m_pannelHeader->Show();
	}

	if (m_listbox==0)
	{
		sGlsListBoxDetail detail;
		cglsListBox::xDefaultDetail(&detail);
		detail.bIsListView=True;
		detail.stack.gls.bRecieveDblClick=True;

		m_listbox = new cglsListBox();
		m_listbox->Create((HWND)this, 2, 0, GetFontHeight(True), m_w, m_h-GetFontHeight(True), &detail);
		m_listbox->SetColumn(m_nColumn, m_columns);


		//m_listbox->Update(eGUM_FORCED);
	}

	ReSize(total_width, m_h);
	return True;
}

// -------------------------------
Bool cglsListView::AddItem(int id, color_t colorFont, Bitmap* bmp, wchar_t* pwszItemText, ...)
{
	sListViewItem* p = (sListViewItem*)&bmp;
	sListViewItem* px = (sListViewItem*)yialloc(m_nColumn*sizeof(sListViewItem));
	va_list ap;
	va_start(ap, pwszItemText);

	for (int i=0; i<m_nColumn; i++)
	{
		
		sListViewItem s = {p[i].bmp, (m_columns[i].m_type==eGCT_NUMERIC)?(wchar_t*)p[i].pszText:(m_columns[i].m_type==eGCT_STRING)?(wchar_t*)yistrheap((char*)p[i].pszText):yistrheapW(p[i].pszText)};
		px[i]= s;//(sListViewItem*)yitoheap(&s, sizeof(s));
	}

	return m_listbox->AddItemEx(id, px, colorFont);//(sListViewItem*)yitoheap(s, sizeof(sListViewItem)*m_nColumn));
}

// -------------------------------
Bool cglsListView::UpdateItem(int id, int index, color_t colorFont, Bitmap* bmp, wchar_t* pwszItemText, ...)
{
	sListViewItem* p = (sListViewItem*)&bmp;
	sListViewItem* px = (sListViewItem*)yialloc(m_nColumn*sizeof(sListViewItem));
	va_list ap;
	va_start(ap, pwszItemText);

	for (int i=0; i<m_nColumn; i++)
	{
		//sListViewItem s = {p[i].bmp, (m_columns[i].m_type==eGCT_NUMERIC)?(wchar_t*)p[i].pszText:yistrheapW(p[i].pszText)};
		sListViewItem s = {p[i].bmp, (m_columns[i].m_type==eGCT_NUMERIC)?(wchar_t*)p[i].pszText:(m_columns[i].m_type==eGCT_STRING)?(wchar_t*)yistrheap((char*)p[i].pszText):yistrheapW(p[i].pszText)};
		//sListViewItem s = {p[i].bmp, yistrheapW(p[i].pszText)};
		px[i]= s;//(sListViewItem*)yitoheap(&s, sizeof(s));
	}

	return m_listbox->UpdateItemEx(id, index, px, colorFont);//(sListViewItem*)yitoheap(s, sizeof(sListViewItem)*m_nColumn));
}

// -------------------------------
Bool cglsListView::InsertItem(int id, int index, color_t colorFont, Bitmap* bmp, wchar_t* pwszItemText, ...)
{
	sListViewItem* p = (sListViewItem*)&bmp;
	sListViewItem* px = (sListViewItem*)yialloc(m_nColumn*sizeof(sListViewItem));
	va_list ap;
	va_start(ap, pwszItemText);

	for (int i=0; i<m_nColumn; i++)
	{
		//sListViewItem s = {p[i].bmp, (m_columns[i].m_type==eGCT_NUMERIC)?(wchar_t*)p[i].pszText:yistrheapW(p[i].pszText)};
		sListViewItem s = {p[i].bmp, (m_columns[i].m_type==eGCT_NUMERIC)?(wchar_t*)p[i].pszText:(m_columns[i].m_type==eGCT_STRING)?(wchar_t*)yistrheap((char*)p[i].pszText):yistrheapW(p[i].pszText)};
		//sListViewItem s = {p[i].bmp, yistrheapW(p[i].pszText)};
		px[i]= s;//(sListViewItem*)yitoheap(&s, sizeof(s));
	}

	return m_listbox->InsertItemEx(id, index, px, colorFont);//(sListViewItem*)yitoheap(s, sizeof(sListViewItem)*m_nColumn));
}

// -------------------------------
Bool cglsListView::Create(cGlassInterface* parent, int id, int x, int y, int w, int h, Font* font)
{
	RECT rect={x, y, w, h};
	m_DrawObject=NULL;

	sGlsDetail detail;
	cGlassInterface::xDefaultDetail(&detail);
	detail.font=font;
	Bool res = cGlassInterface::Create((HWND)parent, NULL, id, &rect, &detail);
	Show();
	return res;
}

#include "glsResourceBitmap.h"

// -------------------------------
int cglsListView::GetSelectedIndex()
{
	return m_listbox->GetSelectedItem();
}

// -------------------------------
int cglsListView::GetSelectedId()
{
	int n = m_listbox->GetSelectedItem();

	if (n!=-1)
	{
		return m_listbox->IndexToId(n);
	}

	return 0;
}

// -------------------------------
int cglsListView::SelectIndex(int index)
{
	return m_listbox->SelectItem(index);//GetSelectedItem();
}


// -------------------------------
Bool cglsListView::Sort(int index, Bool bIsUpper)
{
	int n = m_listbox->GetItemCount();
	sDrawItem** s = m_listbox->GetExportItemz(False);
	sDrawItem* selectedItem=0;
	Bool bIsNumeric=False;
	
	if (index!=-1 &&
		m_columns[index].m_type==eGCT_NUMERIC)
	{
		bIsNumeric=True;
	}

	if (n==0 ||
			n==1)
		return False;


	for (int i=0; i<n; i++)
	{
		Bool bIsChanged=False;
		for (int j=0; j<n-1-i; j++)
		{
			sListViewItem* xx1 = (sListViewItem*)s[j]->param;
			sListViewItem* xx2 = (sListViewItem*)s[j+1]->param;
			Bitmap* temp;
			int nTemp;
			sDrawItem tempItem;
			yiRect rect;
			//sListViewItem* temp;
			
			int res;
			if (index==-1)
			{
				res = s[j]->id-s[j+1]->id;
			}
			else if (bIsNumeric==True)
			{
				res = (int)xx1[index].pszText-(int)xx2[index].pszText;
			}
			else if (m_columns[index].m_type==eGCT_TEXT)
			{
				res = wcscmp(xx1[index].pszText, xx2[index].pszText);
			}
			else
			{
				res = strcmp((char*)xx1[index].pszText, (char*)xx2[index].pszText);
			}

			if (res==0)
				continue;

			if (res<0)
			{
				if (bIsUpper==True)
				{
					rect=s[j]->rect;
					s[j]->rect=s[j+1]->rect;
					s[j+1]->rect=rect;
					memcpy(&tempItem, s[j], sizeof(sDrawItem));
					memcpy(s[j], s[j+1], sizeof(sDrawItem));
					memcpy(s[j+1], &tempItem, sizeof(sDrawItem));
					//s[j]->param=xx2;
					//s[j+1]->param=xx1;
					//
					//for (int k=0; k<2; k++)
					//{
					//	temp=s[j]->pBmp[k];
					//	s[j]->pBmp[k]=s[j+1]->pBmp[k];
					//	s[j+1]->pBmp[k]=temp;
					//}
					//
					//s[j]->pCurrentBmp=s[j]->pBmp[s[j+1]->nLastItemIndex];
					//s[j+1]->pCurrentBmp=s[j+1]->pBmp[s[j]->nLastItemIndex];
					//
					//nTemp=s[j]->nLastItemIndex;
					//s[j]->nLastItemIndex=s[j+1]->nLastItemIndex;
					//s[j+1]->nLastItemIndex=nTemp;
					bIsChanged=True;
				}
			}
			else
			{
				if (bIsUpper==False)
				{
					rect=s[j]->rect;
					s[j]->rect=s[j+1]->rect;
					s[j+1]->rect=rect;

					memcpy(&tempItem, s[j], sizeof(sDrawItem));
					memcpy(s[j], s[j+1], sizeof(sDrawItem));
					memcpy(s[j+1], &tempItem, sizeof(sDrawItem));

/*					s[j]->param=xx2;
					s[j+1]->param=xx1;
					
					for (int k=0; k<2; k++)
					{
						temp=s[j]->pBmp[k];
						s[j]->pBmp[k]=s[j+1]->pBmp[k];
						s[j+1]->pBmp[k]=temp;
					}
					
					s[j]->pCurrentBmp=s[j]->pBmp[s[j+1]->nLastItemIndex];
					s[j+1]->pCurrentBmp=s[j+1]->pBmp[s[j]->nLastItemIndex];
					
					nTemp=s[j]->nLastItemIndex;
					s[j]->nLastItemIndex=s[j+1]->nLastItemIndex;
					s[j+1]->nLastItemIndex=nTemp;*/
					bIsChanged=True;
				}
			}
		}

		if (s[i]->nLastItemIndex==2)
		{
			selectedItem=s[i];
		}

		if (bIsChanged==False)
			break;
	}

	if (selectedItem)
		m_listbox->SelectItem(selectedItem);

	m_listbox->InvalidateRect(0);
	yifree(s);

	return True;
}

// -------------------------------
void cglsListView::Clear()
{
	m_listbox->Clear();
}


// --------------------------
LRESULT cglsListView::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (wParam<=m_nColumn)
	{
		if (m_nSelectIndex!=(wParam-1))
		{
			sDrawItem* item = m_pannelHeader->GetItem(wParam);
			sDrawItem* item2 = m_pannelHeader->GetItem(m_nColumn+1);

			m_pannelHeader->ShowItem(False, m_nColumn+2);

			
			item2->rect.x=item->rect.x+item->rect.w-item2->rect.w-2;
			item2->rect.y=(item->rect.h-item2->rect.h)>>1;

			m_pannelHeader->ShowItem(True, m_nColumn+1);
			m_bIsUp=True;
			m_nSelectIndex=wParam-1;
			m_pannelHeader->InvalidateRect(0);

			Sort(m_nSelectIndex, m_bIsUp);
		}
		else
		{
			int id=m_nColumn+1;
			sDrawItem* item = m_pannelHeader->GetItem(wParam);
			sDrawItem* item2;
			
			if (m_bIsUp==True)
			{
				id=m_nColumn+2;
				m_pannelHeader->ShowItem(False, m_nColumn+1);
			}
			else
			{
				m_pannelHeader->ShowItem(False, m_nColumn+2);
				m_nSelectIndex=-1;
				Sort(m_nSelectIndex, m_bIsUp);
				m_pannelHeader->InvalidateRect(0);
				return 0;
			}
			
			item2 = m_pannelHeader->GetItem(id);
			
			item2->rect.x=item->rect.x+item->rect.w-item2->rect.w-2;
			item2->rect.y=(item->rect.h-item2->rect.h)>>1;

			m_pannelHeader->ShowItem(True, id);
			m_bIsUp=False;
			m_nSelectIndex=wParam-1;
			m_pannelHeader->InvalidateRect(0);

			//if (m_nSelectIndex!=-1)
			//{
			Sort(m_nSelectIndex, m_bIsUp);
			//}

		}

	}
	else
	{
		//if (m_listbox->IsDblClicked()==True)
		//{
		//	return 1;
		//}
		int id = m_listbox->GetSelectedItem();
		SendMessage(m_hNotifyWnd, WM_COMMAND, m_id|(id<<16), m_listbox->IsDblClicked());
	}

	return 0;
}


// --------------------------
int cglsListView::SetFont(Font* font, Bool bIsReference)
{
	int total_width=0;
	int nFontHeight=m_nFontHeightGDI;
	float factor;

	if (m_font)
		delete m_font;
	
	m_font=(Font*)cyiShape::Clone(font);//font->Clone();
	m_nFontHeight = cGlassInterface::CalculateFontHeight(m_font, False);//m_font->GetHeight(__DEFAULTGRAPHICS__);
	m_nFontHeightGDI = cGlassInterface::CalculateFontHeight(m_font, True);

	if (nFontHeight==m_nFontHeightGDI)
	{
		return 0;
	}

	factor=(float)m_nFontHeightGDI/nFontHeight;


	for (int i=0; i<m_nColumn; i++)
	{
		m_columns[i].m_width *= factor;//m_columns[i]->m_width*factor;
		m_columns[i].m_width++;
		total_width+=m_columns[i].m_width;
	}


	ReSize(total_width, m_h*factor+1);
	m_listbox->ReSize(total_width, m_listbox->m_h);
	m_listbox->Move(0, m_nFontHeight);
	Update(eGUM_FORCED);
	return 0;
}


// --------------------------
Bool cglsListView::CreateBitmap(sDrawItem* item, void* pExtra)
{
	if (item->id==-1)
		return False;

	if (item->id==0)
	{
		item->rect.w = m_w;
		item->rect.h = GetFontHeight(True);
		item->pBmp[0]=BITMAP32(item->rect.w, item->rect.h);
		Graphics g(item->pBmp[0]);
		g.DrawRectangle(&Pen(Color::Black), 0, 0, item->rect.w-1, item->rect.h-1);
		return True;
	}

	if (item->id<=m_nColumn)
	{
		item->rect.w=m_columns[item->id-1].m_width;
		item->rect.h=GetFontHeight(True);

		Bitmap* bmp = BITMAP32(item->rect.w, item->rect.h);
		cyiBitmapBlender::FillColor(bmp, Color::Gray);

		LinearGradientBrush brush1(Point(0, 0),Point(0, item->rect.h>>1), Color(64, 255,255,255), Color(0, 255,255,255));
		Graphics g(bmp);
		g.FillRectangle(&brush1, 0, 0, item->rect.w, item->rect.h>>1);

		LinearGradientBrush brush2(Point(0, 0),Point(0, item->rect.h>>2), Color(32, 255,255,255), Color(0, 255,255,255));
		g.FillRectangle(&brush2, 0, (item->rect.h-(item->rect.h>>2)), item->rect.w, item->rect.h>>2);
		
		if (item->txt!=0 &&
				item->txt[0]!=0)
		{
			cyiShape::BitmapStringEx(bmp, item->txt, GetFont(), Color::White, 0, eSTS_NONE, 0, 0, GetFontHeight(True)>>2);
		}

		if (item->pBmp[0]!=0)
			delete item->pBmp[0];

		item->pBmp[0]=bmp;
		item->pCurrentBmp=bmp;
		g.DrawRectangle(&Pen(Color::Black), 0, 0, item->rect.w-1, item->rect.h-1);
		return True;
	}
	else
	{
		cglsResourceBitmap* resource = (cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
		int h = GetFontHeight(True);
		// up
		if (item->id==m_nColumn+1)
		{
			item->pBmp[0]=resource->GetBitmap(eGRID_ARROW_UP, Color::White, (float)h*0.6, (float)h*0.6);
			item->pCurrentBmp=item->pBmp[0];
			item->rect.w=item->pCurrentBmp->GetWidth();
			item->rect.h=item->pCurrentBmp->GetHeight();
		}
		// down
		else
		{
			item->pBmp[0]=resource->GetBitmap(eGRID_ARROW_DOWN, Color::White,(float)h*0.6, (float)h*0.6);
			item->pCurrentBmp=item->pBmp[0];
			item->rect.w=item->pCurrentBmp->GetWidth();
			item->rect.h=item->pCurrentBmp->GetHeight();
		}

		return True;
	}

}
