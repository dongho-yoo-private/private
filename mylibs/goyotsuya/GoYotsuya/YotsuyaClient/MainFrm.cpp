// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "YotsuyaClient.h"
#include "MainFrm.h"


//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_REQUEST_PLAY, &CMainFrame::OnRequestPlay)
	ON_MESSAGE(WM_NOTIFY_FROM_NETWORK, &CMainFrame::OnNotifyFromNetwork)
	ON_MESSAGE(WM_GOBAN_ADDED, &CMainFrame::OnGobanAdded)
	ON_MESSAGE(WM_PUTSTONE, &CMainFrame::OnPutStone)
	ON_MESSAGE(WM_GOBAN_CHANGED, &CMainFrame::OnGobanChanged)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_CMD_GIVEUP, &CMainFrame::OnCmdGiveup)
	ON_COMMAND(ID_CMD_PLAY_CANCEL, &CMainFrame::OnCmdPlayCancel)
	ON_COMMAND(ID_CMD_QUIT_PLAY, &CMainFrame::OnCmdQuitPlay)
	ON_COMMAND(ID_CMD_CALCULATE, &CMainFrame::OnCmdCalculate)
	ON_COMMAND(ID_CMD_CALCULATE_END, &CMainFrame::OnCmdCalculateEnd)
	ON_COMMAND(ID_CMD_WAIT, &CMainFrame::OnCmdWait)
	ON_COMMAND(ID_CMD_ABORT, &CMainFrame::OnCmdAbort)
	ON_COMMAND(ID_CMD_PLAY_END, &CMainFrame::OnCmdPlayEnd)
	ON_COMMAND(ID_CMD_NEXT, &CMainFrame::OnCmdNext)
	ON_COMMAND(ID_CMD_TOP, &CMainFrame::OnCmdTop)
	ON_COMMAND(ID_CMD_PREV, &CMainFrame::OnCmdPrev)
	ON_COMMAND(ID_APP_EXIT, &CMainFrame::OnAppExit)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static Bool xOnDrawVistaStyleButton(int id, Graphics* g, Font* fnt, sControlx* x, eControlxEvent xevent);

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
: m_RequestPlayDlg(this)
{
	// TODO: メンバ初期化コードをここに追加してください。
}

CMainFrame::~CMainFrame()
{
}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
	//	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("ツール バーの作成に失敗しました。\n");
	//	return -1;      // 作成できませんでした。
	//}

	//if (!m_wndStatusBar.Create(this) ||
	//	!m_wndStatusBar.SetIndicators(indicators,
	//	  sizeof(indicators)/sizeof(UINT)))
	//{
	//	TRACE0("ステータス バーの作成に失敗しました。\n");
	//	return -1;      // 作成できませんでした。
	//}

	// TODO: ツール バーをドッキング可能にしない場合は、これらの 3 行を削除してください。
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	CreateYotsuya();


	return 0;
}

#include "yiEncoder.h"
// -------------------------------------
BOOL CMainFrame::CreateYotsuya()
{
	SetWindowLong(*this, GWL_STYLE, ::GetWindowLong(*this, GWL_STYLE)|WS_CLIPCHILDREN);

	m_network = (cypnGoNetwork*)cyPrivateNetwork::GetInstance();
#ifndef xNET_OFF
	m_LoginDlg.m_bFoundUser=FALSE;
	if (m_LoginDlg.DoModal()==TRUE)
	{
		char buffer[256];
		unsigned int id=0;
		memset(buffer, 0, sizeof(buffer));

		char* pId = (char*)*m_LoginDlg.m_txtId;

		//if (m_LoginDlg.m_bIsNormalLogin==TRUE)
		//{
		//	strncpy(buffer, m_LoginDlg.m_txtId, 16);
		//	// LoadUserInfo.

		//	// make id.
		//}
		//else
		//{
		//	strncpy(&buffer[1], m_LoginDlg.m_txtId, 16);
		//	buffer[0]=(char)0xFF;
		//}

		strncpy(buffer, m_LoginDlg.m_szName, 16);


		if (m_network->Start(*this, WM_NOTIFY_FROM_NETWORK, ::yihtol("yotuya.jp", 0), 10003, cyiEncoder::GetYotsuyaSecurityCode("UserServerYotsuya"), id, buffer, m_LoginDlg.m_szPassword, 0)==FALSE)
		{
			return FALSE;
		}

		sypnUserInfo* info = (sypnUserInfo*)m_network->GetMyInfo();
		
		if (info->id==0)
		{
			wsprintf(buffer, "%d部屋 %sさん", info->network.index, &info->szUserName[1]);
		}
		else
		{
			wsprintf(buffer, "%sさんの部屋", info->szUserName);
		}
		SetWindowTextA(buffer);
	}
	else
	{
		return FALSE;
	}

#endif



	RECT rt;
	GetClientRect(&rt);
	m_goban = new cyctrlGoban();
	m_goban->Create(rt.left, rt.top, rt.right-rt.left, rt.bottom-rt.top, 19, *this);

	cyctrlGoban::SetResource(eRTX_IMAGE, cyiResource::GetInstance(eRTX_IMAGE));
	cyctrlGoban::SetResource(eRTX_IMAGE_CACHED, cyiResource::GetInstance(eRTX_IMAGE_CACHED));
	cyctrlGoban::SetResource(eRTX_TEMPORARY, cyiResource::GetInstance(eRTX_TEMPORARY));

	CreatePlayControl();
	CreateEditControl();
	//m_ctrlEdit.Show();
	m_currentCtrl=0;//&m_ctrlEdit;
	//AdjustLayout(m_currentCtrl);

	return TRUE;
}


// -------------------------------------
void CMainFrame::OnRequestPlay()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	stone_t condition=0;


	SET_HANDICAP(condition);
	SET_INIT_STONE(condition, 5);
	SET_KOMI(condition, 0);
	SET_HALF_SCORE(condition);
	SET_REVERSE_KOMI(condition);
	SET_PLAY_TIME(condition, 5*18);
	SET_SECOND(condition, 1);
	SET_SECOND_COUNT(condition, 5);

	m_RequestPlayDlg.bIsRecvMode=FALSE;
	m_RequestPlayDlg.SetDefaultValue(condition, 0);
	// ここで、友人リストがあれば、追加をする。
	//
				//MessageBoxA("相手のIDやパスワードを確認してください。", "対局申し込み", MB_OK|MB_ICONINFORMATION);

	//DoModal
	//m_currentCtrl->ShowWindow(SW_HIDE);
	int res = m_RequestPlayDlg.DoModal();
	//m_currentCtrl->ShowWindow(SW_SHOW);
	//m_currentCtrl->UpdateWindow();
	
	if (res!=0)
	{
		Handle hUser;


		int index;
		if (m_RequestPlayDlg.m_InputUserName==0)
		{
			if ((hUser=m_network->FindUser(m_RequestPlayDlg.m_strSelectedUser, 0))!=0)
			{
				m_network->SendRequestPlay(hUser, m_RequestPlayDlg.m_condition1, m_RequestPlayDlg.m_condition2);
				return ;
			}
			else if ((index=m_network->Connect(0, m_RequestPlayDlg.m_strSelectedUser, 0, m_RequestPlayDlg.m_strRoomPassword))==-1)
			{
				// メッセージボックス。
				MessageBox("相手のIDやパスワードを確認してください。", "対局申し込み", MB_OK|MB_ICONINFORMATION);
				return ;
			}
			
			if(m_network->SendRequestPlay(m_network->IndexToUser(index), m_RequestPlayDlg.m_condition1, m_RequestPlayDlg.m_condition2)==False)
			{
				// メッセージボックス。
				MessageBox("対局申し込みエラー", "対局申し込み", MB_OK|MB_ICONINFORMATION);
				return;
			}
			return ;
		}

		if ((index=m_network->GetUserIndex(0, 0, m_RequestPlayDlg.m_nRoomNo))==-1)
		{
			if ((index=m_network->Connect(0, 0, m_RequestPlayDlg.m_nRoomNo, m_RequestPlayDlg.m_strRoomPassword))!=-1)
			{
				m_network->SendRequestPlay(m_network->IndexToUser(index), m_RequestPlayDlg.m_condition1, m_RequestPlayDlg.m_condition2);
			}
			else
			{
				// メッセージボックス。
				//AfxMessageBox("相手のIDやパスワードを確認してください。",  MB_OK|MB_ICONINFORMATION);
				MessageBoxA("相手のIDやパスワードを確認してください。", "対局申し込み", MB_OK|MB_ICONINFORMATION);
				return ;
			}
		}
		else
		{
			m_network->SendRequestPlay(m_network->IndexToUser(index), m_RequestPlayDlg.m_condition1, m_RequestPlayDlg.m_condition2);

		}
	}
}

// -------------------------------------
LRESULT CMainFrame::OnNotifyFromNetwork(WPARAM wp, LPARAM lp)
{
	switch(wp)
	{
		case eNRMSG_CONNECTION_CLOSED:
		{
			MessageBox("相手と接続が切れました。", "お知らせ", MB_OK);
			return 0;
		}
		case eGNPMSG_REQ_PLAY:
		{
			int* param = (int*)lp;
			HandleUser hUser = (HandleUser)param[0];

			m_RequestPlayDlg.SetDefaultValue(param[1], param[2], cypnGoNetwork::GetUserName(hUser));
			m_RequestPlayDlg.bIsRecvMode=TRUE;

			//m_currentCtrl->ShowWindow(SW_HIDE);
			int res = m_RequestPlayDlg.DoModal();
			//m_currentCtrl->ShowWindow(SW_SHOW);
			//m_currentCtrl->UpdateWindow();

			if (res==0)
			{
				m_network->SendReplyPlay(hUser, 0xFFFFFFFF, 0);
				break;
			}

			if (res!=0)
			{
				if (m_RequestPlayDlg.m_bChangedCondition==TRUE)
				{
					m_network->SendReplyPlay(hUser, m_RequestPlayDlg.m_condition1, m_RequestPlayDlg.m_condition2);
				}
				else
				{
					char title[128];
					int id = m_network->SendRequestPlayStart(hUser, &m_RequestPlayDlg.m_condition1, m_RequestPlayDlg.m_condition2);
					wsprintf(title, "%s vs %s", m_network->GetMyInfo()->szUserName, cypnGoNetwork::GetUserNameA(hUser));
					m_goban->Add(cyctrlGoban::EIM_PLAY, cyctrlGoban::EMM_DEFAULT, title, m_RequestPlayDlg.m_condition1, *this, id);
				}
			}
			break;
		}
		case eGNPMSG_REPLY_PLAY:
		{
			int* param = (int*)lp;
			HandleUser hUser = (HandleUser)param[0];

			if (param[1]==-1)
			{
				// Message Box.
				char message[256];
				wsprintf(message, "%sさんから対局の申し込みが断られました", cypnGoNetwork::GetUserName(hUser));
				MessageBox(message, "対局申し込み", MB_OK|MB_ICONINFORMATION);
				break;
			}

			m_RequestPlayDlg.SetDefaultValue(param[1], param[2], cypnGoNetwork::GetUserName(hUser));
			m_RequestPlayDlg.bIsRecvMode=TRUE;

			//m_currentCtrl->ShowWindow(SW_HIDE);
			int res = m_RequestPlayDlg.DoModal();
			//m_currentCtrl->ShowWindow(SW_SHOW);
			//m_currentCtrl->UpdateWindow();

			if (res==0)
			{
				m_network->SendReplyPlay(hUser, 0xFFFFFFFF, 0);
				break;
			}

			if (res!=0)
			{
				if (m_RequestPlayDlg.m_bChangedCondition==TRUE)
				{
					m_network->SendReplyPlay(hUser, m_RequestPlayDlg.m_condition1, m_RequestPlayDlg.m_condition2);
				}
				else
				{
					char title[128];
					int id = m_network->SendRequestPlayStart(hUser, &m_RequestPlayDlg.m_condition1, m_RequestPlayDlg.m_condition2);

					if (id==0)
					{
						return 0;
					}
					wsprintf(title, "%s vs %s", m_network->GetMyInfo()->szUserName, cypnGoNetwork::GetUserNameA(hUser));
					m_goban->Add(cyctrlGoban::EIM_PLAY, cyctrlGoban::EMM_DEFAULT, title, m_RequestPlayDlg.m_condition1, *this, id);
				}
			}

			break;
		}
		case eGNPMSG_REQ_PLAY_START:
		{
			unsigned int* param = (unsigned  int*)lp;
			unsigned int id = param[1];
			unsigned int condition1 = param[2];
			unsigned int condition2 = param[3];
			HandleUser hUser = (HandleUser)param[0];
			char title[128];

			cypnGoNetwork::SetGoId(hUser, id);
			wsprintf(title, "%s vs %s", m_network->GetMyInfo()->szUserName, cypnGoNetwork::GetUserNameA(hUser));
			m_goban->Add(cyctrlGoban::EIM_PLAY, cyctrlGoban::EMM_DEFAULT, title, condition1, *this, id);
			break;
		}
		case eGNPMSG_REQ_PLAY_END:
		{
			unsigned int* param = (unsigned  int*)lp;
			HandleUser hUser = (HandleUser)param[0];
			unsigned int id = param[1];
			ePlayEndStatus status = (ePlayEndStatus )param[2];

			if (status==ePlayEndStatus::ePES_GIVEUP)
			{
				// 結果表示
				MessageBox("相手が投了しました。", "中断", MB_OK|MB_ICONINFORMATION);
				m_goban->PlayStop(id);
			}
			else if (status==ePlayEndStatus::ePES_CANCEL)
			{
				// 中断ダイアログ。
				// とりあえず、メッセージボックス
				if (MessageBox("相手が対局の中断を求めました。応じますか？", "中断", MB_YESNO|MB_ICONQUESTION)==IDNO)
				{
					// メッセージを送り続行。
					m_network->SendRequestPlayEndOk(hUser, id, status, False);
					break;
				}
				m_network->SendRequestPlayEndOk(hUser, id, status, True);
				m_goban->PlayStop(id);
			}
			else if (status==ePlayEndStatus::ePES_NORMAL)
			{
				if (MessageBox("相手が終局の申し込みをしました。", "終局", MB_YESNO|MB_ICONQUESTION)==IDNO)
				{
					// メッセージを送り続行
					m_network->SendRequestPlayEndOk(hUser, id, status, False);
					break;
				}

				m_network->SendRequestPlayEndOk(hUser, id, status, True);
				m_goban->CalculateMode(id);//>ChangeMode(cyctrlGoban::eInputMode::eIM_CALCULATE, 0, id);
				m_ctrlPlay.Add(ID_CMD_CALCULATE_END, 0, L"地合い計算終了", 5, 10, 10, 0);
				m_ctrlPlay.Update();
			}


			break;
		}
		case eGNPMSG_REQ_PLAY_END_OK:
		{
			unsigned int* param = (unsigned  int*)lp;
			HandleUser hUser = (HandleUser)param[0];
			unsigned int id = param[1];
			ePlayEndStatus status = (ePlayEndStatus )param[2];
			Bool quit = param[3];

			if (status==ePlayEndStatus::ePES_CANCEL)
			{
				if (quit==FALSE)
				{
					MessageBox("対局の中断を断られました。", "中断",  MB_OK|MB_ICONINFORMATION);
					break;
				}
				m_goban->PlayStop(id);
			}
			else if (status==ePlayEndStatus::ePES_NORMAL)
			{
				if (quit==FALSE)
				{
					MessageBox("終局申し込みが断られました。", "終局",  MB_OK|MB_ICONINFORMATION);
					break;
				}
				m_goban->CalculateMode(id);
				m_ctrlPlay.Add(ID_CMD_CALCULATE_END, 0, L"地合い計算終了", 5, 10, 10, 0);
				m_ctrlPlay.Update();
			}
			break;
		}
		case eGNPMSG_REQ_CALCULATE_END:
		{
			unsigned int* param = (unsigned  int*)lp;
			HandleUser hUser = (HandleUser)param[0];
			unsigned int id = param[1];
			//Bool result = param[2];

			if (MessageBox("相手が地合い計算終了を申し込しました。", "地合い計算終了",MB_YESNO|MB_ICONQUESTION)==IDNO)
			{
				// メッセージを送り続行
				m_network->SendRequestCalculateEndOk(hUser, id, False);
				break;
			}
			m_network->SendRequestCalculateEndOk(hUser, id, True);
			int nBlack, nWhite;
			m_goban->CalculateEnd(&nBlack, &nWhite);
			
			char buffer[256];
			wsprintf(buffer, "黒：%d, 白：%d", nBlack, nWhite);
			MessageBox(buffer, "結果", MB_OK|MB_ICONINFORMATION);


			break;
		}
		case eGNPMSG_REQ_CALCULATE_END_OK:
		{
			unsigned int* param = (unsigned  int*)lp;
			HandleUser hUser = (HandleUser)param[0];
			unsigned int id = param[1];
			Bool result = param[2];

			if (result==FALSE)
			{
				MessageBox("地合い計算終了申し込みが断られました。", "地合い計算",  MB_OK|MB_ICONINFORMATION);
				break;
			}

			int nBlack, nWhite;
			m_goban->CalculateEnd(&nBlack, &nWhite);

			char buffer[256];
			wsprintf(buffer, "黒：%d, 白：%d", nBlack, nWhite);
			MessageBox(buffer, "結果", MB_OK|MB_ICONINFORMATION);
			break;
		}
		case eGNPMSG_SEND_STONE:
		{
			unsigned int* param = (unsigned  int*)lp;
			m_goban->PutStone(param[1], param[2]);
			break;
		}
		case eNRMSG_ADD_USER:
		{
			// ユーザリストを更新する。

			break;
		}
		//case eNRMSG_
		default:
			break;
	}

	if (lp!=0)
	{
		yifree((void*)lp);
	}

	return 0;
}

// ----------------------------------------
LRESULT CMainFrame::OnPutStone(WPARAM wp, LPARAM lp)
{
	unsigned int id = (unsigned int)wp;
	HandleUser hUser = m_network->FindUser(0, 0, (unsigned int)wp);

	yiTrace("OnPutStone go id:%u\n", wp);
	m_network->SendStone(hUser, id, (stone_t)lp);
	return 0;
}

// ----------------------------------------
LRESULT CMainFrame::OnGobanChanged(WPARAM wp, LPARAM lp)
{
	cyctrlGoban::eInputMode input = (cyctrlGoban::eInputMode) LOWORD(lp);
	cyctrlGoban::eMouseMode mouse = (cyctrlGoban::eMouseMode) HIWORD(lp);

	if (wp==0)
	{
		if (m_currentCtrl)
			m_currentCtrl->Hide();
		m_currentCtrl=0;
		return 0;
	}

	if (input==cyctrlGoban::eInputMode::EIM_EDIT)
	{
		if (m_currentCtrl!=0)
		{
			m_currentCtrl->Hide();
		}

		m_currentCtrl=&m_ctrlEdit;
		UpdateControlLayout();
	}
	else if (input==cyctrlGoban::eInputMode::EIM_PLAY)
	{
		if (m_currentCtrl!=0)
		{
			m_currentCtrl->Hide();
		}
		m_currentCtrl=&m_ctrlPlay;
		UpdateControlLayout();
	}
	else if (input==cyctrlGoban::eInputMode::EIM_ENJOY)
	{
		if (m_currentCtrl!=0)
		{
			m_currentCtrl->Hide();
		}
		m_currentCtrl=&m_ctrlEdit;
		UpdateControlLayout();
	}

	return 0;
}

// ----------------------------------------
LRESULT CMainFrame::OnGobanAdded(WPARAM wp, LPARAM lp)
{
	//CMenu* pMenu = GetMenu();
	//CMenu* pGobanListMenu = pMenu->GetSubMenu(7);
	//pGobanListMenu->
	return 0;
}

//
void CMainFrame::AdjustLayout(CDialog* control)
{
//	RECT rect_parent;
//	RECT rect;
////	RECT rect2;
//
//	GetClientRect(&rect_parent);
//
//	UINT w = m_goban->GetSize();
//
//	control->GetClientRect(&rect);
//
//	//rect.left=rect.left-rect_parent.left;
//	//rect.top=rect.top-rect_parent.top;
//	//rect.right=rect.right-rect_parent.right;
//	//rect.bottom=rect.bottom-rect_parent.bottom;
//
//	int h = rect.bottom-rect.top;
//	control->SetWindowPos(0, w, rect_parent.bottom-h, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

    cs.style &= ~FWS_ADDTOTITLE;
    cs.style &= ~FWS_PREFIXTITLE;
	//cs.dwExStyle|=WS_CLIPCHILDREN;


	return TRUE;
}


// CMainFrame 診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame メッセージ ハンドラ

// --------------------------
void CMainFrame::UpdateControlLayout()
{
	RECT rt;
	GetClientRect(&rt);

	if (m_currentCtrl)
	{
		m_currentCtrl->Update(True, rt.right-m_goban->GetSize(), rt.bottom);
		m_currentCtrl->Move(m_goban->GetSize(), rt.bottom-m_currentCtrl->m_h);
	}
}

// --------------------------
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	RECT rt;
	GetClientRect(&rt);

	m_goban->Resize(rt.right, rt.bottom);

	if (m_currentCtrl)
	{
		m_currentCtrl->Update(True, rt.right-m_goban->GetSize(), rt.bottom);
		m_currentCtrl->Move(m_goban->GetSize(), rt.bottom-m_currentCtrl->m_h);
	}

	/*AdjustLayout(&m_PlayControl);*/
	//m_PlayControl.ShowWindow(SW_SHOW);
}

// --------------------------
int CMainFrame::MessageBox(const char* pszText, const char* pszCaption, UINT type)
{
	int ret;
	//m_currentCtrl->ShowWindow(SW_HIDE);
	ret = CWnd::MessageBox(pszText, pszCaption, type);
	//m_currentCtrl->ShowWindow(SW_SHOW);
	//m_currentCtrl->UpdateWindow();

	return ret;
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	//UpdateWindow();
	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CMainFrame::OnCmdGiveup()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	unsigned int id = m_goban->GetCurrentGoId();

	if (id==0)
		return ;

	if (MessageBox("投了しますか？", "投了", MB_YESNO|MB_ICONINFORMATION)==IDNO)
	{
		return ;
	}

	HandleUser hUser = m_network->FindUser(0, 0, id);
	if (hUser==0)
	{
		return ;
	}

	m_network->SendRequestPlayEnd(hUser, id,ePES_GIVEUP);
}

void CMainFrame::OnCmdPlayCancel()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	unsigned int id = m_goban->GetCurrentGoId();

	if (id==0)
		return ;

	if (MessageBox("対局取り消しを申し込みますか？", "取り消し", MB_YESNO|MB_ICONINFORMATION)==IDNO)
	{
		return ;
	}
	HandleUser hUser = m_network->FindUser(0, 0, id);
	if (hUser==0)
	{
		return ;
	}

	m_network->SendRequestPlayEnd(hUser, id, ePES_CANCEL);

}

void CMainFrame::OnCmdQuitPlay()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	unsigned int id = m_goban->GetCurrentGoId();

	if (id==0)
		return ;

	if (MessageBox("終局を申し込みますか？", "終局", MB_YESNO|MB_ICONINFORMATION)==IDNO)
	{
		return ;
	}
	HandleUser hUser = m_network->FindUser(0, 0, id);
	if (hUser==0)
	{
		return ;
	}

	m_network->SendRequestPlayEnd(hUser, id, ePES_NORMAL);
}

void CMainFrame::OnCmdCalculate()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	unsigned int id = m_goban->GetCurrentGoId();

	if (id==0)
		return ;


	if (MessageBox("地合い計算を申し込みますか？", "地合い計算", MB_YESNO|MB_ICONINFORMATION)==IDNO)
	{
		return ;
	}
	HandleUser hUser = m_network->FindUser(0, 0, id);
	if (hUser==0)
	{
		return ;
	}

	m_network->SendRequestCalculateEnd(hUser, id);
}

void CMainFrame::OnCmdCalculateEnd()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	unsigned int id = m_goban->GetCurrentGoId();

	if (id==0)
		return ;

	if (MessageBox("地合い計算終了を申し込みますか？", "地合い計算終了", MB_YESNO|MB_ICONINFORMATION)==IDNO)
	{
		return ;
	}
	HandleUser hUser = m_network->FindUser(0, 0, id);
	if (hUser==0)
	{
		return ;
	}
	m_network->SendRequestCalculateEnd(hUser, id);

}

Bool CMainFrame::CreatePlayControl()
{
	m_ctrlPlay.Create(*this, 0, 0, 10, 2, /*xOnDrawButton*/xOnDrawVistaStyleButton, TRUE, new Font(L"MS P 明朝", 10.0), FALSE);
	m_ctrlPlay.Add(ID_CMD_WAIT, 0, L"待った", 5, 5, 7, 0);
	m_ctrlPlay.Add(ID_CMD_ABORT, 0, L"封じ手", 5, 5, 7, 0);
	m_ctrlPlay.Add(ID_CMD_GIVEUP, 0, L"投了", 5, 5, 7, 0);
	m_ctrlPlay.Add(ID_CMD_PLAY_END, 0, L"終局申し込み", 5, 10, 7, 0);
	m_ctrlPlay.Add(ID_CMD_PLAY_CANCEL, 0, L"対局取り消し", 5, 10, 7, 0);
	m_ctrlPlay.Update(False);

	m_ctrlPlay.EnableItem(ID_CMD_WAIT, FALSE);
	m_ctrlPlay.EnableItem(ID_CMD_ABORT, FALSE);
	return True;
}

#include "GoYotsuyaResource.h"

Bool CMainFrame::CreateEditControl()
{
	Bitmap* bmptri = cyiShape::FillRoundTriangle(0, &SolidBrush(Color::Black), RectF(0, 0, 20, 10), 1.0);
	Bitmap* bmpT = cyiShape::FillRoundRect(&SolidBrush(Color::Black), RectF(0, 0, 20, 5), 1);
	Bitmap* bmpN = cyiShape::BitmapString(0, L"1", new Font(L"MS P 明朝", 10.0), &SolidBrush(Color::Black));
	Bitmap bmpdtri(20, 20, PixelFormat32bppARGB);
	Bitmap bmpdTx(20, 15, PixelFormat32bppARGB);
	Bitmap* bmpNx = new Bitmap(20, 20, PixelFormat32bppARGB);

	Graphics g(bmpNx);

	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.DrawImage(bmpN, (INT)(bmpNx->GetWidth()-bmpN->GetWidth())/2+1, (INT)(bmpNx->GetHeight()-bmpN->GetHeight())/2+1);
	g.DrawEllipse(&Pen(Color::Black, 2.0), 1, 1, 17, 17);

	cyiBitmapBlender::RotateCopy(&bmpdtri, bmptri, 0);
	cyiBitmapBlender::RotateCopy(&bmpdtri, bmptri, 0, 0, 10); 

	cyiBitmapBlender::RotateCopy(&bmpdTx, bmptri, 0);
	cyiBitmapBlender::RotateCopy(&bmpdTx, bmpT, 0, 0, 10);


	m_ctrlEdit.Create(*this, 0, 0, 10, 2, /*xOnDrawButton*/xOnDrawVistaStyleButton, TRUE, new Font(L"MS P 明朝", 10.0), FALSE);
	m_ctrlEdit.Add(ID_CMD_TOP, cyiBitmapBlender::RotateCopy(0,&bmpdTx, 3), 0, 0, 36, 36, 0, 0, True);
	m_ctrlEdit.Add(ID_CMD_PREV_PLUS, cyiBitmapBlender::RotateCopy(0,&bmpdtri, 3), 0, 0, 36, 36, 0, 0, True);
	m_ctrlEdit.Add(ID_CMD_PREV, cyiBitmapBlender::RotateCopy(0,bmptri, 3), 0, 0, 36, 36, 0, 0, True);
	m_ctrlEdit.Add(ID_CMD_NEXT, cyiBitmapBlender::RotateCopy(0,bmptri, 1), 0, 0, 36, 36, 0, 0, True);
    m_ctrlEdit.Add(ID_CMD_NEXT_PLUS, cyiBitmapBlender::RotateCopy(0,&bmpdtri, 1), 0, 0, 36, 36, 0, 0, True);
	m_ctrlEdit.Add(ID_CMD_END, cyiBitmapBlender::RotateCopy(0,&bmpdTx, 1), 0, 0, 36, 36, 0, 0, True);
	m_ctrlEdit.Add(ID_CMD_END, bmpNx, 0, 0, 36, 36, (void*)1, 0,True);
	m_ctrlEdit.Add(ID_CMD_END, 0, L"次の枝", 0, 10, 15, 0);
	m_ctrlEdit.Add(ID_CMD_END, 0, L"前の枝", 0, 10, 15, 0);
	m_ctrlEdit.Add(ID_CMD_END, 0, L"次のコメント", 0, 10, 15, 0);
	m_ctrlEdit.Add(ID_CMD_END, 0, L"前のコメント", 0, 10, 15, 0);
	m_ctrlEdit.Add(ID_CMD_END, 0, L"ラベル追加", 0, 10, 15, 0);
	


	m_ctrlEdit.Update(False);

	delete bmptri;
	delete bmpT;

	return True;
}

Bool CMainFrame::CreateEnjoyControl()
{
	return True;
}


static Bool xOnDrawVistaStyleButton(int id, Graphics* g, Font* fnt, sControlx* x, eControlxEvent xevent)
{
	if (xevent==eCE_BACKGROUND)
	{
		POINT* pt =(POINT*)x;
		g->FillRectangle(&SolidBrush(Color::White), 0, 0, pt->x, pt->y);
	}
	else if (xevent==eCE_INIT ||
			xevent==eCE_MOUSE_LEAVE )
	{
		if (x->pBmp[0]!=0)
		{
			g->FillRectangle(&SolidBrush(Color::White), x->rect.left, x->rect.top, x->rect.right, x->rect.bottom);
			g->DrawImage(x->pBmp[0], x->rect.left, x->rect.top);
			//g->DrawImage(x->pBmp[1], x->rect.left, x->rect.top);
			return True;
		}

		Bitmap* bmp = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, x->rect.right-2, x->rect.bottom-2), 1.5);
		Graphics g2(bmp);
		g2.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(32, 32, 32, 32)), RectF(2, 2, x->rect.right-2, x->rect.bottom-2), 1.5);
		
		if (x->bmp!=0)
		{
			float xx, y;
			cyctrlx::GetBmpPos(x, &xx, &y, x->rect.right-2, x->rect.bottom-2, (eContorlxAlign)(eCXA_CENTER|eCXA_VCENTER));
			g2.DrawImage(x->bmp, xx, y);
		}

		int txtlen = wcslen(x->name);

		if (txtlen)
		{
			float xx, y;
			cyctrlx::GetTextPos(x, &xx, &y, x->rect.right-2, x->rect.bottom-2, (eContorlxAlign)(eCXA_CENTER|eCXA_VCENTER));
			g2.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
			g2.DrawString(x->name, wcslen(x->name), fnt, RectF(xx, y, x->txt_w, x->txt_h), 0, &SolidBrush(Color::Black));	
			g2.FillRectangle(&LinearGradientBrush(PointF(0.0, 0.0), PointF(0.0, (x->rect.bottom-2)/2), 
				Color(64, 255, 255, 255), Color(0, 255,255,255)),0, 0, x->rect.right-4, (x->rect.bottom-2)/2);
		}
		g2.DrawImage(bmp3, 0, 0);
		Bitmap* bmp2 = cyiBitmapBlender::DropShadow(bmp, 1, 1, 1, 0.3);
		//::TextRenderingHint

		
		//g->DrawImage(bmp2, x->rect.left, x->rect.top);
		g->FillRectangle(&SolidBrush(Color::White), x->rect.left, x->rect.top, x->rect.right, x->rect.bottom);
		g->DrawImage(bmp2, x->rect.left, x->rect.top);
		//g->DrawImage(bmp3, x->rect.left, x->rect.top);

		x->pBmp[0]=bmp2;
		//x->pBmp[1]=bmp3;
		delete bmp;
		return True;

		//g->DrawString(x->name, wcslen(x->name), fnt, RectF(xx, y, x->txt_w, x->txt_h), 0, &SolidBrush(Color::Black));
	}
	else if (xevent==eCE_MOUSE_IN||
				xevent==eCE_MOUSE_LUP)
	{
		if (x->pBmp[2]!=0)
		{
			g->FillRectangle(&SolidBrush(Color::White), x->rect.left, x->rect.top, x->rect.right, x->rect.bottom);
			g->DrawImage(x->pBmp[2], x->rect.left, x->rect.top);
			//g->DrawImage(x->pBmp[4], x->rect.left, x->rect.top);
			return True;
		}
		Bitmap* bmp = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, x->rect.right-2, x->rect.bottom-2), 1.5);

		Graphics g2(bmp);

		g2.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(8, 16, 64, 16)), RectF(0, 0, x->rect.right-2, x->rect.bottom-2), 1.5);
		Bitmap temp(x->rect.right-2, x->rect.bottom-2, PixelFormat32bppARGB);
		Graphics g3(&temp);
		g3.FillEllipse(&SolidBrush(Color(127, 127, 255, 127)), RectF(5, 5, x->rect.right-2-10, x->rect.bottom-2-10)); 
		Bitmap* bmp5 = cyiBitmapBlender::Blure(&temp, 10.0, 100);

		g2.DrawImage(bmp5, 0, 0);
		g2.DrawImage(bmp3, 0, 0);
	
		if (x->bmp!=0)
		{
			float xx, y;
			cyctrlx::GetBmpPos(x, &xx, &y, x->rect.right-2, x->rect.bottom-2, (eContorlxAlign)(eCXA_CENTER|eCXA_VCENTER));
			g2.DrawImage(x->bmp, xx, y);
		}

		int txtlen=wcslen(x->name);
		if (txtlen)
		{
			float xx, y;
			g2.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
			cyctrlx::GetTextPos(x, &xx, &y, x->rect.right-2, x->rect.bottom-2, (eContorlxAlign)(eCXA_CENTER|eCXA_VCENTER));
			g2.DrawString(x->name, txtlen, fnt, RectF(xx, y, x->txt_w, x->txt_h), 0, &SolidBrush(Color::Black));
		}

		Bitmap* bmp2 = cyiBitmapBlender::DropShadow(bmp, 1, 1, 1, 0.3);
		//::TextRenderingHint

		
		//g->DrawImage(bmp2, x->rect.left, x->rect.top);
		g->FillRectangle(&SolidBrush(Color::White), x->rect.left, x->rect.top, x->rect.right, x->rect.bottom);
		g->DrawImage(bmp2, x->rect.left, x->rect.top);
		//g->DrawImage(bmp3, x->rect.left, x->rect.top);

		x->pBmp[2]=bmp2;
		//x->pBmp[4]=bmp3;

		delete bmp;
		delete bmp5;
		delete bmp3;

		return True;
	}
	else if (xevent==eCE_MOUSE_LDOWN)
	{
		if (x->pBmp[3]!=0)
		{
			g->FillRectangle(&SolidBrush(Color::White), x->rect.left-1, x->rect.top-1, x->rect.right+1, x->rect.bottom+1);
			g->DrawImage(x->pBmp[3], x->rect.left+1, x->rect.top+1);
			return True;
		}
		Bitmap* bmp = cyiShape::DrawRoundRect(&Pen(Color::Black, 1.0), RectF(0, 0, x->rect.right-2, x->rect.bottom-2), 1.5);

		Graphics g2(bmp);
		g2.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		Bitmap* bmp3 = cyiShape::FillRoundRect(&SolidBrush(Color(32, 32, 32, 32)), RectF(0, 0, x->rect.right-2, x->rect.bottom-2), 1.5);
		g2.DrawImage(bmp3, 0, 0);

		if (x->bmp!=0)
		{
			float xx, y;
			cyctrlx::GetBmpPos(x, &xx, &y, x->rect.right-2, x->rect.bottom-2, (eContorlxAlign)(eCXA_CENTER|eCXA_VCENTER));
			g2.DrawImage(x->bmp, xx, y);
		}

		int txtlen=wcslen(x->name);
		if (txtlen)
		{
			float xx, y;
			cyctrlx::GetTextPos(x, &xx, &y, x->rect.right-2, x->rect.bottom-2, (eContorlxAlign)(eCXA_CENTER|eCXA_VCENTER));
			g2.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
			g2.DrawString(x->name, txtlen, fnt, RectF(xx, y, x->txt_w, x->txt_h), 0, &SolidBrush(Color::Black));
		}

		Bitmap* bmp2 = cyiBitmapBlender::DropShadow(bmp, 0, 0, 1.0, 0.7);
		//::TextRenderingHint

		//g->DrawImage(bmp2, x->rect.left, x->rect.top);
		g->FillRectangle(&SolidBrush(Color::White), x->rect.left-1, x->rect.top-1, x->rect.right+1, x->rect.bottom+1);
		g->DrawImage(bmp2, x->rect.left+1, x->rect.top+1);

		delete(bmp3);
		delete bmp;
		x->pBmp[3]=bmp2;
		return True;
	}
	else if (xevent==eCE_DISABLE)
	{
		if (x->pBmp[0]!=0)
		{
			Bitmap* z = cyiBitmapBlender::ArgbShift(x->pBmp[0], -32, 0, 0, 0);

			g->FillRectangle(&SolidBrush(Color::White), x->rect.left, x->rect.top, x->rect.right, x->rect.bottom);
			g->DrawImage(z, x->rect.left, x->rect.top);
			return True;
		}
	}

	return False;
}
void CMainFrame::OnCmdWait()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
}

void CMainFrame::OnCmdAbort()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
}

void CMainFrame::OnCmdPlayEnd()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	// TODO: ここにコマンド ハンドラ コードを追加します。
	unsigned int id = m_goban->GetCurrentGoId();

	if (id==0)
		return ;

	if (MessageBox("終局を申し込みますか？", "終局", MB_YESNO|MB_ICONINFORMATION)==IDNO)
	{
		return ;
	}
	HandleUser hUser = m_network->FindUser(0, 0, id);
	if (hUser==0)
	{
		return ;
	}

	m_network->SendRequestPlayEnd(hUser, id, ePES_NORMAL);
}

#include "cydlgx.h"
void CMainFrame::OnCmdNext()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。

	m_goban->ToStoneIndex(1, cyctrlGoban::eStoneIndexFrom::FROM_CUR);

}

#include "cydlgx.h"
void CMainFrame::OnCmdTop()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。

	int n = m_goban->GetSize();

	m_goban->ShowMessage(0,  L"相手との通信が切れました。", MB_OK);
	//::yiMessageBox(m_hWnd, 100, 100, L"相手との通信が切れました。", MB_OK);
}

void CMainFrame::OnCmdPrev()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	m_goban->ToStoneIndex(-1, cyctrlGoban::eStoneIndexFrom::FROM_CUR);
}

void CMainFrame::OnAppExit()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
}
