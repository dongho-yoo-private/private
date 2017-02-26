#include "glsScrollBar.h"
#include "glsResourceBitmap.h"

cglsScrollBar::cglsScrollBar(void)
{
	__GLASSINTERFACE_NAME__;
}

cglsScrollBar::~cglsScrollBar(void)
{
}


// ---------------------------
void cglsScrollBar::xDefaultDetail(sGlsScrollBarDetail* detail)
{
	detail->bIsVertical=True;
	detail->fTransparency=0.0;
	detail->nBarMinSize=0;
}

// ---------------------------
Bool cglsScrollBar::Create(cGlassInterface* parent, sGlsScrollBarDetail* detail)//HWND hParentWnd, int id, int x, int y, int w, int h, sGlsScrollBarDetail* detail)
{
	sGlsScrollBarDetail sDetail;
	sGlsStackPannelDetail xdetail;
	cglsStackPannel::xDefaultDetail(&xdetail);

	if (detail)
		sDetail=*detail;
	else
		xDefaultDetail(&sDetail);

	xdetail.bIsFixedMode=True;
	xdetail.bIsVertical=sDetail.bIsVertical;
	xdetail.fEnterTransparency=1.0;//sDetail.fEnterTransparency;
	xdetail.fLeaveTransparency=1.0;//sDetail.fLeaveTransparency;
	xdetail.bIsShowScrollBar=True;
	xdetail.fScrollBarEnterTransparency=0.8;
	xdetail.fScrollBarLeaveTransparency=0.0;
	xdetail.gls.nShowAnimationInterval=1000;
	//xdetail.gls.nMouseLeaveAnimationInterval=1000;

	int nFontHeight = parent->GetFont()->GetHeight(__DEFAULTGRAPHICS__);

	RECT rect = {parent->m_w-nFontHeight, 0, nFontHeight, parent->m_h};
	//RECT rect = {0, 0, nFontHeight, parent->m_h};
	if (cglsStackPannel::Create((HWND)parent, -128, &rect, 0, 0, &xdetail)==False)
	{
		return False;
	}

	if (m_bmpCurrent==0)
		m_bmpCurrent=BITMAP32(m_w, m_h);
	
	//m_item.pCurrentBmp=m_bmpCurrent;

	m_item.rect.x=0;
	m_item.rect.y=0;
	m_item.rect.w=m_w;
	m_item.rect.h=m_h;
	//m_parent->AddPositionLink(this, (eGlsPosition)(eGP_RIGHTSIDE|eGP_TOP), -nFontHeight, 0);
	Show();

	return True;
}

// ---------------------------------------
Bool cglsScrollBar::SetRange(int min, int max, int pagesize)
{
	return SetScrollRange(min, max, pagesize);//, True);
}

// ---------------------------------------
int cglsScrollBar::GetPos()
{
	return m_scrollInfo.pos;
	//return (int)((float)m_scrollInfo.max*(float)m_scrollInfo.pos/(float)m_scrollInfo.m_nScrollBarRange);
	//m_scrollInfo.pos=pos;

	//return m_scrollInfo.pos;
}

// ---------------------------------------
void cglsScrollBar::SetPos(int n)
{
	SetScrollPos(n);
}
