#include "glsToolButton.h"

cglsToolButton::cglsToolButton(void)
{
}

cglsToolButton::~cglsToolButton(void)
{
}

// --------------------------------------------------
void cglsToolButton::xDefaultDetail(sGlsToolButtonDetail* detail)
{
	cglsStackPannel::xDefaultDetail(&detail->stack);
	detail->fScale=0.7;
	detail->fShadowWidth=0.0;
}

// --------------------------------------------------
Bool cglsToolButton::Create(cGlassInterface* parent, int id, int x, int y, int w, int h, sGlsToolButtonDetail* detail)
{
	sGlsToolButtonDetail xdetail;
	RECT rect={x, y, w, h};

	Bool m_bIsVertical = xIS_GLS_FLAG(m_eStackFlag, eSPF_BUFFERSIZE_FIXED_VIRTUAL);

	if (detail)
		xdetail=*detail;
	else
		xDefaultDetail(&xdetail);

	m_fScale=xdetail.fScale;
	
	int nFontHeight;
	
	if (xdetail.stack.gls.font==0)
	{
		nFontHeight = parent->GetFont()->GetHeight(__DEFAULTGRAPHICS__);
	}
	else
	{
		nFontHeight = xdetail.stack.gls.font->GetHeight(__DEFAULTGRAPHICS__);
	}
	
	//int margin=nFontHeight>>1;
	int edge=xdetail.stack.gauss.fEdge;
	m_fEdge=xdetail.stack.gauss.fEdge;
	m_fShadowWidth=xdetail.fShadowWidth;
	xdetail.stack.bIsFixedMode=h!=0?True:False;
	xdetail.stack.bIsInsertAndCreate=!xdetail.stack.bIsFixedMode;
	int margin=m_fShadowWidth+edge;
	RECT rectMargin={margin, margin, margin, margin};
	
	if (margin==0)
	{
		if (m_bIsVertical)
		{
			rectMargin.left=1;
			rectMargin.right=0;
		}
		else
		{
			rectMargin.top=1;
			rectMargin.bottom=1;
		}
	}

	return cglsStackPannel::Create((HWND)parent, id, &rect, &rectMargin, this, &xdetail.stack);
}

// --------------------------------------------------
Bool cglsToolButton::Add(eGlsToolButtonType type, eGlsToolButtonShapeStyle style, cglsDrawObject* obj)
{
	//int distance = m_listItem->count()?0:(int)m_fShadowWidth;
	sGlsItemDetail detail;
	cglsStackPannel::xDefaultItemDetail(&detail);
	detail.eAlign=eSA_CENTER;
	return cglsStackPannel::Add(type, 0, 0, 0);//distance);
}

// --------------------------------------------------
static Bool xGetBitmap(sDrawItem* item, cglsResourceObject* resource, int id, int h)
{

	for (int i=0; i<3; i++)
	{
		item->pBmp[i]=resource->GetBitmap(id, i, h, h);
	}

	if (item->pBmp[0]==0)
	{
		return False;
	}
	item->pCurrentBmp=item->pBmp[0];
	item->rect.w=item->pCurrentBmp->GetWidth();
	item->rect.h=item->pCurrentBmp->GetHeight();
	return True;
}
#include "glsButton.h"
#include "glsResourceBitmap.h"
// --------------------------------------------------
Bool cglsToolButton::CreateBitmap(sDrawItem* item, void* pExtra)
{
	cglsResourceBitmap* resource=(cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
	int nFontHeight = GetFont()->GetHeight(__DEFAULTGRAPHICS__);
	// BG
	if (item->id==0)
	{
		int w=item->rect.w, h=item->rect.h;

		color_t colorTop =__COLORp__(96, 0, 0, 0);
		color_t colorBottom=__COLORp__(200, 0, 0, 0);

		sGlsButtonDetail xdetail;
		cglsButton::xDefaultDetail(&xdetail);
		xdetail.edge=m_fEdge;
		xdetail.rect.right=w;
		xdetail.rect.bottom=h;
		xdetail.colorHilight=Color::LightBlue;
		Bitmap* bmpFrame=resource->Draw(eGDID_GLASS_TOOL_PANNEL, 0, w, h, 0, 0, &xdetail);//xDrawGlassBtnPannel();
		m_item.pBmp[2]=m_item.pBmp[1]=resource->Draw(eGDID_GLASS_TOOL_PANNEL, 1, w, h, 0, 0, &xdetail);
		m_item.pBmp[0]=bmpFrame;//bmpShadow;
		m_item.pCurrentBmp=bmpFrame;

		return True;
	}



	if (xIS_GLS_FLAG(m_eStackFlag, eSPF_FIXED_MODE)==True)
	{
		nFontHeight=m_h/m_listItem->count();
	}
	else
	{
		nFontHeight=(int)((float)nFontHeight*m_fScale);
	}

	int id;
	switch(item->id)
	{
		case eGTBT_ALEFT:
			id=eGRID_TOOL_ALEFT;
			break;
		case eGTBT_ARIGHT:
			id=eGRID_TOOL_ARIGHT;
			break;
		case eGTBT_AUP:
			id=eGRID_TOOL_AUP;
			break;
		case eGTBT_ADOWN:
			id=eGRID_TOOL_ADOWN;
			break;
		case eGTBT_PLUS:
			id=eGRID_TOOL_PLUS;
			break;
		case eGTBT_MINUS:
			id=eGRID_TOOL_MINUS;
			break;
		case eGTBT_CROSS:
			id=eGRID_TOOL_CROSS;
			break;
		case eGTBT_CHAR:
			id=eGRID_TOOL_CHAR;
			break;
		case eGTBT_USER:
			id=eGRID_TOOL_CHAR;
			break;
		default:
			return False;
	}

	return xGetBitmap(item, resource, id, nFontHeight);
}

// ------------------------------------
Bool cglsToolButton::OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX, int offsetY)
{
	return False;
}

// ---------------------------------------
LRESULT cglsToolButton::OnClicked(sDrawItem* item)
{
	short id = m_id;
	return SendMessage(this->m_hParentWnd, WM_COMMAND, ((0xFFFF&id) | (item->id << 16)), 0);
}

// ------------------------------------
LRESULT cglsToolButton::OnLButtonRepeated(WPARAM wp, LPARAM lp)
{
	sDrawItem* item = xFindItem(wp, lp, 0);

	if (item==0)
	{
		return 0;
	}
	short id = m_id;
	return SendMessage(this->m_hParentWnd, WM_COMMAND, ((0xFFFF&id) | ((item->id|0x8000) << 16)), 0);
}