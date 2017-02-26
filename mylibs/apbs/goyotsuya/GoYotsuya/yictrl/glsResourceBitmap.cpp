#include "glsResourceBitmap.h"

cglsResourceBitmap::cglsResourceBitmap(void)
{

}

cglsResourceBitmap::~cglsResourceBitmap(void)
{
}

// ---------------------------------------
Bool cglsResourceBitmap::Initialize()
{
	return True;
}

// ---------------------------------------
static Bitmap* xMakeNextBitmap(int subId, int w, int h, color_t color=Color::Black);
// ---------------------------------------
static Bitmap* xMakeTopBitmap(int subId, int w, int h, color_t color=Color::Black);
// ---------------------------------------
static Bitmap* xMakeEndBitmap(int subId, int w, int h, color_t color=Color::Black);
// ---------------------------------------
static Bitmap* xMakePrevBitmap(int subId, int w, int h, color_t color=Color::Black);


// ---------------------------------------
static Bitmap* xMakePPBitmap(int subId, int w, int h, color_t color=Color::Black);
// ---------------------------------------
static Bitmap* xMakeNNBitmap(int subId, int w, int h, color_t color=Color::Black);
// ---------------------------------------
static Bitmap* xMakePrevBranchBitmap(int subId, int w, int h, color_t color=Color::Black);
// ---------------------------------------
static Bitmap* xMakeNextBranchBitmap(int subId, int w, int h, color_t color=Color::Black);
// ---------------------------------------
static Bitmap* xMakeShowNoBitmap(int subId, int w, int h, color_t color=Color::Black);
// ---------------------------------------
static Bitmap* xMakePassBitmap(int subId, int w, int h, color_t color=Color::Black);


// ---------------------------------------
static Bitmap* xDrawIconDetail(int subId, int w, int h);

// ---------------------------------------
static Bitmap* xDrawIconMenu(int subId, int w, int h);

// ---------------------------------------
static Bitmap* xDrawIconMsg(int subId, int w, int h);


// ---------------------------------------
static Bitmap* xMakeCloseBtnBitmap(int subId, int w, int h);

// ---------------------------------------
static Bitmap* xGetCircleBlure10();

// ---------------------------------------
static Bitmap* xDrawGlsSelectHiLight(int w, int h, color_t color, Bool bIsHalf=False);

// ---------------------------------------
static Bitmap* xDrawGlsScrollArrow(int subId, int w, int h, color_t color, int angle);

// ---------------------------------------
static Bitmap* xDrawGlsArrow(color_t subId, int w, int h, int angle);

// ---------------------------------------
static Bitmap* xDrawBranch(color_t subId, int w, int h);

// ---------------------------------------
static Bitmap* xDrawIconNotShowNo(color_t subId, int w, int h);

// ---------------------------------------
static Bitmap* xDrawIconShowNo(color_t subId, int w, int h);

// ---------------------------------------
static Bitmap* xDrawIconShowNoFrom(color_t subId, int w, int h);

// ---------------------------------------
static Bitmap* xDrawToolBtnArrow(int subId, int direct, int h);

// ---------------------------------------
static Bitmap* xDrawRadio(int subId, int direct, int h);

// ---------------------------------------
static Bitmap* xDrawCheck(int subId, int w, int h);

// ---------------------------------------
static Bitmap* xDrawToolCross(int subId, int w, int h);

// ---------------------------------------
static Bitmap* xDrawChangedMark(int subId, int w, int h);




// ---------------------------------------
Bitmap* cglsResourceBitmap::GetBitmap(int id, int subId, IntPtr w, IntPtr h, color_t color, void* extra)
{
	Bitmap* bmp = xGetSameObject(id, subId, color, w, h);
	
	if (color==0)
		color=Color::Black;

	w=w==0?h:w;

	if (bmp!=0)
		return bmp;

	switch (id)
	{
		case eGRID_NEXT:
		{
			bmp=xMakeNextBitmap(subId, w, h, color);
			break;
		}
		case eGRID_TOP:
		{
			bmp=xMakeTopBitmap(subId, w, h, color);
			break;
		}
		case eGRID_PP:
		{
			bmp=xMakePPBitmap(subId, w, h, color);
			break;
		}
		case eGRID_NN:
		{
			bmp=xMakeNNBitmap(subId, w, h, color);
			break;
		}
		case eGRID_PREV:
		{
			bmp=xMakePrevBitmap(subId, w, h, color);
			break;
		}
		case eGRID_END:
		{
			bmp=xMakeEndBitmap(subId, w, h, color);
			break;
		}
		case eGRID_NEXT_BRANCH:
		{
			bmp=xMakeNextBranchBitmap(subId, w, h, color);
			break;
		}
		case eGRID_PREV_BRANCH:
		{
			bmp=xMakePrevBranchBitmap(subId, w, h, color);
			break;
		}
		case eGRID_SHOW_NO:
		{
			bmp=xMakeShowNoBitmap(subId, w, h, color);
			break;
		}
		case eGRID_PASS:
		{
			bmp=xMakePassBitmap(subId, w, h, color);
			break;
		}
		case eGRID_CLOSE_BTN:
		{
			bmp=xMakeCloseBtnBitmap(subId, w, h);
			break;
		}
		case eGRID_GLS_HILIGHT:
		{
			bmp=xGetCircleBlure10();
			break;	
		}
		case eGRID_SCROLL_DOWN:
		{
			bmp=xDrawGlsScrollArrow(subId, w, h, Color::White, 0);
			break;
		}
		case eGRID_SCROLL_UP:
		{
			bmp=xDrawGlsScrollArrow(subId, w, h, Color::White, 2);
			break;
		}
		case eGRID_SCROLL_LEFT:
		{
			bmp=xDrawGlsScrollArrow(subId, w, h, Color::White, 3);
			break;
		}
		case eGRID_SCROLL_RIGHT:
		{
			bmp=xDrawGlsScrollArrow(subId, w, h, Color::White, 1);
			break;
		}
		case eGRID_ARROW_UP:
		{
			bmp=xDrawGlsArrow(subId, w, h,  2);
			break;
		}
		case eGRID_ARROW_DOWN:
		{
			bmp=xDrawGlsArrow(subId, w, h,  0);
			break;
		}
		case eGRID_ARROW_LEFT:
		{
			bmp=xDrawGlsArrow(color, w, h, 3);
			break;
		}
		case eGRID_ARROW_RIGHT:
		{
			bmp=xDrawGlsArrow(color, w, h, 1);
			break;
		}
		case eGRID_ICON_MENU:
		{
			bmp=xDrawIconMenu(subId, w, h);
			break;
		}
		case eGRID_ICON_MESSAGE:
		{
			bmp=xDrawIconMsg(subId, w, h);
			break;
		}
		case eGRID_ICON_DETAIL:
		{
			bmp=xDrawIconDetail(subId, w, h);
			break;
		}
		case eGRID_NOT_SHOW_NO:
		{
			bmp=xDrawIconNotShowNo(subId, w, h);
			break;
		}
		/*case eGRID_SHOW_NO:
		{
			bmp=xDrawIconShowNo(subId, w, h);
			break;
		}*/
		case eGRID_SHOW_NO_FROM:
		{
			bmp=xDrawIconShowNoFrom(subId, w, h);
			break;
		}
		case eGRID_TOOL_ALEFT:
		{
			bmp=xDrawToolBtnArrow(subId, 3, h);
			break;
		}
		case eGRID_TOOL_ARIGHT:
		{
			bmp=xDrawToolBtnArrow(subId, 1, h);
			break;
		}
		case eGRID_TOOL_AUP:
		{
			bmp=xDrawToolBtnArrow(subId, 2, h);
			break;
		}
		case eGRID_TOOL_ADOWN:
		{
			bmp=xDrawToolBtnArrow(subId, 0, h);
			break;
		}
		case eGRID_RADIO:
		{
			bmp=xDrawRadio(subId, w, h);
			break;
		}
		case eGRID_CHECK:
		{
			bmp=xDrawCheck(subId, w, h);
			break;
		}
		case eGRID_TOOL_CROSS:
		{
			bmp=xDrawToolCross(subId, 0, h);
			break;
		}
		case eGRID_CHANGED_MARK:
		{
			bmp=xDrawChangedMark(subId, w, h);
			break;
		}
	/*eGRID_TOOL_ARIGHT,
	eGRID_TOOL_AUP,
	eGRID_TOOL_ADOWN,
	eGRID_TOOL_PLUS,
	eGRID_TOOL_MINUS,
	eGRID_TOOL_CROSS,
	eGRID_TOOL_CHAR
		case eGRID_TOOL_BUTTONITEM:
		{
			xDraw
			break;
		}*/
		default:
			return 0;
	}
	
	xSetObject(bmp, id, subId, color, w, h);
	return bmp;
}

// ---------------------------------------
static Bitmap* xDrawGlassBtnPannel(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra);

// ---------------------------------------
static Bitmap* xDrawNeonBtnPannel(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra);


static Bitmap* xDrawGlassToolPannel(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra);

// ---------------------------------------
static Bitmap* xDrawTabItemBg(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra);

// ---------------------------------------
static Bitmap* xDrawTabBg(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra);

// ---------------------------------------
static Bitmap* xDrawScrollBar(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra);

// ---------------------------------------
static Bitmap* xDrawGlassSplitBtnPannel(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra);

// ---------------------------------------
static Bitmap* xDrawNeonSplitBtnPannel(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra);


// ---------------------------------------
Bitmap* cglsResourceBitmap::Draw(int id, int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra)
{
	switch(id)
	{
		case eGDID_GLASS_BTN_PANNEL:
		{
			return xDrawGlassBtnPannel(subId, w, h, bmp, bmpStr, extra);
		}
		case eGDID_NEON_BTN_PANNEL:
		{
			return xDrawNeonBtnPannel(subId, w, h, bmp, bmpStr, extra);
		}

		case eGDID_GLASS_TOOL_PANNEL:
		{
			return xDrawGlassToolPannel(subId, w, h, bmp, bmpStr, extra);
		}
		case eDID_TAB_ITEM_BG:
		{
			return xDrawTabItemBg(subId, w, h, bmp, bmpStr, extra);
		}
		case eDID_TAB_BG:
		{
			return xDrawTabBg(subId, w, h, bmp, bmpStr, extra);
		}
		case eDID_SCROLL_BAR:
		{
			return xDrawScrollBar(subId, w, h, bmp, bmpStr, extra);
		}
		case eDID_SCROLL_PAGE:
		{
			return xDrawGlsSelectHiLight(w, h, Color::White);
		}
		case eGDID_GLASS_SPLIT_PANNEL:
		{
			return xDrawGlassSplitBtnPannel(subId, w, h, bmp, bmpStr, extra);
		}
		case eGDID_NEON_SPLIT_PANNEL:
		{
			return xDrawNeonSplitBtnPannel(subId, w, h, bmp, bmpStr, extra);
		}
		default:
			break;
	}

	return 0;
}

// ----------------------------------------------
//typedef struct xsGlsTabItem {
//	Bool   bIsVertical;
//	float  radius; // default 3
//	color_t NormalTopColor;
//	color_t NormalTopColorMask; // 0x1FFFFFFF
//	color_t NormalBaseColor; // 0x7FFFFFFF
//	color_t DownTopColor;
//	color_t DownTopColorMask; // 0x7FFFFFFF
//	color_t DownBaseColor; // 0x7FFFFFFF
//	color_t MoveTopColor;
//	color_t MoveTopColorMask; // 0x1FFFFFFF
//	color_t MoveBaseColor; // Color(200, 255, 255, 255)
//	color_t HiLightColor;
//
//	//Brush* NormalBgBrush;
//	//Brush* MoveBgBrush;
//	//Brush* SelectedBgBrush;
//	//Pen* NormalPen;
//	//Pen* MovePen;
//	//Pen* SelectedPen;
//	//Bool bHasClosedButton;
//} sGlsTabItem;

#include "glsTabCtrl.h"

// ---------------------------------------
Bitmap* xDrawTabItemBg(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra)
{
	GraphicsPath* _path = 0;//path;
	GraphicsPath __path;
	Brush* brush = (Brush*)bmp;
	Pen* pen=(Pen*)bmpStr;
	Rect rect(0, 0, w, h+3);
	sGlsTabItem* s=(sGlsTabItem*)extra;
	int radius=(int)s->radius;


	if (_path==NULL)
	{
		_path = &__path;
	}

	_path->AddLine(rect.X, rect.Height-1, rect.X, rect.Y+radius/*radius*/);
	_path->AddArc(rect.X, rect.Y, radius*2, radius*2, 180.0, 90.0);
	_path->AddLine(rect.X+radius, rect.Y, rect.Width-radius-1, rect.Y);
	_path->AddArc(rect.Width-radius*2-1, rect.Y, radius*2, radius*2, -90.0, 90.0);
	_path->AddLine(rect.Width-1, rect.Y+radius, rect.Width-1, rect.Height);
	
	//_path->AddArc(rect.Width-radius*2-1, rect.Height-radius*2-1, radius*2, radius*2, 0.0, 90.0);
	//_path->AddLine(rect.Width-radius-1, rect.Height-1, rect.X+radius, rect.Height-1);
	//_path->AddArc(rect.X, rect.Height-1-radius*2, radius*2, radius*2, 90.0, 90.0);


	if (subId==0)
	{
		Bitmap* xbmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
		Graphics g(xbmp);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		g.FillPath(&LinearGradientBrush(Point(0, 0), Point(0,h), Color(127, 255,255,255), Color(127,255,255,255)), _path);
		g.FillPath(&LinearGradientBrush(Point(0, 0), Point(0,h), Color((ARGB)(s->NormalTopColor&s->NormalTopColorMask)), Color((ARGB)(s->NormalBaseColor))), _path);
		//g.FillPath(&LinearGradientBrush(Point(0, 0), Point(0,h), Color::Yellow&0x1FFFFFFF, Color(127, 255, 255, 255)), _path);
		g.DrawPath(&Pen(Color::DarkGray), _path);

		Bitmap* ret = cyiBitmapBlender::BitmapCopy(0, xbmp, 0, 0, 0, 0, w, h);
		delete xbmp;
		//xbmp=cyiBitmapBlender::DropShadow(ret, 1, 1, 1.0, 0.5);
		//delete ret;
		//ret = xbmp;

		if (s->bIsVertical)
		{
			Bitmap* vret=cyiBitmapBlender::RotateCopy(0, ret, 3);
			delete ret;
			return vret;
		}

		return ret;
	}

	if (subId==2)
	{
		Bitmap* xbmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
		Graphics g(xbmp);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		//g.FillPath(&LinearGradientBrush(Point(0, 0), Point(0,h), (Color::YellowGreen&0x7FFFFFFF), Color(127, 255, 255, 255)), _path);
		g.FillPath(&LinearGradientBrush(Point(0, 0), Point(0,h), Color(127, 255,255,255), Color(127,255,255,255)), _path);
		g.FillPath(&LinearGradientBrush(Point(0, 0), Point(0,h), s->DownTopColor&s->DownTopColorMask, s->DownBaseColor), _path);
		g.DrawPath(&Pen(Color::Black), _path);

		Bitmap* ret = cyiBitmapBlender::BitmapCopy(0, xbmp, 0, 0, 0, 0, w, h);
		delete xbmp;

		if (s->bIsVertical)
		{
			Bitmap* vret=cyiBitmapBlender::RotateCopy(0, ret, 3);
			delete ret;
			return vret;
		}

		return ret;
	}

	if (subId==1)
	{
		Bitmap* xbmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
		Graphics g(xbmp);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		
		g.DrawPath(&Pen(Color::Black), _path);

		Bitmap* hi= xDrawGlsSelectHiLight(rect.Width, rect.Height, s->HiLightColor/*Color::Orange*/, True);
		g.FillPath(&LinearGradientBrush(Point(0, 0), Point(0,h), s->MoveTopColor/*(Color::Orange&0x1FFFFFFF)*/, s->MoveBaseColor/*Color(200, 255, 255, 255))*/), _path);
		g.DrawImage(hi, 0, 0);
		delete hi;
		Bitmap* ret = cyiBitmapBlender::BitmapCopy(0, xbmp, 0, 0, 0, 0, w, h);
		delete xbmp;

		if (s->bIsVertical)
		{
			Bitmap* vret=cyiBitmapBlender::RotateCopy(0, ret, 3);
			delete ret;
			return vret;
		}

		return ret;
	}

	//if (subId==1)
	//{
	//	Bitmap* xbmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
	//	Graphics g(xbmp);
	//	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	//	g.FillPath(&LinearGradientBrush(Point(0, 0), Point(0,h), Color(127, 32, 32, 32), Color(127, 255, 255, 255)), _path);
	//	g.DrawPath(&Pen(Color::Black), _path);

	//	Bitmap* ret = cyiBitmapBlender::BitmapCopy(0, xbmp, 0, 0, 0, 0, w, h);
	//	delete xbmp;

	//	if (extra!=0)
	//	{
	//		Bitmap* vret=cyiBitmapBlender::RotateCopy(0, ret, 3);
	//		delete ret;
	//		return vret;
	//	}
	//}


	return 0;
}

// ---------------------------------------
Bitmap* xDrawTabBg(int subId, int _w, int _h, Bitmap* _bmp, Bitmap* bmpStr, void* extra)
{
	sGeneralDrawItem* detail=(sGeneralDrawItem*)extra;
	Bitmap* bmp = BITMAP32(detail->rect[0].right, detail->rect[0].bottom);
	int x = detail->rect[1].left;
	int y = detail->rect[1].top;
	int w = detail->rect[1].right;
	int h = detail->rect[1].bottom;

	Graphics g(bmp);

	g.FillRectangle(detail->pBrush[0], 0, h, detail->rect[0].right, detail->rect[0].bottom-h);
	g.DrawLine(detail->pPen[0], 0, detail->rect[0].bottom-1, detail->rect[0].right, detail->rect[0].bottom-1 );
	g.DrawLine(detail->pPen[1], 0, h, x, h);	
	g.DrawLine(detail->pPen[1], x+w, h, detail->rect[0].right, h);



	return bmp;

}


// ---------------------------------------
Bitmap* xMakeNextBitmap(int subId, int w, int h, color_t color)
{
	Bitmap* bmptri = cyiShape::FillRoundTriangle(0, &SolidBrush(color), RectF(0, 0, h, h>>1), 1.0);
	Bitmap* ret = cyiBitmapBlender::RotateCopy(0,bmptri, 1);

	return ret;
}

// ---------------------------------------
Bitmap* xMakePrevBitmap(int subId, int w, int h, color_t color)
{
	Bitmap* bmptri = cyiShape::FillRoundTriangle(0, &SolidBrush(color), RectF(0, 0, h, h>>1), 1.0);
	Bitmap* ret = cyiBitmapBlender::RotateCopy(0,bmptri, 3);

	return ret;
}

// ---------------------------------------
Bitmap* xMakePPBitmap(int subId, int w, int h, color_t color)
{
	Bitmap* bmptri = cyiShape::FillRoundTriangle(0, &SolidBrush(color), RectF(0, 0, h, h>>1), 1.0);
	Bitmap* retOneParts = cyiBitmapBlender::RotateCopy(0,bmptri, 3);

	delete bmptri;
	Bitmap* ret = BITMAP32(retOneParts->GetWidth()<<1, retOneParts->GetHeight());

	cyiBitmapBlender::BitmapCopy(ret, retOneParts);
	cyiBitmapBlender::BitmapCopy(ret, retOneParts, retOneParts->GetWidth());
	delete retOneParts;
	return ret;
}

// ---------------------------------------
Bitmap* xMakeNextBranchBitmap(int subId, int w, int h, color_t color)
{
	Bitmap* bmptri = cyiShape::FillRoundTriangle(0, &SolidBrush(color), RectF(0, 0, h, h>>1), 1.0);
	Bitmap* retOneParts = cyiBitmapBlender::RotateCopy(0,bmptri, 1);
	delete bmptri;

	Font* font = cyiShape::GetFontByHeight(h, L"Meiryo");
	Bitmap* bmpStr = cyiShape::BitmapString(0, L"B", font, &SolidBrush(Color(color)));

	Bitmap* ret = BITMAP32(retOneParts->GetWidth()+bmpStr->GetWidth(), retOneParts->GetHeight());
	
	int y = bmpStr->GetHeight();

	//if (y%2)
	//{
	//	y=(y>>1)+1;
	//}
	//else
	//{
	//	y=(y>>1);
	//}

	y=(y>>1);

	cyiBitmapBlender::BitmapCopy(ret, retOneParts);
	Graphics g(ret);

	g.DrawImage(bmpStr, retOneParts->GetWidth(), (h>>1)-y+1);
	//cyiBitmapBlender::BitmapCopy(ret, bmpStr, retOneParts->GetWidth(), (h>>1)-y);

	delete retOneParts;
	delete bmpStr;

	return ret;

}

// ---------------------------------------
Bitmap* xMakePrevBranchBitmap(int subId, int w, int h, color_t color)
{
	Bitmap* bmptri = cyiShape::FillRoundTriangle(0, &SolidBrush(color), RectF(0, 0, h, h>>1), 1.0);
	Bitmap* retOneParts = cyiBitmapBlender::RotateCopy(0,bmptri, 3);
	delete bmptri;

	Font* font = cyiShape::GetFontByHeight(h, L"Meiryo");
	Bitmap* bmpStr = cyiShape::BitmapString(0, L"B", font, &SolidBrush(Color(color)));

	Bitmap* ret = BITMAP32(retOneParts->GetWidth()+bmpStr->GetWidth(), retOneParts->GetHeight());
	
	int y = bmpStr->GetHeight();

	//if (y%2)
	//{
	//	y=(y>>1)+1;
	//}
	//else
	//{
	//	y=(y>>1);
	//}

	y=(y>>1);

	Graphics g(ret);
	g.DrawImage(bmpStr, 0, (h>>1)-y+1);
	//cyiBitmapBlender::BitmapCopy(ret, bmpStr, 0, (h>>1)-y);
	cyiBitmapBlender::BitmapCopy(ret, retOneParts, bmpStr->GetWidth());

	delete retOneParts;
	delete bmpStr;

	return ret;

}

// ---------------------------------------
Bitmap* xMakeShowNoBitmap(int subId, int w, int h, color_t color)
{
	int hOrigin=0;

	//if (h<15)
	//{
	//	hOrigin=h;
	//	h=64;
	//}

	Bitmap* ret = cyiShape::MakeCircleInChar(h, L"1", L"Meiryo", color, color);

	if (hOrigin==0)
		return ret;

	Bitmap* bmp = BITMAP32(hOrigin, hOrigin);
	Graphics gTemp(bmp);

	gTemp.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	gTemp.DrawImage(ret, 0, 0, bmp->GetWidth(), bmp->GetHeight());
	delete ret;
	return bmp;
}

// ---------------------------------------
Bitmap* xMakePassBitmap(int subId, int w, int h, color_t color)
{
	int hOrigin=0;

	if (h<15)
	{
		hOrigin=h;
		h=64;
	}

	Bitmap* bmptri = cyiShape::FillRoundTriangle(0, &SolidBrush(color), RectF(0, 0, h, h>>1), 1.0);
	Bitmap* retOneParts = cyiBitmapBlender::RotateCopy(0,bmptri, 1);

	delete bmptri;
	Bitmap* ret = BITMAP32(retOneParts->GetWidth()+(retOneParts->GetWidth()>>1), retOneParts->GetHeight());

	Graphics g(ret);

	int yOffset = h>>2;
	int hx = (h-yOffset)-yOffset;
	g.FillRectangle(&SolidBrush(color), 0, yOffset, (retOneParts->GetWidth()>>1)+1, hx);
	g.DrawImage(retOneParts, retOneParts->GetWidth()>>1, 0);
	delete retOneParts;

	if (hOrigin!=0)
	{
		Bitmap* bmp = BITMAP32((hOrigin>>1)+(hOrigin>>2), hOrigin);
		Graphics gTemp(bmp);

		gTemp.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		gTemp.DrawImage(ret, 0, 0, bmp->GetWidth(), bmp->GetHeight());
		delete ret;
		return bmp;
	}
	return ret;
}


// ---------------------------------------
Bitmap* xMakeNNBitmap(int subId, int w, int h, color_t color)
{
	Bitmap* bmptri = cyiShape::FillRoundTriangle(0, &SolidBrush(color), RectF(0, 0, h, h>>1), 1.0);
	Bitmap* retOneParts = cyiBitmapBlender::RotateCopy(0,bmptri, 1);

	delete bmptri;
	Bitmap* ret = BITMAP32(retOneParts->GetWidth()<<1, retOneParts->GetHeight());

	cyiBitmapBlender::BitmapCopy(ret, retOneParts);
	cyiBitmapBlender::BitmapCopy(ret, retOneParts, retOneParts->GetWidth());
	
	delete retOneParts;

	return ret;
}

// ---------------------------------------
Bitmap* xMakeTopBitmap(int subId, int w, int h, color_t color)
{
	Bitmap* bmptri = cyiShape::FillRoundTriangle(0, &SolidBrush(color), RectF(0, 0, h, h>>1), 1.0);
	Bitmap* retOneParts = cyiBitmapBlender::RotateCopy(0,bmptri, 3);
	Bitmap* bmpBar = cyiShape::FillRoundRect(&SolidBrush(color), RectF(0, 0, (h>>2), h), 0.0);

	delete bmptri;
	Bitmap* ret = BITMAP32(retOneParts->GetWidth()+bmpBar->GetWidth()+1, retOneParts->GetHeight());

	cyiBitmapBlender::BitmapCopy(ret, bmpBar);
	cyiBitmapBlender::BitmapCopy(ret, retOneParts, bmpBar->GetWidth()+1);

	delete bmpBar;
	delete retOneParts;
	return ret;
}

// ---------------------------------------
Bitmap* xMakeEndBitmap(int subId, int w, int h, color_t color)
{
	Bitmap* bmptri = cyiShape::FillRoundTriangle(0, &SolidBrush(color), RectF(0, 0, h, h>>1), 1.0);
	Bitmap* retOneParts = cyiBitmapBlender::RotateCopy(0,bmptri, 1);
	Bitmap* bmpBar = cyiShape::FillRoundRect(&SolidBrush(color), RectF(0, 0, (h>>2), h), 0.0);

	delete bmptri;
	Bitmap* ret = BITMAP32(retOneParts->GetWidth()+bmpBar->GetWidth()+1, retOneParts->GetHeight());

	cyiBitmapBlender::BitmapCopy(ret, retOneParts);
	cyiBitmapBlender::BitmapCopy(ret, bmpBar, retOneParts->GetWidth()+1);
	delete bmpBar;
	delete retOneParts;
	return ret;
}


// ---------------------------------------
Bitmap* xGetCircleBlure10()
{
	static Bitmap* bmp5=0;

	if (bmp5)
		return bmp5;

	Bitmap temp(48, 48, PixelFormat32bppARGB);
	Graphics g3(&temp);
	g3.FillEllipse(&SolidBrush(Color(98, 0, 0, 0)), RectF(5, 5, 48-10, 48-10)); 
	bmp5 = cyiBitmapBlender::GaussBlureXX(&temp, 10.0, 100, 0);

	return bmp5;
}

// ---------------------------------------
Bitmap* xDrawGlsArrow(color_t color, int w, int h, int angle)
{
	Bitmap* tri = cyiShape::FillRoundTriangle(0, &SolidBrush(Color(color)), RectF(0, 0, w, h), 0.0);

	if (angle)
	{
		Bitmap* pRotatedBmp=cyiBitmapBlender::RotateCopy(0, tri, angle);
		delete tri;
		return pRotatedBmp;
	}
	return tri;
}


// ---------------------------------------
Bitmap* xDrawGlsScrollArrow(int subId, int w, int h, color_t color, int angle)
{
	Bitmap* ret = BITMAP32(w, h);
	int shadowDepth=2;
	int _w = (w-(shadowDepth<<1))-1;
	int _h = (h-(shadowDepth<<1))-1;

	

	Bitmap* tri = cyiShape::FillRoundTriangle(0, &SolidBrush(Color(color)), RectF(0, 0, _w, _h), 0.0);
	Bitmap* bmpShadow;

	if (angle)
	{
		Bitmap* pRotatedBmp=cyiBitmapBlender::RotateCopy(0, tri, angle);
		//Bitmap* ret=cyiBitmapBlender::DropShadow(pRotatedBmp, 1, 1, (float)shadowDepth, 0.5);
		delete tri;
		tri = pRotatedBmp;
	}

	Graphics g(ret);
	// 影を作るかハイライトを作る
	if (subId==0)
	{
		bmpShadow = cyiBitmapBlender::MakeShadow(tri, Color::Black, 2.0, 0.5);
		g.DrawImage(bmpShadow, 1, 1, 0, 0, bmpShadow->GetWidth(), bmpShadow->GetHeight(), UnitPixel);
	}
	else if (subId==1)
	{
		bmpShadow = cyiBitmapBlender::MakeShadow(tri, Color::White, 2.0, 1.0);
		g.DrawImage(bmpShadow, 0, 0, 0, 0, bmpShadow->GetWidth(), bmpShadow->GetHeight(), UnitPixel);
	}
	else
	{
		bmpShadow = cyiBitmapBlender::MakeShadow(tri, Color::Black, 1.0, 0.5);
		g.DrawImage(bmpShadow, 1, 1, 0, 0, bmpShadow->GetWidth(), bmpShadow->GetHeight(), UnitPixel);
	}

	g.DrawImage(tri, shadowDepth, shadowDepth);

	delete bmpShadow;
	delete tri;

	// 合成する。
	return ret; 
}


// ---------------------------------------
Bitmap* xDrawGlsSelectHiLight(int w, int h, color_t color, Bool bIsHalf)
{
	Bitmap* bmp = BITMAP32(w, h);
	Bitmap* bmp2 = xGetCircleBlure10();

	if (bmp2->GetLastStatus()!=Status::Ok)
	{
		return 0;
	}

	Bitmap* bmpx=cyiBitmapBlender::ArgbShift(bmp2, 0, (color>>16)&0xFF, (color>>8)&0xFF, color&0xFF);



	if (bIsHalf)
	{
		Bitmap temp(w, h, PixelFormat32bppARGB);
		Graphics g(&temp);
		g.DrawImage(bmpx, 0, 0, w, h);

		Graphics g2(bmp);
		g2.DrawImage(&temp, 0, h/2);
	}
	else
	{
		Graphics g(bmp);
		g.DrawImage(bmpx, 0, 0, w, h);
	}
	delete bmpx;
	return bmp;

}


#include "glsButton.h"
// ---------------------------------------
Bitmap* xDrawGlassBtnPannel(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra)
{
	// 共通
	sGlsButtonDetail* detail=(sGlsButtonDetail*)extra;
	int w_item = (bmpStr?bmpStr->GetWidth():0) + (bmp?bmp->GetWidth():0);
	int h_item = h;//(bmpStr?bmpStr->GetHeight():0); 
	float fMargin;
	float edge=detail?((detail==(void*)0xFFFFFFFF)?0.0:detail->edge):0.0;
	int nhMargin, nwMargin;

	//if (bmp==0 && bmpStr==0 && extra==0)
	//{
	//	w=detail->rect.right-1;
	//	h=detail->rect.bottom-1;
	//	nwMargin=0;
	//	nhMargin=0;
	//	goto SKIP_CALULATE;
	//}

	if (bmp)
	{
		h_item = h_item > bmp->GetHeight() ? h_item:bmp->GetHeight();
	}

	fMargin = (float)h_item/6.0;
	nhMargin = (int)fMargin;

	nhMargin=nhMargin>2?nhMargin:2;

	fMargin=(float)w_item/6.0;
	nwMargin = (int)fMargin;
	nwMargin=nwMargin>2?nwMargin:2;

	w=w_item+(nwMargin<<1);
	h=h_item+(nhMargin<<1);

	if (w<h)
	{
		w=h;
		nwMargin = (w-w_item)>>1;
	}

	if ((nwMargin<<1 )+w_item > w)
	{
		nwMargin = (w-w_item)>>1;
	}

	if (detail==(void*)0xFFFFFFFF)
	{
		return (Bitmap*)h;
	}

	if ((detail && 
			detail->rect.right!=0/* &&
				detail->rect.bottom!=0*/))
	{
		if (detail->rect.bottom==0)
		{
			detail->rect.bottom=h+1;
		}

		w=detail->rect.right-1;
		h=detail->rect.bottom-1;
		nwMargin=0;
		nhMargin=0;
	}

SKIP_CALULATE:
	// normal
	if (subId==0)
	{
		Bitmap* frame = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, w-2, h-2), edge);
		
		Graphics g(frame);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		//Bitmap* bmp3=cyiShape::FillRoundRect(&SolidBrush(Color(127, 255, 255, 255)), RectF(0, 0, w-2, h-2), 2.0);
		Bitmap* bmp3=cyiShape::FillRoundRect(&LinearGradientBrush(Point(0, 0), Point(0,h-2), Color(64, 32, 32, 32), Color(127, 255, 255, 255)), RectF(0, 0, w-2, h-2), edge);
		//Bitmap* bmp3=cyiShape::FillRoundRect(&LinearGradientBrush(Point(0, 0), Point(0,h-2), Color(127, 127, 127, 127), Color(200, 255, 255, 255)), RectF(0, 0, w-2, h-2), edge);

		g.DrawImage(bmp3, 0, 0);
		delete bmp3;

		if (bmp)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w-2, nwMargin, w_item);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h-2, nhMargin, bmp->GetHeight());
			g.DrawImage(bmp, x, y);
		}

		if (bmpStr)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w-2, nwMargin, w_item) + (bmp?bmp->GetWidth():0)+1+(bmp?1:0);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h-2, nhMargin, bmpStr->GetHeight())+2;
			g.DrawImage(bmpStr, x, y-1);
		}


		Bitmap* ret = cyiBitmapBlender::DropShadowEx(frame, 1, 1, 1, 0.3);
		delete frame;
		return ret;
	}
	else if (subId==1) // MouseMove
	{
		// Mouse Enter
		Bitmap* frame = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, w-2, h-2), edge);

		Graphics g(frame);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(8, 16, 64, 16)), RectF(0, 0, w-2, h-2), edge);
		Bitmap* bmp5 = xDrawGlsSelectHiLight(w-2, h-2, detail->colorHilight);

		g.DrawImage(bmp5, 0, 0);
		g.DrawImage(bmp3, 0, 0);

		if (bmp)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w-2, nwMargin, w_item);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h-2, nhMargin, bmp->GetHeight());
			g.DrawImage(bmp, x, y);
		}

		if (bmpStr)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w-2, nwMargin, w_item) + (bmp?bmp->GetWidth():0)+1+(bmp?1:0);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h-2, nhMargin, bmpStr->GetHeight())+2;
			g.DrawImage(bmpStr, x, y-1);
		}

		Bitmap* ret = cyiBitmapBlender::DropShadowEx(frame, 1, 1, 1, 0.3);
		delete frame;
		delete bmp5;
		delete bmp3;

		return ret;
	}
	else if (subId==2) // MouseDown
	{
		//// Mouse Down
		Bitmap* bmpx = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, w-2, h-2), edge);

		Graphics g(bmpx);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(32, 32, 32, 32)), RectF(0, 0, w-2, h-2), edge);
		g.DrawImage(bmp3, 0, 0);

		if (bmp)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w-2, nwMargin, w_item);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h-2, nhMargin, bmp->GetHeight());
			g.DrawImage(bmp, x, y);
		}

		if (bmpStr)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w-2, nwMargin, w_item) + (bmp?bmp->GetWidth():0)+1+(bmp?1:0);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h-2, nhMargin, bmpStr->GetHeight())+2;
			g.DrawImage(bmpStr, x, y-1);
		}

		Bitmap* bmp5 = cyiBitmapBlender::DropShadowEx(bmpx, 0, 0, 1.0, 0.5);

		Bitmap* ret = BITMAP32(w+1, h+1);
		Graphics gx(ret);

		gx.DrawImage(bmp5, 1, 1);
		
		delete(bmp3);
		delete bmpx;
		delete bmp5;

		return ret;
	}

	return 0;
}

// ---------------------------------------
Bitmap* xDrawNeonBtnPannel(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra)
{
	// 共通
	sGlsButtonDetail* detail=(sGlsButtonDetail*)extra;
	int w_item = (bmpStr?bmpStr->GetWidth():0) + (bmp?bmp->GetWidth():0);
	int h_item = h;//(bmpStr?bmpStr->GetHeight():0); 
	float fMargin;
	float edge=detail?((detail==(void*)0xFFFFFFFF)?0.0:detail->edge):0.0;
	int nhMargin, nwMargin;

	if (bmp)
	{
		h_item = h_item > bmp->GetHeight() ? h_item:bmp->GetHeight();
	}

	fMargin = (float)h_item/6.0;
	nhMargin = (int)fMargin;

	nhMargin=nhMargin>2?nhMargin:2;

	fMargin=(float)w_item/6.0;
	nwMargin = (int)fMargin;
	nwMargin=nwMargin>2?nwMargin:2;

	w=w_item+(nwMargin<<1);
	h=h_item+(nhMargin<<1);

	if (w<h)
	{
		w=h;
		nwMargin = (w-w_item)>>1;
	}

	if ((nwMargin<<1 )+w_item > w)
	{
		nwMargin = (w-w_item)>>1;
	}


	if (subId==0)
	{
		Bitmap* ret = BITMAP32(w, h);
		Graphics g(ret);

		if (bmp)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w, nwMargin, w_item);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h, nhMargin, bmp->GetHeight());
			g.DrawImage(bmp, x, y);
		}

		if (bmpStr)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w, nwMargin, w_item) + (bmp?bmp->GetWidth():0)+1+(bmp?1:0);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h, nhMargin, bmpStr->GetHeight())+2;
			g.DrawImage(bmpStr, x, y-1);
		}

		Bitmap* temp = cyiBitmapBlender::BitmapCopy(0, ret);

		g.DrawImage(temp, 0, 0);

		delete temp;

		return ret;
	}


	return 0;
}


// ---------------------------------------
Bitmap* xDrawGlassToolPannel(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra)
{
	// 共通
	sGlsButtonDetail* detail=(sGlsButtonDetail*)extra;
	int w_item = (bmpStr?bmpStr->GetWidth():0) + (bmp?bmp->GetWidth():0);
	int h_item = h;//(bmpStr?bmpStr->GetHeight():0); 
	float fMargin;
	float edge=detail->edge;
	int nhMargin, nwMargin;

	//if (bmp==0 && bmpStr==0 && extra==0)
	//{
	//	w=detail->rect.right-1;
	//	h=detail->rect.bottom-1;
	//	nwMargin=0;
	//	nhMargin=0;
	//	goto SKIP_CALULATE;
	//}

	if (bmp)
	{
		h_item = h_item > bmp->GetHeight() ? h_item:bmp->GetHeight();
	}

	fMargin = (float)h_item/4.0;
	nhMargin = (int)fMargin;

	nhMargin=nhMargin>2?nhMargin:2;

	fMargin=(float)w_item/6.0;
	nwMargin = (int)fMargin;
	nwMargin=nwMargin>2?nwMargin:2;

	w=w_item+(nwMargin<<1);
	h=h_item+(nhMargin<<1);

	if (w<h)
	{
		w=h;
		nwMargin = (w-w_item)>>1;
	}

	if ((nwMargin<<1 )+w_item > w)
	{
		nwMargin = (w-w_item)>>1;
	}

	if ((detail && 
			detail->rect.right!=0 &&
				detail->rect.bottom!=0))
	{
		w=detail->rect.right-1;
		h=detail->rect.bottom-1;
		nwMargin=0;
		nhMargin=0;
	}

SKIP_CALULATE:
	// normal
	if (subId==1) // MouseMove
	{
		// Mouse Enter
		Bitmap* frame = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, w-1, h-1), edge);

		Graphics g(frame);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(8, 16, 64, 16)), RectF(0, 0, w-1, h-1), edge);
		Bitmap* bmp5 = xDrawGlsSelectHiLight(w-2, h-2, detail->colorHilight);

		g.DrawImage(bmp5, 0, 0);
		g.DrawImage(bmp3, 0, 0);

		if (bmp)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w-1, nwMargin, w_item);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h-1, nhMargin, bmp->GetHeight());
			g.DrawImage(bmp, x, y);
		}

		if (bmpStr)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w-1, nwMargin, w_item) + (bmp?bmp->GetWidth():0)+1+(bmp?1:0);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h-1, nhMargin, bmpStr->GetHeight())+2;
			g.DrawImage(bmpStr, x, y);
		}

		Bitmap* ret = cyiBitmapBlender::DropShadowEx(frame, 0, 0, 1, 0.3);
		delete frame;
		delete bmp5;
		delete bmp3;

		return ret;
	}
	else if (subId==0) // MouseDown
	{
		//// Mouse Down
		Bitmap* bmpx = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, w-1, h-1), edge);

		Graphics g(bmpx);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(32, 32, 32, 32)), RectF(0, 0, w-1, h-1), edge);
		g.DrawImage(bmp3, 0, 0);

		if (bmp)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w-1, nwMargin, w_item);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h-1, nhMargin, bmp->GetHeight());
			g.DrawImage(bmp, x, y);
		}

		if (bmpStr)
		{
			int x = cglsResourceObject::xGetPos(eIA_CENTER, w-1, nwMargin, w_item) + (bmp?bmp->GetWidth():0)+1+(bmp?1:0);
			int y = cglsResourceObject::xGetPos(eIA_CENTER, h-1, nhMargin, bmpStr->GetHeight())+2;
			g.DrawImage(bmpStr, x, y);
		}

		Bitmap* bmp5 = cyiBitmapBlender::DropShadowEx(bmpx, 0, 0, 1.0, 0.5);

		//Bitmap* ret = BITMAP32(w+1, h+1);
		//Graphics gx(ret);

		//gx.DrawImage(bmp5, 0, 0);
		
		delete(bmp3);
		delete bmpx;
		//delete bmp5;

		//return ret;
		return bmp5;
	}

	return 0;
}


// ---------------------------------------
Bitmap* xDrawGlassSplitBtnPannel(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra)
{
	// 共通
	sGlsButtonDetail* detail=(sGlsButtonDetail*)extra;
	int w_item = w;//bmp->GetWidth();//(bmpStr?bmpStr->GetWidth():0) + (bmp?bmp->GetWidth():0);
	int h_item = h;//(bmpStr?bmpStr->GetHeight():0); 
	Bitmap* bmpTri;
	float edge=detail->edge;
	float fMargin = (float)h_item/6.0;
	int nhMargin = (int)fMargin;

	h = h_item+(nhMargin<<1);


	bmpTri = xDrawGlsArrow(Color::Black, h/3, h/3, 0);

	w+=bmpTri->GetWidth()+3;
	//h+=bmpTri->GetHeight();

	if (detail->rect.right!=0 &&
			detail->rect.bottom!=0)
	{
		w=detail->rect.right;
		h=detail->rect.bottom;
	}

	// normal
	if (subId==0)
	{
		Bitmap* frame = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, w-2, h-2), edge);
		
		Graphics g(frame);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3=cyiShape::FillRoundRect(&LinearGradientBrush(Point(0, 0), Point(0,h-2), Color(64, 32, 32, 32), Color(127, 255, 255, 255)), RectF(0, 0, w-2, h-2), edge);

		g.DrawImage(bmp3, 0, 0);
		delete bmp3;

		if (bmp)
		{
			int _w=w-4-bmpTri->GetWidth();
			int _h=h;//-bmpTri->GetHeight();
			int x = cglsResourceObject::xGetPos(eIA_CENTER, _w, 0, bmp->GetWidth());
			int y = cglsResourceObject::xGetPos(eIA_CENTER, _h-2, 0, bmp->GetHeight())+1;
			g.DrawImage(bmp, x, y);
		}

		g.DrawImage(bmpTri, (w-2-3)-(int)(bmpTri->GetWidth()), ((h-2)-(int)(bmpTri->GetHeight()))>>1);

		Bitmap* ret = cyiBitmapBlender::DropShadowEx(frame, 1, 1, 1, 0.3);
		delete frame;
		delete bmpTri;
		return ret;
	}
	else if (subId==1) // MouseMove
	{
		// Mouse Enter
		Bitmap* frame = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, w-2, h-2), edge);

		Graphics g(frame);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(8, 16, 64, 16)), RectF(0, 0, w-2, h-2), edge);
		Bitmap* bmp5 = xDrawGlsSelectHiLight(w-2, h-2, detail->colorHilight);

		g.DrawImage(bmp5, 0, 0);
		g.DrawImage(bmp3, 0, 0);
		delete bmp5;
		delete bmp3;

		if (bmp)
		{
			int _w=w-bmpTri->GetWidth();
			int _h=h;//-bmpTri->GetHeight();
			int x = cglsResourceObject::xGetPos(eIA_CENTER, _w-4, 0, bmp->GetWidth());
			int y = cglsResourceObject::xGetPos(eIA_CENTER, _h-2, 0, bmp->GetHeight())+1;
			g.DrawImage(bmp, x, y);
		}

		g.DrawImage(bmpTri, (w-2-3)-(int)(bmpTri->GetWidth()), ((h-2)-(int)(bmpTri->GetHeight()))>>1);
		//g.DrawImage(bmpTri, (w-2)-(int)bmpTri->GetWidth(), ((h-2)-(int)bmpTri->GetHeight())>>1);

		cyiBitmapBlender::FillColor(frame, 0, &Rect(frame->GetWidth()-bmpTri->GetWidth()-5-1, 0, 1, frame->GetHeight()));

		g.DrawLine(&Pen(Color::Black), frame->GetWidth()-bmpTri->GetWidth()-5-2, 0, frame->GetWidth()-bmpTri->GetWidth()-5-2, h);
		g.DrawLine(&Pen(Color::Black), frame->GetWidth()-bmpTri->GetWidth()-5, 0, frame->GetWidth()-bmpTri->GetWidth()-5, h);

		Bitmap* ret = cyiBitmapBlender::DropShadowEx(frame, 1, 1, 1, 0.3);

		delete frame;
		delete bmpTri;

		return ret;
	}
	else if (subId==2) // MouseDown
	{
		//// Mouse Down
		Bitmap* bmpx = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, w-2, h-2), edge);

		Graphics g(bmpx);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(32, 32, 32, 32)), RectF(0, 0, w-2, h-2), edge);
		g.DrawImage(bmp3, 0, 0);

		if (bmp)
		{
			int _w=w-bmpTri->GetWidth();
			int _h=h;//-bmpTri->GetHeight();
			int x = cglsResourceObject::xGetPos(eIA_CENTER, _w-4, 0, bmp->GetWidth());
			int y = cglsResourceObject::xGetPos(eIA_CENTER, _h-2, 0, bmp->GetHeight())+1;
			g.DrawImage(bmp, x, y);
		}

		//g.DrawImage(bmpTri, (w-2)-(int)bmpTri->GetWidth(), ((h-2)-(int)bmpTri->GetHeight())>>1);
		g.DrawImage(bmpTri, (w-2-3)-(int)(bmpTri->GetWidth()), ((h-2)-(int)(bmpTri->GetHeight()))>>1);

		cyiBitmapBlender::FillColor(bmpx, 0, &Rect(bmpx->GetWidth()-bmpTri->GetWidth()-5-1, 0, 1, bmpx->GetHeight()));
		//cyiBitmapBlender::FillColor(bmpx, 0, &Rect(w-bmpTri->GetWidth(), 0, w, h));

		g.DrawLine(&Pen(Color::Black),bmpx->GetWidth()-bmpTri->GetWidth()-5-2, 0, bmpx->GetWidth()-bmpTri->GetWidth()-5-2, h);
		g.DrawLine(&Pen(Color::Black), bmpx->GetWidth()-bmpTri->GetWidth()-5, 0, bmpx->GetWidth()-bmpTri->GetWidth()-5, h);

		Bitmap* bmp5 = cyiBitmapBlender::DropShadowEx(bmpx, 0, 0, 1.0, 0.5);
		Bitmap* ret = BITMAP32(w+1, h+1);
		Graphics gx(ret);

		gx.DrawImage(bmp5, 1, 1);
		
		delete(bmp3);
		delete bmpx;
		delete bmp5;
		delete bmpTri;

		return ret;
	}

	return 0;
}

// ---------------------------------------
static Bitmap* xDrawNeonSplitBtnPannel(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra)
{
	// 共通
	sGlsButtonDetail* detail=(sGlsButtonDetail*)extra;
	int w_item = w;//bmp->GetWidth();//(bmpStr?bmpStr->GetWidth():0) + (bmp?bmp->GetWidth():0);
	int h_item = h;//(bmpStr?bmpStr->GetHeight():0); 
	Bitmap* bmpTri;
	float edge=detail->edge;
	float fMargin = (float)h_item/6.0;
	int nhMargin = (int)fMargin;

	h = h_item+(nhMargin<<1);


	bmpTri = xDrawGlsArrow(Color::White, h/3, h/3, 0);

	w+=bmpTri->GetWidth()+3;
	//h+=bmpTri->GetHeight();

	if (detail->rect.right!=0 &&
			detail->rect.bottom!=0)
	{
		w=detail->rect.right;
		h=detail->rect.bottom;
	}

	// normal
	if (subId==0)
	{
		Bitmap* frame = cyiShape::DrawRoundRect(&Pen(Color::LightGray, 1.0), RectF(0, 0, w-2, h-2), edge);
		
		Graphics g(frame);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3=cyiShape::FillRoundRect(&LinearGradientBrush(Point(0, 0), Point(0,h-2), Color(16, 32, 32, 32), Color(64, 255, 255, 255)), RectF(0, 0, w-2, h-2), edge);

		g.DrawImage(bmp3, 0, 0);
		delete bmp3;

		if (bmp)
		{
			int _w=w-4-bmpTri->GetWidth();
			int _h=h;//-bmpTri->GetHeight();
			int x = cglsResourceObject::xGetPos(eIA_CENTER, _w, 0, bmp->GetWidth());
			int y = cglsResourceObject::xGetPos(eIA_CENTER, _h, 0, bmp->GetHeight())+1;
			g.DrawImage(bmp, x, y);
		}

		g.DrawImage(bmpTri, (w-2-3)-(int)(bmpTri->GetWidth()), ((h-2)-(int)(bmpTri->GetHeight()))>>1);

		//Bitmap* ret = cyiBitmapBlender::DropShadowEx(frame, 1, 1, 1, 0.3);
		//delete frame;
		//delete bmpTri;
		return frame;
	}
	else if (subId==1) // MouseMove
	{
		// Mouse Enter
		Bitmap* frame = cyiShape::DrawRoundRect(&Pen(Color::White, 1.0), RectF(0, 0, w-2, h-2), edge);

		Graphics g(frame);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(8, 16, 64, 16)), RectF(0, 0, w-2, h-2), edge);
		//Bitmap* bmp5 = xDrawGlsSelectHiLight(w-2, h-2, detail->colorHilight);

		//g.DrawImage(bmp5, 0, 0);
		g.DrawImage(bmp3, 0, 0);
		//delete bmp5;
		delete bmp3;

		if (bmp)
		{
			int _w=w-bmpTri->GetWidth();
			int _h=h;//-bmpTri->GetHeight();

			Bitmap bmpTemp(frame->GetWidth(), frame->GetHeight(), PixelFormat32bppARGB);
			Graphics gTemp(&bmpTemp);
			int x = cglsResourceObject::xGetPos(eIA_CENTER, _w-4, 0, bmp->GetWidth());
			int y = cglsResourceObject::xGetPos(eIA_CENTER, _h, 0, bmp->GetHeight())+1;

			gTemp.DrawImage(bmp, x, y);
			gTemp.DrawImage(bmpTri, (w-2-3)-(int)(bmpTri->GetWidth()), ((h-2)-(int)(bmpTri->GetHeight()))>>1);

			Bitmap* bmpNeon = cBB::NeonSign(&bmpTemp, detail->colorHilight, True);
			g.DrawImage(bmpNeon, 0, 0);
			delete bmpNeon;
		}

		//Bitmap* bmpTriNeon = cBB::NeonSign(bmpTri, detail->colorHilight, True);
		//g.DrawImage(bmpTriNeon, (w-2-3)-(int)(bmpTri->GetWidth()), ((h-2)-(int)(bmpTri->GetHeight()))>>1);
		//delete bmpTriNeon;
		//g.DrawImage(bmpTri, (w-2)-(int)bmpTri->GetWidth(), ((h-2)-(int)bmpTri->GetHeight())>>1);

		cyiBitmapBlender::FillColor(frame, 0, &Rect(frame->GetWidth()-bmpTri->GetWidth()-5-1, 0, 1, frame->GetHeight()));

		g.DrawLine(&Pen(Color::White), frame->GetWidth()-bmpTri->GetWidth()-5-2, 0, frame->GetWidth()-bmpTri->GetWidth()-5-2, h);
		g.DrawLine(&Pen(Color::White), frame->GetWidth()-bmpTri->GetWidth()-5, 0, frame->GetWidth()-bmpTri->GetWidth()-5, h);

		//Bitmap* ret = cyiBitmapBlender::DropShadowEx(frame, 1, 1, 1, 0.3);

		//delete frame;
		delete bmpTri;

		return frame;
	}
	else if (subId==2) // MouseDown
	{
		//// Mouse Down
		Bitmap* bmpx = cyiShape::DrawRoundRect(&Pen(Color::LightGray, 1.0), RectF(0, 0, w-2, h-2), edge);

		Graphics g(bmpx);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(32, 32, 32, 32)), RectF(0, 0, w-2, h-2), edge);
		g.DrawImage(bmp3, 0, 0);

		if (bmp)
		{
			int _w=w-bmpTri->GetWidth();
			int _h=h;//-bmpTri->GetHeight();
			int x = cglsResourceObject::xGetPos(eIA_CENTER, _w-4, 0, bmp->GetWidth());
			int y = cglsResourceObject::xGetPos(eIA_CENTER, _h, 0, bmp->GetHeight())+1;
			Bitmap bmpTemp(bmpx->GetWidth(), bmpx->GetHeight(), PixelFormat32bppARGB);
			Graphics gTemp(&bmpTemp);

			gTemp.DrawImage(bmp, x, y);
			gTemp.DrawImage(bmpTri, (w-2-3)-(int)(bmpTri->GetWidth()), ((h-2)-(int)(bmpTri->GetHeight()))>>1);

			Bitmap* bmpNeon = cBB::NeonSign(&bmpTemp, detail->colorHilight, False);
			g.DrawImage(bmpNeon, 0, 0);
			delete bmpNeon;
		}

		//g.DrawImage(bmpTri, (w-2)-(int)bmpTri->GetWidth(), ((h-2)-(int)bmpTri->GetHeight())>>1);
		//g.DrawImage(bmpTri, (w-2-3)-(int)(bmpTri->GetWidth()), ((h-2)-(int)(bmpTri->GetHeight()))>>1);

		cyiBitmapBlender::FillColor(bmpx, 0, &Rect(bmpx->GetWidth()-bmpTri->GetWidth()-5-1, 0, 1, bmpx->GetHeight()));
		//cyiBitmapBlender::FillColor(bmpx, 0, &Rect(w-bmpTri->GetWidth(), 0, w, h));

		g.DrawLine(&Pen(Color::LightGray),bmpx->GetWidth()-bmpTri->GetWidth()-5-2, 0, bmpx->GetWidth()-bmpTri->GetWidth()-5-2, h);
		g.DrawLine(&Pen(Color::LightGray), bmpx->GetWidth()-bmpTri->GetWidth()-5, 0, bmpx->GetWidth()-bmpTri->GetWidth()-5, h);

		//Bitmap* bmp5 = cyiBitmapBlender::DropShadowEx(bmpx, 0, 0, 1.0, 0.5);
		//Bitmap* ret = BITMAP32(w+1, h+1);
		//Graphics gx(ret);

		//gx.DrawImage(bmp5, 1, 1);
		
		delete(bmp3);
		//delete bmpx;
		//delete bmp5;
		delete bmpTri;

		return bmpx;
	}

	return 0;
}

#include "glsSplitButton.h"

// ---------------------------------------
//Bitmap* xDrawGlassSplitBtnPannel2(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra)
//{
//	sGlsSplitButtonDetail* detail=(sGlsSplitButtonDetail*)extra;
//
//	bmpTri = xDrawGlsArrow(Color::Black, h/3, h/3, 0);
//
//	int rw=bmpTri->GetWidth()+3;
//
//	
//
//	Bitmap* bmpLeft=xCreateLeftSplitPannel(w, h);
//	Bitmap* bmpright=xCreateRightSplitPannel(rw, h);
//
//	// 左側のイメージを生成
//	if (detail->eStyle&eSBS_LEFT_FULLDOWN)
//	{
//		
//	}
//	else
//	{
//	}
//
//	// 右側のイメージを生成
//	if (detail->eStyle&eSBS_LEFT_FULLDOWN)
//	{
//	}
//	else
//	{
//	}
//
//	return 0;
//}

// ---------------------------------------
static Bitmap* xMakeCloseBtnBitmap(int subId, int w, int h)
{
	color_t color;
	w=h;

	if (subId==0)
	{
		color=xARGB(255, 200, 0, 0);
	}
	else if (subId==1)
	{
		color=xARGB(255, 255, 0, 0);
	}
	else if (subId==2)
	{
		color=xARGB(255, 127, 127, 127);
	}

	Bitmap* bmp = cyiShape::DrawCross(0, &Pen(Color::White, 2.0), RectF(0, 0, w-2, h-2));
	Bitmap* bg = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black, 1.0), &SolidBrush(color), RectF(0, 0, w, h), 2.0);

	Graphics g(bg);

	g.DrawImage(bmp, 1, 1);

	delete bmp;
	return bg;
}


//// ---------------------------------------
//Bitmap* xDrawScrollBarPage(subId, w, h, bmp, bmpStr, extra)
//{
//}

//typedef struct xsScrollBarDraw {
//	Bool bIsVertical;
//	color_t color;
//	float rad;
//}sScrollBarDraw;
// ---------------------------------------
Bitmap* xDrawScrollBar(int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra)
{
//	sScrollBarDraw* s=(sScrollBarDraw*) extra;

	int shadowDepth=2;
	int _w = (w-(shadowDepth<<1))-1;
	int _h = (h-(shadowDepth<<1))-1;

	Bitmap* x = cyiShape::FillRoundRect(&SolidBrush(Color::White), RectF(0, 0, _w, _h), 3.0);
	Bitmap* bmpShadow;
	Bitmap* ret = BITMAP32(w, h);
	Graphics g(ret);

	if (subId==0)
	{
		bmpShadow = cyiBitmapBlender::MakeShadow(x, Color::Black, 2.0, 0.5);
		g.DrawImage(bmpShadow, 1, 1);
	}
	else if (subId==1)
	{
		bmpShadow = cyiBitmapBlender::MakeShadow(x, __COLORp__(127, 255,255,255), 2.0, 0.5);
		g.DrawImage(bmpShadow, 0, 0);
	}
	else
	{
		bmpShadow = cyiBitmapBlender::MakeShadow(x, Color::Black, 1.0, 0.5);
		g.DrawImage(bmpShadow, 1, 1);
	}

	g.DrawImage(x, shadowDepth, shadowDepth);

	delete x;
	delete bmpShadow;

	return ret; 
}

// ---------------------------------------
Bitmap* xDrawIconMsg(int subId, int w, int h)
{
	Bitmap* bmp = BITMAP32(w,h);
	Graphics g(bmp);
	int iconH = h>>1;
	int y = (h-iconH)>>2;
	
	subId=Color::Black;
	g.DrawRectangle(&Pen(Color(subId), 2.0), 2, y, w-4, iconH);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	g.DrawLine(&Pen(Color(subId), 1.0), 2, y, w>>1, h>>1);
	g.DrawLine(&Pen(Color(subId), 1.0), w-4, y, w>>1, h>>1);

	return bmp;
}


// ---------------------------------------
Bitmap* xDrawIconMenu(int subId, int w, int h)
{
	Bitmap* bmp = BITMAP32(w,h);
	Graphics g(bmp);

	subId=Color::Black;

	int wMenu=(w>>2)*3;

	g.DrawRectangle(&Pen(Color(subId)), Rect(1, 1, wMenu-1, h-1));
	g.DrawLine(&Pen(Color(subId)), 0, h>>2, wMenu-1, h>>2);
	g.DrawLine(&Pen(Color(subId)), 0, h>>1, wMenu-1, h>>1);
	g.DrawLine(&Pen(Color(subId)), 0, (h>>1)+(h>>2), wMenu, (h>>1)+(h>>2));

	Bitmap* bmpArr = xDrawGlsArrow(subId, w-wMenu-1, h>>1, 1);
	g.DrawImage(bmpArr, wMenu+1, h>>2);
	delete bmpArr;

	return bmp;
}

// ----------------------------------
Bitmap* xMakeCircleInChar(float scale, wchar_t* ch, Font* font, color_t color, color_t color_txt)
{
	//int rad = (float)((float)MAX_BLOCK_SIZE*scale*0.6);
	//int rad_hf=rad>>1;
	//Bitmap* bmp = new Bitmap(rad, rad, PixelFormat32bppARGB);

	//Graphics g(bmp);
	//
	//g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	//g.DrawEllipse(&Pen(color, 1.0+scale), (float)0.0, (float)0.0, (float)rad-(1.0+scale), (float)rad-(1.0+scale));

	//Bitmap* bmpStr = cyiShape::BitmapString(0, ch, font, &SolidBrush(color_txt));

	//int w=bmpStr->GetWidth();
	//int h=bmpStr->GetHeight();

	//g.DrawImage(bmpStr, (INT)((rad-w)/2), (INT)((rad-h)/2)+1);//, (INT)bmpStr->GetWidth(), (INT)bmpStr->GetHeight());
	//delete bmpStr;

	//Bitmap* ret= cyiBitmapBlender::DropShadow(bmp, 0, 0, 1.0, 0.5);

	//delete bmp;

	//return ret;
	return 0;
}


// ---------------------------------------
Bitmap* xDrawIconNotShowNo(color_t subId, int w, int h)
{
	return 0;
}

// ---------------------------------------
Bitmap* xDrawIconShowNo(color_t subId, int w, int h)
{
	return 0;
}

// ---------------------------------------
Bitmap* xDrawIconShowNoFrom(color_t subId, int w, int h)
{
	return 0;
}


// ---------------------------------------
Bitmap* xDrawIconDetail(int subId, int w, int h)
{
	return 0;
}

// ---------------------------------------
Bitmap* xDrawBranch(color_t subId, int w, int h)
{

	return 0;
}

// ---------------------------------------
Bitmap* xDrawRadio(int subId, int w, int h)
{
	Bitmap* bmpMask = BITMAP32(h, h);
	Graphics gMask(bmpMask);

	gMask.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	gMask.FillEllipse(&SolidBrush(Color::Black), 0, 0, h-1, h-1);
	Bitmap* bg = cyiShape::FillRadialGradiation(BITMAP32(h, h), h*2/3, h*2/3, h+h/3, Color::White, Color::Gray);	

	cyiBitmapBlender::SetMask(bg, bmpMask, 0, 0, False);
	////cyiShape::SetMask(bg, bmpMask, 0, 0, False);
	Graphics g(bg);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.DrawEllipse(&Pen(Color::DarkBlue), 0, 0, h-1, h-1);

	delete bmpMask;

	if (subId!=0)
	{
		int hRadio=h-4;
		Bitmap* bmpRadioMask = BITMAP32(hRadio, hRadio);
		Graphics gRadioMask(bmpRadioMask);
		gRadioMask.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		gRadioMask.FillEllipse(&SolidBrush(Color::Black), 0, 0, hRadio, hRadio);

		Bitmap* bmpRadio = cyiShape::FillRadialGradiation(BITMAP32(hRadio, hRadio), hRadio/3, hRadio/3, hRadio+hRadio/3,
			Color::White, Color::DarkBlue);	

		cyiBitmapBlender::SetMask(bmpRadio, bmpRadioMask, 0, 0, False);
		g.DrawImage(bmpRadio, 2, 2);
		
		delete bmpRadio;
		delete bmpRadioMask;
	}
	

	return bg;
}

// ---------------------------------------
Bitmap* xDrawCheck(int subId, int w, int h)
{
	Bitmap* bg = BITMAP32(h, h);
	Graphics g(bg);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	cyiShape::FillRadialGradiation(bg, h*2/3, h*2/3, h+h/3, Color::White, Color::Gray);
	g.DrawRectangle(&Pen(Color::Black), 0, 0, h-1, h-1);

	if (subId==0)
	{
		return bg;
	}

	g.DrawLine(&Pen(Color::Black,2), 2, h/2, h/3, h-h/4);
	g.DrawLine(&Pen(Color::Black,2), h/3, h-h/4, h-2, h/4);
	return bg;
}

// ---------------------------------------
Bitmap* xDrawChangedMark(int subId, int w, int h)
{
	Font* font=(Font*)w;
	return cyiShape::BitmapString(0, L"*", font, &SolidBrush(Color::Black));
}

// ---------------------------------------
Bitmap* xDrawToolCross(int subId, int w, int h)
{
	color_t color=Color::Black;

	if (subId==1)
	{
		color=Color::White;
	}
	else if (subId==2)
	{
		color=Color::Gray;
	}
	else if (subId==4)
	{
		color=Color::Orange;
	}

	h = (float)h*2/3;

	Bitmap* bmp = cyiShape::DrawCross(0, &Pen(color, 2.0), RectF(0, 0, h, h));


	//if (bIsCursor)
	//{
	//	Bitmap* cur = cyiBitmapBlender::DropShadow(bmp, scale, scale, 2, 0.3);
	//	return bmp;
	//}

	////return bmp;
	//Bitmap* ret= cyiBitmapBlender::DropShadow(bmp, 0, 0, 1.0, 0.5);

	//delete bmp;
	return bmp;
}

// ---------------------------------------
Bitmap* xDrawToolBtnArrow(int subId, int direct, int h)
{
	color_t color=Color::Black;//__COLORp__(255, 230, 230, 230);
	int w=(h<<1)/3;

	if (subId==1)
	{
		color=Color::White;
	}
	else if (subId==2)
	{
		color=Color::Gray;
	}
	else if (subId==4)
	{
		color=Color::Orange;
	}

	int x=(h-w)>>1;

	Bitmap* bmp;
	
	if (direct==0 ||
		direct==2)
	{
		bmp = cyiShape::DrawTriAngleEx(BITMAP32(h,h), direct, 0, &SolidBrush(Color(color)), RectF(2, x+2, h-4, w-4));
	}
	else
	{
		bmp = cyiShape::DrawTriAngleEx(BITMAP32(h,h), direct, 0, &SolidBrush(Color(color)), RectF(x+2, 2, w-4, h-4));
	}


	if (subId==0 ||
			subId==2 ||
				subId==4)
	{
		return bmp;
	}

	Bitmap* retBlure=cyiBitmapBlender::GaussBlureXX(bmp, 3.0, 0, 0);
	//Bitmap* retTrans=cyiBitmapBlender::Transparent(retBlure, 1.0);


	Graphics g(bmp);
	g.DrawImage(retBlure, 0, 0);
	g.DrawImage(retBlure, 0, 0);

	delete retBlure;


	return bmp;
}
