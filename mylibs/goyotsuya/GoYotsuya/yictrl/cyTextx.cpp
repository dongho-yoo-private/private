#include "cyTextx.h"
#include "yiResource.h"
#include "cyctrlResource.h"
cyTextx::cyTextx(void)
{
}

cyTextx::~cyTextx(void)
{
}



// -------------------------------
Bool cyTextx::Create(cyctrlbase* ctrl, int id, int x, int y, int w, int nMaxLen, eTextxType type, char* buffer, Bool bIsShow, Bool bIsReadOnly)
{
	m_parent=ctrl;

	if (cyctrlbase::Create(ctrl->m_hWnd, 0, id, x, y, w, 1, bIsShow)==False)
	{
		return False;
	}
	

	m_szLabel[0]=0;

	m_bIsReadOnly=bIsReadOnly;
	m_type=type;
	m_buffer=buffer;
	m_nMaxLen = nMaxLen;
	return True;
}

// -------------------------------
Bool cyTextx::Create(cyctrlbase* ctrl, int id, const wchar_t* pszLabel, int w, int nMaxLen, eTextxType type, char* buffer, Bool bIsShow, Bool bIsReadOnly, int distance)
{
	m_parent=ctrl;

	if (pszLabel)
		wcscpy(m_szLabel, pszLabel);
	else
		m_szLabel[0]=0;

	m_bIsReadOnly=bIsReadOnly;
	m_type=type;
	m_buffer=buffer;
	m_nMaxLen = nMaxLen;
	m_distance=distance;

	if (cyctrlbase::Create(ctrl->m_hWnd, 0, id, 0, 0, w, 1, True)==False)
	{
		return False;
	}

	return True;
}

#include <imm.h>
#pragma comment(lib, "imm32.lib")

// -------------------------------
LRESULT cyTextx::OnCreate(WPARAM wp, LPARAM lp)
{
	HDC        hdc ;
	TEXTMETRIC tm ;
	int        iHeight ;


	//hdc = GetDC(m_hWnd) ;
	//SelectObject( hdc, GetStockObject( SYSTEM_FIXED_FONT)) ;
	//GetTextMetrics( hdc, &tm) ;
	//iHeight = ( tm.tmHeight + tm.tmExternalLeading) * 7 / 4 ;

	//ReleaseDC(m_hWnd, hdc);
	iHeight=24;

	//ReSize(m_w, iHeight);

	m_ctrlx.Create(this, 0, 0, 0, 0, 3, True, (Font*)cyiResource::GetInstance(eResourceTypex::eRTX_TEMPORARY)->Get(eRTID_DEFAULT_FONT));

	m_ctrlx.Add(0, 0, m_szLabel, 10, 0, 3, 0, 0, 0);

	//m_ctrlx.Create(this, 0, 0);

	int w, h;
	m_ctrlx.ArrangeItems(w, h);
	int ctrly = m_h > h ? (iHeight-h)>>1:0;

	m_ctrlx.Move(m_distance?m_distance-w:0, ctrly);

	int txty= iHeight > h ? 0:(h-iHeight)>>1;
	int txtw=m_w;
	ReSize(m_w+(m_distance?m_distance:w), iHeight > h ? iHeight:h);

	m_hText = CreateWindow( 
					"edit",                     // スタティックコントロールのクラス名
					"",							// 表示する文字列
					m_bIsReadOnly?ES_READONLY:0|((m_type==eTextxType::eTXT_TYPE_PASSWORD) ? ES_PASSWORD:0)|  ES_LEFT | WS_CHILD | WS_BORDER | WS_VISIBLE, 
												// ウインドウスタイル
					m_distance?m_distance:w, txty,                       // 左上の座標
					txtw, iHeight,            // 幅と高さ
					m_hWnd,                    // 親ウインドウのウインドウハンドル
					(HMENU)1024,                       // メニューハンドル。NULLでよい。
					(HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE),                  // アプリケーションのインスタンスハンドル。
					NULL                        // ウインドウ作成データ。NULLでよい
				) ;

	if (m_hText==0)
	{
		cyctrlbase::Destroy();
		return False;
	}
	//HIMC hImc = ImmGetContext(m_hText);
	//ImmSetOpenStatus(hImc, FALSE);
	//::SetFocus(m_hText);

	//ImmReleaseContext(m_hText, hImc);




	return 0;
}

// -------------------------------
void cyTextx::SetFocus()
{

	//ShowCaret(m_hText);
	::SetFocus(m_hText);
}




// -------------------------------
void cyTextx::SetText(const char* pszText)
{
	SetWindowTextA(m_hWnd, pszText);
}

// -------------------------------
char* cyTextx::GetText(char* pszText)
{
	GetWindowTextA(m_hText, pszText, 256);
	return pszText;
}

// -------------------------------
void cyTextx::SelectAll()
{
	SendMessage(m_hText, EM_SETSEL, 0, -1);
}

// -------------------------------
LRESULT cyTextx::OnTxtCommand(HWND hWnd, int cmdId)
{
	switch(cmdId)
	{
		case EN_UPDATE:
		{
			char buffer[256];
			buffer[0]=0;
			GetWindowTextA(m_hText, buffer, 256);

			int n = strlen(buffer);

			if (n>m_nMaxLen)
			{
				buffer[m_nMaxLen]=0;
				SetWindowTextA(m_hText, buffer);
				SendMessage(m_hText, EM_SETSEL, m_nMaxLen, m_nMaxLen);
			}

			strncpy(m_buffer, buffer, 256);
			break;
		}
		default:
			break;
	}
	return 0;
}

// ------------------------------------
Bool cyTextx::OnCtrlxDraw(int id, int subId, Bitmap* srf, Bitmap* bg, Font* fnt, sControlxII* s, eControlxEventII eventId)
{
	if (eventId==eCEII_NORMAL ||
			eventId==eCEII_INIT )
	{
		if (wcslen(s->name)==0)
		{
			return False;
		}

		if (s->pBmp[0]==0)
		{
			Bitmap* str =cyiShape::BitmapString(0, s->name, fnt, &SolidBrush(Color((ARGB)0xFF000000)), eSTS_GROW, Color::White, 3.0, 4.0);
			s->pBmp[0]=str;
		}

		int h = s->pBmp[0]->GetHeight();
		int stry=0;
		if (m_h>h)
		{
			stry=(m_h-h)>>1;
		}
		cyctrlbase::OnCtrlxDraw(id, subId, srf, bg, fnt, s, eventId);
		cyiBitmapBlender::RotateCopy(srf, s->pBmp[0], 0, s->rect.left, s->rect.top+stry, 0, 0, s->pBmp[0]->GetWidth(), s->pBmp[0]->GetHeight());
		return True;
	}
	else if (eventId==eCEII_DISABLE)
	{
		return True;
	}

	return False;
}

// ------------------------------------
void cyTextx::Move(UINT x, UINT y)
{
	if (x==m_x &&
			y==m_y)
	{
		return;
	}
	cyctrlbase::Move(x, y);
	InvalidateRect(m_hWnd, NULL, FALSE);
	m_ctrlx.Update(True);
}
