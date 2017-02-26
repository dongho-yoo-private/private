#include "cyctrlx.h"
#include "yiBitmap.h"

cyctrlx::cyctrlx(void)
: cyctrlbase()
{
}

cyctrlx::~cyctrlx(void)
{
}


// -------------------------
static void xInvalidateRect(HWND hWnd, RECT* rect, BOOL bEarse, Bool bIsCoordChanged=False)
{
	if (bIsCoordChanged==True)
	{
		RECT xrect = {rect->top, rect->left, rect->top+rect->bottom, rect->right+rect->left};
		InvalidateRect(hWnd, &xrect, bEarse);
		return ;
	}

	RECT xrect = {rect->left, rect->top, rect->left+rect->right, rect->bottom+rect->top};
	InvalidateRect(hWnd, &xrect, bEarse);
}


// ---------------------------------------
static BOOL xIsInRect(RECT* rect, int x, int y, Bool bIsCoordChanged=False)
{
	if (bIsCoordChanged==True)
	{
		int temp=x;
		x=y;
		y=temp;
	}

	if (x<rect->left)
		return FALSE;
	if (x>(rect->right+rect->left))
		return FALSE;
	if (y>(rect->bottom+rect->top))
		return FALSE;
	if (y<rect->top)
		return FALSE;

	return TRUE;
}


// --------------------------------------------
BOOL cyctrlx::Create(HWND hWndParent, int x, int y, int w_margin, int h_margin, xControlDraw drawf, BOOL bIsHide, Font* fnt, BOOL bIsVertical, unsigned int w, unsigned int h)
{
	m_wMargin=w_margin;
	m_hMargin=h_margin;
	m_xdraw=drawf;

	m_xfont = fnt;

	m_bIsVertical = bIsVertical;
	m_xpp=0;
	m_bmp= new Bitmap(2, 2, PixelFormat32bppARGB);
	m_g= new Graphics(m_bmp);
	m_g->SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
	m_g->SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	m_selected_item=0;
	//m_ToolTipMode=False;
	m_bgFixed=False;

	if (w!=0 || h!=0)
	{
		m_bgFixed=True;
	}

	m_bCoordChanged=False;
	m_hMutex = yiMutexCreate(0);

	m_parent=0;

	return cyctrlbase::Create(hWndParent, 0, 0, x, y, w, h, bIsHide);
}


// --------------------------------------------
void cyctrlx::Destroy()
{
	if (m_xfont)
		delete m_xfont;
	if (m_bmp)
		delete m_bmp;
	if (m_g)
		delete m_g;

	if (m_xpp)
		m_x.DeleteArray((void**)m_xpp);

	m_x.clear(yifree);

	cyctrlbase::Destroy();
}

// --------------------------------------------
void cyctrlx::GetBmpPos(sControlx* s, float* x, float* y, UINT _w, UINT _h, eContorlxAlign align)
{
	int n_bmp_w = s->bmp ? s->bmp->GetWidth():0;
	int n_bmp_h = s->bmp ? s->bmp->GetHeight():0;

	if (align&eCXA_CENTER)
	{
		float w = (float)(_w);
		*x = ((w-s->txt_w-(float)n_bmp_w)/2);
	}
	else if (align&eCXA_LEFT)
	{
		float w = (float)(_w);
		*x = s->rect.left;
	}
	else
	{
		*x=((s->rect.right)-s->txt_w-(float)n_bmp_w);
	}

	if (align&eCXA_VCENTER)
	{
		float h = (float)(_h);
		*y = ((h-n_bmp_h)/2);
	}
	else if (align&eCXA_TOP)
	{
		*y = s->rect.top;
	}
	else
	{
		float h = (float)(_h);
		*y=(s->rect.bottom)-n_bmp_h;
	}
}

// --------------------------------------------
void cyctrlx::GetTextPos(sControlx* s, float* x, float* y, eContorlxAlign align)
{
	if (align&eCXA_CENTER)
	{
		float w = (float)(s->rect.right);
		*x = ((w-s->txt_w)/2)+s->rect.left;
	}
	else if (align&eCXA_LEFT)
	{
		float w = (float)(s->rect.right);
		*x = s->rect.left;
	}
	else
	{
		*x=(s->rect.right-s->rect.left)-s->txt_w;
	}

	if (align&eCXA_VCENTER)
	{
		float h = (float)(s->rect.bottom);
		*y = ((h-s->txt_h)/2)+s->rect.top+1;
	}
	else if (align&eCXA_TOP)
	{
		*y = s->rect.top+1;
	}
	else
	{
		float h = (float)(s->rect.bottom);
		*y=(s->rect.bottom-s->rect.top)-s->txt_h+1;
	}
}

// --------------------------------------------
void cyctrlx::GetTextPos(sControlx* s, float* x, float* y, UINT _w, UINT _h, eContorlxAlign align)
{
	int n_bmp_w = s->bmp ? s->bmp->GetWidth():0;
	int n_bmp_h = s->bmp ? s->bmp->GetHeight():0;

	if (align&eCXA_CENTER)
	{
		float w = (float)(_w);
		*x = (((w-s->txt_w-(float)n_bmp_w)/2)+n_bmp_w);
	}
	else if (align&eCXA_LEFT)
	{
		float w = (float)(_w);
		*x = s->rect.left+n_bmp_w;
	}
	else
	{
		*x=(s->rect.right)-s->txt_w;
	}

	if (align&eCXA_VCENTER)
	{
		float h = (float)(_h);
		*y = ((h-s->txt_h)/2);
	}
	else if (align&eCXA_TOP)
	{
		*y = s->rect.top;
	}
	else
	{
		float h = (float)(_h);
		*y=(s->rect.bottom)-s->txt_h;
	}
}



// -----------------------------
int cyctrlx::Add(int id, Bitmap* bmp, const wchar_t* pszText, int distance, 
				 int marginX, int marginY, void* pParam, const wchar_t* pszToolTip, Bool bIsFixedMode)
{
	sControlx data;
	memset(&data, 0, sizeof(data));
	float h=0.0;

	data.id=id;
	data.distance=distance;
	data.bmp=bmp;

	data.rect.right = marginX;
	data.rect.bottom = marginY;
	data.p=pParam;
	data.bIsEnable=True;



	if (data.bmp!=0)
	{
		data.rect.right+=bmp->GetWidth();
		h = h<(float)bmp->GetHeight() ? (float)bmp->GetHeight():h;
	}

	if (pszText!=0)
	{
		RectF bound;
		int len = wcslen(pszText);
		unsigned int bwidth=0;
		Status ret = m_g->MeasureString(pszText, len, m_xfont, RectF(0, (float)0, (float)1024, (float)30), &bound);
		data.txt_w = bound.Width;
		data.txt_h = bound.Height;

		data.rect.right += ((INT)data.txt_w+2);
		//data.rect.bottom += ((INT)data.txt_h+2);

		h = h<data.txt_h ? data.txt_h:h;
		wcscpy(data.name, pszText);
	}

	if (pszToolTip!=0)
	{
		//m_ToolTipMode=True;
		//xCreateToolTip();
	}

	data.rect.bottom+= h+2;

	if (bIsFixedMode)
	{
		data.w_fixed=marginX;
		data.h_fixed=marginY;

		data.rect.right=data.w_fixed;
		data.rect.bottom=data.h_fixed;
	}

	yiMutexLock(m_hMutex);
	m_x.addref(yitoheap(&data, sizeof(data)));
	yiMutexUnlock(m_hMutex);

	return True;
}

// -----------------------------
int cyctrlx::Addx(int id, Bitmap* bmp, const wchar_t* pszText, int distance, 
				 int marginX, int marginY, int maxw, int minw, void* pParam, const wchar_t* pszToolTip)
{
	sControlx data;
	int wmax=0;

	memset(&data, 0, sizeof(data));
	float h=0.0;

	data.id=id;
	data.distance=distance;
	data.bmp=bmp;

	data.rect.right = marginX;
	data.rect.bottom = marginY;
	data.p=pParam;
	data.bIsEnable=True;



	if (data.bmp!=0)
	{
		data.rect.right+=bmp->GetWidth();
		h = h<(float)bmp->GetHeight() ? (float)bmp->GetHeight():h;
	}

	if (pszText!=0)
	{
		RectF bound;
		int len = wcslen(pszText);
		unsigned int bwidth=0;
		Status ret = m_g->MeasureString(pszText, len, m_xfont, RectF(0, (float)0, (float)1024, (float)30), &bound);
		data.txt_w = bound.Width;
		data.txt_h = bound.Height;

		data.rect.right += ((INT)data.txt_w+2);
		//data.rect.bottom += ((INT)data.txt_h+2);

		h = h<data.txt_h ? data.txt_h:h;
		wcscpy(data.name, pszText);
	}

	if (pszToolTip!=0)
	{
		//m_ToolTipMode=True;
		//xCreateToolTip();
	}

	data.rect.bottom+= h+2;

	data.w_fixed=marginX;
	data.h_fixed=marginY;

	data.rect.right=data.w_fixed;
	data.rect.bottom=data.h_fixed;

	data.rect.right = data.rect.right > wmax ? wmax:data.rect.right;

	data.wmax=maxw;
	data.wmin=minw;

	yiMutexLock(m_hMutex);
	m_x.addref(yitoheap(&data, sizeof(data)));
	yiMutexUnlock(m_hMutex);

	return True;
}

// -----------------------------
static void xDestroyx(sControlx* x)
{
	if (x==0)
		return ;

	if (x->bmp!=0)
		delete x->bmp;
	
	for (int i=0; i<10; i++)
	{
		if (x->pBmp[i]!=0)
			delete x->pBmp[i];
	}

	if ((unsigned int)x->p<256)
	{
		yifree(x);
		return ;
	}
	else if (x->p)
	{
		yifree(x->p);
	}

	yifree(x);
}

// -----------------------------
void cyctrlx::Remove(unsigned int id)
{
	yiMutexLock(m_hMutex);

	for (cyiNode* p = m_x.GetNodeTopFrom(0); p!=0; p=p->np)
	{
		sControlx* x = (sControlx*)p->ref;
		if (id==x->id)
		{
			xDestroyx(x);
			m_x.DeleteArray((void**)m_xpp);
			m_xpp=0;
			m_x.remove(p);

			break;
		}
	}
	yiMutexUnlock(m_hMutex);
}


// -----------------------------
void cyctrlx::Update(Bool bIsShow, unsigned int wmax, unsigned int hmax)
{
	int width=m_wMargin;
	int height=m_hMargin;
	int n = m_x.count();
	int oc_cnt=0;
	int xHeight=0;
	int xWidth=0;

	if (n==0)
		return ;

	yiMutexLock(m_hMutex);

	if (m_xpp!=0)
	{
		m_x.DeleteArray((void**)m_xpp);
	}

	m_xpp = (sControlx**)m_x.ExportArray();

	if (m_bIsVertical==True)
	{
		for (int i=0; i<n; i++)
		{
			m_xpp[i]->rect.left = m_wMargin; 
			m_xpp[i]->rect.top	= height+m_xpp[i]->distance;
			height+=m_xpp[i]->rect.bottom+m_xpp[i]->distance;
			width = width > m_xpp[i]->rect.right ? width:m_xpp[i]->rect.right;
			//m_xpp[i]->rect.top=m_hMargin;
		}
	}
	else
	{
		for (int i=0; i<n; i++)
		{
			m_xpp[i]->rect.left = width+m_xpp[i]->distance;
			width+=m_xpp[i]->rect.right+m_xpp[i]->distance;
			height = height > m_xpp[i]->rect.bottom ? height:m_xpp[i]->rect.bottom;
			m_xpp[i]->rect.top=m_hMargin+xHeight;

			if (wmax &&
				width>wmax)
			{
				oc_cnt++;
				xHeight+=height;
				xWidth = xWidth > width ? xWidth:width;
				width=m_wMargin;

				m_xpp[i]->rect.left=width+m_xpp[i]->distance;
				m_xpp[i]->rect.top=m_hMargin+xHeight;
				height = height > m_xpp[i]->rect.bottom ? height:m_xpp[i]->rect.bottom;
				width+=m_xpp[i]->rect.right+m_xpp[i]->distance;
			}
		}
	}

	if (m_bIsVertical==True)
	{
		width+=(m_wMargin<<1);
		height+=m_hMargin;
	}
	else
	{
		width+=m_wMargin;//(m_wMargin<<1);
		height+=(m_hMargin<<1);
	}

	if (m_bmp)
	{
		delete m_bmp;
	}


	if (m_bgFixed==False)
	{
		xHeight+=height;
		xWidth = xWidth > width ? xWidth:width;

		m_bmp = new Bitmap((UINT)xWidth, (UINT)xHeight);
	}
	else
	{
		m_bmp = new Bitmap((UINT)m_w, (UINT)m_h);
	}

	if (m_g)
	{
		delete m_g;
	}
	m_g = new Graphics(m_bmp);

	m_g->SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
	m_g->SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	// 背景描画
	if (m_bgFixed==True)
	{
		POINT pt = {xWidth, xHeight};
		m_xdraw(0, m_g, 0, (sControlx*)&pt, eCE_BACKGROUND);
	}
	else
	{
		POINT pt = {xWidth, xHeight};
		m_xdraw(0, m_g, 0, (sControlx*)&pt, eCE_BACKGROUND);
	}

	if (m_bIsVertical==True)
	{
		for (int i=0; i<n; i++)
		{
			m_xpp[i]->rect.right=width-(m_wMargin<<1);
			m_xdraw(m_xpp[i]->id, m_g, m_xfont, m_xpp[i], eCE_INIT);
		}
	}
	else
	{
		for (int i=0; i<n; i++)
		{
			m_xpp[i]->rect.bottom=height-(m_hMargin<<1);
			m_xdraw(m_xpp[i]->id,  m_g, m_xfont, m_xpp[i], eCE_INIT);
		}
	}

	if (m_bgFixed==False)
	{
		ReSize(xWidth, xHeight);
		//SetWindowPos(m_hWnd, 0, 0, 0, width, height, SWP_NOMOVE|SWP_NOZORDER);
	}

	InvalidateRect(m_hWnd, 0, TRUE);

	if (bIsShow)
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}
	else
	{
		ShowWindow(m_hWnd, SW_HIDE);
	}

	yiMutexUnlock(m_hMutex);

	return ;

}

// -----------------------------
//void cyctrlx::UpdateRotate(int nLotate, int index, Bool bIsShow, unsigned int wmax, unsigned int hmax)
//{
//	int width=m_wMargin;
//	int height=m_hMargin;
//	int n = m_x.count();
//	int oc_cnt=0;
//	int xHeight=0;
//	int xWidth=0;
//	float avwidth;
//
//	if (n==0)
//		return ;
//
//	yiMutexLock(m_hMutex);
//
//	if (m_xpp!=0)
//	{
//		m_x.DeleteArray((void**)m_xpp);
//	}
//
//	m_xpp = (sControlx**)m_x.ExportArray();
//
//	for (int i=0; i<n; i++)
//	{
//		width+=m_xpp[i]->rect.right;
//		height = height > m_xpp[i]->rect.bottom ? height:m_xpp[i]->rect.bottom;
//	}
//
//	width+=m_wMargin;//(m_wMargin<<1);
//	height+=(m_hMargin<<1);
//
//	if (width>m_max)
//	{
//		avwidth=width/n;
//
//		float sum=0;
//		int   cnt=0;
//		for (int i=0; i<n; i++)
//		{
//			if ((float)m_xpp[i]->rect.right>avwidth)
//			{
//				cnt++;
//				sum+=m_xpp[i]->rect.right;
//			}
//		}
//
//		avwidth=sum/cnt;
//		bIsReduced=True;
//
//		width=0;
//		for (int i=0; i<n; i++)
//		{
//			if (m_xpp[i]->rect.right>(INT)avwidth)
//			{
//				m_xpp[i]->rect.right=(INT)avwidth;
//				width+=m_xpp[i]->rect.right;
//			}
//			m_xpp[i]->rect.bottom=(INT)height;
//		}
//	}
//
//
//	if (m_bmpx)
//	{
//		delete m_bmpx;
//	}
//	m_bmpx = new Bitmap((UINT)width, (UINT)height);
//
//	if (m_gx)
//	{
//		delete m_gx;
//	}
//
//	if (nRotate==1 ||
//			nRotate==3)
//	{
//		m_bCoordChanged=True;
//	}
//	else
//	{
//		m_bCoordChanged=False;
//	}
//
//	m_gx = new Graphics(m_bmp);
//
//	m_gx->SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
//	m_gx->SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
//
//	//// 背景描画
//	//if (m_bgFixed==True)
//	//{
//	//	POINT pt = {xWidth, xHeight};
//	//	m_xdraw(0, m_g, 0, (sControlx*)&pt, eCE_BACKGROUND);
//	//}
//	//else
//	//{
//	//	POINT pt = {xWidth, xHeight};
//	//	m_xdraw(0, m_g, 0, (sControlx*)&pt, eCE_BACKGROUND);
//	//}
//
//
//	for (int i=0; i<n; i++)
//	{
//		m_xpp[i]->rect.bottom=height-(m_hMargin<<1);
//		m_xdraw(m_xpp[i]->id,  m_gx, m_xfont, m_xpp[i], eCE_INIT);
//	}
//
//	cyiBitmapBlender::RotateCopy(m_bmp, m_bmpx, nLotate, 0, 0, m_xpp[index]->rect.left, width<m_w?width:m_w, m_h);
//	m_nStartIndex=index;
//
//	InvalidateRect(m_hWnd, 0, TRUE);
//
//	if (bIsShow)
//	{
//		ShowWindow(m_hWnd, SW_SHOW);
//	}
//	else
//	{
//		ShowWindow(m_hWnd, SW_HIDE);
//	}
//
//	yiMutexUnlock(m_hMutex);
//
//	return ;
//
//}

// -----------------------------
//void cyctrlx::Scroll(int index)
//{
//	yiMutexLock(m_hMutex);
//	int n = m_x.count();
//
//	if (index>=n || index<0)
//	{
//		yiMutexLock(m_hMutex);
//		return ;
//	}
//
//	cyiBitmapBlender::RotateCopy(m_bmp, m_bmpx, nLotate, 0, 0, m_xpp[index]->rect.left, width<m_w?width:m_w, m_h);
//	m_nStartIndex=index;
//	InvalidateRect(m_hWnd, 0, TRUE);
//
//	yiMutexUnlock(m_hMutex);
//}

// ---------------------------------------
BOOL cyctrlx::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	yiMutexLock(m_hMutex);
	Graphics g(hdc);
	g.DrawImage(m_bmp, 0, 0);
	yiMutexUnlock(m_hMutex);

	return TRUE;
}

// ---------------------------------------
LRESULT cyctrlx::OnClicked(WPARAM wp, LPARAM lp)
{
	POINT pos;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	int n;

	if (m_xpp==0)
	{
		return 0;
	}
	
	n = m_x.count();

	for (int i=0; i<n; i++)
	{
		if (xIsInRect(&m_xpp[i]->rect, xPos, yPos, m_bCoordChanged)==TRUE)
		{
			if (m_xpp[i]->bIsEnable==False)
			{
				break;
			}

			SendMessage(m_hParentWnd, WM_COMMAND, m_xpp[i]->id, 0);
			break;
		}
	}
	return 0;
}

// ---------------------------------------
LRESULT cyctrlx::OnMouseLButtonDown(WPARAM wp, LPARAM lp, BOOL& handled)
{
	POINT pos;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	int n;

	if (m_xpp==0)
	{
		return 0;
	}
	
	n = m_x.count();

	for (int i=0; i<n; i++)
	{
		if (xIsInRect(&m_xpp[i]->rect, xPos, yPos)==TRUE)
		{
			//if (m_selected_item==m_xpp[i])
			//{
			//	break;
			//}

			if (m_xpp[i]->bIsEnable==False)
			{
				break;
			}

			if (m_selected_item)
			{
				if (m_xdraw(m_selected_item->id, m_g, m_xfont, m_selected_item, eCE_MOUSE_LUPX)==True)
				{
					xInvalidateRect(m_hWnd, &m_selected_item->rect, FALSE);
				}
			}
			if (m_xdraw(m_xpp[i]->id, m_g, m_xfont, m_xpp[i], eCE_MOUSE_LDOWN)==True)
			{
				xInvalidateRect(m_hWnd, &m_xpp[i]->rect, FALSE);
				m_selected_item = m_xpp[i];
			}

			break;
		}
	}

	return 0;
}

// ---------------------------------------
LRESULT cyctrlx::OnMouseLButtonUp(WPARAM wp, LPARAM lp, BOOL& handled)
{
	POINT pos;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	int n;

	if (m_xpp==0)
	{
		return 0;
	}
	
	n = m_x.count();

	for (int i=0; i<n; i++)
	{
		if (xIsInRect(&m_xpp[i]->rect, xPos, yPos)==TRUE)
		{
			//if (m_selected_item==m_xpp[i])
			//{
			//	break;
			//}
			if (m_xpp[i]->bIsEnable==False)
			{
				break;
			}

			if (m_xdraw(m_xpp[i]->id, m_g, m_xfont, m_xpp[i], eCE_MOUSE_LUP)==True)
			{
				xInvalidateRect(m_hWnd, &m_xpp[i]->rect, FALSE);
				m_selected_item = 0;//m_xpp[i];
			}

			break;
		}
	}

	return 0;
}

// ---------------------------------------
LRESULT cyctrlx::OnMouseMove(WPARAM wp, LPARAM lp, BOOL& handled)
{
	POINT pos;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	int n;

	if (m_xpp==0)
	{
		return 0;
	}
	
	n = m_x.count();

	for (int i=0; i<n; i++)
	{
		if (xIsInRect(&m_xpp[i]->rect, xPos, yPos)==TRUE)
		{
			if (m_xpp[i]->bIsEnable==False)
			{
				if (m_selected_item)
				{
					if (m_xdraw(m_selected_item->id, m_g, m_xfont, m_selected_item, eCE_MOUSE_LEAVE)==True)
					{
						xInvalidateRect(m_hWnd, &m_selected_item->rect, FALSE);
						m_selected_item=0;
					}
				}

				break;
			}
			if (m_selected_item==m_xpp[i])
			{
				break;
			}

			if (m_selected_item)
			{
				if (m_xdraw(m_selected_item->id, m_g, m_xfont, m_selected_item, eCE_MOUSE_LEAVE)==True)
				{
					xInvalidateRect(m_hWnd, &m_selected_item->rect, FALSE);
				}
			}
			if (m_xdraw(m_xpp[i]->id, m_g, m_xfont, m_xpp[i], eCE_MOUSE_IN)==True)
			{
				xInvalidateRect(m_hWnd, &m_xpp[i]->rect, FALSE);
				m_selected_item = m_xpp[i];
			}

			break;
		}
	}

	return 0;

}

// -------------------------
LRESULT cyctrlx::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	sControlx* btn = m_selected_item;
	
	if (btn==0)
		return 0;

	if (m_xdraw(btn->id, m_g, m_xfont, btn, eCE_MOUSE_LEAVE)==True)
	{
		m_selected_item = 0;
		xInvalidateRect(m_hWnd, &btn->rect, FALSE);
	}
	return 0;

}

// -----------------------------
void cyctrlx::EnableItem(int id, Bool bIsEnable)
{
	int n = m_x.count();

	for (int i=0; i<n; i++)
	{
		if (m_xpp[i]->id==id)
		{
			m_xpp[i]->bIsEnable=bIsEnable;
			if (m_xdraw(m_xpp[i]->id, m_g, m_xfont, m_xpp[i], bIsEnable ? eCE_INIT:eCE_DISABLE)==True)
			{
				xInvalidateRect(m_hWnd, &m_xpp[i]->rect, FALSE);
				break;
			}
			
		}
	}
}

