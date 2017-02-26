#include "yctrlbase.h"

cyctrlbase::cyctrlbase(void)
:m_relay_message(0),
m_nToolTipTimer(500),
m_bNowDrag(False),
m_nLButtonLongTimer(0),
m_bNowKeyRepeat(False),
m_hWnd(0),
m_bIsCreated(False),
m_queueToolTip(0),
m_bIsMouseNoInput(False),
m_bIsToolTipLock(False),
m_x(0),
m_y(0),
m_bMouseTrack(True)

{
	memset(&m_ptMouseDown, 0, sizeof(m_ptMouseDown));
	memset(&m_ptMouseMove, 0, sizeof(m_ptMouseMove));
	m_ptToolTip.x=-1;
	m_ptToolTip.y=-1;
	m_ptDragOn.x=-1;
	m_ptDragOn.y=-1;

}

cyctrlbase::~cyctrlbase(void)
{
	//IsWindow(m_hWnd);
	//DestroyWindow(m_hWnd);
}

const char* __pszClassName__ = "yictrlbaseclass";

// --------------------------------------------
BOOL cyctrlbase::Create(HWND hWndParent, int id, int x, int y, int w, int h, BOOL IsHide)
{
	if (m_bIsCreated==True)
	{
		return True;
	}

	WNDCLASSEX wcex;

	m_hInstance=(HINSTANCE)::GetWindowLong(hWndParent, GWL_HINSTANCE);
	m_hParentWnd = hWndParent;
	m_bMouseTrackMode = FALSE;
	m_bPushed=FALSE;

	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CTRLS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)::GetStockObject(NULL_BRUSH);//(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= __pszClassName__;
	wcex.hIconSm		= NULL;

	if (RegisterClassEx(&wcex)==0)
	{
		//return FALSE;
	}

	m_id = id;
	m_hWnd = CreateWindow(__pszClassName__,
							NULL,//タイトルバーにこの名前が表示されます
							WS_CHILD |WS_CLIPSIBLINGS,    //ウィンドウの種類
							x,    //Ｘ座標
							y,    //Ｙ座標
							w,    //幅
							h,    //高さ
							hWndParent,            //親ウィンドウのハンドル、親を作るときはNULL
							(HMENU)id, //メニューハンドル、子供のID
							m_hInstance,            //インスタンスハンドル
							this);
	if (m_hWnd==NULL)
	{
		return FALSE;
	}
	m_bIsCreated=True;

	return TRUE;
}

// ------------------------------------
Bool cyctrlbase::CreateFromReosurce(HWND hWndParent, HINSTANCE hInstance, int id, Bool bIsModal/*=False*/)
{
	if (hInstance==0)
	{
		hInstance=(HINSTANCE)GetWindowLong(hWndParent, GWL_HINSTANCE);
	}
	
	m_hInstance=hInstance;
	m_hParentWnd=hWndParent;

	m_bIsModal=bIsModal;
	if (bIsModal==False)
	{
		m_hWnd = CreateDialogParamA(hInstance, MAKEINTRESOURCEA(id), hWndParent, __dlgproc__, (LPARAM)this);
		return m_hWnd ? True:False;
	}
	
	if(DialogBoxParamA(hInstance, MAKEINTRESOURCEA(id), hWndParent, __dlgproc__, (LPARAM)this)==-1)
	{
		return False;
	}

	return True;
}


// --------------------------------------------
BOOL cyctrlbase::CreatePopup(HWND hWndParent, int x, int y, int w, int h, BOOL IsHide)
{
	if (m_bIsCreated==True)
		return True;

	WNDCLASSEX wcex;

	m_hInstance=(HINSTANCE)::GetWindowLong(hWndParent, GWL_HINSTANCE);
	//m_hResourceModule = hResourceModule ? hResourceModule : (HANDLE)m_hInstance;
	m_hParentWnd = hWndParent;
	m_bMouseTrackMode = FALSE;
	m_bPushed=FALSE;

	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CTRLS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)::GetStockObject(NULL_BRUSH);//(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= __pszClassName__;
	wcex.hIconSm		= NULL;

	if (RegisterClassEx(&wcex)==0)
	{
		//return FALSE;
	}

	//m_id = id;
	m_hWnd = CreateWindow(__pszClassName__,
							NULL,//タイトルバーにこの名前が表示されます
							WS_POPUP | ((IsHide==FALSE)?WS_VISIBLE:0),    //ウィンドウの種類
							x,    //Ｘ座標
							y,    //Ｙ座標
							w,    //幅
							h,    //高さ
							hWndParent,            //親ウィンドウのハンドル、親を作るときはNULL
							(HMENU)0, //メニューハンドル、子供のID
							m_hInstance,            //インスタンスハンドル
							this);
	if (m_hWnd==NULL)
	{
		return FALSE;
	}

	if (IsHide==FALSE)
	{
		::ShowWindow(m_hWnd, SW_SHOWNORMAL);
		::UpdateWindow(m_hWnd);
	}

	m_bIsCreated=TRUE;
	return TRUE;
}



// ------------------------------------
void cyctrlbase::DrawBase()
{
}


// ------------------------------------
void cyctrlbase::Destroy()
{
	DestroyWindow(m_hWnd);
	m_hWnd=0;


	m_bIsCreated=False;

}

// ------------------------------------
Bitmap* cyctrlbase::GetBehindBitmap()
{
	RECT rect;
	GetWindowRect(m_hWnd, &rect);

	HDC hdc = GetDC();

	Bitmap* ret = cyiBitmapBlender::CreateBitmap(hdc, rect.left, rect.top, rect.right, rect.bottom);

	ReleaseDC(hdc);

	return ret;
}

// ------------------------------------
int cyctrlbase::RegisterRelayMessage(UINT msg, HWND hWndRelay)
{
	sRelayMessage xmsg = {msg, hWndRelay};

	if (m_relay_message==0)
		m_relay_message=new cyiList();
	m_relay_message->addref(yitoheap(&xmsg, sizeof(xmsg)));

	return m_relay_message->count();
}


// ------------------------------------
Bitmap* cyctrlbase::MakeBmpBackground(Bitmap* _bg, float edge, HandleShadow hShadow)
{
	Bitmap* bg=0;
	RECT rect;
	int w, h;
	int _w, _h;
	Bool bIsBgDelete=False;
	int nShadowWidth=cyiShape::GetShadowSize(hShadow);

	::GetClientRect(m_hWnd, &rect);
	_w=rect.right;
	_h=rect.bottom;

	w=_w-(nShadowWidth<<1);
	h=_h-(nShadowWidth<<1);


	Bitmap* bmpFrame = cyiShape::DrawRoundRect3D(xARGB(128, 200,200,200), xARGB(128, 0, 0, 0), RectF(0, 0, w, h), 2.0);
	Bitmap* bmpPannel = cyiShape::FillRoundRect(&LinearGradientBrush(PointF(0, 0), PointF(0, h), Color(0, 0,0,0), Color(64, 255,255,255)), RectF(0, 0, w, h), 2.0);
	Bitmap* bmpMask = cyiShape::FillRoundRect(&SolidBrush(Color(255, 0,0, 0)), RectF(0, 0, w, h), 2.0);
	Bitmap* shadow = cyiShape::MakeShadowBitmap(hShadow, _w, _h);

	if (_bg==0)
	{
		bg = GetBehindBitmap();
		bIsBgDelete=True;
	}
	else
	{
		bg = _bg->Clone(0, 0, _bg->GetWidth(), _bg->GetHeight(), PixelFormat32bppARGB);
	}
	Graphics gbg(bg);

	gbg.DrawImage(shadow, 0, 0);
	gbg.DrawImage(bmpPannel, nShadowWidth, nShadowWidth);
	gbg.DrawImage(bmpFrame, nShadowWidth, nShadowWidth);
	//bg = cyiBitmapBlender::SetMask(bg, bmpMask, 0, 0, False);

	//Graphics g(shadow);

	//g.DrawImage(bg, nShadowWidth, nShadowWidth);


	delete bmpPannel;
	delete bmpMask;
	delete bmpFrame;
	delete shadow;

	return bg;
}

// ------------------------------------
Bool cyctrlbase::Move(UINT x, UINT y, Bool bIsActivate)
{
	if (m_x==x &&
			m_y==y)
	{
		return False;
	}

	SetWindowPos(*this, 0, x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER|(bIsActivate?SWP_NOACTIVATE:0));
	m_x=x;
	m_y=y;

	return True;
}

// ------------------------------------
Bool cyctrlbase::ReSizeClientRect(UINT x, UINT y, Bool bIsRepaint)
{
	RECT rectWindow;
	RECT rectClient;
	int dx, dy;

	::GetWindowRect(m_hWnd, &rectWindow);
	::GetClientRect(m_hWnd, &rectClient);

	rectWindow.right-=rectWindow.left;
	rectWindow.bottom-=rectWindow.top;

	dx = rectWindow.right-rectClient.right;
	dy = rectWindow.bottom-rectClient.bottom;

	return ReSize(x==0?m_w:x+dx, y==0?m_h:y+dy, bIsRepaint);
}


// ------------------------------------
Bool cyctrlbase::ReSize(UINT x, UINT y, Bool bIsRepaint)
{
	if (x==m_w && y==m_h)
	{
		// 変化なし
		return False;
	}

	m_w=x;
	m_h=y;

	::SetWindowPos(*this, 0, 0, 0, x, y, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);//(bIsRepaint?SWP_NOACTIVATE:0));

	if (m_w!=0 &&
			m_h!=0)
	{
		if (bIsRepaint==True)
		{
			InvalidateRect(NULL);
		}
	}

	return True;
}

// ------------------------------------
void cyctrlbase::Show()
{
	::ShowWindow(*this, SW_SHOW);
}

// ------------------------------------
void cyctrlbase::Hide()
{
	::ShowWindow(*this, SW_HIDE);
}

// ------------------------------------
HDC cyctrlbase::GetDC()
{
	return ::GetDC(m_hWnd);
}

// ------------------------------------
void cyctrlbase::ReleaseDC(HDC hdc)
{
	::ReleaseDC(m_hWnd, hdc);
}


// ------------------------------------
Bool cyctrlbase::InvalidateRect(RECT* lpRect, Bool bErase/*=False*/,  Bool bIsDirect)
{
	Bool res = ::InvalidateRect(m_hWnd, lpRect, bErase);
	//::PostMessage(m_hWnd, WM_PAINT, 0, 0);

	return res;
}

// ------------------------------------
Bool cyctrlbase::GetClientRect(LPRECT lpRect)
{
	return ::GetClientRect(m_hWnd, lpRect);
}


// ------------------------------------
LRESULT cyctrlbase::OnDestroy(WPARAM wp, LPARAM lp)
{ 

	return 0;
}

// 
LRESULT cyctrlbase::OnMessageInterrupt(UINT msg, WPARAM wp, LPARAM lp, BOOL& IsHandled)
{
	IsHandled=False;
	return 0;
}

// ------------------------------------
void cyctrlbase::GetDragStartPos(int&x, int&y)
{
	x=m_ptDragOn.x;
	y=m_ptDragOn.y;
}

// ------------------------------------
//LRESULT cyctrlbase::OnDragStart(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}
//
//// ------------------------------------
//LRESULT cyctrlbase::OnDrag(WPARAM wParam, LPARAM lParam, Bool& handled)
//{
//	return 0;
//}
//
//// ------------------------------------
//LRESULT cyctrlbase::OnDragEnd(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}

// ------------------------------------
LRESULT cyctrlbase::OnToolTipTimer(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// --------------------------------------
LRESULT cyctrlbase::OnPreMessage(UINT msg, WPARAM wp, LPARAM lp, Bool bIsDialog)
{
	Bool bIsInterrupted=False;
	OnMessageInterrupt(msg, wp, lp, bIsInterrupted);

	if (m_relay_message)
	{
		for (cyiNode* node=m_relay_message->GetNodeTopFrom(); node; node=node->np)
		{
			sRelayMessage* xmsg = (sRelayMessage*)node->ref;

			if (xmsg && 
					xmsg->msg==msg)
			{
				return SendMessage(xmsg->hRelayWnd, msg, wp, lp);
			}
		}
	}

	if (bIsInterrupted==True)
	{
		return 0;
	}
	
	if (msg==WM_PAINT)
	{
		PAINTSTRUCT ps;

		HDC hdc = ::BeginPaint(m_hWnd, &ps);

		if (OnPaint(hdc, &ps)==FALSE)
		{
			::EndPaint(m_hWnd, &ps);

			if (bIsDialog==False)
			{
				return 0;
			}
			else
			{
				return False;
			}
			//return ::CallWindowProc(DefWindowProc, m_hWnd, msg, wp, lp);
		}
		::EndPaint(m_hWnd, &ps);

		if (bIsDialog==False)
		{
			return ::CallWindowProc(DefWindowProc, m_hWnd, msg, wp, lp);
		}
		return False;
	}

	switch(msg)
	{
		case WM_DESTROY:
		{
			OnDestroy(wp, lp);
			::SetWindowLong(m_hWnd, GWL_USERDATA, 0);
			//delete this;
			return ::CallWindowProc(DefWindowProc, m_hWnd, msg, wp, lp);
		}
		case WM_ERASEBKGND:
		{
				// 何も処理しない⇒塗り潰した
				return 0;
		}

		case WM_MOUSEMOVE:
		{
			m_bIsMouseNoInput=False;
			m_ptMouseMove.x=lp&0xFFFF;
			m_ptMouseMove.y=(lp>>16)&0xFFFF;

			m_ptMouseDown=m_ptMouseMove;

			if (m_bNowKeyRepeat)
			{
				EndLButtonRepeat();
			}

			// ドラッグ時
			{
				Bool handled=False;

				if (m_bNowDrag!=0)
				{
					POINT pt;	
					
					GetCursorPos(&pt);
					ScreenToClient(m_hWnd, &pt);
					//lp = ((pt.x&0xFFFF)|(pt.y<<16));

					if (wp&MK_LBUTTON)
					{
						if (SendMessage(m_hWnd, WM_GLS_DRAG, pt.x, pt.y)!=0)
						//if (OnDrag(pt.x, pt.y, handled)!=0)
						{
							return 0;
						}
					}
					else
					{
						//不明
						if (SendMessage(m_hWnd, WM_GLS_DRAG, pt.x, pt.y)!=0)
						//if (OnDragEnd(pt.x, pt.y)!=0)
						{
							return 0;
						}
					}
				}
				
				if (handled==True)
				{
					// MouseMoveイベントを発生させない。
					return 0;
				}
			}

			if (m_nToolTipTimer!=0 &&
					m_bIsToolTipLock==False)
			{
				//if (m_queueToolTip==0)
				//	m_queueToolTip=new cyiQueue();
				//
				//m_queueToolTip->enq(((m_ptMouseMove.x<<16) | m_ptMouseMove.y));

				//xTRACE("ToolTipTimer..... (%d, %d)", m_ptMouseMove.x, m_ptMouseMove.y);
				//m_ptToolTip=m_ptMouseMove;
				m_bIsMouseNoInput=True;
				SetTimer(m_hWnd, WM_MOUSEMOVE, m_nToolTipTimer, 0);
			}


			if (m_bMouseEnter==TRUE)
			{
				break;
			}

			if (m_bMouseTrack==False)
				break;


			m_bMouseEnter=TRUE;
			TRACKMOUSEEVENT tme;

			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = m_hWnd;

			if (TrackMouseEvent(&tme)==FALSE)
			{
				m_bMouseTrackMode=FALSE;
			}
			if (m_bMouseTrackMode==TRUE)
			{
				OnMouseEnter(wp, lp);
			}
			break;
		}
		case WM_MOUSELEAVE:
		{
			m_bIsMouseNoInput=False;
			m_ptMouseMove.x=-1;
			m_ptMouseMove.y=-1;
			m_bMouseEnter=FALSE;
			m_bPushed=FALSE;
			OnMouseLeave(wp, lp);

			//if (m_bNowDrag)
			//{
			//	m_bNowDrag=False;
			//	if (OnDragEnd(wp, lp)!=0)
			//	{
			//		return 0;
			//	}
			//}
			//m_bNowDrag=False;

			break;
		}
		case WM_LBUTTONDOWN:
		{
			m_bPushed=TRUE;

			GetCursorPos(&m_ptMouseDown);
			ScreenToClient(m_hWnd, &m_ptMouseDown);

			if (m_nLButtonLongTimer)
			{
				m_ptLButtonLong=m_ptMouseDown;
				SetTimer(m_hWnd, WM_LBUTTONDOWN, m_nLButtonLongTimer, 0);
			}

			//if (m_bNowDrag==False)
			//{
			//	if (m_nDragTimer==0)
			//	{
			//		if (StartDrag(m_ptMouseDown.x, m_ptMouseDown.y)==True)
			//		{
			//			return 0;
			//		}
			//	}
			//	else if (m_nLButtonLongTimer!=-1)// && m_ptDragOn.x==-1 && m_ptDragOn.y==-1)
			//	{
			//		m_ptLButtonLong=m_ptMouseDown;
			//		//m_ptDragOn=m_ptMouseDown;
			//		SetTimer(m_hWnd, WM_LBUTTONDOWN, m_nLButtonLongTimer, 0);
			//	}
			//}			
			break;
		}
		case WM_LBUTTONUP:
		{
			if (m_bNowKeyRepeat)
			{
				EndLButtonRepeat();
			}

			if (m_bPushed==TRUE)
			{
				OnClicked(wp, lp);
			}
			m_ptMouseDown.x=-1;
			m_ptMouseDown.y=-1;

			if (m_bNowDrag!=0)
			{
				if (EndDrag()==True)
				{
					m_bPushed=FALSE;
					return 0;
				}
			}
			m_bPushed=FALSE;
			break;
		}
		case WM_TIMER:
		{
			if (wp==WM_LBUTTONDOWN)
			{
				KillTimer(m_hWnd, WM_LBUTTONDOWN);
				if (m_bPushed==True)
				{

					if (m_ptLButtonLong.x==m_ptMouseDown.x &&
							m_ptLButtonLong.y==m_ptMouseDown.y)
					{
						if (OnLButtonLongPressed(m_ptLButtonLong.x ,m_ptLButtonLong.y)==False)
						{
							break;
						}
					}
				}
				break;
			}
			else if (wp==WM_LBUTTONUP) // キーリピートタイマー
			{
				OnLButtonRepeated(m_ptLButtonLong.x, m_ptLButtonLong.y);
				break;
			}
			else if (wp==WM_MOUSEMOVE)
			{
				// 座標が入る。
				KillTimer(m_hWnd, WM_MOUSEMOVE);
				POINT pt;
				//yiAnyValue val;
				//int xpt;
				//if (m_queueToolTip==0)
				//{
				//	break;
				//}

				GetCursorPos(&pt);
				ScreenToClient(m_hWnd, &pt);
	
				//if (m_queueToolTip->deq(val)==False)
				//{
				//	break;
				//}

				//xpt=val;
				//xTRACE("OnTimer..... current (%d, %d) : (%d, %d)", pt.x, pt.y, ((xpt>>16)&0xFFFF), xpt&0xFFFF);

				//if (((xpt>>16)&0xFFFF)==pt.x &&
				//		(xpt&0xFFFF)==pt.y)
				//if (m_ptMouseMove.x==m_ptToolTip.x &&
				//		m_ptMouseMove.y==m_ptToolTip.y)

				if (m_bIsMouseNoInput==True)
				{
					//lp = (m_ptMouseMove.x | (m_ptMouseMove.y<<16));
					OnToolTipTimer(pt.x, pt.y);
				}
				//m_ptToolTip.x=-1;
				//m_ptToolTip.y=-1;
			}

			break;
		}
		case WM_NOTIFY:
		{
			Bool bIsHandled=False;
			LRESULT res = OnNotify(wp, lp, bIsHandled);
			return res;
		}
		case WM_COMMAND:
		{
			Bool bIsHandled=False;
			LRESULT res = OnCommand(wp, lp, bIsHandled);

			if (bIsHandled==False)
			{
				OnNotHandledCommand(wp, lp);
			}
			return 0;
		}

		default:
			break;
	}

	BOOL IsHandled=FALSE;

	LRESULT res = OnMessage(msg, wp, lp, IsHandled);

	if (IsHandled==FALSE)
	{
		if (bIsDialog==False)
		{
			return ::CallWindowProc(DefWindowProc, m_hWnd, msg, wp, lp);
		}
	}
	return IsHandled;
}



// --------------------------------------
LRESULT CALLBACK cyctrlbase::__wndproc__(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	
	if (msg==WM_CREATE)
	{

		LPCREATESTRUCT p = (LPCREATESTRUCT)lp;
		cyctrlbase* pClass = (cyctrlbase*)p->lpCreateParams;

		::SetWindowLong(hWnd, GWL_USERDATA, (LONG)pClass);
		pClass->m_hWnd=hWnd;

		//RECT rect;
		//GetClientRect(m_hWnd, &rect);
		//m_w=rect.right;
		//m_h=rect.bottom;

		RECT rect;
		GetWindowRect(hWnd, &rect);

		HWND hParent=GetParent(hWnd);
		pClass->m_w=rect.right-rect.left;
		pClass->m_h=rect.bottom-rect.top;

		if (hParent)
		{
			ScreenToClient(hParent, (LPPOINT)&rect);
		}
		pClass->m_x=rect.left;
		pClass->m_y=rect.top;
		pClass->m_bIsCreated=True;
		return pClass->OnCreate(wp, lp);
		//return ::CallWindowProc(::DefWindowProc, hWnd, msg, wp, lp);
	}


	cyctrlbase* pClass = (cyctrlbase*)::GetWindowLong(hWnd, GWL_USERDATA);

	if (pClass)
	{
		return pClass->OnPreMessage(msg, wp, lp);
	}

	return ::CallWindowProc(::DefWindowProc, hWnd, msg, wp, lp);

}


// --------------------------------------
BOOL cyctrlbase::OnPreMessageDlg(UINT msg, WPARAM wp, LPARAM lp)
{
	if (msg==WM_PAINT)
	{
		PAINTSTRUCT ps;

		HDC hdc = ::BeginPaint(m_hWnd, &ps);
		if (OnPaint(hdc)==FALSE)
		{
			::EndPaint(m_hWnd, &ps);
			return FALSE;
		}
		::EndPaint(m_hWnd, &ps);
		return TRUE;
	}

	switch(msg)
	{
		case WM_INITDIALOG:
		{
			m_bIsCreated=True;
			OnCreate(wp, lp);
			return TRUE;
		}
		case WM_DESTROY:
		{
			OnDestroy(wp, lp);
			::SetWindowLong(m_hWnd, GWL_USERDATA, 0);
			//delete this;
			return FALSE;
		}
		case WM_MOUSEMOVE:
		{
			if (m_bMouseTrack==FALSE)
			{
				break;
			}
			if (m_bMouseEnter==TRUE)
			{
				break;
			}
			m_bMouseEnter=TRUE;
			TRACKMOUSEEVENT tme;

			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = m_hWnd;
			if (::TrackMouseEvent(&tme)==FALSE)
			{
				m_bMouseTrackMode=FALSE;
			}
			if (m_bMouseTrackMode==TRUE)
			{
				OnMouseEnter(wp, lp);
			}
			return TRUE;
		}
		case WM_MOUSELEAVE:
		{
			m_bMouseEnter=FALSE;
			m_bPushed=FALSE;
			OnMouseLeave(wp, lp);
			return TRUE;
		}
		case WM_LBUTTONDOWN:
		{
			m_bPushed=TRUE;
			return TRUE;
		}
		case WM_LBUTTONUP:
		{
			if (m_bPushed==TRUE)
			{
				OnClicked(wp, lp);
			}

			m_bPushed=FALSE;
			return TRUE;
		}
		case WM_COMMAND:
		{
			//Bool bIsHandled=False;
			//OnCommand(wp, lp, bIsHandled);

			//if (bIsHandled==False)
			//{
			//	if (wp==IDOK)
			//		OnOK();
			//	else if (wp==IDCANCEL)
			//		OnCancel();
			//}
			//return TRUE;
		}

		default:
			break;
	}

	BOOL IsHandled=FALSE;

	LRESULT res = OnMessage(msg, wp, lp, IsHandled);

	return IsHandled;
}

// --------------------------------------------
//LRESULT cyctrlbase::OnOK()
//{
//	if (m_bIsModal==True)
//	{
//		::EndDialog(m_hWnd, 0);
//	}
//	else
//	{
//		::DestroyWindow(m_hWnd);
//	}
//	return 0;
//}
//
//// --------------------------------------------
//LRESULT cyctrlbase::OnCancel()
//{
//	if (m_bIsModal==True)
//	{
//		::EndDialog(m_hWnd, 0);
//	}
//	else
//	{
//		::DestroyWindow(m_hWnd);
//	}
//	return 0;
//}

// --------------------------------------
INT_PTR CALLBACK cyctrlbase::__dlgproc__(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	cyctrlbase* pClass = (cyctrlbase*)::GetWindowLong(hWnd, GWL_USERDATA);

	if (msg==WM_INITDIALOG)
	{
		cyctrlbase* pClass = (cyctrlbase*)lp;

		if (pClass==0)
		{
			return False;
		}
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)pClass);
		pClass->m_hWnd=hWnd;
		//return pClass->OnPreMessage(msg, wp, lp, True);
		return pClass->OnCreate(wp, lp);
	}

	if (pClass==NULL)
	{
		return FALSE;
	}
	if (pClass)
	{
		return pClass->OnPreMessage(msg, wp, lp, True);
	}

	pClass->m_hWnd=hWnd;
	return pClass->OnPreMessage(msg, wp, lp, True);
}


// ------------------------------------
void cyctrlbase::SetToolTipTimer(int n)
{
	m_nToolTipTimer=n;
}

// ------------------------------------
void cyctrlbase::SetLButtonLongTimer(int n)
{
	m_nLButtonLongTimer=n;
}

// ------------------------------------
Bool cyctrlbase::StartDrag(int x, int y)
{
	m_ptDragOn.x=x;
	m_ptDragOn.y=y;//=m_ptMouseDown;
	SetCapture(m_hWnd);
	
	if ((m_bNowDrag=SendMessage(m_hWnd, WM_GLS_DRAG_START, x, y))!=0)
	{
		m_bNowDrag=True;
		return True;
		//m_bNowDrag = OnDragStart(0, lp);
	}
	m_ptDragOn.x=-1;
	m_ptDragOn.y=-1;
	return False;
}

// ------------------------------------
Bool cyctrlbase::EndDrag()
{
	POINT pt;	
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	if (SendMessage(m_hWnd, WM_GLS_DRAG_END, pt.x, pt.y)!=0)
	{
		ReleaseCapture();
		m_bNowDrag=0;

		return True;
	}

	//OnDragEnd(pt.x, pt.y);
	ReleaseCapture();
	m_bNowDrag=0;

	return False;

}

// ------------------------------------
Bool cyctrlbase::StartLButtonRepeat(int x, int y, int nMillisec)
{
	//m_nKeyRepeatTimer=nMillisec;
	m_bNowKeyRepeat=True;
	SetTimer(m_hWnd, WM_LBUTTONUP, nMillisec, 0);
	return True;
}

// ------------------------------------
Bool cyctrlbase::EndLButtonRepeat()
{
	m_bNowKeyRepeat=False;
	KillTimer(m_hWnd, WM_LBUTTONUP);
	return True;
}


// ------------------------------------
Bool cyctrlbase::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	return 0;
}

// ------------------------------------
typedef struct xsGlassItem {
	Bitmap* bmpPannel;
	Bitmap* bmpMask;
	Bitmap* bmpShadow;
	HandleShadow hShadow;
} sGlassItem;
#define xARGB(a, r, g, b) ((a<<24)|(r<<16)|(g<<8)|b)
// ------------------------------------
HandleGlassItem cyctrlbase::CreateGlassPannelObject(Bitmap* bmp, int w, int h, float edge, Brush* brush, Bool bIsLike3D, HandleShadow hShadow)
{
	sGlassItem* item = (sGlassItem*)yialloc(sizeof(sGlassItem));
	
	Bitmap* bmpFrame = cyiShape::DrawRoundRect3D(xARGB(128, 200,200,200), xARGB(128, 0, 0, 0), RectF(0, 0, w, h), 2.0);
	item->bmpPannel = cyiShape::FillRoundRect(&LinearGradientBrush(PointF(0, 0), PointF(0, h), Color(0, 0,0,0), Color(64, 255,255,0)), RectF(0, 0, w, h), 2.0);
	item->bmpMask = cyiShape::FillRoundRect(&SolidBrush(Color(255, 0,0, 0)), RectF(0, 0, w, h), 2.0);
	item->hShadow=hShadow;

	int shadow_width = 0;
	
	if (hShadow)
		shadow_width=cyiShape::GetShadowSize(hShadow);

	item->bmpShadow=0;

	if (hShadow)
		item->bmpShadow = cyiShape::MakeShadowBitmap(hShadow, w+(shadow_width<<1), h+(shadow_width<<1));

	Graphics g(item->bmpPannel);
	g.DrawImage(bmpFrame, 0, 0);
	delete bmpFrame;

	g.DrawImage(bmp, (INT)(w-bmp->GetWidth())/2, (INT)(h-bmp->GetHeight())/2);

	return (HandleGlassItem)item;
}

// ------------------------------------
Bitmap* cyctrlbase::MakeGlassPannel(HandleGlassItem hItem, Bitmap* bg, int x, int y)
{
	sGlassItem* item = (sGlassItem*)hItem;

	int w = item->hShadow?item->bmpShadow->GetWidth():item->bmpPannel->GetWidth()+x;
	int h = item->hShadow?item->bmpShadow->GetHeight():item->bmpPannel->GetHeight()+y;
	int shadow_w = item->hShadow?cyiShape::GetShadowSize(item->hShadow):x;
	Bitmap* bg_masked=cyiBitmapBlender::SetMask(bg, item->bmpMask, 0, 0);

	Bitmap* ret = new Bitmap(w+1, h+1);
	Graphics g(ret);
	

	g.DrawImage(bg_masked, shadow_w, shadow_w);

	if (item->hShadow!=0)
	{
		g.DrawImage(item->bmpShadow, 0, 0);
	}

	g.DrawImage(item->bmpPannel, shadow_w, shadow_w);

	return ret;
}

// ------------------------------------
void cyctrlbase::DestroyGlassPannelObject(HandleGlassItem hItem)
{
	sGlassItem* item = (sGlassItem*)hItem;

	delete item->bmpMask;
	delete item->bmpPannel;
	delete item->bmpShadow;

	yifree(item);
}

LPDLGTEMPLATE cyctrlbase::CreateDialogTemplate(int x, int y, int w, int h, wchar_t* caption, int style, int styleex, 
											   int fontSize, wchar_t* fontname, int btn)
{
    HGLOBAL hgbl;
    LPDLGTEMPLATE lpdt;
    //LPDLGITEMTEMPLATE lpdit;
    LPWORD lpw;
    LPWSTR lpwsz;
    int nChar;

    // 実際は、サイズはデータが収まりきるように
    // あらかじめ計算しておく必要があります。
    // GMEM_ZEROINIT であらかじめデータを 0 に設定しておきます。
    // (省略されているメンバは 0 になります)
    lpdt = (LPDLGTEMPLATE)yialloci(1024);//GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, 1024);
    if (!hgbl)
        return 0;

    // ダイアログボックスの初期値を設定します。
    // ※ hgbl と lpdt のアドレスは同じになります (GMEM_FIXED のため)。
    //lpdt = (LPDLGTEMPLATE) GlobalLock(hgbl);
    lpdt->style = style;  // フォント付き
    lpdt->cdit = 0;  // コントロールの数
    lpdt->x = x;   lpdt->y = y;
    lpdt->cx = w; lpdt->cy = h;

    // 次のメンバの設定に移ります。
    lpw = (LPWORD) (lpdt + 1);

    // <Menu ID>
    //   *lpw++ = の文は、その位置に値を設定した後 ++ を行います。
    //   (つまり *lpw = 0; lpw++; と同じ)
    //   正式には lpw++ がインクリメントする前のアドレスを返し、
    //   そのアドレスに対して *XXX = 0 を行っています。
    *lpw++ = 0;

    // <Window Class>
    *lpw++ = 0;

    // <Dialog Title>
    lpwsz = (LPWSTR) lpw;

    // MultiByteToWideChar 関数で ANSI 文字列を Unicode 文字列に変換します。
    // 戻り値 (書き込んだ文字列の長さ) に NULL 文字が含まれているので + 1 しません。
	nChar = wcslen(caption)+1;//MultiByteToWideChar(CP_ACP, 0, "ダイアログ テスト", -1, lpwsz, 50);
	::wcscpy(lpwsz, caption);


    // lpw = (LPWORD) lpwsz は必要ありません (アドレスが同じであるため)。
    lpw += nChar;

    // WORD fontSize
    // ※ DS_SETFONT を指定しなかった場合、コントロールの設定に
    // 　 移るまでのデータ設定は行いません。
    *lpw++ = fontSize;

    // WCHAR fontName[]
    lpwsz = (LPWSTR) lpw;
    nChar =::wcslen(fontname);
    lpw += nChar;
	wcscpy(lpwsz, fontname);

    // コントロールの設定に移ります。
    //   移る前に DWORD 境界に揃えます。
    lpw = (LPWORD) (((IntPtr) lpw + 3) & ~((IntPtr) 3));
    //lpdit = (LPDLGITEMTEMPLATE) lpw;

    //// OK ボタンを作ります。
    //lpdit->x = 10;  lpdit->y = 70;
    //lpdit->cx = 60; lpdit->cy = 14;
    //lpdit->id = IDOK;  // OK ボタンの ID
    //lpdit->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;

    //// 次のメンバの設定に移ります。
    //lpw = (LPWORD) (lpdit + 1);

    //// <Window Class>
    ////   これでも構いません。
    ////     lpwsz = (LPWSTR) lpw;
    ////     nChar = MultiByteToWideChar(CP_ACP, 0, "Button", -1, lpwsz, 50);
    ////     lpw += nChar;
    //*lpw++ = 0xFFFF;  // 数値指定に必要
    //*lpw++ = 0x0080;  // ボタンのウィンドウクラスを指定します。

    //// <Control Text>
    //lpwsz = (LPWSTR) lpw;
    //nChar = MultiByteToWideChar(CP_ACP, 0, "OK", -1, lpwsz, 50);
    //lpw += nChar;

    //// WORD SizeOfCreationData
    ////   特にデータは無いため、0 で終わります。
    ////   ※ データがある場合、データのサイズ + sizeof(WORD) (= 2) を設定します。
    //*lpw++ = 0;

    //// 次のコントロールの設定に移ります。
    //lpw = (LPWORD) (((DWORD) lpw + 3) & ~((DWORD) 3));
    //lpdit = (LPDLGITEMTEMPLATE) lpw;

    //lpdit->x = 10;  lpdit->y = 10;
    //lpdit->cx = 40; lpdit->cy = 9;
    //lpdit->id = -1;  // スタティックコントロールによくある ID (IDC_STATIC = -1)
    //lpdit->style = WS_CHILD | WS_VISIBLE | SS_LEFT;

    //lpw = (LPWORD) (lpdit + 1);

    //// <Window Class>
    ////   これでも構いません。
    ////     lpwsz = (LPWSTR) lpw;
    ////     nChar = MultiByteToWideChar(CP_ACP, 0, "Static", -1, lpwsz, 50);
    ////     lpw += nChar;
    //*lpw++ = 0xFFFF;
    //*lpw++ = 0x0082;  // スタティックコントロールのウィンドウクラスを指定します。

    //// <Control Text>
    //lpwsz = (LPWSTR) lpw;
    //nChar = MultiByteToWideChar(CP_ACP, 0, "メッセージ", -1, lpwsz, 50);
    //lpw += nChar;

    // WORD SizeOfCreationData
    //   特にデータは無いため、0 で終わります。
    *lpw++ = 0;

    // データ設定は終了しました。ロックを解除します。
    //GlobalUnlock(hgbl);
	return lpdt;
}


#include "glsResourceBitmap.h"
// --------------------------------
cglsResourceObject* g_resourceObject=0;

HINSTANCE g_hInstance=0;
// --------------------------------------------
HINSTANCE cyctrlbase::GetModuleHandle()
{
	return g_hInstance;
}

#ifndef STATIC_COMPILE
#include "glsToolTip.h"
#include "yctrls.h"
// -------------------------------------
BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,  // DLL モジュールのハンドル
  DWORD fdwReason,     // 関数を呼び出す理由
  LPVOID lpvReserved   // 予約済み
)
{
	if (fdwReason==DLL_PROCESS_ATTACH)
	{
		// GDI+ 初期化
		//yictrl_initialize();
        InitCommonControls();

		g_resourceObject = new cglsResourceBitmap();
		g_hInstance=hinstDLL;
		yictrl_initialize();
		cglsToolTip::GetInstance(g_hInstance, &Font(L"メイリオ", 10.0));
		return TRUE;
	}
	else if (fdwReason==DLL_PROCESS_ATTACH)
	{
		return TRUE;
	}
	return TRUE;
}
#endif