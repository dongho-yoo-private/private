#include "cyctrlxII.h"
#include "yiBitmap.h"

cyctrlxII::cyctrlxII(void)
: cyctrlbase()
{
	m_xpp=0;
}

cyctrlxII::~cyctrlxII(void)
{
	Destroy();
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
	//InvalidateRect(hWnd, &xrect, bEarse);
	InvalidateRect(hWnd, NULL, bEarse);
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
Bool cyctrlxII::Create(cyctrlbase* base, int id, int x, int y, int w_margin, int h_margin, BOOL bIsHide, Font* fnt, BOOL bIsVertical, unsigned int w, unsigned int h)
{
	m_wMargin=w_margin;
	m_hMargin=h_margin;

	m_xfont = fnt;

	m_bIsVertical = bIsVertical;
	m_id=id;
	//m_xpp=0;
	//m_bmp= new Bitmap(2, 2, PixelFormat32bppARGB);
	//m_g= new Graphics(m_bmp);
	//m_g->SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
	//m_g->SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	m_selected_item=0;
	//m_ToolTipMode=False;
	m_bgFixed=False;

	if (w!=0 || h!=0)
	{
		m_bgFixed=True;
	}

	m_bCoordChanged=False;
	m_hMutex = yiMutexCreate(0);

	m_parent=base;
	m_id = id;

	if (base==0)
	{
		return True;
	}

	if (w==0)
		w=5;
	if (h==0)
		h=5;
	if (cyctrlbase::Create(base->m_hWnd, 0, 0, x, y, w, h, bIsHide)==False)
	{
		return False;
	}

	return True;
}

// --------------------------------------------
Bool cyctrlxII::Create(cyctrlbase* base, int x, int y)
{
	int w, h;
	ArrangeItems(w, h);

	m_parent=base;
	return cyctrlbase::Create(base->m_hWnd, 0, 0, x, y, w, h, True);
}

// --------------------------------------------
static void xOnDelete(void* p)
{
	sControlxII* s = (sControlxII*)p;

	if (s)
	{
		for (int i=5; i<10; i++)
		{
			if (s->pBmp[i])
				delete s->pBmp[i];
		}
	}
}

// --------------------------------------------
void cyctrlxII::Destroy()
{
	//if (m_xfont)
	//	delete m_xfont;

	if (m_xpp)
		m_xx.DeleteArray((void**)m_xpp);

	m_xx.clear(xOnDelete);

	if (m_parent)
		cyctrlbase::Destroy();
}



// --------------------------------------------
void cyctrlxII::GetBmpPos(sControlxII* s, float* x, float* y, UINT _w, UINT _h, eContorlxAlignII align)
{
	int n_bmp_w = s->bmp ? s->bmp->GetWidth():0;
	int n_bmp_h = s->bmp ? s->bmp->GetHeight():0;

	if (align&eCXAII_CENTER)
	{
		float w = (float)(_w);
		*x = ((w-s->txt_w-(float)n_bmp_w)/2);
	}
	else if (align&eCXAII_LEFT)
	{
		float w = (float)(_w);
		*x = s->rect.left;
	}
	else
	{
		*x=((s->rect.right)-s->txt_w-(float)n_bmp_w);
	}

	if (align&eCXAII_VCENTER)
	{
		float h = (float)(_h);
		*y = ((h-n_bmp_h)/2);
	}
	else if (align&eCXAII_TOP)
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
void cyctrlxII::GetTextPos(sControlxII* s, float* x, float* y, eContorlxAlignII align)
{
	if (align&eCXAII_CENTER)
	{
		float w = (float)(s->rect.right);
		*x = ((w-s->txt_w)/2)+s->rect.left;
	}
	else if (align&eCXAII_LEFT)
	{
		float w = (float)(s->rect.right);
		*x = s->rect.left;
	}
	else
	{
		*x=(s->rect.right-s->rect.left)-s->txt_w;
	}

	if (align&eCXAII_VCENTER)
	{
		float h = (float)(s->rect.bottom);
		*y = ((h-s->txt_h)/2)+s->rect.top+1;
	}
	else if (align&eCXAII_TOP)
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
void cyctrlxII::GetTextPos(sControlxII* s, float* x, float* y, UINT _w, UINT _h, eContorlxAlignII align)
{
	int n_bmp_w = s->bmp ? s->bmp->GetWidth():0;
	int n_bmp_h = s->bmp ? s->bmp->GetHeight():0;

	if (align&eCXAII_CENTER)
	{
		float w = (float)(_w);
		*x = (((w-s->txt_w-(float)n_bmp_w)/2)+n_bmp_w);
	}
	else if (align&eCXAII_LEFT)
	{
		float w = (float)(_w);
		*x = s->rect.left+n_bmp_w;
	}
	else
	{
		*x=(s->rect.right)-s->txt_w;
	}

	if (align&eCXAII_VCENTER)
	{
		float h = (float)(_h);
		*y = ((h-s->txt_h)/2);
	}
	else if (align&eCXAII_TOP)
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
int cyctrlxII::Add(int id, Bitmap* bmp, const wchar_t* pszText, int distance, 
				 int marginX, int marginY, void* pParam, const wchar_t* pszToolTip, Bool bIsFixedMode)
{
	sControlxII data;
	memset(&data, 0, sizeof(data));
	float h=0.0;

	data.id=id;
	data.distance=distance;
	data.bmp=bmp;

	data.rect.right = marginX;
	data.rect.bottom = marginY;
	data.p=pParam;
	data.bIsEnable=True;

	if (m_parent->OnMakeItem(m_id, id, m_xfont, &data)==True)
	{
		goto SKIP_MAKE_SIZE;
	}

	if (data.bmp!=0)
	{
		data.rect.right+=bmp->GetWidth();
		h = h<(float)bmp->GetHeight() ? (float)bmp->GetHeight():h;
	}

	if (pszText!=0)
	{
		data.txt_w = 1024;
		data.txt_h = 30;

		if (pszText[0]!=0)
		{
			cyiShape::GetTextSize(m_xfont, pszText, data.txt_w, data.txt_h);

			data.rect.right += ((INT)data.txt_w+2);

			h = h<data.txt_h ? data.txt_h:h;
			wcscpy(data.name, pszText);
		}
	}

	if (pszToolTip!=0)
	{
		//m_ToolTipMode=True;
		//xCreateToolTip();
	}

	data.rect.bottom+= h+2;

SKIP_MAKE_SIZE:

	if (bIsFixedMode)
	{
		data.w_fixed=marginX;
		data.h_fixed=marginY;

		data.rect.right=data.w_fixed;
		data.rect.bottom=data.h_fixed;
	}

	yiMutexLock(m_hMutex);
	m_xx.addref(yitoheap(&data, sizeof(data)));
	yiMutexUnlock(m_hMutex);

	return True;
}

// -----------------------------
int cyctrlxII::Add(int id, cyctrlbase* ctrl, int distance, int marginX, int marginY, void* pParam)
{
	sControlxII data;
	memset(&data, 0, sizeof(data));
	float h=0.0;

	data.ctrl=ctrl;
	data.id=id;
	data.distance=distance;

	data.rect.right = ctrl->m_w+marginX;
	data.rect.bottom = ctrl->m_h+marginY;
	data.p=pParam;
	data.bIsEnable=True;

	if (m_parent->OnMakeItem(m_id, id, m_xfont, &data)==True)
	{
		goto SKIP_MAKE_SIZE;
	}

	data.rect.bottom+= h+2;

SKIP_MAKE_SIZE:

	//if (bIsFixedMode)
	//{
	//	data.w_fixed=marginX;
	//	data.h_fixed=marginY;

	//	data.rect.right=data.w_fixed;
	//	data.rect.bottom=data.h_fixed;
	//}

	yiMutexLock(m_hMutex);
	m_xx.addref(yitoheap(&data, sizeof(data)));
	yiMutexUnlock(m_hMutex);

	return True;
}


// -----------------------------
int cyctrlxII::Addx(int id, Bitmap* bmp, const wchar_t* pszText, int distance, 
				 int marginX, int marginY, int maxw, int minw, void* pParam, const wchar_t* pszToolTip)
{
	sControlxII data;
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
		data.txt_w = 1024;
		data.txt_h = 30;
		cyiShape::GetTextSize(m_xfont, pszText, data.txt_w, data.txt_h);

		data.rect.right += ((INT)data.txt_w+2);
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
	m_xx.addref(yitoheap(&data, sizeof(data)));
	yiMutexUnlock(m_hMutex);

	return True;
}

// -----------------------------
static void xDestroyx(sControlxII* x)
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
void cyctrlxII::Remove(unsigned int id)
{
	int width=m_wMargin;
	int height=m_hMargin;
	int n = m_xx.count();
	int oc_cnt=0;
	int xHeight=0;
	int xWidth=0;

	yiMutexLock(m_hMutex);

	for (cyiNode* p = m_xx.GetNodeTopFrom(0); p!=0; p=p->np)
	{
		sControlxII* x = (sControlxII*)p->ref;
		if (id==x->id)
		{
			xDestroyx(x);
			m_xx.DeleteArray((void**)m_xpp);
			m_xpp=0;
			m_xx.remove(p);

			break;
		}
	}

	yiMutexUnlock(m_hMutex);
}


// -----------------------------
Bool cyctrlxII::ArrangeItems(int&x, int&y, int wmax, int hmax)
{
	int width=m_wMargin;
	int height=m_hMargin;
	int xHeight=0;
	int xWidth=0;
	int oc_cnt=0;
	int n = m_xx.count();

	yiMutexLock(m_hMutex);

	if (m_xpp!=0)
	{
		m_xx.DeleteArray((void**)m_xpp);
	}

	m_xpp = (sControlxII**)m_xx.ExportArray();

	// 縦型の場合
	if (m_bIsVertical==True)
	{
		for (int i=0; i<n; i++)
		{
			m_xpp[i]->rect.left = m_wMargin; 
			m_xpp[i]->rect.top	= height+m_xpp[i]->distance;

			// アイテムがコントロールの場合
			if (m_xpp[i]->ctrl)
			{
				m_xpp[i]->ctrl->Create(this, m_xpp[i]->rect.left, m_xpp[i]->rect.top);
				//m_xpp[i]->ctrl->Move(m_xpp[i]->rect.left, m_xpp[i]->rect.top);
			}

			height+=m_xpp[i]->rect.bottom+m_xpp[i]->distance;
			width = width > m_xpp[i]->rect.right ? width:m_xpp[i]->rect.right;
		}
	}
	// 横型の場合
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

			// アイテムがコントロールの場合
			if (m_xpp[i]->ctrl)
			{
				//m_xpp[i]->ctrl->Move(m_xpp[i]->rect.left, m_xpp[i]->rect.top);
				m_xpp[i]->ctrl->Create(this, m_xpp[i]->rect.left, m_xpp[i]->rect.top);
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

	// 全体の背景のサイズが固定の場合、
	if (m_bgFixed==False)
	{
		xHeight+=height;
		xWidth = xWidth > width ? xWidth:width;

		//m_OffBmp = new cyiSurface((UINT)xWidth, (UINT)xHeight);
	}
	
	x=xWidth;
	y=xHeight;
	ReSize(xWidth, xHeight);
	yiMutexUnlock(m_hMutex);
	return True;
}

// -----------------------------
void cyctrlxII::Update(Bool bIsShow, Bool bIsArrange, unsigned int wmax, unsigned int hmax)
{
	int n = m_xx.count();
	int xHeight=m_h;
	int xWidth=m_w;

	if (n==0)
		return ;

	if (bIsArrange)
	{
		ArrangeItems(xWidth, xHeight, wmax, hmax);
	}

	yiMutexLock(m_hMutex);

	if (bIsArrange==False)
	{
		if (m_xpp!=0)
		{
			m_xx.DeleteArray((void**)m_xpp);
		}

		m_xpp = (sControlxII**)m_xx.ExportArray();
	}
	
	if (m_OffBmp!=0)
	{
		//if (m_OffBmp->GetWidth()!=xWidth ||
		//		m_OffBmp->GetHeight()!=xHeight)
		{
			delete m_OffBmp;
			m_OffBmp=new Bitmap(xWidth, xHeight, PixelFormat32bppARGB);
		}
	}
	else
	{
		m_OffBmp = new Bitmap(xWidth, xHeight, PixelFormat32bppARGB);
	}

	m_parent->OnCtrlxBgDraw(m_id, m_OffBmp, m_x, m_y);
	
	if (m_bgBmp)
	{
		delete m_bgBmp;
	}

	m_bgBmp = m_OffBmp->Clone(0, 0, xWidth, xHeight, PixelFormat32bppARGB);

	if (m_bIsVertical==True)
	{
		for (int i=0; i<n; i++)
		{

			m_xpp[i]->rect.right=xWidth-(m_wMargin<<1);
			if (m_xpp[i]->ctrl)
			{
				//m_xpp[i]->ctrl->OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_INIT);
				m_xpp[i]->ctrl->Show();
				continue;
			}
			m_parent->OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_NORMAL);
		}
	}
	else
	{
		for (int i=0; i<n; i++)
		{
			m_xpp[i]->rect.bottom=xHeight-(m_hMargin<<1);
			if (m_xpp[i]->ctrl)
			{
				//m_xpp[i]->ctrl->OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_INIT);
				m_xpp[i]->ctrl->Show();
				continue;
			}
			m_parent->OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_NORMAL);
		}
	}

	if (bIsShow)
	{
		Show();
		InvalidateRect(m_hWnd, 0, TRUE);
		UpdateWindow(m_hWnd);
	}
	else
	{
		Hide();
	}

	yiMutexUnlock(m_hMutex);

	return ;

}


// ---------------------------------------
BOOL cyctrlxII::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	yiMutexLock(m_hMutex);
	//m_OffBmp->Draw(hdc, ps->rcPaint.left, ps->rcPaint.top, ps->rcPaint.right, ps->rcPaint.bottom);
	//m_OffBmp->Draw(hdc, 0, 0);
	//Bitmap* bmp = m_OffBmp->export_bmp(ps->rcPaint.left, ps->rcPaint.top, ps->rcPaint.right, ps->rcPaint.bottom);
	//Graphics g(hdc);
	//g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	//g.DrawImage(bmp, 0, 0);
	Graphics g(hdc);

	Bitmap xbmp(m_bgBmp->GetWidth(), m_bgBmp->GetHeight(), PixelFormat32bppARGB);
	Graphics gx(&xbmp);

	gx.SetSmoothingMode(SmoothingMode::SmoothingModeHighSpeed);
	gx.DrawImage(m_bgBmp, 0, 0);
	gx.DrawImage(m_OffBmp, 0, 0);

	g.DrawImage(&xbmp, 0, 0);
	//g.DrawImage(m_OffBmp, 0, 0);
	yiMutexUnlock(m_hMutex);

	return TRUE;
}


// ---------------------------------------
LRESULT cyctrlxII::OnClicked(WPARAM wp, LPARAM lp)
{
	POINT pos;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	int n;

	if (m_xpp==0)
	{
		return 0;
	}
	
	n = m_xx.count();

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
LRESULT cyctrlxII::OnMouseLButtonDown(WPARAM wp, LPARAM lp, BOOL& handled)
{
	POINT pos;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	int n;

	if (m_xpp==0)
	{
		return 0;
	}
	
	n = m_xx.count();

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
				if (m_parent->OnCtrlxDraw(m_id, m_selected_item->id, m_OffBmp, m_bgBmp, m_xfont, m_selected_item, eCEII_MOUSE_LUPX)==True)
				//if (m_xxdraw(m_selected_item->id, m_g, m_xfont, m_selected_item, eCEII_MOUSE_LUPX)==True)
				{
					xInvalidateRect(m_hWnd, &m_selected_item->rect, FALSE);
				}
			}

			if (m_parent->OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_MOUSE_LDOWN)==True)
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
LRESULT cyctrlxII::OnMouseLButtonUp(WPARAM wp, LPARAM lp, BOOL& handled)
{
	POINT pos;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	int n;

	if (m_xpp==0)
	{
		return 0;
	}
	
	n = m_xx.count();

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

			if (m_parent->OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_MOUSE_LUP)==True)
			//if (m_xxdraw(m_xpp[i]->id, m_g, m_xfont, m_xpp[i], eCEII_MOUSE_LUP)==True)
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
LRESULT cyctrlxII::OnMouseMove(WPARAM wp, LPARAM lp, BOOL& handled)
{
	POINT pos;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	int n;

	if (m_xpp==0)
	{
		return 0;
	}
	
	n = m_xx.count();

	for (int i=0; i<n; i++)
	{
		if (xIsInRect(&m_xpp[i]->rect, xPos, yPos)==TRUE)
		{
			if (m_xpp[i]->bIsEnable==False)
			{
				if (m_selected_item)
				{
					if (m_parent->OnCtrlxDraw(m_id, m_selected_item->id, m_OffBmp, m_bgBmp, m_xfont, m_selected_item, eCEII_MOUSE_LEAVE)==True)
					//if (m_xxdraw(m_selected_item->id, m_g, m_xfont, m_selected_item, eCEII_MOUSE_LEAVE)==True)
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
				if (m_parent->OnCtrlxDraw(m_id, m_selected_item->id, m_OffBmp, m_bgBmp, m_xfont, m_selected_item, eCEII_MOUSE_LEAVE)==True)
				//if (m_xxdraw(m_selected_item->id, m_g, m_xfont, m_selected_item, eCEII_MOUSE_LEAVE)==True)
				{
					xInvalidateRect(m_hWnd, &m_selected_item->rect, FALSE);
				}
			}
			if (m_parent->OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_MOUSE_IN)==True)
			//if (m_xxdraw(m_xpp[i]->id, m_g, m_xfont, m_xpp[i], eCEII_MOUSE_IN)==True)
			{
				xInvalidateRect(m_hWnd, &m_xpp[i]->rect, FALSE);
				m_selected_item = m_xpp[i];

				if (m_parent)
				{
					SendMessage(m_parent->m_hWnd, WM_ITEM_MOUSE_ENTER, m_xpp[i]->id, 0);
				}
			}

			break;
		}
	}

	return 0;

}

// -------------------------
LRESULT cyctrlxII::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	sControlxII* btn = m_selected_item;
	
	if (btn==0)
		return 0;

	if (m_parent->OnCtrlxDraw(m_id, btn->id, m_OffBmp, m_bgBmp, m_xfont, btn, eCEII_MOUSE_LEAVE)==True)
	{
		m_selected_item = 0;
		xInvalidateRect(m_hWnd, &btn->rect, FALSE);
	}
	return 0;

}

// -----------------------------
void cyctrlxII::EnableItem(int id, Bool bIsEnable)
{
	int n = m_xx.count();

	if (id==-1)
	{
		Bool bIsUpdate=False;
		for (int i=0; i<n; i++)
		{
			m_xpp[i]->bIsEnable=bIsEnable;

			if (m_xpp[i]->ctrl)
			{
				m_xpp[i]->ctrl->EnableItem(bIsEnable); //OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_DISABLE);
				continue;
			}
			else if (m_parent->OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_DISABLE)==True)
			{
				bIsUpdate=True;
			}
		}
		xInvalidateRect(m_hWnd, 0, FALSE);
		return ;
	}

	for (int i=0; i<n; i++)
	{
		if (m_xpp[i]->id==id)
		{
			m_xpp[i]->bIsEnable=bIsEnable;
			if (m_xpp[i]->ctrl)
			{
				m_xpp[i]->ctrl->EnableItem(bIsEnable); //OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_DISABLE);
				continue;
			}
			else if (m_parent->OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_DISABLE)==True)
			{
				xInvalidateRect(m_hWnd, &m_xpp[i]->rect, FALSE);
				break;
			}
			
		}
	}
}

// -----------------------------
int cyctrlxII::SelectionChange(int id)
{
	int n = m_xx.count();
	int ret = 0;//m_selected_item;

	for (int i=0; i<n; i++)
	{
		if (m_xpp[i]->id==id)
		{
			if (m_xpp[i]->bIsEnable==False)
			{
				if (m_selected_item)
				{
					if (m_parent->OnCtrlxDraw(m_id, m_selected_item->id, m_OffBmp, m_bgBmp, m_xfont, m_selected_item, eCEII_INIT)==True)
					//if (m_xxdraw(m_selected_item->id, m_g, m_xfont, m_selected_item, eCEII_MOUSE_LEAVE)==True)
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
				if (m_parent->OnCtrlxDraw(m_id, m_selected_item->id, m_OffBmp, m_bgBmp, m_xfont, m_selected_item, eCEII_UNSELECTED)==True)
				//if (m_xxdraw(m_selected_item->id, m_g, m_xfont, m_selected_item, eCEII_MOUSE_LEAVE)==True)
				{
					xInvalidateRect(m_hWnd, &m_selected_item->rect, FALSE);
				}
			}
			if (m_parent->OnCtrlxDraw(m_id, m_xpp[i]->id, m_OffBmp, m_bgBmp, m_xfont, m_xpp[i], eCEII_SELECTED)==True)
			//if (m_xxdraw(m_xpp[i]->id, m_g, m_xfont, m_xpp[i], eCEII_MOUSE_IN)==True)
			{
				xInvalidateRect(m_hWnd, &m_xpp[i]->rect, FALSE);
				m_selected_item = m_xpp[i];
			}

			break;
		}
	}

	return ret;
}