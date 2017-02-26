#include "glsImageBox.h"

cglsImageBox::cglsImageBox(void)
{
	m_szImagePath[0]=0;
	m_nCurrentSelectedId=-1;
	m_listBitmaps=0;
}

cglsImageBox::~cglsImageBox(void)
{
}
// ----------------------------------
void cglsImageBox::xDefaultDetail(sGlsImageBoxDetail* detail)
{
	cGlassInterface::xDefaultDetail(&detail->gls);
	detail->bIsBorderDropShadow=False;
	detail->bIsClickEvent=False;
	detail->bIsDragAndDrop=True;
	detail->eInitialStyle=eGIBVS_TRIM_BORDER_FIXED;
	detail->bHasBorder=True;
	detail->pen=0;
	detail->bAutoResize=False;
	detail->bIsImageListMode=False;
	detail->bIsMouseParent=False;
}

enum eGlsImageBoxFlag {
	eGIBF_BORDER_DROPSHADOW	=1<<1,
	eGIBF_HAS_BORDER		=1<<2,
	eGIBF_CLICK_EVENT		=1<<3,
	eGIBF_DRAG_AND_DROP		=1<<4,
	eGIBF_AUTO_RESIZE		=1<<5,
	eGIBF_IMAGE_LIST		=1<<6,
	eGIBF_ONMOUSE_PARENT	=1<<7
};


// ----------------------------------
Bool cglsImageBox::Create(cGlassInterface* parent, int id, int x, int y, int w, int h, Bitmap* bmp, sGlsImageBoxDetail* pdetail)
{
	sGlsImageBoxDetail detail;

	if (pdetail)
	{
		detail=*pdetail;
	}
	else
	{
		xDefaultDetail(&detail);
	}

	xSET_GLS_FLAG(m_myFlag, eGIBF_BORDER_DROPSHADOW, detail.bIsBorderDropShadow);
	xSET_GLS_FLAG(m_myFlag, eGIBF_HAS_BORDER, detail.bHasBorder);
	xSET_GLS_FLAG(m_myFlag, eGIBF_CLICK_EVENT, detail.bIsClickEvent);
	xSET_GLS_FLAG(m_myFlag, eGIBF_DRAG_AND_DROP, detail.bIsDragAndDrop);
	xSET_GLS_FLAG(m_myFlag, eGIBF_AUTO_RESIZE, detail.bAutoResize);
	xSET_GLS_FLAG(m_myFlag, eGIBF_IMAGE_LIST, detail.bIsImageListMode);
	xSET_GLS_FLAG(m_myFlag, eGIBF_ONMOUSE_PARENT, detail.bIsMouseParent);

	//if (detail.bIsMouseParent==True)
	//	m_bMouseTrack=False;

	m_eStyle = detail.eInitialStyle;
	m_pen = detail.pen;

	if (m_pen==0)
	{
		if (detail.bHasBorder==True)
		{
			m_pen = new Pen(Color::Black);
		}
	}

	if (bmp==0)
	{
		bmp = BITMAP32(w, h);
		Graphics g(bmp);
		g.DrawRectangle(&Pen(Color::Black), 0, 0, w-1, h-1);
		m_item.pBmp[0]=bmp;
	}
	else
	{
		m_item.pBmp[0]=cyiBitmapBlender::BitmapCopy(0, bmp);
	}

	RECT rect = {x, y, bmp->GetWidth(), bmp->GetHeight()};
	m_item.pCurrentBmp=m_item.pBmp[0];
	m_item.rect.w=rect.right;
	m_item.rect.h=rect.bottom;

	if (bmp==0)
	{
		if (xIS_GLS_FLAG(m_myFlag, eGIBF_DRAG_AND_DROP)==True)
		{
			xDrawDragAndDrop();
		}
	}

	detail.gls.bIsNoInput=(detail.bIsClickEvent==False);
	detail.gls.bIsNoMouseMove=True;//detail.gls.bIsNoInput;

	Bool res = cGlassInterface::Create((HWND)parent, 0, id, &rect, &detail.gls);
	if (xIS_GLS_FLAG(m_myFlag, eGIBF_DRAG_AND_DROP)==True)
	{
		// ドラッグアンドドロップを有効
		DragAcceptFiles(m_hWnd, TRUE);
	}

	if (res==True)
	{
		Show();
		InvalidateRect(0);
	}
	
	return res;
}

// ----------------------------------
void cglsImageBox::Destroy()
{
	__super::Destroy();
}

// ----------------------------------
void cglsImageBox::SetImage(wchar_t* pszImagePath, eGlsImageBoxViewStyle style)
{
	Bool bIsDir=False;

	Image img(pszImagePath);

	if (img.GetLastStatus()!=Status::Ok)
	{
		return ;
	}

	wcscpy(m_szImagePath, pszImagePath);

	SetImage((Bitmap*)&img, style);
}


// ----------------------------------
Bitmap* cglsImageBox::CreateImage(Bitmap* dest, Bitmap* bmp, int _w, int _h, eGlsImageBoxViewStyle style)
{
	Bool bDrawBorder=(dest!=0);

	if (bmp==0)
	{
		//if (m_item.pBmp[0])
		//	delete m_item.pBmp[0];
		if (m_item.pBmp[0])
		{
			Graphics g(m_item.pBmp[0]);
			g.Clear(0);
			//if (bDrawBorder==True)
			//{
			if (m_pen)
				g.DrawRectangle(m_pen, 0, 0, m_w-(INT)m_pen->GetWidth(), m_h-(INT)m_pen->GetWidth());
			//}
		}
		return m_item.pBmp[0];
	}

	if (style==eGIBVS_TRIM_BORDER_FIXED)
	{
		int w = bmp->GetWidth();
		int h = bmp->GetHeight();

		float factorW = (float)_w/(float)w;
		float factorH = (float)_h/(float)h;
		float factor = factorW>factorH?factorH:factorW;
		
		int w2 = (int)((float)w*factor);
		int h2 = (int)((float)h*factor);

		if (dest==0)
		{
			_w=w2;
			_h=h2;
			dest = BITMAP32(_w, _h);
		}

		Graphics g(dest);
		
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.Clear(0);


		int x = factorW>factorH?((_w-w2)>>1):0;
		int y = factorW>factorH?0:((_h-h2)>>1);

		g.DrawImage(bmp, x, y, w2, h2);

		if (bDrawBorder==True)
		{
			if (m_pen)
				g.DrawRectangle(m_pen, x, y, w2-(INT)m_pen->GetWidth(), h2-(INT)m_pen->GetWidth());
		}
	}
	else if (style==eGIBVS_NONE)
	{
		if (xIS_GLS_FLAG(m_myFlag, eGIBF_AUTO_RESIZE)==True)
		{

			if (ReSize(bmp->GetWidth(), bmp->GetHeight())==True)
			{
				//if (dest)
				//{
				//	delete dest;
				//	dest=0;
				//}
				dest=m_item.pBmp[0];
			}

			if (dest==0)
			{
				dest = BITMAP32(bmp->GetWidth(), bmp->GetHeight());
			}

			Graphics g(dest);

			g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
			g.Clear(0);
			g.DrawImage(bmp, 0, 0);
			
			if (bDrawBorder==True)
			{
				if (m_pen!=0)
					g.DrawRectangle(m_pen, 0, 0, m_w-(INT)m_pen->GetWidth(), m_h-(INT)m_pen->GetWidth());
			}
		}
		else
		{
			if (dest==0)
				dest = BITMAP32(_w, _h);
			Graphics g(dest);

			g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
			g.Clear(0);
			g.DrawImage(bmp, 0, 0);

			if (bDrawBorder==True)
			{
				if (m_pen!=0)
					g.DrawRectangle(m_pen, 0, 0, _w-(INT)m_pen->GetWidth(), _h-(INT)m_pen->GetWidth());
			}
		}
	}
	else
	{
		if (dest==0)
			dest = BITMAP32(_w, _h);
		Graphics g(dest);
		
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.Clear(0);
		g.DrawImage(bmp, 0, 0, _w, _h);

		if (bDrawBorder==True)
		{
			if (m_pen!=0)
				g.DrawRectangle(m_pen, 0, 0, _w-(INT)m_pen->GetWidth(), _h-(INT)m_pen->GetWidth());
		}
	}

	return dest;
}

// ----------------------------------
void cglsImageBox::SetImage(const char* pszImagePath, eGlsImageBoxViewStyle style)
{
	wchar_t wszImagePath[256];

	yiToUnicode(wszImagePath, pszImagePath, 0);
	SetImage(wszImagePath, style);
}
#include "yiStream.h"

// ----------------------------------
void cglsImageBox::SetImage(void* raw, int size, eGlsImageBoxViewStyle style)
{
	IO::Stream stream;
	stream.CreateOnMemory(raw, size);//(GetModuleHandle("yctrlGo.dll"), MAKEINTRESOURCE(IDB_GOBAN), "PNG");

	SetImage(&Bitmap(stream), style);
}


// ----------------------------------
void cglsImageBox::SetImage(Bitmap* bmp, eGlsImageBoxViewStyle style)
{
	m_item.pBmp[0]=CreateImage(m_item.pBmp[0], bmp, m_w, m_h, style);
	m_item.pCurrentBmp=m_item.pBmp[0];

	InvalidateRect(0);
}
	//eGIBVS_CENTER,	// 真中に置く。
	//eGIBVS_FIX,		// 伸ばす
	//eGIBVS_TILE,	// タイル
	//eGIBVS_TRIM,	// トリム
	//eGIBVS_TRIM_BORDER_FIXED	// 枠をイメージに合わす（サイズ固定）
// ----------------------------------
void cglsImageBox::xDrawDragAndDrop()
{
	const wchar_t* p = L"画像をドラッグ";
	//Bitmap * pBg = GetParentBg();
	Bitmap* pBg = cyiShape::BitmapStringEx(0, p, GetFont(), Color::Gray, Color::White, eSTS_NONE, DT_CENTER|DT_VCENTER, m_w);

	//cyiBitmapBlender::BitmapCopy(m_item.pBmp[0], pBg);
	Graphics g(m_item.pBmp[0]);

	if (m_eStyle==eGIBVS_CENTER)
	{
		g.Clear(Color::White);

	}
	
	g.DrawRectangle(&Pen(Color::Black), 0, 0, m_w-1, m_h-1);
	g.DrawImage(pBg, (INT)((m_w-pBg->GetWidth())>>1), (INT)((m_h-pBg->GetHeight())>>1));
	m_item.pCurrentBmp=m_item.pBmp[0];
	delete pBg;
	return ;
}


// ----------------------------------
Bool cglsImageBox::ReSize(UINT x, UINT y, Bool bIsRepaint)
{
	Bool res = cGlassInterface::ReSize(x, y, bIsRepaint);

	if (xIS_GLS_FLAG(m_myFlag, eGIBF_IMAGE_LIST)==True)
	{
		return res;
	}
	
	if (res==False)
		return False;

	if (m_item.pBmp[0])
		delete m_item.pBmp[0];
	m_item.pBmp[0]=BITMAP32(x, y);
	m_item.pCurrentBmp=m_item.pBmp[0];

	if (m_szImagePath[0]!=0)
	{
		Image img(m_szImagePath);

		if (img.GetLastStatus()==Status::Ok)
		{
			SetImage((Bitmap*)&img, m_eStyle);
			/*Graphics g(m_item.pBmp[0]);
			g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
			g.DrawImage(&img, 0, 0, x, y);
			g.DrawRectangle(&Pen(Color::Black), 0, 0, m_w-1, m_h-1);*/
		}
	}
	else
	{
		if (xIS_GLS_FLAG(m_myFlag, eGIBF_DRAG_AND_DROP)==True)
		{
			xDrawDragAndDrop();
		}
	}


	return res;
}

typedef struct {
	int id;
	Bitmap* bmp;
	Bool bDelete;
} sBitmaps;

// ----------------------------------
void cglsImageBox::ClearBitmaps()
{
	if (m_listBitmaps)
	{
		for (cyiNode* node=m_listBitmaps->GetNodeTopFrom(0); node; node=node->np)
		{
			sBitmaps* s = (sBitmaps*)node->ref;

			if (s)
			{
				if (s->bDelete==True)
				{
					delete s->bmp;
				}
				yifree(s);
			}
		}
		m_listBitmaps->clear();

	}

	m_item.pBmp[0]=0;
	m_item.pCurrentBmp=0;
	m_nCurrentSelectedId=-1;
}

// ----------------------------------
Bitmap* cglsImageBox::GetBitmap(int id)
{
	if (m_listBitmaps==0)
		return 0;

	sBitmaps* s = (sBitmaps*)m_listBitmaps->GetFromId(id);

	if (s==0)
		return 0;

	return s->bmp;
}

// ----------------------------------
Bitmap* cglsImageBox::SetBitmap(int id, Bitmap* bmp, Bool bDelete)
{
	Bitmap* bmpPrev=0;
	sBitmaps* s = m_listBitmaps==0?0:(sBitmaps*)m_listBitmaps->GetFromId(id);

	if (s==0)
	{
		AppendBitmap(id, bmp, bDelete);
		return 0;
	}
	
	//s->bDelete=bDelete;
	bmpPrev=s->bmp;
	if (s->bDelete)
	{
		//if (s->bmp)
		//	delete s->bmp;
	}
	s->bmp=bmp;

	return bmpPrev;
}


// ----------------------------------
void cglsImageBox::AppendBitmap(int id, Bitmap* bmp, Bool bDelete)
{

	if (m_listBitmaps==0)
	{
		m_listBitmaps = new cyiList();
	}

	if (GetBitmap(id)!=0)
		return ;

	sBitmaps s = {id, bmp, bDelete};

	m_listBitmaps->addref(yitoheap(&s, sizeof(s)));

	if (m_nCurrentSelectedId==-1)
	{
		SelectBitmap(id, False);
	}
}

// ----------------------------------
void cglsImageBox::SelectBitmap(int id, Bool bDrawDirect, Bool bIsForced)
{
	if (bIsForced==False &&
		m_nCurrentSelectedId==id)
		return ;

	if (m_listBitmaps==0)
		return ;

	sBitmaps* s = (sBitmaps*)m_listBitmaps->GetFromId(id);

	if (s)
	{
		m_item.pBmp[0]=s->bmp;
		m_item.pCurrentBmp=m_item.pBmp[0];
		ReSize(m_item.pBmp[0]->GetWidth(), m_item.pBmp[0]->GetHeight());

		InvalidateRect(0, 0, bDrawDirect);
		m_nCurrentSelectedId=id;
	}

}

// ----------------------------------
int cglsImageBox::GetSelectedBitmap()
{
	return m_nCurrentSelectedId;
}

// ----------------------------------
Bitmap* cglsImageBox::ExportBitmap(int w, int h)
{
	Image img(m_szImagePath);

	if (img.GetLastStatus()==Status::Ok)
	{
		return CreateImage(0, (Bitmap*)&img, w, h, m_eStyle);
	}
	return 0;
}

// ----------------------------------
LRESULT cglsImageBox::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (xIS_GLS_FLAG(m_myFlag, eGIBF_ONMOUSE_PARENT)==True)
	{
		m_parent->OnMouseLeave(wParam, lParam);
	}
	return 0;
}

// ----------------------------------
LRESULT cglsImageBox::OnMouse(int id, WPARAM wParam, LPARAM lParam, Bool& handled)
{
	handled=False;
	if (xIS_GLS_FLAG(m_myFlag, eGIBF_ONMOUSE_PARENT)==True)
	{
		handled=True;
		int x = lParam&0xFFFF;
		int y = lParam>>16;

		x+=m_x;
		y+=m_y;
		::SendMessage(*m_parent, id, wParam, (y<<16)|x);
	}

	return 0;
}

// ----------------------------------
LRESULT cglsImageBox::OnDropFiles(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	HDROP hDrop=(HDROP)wParam;
	char buffer[MAX_PATH];
	wchar_t bufferW[MAX_PATH];
	
	DragQueryFile(hDrop, 0, (LPSTR)buffer, MAX_PATH);
	DragFinish(hDrop);

	Image img(yiToUnicode(bufferW, buffer, 0));

	if (img.GetLastStatus()!=Status::Ok)
	{
		return 0;
	}

	wcscpy(m_szImagePath, bufferW);

	SetImage((Bitmap*)&img, m_eStyle);

	//Graphics g(m_item.pBmp[0]);
	//g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);


	//g.Clear(Color(0, 0, 0, 0));
	//g.DrawImage(&img, 0, 0, m_w, m_h);
	//g.DrawRectangle(&Pen(Color::Black), 0, 0, m_w-1, m_h-1);

	handled=True;

	InvalidateRect(0);

	::SendMessage(m_hNotifyWnd, WM_COMMAND, (WPARAM)m_id, (LPARAM)this);

	return 0;
}