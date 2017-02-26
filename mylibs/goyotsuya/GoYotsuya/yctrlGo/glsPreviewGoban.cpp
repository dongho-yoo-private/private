#include "glsPreviewGoban.h"
#include "yctrlGobanResource.h"
#include "resource.h"

#define MAX_PREVIEW_COUNT 500

// ---------------------------------
static Bitmap* xMakeGoban(int width, int nLineCount);
static Bitmap* xMakeStone(Bool bIsBlack, int w);


cglsPreviewGoban::cglsPreviewGoban(void)
: m_bIsThreadStop(False),
	m_nBufferringCount(0),
	m_hThreadEvent(0),
	m_bIsSlowLoad(0),
	m_bIsSearchStop(0),
	m_bIsDataInvalid(False),
	m_hUpdteThreadMutex(0),
	m_currentNode(0),
	m_bmpBgGoban(0),
	m_bIsJosekiMode(False),
	m_panel(0),
	m_WaitThreadCnt(0),
	previewList(0)
{
	__GLASSINTERFACE_NAME__;
	listDeleteList=0;

}

cglsPreviewGoban::~cglsPreviewGoban(void)
{
}

// ---------------------------------
Bool cglsPreviewGoban::xCreate(HWND hWnd, int id, int x, int y, int width, int nCount, int nLineCount, int nWindowStyle, int nWindowStyleEx)
{
	Bool res = cGlassInterface::Create(hWnd, id, x, y, width, width*nCount, nWindowStyleEx,nWindowStyle /*WS_CLIPCHILDREN|*/);

	if (res==False)
	{
		return False;
	}

	m_hPaintMutex=yiMutexCreateEx(10);
	m_hMutexThread2 = yiMutexCreateEx(3);

	RECT rect;
	::GetClientRect(m_hWnd, &rect);

	m_prevNode=0;
	m_bModePlay=False;
	m_nShowCnt=nCount;
	m_nBufferringCount=nCount*10;
	m_nStartIndex=0;
	m_nPrevWidth=(rect.right);
	m_bIsCreated=True;
	memset(m_gobanz, 0, sizeof(m_gobanz));
	m_nHitCount=0;

	memset(&m_sSearchFilter, 0, sizeof(m_sSearchFilter));

	if ((nWindowStyle&WS_CHILD)==0)
	{
		//m_cmbFilter.Create(m_hWnd, 5, 2, (int)(m_nPrevWidth*m_nShowCnt), m_nPrevWidth, m_nPrevWidth/4);

		sGlsSplitButtonDetail detail;

		cglsSplitButton::xDefaultDetail(&detail);

		detail.rect.left=0;		
		detail.rect.top=0;		
		detail.rect.right=m_nPrevWidth;		
		detail.rect.bottom=m_nPrevWidth/4+1;
		detail.colorHilight=Color::LightBlue;
		detail.gls.font=new Font(L"Merio", 10.5);

		m_cmbFilter.Create((HWND)this, 5, 0, (int)(m_nPrevWidth*m_nShowCnt), m_nPrevWidth, (m_nPrevWidth>>2)/*GetFontHeight(True)m_nPrevWidth/4*/, 0, &detail);

		//Font* font = cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL);
		
		//HDC hdc = GetDC();
		//Graphics g(hdc);
		//LOGFONTA logfnt;

		//font->GetLogFontA(&g, &logfnt);
		//ReleaseDC(hdc);

		//HFONT hFont = CreateFontIndirect(&logfnt);

		//SendMessage(m_cmbFilter.m_hWnd, WM_SETFONT, (WPARAM)hFont ,True);


		//m_btnDetail.Show();

		m_cmbFilter.AddString(L"全て");
		m_cmbFilter.AddString(L"定石");
		m_cmbFilter.AddString(L"はめ手");
		m_cmbFilter.AddString(L"その他の変化");
		m_cmbFilter.AddString(L"定石後の変化");
		m_cmbFilter.AddString(L"手抜き定石");
		m_cmbFilter.AddString(L"場合の手");
		m_cmbFilter.AddString(L"編集済みでない");
		m_cmbFilter.AddString(L"再生済みでない");
		m_cmbFilter.AddString(L"同一型");
		m_cmbFilter.AddString(L"お気に入り");
		m_cmbFilter.AddString(L"条件検索");

		m_cmbFilter.Show();
		m_cmbFilter.Update(eGUM_FORCED);

		Bitmap* bg=BITMAP32(m_cmbFilter.m_w, m_cmbFilter.m_h);
		cyiBitmapBlender::FillColor(bg, Color::White);
		m_cmbFilter.SetBgBitmap(bg);

		m_nFilter=0;
		//m_cmbFilter.SelectIndex(0);
	}

   m_nLineCount=nLineCount;

   RECT wrect;
   GetWindowRect(m_hWnd, &wrect);

   int ncHeight = wrect.bottom-rect.bottom-wrect.top;

	if ((nWindowStyle&WS_CHILD)==0)
	{
		m_bmpGoban = cyctrlGobanResource::GetInstance().GetGobanBitmap((float)((float)m_nPrevWidth)/*/0.55*/, nLineCount, m_coord, &m_nBlockSize);//;xMakeGoban(m_nPrevWidth, 19);
		m_bmpBgGoban = cyiBitmapBlender::GrayScale(m_bmpGoban, 0.7);
		m_bmpBlack = xMakeStone(True, m_nBlockSize);
		m_bmpWhite = xMakeStone(False, m_nBlockSize);
		ReSize(width, m_nPrevWidth*m_nShowCnt+ncHeight+m_nPrevWidth/4 + 2);

		RECT rectPanel = {0, 0, m_nPrevWidth, m_nPrevWidth*m_nShowCnt};
		m_panel = new cglsStackPannel();

		sGlsStackPannelDetail stackDetail;
		cglsStackPannel::xDefaultDetail(&stackDetail);
		stackDetail.bIsFixedMode=True;
		stackDetail.bIsShowScrollBar=True;
		stackDetail.bIsVertical=True;
		m_panel->Create((HWND)this, 11, &rectPanel, 0, this, &stackDetail);
		m_panel->Show();

	}
	else
	{
		RECT rect;
		::GetClientRect(m_hParentWnd, &rect);
		m_panel=0;
		AutoReSize(rect.bottom);
	}


	OnBgDraw();
	ShowWindow(m_hWnd, SW_NORMAL);
	InvalidateRect(0);
	//::InvalidateRect(m_hWnd, 0, FALSE);
	//UpdateWindow(m_hWnd);

	return True;
}

// ---------------------------------
void cglsPreviewGoban::xDefaultDetail(sGlsPreviewGoban* detail)
{
	detail->nCount=5;
	detail->nMinimumCount=3;
	detail->nWidth=128;
	detail->nMinimumWidth=96;
}


// ---------------------------------
Bool cglsPreviewGoban::Create(HWND hWnd, int width, int nCount, int nLineCount, ePreviewMode mode)
{
	m_eMode=mode;
	if ( xCreate(hWnd, 0, CW_USEDEFAULT, CW_USEDEFAULT, width, nCount, nLineCount, WS_OVERLAPPED|WS_SYSMENU,WS_EX_TOOLWINDOW)==True)
	{
		return True;
	}

	return False;
}

// ---------------------------------
Bool cglsPreviewGoban::CreateAsChild(HWND hWnd, int id, int x, int y, int nLineCount, ePreviewMode mode, sGlsPreviewGoban* detail)
{
	if (detail)
	{
		m_detail=*detail;
	}
	else
	{
		xDefaultDetail(&m_detail);
	}

	m_eMode=mode;
	return xCreate(hWnd, id, x, y, m_detail.nWidth, m_detail.nCount, nLineCount, WS_CHILD|WS_VISIBLE, 0);
}

// ---------------------------------
int cglsPreviewGoban::AutoReSize(int nHeight)
{
	m_nPrevWidth=m_detail.nWidth;
	m_nShowCnt=m_detail.nCount;

	int n = nHeight/m_detail.nCount;
	if (m_detail.nMinimumWidth>n)
	{
		m_nPrevWidth=m_detail.nMinimumWidth;
	}

	m_nShowCnt=nHeight/m_nPrevWidth;
	m_nShowCnt+=(nHeight%m_nPrevWidth)?1:0;

	m_bmpGoban = cyctrlGobanResource::GetInstance().GetGobanBitmap((float)((float)m_nPrevWidth)/*/0.55*/, m_nLineCount, m_coord, &m_nBlockSize);//;xMakeGoban(m_nPrevWidth, 19);
	m_bmpBgGoban = cyiBitmapBlender::GrayScale(m_bmpGoban, 1.0);
	m_bmpBlack = xMakeStone(True, m_nBlockSize);
	m_bmpWhite = xMakeStone(False, m_nBlockSize);

	ReSize(m_nPrevWidth, nHeight);

	if (m_panel==0)
	{
		RECT rectPanel = {0, 0, m_nPrevWidth, nHeight};
		m_panel = new cglsStackPannel();

		sGlsStackPannelDetail stackDetail;
		cglsStackPannel::xDefaultDetail(&stackDetail);
		stackDetail.bIsFixedMode=True;
		stackDetail.bIsShowScrollBar=True;
		stackDetail.bIsVertical=True;
		m_panel->Create((HWND)this, 11, &rectPanel, 0, this, &stackDetail);
		m_panel->Show();
	}
	else
	{
		m_panel->ReSize(m_nPrevWidth, nHeight);
		InvalidateRect(0);
	}

	return m_nPrevWidth;
}



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



#define PREV_WINDOW_CLASS "PreviewGobanClass"

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
			if ((info->flag32&(1<<19)/*0x80000000*/)==0 ||
					node->GetChildList().count()!=0)
				return False;
		}
		else // 再生済みではない
		{
			if (/*node->GetChildList().count()==0 &&*/
				(info->flag32&(1<<19)/*&0x80000000*/)!=0)
			{
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
Bool cglsPreviewGoban::xSearchMap(cyiTreeNode* top, sSearchFilter* filter)
{
	if (m_bIsSearchStop==True)
	{
		return False;
	}

	const int xCount=30;

	// 定石のみ拾う
	if (m_eMode==ePM_JOSEKI_JITEN &&
			filter!=0)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)top->GetData();
		
		if (info && info->map)
		{
			if (xHitFirstCondition(top, filter)==True)
			{
				if (xHitSecondCondition(top, filter)==True)
				{
					previewList->lock();
					previewList->add((yiAnyValue)top);

					int cnt = previewList->count();
					if (cnt && cnt%xCount==0)
					{
						Bool bIsHandled;
						OnRequestAddBitmap(cnt, (LPARAM)top, bIsHandled);
					}
					previewList->unlock();
				}
			}
		}
	}
	else if (m_eMode==ePM_TAB)
	{
		cyiList* list=(cyiList*)top;

		for (cyiNode* node=list->GetNodeTopFrom(); node; node->np)
		{
			cyiTreeNode* t=(cyiTreeNode* )node->ref;

			previewList->lock();
			previewList->add((yiAnyValue)t);
			int cnt = previewList->count();
			if (cnt && cnt%xCount==0)
			{
				PostMessage(m_hWnd, WM_REQ_ADD_BITMAP, cnt, (LPARAM)t);
			}
			previewList->unlock();
		}
		
		return True;
	}
	else
	{
		cSgfStoneInfo* info=(cSgfStoneInfo*)top->GetData();

		if (info)
		{
			if (top->GetChildList().count()==0)
			{
				previewList->lock();
				previewList->add((yiAnyValue)top);

				int cnt = previewList->count();
				if (cnt && cnt%xCount==0)
				{
					Bool bIsHandled;
					OnRequestAddBitmap(cnt, (LPARAM)top, bIsHandled);
					//PostMessage(m_hWnd, WM_REQ_ADD_BITMAP, cnt, (LPARAM)top);
				}

				previewList->unlock();


				return True;
			}
			else if (top->GetChildList().count()!=1)
			{
				previewList->lock();
				previewList->add((yiAnyValue)top);

				int cnt = previewList->count();
				if (cnt && cnt%xCount==0)
				{
					Bool bIsHandled;
					OnRequestAddBitmap(cnt, (LPARAM)top, bIsHandled);
					//PostMessage(m_hWnd, WM_REQ_ADD_BITMAP, cnt, (LPARAM)top);
				}

				previewList->unlock();

			}
			else if (top->GetData()!=0)
			{
				if (info->reference)
				{
					previewList->lock();
					previewList->add((yiAnyValue)top);
					int cnt = previewList->count();
					if (cnt && cnt%xCount==0)
					{
						Bool bIsHandled;
						OnRequestAddBitmap(cnt, (LPARAM)top, bIsHandled);
						//PostMessage(m_hWnd, WM_REQ_ADD_BITMAP, cnt, (LPARAM)top);
					}

					previewList->unlock();

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
		if (xSearchMap(tree, filter)==False)
		{
			return False;
		}
	}

	return True;
}

// ---------------------------------
Bitmap* cglsPreviewGoban::CreateGobanBitmap(char** map)
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

			if (stone==eST_NONE)
				continue;

			
			//g.DrawImage(stone==eST_BLACK?m_bmpBlack:m_bmpWhite, (float)m_coord[i]-(float)half, m_coord[j]-(float)half, (float)m_nBlockSize, (float)m_nBlockSize);
			g.FillEllipse(&SolidBrush(stone==eST_BLACK?Color::Black:Color::White),
							(float)((float)m_coord[i]-(float)half)+2.0, (float)(m_coord[j]-(float)half)+2.0, (float)m_nBlockSize, (float)m_nBlockSize);
		}
	}

	if (m_bIsJosekiMode==True)
	{
		float fw = (float)w*0.45;
		Bitmap* ret = new Bitmap((int)((float)w*0.55), (int)((float)w*0.55), PixelFormat32bppARGB);
		Graphics gret(ret);

		gret.DrawImage(bmp, (float)0.0, (float)0.0, fw, (float)0.0, (float)((float)w-fw), (float)((float)w-fw), Unit::UnitPixel);
		gret.DrawRectangle(&Pen(Color::Black), 0, 0, (int)(w-fw-1), (int)(w-fw-1));
		return ret;
	}
	else
	{
		Bitmap* ret = new Bitmap(w, h, PixelFormat32bppARGB);
		Graphics gret(ret);

		gret.DrawImage(bmp, 0, 0);//(float)0.0, (float)0.0, fw, (float)0.0, (float)((float)w-fw), (float)((float)w-fw), Unit::UnitPixel);
		gret.DrawRectangle(&Pen(Color::Black), 0, 0, (int)(w-1), (int)(w-1));
		return ret;
	}
	delete bmp;
	return 0;
}

// --------------------------
LRESULT cglsPreviewGoban::OnDestroy(WPARAM wp, LPARAM lp)
{
	YITRACE(5, "cglsPreviewGoban::OnDestroy called");
	m_bIsSearchStop=True;
	m_bIsThreadStop=True;

	EndThreadz();

	return cyctrlbase::OnDestroy(wp, lp);
}

void xOnDeleteList(void* param)
{
	cyiList* list = (cyiList*)param;
	if (list)
	{
		YITRACE(5,"xOnDeleteList(%d) cleared", list->count());
		list->clear();
		delete list;
		
	}
	::_endthread();
}

// --------------------------
void cglsPreviewGoban::OnThreadStart(int id, int param1, int param2)
{
	YITRACE(5,"cglsPreviewGoban::OnThreadStart(%d) called", id);
	if (id==THREAD_ANY_ID)
	{
		int nRetryCnt=0;
		YITRACE(5, "cglsPreviewGoban::OnThreadStart(%d) Wait CriticalSection", id);
		yiMutexLockEx(m_hUpdteThreadMutex);
		YITRACE(5, "cglsPreviewGoban::OnThreadStart(%d) Enter CriticalSection", id);
		cyiTreeNode* top=(cyiTreeNode*)param1;
		unsigned long t1 = cyiTime::CurrentmSec();
		
		m_bIsSearchStop=True;
		m_bIsThreadStop=True;

		m_WaitThreadCnt++;
		m_lastNode=top;
		Bool res = WaitThread(2, INFINITE);

		::yiMutexLockEx(m_hMutexThread2);

		if (m_WaitThreadCnt>1)
		{
			YITRACE(5, "OnThreadStart(%x) WaitThread(2) waitcnt:%d", res, m_WaitThreadCnt);
			m_WaitThreadCnt--;
			::yiMutexUnlockEx(m_hMutexThread2);
			return;
		}

		m_WaitThreadCnt--;
		top = m_lastNode;
		

		YITRACE(5, "OnThreadStart(%x) WaitThread(2)==%d, param1:%x, node:%x", res, param1, m_lastNode);

		m_bIsDataInvalid=True;
		//res = WaitThread(1, INFINITE);
		//YITRACE(5, "OnThreadStart(%x) WaitThread(1)==%d", res);

		if (top==(cyiTreeNode*)0xFFFFFFFF)
		{
			top=m_currentNode;
		}

		m_bIsThreadStop=False;
		m_bIsSearchStop=False;


		//YITRACE(5, "OnThreadStart(%x) StartThread(2) %d", top, cyiTime::GetSecDiff(t1, cyiTime::CurrentmSec()));
RETRY:
		YITRACE(5, "UpdateTree(%x) StartThread(2) ....", top);

		if (nRetryCnt<10)
		{
			if (StartThread(2, (int)top)==False)
			{
				YITRACE(5, "UpdateTree(%x) StartThread(2) Failure... so retry....");
				m_bIsSearchStop=True;
				m_bIsThreadStop=True;

				WaitThread(2, 100);
				m_bIsThreadStop=False;
				m_bIsSearchStop=False;
				nRetryCnt++;
				goto RETRY;
			}
		}
		::yiMutexUnlockEx(m_hMutexThread2);
		yiMutexUnlockEx(m_hUpdteThreadMutex);
		YITRACE(5, "cglsPreviewGoban::OnThreadStart(%d) Leave CriticalSection", id);

	}
	else if (id==1)
	{
		int cnt=0;
		int queueCnt=10;
		Bool bIsFirst=True;
		cyiQueue* xqueue = new cyiQueue();
		YITRACE(5, "cglsPreviewGoban::OnThreadStart(1) (%d)", previewList->count());
		Sleep(100);

		//RECT xRect = {0, 0, m_nPrevWidth, m_nPrevWidth*param1};
		RECT xRect = {0, 0, 0, 0};

		m_panel->Clear();
		m_panel->SetBufferSize(xRect, True);
		
		//m_bmpCurrent

		for (cyiNode* node = previewList->GetNodeTopFrom(); node; node=node->np)
		{
			cyiTreeNode* xnode = (cyiTreeNode*)node->data;

			cSgfStoneInfo* info = (cSgfStoneInfo*)xnode->GetData();
			Bitmap* bmp=CreateGobanBitmap(info->map);			
			
			if (m_bIsThreadStop==True)
			{
				YITRACE(5, "cglsPreviewGoban::OnThreadStart(1) aborted (%d)", cnt);

				delete bmp;
				delete xqueue;
				return ;
			}
			xqueue->enq((yiAnyValue)bmp);
			++cnt;

			if (xqueue->count()==10)
			{
				PostMessage(m_hWnd, WM_REQ_ADD_BITMAP, (WPARAM)xqueue, bIsFirst);
				bIsFirst=False;
				xqueue = new cyiQueue();
			}

			//if (m_bIsSlowLoad)
			//{
				Sleep(10);
			//}
		}
		
		if (xqueue->count()!=0)
		{
			PostMessage(m_hWnd, WM_REQ_ADD_BITMAP, (WPARAM)xqueue, bIsFirst);
		}
		else
		{
			delete xqueue;
		}

		YITRACE(5, "cglsPreviewGoban::OnThreadStart(1) end(%d)", cnt);


	}
	else if (id==2)
	{
		if (previewList)
		{
			_beginthread(xOnDeleteList, 0, previewList);
			Bool bIsHandled;
			OnRequestAddBitmap(0, 0, bIsHandled);
			//PostMessage(m_hWnd, WM_REQ_ADD_BITMAP, 0, 0);
		}

		previewList = new cyiList(True);

		previewList->lock();
		previewList->clear();
		previewList->unlock();

		if (m_bIsSearchStop==True)
		{
			//previewList->unlock();
			return ;
		}
		unsigned int t1 = cyiTime::CurrentmSec();

		YITRACE(5, "xSearchMap filter, %d, %d", m_sSearchFilter.first_condition, m_sSearchFilter.second_condition);

		if (param1)
			m_currentNode=(cyiTreeNode*)param1;
		
		if (m_currentNode==0)
		{
			YITRACE(5, "OnThreadStart(2) currentNode is null!!");
			//PostMessage(m_hWnd, WM_REQ_ADD_BITMAP, 0, 0);
			Bool bIsHandled;
			OnRequestAddBitmap(0, 0, bIsHandled);
			return ;
		}

		if (xSearchMap((cyiTreeNode*)m_currentNode, &m_sSearchFilter)==False)
		{
			YITRACE(5, "OnThreadStart(2) xSearchMap is aborted");
			//previewList->
			//m_panel->Clear();
			//previewList->unlock();

			// クリア
			m_currentNode=0;
			Bool bIsHandled;
			OnRequestAddBitmap(0, 0, bIsHandled);
			//PostMessage(m_hWnd, WM_REQ_ADD_BITMAP, 0, 0);
			return ;
		}

		PostMessage(m_hWnd, WM_REQ_ADD_BITMAP, previewList->count(), (LPARAM)m_currentNode);
		PostMessage(m_hWnd, WM_REQ_ADD_BITMAP, previewList->count(), (LPARAM)0);
		YITRACE(5, "OnThreadStart(2) xSearchMap is success node=%d, %d msec", previewList->count(), cyiTime::GetSecDiff(t1, cyiTime::CurrentmSec()));

		
		//m_nStartIndex=0;
		//if (StartCreateBitmap()==True)
		//{
		//	::PostMessage(m_hWnd, WM_XSEARCHMAP_END, 0, 0);
		//}
		//previewList->unlock();
	}

	YITRACE(5, "cglsPreviewGoban::OnThreadStart(%d) exit", id);
}

// -------------------------------
LRESULT cglsPreviewGoban::OnFocus(WPARAM wp, LPARAM lp, Bool& handled)
{
	if (m_panel)
		SetFocus(m_panel->m_hWnd);
	return 0;
}

// -------------------------------
LRESULT cglsPreviewGoban::OnXSearchEnd(WPARAM wp, LPARAM lp, Bool& handled)
{
	YITRACE(5, "cglsPreviewGoban::OnXSearchEnd() called");
	//StartCreateBitmap();
	InvalidateRect(0, False);
	return 0;
}

// -------------------------------
LRESULT cglsPreviewGoban::OnNodeChanged(WPARAM wp, LPARAM lp, Bool& Handled)
{
	UpdateTree((cyiTreeNode*)lp);
	return 0;
}

// -------------------------------
LRESULT cglsPreviewGoban::OnRequestAddBitmap(WPARAM wp, LPARAM lp, Bool& Handled)
{
	int cnt =0;
	int delta=0;

	m_panel->MutexLock();

	if (wp!=0 &&
			lp==0)
	{
		// 正常終了
		char buffer[32];
		wsprintf(buffer, "全%d件", wp);
		SetWindowText(m_hWnd, buffer);
		m_panel->InvalidateRect(0);
		::YITRACE(5, "cglsPreviewGoban::OnRequestAddBitmap() success (%d)", wp);
	}
	else if (lp==0)
	{
		RECT xRect = {0, 0, 0, 0};
		m_panel->Clear();
		m_panel->SetBufferSize(xRect, True);
		InvalidateRect(0);
		SetWindowText(m_hWnd, 0);
		::YITRACE(5, "cglsPreviewGoban::OnRequestAddBitmap() clear buffer");
		m_panel->InvalidateRect(0);
		m_panel->MutexUnlock();
		return 0;
	}


	//unsigned int tbefore;
	//unsigned int t1 = cyiTime::CurrentmSec();


	cnt = m_panel->GetItemCount();


	//::YITRACE(5, "cglsPreviewGoban::OnRequestAddBitmap() count:%d", wp);
	delta = wp-cnt;
	for (int i=0; i<delta; i++)
	{
		m_panel->Add(512+cnt+i, 0, 0, 0);
	}

	

	RECT xRect = {0, 0, 0, previewList->count()*m_nPrevWidth};

	m_panel->SetBufferSize(xRect, True);

	if (cnt==0)
	{
		m_panel->InvalidateRect(0);
	}

	m_panel->MutexUnlock();



	

/*	cyiQueue* q = (cyiQueue*)wp;

	YITRACE(5, "cglsPreviewGoban::OnRequestAddBitmap() called ");

	if (lp==1)
	{
		m_bIsDataInvalid=False;
		Graphics g(m_bmpCurrent);
		//g.FillRectangle(&TextureBrush(m_bmpBgGoban), 0, 0, m_w, m_h);
		g.FillRectangle(&TextureBrush(m_bmpBgGoban), 0, 0, m_nPrevWidth, m_nPrevWidth*m_nShowCnt);

		//cyiBitmapBlender::FillColor(m_bmpCurrent, Color::White);
		InvalidateRect(0, False);
	}

	if (m_bIsDataInvalid==True)
	{
		YITRACE(5, "cglsPreviewGoban::OnRequestAddBitmap() discarded");
		yiAnyValue data;
		while(q->deq(data)==True)
		{
			Bitmap* bmp = (Bitmap*)data;
			delete bmp;
		}
		delete q;

		return 0;
	}

	yiAnyValue data;
	while(q->deq(data)==True)
	{

		Bitmap* bmp = (Bitmap*)data;

		//if (lp>m_panel->GetItemCount()+1)
		//{
		//	YITRACE(5, "cglsPreviewGoban::OnRequestAddBitmap() invalid (%d, %d)", lp, m_panel->GetItemCount());
		//	return 0;
		//}

		int id=m_panel->GetItemCount()+512;
		m_panel->Add(id, bmp, 0, 0);
	}


	delete q;
	//if (m_panel->GetItemCount()==m_nBufferringCount)
	//{
	//	m_bIsSlowLoad=True;
	//}

	//m_panel->UpdateTree(eGUM_SKIP_REDRAW);	
	*/
	return 0;
}

// ---------------------------------------
Bool cglsPreviewGoban::OnBgDraw()
{
	//return cglsWindow::OnBgDraw();
	////return True;
	m_bIsDataInvalid=False;

	if (m_bmpBgGoban==0)
		return False;

	if (m_bmpCurrent &&
		(m_bmpCurrent->GetWidth()!=m_w ||
			m_bmpCurrent->GetHeight()!=m_h))
	{
		delete m_bmpCurrent;
		m_bmpCurrent=0;

		if (m_bg)
			delete m_bg;
		m_bg=0;
	}

	if (m_bmpCurrent==0)
	{
		m_bmpCurrent=BITMAP32(m_w, m_h);
	}

	if (m_bg==0)
	{
		m_bg=BITMAP32(m_bmpCurrent->GetWidth(), m_bmpCurrent->GetHeight());
	}

	Graphics g(m_bg);

	cyiBitmapBlender::FillColor(m_bg, Color::White);
	g.FillRectangle(&TextureBrush(m_bmpBgGoban), 0, 0, m_nPrevWidth, m_nPrevWidth*m_nShowCnt);
	cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);
	//g.DrawImage(m_bmpNoData);
	//cyiBitmapBlender::FillColor(m_bmpCurrent, Color::White);
	//InvalidateRect(0, False);
	Update(m_bmpCurrent, 0);
	InvalidateRect(0, False);

	return True;
}


// ------------------------------------
LRESULT cglsPreviewGoban::OnNotHandledCommand(WPARAM wp, LPARAM lp)
{
	int id = wp;//((wp>>16)&0xFFFF);

	if (id-512<0)
		return 0;

	cyiNode* node = previewList->GetNodeTopFrom(id-512);

	if (node)
	{
		cyiTreeNode* t = (cyiTreeNode*)node->data;
		unsigned short ret = GetKeyState(VK_LCONTROL);
		::PostMessage(m_hNotifyWnd, WM_PREVIEW_SELECTED, (ret&0x8000)==0?0:1, (LPARAM)t);
	}
	return 0;
}

// ---------------------------------------
//int cglsPreviewGoban::GetBgIndex(eDrawItemStatus status, Bool bIsHitted, Bool& bIsSkipAnimation)
//{
//	return -1;
//}

// ---------------------------------
Bool cglsPreviewGoban::StartCreateBitmap()
{
	unsigned long t1 = cyiTime::CurrentmSec();

	m_bIsSlowLoad=False;

	int n = previewList->count();

	{
		int cnt=0;
		int queueCnt=10;
		Bool bIsFirst=True;
		cyiQueue* xqueue = new cyiQueue();
		//Sleep(100);

		RECT xRect = {0, 0, 0, previewList->count()*m_nPrevWidth};

		YITRACE(5, "cglsPreviewGoban::StartCreateBitmap stack pannel clear before");//, /*GetThreadId(GetCurrentThread())*/);
		m_panel->MutexLock();
		YITRACE(5, "cglsPreviewGoban::StartCreateBitmap stack pannel clear after");
		m_panel->Clear();
		m_panel->SetBufferSize(xRect, True);

		
		for (cyiNode* node = previewList->GetNodeTopFrom(); node; node=node->np)
		{
			cyiTreeNode* xnode = (cyiTreeNode*)node->data;

			if (m_bIsThreadStop==True)// || cnt > MAX_PREVIEW_COUNT)
			{
				YITRACE(5, "cglsPreviewGoban::OnThreadStart(1) aborted (%d)", cnt);
				m_panel->MutexUnlock();
				return False;
			}

			m_panel->Add(512+cnt, 0, 0, 0);
			++cnt;

		}

		m_panel->MutexUnlock();
		YITRACE(5, "%d item added %d msec", cnt, cyiTime::GetSecDiff(t1, cyiTime::CurrentmSec()));
	}

	return True;
}


// ---------------------------------
Bool cglsPreviewGoban::UpdateTree(cyiTreeNode* top, Bool bIsJosekiMode)
{
	Bool bIsNoUpdateScroll=True;

	if (top==m_currentNode)
	{
		return False;
	}

	//if (m_currentNode)
	//{
	//	if (previewList->count()==0 ||
	//			previewList->count()==1)
	//	{
	//		if (m_currentNode->GetParent()==top)
	//		{
	//			m_currentNode=top;

	//			int cnt = top->GetChildList().count();

	//			if (cnt==previewList->count())
	//			{
	//				return False;
	//			}
	//		}
	//		if (m_currentNode->GetChildList().count()==1)
	//		{
	//			if ((cyiTreeNode*)(m_currentNode->GetChildList().GetNodeTopFrom()->data)==top)
	//			{
	//				m_currentNode=top;
	//				return False;
	//			}
	//		}
	//	}
	//}

	if (m_hUpdteThreadMutex==0)
	{
		if (m_hUpdteThreadMutex=yiMutexCreateEx(3)==False)
		{
			YITRACE(5, "cglsPreviewGoban::UpdateTree(%x) Mutex Create Failure", top);
		}
	}

	m_bIsJosekiMode=bIsJosekiMode;

	YITRACE(5, "cglsPreviewGoban::UpdateTree(%x) called", top);
	if (top)
	{
		StartThread(THREAD_ANY_ID, (int)top);
	}
	else
	{
		top=m_currentNode;
		bIsNoUpdateScroll=False;
	}

	//if ((nListCount=previewList->count())==0)
	//{
	//	//return False;
	//}

	return 0;
}

// ---------------------------------------
BOOL cglsPreviewGoban::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	return True;
	//yiMutexLockEx(m_hPaintMutex);

	cglsWindow::OnPaint(hdc, ps);

	//yiMutexUnlockEx(m_hPaintMutex);
	
	//hdc = GetDC(m_hWnd);

	//Graphics gx(hdc);

	//gx.DrawEllipse(&Pen(Color::Black), RectF(rand()%10, 10, rand()%50, 50));
	//ReleaseDC(m_hWnd, hdc);
	return True;

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
LRESULT cglsPreviewGoban::OnClicked(WPARAM wp, LPARAM lp)
{
	int xPos   =  lp & 0xFFFF;
	int yPos   = (lp >> 16) & 0xFFFF;
	RECT rect;

	::GetClientRect(m_hWnd, &rect);

	int nBlockSize = rect.bottom/m_nShowCnt;
	int n = yPos/nBlockSize;

	if (xPos>m_nPrevWidth)
		return 0;

	if (m_bModePlay==True)
	{
		if (m_prevNode)
		{
			m_nStartIndex=m_nPrevStartIndex;
			UpdateTree((cyiTreeNode*)m_prevNode);
		}
		return 0;
	}

	cyiNode* listNode = previewList->GetNodeTopFrom(m_nStartIndex+n);

	if (listNode==0)
	{
		UpdateTree(0);
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
	//SetScrollRange(m_hWnd, SB_VERT,0, 1, TRUE);
	//SetScrollPos(m_hWnd, SB_VERT, 0, TRUE);

	m_prevNode=m_currentNode;
	m_bModePlay=True;
	UpdateTree(m_selectedNode);


	//UpdateTree(0);

	//m_nStartIndex=0;

	::InvalidateRect(m_hWnd, 0, False);
	return 0;
}

// ------------------------------------
LRESULT cglsPreviewGoban::OnMouseMove(WPARAM wp, LPARAM lp, Bool& handled)
{
	int xPos   =  lp & 0xFFFF;
	int yPos   = (lp >> 16) & 0xFFFF;
	RECT rect;

	::GetClientRect(m_hWnd, &rect);

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
LRESULT cglsPreviewGoban::OnPreviewDetailClosed(WPARAM wp, LPARAM lp, Bool& handled)
{
	UpdateTree((cyiTreeNode*)0xFFFFFFFF);
	return 0;
}

// ------------------------------------
LRESULT cglsPreviewGoban::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	m_nSelectedIndex=-1;
	::InvalidateRect(m_hWnd, 0, FALSE);
	return 0;
}

// -------------------------------
LRESULT cglsPreviewGoban::OnPlay(HWND hWnd, int code)
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
LRESULT cglsPreviewGoban::OnBack(HWND hWnd, int code)
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
LRESULT cglsPreviewGoban::OnPlayTop(HWND hWnd, int code)
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
LRESULT cglsPreviewGoban::OnPlayEnd(HWND hWnd, int code)
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
LRESULT cglsPreviewGoban::OnFilterChanged(HWND hWnd, int code)
{

	if (code!=CBN_SELCHANGE)
		return 0;

	int nFilter = m_cmbFilter.GetSelectIndex();

	YITRACE(5, "cglsPreviewGoban::OnFilterChanged(%d)", nFilter);

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

	UpdateTree((cyiTreeNode*)0xFFFFFFFF);
	return 0;
}

// -------------------------------
LRESULT cglsPreviewGoban::OnDetail(HWND hWnd, int code)
{
	m_dlgDetail.CreateFromReosurce(m_hWnd, 0, IDD_PREVIEW_FOR_POPUP, &m_sSearchFilter);
	return 0;
}

// -------------------------------
LRESULT cglsPreviewGoban::OnPreviewSelected(HWND hWnd, int code)
{
	return 0;
}

// ---------------------------------
void cglsPreviewGoban::Clear()
{

}



// ---------------------------------
LRESULT cglsPreviewGoban::OnVScroll(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	return 0;
	int nScrollCode = (int) LOWORD(wParam); // スクロールコード
	int nPos = (short int) HIWORD(wParam);  // スクロールボックス（つまみ）の位置 
	HWND hwndScrollBar = m_hWnd;//(HWND) lParam;      // スクロールバーのハンドル 
	int n=0;

	SCROLLINFO info;
	info.cbSize=sizeof(info);
	info.fMask=SIF_POS;
	GetScrollInfo(m_hWnd, SB_VERT, &info);

	nPos=info.nPos;

    switch (LOWORD(wParam)) {
        case SB_LINEUP:
			m_panel->Scroll(-m_nPrevWidth, 300);
			n=-1*m_nPrevWidth;
            break;
        case SB_LINEDOWN:
			m_panel->Scroll(m_nPrevWidth, 300);
			n=1*m_nPrevWidth;
            break;
        //case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
		{
            int n=nPos;// = HIWORD(wParam);
			//n*=m_nPrevWidth;
			int cur=m_panel->GetCurrentScrolledPos();
			m_panel->Scroll(n-cur,0);

            break;
		}
        case SB_PAGEDOWN:
			m_panel->Scroll(m_nShowCnt*m_nPrevWidth, 500, 128, 1000);
			n=m_nShowCnt*m_nPrevWidth;
            break;
        case SB_PAGEUP:
			m_panel->Scroll(-m_nShowCnt*m_nPrevWidth, 500, -128, 1000);
			n=-m_nShowCnt*m_nPrevWidth;
            break;
        default:
           return 0;
    }

	//if (m_nStartIndex>(m_nHitCount-m_nShowCnt))
	//	m_nStartIndex=m_nHitCount-m_nShowCnt;

	//if (m_nStartIndex<0)
	//	m_nStartIndex=0;

	//UpdateTree(0);
	//m_nStartIndex+=n;

	float ratio;
	int all = previewList->count()-m_nShowCnt;
	int xpos=m_panel->GetCurrentScrolledPos(&ratio);

	info.fMask=SIF_POS;
	info.nPos=(int)xpos;
	SetScrollInfo(m_hWnd, SB_VERT, &info, True);

	//SetScrollPos(m_hWnd, SB_VERT, (int)xpos/*((float)all*ratio)*/, TRUE);


	return 0;
}

// --------------------------
Bool cglsPreviewGoban::CreateBitmap(sDrawItem* item, void* pExtra)
{
	if (item->id==0)
	{
		item->pCurrentBmp=0;
		return False;

		if (item->pCurrentBmp)
			delete item->pCurrentBmp;

		item->pCurrentBmp=BITMAP32(item->rect.w,item->rect.h);
		Graphics g(item->pCurrentBmp);
		//g.FillRectangle(&SolidBrush(Color::White), 0, 0, item->rect.w, item->rect.h);
		//g.FillRectangle(&::TextureBrush(m_bmpBgGoban), 0, 0, item->rect.w, item->rect.h);
		g.FillRectangle(&TextureBrush(m_bmpBgGoban), 0, 0, m_nPrevWidth, m_nPrevWidth*m_nShowCnt);

		return True;
	}

	item->rect.w =m_nPrevWidth;
	item->rect.h = m_nPrevWidth;

	/*
	Bitmap* bmp = (Bitmap*)item->bitmapId;

	item->rect.w=bmp->GetWidth();
	item->rect.h=bmp->GetHeight();

	item->pBmp[0]=bmp;
	item->pCurrentBmp=item->pBmp[0];
	*/
	return True;
}

Bitmap* g_bmpCover=0;

Bitmap* g_bmpCoverDown=0;

#include "yiGoEngine.h"
// --------------------------
Bool cglsPreviewGoban::OnNullBitmapDraw(sDrawItem* item, void* pExtra)
{
	if ((int)pExtra==0)
	{
		int index = item->id-512;
		cyiNode* node= previewList->GetNodeTopFrom(index);

		if (node==0)
			return False;

		cyiTreeNode* xnode =(cyiTreeNode*)node->data;
		
		cSgfStoneInfo* info = (cSgfStoneInfo*)xnode->GetData();

		if (info==0)
		{
			return False;
		}

		if (info->map==0)
		{
			info->map = cyiGoEngine::CreateMap(xnode);
		}

		item->pBmp[0]=CreateGobanBitmap(info->map);	
		item->pCurrentBmp=item->pBmp[0];
		return True;
	}
	else if ((int)pExtra==1)
	{
		int w=m_nPrevWidth;//item->pBmp[0]->GetWidth();
		int h=m_nPrevWidth;//item->pBmp[0]->GetHeight();

		if (g_bmpCover==0)
		{
			g_bmpCover=cyiShape::DrawRoundRectEx(0, &Pen(Color::White), &LinearGradientBrush(PointF(0, 0), PointF(0, h), Color(64, 255,255,255), Color(128, 255,255,255)), RectF(0, 0, w, h), 2.0);
		}

		item->pBmp[(int)pExtra]=cyiBitmapBlender::BitmapCopy(0, item->pBmp[0]);
		Graphics g(item->pBmp[(int)pExtra]);
		g.DrawImage(g_bmpCover, 0, 0);

		return True;
	}
	else if ((int)pExtra==2)
	{
		int w=m_nPrevWidth;//item->pBmp[0]->GetWidth();
		int h=m_nPrevWidth;//item->pBmp[0]->GetHeight();

		if (g_bmpCoverDown==0)
		{
			g_bmpCoverDown=cyiShape::DrawRoundRectEx(0, &Pen(Color::White), &LinearGradientBrush(PointF(0, 0), PointF(0, h), Color(32, 255,255,255), Color(64, 255,255,255)), RectF(0, 0, w, h), 2.0);
		}

		item->pBmp[(int)pExtra]=cyiBitmapBlender::BitmapCopy(0, item->pBmp[0]);
		Graphics g(item->pBmp[(int)pExtra]);
		g.DrawImage(g_bmpCoverDown, 0, 0);

		return True;
	}

	return False;
}

// --------------------------
void cglsPreviewGoban::DestroyBitmap(sDrawItem* item)
{
	if (item->pBmp[0])
		delete item->pBmp[0];

	if (item->pBmp[1])
		delete item->pBmp[1];

	if (item->pBmp[2])
		delete item->pBmp[2];

	item->pBmp[0]=0;
	item->pBmp[1]=0;
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

