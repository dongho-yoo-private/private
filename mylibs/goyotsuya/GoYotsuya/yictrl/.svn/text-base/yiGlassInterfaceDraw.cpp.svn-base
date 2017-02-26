#include "yiGlassInterfaceDraw.h"

#include "yistd.h"
#include "yiResource.h"
#include "cyctrlResource.h"

cyiGlassInterfaceDraw::cyiGlassInterfaceDraw(void)
{
}

cyiGlassInterfaceDraw::~cyiGlassInterfaceDraw(void)
{
}

typedef struct xsShadowBitmapz {
	int id;
	Bitmap* bmp[8];
}sShadowBitmapz;

sShadowBitmapz g_ShadowBitmapz[10] = {
	{-1, 0, 0, 0, 0, 0, 0, 0, 0} , };

// ---------------------------
void cyiGlassInterfaceDraw::MakeShadowImage(int id, int nShadowWidth, float edge, float level)
{
	cyiResource* res = cyiResource::GetInstance(eResourceTypex::eRTX_IMAGE_CACHED);

	if (res->Get(id)!=0)
	{
		return ;
	}


	int nShadowWidthx=nShadowWidth<<1;
	int w=nShadowWidthx*3;

	RectF rectFull(0, 0, w, w);
	Bitmap* pMask=cyiShape::FillRoundRect(&SolidBrush(Color::Black), rectFull, edge);
	Bitmap* pShadow = cyiBitmapBlender::ShadowBitmapx(pMask, nShadowWidth, 0xFF000000, level, &cyiShapeMask(cyiShape::MarginRect(Rect(0, 0, w, w), nShadowWidth, nShadowWidth), True));

	//return pShadow;
	w=pShadow->GetWidth();

	Bitmap* bmpShadow;
	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, 0, 0, nShadowWidthx, nShadowWidthx);
	res->Register(id, 0, bmpShadow, 0);
	
	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, nShadowWidthx, 0, nShadowWidthx, nShadowWidthx);
	res->Register(id, 1, bmpShadow, 0);
	
	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, w-nShadowWidthx, 0, nShadowWidthx, nShadowWidthx);
	res->Register(id, 2, bmpShadow, 0);

	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, w-nShadowWidthx, nShadowWidthx, nShadowWidthx, nShadowWidthx);
	res->Register(id, 3, bmpShadow, 0);

	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, w-nShadowWidthx, w-nShadowWidthx, nShadowWidthx, nShadowWidthx);
	res->Register(id, 4, bmpShadow, 0);

	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, nShadowWidthx, w-nShadowWidthx, nShadowWidthx, nShadowWidthx);
	res->Register(id, 5, bmpShadow, 0);

	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, 0, w-nShadowWidthx, nShadowWidthx, nShadowWidthx);
	res->Register(id, 6, bmpShadow, 0);

	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, 0, nShadowWidthx, nShadowWidthx, nShadowWidthx);
	res->Register(id, 7, bmpShadow, 0);

	Bool bShadowFound=False;

	for (int i=0; i<10; i++)
	{
		if (g_ShadowBitmapz[i].id==id)
		{
			bShadowFound=True;
			break;
		}
	}

	for (int i=0; i<10; i++)
	{
		if (g_ShadowBitmapz[i].id==-1)
		{
			g_ShadowBitmapz[i].id=id;

			for (int j=0; j<8; j++)
				g_ShadowBitmapz[i].bmp[j]=(Bitmap*)res->Get(id, j);
			break;
		}
	}
}


// ---------------------------
Bitmap* cyiGlassInterfaceDraw::GetShadowBitmap(int id, int w, int h, cyiShapeMask* pMask)
{
	sShadowBitmapz* bmpz=0;
	for (int i=0; i<10; i++)
	{
		if (g_ShadowBitmapz[i].id==id)
		{
			bmpz = &g_ShadowBitmapz[i];
			break;
		}
	}

	if (bmpz==0)
		return 0;

	Bitmap* bmp = new Bitmap(w, h, PixelFormat32bppARGB);
	Graphics g(bmp);

	//g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	int nShadowSizeX = bmpz->bmp[0]->GetWidth();
	int nShadowSize	 = nShadowSizeX>>1;

	int wLen = (w-((nShadowSize)<<2));
	int hLen = (h-((nShadowSize)<<2));

	g.DrawImage(bmpz->bmp[0], (INT)0, (INT)0);
	g.DrawImage(bmpz->bmp[2], w-nShadowSizeX, 0);
	g.DrawImage(bmpz->bmp[4], w-nShadowSizeX, h-nShadowSizeX);
	g.DrawImage(bmpz->bmp[6], 0, (h-nShadowSizeX));

	Bitmap bmpTemp(nShadowSizeX, hLen);
	Graphics gTmp(&bmpTemp);
	gTmp.FillRectangle(&TextureBrush(bmpz->bmp[3]), 0, 0, nShadowSizeX, hLen);
	g.DrawImage(&bmpTemp, w-nShadowSizeX, nShadowSizeX);

	gTmp.Clear(Color(0, 0, 0, 0));
	gTmp.FillRectangle(&TextureBrush(bmpz->bmp[7]), 0, 0, nShadowSizeX, hLen);
	g.DrawImage(&bmpTemp, 0, nShadowSizeX);

	Bitmap bmpTemp2(wLen, nShadowSizeX);
	Graphics gTmp2(&bmpTemp2);
	gTmp2.FillRectangle(&TextureBrush(bmpz->bmp[5]), 0, 0, wLen, nShadowSizeX);
	g.DrawImage(&bmpTemp2, nShadowSizeX, h-nShadowSizeX);

	gTmp2.Clear(Color(0, 0, 0, 0));
	gTmp2.FillRectangle(&TextureBrush(bmpz->bmp[1]), 0, 0, wLen, nShadowSizeX);
	g.DrawImage(&bmpTemp2, nShadowSizeX, 0);

	if (pMask==0)
		return bmp;

	//cyiShape::Clear(bmp, pMask);

	return bmp;
}



// ---------------------------
Bitmap* cyiGlassInterfaceDraw::DrawBackground(int _w, int _h, int nShadowSize, int nShadowShift, float edge, float level, Brush* brush/*=0*/)
{
	int w = _w-(nShadowSize<<1);
	int h = _h-(nShadowSize<<1);
	int x = nShadowSize-nShadowShift;
	int y = nShadowSize-nShadowShift;
	RectF rectFull(0, 0, w, h);

	if (brush==0)
	{
		brush = new LinearGradientBrush(Point(0, 0), Point(0, h), Color(80, 0, 0, 0), Color(128, 0, 0, 0));
		//LinearGradientBrush bgbr(Point(0, 0), Point(0, h), Color(80, 0, 0, 0), Color(128, 0, 0, 0));
	}
	Bitmap* pBg = cyiShape::FillRoundRect(brush/*SolidBrush(Color::Black&0x3FFFFFFF)*/, rectFull, edge);

	Bitmap* pLine  = cyiShape::DrawRoundRect(&Pen(Color::Black), rectFull,  edge);
	Bitmap* pLine2 = cyiShape::DrawRoundRect(&Pen(Color::White&0x7FFFFFFF),  RectF(1, 1, w-2, h-2), edge);

	Bitmap* ret = new Bitmap(w, h, PixelFormat32bppARGB);
	Graphics g(ret);

	g.DrawImage(pBg, 0, 0);
	g.DrawImage(pLine, 0, 0);
	g.DrawImage(pLine2, 0, 0);

	delete pBg;
	delete pLine;
	delete pLine2;

	return ret;

}

// ---------------------------
Bitmap* cyiGlassInterfaceDraw::DrawBackground(const wchar_t* caption, int w, int h, float edge, float level, Brush* brush)
{
	return 0;
}


// ---------------------------
Bitmap* cyiGlassInterfaceDraw::DrawDefaultButton(Bitmap* icon, const wchar_t* pszText, int xMargin, int yMargin, Font* _font)
{
	int fntw=0, fnth=0;
	int iconw=0, iconh=0;
	int _w=0, _h=0;
	Font* font=_font;

	if (pszText!=0)
	{
		if (font==0)
		{
			font = (Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_BUTTON_FONT);
		}

		cyiShape::GetTextSize(font, pszText, fntw, fnth);

		_w+=fntw;
		_h+=fnth;
	}

	if (icon!=0)
	{
		iconw=icon->GetWidth();
		iconh=icon->GetHeight();

		_w+=iconw;
		_h = _h < iconh ? iconh:_h;
	}

	_w+=(xMargin<<1);
	_h+=(yMargin<<1);

	Bitmap* pBtn = cyiShape::DrawRoundRect(&Pen(Color(255, 32, 32, 32)), RectF(0, 0, _w, _h), 2);
	//Bitmap* pBtn2 = cyiShape::DrawRoundRect(&Pen(Color(255, 255, 255, 255)), RectF(0, 0, _w, _h), 2);
    Bitmap* pBtn2  = cyiBitmapBlender::ShadowBitmap(pBtn, 1.0, 0xFFFFFFFF, 0.5, &cyiShapeMask(Rect(2, 2, _w-4, _h-4), True));
	Bitmap* pBtn3  = cyiShape::DrawRoundRect(&Pen(Color(64, 255, 255, 255)), RectF(0, 0, _w-2, _h-2), 2);
	Bitmap bmpx(_w, _h, PixelFormat32bppARGB);
	Graphics gxx(&bmpx);

	gxx.FillRectangle(&SolidBrush(Color(64, 255, 255, 255)), RectF(0, 0, _w, _h>>1));

	Bitmap* pBtnBg2 = cyiShape::FillRoundRect(&LinearGradientBrush(Point(0, 0), Point(0, _h), Color(0, 0,0,0), Color(64, 0, 0, 0)), RectF(0, 0, _w, _h), 2.0);
	Bitmap* pBtnBg = cyiShape::FillRoundRect(&SolidBrush(Color::Black)/*&btnbr*/, RectF(0, 0, _w, _h), 2);
	Bitmap* bmpCover=cyiBitmapBlender::RotateCopy(0, &bmpx, 0, 0, 0, 0, 0, 0, 0, &cyiShapeMask(0, 0,pBtnBg));

	Bitmap bmpLight(_w, _h, PixelFormat32bppARGB);
	Graphics gLight(&bmpLight);
	gLight.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	gLight.FillEllipse(&SolidBrush(Color(128, 255, 255, 255)), Rect(2 ,_h-(_h>>3)-2, _w-4, _h>>2));

	Bitmap* pLight = cyiBitmapBlender::GaussBlure(&bmpLight, 5.0, 2);

	int txtx, txty, iconx, icony, itemx, itemw;

	Bitmap* pTxt=0;
	if (pszText)
	{
		pTxt = cyiShape::BitmapString(0, pszText, font, &SolidBrush(Color::Black), eSTS_GROW, Color::White, 3.0, 5.0);

		fntw=pTxt->GetWidth();
		fnth=pTxt->GetHeight();
	}

	itemw = fntw+iconw;
	//itemh = fnth > iconh ? fnth:iconh;

	itemx = (_w-itemw)>>1;
	icony = (_h-iconh)>>1;
	txty  = (((_h-fnth)>>1)+4);

	Bitmap* ret = new Bitmap(_w+2, _h+2);
	Graphics gret(ret);

	gret.DrawImage(pBtnBg2, 1, 1);
	
	if (icon)
	{
		gret.DrawImage(icon, itemx, icony);
	}

	if (pTxt)
	{
		gret.DrawImage(pTxt, itemx+iconw, txty);
	}

	gret.DrawImage(bmpCover, 1, 1);
	gret.DrawImage(pBtn2, 0, 0);

	gret.DrawImage(pBtn, 1, 1);
	gret.DrawImage(pBtn3, 2, 2);
	gret.DrawImage(pLight,  1, 1);

	delete pBtn;
	delete pBtn3;
	delete pBtn2;
	delete pLight;

	return ret;
}


// ---------------------------
Bitmap* cyiGlassInterfaceDraw::DrawDefaultButton(Bitmap* icon, const wchar_t* pszText, RECT* xrect, Font* _font, color_t xcolor)
{
	Rect _rect(xrect->left, xrect->top, xrect->right, xrect->bottom);
	Rect* rect=&_rect;
	int fntw=0, fnth=0;
	int iconw=0, iconh=0;
	int _w=rect->Width, _h=rect->Height;
	Font* font=_font;

	if (pszText!=0)
	{
		if (font==0)
		{
			font = (Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_BUTTON_FONT);
		}

		cyiShape::GetTextSize(font, pszText, fntw, fnth);

		//_w+=fntw;
		//_h+=fnth;
	}

	if (icon!=0)
	{
		iconw=icon->GetWidth();
		iconh=icon->GetHeight();

		_w+=iconw;
		//_h = _h < iconh ? iconh:_h;
	}

	//_w+=(xMargin<<1);
	//_h+=(yMargin<<1);

	Bitmap* pBtn = cyiShape::DrawRoundRect(&Pen(Color(255, 32, 32, 32)), RectF(0, 0, _w, _h), 2);
	//Bitmap* pBtn2 = cyiShape::DrawRoundRect(&Pen(Color(255, 255, 255, 255)), RectF(0, 0, _w, _h), 2);
    Bitmap* pBtn2  = cyiBitmapBlender::ShadowBitmap(pBtn, 1.0, 0xFFFFFFFF, 0.5, &cyiShapeMask(Rect(2, 2, _w-4, _h-4), True));
	Bitmap* pBtn3  = cyiShape::DrawRoundRect(&Pen(Color(64, 255, 255, 255)), RectF(0, 0, _w-2, _h-2), 2);
	Bitmap bmpx(_w, _h, PixelFormat32bppARGB);
	Graphics gxx(&bmpx);

	gxx.FillRectangle(&SolidBrush(Color(64, 255, 255, 255)), RectF(0, 0, _w, _h>>1));

	Bitmap* pBtnBg2 = cyiShape::FillRoundRect(&LinearGradientBrush(Point(0, 0), Point(0, _h), Color(0, 0,0,0), Color(64, 0, 0, 0)), RectF(0, 0, _w, _h), 2.0);
	Bitmap* pBtnBg = cyiShape::FillRoundRect(&SolidBrush(Color::Black)/*&btnbr*/, RectF(0, 0, _w, _h), 2);
	Bitmap* bmpCover=cyiBitmapBlender::RotateCopy(0, &bmpx, 0, 0, 0, 0, 0, 0, 0, &cyiShapeMask(0, 0,pBtnBg));

	Bitmap bmpLight(_w, _h, PixelFormat32bppARGB);
	Graphics gLight(&bmpLight);
	gLight.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	gLight.FillEllipse(&SolidBrush(__COLORt__(64, xcolor))/*xcolorColor(128, 255, 255, 255))*/, Rect(2 ,_h-(_h>>3)-2, _w-4, _h>>2));

	Bitmap* pLight = cyiBitmapBlender::GaussBlure(&bmpLight, 5.0, 2);

	int txtx, txty, iconx, icony, itemx, itemw;

	Bitmap* pTxt=0;
	if (pszText)
	{
		if (xcolor!=0xFFFFFFFF)
		{
			pTxt = cyiShape::BitmapString(0, pszText, font, &SolidBrush(Color::White), eSTS_GROW, xcolor, 3.0, 5.0);
		}
		else
		{
			pTxt = cyiShape::BitmapString(0, pszText, font, &SolidBrush(Color::Black), eSTS_GROW, xcolor, 3.0, 5.0);
		}

		fntw=pTxt->GetWidth();
		fnth=pTxt->GetHeight();
	}

	itemw = fntw+iconw;
	//itemh = fnth > iconh ? fnth:iconh;

	itemx = (_w-itemw)>>1;
	icony = (_h-iconh)>>1;
	txty  = (((_h-fnth)>>1)+4);

	Bitmap* ret = new Bitmap(_w+2, _h+2, PixelFormat32bppARGB);
	Graphics gret(ret);

	gret.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	//gret.DrawImage(pBtnBg2, 1, 1);
	
	if (icon)
	{
		gret.DrawImage(icon, itemx, icony);
	}

	if (pTxt)
	{
		gret.DrawImage(pTxt, itemx+iconw, txty);
	}

	gret.DrawImage(bmpCover, 1, 1);
	gret.DrawImage(pBtn2, 0, 0);

	gret.DrawImage(pBtn, 1, 1);
	gret.DrawImage(pBtn3, 2, 2);
	gret.DrawImage(pLight,  1, 1);

	delete pBtn;
	delete pBtn3;
	delete pBtn2;
	delete pLight;

	return ret;
}

// ---------------------------
Bitmap* cyiGlassInterfaceDraw::DrawDefaultButtonDisable(Bitmap* icon, const wchar_t* pszText, RECT* xrect, Font* _font)
{
	Rect _rect(xrect->left, xrect->top, xrect->right, xrect->bottom);
	Rect* rect=&_rect;
	int fntw=0, fnth=0;
	int iconw=0, iconh=0;
	int _w=rect->Width, _h=rect->Height;
	Font* font=_font;

	if (pszText!=0)
	{
		if (font==0)
		{
			font = (Font*)cyiResource::GetInstance(eRTX_TEMPORARY)->Get(eRTID_BUTTON_FONT);
		}

		cyiShape::GetTextSize(font, pszText, fntw, fnth);

		//_w+=fntw;
		//_h+=fnth;
	}

	if (icon!=0)
	{
		iconw=icon->GetWidth();
		iconh=icon->GetHeight();

		_w+=iconw;
		//_h = _h < iconh ? iconh:_h;
	}

	//_w+=(xMargin<<1);
	//_h+=(yMargin<<1);

	Bitmap* pBtn = cyiShape::DrawRoundRect(&Pen(Color(255, 32, 32, 32)), RectF(0, 0, _w, _h), 2);
	//Bitmap* pBtn2 = cyiShape::DrawRoundRect(&Pen(Color(255, 255, 255, 255)), RectF(0, 0, _w, _h), 2);
    Bitmap* pBtn2  = cyiBitmapBlender::ShadowBitmap(pBtn, 1.0, 0xFFFFFFFF, 0.5, &cyiShapeMask(Rect(2, 2, _w-4, _h-4), True));
	Bitmap* pBtn3  = cyiShape::DrawRoundRect(&Pen(Color(32, 255, 255, 255)), RectF(0, 0, _w-2, _h-2), 2);
	Bitmap bmpx(_w, _h, PixelFormat32bppARGB);
	Graphics gxx(&bmpx);

	gxx.FillRectangle(&SolidBrush(Color(64, 255, 255, 255)), RectF(0, 0, _w, _h>>1));

	Bitmap* pBtnBg2 = cyiShape::FillRoundRect(&LinearGradientBrush(Point(0, 0), Point(0, _h), Color(0, 0,0,0), Color(64, 0, 0, 0)), RectF(0, 0, _w, _h), 2.0);
	Bitmap* pBtnBg = cyiShape::FillRoundRect(&SolidBrush(Color::Black)/*&btnbr*/, RectF(0, 0, _w, _h), 2);
	Bitmap* bmpCover=cyiBitmapBlender::RotateCopy(0, &bmpx, 0, 0, 0, 0, 0, 0, 0, &cyiShapeMask(0, 0,pBtnBg));

	Bitmap bmpLight(_w, _h, PixelFormat32bppARGB);
	Graphics gLight(&bmpLight);
	gLight.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	gLight.FillEllipse(&SolidBrush(Color(128, 255, 255, 255)), Rect(2 ,_h-(_h>>3)-2, _w-4, _h>>2));

	Bitmap* pLight = cyiBitmapBlender::GaussBlure(&bmpLight, 5.0, 2);

	int txtx, txty, iconx, icony, itemx, itemw;

	Bitmap* pTxt=0;
	if (pszText)
	{
		pTxt = cyiShape::BitmapString(0, pszText, font, &SolidBrush(Color(128, 0, 0, 0)), eSTS_GROW, __COLORp__(128, 255,255,255), 3.0, 5.0);

		fntw=pTxt->GetWidth();
		fnth=pTxt->GetHeight();
	}

	itemw = fntw+iconw;
	//itemh = fnth > iconh ? fnth:iconh;

	itemx = (_w-itemw)>>1;
	icony = (_h-iconh)>>1;
	txty  = (((_h-fnth)>>1)+4);

	Bitmap* ret = new Bitmap(_w+2, _h+2, PixelFormat32bppARGB);
	Graphics gret(ret);

	gret.DrawImage(pBtnBg2, 1, 1);
	
	if (icon)
	{
		gret.DrawImage(icon, itemx, icony);
	}

	if (pTxt)
	{
		gret.DrawImage(pTxt, itemx+iconw, txty);
	}

	//gret.DrawImage(bmpCover, 1, 1);
	gret.DrawImage(pBtn2, 0, 0);

	gret.DrawImage(pBtn, 1, 1);
	gret.DrawImage(pBtn3, 2, 2);
	//gret.DrawImage(pLight,  1, 1);

	delete pBtn;
	delete pBtn3;
	delete pBtn2;
	delete pLight;

	return ret;
}

// ---------------------------
Bitmap* MakeRadioCircle(int rad, color_t color, Bool bIsSelected)
{
	return 0;
}
	
// ---------------------------
Bitmap* MakeCheckMark(int width, color_t color, Bool bIsSelected)
{
	return 0;
}