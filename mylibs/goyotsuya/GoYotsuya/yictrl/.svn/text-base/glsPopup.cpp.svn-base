#include "glsPopup.h"

cglsPopup::cglsPopup(void)
{
}

cglsPopup::~cglsPopup(void)
{
}

// -----------------------
void cglsPopup::xDefaultDetail(sGlsPopupDetail* detail)
{
	cglsGaussPannel::xDefaultDetail(&detail->gauss);
	cGlassInterface::xDefaultDetail(&detail->gls);
	detail->colorFont=(color_t)Color::Black;
	detail->margin.left=2;
	detail->margin.top=2;
	detail->margin.right=2;
	detail->margin.bottom=2;
}

// -----------------------
Bool cglsPopup::Create(HWND hWnd, int x, int y, int id, Bitmap* icon, const wchar_t* text, sGlsPopupDetail* detail)
{
	sGlsPopupDetail xdetail;

	if (detail)
	{
		xdetail=*detail;
	}
	else
	{
		xDefaultDetail(&xdetail);
	}

	m_colorFont=xdetail.colorFont;
	m_rectMargin=xdetail.margin;
	m_rectMargin.left+=(int)xdetail.gauss.fShadowWidth;
	m_rectMargin.top+=(int)xdetail.gauss.fShadowWidth;
	m_rectMargin.right+=(int)xdetail.gauss.fShadowWidth;
	m_rectMargin.bottom+=(int)xdetail.gauss.fShadowWidth;

	//m_bIsAlwaysDraw=True;
	m_DrawObject=this;

	memset(&m_item, 0, sizeof(m_item));

	if (xdetail.gls.font)
		m_font=xdetail.gls.font;
	
	m_item.id=id;
	m_item.bitmapId=(IntPtr)icon;
	m_item.txt=(wchar_t*)text;
	m_DrawObject->CreateBitmap(&m_item);

	m_font=0;

	RECT rect = {x, y, m_item.rect.w, m_item.rect.h};
	if (cGlassInterface::Create(hWnd, 0, id, &rect, &xdetail.gls)==True)
	{
		xSET_GLS_FLAG(m_flag, eGF_UPDATE_INDEPENDENT, True);
		return True;
	}

	return False;
}

// -----------------------
//Bool cglsPopup::Create(HWND hWnd, int x, int y, int id, cGlassInterface* obj, sGlsPopupDetail* detail)
//{
//	sGlsPopupDetail xdetail;
//
//	if (detail)
//	{
//		xdetail=*detail;
//	}
//	else
//	{
//		xDefaultDetail(&xdetail);
//	}
//
//	m_DrawObject=this;
//
//	memset(&m_item, 0, sizeof(m_item));
//	m_item.id=id;
//	m_item.bitmapId=(int)icon;
//	m_item.txt=text;
//	m_DrawObject->CreateBitmap(&m_item);
//
//	RECT rect = {x, y, m_item.rect.w, m_item.rect.h};
//	if (cGlassInterface::Create(hWnd, 0, id, &rect, &detail->gls)==True)
//	{
//		return True;
//	}
//
//	return True;
//}

// -----------------------
void cglsPopup::Destroy()
{
	cGlassInterface::Destroy();
	return ;
}


// -----------------------
Bool cglsPopup::Show(int nAnimationTime)
{
	//return False;
	if (IsWindowVisible(m_hWnd)==TRUE)
	{
		return False;
	}

	nAnimationTime=0;
	ZOrder(eZO_TOP);
	//m_glsDrawInterface->m_nRequestUpdateCount++;
	if (nAnimationTime==0)
	{
		cGlassInterface::Show();
		//cyctrlbase::Show();
		return InvalidateRect(0);
	}


	cyctrlbase::Show();
	//cGlassInterface::AnimationShow(True, 0.0, 1.0);

	return True;
}

// -----------------------
Bool cglsPopup::Hide(int nAnimationTime)
{
	//return False;

	if (IsWindowVisible(m_hWnd)==FALSE)
	{
		return False;
	}

	nAnimationTime=0;
	if (nAnimationTime==0)
	{
		cGlassInterface::Hide();
		return InvalidateRect(0);
	}

	//cGlassInterface::AnimationShow(False, 1.0, 0.0);
	return True;
}


#include "glsResourceBitmap.h"

int cglsPopup::GetDrawOrder()
{
	return 1025;
}

// --------------------------
Bool cglsPopup::CreateBitmap(sDrawItem* item, void* pExtra)
{
	//m_bIsDefaultDraw=True;


	if (item->id==0)
	{
		return False;
	}

	cglsResourceBitmap* resource = (cglsResourceBitmap*)m_xresource;
	
	if (resource==0)
	{
		resource=(cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
	}


	Bitmap* bmp=0;
	Bitmap* bmpStr=0;


	//if (m_font==0)
	//{
	//	if (m_xfont==0)
	//	{
	//		m_xfont=m_font;

	//		if (m_xfont==0)
	//		{
	//			//m_xfont=m_parent->GetFont();
	//		}
	//	}

	//	// デフォルト
	//	if (m_xfont==0)
	//	{
	//		m_xfont=new Font(L"メイリオ", 11.0);
	//	}
	//}
	//else
	//{
	//	m_xfont=m_font;
	//}

	m_xfont=GetFont();
	int nFontHeight = cglsResourceObject::GetFontHeight(m_xfont);
	int w=0, h=0;


	if (item->txt)
	{
		bmpStr = cyiShape::BitmapString(0, item->txt, m_xfont, &SolidBrush(m_colorFont));

		w+=bmpStr->GetWidth();
		h=bmpStr->GetHeight();
	}

	if (item->bitmapId)
	{
		if ((item->bitmapId&0xFFFF0000)==0)
			bmp = resource->GetBitmap(item->bitmapId, 0, 0, ((float)nFontHeight*0.7));
		else
			bmp=(Bitmap*)item->bitmapId;

		w+=bmp->GetWidth();
		h= (h>bmp->GetHeight())?h:bmp->GetHeight();
	}
	
	Bitmap* ret = BITMAP32(w+m_rectMargin.left+m_rectMargin.right, h+m_rectMargin.top+m_rectMargin.bottom);
	Graphics g(ret);

	if (bmp)
	{
		g.DrawImage(bmp, m_rectMargin.left, ((ret->GetHeight()-bmp->GetHeight())>>1));
	}

	if (bmpStr)
	{
		int x = bmp ? bmp->GetWidth():m_rectMargin.left;
		g.DrawImage(bmpStr, x, ((ret->GetHeight()-bmpStr->GetHeight())>>1));
	}

	item->pCurrentBmp=item->pBmp[0]=ret;

	item->rect.x=0;
	item->rect.y=0;
	item->rect.w=item->pCurrentBmp->GetWidth();
	item->rect.h=item->pCurrentBmp->GetHeight();

	return True;
}

