#include "glsButton.h"

cglsButton::cglsButton(void)
:cGlassInterface()
{
	xDefaultDetail(&m_detail);
	__GLASSINTERFACE_NAME__;
}

cglsButton::~cglsButton(void)
{
}

// --------------------------------
void cglsButton::xDefaultDetail(sGlsButtonDetail* detail)
{
	cGlassInterface::xDefaultDetail(&detail->gls);
	detail->colorHilight=0;//__COLORp__(0, 127, 255, 127);//255, 127);
	detail->edge=2.0;
	detail->bIsMouseDownNotify=False;
	detail->gls.bFocusStop=True;
	detail->bIsOnOff=False;
	detail->bHasBorder=True;
	memset(&detail->rect, 0, sizeof(RECT));
}

// ------------------------------------
Bool cglsButton::Create(HWND hWndParent, int id, int x, int y, int w, int h, const char* pszCaption)
{
    m_hWnd = CreateWindow(
    "BUTTON", pszCaption, 
    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	x, y, w, h, hWndParent, (HMENU)id, ::GetModuleHandleA(0),NULL);

	return m_hWnd ? True:False;

}


// ------------------------------------
Bool cglsButton::Create(HWND hParent, int id, int x, int y, Bitmap* bmp, const wchar_t* pszText, const wchar_t* pszToolTip, sGlsButtonDetail* detail, cglsDrawObject* obj/*=0*/)
{
	m_DrawObject=obj;

	m_item.id=id;

	if (!m_DrawObject)
	{
		m_DrawObject=this;
	}

	if (detail)
		m_detail=*detail;

	if (m_detail.gls.style==eGGS_NEON)
	{
		if (m_detail.colorHilight==0)
			m_detail.colorHilight=__COLORp__(0, 255, 150, 127);
	}
	else
	{
		if (m_detail.colorHilight==0)
			m_detail.colorHilight=__COLORp__(0, 127, 255, 127);
	}

	if (m_detail.bIsOnOff==True)
	{
		m_detail.gls.bMouseDownCommand=True;
	}

	xSET_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW, m_detail.gls.bIsNotOwnDraw);

	cGlassInterface::InitResource();

	if (cGlassInterface::Create(hParent, 0, id, 0, &m_detail.gls)==False)
	{
		return False;
	}

	if (((IntPtr)bmp)&0xFFFF0000==0)
	{
		m_item.bitmapId=(IntPtr)bmp;
	}
	else
	{
		m_item.bitmapId=(IntPtr)bmp;
	}
	
	if (pszText)
		m_item.txt=yistrheapW(pszText);

	if (m_DrawObject==0)
	{
		m_DrawObject=this;
	}

	Move(x, y);

	//m_DrawObject->CreateBitmap(&m_item);

	//ReSize(m_item.rect.w, m_item.rect.h);

	if (pszToolTip)
		CreateToolTip(pszToolTip, 500, 0);

	Show();
	InvalidateRect(0);

	return True;
}

// ------------------------------------
Bool cglsButton::SetItemChange(Bitmap* bmp, wchar_t* pszText)
{
	Bool bIsChanged=False;
	if ((IntPtr)bmp!=-1)
	{
		if (((IntPtr)bmp)&0xFFFF0000==0)
		{
			m_item.bitmapId=(IntPtr)bmp;
		}
		else
		{
			m_item.bitmapId=(IntPtr)bmp;
		}
		bIsChanged=True;
	}

	if ((int)pszText!=-1)
	{
		if (pszText)
		{
			if (m_item.txt)
				yifree(m_item.txt);
			m_item.txt=yistrheapW(pszText);
		}
		bIsChanged=True;
	}

	if (bIsChanged)
	{
		Update(eGUM_FORCED);
		InvalidateRect(NULL);
	}


	return bIsChanged;
}

#include "glsResourceBitmap.h"

//// -----------------------------
//void cglsButton::OnUpdateBufferBefore(Graphics*g, Bitmap* bmpBuffer, int x, int y, int w, int h)
//{
//	if (m_bIsDefaultDraw==True)
//	{
//		//Bitmap* bmp = cyiBitmapBlender::BitmapCopy(0, bmpBuffer, 0, 0, x, y, w, h);
//
//		//cyiShapeMask mask(Rect(2, 2, w-4, h-4));
//		//m_bg =cyiBitmapBlender::GaussBlureXX(bmp, 4.0, 0,  &mask);
//		//delete bmp;
//		//
//		//g->DrawImage(m_bg, x, y, 0, 0, w, h, Unit::UnitPixel);
//	}
//}

// --------------------------------
IntPtr cglsButton::GetMeasureHeight(Font* font)
{
	cglsResourceBitmap* resource = (cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
	int nFontHeight = cglsResourceObject::GetFontHeight(font);

	return (IntPtr)resource->Draw(eGDID_GLASS_BTN_PANNEL, 0, 0, nFontHeight, 0, 0, (void*)0xFFFFFFFF)+1;
}

// --------------------------
int cglsButton::GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation)
{
	if (m_detail.bIsOnOff==False)
		return __super::GetStatusToBitmapIndex(item, status, bIsNoAnimation);

	if (item->nLastItemIndex!=2)
	{
		return __super::GetStatusToBitmapIndex(item, status, bIsNoAnimation);
	}

	if (status==eDIS_MOUSE_DOWN)
	{
		SetItemStatus(item, eDIS_NORMAL);
		return 0;
	}
	
	return -1;
}

// ------------------------------------
Bool cglsButton::IsOn()
{
	return (m_item.nLastItemIndex==2);
}

// ------------------------------------
Bool cglsButton::SetOnOff(Bool bIsOnOff)
{
	m_item.nLastItemIndex=2;
	InvalidateRect(0);
	return True;
}


// --------------------------
Bool cglsButton::CreateBitmap(sDrawItem* item, void* pExtra)
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
	color_t color=Color::Black;


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

	if (m_guiStyle==::eGGS_NEON)
	{
		color=Color::White;
	}

	if (item->txt)
	{
		bmpStr = cyiShape::BitmapString(0, item->txt, m_xfont, &SolidBrush(color));//, eSTS_GROW, Color::White, 5.0, 3.0);
		//bmpStr = cyiShape::BitmapString(0, item->txt, m_xfont, &SolidBrush(Color::Black), eSTS_FIXED, 0, 40);
		//nFontHeight = bmpStr->GetHeight() > nFontHeight ? bmpStr->GetHeight():nFontHeight;
	}

	if (item->bitmapId)
	{
		if ((item->bitmapId&0xFFFF0000)==0)
			bmp = resource->GetBitmap(item->bitmapId, 0, 0, ((float)nFontHeight*0.7), color);
		else
			bmp=(Bitmap*)item->bitmapId;
	}
	
	if (m_detail.bIsOnOff==False)
	{
		for (int i=0; i<3; i++)
		{
			if (item->pBmp[i])
			{
				delete item->pBmp[i];
			}
		}
	}
	else
	{
		for (int i=0; i<2; i++)
		{
			if (item->pBmp[i])
			{
				delete item->pBmp[i];
			}
		}
	}

	if (m_guiStyle==eGGS_NEON)
	{
		item->pBmp[0] = resource->Draw(eGDID_NEON_BTN_PANNEL, 0, 0, nFontHeight, bmp, bmpStr, &m_detail);

		// 1. ネオンサインのベース作成
		Bitmap* neonBase = cyiBitmapBlender::NeonSign(item->pBmp[0], m_detail.colorHilight, True);
		
		// 1->2->pBmp[0] の順で描画
		// 枠を作成。
		Graphics g(neonBase);

		if (m_detail.bHasBorder==True)
		{
			g.DrawRectangle(&Pen(Color::White), 0, 0, neonBase->GetWidth()-1, neonBase->GetHeight()-1);
		}

		item->pBmp[1]=neonBase;


		if (m_detail.bIsOnOff==True)
		{
			item->pBmp[2]=item->pBmp[1];
		}
		else
		{
			neonBase = cyiBitmapBlender::NeonSign(item->pBmp[0], m_detail.colorHilight, False);
			Bitmap* neonBaseGray = cyiBitmapBlender::ArgbShift(item->pBmp[0], 0, -32, -32, -32);
			Graphics g2(neonBaseGray);
			if (m_detail.bHasBorder==True)
			{
				g2.DrawRectangle(&Pen(Color::LightGray), 0, 0, neonBase->GetWidth()-1, neonBase->GetHeight()-1);
			}
			item->pBmp[2]=neonBaseGray;
			delete neonBase;
		}
		
		//item->pBmp[1] = resource->Draw(eGDID_NEON_BTN_PANNEL, 1, 0, nFontHeight, bmp, bmpStr, &m_detail);
		//item->pBmp[2] = resource->Draw(eGDID_NEON_BTN_PANNEL, 2, 0, nFontHeight, bmp, bmpStr, &m_detail);
	}
	else
	{
		item->pBmp[0] = resource->Draw(eGDID_GLASS_BTN_PANNEL, 0, 0, nFontHeight, bmp, bmpStr, &m_detail);
		item->pBmp[1] = resource->Draw(eGDID_GLASS_BTN_PANNEL, 1, 0, nFontHeight, bmp, bmpStr, &m_detail);
		if (m_detail.bIsOnOff==True)
		{
			item->pBmp[2]=item->pBmp[1];
		}
		else
		{
			item->pBmp[2] = resource->Draw(eGDID_GLASS_BTN_PANNEL, 2, 0, nFontHeight, bmp, bmpStr, &m_detail);
		}
	}

	YIDEBUG("glsButton Height:%d", item->pBmp[0]->GetHeight());

	if (bmpStr)
		delete bmpStr;

	if (bmp && (item->bitmapId&0xFFFF0000)!=0)
	{
		delete bmp;
	}

	item->pCurrentBmp=item->pBmp[0];

	item->rect.x=0;
	item->rect.y=0;
	item->rect.w=item->pCurrentBmp->GetWidth();
	item->rect.h=item->pCurrentBmp->GetHeight();

	return True;
}



