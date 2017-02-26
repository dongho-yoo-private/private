#include "yctrlPreviewGoban.h"
#include "yctrlGobanResource.h"
#include "resource.h"


static int xGetJosekiName(cyiTreeNode* node, int& _h, int& _l, int& _d);

// --------------------------------
const eGoResultStatus g_status_code[10] = {
	eGRS_UNKNOWN, 
	eGRS_SAME, 
	eGS_BLACK_GOOD, 
	eGS_WHITE_GOOD, 
	eGS_BLACK_LITTLE_GOOD, 
	eGS_WHITE_LITTLE_GOOD,
	eGS_BLACK_TEREBLE,
	eGS_WHITE_TEREBLE,
	eGRS_BLACK_OTHER, // 状況次第
	eGRS_WHITE_OTHER  // 状況次第
};

// ---------------------------------
static Bitmap* xMakeGoban(int width, int nLineCount);
static Bitmap* xMakeStone(Bool bIsBlack, int w);

cyctrlPreviewGoban::cyctrlPreviewGoban(void)
{
}

cyctrlPreviewGoban::~cyctrlPreviewGoban(void)
{
}

#define PREV_WINDOW_CLASS "PreviewGobanClass"

// ---------------------------------
Bool cyctrlPreviewGoban::Create(HWND hWnd, int width, int nCount, int nLineCount)
{
	HINSTANCE hInstance = GetModuleHandle("yctrlGo.dll"); // グローバル変数にインスタンス処理を格納します。
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GOYOTSUYA));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)NULL_BRUSH;//(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_GOYOTSUYA);
	wcex.lpszClassName	= PREV_WINDOW_CLASS;
	wcex.hIconSm		= 0;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);

	m_hNotifyWnd=hWnd;
	m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, PREV_WINDOW_CLASS, 0, WS_CLIPCHILDREN|WS_OVERLAPPED|WS_SYSMENU|WS_VSCROLL,
      CW_USEDEFAULT, 0, width, width*nCount, hWnd, NULL, hInstance, this);

	if (!m_hWnd)
	{
	  return 0;
	}


	MouseTrackMode(True);

	RECT rect;
	GetClientRect(m_hWnd, &rect);

	m_prevNode=0;
	m_bModePlay=False;
	m_nShowCnt=nCount;
	m_nStartIndex=0;
	m_nPrevWidth=(rect.right);
	m_bIsCreated=True;
	memset(m_gobanz, 0, sizeof(m_gobanz));
	m_nHitCount=0;
	::SetScrollRange(m_hWnd, SB_VERT,0, 0, TRUE);
	::SetScrollPos(m_hWnd, SB_VERT, 0, TRUE);


	memset(&m_sSearchFilter, 0, sizeof(m_sSearchFilter));
	m_cmbFilter.Create(m_hWnd, 5, 2, (int)(m_nPrevWidth*m_nShowCnt), m_nPrevWidth, m_nPrevWidth/4);
	//m_btnDetail.Create(m_hWnd, 6, m_nPrevWidth*0.7+2, (int)(m_nPrevWidth*m_nShowCnt), m_nPrevWidth*0.3, m_nPrevWidth/4, "...");

	// てきとー
	Font* font = cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL);
	
	HDC hdc = GetDC(m_hWnd);
	Graphics g(hdc);
	LOGFONTA logfnt;

	font->GetLogFontA(&g, &logfnt);
	ReleaseDC(m_hWnd, hdc);

	HFONT hFont = CreateFontIndirect(&logfnt);

	SendMessage(m_cmbFilter.m_hWnd, WM_SETFONT, (WPARAM)hFont ,True);

	m_cmbFilter.Show();
	//m_btnDetail.Show();

	m_cmbFilter.AddString("全て");
	m_cmbFilter.AddString("定石");
	m_cmbFilter.AddString("はめ手");
	m_cmbFilter.AddString("その他の変化");
	m_cmbFilter.AddString("定石後の変化");
	m_cmbFilter.AddString("手抜き定石");
	m_cmbFilter.AddString("場合の手");
	m_cmbFilter.AddString("編集済みでない");
	m_cmbFilter.AddString("再生済みでない");
	m_cmbFilter.AddString("同一型");
	m_cmbFilter.AddString("お気に入り");
	m_cmbFilter.AddString("条件検索");



	m_nFilter=0;
	m_cmbFilter.SetSelectIndex(0);

	m_btnPrev.Create(m_hWnd, 1, 2, m_nPrevWidth+2, m_nPrevWidth/2, m_nPrevWidth/4, "<");
	m_btnPlay.Create(m_hWnd, 2, m_nPrevWidth/2+2, m_nPrevWidth+2, m_nPrevWidth/2, m_nPrevWidth/4, ">");
	m_btnPlayTop.Create(m_hWnd, 3, 2, m_nPrevWidth+m_nPrevWidth/4+4, m_nPrevWidth/2, m_nPrevWidth/4, "|<");
	m_btnPlayEnd.Create(m_hWnd, 4, m_nPrevWidth/2+2, m_nPrevWidth+m_nPrevWidth/4+4, m_nPrevWidth/2, m_nPrevWidth/4, ">|");

	m_btnPlay.Hide();
	m_btnPrev.Hide();
	m_btnPlayTop.Hide();
	m_btnPlayEnd.Hide();

   m_nLineCount=nLineCount;
   m_bmpGoban = cyctrlGobanResource::GetInstance().GetGobanBitmap((float)((float)m_nPrevWidth)/0.55, nLineCount, m_coord, &m_nBlockSize);//;xMakeGoban(m_nPrevWidth, 19);

   m_bmpBlack = xMakeStone(True, m_nBlockSize);
   m_bmpWhite = xMakeStone(False, m_nBlockSize);

   RECT wrect;
   GetWindowRect(m_hWnd, &wrect);

   int ncHeight = wrect.bottom-rect.bottom-wrect.top;


   ReSize(width, m_nPrevWidth*m_nShowCnt+ncHeight+m_nPrevWidth/4 + 2);
   ShowWindow(m_hWnd, SW_NORMAL);
   ::InvalidateRect(m_hWnd, 0, FALSE);
   UpdateWindow(m_hWnd);

	return 0;
}

#include "sgfmanager.h"

// ---------------------------------
static Bool xHitFirstCondition(cyiTreeNode* node, sSearchFilter* filter)
{
	cSgfStoneInfo* info=(cSgfStoneInfo*)node->GetData();
	unsigned int condition = filter->first_condition;
	int nChildCount=node->GetChildList().count();

	if (info==0)
	{
		condition = eFC_JOSEKI;
		return False;
	}


	// 全て
	if ((filter->first_condition==0 || (filter->first_condition&eFC_OTHER)))
	{
		if (info->exinfo==0)
			return True;

		if (filter->first_condition==0)
			condition = 0xFFFFFFFF;
	}

	if (info->exinfo!=0)
	{
		if ((condition & eFC_JOSEKI)&& (info->exinfo->option&eGRO_JOSEKI))
		{
			return True;
		}
		else if ((condition & eFC_HAMETE) && (info->exinfo->option&eGRO_HAMETE))
		{
			return True;
		}
		else if ((condition & eFC_JOSEKI_AFTER) && (info->exinfo->option&eGRO_JOSEKI_AFTER))
		{
			return True;
		}
		else if ((condition & eFC_JOSEKI_TENUKI) && (info->exinfo->option&eGRO_JOSEKI_TENUKI))
		{
			return True;
		}
		else if ((condition & eFC_CASE_BY) && (info->exinfo->option&eGRO_JOSEKI_CASE))
		{
			return True;
		}
		else if ((condition & eFC_OTHER) && ((info->exinfo->option&eGRO_JOSEKI_OTHER) || (nChildCount==0 && (info->exinfo->option&eGRO_JOSEKI_KATEGORI)==0)))
		{
			return True;
		}
	}


	return False;
}



// ---------------------------------
static Bool xHitSecondCondition(cyiTreeNode* node, sSearchFilter* filter)
{
	Bool bIsAll=True;
	cSgfStoneInfo* info=(cSgfStoneInfo*)node->GetData();
	int nChildCount=node->GetChildList().count();

	if (info==0)
		return False;

	for (int i=0; i<32; i++)
	{
		if (filter->second_condition[i]!=0)
		{
			bIsAll=False;
			break;
		}
	}

	if (bIsAll==True)
		return True;

	// 評価
	if (filter->second_condition[0])
	{
		if (info->exinfo)
		{
			int code = g_status_code[filter->second_condition[0]-1];
			if (info->exinfo->status!=code)
				return False;
		}
		else
		{
			if (filter->second_condition[0]!=1)
				return False;
		}
	}

	// 編集済みであるかないか
	if (filter->second_condition[1])
	{	
		if (filter->second_condition[1]==1) // 編集済み
		{
			if (info->exinfo==0)
				return False;
			if ((info->exinfo->option&eGRO_EDIT_COMPRETE)==0)
				return False;
		}
		else // 編集済みではない
		{
			if (info->exinfo)
			{
				if ((info->exinfo->option&eGRO_EDIT_COMPRETE)!=0)
					return False;
			}
		}
	}

	// 再生済みであるかないか
	if (filter->second_condition[2])
	{	
		if (filter->second_condition[2]==1) // 再生済み
		{
			//if (info->exinfo==0)
			//	return False;
			if ((info->flag32&0x80000000)==0)
				return False;
		}
		else // 編集済みではない
		{
			if (info->exinfo)
			{
				if ((info->flag32&0x80000000)!=0)
					return False;
			}
		}
	}

	// シチョウ関係であるかないか
	if (filter->second_condition[3])
	{	
		if (filter->second_condition[3]==1) // 再生済み
		{
			if (info->exinfo==0)
				return False;
			if ((info->exinfo->option&eGRO_SICHOU)==0)
				return False;
		}
		else // 編集済みではない
		{
			if (info->exinfo)
			{
				if ((info->exinfo->option&eGRO_SICHOU)!=0)
					return False;
			}
		}
	}

	// 置き碁おすすめであるかないか
	if (filter->second_condition[4])
	{	
		if (filter->second_condition[4]==1) // 再生済み
		{
			if (info->exinfo==0)
				return False;
			if ((info->exinfo->option&eGRO_OKIGO)==0)
				return False;
		}
		else // 編集済みではない
		{
			if (info->exinfo)
			{
				if ((info->exinfo->option&eGRO_OKIGO)!=0)
					return False;
			}
		}
	}

	// 同一型の分岐であるかどうか
	if (filter->second_condition[5])
	{	
		if (filter->second_condition[5]==1) // 再生済み
		{
			if (info->refNo!=0 || info->ref!=0)
			{
			}
			else
				return False;
		}
		else // 編集済みではない
		{
			if (info->refNo!=0 ||
					info->ref!=0)
				return False;
		}
	}

	// レベル
	int n;
	if ((n=(filter->second_condition[6]>>4))!=0)
	{	
		if (info->exinfo)
		{
			int x = filter->second_condition[6]&0x0F;
			int y = (info->exinfo->flag32>>24)&0x0F;

			if (x==0) // ひとしい
			{
				if(n!=y)
					return False;
			}
			else if (x==1) // 以上
			{
				if(n>=y)
					return False;
				if (y==0)
					return False;
			}
			else if (x==2) // 以下
			{
				if(n<=y)
					return False;
			}
		}
		else
		{
			if (n!=0)
				return False;
		}

	}

	// 頻度
	if ((n=(filter->second_condition[7]>>4))!=0)
	{	
		if (info->exinfo)
		{
			int x = filter->second_condition[7]&0x0F;
			int y = ((info->exinfo->flag32>>28)&0x0F);

			if (x==0) // ひとしい
			{
				if(n!=y)
					return False;
			}
			else if (x==1) // 以上
			{
				if(n>=y)
					return False;

				if (y==0)
					return False;
			}
			else if (x==2) // 以下
			{
				if(n<=y)
					return False;
			}
		}
		else
		{
			if (n!=0)
				return False;
		}

	}

	if ((n=filter->second_condition[10])!=0)
	{
		if (n==1)
		{
			if ((info->flag32&0x40000000)==0)
				return False;
		}
		else 
		{
			if ((info->flag32&0x40000000)!=0)
				return False;
		}
	}

	// 定石名
	if (filter->second_condition[8])
	{
		if (info->exinfo==0 &&
			nChildCount!=0)
			return False;
		
		int h=(filter->second_condition[8])>>4;
		int l=(filter->second_condition[8])&0x0F;
		int d=(filter->second_condition[9]);

		if (h)
		{
			int _h=0, _l=0, _d=0;

			xGetJosekiName(node, _h, _l, _d);

			if (_h!=h-1)
				return False;
			if (l)
			{
				if (_l!=l-1)
					return False;

				if (d)
				{
					if (_d!=d-1)
						return False;
				}
			}
		}
	}

	return True;
}

// ---------------------------------
Bool cyctrlPreviewGoban::xSearchMap(cyiTreeNode* top, sSearchFilter* filter)
{
	// 定石のみ拾う
	if (filter!=0)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)top->GetData();
		
		if (info && info->map)
		{
			if (xHitFirstCondition(top, filter)==True)
			{
				if (xHitSecondCondition(top, filter)==True)
				{
					previewList.add((yiAnyValue)top);
				}
			}
		}
	}

	if (top->GetChildList().count()==0)
	{
		return True;
	}

	for (cyiNode* node = top->GetChildList().GetNodeTopFrom(0); node; node=node->np)
	{
		cyiTreeNode* tree = (cyiTreeNode*)node->data;
		xSearchMap(tree, filter);
	}

	return True;
}

// ---------------------------------
Bitmap* cyctrlPreviewGoban::CreateGobanBitmap(char** map)
{
	int w = m_bmpGoban->GetWidth();
	int h = m_bmpGoban->GetHeight();
	int half = m_nBlockSize>>1;

	Bitmap* bmp = new Bitmap(w, h,PixelFormat32bppARGB);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.DrawImage(m_bmpGoban, 0, 0);

	for (int i=0; i<m_nLineCount; i++)
	{
		for (int j=0; j<m_nLineCount; j++)
		{
			stone_t stone = (stone_t)map[i][j];

			if (stone==STONE_NONE)
				continue;

			
			//g.DrawImage(stone==STONE_BLACK?m_bmpBlack:m_bmpWhite, (float)m_coord[i]-(float)half, m_coord[j]-(float)half, (float)m_nBlockSize, (float)m_nBlockSize);
			g.FillEllipse(&SolidBrush(stone==STONE_BLACK?Color::Black:Color::White),
							(INT)((float)m_coord[i]-(float)half), (INT)(m_coord[j]-(float)half), m_nBlockSize, m_nBlockSize);
		}
	}

	float fw = (float)w*0.45;
	Bitmap* ret = new Bitmap((int)((float)w*0.55), (int)((float)w*0.55), PixelFormat32bppARGB);
	Graphics gret(ret);
	


	gret.DrawImage(bmp, (float)0.0, (float)0.0, fw, (float)0.0, (float)((float)w-fw), (float)((float)w-fw), Unit::UnitPixel);
	gret.DrawRectangle(&Pen(Color::Black), 0, 0, (int)(w-fw-1), (int)(w-fw-1));
	delete bmp;
	return ret;
}

// ---------------------------------
Bool cyctrlPreviewGoban::Update(cyiTreeNode* top)
{
	int nListCount;
	Bool bIsNoUpdateScroll=True;



	//int nFilterIndex=m_cmbFilter.GetSelectIndex();

	if (top)
	{
		if (top==(cyiTreeNode*)0xFFFFFFFF)
		{
			top=m_currentNode;
		}

		previewList.clear();
		xSearchMap(top, &m_sSearchFilter);

		m_currentNode=top;

		m_nStartIndex=0;

	}
	else
	{
		top=m_currentNode;
		bIsNoUpdateScroll=False;
	}

	if ((nListCount=previewList.count())==0)
	{
		//return False;
	}


	int nUpdateCount = nListCount > m_nShowCnt ? m_nShowCnt:nListCount;
	int cnt=0;

	for (int i=0; i<m_nShowCnt; i++)
	{
		if (m_gobanz[i]!=0)
			delete m_gobanz[i];
		m_gobanz[i]=0;
	}

	// ---------------------------------------
	for (cyiNode* node=previewList.GetNodeTopFrom(m_nStartIndex); node && cnt<nUpdateCount; node=node->np)
	{
		cyiTreeNode* xnode = (cyiTreeNode*)node->data;

		cSgfStoneInfo* info = (cSgfStoneInfo*)xnode->GetData();
		m_gobanz[cnt++]=CreateGobanBitmap(info->map);
	}


	m_nSelectedIndex=-1;
	m_nHitCount=nListCount;

	if (m_nHitCount!=1 && 
			m_selectedNode!=0)
	{
		m_btnPlay.Hide();
		m_btnPrev.Hide();
		m_btnPlayTop.Hide();
		m_btnPlayEnd.Hide();
		m_selectedNode=0;
		m_bModePlay=False;
		::InvalidateRect(m_hWnd, 0, False);
	}

	if (bIsNoUpdateScroll==True &&
				m_nHitCount>m_nShowCnt)
	{
		::SetScrollRange(m_hWnd, SB_VERT,0, m_nHitCount-m_nShowCnt+1, TRUE);
		::SetScrollPos(m_hWnd, SB_VERT, 0, TRUE);
	}

	::InvalidateRect(m_hWnd, 0, TRUE);
	return 0;
}

// ---------------------------------------
BOOL cyctrlPreviewGoban::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	Graphics gdc(hdc);
	Bitmap* bmp = new Bitmap(ps->rcPaint.right, ps->rcPaint.bottom, PixelFormat32bppARGB);
	Graphics g(bmp);
	//if (m_gobanz[0]==0)
	//	return True;

	//if (m_nHitCount==0)
	//{
	//	return True;
	//}


	g.FillRectangle(&SolidBrush(Color::Gray), 0, 0, ps->rcPaint.right, ps->rcPaint.bottom);

	if (m_nHitCount==0)
	{
		gdc.DrawImage(bmp, 0, 0);
		delete bmp;
		return True;
	}

	for (int i=0; i<m_nShowCnt; i++)
	{
		if (m_gobanz[i]==0)
		{
			//if (m_nHitCount!=1)
			//	g.FillRectangle(&SolidBrush(Color::LightGray), 1, i*m_nPrevWidth+1, m_nPrevWidth-2, m_nPrevWidth-2);
			//else
			//	g.FillRectangle(&SolidBrush(Color::White), 0, i*m_nPrevWidth, m_nPrevWidth, m_nPrevWidth);
			//g.DrawRectangle(&Pen(Color::Black), 0, i*m_nPrevWidth, m_nPrevWidth-1, m_nPrevWidth-1);

			continue;
		}

		int h = m_gobanz[i]->GetHeight();
		int y = i*h;
		g.DrawImage(m_gobanz[i], 0, y);

		if (m_nSelectedIndex==i)
		{
			g.FillRectangle(&SolidBrush(Color(64, 255, 255, 255))/*&LinearGradientBrush(Point(0, y), Point(0, h), Color(64, 255, 255, 255), Color(32, 255, 255, 255))*/, 0, y, h-1, h-1);
		}
		//g.DrawRectangle(&Pen(Color::Black, 1.0), 0,  i*m_gobanz[i]->GetHeight(),  i*m_gobanz[i]->GetWidth()-1,  i*m_gobanz[i]->GetHeight()-1);
	}

	gdc.DrawImage(bmp, 0, 0);
	delete bmp;

	return True;
}

// ------------------------------------
LRESULT cyctrlPreviewGoban::OnClicked(WPARAM wp, LPARAM lp)
{
	int xPos   =  lp & 0xFFFF;
	int yPos   = (lp >> 16) & 0xFFFF;
	RECT rect;

	GetClientRect(m_hWnd, &rect);

	int nBlockSize = rect.bottom/m_nShowCnt;
	int n = yPos/nBlockSize;

	if (xPos>m_nPrevWidth)
		return 0;

	if (m_bModePlay==True)
	{
		if (m_prevNode)
		{
			m_nStartIndex=m_nPrevStartIndex;
			Update((cyiTreeNode*)m_prevNode);
		}
		return 0;
	}

	cyiNode* listNode = previewList.GetNodeTopFrom(m_nStartIndex+n);

	if (listNode==0)
	{
		Update(0);
		return 0;
	}

	cyiTreeNode* node = (cyiTreeNode*)listNode->data;
	m_selectedNode = node;

	m_listHistory.clear();

	cyiTreeNode* t = m_selectedNode;
	while(t)
	{
		m_listHistory.insert(0, (yiAnyValue)t);
		t=t->GetParent();
	}

	m_nCurrentPlayIndex=m_listHistory.count()-1;

	::PostMessage(m_hNotifyWnd, WM_PREVIEW_SELECTED, 0, (LPARAM)node);

	m_nPrevStartIndex=m_nStartIndex;
	m_nStartIndex=0;
	SetScrollRange(m_hWnd, SB_VERT,0, 1, TRUE);
	SetScrollPos(m_hWnd, SB_VERT, 0, TRUE);

	m_prevNode=m_currentNode;
	m_bModePlay=True;
	Update(m_selectedNode);


	//Update(0);

	//m_nStartIndex=0;

	m_btnPlay.Show();
	m_btnPrev.Show();
	m_btnPlayTop.Show();
	m_btnPlayEnd.Show();

	::InvalidateRect(m_hWnd, 0, False);
	return 0;
}

// ------------------------------------
LRESULT cyctrlPreviewGoban::OnMouseMove(WPARAM wp, LPARAM lp, Bool& handled)
{
	int xPos   =  lp & 0xFFFF;
	int yPos   = (lp >> 16) & 0xFFFF;
	RECT rect;

	GetClientRect(m_hWnd, &rect);

	int nBlockSize = rect.bottom/m_nShowCnt;
	int n = yPos/nBlockSize;

	handled=True;

	if (m_nSelectedIndex==n)
		return 0;

	m_nSelectedIndex=n;

	::InvalidateRect(m_hWnd, 0, FALSE);

	return 0;
}

// -------------------------------
LRESULT cyctrlPreviewGoban::OnPreviewDetailClosed(WPARAM wp, LPARAM lp, Bool& handled)
{
	Update((cyiTreeNode*)0xFFFFFFFF);
	return 0;
}

// ------------------------------------
LRESULT cyctrlPreviewGoban::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	m_nSelectedIndex=-1;
	::InvalidateRect(m_hWnd, 0, FALSE);
	return 0;
}

// -------------------------------
LRESULT cyctrlPreviewGoban::OnPlay(HWND hWnd, int code)
{
	cyiNode* node = m_listHistory.GetNodeTopFrom(m_nCurrentPlayIndex+1);

	if (node==0)
	{
		return 0;
	}
	m_nCurrentPlayIndex++;
	
	cyiTreeNode* t = (cyiTreeNode*)node->data;

	::PostMessage(m_hNotifyWnd, WM_PREVIEW_PLAY, 0, (LPARAM)t);
	return 0;
}

// -------------------------------
LRESULT cyctrlPreviewGoban::OnBack(HWND hWnd, int code)
{
	cyiNode* node = m_listHistory.GetNodeTopFrom(m_nCurrentPlayIndex-1);

	if (node==0)
	{
		return 0;
	}
	m_nCurrentPlayIndex--;
	
	cyiTreeNode* t = (cyiTreeNode*)node->data;

	::PostMessage(m_hNotifyWnd, WM_PREVIEW_PLAY, 0, (LPARAM)t);
	return 0;
}

// -------------------------------
LRESULT cyctrlPreviewGoban::OnPlayTop(HWND hWnd, int code)
{
	cyiNode* node = m_listHistory.GetNodeTopFrom(0);

	if (node==0)
	{
		return 0;
	}
	m_nCurrentPlayIndex=0;
	
	cyiTreeNode* t = (cyiTreeNode*)node->data;

	::PostMessage(m_hNotifyWnd, WM_PREVIEW_PLAY, 0, (LPARAM)t);
	return 0;
}

// -------------------------------
LRESULT cyctrlPreviewGoban::OnPlayEnd(HWND hWnd, int code)
{
	cyiNode* node = m_listHistory.GetNodeTailFrom(0);

	if (node==0)
	{
		return 0;
	}
	m_nCurrentPlayIndex=m_listHistory.count()-1;
	
	cyiTreeNode* t = (cyiTreeNode*)node->data;

	::PostMessage(m_hNotifyWnd, WM_PREVIEW_PLAY, 0, (LPARAM)t);
	return 0;
}

// -------------------------------
LRESULT cyctrlPreviewGoban::OnFilterChanged(HWND hWnd, int code)
{

	if (code!=CBN_SELCHANGE)
		return 0;

	int nFilter = m_cmbFilter.GetSelectIndex();

	if (nFilter!=11)
			memset(&m_sSearchFilter, 0, sizeof(m_sSearchFilter));

	if (nFilter)
	{
		if (nFilter==1)
			m_sSearchFilter.first_condition=eFC_JOSEKI;
		else if (nFilter==2)
			m_sSearchFilter.first_condition=eFC_HAMETE;
		else if (nFilter==3)
			m_sSearchFilter.first_condition=eFC_OTHER;
		else if (nFilter==4)
			m_sSearchFilter.first_condition=eFC_JOSEKI_AFTER;
		else if (nFilter==5)
			m_sSearchFilter.first_condition=eFC_JOSEKI_TENUKI;
		else if (nFilter==6)
			m_sSearchFilter.first_condition=eFC_CASE_BY;
		else if (nFilter==7)
			m_sSearchFilter.second_condition[1]=2;
		else if (nFilter==8)
			m_sSearchFilter.second_condition[2]=2;
		else if (nFilter==9)
			m_sSearchFilter.second_condition[5]=1;
		else if (nFilter==10)
		{
			m_sSearchFilter.second_condition[10]=1;
		}
		else if (nFilter==11)
		{
			m_dlgDetail.CreateFromReosurce(m_hWnd, 0, IDD_PREVIEW_FOR_POPUP, &m_sSearchFilter);
			return 0;
		}
	}

	Update((cyiTreeNode*)0xFFFFFFFF);
	return 0;
}

// -------------------------------
LRESULT cyctrlPreviewGoban::OnDetail(HWND hWnd, int code)
{

	m_dlgDetail.CreateFromReosurce(m_hWnd, 0, IDD_PREVIEW_FOR_POPUP, &m_sSearchFilter);
	return 0;
}


// ---------------------------------
void cyctrlPreviewGoban::Clear()
{

}

// ---------------------------------
LRESULT cyctrlPreviewGoban::OnVScroll(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	int nScrollCode = (int) LOWORD(wParam); // スクロールコード
	int nPos = (short int) HIWORD(wParam);  // スクロールボックス（つまみ）の位置 
	HWND hwndScrollBar = (HWND) lParam;      // スクロールバーのハンドル 

    switch (LOWORD(wParam)) {
        case SB_LINEUP:
            m_nStartIndex--;
            break;
        case SB_LINEDOWN:
            m_nStartIndex++;
            break;
        case SB_THUMBPOSITION:
            m_nStartIndex = HIWORD(wParam);
            break;
        case SB_PAGEDOWN:
            m_nStartIndex+=m_nShowCnt;
            break;
        case SB_PAGEUP:
            m_nStartIndex-=m_nShowCnt;
            break;
        default:
           return 0;
    }


	if (m_nStartIndex>(m_nHitCount-m_nShowCnt))
		m_nStartIndex=m_nHitCount-m_nShowCnt;

	if (m_nStartIndex<0)
		m_nStartIndex=0;

	Update(0);
	SetScrollPos(m_hWnd, SB_VERT, m_nStartIndex, TRUE);


	return 0;
}


// ---------------------------------
Bitmap* xMakeGoban(int width, int nLineCount)
{
	Bitmap* bmp = new Bitmap(width, width, PixelFormat32bppARGB);
	float fLineWidth = (float)width/(float)(nLineCount+1);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	g.FillRectangle(&SolidBrush(Color(255, 211, 178, 104)), 0, 0, width, width);

	for (int i=0; i<nLineCount+1; i++)
	{
		g.DrawLine(&Pen(Color::Black, 0.5), (float)i*fLineWidth, fLineWidth, (float)i*fLineWidth, (float)width-fLineWidth);
		g.DrawLine(&Pen(Color::Black, 0.5), (float)fLineWidth, (float)i*fLineWidth, (float)width-fLineWidth, (float)i*fLineWidth);
	}

	//g.DrawRectangle(&Pen(Color::Black, 1.0), fLineWidth, fLineWidth, 

	return bmp;
}

static Bitmap* xMakeStone(Bool bIsBlack, int w)
{
	Bitmap* bmp = new Bitmap(w, w, PixelFormat32bppARGB);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	g.FillEllipse(&SolidBrush(bIsBlack?Color::Black:Color::White), 1, 1, w-1, w-1);

	return bmp;
}


// ----------------------------------
// 
int xGetJosekiName(cyiTreeNode* node, int& _h, int& _l, int& _d)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();
	cyiTreeNode* t = node;
	int joseki_code=0;
	int h=0, l=0, d=0;
	Bool bIsHasAName=False;
	_h=0;
	_l=0;
	_d=0;

	if (info==0)
		return -1;

	while(t)
	{
		info = (cSgfStoneInfo*)t->GetData();

		if (info==0)
		{
			return -1;
		}

		if (info->exinfo!=0)
		{
			joseki_code = GET_JOSEKI_NAME(info->exinfo->flag32);


			if (joseki_code!=0)
			{
				h = GET_JOSEKI_HIGH(joseki_code);
				l = GET_JOSEKI_LOW(joseki_code);
				d = GET_JOSEKI_DETAIL(joseki_code);

				if (node==t)
				{
					bIsHasAName=True;
				}

				if (d!=0)
					_d=d;
				if (l!=0)
					_l=l;
				if (h!=0)
					_h=h;

				if (h!=0)
					break;
			}
		}

		t=t->GetParent();
	}

	if (_h==0)
		return -1;

	return 0;
}