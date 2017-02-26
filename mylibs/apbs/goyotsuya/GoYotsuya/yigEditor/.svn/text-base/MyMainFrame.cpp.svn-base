//#include "StdAfx.h"
#include "MyMainFrame.h"
#include "yiGoCommandz.h"

extern Font* xGetSystemFont(int w, int h);

// ---------------------------------------
static ATOM xMyRegisterClass(HINSTANCE hInstance, const char* pszClassName);

// ---------------------------------------
static HWND xInitInstance(HINSTANCE hInstance, const char* pszClassName, const char* pszTitle, int nCmdShow, void* param);

// ---------------------------------------
cMyMainFrame::cMyMainFrame()
:m_imgBg(0)
,m_bEnterSizeMove(0)
,m_bOnSizeChangedCalled(0)
,m_imgDefault(0)
{
	__GLASSINTERFACE_NAME__;
}

// ---------------------------------------
cMyMainFrame::~cMyMainFrame()
{
}

// -------------------------------------
LRESULT cMyMainFrame::OnCreate(WPARAM wParam, LPARAM lParam)
{
	cglsWindow::OnCreate(wParam, lParam);


	RegisterSystem();
	m_app=(CGoEditorApp*)xGetMyApp();

	m_system->RegisterCommandCenter((cglsCommandCenter*)m_app);
	// 関連コマンド登録


#ifdef _GOYOTSUYA
	memset(m_nNetworkUserPacket, 0, sizeof(m_nNetworkUserPacket));
	m_nNetworkUserPacket[eGNPMSG_SEND_TEXT_MESSAGE]=eGCID_TEXT_MESSAGE;
	m_nNetworkUserPacket[eGNPMSG_WAIT_RESULT]=eGCID_WAIT_RESULT;
	m_nNetworkUserPacket[eGNPMSG_STOP_TIMER]=eGCID_STOP_TIMER;
	m_nNetworkUserPacket[eGNPMSG_WAIT]=eGCID_WAIT;
	m_nNetworkUserPacket[eGNPMSG_TIME_LOSE]=eGCID_TIME_LOSE;
	m_nNetworkUserPacket[eGNPMSG_GIVEUP]=eGCID_GIVEUP;
	m_nNetworkUserPacket[eGNPMSG_REQ_PLAY_START]=eGCID_REQ_PLAY_START;
	m_nNetworkUserPacket[eGNPMSG_REQ_CALCULATE_END_OK]=eGCID_CALCULATE_END_OK;
	m_nNetworkUserPacket[eGNPMSG_REQ_CALCULATE_END]=eGCID_CALCULATE_END;
	m_nNetworkUserPacket[eGNPMSG_REQ_PLAY_END_OK]=eGCID_REQ_PLAY_END_OK;
	m_nNetworkUserPacket[eGNPMSG_REQ_PLAY_END]=eGCID_REQ_PLAY_END;
	m_nNetworkUserPacket[eGNPMSG_SEND_STONE]=eGCID_SEND_STONE;
	m_nNetworkUserPacket[eGNPMSG_REPLY_PLAY]=eGCID_RPL_PLAY;
	m_nNetworkUserPacket[eGNPMSG_REQ_PLAY]=eGCID_REQ_PLAY;
	m_nNetworkUserPacket[eGNPMSG_REQ_INFO_DETAIL]=eGCID_REQ_INFO_DETAIL;
	m_nNetworkUserPacket[eGNPMSG_RPY_INFO_DETAIL]=eGCID_RPY_INFO_DETAIL;
	m_nNetworkUserPacket[eGNPMSG_REQ_PICTURE] = eGCID_REQ_PICTURE;
	m_nNetworkUserPacket[eGNPMSG_RPY_PICTURE] = eGCID_RPY_PICTURE;
	m_nNetworkUserPacket[eGNPMSG_REQ_DATA]		=eGCID_REQ_DATA;
	m_nNetworkUserPacket[eGNPMSG_RPY_DATA]		= eGCID_RPY_DATA;
	m_nNetworkUserPacket[eGNPMSG_SEND_CMD]		= eGCID_SEND_CMD;
	//m_nNetworkUserPacket[eGNPMSG_STUDY_ANSWER]		= eGCID_STUDY_ANSWER;
	//m_nNetworkUserPacket[eGNPMSG_STUDY_QUESTION]		= eGCID_STUDY_QUESTION;
	//m_nNetworkUserPacket[eGNPMSG_STUDY_END]				= eGCID_STUDY_END;
	m_nNetworkUserPacket[eGNPMSG_CURRENT_GO_LIST]		= eGCID_CURRENT_GO_LIST;
	m_nNetworkUserPacket[eGNPMSG_HAGASHI]				= eGCID_HAGASHI;
	m_nNetworkUserPacket[eGNPMSG_NOTIFY_PUTSTONE]		= eGCID_NOTIFY_PUTSTONE;
	m_nNetworkUserPacket[eGNPMSG_START_PLAY]			= eGCID_START_PLAY;
	m_nNetworkUserPacket[eGNPMSG_CANCEL_FORCED]			= eGCID_CANCEL_FORCED;
	m_nNetworkUserPacket[eGNPMSG_CANCEL]				= eGCID_CANCEL;
	m_nNetworkUserPacket[eGNPMSG_NO_CANCEL]				= eGCID_NO_CANCEL;
	m_nNetworkUserPacket[eGNPMSG_WATCH_GO]				= eGCID_WATCH_GO;
	m_nNetworkUserPacket[eGNPMSG_WATCH_GO_RESULT]		= eGCID_WATCH_GO_RESULT;
	m_nNetworkUserPacket[eGNPMSG_GO_RESULT]				= eGCID_GO_RESULT;
	m_nNetworkUserPacket[eGNPMSG_WATCH_REMAIN_TIME]		= eGCID_WATCH_REMAIN_TIME;
	m_nNetworkUserPacket[eGNPMSG_CALCULATE_CONFIRM]		= eGCID_CALCULATE_CONFIRM;
	m_nNetworkUserPacket[eGNPMSG_CALCULATE_NO_CONFIRM]	= eGCID_CALCULATE_NO_CONFIRM;
	m_nNetworkUserPacket[eGNPMSG_CALCULATE_END]			= eGCID_CALCULATE_END;
	m_nNetworkUserPacket[eGNPMSG_STONE_MESSAGE]			= eGCID_STONE_MESSAGE;
	m_nNetworkUserPacket[eGNPMSG_REQ_REPLAY]			= eGCID_REQ_REPLAY;
	m_nNetworkUserPacket[eGNPMSG_REQ_REPLAY_OK]			= eGCID_REQ_REPLAY_OK;
	m_nNetworkUserPacket[eGNPMSG_PAUSE]					= eGCID_PAUSE;

	m_system->RegisterCommand(this, eGCID_ADD_USER ,eNRMSG_ADD_USER);
	m_system->RegisterCommand(this, eGCID_REMOVE_USER ,eNRMSG_REMOVE_USER);
	m_system->RegisterCommand(this, eGCID_CONNECTION_CLOSED, eNRMSG_CONNECTION_CLOSED);
#endif

	m_app->OnCreate((HWND)this);

	HDC hdc=::GetDC(GetDesktopWindow());

	int nWidth = GetDeviceCaps(hdc, HORZRES);
	int nHeight = GetDeviceCaps(hdc, VERTRES);

	::ReleaseDC(GetDesktopWindow(), hdc);
	m_system->SetDisplayResolution(nWidth, nHeight);

		//case eNRMSG_SERVER_CLOSED:
		//case eNRMSG_CONNECTION_CLOSED:
		//case eNRMSG_WAIT_ERROR:
		//case eNRMSG_CONNECTION_FAILURE:
		//case eNRMSG_ADD_USER:
		//case eNRMSG_SYSTEM_ERROR:
		//case eNRMSG_LOOP_EXIT:

	char szFileName[256];
	wchar_t wszFileName[256];
	m_app->GetProperty(0, "BackgroundImage", szFileName);

	yiToUnicode(wszFileName, szFileName, 0);
	Image* img = new Image(wszFileName);

	if (img->GetLastStatus()==0)
	{
		m_imgBg=img;
	}
	else
	{
		m_imgDefault = BITMAP32(32, 32);//new Image(L".\\data\\131.jpg");

		Graphics gt(m_imgDefault);

		gt.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

#ifdef _GOYOTSUYA
		gt.FillRectangle(&SolidBrush(Color::Beige), 0, 0, 32, 32);
		gt.FillEllipse(&SolidBrush(Color::YellowGreen), Rect(8, 8, 16, 16));
#endif
#ifdef JOSEKI_DICTIONARY
		gt.FillRectangle(&SolidBrush(Color::White), 0, 0, 32, 32);
		gt.FillEllipse(&SolidBrush(Color::LightPink), Rect(8, 8, 16, 16));
#endif
#ifdef _GOMAIL
		gt.FillRectangle(&SolidBrush(Color::White), 0, 0, 32, 32);
		gt.FillEllipse(&SolidBrush(Color::LightBlue), Rect(8, 8, 16, 16));
#endif
	}


	return 0;
}

// -------------------------------------
LRESULT cMyMainFrame::OnClose(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	return 0;
}


// -------------------------------------
LRESULT cMyMainFrame::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	{
		RECT rect;
		char szValue[32];
		GetWindowRect(m_hWnd, &rect);
		wsprintf(szValue, "(%d,%d,%d,%d)", rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top);
		m_app->SetProperty(0, "DISPLAY", szValue);
	}

	PostQuitMessage(0);
	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnEnterSizeMove(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	m_bEnterSizeMove=True;
	m_bOnSizeChangedCalled=False;
	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnExitSizeMove(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	RECT rect;
	m_bEnterSizeMove=False;

	// この場合はMove
	if (m_bOnSizeChangedCalled==False)
		return 0;

	if (wParam==SIZE_MINIMIZED)
	{
		handled=False;
		return 0;
	}

	::GetClientRect(m_hWnd, &rect);

	// for minimize.
	if (rect.right==0 && rect.bottom==0)
	{
		handled=False;
		return 0;
	}

	if (m_bPushed==True)
	{
		handled=True;
		return 0;
	}

	
	//int w;
	//app.m_goban->GetSize(&w);//GetSize();
	//SetWindowPos(app.m_hControlWnd, 0, w, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);

	//RECT rect;
	//::GetClientRect(m_hWnd, &rect);

	__super::OnSizeChanged(0, lParam, handled);
	//m_app->m_goban->Resize(rect.right, rect.bottom);

	cglsGoban* goban = m_app->m_goban;

	if (goban->m_h==m_h &&
			goban->GetRealWidth()==m_w)
	{
		return 0;
	}

	if (goban)
	{
		goban->ReSize(rect.right, rect.bottom);
		int n = goban->GetRealWidth();
		ReSizeClientRect(n, m_h);
	}

	handled=True;

	Font* font = xGetSystemFont(m_w, m_h);

	if (font)
	{
		SetFont(font);
	}

	return 0;

}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled)
{

	if (m_bEnterSizeMove==True)
	{
		m_bOnSizeChangedCalled=True;
		return 0;
	}

	RECT rect;

	if (wParam==SIZE_MINIMIZED)
	{
		handled=False;
		return 0;
	}

	::GetClientRect(m_hWnd, &rect);

	// for minimize.
	if (rect.right==0 && rect.bottom==0)
	{
		handled=False;
		return 0;
	}

	if (m_bPushed==True)
	{
		handled=True;
		return 0;
	}

	
	//int w;
	//app.m_goban->GetSize(&w);//GetSize();
	//SetWindowPos(app.m_hControlWnd, 0, w, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);

	__super::OnSizeChanged(wParam, lParam, handled);

	cglsGoban* goban = m_app->m_goban;

	if (goban->m_h==m_h &&
			goban->GetRealWidth()==m_w)
	{
		InvalidateRect(0);
		return 0;
	}

	if (goban)
	{
		goban->ReSize(rect.right, rect.bottom);
		int n = goban->GetRealWidth();
		ReSizeClientRect(n, m_h);
	}

	//m_app->m_goban->Resize(rect.right, rect.bottom);

	InvalidateRect(0, False, True);

	//Font* font = xGetSystemFont(m_w, m_h);

	//if (font)
	//{
	//	SetFont(font);
	//}

	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnNodeChanged(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	m_app->OnNodeChanged(wParam, lParam);
	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnPreviewSelected(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	char buffer[16];
	if (wParam==0)
	{
		m_app->m_goban->PlayByNode((cyiTreeNode*)lParam);
	}
	else
	{
		m_app->m_goban->AddByNode((cyiTreeNode*)lParam, "プレビュー", False, True);
	}
	/*
	if (m_app->GetProperty(0, "bIsPreviewGotoSame", buffer)==True)
	{
		if (stricmp(buffer, "True")==0)
		{
			m_app->m_goban->PlayByNode((cyiTreeNode*)lParam);
		}
	}

	if (m_app->GetProperty(0, "bIsPreviewNewTab", buffer)==True)
	{
		if (stricmp(buffer, "True")==0)
		{
			m_app->m_goban->AddByNode((cyiTreeNode*)lParam, "");
		}
	}*/
	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnPreviewPlay(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnCopyData(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	COPYDATASTRUCT *pCD = (COPYDATASTRUCT*)lParam;

	//MessageBox(m_hWnd, "OnCopyData Called", "", MB_OK);

	if (wParam==0)
	{
		char* pszCommandLine=(char*)pCD->lpData;
		//MessageBox(m_hWnd, pszCommandLine, "filename", MB_OK);
		if (pszCommandLine!=0 && pszCommandLine[0])
		{
			char* pszTemp = (char*)(pszCommandLine[0]=='\"')?(char*)&pszCommandLine[1]:(char*)pszCommandLine;

			if (pszTemp!=pszCommandLine)
			{
				pszTemp[strlen(pszTemp)-1]=(char)0;
			}


			Bool bIsDir=False;
			if (::yiIsExist(pszTemp, &bIsDir)==True)
			{
				m_app->m_goban->AddForEditByFile(pszTemp);
			}
		}
	}

	return 1;
}

#include "yiGoCommandz.h"

// ---------------------------------------------------
LRESULT cMyMainFrame::OnNotifyFromServer(WPARAM wParam, LPARAM lParam, Bool& handled)
{
#ifdef _GOYOTSUYA
	int nSystemId=0;
	handled=True;
	int id = (wParam&0xFFFF);

	if (wParam==eNRMSG_SERVER_CLOSED)
	{
		// ReConnect ?
		xTRACE("cMyMainFrame::OnNotifyFromNetwork() server disconnected!\n");
		return 0;
	}

	switch(id)
	{
		case eUSPID_REQ_ADD_FRIEND:
		{
			nSystemId=eGCID_ADD_FRIEND;
			break;
		}
		case eUSPID_NOTIFY_MSG32:
		{
			//int* ret = (int*)lParam;

			//if (*ret==0)
			//{
			//	cypnGoNetwork* network=(cypnGoNetwork*)cypnGoNetwork::GetInstance();

			//	network->SendToServer(eUSPID_GET_MSG32, &network->m_myinfo.id, sizeof(unsigned int), False);
			//}

			//yifree((void*)ret);

			nSystemId=eGCID_NOTIFY_MSG32;
			break;
		}
		case eUSPID_GET_MSG32:
		{
			nSystemId=eGCID_GET_MSG32;
			break;
		}
		case eUSPID_REQ_ADD_FRIEND_OK:
		{
			nSystemId=eGCID_ADD_FRIEND_RESULT;
			break;
		}
		case eUSPID_GET_FRIEND_LIST:
		{
			nSystemId=eGCID_GET_FRIEND_LIST;
			break;
		}
		case eUSPID_REQ_CONNECT_UDP_ME:
		{
			nSystemId=eGCID_CONNECT_ME;
			break;
		}
		default:
			handled=False;
			break;
	}
	
	if (nSystemId)
		GetSystem()->GetCommandCenter()->OnCommand(nSystemId, this, lParam, (wParam>>16));
#endif
	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnNotifyFromNetwork(WPARAM wParam, LPARAM lParam, Bool& handled)
{
#ifdef _GOYOTSUYA

	//xTRACE("cMyMainFrame::OnNotifyFromNetwork() called");
	cglsSystem* system = GetSystem();

	if (wParam==eNRMSG_SERVER_CLOSED)
	{
		// ReConnected.
		xTRACE("cMyMainFrame::OnNotifyFromNetwork() server disconnected!\n");
		return 0;
	}

	// main udpsock.
	if ((wParam&0x80000000)!=0 &&
			(wParam&0x40000000)==0)
	{
		system->GetCommandCenter()->OnCommand(eGCID_MSG_MAIN_UDP, this, wParam&0x7FFFFFFF, lParam);
		yifree((void*)lParam);
		return 0;
	}

	if (system->GetCommandId(this, wParam)==0)
	{
		if (m_nNetworkUserPacket[wParam]!=0)
		{
			system->GetCommandCenter()->OnCommand(m_nNetworkUserPacket[wParam], this, lParam);
		}
		return 0;
	}

	system->GetCommandCenter()->OnCommand(system->GetCommandId(this, wParam), this, lParam);

#endif
	//xTRACE("cMyMainFrame::OnNotifyFromNetwork() exit");
	return 0;
}


// ---------------------------------------------------
LRESULT cMyMainFrame::OnOpenFile(HWND hWnd, int code)
{
	char szFileName[256]={0,};
	if (yiFileOpenDialog(m_hWnd, "Open", "", "*.sgf", "SGF(*.sgf)", szFileName) ==True)
	{
		m_app->m_goban->AddForEditByFile(szFileName);
	}
	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnNew(HWND hWnd, int code)
{
	unsigned int flag=0;
	unsigned int condition=cyiGoEngine::DefaultCondition(flag);
	m_app->m_goban->AddForEdit(0, 0, 0, condition, flag);
	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnSave(HWND hWnd, int code)
{
	if (m_app->m_goban->Save(0)==False)
	{
		return OnSaveAs(hWnd, code);
	}
	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnSaveAs(HWND hWnd, int code)
{
	char szFileName[256]={0,};
	if (yiFileSaveDialog(m_hWnd, "Save", "", "*.sgf", "SGF(*.sgf)", szFileName) ==True)
	{
		m_app->m_goban->Save(szFileName);//(szFileName);
	}
	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnPreview(HWND hWnd, int code)
{
	m_app->m_goban->ShowPreviewWindow();
	return 0;
}

// -------------------------------------
Bitmap* cMyMainFrame::GetBgBitmap()
{
	if (m_bg==0 ||
			m_bg->GetWidth()!=m_w ||
				m_bg->GetHeight()!=m_h)
	{
		if (m_bg)
			delete m_bg;
		m_bg=0;

		if (m_imgBg)
		{
			if (m_bg==0)
			{
				m_bg=BITMAP32(m_w, m_h);
			}

			Graphics g(m_bg);

			TextureBrush br(m_imgBg);

			g.FillRectangle(&br, 0, 0, m_w, m_h);


		}

		if (m_imgDefault)
		{
			if (m_bg==0)
			{
				m_bg=BITMAP32(m_w, m_h);
			}

			Graphics g(m_bg);

			TextureBrush br(m_imgDefault);
			g.FillRectangle(&br, 0, 0, m_w, m_h);


		}
	}

	return m_bg;
}

// -------------------------------------
Bool cMyMainFrame::OnBgDraw()
{
	if (m_imgBg)
	{
		if (m_bmpCurrent==0)
		{
			m_bmpCurrent=BITMAP32(m_w, m_h);
		}
		if (m_bg==0)
		{
			m_bg=BITMAP32(m_w, m_h);
		}

		int w=m_bmpCurrent->GetWidth();
		int h=m_bmpCurrent->GetHeight();

		if (w!=m_w || h!=m_h)
		{
			delete m_bmpCurrent;
			delete m_bg;
			m_bmpCurrent=BITMAP32(m_w, m_h);
			m_bg=BITMAP32(m_w, m_h);
		}

		Graphics g(m_bg);

		TextureBrush br(m_imgBg);

		if (m_w>m_h)
		{
			g.FillRectangle(&br, m_h, 0, m_w, m_h);
		}

		cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);

		//m_nRequestUpdateOrder=0;
		//Update(m_bmpCurrent);
		InvalidateRect(0);
		return True;
	}

	if (m_imgDefault)
	{
		if (m_bmpCurrent==0)
		{
			m_bmpCurrent=BITMAP32(m_w, m_h);
		}
		if (m_bg==0)
		{
			m_bg=BITMAP32(m_w, m_h);
		}

		int w=m_bmpCurrent->GetWidth();
		int h=m_bmpCurrent->GetHeight();

		if (w!=m_w || h!=m_h)
		{
			delete m_bmpCurrent;
			delete m_bg;
			m_bmpCurrent=BITMAP32(m_w, m_h);
			m_bg=BITMAP32(m_w, m_h);
		}

		Graphics g(m_bg);

		TextureBrush br(m_imgDefault);
		//if (m_w>m_h)
		//{
			g.FillRectangle(&br, 0, 0, m_w, m_h);
			//g.FillRectangle(&br, m_h, 0, m_w, m_h);
		//}

		cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);
		InvalidateRect(0);
		return True;
	}

	return __super::OnBgDraw();
}
#ifdef _DEBUG
// ---------------------------------------------------
int cMyMainFrame::RegisterChild(cGlassInterface* child)
{
	int ret = __super::RegisterChild(child);
	return ret;
}
#endif

// ---------------------------------------------------
LRESULT cMyMainFrame::OnChangeBg(HWND hWnd, int code)
{
	char szFileName[256]={0,};
	if (yiFileOpenDialog(m_hWnd, "Open", "", "Image files (*.jpg;*.jpeg;*.gif;*.bmp;*.png)", "イメージファイル(*.jpeg)", szFileName) ==True)
	{
		wchar_t wszFileName[256];

		yiToUnicode(wszFileName, szFileName, 0);
		Image* img = new Image(wszFileName);

		if (img->GetLastStatus()!=0)
		{
			return 0;
		}

		m_app->SetProperty(0, "BackgroundImage", szFileName);

		if (m_imgBg)
			delete m_imgBg;

		m_imgBg=img;

		OnBgDraw();
	}
	return 0;
}

// ---------------------------------------
LRESULT cMyMainFrame::OnPostCommand(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	return GetSystem()->GetCommandCenter()->OnCommand(wParam, this, (lParam>>16), lParam&0xFFFF);
}

// ---------------------------------------
LRESULT cMyMainFrame::OnWhois(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	return 0;
}

// ---------------------------------------------------
LRESULT cMyMainFrame::OnNotDefineMessage(UINT msg, WPARAM wParam, LPARAM lParam, Bool& handled)
{
	//switch(msg)
	//{
	//	case eNRMSG_SERVER_CLOSED:
	//	case eNRMSG_CONNECTION_CLOSED:
	//	case eNRMSG_WAIT_ERROR:
	//	case eNRMSG_CONNECTION_FAILURE:
	//	case eNRMSG_ADD_USER:
	//	case eNRMSG_SYSTEM_ERROR:
	//	case eNRMSG_LOOP_EXIT:
	//	{
	//		int id = GetSystem()->GetCommandId(this, msg);

	//		if (id)
	//		{
	//			handled=True;
	//			GetSystem()->GetCommandCenter()->OnCommand(id, this, wParam, lParam);
	//		}
	//	}
	//}

	return 0;
}



// ---------------------------------------------------
//LRESULT cMyMainFrame::OnMenuBtnClicked(HWND hWnd, int code)
//{
//	POINT pt;
//	GetCursorPos(&pt);
//
//	m_app->m_gobanMenu->Show(pt.x, pt.y);
//	return 0;
//}
