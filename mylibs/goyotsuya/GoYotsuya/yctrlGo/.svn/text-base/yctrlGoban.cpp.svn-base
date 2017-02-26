#include "yi.h"
#include "yiGoEngine.h"
#include "yctrlGoban.h"
#include "shellapi.h"
#include "yistd.h"
#include "yctrlGobanResource.h"
#include "resource.h"




cyctrlGoban::cyctrlGoban(void)
:	cyctrlbase(),
	m_datas(True)
{
	//m_OffGraphics=NULL;
}

cyctrlGoban::~cyctrlGoban(void)
{
}



 ULONG_PTR           gdiToken;
 GdiplusStartupInput gdiSI;

#define GO_ID_RESERVED ((cyiGoEngine*)1)


 void cyctrlGoban::InitMember()
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
 void cyctrlGoban::InitGraphics()
 {
	m_scale = (float)m_nGobanWidth/(float)MAX_GOBAN_SIZE;
	cyctrlGobanResource::GetInstance().Initialize(m_scale);
	int w = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN)->GetWidth();
	int h = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN)->GetHeight();

	for (int i=0; i<19; i++)
		m_scaled_point[i] = ((float)MAX_START_INDEX+(float)MAX_BLOCK_SIZE*(float)i)*m_scale;

	m_BgLayer[0]=new cyiSurface(w, h);
	m_BgLayer[1]=new cyiSurface(w, h);

	_DrawBackGroundLayer(True);
 }

#include "yctrlGobanResource.h"
// ----------------------------------------
BOOL cyctrlGoban::Create(long x, long y, long w, long h, long nLine, HWND hOwnerWnd)
{
	const LPCSTR pszWindowClass = "YotuyacYotsuyaGoCtr";
	WNDCLASSEX wcex;

	//m_hInstance=(HINSTANCE)GetWindowLong(hOwnerWnd, GWL_HINSTANCE);//hInstance;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= 0;//CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= (m_hInstance=(HINSTANCE)GetModuleHandle("yctrlGoban.dll"));//GetWindowLong(hOwnerWnd, GWL_HINSTANCE));
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

	m_hOwner=hOwnerWnd;

	m_w = w>h?h:w;
	m_h = w>h?h:w;

	m_x = x;
	m_y = y;

	m_nGobanWidth = m_w;

	m_hOwner = hOwnerWnd;
	m_nDefaultLineCount = nLine;
	m_hBgThread=0;
	m_aCurrentLabelList[0]=STONE_EOF;

	m_prevMouseCursorType=-1;


	m_prevX=-1;
	m_prevY=-1;

	m_bmpCurrentLabel=0;
	m_bmpCurrentLabelW=0;
	m_bmpCurrentLabelB=0;
	m_bLabelShow=True;
	m_eBranchViewMode=eBVM_NORMAL;


	m_branches.clear();


	memset(m_GoEnginez, 0, sizeof(m_GoEnginez));

	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		m_nCpuCount = info.dwNumberOfProcessors;
	}

	InitMember();
	// 描画時の排他処理の初期化
	DrawInitMutex();
	m_hDrawMutex = yiMutexCreateEx(3);

	m_hWnd = CreateWindow((LPCSTR)pszWindowClass, (LPCSTR)pszWindowClass, /*WS_CLIPSIBLINGS|*/WS_CHILD|WS_VISIBLE,
	x, y, w, h, hOwnerWnd, (HMENU)1, (HINSTANCE)m_hInstance, this);



	if (m_hWnd==NULL)
	{
		return FALSE;
	}

	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
	ChangeMouseCursor(0);

	// サウンドリソースロード
	HRSRC hResInfo = FindResource(GetModuleHandleA("yctrlGo.dll"), MAKEINTRESOURCEA(IDR_SOUND_MOVE), "WAVE");

	if (hResInfo)
	{
		m_soundMove = LockResource(::LoadResource(GetModuleHandleA("yctrlGo.dll"), hResInfo));
	}

	// TODO: ここでタブコントロールを生成

	if (m_hWnd!=NULL)
	{
		// リソースのロード
		//yictrl_initialize();
		InitGraphics();
		Update();
		SetWindowPos(m_hWnd, 0, x, y, m_w, m_w, SWP_SHOWWINDOW | SWP_NOZORDER);
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);
		return TRUE;
	}

	return FALSE;
}

// ------------------------------------------------------
void cyctrlGoban::Destroy()
{
	WaitForSingleObject(m_hBgThread, 3000);

	CloseHandle(m_hBgThread);
}


// ------------------------------------------------------
// 対局時に碁盤を追加するときに呼び出される。
// 注意すべきは、goid_tは、下位16ビットがローカルのIDであり、上位16ビットはリモートのIDである。
// リモートのIDがゼロの場合は失敗
goid_t cyctrlGoban::AddForPlay(goid_t reservedId, const char* pszBlackName, 
										const char* pszWhiteName, const char* pszTitle, 
											unsigned int condition, unsigned int flag)
{
	if (HIWORD(reservedId)==0)
	{
		return 0;
	}

	sGoHeader* header = (sGoHeader*)yialloci(sizeof(sGoHeader));

	if (pszBlackName)
		header->pszBlackName = yistrheap(pszBlackName);

	if (pszWhiteName)
		header->pszWhiteName = yistrheap(pszWhiteName);

	if (pszTitle)
		header->pszEventName = yistrheap(pszTitle);

	header->condition = condition;
	header->flag32 = flag;

	return Add(eInputMode::eIM_PLAY, eMouseMode::eMM_RBUTTON_RETURN, header, reservedId);
}

// ------------------------------------------------------
// ファイルからの読み込み
//
goid_t cyctrlGoban::AddForEditByFile(const char* pszFileName, eGoFileType type)
{
	cyiGoEngine* eng = new cyiGoEngine();

	if (eng->Create(pszFileName, type)==False)
	{
		return 0;
	}

	//eng->SetInputMode(eIM_ENJOY);
	eng->SetInputMode(eIM_EDIT);
	eng->SetMouseMode(eMM_RBUTTON_RETURN);

	goid_t id = ReserveId();
	m_GoEnginez[id]=eng;

	ChangeCurrentGoban(id);

	PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());

	//SetBranch(eng);
	Update(eGL_ALL, eng->GetStoneMap());


	m_nGobanCount++;

	PostMessage(m_hOwner, WM_GOBAN_ADDED, 0, (LPARAM)id);
	return id;
}

#define THREAD_ID_SEARCH 1
#define THREAD_ID_OPTIMIZE 2

// ------------------------------------------------------
void cyctrlGoban::OnThreadStart(int id, int param1, int param2)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return ;

	if (id==THREAD_ID_SEARCH)
	{
		eng->SearchResult();
		PostMessage(m_hOwner, WM_QUIT_SEARCHRESULT, 0, (LPARAM)eng->GetCurrentNode());
	}
	else if (id==THREAD_ID_OPTIMIZE)
	{
		int n = eng->OptimizeNode((int*)param1, (Bool*)param2);
		PostMessage(m_hOwner, WM_QUIT_OPTIMIZE, n, 0);

		if (n)
		{
			SetChangedFlag(True);
		}
	}
}

// ------------------------------------------------------
void cyctrlGoban::SearchResult()
{
	StartThread(THREAD_ID_SEARCH);
}


// -----------------------------------
int cyctrlGoban::OptimizeNode(int* nProgress, Bool* canceled)
{
	StartThread(THREAD_ID_OPTIMIZE, (int)nProgress, (int)canceled);
	return 0;
}

// -----------------------------------
int cyctrlGoban::GetFullNodeCnt()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return 0;
	return eng->GetFullNodeCnt();
}

// -----------------------------------
Bool cyctrlGoban::IsChanged()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return False;

	return eng->IsChanged();
}

// -----------------------------------
void cyctrlGoban::SetChangedFlag(Bool bIsChanged)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return ;

	eng->SetChangedFlag(bIsChanged);
}


// ------------------------------------------------------
void cyctrlGoban::CreateMapIamge(cyiTreeNode* node)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return ;

	eng->CreateMapImage(node);
}

// ------------------------------------------------------
goid_t cyctrlGoban::AddForEdit(const char* pszBlackName, const char* pszWhiteName, 
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

	return Add(eIM_EDIT, eMM_RBUTTON_RETURN, header);
}


// ------------------------------------------------------
// 碁盤を追加
// SGFファイルを開くとき、対局開始時、などなど
// 失敗時は、ゼロ。
goid_t cyctrlGoban::Add(eInputMode input, eMouseMode mouseMode, sGoHeader* header, goid_t reservedId)
{
	if (m_nGobanCount==MAX_GOBAN_COUNT)
	{
		return 0;
	}

	cyiGoEngine* eng = new cyiGoEngine();

	if (eng==NULL)
		return -1;
	
	goid_t id = LOWORD(reservedId) ? LOWORD(reservedId):ReserveId();

	if (id==0)
		return 0;

	eng->Create(header);
	eng->SetInputMode(input);
	eng->SetMouseMode(mouseMode);

	eng->SetRemoteId(HIWORD(reservedId));

	m_GoEnginez[id]=eng;

	ChangeCurrentGoban(id);

	PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
	//Update(eGL_ALL, eng->GetStoneMap());
	Update();

	m_nGobanCount++;

	PostMessage(m_hOwner, WM_GOBAN_ADDED, 0, (LPARAM)id);
	return LOWORD(id);
}

// -----------------------------------------
// ロケール変更により、再描画が行われる。
void cyctrlGoban::ChangeLocale(goid_t id, eCharacterSet eLocale/*=eCS_JAPANESE*/)
{
	m_GoEnginez[id]->SetLocale(eLocale);
	Update();

	// TODO: 
	// UpdateControl();
	// UpdateViewer();
	::PostMessage(m_hOwner, WM_LOCALE_CHANGED, (WPARAM)id, (LPARAM)eLocale);
}

// ------------------------------------------------------
// 碁盤削除
void cyctrlGoban::Remove(goid_t id)
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
unsigned int cyctrlGoban::GetCount()
{
	return m_nGobanCount;
}

// -----------------------------------------
goid_t cyctrlGoban::GetCurrentGoId()
{
	return m_nCurrentGoId;
}


// ------------------------------------------------------
void cyctrlGoban::ShowNo(Bool bIsTrue, int nFrom/*=0*/)
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

// -------------------------------
// 石を置く
stone_t** cyctrlGoban::PutStone(goid_t id, stone_t stone)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];

	if (!eng)
		return 0;

	if (stone==STONE_PASS)
	{
		stone_t x = eng->GetLastStone();
		eStoneType type  = STONE_TYPE(x);

		if (type==STONE_BLACK ||
				type==STONE_BLACK_PASS)
		{
			stone=MAKESTONE(STONE_WHITE_PASS, 0, 0, 0);
		}
		else if (type==STONE_WHITE ||
					type==STONE_WHITE_PASS)
		{
			stone=MAKESTONE(STONE_BLACK_PASS, 0, 0, 0);
		}
		else
			return 0;
	}

	aStoneList list;
	stone_t** stonez = eng->PutStone(stone, list);

	if (stonez!=0)
	{
		::PlaySound((LPCSTR)m_soundMove, GetModuleHandleA("yctrlGo.dll"), SND_ASYNC|SND_MEMORY);
	}

	if (id==m_nCurrentGoId)
	{
		// あげ石がある場合、通知する。
		if (stone!=0 && 
			list[0]!=STONE_EOF)
		{
			int nBlack, nWhite;
			eng->GetDeadStone(nBlack, nWhite);
			PostMessage(m_hOwner, WM_CHANGE_DEAD_STONE, id, MAKELONG(nBlack, nWhite));
		}

		PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());


		//SetBranch(eng);

		// 描画する。
		Update(eGL_STONE, stonez);
	}

	return stonez;
}

// -------------------------------
void cyctrlGoban::PlayByNode(cyiTreeNode* node)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return ;

	stone_t** stone = eng->PlayByNode(node);

	if (stone==0)
		return ;

	::PlaySound((LPCSTR)m_soundMove, GetModuleHandleA("yctrlGo.dll"), SND_ASYNC|SND_MEMORY);

	int nBlack, nWhite;
	eng->GetDeadStone(nBlack, nWhite);
	PostMessage(m_hOwner, WM_CHANGE_DEAD_STONE, m_nCurrentGoId, MAKELONG(nBlack, nWhite));
	PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());

	Update(eGL_ALL, stone);
}

// --------------------------------------
cyiTreeNode* cyctrlGoban::GetCurrentNode()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return False;

	return eng->GetCurrentNode();
}

// -------------------------------
// ラベルを置く
Bool cyctrlGoban::PutLabel(goid_t id, int i, int j,wchar_t label)
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
cyiList* cyctrlGoban::GetLabel(goid_t id)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (!eng)
		return 0;


	return eng->GetLabel();
}

// -------------------------------
// コメントを入力
Bool cyctrlGoban::PutComment(goid_t id, LPCSTR comment)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (!eng)
		return False;

	return eng->PutComment(comment);
}

// -------------------------------
// コメントを取得
LPCSTR cyctrlGoban::GetComment(goid_t id, LPCSTR comment)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (!eng)
		return 0;

	return (const char*)eng->GetComment();
}

// --------------------------------
stone_t** cyctrlGoban::Goto(goid_t id, int nIndex, aStoneList branches)//, eInputMode mode/*=eIM_DEFAULT*/)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (!eng)
		return 0;

	m_branches.clear();

	stone_t** stone=eng->Goto(nIndex, branches);

	if (stone==0)
	{
		return 0;
	}


	if (id==m_nCurrentGoId)
	{
		//SetBranch(eng);
		if (stone!=0)
		{
			Update(eGL_ALL, stone);
		}
		PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
		return stone;
	}

	if (stone!=0)
	{
		Update(eGL_ALL, stone);
	}
	return stone;
}


// --------------------------------
Bool cyctrlGoban::StartFindComment()
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
Bool cyctrlGoban::EndFindComment()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return 0;

	eng->CloseCommentList();
}

// --------------------------------
stone_t** cyctrlGoban::GotoNextComment(int nIndex/*=-1*/)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return 0;

	stone_t** stone= eng->GotoNextComment(nIndex);

	PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
	Update(eGL_ALL, stone);

	return stone;
}

// --------------------------------
stone_t** cyctrlGoban::GotoPrevComment()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return 0;

	stone_t** stone = eng->GotoPrevComment();

	PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
	Update(eGL_ALL, stone);

	return stone;
}




// --------------------------------
stone_t** cyctrlGoban::GotoNextBranch(goid_t id, aStoneList branches)
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
		PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
		Update(eGL_ALL, stone);
	}

	return stone;
}

// --------------------------------
stone_t** cyctrlGoban::GotoPrevBranch(goid_t id, aStoneList branches)
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
		PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
		Update(eGL_ALL, stone);
	}

	return stone;


}

// --------------------------------
void cyctrlGoban::FreeInitStoneMode(int count)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return ;

	if (eng->GetLastStone()==STONE_INVALID)
		eng->StartInitStone(count);
}



// ------------------------------------------------------
void cyctrlGoban::DeleteNode(cyiTreeNode* node)
{

	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return ;

	eng->DeleteNode();

	// この場合は、PostMessageではない。
	// 緊急
	SendMessage(m_hOwner, WM_NODE_CHANGED, 1, (LPARAM)eng->GetCurrentNode());
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
// WM_CHANGE_GOBAN時の処理
// WPARAM : 
void cyctrlGoban::ChangeCurrentGoban(goid_t id)
{
	cyiGoEngine* eng = m_GoEnginez[id];
	if (eng==0)
		return ;
	
	m_nCurrentGoId=id;
	//Update();

	// TODO: タブコントロールのフォーカス
	// SetTabFocus(id);
}


// ------------------------------------------------------
LPCSTR cyctrlGoban::GetTitle(goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return 0;

	return (LPCSTR)eng->GetTitle();//GetHeader()->pszEventName;
}

// ------------------------------------------------------
LPCSTR cyctrlGoban::GetBlackName(goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return 0;

	return (LPCSTR)eng->GetHeader()->pszBlackName;
}

// ------------------------------------------------------
void cyctrlGoban::SetBlackName(LPCSTR name, goid_t id/*=0*/)
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
LPCSTR cyctrlGoban::GetWhiteName(goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return 0;
	return (LPCSTR)eng->GetHeader()->pszWhiteName;
}

// ------------------------------------------------------
void cyctrlGoban::SetWhiteName(LPCSTR name, goid_t id/*=0*/)
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
unsigned int cyctrlGoban::GetDetailInfo(goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return 0;
	return eng->GetHeader()->condition;
}

// ------------------------------------------------------
void cyctrlGoban::SetDetailInfo(unsigned int condition, goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return ;

	sGoHeader* header = eng->GetHeader();
	eng->GetHeader()->condition = condition;
}

// ------------------------------------------------------
unsigned int cyctrlGoban::GetDetailFlag(goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return 0;

	return eng->GetHeader()->flag32;
}

// ------------------------------------------------------
void cyctrlGoban::SetDetailFlag(unsigned int flag, goid_t id/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[id?id:m_nCurrentGoId];
	if (eng==0)
		return ;

	sGoHeader* header = eng->GetHeader();
	eng->GetHeader()->flag32= flag;
}

//----------------------------------------
void cyctrlGoban::Play()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	if (eng==0)
		return ;

	m_branches.clear();

	aStoneList list;
	stone_t** stone = eng->Goto(1, list);

	//SetBranch(eng);

	if (stone)
	{
		PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
		Update(eGL_ALL, stone);
	}
}

// ----------------------------------------
Bool cyctrlGoban::Save(const char* pszFileName, eGoFileType type/*=eGFT_SGF*/, goid_t _id/*=0xFFFFFFFF*/)
{
	goid_t id = _id==0xFFFFFFFF?m_nCurrentGoId:_id;
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng==0)
		return False;

	return eng->Save(pszFileName, type);
}


//----------------------------------------
BOOL cyctrlGoban::Resize(long w, long h)
{
	return Resize(m_x, m_y, w, h);
}

//----------------------------------------
BOOL cyctrlGoban::Resize(int x, int y, long w, long h)
{
	m_w = w>h?h:w;
	m_h = w>h?h:w;

	if (m_x==x && m_y==y)
	{
		::SetWindowPos(m_hWnd, 0, x, y, m_w, m_w, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		::SetWindowPos(m_hWnd, 0, x, y, m_w, m_w, SWP_SHOWWINDOW |  SWP_NOZORDER);
		m_x=x;
		m_y=y;
	}

	this->m_nGobanWidth=m_w;
	// TODO: イメージのスケール変更を行う。
	AdjustScale();


	// メッセージなどが表示されている場合の処理を行う。

	// 描画ルーチンを走らせる。
	//Update(eGL_ALL);
	return FALSE;
}

// -------------------------------------------------------
void cyctrlGoban::CalculateMode(goid_t id)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng==0)
		return ;

	stone_t** stone=eng->StartCalculate();

	Update(eGL_CALCULATE, stone);
}

// ------------------------------------------------------------------
stone_t** cyctrlGoban::CalculateEnd(goid_t id, int* xnBlack, int* xnWhite)
{
	cyiGoEngine* eng = m_GoEnginez[id];

	if (eng==0)
		return 0;

	sResult result;
	stone_t** stone = eng->EndCalculate(&result);
	Update(eGobanLayer::eGL_CALCULATE_END, stone);

	char buffer[256];
	float b_a=result.nBlackArea+result.nWhiteDeadStone-result.nWhiteArea-result.nBlackDeadStone-6.5;
	wsprintf(buffer, "black: %d+%d=%d \r\nwhite: %d+%d+6.5=%0.2f\r\n%s win %0.2f", 
			result.nBlackArea, result.nWhiteDeadStone, result.nBlackArea+result.nWhiteDeadStone,
			result.nWhiteArea, result.nWhiteDeadStone, (float)((float)result.nWhiteArea+result.nBlackDeadStone+6.5), b_a<0?"White":"Black", (float)-b_a);

	MessageBox(m_hWnd, buffer, "result", MB_OK);
	return stone;

}



// ---------------------------------------------------------------
Bool cyctrlGoban::GetNearCoordinate(int x, int y, POINT* ps, int nLineCnt)
{
	float w;
	float b;
	float s;

	cyctrlGobanResource::GetInstance().GetScaledSize(w, s, b);

	float nx = ((float)x/*/m_scale*/ - s)/b;
	float ny = ((float)y/*/m_scale*/ - s)/b;

	ps->x = __INTEGER__(nx);//(nx-(float)(int)nx>0.5)? (int)nx+1:(int)nx;
	ps->y = __INTEGER__(ny); //(ny-(float)(int)ny>0.5)? (int)ny+1:(int)ny;

	if (ps->x > nLineCnt-1 ||
			ps->y > nLineCnt-1)
	{
		return False;
	}

	return True;
}

// -------------------------------------------------------
void cyctrlGoban::ChangeInputMode(eInputMode input)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
	{
		return ;
	}

	eng->SetInputMode(input);
}

// -------------------------------------------------------
UINT cyctrlGoban::GetSize()
{
	return m_nGobanWidth;
}

	

// ------------------------------------------------------
// 番号表示	
//void cyctrlGoban::ShowNo(Bool bIsTrue, int nFrom/*=0*/)
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
int cyctrlGoban::ShowMessage(unsigned int id, const wchar_t* message, int flag, int timeout)
{
	return 0;
}


///// private

// -------------------------------------------------------
void cyctrlGoban::DrawInitMutex()
{
	memset(&m_drawMutex, 0, sizeof(m_drawMutex));
	InitializeCriticalSectionAndSpinCount(&m_drawMutex, 5);
}

// -------------------------------------------------------
void cyctrlGoban::DrawMutexLock()
{
	EnterCriticalSection(&m_drawMutex);
}

// -------------------------------------------------------
void cyctrlGoban::DrawMutexUnlock()
{
	LeaveCriticalSection(&m_drawMutex);
}

// ------------------------------------------------------
goid_t cyctrlGoban::ReserveId()
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
void cyctrlGoban::FreeId(goid_t id)
{
	m_GoEnginez[id]=0;
}

// ------------------------------------------------------
void cyctrlGoban::ChangeMouseCursor(int type)
{
	if (m_prevMouseCursorType==type)
		return ;

	m_prevMouseCursorType=type;
	HCURSOR hCursor = LoadCursorA(m_hInstance, type==0?IDC_ARROW:IDC_HAND);
	HCURSOR hPrevCursor=SetCursor(hCursor);

	//CloseHandle(hPrevCursor);
}

#ifdef _WINDLL


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
		return TRUE;
	}
	else if (fdwReason==DLL_PROCESS_ATTACH)
	{
		return TRUE;
	}
	return TRUE;
}

#endif