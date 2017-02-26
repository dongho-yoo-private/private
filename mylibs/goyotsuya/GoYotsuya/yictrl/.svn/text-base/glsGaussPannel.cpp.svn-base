#include "glsGaussPannel.h"
#include "yistd.h"

cglsGaussPannel::cglsGaussPannel(void)
:cGlassInterface(),
m_hShadow(0),
m_bmpBehind(0),
m_bmpShadow(0),
m_bmpFrame(0),
m_bmpBluredBg(0),
m_bIsAllRangeDraw(False),
m_prevUpdateOrder(0),
m_bIsDirect(False)
{
	sGlsGaussPannelDetail detail;
	xDefaultDetail(&detail);

	m_fBlureRad=detail.fBlureRad;
	m_fEdge=detail.fEdge;
	m_fShadowWidth=detail.fShadowWidth;

	m_colorTop=detail.colorTop;
	m_colorBottom=detail.colorBottom;
}

cglsGaussPannel::~cglsGaussPannel(void)
{
	//Destroy();

}

// --------------------------------
void cglsGaussPannel::xDefaultDetail(sGlsGaussPannelDetail* detail)
{
	detail->fEdge=3.0;
	detail->fShadowWidth=7.0;
	detail->fBlureRad=5.0;
	detail->colorTop=__COLORp__(127, 255, 255, 255);
	detail->colorBottom=__COLORp__(224, 255, 255, 255);
	detail->colorTop=__COLORp__(180, 255, 255, 255);
	detail->colorBottom=__COLORp__(255, 255, 255, 255);
}

// --------------------------------
void cglsGaussPannel::SetGaussPannelDetail(sGlsGaussPannelDetail* detail)
{
	m_fBlureRad=detail->fBlureRad;
	m_fEdge=detail->fEdge;
	m_fShadowWidth=detail->fShadowWidth;

	m_colorTop=detail->colorTop;
	m_colorBottom=detail->colorBottom;
}


// -----------------------------
Bitmap* cglsGaussPannel::GetBgBitmap()
{
	return m_bmpBluredBg;
}

// ------------------------------------
Bitmap* cglsGaussPannel::GetParentBg(Bitmap* bmp, RECT* rect, cGlassInterface* sender)
{
	RECT _rect;

	if (rect==0)
	{
		_rect.left=0;
		_rect.top=0;
		_rect.right=m_w;
		_rect.bottom=m_h;
		rect = &_rect;
	}

	if (sender==this)
	{
		return __super::GetParentBg(bmp, rect, sender);
	}
	else
	{
		
		if (m_bmpBluredBg)
		{
			if (bmp==0)
				bmp = BITMAP32(rect->right, rect->bottom);
			cyiBitmapBlender::BitmapCopy(bmp, m_bmpBluredBg, 0, 0, rect->left, rect->top, rect->right, rect->bottom);

			return bmp;
		}
		else
		{
			return __super::GetParentBg(bmp, rect, sender);
		}
	}

	return 0;
}

// -----------------------------
Bool cglsGaussPannel::InvalidateRect(RECT* lpRect, Bool bErase, Bool bIsDirect)
{
	//m_glsDrawInterface->m_nRequestUpdateCount++;
	m_bIsDirect=bIsDirect;
	if (lpRect==0)
	{
		m_bIsAllRangeDraw=True;
	}

	return cGlassInterface::InvalidateRect(lpRect, bErase, bIsDirect);
}

// -----------------------------
Bool cglsGaussPannel::GetClientRect(LPRECT lpRect)
{
	if (m_hShadow==0)
	{
		return __super::GetClientRect(lpRect);
	}

	int w = m_hShadow==0?0:cyiShape::GetShadowSize(m_hShadow);
	lpRect->left=w;
	lpRect->top=w;
	lpRect->right=m_w-(w<<1);
	lpRect->bottom=m_h-(w<<1);
	return True;
}

// ------------------------------------
void cglsGaussPannel::Destroy()
{
	if (m_bIsCreated==False)
		return ;

	if (m_bmpBehind)
		delete m_bmpBehind;
	m_bmpBehind=0;

	if (m_bmpBluredBg)
		delete m_bmpBluredBg;
	m_bmpBluredBg=0;

	if (m_bmpShadow)
		delete m_bmpShadow;
	m_bmpShadow=0;

	if (m_hShadow)
		cyiShape::DestroyShadow(m_hShadow);
	m_hShadow=0;

	if (m_bmpFrame)
		delete m_bmpFrame;
	m_bmpFrame=0;

	__super::Destroy();

}

// ------------------------------------
Bool cglsGaussPannel::OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX, int offsetY)
{
	if (m_bIsCreated==False)
	{
		return False;
	}
	Bool bIsSizeChanged=False;
	int w;
	int h;
	int nRad;
	int nShadowWidth;
	int nClientWidth;
	int nClientHeight;
	Bool bIsClientAreaUpdated=False;
	RECT rectDrawBlure;
	int nDrawOrder = GetDrawOrder();
#ifdef _DEBUG
	//xTRACE("cglsGaussPannel::OnDrawMyself(%s, %d, %d) m_bmpBehind:%x, (%d, %d, %d, %d)", m_szClassName, nDrawOrder, m_glsDrawInterface->m_nRequestUpdateOrder, m_bmpBehind, childRect->left, childRect->top, childRect->right, childRect->bottom);
#endif
	if (m_bmpBehind==0)
	{
		if (childRect->right!=m_w ||
				childRect->bottom!=m_h)
		{
			return False;
		}
	}

	if (m_bIsDirect==True)
	{
		m_bIsDirect=False;
		bIsSizeChanged=True;
		goto FORCED;
	}

	if (m_bmpBehind)
	{
		if (m_bmpBehind->GetWidth()!=m_w ||
				m_bmpBehind->GetHeight()!=m_h)
		{
			xTRACE("cglsGaussPannel::OnDrawMyself() size is changed");
			bIsSizeChanged=True;
			goto FORCED;
		}
	}
	else
	{
		goto FORCED;
	}

	if (m_glsDrawInterface->m_nRequestUpdateOrder<=nDrawOrder)
	{
		if (m_glsDrawInterface->m_nRequestUpdateOrder==nDrawOrder)
		{
			if (m_prevUpdateOrder!=nDrawOrder)
			{
				goto FORCED;
			}

			return False;
		}
		goto FORCED;
	}

	m_prevUpdateOrder=m_glsDrawInterface->m_nRequestUpdateOrder;
	return False;

FORCED:
	
	w=m_w;
	h=m_h;
	nRad=__INTEGER__(m_fBlureRad);

	if (m_hShadow==0)
	{
		m_hShadow=cyiShape::CreateShadow((int)m_fShadowWidth, m_fEdge, 0.7, 0xFF000000);
	}

	nShadowWidth = cyiShape::GetShadowSize(m_hShadow);
	nClientWidth=w-(nShadowWidth<<1);
	nClientHeight=h-(nShadowWidth<<1);

	RECT rectBlure = {nShadowWidth+1, nShadowWidth+1, nClientWidth-2, nClientHeight-2};
	if (m_bmpBehind==0 || bIsSizeChanged==True)
	{
		if (m_bmpBehind)
			delete m_bmpBehind;

		m_bmpBehind=BITMAP32(m_w, m_h);
	}



	cyiBitmapBlender::BitmapCopy(m_bmpBehind, bmp, childRect->left, childRect->top, rect->left, rect->top, rect->right, rect->bottom);

	if (m_bmpFrame==0 || bIsSizeChanged==True)
	{
		if (m_bmpFrame)
			delete m_bmpFrame;

		m_bmpFrame = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black),&LinearGradientBrush(Point(0, 0), Point(0, nClientHeight), m_colorTop, m_colorBottom), RectF(0,0, nClientWidth, nClientHeight), m_fEdge);
	}

	if (m_bmpShadow==0 || bIsSizeChanged==True)
	{
		if (m_bmpShadow)
			delete m_bmpShadow;
		m_bmpShadow=cyiShape::MakeShadowBitmap(m_hShadow, w, h);
	}

	//if (m_parent->GetEffectedBg(m_bmpBehind, 0, 0, m_x, m_y, m_w, m_h)==True)
	//{
	//	goto SKIP_BLURE;
	//}

	Bitmap*xBlured=0;
	RECT rectBlureDrawArea=rectBlure;//{0,0,0,0};
	RECT rectBmp;
	if (xIsInRect(childRect, &rectBlure, rectDrawBlure)==True)
	{
		int nRadTwo=(nRad<<1);
		RECT rectRequiredBlure = {rectDrawBlure.left-nRadTwo, rectDrawBlure.top-nRadTwo, rectDrawBlure.right+(nRadTwo<<1),rectDrawBlure.bottom+(nRadTwo<<1)};
		RECT rectAll = {0, 0, m_w, m_h};


		xIsInRect(&rectAll ,&rectRequiredBlure, rectBmp);

		Bitmap*x = cyiBitmapBlender::BitmapCopy(0, m_bmpBehind, 0, 0, rectBmp.left, rectBmp.top, 
			rectBmp.right, rectBmp.bottom);//rect->right, rect->bottom);

		//Bitmap*x = cyiBitmapBlender::BitmapCopy(0, m_bmpBehind, 0, 0, rectDrawBlure.left-nRad, rectDrawBlure.top-nRad, 
		//	rectDrawBlure.right+(nRad<<1), rectDrawBlure.bottom+(nRad<<1));//rect->right, rect->bottom);

		xBlured=cyiBitmapBlender::GaussBlureEx(x, m_fBlureRad, 0);//::GaussBlureXX(x, m_fBlureRad, 0,0);
		delete x;
		//xBlured=x;

		rectAll=*childRect;
		rectAll.left-=nRad;
		rectAll.top-=nRad;
		rectAll.right+=nRadTwo;
		rectAll.bottom+=nRadTwo;

		xIsInRect(&rectAll, &rectBlure, rectDrawBlure);
		xIsInRect(&rectDrawBlure, &rectBmp, rectBlureDrawArea);

		bIsClientAreaUpdated=True;
	}


	Bitmap bmpTemp(m_w, m_h, PixelFormat32bppARGB);
	Graphics g(&bmpTemp);

	
	RECT rectDraw;

	if (bIsClientAreaUpdated)
	{
		yiUtilGetAddRect2(&rectDraw, childRect, &rectBlureDrawArea);
		
		cyiBitmapBlender::BitmapCopy(&bmpTemp, m_bmpBehind, rectDraw.left, rectDraw.top, 
							rectDraw.left, rectDraw.top, rectDraw.right, rectDraw.bottom);
		g.SetClip(Rect(rectDraw.left, rectDraw.top, rectDraw.right, rectDraw.bottom));
		g.DrawImage(m_bmpShadow, 0, 0, 0, 0, w, h, UnitPixel);

		cyiBitmapBlender::BitmapCopy(&bmpTemp, xBlured, rectBlureDrawArea.left, rectBlureDrawArea.top, 
										rectBlureDrawArea.left-rectBmp.left, rectBlureDrawArea.top-rectBmp.top, rectBlureDrawArea.right, rectBlureDrawArea.bottom);
	}
	else
	{
		rectDraw.left=0;
		rectDraw.top=0;
		rectDraw.right=m_w;
		rectDraw.bottom=m_h;


		g.SetClip(Rect(childRect->left, childRect->top, childRect->right, childRect->bottom));
		g.DrawImage(m_bmpShadow, 0, 0, 0, 0, w, h, UnitPixel);
		cyiBitmapBlender::BitmapCopy(&bmpTemp, m_bmpBluredBg, rectBlure.left, rectBlure.top, 
							rectBlure.left, rectBlure.top, rectBlure.right, rectBlure.bottom);
	}

	g.DrawImage(m_bmpFrame, nShadowWidth, nShadowWidth);
	//g.DrawRectangle(&Pen(Color::Black), rectBlureDrawArea.left, rectBlureDrawArea.top, rectBlureDrawArea.right, rectBlureDrawArea.bottom);



	Graphics gx(bmp);

	if (bIsClientAreaUpdated==0)
	{	
		gx.SetClip(Rect(rect->left, rect->top, rect->right, rect->bottom));
	}
	gx.DrawImage(&bmpTemp, rectDraw.left+m_nAbsX, rectDraw.top+m_nAbsY, rectDraw.left, rectDraw.top, rectDraw.right, rectDraw.bottom, UnitPixel);
	//gx.DrawRectangle(&Pen(Color::Black), rectDraw.left+m_nAbsX, rectDraw.top+m_nAbsY, rectDraw.right-1, rectDraw.bottom-1);
	
	if (xBlured)
		delete xBlured;

	rectDraw.left+=m_nAbsX;
	rectDraw.top+=m_nAbsY;

	if (bIsClientAreaUpdated)
	{
		rect->left=rectDraw.left;
		rect->top=rectDraw.top;
		rect->right=rectDraw.right;
		rect->bottom=rectDraw.bottom;

		//m_glsDrawInterface->SetChangedDrawArea(&rectDraw);
	}

	if (m_bmpBluredBg==0|| bIsSizeChanged==True)
	{
		if (m_bmpBluredBg)
			delete m_bmpBluredBg;

		m_bmpBluredBg=BITMAP32(m_w, m_h);
	}

	if (bIsClientAreaUpdated==True)
	{
		cyiBitmapBlender::BitmapCopy(m_bmpBluredBg, bmp, rectDraw.left-m_nAbsX, rectDraw.top-m_nAbsY, rectDraw.left, rectDraw.top, rectDraw.right, rectDraw.bottom);
	}

	if (m_item.fTransparency==0.0)
	{
		cyiBitmapBlender::BitmapCopy(bmp, m_bmpBehind, rect->left, rect->top, childRect->left, childRect->top, rect->right, rect->bottom);
		//cyiBitmapBlender::FillColor(bmp, Color::Red, &Rect(rect->left, rect->top, rect->right, rect->bottom));
	}
	m_prevUpdateOrder=nDrawOrder;

	return False;
}

// -----------------------------
void cglsGaussPannel::Update(Bitmap* bmp, RECT* rect, RECT* childRect,int offsetX, int offsetY, Bool bIsLocalBufferUpdate)
{
	RECT rectAll=*rect;
	__super::Update(bmp, rect, childRect, offsetX, offsetY, bIsLocalBufferUpdate);
	*rect=rectAll;
}