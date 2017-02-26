#include "glsToolContainer.h"
#include "glsDialog.h"
#include "glsResourceBitmap.h"
#include "yiGoCommandz.h"


// ---------------------------------------------
cglsToolContainer::cglsToolContainer(void)
: m_msgMenu(0)
, m_msgBox(0)
, m_listMsg(0)
, m_nCurrSel(-1)
, m_bIsMessageButtonOn(0)
, m_mode(eGAM_NORMAL)
{
}

// ---------------------------------------------
cglsToolContainer::~cglsToolContainer(void)
{
}

// ---------------------------------------------
Bool cglsToolContainer::Create(cGlassInterface* parent, int id, int x, int y, int h, cglsPopupMenu* menu, eGlsToolContainerStyle style, HWND hWndMenuNotifyWnd, HWND hWndNotifyTab, Font* font, eGobanAppMode mode)//Bool bIsJosekiMode)
{
	sGlsDialogDetail xdetail;
	cglsDialog::xDefaultDetail(&xdetail);
	
	xdetail.font=font;
	xdetail.gauss.fBlureRad=5.0;
	xdetail.gauss.fShadowWidth=0.0;
	
	m_font=font;//detail->font1;
	m_hMenuNotifyWnd=hWndMenuNotifyWnd;
	m_hTabNotifyWnd=hWndNotifyTab;

	xdetail.gauss.colorTop=__COLORp__(168, 255, 255, 255);
	xdetail.gauss.colorBottom=__COLORp__(200, 255, 255, 255);
	//detail->colorTop=__COLORp__(180, 255, 255, 255);
	//detail->colorBottom=__COLORp__(255, 255, 255, 255);

	//m_eStyle=detail->style;
	m_menu=menu;
	m_mode=mode;
	//m_bisJosekiMode=bIsJosekiMode;
	xdetail.bIsAnimationShow=False;
	return cglsDialog::Create((HWND)parent, id , 0, x, y, 15, h, &xdetail);
}

// --------------------------------------------
cglsTabCtrl* cglsToolContainer::GetTab()
{
	cglsTabCtrl* tab = (cglsTabCtrl*)GetChild(128+2);
	return tab;
}

// ------------------------------------
void cglsToolContainer::SetPannelStyle(eGlsToolContainerStyle style)
{
	m_eStyle=style;
	ReArrangeItemz();
}

#include "svry/svrydefs.h"
// --------------------------------------------
void cglsToolContainer::UpdateMessage(cyiList* list)
{
	GetChild(17)->SetHiLight(True);

	if (m_msgMenu)
		m_msgMenu = new cglsListMenu(GetFont());

	m_msgMenu->Clear();

	if (list==0)
		return ;

	int id=512;
	for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
	{
		sMsg* msg = (sMsg*)node->ref;
		//m_msgMenu->AddItem(id++, msg->, 0);
	}

	m_listMsg=list;

	return ;
}

#include "glsMessageBox.h"

// --------------------------------------------
void cglsToolContainer::AddMessage(sMsg* msg, Bool bShowMessage, const wchar_t* menuMsg)
{
	wchar_t text[1024];
	//wchar_t message[64];
	const wchar_t* message=0;

	if ((msg->type&0xFFFF)==0)
	{
		message=L"お知らせ";
	}
	else if ((msg->type&0xFFFF)==1)
	{
		message=L"失敗";
	}
	else
	{
		message=L"ユーザからの要求";
	}

	if (menuMsg!=0)
		message=menuMsg;

	wsprintfW(text, L"%d/%02d/%02d %02d:%02d:%02d\r\n%s", DAY_YEAR(msg->day), DAY_MON(msg->day), DAY_DAY(msg->day),
															xHOUR(msg->time), xMINUTE(msg->time), xSECOND(msg->time),
																msg->data);
	
	int size = sizeof(sMsg)+(wcslen(text)<<1);
	sMsg* xxx = (sMsg*)yialloci(size);

	memcpy(xxx, msg, sizeof(*xxx));
	wcscpy((wchar_t*)xxx->data, text);

	if (m_listMsg==0)
		m_listMsg = new cyiList();
	m_listMsg->addref(xxx);

	if (m_msgMenu==0)
	{
		m_msgMenu=new cglsListMenu(GetFont(), True, True);
	}
	
	m_msgMenu->AddItem(m_listMsg->count()+512, message);
	//m_menu->TrackPopup(m_hWnd, pt.x, pt.y, m_w, nScreenHeight);//!=0)

	if (bShowMessage)
	{
		if (m_msgBox)
		{
			if (m_msgBox->m_bIsCreated==True)
			{
				//m_msgBox->Destroy();
				//delete m_msgBox;
				//m_msgMenu->SetExceptWindow(0);
				//m_msgBox=new cglsMessageBox();
				//return ;
				return ;
			}
			//delete m_msgBox;
		}
		else
		{
			m_msgBox = new cglsMessageBox();
		}
		
		cGlassInterface* child=GetChild(17);
		POINT pt={child->m_x+child->m_w, child->m_y};
		ClientToScreen(*this, &pt);
		ScreenToClient(*m_parent, &pt);

		
		m_msgBox->ShowByTimer(m_parent, (wchar_t*)msg->data, L"", -1, False, 3000, 1000, 1000, GetFont(), &Point(pt.x, pt.y));

		m_bIsMessageButtonOn=True;
		//SetTimer(*this, 1, 3000, NULL);
	}

	

	return ;
}


// --------------------------------------------
LRESULT cglsToolContainer::OnTimer(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (m_bIsMessageButtonOn==True)
		GetChild(17)->OnMouseEnter(0, 0);
	else
		GetChild(17)->OnMouseLeave(0, 0);

	m_bIsMessageButtonOn=1-m_bIsMessageButtonOn;

	return 0;
}



// --------------------------------------------
LRESULT cglsToolContainer::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (wParam&0x04000000 &&
			(wParam&0xFFFF)>=512)
	{
		int msgId = (wParam&0xFFFF);//((wParam&0xFFFF)-512);
		int index = m_msgMenu->GetIndex(msgId);

		if (index==-1)
			return 0;

		sMsg* msg = (sMsg*)m_listMsg->GetTopFromRef(index);

		bIsHandled=True;

		if (msg==0)
			return 0;

		if (m_msgBox)
		{
			if (IsWindowVisible(*m_msgBox)==True)
			{
				m_msgBox->Destroy();
				m_msgMenu->SetExceptWindow(0);
				//delete m_msgBox;
				//m_msgBox=0;
			}
		}

		m_nCurrSel=index;
		
		// 確認
		if (msg->type==0)
		{
			POINT pt={(lParam>>16)+m_msgMenu->m_w, lParam&0xFFFF};
			ScreenToClient(*m_parent, &pt);
			//x+=m_msgMenu->m_w;
			//wchar_t* uni = yiToUnicode(0, msg->data, 0);

			if (m_msgBox!=0)
			{
				m_msgBox->Destroy();
				//delete m_msgBox;
			}
			//\ cglsMessageBox();
			m_msgMenu->SetExceptWindow(m_msgBox);
			m_msgBox->Show(20, m_parent, (const wchar_t*)msg->data, L"", MB_OK, GetFont(), &Point(pt.x, pt.y)); 
			m_msgBox->SetNotifyWindow(*this);
			//yifree(uni);
		}
		if ((msg->type&0xFFFF)==2)
		{
			POINT pt={(lParam>>16)+m_msgMenu->m_w, lParam&0xFFFF};
			ScreenToClient(*m_parent, &pt);
			//x+=m_msgMenu->m_w;
			//wchar_t* uni = yiToUnicode(0, msg->data, 0);

			if (m_msgBox!=0)
			{
				m_msgBox->Destroy();
				//delete m_msgBox;
			}
			//\ cglsMessageBox();
			m_msgMenu->SetExceptWindow(m_msgBox);
			m_msgBox->Show(20, m_parent, (const wchar_t*)msg->data, L"", MB_YESNO, GetFont(), &Point(pt.x, pt.y)); 
			m_msgBox->SetNotifyWindow(*this);
		}
	}
	else if ((wParam&0xFFFF)==16)
	{
		bIsHandled=True;
		OnShowMenu((HWND)lParam, (wParam)>>16);
	}
	else if ((wParam&0xFFFF)==17)
	{
		cGlassInterface* child=GetChild(17);
		POINT pt={child->m_w, 0};
		ClientToScreen(*child, &pt);
		
		if (m_msgMenu==0)
			return 0;

		KillTimer(*this, 1);
		m_nCurrSel=-1;
		m_msgMenu->TrackPopup(m_hWnd, pt.x, pt.y, GetFontHeight(True)*5, 200, False, 0, False);//!=0)

		if (m_msgBox)
		{
			if (IsWindow(*m_msgBox)==True)
			{
				m_msgBox->Destroy();
				//delete m_msgBox;
				//m_msgBox=0;
				m_msgMenu->SetExceptWindow(0);
			}
		}
	}
	else if ((wParam&0xFFFF)==20)
	{
		int code = wParam>>16;
		
		if (m_nCurrSel==-1)
			return 0;

		sMsg* msg = (sMsg*)m_listMsg->GetTopFromRef(m_nCurrSel);

		if (msg==0)
		{
			m_nCurrSel=-1;
			return 0;
		}

		if (code==IDOK)
		{
			if (msg->type==0)
			{
				if (msg->msgId!=0xFF)
				{
					GetSystem()->GetCommandCenter()->OnCommand(eGCID_DEL_MSG32, this, msg->msgId, 0);
				}
				m_listMsg->remove(m_nCurrSel);
				m_msgMenu->RemoveItem(m_nCurrSel);
				yifree(msg);
			}
			else if ((msg->type&0xFFFF)==2)
			{
				GetSystem()->GetCommandCenter()->OnCommand(eGCID_MSG_EXEC, this, (IntPtr)msg, 1);

				m_listMsg->remove(m_nCurrSel);
				m_msgMenu->RemoveItem(m_nCurrSel);
				yifree(msg);
			}
		}
		else
		{
			if ((msg->type&0xFFFF)==2)
			{
				GetSystem()->GetCommandCenter()->OnCommand(eGCID_MSG_EXEC, this, (IntPtr)msg, 0);
				m_listMsg->remove(m_nCurrSel);
				m_msgMenu->RemoveItem(m_nCurrSel);
				yifree(msg);
			}

		}
		
		m_nCurrSel=-1;
	}

	return 0;
}

// ------------------------------------
void cglsToolContainer::OnItemReArranged()
{
	RECT rect;
	GetClientRect(&rect);
	int x=rect.left+2;
	int y=rect.top+2;
	int length=m_h-(rect.left<<1);
	int maxWidth=0;
	cGlassInterface* child;

	GetChild(16)->Move(x, y);
	y+=GetChild(16)->m_h;

	maxWidth=GetChild(16)->m_w;

	if (m_eStyle==eGTCS_AFTER_LOGIN)
	{
		child =GetChild(17);
		child->Move(x, y);
		y+=child->m_h;
		child->Show();
		child->Update(eGUM_FORCED);
	}
	else
	{
		GetChild(17)->Hide();
	}

	cglsTabCtrl* tab=(cglsTabCtrl* )GetChild(128+2);
	tab->Move(x, y);
	tab->ReSize(m_h-y-rect.top, GetFont());
	tab->Update(eGUM_FORCED);
	
	if (tab->GetItemCount()==0)
	{
		maxWidth=maxWidth>tab->GetMeasureHeight()?maxWidth:tab->GetMeasureHeight();
	}
	else
	{
		maxWidth=maxWidth>tab->m_w?maxWidth:tab->m_w;
	}

	ReSize(maxWidth+(x<<1), m_h);
	GetChild(16)->Move((m_w-GetChild(16)->m_w)>>1, GetChild(16)->m_y);
	GetChild(17)->Move((m_w-GetChild(17)->m_w)>>1, GetChild(17)->m_y);


	InvalidateRect(0, 0, True);

	return ;
}


// --------------------------------------------
LRESULT cglsToolContainer::OnCreate(WPARAM wParam, LPARAM lParam)
{
	//cglsPopupMenu* m_menu=new cglsPopupMenu();
	//m_menu->CreateFromResource(m_hMenuNotifyWnd, m_hMenuInstance, IDR_GOBAN_TOP);

	//cglsSystem* system = GetSystem();//m_main->GetSystem();

	//if (system)
	//{
	//	system->RegisterCommand(m_gobanMenu, eGCID_CALCULATE_START, IDM_CALCULATE_START);
	//	system->RegisterCommand(m_gobanMenu, eGCID_LOGIN, IDM_LOGIN);
	//	system->RegisterCommand(m_gobanMenu, eGCID_REQ_PLAY_DIALOG, IDM_REQUEST_PLAY);
	//	system->RegisterCommand(m_gobanMenu, eGCID_ADD_NEW, IDC_CREATE_NEW);
	//}

	// ボタン生成
	sGlsButtonDetail detail;

	cglsButton::xDefaultDetail(&detail);

	detail.colorHilight=Color::LightBlue;
	//detail.gls.font=cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON);
	cglsButton* btn = new cglsButton();
	btn->Create((HWND)this, 16, 0, 0, (Bitmap*)eGRID_ICON_MENU, 0, 0, &detail); 
	btn->Show();
	btn->SetNotifyWindow(m_hWnd);
	btn->Update(eGUM_FORCED);

	btn = new cglsButton();
	btn->Create((HWND)this, 17, 0, 0, (Bitmap*)eGRID_ICON_MESSAGE, 0, 0, &detail); 
	btn->Hide();
	btn->SetNotifyWindow(m_hWnd);
	btn->Update(eGUM_FORCED);


	// タブ生成
	sGlsTabDetail tabDetail;
	cglsTabCtrl::xDefaultDetail(&tabDetail);
	tabDetail.stack.bIsVertical=True;
	tabDetail.wMaxShowCount=1;
	tabDetail.wMinShowCount=5;

	//detail.font=cyctrlGobanResource::GetInstance().GetFont(eRIDF_TAB_CONTROL);

	cglsTabCtrl* tab = new cglsTabCtrl();
	tab->Create((HWND)this, 128+2, 0, 0, m_h, &tabDetail);
	tab->Show();
	tab->SetNotifyWindow(m_hTabNotifyWnd);

	if (m_mode!=eGAM_JOSEKI_EDITOR &&
			m_mode!=eGAM_JOSEKI_PLAYER)
	{
		sGlsTabItem item;
		cglsTabCtrl::xDefaultItemDetail(&item, Color::LightBlue);
		item.bIsNoLock=True;
		tab->AddItem(256, 0, L"+", False, &item);
		tab->Update(eGUM_FORCED);
	}

	ReArrangeItemz();
	return 0;
}


// ---------------------------------------------------------------
LRESULT cglsToolContainer::OnShowMenu(HWND hWnd, int code)
{
	if (m_menu)
	{
		POINT pt;
		GetCursorPos(&pt);
		m_menu->Show(pt.x, pt.y);
	}
	return 0;
}