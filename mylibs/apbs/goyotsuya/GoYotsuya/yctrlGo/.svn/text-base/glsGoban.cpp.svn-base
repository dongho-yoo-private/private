#include "yi.h"
#include "yiGoEngine.h"
#include "glsGoban.h"
#include "shellapi.h"
#include "yistd.h"
#include "yctrlGobanResource.h"
#include "resource.h"
#include "yiGoCommandz.h"

#ifdef _TIMER_DEBUG_
sGoRemainTime g_black={0, };
sGoRemainTime g_white={0, };
#endif

HINSTANCE g_hDllInstace=0;
HINSTANCE g_hInstanceGoban=0;

cglsGoban::cglsGoban(void)
:	cGlassInterface(),
	m_datas(True),
	m_node_for_timer(0),
	m_nJumpCount(10),
	m_preview(0),
	m_listNodeChangeNotifyWnds(0),
	m_hBgThread(0),
	m_nSendStoneId(0),
	m_hCurrentCursor(0),
	m_nAddNewId(0),
#ifndef NEW_STYLE
	m_myView(0),
	m_nSelectedView(0),
	m_currentView(0),
	m_myFriendView(0),
	m_userMgr(0),
#endif
	m_imbCursor(0),
	m_menuStudy(0),
	m_hResourceInstance(0),
	m_bIsPreviewMode(False),

	m_imgPosX(0),
	m_imgPosY(0),
	m_cursor(0),
	m_editBox(0),
#ifdef NEW_STYLE
	m_bluredBg(0),
	m_hBlureThread(0),
	m_blureStop(0),
	m_listUserControl(0),
#endif
#ifndef NEW_STYLE
	m_infoDlg(0),
	m_infoLabel(0),
	m_stoneInfoDlg(0),
	m_currentCtrl(0),
	m_ctrlForPlay(0),
	m_ctrlForEdit(0),
	m_ctrlForEnjoy(0),
	m_ctrlForCalculate(0),
	m_titleDlg(0),
	m_playerBlack(0),
	m_playerWhite(0),
	m_textPannel(0),
#endif
	m_tab(0)

{
	//m_OffGraphics=NULL;
	//memset(m_goban_detail, 0, sizeof(m_goban_detail));
	m_ptEditBox.x=0;
	m_ptEditBox.y=0;

	m_BgLayer[0]=0;
	m_BgLayer[1]=0;

	for (int i=0; i<eGCT_COUNT; i++)
	{
		m_controlz[i]=0;
	}
	__GLASSINTERFACE_NAME__;

	m_hDrawMutex=0;
}

cglsGoban::~cglsGoban(void)
{
}



 ULONG_PTR           gdiToken;
 GdiplusStartupInput gdiSI;

#define GO_ID_RESERVED ((cyiGoEngine*)1)


 void cglsGoban::InitMember()
 {
	// 初期スケール
	m_scale=1.0;

	// 碁盤の数を初期化
	m_nGobanCount=0;

	//
	m_bReqiredLayerUpdate=False;

	// 選択されている碁盤ID
	m_nCurrentGoId=0;

	// ダブルバッファーリングをするためのインデックス
	m_UsedLayerIndex=0;

	m_bmpCursorBg=0;

	m_nDrawedIndex=0;

 }

// ----------------------------------------
ugoid_t& cglsGoban::MakeUniversalGoId(ubit32_t myId, ubit32_t remoteId, sSimpleGoHeader* hd, Bool* bReverse)
{
	static ugoid_t ugoid(myId, remoteId);
	
	ugoid.day  = hd->day;
	ugoid.time = hd->time;

	if (bReverse)
		*bReverse=False;

	if (hd->remoteId!=myId)
	{
		if ((hd->result&0x80000000)==0)
		{
			ugoid.b_id=hd->remoteId;
			ugoid.w_id=myId;
		}

		return ugoid;
	}

	if (bReverse)
		*bReverse=True;

	// 相手のファイル
	if ((hd->result&0x80000000)!=0)
	{
		ugoid.b_id=remoteId;
		ugoid.w_id=myId;
	}


	return ugoid;

}

// ----------------------------------------
void cglsGoban::InitGraphics(HINSTANCE hInstance)
{
#ifndef NEW_STYLE
	m_scale = (float)m_nGobanWidth/(float)MAX_GOBAN_SIZE;
#else
	m_scale = (float)m_w/(float)MAX_GOBAN_WIDTH;
#endif

	//cyctrlGobanResource::GetInstance().Initialize(m_scale, hInstance);
	//int w = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN)->GetWidth();
	//int h = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN)->GetHeight();

	//for (int i=0; i<19; i++)
	//	m_scaled_point[i] = ((float)MAX_START_INDEX+(float)MAX_BLOCK_SIZE*(float)i)*m_scale;

	//m_BgLayer[0]=new cyiSurface(w, h);
	//m_BgLayer[1]=new cyiSurface(w, h);

	//_DrawBackGroundLayer(True);
 }

#include "yctrlGobanResource.h"
// ----------------------------------------
//BOOL cglsGoban::Create(long x, long y, long w, long h, long nLine, HWND _hOwnerWnd)

// ------------------------------------------------------
void  cglsGoban::xDefaultDetail(sGobanDetail* detail)
{
	memset(detail, 0, sizeof(sGobanDetail));

	detail->mode=eGAM_NORMAL;
	//detail->bIsJosekiEditorMode=False;
	detail->bIsPreviewGotoSame=False;
	detail->nDefaultLineCount=19;
	detail->bIsDefaultLabelShow=True;
	detail->bIsPreviewClikFocus=True;
	detail->bIsPreviewNewTab=True;
	detail->eControlPos=eGCP_DEFAULT;
	detail->bIsPrevewShow=True;
	detail->bIsPreviewPopup=True;
	detail->bIsOldInterface=False;
	detail->menu=0;
}

// ------------------------------------------------------
// プレビューなどで使われる形式。
Bool cglsGoban::CreateForSimple(cGlassInterface* parent, HINSTANCE hModuleInstance, int id, int x, int y, int w, int h)
{
	const LPCSTR pszWindowClass = "YotuyacYotsuyaGoCtr";
	HWND hOwnerWnd = parent->m_hWnd;//IsWindow(_hOwnerWnd)==TRUE?_hOwnerWnd:0;

	m_parent=parent;
	m_hResourceInstance=hModuleInstance;
	m_bIsPreviewMode=True;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_DBLCLKS ;//CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hModuleInstance;//(m_hInstance=(HINSTANCE)::GetModuleHandle("yctrlGo.dll"));//GetWindowLong(hOwnerWnd, GWL_HINSTANCE));
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;//LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	=(HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= (LPCSTR)pszWindowClass;
	wcex.hIconSm		= NULL;

	ATOM ret = RegisterClassEx(&wcex);

	//if (ret==0)
	//{
	//	return FALSE;
	//}

	m_hParentWnd=hOwnerWnd;
	m_parent->RegisterChild(this);
	
	{
		HWND hOut;
		xSET_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW, True);
		xSET_GLS_FLAG(m_flag, eGF_UPDATE_INDEPENDENT, True);
		m_glsDrawInterface = GetDrawWindow(hOut);
	}

	m_w = w>h?h:w;
	m_h = w>h?h:w;

	m_x = x;
	m_y = y;

#ifndef NEW_STYLE
	m_nGobanWidth = m_w;
#endif
	memset(m_GoEnginez, 0, sizeof(m_GoEnginez));
	InitMember();

	m_hDrawMutex=0;


	m_hWnd = CreateWindow((LPCSTR)pszWindowClass, (LPCSTR)pszWindowClass, /*WS_CLIPSIBLINGS|*/WS_CHILD|WS_VISIBLE,
	x, y, m_w, m_h, hOwnerWnd, (HMENU)id, (HINSTANCE)hModuleInstance, this);




	if (m_hWnd==NULL)
	{
		return FALSE;
	}

	//m_x=x;
	//m_y=y;
	if (m_glsDrawInterface)
	{
		GetAbsoluteCoordination(m_nAbsX, m_nAbsY);
	}

	::SetWindowLongPtrA(m_hWnd, GWL_USERDATA, (IntPtr)this);

	SetWindowPos(m_hWnd, 0, x, y, m_w, m_w, SWP_SHOWWINDOW | SWP_NOZORDER);
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	//ctrl=GetGobanControl(mode);


	//GetControlPos(x, y);
#ifndef NEW_STYLE
	if (m_currentCtrl==0)
	{
		m_currentCtrl = new cglsGobanControl();
		sGlsGobanControlDetail detail;
		memset(&detail, 0, sizeof(detail));

		detail.font=cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON);

		m_currentCtrl->Create((HWND)m_parent, xCHILD_ID_CTRL_FOR_EDIT, m_scale, 0, 0, eGCS_SUPER_SIMPLE, &detail);
		m_currentCtrl->Show();
		m_currentCtrl->Move(m_x+((m_w-m_currentCtrl->m_w)>>1), m_y+m_h);
		m_currentCtrl->SetNotifyWindow(m_hWnd);
	}

	m_bIsCreated=True;

	m_infoLabel = new cglsStatic();
	m_infoLabel->Create((HWND)m_parent, m_x, m_currentCtrl->m_y+m_currentCtrl->m_h, 10, 10, 0, 0);
	m_infoLabel->Show();
#endif

	return True;

}

// ------------------------------------------------------
Bool cglsGoban::ShowBaloon(goid_t id, const wchar_t* pszText, int i, int j)
{
	if (IsNoDisplayComment(id)==True)
	{
		return False;
	}

	Bitmap* bmp = cyctrlGobanResource::GetInstance().GetImage(eRIDB_BSTONE);
	int x = m_scaled_point[i].x;//(bmp->GetWidth()>>1);//xCENTER(m_scaled_point[i], )+1;
	int y = m_scaled_point[j].y;//xCENTER(m_scaled_point[j], bmp->GetHeight())+1;

	sGoPlayInfo* info = (sGoPlayInfo*)GetExtraData(id);

	if (info==0)
		return False;

	Bool bIsUp = True;

	if (j>10)
		bIsUp=False;

	if (info->baloon==0)
	{
		info->baloon = xcglsBaloon();
		info->baloon->Create(this, xID_BALOON+id, x, y, pszText, bIsUp);//L"これはテストだけどどうだろう？うまくできている？", True);
	}
	else
	{
		info->baloon->SetText(pszText, bIsUp);
		info->baloon->Move(x, y);
	}

	info->baloon->Show();

	return True;
}

// ------------------------------------------------------
// 最終的にはこのように
BOOL cglsGoban::Create(HWND _hOwnerWnd, long x, long y, long w, long h, CUserManager* mgr, sGobanDetail* detail/*=0*/)
{
	const LPCSTR pszWindowClass = "YotuyacYotsuyaGoCtr";
	WNDCLASSEX wcex;
	HWND hOwnerWnd = IsWindow(_hOwnerWnd)==TRUE?_hOwnerWnd:0;

	if (detail)
		m_detail=*detail;
	else
		xDefaultDetail(&m_detail);

	m_nDefaultLineCount=m_detail.nDefaultLineCount;

	m_x=x;
	m_y=y;
	if (hOwnerWnd==0)
	{
		m_parent=(cGlassInterface*)_hOwnerWnd;
		hOwnerWnd=m_parent->m_hWnd;

		m_parent->RegisterChild(this);
		
		{
			HWND hOut;
			xSET_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW, True);
			xSET_GLS_FLAG(m_flag, eGF_UPDATE_INDEPENDENT, True);
			m_glsDrawInterface = GetDrawWindow(hOut);


		}
	}

#ifdef STATIC_COMPILE
	m_hInstance=(HINSTANCE)GetWindowLong(hOwnerWnd, GWL_HINSTANCE);//hInstance;
#else
	m_hInstance=g_hInstanceGoban;
#endif

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_DBLCLKS ;//CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;//(m_hInstance=(HINSTANCE)::GetModuleHandle("glsGo.dll"));//GetWindowLong(hOwnerWnd, GWL_HINSTANCE));
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;//LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	=(HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= (LPCSTR)pszWindowClass;
	wcex.hIconSm		= NULL;

	ATOM ret = RegisterClassEx(&wcex);

	if (ret==0)
	{
		return FALSE;
	}

	m_hParentWnd=hOwnerWnd;

	m_w = w>h?h:w;
	m_h = w>h?h:w;

	m_x = x;
	m_y = y;

#ifndef NEW_STYLE
	m_nGobanWidth = m_w;
#endif


	m_nDefaultLineCount = m_detail.nDefaultLineCount;
	m_hBgThread=0;
	m_aCurrentLabelList[0]=STONE_EOF;

	m_prevMouseCursorType=-1;

	//m_font=m_parent->m_font;

	m_prevX=-1;
	m_prevY=-1;

	m_bmpCurrentLabel=0;
	m_bmpCurrentLabelW=0;
	m_bmpCurrentLabelB=0;
	m_bLabelShow=m_detail.bIsDefaultLabelShow;

	m_branches.clear();


	memset(m_GoEnginez, 0, sizeof(m_GoEnginez));

	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		m_nCpuCount = info.dwNumberOfProcessors;
	}

	if (m_detail.bIsPreviewPopup==False)
	{
		m_detail.bIsPrevewShow=False;
	}

	if (m_detail.bIsPrevewShow==True)
	{
		m_preview = new cglsPreviewGoban();

		//if (m_detail.bIsPreviewPopup==False)
		//{
		//	m_preview->CreateAsChild((HWND)m_parent, CHILD_ID_GOBAN_PREVIEW, 0, 0, 19, m_detail.bIsJosekiEditorMode==True?ePM_JOSEKI_JITEN:ePM_NORMAL);
		//	x+=(m_preview->m_x+m_preview->m_w);

		//}
		//else
		//{
		ePreviewMode mode=ePreviewMode::ePM_NORMAL;
		mode=(m_detail.mode==eGAM_JOSEKI_EDITOR || m_detail.mode==eGAM_JOSEKI_PLAYER)?ePM_JOSEKI_JITEN:mode;
		m_preview->Create(hOwnerWnd, 128, 5, 19, mode);//m_detail.bIsJosekiEditorMode==True?ePM_JOSEKI_JITEN:ePM_NORMAL);
		//}

		RegisterNodeChangeNotifyWindow(*m_preview);
	}

	m_item.fTransparency=1.0;
	//m_item.fScale=1.0;

	InitMember();
	// 描画時の排他処理の初期化
	//DrawInitMutex();
	//m_hDrawMutex = yiMutexCreateEx(3);
	//m_hDrawMutex=0;

	m_id=1;
	m_hWnd = CreateWindow((LPCSTR)pszWindowClass, (LPCSTR)pszWindowClass, /*WS_CLIPSIBLINGS|*/WS_CHILD|WS_VISIBLE,
	x, y, w, h, hOwnerWnd, (HMENU)m_id, (HINSTANCE)m_hInstance, this);

	SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL));
	if (m_hWnd==NULL)
	{
		return FALSE;
	}

	//m_parent->RegisterChild(this);}
	m_bMouseTrackMode=True;
	//m_hCurrentCursor = LoadCursorA(g_hDllInstace, IDC_ARROW);
	//SetCursor(m_hCurrentCursor);

	m_x=x;
	m_y=y;
	if (m_glsDrawInterface)
	{
		GetAbsoluteCoordination(m_nAbsX, m_nAbsY);
	}

	SetWindowLongPtr(m_hWnd, GWL_USERDATA, (IntPtr)this);
	ChangeMouseCursor(0);

	// サウンドリソースロード
	HRSRC hResInfo = FindResource(m_hInstance, MAKEINTRESOURCEA(IDR_SOUND_MOVE), "WAVE");

	if (hResInfo)
	{
		m_soundMove = LockResource(::LoadResource(m_hInstance, hResInfo));
	}

	if (m_hWnd!=NULL)
	{
		// リソースのロード
		//yictrl_initialize();
		//InitGraphics();


		Update();
		SetWindowPos(m_hWnd, 0, x, y, m_w, m_w, SWP_SHOWWINDOW | SWP_NOZORDER);
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);

#ifndef NEW_STYLE
		m_userMgr=mgr;
#endif

		m_bIsCreated=True;
		YIINFO("cglsGoban::Create() success!");
		return TRUE;
	}

	return FALSE;
}

// ------------------------------------------------------
void cglsGoban::Destroy()
{
	if (m_bIsCreated==False)
		return ;

	if (m_hBgThread)
	{
		WaitForSingleObject(m_hBgThread, 3000);
		CloseHandle(m_hBgThread);
	}

	for (int i=0; i<MAX_GOBAN_COUNT; i++)
	{
		if (m_GoEnginez[i]==0)
			continue;

		void* p = m_GoEnginez[i]->GetExtraData();
		if (p)
			yifree(p);
		m_GoEnginez[i]->Destroy();
		delete m_GoEnginez[i];

		m_GoEnginez[i]=0;
	}

	__super::Destroy();
}

float cglsGoban::GetScale()
{
	return m_scale;
}

// -----------------------------------------
void cglsGoban::GetSize(int* all, int* goban, int* tab)
{
	int wGoban = m_h;
	int wTab=m_container->m_w;//m_tab->m_w;
	
	if (all)
		*all=wGoban+wTab;

	if (goban)
		*goban=wGoban;

	if (tab)
		*tab=wTab;
}

// ------------------------------------------------------
goid_t cglsGoban::AddForStream(IStream* stream)
{
	STATSTG stg;
	HRESULT hr = stream->Stat(&stg, STATFLAG_DEFAULT);

	if (FAILED(hr))
	{
		return 0;
	}

	if (stg.cbSize.LowPart==0)
	{
		return 0;
	}

	/*if (stg.pwcsName!=0 &&
			stg.pwcsName[0]!=0)
	{

	}*/

	void* buffer = yialloc(stg.cbSize.LowPart+16);
	char* pIndex=(char*)buffer;
	
	pIndex[stg.cbSize.LowPart]=0;

	ULONG reads=0;
	hr = stream->Read(buffer, stg.cbSize.LowPart, &reads);

	if (FAILED(hr))
	{
		yifree(buffer);
		return 0;
	}

	if (m_GoEnginez[1]!=0)
	{
		m_GoEnginez[1]->Destroy();
		delete m_GoEnginez[1];
		m_GoEnginez[1]=0;
	}

	Bool bIsJosekiEditor=False;
	cyiGoEngine* eng = new cyiGoEngine();
	sGobanContentsDetail detail;

	cyiGoEngine::xDefaultDetail(&detail);
	detail.eBranchViewModeValue = eBVM_HIDE;
	detail.eInputModeValue=eIM_ENJOY;

	if (eng->Create(buffer, (int)stg.cbSize.LowPart, &detail)==False)
	{
		delete eng;
		yifree(buffer);
		return 0;
	}
	yifree(buffer);
	//eng->SetExtraData(yialloci(sizeof(sGobanExtraData)));

	goid_t id = 1;//ReserveId();
	m_GoEnginez[id]=eng;

	//eng->SetExtraData(yialloci(sizeof(sGoPlayInfo)));
	//sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();

	eng->m_id=id;

/*
	{
		wchar_t buffer[128];
		char* pszTitle=(char*)eng->GetTitle();
		buffer[0]=0;
		yiToUnicode(buffer, pszTitle, eCS_JAPANESE);

		sGlsTabItem item;
		Bool bHasAButton=True;

		if (bIsJosekiEditor)
		{
			cglsTabCtrl::xDefaultItemDetail(&item, Color::Gray);
			bHasAButton=False;
		}
		else
		{
			cglsTabCtrl::xDefaultItemDetail(&item, Color::LightBlue);
		}

		m_tab->AddItem(id, 0, buffer, bHasAButton, &item);
		m_tab->SetCurSel(0,id, True);
	}
*/
	//ChangeCurrentGoban(id);

	//SetBranch(eng);
	m_nCurrentGoId=id;
	eng->Goto(MOVE_END, 0, True);
	Update(eGL_ALL, eng->GetStoneMap());

	//NotifyNodeChanged(True);

	m_nGobanCount++;

#ifndef NEW_STYLE
	if (m_infoLabel)
	{
		char buffer[256];
		char szDay[64];
		char szBlack[32];
		char szWhite[32];
		char szResult[32];
		sGoHeader* header = eng->GetSgfHeader();
		int year = DAY_YEAR(header->playDate);
		int mon  = DAY_MON(header->playDate);
		int day  = DAY_DAY(header->playDate);

		int winer = GET_WINER(header->flag32);
		int score = GET_SCORE(header->flag32);

		if (year!=0)
		{
			wsprintf(szDay, "対局日時:%4d-%2d-%2d", year, mon, day);
		}
		else
		{
			wsprintf(szDay, "対局日時:不明");
		}

		if (header->pszBlackName)
		{
			wsprintf(szBlack, "黒:%s", header->pszBlackName);
		}
		else
		{
			wsprintf(szBlack, "黒:不明", header->pszBlackName);
		}

		if (header->pszWhiteName)
		{
			wsprintf(szWhite, "白:%s", header->pszWhiteName);
		}
		else
		{
			wsprintf(szWhite, "白:不明");
		}

		if (winer==0)
		{
			wsprintf(szResult, "結果:不明");
		}
		else if (winer==3)
		{
			wsprintf(szResult, "結果:ジゴ");
		}
		else if (score==0xFF)
		{
			wsprintf(szResult, "結果: %s中押し勝ち", winer==1?"黒":"白");
		}
		else if (score==0xFE)
		{
			wsprintf(szResult, "結果: %s時間勝ち", winer==1?"黒":"白");
		}
		else
		{
			Bool bIsHalfScore=HALF_SCORE(header->condition);

			if (bIsHalfScore==False)
			{
				wsprintf(szResult, "結果: %s %d目勝ち", winer==1?"黒":"白", score);
			}
			else
			{
				if (score==0)
				{
					wsprintf(szResult, "結果: %s 半目勝ち", winer==1?"黒":"白", score);
				}
				else
					wsprintf(szResult, "結果: %s %d目半勝ち", winer==1?"黒":"白", score);
			}
		}

		wsprintf(buffer, " %s\r\n %s\r\n %s\r\n %s", szDay, szBlack, szWhite, szResult);
		m_infoLabel->SetText(0, buffer);
		m_infoLabel->Update(eGUM_FORCED);
	}
#endif

	return id;
}


// ------------------------------------------------------
goid_t cglsGoban::AddForWatch(sSimpleGoData* data, int size, ugoid_t& id, const char* pszBlackName, const char* pszWhiteName)
{
	goid_t goid=AddForEditBySimpleGodata(data, size, pszBlackName, pszWhiteName, eIM_WATCH);

	if (goid==0)
		return 0;

	m_GoEnginez[goid]->m_ugoid=id;
	//ChangeInputMode(goid, eIM_WATCH);
	Goto(goid, MOVE_END, 0);

	return goid;
	/*
	sGoHeader* header = (sGoHeader*)yialloci(sizeof(sGoHeader));

	if (pszBlackName)
		header->pszBlackName = yistrheap(pszBlackName);

	if (pszWhiteName)
		header->pszWhiteName = yistrheap(pszWhiteName);



	header->condition = data->condition;
	header->condition2= data->condition2;
	header->flag32 = data->flag32;//flag;

	header->playDate=id.day;

	SET_LINE_COUNT(header->flag32, 19);

	sGobanContentsDetail detail;
	cyiGoEngine::xDefaultDetail(&detail);

	detail.eBranchViewModeValue=eBVM_HIDE;
	detail.eContentsMode=eGCM_PLAY;
	detail.eInputModeValue=eIM_ENJOY;
	detail.eMouseModeValue=eMM_RBUTTON_RETURN;

	//m_GoEnginez[rId]->SetExtraData(info);
	goid_t ret = Add(header, ReserveId(), &detail, 0,&id);// id);

	//Goto(0, MOVE_TOP, 0);
	//Goto(0, MOVE_END, 0);


	//m_GoEnginez[ret]->Goto(MOVE_TOP, 0);
	////Goto(1, 0, 0);
	//Update(eGL_ALL);

	//if (m_currentCtrl==0)
	//{
	//	m_currentCtrl=GetGobanControl(eInputMode::eIM_ENJOY);
	//	xResetGobanControl(m_GoEnginez[ret]);
	//}



	if (m_currentView)
	{
		m_currentView->Hide();
	}




	//m_currentCtrl->InvalidateRect(0);
*/
	//return ret;

}

// ------------------------------------------------------
// 対局時に碁盤を追加するときに呼び出される。
// 注意すべきは、goid_tは、下位16ビットがローカルのIDであり、上位16ビットはリモートのIDである。
// リモートのIDがゼロの場合は失敗
goid_t cglsGoban::AddForPlay(goid_t rId, sGoPlayInfo* info, const char* pszBlackName, 
										const char* pszWhiteName, const char* pszTitle, 
											unsigned int condition, unsigned int condition2, unsigned int flag, 
												ugoid_t* uid, sSimpleGoData* godata, int godataSize)
{
	//goid_t id = ReserveId();
	Bool bIsIamBlack=IAM_BLACK(condition);

	if (godata!=0 && godataSize!=0)
	{
		condition=godata->condition;
		condition2=godata->condition2;
		flag = godata->flag32;

		bIsIamBlack=IAM_BLACK(condition);
		bIsIamBlack=1-bIsIamBlack;

		if (bIsIamBlack)
			SET_IAM_BLACK(condition);
		else
			SET_IAM_WHITE(condition);

		godata->condition=condition;
	}

	sGoHeader* header = (sGoHeader*)yialloci(sizeof(sGoHeader));

	if (pszBlackName)
		header->pszBlackName = yistrheap(pszBlackName);

	if (pszWhiteName)
		header->pszWhiteName = yistrheap(pszWhiteName);

	if (pszTitle)
		header->pszEventName = yistrheap(pszTitle);

	header->condition = condition;
	header->condition2= condition2;
	header->flag32 = flag;

	if (uid)
		header->playDate=uid->day;
	else
		cyiTime::GetLocalTime(header->playDate);

	SET_LINE_COUNT(header->flag32, 19);

	sGobanContentsDetail detail;
	cyiGoEngine::xDefaultDetail(&detail);

	detail.eBranchViewModeValue=eBVM_HIDE;
	detail.eContentsMode=eGCM_PLAY;
	detail.eInputModeValue=eIM_PLAY;
	detail.eMouseModeValue=eMM_RBUTTON_RETURN;

	//m_GoEnginez[rId]->SetExtraData(info);
	goid_t ret = Add(header, rId, &detail, info, uid, Color::LightPink, False);

	Goto(0, MOVE_TOP, 0);
	//m_GoEnginez[ret]->Goto(MOVE_TOP, 0);
	////Goto(1, 0, 0);
	//Update(eGL_ALL);

#ifndef NEW_STYLE
	if (m_currentCtrl==0)
	{
		m_currentCtrl=GetGobanControl(eInputMode::eIM_PLAY);
	}
#endif
	
	//
	
	Bool bIsEnable=True;
	if (IAM_BLACK(header->condition)==True)
	{

		if (m_GoEnginez[ret]->IsCurrentOrderBlack()==False)
		{
			bIsEnable=False;
		}
	}
	else
	{
		if (m_GoEnginez[ret]->IsCurrentOrderBlack()!=False)
		{
			bIsEnable=False;
		}
	}

#ifndef NEW_STYLE
	m_currentCtrl->EnableItem(eGCS_PASS ,bIsEnable);
	m_currentCtrl->EnableItem(eGCS_GIVEUP,bIsEnable);
	m_currentCtrl->EnableItem(eGCS_PAUSE, bIsEnable);
#endif

	if (rId==m_nCurrentGoId)
	{
		xResetGobanControl(m_GoEnginez[rId]);
	}
	else
	{
		ChangeCurrentGoban(rId);
	}



	//m_currentCtrl->InvalidateRect(0);

	return ret;
}

// ------------------------------------------------------
goid_t cglsGoban::AddForPlayBySimpleGoData(ubit32_t myId, ugoid_t& uid, sSimpleGoData* data)
{
	Bool bIsDataConvert=False;

	if (myId==data->hd.remoteId)
	{
		Bool bIsBlack = (data->hd.result&0x80000000)!=0;

		if (bIsBlack)
		{
			SET_IAM_WHITE(data->condition);
			data->hd.remoteId=uid.b_id;
		}
		else
		{
			SET_IAM_BLACK(data->condition);
			data->hd.remoteId=uid.w_id;
		}

		bIsDataConvert=True;

	}
	
	cyiGoEngine* eng = new cyiGoEngine();
	
	int size = sizeof(sSimpleGoData)+(((data->hd.result>>6)&0x1FF)<<1);
	if (eng->Create(data, size, 0, 0, eIM_PLAY, myId)==False)
	{
		delete eng;
		return False;
	}
	
	if (bIsDataConvert)
	{
		eng->ConvertGobanDirection();
		eng->Goto(MOVE_TOP, 0, 0);
	}

	eng->Goto(MOVE_END, 0, 0);

	goid_t id = ReserveId();
	sGoPlayInfo*s =0;
	m_GoEnginez[id]=eng;

	eng->SetExtraData((s=(sGoPlayInfo*)yialloci(sizeof(sGoPlayInfo))));
	eng->m_id=id;

//	eng->SetInputMode(eIM_PLAY);

	s->uid=uid;

	ChangeCurrentGoban(id);

	{
		wchar_t buffer[128];
		char* pszTitle=(char*)eng->GetTitle();
		buffer[0]=0;
		yiToUnicode(buffer, pszTitle, eCS_JAPANESE);

		sGlsTabItem item;
		Bool bHasAButton=False;
		cglsTabCtrl::xDefaultItemDetail(&item, Color::LightPink);

		m_tab->AddItem(id, 0, buffer, bHasAButton, &item);
		m_tab->SetCurSel(0,id, True);
	}

	//ChangeInputMode(eIM_PLAY);
	xResetGobanControl(m_GoEnginez[id]);


	//m_nCurrentGoId=id;
	//if (m_currentCtrl==0)
	//{
	//	m_currentCtrl=GetGobanControl(eInputMode::eIM_PLAY);
	//}
	
	//
	sGoHeader* header = eng->GetHeader();
	Bool bIsEnable=True;
	if (IAM_BLACK(header->condition)==True)
	{
		if (eng->IsCurrentOrderBlack()==False)
		{
			bIsEnable=False;
		}
	}
	else
	{
		if (eng->IsCurrentOrderBlack()!=False)
		{
			bIsEnable=False;
			
		}
	}

#ifndef NEW_STYLE
	m_currentCtrl->EnableItem(eGCS_PASS ,bIsEnable);
	m_currentCtrl->EnableItem(eGCS_GIVEUP,bIsEnable);
	m_currentCtrl->EnableItem(eGCS_PAUSE, bIsEnable);
#endif

#ifndef NEW_STYLE
	int blackCur		= (data->reserved[0]>>16);		// 黒の残り時刻。
	int blackRemainReadSec	= (data->reserved[0]&0xFFFF);	// 黒の残り秒読み
	int whiteCur		= (data->reserved[1]>>16);		// 白の残り時刻。
	int whiteRemainReadSec	= (data->reserved[1]&0xFFFF);	// 白の残り秒読み
	unsigned int nNormalReadSec = GET_SECOND_READ(header->condition);
	unsigned int nReadSec = GET_SECOND_READ2(header->condition);

	m_playerBlack->AddTimer(id, blackCur, nNormalReadSec, nReadSec, blackRemainReadSec);
	m_playerWhite->AddTimer(id, whiteCur, nNormalReadSec, nReadSec, whiteRemainReadSec);

	if (eng->IsCurrentOrderBlack()==True)
	{
		s->black.startTime=cyiTime::CurrentmSec();
		m_playerBlack->StartTimer(id);
		m_playerWhite->StopTimer(id, 0);
	}
	else
	{
		s->white.startTime=cyiTime::CurrentmSec();
		m_playerWhite->StartTimer(id);
		m_playerBlack->StopTimer(id, 0);
	}
#endif

	//if (id==m_nCurrentGoId)
	//{
	//	xResetGobanControl(eng);
	//}
	//else
	//{
	//	ChangeCurrentGoban(id);
	//}


	
	return id;
}

// ------------------------------------------------------
goid_t cglsGoban::AddForEditBySimpleGodata(sSimpleGoData* data, int size, const char* pszBlackName, const char* pszWhiteName, eInputMode input)
{
	cyiGoEngine* eng = new cyiGoEngine();

	if (eng->Create(data, size, pszBlackName, pszWhiteName, input)==False)
	{
		delete eng;
		return False;
	}

	goid_t id = ReserveId();
	m_GoEnginez[id]=eng;

	eng->SetExtraData(yialloci(sizeof(sGoPlayInfo)));
	eng->m_id=id;

	{
		wchar_t buffer[128];
		char* pszTitle=(char*)eng->GetTitle();
		buffer[0]=0;
		yiToUnicode(buffer, pszTitle, eCS_JAPANESE);

		sGlsTabItem item;
		Bool bHasAButton=True;
		cglsTabCtrl::xDefaultItemDetail(&item, Color::LightBlue);

		m_tab->AddItem(id, 0, buffer, bHasAButton, &item);
		m_tab->SetCurSel(0,id, True);
	}

	ChangeCurrentGoban(id);

	//SetBranch(eng);
	Update(eGL_ALL, eng->GetStoneMap());

	NotifyNodeChanged(True);

	MSG msg;
	while(::PeekMessage(&msg, m_hWnd, 0, 0, True))
	{
		if (msg.message==WM_LBUTTONDOWN)
		{
			continue;
		}
		if (msg.message==WM_MOUSEMOVE)
		{
			continue;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	m_nGobanCount++;

	PostMessage(m_hParentWnd, WM_GOBAN_ADDED, 0, (LPARAM)id);
	return id;
}


// ------------------------------------------------------
// ファイルからの読み込み
//
goid_t cglsGoban::AddForEditByFile(const char* pszFileName, sGobanContentsDetail* detail)
{
	int size=0;
	void* p = yiFileLoad(pszFileName, &size);

	if (p)
	{
		return AddForEditByFile(p, size, detail);
	}
	return 0;
}

goid_t cglsGoban::AddForEditByFile(void* p, int size, sGobanContentsDetail* detail)
{
	Bool bIsJosekiEditor=False;
	cyiGoEngine* eng = new cyiGoEngine();

	if (eng->Create(p, size, detail)==False)
	//if (eng->Create(pszFileName, detail)==False)
	{
		delete eng;
		return 0;
	}
	//eng->SetExtraData(yialloci(sizeof(sGobanExtraData)));

	goid_t id = ReserveId();
	m_GoEnginez[id]=eng;

	eng->SetExtraData(yialloci(sizeof(sGoPlayInfo)));
	//sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();

	eng->m_id=id;

	if (xGET_GOBAN_MODE(eng->m_flag)==eGCM_JOSEKI_EDITOR)
	{
		eng->SetTitle(cyctrlGobanResource::GetInstance().GetStringA(eGTRID_MAIN));
		bIsJosekiEditor=True;
	}

	{
		wchar_t buffer[128];
		char* pszTitle=(char*)eng->GetTitle();
		buffer[0]=0;
		yiToUnicode(buffer, pszTitle, eCS_JAPANESE);

		sGlsTabItem item;
		Bool bHasAButton=True;

		if (bIsJosekiEditor)
		{
			cglsTabCtrl::xDefaultItemDetail(&item, Color::Gray);
			bHasAButton=False;
		}
		else
		{
			cglsTabCtrl::xDefaultItemDetail(&item, Color::LightBlue);
		}

		m_tab->AddItem(id, 0, buffer, bHasAButton, &item);
		m_tab->SetCurSel(0,id, True);
	}

	ChangeCurrentGoban(id);

	//SetBranch(eng);
	Update(eGL_ALL, eng->GetStoneMap());

	NotifyNodeChanged(True);

	m_nGobanCount++;

	MSG msg;
	while(::PeekMessage(&msg, m_hWnd, 0, 0, True))
	{
		if (msg.message==WM_LBUTTONDOWN)
		{
			continue;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	PostMessage(m_hParentWnd, WM_GOBAN_ADDED, 0, (LPARAM)id);
	SetTimer(m_hWnd, 3, 500, 0);
	return id;
}

#define THREAD_ID_SEARCH 1
#define THREAD_ID_OPTIMIZE 2

// ------------------------------------------------------
void cglsGoban::OnThreadStart(int id, int param1, int param2)
{

	if (id==THREAD_ID_SEARCH)
	{
		param1=param1?param1:m_nCurrentGoId;
		cyiGoEngine* eng = m_GoEnginez[param1];

		if (!eng)
			return ;

		eng->SearchResult();
		if (m_nCurrentGoId==param1)
		{
			PostMessage(m_hWnd/*m_hParentWnd*/, WM_QUIT_SEARCHRESULT, 0, (LPARAM)eng->GetCurrentNode());
		}
	}
	else if (id==THREAD_ID_OPTIMIZE)
	{
		cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
		int n = eng->OptimizeNode((int*)param1, (Bool*)param2);
		PostMessage(m_hParentWnd, WM_QUIT_OPTIMIZE, n, 0);

		if (n)
		{
			SetChangedFlag(True);
		}
	}
}

// ------------------------------------------------------
void cglsGoban::SearchResult(goid_t id)
{
	StartThread(THREAD_ID_SEARCH, id);
}


// -----------------------------------
int cglsGoban::OptimizeNode(int* nProgress, Bool* canceled)
{
	StartThread(THREAD_ID_OPTIMIZE, (IntPtr)nProgress, (IntPtr)canceled);
	return 0;
}

// -----------------------------------
int cglsGoban::GetFullNodeCnt()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return 0;
	return eng->GetFullNodeCnt();
}
#ifndef NEW_STYLE
#include "glsMyView.h"
#endif
// -----------------------------------
//Bool cglsGoban::SetMyView(sMyViewInfo* info, eMyViewStyle style, CUserManager* mgr)
//{
//	if (m_myView==0)
//	{
//		float fFontSize = 24.0*m_scale;
//		fFontSize=fFontSize>16.0?16.0:fFontSize;
//
//		Font font(L"Meiryo UI", fFontSize);
//
//		m_tab->AddItem(257, 0, L"マイページ");
//		m_tab->SetCurSel(0, 257);
//		m_myView = new cglsMyView();
//
//		m_myView->Create(this, 0, 0, m_w, m_w, info, &font, style);
//		m_myView->Show();
//
//		m_currentView=m_myView;
//
//		xCreateMyButtons();
//	}
//
//	m_userMgr=mgr;
//
//	return True;
//}

// -----------------------------------
//Bool cglsGoban::SetMyFriendView(CUserManager* mgr)
//{
//	//float fFontSize = 24.0*m_scale;
//	//fFontSize=fFontSize>16.0?16.0:fFontSize;
//	//Font font2(L"Meiryo UI", fFontSize*0.8);
//
//	if (m_myFriendView==0)
//	{
//		float fFontSize = 24.0*m_scale;
//		fFontSize=fFontSize>16.0?16.0:fFontSize;
//
//		Font font(L"Meiryo UI", fFontSize);
//
//
//	}
//	else
//	{
//		m_myFriendView->UpdateFriends();
//	}
//	//m_myFriendView->Show();//AnimationShow(True, 300);
//	//m_myFriendView->Move(m_container->m_x+m_container->m_w, 0);
//	//m_myFriendView->SetFriendList(friendList, listCached);
//
//	return True;
//}


#include "sgfmanager.h"
// -----------------------------------
Bool cglsGoban::ReadHeader(const char* pszFileName, sGoHeaderEx* header)
{
	cSgfManager mgr;
	cSgfManager* pMgr=&mgr;
	cStoneMap map;

	if (pszFileName!=0)
	{
		if (mgr.LoadSgfFile(pszFileName)==False)
			return False;
	}
	else
	{
		pMgr=GetCurrentGoEngine()->GetSgfManager();
	}

	
	//header->header=mgr.m_header;
	header->header.condition=pMgr->m_header->condition;
	header->header.condition2=pMgr->m_header->condition2;
	header->header.flag32=pMgr->m_header->flag32;
	header->header.playDate=pMgr->m_header->playDate;
	header->header.pszEventName=yistrheap(pMgr->m_header->pszEventName);
	header->header.pszBlackName=yistrheap(pMgr->m_header->pszBlackName);
	header->header.pszWhiteName=yistrheap(pMgr->m_header->pszWhiteName);

	header->n=pMgr->CreateMapImageEx(0, -1, header->map);

	if (header->n==0)
		return False;

	return True;
}

// -----------------------------------
Bool cglsGoban::Lock(goid_t id)
{
	id = id?id:m_nCurrentGoId;
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng)
	{
		eng->m_bIsLocked=True;

		if (id==m_nCurrentGoId)
		{
#ifndef NEW_STYLE
			if (m_currentCtrl)
			{
				m_currentCtrl->Enable(False);
			}
#endif
		}
	}
	return True;
}

// -----------------------------------
Bool cglsGoban::Unlock(goid_t id)
{
	id = id?id:m_nCurrentGoId;
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng)
	{
		eng->m_bIsLocked=False;

		if (id==m_nCurrentGoId)
		{
#ifndef NEW_STYLE
			if (m_currentCtrl)
			{
				m_currentCtrl->Enable(True);
			}
#endif
		}		
	}
	return True;
}

// -----------------------------------
Bool cglsGoban::SetChanged(goid_t id, Bool bIsChanged)
{
	id = id?id:m_nCurrentGoId;
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng)
	{
		if (eng->GetInputMode()==eIM_EDIT)
		{
			//Bool xbIsChanged = eng->IsChanged();
			//if (xbIsChanged!=bIsChanged)
			//{
				eng->SetChangedFlag(bIsChanged);
				m_tab->SetItemChanged(id, bIsChanged);
			//}
		}
	}

	return False;
}

// -----------------------------------
Bitmap* cglsGoban::CreateResultBitmap(int winer, int score, Bool bIsHalfScore)
{
	Bitmap* bmpTitle=0;

	float fontSize = 48.0*m_scale;

	if (winer!=3)
	{
		if (score==0xFF)
		{
			if (winer==1)
			{
				bmpTitle=cyiShape::BitmapString(0, L"黒 中押し勝ち!!", &Font(L"ＭＳ Ｐ明朝", fontSize), &SolidBrush(Color::Black), eSTS_GROW, Color::White, 5.0, 0.0);
			}
			else
			{
				bmpTitle=cyiShape::BitmapString(0, L"白 中押し勝ち!!", &Font(L"ＭＳ Ｐ明朝", fontSize), &SolidBrush(Color::White), eSTS_GROW, Color::Black, 5.0, 0.0);
			}
		}
		else if (score=0xFE)
		{
			if (winer==1)
			{
				bmpTitle=cyiShape::BitmapString(0, L"黒 時間勝ち!!", &Font(L"ＭＳ Ｐ明朝", fontSize), &SolidBrush(Color::Black), eSTS_GROW, Color::White, 5.0, 0.0);
			}
			else
			{
				bmpTitle=cyiShape::BitmapString(0, L"白 時間勝ち!!", &Font(L"ＭＳ Ｐ明朝", fontSize), &SolidBrush(Color::White), eSTS_GROW, Color::Black, 5.0, 0.0);
			}
		}
	}

	if (winer==3)
	{
		bmpTitle = cyiShape::BitmapString(0, L"ジゴ", &Font(L"ＭＳ Ｐ明朝", fontSize*m_scale), &SolidBrush(Color::Black), eSTS_GROW, Color::White, 5.0, 0.0);
	}
	else
	{
		wchar_t wszTitle[64];

		if (bIsHalfScore==False)
		{
			wsprintfW(wszTitle, L"%s %d目勝ち", winer==1?L"黒":L"白", score);
		}
		else
		{
			if (score!=0)
			{
				wsprintfW(wszTitle, L"%s %d目半勝ち", winer==1?L"黒":L"白", score);
			}
			else
			{
				wsprintfW(wszTitle, L"%s 半目勝ち", winer==1?L"黒":L"白", score);
			}
		}
	}


	return bmpTitle;

}

#include "cglsResultDialog.h"

// -----------------------------------
void cglsGoban::SetPlayResult(int goid, int winer, int score, Bool bIsShowResult, sResult* result)
{
	goid=(goid==0)?m_nCurrentGoId:goid;
	cyiGoEngine* eng = m_GoEnginez[goid];

	if (eng==0)
		return ;

	if (winer==0 && score==0)
	{
		ShowMessage(goid, L"対局が中断されました。", L"", MB_OK);
		return ;
	}

	SET_SCORE(eng->m_sgfMgr.m_header->flag32, score);
	
	if (winer==1)
		SET_BLACK_WIN(eng->m_sgfMgr.m_header->flag32);
	else if (winer==2)
		SET_WHITE_WIN(eng->m_sgfMgr.m_header->flag32);
	else
		SET_DRAW(eng->m_sgfMgr.m_header->flag32);

	xSET_BIT(eng->m_flag, eGEF_NOW_SHOW_RESULT, True);

	sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();

	if (info==0)
	{
		eng->SetExtraData((info=(sGoPlayInfo*)yialloci(sizeof(sGoPlayInfo))));
	}

	if (info)
	{
		if (info->dlg)
		{
			info->dlg->Destroy();
			delete info->dlg;
		}

		cglsResultDialog* dlg = new cglsResultDialog();
		dlg->ShowResult(this, goid, eng->GetHeader(), result, &Font(L"Meiryo UI", 20.0*m_scale));
		dlg->Hide();
		info->dlg=dlg;
	}

	if (bIsShowResult==True &&
			goid==m_nCurrentGoId)
	{
		info->dlg->Show();

		//m_infoDlg->UpdateHeader(eng->m_sgfMgr.m_header);
		//m_infoDlg->InvalidateRect(0);

		//if (m_imbResult==0)
		//{
		//	sGlsImageBoxDetail detail;
		//	cglsImageBox::xDefaultDetail(&detail);
		//	m_imbResult=new cglsImageBox();

		//	detail.bHasBorder=False;
		//	detail.bAutoResize=True;
		//	detail.bIsDragAndDrop=False;
		//	detail.bIsClickEvent=True;
		//	m_imbResult->Create(this,xCHILD_ID_RESULT, 0, 0, 100, 100, 0, &detail);
		//}

		//m_imbResult->SetImage(CreateResultBitmap(winer, score, HALF_SCORE(eng->m_sgfMgr.m_header->condition)), eGIBVS_NONE);
		//m_imbResult->Move((m_w-m_imbResult->m_w)>>1, (m_h-m_imbResult->m_h)>>1);
		//m_imbResult->AnimationShow(True, 1000);
	}



}


// -----------------------------------
Bool cglsGoban::GetHeaderEx(int goid, sGoHeaderEx* header)
{
	goid=(goid==0)?m_nCurrentGoId:goid;
	cyiGoEngine* eng = m_GoEnginez[goid];

	if (eng==0)
		return False;

	//header->header=mgr.m_header;
	header->header.condition=eng->m_sgfMgr.m_header->condition;
	header->header.condition2=eng->m_sgfMgr.m_header->condition2;
	header->header.flag32=eng->m_sgfMgr.m_header->flag32;
	header->header.playDate=eng->m_sgfMgr.m_header->playDate;
	header->header.pszEventName=yistrheap(eng->m_sgfMgr.m_header->pszEventName);
	header->header.pszBlackName=yistrheap(eng->m_sgfMgr.m_header->pszBlackName);
	header->header.pszWhiteName=yistrheap(eng->m_sgfMgr.m_header->pszWhiteName);

	header->n=eng->m_sgfMgr.CreateMapImageEx(0, -1, header->map);

	if (header->n==0)
		return False;

	return True;
}

// ---------------------------------------------------------------
int cglsGoban::GetCurrentOrder(goid_t id)
{
	id = id==0?m_nCurrentGoId:id;

	if (m_GoEnginez[id])
	{
		return m_GoEnginez[id]->GetCurrentOrder();
	}

	return 0;
}

// ---------------------------------------------------------------
void cglsGoban::AppendComment(goid_t id, const char* pszComment)
{
	id = id==0?m_nCurrentGoId:id;

	if (m_GoEnginez[id])
	{
		sGoHeader* header = m_GoEnginez[id]->GetHeader();

		m_GoEnginez[id]->PutComment(pszComment, True);
	}
}


// ---------------------------------------------------------------
Bool cglsGoban::IsNoDisplayComment(goid_t id)
{
#ifndef NEW_STYLE
	if (m_textPannel)
	{
		return m_textPannel->IsMessageInGobanVisible();
	}
#endif
	return False;
}



// -----------------------------------
Bool cglsGoban::ShowPreviewWindow(Bool bIsShow)
{
	m_detail.bIsPrevewShow=bIsShow;

	if (m_detail.bIsPreviewPopup==False)
	{
		m_preview=0;
		return False;
	}

	if (m_detail.bIsPrevewShow)
	{
		if (m_preview==0)
		{
			m_preview = new cglsPreviewGoban();
		}

		//if (m_detail.bIsPreviewPopup==False)
		//{
		//	m_preview->CreateAsChild(m_hParentWnd, CHILD_ID_GOBAN_PREVIEW, 0, 0, 19, m_detail.bIsJosekiEditorMode==True?ePM_JOSEKI_JITEN:ePM_NORMAL);
		//}
		//else
		//{
		ePreviewMode mode=ePreviewMode::ePM_NORMAL;
		mode=(m_detail.mode==eGAM_JOSEKI_EDITOR || m_detail.mode==eGAM_JOSEKI_PLAYER)?ePM_JOSEKI_JITEN:mode;

		m_preview->Create(m_hParentWnd, 128, 5, 19, mode);
		//}
		
		m_preview->Show();
		RegisterNodeChangeNotifyWindow(*m_preview);
	}
	else
	{
		if (m_preview)
		{
			m_preview->Hide();
		}
	}

	if (m_detail.bIsPreviewPopup==False)
	{
		if (m_detail.bIsPrevewShow==True)
		{
			Resize(m_preview->m_x+m_preview->m_w, 0, m_w, m_h);
		}
		else 
		{
			Resize(0, 0, m_w, m_h);
		}
	}
	return True;
}

// -----------------------------------
Bool cglsGoban::IsChanged()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return False;

	return eng->IsChanged();
}

// -----------------------------------
void cglsGoban::SetChangedFlag(Bool bIsChanged)
{
	SetChanged(m_nCurrentGoId, bIsChanged);
}


// ------------------------------------------------------
void cglsGoban::CreateMapIamge(cyiTreeNode* node)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return ;

	eng->CreateMapImage(node);
}



// ------------------------------------------------------
goid_t cglsGoban::AddForEdit(const char* pszBlackName, const char* pszWhiteName, 
							   const char* pszTitle, unsigned int condition, unsigned int flag)
{
	sGoHeader* header = (sGoHeader*)yialloci(sizeof(sGoHeader));

	if (pszBlackName)
		header->pszBlackName = yistrheap(pszBlackName);

	if (pszWhiteName)
		header->pszWhiteName = yistrheap(pszWhiteName);

	if (pszTitle)
		header->pszEventName = yistrheap(pszTitle);

	header->condition = condition;
	header->flag32 = flag;

	sGobanContentsDetail detail;
	cyiGoEngine::xDefaultDetail(&detail);
	detail.eBranchViewModeValue=eBVM_NORMAL;
	return Add(header, 0, &detail);
}

// ------------------------------------------------------
goid_t cglsGoban::AddByNode(cyiTreeNode* node, const char* pszTitle, Bool bIsEditable, Bool bIsActivate)
{
	goid_t id=ReserveId();

	if (id==0)
		return 0;

	cyiGoEngine* eng = new cyiGoEngine();
	cyiStack stack;
	cyiTreeNode* pNode=node;
	stack.clear();
	
	while(pNode)
	{
		stack.push((yiAnyValue)pNode);
		pNode=pNode->GetParent();
	}


	sGobanContentsDetail detail;
	cyiGoEngine::xDefaultDetail(&detail);

	detail.eContentsMode=eGCM_PREVIEW;
	detail.eInputModeValue=bIsEditable==True?eIM_EDIT:eIM_ENJOY;
	detail.eMouseModeValue=eMM_RBUTTON_RETURN;

	eng->m_id=id;
	eng->Create((sGoHeader*)0, &detail);
	eng->SetRemoteId(HIWORD(0));

	if (pszTitle)
		eng->SetTitle(pszTitle);



	yiAnyValue data;
	while(stack.pop((yiAnyValue&)data)==True)
	{
		pNode=(cyiTreeNode*)data;

		cSgfStoneInfo* stoneInfo = (cSgfStoneInfo*)pNode->GetData();

		if (stoneInfo==0)
		{
			continue;
		}

		aStoneList list;
		eng->PutStone(stoneInfo->stone, list);
		if (stoneInfo->comment)
		{
			eng->PutComment(stoneInfo->comment);
		}
		
		//if (stoneInfo->label)
		//{
		//	for (cyiNode* node=stoneInfo->label
		//}
	}


	aStoneList branches;
	eng->Goto(MOVE_TOP, branches);
	eng->Goto(1, branches);
	
	m_GoEnginez[id]=eng;
	eng->m_id=id;

	sGoPlayInfo* info=(sGoPlayInfo*)eng->GetExtraData();
	if (info)
	{
		if (info->dlg)
		{
			info->dlg->Destroy();
			delete info->dlg;
			info->dlg=0;
		}
	}
	if (info)
	{
		memset(info,0, sizeof(*info));
	}
	else
	{
		info=(sGoPlayInfo*)yialloci(sizeof(sGoPlayInfo));
		eng->SetExtraData(info);
	}
	//m_goban_detail[id] = (sGobanExtraDetail* )yialloci(sizeof(sGobanExtraDetail));

	m_nGobanCount++;

	if (m_detail.bIsPreviewClikFocus==True)
	{
		ChangeCurrentGoban(id);
	}
	//Update();

	{
		wchar_t buffer[128];
		char* pszTitle=(char*)eng->GetTitle();
		yiToUnicode(buffer, pszTitle, eCS_JAPANESE);

		sGlsTabItem item;
		cglsTabCtrl::xDefaultItemDetail(&item, Color::LightPink);
		m_tab->AddItem(id, 0, buffer, True, &item);
		
		if (m_detail.bIsPreviewClikFocus==True ||
			bIsActivate==True)
		{
			m_tab->SetCurSel(0, id, False);
			//m_tab->Update(eGUM_FORCED);
		}

	}

	return id;
}

// ------------------------------------------------------
void cglsGoban::ChangeOrder()
{
	stone_t stone = m_GoEnginez[m_nCurrentGoId]->m_laststone;

	if (STONE_TYPE(stone)==eST_BLACK ||
			STONE_TYPE(stone)==eST_BPASS)
	{
		m_GoEnginez[m_nCurrentGoId]->m_laststone=MAKESTONE(eST_WPASS, 0, 0, 0);
	}
	else
	{
		m_GoEnginez[m_nCurrentGoId]->m_laststone=MAKESTONE(eST_BPASS, 0, 0, 0);
	}
}

// ------------------------------------------------------
goid_t cglsGoban::xAddReference(goid_t id, cyiTreeNode* node, int nIndex, goid_t nCurrentGoId, cyiTreeNode* from/*=True*/)
{
	if (id==0)
	{
		id=ReserveId();
	}

	if (id==0)
		return 0;

	cyiGoEngine* eng = new cyiGoEngine();
	aStoneList branches;

	sGobanContentsDetail detail;
	cyiGoEngine::xDefaultDetail(&detail);
	detail.eContentsMode=from==0?eGCM_REFERENCE:eGCM_REFERENCE2;

	detail.eBranchViewModeValue=xGET_BRANCH_VIEW_MODE(m_GoEnginez[nCurrentGoId]->m_flag);
	detail.eInputModeValue=m_GoEnginez[nCurrentGoId]->m_input_mode;
	detail.eMouseModeValue=m_GoEnginez[nCurrentGoId]->m_mouse_mode;
	eng->Create((sGoHeader*)0, &detail);
	//eng->SetExtraData(yialloci(sizeof(sGobanExtraData)));

	eng->SetRemoteId(HIWORD(0));
	eng->m_nReferenceFromId=nCurrentGoId;
	eng->m_laststone=m_GoEnginez[nCurrentGoId]->m_laststone;
	eng->m_parent=m_GoEnginez[nCurrentGoId];
	eng->m_id=id;


	if (node==0)
	{
		node=m_GoEnginez[m_nCurrentGoId]->m_sgfMgr.m_pNodeTop;//m_node;
	}
	eng->AddNode(node);//, bIsTopNodeclear);
	eng->m_pLimitNode=from;

	if (from==0)
	{
		eng->Goto(MOVE_END,branches);
		eng->SetShowNo(0);
	}
	else
	{
		eng->PlayByNode(from);
	}

	{
		char buffer[128];
		wsprintf(buffer, "%s-%s%d", m_GoEnginez[m_nCurrentGoId]->GetTitle(), cyctrlGobanResource::GetInstance().GetStringA(eGTRID_REFERENCE), nIndex+1);
		eng->SetTitle(buffer);
	}

	m_GoEnginez[id]=eng;
	SearchResult(id);
	m_nGobanCount++;

	if (eng->GetExtraData())
	{
		sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();
		if (info->dlg)
		{
			info->dlg->Destroy();
			delete info->dlg;
			info->dlg=0;
		}

		memset(info, 0, sizeof(*info));
		//yifree(info);

		//eng->SetExtraData(0);
		//m_goban_detail[id]=0;
	}
	else
	{
		eng->SetExtraData(yialloci(sizeof(sGoPlayInfo)));
	}

	//ChangeCurrentGoban(id);

	{
		wchar_t buffer[128];
		char* pszTitle=(char*)eng->GetTitle();
		yiToUnicode(buffer, pszTitle, eCS_JAPANESE);

		sGlsTabItem item;
		cglsTabCtrl::xDefaultItemDetail(&item, cyiBitmapBlender::ColorShift(Color::YellowGreen, -32, -32, -32, -32));

		int index=m_tab->GetIndex(eng->m_nReferenceFromId);
		m_tab->InsertItem(index+1+nIndex, id, 0, buffer, from==0?True:False, &item);
	}
	
	//PostMessage(m_hOwner, WM_GOBAN_ADDED, 0, (LPARAM)id);

	return id;
}

// ------------------------------------------------------
Bool cglsGoban::GetPlayTime(goid_t goid, int* cur, int* cnt, Bool bIsBlack)
{
	goid=goid==0?m_nCurrentGoId:goid;
#ifndef NEW_STYLE
	if (bIsBlack)
	{
		*cur = m_playerBlack->xGetCurrentTime(goid, cnt);
	}
	else
	{
		*cur = m_playerWhite->xGetCurrentTime(goid, cnt);
	}
#endif
	return True;
}


// ------------------------------------------------------
ugoid_t* cglsGoban::GetUniversalGoid(goid_t id)
{
	cyiGoEngine* eng = m_GoEnginez[id==0?m_nCurrentGoId:id];

	if (eng==0)
		return 0;

	return &eng->m_ugoid;
}

// ------------------------------------------------------
goid_t cglsGoban::FindGoban(ugoid_t& id)
{
	for (int i=0; i<MAX_GOBAN_COUNT; i++)
	{
		 cyiGoEngine* eng = m_GoEnginez[i];

		 if (eng==0)
			 continue;

		 if (eng->m_ugoid==id)
		 {
			 return eng->m_id;//ExportSimpleGoData(eng->m_id, size);
		 }
	}
	return 0;
}

// ------------------------------------------------------
sSimpleGoData* cglsGoban::ExportSimpleGoData(ugoid_t& id, int* size)
{
	goid_t goid = FindGoban(id);

	if (goid==0)
		return 0;

	return ExportSimpleGoData(goid, size);
}


// ------------------------------------------------------
sSimpleGoData* cglsGoban::ExportSimpleGoData(unsigned int goid, int* size)
{
	cyiGoEngine* eng = m_GoEnginez[goid==0?m_nCurrentGoId:goid];

	if (eng==0)
		return 0;

	return eng->Export(size);
}

// ------------------------------------------------------
int cglsGoban::ImWin(goid_t goid)
{
	cyiGoEngine* eng = m_GoEnginez[goid==0?m_nCurrentGoId:goid];

	if (eng==0)
		return 0;

	sGoHeader* header=eng->GetHeader();

	int n = GET_WINER(header->flag32);

	if (n==3)
	{
		return 0;
	}

	if (n==0)
	{
		return 0xFF;
	}

	if (IAM_BLACK(header->condition))
	{
		if (n==1)
			return 1;
		return -1;
	}
	else
	{
		if (n==2)
			return 1;
		return -1;
	}

	return 0xFF;
}


// ------------------------------------------------------
int cglsGoban::DeleteGoban(goid_t id, Bool bIsFree)
{
	id=id==0?m_nCurrentGoId:id;

	if (bIsFree==True)
	{
		m_GoEnginez[id]->Destroy();
		delete m_GoEnginez[id];
		FreeId(id);
		m_nGobanCount--;
	}

	return 0;
}

// ------------------------------------------------------
const sGobanDetail* cglsGoban::GetGobanDetail()
{
	return &m_detail;
}

#ifndef NEW_STYLE
// ------------------------------------------------------
cglsDialog* cglsGoban::GetMyView(int index)
{
	if (index==0)
	{
		return m_myView;
	}

	if (index==1)
	{
		return m_myFriendView;
	}

	return 0;
}


// ------------------------------------------------------
Bool cglsGoban::UpdateMyView(int index, unsigned int userId, unsigned int groupId, int flag)
{
	if (m_userMgr==0)
		r1431qqqqqaqqaeturn False;

	if (m_myView==0)
	{
		xCreateMyViews(m_userMgr);
		xCreateMyButtons();
		return True;
	}

	if (index==0)
	{
		if (m_myView==0)
			return False;

		m_myView->UpdateMyView((eMyViewStyle)flag);
	}
	else if (index==1)
	{
		if (m_myFriendView==0)
			return False;

		m_myFriendView->UpdateFriends(userId, flag);
	}

	return True;

}


// ------------------------------------------------------
Bool cglsGoban::xCreateMyViews(CUserManager* mgr)
{
	float fFontSize = 22.0*m_scale;
	fFontSize=fFontSize>16.0?16.0:fFontSize;

	Font font(L"Meiryo UI", fFontSize);

	m_tab->AddItem(257, 0, L"マイページ");
	m_tab->SetCurSel(0, 257);
	m_myView = new cglsMyView();

	m_myView->Create(this, 0, 0, m_w, m_w, mgr, &font, mgr->GetMyId()?eMVS_NORMAL:eMVS_NOID);
	m_myView->Show();

	m_currentView=m_myView;

	m_myFriendView = new cglsMyFriendView();
	m_myFriendView->Create(this, 0, 0, m_w, m_w, mgr, &font);
	m_myFriendView->Hide();

	return True;
}

// ------------------------------------------------------
Bool cglsGoban::xCreateMyButtons()
{
	for (int i=xCHILD_ID_BTN_MYPAGE; i<=xCHILD_ID_BTN_DETAIL; i++)
	{
		cglsButton* btn = new cglsButton();
		btn->Create((HWND)this, i, 0, 0, 0, 0, L"マイページ", 0, this);
		btn->Update(eGUM_FORCED);
		btn->Hide();
	}

	return True;
}

#endif




// ------------------------------------------------------
Bool cglsGoban::CreateBitmap(sDrawItem* item, void* pExtra)
{
	Bitmap* bmp;

	for (int i=0; i<3; i++)
		if (item->pBmp[i])
			delete item->pBmp[i];

	if (item->id==xCHILD_ID_BTN_MYPAGE)
	{
		LinearGradientBrush brush(Point(0, 0), Point(0, GetFontHeight(True)<<1), Color::LightGreen, Color::White);
		bmp = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &brush, RectF(0, 0, GetFontHeight(True)<<1, GetFontHeight(True)<<1), 2.0);

		Bitmap* bmpGray = cyiBitmapBlender::GrayScale(bmp, 0.5);
		Bitmap* bmpShift = cyiBitmapBlender::ArgbShift(bmp, 0, 50, 50, 50);
		item->pBmp[0]=cyiBitmapBlender::DropShadowEx(bmpGray, 1, 1, 1.0, 0.3);
		item->pBmp[1]=cyiBitmapBlender::DropShadowEx(bmpShift, 1, 1, 1.0, 0.3);
		item->pBmp[2]=cyiBitmapBlender::DropShadowEx(bmp, 1, 1, 1.0, 0.3);

		delete bmp;
		delete bmpShift;
		delete bmpGray;
	}
	else if (item->id==xCHILD_ID_BTN_MYFRIEND)
	{
		LinearGradientBrush brush(Point(0, 0), Point(0, GetFontHeight(True)<<1), Color::LightBlue, Color::White);
		bmp = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &brush, RectF(0, 0, GetFontHeight(True)<<1, GetFontHeight(True)<<1), 2.0);

		Bitmap* bmpGray = cyiBitmapBlender::GrayScale(bmp, 0.5);
		Bitmap* bmpShift = cyiBitmapBlender::ArgbShift(bmp, 0, 50, 50, 50);
		item->pBmp[0]=cyiBitmapBlender::DropShadowEx(bmpGray, 1, 1, 1.0, 0.3);
		item->pBmp[1]=cyiBitmapBlender::DropShadowEx(bmpShift, 1, 1, 1.0, 0.3);
		item->pBmp[2]=cyiBitmapBlender::DropShadowEx(bmp, 1, 1, 1.0, 0.3);

		delete bmp;
		delete bmpShift;
		delete bmpGray;
	}
	else if (item->id==xCHILD_ID_BTN_MYFRIEND)
	{
		LinearGradientBrush brush(Point(0, 0), Point(0, GetFontHeight(True)<<1), Color::LightBlue, Color::White);
		bmp = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &brush, RectF(0, 0, GetFontHeight(True)<<1, GetFontHeight(True)<<1), 2.0);

		Bitmap* bmpGray = cyiBitmapBlender::GrayScale(bmp, 0.5);
		Bitmap* bmpShift = cyiBitmapBlender::ArgbShift(bmp, 0, 50, 50, 50);
		item->pBmp[0]=cyiBitmapBlender::DropShadowEx(bmpGray, 1, 1, 1.0, 0.3);
		item->pBmp[1]=cyiBitmapBlender::DropShadowEx(bmpShift, 1, 1, 1.0, 0.3);
		item->pBmp[2]=cyiBitmapBlender::DropShadowEx(bmp, 1, 1, 1.0, 0.3);

		delete bmp;
		delete bmpShift;
		delete bmpGray;
	}
	else if (item->id==xCHILD_ID_BTN_MYGROUP)
	{
		LinearGradientBrush brush(Point(0, 0), Point(0, GetFontHeight(True)<<1), Color::Purple, Color::White);
		bmp = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &brush, RectF(0, 0, GetFontHeight(True)<<1, GetFontHeight(True)<<1), 2.0);

		Bitmap* bmpGray = cyiBitmapBlender::GrayScale(bmp, 0.5);
		Bitmap* bmpShift = cyiBitmapBlender::ArgbShift(bmp, 0, 50, 50, 50);
		item->pBmp[0]=cyiBitmapBlender::DropShadowEx(bmpGray, 1, 1, 1.0, 0.3);
		item->pBmp[1]=cyiBitmapBlender::DropShadowEx(bmpShift, 1, 1, 1.0, 0.3);
		item->pBmp[2]=cyiBitmapBlender::DropShadowEx(bmp, 1, 1, 1.0, 0.3);

		delete bmp;
		delete bmpShift;
		delete bmpGray;
	}
	else if (item->id==xCHILD_ID_BTN_GROUP_DETAIL)
	{
		LinearGradientBrush brush(Point(0, 0), Point(0, GetFontHeight(True)<<1), Color::LightPink, Color::White);
		bmp = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &brush, RectF(0, 0, GetFontHeight(True)<<1, GetFontHeight(True)<<1), 2.0);

		Bitmap* bmpGray = cyiBitmapBlender::GrayScale(bmp, 0.5);
		Bitmap* bmpShift = cyiBitmapBlender::ArgbShift(bmp, 0, 50, 50, 50);
		item->pBmp[0]=cyiBitmapBlender::DropShadowEx(bmpGray, 1, 1, 1.0, 0.3);
		item->pBmp[1]=cyiBitmapBlender::DropShadowEx(bmpShift, 1, 1, 1.0, 0.3);
		item->pBmp[2]=cyiBitmapBlender::DropShadowEx(bmp, 1, 1, 1.0, 0.3);

		delete bmp;
		delete bmpShift;
		delete bmpGray;
	}
	else if (item->id==xCHILD_ID_BTN_DETAIL)
	{
		LinearGradientBrush brush(Point(0, 0), Point(0, GetFontHeight(True)<<1), Color::Beige, Color::White);
		bmp = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &brush, RectF(0, 0, GetFontHeight(True)<<1, GetFontHeight(True)<<1), 2.0);

		Bitmap* bmpGray = cyiBitmapBlender::GrayScale(bmp, 0.5);
		Bitmap* bmpShift = cyiBitmapBlender::ArgbShift(bmp, 0, 50, 50, 50);
		item->pBmp[0]=cyiBitmapBlender::DropShadowEx(bmpGray, 1, 1, 1.0, 0.3);
		item->pBmp[1]=cyiBitmapBlender::DropShadowEx(bmpShift, 1, 1, 1.0, 0.3);
		item->pBmp[2]=cyiBitmapBlender::DropShadowEx(bmp, 1, 1, 1.0, 0.3);

		delete bmp;
		delete bmpShift;
		delete bmpGray;
	}

	item->pCurrentBmp=item->pBmp[item->nLastItemIndex];
	item->rect.w=item->pBmp[0]->GetWidth();
	item->rect.h=item->pBmp[0]->GetHeight();

	return True;
}

// ------------------------------------------------------
Bool cglsGoban::xUpdateTab()
{
	int n;
	if ((n=m_tab->GetItemCount())==0)
	{
		m_tab->Update(eGUM_FORCED);
		return False;
	}

	int nSelectedId = m_tab->GetCurSel();
	cyiGoEngine* eng = m_GoEnginez[nSelectedId];

	if (eng==0)
		return False;
	
	// 全てのタブをHide状態にする。
	// ルートはShow状態
	for (int i=0; i<n; i++)
	{
		int xId = m_tab->GetId(i);

		if (xId==0)
			continue;
		if (xId==256)
			continue;
		if (xId==257)
			continue;
		if (m_GoEnginez[xId]==0)
		{
			m_tab->ShowItem(xId, True);
			continue;
		}
		if (nSelectedId==xId)
		{
			m_tab->ShowItem(xId, True);
			continue;
		}
		if (m_GoEnginez[xId]->m_parent==0)
		{
			m_tab->ShowItem(xId, True);
			continue;
		}

		if (m_GoEnginez[xId]->m_parent==eng)
		{
			m_tab->ShowItem(xId, True);
			continue;
		}

		m_tab->ShowItem(xId, False);
	}

	cyiGoEngine* parent=eng->m_parent;
	while(parent)
	{
		m_tab->ShowItem(parent->m_id, True);
		parent=parent->m_parent;
	}

	m_tab->Update(eGUM_FORCED);
	m_tab->InvalidateRect(0);

	return True;
}




// ------------------------------------------------------
int cglsGoban::xDeleteReference(goid_t id, Bool bIsFree)
{
	int n = 0;//m_tab->GetItemCount();
	int nDeleteCount=0;

	if (m_tab==0)
		return 0;
TOP:
	n=m_tab->GetItemCount();
	for (int i=0; i<n; i++)
	{
		int xId = m_tab->GetId(i);

		if (xId==0)
			continue;
		if (m_GoEnginez[xId]==0)
			continue;
		if (id==xId)
			continue;

		if (xId>(MAX_GOBAN_COUNT-1))
		{
			continue;
		}
		if (m_GoEnginez[xId]->m_nReferenceFromId==id)
		{
			int nSubDel;
			if ((nSubDel=xDeleteReference(xId, bIsFree)))
			{
				nDeleteCount+=nSubDel;
				goto TOP;
			}

			if (xId==m_nCurrentGoId)
			{
				m_tab->DeleteItem(i, -1, m_GoEnginez[xId]->m_nReferenceFromId, False);
			}
			else
			{
				m_tab->DeleteItem(i, -1, m_nCurrentGoId, False);
			}
			n--;
			i=0;

			if (bIsFree)
				DeleteGoban(xId, True);
			nDeleteCount++;
		}
	}

 	return nDeleteCount;
}

// ------------------------------------------------------
int cglsGoban::UnregisterNodeChangeNotifyWindow(HWND hWnd)
{
	if (m_listNodeChangeNotifyWnds==0)
	{
		return 0;
	}

	for (cyiNode* node = m_listNodeChangeNotifyWnds->GetNodeTopFrom(); node; node=node->np)
	{
		HWND _hWnd = (HWND)node->ref;

		if (hWnd==_hWnd)
		{
			m_listNodeChangeNotifyWnds->remove(node);
			break;
		}
	}

	return m_listNodeChangeNotifyWnds->count();
}


// ------------------------------------------------------
int cglsGoban::RegisterNodeChangeNotifyWindow(HWND hWnd)
{
	if (m_listNodeChangeNotifyWnds==0)
	{
		m_listNodeChangeNotifyWnds = new cyiList();
	}

	for (cyiNode* node = m_listNodeChangeNotifyWnds->GetNodeTopFrom(); node; node=node->np)
	{
		HWND _hWnd = (HWND)node->ref;

		if (hWnd==_hWnd)
		{
			// すでに登録されているので何もしない。
			return m_listNodeChangeNotifyWnds->count();
		}
	}

	m_listNodeChangeNotifyWnds->addref(hWnd);

	return m_listNodeChangeNotifyWnds->count();
}

// ------------------------------------------------------
// プレビューなどから
goid_t cglsGoban::AddByReference(const char* pszTitle, Bool bIsEditable)
{
	goid_t nCurrentId=m_nCurrentGoId;
	int nIndex=0;
	goid_t id=ReserveId();

	if (id==0)
		return 0;

	if (xGET_GOBAN_MODE(m_GoEnginez[nCurrentId]->m_flag)==eGCM_REFERENCE)
	{
		nCurrentId=m_GoEnginez[nCurrentId]->m_nReferenceFromId;
	}

	cyiTreeNode* node = m_GoEnginez[nCurrentId]->CreateReference(pszTitle, nIndex);

	if (node==0)
	{
		FreeId(id);
		return 0;
	}

	xAddReference(id, node, nIndex, nCurrentId);
	ChangeCurrentGoban(id,  True);
	m_tab->SetCurSel(0,id, False);
	//m_tab->Update(eGUM_FORCED);
	ShowNo(True);

	return id;
}

// ------------------------------------------------------
// 碁盤を追加
// SGFファイルを開くとき、対局開始時、などなど
// 失敗時は、ゼロ。
goid_t cglsGoban::Add(sGoHeader* header, goid_t reservedId, sGobanContentsDetail* detail, void* pExtraData, ugoid_t* uid, color_t color, Bool bHasButton)
{
	if (m_nGobanCount==MAX_GOBAN_COUNT)
	{
		return 0;
	}

	if (header->playDate==0)
	{
		cyiTime::GetLocalTime(header->playDate);
	}

	cyiGoEngine* eng = new cyiGoEngine();


	if (eng==NULL)
		return -1;
	
	goid_t id = LOWORD(reservedId) ? LOWORD(reservedId):ReserveId();

	if (id==0)
		return 0;

	eng->m_id=id;
	eng->Create(header, detail);

	if (pExtraData)
		eng->SetExtraData(pExtraData);
	else
		eng->SetExtraData(yialloci(sizeof(sGoPlayInfo)));

	if (uid!=0)
	{
		eng->m_ugoid=*uid;
	}

	//eng->SetInputMode(input);
	//eng->SetMouseMode(mouseMode);

	eng->SetRemoteId(HIWORD(reservedId));
	//eng->SetExtraData(yialloci(sizeof(sGobanExtraData)));

	m_GoEnginez[id]=eng;

	//if (m_goban_detail[id])
	//{
	//	if (m_goban_detail[id]->dlg)
	//	{
	//		m_goban_detail[id]->dlg->Destroy();
	//		delete m_goban_detail[id]->dlg;
	//	}
	//	yifree(m_goban_detail[id]);
	//}
	//m_goban_detail[id] = (sGobanExtraDetail* )yialloci(sizeof(sGobanExtraDetail));

	ChangeCurrentGoban(id);

	NotifyNodeChanged(True);

		//PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
	//Update(eGL_ALL, eng->GetStoneMap());
	//Update();

	m_nGobanCount++;
	eng->Goto(MOVE_TOP, 0);
#ifndef NEW_STYLE
	if (m_myView)
	{
		m_myView->Hide();
	}
#endif

	{
		wchar_t buffer[128];
		char* pszTitle=(char*)eng->GetTitle();
		yiToUnicode(buffer, pszTitle, eCS_JAPANESE);
		sGlsTabItem item;
		cglsTabCtrl::xDefaultItemDetail(&item, color==0?Color::YellowGreen:color);
		m_tab->AddItem(id, 0, buffer, bHasButton, &item);
		m_tab->SetCurSel(0, id, False);
		m_tab->Update(eGUM_FORCED);
	}
	PostMessage(m_hParentWnd, WM_GOBAN_ADDED, 0, (LPARAM)id);
	return LOWORD(id);
}

// -----------------------------------------
// ロケール変更により、再描画が行われる。
void cglsGoban::ChangeLocale(goid_t id, eCharacterSet eLocale/*=eCS_JAPANESE*/)
{
	m_GoEnginez[id]->SetLocale(eLocale);
	Update();

	// TODO: 
	// UpdateControl();
	// UpdateViewer();
	::PostMessage(m_hParentWnd, WM_LOCALE_CHANGED, (WPARAM)id, (LPARAM)eLocale);
}

// ------------------------------------------------------
// 碁盤削除
void cglsGoban::Remove(goid_t id)
{
	if (m_GoEnginez[id]!=0 &&
			m_GoEnginez[id]!=GO_ID_RESERVED)
	{
		m_nGobanCount--;
		delete m_GoEnginez[id];
	}

	m_GoEnginez[id]=0;
	return ;
}

// ------------------------------------------------------
unsigned int cglsGoban::GetCount()
{
	return m_nGobanCount;
}

// -----------------------------------------
goid_t cglsGoban::GetCurrentGoId()
{
	return m_nCurrentGoId;
}

// -----------------------------------------
void* cglsGoban::GetExtraData(goid_t id)
{
	id = id?id:m_nCurrentGoId;

	if (m_GoEnginez[id])
		return m_GoEnginez[id]->GetExtraData();

	return 0;
}

// -----------------------------------------
Bool cglsGoban::IsCurrentBlack(goid_t id)
{
	id = id?id:m_nCurrentGoId;
	if (m_GoEnginez[id])
	{
		return m_GoEnginez[m_nCurrentGoId]->IsCurrentOrderBlack();//IsCurrentBlack();
	}
	return False;
}

// -----------------------------------------
Bool cglsGoban::ImBlack(goid_t id)
{
	id = id?id:m_nCurrentGoId;
	if (m_GoEnginez[id])
	{
		return IAM_BLACK(m_GoEnginez[m_nCurrentGoId]->GetHeader()->condition);
	}

	return False;
}


// ------------------------------------------------------
void cglsGoban::ShowNo(Bool bIsTrue, int nFrom/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return ;

	int n = eng->GetShowNo();

	// この場合は隠す
	if (n!=-1)
	{
		eng->SetShowNo(-1);
		Update(eGL_ALL, eng->GetStoneMap());
		return ;
	}

	n = bIsTrue ? 0 : -1;

	n = n==-1? -1:nFrom;
	eng->SetShowNo(n);
	Update(eGL_ALL, eng->GetStoneMap());
}
#include "yiUtil.h"
extern stone_t xChangeStoneCoordinate(stone_t stone, int nLineCnt);

// ------------------------------------------------------
void cglsGoban::SetSecondReadCount(goid_t id, Bool bIsOrderBlack, int nCount)
{
#ifndef NEW_STYLE
	if (bIsOrderBlack)
	{
		m_playerBlack->SetSecondReadCount(id, nCount);
	}
	else
	{
		m_playerWhite->SetSecondReadCount(id, nCount);
	}
#endif
	return ;
}

#ifdef NEW_STYLE
Bool cglsGoban::GetEffectedBg(Bitmap* dest, int x, int y, int sx, int sy, int w, int h)
{
	if (m_bluredBg==0)
		return False;

	yiMutexLockEx(m_hDrawMutex);
	cyiBitmapBlender::BitmapCopy(dest, m_bluredBg, x, y, sx, sy, w, h);
	yiMutexUnlockEx(m_hDrawMutex);

	return True;
}
#endif

// ------------------------------------------------------
void cglsGoban::SetSecondRead(goid_t goid, int n)
{
	goid = goid==0?m_nCurrentGoId:goid;

	if (goid!=m_nCurrentGoId)
		return ;

	if (IsWindowVisible(*m_cursor)==False)
		return ;

	if (n<10 &&
			n>5)
	{
		m_cursor->SelectBitmap(ImBlack(goid)==True?1:2);
		return ;
	}

	int id = m_cursor->GetSelectedBitmap();
	Bitmap* bmp = m_cursor->GetBitmap(ImBlack(goid)==True?1:2);
	Bitmap* bmpSec=cyiBitmapBlender::BitmapCopy(0, bmp);
	
	wchar_t szNo[5];
	wsprintfW(szNo, L"%d", n);
	cyiShape::BitmapString(bmpSec, szNo, cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL), &SolidBrush(Color::Red));

	m_cursor->SetBitmap(8, bmpSec, True);
	m_cursor->SelectBitmap(8, False, True);

}


// ------------------------------------------------------
int cglsGoban::GetSecondReadCount(goid_t id, Bool bIsOrderBlack)
{
#ifndef NEW_STYLE
	if (bIsOrderBlack)
	{
		return m_playerBlack->GetSecondReadCount(id);//, nCount);
	}
	return	m_playerWhite->GetSecondReadCount(id);//, nCount);
#endif
	return 0;
}

// ------------------------------------------------------
Bool cglsGoban::SetDeadStone(int nBlack, int nWhite)
{
#ifndef NEW_STYLE
	if (m_playerBlack)
		m_playerBlack->SetDeadStone(nWhite);
	if (m_playerWhite)
		m_playerWhite->SetDeadStone(nBlack);
#endif
	return True;
}


// ------------------------------------------------------
Bool cglsGoban::ChangeTimer(goid_t id, Bool bIsOrderBlack, unsigned int usedTime, sGoPlayInfo* info, Bool bIsLastRemainTime)
{
	id=id?id:m_nCurrentGoId;

	if (info==0)
	{
		if (m_GoEnginez[id]!=0)
		{
			info = (sGoPlayInfo*)m_GoEnginez[id]->GetExtraData();
		}
	}

	if (info==0)
		return False;

#ifndef NEW_STYLE

	if (bIsOrderBlack==True)
	{
		info->black.startTime=cyiTime::CurrentmSec();
		
		if (bIsLastRemainTime==True)
		{
			info->white.usedTime=0;
		}
		else
		{
			info->white.usedTime=cyiTime::CurrentmSec(info->white.startTime);
		}

		info->white.totalUsedTime+=usedTime?usedTime:info->white.usedTime;//(bIsPass?info->white.usedTime:usedTime);

		m_playerWhite->StopTimer(id, info->white.totalUsedTime);
		m_playerBlack->StartTimer(id, bIsLastRemainTime);
	}
	else
	{
		info->white.startTime=cyiTime::CurrentmSec();
		if (bIsLastRemainTime==True)
		{
			info->black.usedTime=0;
		}
		else
		{
			info->black.usedTime=cyiTime::CurrentmSec(info->black.startTime);
		}
		//info->black.usedTime=cyiTime::CurrentmSec(info->white.startTime);

		info->black.totalUsedTime+=usedTime?usedTime:info->black.usedTime;//(bIsPass?info->black.usedTime:usedTime);//info->black.usedTime;
		m_playerBlack->StopTimer(id, info->black.totalUsedTime);
		m_playerWhite->StartTimer(id, bIsLastRemainTime);

	}
#endif
	return True;
}

// -------------------------------
stone_t** cglsGoban::PutStone(ugoid_t& uid, stone_t stone, unsigned int usedMilliSec)
{
	for (int i=0; i<MAX_GOBAN_COUNT; i++)
	{
		cyiGoEngine* eng = m_GoEnginez[i];
		if (eng==0)
			continue;

		if (eng->m_ugoid==uid)
		{
			if (eng->GetInputMode()==eIM_PLAY)
				continue;

			return PutStone(eng->m_id, stone, usedMilliSec);
		}
	}

	return 0;
}

// -------------------------------
// 石を置く
// ここはネットワークを通じてしか呼び出されない？
stone_t** cglsGoban::PutStone(goid_t id, stone_t stone, unsigned int usedTime, Bool bIsFromNetwork)
{
	xTRACE("cglsGoban::PutStone called %x, %d", stone, usedTime);
	id=id?id:m_nCurrentGoId;
	cyiGoEngine* eng = m_GoEnginez[id];

	Bool bIsQuit=False;
	Bool bIsPass=False;


	if (!eng)
		return 0;

	//if (eng->GetInputMode()!=eIM_PLAY)
	//{
	//	return 0;
	//}

	if (bIsFromNetwork==True &&
			eng->GetInputMode()==eIM_PLAY)
	{
		if (eng->IsCurrentOrderMine()==True)
		{
			return 0; // 無効(ハガシなどによるすりちがい）
		}
	}

	if (stone==eST_PASS)
	{
		stone_t x = eng->GetLastStone();
		eStoneType type  = STONE_TYPE(x);

		if (type==eST_BLACK ||
				type==eST_BPASS)
		{
			stone=MAKESTONE(eST_WPASS, 0, 0, 0);
		}
		else if (type==eST_WHITE ||
					type==eST_WPASS)
		{
			stone=MAKESTONE(eST_BPASS, 0, 0, 0);
		}
		else
			return 0;

		// 2連続パス
		if (type==eST_BPASS ||
				type==eST_WPASS)
		{
			bIsQuit=True;
		}
		bIsPass=True;
		yiBeep(MB_ICONASTERISK);
	}

	aStoneList list;
	sGoPlayInfo* info = (sGoPlayInfo* )eng->GetExtraData();

	if (info!=0 && eng->GetInputMode()==eIM_CALCULATE)
	{
		
		SelectStone(eng, STONE_POS_X(stone), STONE_POS_Y(stone));
		return 0;
	}

	stone_t** stonez = eng->PutStone(stone, list);

	int nBlack=0, nWhite=0;
	eng->GetDeadStone(nBlack, nWhite);

	SetDeadStone(nWhite, nBlack);



	if (eng->GetInputMode()==eIM_STUDY)
	{
		unsigned int con = eng->GetHeader()->condition;

		if (IAM_BLACK(con)==True)
		{
			stone_t send_stone = xChangeStoneCoordinate(stone, eng->GetLineCount());
			GetSystem()->GetCommandCenter()->OnCommand(m_nSendStoneId ,this, send_stone, (unsigned int)eng);
		}
	}

	if (eng->GetInputMode()!=eIM_PLAY &&
			eng->GetInputMode()!=eIM_WATCH)
	{
		goto SKIP_TIMER;
	}

	if (stonez!=0)
	{
		if (info!=0)
		{
			ChangeTimer(eng->m_id, eng->IsCurrentOrderBlack(), usedTime, info);

			
			//if (eng->IsCurrentOrderBlack()==True)
			//{
			//	info->black.startTime = cyiTime::CurrentmSec();
			//	info->white.totalUsedTime+=usedTime;
			//	//if (id==m_nCurrentGoId)
			//	//{
			//		m_playerWhite->StopTimer(eng->m_id, info->white.totalUsedTime);
			//		m_playerBlack->StartTimer(eng->m_id);
			//	//}
			//}
			//else
			//{
			//	info->white.startTime = cyiTime::CurrentmSec();
			//	info->black.totalUsedTime+=usedTime;

			//	//if (id==m_nCurrentGoId)
			//	//{
			//		m_playerBlack->StopTimer(eng->m_id, info->black.totalUsedTime);
			//		m_playerWhite->StartTimer(eng->m_id);
			//	//}
			//}
		}

		if (bIsPass==False)
		{
			::PlaySound((LPCSTR)m_soundMove, m_hInstance, SND_ASYNC|SND_MEMORY);
		}
		//GetSystem()->EnableCommand(eGCID_PASS, True);

	}


SKIP_TIMER:
	if (id==m_nCurrentGoId)
	{
		// あげ石がある場合、通知する。
		if (stone!=0 && 
			list[0]!=STONE_EOF)
		{
			int nBlack, nWhite;
			eng->GetDeadStone(nBlack, nWhite);
			PostMessage(m_hParentWnd, WM_CHANGE_DEAD_STONE, id, MAKELONG(nBlack, nWhite));
		}

		//SetBranch(eng);

		// 描画する。

		if (stonez!=0)
			Update(eGL_STONE, stonez);
		NotifyNodeChanged(True);
	}

	// --------------------------------

	if (bIsPass==True)
	{
		if (eng->GetInputMode()==eIM_PLAY)
		{
			if (bIsFromNetwork==False)
			{
				if (m_nSendStoneId==0 ||
						GetSystem()==0)
				{
					PostMessage(m_hParentWnd, WM_PUTSTONE, MAKELONG(m_nCurrentGoId, eng->GetRemoteId()), eST_PASS);
				}
				else
				{
					GetSystem()->GetCommandCenter()->OnCommand(m_nSendStoneId ,this, eST_PASS, (unsigned int)eng);
				}
			}
		}

	}

	if (eng->GetInputMode()==eIM_PLAY &&
			bIsQuit==True)
	{
		//if (m_nSendStoneId==0 ||
		//		GetSystem()==0)
		//{
		//	PostMessage(m_hParentWnd, WM_PUTSTONE, MAKELONG(m_nCurrentGoId, eng->GetRemoteId()), stone);
		//}
		//else
		//{
		//	GetSystem()->GetCommandCenter()->OnCommand(m_nSendStoneId ,this, stone, (unsigned int)eng);
		//}

		StartCalculateMode(0);
		ShowMessage(0, L"2回連続でパスが発生したために終局とします。\r\n相手のあげいしを指定してください\r\n", L"", MB_OK);

	}

	if (eng->GetInputMode()==eIM_PLAY)
	{
#ifndef NEW_STYLE
		m_currentCtrl->EnableItem(eGCS_PASS ,bIsFromNetwork);
		m_currentCtrl->EnableItem(eGCS_GIVEUP,bIsFromNetwork);
		m_currentCtrl->EnableItem(eGCS_PAUSE, bIsFromNetwork);
#endif

	}


	return stonez;
}

// -------------------------------
void cglsGoban::PlayByNode(cyiTreeNode* node)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return ;

	stone_t** stone = eng->PlayByNode(node);

	if (stone==0)
		return ;

	xDeleteReference(m_nCurrentGoId, True);//False);

	::PlaySound((LPCSTR)m_soundMove, m_hInstance, SND_ASYNC|SND_MEMORY);

	int nBlack, nWhite;
	eng->GetDeadStone(nBlack, nWhite);
	PostMessage(m_hParentWnd, WM_CHANGE_DEAD_STONE, m_nCurrentGoId, MAKELONG(nBlack, nWhite));


	Update(eGL_ALL, stone);

	NotifyNodeChanged();

	cyiList* reference = eng->GetReference();
	int cnt=0;
	if (reference)
	{
		for (cyiNode* node=reference->GetNodeTopFrom(); node; node=node->np)
		{
			cyiTreeNode* t = (cyiTreeNode*)node->ref;

			xAddReference(0, t, cnt, m_nCurrentGoId);

			cnt++;
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
}

// --------------------------------------
cyiTreeNode* cglsGoban::GetCurrentNode()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return False;

	return eng->GetCurrentNode();
}

// -------------------------------
// ラベルを置く
Bool cglsGoban::PutLabel(goid_t id, int i, int j,wchar_t label)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (!eng)
		return False;

	if (eng->PutLabel(i, j, label))
	{
		Update(eGL_LABEL);
		return True;
	}

	return False;
}

// -------------------------------
// ラベルリストを取得
cyiList* cglsGoban::GetLabel(goid_t id)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (!eng)
		return 0;


	return eng->GetLabel();
}

// -------------------------------
// コメントを入力
Bool cglsGoban::PutComment(goid_t id, LPCSTR comment)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (!eng)
		return False;

	return eng->PutComment(comment);
}

// -------------------------------
// コメントを取得
LPCSTR cglsGoban::GetComment(goid_t id, LPCSTR comment)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (!eng)
		return 0;

	return (const char*)eng->GetComment();
}

// --------------------------------
stone_t** cglsGoban::Goto(goid_t id, int nIndex, aStoneList branches, Bool bIsFromNetwork)//, eInputMode mode/*=eIM_DEFAULT*/)
{
	cyiGoEngine* eng = m_GoEnginez[(id=id?id:m_nCurrentGoId)];

	if (!eng)
		return 0;

	if (eng->GetInputMode()==eIM_STUDY)
	{
		sGoHeader* header = eng->GetHeader();
		if (IAM_BLACK(header->condition)==False)
		{
			if (bIsFromNetwork==False)
			{
				return (stone_t**)0xFFFFFFFF;
			}
		}
	}

	m_branches.clear();

	stone_t** stone=eng->Goto(nIndex, branches);

	if (stone==0)
	{
		if ((nIndex==MOVE_TOP||nIndex==-1) &&
				eng->m_parent!=0)
		{
			ChangeCurrentGoban(eng->m_parent->m_id);
			return Goto(0, -1, branches);
		}
		return 0;
	}



	xDeleteReference(id, True);//False);

	if (id==m_nCurrentGoId)
	{
		//SetBranch(eng);
		if (stone!=0)
		{
			Update(eGL_ALL, stone);
		}

		NotifyNodeChanged();

		//PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());

		cyiList* reference =eng->GetReference();

		int cnt=0;
		if (reference)
		{
			for (cyiNode* node=reference->GetNodeTopFrom(); node; node=node->np)
			{
				cyiTreeNode* t = (cyiTreeNode*)node->ref;

				xAddReference(0, t, cnt, id);

				cnt++;
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

		//cyiList hide_branches;
		//int n=eng->GetHideBranches(hide_branches);

		//if (n!=0)
		//{
		//	for (cyiNode* node=hide_branches.GetNodeTopFrom(); node; node=node->np)
		//	{
		//		cyiTreeNode*t = (cyiTreeNode*)node->ref;
		//		id = xAddReference(0, t, cnt, id, False);
		//		cnt++;
		//	}
		//}

		int nBlack=0, nWhite=0;
		eng->GetDeadStone(nBlack, nWhite);
		SetDeadStone(nWhite, nBlack);

		if (eng->GetInputMode()==eIM_STUDY)
		{
			sGoHeader* header = eng->GetHeader();
			if (IAM_BLACK(header->condition)==True)
			{
				//if (bIsFromNetwork==True)
				//{
				return (stone_t**)0xFFFFFFFF;
				//}
			}
			//else
			//{
			//	if (bIsFromNetwork==True)
			//	{
			//		return stone;
			//	}
			//}
		}

		return stone;
	}

	if (eng->GetInputMode()==eIM_STUDY)
	{
		sGoHeader* header = eng->GetHeader();
		if (IAM_BLACK(header->condition)==True)
		{
			return (stone_t**)0xFFFFFFFF;
		}
		else
		{
			if (bIsFromNetwork==True)
			{
				return stone;
			}
		}
	}

	//if (stone!=0)
	//{
	//	Update(eGL_ALL, stone);
	//}
	int nBlack=0, nWhite=0;
	eng->GetDeadStone(nBlack, nWhite);

	SetDeadStone(nBlack, nWhite);
	return stone;
}


// --------------------------------
Bool cglsGoban::StartFindComment()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return 0;

	if (eng->GetInputMode()!=eIM_EDIT)
	{
		return False;
	}

	if (eng->FindCommentList()==0)
		return False;

	return True;
}

// --------------------------------
Bool cglsGoban::EndFindComment()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return 0;

	eng->CloseCommentList();
	return True;
}

// --------------------------------
stone_t** cglsGoban::GotoNextComment(int nIndex/*=-1*/)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return 0;

	stone_t** stone= eng->GotoNextComment(nIndex);

	Update(eGL_ALL, stone);

	NotifyNodeChanged();



	return stone;
}

// --------------------------------
stone_t** cglsGoban::GotoPrevComment()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return 0;

	stone_t** stone = eng->GotoPrevComment();

	Update(eGL_ALL, stone);

	if (xIS_BIT(eng->m_flag, eGEF_NOW_SEARCH_RESULT)==False)
		PostMessage(m_hParentWnd, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());


	return stone;
}




// --------------------------------
stone_t** cglsGoban::GotoNextBranch(goid_t id, aStoneList branches)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (!eng)
		return 0;

	branches[0]=STONE_EOF;
	stone_t** stone = eng->GotoNextBranch(branches);

	if (branches[0]!=STONE_EOF)
	{
		for (int i=0; branches[i]!=STONE_EOF; i++)
		{
			m_branches.push((yiAnyValue)branches[i]);
		}
	}

	if (stone)
	{
		Update(eGL_ALL, stone);
		NotifyNodeChanged();


	}

	return stone;
}

// --------------------------------
stone_t** cglsGoban::GotoPrevBranch(goid_t id, aStoneList branches)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (!eng)
		return 0;

	stone_t** stone = eng->GotoPrevBranch(branches);

	if (branches[0]!=STONE_EOF)
	{
		for (int i=0; branches[i]!=STONE_EOF; i++)
		{
			m_branches.push((yiAnyValue)branches[i]);
		}
	}


	if (stone)
	{
		Update(eGL_ALL, stone);
		NotifyNodeChanged();


	}

	return stone;


}

// --------------------------------
void cglsGoban::FreeInitStoneMode(int count)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return ;

	if (eng->GetLastStone()==eST_INVALID)
		eng->StartInitStone(count);
}

// ---------------------------------------------------------------
Bool cglsGoban::xDeleteTab(int id, Bool bIsDeleteSelf)
{
	cyiGoEngine* eng = m_GoEnginez[id];
	int n=m_tab->GetItemCount();
	for (int i=0; i<n; i++)
	{
		int xId = m_tab->GetId(i);

		if (xId>MAX_GOBAN_COUNT)
			continue;
		cyiGoEngine* xeng=m_GoEnginez[xId];
		if (xeng->m_nReferenceFromId==id)
		{
			if (xDeleteTab(xId, True)==True)
			{
				i=0;
				n=m_tab->GetItemCount();
			}
		}
	}

	if (bIsDeleteSelf)
	{
		m_tab->DeleteItem(0, id, eng->m_nReferenceFromId, False);
		DeleteGoban(id, True);
		//ChangeCurrentGoban(1, False);

		return True;
	}

	return False;
}

// ------------------------------------------------------
void cglsGoban::DeleteNode(goid_t goid, cyiTreeNode* node, Bool bIsCurrentOnly)
{
	int nReferenceFromId;
	cyiGoEngine* eng = m_GoEnginez[goid?goid:m_nCurrentGoId];

	
	if (!eng)
		return ;

	if (bIsCurrentOnly==False)
	{
		Bool bRequestDeleteTab=False;
		int id=eng->m_id;
		cSgfStoneInfo* info= (cSgfStoneInfo*)eng->GetCurrentNode()->GetData();
		cyiTreeNode* nodeParent=node? node->GetParent():eng->GetCurrentNode()->GetParent();
		
		if (eng->m_nReferenceFromId!=0 &&
				(info && (info->referenceFrom!=0 ||
							info->m_AddStoneList!=0)))
		{
			bRequestDeleteTab=True;
			nReferenceFromId=eng->m_nReferenceFromId;
		}
				
		eng->DeleteNode();

		if (xDeleteTab(eng->m_id, bRequestDeleteTab)==True)
		{
			ChangeCurrentGoban(nReferenceFromId, False);
			eng=m_GoEnginez[m_nCurrentGoId];
		}

		SetChanged(m_nCurrentGoId, True);

		/*int n=m_tab->GetItemCount();
		for (int i=0; i<n; i++)
		{
			int xId = m_tab->GetId(i);
			cyiGoEngine* xeng=m_GoEnginez[xId];
			if (xeng->m_nReferenceFromId==id)
			{
				int stack=m_nCurrentGoId;
				m_nCurrentGoId=xId;
				DeleteNode(xeng->m_pLimitNode);
				m_nCurrentGoId=stack;
				//m_tab->DeleteItem(i, -1, xeng->m_nReferenceFromId);
				//n--;
				continue;
			}

			if (bRequestDeleteTab==True && 
					xId==id)
			{
				m_tab->DeleteItem(i, -1, eng->m_nReferenceFromId);
				DeleteGoban(id, True);
				ChangeCurrentGoban(1, False);
				eng=m_GoEnginez[m_nCurrentGoId];
				n--;
				i--;
				continue;
				//break;
			}
		}
		*/

	}
	else
	{
		cyiTreeNode* node = eng->GetCurrentNode();
		if (node->GetParent()==0)
			return ;

		aStoneList list;
		Goto(this->m_nCurrentGoId, -1, list);

		for (cyiNode* x=node->GetParent()->GetChildList().GetNodeTopFrom(); x; x=x->np)
		{
			cyiTreeNode* xNode=(cyiTreeNode*)x->data;

			if (node==xNode)
			{
				node->GetParent()->GetChildList().remove(x);
				break;
			}
		}

		for (cyiNode*x = node->GetChildList().GetNodeTopFrom(); x; x=x->np)
		{
			cyiTreeNode* xNode=(cyiTreeNode*)x->data;

			node->GetParent()->add(xNode);
		}
		eng->m_sgfMgr.m_currentNode=node->GetParent();

		cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

		//if (info)
		//	delete info;
		//&delete node;
		SetChanged(m_nCurrentGoId, True);

		return ;

	}

	// この場合は、PostMessageではない。
	// 緊急
	//if (m_preview && IsWindowVisible(*m_preview))
	//	SendMessage(*m_preview, WM_NODE_CHANGED, 1, (LPARAM)eng->GetCurrentNode());

	NotifyNodeChanged(True, True);
	/*
	if (node==0)
	{
		node = eng->GetCurrentNode();
	}

	xDeleteNode(node);
	xRemoveFromParent(node);

	aStoneList list;
	stone_t** stonez = eng->Goto(MOVE_TOP, list);*/
	Update(eGL_ALL, eng->GetStoneMap() );
}


// ------------------------------------------------------
void cglsGoban::DeleteNode(cyiTreeNode* node, Bool bIsCurrentOnly)
{
	DeleteNode(m_nCurrentGoId, node, bIsCurrentOnly);
}

// ------------------------------------------------------
void cglsGoban::NotifyNodeChanged(Bool bIsForced, Bool bIsNotUseQueue)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
		return ;

	if (bIsForced==True && bIsNotUseQueue==True)
	{
		goto FORCED;
	}

	if (bIsForced==False)
	{
		if (xIS_BIT(eng->m_flag, eGEF_NOW_SEARCH_RESULT)!=False)
			return ;
	}

	bIsForced=True;
FORCED:
	// タブの切り替えなど
	if (bIsForced==True)
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
						if (bIsNotUseQueue==True)
						{
							SendMessage(hWnd, WM_NODE_CHANGED, 1, (LPARAM)eng->GetCurrentNode());
						}
						else 
						{
							PostMessage(hWnd, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
						}
					}
				}
			}
		}
		return ;
	}

	m_node_for_timer=GetCurrentNode();
	SetTimer(m_hWnd, 1, 500, 0);

}

// ---------------------------------------------------------------
LRESULT cglsGoban::OnSearchResultEnd(WPARAM wp, LPARAM lp, Bool& bIsHandled)
{
	NotifyNodeChanged();
	return 0;
}

// ------------------------------------------------------
void cglsGoban::GetControlPos(int&x, int&y)
{
	x = m_tab->m_x;//+m_tab->m_w;
	y = m_w;

}

#ifndef NEW_STYLE
#include "glsGobanControl.h"
// ------------------------------------------------------
cglsGobanControl* cglsGoban::GetGobanControl(eInputMode mode)
{
	int x, y;
	cglsGobanControl* ctrl=0;
	sGlsGobanControlDetail detail;

	memset(&detail, 0, sizeof(detail));

	detail.font=cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON);

	GetControlPos(x, y);

	if (m_currentCtrl==0)
	{
		m_currentCtrl = new cglsGobanControl();

		
		m_currentCtrl->Create((HWND)m_parent, xCHILD_ID_CTRL_FOR_EDIT, m_scale, x, y, eGCS_NORMAL_EDITOR, &detail);
	}

	if (mode==eIM_PLAY||
		mode==eIM_WAIT_USER)
	{
		m_currentCtrl->ChangeControlSet(eGCS_PLAY);

		cyiGoEngine* eng = GetCurrentGoEngine();
		sGoHeader* header = eng->GetHeader();

		Bool on=False;
		on = GET_HUU_COUNT(header->condition2);
		m_currentCtrl->ShowItem(on, eGCS_HUU);

		on = IS_PLAYPAUSE(header->condition2);
		m_currentCtrl->ShowItem(on, eGCS_PAUSE);

		on = GET_WAITABLE(header->condition2);
		m_currentCtrl->ShowItem(on, eGCS_WAIT);
	//	m_currentCtrl->EnableItem(eGCS_WAIT, on);

	}
	else if (mode==eIM_STUDY)
	{
		m_currentCtrl->ChangeControlSet(eGCS_STUDY);
	}
	else if (mode==eIM_ENJOY)
	{
		m_currentCtrl->ChangeControlSet(eGCS_NORMAL_EDITOR);//eGCS_SIMPLE);
	}
	else if (mode==eIM_WATCH)
	{
		m_currentCtrl->ChangeControlSet(eGCS_WATCH);
	}
	else
	{
#ifdef _TIMER_DEBUG_
		m_currentCtrl->ChangeControlSet(eGCS_PLAY);//eGCS_NORMAL_EDITOR);
#else
		m_currentCtrl->ChangeControlSet(eGCS_NORMAL_EDITOR);
#endif
	}
	ctrl=m_currentCtrl;

	ctrl->SetNotifyWindow(m_hWnd);
	ctrl->Update(eGUM_SKIP_REDRAW);
	return ctrl;
}
#endif

#ifdef 	NEW_STYLE
void cglsGoban::UpdateGobanControl(cyiGoEngine* eng)
{
}

void cglsGoban::xResizeGobanControl()
{
	int n = MAX_BLOCK_SIZE*m_scale;//*0.80;
	Font* font = cyiShape::GetFontByHeight(n, L"Meiryo");

	if (m_controlz[eGCT_TITLE])
	{
		m_controlz[eGCT_TITLE]->SetFont(font);
		m_controlz[eGCT_TITLE]->ReSize(m_w-4, n);
		m_controlz[eGCT_TITLE]->Update(eGUM_FORCED);
	}

	if (m_controlz[eGCT_CONTROL])
	{
		m_controlz[eGCT_CONTROL]->SetFont(font);
		m_controlz[eGCT_CONTROL]->ReSize(m_w-4, (int)((float)n*1.3333));
		m_controlz[eGCT_CONTROL]->Update(eGUM_FORCED);
	}
}

#endif

#ifndef NEW_STYLE
// ------------------------------------------------------
void cglsGoban::xResetGobanControl(cyiGoEngine* eng)
{
	cglsGobanControl* ctrl=0;

	if (m_bIsPreviewMode==True)
		return ;

	//if (m_detail.bIsJosekiEditorMode==True)
	//	return ;//bIsJosekiEditorMode

	if (m_detail.bIsOldInterface==True)
	{
		return ;
	}


	// MyPage.
	if (m_tab->GetCurSel()==257)
	{
		if (m_currentCtrl)
			m_currentCtrl->Hide();

		if (m_titleDlg)
			m_titleDlg->Hide();

		if (m_infoDlg)
			m_infoDlg->Hide();

		if (m_playerBlack)
			m_playerBlack->Hide();
		
		if (m_playerWhite)
			m_playerWhite->Hide();

		if (m_textPannel)
			m_textPannel->Hide();

		//if (m_nSelectedView==xCHILD_ID_BTN_MYPAGE)
		//{
		m_currentView->Show();
		m_currentView->InvalidateRect(0, 0, True);
			//m_myView->Show();
		
		//}

		int n = xCHILD_ID_BTN_DETAIL-xCHILD_ID_BTN_MYPAGE;
		int y=m_w-(GetFontHeight(True)<<1)-GetFontHeight(True);
		int x=(m_w-((GetFontHeight(True)<<1)*n+(GetFontHeight(True)>>1)*(n-1)))>>1;
		for (int i=xCHILD_ID_BTN_MYPAGE; i<=xCHILD_ID_BTN_DETAIL; i++)
		{
			GetChild(i)->Show();
			GetChild(i)->Move(x, y);
			x+=(GetFontHeight(True)<<1)+(GetFontHeight(True)>>1);
		}
		//m_myFriendView->Show();
		//m_myFriendView->InvalidateRect(0);

		//int nControlWidth=this->m_currentCtrl->m_w;

		//if (m_currentCtrl<m_w/3)
		//{
		//	nControlWidth=m_w/3;
		//}
		
		if (m_GoEnginez[m_nCurrentGoId])
		{
			cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
			sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();

			if (info ->dlg)
			{
				if (info->dlg->IsCreated()==True)
				{
					info->dlg->Hide();
				}
				else
				{
					delete info->dlg;
					info->dlg=0;
				}
			}
		}
		return ;
	}



	if (m_currentView!=0)
	{
		m_currentView->Hide();
		for (int i=xCHILD_ID_BTN_MYPAGE; i<=xCHILD_ID_BTN_DETAIL; i++)
		{
			GetChild(i)->Hide();
		}
	}

	if (eng==0)
		return ;

	if (eng->GetExtraData())
	{
		sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();
		if (info->dlg)
		{
			if (info->dlg->IsCreated()==True)
			{
				info->dlg->Show();
			}
			else
			{
				delete info->dlg;
				info->dlg=0;
			}
		}
	}

	eInputMode mode=eng->GetInputMode();

	ctrl=GetGobanControl(mode);

	int xOffset=this->m_container->m_x+m_container->m_w+5;//m_tab->m_x+m_tab->GetMeasureHeight()+5;
	int yOffset=0;
	int controlWidth;//=ctrl->m_h;


	if (ctrl)
	{
		if (IsWindowVisible(*ctrl)==False)
			ctrl->Show();
	}

	//if (m_currentCtrl!=ctrl)
	//{
	//	if (m_currentCtrl)
	//	{
	//		m_currentCtrl->Hide();
	//		m_currentCtrl->InvalidateRect(0);
	//	}

		m_currentCtrl=ctrl;
		m_currentCtrl->Move(xOffset, m_w-m_currentCtrl->m_h);
		m_currentCtrl->Show();
		m_currentCtrl->InvalidateRect(0);
	//}

	controlWidth=GetFontHeight(True)*18;//m_w/2;//(m_currentCtrl->m_w<m_w/2)?m_w/2:m_currentCtrl->m_w;

	if (m_titleDlg==0)
	{
		int w=controlWidth;//GetWidth();//m_w/2;
		int h=0;//cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON)->GetHeight(__DEFAULTGRAPHICS__);
		wchar_t szTitle[128];

		h=(h<<1);
		yiToUnicode(szTitle, eng->GetTitle(), 0);

		m_titleDlg = new cglsTitlePannel();
		m_titleDlg->Create((HWND)m_parent, xCHILD_ID_TITLE_DIALOG, xOffset, yOffset, w, h, szTitle, cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON_LARGE));// 0, 0, 0);
		m_titleDlg->SetNotifyWindow(m_hWnd);
		m_titleDlg->Show();
		m_titleDlg->InvalidateRect(0);
		yOffset+=m_titleDlg->m_h;

	}
	else
	{
		wchar_t szTitle[128];
		yiToUnicode(szTitle, eng->GetTitle(), 0);
		m_titleDlg->SetTitle(szTitle);

		if (IsWindowVisible(*m_titleDlg)==False)
		{
			m_titleDlg->Show();
		}
	}

	if (xGET_GOBAN_MODE(eng->m_flag)==eGCM_JOSEKI_EDITOR)
	{
		RegisterNodeChangeNotifyWindow(*m_titleDlg);
	}
	else
	{
		UnregisterNodeChangeNotifyWindow(*m_titleDlg);
	}

	if (m_infoDlg==0)
	{
		m_infoDlg = new cglsInformationDlg();
		m_infoDlg->Create((HWND)m_parent, xCHILD_ID_INFORMATION_DIALOG, xOffset, yOffset, m_titleDlg->m_w, 20, eng->GetSgfHeader(), eGIDID_ALL, cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON_SMALL));
		m_infoDlg->SetNotifyWindow(m_hWnd);
		m_infoDlg->Show();
		m_infoDlg->InvalidateRect(0);
		yOffset+=m_infoDlg->m_h;
	}
	else
	{
		if (m_stoneInfoDlg==0)
		{
			m_infoDlg->UpdateHeader(eng->GetSgfHeader());
			if (IsWindowVisible(*m_infoDlg)==False)
			{
				m_infoDlg->Show();
			}
		}
	}


	if (xGET_GOBAN_MODE(eng->m_flag)==eGCM_JOSEKI_EDITOR)
	{
		m_infoDlg->Hide();

		if (m_stoneInfoDlg==0)
		{
			m_stoneInfoDlg = new cglsStoneInfoDlg();
			m_stoneInfoDlg->Create((HWND)m_parent, xCHILD_ID_STONE_INFO_DLG, xOffset, yOffset, m_titleDlg->m_w, 20, eng->GetCurrentNode(), cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON));
			m_stoneInfoDlg->SetNotifyWindow(m_hWnd);
			m_stoneInfoDlg->Show();
			m_stoneInfoDlg->InvalidateRect(0);
			yOffset+=m_stoneInfoDlg->m_h;
			RegisterNodeChangeNotifyWindow(*m_stoneInfoDlg);
		}
		else
		{
			m_stoneInfoDlg->UpdateTree(eng->GetCurrentNode());//GetSgfHeader());
			if (IsWindowVisible(*m_stoneInfoDlg)==False)
			{
				m_stoneInfoDlg->Show();
			}
		}
	}
	else
	{
		if (m_stoneInfoDlg && IsWindowVisible(*m_stoneInfoDlg)==True)
		{
			m_stoneInfoDlg->Hide();
		}
	}

	char* pszBlackPlayer=eng->GetHeader()->pszBlackName;
	char* pszWhitePlayer=eng->GetHeader()->pszWhiteName;
	int nBlackLevel=GET_BLACK_LEVEL(eng->GetHeader()->flag32);
	int nWhiteLevel=GET_WHITE_LEVEL(eng->GetHeader()->flag32);

#ifdef _TIMER_DEBUG_
	eGlsPlayerPannelStyle style=eGPPS_PLAY3;
#else
	eGlsPlayerPannelStyle style=eGPPS_EDIT;
#endif

	if (eng->GetInputMode()==eIM_PLAY ||
		eng->GetInputMode()==eIM_WATCH ||
			eng->GetInputMode()==eIM_WAIT_USER)
	{
		style=eGPPS_AGEHAMA;
		
		sGoHeader* header=eng->GetHeader();

		if (IS_LIMITED_PLAY_TIME(header->condition)!=0)
			style=(eGlsPlayerPannelStyle)((int)style|eGPPS_TIME);
		if (GET_SECOND_READ_COUNT(header->condition2)!=0)
			style=(eGlsPlayerPannelStyle)((int)style|eGPPS_TIME2);

		//sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();
	}

	int playerWidth=m_titleDlg->m_w;

	if (eng->GetInputMode()!=eIM_PLAY&&
		eng->GetInputMode()!=eIM_WATCH)
	{
		playerWidth=playerWidth;
	}


	if (xGET_GOBAN_MODE(eng->m_flag)==eGCM_JOSEKI_EDITOR)
	{
		goto SKIP_PLAYER_PANNEL;
	}

	if (m_playerBlack==0)
	{
		sGlsPlayerPannelDetail detail;

		detail.font1=cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON_SMALL);
		detail.font2=cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON);
		int h=cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON)->GetHeight(__DEFAULTGRAPHICS__);



		m_playerBlack = new cglsPlayerPannel();
		m_playerBlack->Create((HWND)m_parent, xCHILD_ID_BLACK_PANNEL, xOffset, yOffset, playerWidth, h, True, 
			eng->GetHeader(), style, &detail);
		m_playerBlack->SetNotifyWindow(*this);
		m_playerBlack->SetPlayerInfo(pszBlackPlayer==0?cyctrlGobanResource::GetInstance().GetStringA(eGTRID_BLACK):pszBlackPlayer, nBlackLevel, eng->GetHeader()->pszBlackLevel);

		int delta=0;
		if (eng->GetInputMode()==eIM_PLAY ||
				eng->GetInputMode()==eIM_WATCH)
		{
			yOffset+=m_playerBlack->m_h;
		}
		else
		{
			delta=m_playerBlack->m_w;
		}

		m_playerWhite = new cglsPlayerPannel();
		m_playerWhite->Create((HWND)m_parent, xCHILD_ID_WHITE_PANNEL, 
			xOffset+delta, yOffset, playerWidth, h, False, eng->GetHeader(), style/*eGPPS_PLAY*/, &detail);
		m_playerWhite->SetNotifyWindow(*this);
		m_playerWhite->SetPlayerInfo(pszWhitePlayer==0?cyctrlGobanResource::GetInstance().GetStringA(eGTRID_WHITE):pszWhitePlayer, nWhiteLevel, eng->GetHeader()->pszWhiteLevel);

		yOffset+=m_playerWhite->m_h;
	}
	else
	{
		int nDeadWhite,nDeadBlack;
		eng->GetDeadStone(nDeadBlack, nDeadWhite);
		//if (eng->GetHeader()->pszBlackLevel)

		m_playerBlack->SetPannelStyle(style);
		m_playerBlack->SetPlayerInfo(pszBlackPlayer==0?cyctrlGobanResource::GetInstance().GetStringA(eGTRID_BLACK):pszBlackPlayer, nBlackLevel, eng->GetHeader()->pszBlackLevel);
		m_playerBlack->SetDeadStone(nDeadWhite,False);
		


		//yOffset+=m_playerBlack->m_h;
		//else
		//	m_playerBlack->SetPlayerInfo(pszBlackPlayer==0?cyctrlGobanResource::GetInstance().GetStringA(eGTRID_BLACK):pszBlackPlayer, nBlackLevel);

		//if (eng->GetHeader()->pszWhiteLevel)
		m_playerWhite->SetPannelStyle(style);
		m_playerWhite->SetPlayerInfo(pszWhitePlayer==0?cyctrlGobanResource::GetInstance().GetStringA(eGTRID_WHITE):pszWhitePlayer, nWhiteLevel, eng->GetHeader()->pszWhiteLevel);
		m_playerWhite->SetDeadStone(nDeadBlack, False);

		//yOffset+=m_playerWhite->m_h;
		//else
		//	m_playerWhite->SetPlayerInfo(pszWhitePlayer==0?cyctrlGobanResource::GetInstance().GetStringA(eGTRID_WHITE):pszWhitePlayer, nWhiteLevel);

		if (IsWindowVisible(*m_playerBlack)==False)
		{
			m_playerBlack->Show();
		}
		if (IsWindowVisible(*m_playerWhite)==False)
		{
			m_playerWhite->Show();
		}
	}


	if (eng->GetInputMode()==eIM_PLAY ||
			eng->GetInputMode()==eIM_WATCH ||
			eng->GetInputMode()==eIM_WAIT_USER)
	{
		if (m_playerBlack->ReSize(m_titleDlg->m_w, m_playerBlack->m_h)==True)
		{
			m_playerBlack->ReArrangeItemz();
		}
		if (m_playerWhite->ReSize(m_titleDlg->m_w, m_playerWhite->m_h)==True)
		{
			m_playerWhite->ReArrangeItemz();
		}
		m_playerWhite->Move(m_playerBlack->m_x, m_playerBlack->m_h+m_playerBlack->m_y);
	}
	else
	{
		if (m_playerBlack->ReSize(m_titleDlg->m_w/2, m_playerBlack->m_h)==True)
		{
			m_playerBlack->ReArrangeItemz();
		}

		if (m_playerWhite->ReSize(m_titleDlg->m_w/2, m_playerWhite->m_h)==True)
		{
			m_playerWhite->ReArrangeItemz();
		}
		m_playerWhite->Move(m_playerBlack->m_x+m_playerBlack->m_w, m_playerBlack->m_y);
	}
	
	if (eng->GetInputMode()==eIM_PLAY ||
			eng->GetInputMode()==eIM_WATCH)
	{
		sGoPlayInfo* info = (sGoPlayInfo* )eng->GetExtraData();
		sGoHeader* header = eng->GetHeader();

		unsigned int nTotalTime = GET_PLAY_TIME(eng->GetHeader()->condition)*60;
		unsigned int nNormalReadSec = GET_SECOND_READ(eng->GetHeader()->condition);
		unsigned int nReadSec = GET_SECOND_READ2(eng->GetHeader()->condition);
		unsigned int count = GET_SECOND_READ_COUNT(eng->GetHeader()->condition2);

		m_playerBlack->GobanChanged(m_nCurrentGoId);
		m_playerWhite->GobanChanged(m_nCurrentGoId);

		Bool bIsNotRegistered=m_playerBlack->AddTimer(m_nCurrentGoId, nTotalTime, nNormalReadSec, nReadSec, count);
		m_playerWhite->AddTimer(m_nCurrentGoId, nTotalTime, nNormalReadSec, nReadSec, count);

		/*if (m_playerBlack->IsRegistered(m_nCurrentGoId)==False)
		{
			m_playerBlack->AddTimer(m_nCurrentGoId, nTotalTime, nNormalReadSec, nReadSec, count);
			m_playerWhite->AddTimer(m_nCurrentGoId, nTotalTime, nNormalReadSec, nReadSec, count);
		}
		*/

		if (eng->IsCurrentOrderBlack()==True)
		{
			//m_playerWhite->ShowTimer(eng->m_id);
			//m_playerBlack->ShowTimer(eng->m_id);

			m_playerWhite->StopTimer(eng->m_id, info->white.totalUsedTime);
			m_playerBlack->StartTimer(eng->m_id);
		}
		else
		{
			//m_playerWhite->ShowTimer(eng->m_id);
			//m_playerBlack->ShowTimer(eng->m_id);

			m_playerBlack->StopTimer(eng->m_id, info->black.totalUsedTime);
			m_playerWhite->StartTimer(eng->m_id);
		}
	}

#ifdef _TIMER_DEBUG_

	if (eng->GetInputMode()!=eIM_ENJOY)
	{
		unsigned int nTotalTime = 20;
		unsigned int nNormalReadSec = 10;
		unsigned int nReadSec = 15;
		unsigned int count = 1;//GET_SECOND_READ_COUNT(3);

		memset(&g_black, 0, sizeof(g_black));
		memset(&g_white, 0, sizeof(g_white));

		m_playerBlack->AddTimer(m_nCurrentGoId,nTotalTime, nNormalReadSec, nReadSec, count);
		m_playerWhite->AddTimer(m_nCurrentGoId,nTotalTime, nNormalReadSec, nReadSec, count);
		//m_playerBlack->SetPlayerTime(
		//m_playerWhite->SetPlayerTime(nTotalTime, nNormalReadSec, nReadSec, count);
		m_playerBlack->SetUsedTime(m_nCurrentGoId, g_black.totalUsedTime);//>SetRemainTime(info->black.remainTime, info->black.remainSecReadCount);
		m_playerWhite->SetUsedTime(m_nCurrentGoId, g_white.totalUsedTime);//SetRemainTime(info->white.remainTime, info->white.remainSecReadCount);
		

		if (eng->IsCurrentOrderBlack()==True)
		{
			g_black.startTime=cyiTime::CurrentmSec();
			m_playerBlack->StartTimer(eng->m_id);
			m_playerWhite->StopTimer(eng->m_id, 0);
		}
		else
		{
			g_white.startTime=cyiTime::CurrentmSec();
			m_playerWhite->StartTimer(eng->m_id);
			m_playerBlack->StopTimer(eng->m_id, 0);
		}
	}
#endif

SKIP_PLAYER_PANNEL:

	if (m_textPannel==0)
	{
		m_textPannel = new cglsTextPannel();
		if (m_playerWhite)
		{
			int h=ctrl->m_y-(m_playerWhite->m_h+m_playerWhite->m_y)+10;

			if (h<100)
				h=100;
			m_textPannel->Create((HWND)m_parent, xCHILD_ID_TEXT_PANNEL, xOffset, yOffset, controlWidth,100 , False, eng->GetHeader(), eGTPS_EDIT, cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON));
		}
		else
		{
			m_textPannel->Create((HWND)m_parent, xCHILD_ID_TEXT_PANNEL, xOffset, yOffset, controlWidth, controlWidth*2/3, False, eng->GetHeader(), eGTPS_EDIT, cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON));
		}
		RegisterNodeChangeNotifyWindow(*m_textPannel);
	}
	
	if (m_playerWhite)
	{
		m_textPannel->Move(m_textPannel->m_x, m_playerWhite->m_y+m_playerWhite->m_h);
		m_textPannel->ReSize(m_textPannel->m_w, m_currentCtrl->m_y-m_playerWhite->m_y-m_playerWhite->m_h);
	}

	if (eng->GetInputMode()==eIM_PLAY ||
			eng->GetInputMode()==eIM_WATCH ||
				eng->GetInputMode()==eIM_WAIT_USER)
	{
		m_textPannel->SetTextPannelStyle(eGTPS_PLAY);
	}
	else
	{
		m_textPannel->SetTextPannelStyle(eGTPS_EDIT);
	}

	if (xGET_GOBAN_MODE(eng->m_flag)==eGCM_JOSEKI_EDITOR)
	{
		m_textPannel->SetEnableInput(False);
	}
	else
	{
		m_textPannel->SetEnableInput(True);
	}

	if (IsWindowVisible(*m_textPannel)==False)
	{
		m_textPannel->Show();
	}
}
#endif

// ------------------------------------------------------
void cglsGoban::xShowEditBox(int x, int y)
{
	m_ptEditBox.x=x;
	m_ptEditBox.y=y;

	if (m_editBox==0)
	{
		sGlsEditBoxDetail detail;
		m_editBox = xcglsEditBox();
		cglsEditBox::xDefaultDetail(&detail);

		detail.bIsMutipleLine=True;
		detail.nEnterCommand=xCHILD_EDITBOX;
		m_editBox->Create((HWND)this, xCHILD_EDITBOX, x, y, GetFontHeight(True)*10, GetFontHeight(True)*3, &detail);

	}

	int nModifiedX = x;
	int nModifiedY = y;

	if ((x+m_editBox->m_w)>m_w)
	{
		nModifiedX=m_w-m_editBox->m_w;
	}

	if ((m_editBox->m_h+y)>m_h)
	{
		nModifiedY=m_h-m_editBox->m_h;
	}

	m_editBox->Move(nModifiedX, nModifiedY);

	m_editBox->SetText((wchar_t*)0);
	m_editBox->Show();
	SetFocus(*m_editBox);

}

#ifdef NEW_STYLE
// ------------------------------------------------------
const wchar_t* cglsGoban::GetSubTitle()
{
	cglsStatic* label = (cglsStatic*)m_controlz[0]->GetPage(1);

	return label->GetText();
}


// ------------------------------------------------------
void cglsGoban::SetSubTitle(wchar_t* title)
{
	SetTitleText(title);
	return ;
}
#endif

// ------------------------------------------------------
// WM_CHANGE_GOBAN時の処理
// WPARAM : 
void cglsGoban::ChangeCurrentGoban(goid_t id, Bool bIsNew)
{
	cglsGobanControl* ctrl=0;
	cyiGoEngine* eng = m_GoEnginez[id];
	eInputMode mode;
	if (eng==0)
		return ;

	if (m_GoEnginez[m_nCurrentGoId])
	{
		sGoPlayInfo* info = (sGoPlayInfo*)m_GoEnginez[m_nCurrentGoId]->GetExtraData();
		if (info)
		{
			if (info->dlg)
			{
				info->dlg->Hide();
			}
		}

		if (info)
		{
			if (info->baloon)
			{
				info->baloon->Hide();
			}
		}
	}

	xTRACE("cglsGoban::ChangeCurrentGoban() called %d->%d", m_nCurrentGoId, id);
	if (m_nCurrentGoId==id)
	{
		return ;
	}

	sGoPlayInfo* info= (sGoPlayInfo* )eng->GetExtraData();

	if (info)
	{
		if (info->dlg)
		{
			if (info->dlg->IsCreated()==True)
			{
				info->dlg->Show();
			}
			else
			{
				delete info->dlg;//->Show();
				info->dlg=0;
			}
		}

		if (info->baloon)
		{
			info->baloon->Show();
		}

#ifdef NEW_STYLE
		if (info->nLastControlIndex==0)
		{
			SetTimer(m_hWnd, 2, 500, 0);
		}
#endif
	}

	if (m_editBox!=0)
	{
		m_editBox->SetText((wchar_t*)0);
		m_editBox->Hide();
	}

	mode=eng->GetInputMode();

	//m_detail.bIsJosekiEditorMode=True;

	m_nCurrentGoId=id;

#ifndef NEW_STYLE
	if (eng->m_bIsLocked)
	{
		if (this->m_currentCtrl)
			m_currentCtrl->Enable(False);
	}
	else
	{
		if (m_currentCtrl)
			m_currentCtrl->Enable(True);
	}

	if (m_detail.mode!=eGAM_JOSEKI_EDITOR)//m_detail.bIsJosekiEditorMode==False)
	{
		xResetGobanControl(eng);
	}
#endif
	Update(eGL_ALL, eng->GetStoneMap());//eGL_ALL, eng->GetStoneMap());



	if (bIsNew==True)
	{
	//if (xIS_BIT(eng->m_flag, eGEF_NOW_SEARCH_RESULT)==False)
	//	eng->m_bNowSearchResult=True;
		SearchResult();
	}
	else
	{
		NotifyNodeChanged(True);

	}

	// TODO: タブコントロールのフォーカス
	// SetTabFocus(id);
}


// ------------------------------------------------------
LPCSTR cglsGoban::GetTitle(goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return 0;

	return (LPCSTR)eng->GetTitle();//GetHeader()->pszEventName;
}

// ------------------------------------------------------
LPCSTR cglsGoban::GetBlackName(goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return 0;

	return (LPCSTR)eng->GetHeader()->pszBlackName;
}

// ------------------------------------------------------
void cglsGoban::SetBlackName(LPCSTR name, goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return ;

	sGoHeader* header = eng->GetHeader();

	LPCSTR str = eng->GetHeader()->pszBlackName;
	header->pszBlackName= yistrheap(name);

	yifree((void*)str);
}

// ------------------------------------------------------
LPCSTR cglsGoban::GetWhiteName(goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return 0;
	return (LPCSTR)eng->GetHeader()->pszWhiteName;
}

// ------------------------------------------------------
void cglsGoban::SetWhiteName(LPCSTR name, goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return ;
	sGoHeader* header = eng->GetHeader();

	LPCSTR str = eng->GetHeader()->pszWhiteName;
	header->pszWhiteName= yistrheap(name);

	yifree((void*)str);

}

// ------------------------------------------------------
unsigned int cglsGoban::GetDetailInfo(goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return 0;
	return eng->GetHeader()->condition;
}

// ------------------------------------------------------
void cglsGoban::SetDetailInfo(unsigned int condition, goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return ;

	sGoHeader* header = eng->GetHeader();
	eng->GetHeader()->condition = condition;
}

// ------------------------------------------------------
unsigned int cglsGoban::GetDetailFlag(goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return 0;

	return eng->GetHeader()->flag32;
}

// ------------------------------------------------------
void cglsGoban::SetDetailFlag(unsigned int flag, goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return ;

	sGoHeader* header = eng->GetHeader();
	eng->GetHeader()->flag32= flag;
}

//----------------------------------------
void cglsGoban::Play()
{
	//cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	//if (eng==0)
	//	return ;

	//m_branches.clear();

	//aStoneList list;
	//stone_t** stone = eng->Goto(1, list);

	////SetBranch(eng);

	//if (stone)
	//{
	//	PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
	//	Update(eGL_ALL, stone);
	//}

	aStoneList list;
	Goto(m_nCurrentGoId, 1, list);
}

// ----------------------------------------
Binary cglsGoban::Export(const char* pszFileName, sSoftwareLisenseCode* license)
{
	goid_t id = m_nCurrentGoId;
	cyiGoEngine* eng = m_GoEnginez[id];

	return (Binary)eng->Save(pszFileName, eGFT_BIN, license, sizeof(*license));
}

// ----------------------------------------
Bool cglsGoban::GetLicenseCode(const char* pszFileName, sSoftwareLisenseCode* license)
{
	char buffer[4+sizeof(*license)];
	if (yiFileRead(pszFileName, buffer, 0, 4+4+sizeof(*license))==False)
	{
		return False;
	}

	if (strncmp(buffer, "BGF", 3)!=0)
	{
		return False;
	}

	int* n = (int*)&buffer[4];

	if (*n!=sizeof(*license))
	{
		return False;
	}

	memcpy(license, &buffer[8], sizeof(*license));
	return True;

}


// ----------------------------------------
Bool cglsGoban::Save(const char* pszFileName, eGoFileType type/*=eGFT_SGF*/, goid_t _id/*=0xFFFFFFFF*/)
{
	goid_t id = _id==0xFFFFFFFF?m_nCurrentGoId:_id;
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng==0)
		return False;

	while(1)
	{
		eGoContentsMode eMode = xGET_GOBAN_MODE(eng->m_flag);
		if (eMode==eGCM_REFERENCE ||
				eMode==eGCM_REFERENCE2)
		{
			eng = m_GoEnginez[eng->m_nReferenceFromId];

			if (eng==0)
			{
				return False;
			}
		}
		else
		{
			break;
		}
	}

	return eng->Save(pszFileName, type);
}

//----------------------------------------
Bool cglsGoban::Move(unsigned int x, unsigned int y, Bool bIsNoActivate)
{
	return __super::Move(x, y, bIsNoActivate);
}

#include "glsSystem.h"



#ifdef NEW_STYLE
#include "glsGobanControlEx.h"
#include "glsSlide.h"

// ---------------------
void cglsGoban::xGetGobanSize(int&w, int&h)
{
	Font* font = cyctrlGobanResource::GetInstance().GetFont(eRIDF_TAB_CONTROL);

	int wt = CalculateFontHeight(font==0?GetFont():font, True);
	wt+=(wt>>1);

	float factor=(float)MAX_GOBAN_HEIGHT/(float)MAX_GOBAN_WIDTH;

	// 1. wに合わせてみる。
	int _h = (int)((float)(w+wt)*factor);

	if (_h<=m_h)
	{
		h=_h;
	}

	// 2. はみ出す場合は hに合わせる。
	w = (int)((float)h/factor);
}


//----------------------------------------
Bool cglsGoban::ReSize(unsigned int x, unsigned int y, Bool NoActivate)
{
	return ReSize(m_x, m_y, x, y, NoActivate);
}

//----------------------------------------
Bool cglsGoban::ReSize(int x, int y, long w, long h, Bool NoActivate)
{
	//w=w<h?w:h;

	xGetGobanSize((int&)w, (int&)h);

	if (cGlassInterface::ReSize(w, h, NoActivate)==False)
	{
		return False;
	}

	//m_nGobanWidth=m_w;

	// TODO: イメージのスケール変更を行う。
	AdjustScale();

	// コンテナーをサイズに合わせて移動させる。

	if (m_container)
	{
		m_container->Move(m_w+m_x+2, 0);
		int w = CalculateFontHeight(cyctrlGobanResource::GetInstance().GetFont(eRIDF_TAB_CONTROL), True);
		w+=(w>>1);
		m_container->ReSize(w, m_h, NoActivate);
		m_container->SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_TAB_CONTROL));
	}

	xResizeGobanControl();

	//cglsSlide* slide = (cglsSlide*)GetChild(2);

	//int n = MAX_BLOCK_SIZE*m_scale*0.75;
	//Font* font = cyiShape::GetFontByHeight(n-2, L"Meiryo");

	//if (slide==0)
	//{
	//	xCreateSlideControl();

	//	slide = new cglsSlide();
	//	slide->Cre	ate(this, 2, 0, m_w-n, m_w, n-2);//GetFontHeight());
	//	slide->Show();

	//	cglsGobanControl* control = new cglsGobanControl();
	//	control->Create(slide, 1, m_w, n-2, eGCS_BASE);
	//	//contorl->Update(eGUM_FORCED);
	//	control->Show();

	//	slide->AddControl(control, eGIA_CENTER);
	//}


	//slide->ReSize(w, n-2);
	//slide->SetFont(font);
	//slide->Update(eGUM_FORCED);
	//slide->Move(0, m_w-slide->m_h);
	//slide->InvalidateRect(0);

	//slide->Scroll(1, eSBSD_TORIGHT, eGAP_FISO, 15, 3000);


	//delete font;

	return True;
}

int cglsGoban::GetRealWidth()
{
	if (m_container==0)
		return m_w;

	return m_w+m_container->m_w+2;
}

#endif

#ifdef NEW_STYLE
typedef struct {
	cGlassInterface* gls;
	eGobanControlType type;
	eGobanMode mode;
}sUserControlList;
//----------------------------------------
Bool cglsGoban::AddControl(cGlassInterface* gls, eGobanControlType type, eGobanMode mode)
{
	if (m_listUserControl==0)
		m_listUserControl = new cyiList();

	sUserControlList s = {gls, type, mode};
	m_listUserControl->addref(yitoheap(&s, sizeof(s)));
	m_controlz[type]->AddPage(gls);

	return True;
}

//----------------------------------------
Bool cglsGoban::ShowControl(int id, eGobanControlType type)
{
	return m_controlz[type]->ShowPage(id);
}

//----------------------------------------
cGlassInterface* cglsGoban::GetControl(int id, eGobanControlType type)
{
	return m_controlz[type]->GetPage(id);
}

// ---------------------------------------
void cglsGoban::SetTitleText(const wchar_t* text)
{
	cglsStatic* label = (cglsStatic*)m_controlz[0]->GetPage(1);
	m_controlz[0]->ShowPage(1);
	label->SetText(0, text);
	label->Update(eGUM_FORCED);
	label->SetItemPos(0, label->m_y, eGIA_CENTER);
	label->InvalidateRect(0);

	return ;
}

// ---------------------------------------
void cglsGoban::SetTitleText(const char* text)
{
	wchar_t* p = yiToUnicode(0, text, 0);
	SetTitleText(p);
	yifree(p);

	return ;
}



#endif

#ifndef NEW_STYLE

//----------------------------------------
Bool cglsGoban::Resize(long w, long h, Bool NoActivate)
{
	return Resize(m_x, m_y, w, h, NoActivate);
}



//----------------------------------------
Bool cglsGoban::Resize(int x, int y, long w, long h, Bool NoActivate)
{
	//m_w = w>h?h:w;
	//m_h = w>h?h:w;

	//if (m_x==x && m_y==y)
	//{
	//	::SetWindowPos(m_hWnd, 0, x, y, m_w, m_w, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOZORDER);
	//}
	//else
	//{
	//	::SetWindowPos(m_hWnd, 0, x, y, m_w, m_w, SWP_SHOWWINDOW |  SWP_NOZORDER);
	//	m_x=x;
	//	m_y=y;
	//}

	w=w<h?w:h;
	if (cGlassInterface::ReSize(w, w, NoActivate)==False)
	{
		return False;
	}


	m_nGobanWidth=m_w;

	if (m_preview && IsWindowVisible(*m_preview))
	{
		if (m_detail.bIsPreviewPopup==False)
		{
			int x=m_preview->AutoReSize(m_w);
			Move(x, m_y);
			//SetWindowPos(m_hWnd, 0, m_x, y, m_w, m_w, SWP_SHOWWINDOW |  SWP_NOZORDER);
		}
	}

	// TODO: イメージのスケール変更を行う。
	AdjustScale();

#ifndef NEW_STYLE
	if (m_bIsPreviewMode)
	{
		if (m_currentCtrl)
		{
			m_currentCtrl->Move(m_x+((m_w-m_currentCtrl->m_w)>>1), m_y+m_h, NoActivate);
			
			if (m_infoLabel)
			{
				m_infoLabel->Move(m_x, m_currentCtrl->m_h+m_currentCtrl->m_y, NoActivate);
			}
		}

	}
#endif

	SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL));

	//if (m_btnMenu)
	//{
	//	m_btnMenu->Move(m_w+m_x, 0);
	//	m_btnMenu->SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON));
	//}

	//m_tab->Move(m_w+m_x, m_btnMenu?m_btnMenu->m_h:0);
	//m_tab->ReSize(m_w-(m_btnMenu?m_btnMenu->m_h:0), cyctrlGobanResource::GetInstance().GetFont(eRIDF_TAB_CONTROL));
	m_container->Move(m_w+m_x, 0);
	m_container->ReSize(m_container->m_w, m_h, NoActivate);
	m_container->SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_TAB_CONTROL));

	if (m_myView!=0)
	{
		float fFontSize = 22.0*m_scale;
		fFontSize=fFontSize>16.0?16.0:fFontSize;

		Font font(L"Meiryo UI", fFontSize);
		m_myView->SetFont(&font);
		m_myView->Move((m_w-m_myView->m_w)>>1, (m_h-m_myView->m_h)>>1);

		//m_myFriendView->Move(m_container->m_x+m_container->m_w, 0);
	}


	int xOffset = m_container->m_x+m_container->m_w+5;
	int yOffset = 0;
	int nControlWidth=0;

	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];


	if (eng==0)
	{
		return True;
	}

	if (m_currentCtrl)
	{
		m_currentCtrl->Move(xOffset, m_w-m_currentCtrl->m_h);
		m_currentCtrl->SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON));
		nControlWidth=m_currentCtrl->m_w;
	}
	
	if (m_titleDlg && IsWindowVisible(*m_titleDlg))
	{
		m_titleDlg->Move(xOffset, yOffset);
		m_titleDlg->ReSize(nControlWidth, m_titleDlg->m_h);
		m_titleDlg->SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON_LARGE));
		yOffset+=m_titleDlg->m_h;
	}

	if (m_infoDlg&& IsWindowVisible(*m_infoDlg))
	{
		m_infoDlg->Move(xOffset,yOffset);// m_titleDlg->m_y+m_titleDlg->m_h);
		m_infoDlg->ReSize(nControlWidth, m_infoDlg->m_h);
		m_infoDlg->SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON_SMALL));
		yOffset+=m_infoDlg->m_h;
	}

	if (m_stoneInfoDlg && IsWindowVisible(*m_stoneInfoDlg))
	{
		m_stoneInfoDlg->Move(xOffset,yOffset);// m_titleDlg->m_y+m_titleDlg->m_h);
		m_stoneInfoDlg->ReSize(nControlWidth, m_infoDlg->m_h);
		m_stoneInfoDlg->SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON_SMALL));
		yOffset+=m_stoneInfoDlg->m_h;
	}


	int playerWidth=nControlWidth;
	int delta=0;

	if (eng && 
		eng->GetInputMode()!=eIM_PLAY)
	{
		playerWidth=nControlWidth>>1;
	}

	if (m_playerBlack&& IsWindowVisible(*m_playerBlack))
	{
		m_playerBlack->Move(xOffset,yOffset);// m_infoDlg->m_y+m_infoDlg->m_h);
		m_playerBlack->ReSize(playerWidth, m_playerBlack->m_h);
		m_playerBlack->SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON_SMALL), cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON));

		if (eng->GetInputMode()==eIM_PLAY)
		{
			yOffset+=m_playerBlack->m_h;
		}
		else
			delta=m_playerBlack->m_w;
	}

	if (m_playerWhite&& IsWindowVisible(*m_playerWhite))
	{
		m_playerWhite->Move(xOffset+delta, yOffset);//m_playerBlack->m_y+m_playerBlack->m_h);
		m_playerWhite->ReSize(playerWidth, m_playerWhite->m_h);
		m_playerWhite->SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON_SMALL), cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON));
		yOffset+=m_playerWhite->m_h;

	}

	if (m_textPannel&& IsWindowVisible(*m_textPannel))
	{
		m_textPannel->Move(xOffset, yOffset);//m_playerWhite->m_y+m_playerWhite->m_h);

		if (m_playerWhite)
		{
			int h=m_currentCtrl->m_y-(m_playerWhite->m_h+m_playerWhite->m_y)+10;

			if (h<100)
				h=100;

			m_textPannel->ReSize(nControlWidth, h);
			m_textPannel->SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON));
		}
		else
		{
			m_textPannel->ReSize(nControlWidth, nControlWidth*2/3);
			m_textPannel->SetFont(cyctrlGobanResource::GetInstance().GetFont(eRIDF_BUTTON));
		}
	}

	if (m_currentCtrl)
	{
		m_currentCtrl->ZOrder(eZO_TOP);
	}
	// メッセージなどが表示されている場合の処理を行う。

	// 描画ルーチンを走らせる。
	//Update(eGL_ALL);
	return True;
}
#endif

// -------------------------------------------------------
void cglsGoban::StartCalculateMode(goid_t id)
{
	id = id?id:m_nCurrentGoId;
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng==0)
		return ;

	stone_t** stone=eng->StartCalculate();

#ifndef NEW_STYLE
	m_currentCtrl->ChangeControlSet(eGCS_CALCULATE_MODE);

	m_playerBlack->RemoveTimer(id);
	m_playerWhite->RemoveTimer(id);
#endif
	Update(eGL_CALCULATE, stone);
}

// ------------------------------------------------------------------
stone_t** cglsGoban::CalculateEnd(goid_t id, int* xnBlack, int* xnWhite, int& winer, int& score, Bool bShowDialog)
{
	id = id==0?m_nCurrentGoId:id;
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng==0)
		return 0;

	sResult result;
	stone_t** stone = eng->EndCalculate(&result);
	Update(eGobanLayer::eGL_CALCULATE_END, stone);

	int n = GET_KOMI(eng->GetHeader()->condition);
	Bool bIsHalfScore=HALF_SCORE(eng->GetHeader()->condition);
	Bool bIsReverse=REVERSE_KOMI(eng->GetHeader()->condition);
	float fKomi = (float)n + (bIsHalfScore?0.5:0.0);
	result.nKomi=fKomi;

	if (bIsReverse)
		fKomi*=-1;

	wchar_t buffer[1024];
	float b_a=result.nBlackArea+result.nWhiteDeadStone-result.nWhiteArea-result.nBlackDeadStone-fKomi;

	wsprintfW(buffer, L"黒: %d+%d=%d \r\n白: %d+%d+(%d%s)=%d目半\r\n%s %d目%s勝ち", 
			result.nBlackArea, result.nWhiteDeadStone, result.nBlackArea+result.nWhiteDeadStone,
			result.nWhiteArea, result.nBlackDeadStone, (int)fKomi, bIsHalfScore?L".5":L"", result.nWhiteArea+result.nBlackDeadStone+(int)fKomi, (b_a<0.0?L"白":L"黒"), abs((int)-b_a), bIsHalfScore?L"":L"半");

	if (b_a==0.0)
		winer=3;
	else if (b_a>0.0)
		winer=1;
	else 
		winer=2;

	score = abs((int)b_a);

	if (score>0xFE)
	{
		score=0xFD;
	}

	if (bShowDialog)
		ShowMessage(0, buffer, L"結果", MB_OK);
	else
		SetPlayResult(id, winer, score, True, &result);

	xOFF_BIT(eng->m_flag, eGEF_CALCULATE_MODE);

	//if (bShowDialog)
	//{
		//eng->Goto(MOVE_TOP, 0);
		ChangeInputMode(eIM_EDIT);
		//eng->Goto(MOVE_END, 0);
#ifndef NEW_STYLE
		this->m_currentCtrl->ChangeControlSet(eGCS_NORMAL_EDITOR);
#endif
		//Update(eGL_ALL, eng->GetStoneMap());
	//}

	return stone;

}



// ---------------------------------------------------------------
Bool cglsGoban::GetNearCoordinate(int x, int y, POINT* ps, int nLineCnt)
{
	//1. まずは、x座標から
	int xi=-1, yi=-1;

	for (int i=0; i<nLineCnt-1; i++)
	{
		if ((float)x>=m_scaled_point[i].x &&
				(float)x<m_scaled_point[i+1].x)
		{
			if (((float)x-m_scaled_point[i].x)<(m_scaled_point[i+1].x)-(float)x)
			{
				xi=i;
			}
			else
			{
				xi=i+1;
			}
			break;
		}
	}

	if (xi==-1)
		return False;

	for (int i=0; i<nLineCnt-1; i++)
	{
		if ((float)y>=m_scaled_point[i].y &&
				(float)y<m_scaled_point[i+1].y)
		{
			if (((float)y-m_scaled_point[i].y)<(m_scaled_point[i+1].y)-(float)y)
			{
				yi=i;
			}
			else
			{
				yi=i+1;
			}
			break;
		}
	}

	if (yi==-1)
		return False;
	ps->x=xi;
	ps->y=yi;


	return True;
}

// -------------------------------------------------------
cyiGoEngine* cglsGoban::GetGoEngineById(goid_t id)
{
	return m_GoEnginez[id];
}

// -------------------------------------------------------
eInputMode cglsGoban::GetInputMode(goid_t id)
{
	id=id==0?m_nCurrentGoId:id;
	cyiGoEngine* eng = GetGoEngineById(id);

	if (eng==0)
		return eIM_INVALID;

	return eng->GetInputMode();
}


// -------------------------------------------------------
void cglsGoban::ChangeInputMode(goid_t id, eInputMode input)
{
	id=id==0?m_nCurrentGoId:id;
	cyiGoEngine* eng = GetGoEngineById(id);

	if (eng==0)
		return ;

	int prev = eng->GetInputMode();

	if (input!=eIM_PLAY)
	{
		if (prev==eIM_PLAY)
		{
			m_tab->SetItemShowButton(id, True);
		}
	}

	// この場合はタイマーを終了させる。
	if (input!=eIM_PLAY /*&&
			eng->GetInputMode()==eIM_PLAY*/)
	{
#ifndef NEW_STYLE
		m_playerBlack->RemoveTimer(eng->m_id);
		m_playerWhite->RemoveTimer(eng->m_id);
#endif
	}

	eng->SetInputMode(input);

	if (input==eIM_STUDY)
	{
		if (m_menuStudy==0)
		{
			m_menuStudy = new cglsListMenu(cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL));
			m_menuStudy->AddItem(xCHILD_ID_STUDY_START+1, L"この手は良い手だと思う。");
			m_menuStudy->AddItem(xCHILD_ID_STUDY_START+2, L"この手は悪手だと思う。");
			m_menuStudy->AddItem(xCHILD_ID_STUDY_START+3, L"次はどう打つの？");
			m_menuStudy->AddItem(xCHILD_ID_STUDY_START+4, L"この手だめだった？");
			m_menuStudy->AddItem(xCHILD_ID_STUDY_START+5, L"これはすごい手だね。");
			m_menuStudy->AddItem(xCHILD_ID_STUDY_START+5, L"コメント");//, 
		}
	}

	if (input==eIM_EDIT ||
			input==eIM_STUDY)
	{
		xSET_BRANCH_VIEW_MODE(eng->m_flag, eBVM_NORMAL);
		xTRACE("Set branch mode normal\n");
	}
	else
	{
		xSET_BRANCH_VIEW_MODE(eng->m_flag, eBVM_HIDE);
		xTRACE("Set branch mode hide\n");
	}

	if (id==m_nCurrentGoId)
	{
		xResetGobanControl(eng);
	}
}

#include "yiGoCommandz.h"
// -------------------------------------------------------
void cglsGoban::ChangeInputMode(eInputMode input)
{
	ChangeInputMode(m_nCurrentGoId, input);

	///*
	//cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	//if (!eng)
	//{
	//	return ;
	//}

	//// この場合はタイマーを終了させる。
	//if (input!=eIM_PLAY /*&&
	//		eng->GetInputMode()==eIM_PLAY*/)
	//{
	//	xTRACE("cglsGoban::ChangeInputMode() Stop Timer!");
	//	m_playerBlack->RemoveTimer(eng->m_id);
	//	m_playerWhite->RemoveTimer(eng->m_id);
	//	
	//	cglsSystem* system=GetSystem();

	//	if (system)
	//	{
	//		system->EnableCommand(eGCID_PASS, True);
	//	}
	//}

	//if (input==eIM_EDIT ||
	//		input==eIM_STUDY)
	//{
	//	xSET_BRANCH_VIEW_MODE(eng->m_flag, eBVM_NORMAL);
	//	xTRACE("Set branch mode normal\n");
	//}
	//else
	//{
	//	xSET_BRANCH_VIEW_MODE(eng->m_flag, eBVM_HIDE);
	//	xTRACE("Set branch mode hide\n");
	//}

	//eng->SetInputMode(input);

	//xResetGobanControl(eng);
}

// -------------------------------------------------------
//UINT cglsGoban::GetSize()
//{
//	return m_nGobanWidth;
//}

	

// ------------------------------------------------------
// 番号表示	
//void cglsGoban::ShowNo(Bool bIsTrue, int nFrom/*=0*/)
//{
//	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
//
//	if (eng)
//	{
//		Update(eGL_STONE, eng->GetStoneMap(), bIsTrue, nFrom);
//	}
//	return ;
//}

// -----------------------------------------
//int cglsGoban::ShowMessage(unsigned int id, const wchar_t* message, int flag, int timeout)
//{
//	return 0;
//}


///// private

// -------------------------------------------------------
void cglsGoban::DrawInitMutex()
{
	memset(&m_drawMutex, 0, sizeof(m_drawMutex));
	InitializeCriticalSectionAndSpinCount(&m_drawMutex, 5);
}

// -------------------------------------------------------
void cglsGoban::DrawMutexLock()
{
	EnterCriticalSection(&m_drawMutex);
}

// -------------------------------------------------------
void cglsGoban::DrawMutexUnlock()
{
	LeaveCriticalSection(&m_drawMutex);
}

// ------------------------------------------------------
goid_t cglsGoban::ReserveId()
{
	for (int i=1; i<MAX_GOBAN_COUNT; i++)
	{
		if (m_GoEnginez[i]==0)
		{
			m_GoEnginez[i]=GO_ID_RESERVED;
			return i;
		}
	}

	return 0;
}

// ------------------------------------------------------
goid_t* cglsGoban::GetCurrentPlayGobanList()
{
	goid_t buffer[MAX_GOBAN_COUNT];
	int cnt=0;

	for (int i=1; i<MAX_GOBAN_COUNT; i++)
	{
		if (m_GoEnginez[i]==0)
		{
			continue;
		}
		
		eInputMode input = m_GoEnginez[i]->GetInputMode();

		if (input==eIM_PLAY ||
				input==eIM_STUDY)
		{
			buffer[cnt++]=i;
		}
		
	}

	if (cnt==0)
		return 0;

	goid_t* ret = (goid_t*)yialloci(((cnt+1)<<2));
	
	for (int i=0; i<cnt; i++)
	{
		ret[i]=buffer[i];
	}

	return ret;
}

// ------------------------------------------------------
const char* cglsGoban::GetFileName(goid_t id)
{
	id = id==0?m_nCurrentGoId:id;
	cyiGoEngine* eng = m_GoEnginez[id];

	return eng->GetSgfManager()->m_szFileName;
}


// ------------------------------------------------------
sSimpleGoHeader* cglsGoban::GetSimpleGoHeader(goid_t id)
{
	if (id>MAX_GOBAN_CNT)
	{
		return 0;
	}

	if (m_GoEnginez[id]==0)
		return 0;

	return (sSimpleGoHeader* )m_GoEnginez[id]->Export(0);
}

// ------------------------------------------------------
static void xCopyNode(cyiTreeNode* dest, cyiTreeNode* src)
{
	if (src->GetChildList().count()==0)
		return ;

	for (cyiNode* node = src->GetChildList().GetNodeTopFrom(); node; node=node->np)
	{
		cyiTreeNode* x = (cyiTreeNode*)node->ref;

		cSgfStoneInfo* info = (cSgfStoneInfo*)x->GetData();

		if (info==0)
		{
			// TOP?
			continue;
		}

		cyiTreeNode* newNode = new cyiTreeNode((yiAnyValue)new cSgfStoneInfo(info));
		dest->add(newNode);
		xCopyNode(newNode, x);
	}
}

// ------------------------------------------------------
goid_t cglsGoban::CopyAndAdd(goid_t id, eInputMode inputMode, cyiTreeNode* _node, Bool bIsUntilCur)
{
	id = id==0?m_nCurrentGoId:id;
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng==0)
		return 0;

	sGoHeader*header = eng->GetHeader();
	cyiTreeNode* node = _node;
	
	if (node==NODE_CUR)
		node=eng->GetCurrentNode();

	sGobanContentsDetail detail;
	cyiGoEngine::xDefaultDetail(&detail);
	detail.eInputModeValue=inputMode;

	goid_t goid = Add(header, 0, &detail, 0, 0, Color::Orange, True);

	if (goid==0)
		return 0;

	cyiGoEngine* engCopied = m_GoEnginez[goid];
	
		cyiStack stack;
	cyiTreeNode* parent=node;
	while(parent)
	{

		stack.push((yiAnyValue)parent);

		parent=parent->m_parent;
	}

	int n=stack.count();
	cyiTreeNode* pNode=engCopied->GetTopNode();
	engCopied->m_sgfMgr.m_currentNode=pNode;
	for (int i=0; i<n; i++)
	{
		yiAnyValue val;
		
		if (stack.pop(val)==False)
			break;

		cyiTreeNode* xx = (cyiTreeNode*)val;
		
		cSgfStoneInfo* info = (cSgfStoneInfo*)xx->GetData();

		if (info==0)
		{
			// TOP?
			continue;
		}

		cyiTreeNode* newNode = new cyiTreeNode((yiAnyValue)new cSgfStoneInfo(info));
		pNode->add(newNode);
		pNode=newNode;
	}

	if (bIsUntilCur==False)
	{
		if (node->GetChildList().count()!=0)
		{
			xCopyNode(pNode, node);
		}
	}

	engCopied->Goto(MOVE_TOP, 0);
	engCopied->Goto(MOVE_END, 0);

	return id;
}

// ------------------------------------------------------
static cyiTreeNode* xxFindNode(cyiTreeNode* parent, cyiTreeNode* child)
{
	cSgfStoneInfo* childinfo = (cSgfStoneInfo*)child->GetData();

	for (cyiNode* x = parent->GetChildList().GetNodeTopFrom(); x; x=x->np)
	{
		cyiTreeNode* p = (cyiTreeNode*)x->ref;
		cSgfStoneInfo* info = (cSgfStoneInfo*)p->GetData();

		if (childinfo->isSameStone(*info)==False)
			continue;
		return p;
	}

	return 0;
}

typedef struct {
	unsigned int flag;
	cyiTreeNode* orgNode;
	cyiTreeNode* newNode;
} sMergeData;

// ------------------------------------------------------
static cyiList* xIsNodeSame(cyiList* list, cyiTreeNode* node, cyiTreeNode* node2)
{
	unsigned int reason=0;
	int cnt=0;

	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();
	cSgfStoneInfo* info2 = (cSgfStoneInfo*)node2->GetData();

	if ((info==0 && info2!=0) ||
			(info2==0 && info!=0))
	{
		if (info2)
		{
			node->SetData((yiAnyValue)new cSgfStoneInfo(info2));
			list->addref(node);
		}
	}
	else
	{
		if (info!=0 && info2!=0)
		{
			if (*info!=*info2)
			{
				info->set(*info2);
				list->addref(node);
			}
		}
	}

	for (cyiNode* x = node2->GetChildList().GetNodeTopFrom(); x; x=x->np)
	{
		cyiTreeNode* foundNode=0;
		if ((foundNode=xxFindNode(node, (cyiTreeNode*)x->ref))==0)
		{
			// 追加ノード
			node->add((cyiTreeNode*)x->ref);
			//info->set(info2);
			list->addref((cyiTreeNode*)x->ref);

			//sMergeData data = {1, 0, (cyiTreeNode*)x->ref};
			//list->addref(yitoheap(&data, sizeof(data)));
			//cnt++;
			continue;
		}
		xIsNodeSame(list, foundNode, (cyiTreeNode*)x->ref);
		cnt++;
	}

	//for (cyiNode* x = node->GetChildList().GetNodeTopFrom(); x; x=x->np)
	//{
	//	cyiTreeNode* foundNode=0;
	//	if ((foundNode=xxFindNode(node2, (cyiTreeNode*)x->ref))==0)
	//	{
	//		// 削除？
	//		sMergeData data = {5, (cyiTreeNode*)x->ref, 0};
	//		list->addref(yitoheap(&data, sizeof(data)));
	//		cnt++;
	//		continue;
	//	}
	//	cnt++;
	//}

	//if (list->count()==0)
	//{
	//	delete list;
	//	return 0;
	//}
	return list;
}


void cglsGoban::ReleaseMergeData(cyiList* list)
{
	if (list)
	{
		list->clear();
		delete list;
	}

	//int cnt = list->count();
	//for (int i=0; i<cnt; i++)
	//	yifree(list->removeEx(0));
}

// ------------------------------------------------------
cyiList* cglsGoban::GetMergeData(goid_t goid, char* file, Bool bIsNormalSgf)
{
	goid = goid==0?m_nCurrentGoId:goid;
	cyiGoEngine* eng = m_GoEnginez[goid];
	
	sGobanContentsDetail detail;

	cyiGoEngine::xDefaultDetail(&detail);
	detail.eContentsMode = bIsNormalSgf?eGCM_NORMAL:eGCM_JOSEKI_EDITOR;

	cyiGoEngine* eng2 = new cyiGoEngine();
	
	if (eng2->Create(file, &detail)==False)
	{
		return False;
	}

	cyiTreeNode* node = eng->GetCurrentNode();
	
	// まず、eng2もengと同じノードへ移動。。。
	cyiStack st;
	st.clear();
	cyiTreeNode* parent = node;

	eng2->Goto(MOVE_TOP, 0);
	cyiTreeNode* node2 = eng2->GetCurrentNode();

	while(parent)
	{
		st.push((yiAnyValue)parent);
		parent=parent->m_parent;
	}

	cyiTreeNode* p=0;
	while(st.pop((yiAnyValue&)p))
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)p->GetData();	

		if (info==0)
		{
			// top?
			continue;
		}

		cyiTreeNode* nodeFound=0;
		for (cyiNode* x = node2->GetChildList().GetNodeTopFrom(); x; x=x->np)
		{
			cyiTreeNode* child = (cyiTreeNode*)x->ref;
			cSgfStoneInfo* xChild = (cSgfStoneInfo*)child->GetData();
			if (info->isSameStone(*xChild)==True)
			{
				nodeFound=child;
				break;
			}
		}

		if (nodeFound==0)
		{
			return 0;
		}
		node2=nodeFound;

	}

	cyiList* list = xIsNodeSame(new cyiList(), eng->GetCurrentNode(), node2);

	return list;

}


// ------------------------------------------------------
cyiGoEngine* cglsGoban::GetCurrentGoEngine()
{
	return m_GoEnginez[m_nCurrentGoId];
}


// ------------------------------------------------------
void cglsGoban::SetSendStoneId(int id)
{
	m_nSendStoneId=id;
}

// ------------------------------------------------------
void cglsGoban::SetAddNewId(int id)
{
	m_nAddNewId=id;
}

// ------------------------------------------------------
void cglsGoban::FreeId(goid_t id)
{
	m_GoEnginez[id]=0;
}

// ------------------------------------------------------
void cglsGoban::ChangeMouseCursor(int type, Bool bIsForced)
{

	if (bIsForced==False &&
			m_prevMouseCursorType==type)
		return ;

	/*if (GetCursor()==m_hCurrentCursor)
	{
		return ;
	}*/

	//HWND hWndParent = GetParent(m_hWnd);
	//HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWndParent, GWL_HINSTANCE);
	m_prevMouseCursorType=type;
	m_hCurrentCursor = LoadCursorA(NULL/*m_hInstance*/, type==0?IDC_ARROW:IDC_HAND);
	HCURSOR hPrevCursor=SetCursor(m_hCurrentCursor);

	//CloseHandle(hPrevCursor);
}

#include "glsMessageBox.h"
#include "glsQuestionDialog.h"
// ------------------------------------------------------
int cglsGoban::ShowMessage(int id, const wchar_t* psaMessage, const wchar_t* pCaption, int btn, int notifycode)
{
	id=id?id:m_nCurrentGoId;
	cyiGoEngine* eng = m_GoEnginez[id];

	sGoPlayInfo* info = (sGoPlayInfo*)eng->GetExtraData();

	if (info->dlg)
	{
		if (info->dlg->IsCreated()==True)
		{
			if (info->dlg->IsButtonOnlyOK()==True)
			{
				info->dlg->Destroy();
				delete info->dlg;
				info->dlg=0;
			}
		}
		else
		{
				delete info->dlg;
				info->dlg=0;
		}
	}

	if (info->dlg==0)
	{
		cglsMessageBox* msg = cglsMessageBox::xNEW();

		Bool res = msg->Show(xID_MESSAGEBOX+id, this, psaMessage, pCaption, btn, &Font(L"Meiryo UI", 20*m_scale), 0);
		msg->SetNotifyCode(notifycode);
		info->dlg=msg;

		if (id!=m_nCurrentGoId)
		{
			info->dlg->Hide();
		}

		return res;
	}

	return False;
	//cglsMessageBox box;
	//return box.Show(m_parent, psaMessage, pCaption, btn, &Font(L"Meiryo", 24.0*m_scale));
}

// ------------------------------------------------------
int cglsGoban::MessageIdToGoId(int messageId)
{
	if (messageId<xID_MESSAGEBOX)
		return messageId;

	return messageId-xID_MESSAGEBOX;
}



// ------------------------------------------------------
//int cglsGoban::ShowQuestion(goid_t goid, int id)
//{
//	cglsQuestionDialog dlg;
//	return dlg.Show(m_parent/*this*/, id, &Font(L"Meiryo UI", 20*m_scale));
//}


// ------------------------------------------------------
void cglsGoban::PauseTimer(goid_t goid)
{
	//goid=goid==0?m_nCurrentGoId:goid;
	//cyiGoEngine* eng = m_GoEnginez[goid];

	//if (eng==0)
	//	return ;

	//if (eng->IsCurrentOrderBlack()==True)
	//{

	//}
}

// ------------------------------------------------------
void cglsGoban::PlayTimer(goid_t goid)
{

}

// ------------------------------------------------------
Bool cglsGoban::StartTimer(goid_t goid)
{
	goid=goid==0?m_nCurrentGoId:goid;
#ifndef NEW_STYLE

	if (this->IsCurrentBlack()==True)
	{
		sGoPlayInfo* info = (sGoPlayInfo*)GetExtraData(goid);//GetExtraData();
		info->black.startTime = cyiTime::CurrentmSec();
		m_playerBlack->StartTimer(goid);
	}
	else
	{
		sGoPlayInfo* info = (sGoPlayInfo*)GetExtraData(goid);//GetExtraData();
		info->white.startTime = cyiTime::CurrentmSec();
		m_playerWhite->StartTimer(goid);
	}
#endif
	return True;
}

// ------------------------------------------------------
unsigned int cglsGoban::StopTimer(goid_t goid, unsigned int nUsedTime)
{
#ifndef NEW_STYLE

	if (IsCurrentBlack()==True)
	{
		sGoPlayInfo* info = (sGoPlayInfo* )GetExtraData(goid);
		nUsedTime = (ImBlack(goid)==True)?cyiTime::CurrentmSec(info->black.startTime):nUsedTime;
		info->black.totalUsedTime+= nUsedTime;
		m_playerBlack->StopTimer(goid, info->black.totalUsedTime);
	}
	else
	{
		sGoPlayInfo* info = (sGoPlayInfo* )GetExtraData(goid);
		nUsedTime = (ImBlack(goid)==False)?cyiTime::CurrentmSec(info->white.startTime):nUsedTime;
		info->white.totalUsedTime+=nUsedTime;
		m_playerWhite->StopTimer(goid, info->white.totalUsedTime);
	}
#endif
	return nUsedTime;
}

// ------------------------------------------------------
void cglsGoban::SetTotalRemainTime(goid_t goid, int time, Bool bIsBlack)
{
#ifndef NEW_STYLE

	if (bIsBlack)
	{
		return m_playerBlack->SetTotalRemainTime(goid, time);
	}
	else
	{
		return m_playerWhite->SetTotalRemainTime(goid, time);
	}
#endif
}


// ------------------------------------------------------
int cglsGoban::GetTotalUsedTime(goid_t goid, Bool bIsBlack)
{
#ifndef NEW_STYLE

	if (bIsBlack)
	{
		return m_playerBlack->GetTotalUsedTime(goid);
	}
	else
	{
		return m_playerWhite->GetTotalUsedTime(goid);
	}
#endif
	return 0;
}


// ------------------------------------------------------
int cglsGoban::GetRemainTime(goid_t goid, Bool bIsBlack)
{
#ifndef NEW_STYLE

	if (bIsBlack)
	{
		return m_playerBlack->GetRemainTime(goid);
	}
	else
	{
		return m_playerWhite->GetRemainTime(goid);
	}
#endif
	return 0;
}

// ------------------------------------------------------
void cglsGoban::AppendMessage(goid_t id, const wchar_t* message, int size)
{
#ifndef NEW_STYLE

	if (id==0 ||
			id==m_nCurrentGoId)
	{
		m_textPannel->AppendText(message);
	}
#endif
}

static void xFindComment(cyiTreeNode* node, cyiList* list)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

	if (info!=0)
	{
		if (info->comment!=0 && info->comment[0]!=0)
		{
			list->addref(node);
		}
	}

	if (node->GetChildList().count()==0)
	{
		return ;
	}

	for (cyiNode* p = node->GetChildList().GetNodeTopFrom(); p; p=p->np)
	{
		cyiTreeNode* child = (cyiTreeNode*)p->data;
		xFindComment(child, list);
	}

	return ;
}

// ------------------------------------------------------
cyiList* cglsGoban::GetCommentList(goid_t id)
{
	id = id==0?m_nCurrentGoId:id;
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng==0)
		return 0;

	cyiTreeNode* node = eng->GetCurrentNode();

	if (node==0)
		return 0;

	cyiList* list = new cyiList();
	xFindComment(node, list);

	if (list->count()==0)
	{
		delete list;
		return 0;
	}
	return list;
}

// ------------------------------------------------------
void cglsGoban::ReleaseCommentList(cyiList* list)
{
	if (list)
	{
		delete list;
	}
}

// ------------------------------------------------------
Bool xIsProblem(cyiTreeNode* t, char* flags)
{
	int nBp=0, nWp=0, nShi=0, nCase=0, nKou=0;

	cSgfStoneInfo* info = (cSgfStoneInfo*)t->GetData();

	if (info==0)
		return False;

	if (info->exinfo==0)
	{
		if (t->GetChildList().count()!=0)
		{
			return False;
		}
		else
		{
			if (info->ref!=0)
			{
				return False;
			}
		}
	}
	else
	{
		if (t->GetChildList().count()!=0)
		{
			if (info->exinfo->option==0)
			{
				return False;
			}
		}
	}

	unsigned int option=info->exinfo?info->exinfo->option:eGRO_JOSEKI_OTHER;
	unsigned int status = info->exinfo?info->exinfo->status:eGRS_SAME;

	if (status==0)
	{
		status=eGRS_SAME;
	}

	if (option==0)
	{
		option=eGRO_JOSEKI_OTHER;
	}

	cyiTreeNode* parent=t;
	while(parent)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)parent->GetData();

		if (info==0)
		{
			parent=parent->m_parent;
			continue;
		}

		if (info->stone==eST_INVALID)
		{
			parent=parent->m_parent;
			continue;
		}

		if (info->exinfo==0)
		{
			parent=parent->m_parent;
			continue;
		}

		eStoneType type = STONE_TYPE(info->stone);
		if (info->exinfo->stonevalue==eSV_NOGOOD)
		{
			if (type==eST_BLACK || type==eST_BPASS)
			{
				nBp++;
			}
			else
			{
				nWp++;
			}
		}
		else if (info->exinfo->stonevalue==eSV_BAD ||
					info->exinfo->stonevalue==eSV_BADSUJI ||
						info->exinfo->stonevalue==eSV_QUESTION)
		{
			if (type==eST_BLACK || type==eST_BPASS)
			{
				nBp+=2;
			}
			else
			{
				nWp+=2;
			}
		}
		else if (type==eSV_CASE)
		{
			nCase++;
		}
		else if (type==eSV_SICHO ||
					type==eSV_NOSICHO)
		{
			nShi++;
		}
		else if (type==eSV_KOU)
		{
			nKou++;
		}
		parent=parent->m_parent;
	}

	flags[0]=nBp;
	flags[1]=nWp;
	flags[2]=nShi;
	flags[3]=nCase;
	flags[4]=nKou;

	if ((option&eGRO_JOSEKI_CASE)!=0)
	{
		if (nCase==0)
		{
			return True;
		}

		if (((option&eGRS_BLACK_OTHER)==0) && ((option&eGRS_WHITE_OTHER)==0))
		{
			return True;
		}
	}

	if ((option&eGRO_JOSEKI)!=0)
	{
		if (nBp!=0 ||
				nWp!=0)
		{
			return True;
		}
	}

	if (status==eGRS_SAME)
	{
		if (nBp!=0 ||
				nWp!=0)
		{
			return True;
		}
	}

	if (nShi!=0)
	{
		if ((option&eGRO_SICHOU)==0)
		{
			return True;
		}
	}

	//if (nKou!=0)
	//{
	//	if ((option&)==0)
	//	return True;
	//}


	return False;

}

typedef struct {
	cyiTreeNode* t;
	char flags[6];
} sXXXXX;
// ------------------------------------------------------
static void xFindProblem(cyiTreeNode* t, cyiList* list)
{
	char flags[6];
	if (xIsProblem(t, flags)==True)
	{
		sXXXXX* p = (sXXXXX*)yialloci(sizeof(*p));

		memcpy(p->flags, flags, 6);
		//p->flag=flag;
		p->t=t;
		list->addref(p);
		//list->addref(t);
	}

	if (t->GetChildList().count()==0)
	{
		return ;
	}

	for (cyiNode* p = t->GetChildList().GetNodeTopFrom(); p; p=p->np)
	{
		cyiTreeNode* child = (cyiTreeNode*)p->data;
		xFindProblem(child, list);
	}
}

// ------------------------------------------------------
cyiList* cglsGoban::GetProblemList(goid_t id)
{
	id = id==0?m_nCurrentGoId:id;
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng==0)
		return 0;

	cyiTreeNode* node = eng->GetCurrentNode();

	if (node==0)
		return 0;

	cyiList* list = new cyiList();
	xFindProblem(node, list);

	if (list->count()==0)
	{
		delete list;
		return 0;
	}
	return list;
}

// ------------------------------------------------------
void cglsGoban::ReleaseProblemList(cyiList* list)
{
	if (list)
	{
		delete list;
	}
}

// ------------------------------------------------------
cglsToolContainer* cglsGoban::GetContainer()
{
	return this->m_container;
}





// ------------------------------------------------------
void cglsGoban::SetRemainTime(goid_t goid, int time, Bool bIsBlack, Bool bIsMinus)
{
#ifndef NEW_STYLE
	if (bIsBlack)
	{
		m_playerBlack->SetRemainTime(goid, time, bIsMinus);
	}
	else
	{
		m_playerWhite->SetRemainTime(goid, time, bIsMinus);
	}
#endif
}

#ifdef NEW_STYLE
// ---------------------
void cglsGoban::xCreateCommonGobanControl()
{
	int n = MAX_BLOCK_SIZE*m_scale*0.75;
	Font* font = cyiShape::GetFontByHeight(n-2, L"Meiryo");
	
	m_controlz[eGCT_CONTROL] = new cglsControlBar();
	m_controlz[eGCT_CONTROL]->Create(this, 2, font, eGIA_BOTTOM, eGGS_NEON, 0);

	// 1. 一般的なコントロールを追加
	cglsGobanControl* control = new cglsGobanControl();
	control->Create(m_controlz[eGCT_CONTROL], eGCBID_BASE, m_w, n-2, eGCS_BASE, 0);
	control->Show();
	m_controlz[eGCT_CONTROL]->AddPage(control);

	control = new cglsGobanControl();
	control->Create(m_controlz[eGCT_CONTROL], eGCBID_EDIT, m_w, n-2, eGCS_EDITOR, 0);
	control->Show();
	m_controlz[eGCT_CONTROL]->AddPage(control);

	control = new cglsGobanControl();
	control->Create(m_controlz[eGCT_CONTROL], eGCBID_CALCULATE, m_w, n-2, eGCS_CALCULATE, 0);
	control->Show();
	m_controlz[eGCT_CONTROL]->AddPage(control);
	m_controlz[eGCT_CONTROL]->Update(eGUM_FORCED);

	m_controlz[eGCT_TITLE] = new cglsControlBar();
	m_controlz[eGCT_TITLE]->Create(this, 3, font, eGIA_TOP, eGGS_NEON_WHITE, 0);

	sGlsStaticDetail detail;
	cglsStatic::xDefaultDetail(&detail);

	detail.id=1;
	cglsStatic* label = new cglsStatic();
	label->Create((HWND)m_controlz[eGCT_TITLE], 0, 0, 0, 0, 0, L"四谷 定石辞典", &detail);
	label->Show();

	m_controlz[eGCT_TITLE]->AddPage(label);
	m_controlz[eGCT_TITLE]->Update(eGUM_FORCED);
	m_controlz[eGCT_TITLE]->Show();


	m_controlz[eGCT_VIEW] = 0;
	//m_controlz[eGCT_VIEW] = new cglsControlBar();
	//m_controlz[eGCT_VIEW]->Create(this, 4, font, eGIA_CENTER, eGGS_GAUSS, 0);
	xResizeGobanControl();

	delete font;

}

#endif

#include "yctrls.h"



#ifndef STATIC_COMPILE
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
		g_hInstanceGoban=g_hDllInstace=hinstDLL;
		return TRUE;
	}
	else if (fdwReason==DLL_PROCESS_ATTACH)
	{
		return TRUE;
	}
	return TRUE;
}

#endif