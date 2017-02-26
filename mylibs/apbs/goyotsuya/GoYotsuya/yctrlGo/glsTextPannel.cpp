#include "glsTextPannel.h"

cglsTextPannel::cglsTextPannel(void)
:m_currFocusEditor(0)
,m_currentNode(0)
,m_check(0)
,m_inputEditor(0)
{
	__GLASSINTERFACE_NAME__;
}

cglsTextPannel::~cglsTextPannel(void)
{
}

Bool cglsTextPannel::Create(HWND hWnd, int id, int x, int y, int w, int h, Bool bIsBlack, sGoHeader* header, eGlsTextPannelStyle style, Font* font)
{
	sGlsDialogDetail xdetail;
	//int nMaxLabelWidth = CreateLabels();

	//yiToUnicode(m_szPlayerName, pszPlayerName, 0);
	////wcscpy(m_szPlayerName, pszPlayerName);
	//cyctrlGobanResource::GetInstance().GetLevelString(nLevel, m_szLevel);
	//m_header=header;
	//m_eStyle=style;

	//xdetail.font=detail->font1;
	//m_font2=detail->font2;

	m_eStyle=style;

	cglsDialog::xDefaultDetail(&xdetail);
	xdetail.gauss.fShadowWidth=5.0;
	m_bgColor=Color::Beige;
	m_eStyle=style;
	xdetail.font=font;
	//m_font=(Font*)cyiShape::Clone(font);
	xdetail.bIsAnimationShow=False;
	return cglsDialog::Create(hWnd, id , 0, x, y, w, h, &xdetail);
}

// -----------------------------
void cglsTextPannel::SetTextPannelStyle(eGlsTextPannelStyle style)
{
	if (style==m_eStyle)
		return ;

	if (m_editor)
	{
		m_editor->SetText((wchar_t*)0);
	}
	m_eStyle=style;
	ReArrangeItemz();
}

// ------------------------------------
Bool cglsTextPannel::OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX, int offsetY)
{
	Bool bIsSizeChanged=False;
	int w;
	int h;
	int nRad;
	int nShadowWidth;
	int nClientWidth;
	int nClientHeight;
	Bool bIsClientAreaUpdated=False;
	RECT rectDrawBlure;
	int nDrawOrder = GetDrawOrder();

#ifdef _DEBUG
	xTRACE("cglsTextPannel::OnDrawMyself(%s, %d, %d) m_bmpBehind:%x, (%d, %d, %d, %d)", m_szClassName, nDrawOrder, m_glsDrawInterface->m_nRequestUpdateOrder, m_bmpBehind, childRect->left, childRect->top, childRect->right, childRect->bottom);
#endif
	if (m_bmpBluredBg!=0)
	{
		if (m_bmpBluredBg->GetWidth()!=m_w ||
				m_bmpBluredBg->GetHeight()!=m_h)
		{
			xTRACE("cglsTextPannel::OnDrawMyself() size is changed");
			bIsSizeChanged=True;
			goto FORCED;
		}
	}
	else
	{
		goto FORCED;
	}

	if (m_glsDrawInterface->m_nRequestUpdateOrder<=nDrawOrder)
	{
		goto FORCED;
	}

	return False;

	if (m_bmpBluredBg==0)
		goto FORCED;

	if (m_bmpBluredBg->GetWidth()!=m_w ||
			m_bmpBluredBg->GetHeight()!=m_h)
	{
		//delete m_bmpCurrent;
		//m_bmpCurrent = new BITMAP32(m_w, m_h);
		//m_item.pCurrentBmp=m_bmpCurrent;
		//bIsSizeChanged=True;
		goto FORCED;
	}

	m_item.pCurrentBmp=m_bmpCurrent;
	return False;

FORCED:
	
	w=m_w;
	h=m_h;
	nRad=__INTEGER__(m_fBlureRad);

	if (m_hShadow==0)
	{
		m_hShadow=cyiShape::CreateShadow((int)m_fShadowWidth, m_fEdge, 0.7, 0xFF000000);
	}

	if (m_bmpShadow==0 || bIsSizeChanged==True)
	{
		if (m_bmpShadow)
			delete m_bmpShadow;
		m_bmpShadow=cyiShape::MakeShadowBitmap(m_hShadow, w, h);
	}

	if (m_bmpBluredBg==0|| bIsSizeChanged==True)
	{
		if (m_bmpBluredBg)
			delete m_bmpBluredBg;

		m_bmpBluredBg=BITMAP32(m_w, m_h);
	}

	nShadowWidth = cyiShape::GetShadowSize(m_hShadow);
	cyiBitmapBlender::BitmapCopy(m_bmpBluredBg, m_bmpShadow);
	Bitmap* xbmp = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &SolidBrush(Color(m_bgColor)), RectF(0, 0, m_w-(nShadowWidth<<1), m_h-(nShadowWidth<<1)), m_fEdge);

	Graphics g(m_bmpBluredBg);
	g.DrawImage(xbmp, nShadowWidth-1, nShadowWidth-1);

	//Graphics gx(bmp);
	//gx.DrawImage(m_bmpBluredBg, 

	//cyiBitmapBlender::BitmapCopy(m_bmpBluredBg, m_bmpCurrent, 0, 0);//rectDraw.left-m_nAbsX, rectDraw.top-m_nAbsY, rectDraw.left, rectDraw.top, rectDraw.right, rectDraw.bottom);
	delete xbmp;

	return False;
}

// -----------------------------
void cglsTextPannel::SetText(wchar_t* buffer)
{
	m_editor->SetText(buffer);
}

// -----------------------------
wchar_t* cglsTextPannel::GetText(int* length)
{
	return 0;
}

// -----------------------------
void cglsTextPannel::AppendText(const wchar_t* buffer)
{
	m_editor->AppendTextEx(buffer, 0, 0, 0);
	//m_editor->AppendTextEx(buffer
}

// -----------------------------
void cglsTextPannel::SetEnableInput(Bool bIsEnable)
{
	m_editor->EnableInput(bIsEnable);
}


// -----------------------------
Bool cglsTextPannel::IsMessageInGobanVisible()
{
	if (m_check)
	{
		return m_check->GetChecked(0);
	}
	return False;
}

// -----------------------------
void cglsTextPannel::Update(Bitmap* bmp, RECT* rect, RECT* _childRect, int offsetX, int offsetY, Bool bIsLocalBufferUpdate)
{
	__super::Update(bmp, rect, _childRect, offsetX, offsetY, bIsLocalBufferUpdate);
}

// ------------------------------------
void cglsTextPannel::OnItemReArranged()
{
	RECT rect;
	GetClientRect(&rect);
	int delta = m_fEdge;
	
	m_editor->Move(rect.left+delta, rect.top+delta);
	m_inputEditor->Hide();

	if (m_eStyle==eGTPS_PLAY)
	{
		m_editor->ReSize(rect.right-(delta<<1), rect.bottom-(delta<<1)-m_inputEditor->m_h);
		//m_inputEditor->Show();
		m_inputEditor->Move(rect.left+delta+2, m_editor->m_y+m_editor->m_h);
		m_inputEditor->ReSize(m_editor->m_w-4, m_inputEditor->m_h);
		m_currFocusEditor=m_inputEditor;

		m_check->Move(rect.left+delta+2, m_editor->m_y+m_editor->m_h);
		m_check->Show();
	}
	else
	{
		m_editor->ReSize(rect.right-(delta<<1), rect.bottom-(delta<<1));
		m_inputEditor->Hide();
		m_check->Hide();
		m_currFocusEditor=m_editor;
	}
}

// --------------------------------------------
LRESULT cglsTextPannel::OnCreate(WPARAM wParam, LPARAM lParam)
{
	sGlsEditBoxDetail detail;
	cglsEditBox::xDefaultDetail(&detail);

	detail.bIsMutipleLine=True;
	detail.colorBg=m_bgColor;
	detail.colorBorder=0;
	m_editor = xcglsEditBox();//new cglsEditBox();

	m_editor->Create((HWND)this, 16, 0, 0, m_w, m_h, &detail);


	detail.bIsMutipleLine=False;
	detail.nEnterCommand=18;
	detail.colorBorder=Color::Black;



	m_check = xcglsCheckGroup();

	m_check->Create(this, 20, 0, 0);
	m_check->AddItem(1, L"碁盤には表示させない");
	m_check->Update(eGUM_FORCED);
	m_check->Hide();

	m_inputEditor = xcglsEditBox();//new cglsEditBox();
	detail.colorBg=Color::White;
	m_inputEditor->Create((HWND)this, 17, 0, 0, m_w, 0, &detail);

	cglsSystem* system = GetSystem();

	if (system)
	{
		system->RegisterKeyCenter(this);
	}

	return ReArrangeItemz();
}

//------------------------------------
void cglsTextPannel::OnChar(int nKeyCode, cGlassInterface* from)
{
	if (m_currFocusEditor==m_inputEditor)
	{
		if (IsWindowVisible(*m_inputEditor)==False)
		{
			m_inputEditor->Show();
		}
	}
	::SetFocus(m_currFocusEditor->m_hWnd);
	::PostMessage(m_currFocusEditor->m_hWnd, WM_CHAR, nKeyCode, (LPARAM)from);
}

// ------------------------------------
void cglsTextPannel::OnImeChar(int nKeyCode, cGlassInterface* from)
{
	if (m_currFocusEditor==m_inputEditor)
	{
		if (IsWindowVisible(*m_inputEditor)==False)
		{
			m_inputEditor->Show();
		}
	}
	::SetFocus(m_currFocusEditor->m_hWnd);
	::PostMessage(m_currFocusEditor->m_hWnd, WM_IME_CHAR, nKeyCode, (LPARAM)from);}

// --------------------------------------
void cglsTextPannel::OnSysKey(int nKeyCode, cGlassInterface* from)
{
	//::SetFocus(m_currFocusEditor->m_hWnd);
	::PostMessage(m_currFocusEditor->m_hWnd, WM_KEYDOWN, nKeyCode, (LPARAM)from);}

// --------------------------------------
void cglsTextPannel::OnStartComposition(unsigned long param, cGlassInterface* from)
{
	if (m_currFocusEditor==m_inputEditor)
	{
		if (IsWindowVisible(*m_inputEditor)==False)
		{
			m_inputEditor->Show();
		}
	}
	::SetFocus(m_currFocusEditor->m_hWnd);
	::PostMessage(m_currFocusEditor->m_hWnd, WM_IME_STARTCOMPOSITION, param, (LPARAM)from);}

// --------------------------------------
void cglsTextPannel::OnComposition(unsigned long param, cGlassInterface* from)
{
	::SetFocus(m_currFocusEditor->m_hWnd);
	::PostMessage(m_currFocusEditor->m_hWnd, WM_IME_COMPOSITION, param, (LPARAM)from);
}

// --------------------------------------
void cglsTextPannel::OnEndComposition(unsigned long param, cGlassInterface* from)
{
	::SetFocus(m_currFocusEditor->m_hWnd);
	::PostMessage(m_currFocusEditor->m_hWnd, WM_IME_ENDCOMPOSITION, param, (LPARAM)from);
}

#include "sgfmanager.h"
// ------------------------------------
LRESULT cglsTextPannel::OnNodeChanged(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	if (m_eStyle==eGTPS_PLAY)
		return 0;

	if (wParam==0)
	{
		cyiTreeNode* node=(cyiTreeNode*)lParam;
		cSgfStoneInfo* info=(cSgfStoneInfo*)node->GetData();

		if (info==0)
		{
			m_editor->SetText((wchar_t*)0);
			return 0;
		}
		wchar_t* p=yiToUnicode(0, info->comment, 0);
		m_editor->SetText(p);
		m_editor->SetTopPos(0);
		
		if (p)
			yifree(p);

		m_currentNode=(cyiTreeNode*)lParam;
	}
	else
	{
		//　ノードが削除された場合。
		m_editor->SetText((wchar_t*)0);
		m_currentNode=0;//(cyiTreeNode*)lParam;
	}

	return 0;

}

// ------------------------------------
LRESULT cglsTextPannel::OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	ReArrangeItemz();
	return 0;
}

#include "yiGoCommandz.h"
// ------------------------------------
LRESULT cglsTextPannel::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	bIsHandled=True;

	if ((wParam&0xFFFF)==18)
	{
		wchar_t szMessage[256];
		m_inputEditor->GetText(szMessage, 256);
	
		wchar_t* result = (wchar_t*)GetSystem()->GetCommandCenter()->OnCommand(eGCID_CHATT, this, (unsigned int)szMessage);

		m_editor->AppendTextEx(result, 0, 0);
		yifree(result);
		m_inputEditor->Hide();
	}
	else if ((wParam&0xFFFF)==16)
	{
		if (m_eStyle!=eGTPS_PLAY)
		{
			if (m_currentNode==0)
			{
				return 0;
			}

			cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

			if (info==0)
				info = new cSgfStoneInfo();

			if (info->comment)
				yifree(info->comment);

			char szText[1024];
			if (m_editor->GetText(szText, 1024)==0)
				info->comment=0;
			else
				info->comment = yistrheap(szText);
		}

	}

	return 0;
}
