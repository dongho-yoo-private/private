#include "glsEditBox.h"

//http://nienie.com/~masapico/sample_index.html


cglsEditBox* xcglsEditBox()
{
	return new cglsEditBox();
}

typedef cyitList<sGlsEditBoxCharElement> cTextList;
typedef cyitNode<sGlsEditBoxCharElement> cTextNode;

cglsEditBox::cglsEditBox(void)
: m_bIsUpdated(True),
	m_txt(0),
	m_listText(0),
	m_posStart(0),
	m_posCarot(0),
	m_posSelection(0),
	m_bmpBuffer(0),
	m_bIsCarotShow(0),
	m_bSelectionMode(False),
	m_bNowModeEditor(True),
	m_bIsStaticEditorMode(False),
	m_bIsFontSizeVariable(False),
	m_bmpCarotSelection(0),
	m_scroll(0),
	m_nScrollHeight(0),
	m_nScreenTop(0),
	m_nScreenBottom(0),
	m_listLineInfo(0),
	m_hScrollAnit(0),
	m_bIsPasswordMode(False),
	m_bIsNoImeMode(False),
	m_nMaxLength(-1),
	m_bIsHeightIsFontSize(False),
	m_eImeMode(eGTIM_IME_ON),
	m_nEnterCommand(0),
	m_filter(0),
	m_bIsMouseDownShow(0),
	m_pwszInitialComment(0),
	m_bIsNoInput(0),
	m_fShowCharFactor(1.0)
{
}

cglsEditBox::~cglsEditBox(void)
{
	__GLASSINTERFACE_NAME__;
}

// --------------------------------
void cglsEditBox::xDefaultDetail(sGlsEditBoxDetail* detail)
{
	cGlassInterface::xDefaultDetail(&detail->gls);
	detail->bIsMutipleLine=False;
	detail->gls.bIsWindowControl=True;
	detail->gls.bIsNoAnimation=True;
	detail->gls.bIsNotOwnDraw=True;
	detail->bmpCarot=0;
	detail->colorBg = Color::White;
	detail->colorBorder = Color::Black;
	detail->colorFont = Color::Black;
	detail->colorSelection = __COLORp__(255, 32, 128, 255);//::Blue;
	detail->colorSelectionFont=Color::White;
	detail->nCarotTimer=500;
	detail->bIsStaticEditorMode=False;
	detail->colorStaicModeFont=Color::Black;
	detail->colorStaticModeBg=0;
	detail->bIsFontSizeVariable=False;
	//detail->layout=eGEBL_LEFT;
	detail->bIsStaticPopupMode=False;
	detail->nAutoResizeWidth=0;
	detail->eAlign=(eGlsItemAlign)(eGIA_LEFT|eGIA_BOTTOM);
	detail->assistant=0;
	detail->bIsScrollAnimation=True;
	detail->nEnterCommand=0;
	detail->bIsMouseDownShow=False;
	return ;
}

#define xMIN_EDITBOX_WIDTH 10

// -------------------------------
// SingleLineの場合は、高さは無視される。
Bool cglsEditBox::Create(HWND hWnd, int id, int x, int y, int w, int h, sGlsEditBoxDetail* detail, const wchar_t* pwszInitialComment)
{
	sGlsEditBoxDetail xdetail;
	if (detail)
	{
		xdetail=*detail;
	}
	else
	{
		xDefaultDetail(&xdetail);
	}

	if (xdetail.bmpCarot)
	{
		m_bmpCarot=xdetail.bmpCarot;
	}

	if (w==0)
	{
		w=xMIN_EDITBOX_WIDTH;
	}

	m_nAutoResizeWidth=xdetail.nAutoResizeWidth;
	m_parent=(cGlassInterface*)hWnd;

	if (h==0)
	{
		m_font=xdetail.gls.font;

		if (m_font==0)
		{
			m_font=GetFont();
		}

		m_nFontHeight = m_font->GetHeight(__DEFAULTGRAPHICS__);

		unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, L"1", m_font, 0, 0);
		m_nFontHeightGDI = (pos&0xFFFF);

		h=m_nFontHeightGDI+(xTEXT_MARGINE<<1);

		m_font=0;

		m_bIsHeightIsFontSize=True;
	}
	else
	{
		xdetail.gls.font = xdetail.gls.font?xdetail.gls.font:GetFont();
	}

	m_item.pBmp[0]=BITMAP32(w, h);
	m_item.pCurrentBmp=m_item.pBmp[0];


	Graphics  g(m_item.pBmp[0]);
	g.FillRectangle(&SolidBrush(Color(xdetail.colorBg)), 0, 0, w, h);
	g.DrawRectangle(&Pen(Color(xdetail.colorBorder)), 0, 0, w-1,h-1);

	m_assistant=xdetail.assistant;//detail->assistant;
	if (xdetail.bIsStaticEditorMode==True)
	{
		m_bmpBuffer=BITMAP32(w,h);
	}
	else
	{
		m_bmpBuffer=BITMAP32(w-2,h-2);
	}

	if (xdetail.bIsStaticEditorMode==False)
	{
		Graphics g(m_bmpBuffer);
		g.FillRectangle(&SolidBrush(Color(xdetail.colorBg)), 0, 0, w, h);
	}
	else
	{
		m_bNowModeEditor=False;
	}

	m_colorBg=xdetail.colorBg;
	m_colorBorder=xdetail.colorBorder;
	m_colorFont=xdetail.colorFont;
	m_colorSelection=xdetail.colorSelection;
	m_colorSelectionFont = xdetail.colorSelectionFont;
	m_colorStaicModeFont =xdetail.colorStaicModeFont;
	m_colorStaticModeBg =xdetail.colorStaticModeBg;
	m_bIsFontSizeVariable=xdetail.bIsFontSizeVariable;
	m_eLayout=xdetail.eAlign;//detail->layout;
	m_nEnterCommand=xdetail.nEnterCommand;

	m_bIsMultiLine=xdetail.bIsMutipleLine;
	m_bIsStaticEditorMode=xdetail.bIsStaticEditorMode;

	m_bIsMouseDownShow=xdetail.bIsMouseDownShow;

	m_bmpCarot=xdetail.bmpCarot;
	if (xdetail.bmpCarot==0)
	{
		m_h=h;	
		m_font = xdetail.gls.font;

		if (m_font==0)
			m_font=GetFont();

		m_bmpCarot=CreateCarot();
		m_bmpCarotSelection=CreateCarot(True);

		m_font=0;
	}

	if (xdetail.bIsStaticEditorMode==True)
	{
		xdetail.gls.bRecieveDblClick=True;
	}

	xdetail.gls.bIsNoMouseMove=True;

	if (m_bIsMultiLine==True)
	{
		m_nScreenTop=0;
		m_nScreenBottom=m_h;
		m_listLineInfo = new cyiList();

		if (xdetail.bIsScrollAnimation==True)
		{
			m_hScrollAnit=yiAnitCreate();
		}
	}

	RECT rect = {x, y, w, h};
	xdetail.gls.bIsNoSendCommand=True;
	xdetail.gls.bFocusStop=True;
	if (cGlassInterface::Create(hWnd, 0, id, &rect, &xdetail.gls)==True)
	{
		m_hShadow = cyiShape::CreateShadow(2, 0, 0.5);
		m_listText = new cTextList(False);

		m_nCarotTimer=xdetail.nCarotTimer;
		if (m_bIsStaticEditorMode==False)
		{
			SetTimer(m_hWnd, 1, m_nCarotTimer, 0);
		}

		Show();

		if (xdetail.assistant)
		{
			wchar_t buffer[256];
			xdetail.assistant->GetDefaultValue(buffer);
			if (buffer[0])
			{
				SetText(buffer);
			}
		}
		InvalidateRect(0);
		m_hImc=ImmGetContext(m_hWnd);
		ImmSetOpenStatus(m_hImc, TRUE);
		ImmReleaseContext(m_hWnd, m_hImc);

		if (pwszInitialComment)
		{
			m_pwszInitialComment=yistrheapW(pwszInitialComment);
		}

		return True;
	}

	
	return False;
}

// -------------------------------
int cglsEditBox::SetContextText(wchar_t* text)
{
	wcsncpy(m_szContextText, text, 32);
	return 0;
}

// -------------------------------
int cglsEditBox::GetText(char* text, int nBufferSize)
{
	if (text==0)
	{
		if (m_listText==0)
			return 0;

		return m_listText->count();
	}

	wchar_t* p = (wchar_t*)yialloc(sizeof(wchar_t)*nBufferSize);

	text[0]=0;
	int n = GetText(p, nBufferSize);
	::yiToMultiByteCode(text, p, 0);
	yifree(p);

	return n;
}

// -------------------------------
int cglsEditBox::GetText(wchar_t* text, int nBufferSize)
{
	int cnt=0;
	if (text==0)
	{
		if (m_listText==0)
			return 0;

		return m_listText->count();
	}

	text[0]=0;
	if (m_listText==0)
	{
		return 0;
	}

	for (cTextNode* node = m_listText->GetNodeTopFrom(); node; node=node->np)
	{
		sGlsEditBoxCharElement* el = node->ref;//(sGlsEditBoxCharElement*)node->ref;	

		if (cnt>nBufferSize)
		{	
			text[nBufferSize]=0;
			return m_listText->count();
		}
		text[cnt++]=el->ch;
	}
	text[cnt]=0;
	return m_listText->count();
}

// -------------------------------
//int cglsEditBox::GetText(char* text, int nBufferSize)
//{
//	return 0;
//}

// -------------------------------
//Bool cglsEditBox::SetText(char* text)
//{
//	return 0;
//}

// -----------------------------
void cglsEditBox::EnableInput(Bool bIsEnable)
{
	m_bIsNoInput = bIsEnable==True?False:True;
	m_bIsCarotShow=bIsEnable;

}


// -------------------------------
void cglsEditBox::SetImeMode(eGlsTextImeMode eMode)
{
	m_eImeMode=eMode;
	HIMC hImc;
	if (eMode==eGTIM_IME_ALWAYS_OFF)
	{
		hImc = ImmAssociateContext(m_hWnd, NULL);
		ImmReleaseContext(m_hWnd, hImc);
		return ;
	}
	else
	{
		hImc=ImmGetContext(m_hWnd);
		ImmSetOpenStatus(hImc, TRUE);
		ImmReleaseContext(m_hWnd, hImc);
		//ImmAssociateContext(m_hWnd, hIMC); 
	}

	//if (eMode==eGTIM_IME_ON)
	//{
	//}
}

// -------------------------------
void cglsEditBox::SetPasswordMode(Bool bIsPasswordMode)
{
	m_bIsPasswordMode=bIsPasswordMode;

	if (bIsPasswordMode)
	{
		float height;
		m_nPasswordWidth = GetCharWidth(L'*', height);
	}

	Update(eGUM_FORCED);
	
}

const char* g_pszNumericFilter = "\0-.0123456789";
// -------------------------------
void cglsEditBox::SetNumeric(Bool bIsNumeric)
{
	if (bIsNumeric==True)
		m_filter=(char*)g_pszNumericFilter;
	else
		m_filter=0;
}

// -------------------------------
void cglsEditBox::SetNoImeMode(Bool bIsNoImeMode, char* filter)
{
	m_bIsNoImeMode=bIsNoImeMode;
	m_filter=filter;
}

// -------------------------------
// n==-1 無限
void cglsEditBox::SetMaxLength(int n)
{
	m_nMaxLength=n;
}

// -------------------------------
void cglsEditBox::SetTopPos(int nLine)
{
	m_nScreenTop=0;
	InvalidateRect(0);
}

// -------------------------------
Bool cglsEditBox::SetText(const char* text, int nAnimationSpeed, int nTerm)
{
	wchar_t* textw=yiToUnicode(0, text, 0);
	Bool res = SetText(textw);

	if (textw)
		yifree(textw);

	return res;
}


// -------------------------------
Bool cglsEditBox::SetText(const wchar_t* text, int nAnimationSpeed, int nTerm)
{
	//m_listText->clear(yifree);
	int count = m_listText->count();
	wchar_t* pText=(wchar_t*)text;

	if (text==0 || text[0]==0)
	{
		m_posCarot=0;
		m_posStart=0;
		m_posSelection=0;
		
		DeleteString(0, -1);

		UpdateText();
		InvalidateRect(0);
		return True;
	}

	if (m_nMaxLength!=-1)
	{
		if (wcslen(pText)>m_nMaxLength)
		{
			pText[m_nMaxLength]=0;
		}
	}

	m_posCarot=0;
	m_posStart=0;
	m_posSelection=0;
	
	DeleteString(0, -1);

	InsertString(0, pText);

	if (nAnimationSpeed!=0)
	{
		m_fShowCharFactor=0.0;

		if (m_item.hAnit==0)
		{
			m_item.hAnit=::yiAnitCreate();
		}

		if (::yiAnitIsAlived(m_item.hAnit)==True)
		{
			::yiAnitStop(m_item.hAnit, True);
		}

		::yiAnitStart(m_item.hAnit, *this, WM_EDIT_ANIMATION, 
	}
	return True;
}

// -------------------------------
Bool cglsEditBox::SetTextEx(const wchar_t* text, color_t colorFont, color_t colorBg, Font* font, int nAnimationSpeed, int nTerm)
{
	m_listText->clear(yifree);

	m_posCarot=0;
	m_posStart=0;
	m_posSelection=0;

	return InsertStringEx(0, text, colorFont, colorBg, font);
}

// -------------------------------
Bool cglsEditBox::AppendTextEx(const wchar_t* text, color_t colorFont, color_t colorBg, Font* font)
{
	//m_listText->clear(yifree);

	return InsertStringEx(-1, text, colorFont, colorBg, font);
}

// -------------------------------
cGlsAssistant* cglsEditBox::GetAssistant()
{
	return m_assistant;
}


// -----------------------------
Bool cglsEditBox::IsSelectionArea(int pos)
{
	int nStart, nEnd;

	if (m_posCarot==m_posSelection)
		return False;
	
	nStart = m_posCarot>m_posSelection?m_posSelection:m_posCarot;
	nEnd = m_posCarot<m_posSelection?m_posSelection:m_posCarot;

	if (pos<nStart ||
			pos>=nEnd)
	{
		return False;
	}

	return True;
}

// -----------------------------
int cglsEditBox::GetCoordiateToPos(int n)
{
	if (m_listText==0)
		return 0;
	n+=m_posStart;
	m_listText->lock();
	int cnt=0;
	float sum=0;
	for (cTextNode* node = m_listText->GetNodeTopFrom(); node; node=node->np)
	{
		sGlsEditBoxCharElement* el = node->ref;
		sum+=el->width;
		if (n<=sum)
		{
			m_listText->unlock();
			return cnt;
		}

		cnt++;

		//if (n==cnt)
		//	break;
	}

	m_listText->unlock();

	return m_listText->count();
}

// -----------------------------
int cglsEditBox::GetCoordiateToPos(int x, int y)
{
	int nBeforeNodeCount=0;
	float nBeforeHeight=0.0;
	float nLastCarotPos=0.0;

	float nLineHeight=0.0;
	float nTotalHeight=0;
	float nLineWidth=0.0;
	int count=0;
	int nClientWidth=m_w-2;
	Bool bIsCatched=False;
	cTextNode* nodeEnd=0;

	y+=m_nScreenTop;

	if (m_listText->count()==0)
	{
		return 0;
	}

	m_listText->lock();

	if (m_listText->count()==0)
	{
		return 0;
	}

	// まず、全体のサイズを計算する。
	for (cTextNode* node = m_listText->GetNodeTopFrom(0); node; node=node->np)
	{
		sGlsEditBoxCharElement* el=node->ref;
		count++;

		nLineHeight=nLineHeight>el->height?nLineHeight:el->height;

		if (y<nTotalHeight)
		{
			if (node->pp)
			{
				sGlsEditBoxCharElement* elx = node->pp->ref;
				if (elx->ch=='\n')
					return nBeforeNodeCount;
			}
			return nBeforeNodeCount+1;
		}

		if (x>nLineWidth)
		{
			nBeforeNodeCount=count-1;
		}

		if (el->ch==L'\n')
		{
			nTotalHeight+=nLineHeight;
			nLineHeight=0;
			nLineWidth=0;

			continue;
		}

		if (el->ch==' ')
		{
			if (nLineWidth+el->width > (float)nClientWidth)
			{
				nTotalHeight+=nLineHeight;
				nLineHeight=0;
				nLineWidth=0;
			}

			nLineWidth+=el->width;
			continue;
		}

		if (nLineWidth+el->width>(float)nClientWidth)
		{
			nTotalHeight+=nLineHeight;
			nLineHeight=0;
			nLineWidth=0;
		}

		nLineWidth+=el->width;
	}



	m_listText->unlock();
	return nBeforeNodeCount+1;

	//return m_listText->count();
}

// ------------------------------------
int cglsEditBox::xGetLineTop(int pos)
{
	if (m_bIsMultiLine)
	{
		float x=0;
		float y=0;
		float sum=0.0;
		float fMaxHeight=0.0;
		float posHeight=0.0;
		int cnt=0;
		int top=0;
		Bool bIsNextLine=False;

		if (pos==0)
		{
			return 0;
		}

		m_listText->lock();

		for (cTextNode* node = m_listText->GetNodeTopFrom(pos-1); node; node=node->pp)
		{
			sGlsEditBoxCharElement* el = node->ref;

			if (el->ch=='\n')
			{
				m_listText->unlock();
				return pos;
			}
			pos--;
		}


		m_listText->unlock();
		return 0;
	}

	return 0;
}

// ------------------------------------
int cglsEditBox::xGetLineEnd(int pos)
{
	if (m_bIsMultiLine)
	{
		if (pos==m_listText->count())
		{
			return pos;
		}

		m_listText->lock();

		for (cTextNode* node = m_listText->GetNodeTopFrom(pos); node; node=node->np)
		{
			sGlsEditBoxCharElement* el = node->ref;
			if (node->ref->ch=='\n')
			{
				//pos++;
				m_listText->unlock();
				return pos;
			}
			pos++;
		}
		m_listText->unlock();
		return m_listText->count();

	}

	return m_listText->count();
}


// ------------------------------------
int cglsEditBox::xGetNextLinePos(int pos)
{
	if (m_bIsMultiLine)
	{
		float x=0;
		float y=0;
		float sum=0.0;
		float fMaxHeight=0.0;
		float posHeight=0.0;
		int cnt=0;
		Bool bIsNextLine=False;
		m_listText->lock();

		for (cTextNode* node = m_listText->GetNodeTopFrom(); node; node=node->np)
		{
			sGlsEditBoxCharElement* el = node->ref;
			
			fMaxHeight= fMaxHeight<el->height?el->height:fMaxHeight;

			if (pos==cnt)
			{
				y=posHeight;
				x=sum;
			}

			if (cnt>pos &&
					y<posHeight)
			{
				bIsNextLine=True;
			}

			if (bIsNextLine==True &&
				sum>x)
			{
				return cnt-1;
			}

			if (el->ch=='\n')
			{
				if (bIsNextLine==True)
				{
					break;
				}

				cnt++;
				posHeight+=fMaxHeight;
				sum=0.0;

				continue;
			}

			if ((sum+el->width)>m_w-2)
			{
				if (bIsNextLine==True)
				{
					break;
				}
				posHeight+=fMaxHeight;
				sum=0.0;
			}

			sum+=el->width;
			cnt++;
		}

		m_listText->unlock();
		//posHeight-=fMaxHeight;
		return cnt;
	}

	return pos;
}

// ------------------------------------
int cglsEditBox::xGetPrevLinePos(int pos)
{
	if (m_bIsMultiLine)
	{
		float x=0;
		float y=0;
		float sum=0.0;
		float fMaxHeight=0.0;
		float posHeight=0.0;
		int cnt=0;
		int nLineCnt=0;
		Bool bIsNextLine=False;
		m_listText->lock();

		for (cTextNode* node = m_listText->GetNodeTopFrom(); node; node=node->np)
		{
			sGlsEditBoxCharElement* el = node->ref;
			fMaxHeight= fMaxHeight<el->height?el->height:fMaxHeight;

			if (pos==cnt)
			{
				y=posHeight;
				x=sum;
				break;
			}

			if (el->ch=='\n')
			{
				cnt++;
				posHeight+=fMaxHeight;
				nLineCnt++;
				sum=0.0;

				continue;
			}

			if ((sum+el->width)>m_w-2)
			{
				posHeight+=fMaxHeight;
				sum=0.0;
				nLineCnt++;
			}

			sum+=el->width;
			cnt++;
		}

		int nLineCnt2=0;
		cnt=0;
		sum=0.0;
		fMaxHeight=0.0;
		for (cTextNode* node = m_listText->GetNodeTopFrom(); node; node=node->np)
		{
			sGlsEditBoxCharElement* el = node->ref;
			fMaxHeight= fMaxHeight<el->height?el->height:fMaxHeight;

			if (nLineCnt2>=nLineCnt)
			{
				m_listText->unlock();
				return pos;
			}

			if (nLineCnt2==nLineCnt-1 &&
					sum>=x)
			{
				m_listText->unlock();

				if (x==sum)
					return cnt;

				return cnt-1;
			}

			if (el->ch=='\n')
			{
				if (nLineCnt2==nLineCnt-1)
				{
					m_listText->unlock();
					return cnt;
				}

				cnt++;
				posHeight+=fMaxHeight;
				nLineCnt2++;
				sum=0.0;

				continue;
			}

			if ((sum+el->width)>m_w-2)
			{
				posHeight+=fMaxHeight;
				sum=0.0;
				nLineCnt2++;
			}

			sum+=el->width;
			cnt++;
		}

		m_listText->unlock();
		//posHeight-=fMaxHeight;
		return pos;
	}

	return pos;
}


// -----------------------------
float cglsEditBox::GetPosToCoordiate(int n, float& posHeight)
{
	float sum=0;
	int cnt=0;

	if (m_listText==0)
		return 0;


	posHeight=0.0;

	if (n==0)
	{
		return 0.0;
	}

	m_listText->lock();

	if (m_bIsMultiLine)
	{
		float fMaxHeight=0;
		for (cTextNode* node = m_listText->GetNodeTopFrom(); node; node=node->np)
		{
			sGlsEditBoxCharElement* el = node->ref;
			
			fMaxHeight= fMaxHeight<el->height?el->height:fMaxHeight;

			if (el->ch=='\n')
			{
				posHeight+=fMaxHeight;
				sum=0.0;
				cnt++;
				if (n==cnt)
					break;

				continue;
			}

			if ((sum+el->width)>m_w-2)
			{
				posHeight+=fMaxHeight;
				sum=0.0;
				fMaxHeight=el->height;
			}
			
			sum+=el->width;
			cnt++;

			if (n==cnt)
				break;

		}

		m_listText->unlock();
		//posHeight-=fMaxHeight;
		return sum;
	}

	for (cTextNode* node = m_listText->GetNodeTopFrom(); node; node=node->np)
	{
		sGlsEditBoxCharElement* el = node->ref;

		if (n==cnt)
			break;
		sum+=el->width;
		cnt++;
	}

	m_listText->unlock();

	return sum;
}


// -------------------------------
Bool cglsEditBox::CreateBitmap(sDrawItem* item, void* pExtra)
{
	return False;
}

// -----------------------------
float cglsEditBox::GetCharWidth(wchar_t ch, float& h)
{
	RectF bound;
	RectF bound2;

	if (ch==L'\n')
		ch=L' ';

	wchar_t x[2]={ch, 0};

	unsigned int size = (unsigned int)cyiShape::BitmapStringEx(0, x, GetFont()/*m_font*/, 0, 0);

	h=(size&0xFFFF);
	return (size>>16);


	wchar_t buffer[3]={L'a', ch, L'a'};

	

	Graphics g(m_item.pBmp[0]);
	g.SetTextRenderingHint(TextRenderingHintAntiAlias);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	Status ret = g.MeasureString(buffer, 3, GetFont(), RectF(0, 0, m_w, m_h), 0, &bound);
	ret = g.MeasureString(L"aa", 2, GetFont(), RectF(0, 0, m_w, m_h), 0, &bound2);

	if (ret==Status::Ok)
	{
		float ret = bound.Width-bound2.Width;
		h=bound.Height;
		return ret;// __INTEGERF__(ret);
	}

	return 0;
}

Bool cglsEditBox::xGetSelectedString(wchar_t* buffer)
{
	int xxx=0;
	int cnt=0;

	for (cTextNode* node = m_listText->GetNodeTopFrom(); node; node=node->np)
	{
		sGlsEditBoxCharElement* el=node->ref;

		if (IsSelectionArea(cnt++)==True)
		{
			buffer[xxx++]=el->ch;
		}
	}
	buffer[xxx]=0;

	if (xxx)
		return True;

	return False;
}

// -------------------------------
LRESULT cglsEditBox::OnImeChar(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	handled=True;
	if (m_bIsNoInput==True)
		return 0;

	//HIMC hIMC = ImmGetContext(this->m_hWnd);

	//int len = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);

	//if (len!=0)
	//{
	//	HANDLE hData = (HANDLE)::GlobalAlloc(GPTR, (DWORD)len+1);

	//	char *pData = (char *)::GlobalLock(hData);

	//	ImmGetCompositionString(hIMC, GCS_RESULTSTR, pData, len + 1);

	//	//（Ｄ）ここで取得した文字列を処理します。終わったらメモリの開放を忘れずに！
	//	wchar_t unicode[1024];

	//	yiToUnicode(unicode, pData, eCS_JAPANESE);

	//	if (m_listText==0 ||
	//		m_listText->count()==m_posCarot)
	//	{
	//		InsertString(-1, unicode);
	//	}
	//	else
	//	{
	//		InsertString(m_posCarot, unicode);
	//	}

	//	::GlobalUnlock(hData);
	//	::GlobalFree(hData);

	//}

	//ImmReleaseContext(this->m_hWnd, hIMC);
	return 0;
}

// -------------------------------
LRESULT cglsEditBox::OnChar(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	wchar_t ch[2]={0, 0};
	char _ch[2] = {(char)wParam, 0};

	if (m_bIsNoInput==True)
		return 0;

	if (m_filter!=0)
	{
		Bool bIsFilter=m_filter[0]==0?False:True;

		if (bIsFilter==True)
		{
			char* pIndex=&m_filter[1];
			while(*pIndex)
			{
				if (_ch[0]==*pIndex)
				{
					// 何もしない。（入力されてないと見なす）
					handled=True;
					return 0;
				}
				pIndex++;
			}
		}
		else
		{
			char* pIndex=&m_filter[1];
			while(*pIndex)
			{
				if (_ch[0]==*pIndex)
				{
					//入力可能な文字
					break;
				}
				else if (_ch[0]<0x20) // BS
				{
					break;
				}
				pIndex++;
			}

			if (*pIndex==0)
				return 0;
		}
	}

	handled=False;

	m_bIsCarotShow=1;
	if (wParam==0x08)
	{
xDELETE:
		int start = m_posSelection < m_posCarot ? m_posSelection:m_posCarot;
		int end = m_posSelection > m_posCarot ? m_posSelection:m_posCarot;

		if (start==end)
		{
			DeleteString(m_posCarot-1, m_posCarot-1);
			m_posCarot--;

		}
		else
		{
			DeleteString(start, end);
			m_posCarot=start;
		}

		if ((int)m_posCarot<0)
			m_posCarot=0;
		
		m_posSelection=m_posCarot;

		UpdateText();
		InvalidateRect(0);
		handled=True;
		//m_posCarot--;
		return 0;
	}
	// tab
	if (wParam==0x09)
	{
		if (m_bIsMultiLine==False)
		{
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				PrevFocus();
			}
			else
				NextFocus();
			handled=True;

			return 0;
		}
	}
	// ctrl+A
	if(wParam==0x01)
	{
		m_posCarot=m_listText->count();
		m_posSelection=0;

		UpdateText();
		InvalidateRect(0);
		handled=True;

		return 0;
	}

	// 0x16 ctrl+V
	else if (wParam==0x16)
	{
		if (m_posCarot!=m_posSelection)
		{
			int start = m_posSelection < m_posCarot ? m_posSelection:m_posCarot;
			int end = m_posSelection > m_posCarot ? m_posSelection:m_posCarot;

			DeleteString(start, end);
			m_posSelection=m_posCarot=start;
		}

		wchar_t szClipboard[1024];
		int nLength=1024;

		szClipboard[0]=0;
		
		if (yiUtilFromClipBoard(szClipboard, &nLength))
		{
			InsertString(m_posCarot, szClipboard);
		}

		UpdateText();
		InvalidateRect(0);
		handled=True;

		return 0;
	}
	// Control+C
	if (wParam==0x03 && (m_posCarot!=m_posSelection))
	{
		wchar_t buffer[1024];

		if (xGetSelectedString(buffer)==True)
		{
			yiUtilToClipBoard(buffer);
		}
		handled=True;
		return 0;
	}
	// ctrl+X
	else if (wParam==0x18 && (m_posCarot!=m_posSelection))
	{
		wchar_t buffer[1024];

		if (xGetSelectedString(buffer)==True)
		{
			yiUtilToClipBoard(buffer);
		}
		goto xDELETE;
	}
	else if (wParam==0x0d) // Enter.
	{
		handled=True;
		if (m_bIsStaticEditorMode==True)
		{
			OnKillFocus(wParam, lParam, handled);
			SendMessage(m_hNotifyWnd, WM_COMMAND, m_id, (LPARAM)this);
			return 0;
		}
		if (m_nEnterCommand!=0)
		{
			SendMessage(m_hNotifyWnd, WM_COMMAND, m_nEnterCommand, 0);
			DeleteString(0, -1);
			m_posCarot=0;
			m_posSelection=0;
			UpdateText();
			InvalidateRect(0);
			return 0;
		}
		if (m_bIsMultiLine)
		{
			InsertString(-1, L"\n");
			return 0;
		}
	}

	if (handled==False &&
			m_bIsMultiLine==False)
	{
		handled=True;
		__super::OnChar(wParam, lParam, handled);
	}

	if (handled==True)
	{
		return 0;
	}

	if (wParam<0x20)
	{
		return 0;
	}

	yiToUnicode(ch, _ch, eCS_JAPANESE);

	if (m_posCarot!=m_posSelection)
	{
		DeleteString(m_posCarot<m_posSelection?m_posCarot:m_posSelection, m_posCarot<m_posSelection?m_posSelection:m_posCarot);
		m_posCarot=m_posCarot<m_posSelection?m_posCarot:m_posSelection;
		m_posSelection=m_posCarot;
	}

	if (m_listText==0 ||
		m_listText->count()==m_posCarot)
	{
		InsertString(-1, ch);
	}
	else
	{
		InsertString(m_posCarot, ch);
	}

	return 0;
}

// -------------------------------
LRESULT cglsEditBox::OnSysKeyUp(WPARAM wParam, LPARAM lParam, Bool& handled)
{

	if (wParam==0x10)
	{
		m_bSelectionMode=False;
	}
	return 0;
}

// -------------------------------
LRESULT cglsEditBox::OnSysKeyDown(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	Bool bReqUpdateText=False;
	m_bIsCarotShow=1;

	handled=True;

	if (m_bIsNoInput==True)
		return 0;

	if (m_bSelectionMode==False)
	{
		bReqUpdateText = (m_posSelection!=m_posCarot);
	}

	// ->
	if (wParam==0x27)
	{
		m_posCarot++;
	}
	// <-
	else if (wParam==0x25)
	{
		m_posCarot--;
	} 
	// UP
	else if (wParam==0x26)
	{
		m_posCarot=xGetPrevLinePos(m_posCarot);
	}
	// DOWN
	else if (wParam==0x28)
	{
		m_posCarot=xGetNextLinePos(m_posCarot);
	}
	// DEL
	else if (wParam==0x2e)
	{
		int start = m_posSelection < m_posCarot ? m_posSelection:m_posCarot;
		int end = m_posSelection > m_posCarot ? m_posSelection:m_posCarot;

		DeleteString(start, end);
		m_posSelection=m_posCarot=start;

		UpdateText();
		InvalidateRect(0);
		return 0;
	}
	//HOME
	else if (wParam==0x24)
	{
		m_posCarot=xGetLineTop(m_posCarot);
	}
	// END
	else if (wParam==0x23)
	{

		if (m_listText)
		{
			m_posCarot=xGetLineEnd(m_posCarot);
		}
	}
	// Shift
	else if (wParam==0x10)
	{
		m_bSelectionMode=True;
		return 0;
	}
	else
	{
		handled=True;
		__super::OnSysKeyDown(wParam, lParam, handled);

		return 0;
	}

	//else
	//{
	//	return 0;
	//}

	if (m_posCarot==0xFFFFFFFF)
		m_posCarot=0;

	if (m_posCarot>m_listText->count())
	{
		m_posCarot=m_listText->count();
	}

	if (m_bSelectionMode==False)
	{
		m_posSelection=m_posCarot;

		if (bReqUpdateText)
		{
			UpdateText();
		}
	}
	else
	{
		UpdateText();
	}

	if (m_bIsMultiLine==True)
	{
		float fPosCarotY;
		float nPosCarot = GetPosToCoordiate(m_posCarot, fPosCarotY);

		if (fPosCarotY+m_bmpCarot->GetHeight()>m_h+m_nScreenTop)
		{
			m_nScreenTop=fPosCarotY+m_bmpCarot->GetHeight()-m_h;
		}
		else if (fPosCarotY<m_nScreenTop)
		{
			m_nScreenTop=fPosCarotY;
		}
	}

	InvalidateRect(0);

	return 0;
}

// -------------------------------
LRESULT cglsEditBox::OnStartComposition(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	if (m_bIsNoInput==True)
		return 0;

	HIMC hIMC = ImmGetContext(m_hWnd);

	LOGFONT lf;// = … ;

	Bitmap temp(1,1,PixelFormat32bppARGB);
	Graphics g(&temp);

	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);

	FontFamily fam;
	wchar_t name[128];
	GetFont()->GetLogFontA(&g, &lf);
	GetFont()->GetFamily(&fam);

	fam.GetFamilyName(name);
	ImmSetCompositionFont(hIMC, &lf);

	//int len = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);

	//if (len!=0)
	//{
	//	HANDLE hData = (HANDLE)::GlobalAlloc(GPTR, (DWORD)len+1);

	//	char *pData = (char *)::GlobalLock(hData);

	//	ImmGetCompositionString(hIMC, GCS_RESULTSTR, pData, len + 1);

	//	//（Ｄ）ここで取得した文字列を処理します。終わったらメモリの開放を忘れずに！
	//	wchar_t unicode[1024];

	//	yiToUnicode(unicode, pData, eCS_JAPANESE);

	//	if (m_listText==0 ||
	//		m_listText->count()==m_posCarot)
	//	{
	//		InsertString(-1, unicode);
	//	}
	//	else
	//	{
	//		InsertString(m_posCarot, unicode);
	//	}

	//	::GlobalUnlock(hData);
	//	::GlobalFree(hData);
	//}

	float yPos;
    COMPOSITIONFORM cf;

    cf.dwStyle = CFS_POINT;
    cf.ptCurrentPos.x=GetPosToCoordiate(m_posCarot, yPos)+1;// = pt;
	
	if (m_bIsMultiLine)
		cf.ptCurrentPos.y=1+yPos-m_nScreenTop;
	else
		cf.ptCurrentPos.y=xTEXT_MARGINE+yPos;

	
    ImmSetCompositionWindow(hIMC, &cf);

	ImmReleaseContext(m_hWnd, hIMC);

	handled=False;
	return 0;
}

// -------------------------------
static wchar_t* xGetResultString(HWND hWnd)
{
    HIMC hImc = ImmGetContext(hWnd);
    if (hImc==NULL)
        return NULL;
 
    // ImmGetCompositionStringの第４引数を0にすると必要なメモリサイズが取得できる
    long len = ImmGetCompositionString(hImc,GCS_RESULTSTR,NULL,0);
    wchar_t* result = NULL;
    if (len)
    {
		//HANDLE hData = (HANDLE)::GlobalAlloc(GPTR, len+sizeof(wchar_t));

		//result = (wchar_t*)::GlobalLock(hData);
        result = (wchar_t*)yialloci(len+sizeof(wchar_t));

        // 確定文字列を取得する
      // 第二引数にGCS_RESULTSTRを指定すると確定した文字列が取得できる
        ImmGetCompositionString(hImc,GCS_RESULTSTR,result,len);
		//result[len]=0;
        //result[(len / sizeof(TCHAR))] = _T(’\0’);
    }
 
    ImmReleaseContext(hWnd,hImc);
    return result;

}

// -------------------------------
static wchar_t* xGetCompositionString(HIMC hImc)
{
 
    long len = ImmGetCompositionString(hImc,GCS_COMPSTR,NULL,0);
    wchar_t* result = NULL;

	if (len)
    {        
        //len++;
        result = (wchar_t*)yialloci(len+sizeof(wchar_t));//new TCHAR[(len / sizeof(TCHAR)) + 1];
        // 第二引数にGCS_COMPSTRを指定すると確定した文字列が取得できる
        ImmGetCompositionString(hImc,GCS_COMPSTR,result,len);        
		result[len]=0;
    }
 
    return result;
}

// -------------------------------
LRESULT cglsEditBox::OnComposition(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	wchar_t *result = NULL;
    HIMC hImc = ImmGetContext(m_hWnd);
    if (hImc==NULL)
        return NULL;

	//HIMC hIMC = ImmGetContext(this->m_hWnd);

	//if (hIMC==0)
	//	return 0;

	if ((lParam&GCS_RESULTSTR)!=0)
	{
		wchar_t unicode[1024];
		result = xGetResultString(m_hWnd);
		xTRACE("cglsEditBox::OnComposition GCS_RESULTSTR:%s", result);
		yiToUnicode(unicode, (char*)result, eCS_JAPANESE);

		if (m_listText==0 ||
			m_listText->count()==m_posCarot)
		{
			InsertString(-1, unicode);
		}
		else
		{
			InsertString(m_posCarot, unicode);
		}
		yifree(result);

		float yPos;
		COMPOSITIONFORM cf;

		cf.dwStyle = CFS_POINT;
		cf.ptCurrentPos.x=GetPosToCoordiate(m_posCarot, yPos)+1;// = pt;
		
		if (m_bIsMultiLine)
			cf.ptCurrentPos.y=1+yPos-m_nScreenTop;
		else
			cf.ptCurrentPos.y=xTEXT_MARGINE+yPos;
		ImmSetCompositionWindow(hImc, &cf);

	}
    ImmReleaseContext(m_hWnd,hImc);

	//else if ((lParam & GCS_COMPSTR) != 0)
	//{
 //       result = xGetCompositionString(m_hWnd);
	//	xTRACE("cglsEditBox::OnComposition GCS_COMPSTR:%s", result);
	//	//yifree(result);
	//}

	//handled=True;
	return 0;
}


// -------------------------------
LRESULT cglsEditBox::OnEndComposition(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	if (m_bIsNoInput==True)
		return 0;

	//（Ａ）確定文字列が生じたのかどうかを検査します。 
	handled=False;
	return 0;
	//if (lParam & GCS_RESULTSTR) {
		{		
			//（Ｂ）ＩＭＥコンテキストを取得します。ＩＭＥを操作する場合には必要なおまじないです。

			HIMC hIMC = ImmGetContext(this->m_hWnd);


			//（Ｃ）確定文字列の長さを調べ、その分のメモリを獲得してから確定文字列を取り込みます。

			int len = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);

			if (len==0)
			{
				ImmReleaseContext(this->m_hWnd, hIMC);
				return 0;
			}

			HANDLE hData = (HANDLE)::GlobalAlloc(GPTR, (DWORD)len+1);

			char *pData = (char *)::GlobalLock(hData);

			ImmGetCompositionString(hIMC, GCS_RESULTSTR, pData, len + 1);
			::GlobalUnlock(hData);
			::GlobalFree(hData);
			
			xTRACE("cglsEditBox::OnEndComposition GCS_RESULTSTR:%s", pData);

			//（Ｄ）ここで取得した文字列を処理します。終わったらメモリの開放を忘れずに！
			wchar_t unicode[1024];

			yiToUnicode(unicode, pData, eCS_JAPANESE);

			if (m_listText==0 ||
				m_listText->count()==m_posCarot)
			{
				InsertString(-1, unicode);
			}
			else
			{
				InsertString(m_posCarot, unicode);
			}





			//（Ｅ）未確定文字列が残っている場合がありますので、その未確定文字列を表示する領域の左上隅座標を指定します。

			//CPoint pt = … ;

			//COMPOSITIONFORM cf;

			//cf.dwStyle = CFS_POINT;

			//cf.ptCurrentPos = pt;

			//ImmSetCompositionWindow(hIMC, &cf);



			//（Ｆ）ＩＭＥコンテキストを開放します。ＩＭＥを操作し終わった場合のおまじないです。

			ImmReleaseContext(this->m_hWnd, hIMC);

	}


	return 0;
}


// ---------------------------------------
LRESULT cglsEditBox::OnMouseLButtonDown(WPARAM wp, LPARAM lp, BOOL& handled)
{
	int x = 0xFFFF&lp;
	int y = (lp>>16)&0xFFFF;
	int n = 0;
	Bool bReqUpdateText =False;

	if (m_bIsNoInput==True)
	{
		return 0;
	}


	if (m_bIsMouseDownShow==True)
	{
		OnMouseLButtonDblClick(wp, lp, handled);

		if (handled==True)
			return 0;
	}

	if (GetCapture()!=m_hWnd)
	{
		SetCapture(m_hWnd);
	}

	if (m_bIsMultiLine)
	{
		n=GetCoordiateToPos(x, y);
	}
	else
		n=GetCoordiateToPos(x);

	if (m_bNowModeEditor==False)
	{
		return 0;
	}

	if (m_bSelectionMode==False)
	{
		bReqUpdateText = (m_posSelection!=m_posCarot);
	}

	m_bIsCarotShow=True;
	m_posCarot=n;
	m_posSelection=m_posCarot;

	SetFocus(m_hWnd);
	if (m_bIsStaticEditorMode==False)
	{
		SetTimer(m_hWnd, 1, m_nCarotTimer, 0);
	}

	if (bReqUpdateText)
		UpdateText();
	InvalidateRect(0);
	return 0;
}

// -------------------------------
LRESULT cglsEditBox::OnTimer(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	//cyiBitmapBlender::BitmapCopy(m_item.pBmp[0], m_bmpText);
	if (m_bIsNoInput==True)
	{
		return 0;
	}

	if (GetFocus()!=m_hWnd)
	{
		if (m_bIsCarotShow)
		{
			InvalidateRect(0);
		}

		m_bIsCarotShow=0;
		return False;
	}

	//if (m_bIsCarotShow)
	//{
		//Graphics g(m_item.pBmp[0]);
		//g.DrawImage(m_bmpCarot, m_nCarotPosX, 1);
		m_bIsCarotShow=1-m_bIsCarotShow;
	//}

	float y;
	int x = GetPosToCoordiate(m_posCarot, y);
	RECT rect = {x+1, xTEXT_MARGINE, m_bmpCarot->GetWidth()+x+3, xTEXT_MARGINE+m_bmpCarot->GetHeight()+1};
	InvalidateRect(0);//&rect);


	return 0;
}

// -----------------------------
Bool cglsEditBox::InsertStringEx(int n, const wchar_t* pszString, color_t colorFont, color_t colorBg, Font* font)
{
	wchar_t* pIndex=(wchar_t*)pszString;

	int count = m_listText->count();

	if (m_nMaxLength!=-1)
	{
		if (count>=m_nMaxLength)
		{
			return False;
		}

		if (count+wcslen(pIndex)>m_nMaxLength)
		{
			pIndex[wcslen(pIndex)-count]=0;
		}
	}


	float nPassHeight=0;
	if (m_bIsPasswordMode)
	{
		m_nPasswordWidth=GetCharWidth(L'*', nPassHeight);
	}

	if (n==-1)
	{
		while(*pIndex)
		{
			sGlsEditBoxCharElement*s = (sGlsEditBoxCharElement*)yialloci(sizeof(sGlsEditBoxCharElement));
			s->ch=*pIndex;

			if (m_bIsPasswordMode==True)
			{
				s->width=m_nPasswordWidth;
				s->height=nPassHeight;
			}
			else
			{
				s->width=this->GetCharWidth(*pIndex, s->height);
			}
			s->font=font;
			s->colorFont=colorFont;
			s->colorBg=colorBg;

			m_listText->add(s);
			pIndex++;
			m_posCarot++;
			m_posSelection++;
		}
	}
	else
	{
		while(*pIndex)
		{
			sGlsEditBoxCharElement*s= (sGlsEditBoxCharElement*)yialloci(sizeof(sGlsEditBoxCharElement));
			s->ch=*pIndex;
			if (m_bIsPasswordMode==True)
			{
				s->width=m_nPasswordWidth;
				s->height=nPassHeight;
			}
			else
			{
				s->width=this->GetCharWidth(*pIndex, s->height);
			}

			//s->width=GetCharWidth(*pIndex, s->height);
			m_listText->insert(n++, s);
			s->font=font;
			s->colorFont=colorFont;
			s->colorBg=colorBg;
			pIndex++;
			m_posCarot++;
			m_posSelection++;
		}
	}


	UpdateText();
	InvalidateRect(0);

	if (m_bIsStaticEditorMode==False &&
		m_bNowModeEditor==True)
	{
		PostMessage(m_hNotifyWnd, WM_COMMAND, ((EN_CHANGE<<16) | m_id), (LPARAM)m_hWnd);
	}
	return True;
}

// -----------------------------
Bool cglsEditBox::InsertString(int n, const wchar_t* pszString)
{
	return InsertStringEx(n, pszString, 0, 0, 0);
}

// -----------------------------
Bool cglsEditBox::DeleteString(int from, int to)
{
	int nDelCnt=1;

	if (to==-1)
		to = m_listText->count();

	if (m_listText->count()==0)
		return False;

	if (from==to)
	{
		yifree(m_listText->removeEx(from));
	}
	else
	{
		for (int i=from; i<to; i++)
		{
			sGlsEditBoxCharElement* el = (sGlsEditBoxCharElement*)m_listText->removeEx(from);
			delete el->bmp;
			yifree(el);
			//yifree(m_listText->removeEx(from));
		}
		nDelCnt+=(to-from);
	}

	if (m_bIsStaticEditorMode==False &&
		m_bNowModeEditor==True)
	{
		PostMessage(m_hNotifyWnd, WM_COMMAND, ((EN_CHANGE<<16) | m_id), (LPARAM)m_hWnd);
	}
	return True;
}

// -----------------------------
Bool cglsEditBox::SetCarotPos(int pos)
{
	if (pos==-1)
	{
		if (m_listText)
		{
			m_posCarot=m_listText->count();
		}
		else
		{
			m_posCarot=0;
		}
	}
	else
	{
		m_posCarot=pos;
	}
	InvalidateRect(0);
	return True;
}


// -----------------------------
Bool cglsEditBox::SetCurSel(int to)
{
	//if (to==-1)
	//{
	//	m_posSelection=;
	//}

	if (to==-1)
	{
		m_posSelection=m_listText->count();
	}
	else
		m_posSelection=to;

	UpdateText();
	InvalidateRect(0);

	return True;
}

#include "glsStatic.h"

static Bitmap* xCreateStringBitmap(wchar_t ch, Font* m_font, color_t colorFont, color_t colorBg)
{
	wchar_t chs[2]={ch, 0};
	return cyiShape::BitmapStringEx(0, chs, m_font, colorFont, colorBg);
	//return cyiShape::BitmapString(0, chs, m_font, &SolidBrush(colorFont));
}

// -----------------------------
int cglsEditBox::GetTopLineHeight(int height)
{
	cyiNode* node=0;
	for (node = m_listLineInfo->GetNodeTopFrom(0); node; node=node->np)
	{
		int nTotalHeight=(node->data>>8);
		int nHeight = (node->data&0xFF);

		if (height<=nTotalHeight)
			break;
	}

	if (node==0)/* ||
			node->pp==0)*/
	{
		return 0;
	}
	return (node->data&0xFF);
}

// -----------------------------
int cglsEditBox::GetBottomLineHeight(int height)
{
	cyiNode* node=0;
	for (node = m_listLineInfo->GetNodeTailFrom(0); node; node=node->pp)
	{
		int nTotalHeight=(node->data>>8);
		int nHeight = (node->data&0xFF);

		if (height>=nTotalHeight)
			break;
	}

	if (node==0 ||
			node->np==0)
	{
		node = m_listLineInfo->GetNodeTailFrom(0);

		if (node)
		{
			return (node->data&0xFF);
		}
		return 0;
	}
	return (node->np->data&0xFF);
}


// -----------------------------
Bool cglsEditBox::UpdateTextForMultiline()
{
	float fPrevLineHeight=0.0;
	float nTotalHeight=0;
	float nLineWidth=0;
	float nLineHeight=0;
	int nMaxHeight=0;
	int nClientWidth=m_w-2;
	int w=0, h=0;
	int nLineCnt=0;

	if (m_bmpBuffer)
	{
		w=m_bmpBuffer->GetWidth();
		h=m_bmpBuffer->GetHeight();
	}

	m_listLineInfo->clear();

	// まず、全体のサイズを計算する。
	for (cTextNode* node = m_listText->GetNodeTopFrom(0); node; node=node->np)
	{
		sGlsEditBoxCharElement* el=node->ref;

		nLineHeight=nLineHeight>el->height?nLineHeight:el->height;

		if (el->ch==L'\n')
		{
			nTotalHeight+=nLineHeight;
			m_listLineInfo->add((yiAnyValue)((int)nLineHeight|((int)nTotalHeight<<8)));
			//nLineHeight=nLineHeight;
			fPrevLineHeight=nLineHeight;
			nLineHeight=0.0;
			nLineWidth=0;
			continue;
		}

		if (el->ch==' ')
		{
			if (nLineWidth+el->width > (float)nClientWidth)
			{
				nTotalHeight+=nLineHeight;
				nLineHeight=0;
				nLineWidth=0;
			}

			nLineWidth+=el->width;
			continue;
		}

		if (el->bmp==0)
		{
			el->bmp=xCreateStringBitmap(el->ch, el->font?el->font:GetFont(), this->m_colorFont, m_colorBg);
		}

		if (nLineWidth+el->width>(float)nClientWidth)
		{
			nTotalHeight+=nLineHeight;
			m_listLineInfo->add((yiAnyValue)((int)nLineHeight|(int)nTotalHeight<<8));

			nLineHeight=el->height;
			nLineWidth=0;
		}

		nLineWidth+=el->width;
	}
	nTotalHeight+=nLineHeight==0?fPrevLineHeight:nLineHeight;

	if (h!=(int)nTotalHeight+1)
	{
		delete m_bmpBuffer;
		m_bmpBuffer=BITMAP32(w, (int)nTotalHeight+1);
	}

	if (m_bmpBuffer->GetHeight()>m_h-2/* &&
		h!=(int)nTotalHeight+1*/)
	{
		if (m_scroll==0)
		{
			m_scroll = new cglsScrollBar();
			m_scroll->Create(this);
			m_scroll->SetScrollNotifyWindow(*this);
		}
		m_scroll->SetRange(0, nTotalHeight+1, m_h-2);

		float all = nTotalHeight-(m_h-2);
		float factor = ((float)m_nScreenTop/all)*(nTotalHeight+1);
		//InvalidateRect(0);
		m_scroll->SetPos((int)factor);
		m_scroll->Show();
	}
	else
	{
		if (m_scroll)
			m_scroll->Hide();
	}

	// 描画する。
	float fSumWidth=0.0;
	float fSumHeight=0.0;

	cyiBitmapBlender::FillColor(m_bmpBuffer, m_colorBg);//Color::White);
	
	int nSelStart=0;
	int nSelEnd=0;
	Bool bHasSelection=False;

	if (m_posCarot!=m_posSelection)
	{
		nSelStart=m_posCarot>m_posSelection?m_posSelection:m_posCarot;
		nSelEnd=m_posCarot<m_posSelection?m_posSelection:m_posCarot;
		bHasSelection=True;
	}

	int count=0;
	int nCharCount = (int)((float)m_listText->count()*m_fShowCharFactor);
	for (cTextNode* node = m_listText->GetNodeTopFrom(0); node; node=node->np, count++)
	{
		if (count>nCharCount)
			break;
		
		sGlsEditBoxCharElement* el=node->ref;

		nLineHeight=nLineHeight>el->height?nLineHeight:el->height;

		if (el->ch==L'\n')
		{
			fSumHeight+=nLineHeight;
			nLineHeight=0;
			fSumWidth=0;
			continue;
		}

		if (el->ch==L' ')
		{
			if (nLineWidth+el->width > (float)nClientWidth)
			{
				fSumHeight+=nLineHeight;
				//nLineHeight=0;
				nLineHeight=el->height;
			}
			
			fSumWidth+=el->width;
			continue;
		}

		if (fSumWidth+el->width>(float)nClientWidth)
		{
			fSumWidth=0.0;
			fSumHeight+=nLineHeight;
			nLineHeight=el->height;
		}
		
		//g.DrawImage(el->bmp, ffSumWidth, fSumHeight);

		if (bHasSelection==False)
		{
			cyiBitmapBlender::BitmapCopy(m_bmpBuffer, el->bmp, (int)fSumWidth, (int)fSumHeight);
			fSumWidth+=el->width;
			continue;
		}

		if (count>=nSelStart && count<nSelEnd)
		{
			Bitmap* bmpSel = ::xCreateStringBitmap(el->ch, GetFont(), m_colorSelectionFont, m_colorSelection);
			cyiBitmapBlender::BitmapCopy(m_bmpBuffer, bmpSel, (int)fSumWidth, (int)fSumHeight);
			delete bmpSel;
		}
		else
		{
			cyiBitmapBlender::BitmapCopy(m_bmpBuffer, el->bmp, (int)fSumWidth, (int)fSumHeight);
		}
		fSumWidth+=el->width;
	}

	if (m_listText->count()==0)
	{
		if (m_pwszInitialComment)
		{
			Graphics g(m_bmpBuffer);
			Bitmap* bmpStr=cyiShape::BitmapStringEx(0, m_pwszInitialComment, GetFont(), Color::Gray, m_colorBg);
			g.DrawImage(bmpStr, (INT)1, (INT)xTEXT_MARGINE); 
			delete bmpStr;
		}
	}

	float fPosCarotY;
	float nPosCarot = GetPosToCoordiate(m_posCarot, fPosCarotY);

	if (m_bmpCarot==0)
	{
		m_bmpCarot=CreateCarot();
		m_bmpCarotSelection=CreateCarot();
	}

	if (fPosCarotY+m_bmpCarot->GetHeight()>m_h+m_nScreenTop)
	{
		m_nScreenTop=fPosCarotY+m_bmpCarot->GetHeight()-m_h;
	}
	else if (fPosCarotY<m_nScreenTop)
	{
		m_nScreenTop=fPosCarotY;
	}


	return True;
}

// -----------------------------
Bool cglsEditBox::UpdateText()
{
	float sumWidth=0;
	float nBufferWidth=0;

	if (m_bIsMultiLine==True)
	{
		return UpdateTextForMultiline();
	}


	// スタティックモード時
	if (m_bNowModeEditor==False)
	{
		if (m_nAutoResizeWidth!=0)
		{
			for (cTextNode* node = m_listText->GetNodeTopFrom(0); node; node=node->np)
			{
				sGlsEditBoxCharElement* el=node->ref;//(sGlsEditBoxCharElement*)node->ref;
				nBufferWidth+=el->width;
			}

			if (m_nAutoResizeWidth!=-1 &&
					m_nAutoResizeWidth > nBufferWidth)
			{
				ReSize(m_nAutoResizeWidth, m_h);
			}
			else
			{
				ReSize((UINT)(nBufferWidth)+1, m_h);
			}

		}
		return True;
	}

	if (m_bmpBuffer)
		delete m_bmpBuffer;

	if (m_listText==0)
	{
		m_listText = new cyitList<sGlsEditBoxCharElement>();
	}

	for (cTextNode* node = m_listText->GetNodeTopFrom(0); node; node=node->np)
	{
		sGlsEditBoxCharElement* el=node->ref;//(sGlsEditBoxCharElement*)node->ref;
		nBufferWidth+=el->width;
	}


	// 文字列の長さによって幅のサイズが変更されるモード時
	if (m_nAutoResizeWidth!=0)
	{
		if (m_nAutoResizeWidth!=-1 &&
				m_nAutoResizeWidth > nBufferWidth)
		{
			ReSize(m_nAutoResizeWidth, m_h);
		}
		else
		{
			ReSize((UINT)(nBufferWidth)+1, m_h);
		}
	}

	if (m_bmpCarot==0)
	{
		m_bmpCarot=CreateCarot();
		m_bmpCarotSelection=CreateCarot(True);
	}

	int w = (int)(nBufferWidth)+1+m_bmpCarot->GetWidth()+1;
	m_bmpBuffer = BITMAP32(w>(m_w-2)?w:(m_w-2), m_h-xTEXT_MARGINE);
	cyiBitmapBlender::FillColor(m_bmpBuffer, m_colorBg);
	Graphics g(m_bmpBuffer);
	g.SetTextRenderingHint(TextRenderingHintAntiAlias);


	// セレクションエリアがある場合
	if (m_posCarot!=m_posSelection)
	{
		float y;
		int nSelectStart = GetPosToCoordiate(m_posCarot<m_posSelection?m_posCarot:m_posSelection, y);
		int nSelectEnd = GetPosToCoordiate(m_posCarot>m_posSelection?m_posCarot:m_posSelection, y);
		g.SetSmoothingMode(SmoothingModeHighSpeed);
		g.FillRectangle(&SolidBrush(m_colorSelection), Rect(nSelectStart, xTEXT_MARGINE, nSelectEnd-nSelectStart+1, m_bmpCarot->GetHeight()));
	}

	g.SetSmoothingMode(SmoothingModeHighQuality);


	// ここでm_bmpBufferに描画を行う。
	// 描画はパスワードモード時と一般モード時と異なる。
	if (m_bIsPasswordMode==True)
	{
		int cnt=0;

		sumWidth=0;
		for (cTextNode* node = m_listText->GetNodeTopFrom(); node; node=node->np)
		{
			sGlsEditBoxCharElement* el=node->ref;//(sGlsEditBoxCharElement*)node->ref;

			if (IsSelectionArea(cnt++)==True)
			{
				wchar_t buffer[2] = {L'*', 0};
				//g.DrawString(&el->ch, 1, m_font, RectF(sumWidth, xTEXT_MARGINE, m_bmpBuffer->GetWidth()-sumWidth, m_bmpBuffer->GetHeight()), 0, &SolidBrush(Color(m_colorSelectionFont)));
				Bitmap* bmpStr=cyiShape::BitmapStringEx(0, buffer, GetFont(), m_colorSelectionFont, m_colorSelection);
				g.DrawImage(bmpStr, (INT)sumWidth, (INT)xTEXT_MARGINE); 
			}
			else
			{
				wchar_t buffer[2] = {L'*', 0};
				Bitmap* bmpStr=cyiShape::BitmapStringEx(0, buffer, GetFont(), m_colorFont, m_colorBg);
				g.DrawImage(bmpStr, (INT)sumWidth, (INT)xTEXT_MARGINE); 
				//g.DrawString(&el->ch, 1, m_font, RectF(sumWidth, xTEXT_MARGINE, m_bmpBuffer->GetWidth()-sumWidth, m_bmpBuffer->GetHeight()), 0, &SolidBrush(Color(m_colorFont)));
			}
			sumWidth+=m_nPasswordWidth;
		}

		if (m_listText->count()==0)
		{
			if (m_pwszInitialComment)
			{
				Bitmap* bmpStr=cyiShape::BitmapStringEx(0, m_pwszInitialComment, GetFont(), Color::Gray, m_colorBg);
				g.DrawImage(bmpStr, (INT)1, (INT)xTEXT_MARGINE); 
			}
		}
	}
	else // これが通常モード
	{
		int cnt=0;

		sumWidth=0;
		int nCharCount = (int)((float)m_listText->count()*m_fShowCharFactor);
		for (cTextNode* node = m_listText->GetNodeTopFrom(); node; node=node->np,cnt++)
		{
			if (cnt>nCharCount)
				break;
			sGlsEditBoxCharElement* el=node->ref;//(sGlsEditBoxCharElement*)node->ref;

			if (IsSelectionArea(cnt++)==True)
			{
				wchar_t buffer[2] = {el->ch, 0};
				//g.DrawString(&el->ch, 1, m_font, RectF(sumWidth, xTEXT_MARGINE, m_bmpBuffer->GetWidth()-sumWidth, m_bmpBuffer->GetHeight()), 0, &SolidBrush(Color(m_colorSelectionFont)));
				Bitmap* bmpStr=cyiShape::BitmapStringEx(0, buffer, GetFont(), m_colorSelectionFont, m_colorSelection);
				g.DrawImage(bmpStr, (INT)sumWidth, (INT)xTEXT_MARGINE); 
			}
			else
			{
				wchar_t buffer[2] = {el->ch, 0};
				Bitmap* bmpStr=cyiShape::BitmapStringEx(0, buffer, GetFont(), m_colorFont, m_colorBg);
				g.DrawImage(bmpStr, (INT)sumWidth, (INT)xTEXT_MARGINE); 
				//g.DrawString(&el->ch, 1, m_font, RectF(sumWidth, xTEXT_MARGINE, m_bmpBuffer->GetWidth()-sumWidth, m_bmpBuffer->GetHeight()), 0, &SolidBrush(Color(m_colorFont)));
			}
			sumWidth+=el->width;
		}

		if (m_listText->count()==0)
		{
			if (m_pwszInitialComment)
			{
				Bitmap* bmpStr=cyiShape::BitmapStringEx(0, m_pwszInitialComment, GetFont(), Color::Gray, m_colorBg);
				g.DrawImage(bmpStr, (INT)1, (INT)xTEXT_MARGINE); 
			}
		}
	}

	//buffer[xxx]=0;
	//g.DrawString(buffer, ::wcslen(buffer), m_font, RectF(0, 0, m_bmpBuffer->GetWidth(), m_bmpBuffer->GetHeight()), 0, &SolidBrush(Color(m_colorFont)));
	return True;
}

// -----------------------------
void cglsEditBox::Update(Bitmap* bmp, RECT* rect, RECT* _childRect, int offsetX, int offsetY, Bool bIsLocalBufferUpdate)
{


	// サイズ変更時
	if (m_item.pBmp[0]==0 ||
			m_item.pBmp[0]->GetWidth()!=m_w ||
				m_item.pBmp[0]->GetHeight()!=m_h)
	{
		delete m_item.pBmp[0];
		m_item.pBmp[0]=BITMAP32(m_w, m_h);
		Graphics  g(m_item.pBmp[0]);
		g.FillRectangle(&SolidBrush(m_colorBg), 0, 0, m_w, m_h);
		g.DrawRectangle(&Pen(Color(m_colorBorder)), 0, 0, m_w-1,m_h-1);

		if (m_bmpCarot)
			delete m_bmpCarot;

		m_bmpCarot = CreateCarot();

		// ここで作っておく。
		//m_bmpContextText=;
	}

	m_item.pCurrentBmp=m_item.pBmp[0];

	float fPosCarotY;
	float nPosCarot = GetPosToCoordiate(m_posCarot, fPosCarotY);
	//xTRACE("cglsEditBox::Update() trace.......1");
	if (m_bIsMultiLine==False)
	{
		if (nPosCarot<m_posStart)
		{
			m_posStart=nPosCarot;
		}
		else if (nPosCarot > (m_w-2+m_posStart-2))
		{
			m_posStart = nPosCarot-m_w+2+2;
		}
	}


	int w=m_w-2;
	if (m_bIsMultiLine==False)
		w = (m_bmpBuffer->GetWidth()-(int)m_posStart) < m_w-2?(m_bmpBuffer->GetWidth()-(int)m_posStart):m_w-2;

	if (m_bNowModeEditor==False)
	{
		int nLength = m_listText->count();

		if (nLength==0)
			return ;

		wchar_t* pszText = (wchar_t*)yialloci((nLength+1)<<1);
		int cnt=0;
		float fWidth=0.0;
		float fHeight=0.0;
		Font* font=0;

		for (cTextNode* node = m_listText->GetNodeTopFrom(0); node; node=node->np)
		{
			sGlsEditBoxCharElement* el=node->ref;//(sGlsEditBoxCharElement*)node->ref;
			pszText[cnt++]=el->ch;
		}

		// もし、フォントサイズが可変ならば、ここで調整する。
		if (m_bIsFontSizeVariable==True)
		{
			font = cglsStatic::xGetVariableFont(GetFont(), pszText, 0, 0, m_w, m_h);

		}
	
		unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, pszText, font?font:GetFont(), 0, 0);

		int x=0;
		int y=0;
		int w=(pos>>16);
		int h=(pos&0xFFFF);

		w=w>(m_w-2)?(m_w-2):w;
		h=h>(m_h)?(m_h):h;

		// ここで開始座標取得
		if (m_eLayout==eGEBL_CENTER)
		{
			x=((m_w-2-w)>>1);
		}
		else if (m_eLayout==eGEBL_RIGHT)
		{
			x=(m_w-1-w);
		}

		y=((m_h-h)>>1);
		//y=(m_h-h)-1;

		RECT xrect = {x, y, w, h};
		RECT resultRect;

		if (xIsInRect(&xrect, _childRect, resultRect)==True)
		{
			Bitmap* bmpStr = GetParentBg(0, &xrect);

			cyiShape::BitmapStringEx(bmpStr, pszText, font?font:GetFont(), m_colorStaicModeFont, 0);
			cyiBitmapBlender::BitmapCopy(bmp, bmpStr, resultRect.left+m_nAbsX+1, resultRect.top+m_nAbsY, 0/*resultRect.left*/, 0/*resultRect.top-y*/, resultRect.right, resultRect.bottom);
			delete bmpStr;

		}

		if (pszText)
			yifree(pszText);

		if (font)
			delete font;

		return ;
	}
	else
	{
		//cyiBitmapBlender::FillColor(m_item.pCurrentBmp, m_colorBg);
		Graphics g(m_item.pCurrentBmp);
		g.FillRectangle(&SolidBrush(m_colorBg), 1,1,m_w-2, m_h-2);

		if (m_bIsMultiLine==False)
		{

			cyiBitmapBlender::BitmapCopy(m_item.pCurrentBmp, m_bmpBuffer, 1, 1, 0, m_nScreenTop, w, m_bmpBuffer->GetHeight());
		}
		else
		{
			int h=m_bmpBuffer->GetHeight()>(m_h-2)?m_bmpBuffer->GetHeight():m_h-2;
			cyiBitmapBlender::BitmapCopy(m_item.pCurrentBmp, m_bmpBuffer, 1, 1, (int)0, m_nScreenTop, w, h);//m_bmpBuffer->GetHeight());
		}

		if (m_bIsCarotShow)
		{
			//int x = GetPosToCoordiate(m_posCarot);
			Graphics g(m_item.pCurrentBmp);
			int x=(int)(nPosCarot)+1-(int)(m_posStart);
			int y=(xTEXT_MARGINE+1)+(int)fPosCarotY;

			if (m_bmpCarot==0)
			{
				m_bmpCarot=this->CreateCarot(False);
				this->m_bmpCarotSelection=this->CreateCarot(True);
			}

			if (x+m_bmpCarot->GetWidth()>w)
			{
				x=w-m_bmpCarot->GetWidth();
				
			}

			if (m_posCarot==m_posSelection)
			{
				g.DrawImage(m_bmpCarot, (int)x, (xTEXT_MARGINE+1)+(int)fPosCarotY-m_nScreenTop);
			}
			else
			{
				g.DrawImage(m_bmpCarotSelection, (int)x, (xTEXT_MARGINE+1)+(int)fPosCarotY-m_nScreenTop);//(int)(nPosCarot)+1-(int)(m_posStart), (xTEXT_MARGINE+1));
			}
		}
	}

	// 一文字もなかった場合に
	// ContextTextを表示する。


	m_item.rect.x=m_x;
	m_item.rect.y=m_y;
	m_item.rect.w=m_item.pCurrentBmp->GetWidth();
	m_item.rect.h=m_item.pCurrentBmp->GetHeight();

	cGlassInterface::Update(bmp, rect, _childRect, offsetX, offsetY, bIsLocalBufferUpdate);

}

// -----------------------------
void cglsEditBox::Update(eGlsUpdateMode mode, eDrawItemStatus status)
{
	__super::Update(mode, status);

	int nFontHeight=GetFontHeight(True);
	int h=nFontHeight+(xTEXT_MARGINE<<1);


	if (m_bIsHeightIsFontSize==True)
	{
		// フォントサイズが変更された場合
		if (m_h!=h)
		{
			if (m_item.pBmp[0])
				delete m_item.pBmp[0];

			ReSize(m_w, h);
			m_item.pBmp[0] = BITMAP32(m_w, m_h);
			m_item.pCurrentBmp=m_item.pBmp[0];
		
			if (m_bIsPasswordMode==True)
			{
				float height;
				m_nPasswordWidth=GetCharWidth(L'*', height);
				for (cTextNode* node = m_listText->GetNodeTopFrom(0); node; node=node->np)
				{
					sGlsEditBoxCharElement* el=node->ref;//(sGlsEditBoxCharElement*)node->ref;
					el->width=m_nPasswordWidth;//this->GetCharWidth(el->ch, el->height);
				}
			}
			else
			{
				for (cTextNode* node = m_listText->GetNodeTopFrom(0); node; node=node->np)
				{
					sGlsEditBoxCharElement* el=node->ref;//(sGlsEditBoxCharElement*)node->ref;
					el->width=this->GetCharWidth(el->ch, el->height);
				}
			}
		}
	}

	UpdateText();
}

// -------------------------------
Bitmap* cglsEditBox::CreateCarot(Bool bIsSelection)
{
	//Graphics gx(m_item.pBmp[0]);
	//int h =  (int)GetFont()->GetHeight(&gx);
	int h = GetFontHeight(True);

	if (h==0)
		return 0;

	Bitmap* bmp=BITMAP32(1,  h);
	Graphics g(bmp);

	if (bIsSelection==False)
	{
		g.FillRectangle(&SolidBrush(m_colorFont), 0, 0, 1, h);
	}
	else
	{
		g.FillRectangle(&SolidBrush(((~m_colorSelection)|0xFF000000)), 0, 0, 1, h);
	}

	return bmp;
}

// -----------------------------
LRESULT cglsEditBox::OnMouseMove(WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	handled = True;

	if (m_bPushed==True)
	{
		POINT pt;

		//if (m_bIsSetCapture==False)
		//{
		//	SetCapture(m_hWnd);
		//	m_bIsSetCapture=True;
		//}

		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);
		
		if (pt.x<0)
			pt.x=0;
		if (pt.x>m_w)
			pt.x=m_w;
		if (pt.y<0)
			pt.y=0;
		if (pt.y>m_h)
			pt.y=m_h;

		int n = m_bIsMultiLine==False?GetCoordiateToPos(pt.x):GetCoordiateToPos(pt.x, pt.y);

		if (m_posCarot==n)
		{
			return 0;
		}

		if (GetCapture()!=m_hWnd)
		{
			SetCapture(m_hWnd);
		}

		m_posCarot=n;

		UpdateText();
		InvalidateRect(0);

	}
	return 0;
}

// -----------------------------
LRESULT cglsEditBox::OnMouseLButtonDblClick(WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	handled=False;

	if (m_bNowModeEditor==False)
	{
		m_bNowModeEditor=True;
		SetFocus(m_hWnd);

		if (m_assistant)
		{
			wchar_t buffer[256];
			POINT pt={0, m_h};

			m_posCarot=0;

			if (m_listText)
			{
				m_posCarot=m_listText->count();
			}
			SetCurSel(0);

			UpdateText();
			InvalidateRect(0, False, True);
			ClientToScreen(m_hWnd, &pt);
			
			wchar_t result[256];
			GetText(result, 256);

			//m_assistant->SetDefaultValue(result);
			m_assistant->ShowAssistant(GetParent(m_hWnd), pt.x, pt.y);

			handled=True;
			
			if (m_assistant->IsChanged()==True)
			{
				if (m_assistant->GetResult(buffer)==True)
				{
					SetText(buffer);
				}
				SendMessage(m_hNotifyWnd, WM_COMMAND, m_id, (LPARAM)m_assistant);
			}
			m_bNowModeEditor=False;

			//SetTimer(m_hWnd, 1, this->m_nCarotTimer, 0);
			UpdateText();
			InvalidateRect(0);
			
			return 0;
		}
		else
		{
			m_bIsCarotShow=1;
			SetTimer(m_hWnd, 1, m_nCarotTimer, 0);
			
			m_posCarot=0;

			if (m_listText)
			{
				m_posCarot=m_listText->count();
			}
		}
		
		SetCurSel(0);//m_listText->count());
		UpdateText();
		InvalidateRect(0);
	}
	return 0;
}

// -----------------------------
LRESULT cglsEditBox::OnSetFocus(WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	m_bIsCarotShow=True;
	SetTimer(m_hWnd, 1, m_nCarotTimer, 0);

	InvalidateRect(0);
	return 0;
}


// -----------------------------
LRESULT cglsEditBox::OnKillFocus(WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	
	if (m_bIsStaticEditorMode==True &&
		m_bNowModeEditor==True)
	{
		if (m_assistant)
		{
			return 0;
		}

		m_bNowModeEditor=False;
		KillTimer(m_hWnd, 1);
		m_bIsCarotShow=0;
		m_posCarot=0;

		UpdateText();
		InvalidateRect(0);

		PostMessage(m_hNotifyWnd, WM_COMMAND, ((EN_CHANGE<<16) | m_id), (LPARAM)m_hWnd);
		return 0;
	}

	KillTimer(m_hWnd, 1);

	m_bIsCarotShow=0;
	m_posSelection=m_posCarot;
	UpdateText();
	InvalidateRect(0);

	return 0;
}

// -----------------------------
LRESULT cglsEditBox::OnMouseLButtonUp(WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	handled=True;

	if (GetCapture()==m_hWnd)
	{
		ReleaseCapture();
	}

	return 0;
}

typedef struct {
	int start;
	int delta;
} sEditScrollAnit;

// -----------------------------
void cglsEditBox::OnFontChanged()
{
	if (m_bIsHeightIsFontSize==True)
	{
		__super::OnFontChanged();
		Update(eGUM_FORCED);
	}
}

// ---------------------------------------
LRESULT cglsEditBox::OnMouseWheel(WPARAM wp, LPARAM lp, BOOL& handled)
{
	int zDelta = GET_WHEEL_DELTA_WPARAM(wp); 
	int n=zDelta/120;
	int delta=0;

	n=-n;

	if (m_scroll==0)
		return 0;

	if (m_bIsMultiLine==False)
	{
		return 0;
	}

	if (n==-1)
	{
		delta = -GetTopLineHeight(m_nScreenTop);
	}
	else if (n==1)
	{
		delta = GetTopLineHeight(m_nScreenTop);
	}
	else if (n<0)
	{
		delta = -(m_h-2);
	}
	else
	{
		delta = (m_h-2);//GetTopLineHeight(m_nScreenTop);
	}

	if (m_hScrollAnit)
	{
		sEditScrollAnit s = {m_nScreenTop, delta};
		if (yiAnitIsAlived(m_hScrollAnit)==True)
		{
			yiAnitStop(m_hScrollAnit, True);
		}
		yiAnitStart(m_hScrollAnit, m_hWnd, WM_EDIT_SCROLL, 100, 10, eGAP_FISO, yitoheap(&s, sizeof(s)));
		return 1;
	}


	if (m_nScreenTop<0)
		m_nScreenTop=0;

	if (m_nScreenTop> m_bmpBuffer->GetHeight()-(m_h-2))
	{
		m_nScreenTop=m_bmpBuffer->GetHeight()-(m_h-2);
	}
	
	//float factor = ((float)m_nScreenTop/all)*(m_bmpBuffer->GetHeight());

	InvalidateRect(0);
	m_scroll->SetPos(m_nScreenTop);

	return 0;
}

// ---------------------------------------
LRESULT cglsEditBox::OnVScroll(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	handled=True;
	int nAnimationTerm=100;
	cglsScrollBar* bar = (cglsScrollBar*)lParam;
	int cur = bar->GetPos();
	int delta=0;
	//float all = m_bmpBuffer->GetHeight()-(m_h-2);

	switch(LOWORD(wParam)) {
	case SB_LINEUP:
		//m_nScreenTop-=GetTopLineHeight(m_nScreenTop);//m_nFontHeight;
		delta = -GetTopLineHeight(m_nScreenTop);
		break;
	case SB_LINEDOWN:
		delta = GetBottomLineHeight(m_nScreenTop+m_bmpBuffer->GetHeight());
		//m_nScreenTop+=GetBottomLineHeight(m_nScreenTop+m_bmpBuffer->GetHeight());
		break;
	case SB_PAGEUP:
		delta=-(m_h-2);
		nAnimationTerm=200;
		//m_nScreenTop-=m_h;
		break;
	case SB_PAGEDOWN:
		nAnimationTerm=200;
		delta=(m_h-2);
		//m_nScreenTop+=m_h;
		break;
	case SB_THUMBPOSITION:
		{
			m_nScreenTop=cur;
			InvalidateRect(0);
			return 1;
		}
	}

	if (m_hScrollAnit)
	{

		sEditScrollAnit s = {m_nScreenTop, delta};
		if (yiAnitIsAlived(m_hScrollAnit)==True)
		{
			yiAnitStop(m_hScrollAnit, True);
		}				
		yiAnitStart(m_hScrollAnit, m_hWnd, WM_EDIT_SCROLL, nAnimationTerm, 10, eGAP_FISO, yitoheap(&s, sizeof(s)));
		return 1;
	}


	if (m_nScreenTop<0)
		m_nScreenTop=0;

	if (m_nScreenTop> m_bmpBuffer->GetHeight()-(m_h-2))
	{
		m_nScreenTop=m_bmpBuffer->GetHeight()-(m_h-2);
	}
	
	//float factor = ((float)m_nScreenTop/all)*(m_bmpBuffer->GetHeight());

	InvalidateRect(0);
	bar->SetPos(m_nScreenTop);
	return 1;
}

// ---------------------------------------
LRESULT cglsEditBox::OnScroll(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	float factor;
	sEditScrollAnit* s = (sEditScrollAnit*)lParam;

	int cnt=wParam;

	// アニメーションが既にきゃんせるされている
	if (s==0)
	{
		//何もせず。
		return 0;
	}

	if (yiAnitIsAlived(m_hScrollAnit)==False)
	{
		yiAnitStop(m_hScrollAnit, False);
		return 0;
	}

	if ((factor=yiAnitGetFactor(m_hScrollAnit, cnt))<0.0)
	{
		yiAnitStop(m_hScrollAnit, False);

		m_nScreenTop=s->start+s->delta;

		if (m_nScreenTop<0)
			m_nScreenTop=0;

		if (m_nScreenTop> m_bmpBuffer->GetHeight()-(m_h-2))
		{
			m_nScreenTop=m_bmpBuffer->GetHeight()-(m_h-2);
		}

		m_scroll->SetPos(m_nScreenTop);
		InvalidateRect(0);
		yifree(s);
		return 0;

	}
	else
	{
		m_nScreenTop=s->start+(s->delta*factor);

		if (m_nScreenTop<0)
			m_nScreenTop=0;

		if (m_nScreenTop> m_bmpBuffer->GetHeight()-(m_h-2))
		{
			m_nScreenTop=m_bmpBuffer->GetHeight()-(m_h-2);
		}
		
		m_scroll->SetPos(m_nScreenTop);

		InvalidateRect(0);
	}

	return 0;
}


// -------------------------------
//LRESULT cglsEditBox::OnSetText(WPARAM wParam, LPARAM lParam, Bool& handled)
//{
//	LPCSTR txt = (LPCTSTR )lParam;
//	int n = strlen(txt);
//
//	wchar_t* pszUnicode=yiToUnicode(0, txt, 0);
//	m_listText->clear(yifree);
//
//	m_posCarot=0;
//	m_posStart=0;
//	m_posSelection=0;
//
//	InsertString(0, pszUnicode);
//
//	yifree(pszUnicode);
//
//	return 0;
//}
//
//// -------------------------------
//LRESULT cglsEditBox::OnGetText(WPARAM wParam, LPARAM lParam, Bool& handled)
//{
//	return 0;
//}