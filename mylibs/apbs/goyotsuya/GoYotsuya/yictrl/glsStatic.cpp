#include "glsStatic.h"

cglsStatic::cglsStatic(void)
: m_icon(0),
  m_bmpStr(0),
  m_nBmpStrWidth(0),
  m_nBmpStrHeight(0),
  m_fontAuto(0),
  m_bg(0)
{
	__GLASSINTERFACE_NAME__;
}

cglsStatic::~cglsStatic(void)
{
}

cglsStatic* xcglsStatic()
{
	return new cglsStatic();
}

// --------------------------------
void cglsStatic::xDefaultDetail(sGlsStaticDetail* detail)
{
	cGlassInterface::xDefaultDetail(&detail->gls);
	detail->gls.bIsNoInput=True;
	detail->gls.bIsNoMouseMove=True;
	detail->colorBg=0;
	detail->colorFont=Color::Black;
	detail->bIsMultiLine=False;
	detail->bIsAutoFontSize=False;
	detail->id=-1;
	detail->eStyle=eSTS_NONE;
	detail->nFixedWidth=0;
	return ;
}



// --------------------------------
Bool cglsStatic::Create(HWND hWnd, int x, int y, int w, int h, Bitmap* icon, const wchar_t* pszText, sGlsStaticDetail* detail)
{
	sGlsStaticDetail xdetail;

	if (detail)
		xdetail=*detail;
	else
		xDefaultDetail(&xdetail);

	RECT rect =  {x, y, w, h};

	m_bIsAutoFontSize=xdetail.bIsAutoFontSize;
	m_bIsMultiLine=xdetail.bIsMultiLine;
	m_colorBg=xdetail.colorBg;
	m_colorFont=xdetail.colorFont;
	m_eTextStyle=xdetail.eStyle;

	m_nFixedWidth=xdetail.nFixedWidth;

	if (cGlassInterface::Create(hWnd, 0, xdetail.id, &rect, &xdetail.gls) ==False)
	{
		return False;
	}

	m_item.bitmapId=(IntPtr)icon;
	m_item.txt=::yistrheapW(pszText);
	m_item.rect.x=x;
	m_item.rect.y=y;
	m_item.rect.w=w;
	m_item.rect.h=h;

	if (!m_bIsAutoFontSize)
	{
		RECT xItemRect;
		cGlassInterface::xMesureBitmap(icon, (wchar_t*)pszText, GetFont(), &xItemRect, m_nFixedWidth);
		m_item.rect.w=xItemRect.right;
		m_item.rect.h=xItemRect.bottom;
	}

	//if (!m_bIsAutoFontSize)
	//{
	//	return xCreateStaticBitmap();
	//}
	
	ReSize(m_item.rect.w, m_item.rect.h);
	Update(eGUM_FORCED);
	return True;
}

// --------------------------------
const wchar_t* cglsStatic::GetText()
{
	return m_item.txt;
}


// --------------------------------
void cglsStatic::SetText(Bitmap* icon, const wchar_t* pszText)
{
	//if (m_item.bmpId&0xFFFF0000==0)
	//{
	//}

	// これでいいの？


	if (m_item.txt)
		yifree(m_item.txt);
	m_item.txt=0;

	if (pszText)
		m_item.txt=yistrheapW(pszText);

	m_item.bitmapId=(IntPtr)icon;


	Update(eGUM_FORCED);

	//xCreateStaticBitmap();

	return ;
}

// --------------------------------
void cglsStatic::SetText(Bitmap* icon, const char* pszText)
{
	wchar_t* wpszText = yiToUnicode(0, pszText, 0);
	SetText(icon, wpszText);

	yifree(wpszText);
}

// ------------------------------------
Bitmap* cglsStatic::GetBgBitmap()
{
	return m_bg;
}


// ------------------------------------
Bool cglsStatic::OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX, int offsetY)
{
	//Graphics g(bmp);

	if (m_bg!=0)
		return False;

	m_bg = BITMAP32(m_w, m_h);

	Graphics g(m_bg);

	if (m_icon)
	{
		RECT crect = {0, ((m_h-m_icon->GetHeight())>>1), m_icon->GetWidth(), m_icon->GetHeight()};
		RECT result;

		g.DrawImage(m_icon, 0, (m_h-m_icon->GetHeight())>>1);//, result.left, result.top, result.right, result.bottom, UnitPixel);
	}

	if (m_item.txt)
	{
		int x = m_icon?m_icon->GetWidth(): 0;
		int y = (m_h-m_nBmpStrHeight)>>1;

		if (m_bmpStr)
		{
			delete m_bmpStr;
			m_bmpStr=0;
		}

		Bitmap* bg=GetParentBg(0);//, &crect);

		if (bg)
		{
			m_bmpStr=cyiShape::BitmapStringEx(bg, m_item.txt, m_fontAuto?m_fontAuto:GetFont(), m_colorFont, m_colorBg, m_eTextStyle, 0, m_nFixedWidth);
		}
		g.DrawImage(m_bmpStr, x, y);//result.left+m_nAbsX-offsetX, result.top+m_nAbsY-offsetY, result.left-x, result.top-y, result.right, result.bottom, UnitPixel);
	}


	return False;
}

#include "glsResourceBitmap.h"

// -----------------------------
Bitmap* xCreateIcon(int id, Font* font)
{
	if (id)
	{
		cglsResourceBitmap* resource = (cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
		int nFontHeight = font->GetHeight(__DEFAULTGRAPHICS__);
		Bitmap* bmp;

		if (((int)id&0xFFFF0000)==0)
		{
			bmp = cyiBitmapBlender::BitmapCopy(0, (Bitmap*)id);
		}
		else
		{
			int w=nFontHeight*0.8;
			bmp=resource->GetBitmap((int)id, 0, w, w);
		}

		return bmp;
	}

	return 0;
}

// -----------------------------
Font* cglsStatic::xGetVariableFont(Font* font, wchar_t* pszText, int w, int h, int aw, int ah)
{
	float factor1=(float)((float)w-0.1)/(float)aw;
	float factor2=(float)((float)h-0.1)/(float)ah;
	float factor=(factor1>factor2)?factor1:factor2;
	Font* ret;
	FontFamily fam;
	wchar_t fontname[32];
	float fontSize = font->GetSize();
	font->GetFamily(&fam);
	fam.GetFamilyName(fontname);

	if (w==0 ||
			h==0) // w, hがゼロの場合
	{
		unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, pszText, font, 0, 0);
		w=(pos>>16);
		h=(pos&0xFFFF);

		if (w==0 && h==0)
		{
			w=10;
			h=10;
		}

		factor1=(float)((float)w-0.1)/(float)aw;
		factor2=(float)((float)h-0.1)/(float)ah;
		factor=(factor1>factor2)?factor1:factor2;
		
		if (factor<=1.0)
		{
			return 0;
		}
	}

RESIZE_FONT:
	fontSize = fontSize/factor;


	ret = new Font(fontname, fontSize);

	{
		unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, pszText, ret, 0, 0);

		factor1=(float)((float)(pos>>16)-0.1)/(float)aw;
		factor2=(float)((float)(pos&0xFFFF)-0.1)/(float)ah;
		factor=(factor1>factor2)?factor1:factor2;

		if (factor<=1.0)
		{
			return ret;
		}
		delete ret;
		goto RESIZE_FONT;
	}

	return 0;
}

// -----------------------------
void cglsStatic::Update(eGlsUpdateMode mode, eDrawItemStatus status)
{
	Bitmap* bmp=0, *bmpStr=0;
	int nBmpWidth=0,nBmpHeight=0;
	int nBmpStrWidth=0,nBmpStrHeight=0;
	int w=0, h=0;
	Font* font=GetFont();

	// サイズやフォントのサイズ変更時
	if (mode==eGUM_FORCED)
	{
		bmp = xCreateIcon(m_item.bitmapId, font);

		if (bmp==0 &&
				(m_item.txt==0 || *m_item.txt==0))
		{
			ReSize(m_w, GetFontHeight(True));
			return ;
		}

		if (bmp)
		{
			nBmpWidth=bmp->GetWidth();
			nBmpHeight=bmp->GetHeight();
		}

		if (m_item.txt)
		{
			int x = bmp?bmp->GetWidth():0;
			int y=0;

			unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, m_item.txt, GetFont(), 0, 0, eSTS_NONE, 0, m_nFixedWidth);
			nBmpStrWidth=pos>>16;//bmpStr->GetWidth();
			nBmpStrHeight=pos&0xFFFF;
		}

		w=nBmpWidth+nBmpStrWidth;
		h=nBmpHeight>nBmpStrHeight?nBmpHeight:nBmpStrHeight;

		// フォントサイズ自動調整の場合
		if (m_bIsAutoFontSize==True)
		{
			// フォントがWindowサイズをはみ出ればフォント調整。
			if (w>m_w ||
					h>m_h)
			{
				Font* fntAuto=xGetVariableFont(GetFont(), m_item.txt, w, h, m_w, m_h);
	
				if (bmp)
					delete bmp;
				
				bmp = xCreateIcon(m_item.bitmapId, font);

				if (m_item.txt)
				{
					int x = bmp?bmp->GetWidth():0;
					int y=0;

					unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, m_item.txt, fntAuto/*m_font*/, 0, 0, m_eTextStyle, 0,m_nFixedWidth);
					nBmpStrWidth=pos>>16;
					nBmpStrHeight=pos&0xFFFF;
				}

				w=nBmpWidth+nBmpStrWidth;
				h=nBmpHeight>nBmpStrHeight?nBmpHeight:nBmpStrHeight;

				if (m_fontAuto)
					delete m_fontAuto;

				m_fontAuto=fntAuto;
			}
			else
			{
				if (m_fontAuto)
					delete m_fontAuto;

				m_fontAuto=0;
			}
		}
		// フォント自動調整ではないので、Windowのサイズ変更
		else
		{
			ReSize(w, h);
		}

		if (m_icon)
			delete m_icon;

		if (m_bmpStr)
			delete m_bmpStr;

		m_bmpStr=0;

		m_icon=bmp;

		m_nBmpStrWidth=nBmpStrWidth;
		m_nBmpStrHeight=nBmpStrHeight;

		if (m_bg)
			DELETEBMP(m_bg);

		m_bg=0;

		//if (m_item.txt)
		//{
		//	RECT rect = {};
		//	GetParentBg(0, &rect);
		//	bmpStr=cyiShape::BitmapStringEx(0, m_item.txt, m_font, 0, 0);
		//}
		InvalidateRect(0);
	}
}

// --------------------------------
//Bool cglsStatic::xCreateStaticBitmap()
//{
//	Bitmap* bmp = cGlassInterface::xCreateBitmap((Bitmap*)m_item.bitmapId, m_item.txt, m_font);
//
//	if (m_bIsAutoFontSize)
//	{
//		if (bmp->GetWidth()>m_w ||
//				bmp->GetHeight()>m_h)
//		{
//			float factor1=(float)bmp->GetWidth()/(float)m_w;
//			float factor2=(float)bmp->GetHeight()/(float)m_h;
//			float factor=(factor1>factor2)?factor1:factor2;
//			FontFamily fam;
//			wchar_t fontname[32];
//			float fontSize = m_font->GetSize();
//
//			fontSize = fontSize/factor;
//			m_font->GetFamily(&fam);
//			fam.GetFamilyName(fontname);
//			
//			delete bmp;
//
//			bmp = cGlassInterface::xCreateBitmap((Bitmap*)m_item.bitmapId, m_item.txt, &Font(fontname, fontSize));
//		}
//
//		if (m_item.pBmp[0])
//			delete m_item.pBmp[0];
//
//		m_item.pBmp[0]=BITMAP32(m_w, m_h);
//		if (m_colorBg)
//		{
//			cyiBitmapBlender::FillColor(m_item.pBmp[0], m_colorBg);
//			
//			Graphics g(m_item.pBmp[0]);
//			g.DrawImage(bmp, (int)(m_w-bmp->GetWidth())>>1,(int)(m_h-bmp->GetHeight())>>1);
//			delete bmp;
//		}
//		else
//		{
//			//m_item.pBmp[0]=bmp;
//			Graphics g(m_item.pBmp[0]);
//			g.DrawImage(bmp, (int)(m_w-bmp->GetWidth())>>1,(int)(m_h-bmp->GetHeight())>>1);
//		}
//	}
//	else
//	{
//		if (m_item.pBmp[0])
//			delete m_item.pBmp[0];
//
//		if (m_colorBg)
//		{
//			m_item.pBmp[0]=BITMAP32(bmp->GetWidth(), bmp->GetHeight());
//			cyiBitmapBlender::FillColor(m_item.pBmp[0], m_colorBg);
//			
//			Graphics g(m_item.pBmp[0]);
//			g.DrawImage(bmp, 0,0);
//			delete bmp;
//		}
//		else
//		{
//			m_item.pBmp[0]=bmp;
//		}
//
//		ReSize(m_item.pBmp[0]->GetWidth(), m_item.pBmp[0]->GetHeight());
//	}
//	m_item.pCurrentBmp=m_item.pBmp[0];
//	InvalidateRect(0);
//	return True;
//}
