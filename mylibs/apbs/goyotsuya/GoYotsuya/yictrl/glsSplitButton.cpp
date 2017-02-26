#include "glsSplitButton.h"

cglsSplitButton::cglsSplitButton(void)
: m_listSplitBtnItem(0),
m_nSelectedIndex(0),
m_menu(0)
,m_bIsRightDown(False)
,m_eStyle(eSBS_LEFT_FULLDOWN)
,m_bmpRightFullDown(0)
,m_bIsBeforeMenuShow(False)
,m_bIsAutoWidth(False)
{
	__GLASSINTERFACE_NAME__;
}

cglsSplitButton::~cglsSplitButton(void)
{
}

typedef struct 
{
	int id;
	wchar_t* text;
	wchar_t* comment;
	Bitmap* icon;
	Bitmap* pItemBmp;
	Bitmap* pBmp[3];
	Bitmap* pBmpEx[3];
} sSplitButtonItem;

#define xSTRUCTHEAP(a) (a*)yialloci(sizeof(a))

// ---------------------------
void cglsSplitButton::xDefaultDetail(sGlsSplitButtonDetail* detail)
{
	cGlassInterface::xDefaultDetail(&detail->gls);
	detail->colorHilight=Color::LightBlue;
	detail->edge=2.0;
	memset(&detail->rect, 0, sizeof(RECT));
	detail->eStyle=eSBS_LEFT_FULLDOWN;


	//cglsButton::xDefaultDetail((sGlsButtonDetail*)detail);
}

// ---------------------------
Bool cglsSplitButton::Create(HWND hWnd, int id, int x, int y, int w, int h, const wchar_t* pszToolTip, sGlsSplitButtonDetail* detail)
{
	if (detail)
	{
		m_detail=*detail;
	}
	else
	{
		xDefaultDetail(&m_detail);
		//m_detail.bIsMouseDownNotify=True;
	}

	m_DrawObject=this;
	m_eStyle=m_detail.eStyle;
	RECT rect={x, y, w, h};

	m_bIsAutoWidth=True;
	//m_factor=0.0;
	//if (w==0)
	//{
	//	m_bIsAutoWidth=True;
	//	w=10;
	//}
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

	if (m_detail.rect.right!=0 &&
			m_detail.rect.bottom!=0)
	{
		m_bIsAutoWidth=False;
	}


	if (cGlassInterface::Create(hWnd, 0, id, &rect, &m_detail.gls)==True)
	{
		m_factor=(float)w/(float)GetFontHeight(True);

		ReSize(w, h);
		Hide();
		//m_menu = new cglsPopupMenu();
		m_menu = new cglsListMenu(GetFont());
		//m_menu->Create(
		//m_menu->Create(m_hWnd, 0); // デフォルトは-1
		//m_menu->AddItem(0xFFFF, 0, pszComment);
	
		if (pszToolTip)
			CreateToolTip(pszToolTip, 500, 0);

		m_listSplitBtnItem = new cyiList();
		return True;
	}
	
	return False;
}

// ---------------------------
Bool cglsSplitButton::AddItem(int subId, Bitmap* icon, const wchar_t* defultText, const wchar_t* pszComment, Bool bIsDefault)
{
	sSplitButtonItem* item  = xSTRUCTHEAP(sSplitButtonItem);//xCreateItem(0, icon, i, defaultText);
	m_listSplitBtnItem->addref(item);

	item->id=subId;
	item->icon=icon;
	item->text=yistrheapW(defultText);
	item->comment=pszComment==0?0:yistrheapW(pszComment);
	//m_menu->AddItem(subId, 0, item->comment?item->comment:item->text);
	m_menu->AddItem(subId, icon, item->comment?item->comment:item->text);

	if (bIsDefault==True)
	{
		this->m_nSelectedIndex=m_listSplitBtnItem->count()-1;
	}

	return True;
}

// ---------------------------
Bool cglsSplitButton::AddString(wchar_t* text)
{
	int n = m_listSplitBtnItem->count();
	return AddItem(n+1, 0, text, text);
}

#include "glsResourceBitmap.h"



static void xCreateItemBitmap(sSplitButtonItem* item, Font* font, int length=0, color_t color=Color::Black)
{
	if (item->pItemBmp)
	{
		delete item->pItemBmp;
	}

	item->pItemBmp=cGlassInterface::xCreateBitmap(item->icon, item->text, font, length, color);
	
}

// -----------------------------------------------------
LRESULT cglsSplitButton::OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled)
{
	m_menu->Close();

	int cnt =0;
	int nPrviousIndex=m_nSelectedIndex;

	//int id=m_menu->GetResultCode();

	wp=wp&0xFFFF;
	if (wp!=0)
	{
		for (cyiNode* node=m_listSplitBtnItem->GetNodeTopFrom(); node; node=node->np)
		{
			sSplitButtonItem* item = (sSplitButtonItem*)node->ref;
			if (item->id==wp)
			{
				break;
			}
			cnt++;
		}

		//m_nSelectedIndex=cnt;
	}

	//m_bIsRightDown=False;
	
	//if (cnt!=nPrviousIndex)
	{
		SelectIndex(cnt);
		if (m_eStyle!=eSBS_RIGHT_FULLDOWN)
		{
			cglsSystem* system = GetSystem();
			int nSystemId=0;

			if (system)
				nSystemId=system->GetCommandId(this, m_id);

			if (system==0 ||
					nSystemId==0)
			{
				SendMessage(m_hNotifyWnd, WM_COMMAND, MAKELONG(m_id, CBN_SELCHANGE), (LPARAM)m_hWnd);
			}
			else
			{
				int nSystemId = system->GetCommandId(this, m_id);
				int index = GetSelectIndex();
				system->GetCommandCenter()->OnCommand(nSystemId, this, CBN_SELCHANGE, index);

			}
		}
	}
	//else
	//{
	//	Update(eGUM_UPDATE, eDIS_MOUSE_LEAVE);
	//}

	xSET_GLS_FLAG(m_flag, eGF_NO_INPUT, False);
	xSET_GLS_FLAG(m_flag, eGF_NO_MOUSEMOVE, False);
	InvalidateRect(0);


	return 0;
}

// ---------------------------------------
LRESULT cglsSplitButton::OnClicked(WPARAM wp, LPARAM lp)
{
	if (m_eStyle==eSBS_RIGHT_FULLDOWN &&
			m_bIsRightDown==False)
	{
		SendMessage(m_hNotifyWnd/*m_hParentWnd*/, WM_COMMAND,  MAKELONG(m_id, m_nSelectedIndex), (LPARAM)m_hWnd);
	}
	return 0;
}

// -----------------------------
static Bitmap* xGetExtendBitmap(Bitmap* bmp1, Bitmap* bmp2)
{
}

// -----------------------------
void cglsSplitButton::Update(eGlsUpdateMode mode, eDrawItemStatus status)
{
	if (mode==eGUM_FORCED)
	{
		int nMaxWidth=0;
		int nMaxHeight=0;
		int nFontHeight=GetFontHeight(True);//GetFont()->GetHeight(__DEFAULTGRAPHICS__);
		int nRightLength=5+(m_h/3);

		sSplitButtonItem* item = (sSplitButtonItem* )m_listSplitBtnItem->GetTopFromRef(m_nSelectedIndex);

		if (item==0)
			return ;

		if (item)
		{
			if (m_guiStyle==eGGS_NEON)
			{
				::xCreateItemBitmap(item, GetFont(), m_w-nRightLength, Color::White);
			}
			else
			{
				::xCreateItemBitmap(item, GetFont(), m_w-nRightLength);
			}
		}	

		cglsResourceBitmap* resource = (cglsResourceBitmap*)cGlassInterface::GetDefaultResource();

		int w = m_w;//(int)((float)nMaxWidth*1.2);
		int h = m_h;//(int)((float)nFontHeight + (float)nFontHeight/2.0);

		if (m_bIsAutoWidth==True)
		{
			if (m_factor!=0.0)
			{
				ReSize(GetFontHeight()*m_factor, cglsButton::GetMeasureHeight(GetFont()));
				w=m_w;
				h=m_h;
			}

			m_rectRight.left=m_w-nRightLength+1;
			m_rectRight.top=0;
			m_rectRight.bottom=h;
			m_rectRight.right=nRightLength+m_rectRight.left;

			m_detail.rect.left=0;
			m_detail.rect.top=0;
			m_detail.rect.right=w-1;
			m_detail.rect.bottom=h-1;
		}
		else
		{
			m_rectRight.left=m_detail.rect.right-nRightLength+1;
			m_rectRight.top=0;
			m_rectRight.bottom=m_detail.rect.bottom;
			m_rectRight.right=nRightLength+m_rectRight.left;

			if (m_detail.rect.right!=m_w ||
					m_detail.rect.bottom!=m_h)
			{
				ReSize(m_detail.rect.right, m_detail.rect.bottom);
			}
		}


		if (m_guiStyle==eGGS_NEON)
		{
			for (int i=0; i<3; i++)
			{
				if (m_item.pBmp[i])
					delete m_item.pBmp[i];

				m_item.pBmp[i]=resource->Draw(eGDID_NEON_SPLIT_PANNEL, i, w-1, nFontHeight, item->pItemBmp, 0, &m_detail);
			}

		}
		else
		{
			for (int i=0; i<3; i++)
			{
				if (m_item.pBmp[i])
					delete m_item.pBmp[i];

				m_item.pBmp[i]=resource->Draw(eGDID_GLASS_SPLIT_PANNEL, i, w-1, nFontHeight, item->pItemBmp, 0, &m_detail);
			}
		}

		int hhhh= m_item.pBmp[0]->GetHeight();

		YIDEBUG("SplitButton Height:%d", hhhh);

		if (m_eStyle==eSBS_RIGHT_FULLDOWN)
		{
			if (m_bmpRightFullDown)
				delete m_bmpRightFullDown;
			m_bmpRightFullDown = BITMAP32(m_item.pBmp[0]->GetWidth(), m_item.pBmp[0]->GetHeight());
			cyiBitmapBlender::BitmapCopy(m_bmpRightFullDown, m_item.pBmp[1], 0,0, 0, 0, m_w-nRightLength-2, m_h);
			cyiBitmapBlender::BitmapCopy(m_bmpRightFullDown, m_item.pBmp[2], m_w-nRightLength-1, 0, m_w-nRightLength-1, 0, nRightLength+1, m_h);
		}


		if (m_bIsAutoWidth)
		{
			if (m_item.pBmp[0]->GetWidth()>m_w)
			{
				ReSize(m_item.pBmp[0]->GetWidth(), m_h);
			}
		}

		//}

		//m_menu->Update(eGUM_FORCED);
		//sSplitButtonItem* x = (sSplitButtonItem*)m_listSplitBtnItem->GetNodeTopFrom(m_nSelectedIndex)->ref;

	}

	//sSplitButtonItem* x = (sSplitButtonItem*)m_listSplitBtnItem->GetNodeTopFrom(m_nSelectedIndex)->ref;
	

	// ここで右と左どちらが呼び出されているかをチェックしてビットマップをチェックする。
	//for (int i=0; i<3; i++)
	//{
	//	m_item.pBmp[i]=x->pBmp[i];
	//}

	//if (m_eStyle==eSBS_RIGHT_FULLDOWN)
	//{
	//	m_item.pBmp[5]=x->pBmpEx[0];
	//}
	
	m_item.pCurrentBmp=m_item.pBmp[m_item.nLastItemIndex];

	cGlassInterface::Update(mode, status);

	return ;
}

// ---------------------------
void cglsSplitButton::Clear()
{
	sSplitButtonItem*s =0;

	if (m_listSplitBtnItem==0)
		return ;

	while ((s=(sSplitButtonItem* )m_listSplitBtnItem->removeEx(0))!=0)//; node; node=node->np)
	{
		if (s->text)
			yifree(s->text);

		if (s->comment)
			yifree(s->comment);

		if (s->pItemBmp)
		{
			delete s->pItemBmp;
		}
	}
	
	m_menu->Destroy();
	delete m_menu;

	//m_menu = new cglsPopupMenu();
	m_menu = new cglsListMenu(GetFont());
	//m_menu->Create(m_hWnd, 0); // デフォルトは-1
}


// ---------------------------
Bool cglsSplitButton::CreateBitmap(sDrawItem* item, void* param)
{
	return True;
}

#include "glsListBox.h"
// ------------------------------------
LRESULT cglsSplitButton::OnMouseLButtonDown(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	POINT pt = {0, m_h};

	//　親に流すため
	handled=True;

	if (m_eStyle==eSBS_RIGHT_FULLDOWN)
	{
		int x   =  lParam & 0xFFFF;         // カーソルx座標
		int y   = (lParam >> 16) & 0xFFFF;  // カーソルy座標

		m_bIsRightDown=False;
		if (x<m_rectRight.right && x>m_rectRight.left &&
				y<m_rectRight.bottom && y>m_rectRight.top)
		{
			m_bIsRightDown=True;
		}

		if (m_bIsRightDown==False)
		{
			return __super::OnMouseLButtonDown(wParam, lParam, handled);
		}
	}

	ClientToScreen(m_hWnd, &pt);
	m_bIsBeforeMenuShow=True;
	//for (int i=0; i<m_listSplitBtnItem->count(); i++)
	//{
	//	m_menu->SetCheck(i, False, False);
	//}
	//m_menu->SetCheck(m_nSelectedIndex, True, False);
	//m_menu->Show(pt.x, pt.y, TPM_LEFTALIGN | TPM_TOPALIGN);

	__super::OnMouseLButtonDown(wParam, lParam, handled);
	xSET_GLS_FLAG(m_flag, eGF_NO_INPUT, True);
	xSET_GLS_FLAG(m_flag, eGF_NO_MOUSEMOVE, True);

	m_menu->SetCheck(m_nSelectedIndex, 0, True);

	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);


	if (m_guiStyle==eGGS_NEON)
	{
		sGlsListBoxDetail detail;

		cglsListBox::xDefaultDetail(&detail);

		detail.colorBg1=Color::Black;
		detail.colorBg2=Color::DarkRed;
		detail.colorFont1=Color::White;
		detail.colorFont2=Color::White;
		detail.colorCheck=Color::White;
		m_menu->TrackPopup(m_hWnd, pt.x, pt.y, m_w, nScreenHeight, 1, 0, 1, &detail);//!=0)
	}
	else
	{
		m_menu->TrackPopup(m_hWnd, pt.x, pt.y, m_w, nScreenHeight);//!=0)
	}

	xSET_GLS_FLAG(m_flag, eGF_NO_INPUT, False);
	xSET_GLS_FLAG(m_flag, eGF_NO_MOUSEMOVE, False);
	SetItemStatus(&m_item, eDIS_MOUSE_DOWN);
	OnMouseLeave(0, 0);
	//Update(eGUM_UPDATE, eDIS_MOUSE_LEAVE);
	InvalidateRect(0);

	//{
	//	SendMessage(m_hNotifyWnd, WM_COMMAND, MAKELONG(m_id, CBN_SELCHANGE), (LPARAM)m_hWnd);
	//}

	m_bIsBeforeMenuShow=False;

	
	//
	//xSET_GLS_FLAG(m_flag, eGF_NO_INPUT, True);
	//xSET_GLS_FLAG(m_flag, eGF_NO_MOUSEMOVE, True);
	return 0;
}

// ------------------------------------
LRESULT cglsSplitButton::OnMouseLButtonUp(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	handled=True;
	//m_bIsRightDown=False;

	if (m_eStyle==eSBS_RIGHT_FULLDOWN)
	{
		if (m_bIsRightDown==False)
		{
			return __super::OnMouseLButtonUp(wParam, lParam, handled);
		}
	}
	return 0;
}

// -----------------------------
int cglsSplitButton::GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation)
{
	if (m_eStyle!=eSBS_RIGHT_FULLDOWN)
	{
		return __super::GetStatusToBitmapIndex(item, status, bIsNoAnimation);
	}

	if (status==eDIS_MOUSE_DOWN)
	{
		if (m_bIsRightDown==True)
		{
			bIsNoAnimation=True;
			//*ret=m_bmpRightFullDown;
			return 2;
		}
	}
	else if (status==eDIS_MOUSE_LEAVE)
	{
		if (m_bIsRightDown==True)
		{
			m_bIsRightDown=False;
			bIsNoAnimation=True;
			return 0;
		}
	}
	return __super::GetStatusToBitmapIndex(item, status, bIsNoAnimation);
}

// -----------------------------------------------------
int cglsSplitButton::GetItemCount()
{
	return m_listSplitBtnItem->count();
}


// -----------------------------------------------------
int cglsSplitButton::SelectIndex(int n)
{
	int prev=m_nSelectedIndex;
	m_nSelectedIndex=n;
	Update(eGUM_FORCED);

	InvalidateRect(0);
	//PostMessage(m_hNotifyWnd, WM_COMMAND, MAKELONG(m_id, CBN_SELCHANGE), (LPARAM)m_hWnd);
	return prev;
}

// -----------------------------------------------------
int cglsSplitButton::GetSelectIndex()
{
	return m_nSelectedIndex;
}

// -----------------------------------------------------
int cglsSplitButton::GetSelectedId()
{
	sSplitButtonItem* item = (sSplitButtonItem* )m_listSplitBtnItem->GetTopFromRef(m_nSelectedIndex);
	
	if (item==0)
		return 0;
	return item->id;
}

// -----------------------------------------------------
Bool cglsSplitButton::GetSelectedText(wchar_t* buffer)
{
	if (m_listSplitBtnItem==0)
		return False;

	sSplitButtonItem* item = (sSplitButtonItem* )m_listSplitBtnItem->GetTopFromRef(m_nSelectedIndex);

	if (item==0)
		return False;

	wcscpy(buffer, item->text);

	return True;

}