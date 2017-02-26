#include "glsGoban.h"
#include "yctrlGobanResource.h"
#include "yiGoCommandz.h"

#ifdef _TIMER_DEBUG_
extern sGoRemainTime g_black;//={0, };
extern sGoRemainTime g_white;//={0, };
#endif

// ----------------------------------------
stone_t cglsGoban::xChangeStoneCoordinate(stone_t stone, int nLineCnt)
{
	int x = STONE_POS_X(stone);
	int y = STONE_POS_Y(stone);
	int type = STONE_TYPE(stone);
	int index = STONE_ORDER(stone);

	x=nLineCnt-x-1;
	y=nLineCnt-y-1;
	return MAKESTONE(type, index, x, y);
}



// ---------------------------------------------------------------
LRESULT cglsGoban::OnCreate(WPARAM wp, LPARAM lp)
{
	if (m_bIsPreviewMode==True)
	{
		InitGraphics(m_hResourceInstance);
		return 0;
	}

	int y=0;
	RECT rect;

	::GetClientRect(m_hWnd, &rect);
	InitGraphics(m_hInstance);


	//if (m_detail.menu)
	//{
	//	sGlsButtonDetail detail;

	//	cglsButton::xDefaultDetail(&detail);

	//	detail.colorHilight=Color::LightBlue;
	//	detail.gls.font=cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON);
	//	m_btnMenu = new cglsButton();
	//	m_btnMenu->Create((HWND)m_parent, CHILD_ID_GOBAN_MENU, m_x+m_w, 0, (Bitmap*)eGRID_ICON_MENU, 0, 0, &detail); 
	//	m_btnMenu->Show();
	//	m_btnMenu->SetNotifyWindow(m_hWnd);
	//	m_btnMenu->Update(eGUM_FORCED);
	//	y=m_btnMenu->m_h;
	//}

	// TODO: ここでタブコントロールを生成
	//sGlsTabDetail detail;
	//cglsTabCtrl::xDefaultDetail(&detail);
	//detail.stack.bIsVertical=True;
	//detail.wMaxShowCount=1;
	//detail.wMinShowCount=5;
	//detail.font=cyctrlGobanResource::GetInstance().GetFont(eRIDF_TAB_CONTROL);

	//m_tab->Create((HWND)m_parent, xCHILD_ID_TAB, m_x+m_w, y, m_w-y, &detail);
	//m_tab->Show();
	//m_tab->SetNotifyWindow(m_hWnd);
	//m_tab->AddItem(256, 0,L"+");
	
	//m_tab->Update(eGUM_FORCED);

	sGlsImageBoxDetail detail;

	cglsImageBox::xDefaultDetail(&detail);
	detail.bIsImageListMode=True;
	detail.bIsMouseParent=True;
	detail.bIsDragAndDrop=False;

	m_cursor = new cglsImageBox();
	m_cursor->Create(this, CHILD_ID_CURSOR, 0, 0, 0, 0, 0, &detail);
	m_cursor->Hide();

	//if (m_detail.bIsJosekiEditorMode==False)
	{
		detail.bIsClickEvent=False;
		m_imgPosX = new cglsImageBox();
		m_imgPosX->Create(this, CHILD_ID_POS_X, 0, 0, 0, 0, 0, &detail);
		m_imgPosX->Hide();

		m_imgPosY = new cglsImageBox();
		m_imgPosY->Create(this, CHILD_ID_POS_Y, 0, 0, 0, 0, 0, &detail);
		m_imgPosY->Hide();
	}
	//else
	//{
	//	m_imgPosX=0;
	//	m_imgPosY=0;
	//}


	m_container = new cglsToolContainer();
	
	int w = GetFontHeight(True);//CalculateFontHeight(cyctrlGobanResource::GetInstance().GetFont(eRIDF_TAB_CONTROL), True);
	w+=(w>>1);
	m_container->Create(m_parent, xCHILD_TOOL, m_x+m_w+2, w, m_h, m_detail.menu, eGTCS_BEFORE_LOGIN, *m_parent, *this, cyctrlGobanResource::GetInstance().GetFont(eRIDF_TAB_CONTROL), m_detail.mode);
	m_tab=m_container->GetTab();
	m_container->Show();

#ifdef NEW_STYLE
	xCreateCommonGobanControl();
	m_controlz[eGCT_CONTROL]->Show();
#endif

	//m_container->AnimationShow(True, 500);

	//m_tab->AddItem(256, 0, L"+");
	//m_tab->Update(eGUM_FORCED);
	//m_container->ReArrangeItemz();

	// ドラッグアンドドロップを有効
	DragAcceptFiles(m_hWnd, TRUE);


	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnDropFiles(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	HDROP hDrop=(HDROP)wParam;
	char buffer[MAX_PATH];

	int n = DragQueryFile(hDrop, -1, (LPSTR)buffer, MAX_PATH);

	
	for (int i=0; i<n; i++)
	{
		DragQueryFile(hDrop, i, (LPSTR)buffer, MAX_PATH);

		cglsSystem* system = GetSystem();

		if (system==0)
		{
			sGobanContentsDetail detail;
			cyiGoEngine::xDefaultDetail(&detail);
			detail.eFormat=eGFT_SGF;
			goid_t id = AddForEditByFile(buffer);
			
			PostMessage(m_hParentWnd, WM_GOBAN_ADDED, 0, (LPARAM)id);
		}
		else
		{
			system->GetCommandCenter()->OnCommand(eGCID_ADD_GOBAN, this, (IntPtr)buffer, 0);

			if (n==1)
			{
				sGobanContentsDetail detail;
				cyiGoEngine::xDefaultDetail(&detail);
				detail.eFormat=eGFT_SGF;
				goid_t id = AddForEditByFile(buffer);
			}
			
			//PostMessage(m_hParentWnd, WM_GOBAN_ADDED, 0, (LPARAM)id);
		}
	}
	DragFinish(hDrop);

	//Update(eGL_ALL);
	
	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnTimer(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (wParam==1)
	{
		KillTimer(m_hWnd, 1);
		if (GetCurrentNode()==m_node_for_timer)
		{
			cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

			if (eng)
			{
				if (m_listNodeChangeNotifyWnds)
				{
					for (cyiNode* node=m_listNodeChangeNotifyWnds->GetNodeTopFrom(); node; node=node->np)
					{
						HWND hWnd = (HWND)node->ref;

						if (hWnd && IsWindowVisible(hWnd))
						{
							PostMessage(hWnd, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
						}
					}

				}
				//if (IsWindowVisible(m_preview->m_hWnd)==True)
				//{
				//	//PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
				//	PostMessage(*m_preview, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
				//}
			}
			//if (eng)
			//{
			//	PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
			//}
		}
	}
	else if (wParam==2)
	{
		KillTimer(m_hWnd, 2);

		cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

		if (eng)
		{
			sGoPlayInfo* info= (sGoPlayInfo* )eng->GetExtraData();
			m_controlz[eGCT_CONTROL]->ShowPage(1, eSBSD_TOLEFT);
			info->nLastControlIndex=1;

			if (m_detail.mode==eGAM_JOSEKI_EDITOR)
			{
				return 0;
			}

			SetTitleText(eng->GetTitle());
		}
	}
	else if (wParam==3)
	{
		KillTimer(m_hWnd, 3);
		NotifyNodeChanged(True);
	}

	return 0;
}

// ---------------------------------------------------------------
// マウスが離れた場合、カーソルを隠す。
LRESULT cglsGoban::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	if (this->m_bIsPreviewMode==True)
		return 0;

	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
		return 0;

	if (eng->m_bIsLocked==True)
	{
		if (m_cursor)
			m_cursor->Hide();
		return 0;
	}


	if (pt.x<0 || pt.y<0)
	{
		m_cursor->Hide();

		if (m_imgPosX)
		{
			m_imgPosX->Hide();
			m_imgPosY->Hide();
		}
	}
	if (pt.x>m_w ||
			pt.y>m_w)
	{
		m_cursor->Hide();

		if (m_imgPosX)
		{
			m_imgPosX->Hide();
			m_imgPosY->Hide();
		}
	}
	//DrawCursor(0, -1, -1, True);
	return 0;
}

// ---------------------------------------------------------------
// マウスが離れた場合、カーソルを隠す。
LRESULT cglsGoban::OnMouseEnter(WPARAM wParam, LPARAM lParam)
{
	if (this->m_bIsPreviewMode==True)
		return 0;
	//DrawCursor(0, -1, -1, True);
	ChangeMouseCursor(0, True);

	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
		return 0;

	if (eng->m_bIsLocked==True)
	{
		if (m_cursor)
			m_cursor->Hide();
		return 0;
	}

	return 0;
}


// -------------------------------
Bool cglsGoban::ShowCursor(Bool bIsShow)
{
	ClearCursor();

	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
		return 0;

	xSET_BIT(eng->m_flag, eGEF_HIDE_CURSOR, bIsShow);
	return True;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnKeyDown(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (wParam == VK_SHIFT)
	{
		DrawCursor(0, 0, 0, 0, MK_SHIFT, True);
		return 0;
	}
	else if (wParam == VK_CONTROL)
	{
		DrawCursor(0, 0, 0, 0, MK_CONTROL, True);
		return 0;
	}
	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnKeySysDown(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	//if (wParam==VK_MENU)
	//{
	//	bIsHandled=True;		
	//	SetLabelMode(eLT_ROMA_LARGE);
	//	DrawCursor(0, -1, -1, 0, 0, True);

	//	return 0;
	//}
	//else if (wParam == VK_CONTROL)
	//{
	//	DrawCursor(0, 0, 0, 0, MK_CONTROL, True);
	//	return 0;
	//}

	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnKeySysUp(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (wParam == VK_MENU)
	{
		bIsHandled=True;
		SetLabelMode(eLT_LABLE_CLOSE);
		DrawCursor(0, -1, -1, 0, 0, True);

		return 0;
	}

	bIsHandled=False;
	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnKeyUp(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	DrawCursor(0, 0, 0, 0, 0, True);
	return 0;
}

// ---------------------------------------------------------------
// MouseMove時は、カーソルを表示する。
LRESULT cglsGoban::OnMouseMove(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	POINT ps;
	int xPos   =  lParam & 0xFFFF;
	int yPos   = (lParam >> 16) & 0xFFFF;

	if (this->m_bIsPreviewMode==True)
	{
		// 今のところ
		return 0;
	}
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];


	//ChangeMouseCursor(0);
	//if (GetFocus()!=m_hWnd)
	//{
	//	SetFocus(m_hWnd);
	//}


	if (eng==0)
	{
		if (m_imgPosX)
		{
			m_imgPosX->Hide();
			m_imgPosY->Hide();
		}
		return 0;
	}

	if (eng->m_bIsLocked==True)
	{
		m_cursor->Hide();
		return 0;
	}


	sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();
	if (info)
	{
		if (info->dlg)
		{
			if (info->dlg->IsCreated()==True)
			{
				return 0;
			}
			delete info->dlg;
			info->dlg=0;
		}

		//if (info->baloon)
		//{
		//	if (IsWindowVisible(*info->baloon)==True)
		//	{
		//		info->baloon->Hide();
		//		info->baloon->SetText(0, 0);
		//		return 0;
		//	}
		//}
	}

#ifndef NEW_STYLE
	if (m_currentView)
	{
		if (IsWindowVisible(*m_currentView)==True)
		{
			return 0;
		}
	}
#endif

	if (xIS_BIT(eng->m_flag, eGEF_HIDE_CURSOR)==True)
	{
		return 0;
	}

	eInputMode InputMode = eng->GetInputMode();
	// 描画しないケース
	// 1. eIM_LOCK時
	// 2. 地合計算時
	// 3. 試合時、自分の番ではないとき
	
	if (InputMode==eIM_LOCK)
	{
		DrawCursor(0, -1, -1, True, wParam);

		if (m_imgPosX)
		{
			m_imgPosX->Hide();
			m_imgPosY->Hide();
		}
		return 0;
	}



	// 何もしない。
	if (InputMode==eIM_FIND_COMMENT)
	{
		DrawCursor(0, -1, -1, True, wParam);
		return 0;
	}

	if (GetNearCoordinate(xPos, yPos, &ps)==False)
	{
		DrawCursor(0, -1, -1, True, wParam);
		return 0;
	}

	if (m_detail.mode==eGAM_JOSEKI_EDITOR)//bIsJosekiEditorMode)
	{
		if (InputMode==eIM_LABEL)
		{
			DrawCursor(0, ps.x, ps.y, False, wParam);
			return 0;
		}

		if (InputMode==eIM_INITSTONE_MODE)
		{
			DrawCursor(0, ps.x, ps.y, True, wParam);
			return 0;
		}
		DrawCursor(0, ps.x, ps.y, STONE_TYPE(eng->GetLastStone())==eST_BLACK?True:False , wParam);
		return 0;
	}

	int xpos=ps.x;
	int ypos=ps.y;

	if (wParam & MK_CONTROL)
	{
		xpos=18-xpos;
		ypos=18-ypos;
	}

	if (m_imgPosX)
	{
		if (m_imgPosX->GetBitmap(xpos+1)==0)
		{
			Bitmap* bmp = xCreatePosImage(xpos, False);
			m_imgPosX->AppendBitmap(xpos+1, bmp, True);

		}
		m_imgPosX->SelectBitmap(xpos+1, False);
		if (m_imgPosY->GetBitmap(ypos+1)==0)
		{
			Bitmap* bmp = xCreatePosImage(ypos, True);
			m_imgPosY->AppendBitmap(ypos+1, bmp, True);
		}
		m_imgPosY->SelectBitmap(ypos+1, False);

		m_imgPosX->Move(xCENTER(m_scaled_point[ps.x].x, m_imgPosX->m_w),0);
		m_imgPosY->Move(0/*m_w-m_imgPosY->m_w*/,xCENTER(m_scaled_point[ps.y].y, m_imgPosY->m_h));

		m_imgPosX->Show();
		m_imgPosY->Show();
	}

	if (/*InputMode==eInputMode::eIM_CALCULATE ||*/
		InputMode==eInputMode::eIM_CALCULATE_END)
	{
		DrawCursor(0, -1, -1, True, wParam);
		return 0;
	}

	if (InputMode==eInputMode::eIM_PLAY)
	{
		Bool bIamBlack = IAM_BLACK(eng->GetHeader()->condition);
		
		if (bIamBlack!=eng->IsCurrentOrderBlack())
		{
			DrawCursor(0, -1, -1, True, wParam);
			return 0;
		}

		//stone_t stone = eng->GetLastStone();

		//if (STONE_TYPE(stone)==eST_BLACK && bIamBlack==True)
		//{
		//	return 0;
		//}

		//if (STONE_TYPE(stone)==eST_WHITE && bIamBlack==True)
		//{
		//	DrawCursor(0, -1, -1, True, wParam);
		//	return 0;
		//}
	}

	if (InputMode==eIM_LABEL)
	{
		DrawCursor(0, ps.x, ps.y, False, wParam);
		return 0;
	}

	if (InputMode==eIM_INITSTONE_MODE)
	{
		DrawCursor(0, ps.x, ps.y, True, wParam);
		return 0;
	}

	DrawCursor(0, ps.x, ps.y, STONE_TYPE(eng->GetLastStone())==eST_BLACK?True:False , wParam);

	return 0;
}

// ---------------------------------------------------------------
// マウスの左クリック時、マウスのボタンから指が離れてから石を置く。
// ただし、対局モード時のもである。（マウスミスを防ぐために）
LRESULT cglsGoban::OnLButtonUp(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
xTRACE("cglsGoban::OnLButtonUp() call");

	if (m_bIsPreviewMode==True)
		return 0;

	if (m_detail.mode==eGAM_JOSEKI_EDITOR ||
			m_detail.mode==eGAM_JOSEKI_PLAYER)
	{
		return 0;
	}

	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	Bool bIsControlDown = ((wParam&VK_CONTROL)!=0);


	if (eng==0)
	{
		m_cursor->Hide();
		return 0;
	}


	if (eng->m_bIsLocked==True)
	{
		m_cursor->Hide();
		return 0;
	}

	if (eng->GetInputMode()==eIM_PLAY)
	{
		m_cursor->Hide();
	}

	//if (GetFocus()!=m_hWnd)
	//{
	//	SetFocus(m_hWnd);
	//}

	eInputMode inputMode = eng->GetInputMode();
	eMouseMode mouseMode = eng->GetMouseMode();


	if (inputMode==eIM_CALCULATE ||
			inputMode==eIM_STUDY)
	{
		return 0;
	}

	// 置石設定が終わってない場合。
	if (IAM_BLACK(eng->GetHeader()->condition) &&
				eng->GetCurrentState()!=eCS_PLAY)
	{
		POINT ps;

		int xPos   =  lParam & 0xFFFF;
		int yPos   = (lParam >> 16) & 0xFFFF;
		eStoneType stoneType=STONE_TYPE(eng->GetLastStone());

		if (GetNearCoordinate(xPos, yPos, &ps)==False)
		{
			return 0;
		}

		eng->InitStone(ps.x, ps.y);

		stone_t send_stone = xChangeStoneCoordinate(MAKESTONE(eST_BLACK, 0, ps.x, ps.y), eng->GetLineCount());
		PostMessage(m_hParentWnd, WM_INITSTONE, MAKELONG(m_nCurrentGoId, eng->GetRemoteId()), send_stone);
	}

	if (inputMode==eIM_PLAY)
	{
		POINT ps;

		int xPos   =  lParam & 0xFFFF;
		int yPos   = (lParam >> 16) & 0xFFFF;
		//eStoneType stoneType=eng->IsCurrentOrderBlack()?eST_BLACK:eST_WHITE;//STONE_TYPE(eng->GetLastStone());
		Bool bIsOrderBlack=eng->IsCurrentOrderBlack();

		if (GetNearCoordinate(xPos, yPos, &ps)==False)
		{
			return 0;
		}

		// ２度打ち禁止（置石モードは？、相手がパスしたらどうする？）
		if (bIsOrderBlack!=(IAM_BLACK(eng->GetHeader()->condition)?True:False))
		{
			return 0;
		}

		int stoneType = IAM_BLACK(eng->GetHeader()->condition) ? eST_BLACK:eST_WHITE;

		m_branches.clear();
		aStoneList list;
		stone_t** stonez = eng->PutStone(MAKESTONE(stoneType, 0, ps.x, ps.y), list);

		if (stonez==0)
		{
			// 着手禁止の場合
			return 0;
		}

		int nWhite=0, nBlack=0;
		eng->GetDeadStone(nBlack, nWhite);

		SetDeadStone(nWhite, nBlack);

		// このタイミングで時間設定を行う。
		// この場合は、直接マウスで打っている。
		sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();


		ChangeTimer(0, eng->IsCurrentOrderBlack(), 0, info);
		/*if (eng->IsCurrentOrderBlack()==True)
		{
			info->black.startTime=cyiTime::CurrentmSec();
			info->white.usedTime=cyiTime::CurrentmSec(info->white.startTime);
			info->white.totalUsedTime+=info->white.usedTime;
			m_playerWhite->StopTimer(m_nCurrentGoId, info->white.totalUsedTime);
			m_playerBlack->StartTimer(m_nCurrentGoId);

			// ほったらかし対策
			//SetTimer(*this, eng->m_id|0x80000000, ,NULL);
		}
		else
		{
			info->white.startTime=cyiTime::CurrentmSec();
			info->black.usedTime=cyiTime::CurrentmSec(info->black.startTime);
			info->black.totalUsedTime+=info->black.usedTime;
			m_playerBlack->StopTimer(m_nCurrentGoId, info->black.totalUsedTime);
			m_playerWhite->StartTimer(m_nCurrentGoId);
			// ほったらかし対策
			//SetTimer(*this, eng->m_id|0x80000000, ,NULL);
		}*/

		::PlaySound((LPCSTR)m_soundMove, m_hInstance, SND_ASYNC|SND_MEMORY);

		if (eng->GetShowNo()!=-1)
		{
			// 碁石のみ
			Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
		}
		else
		{
			// 碁石のみ
			Update(eGL_STONE, stonez);
		}

		stone_t send_stone = xChangeStoneCoordinate(eng->GetLastStone(), eng->GetLineCount());

		if (m_nSendStoneId==0 ||
				GetSystem()==0)
		{
			PostMessage(m_hParentWnd, WM_PUTSTONE, MAKELONG(m_nCurrentGoId, eng->GetRemoteId()), send_stone);
		}
		else
		{
			GetSystem()->GetCommandCenter()->OnCommand(m_nSendStoneId ,this, send_stone, (unsigned int)eng);
		}

		if (inputMode==eIM_PLAY)
		{
			sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();

			info->last_put_time = cyiTime::CurrentmSec();
			info->hagasiCnt=0;
		}

#ifndef NEW_STYLE
		m_currentCtrl->EnableItem(eGCS_PASS ,False);
		m_currentCtrl->EnableItem(eGCS_GIVEUP,False);
		m_currentCtrl->EnableItem(eGCS_PAUSE,False);
#endif
	}

	return 0;
}


// ------------------------------------------------------
void cglsGoban::SetBrancheViewMode(eBranchViewMode mode/*=eBVM_NORMAL*/)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	if (eng)
	{
		xSET_BRANCH_VIEW_MODE(eng->m_flag, mode);
	}
}

// ---------------------------------------------------------------
int cglsGoban::SetBranch(cyiGoEngine* eng, stone_t* stone)
{
	if (!eng)
		return 0;

	int n;
	aStoneList branches;
	branches[0]=STONE_EOF;
	eBranchViewMode eBranchMode=xGET_BRANCH_VIEW_MODE(eng->m_flag);
	int nUntil = eBranchMode==eBVM_NORMAL?1:0;


	if (eBranchMode==eBVM_HIDE)
	{
		m_branches.clear();
		return 0;
	}

	if (stone)
		*stone=0;

	if (eng==0){
		m_branches.clear();
		return 0;
	}

	if ((n=eng->GetBranchList(branches))>nUntil)
	{
		m_branches.clear();
		for (int i=0; branches[i]!=STONE_EOF; i++)
		{
			eStoneType type = STONE_TYPE(branches[i]);


			if (type==eST_BLACK ||
					type==eST_WHITE)
			{
				int x = STONE_POS_X(branches[i]);
				int y = STONE_POS_Y(branches[i]);

				//if (stone && x==0 && y==0)
				//{
				//	stone_t laststone=eng->GetLastStone();
				//	*stone = MAKESTONE(type, 0, STONE_POS_X(laststone), STONE_POS_Y(laststone));
				//	continue;
				//}
				m_branches.push((yiAnyValue)branches[i]);
			}

			else if (stone && (type==eST_BPASS ||
					type==eST_WPASS))
			{
				stone_t laststone=eng->GetLastStone();
				*stone = MAKESTONE(type, 0, STONE_POS_X(laststone), STONE_POS_Y(laststone));
			}

		}
	}

	return n;
}

// ---------------------------------------------------------------
Bool cglsGoban::IsBranches(int x, int y)
{
	int n;
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	stone_t last_stone;
	aStoneList branches;
	branches[0]=STONE_EOF;

	if (eng==0){
		return 0;
	}

	last_stone=eng->GetLastStone();

	if ((n=eng->GetBranchList(branches))>0)
	{
		m_branches.clear();
		for (int i=0; branches[i]!=STONE_EOF; i++)
		{
			eStoneType type = STONE_TYPE(branches[i]);

			if (type==eST_BLACK ||
					type==eST_WHITE)
			{
				int _x = STONE_POS_X(branches[i]);
				int _y = STONE_POS_Y(branches[i]);

				if (x==_x &&
						y==_y)
				{
					return True;
				}
			}
			else if (type==eST_BPASS ||
							type==eST_WPASS)
			{

				int _x = STONE_POS_X(last_stone);
				int _y = STONE_POS_Y(last_stone);

				if (x==_x &&
						y==_y)
				{
					return True;
				}
			}

		}
	}

	return False;
}


// ---------------------------------------
LRESULT cglsGoban::OnLButtonDblClick(WPARAM wParam, LPARAM lp, Bool& handled)
{	
	return 0;
	if (this->m_bIsPreviewMode)
		return 0;

	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	eInputMode inputMode;
	Bool bIsControlDown = ((wParam&MK_CONTROL)!=0);
	
	//if (bIsControlDown==0)
	//	return 0;
	if (eng==0)
		return 0;

	if (eng->m_bIsLocked==True)
	{
		m_cursor->Hide();
		return 0;
	}

	inputMode = eng->GetInputMode();
/*
	if (inputMode==eIM_ENJOY ||
			inputMode==eIM_EDIT)
	{
		Goto(m_nCurrentGoId, 10, 0);
	}
*/
	if (inputMode!=eIM_STUDY)
	{
		return 0;
	}

	xShowEditBox(lp&0xFFFF, lp>>16);
	return 0;
}

// ---------------------------------------
LRESULT cglsGoban::OnRButtonDblClick(WPARAM wParam, LPARAM lp, Bool& handled)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	eInputMode inputMode;
	Bool bIsControlDown = ((wParam&MK_CONTROL)!=0);

	if (bIsControlDown==0)
		return 0;
	if (eng==0)
		return 0;

	inputMode = eng->GetInputMode();

	if (inputMode==eIM_ENJOY ||
			inputMode==eIM_EDIT)
	{
		Goto(m_nCurrentGoId, -10, 0);
		return 0;
	}



	return 0;
}

// ---------------------------------------
LRESULT cglsGoban::OnMouseWheel(WPARAM wp, LPARAM lp, BOOL& handled)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	eInputMode inputMode;
	int zDelta = GET_WHEEL_DELTA_WPARAM(wp); 
	int n=zDelta/120;
	aStoneList list;

	if (eng==0)
		return 0;
	inputMode = eng->GetInputMode();

	if (inputMode!=eIM_ENJOY &&
			inputMode!=eIM_EDIT)
	{
		return 0;

	}

	if ((zDelta/120)==-1)
	{
		Goto(m_nCurrentGoId, 1, list);
	}
	else if ((zDelta/120)==1)
	{
		Goto(m_nCurrentGoId, -1, list);
	}
	else if ((zDelta/120)<0)
	{
		Goto(m_nCurrentGoId, 10, list);
	}
	else
	{
		Goto(m_nCurrentGoId, -10, list);
	}

	return 0;
}

// ---------------------------------------------------------------
// eIM_CALCULATE時のみ呼び出される。
LRESULT cglsGoban::SelectStone(cyiGoEngine* eng, int x, int y)
{
	// 地合計算時のマップを取得
	stone_t** stonez = eng->GetStoneMap();

	if (stonez[x][y]!=eST_NONE)
	{
		stone_t stone = MAKESTONE(stonez[x][y], 0, x, y);
		stone_t** ret;
		int black, white;

		xTRACE("stone selected (%d, %d) %s", x, y, STONE_TYPE(stone)==eST_BLACK?"black":"white");
		if ((ret=eng->SetDeadStone(stone, &black, &white))!=0)
		{
			int b, w;
			eng->GetDeadStone(b, w);

			SetDeadStone(white+w,black+b);
			/*if (m_playerBlack)
				m_playerBlack->SetDeadStone(white+w);
			if (m_playerWhite)
				m_playerWhite->SetDeadStone(black+b);*/
			
			Update(eGL_CALCULATE, ret);
		}


		ClearCursor();
		return stone;
	}

	return 0;
}


#include "glsBaloon.h"

// ---------------------------------------------------------------
// 編集モード時は、マウスが押されたタイミングで描画する。
// 
LRESULT cglsGoban::OnLButtonDown(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (m_nCurrentGoId==0)
		return 0;

	if (m_bIsPreviewMode==True)
		return 0;

	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
	{
		m_cursor->Hide();
		return 0;
	}

	if (eng->m_bIsLocked==True)
	{
		m_cursor->Hide();
		return 0;
	}


	if (eng->GetInputMode()!=eIM_PLAY)
	{
		m_cursor->Hide();
	}

	if (eng->GetInputMode()==eIM_STUDY)
	{
		sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();
		if (info)
		{
			if (info->baloon)
			{
				if (IsWindowVisible(*info->baloon)==True)
				{
					info->baloon->Hide();
					info->baloon->SetText(0, 0);
					return 0;
				}
			}
		}
	}

	Bool bIsChanged=eng->IsChanged();

	eInputMode inputMode;
	eMouseMode mouseMode;
	POINT ps;
	int xPos, yPos;
	Bool bIsShiftDown = ((wParam&MK_SHIFT)!=0);
	Bool bIsControlDown = ((wParam&MK_CONTROL)!=0);

	sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();
	if (info)
	{
		if (info->dlg)
		{
			if (info->dlg->IsCreated()==True)
			{
				return 0;
			}
			delete info->dlg;
			info->dlg=0;
		}
	}


	// 現在、碁盤がない場合は編集モードとして新たな碁盤を開く
	//if (eng==0)
	//{
	//	unsigned int flag=0;
	//	unsigned int condition=	cyiGoEngine::DefaultCondition(flag);

	//	//SET_INIT_STONE(condition,7);
	//	m_nCurrentGoId = AddForEdit("", "", "no title", condition, flag);
	//	Update(eGL_ALL, m_GoEnginez[m_nCurrentGoId]->GetStoneMap());

	//	return 0;
	//}


	inputMode = eng->GetInputMode();
	mouseMode = eng->GetMouseMode();

	if (bIsShiftDown && 
			bIsControlDown)
	{
		if (inputMode==eIM_ENJOY ||
			inputMode==eIM_EDIT)
		{
			Goto(m_nCurrentGoId, MOVE_TOP, 0);
			return 0;
		}
	}



	// 地合計算終了モードならば、
	if (inputMode == eIM_CALCULATE_END)
	{
		//eng->SetInputMode(eIM_EDIT);

		//Update(eGL_ALL, eng->GetStoneMap());
		//ShowResult();
		return 0;
	}

	// コメント検索モード時は、石やラベルを置けない。
	// 何もしない。
	if (inputMode==eIM_FIND_COMMENT)
	{
		return 0;
	}

	// 現在のマウスポイントを座標に変換
	xPos   =  lParam & 0xFFFF;
	yPos   = (lParam >> 16) & 0xFFFF;

	if (GetNearCoordinate(xPos, yPos, &ps)==False)
	{
		return 0;
	}

	if (bIsShiftDown)
		goto ADD_STONE;



	// ラベル入力モード時はラベルを描画する。
	if (inputMode == eIM_LABEL)
	{
		cyiList* list = eng->GetLabel();

		if (list!=0 && list->count()!=0)
		{
			for (cyiNode* node=list->GetNodeTopFrom(0); node!=0; node=node->np)
			{
				label_t label = (label_t)node->ref;
				int x = label>>24;
				int y = (label>>16)&0xFF;

				// 既にラベルがある場合、削除する。
				if (x==ps.x && y==ps.y)
				{
					eng->RemoveLabel(ps.x, ps.y);
					SetLabelMode(m_eCurrentLabelType);
					Update(eGL_ALL, eng->GetStoneMap());

					if (bIsChanged!=eng->IsChanged())
					{
						m_tab->SetItemChanged(eng->m_id, eng->IsChanged());
					}
					return 0;
				}

			}
		}

		// ラベル入力
		eng->PutLabel(ps.x, ps.y, m_CurrentLabel);

		if (bIsChanged!=eng->IsChanged())
		{
			m_tab->SetItemChanged(eng->m_id, eng->IsChanged());
		}
		
		m_prev_cursor_x=-1;
		m_prev_cursor_y=-1;

		SetLabelMode(m_eCurrentLabelType);
		Update(eGL_LABEL);
		return 0;
	}

	// 置き石モード時
	if (inputMode == eIM_INITSTONE_MODE)
	{
		eng->InitStone(ps.x, ps.y);
		Update(eGL_ALL, eng->GetStoneMap());
		return 0;
	}

	// 石追加モード時
	if (inputMode==(int)eInputMode::eIM_EDIT_ADDSTONE)
	{
ADD_STONE:
		aStoneList branches;

		stone_t** stonez=eng->PutStone(MAKESTONE(eST_BLACK, 0, ps.x, ps.y), branches, True);
		// ここで、もし既存のツリーをたどるのならば、
		// ラベルがあるかどうか、コメントがあるかどうかをチェック


		cyiList* label=eng->GetLabel();

		// ラベルがある場合
		if (label!=0)
		{
			Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
			//return 0;
		}
		else
		{
			Update(eGL_STONE, stonez);
		}

		NotifyNodeChanged();
		if (bIsChanged!=eng->IsChanged())
		{
			m_tab->SetItemChanged(eng->m_id, eng->IsChanged());
		}


		return 0;
	}

	if (bIsControlDown)
	{
		if (inputMode==eIM_EDIT ||
				inputMode==eIM_ENJOY)
		{
			aStoneList list;
			GotoNextBranch(m_nCurrentGoId, list);
			return 0;
		}
	}


	// 通常の編集モード
	if (inputMode == (int)eInputMode::eIM_EDIT ||
			inputMode == (int)eInputMode::eIM_STUDY)
	{
		if (mouseMode == eMouseMode::eMM_RBUTTON_RETURN)
		{
			stone_t laststone = eng->GetLastStone();
			eStoneType type=/*eng->GetNextStone()*/eng->IsCurrentOrderBlack()==True ? eST_BLACK:eST_WHITE;


			//if (eng->GetReference())
			//{
			//}

			// 検討モード時
			if (inputMode == eIM_STUDY)
			{
				unsigned int con = eng->GetHeader()->condition;

				stone_t send_stone = xChangeStoneCoordinate(MAKESTONE(type, 0, ps.x, ps.y), eng->GetLineCount());

				if (m_nSendStoneId==0 ||
						GetSystem()==0)
				{
					PostMessage(m_hParentWnd, WM_PUTSTONE, MAKELONG(m_nCurrentGoId, eng->GetRemoteId()), send_stone);
				}
				else
				{
					GetSystem()->GetCommandCenter()->OnCommand(m_nSendStoneId ,this, send_stone, (unsigned int)eng);
				}

				if (IAM_BLACK(con)==False)
				{
					return 0;
				}

				sGoHeader* header = eng->GetHeader();

				if (IAM_BLACK(header->condition)==False)
				{
					return 0;
				}
			}

			xDeleteReference(m_nCurrentGoId, True);//False);

			m_branches.clear();	
			aStoneList branches;
			stone_t** stonez=eng->PutStone(MAKESTONE(type, 0, ps.x, ps.y), branches, False, False);//m_detail.bIsJosekiEditorMode==True?True:False);


			// 着手禁止と見なす。
			if (stonez==0)
			{
				// この場合は、着手禁止とか。。。？
				return 0;
			}



#ifdef _TIMER_DEBUG_
			
			if (eng->IsCurrentOrderBlack()==True)
			{
				g_black.startTime=cyiTime::CurrentmSec();
				g_white.usedTime=cyiTime::CurrentmSec(g_white.startTime);
				g_white.totalUsedTime+=g_white.usedTime;
				m_playerWhite->StopTimer(m_nCurrentGoId, g_white.totalUsedTime);
				m_playerBlack->StartTimer(m_nCurrentGoId);
			}
			else
			{
				g_white.startTime=cyiTime::CurrentmSec();
				g_black.usedTime=cyiTime::CurrentmSec(g_black.startTime);
				g_black.totalUsedTime+=g_black.usedTime;
				m_playerBlack->StopTimer(m_nCurrentGoId, g_black.totalUsedTime);
				m_playerWhite->StartTimer(m_nCurrentGoId);
			}
#endif


			::PlaySound((LPCSTR)m_soundMove, m_hInstance, SND_ASYNC|SND_MEMORY);

			cyiList* reference;
			int cnt=0;
			if (reference=eng->GetReference())
			{

				int nId=m_nCurrentGoId;
				for (cyiNode* node=reference->GetNodeTopFrom(0); node; node=node->np)
				{
					xAddReference(0, (cyiTreeNode*)node->ref, cnt++, nId);
				}
			}

			cyiList hide_branches;
			int n=eng->GetHideBranches(hide_branches);

			if (n!=0)
			{
				for (cyiNode* node=hide_branches.GetNodeTopFrom(); node; node=node->np)
				{
					cyiTreeNode*t = (cyiTreeNode*)node->ref;
					xAddReference(0, 0, cnt++, m_nCurrentGoId, t);
				}
			}
			xUpdateTab();
			//m_tab->Update(eGUM_FORCED);

			int nBranchDetermin = xGET_BRANCH_VIEW_MODE(eng->m_flag)==eBVM_SHOW_ONE_NODE?0:1;
			int nBrances=SetBranch(eng);
			// 枝分かれしている場合の処理
			Bool bIsReqFree=False;

			cyiList* label=eng->GetLabel(&bIsReqFree);

			// ラベルがある場合
			if (label!=0 || nBrances >nBranchDetermin || eng->GetShowNo()!=-1)
			{
				Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
				//return 0;
			}
			else
				Update(eGL_STONE, stonez);

			if (label && bIsReqFree)
				eng->DeleteLabel(label);
				//delete label;

			// コメントの更新
			NotifyNodeChanged();


			::PostMessage(m_hParentWnd, WM_PUT_STONED, m_nCurrentGoId, (LPARAM)eng->GetCurrentNode());

			if (eng->GetInputMode()==eIM_EDIT &&
					bIsChanged!=eng->IsChanged())
			{
				m_tab->SetItemChanged(eng->m_id, eng->IsChanged());
			}
		}
		// 廃止予定
		else if (mouseMode == eMouseMode::eMM_RBUTTON_WHITE)
		{
			stone_t** stonez=eng->PutStone(MAKESTONE(eST_BLACK, 0, ps.x, ps.y), 0);
			// ここで、もし既存のツリーをたどるのならば、
			// ラベルがあるかどうか、コメントがあるかどうかをチェック



			cyiList* label=eng->GetLabel();

			// ラベルがある場合
			if (label!=0)
			{
				Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
				return 0;
			}

			Update(eGL_STONE, stonez);

			// これは検討要
			//if (eng->GetComment()!=0)
			//{
			NotifyNodeChanged();

			//}
		}
		// 廃止予定
		else if (eng->GetCurrentState()!=eCS_PLAY)
		{
			eng->InitStone(ps.x, ps.y);
			Update(eGL_STONE, eng->GetStoneMap());
			return 0;
		}
	}
	// 観戦などのモード
	else if (inputMode==eIM_ENJOY ||
					inputMode==eIM_WATCH)
	{
		aStoneList list;
		aStoneList branches;

		int n = eng->GetBranchList(branches);

		if (n!=0)
		{
			int index=-1;

			for (int i=0; i<n; i++)
			{
				stone_t stone = branches[i];

				int x = STONE_POS_X(stone);
				int y = STONE_POS_Y(stone);

				if (x==ps.x &&
						y==ps.y)
				{
					index=i;//=True;
					break;
				}
			}

			if (index!=-1)
			{
				xDeleteReference(m_nCurrentGoId, True);//False);

				m_branches.clear();	
				stone_t** stonez = eng->Play(index);//(index ,0);//eng->Goto(1, list);

				::PlaySound((LPCSTR)m_soundMove, m_hInstance, SND_ASYNC|SND_MEMORY);

				cyiList* reference;
				int cnt=0;
				if (reference=eng->GetReference())
				{

					int nId=m_nCurrentGoId;
					for (cyiNode* node=reference->GetNodeTopFrom(0); node; node=node->np)
					{
						xAddReference(0, (cyiTreeNode*)node->ref, cnt++, nId);
					}
				}

				cyiList hide_branches;
				int n=eng->GetHideBranches(hide_branches);

				if (n!=0)
				{
					for (cyiNode* node=hide_branches.GetNodeTopFrom(); node; node=node->np)
					{
						cyiTreeNode*t = (cyiTreeNode*)node->ref;
						xAddReference(0, 0, cnt++, m_nCurrentGoId, t);
					}
				}
				xUpdateTab();
				//m_tab->Update(eGUM_FORCED);

				int nBranchDetermin = xGET_BRANCH_VIEW_MODE(eng->m_flag)==eBVM_SHOW_ONE_NODE?0:1;
				int nBrances=SetBranch(eng);
				// 枝分かれしている場合の処理
				Bool bIsReqFree=False;

				cyiList* label=eng->GetLabel(&bIsReqFree);

				// ラベルがある場合
				if (label!=0 || nBrances >nBranchDetermin || eng->GetShowNo()!=-1)
				{
					Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
					//return 0;
				}
				else
					Update(eGL_STONE, stonez);

				if (label && bIsReqFree)
					eng->DeleteLabel(label);
			/*
				cyiList* label = eng->GetLabel();

				// ラベルがある場合
				if (label!=0 || eng->GetShowNo()!=-1)
				{
					Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
				}
				else
				{
					Update(eGL_STONE, stonez);
				}*/
				NotifyNodeChanged();
			}
		}

	}
	// 地合い計算時
	else if (inputMode == (int)eInputMode::eIM_CALCULATE)
	{
		stone_t stone = SelectStone(eng, ps.x, ps.y);

		if (stone!=0 &&
			eng->GetExtraData()!=0)
		{

			stone_t send_stone = xChangeStoneCoordinate(stone, eng->GetLineCount());
			if (m_nSendStoneId==0 ||
					GetSystem()==0)
			{
				PostMessage(m_hParentWnd, WM_PUTSTONE, MAKELONG(m_nCurrentGoId, eng->GetRemoteId()), send_stone);
			}
			else
			{
				GetSystem()->GetCommandCenter()->OnCommand(m_nSendStoneId ,this, send_stone, (unsigned int)eng);
			}
			//PostMessage(m_hParentWnd, WM_PUTSTONE, MAKELONG(m_nCurrentGoId, eng->GetRemoteId()), send_stone);
		}
	}

	return 0;
}

// ---------------------------------------------------------------
// 右クリック時の挙動
// 
LRESULT cglsGoban::OnRButtonDown(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	eInputMode inputMode;
	eMouseMode mouseMode;
	POINT ps;
	int xPos   =  lParam & 0xFFFF;
	int yPos   = (lParam >> 16) & 0xFFFF;

	Bool bIsShiftDown = ((wParam&MK_SHIFT)!=0);
	Bool bIsControlDown = ((wParam&MK_CONTROL)!=0);

	if (eng==0)
		return 0;

	inputMode = eng->GetInputMode();
	mouseMode = eng->GetMouseMode();



	if (inputMode==eIM_PLAY)
	{
		sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();

		if (eng->IsCurrentOrderMine()==True)
		{
			if (m_editBox != 0 &&
					IsWindowVisible(*m_editBox)==True)
			{
				m_editBox->Hide();
				return 0;
			}
			xShowEditBox(lParam&0xFFFF, lParam>>16);
			return 0;
		}

		if (info->hagasiCnt<3)
		{
			unsigned int time = cyiTime::CurrentmSec(info->last_put_time);

			info->last_put_time = 0;

			if (time<1000)
			{
				//Goto(m_nCurrentGoId, -1, 0, 0);
				//DeleteNode(0);
				info->hagasiCnt++;
				GetSystem()->GetCommandCenter()->OnCommand(eGCID_HAGASHI, this, 0, 0);
				return 0;
			}
		}
	}

	if (inputMode==eIM_STUDY ||
		inputMode==eIM_PLAY)
	{
		if (m_editBox != 0 &&
				IsWindowVisible(*m_editBox)==True)
		{
			m_editBox->Hide();
			return 0;
		}

		xShowEditBox(lParam&0xFFFF, lParam>>16);
		return 0;
	}


	if (bIsShiftDown && 
			bIsControlDown)
	{
		if (inputMode==eIM_ENJOY ||
			inputMode==eIM_EDIT)
		{
			Goto(m_nCurrentGoId, MOVE_END, 0);
			return 0;
		}
	}

	if (bIsControlDown)
	{
		if (inputMode==eIM_EDIT ||
				inputMode==eIM_ENJOY)
		{
			aStoneList list;
			GotoPrevBranch(m_nCurrentGoId, list);
			return 0;
		}
	}


	// ラベル入力モードならば、最後のラベルを削除する。
	if (inputMode == eIM_LABEL)
	{
		cyiList* list = eng->GetLabel();

		if (list==0 ||
				list->count()==0)
		{
			return 0;
		}

		cyiNode* node = list->GetNodeTailFrom(0);
		list->remove(node);

		m_prev_cursor_x=-1;
		m_prev_cursor_y=-1;

		SetLabelMode(m_eCurrentLabelType);
		Update(eGL_ALL, eng->GetStoneMap());
		return 0;
	}



	if (GetNearCoordinate(xPos, yPos, &ps)==False)
	{
		return 0;
	}

	if (bIsShiftDown)
		goto ADD_STONE;


	// AB, AWモード時
	if (inputMode==eIM_EDIT_ADDSTONE)
	{
ADD_STONE:

		aStoneList branches;
		stone_t** stonez=eng->PutStone(MAKESTONE(eST_WHITE, 0, ps.x, ps.y), branches, True);
		// ここで、もし既存のツリーをたどるのならば、
		// ラベルがあるかどうか、コメントがあるかどうかをチェック



		cyiList* label=eng->GetLabel();

		// ラベルがある場合
		if (label!=0)
		{
			Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
			//return 0;
		}
		else
		{
			Update(eGL_STONE, stonez);
		}

		NotifyNodeChanged();
	
		return 0;
	}

	if (inputMode == (int)eInputMode::eIM_EDIT ||
		inputMode == eInputMode::eIM_ENJOY)
	{
		if (mouseMode == eMouseMode::eMM_RBUTTON_RETURN)
		{
			aStoneList list;
			//stone_t** stonez = eng->Goto(-1, list);

			//if (stonez==0)
			//	return 0;


			//if (list[0]!=STONE_EOF)
			//{
			//	m_branches.clear();
			//	for (int i=0; list[i]!=STONE_EOF; i++)
			//	{
			//		m_branches.push((yiAnyValue)list[i]);
			//	}
			//}

			//Update(eGL_ALL, stonez);
			//if (xIS_BIT(eng->m_flag, eGEF_NOW_SEARCH_RESULT)==False)
			//	NotifyNodeChanged();
			Goto(0, -1, list);
		}
		else if (mouseMode==eMouseMode::eMM_RBUTTON_WHITE)
		{
			POINT ps;

			int xPos   =  lParam & 0xFFFF;
			int yPos   = (lParam >> 16) & 0xFFFF;

			if (GetNearCoordinate(xPos, yPos, &ps)==False)
			{
				return 0;
			}
			stone_t stone = MAKESTONE(eST_WHITE, 0, ps.x, ps.y);
			stone_t** stonez = eng->PutStone(stone, 0);

			if (stonez==0)
				return 0;


			cyiList* label=eng->GetLabel();

			// ラベルがある場合
			if (label!=0)
			{
				Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
			}
			else
			{
				Update(eGL_STONE, stonez);
			}
			// これは検討要
			NotifyNodeChanged();
	
		}
	}

	return 0;
}
#include "glsMessageBox.h"
// ---------------------------------------------------------------
LRESULT cglsGoban::OnNotifyFromTab(int id, int subId, int code)
{
	// タブコントロール
	if (id==xCHILD_ID_TAB)
	{

		if (code==TCN_SELCHANGE)
		{
			if (subId==256)
			{
				// new.
				cglsSystem* system=GetSystem();
				if (system==0)
					return 0;
				if (m_nAddNewId)
					system->GetCommandCenter()->OnCommand(m_nAddNewId, this);
#ifndef NEW_STYLE
				if (m_myView)
				{
					m_myView->Hide();
				}
#endif
				return 0;
			}
#ifndef NEW_STYLE

			if (subId==257)
			{
				if (m_myView)
				{
					xResetGobanControl(0);
				}
				return 0;
			}
			else
			{
				if (m_myView)
				{
					m_myView->Hide();
					//m_myFriendView->Hide();
				}
			}
#endif
			if (subId==m_nCurrentGoId)
			{
				// 追加時のイベントはここを通る。
				//xUpdateTab();
#ifndef NEW_STYLE
				if (m_currentCtrl==0 ||
						IsWindowVisible(*m_currentCtrl)==False)
				{
					xResetGobanControl(GetCurrentGoEngine());
				}
#endif
				return 0;
			}

			ChangeCurrentGoban(subId, False);
			xUpdateTab();

			//m_nCurrentGoId=subId;
			//Update(eGL_ALL);
			return 0;
		}
		else if (code==TCN_REMOVE_CONFIRM)
		{
			int resx=0;
			char buffer[64];
			wchar_t bufferw[64];
			cyctrlGobanResource& res = cyctrlGobanResource::GetInstance();
			eGoContentsMode eNodeType = xGET_GOBAN_MODE(m_GoEnginez[subId]->m_flag);
			Bool bIsChanged = IsChanged();

			if (bIsChanged==True &&
					eNodeType==eGCM_NORMAL)
			{
				wsprintf(buffer, res.GetStringA(eGTRID_SAVE_CONFIRM), m_GoEnginez[subId]->GetTitle());

				cglsMessageBox msg;
				resx = msg.Show(this, yiToUnicode(bufferw, buffer, 0), res.GetStringW(eGTRID_WARNING),MB_OKCANCEL, &Font(L"Meiryo UI", m_scale*20.0));
				//resx = ::MessageBox(m_hWnd, buffer, res.GetStringA(eGTRID_INFORMATION), MB_YESNO|MB_ICONWARNING);

				if (resx==IDOK)
				{
					// TODO: 保存処理
					cglsSystem* system = GetSystem();

					if (system)
					{
						system->GetCommandCenter()->OnCommand(eGCID_CURRENT_SAVE, this, subId);
					}
				}

				resx=IDOK;
			}
			else if (xGET_GOBAN_MODE(m_GoEnginez[subId]->m_flag)==eGCM_REFERENCE)
			{
				wsprintf(buffer, res.GetStringA(eGTRID_DELETE_CONFIRM), m_GoEnginez[subId]->GetTitle());

				cglsMessageBox msg;
				resx = msg.Show(this, yiToUnicode(bufferw, buffer, 0), res.GetStringW(eGTRID_WARNING),MB_OKCANCEL, &Font(L"Meiryo UI", m_scale*20.0));

				//resx = ::MessageBox(m_hWnd, buffer, res.GetStringA(eGTRID_WARNING), MB_YESNO|MB_ICONWARNING);
			}
			else //if (m_GoEnginez[subId]->m_eNodeType==eGoNodeType::eGNT_PREVIEW)
			{
				resx=IDOK;
				//wsprintf(buffer, res.GetStringA(eGTRID_CLOSE_CONFIRM), m_GoEnginez[subId]->GetTitle());
			}


			if (resx ==IDOK)
			{
				return 0;
			}
			// TODO: Save

			return 1;
		}
		else if (code==TCN_REMOVED)
		{
			cyiGoEngine* eng = m_GoEnginez[subId];

			if (eng)
			{
				//if (m_preview && IsWindowVisible(*m_preview))
				//	SendMessage(*m_preview, WM_NODE_CHANGED, 1, (LPARAM)0/*eng->GetCurrentNode()*/);
				NotifyNodeChanged(True, True);

				if (xGET_GOBAN_MODE(eng->m_flag)==eGCM_REFERENCE)
				{
					cyiTreeNode* top = eng->GetTopNode();
					cSgfStoneInfo* topInfo = (cSgfStoneInfo*)top->GetData();

					if (topInfo->referenceFrom)
					{
						cSgfStoneInfo* info = (cSgfStoneInfo*)topInfo->referenceFrom->GetData();

						for (cyiNode* node=info->reference->GetNodeTopFrom(); node; node=node->np)
						{
							cyiTreeNode* t = (cyiTreeNode*)node->ref;
							if (t==top)
							{
								info->reference->remove(node);
								break;
							}
						}
					}


					eng->DeleteNode(top);
				}
				xUpdateTab();
				DeleteGoban(subId, True);
			}
			return 0;
		}
	}

	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnShowMenu(HWND hWnd, int code)
{
	if (m_detail.menu)
	{
		POINT pt;
		GetCursorPos(&pt);
		m_detail.menu->Show(pt.x, pt.y);
	}
	return 0;
}


// ---------------------------------------------------------------
LRESULT cglsGoban::OnClickTop(HWND hWnd, int code)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	if (eng==0)
		return 0;
	aStoneList list;
	Goto(m_nCurrentGoId, MOVE_TOP, list);
	NotifyNodeChanged();
	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnClickPP(HWND hWnd, int code)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	if (eng==0)
		return 0;
	aStoneList list;
	Goto(m_nCurrentGoId, -m_nJumpCount, list);

	NotifyNodeChanged();
	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnClickPrev(HWND hWnd, int code)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	if (eng==0)
		return 0;
	aStoneList list;
	Goto(m_nCurrentGoId, -1, list);

	NotifyNodeChanged();
	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnClickNext(HWND hWnd, int code)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	if (eng==0)
		return 0;
	aStoneList list;
	Goto(m_nCurrentGoId, 1, list);

	NotifyNodeChanged();
	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnClickNN(HWND hWnd, int code)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	if (eng==0)
		return 0;
	aStoneList list;
	Goto(m_nCurrentGoId, m_nJumpCount, list);

	NotifyNodeChanged();
	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnClickEnd(HWND hWnd, int code)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	if (eng==0)
		return 0;

	aStoneList list;
	Goto(m_nCurrentGoId, MOVE_END, list);

	NotifyNodeChanged();
	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnResultImageClicked(HWND hWnd, int code)
{
	cGlassInterface* child = GetChild(xCHILD_ID_RESULT);

	if (child)
		child->Hide();
	return 0;
}


// ---------------------------------------------------------------
LRESULT cglsGoban::OnTitleChanged(HWND hWnd, int code)
{
#ifndef NEW_STYLE
	if (code==EN_CHANGE)
	{
		cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
		char szBuffer[128];
		if (eng==0)
			return 0;

		SetChanged(0, True);

		yiToMultiByteCode(szBuffer, (const wchar_t*)m_titleDlg->GetTitle(), 0);
		eng->SetTitle(szBuffer);

		m_tab->SetItem(m_nCurrentGoId, -1, m_titleDlg->GetTitle(), True);
		m_tab->Update(eGUM_FORCED);

	}
#endif
	return 0;
}

#ifndef NEW_STYLE

#define xCHILD_ID_BTN_MYPAGE (256+16)
#define xCHILD_ID_BTN_MYFRIEND (256+17)
#define xCHILD_ID_BTN_MYGROUP (256+18)
#define xCHILD_ID_BTN_GROUP_DETAIL (256+19)
#define xCHILD_ID_BTN_DETAIL (256+20)

// ---------------------------------------------------------------
LRESULT cglsGoban::OnChangeView(WPARAM wParam, LPARAM lParam)
{
	if (m_nSelectedView!=wParam)
	{
		m_nSelectedView=wParam;//m_nSelect
		m_currentView->Hide();

		if (wParam==xCHILD_ID_BTN_MYPAGE)
			m_currentView=m_myView;
		else if (wParam==xCHILD_ID_BTN_MYFRIEND)
		{
			if (m_myFriendView==0)
			{
				float fFontSize = 24.0*m_scale;
				fFontSize=fFontSize>16.0?16.0:fFontSize;

				Font font(L"Meiryo UI", fFontSize);

				m_myFriendView = new cglsMyFriendView();
				m_myFriendView->Create(this, 0, 0, m_w, m_w, m_userMgr, &font);
			}
			m_currentView=m_myFriendView;
		}
		else
			m_currentView=m_myView;


		
		xResetGobanControl(0);
	}

	return 0;
}
#endif

// ---------------------------------------------------------------
LRESULT cglsGoban::OnNotHandledCommand(WPARAM wp, LPARAM lp)
{
	int id = (wp&0xFFFF);

	if (id==xCHILD_EDITBOX)
	{
		if ((wp>>16)==0)
		{
			wchar_t buffer[256];
			m_editBox->GetText(buffer, 256);
			int x=m_editBox->m_x;
			int y=m_editBox->m_y;
			POINT pt;
			this->GetNearCoordinate(m_ptEditBox.x, m_ptEditBox.y, &pt);
			if (buffer[0]!=0)
			{
				GetSystem()->GetCommandCenter()->OnCommand(eGCID_STONE_MESSAGE, this, m_nCurrentGoId|(pt.y<<16)|(pt.x<<8), (LongPtr)buffer);
			}
			m_editBox->Hide();
		}

		return 0;
	}

	if (id>xID_MESSAGEBOX &&
		id<(xID_MESSAGEBOX+MAX_GOBAN_COUNT))
	{
		// メッセージボックス通知
		int code = wp>>16;

		if (lp!=0)
		{
			GetSystem()->GetCommandCenter()->OnCommand(lp, this, wp);
		}
	}
	else if (id>xID_BALOON &&
				id<(xID_BALOON+MAX_GOBAN_COUNT))
	{
		goid_t goid=(id-xID_BALOON);
		sGoPlayInfo* info = (sGoPlayInfo*)GetExtraData(goid);

		// Mouse Enter
		if ((wp>>16)==1)
		{
			eInputMode mode = GetCurrentGoEngine()->GetInputMode();

			if (mode==eIM_PLAY)
			{
				info->baloon->Hide();
				info->baloon->SetText(0, False);
			}

			return 0;
		}

		if (info->baloon)
		{
			wchar_t text[256];
			int n = info->baloon->GetText(text, 256);
			int x = info->baloon->m_x;
			int y = info->baloon->m_y;
			POINT pt;
			info->baloon->GetBaloonPos(pt);

			info->baloon->Hide();
			info->baloon->SetText(0, False);
			xShowEditBox(pt.x, pt.y);
		}
	}

	return 0;
}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnHeaderChanged(WPARAM wp, LPARAM lp, Bool& bIsHandled)
{
	SetChanged(0, True);
	return 0;
}
