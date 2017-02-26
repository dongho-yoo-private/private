#include "glsRadioGroup.h"

cglsRadioGroup::cglsRadioGroup(void)
{
	m_checkedIndex=0;
	m_bIsTypeCheck=False;
}

cglsRadioGroup::~cglsRadioGroup(void)
{
}

// -----------------------------
void cglsRadioGroup::xDefaultDetail(sRadioGroupDetail* detail)
{
	detail->bIsAutoArrange=True;
	detail->colorRadio=Color::LightBlue;
	detail->colorRadioBorder=Color::Black;
	detail->colorText=Color::Black;
	detail->nDistance=3;
	cGlassInterface::xDefaultDetail(&detail->gls);
}

// -----------------------------
Bool cglsRadioGroup::Create(cGlassInterface* parent, int id, int x, int y, sRadioGroupDetail* detail)
{
	sRadioGroupDetail xdetail;
	sGlsStackPannelDetail stackDetail;

	if (detail)
		xdetail=*detail;
	else
		xDefaultDetail(&xdetail);

	m_colorRadio=xdetail.colorRadio;
	m_colorRadioBorder=xdetail.colorRadioBorder;
	m_colorText=xdetail.colorText;
	m_nDistance=xdetail.nDistance;

	stackDetail.bIsFixedMode=False;

	RECT xrect ={x, y, 0, 0};
	cglsStackPannel::xDefaultDetail(&stackDetail);
	//stackDetail.gls=xdetail.gls;
	stackDetail.gls=xdetail.gls;

	return cglsStackPannel::Create((HWND)parent, id, &xrect, 0, 0, &stackDetail);
}

// -----------------------------
Bool cglsRadioGroup::AddItem(int id, const wchar_t* pszText)
{
	xglsRadioButton* btn = new xglsRadioButton();
	sGlsDetail detail;
	cGlassInterface::xDefaultDetail(&detail);
	detail.style=m_guiStyle;
	
	btn->m_bIsTypeCheck=m_bIsTypeCheck;
	if (btn->Create(this, id, 0, 0, pszText, m_colorRadio, m_colorText, m_colorRadioBorder, &detail)==False)
	{
		delete btn;
		return False;
	}

	return cglsStackPannel::Add(btn, m_nDistance, eSA_CENTER)>0?True:False;
}

// -----------------------------
void cglsRadioGroup::SetChecked(int index)
{
	int n=0;
	int id=0;
	for (cyiNode* node=m_listItem->GetNodeTopFrom(0); node; node=node->np, n++)
	{
		sDrawItem* s = (sDrawItem*)node->ref;

		if (s && s->gls)
		{
			xglsRadioButton* btn = (xglsRadioButton*)s->gls;

			if (n==index)
			{
				btn->SetCheck(True);
				id=btn->m_id;
				m_checkedIndex=index;
				continue;
			}

			if (btn->GetCheck()==True)
			{
				btn->SetCheck(False);
			}
		}
	}

	SendMessage(m_hNotifyWnd, WM_COMMAND, (m_id|(id<<16)), (LPARAM)this);
}

// -----------------------------
LRESULT cglsRadioGroup::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	SetCheckedById(wParam);
	bIsHandled=True;
	return 0;
}


// -----------------------------
void cglsRadioGroup::SetCheckedById(int id)
{
	SetChecked(GetIndex(id));
}

// -----------------------------
int cglsRadioGroup::GetChecked()
{
	return m_checkedIndex;
}



// -----------------------------
xglsRadioButton::xglsRadioButton()
:m_bIsChecked(False)
,m_bmpText(0)
,m_bmpCover(0)
,m_bg(0)
,m_nRadioSize(0)
,m_bIsTypeCheck(False)
{
}

// -----------------------------
xglsRadioButton::~xglsRadioButton()
{
}

#define RADIO_FACTOR 0.6
// -----------------------------
Bool xglsRadioButton::Create(cGlassInterface* parent, int id, int x, int y, const wchar_t* text, color_t colorRadio, color_t colorText, color_t colorRadioBorder, sGlsDetail* detail)
{
	sGlsDetail xdetail;

	if (detail)
		xdetail=*detail;
	else
		cGlassInterface::xDefaultDetail(&xdetail);

	RECT rect={x, y, 0, 0};

	m_szText[0]=0;

	if (text)
		wcsncpy(m_szText, text, 256);

	//xdetail.bIsNoMouseMove=True;
	//xdetail.bIsNoInput=True;
	xdetail.bMouseDownCommand=True;

	Bool res = cGlassInterface::Create((HWND)parent, 0, id, &rect, &xdetail);

	if (res==False)
		return False;

	Show();

	m_colorRadio=colorRadio;
	m_colorText =colorText;
	m_colorRadioBorder;

	int nFontHeight=GetFontHeight(True);

	unsigned int size = 0;
	if (text)
	{
		size = (unsigned int)cyiShape::BitmapStringEx(0, m_szText, GetFont(), 0, 0);
	}

	m_item.rect.x=0;
	m_item.rect.y=0;
	m_item.rect.w=((size>>16)&0xFFFF)+nFontHeight*RADIO_FACTOR+3+2;//<<1)/3;
	m_item.rect.h= nFontHeight+2;//size&0xFFFF;

	ReSize(m_item.rect.w, m_item.rect.h);
	m_eUpdateReason=eGUR_FORCED;
	InvalidateRect(0);
	return res;
}

#include "glsResourceBitmap.h"

void xglsRadioButton::Update(Bitmap* bmp, RECT* rect, RECT* _childRect, int offsetX, int offsetY, Bool bIsLocalBufferUpdate)
{
	if (GetDrawOrder() > m_glsDrawInterface->m_nRequestUpdateOrder)
			//IsBgChanged()==True)
	{
		int nFontHeight = GetFontHeight(True);

		if (m_item.pBmp[0])
			delete m_item.pBmp[0];
		if (m_item.pBmp[1])
			delete m_item.pBmp[1];
		if (m_item.pBmp[2])
			delete m_item.pBmp[2];

		cglsResourceBitmap* resource = (cglsResourceBitmap* )cGlassInterface::GetDefaultResource();


		if (m_szText[0]==0 &&
				m_guiStyle==eGGS_NEON)
		{
			int n = nFontHeight*RADIO_FACTOR*0.8;
			Bitmap* xbmp = BITMAP32(nFontHeight, nFontHeight);
			Graphics g(xbmp);
			g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
			g.FillEllipse(&SolidBrush(Color::White), (nFontHeight-n)>>1, (nFontHeight-n)>>1, n, n);

			m_item.pBmp[0]=xbmp;//cBB::NeonSign(bmp, this->m_colorRadio, True);
			m_item.pBmp[1]=cBB::NeonSign(xbmp, this->m_colorRadio, True);
			m_item.pBmp[2]=cBB::NeonSign(xbmp, this->m_colorRadio, False);
			m_item.pCurrentBmp=m_item.pBmp[m_item.nLastItemIndex];
			cGlassInterface::Update(bmp, rect, _childRect, offsetX, offsetY, bIsLocalBufferUpdate);
			return ;
		}

		m_nRadioSize=nFontHeight*RADIO_FACTOR;//<<1)/3;
		int nBitmapId=m_bIsTypeCheck==True?eGRID_CHECK:eGRID_RADIO;

		//nBitmapId=eGRID_RADIO;
		Bitmap* radio  = resource->GetBitmap(nBitmapId, 0, 0, m_nRadioSize);
		Bitmap* radio2 = resource->GetBitmap(nBitmapId, 1, 0, m_nRadioSize);
		unsigned int size = (unsigned int)cyiShape::BitmapStringEx(0, m_szText, GetFont(), 0, 0);

		int nTextWidth= (size>>16)&0xFFFF;
		int nTextHeight= size&0xFFFF;

		//ReSize(nTextWidth+radio->GetWidth()+2+3, nTextHeight+2);

		if (m_bg)
			delete m_bg;

		RECT xrectClient={1+radio->GetWidth()+3, 1, nTextWidth, nTextHeight};
		m_bg=GetParentBg();//0, &xrectClient);

		m_bmpText =cyiBitmapBlender::BitmapCopy(0, m_bg, 0, 0, xrectClient.left, xrectClient.top, xrectClient.right, xrectClient.bottom);

		cyiShape::BitmapStringEx(m_bmpText, m_szText, GetFont(), m_colorText, 0);//, eSTS_DROP_SHADOW);

		m_item.pBmp[0]=cyiBitmapBlender::BitmapCopy(0, m_bg);
		cyiBitmapBlender::BitmapCopy(m_item.pBmp[0], radio, 1, (m_h-radio->GetHeight())>>1, 0, 0);
		cyiBitmapBlender::BitmapCopy(m_item.pBmp[0], m_bmpText, 1+radio->GetWidth()+3, 1);


		m_item.pBmp[2]=cyiBitmapBlender::BitmapCopy(0, m_bg);
		cyiBitmapBlender::BitmapCopy(m_item.pBmp[2], radio2, 1, (m_h-radio->GetHeight())>>1, 0, 0);
		cyiBitmapBlender::BitmapCopy(m_item.pBmp[2], m_bmpText, 1+radio2->GetWidth()+3, 1);


		if (m_bmpCover)
			delete m_bmpCover;

		m_bmpCover=cyiShape::DrawGlassCover(m_w, m_h, 2.0);
		
		if (m_bIsChecked==True)
		{
			m_item.pBmp[1]=cyiBitmapBlender::BitmapCopy(0, m_item.pBmp[2]);
		}
		else
		{
			m_item.pBmp[1]=cyiBitmapBlender::BitmapCopy(0, m_item.pBmp[0]);
		}

		Graphics gx(m_item.pBmp[1]);
		gx.DrawImage(m_bmpCover, 0, 0);		

		
		m_item.pCurrentBmp=m_item.pBmp[m_item.nLastItemIndex];
	}

	cGlassInterface::Update(bmp, rect, _childRect, offsetX, offsetY, bIsLocalBufferUpdate);
}

// -----------------------------
Bool xglsRadioButton::GetCheck()
{
	return m_bIsChecked;
}
	
// -----------------------------
void xglsRadioButton::SetCheck(Bool bIsChecked)
{
	//if (m_bIsChecked==bIsChecked)
	//{
	//	return ;
	//}

	m_bIsChecked=bIsChecked;

	if (m_item.pBmp[1])
		delete m_item.pBmp[1];

	if (m_bIsChecked==True)
	{
		m_item.pBmp[1]=cyiBitmapBlender::BitmapCopy(0, m_item.pBmp[2]);
	}
	else
	{
		m_item.pBmp[1]=cyiBitmapBlender::BitmapCopy(0, m_item.pBmp[0]);
	}

	Graphics gx(m_item.pBmp[1]);
	gx.DrawImage(m_bmpCover, 0, 0);		


	m_item.nLastItemIndex=m_bIsChecked==True?2:0;
	m_item.pCurrentBmp=m_item.pBmp[m_item.nLastItemIndex];
	InvalidateRect(0);
}

// -----------------------------
int xglsRadioButton::GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation)
{
	int res = __super::GetStatusToBitmapIndex(item, status, bIsNoAnimation);


	// ‹°‚ç‚­MouseLeave...
	if (res==0 &&
			bIsNoAnimation==False)
	{
		return m_bIsChecked?2:0;
		//return nLastIndex;
	}

	if (m_bIsTypeCheck==True)
	{
		if (res==2 &&
				bIsNoAnimation==True &&
					m_bIsChecked==False)
		{
			res=0;
		}
	}

	return res;
}