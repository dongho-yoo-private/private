#include "GoEditorApp.h"
#include "MyMainFrame.h"
#include "yiGoCommandz.h"

#include "glsMessageBox.h"
#ifdef _GOYOTSUYA
#include "yigLoginDialog.h"
#include "yigUserDialog.h"
#include "yigRegisterDialog.h"
#include "yigNoIdDialog.h"
#include "glsMyView.h"
#include "glsMyFriendView.h"
#endif

#define __FONT_MESSAGEBOX__ &Font(L"Meiryo UI", 12.0)

static int xGetJosekiName(char* pszJosekiName, cyiTreeNode* node, int& _h, int& _l, int& _d);

CGoEditorApp::CGoEditorApp(const char* pszSoftwareName, const char* pszApplicationName, Bool bIsMultiple)
:cyiGoApp(pszSoftwareName, pszApplicationName, bIsMultiple)
, m_system(0)
#ifdef _GOYOTSUYA
, m_network(0)
, m_bIsLoginSuccess(False)
, m_bIsNoUser(True)
, m_user(0)
, m_bIsLoginNormal(False)
, m_server_addr(0)
, m_myId(0)
, m_goHeaders(0)
, m_hUpnp(0)
, m_bIsFindRouterDone(False)
, m_myAbsAddr(0)
#endif
{
#ifdef _GOYOTSUYA
	m_szUserDir[0]=0;
	m_szMyDir[0]=0;
	m_dictionary.n=0;
	m_dictionary.user=0;
	m_szMyId[0]=0;
	m_goHeaderSize=0;
#endif
}

CGoEditorApp::~CGoEditorApp(void)
{
}

GdiplusStartupInput __g_gdiSI__;
ULONG_PTR           __g_gdiToken__;

// ------------------------
Bool CGoEditorApp::Initialize(const char* pszCommandLine)
{
	::YIINFO("CGoEditorApp::Initialize() called");
	// gdi+初期化
	GdiplusStartup(&__g_gdiToken__, &__g_gdiSI__, NULL);

	// winsock2.2 初期化
	yiInitWinsock(2, 2);
	Bool res = __super::Initialize(pszCommandLine);

	// 起動時にディレクトリ作成
	char path[256];

#ifdef JOSEKI_DICTIONARY
	wsprintf(path, "%s\\data", m_szInstDir);
	yiMakeDir(path);
#endif

#ifdef _GOMAIL
	wsprintf(path, "%s\\data", m_szInstDir);
	yiMakeDir(path);
#endif

#ifdef _GOYOTSUYA
	// -----------------------------------
	if (GetProperty(0, "UserDir", m_szUserDir)==False)
	{
		wsprintf(m_szUserDir, "%s\\users", m_szInstDir);
	}

	yiMakeDir(m_szUserDir);

	wsprintf(path, "%s\\data", m_szUserDir); 
	yiMakeDir(path);

	wsprintf(path, "%s\\data\\other", m_szUserDir); 
	yiMakeDir(path);
#endif



	Bool bRegister=False;
	//HKEY hKey = ::yiUtilRegOpenKeyForRead(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\PreviewHandlers");
	char buffer[256]={0, };
	DWORD n=sizeof(buffer);
	DWORD type;

#ifdef _GOYOTSUYA
	if(RegGetValueA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\PreviewHandlers", 
		"{0E64B778-416D-458b-B777-700EBA8D04C7}", RRF_RT_ANY, &type, buffer, &n)!=ERROR_SUCCESS)
	{
		if (MessageBox(::GetDesktopWindow(), "GoYotsuya プレビューが登録されていません。\r\n登録しますか？", "お知らせ", MB_YESNO)==IDYES)
		{
			bRegister=True;
		}
	}
	else if (buffer[0]==0)
	{
		if (MessageBox(::GetDesktopWindow(), "GoYotsuya プレビューが登録されていません。\r\n登録しますか？", "お知らせ", MB_YESNO)==IDYES)
		{
			bRegister=True;
		}
	}

	if (bRegister)
	{
		::ShellExecuteA(NULL, "open", "regsvr32", "gyp.dll", NULL, SW_SHOWNORMAL);
	}
#endif
	// 臨時的に変更
	return True;
}

#ifdef _GOYOTSUYA
// ------------------------
Bool CGoEditorApp::GetPropertyUser(const char* Key, char* value)
{
	return cyiGoApp::GetAppAttributeStr(m_szMyDir, CUserManager::GetInstance()->GetMyName(), Key, value);
}

// ------------------------
Bool CGoEditorApp::GetPropertyUserInt(const char* Key, int& value, int defaultValue)
{
	return cyiGoApp::GetAppAttributeInt(m_szMyDir, CUserManager::GetInstance()->GetMyName(), Key, &value, defaultValue);
}

// ------------------------
Bool CGoEditorApp::SetPropertyUser(const char* Key, char* value)
{
	return cyiGoApp::SetAppAttributeStr(m_szMyDir, CUserManager::GetInstance()->GetMyName(), Key, value);
}

// ------------------------
Bool CGoEditorApp::SetPropertyUserInt(const char* Key, int n)
{
	return cyiGoApp::SetAppAttributeInt(m_szMyDir, CUserManager::GetInstance()->GetMyName(), Key, n);
}
#endif

// ------------------------
//void CGoEditorApp::OnApplicationDuplicated(HWND hWnd)
//{
//	return ;
//}

cglsPopupMenu* CGoEditorApp::xCreateMenu()
{
	cglsPopupMenu* m_gobanMenu=new cglsPopupMenu();
	m_gobanMenu->CreateFromResource(m_hWnd, IDR_GOBAN_TOP);

	cglsSystem* system = m_main->GetSystem();

	if (system)
	{
		system->RegisterCommand(m_gobanMenu, eGCID_CALCULATE_START, IDM_CALCULATE_START);
		system->RegisterCommand(m_gobanMenu, eGCID_LOGIN, IDM_LOGIN);
		system->RegisterCommand(m_gobanMenu, eGCID_REGISTER_USER, IDM_REGISTER_USER);
		system->RegisterCommand(m_gobanMenu, eGCID_REQ_PLAY_DIALOG, IDM_REQUEST_PLAY);
		system->RegisterCommand(m_gobanMenu, eGCID_ADD_NEW, IDC_CREATE_NEW);
		system->RegisterCommand(m_gobanMenu, eGCID_CURRENT_SAVE, IDC_SAVE);
		system->RegisterCommand(m_gobanMenu, eGCID_CURRENT_SAVE_AS, IDC_SAVE_AS);
		system->RegisterCommand(m_gobanMenu, eGCID_COPY, IDC_COPY);

	}

	return m_gobanMenu;
}

// --------------------------------------
void CGoEditorApp::OnNodeChanged(WPARAM wParam, LPARAM lParam)
{
	if (wParam==0) 
	{
		// 1. まず、定石名を調べる。
		cyiTreeNode* t = (cyiTreeNode*)lParam;
		int h, l, d;
		char szJosekiName[128];
		szJosekiName[0]=0;
		xGetJosekiName(szJosekiName, t, h, l, d);

		wchar_t swzJosekiName[128];
		yiToUnicode(swzJosekiName, szJosekiName, 0);

		const wchar_t* p = m_goban->GetSubTitle();

		if (swzJosekiName[0]==0)
		{
#ifdef SPECIAL_EDITION

			wcscpy(swzJosekiName, L"四谷定石辞典 -星限定版-");
#else
			wcscpy(swzJosekiName, L"四谷定石辞典");
#endif

		}

		if (wcscmp(swzJosekiName, p)!=0)
			m_goban->SetSubTitle(swzJosekiName);

		// 2. コメントがあるかないかを見る。
		// コメント、もしくは結果がある場合、表示。

		//InvalidateRect(0, True, True);
	}	
}


// ------------------------
Bool CGoEditorApp::OnCreate(HWND _hWnd)
{
	RECT rect;
	cGlassInterface* obj = (cGlassInterface*)_hWnd;
	HWND hWnd = obj->m_hWnd;

	m_main=obj;
	m_system=m_main->GetSystem();
	GetClientRect(hWnd, &rect);
	int nWidth=rect.right > rect.bottom ? rect.bottom:rect.right;

	m_hWnd = hWnd;

	cglsPopupMenu* m_gobanMenu=xCreateMenu();
	/*new cglsPopupMenu();
	m_gobanMenu->CreateFromResource(m_hWnd, IDR_GOBAN_TOP);

	cglsSystem* system = GetSystem();

	if (system)
	{
		system->RegisterCommand(m_detail.menu, eGCID_CALCULATE_START, IDM_CALCULATE_START);
	}
	*/

	sGobanDetail detail;
	CGoEditorApp* app = xGetMyApp();

	detail.menu=m_gobanMenu;

	int nLevel;
	GetPropertyInt(0, "PerformanceLevel", nLevel, 5);

	cglsSystem* system = m_main->GetSystem();
	system->SetPerformance(nLevel);

	InitailizeHotKey();
	
	// 碁盤生成
	cglsGoban::xDefaultDetail(&detail);
	m_goban=new cglsGoban();

	char buffer[128];
	if (app->GetPropertyInt(0, "nDefaultLineCount", detail.nDefaultLineCount)==False)
	{
		app->SetProperty(0, "bIsDefaultLabelShow", detail.bIsDefaultLabelShow?"True":"False");
//		app->SetProperty(0, "bIsJosekiEditorMode", detail.bIsJosekiEditorMode?"True":"False");
		app->SetProperty(0, "bIsPreviewClikFocus", detail.bIsPreviewClikFocus?"True":"False");
		app->SetProperty(0, "bIsPreviewGotoSame", detail.bIsPreviewGotoSame?"True":"False");
		app->SetProperty(0, "bIsPreviewNewTab", detail.bIsPreviewNewTab?"True":"False");
		app->SetProperty(0, "bIsPreviewShow", detail.bIsPrevewShow?"True":"False");
		app->SetProperty(0, "bIsPreviewPopup", detail.bIsPreviewPopup?"True":"False");

		//app->SetPropertyInt(0, "nBranchViewMode", detail.nBranchViewMode);
		app->SetPropertyInt(0, "nDefaultLineCount", detail.nDefaultLineCount);
		
		detail.mode=eGAM_NORMAL;

		m_goban->RegisterNodeChangeNotifyWindow(*m_main);

#ifdef JOSEKI_DICTIONARY
		detail.mode = eGAM_JOSEKI_PLAYER;
		//detail.bIsJosekiEditorMode=True;
		m_goban->Create((HWND)obj, 0, 0, rect.right, rect.bottom, 0, &detail);
		
#endif

#ifdef _GOMAIL
		detail.mode = eGAM_GO_MAIL;
#endif
#ifdef _GOYOTSUYA
		m_goban->Create((HWND)obj, 0, 0, rect.right, rect.bottom, CUserManager::GetInstance(), &detail);
#endif

	}
	else
	{
		detail.bIsPreviewGotoSame=False;
		if (app->GetProperty(0, "bIsPreviewGotoSame", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsPreviewGotoSame=True;
			}
		}
		detail.bIsDefaultLabelShow=False;
		if (app->GetProperty(0, "bIsDefaultLabelShow", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsDefaultLabelShow=True;
			}
		}
		//detail.bIsJosekiEditorMode=False;
		//if (app->GetProperty(0, "bIsJosekiEditorMode", buffer)==True)
		//{
		//	if (stricmp(buffer, "True")==0)
		//	{
		//		detail.bIsJosekiEditorMode=True;
		//	}
		//}
		detail.bIsPreviewClikFocus=False;
		if (app->GetProperty(0, "bIsPreviewClikFocus", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsPreviewClikFocus=True;
			}
		}
		detail.bIsPreviewNewTab=False;
		if (app->GetProperty(0, "bIsPreviewNewTab", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsPreviewNewTab=True;
			}
		}
		detail.bIsPrevewShow=False;
		if (app->GetProperty(0, "bIsPreviewShow", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsPrevewShow=True;
			}
		}
		detail.bIsPreviewPopup=False;
		if (app->GetProperty(0, "bIsPreviewPopup", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsPreviewPopup=True;
			}
		}

		m_goban->RegisterNodeChangeNotifyWindow(*m_main);
		//detail.nBranchViewMode=(eBranchViewMode)0;
		//app->GetPropertyInt(0, "nBranchViewMode", (int&)detail.nBranchViewMode, 0);
#ifdef JOSEKI_DICTIONARY
		detail.mode = eGAM_JOSEKI_EDITOR;
		//detail.bIsJosekiEditorMode=True;
		m_goban->Create((HWND)obj, 0, 0, rect.right, rect.bottom, 0, &detail);
#endif

#ifdef _GOMAIL
		detail.mode = eGAM_GO_MAIL;
#endif
#ifdef _GOYOTSUYA
		m_goban->Create((HWND)obj, 0, 0, rect.right, rect.bottom, CUserManager::GetInstance(), &detail);
#endif

		m_goban->SetSendStoneId(eGCID_SEND_STONE);
		m_goban->SetAddNewId(eGCID_ADD_NEW);

	}

	sGobanContentsDetail josekidetail;
#ifdef JOSEKI_DICTIONARY
	goid_t id=0;

	cyiGoEngine::xDefaultDetail(&josekidetail);
	josekidetail.eBranchViewModeValue=eBVM_SHOW_ONE_NODE;
	josekidetail.eContentsMode=eGCM_JOSEKI_EDITOR;
	josekidetail.eFormat = eGoFileType::eGFT_BIN;
	josekidetail.eInputModeValue = eIM_ENJOY;

	YITRACE(6, "before joseki load.....");
	int size=0;
	void* p = yiFileLoad(".\\data\\joseki.bin", &size);

	if (p==0)
	{
		return False;
	}


	HandleArchive hArch = yiArchiveCreate(p, size);

	yifree(p);
	if (hArch==0)
	{
		return 0;
	}

	p = (char*)yiArchiveEnum(hArch, 0);

	if (memcmp(p, "BGF\0", 4)!=0)
	{
		return False;
	}

	// check licese.


	int* pData = (int*)yiArchiveEnum(hArch, 1);
	if (pData[0]!=1)
	{
		return False;
	}

	cyiMemoryStream* stream = yiUtilDeCompress(&pData[1], xBIN_SIZE(pData)-sizeof(int));

	if (stream==0)
	{
		yiArchiveClose(hArch);
		return False;
	}
	//yiArchiveClose(hArch);
	
	p = stream->ExportToBinary();
	id = m_goban->AddForEditByFile(p, xBIN_SIZE(p), &josekidetail);
	m_goban->ShowCursor(True);


	yiBinaryDelete(p);
	//yifree(p);
	delete stream;

	// ここで付加データを処理する。
	//int i=2;
	//do
	//{
	//	int* pData = (int*)yiArchiveEnum(hArch, i++);

	//	if (pData==0)
	//		break;
	//	
	//	// お気に入り
	//	if (*pData==2)
	//	{
	//		m_goban->UpdateNode(&pData[1], UN_FAV);
	//	}
	//	// 追加データ
	//	else if (*pData==0xFF)
	//	{
	//		m_goban->UpdateNode(&pData[1], UN_NEW);
	//	}

	//} while(pData);

	yiArchiveClose(hArch);

	/*sSoftwareLisenseCode license;
	if (m_goban->GetLicenseCode(".\\data\\joseki.bin", &license)==True)
	{
		id = m_goban->AddForEditByFile(".\\data\\joseki.bin", &josekidetail);//eGoFileType::eGFT_SGF, eGNT_JOSEKI_DICTIONARY_EDITOR);
	}*/	
	if (id==0)
	{
		YITRACE(6, "before joseki load..... failure!");
		return False;
	}
	YITRACE(6, "before joseki load..... success !");

#endif


#ifdef _GOMAIL
	// pop3でメールを読み込む。
#endif

	return True;
}

// ------------------------
void CGoEditorApp::OnMainWindowCreateAfter(const char* pszCommandLine)
{
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
			m_goban->AddForEditByFile(pszTemp);
		}
	}

	Bool bLoginDialog=True;
	char buffer[128];
	//// -----------------------------------
	//if (GetProperty(0, "UserDir", m_szUserDir)==False)
	//{
	//	wsprintf(m_szUserDir, "%s\\users", m_szInstDir);
	//}

	//yiMakeDir(m_szUserDir);


#ifdef _GOYOTSUYA
	if (GetProperty(0, "bIsAutoLogin", buffer)==True)
	{
		if (stricmp(buffer, "False")==0)
		{
			bLoginDialog=False;
		}
	}
	else
	{
		SetProperty(0, "bIsAutoLogin", "True");
	}

	m_network=0;
	if (bLoginDialog)
	{
		OnCommand(eGCID_LOGIN, 0);
	}
#endif

#ifdef _GOMAIL
	::SetWindowText(*m_main, "Go!Mail");
#endif
	return ;
}

// ------------------------
Bool CGoEditorApp::Finalize()
{
	//GdiplusShutdown(__g_gdiToken__);
	yiCloseWinsock();
	return __super::Finalize();
}

#include "glsMessageBox.h"

#ifdef _GOYOTSUYA
// --------------------------------
sMsg* CreateMsg(const wchar_t* pszMsg)
{
	int n = wcslen(pszMsg);
	sMsg* msg = (sMsg*)yialloc(n+sizeof(sMsg));
	
	msg->remoteId=-1;
	msg->type=0;
	wcscpy((wchar_t*)msg->data, pszMsg);
	return msg;
	return 0;
}

// --------------------------------
Bool CGoEditorApp::PostMessageBox(sMsg* msg)
{
	m_network->m_msg->addref(msg);
	xUpdateMessage(m_network->m_msg);
	return True;
}
#endif


// --------------------------------------
Bool CGoEditorApp::PostCommand(int nSystemId, cGlassInterface* from, unsigned int param1, unsigned int param2)
{
	return PostMessage(*m_main, WM_POST_COMMAND, nSystemId, ((param1<<16)|(param2&0xFFFF)));
}

#ifdef _GOYOTSUYA
#include "GoEditorApp.OnCommand.cpp"
#endif

#ifdef JOSEKI_DICTIONARY
// --------------------------------------
Bool CGoEditorApp::OnCommand(int nSystemId, cGlassInterface* from, IntPtr param1, LongPtr param2)
{
	switch(nSystemId)
	{

		case eGCID_SHOW_NO:
		{
			if (param1!=CBN_SELCHANGE)
			{
				break;
			}
			cglsSplitButton* btn = (cglsSplitButton*)from;

			int index = btn->GetSelectIndex();			

			if (index==0)
			{
				m_goban->ShowNo(False);
			}
			else
			{
				m_goban->ShowNo(True);
			}
			break;
		}
		case eGCID_DELETE_NODE:
		{
			m_goban->DeleteNode(0);
			break;
		}
		case eGCID_TOP:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, MOVE_TOP, list, (from==0));
			break;
		}
		case eGCID_NEXT:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, 1, list, (from==0));
			break;
		}
		case eGCID_PP:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, -10, list, (from==0));
			break;
		}
		case eGCID_PREV:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, -1, list, (from==0));
			break;
		}
		case eGCID_NN:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, 10, list, (from==0));
			break;
		}
		case eGCID_END:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, MOVE_END, list, (from==0));
			break;
		}
		case eGCID_NEXT_BRANCH:
		{
			aStoneList branches;
			m_goban->GotoNextBranch(m_goban->GetCurrentGoId(), branches);
			break;
		}
		case eGCID_PREV_BRANCH:
		{
			aStoneList branches;
			m_goban->GotoPrevBranch(m_goban->GetCurrentGoId(), branches);
			break;
		}
		case eGCID_PASS:
		{
			m_goban->PutStone(0, eST_PASS, 0);
			break;
		}
		default:
			break;
	}
	return True;
}
#endif

#ifdef _GOMAIL
// --------------------------------------
Bool CGoEditorApp::OnCommand(int nSystemId, cGlassInterface* from, IntPtr param1, LongPtr param2)
{
	switch(nSystemId)
	{
		case eGCID_LABEL:
		{
			if (param1!=CBN_SELCHANGE)
			{
				break;
			}
			cglsSplitButton* btn = (cglsSplitButton*)from;

			int id = btn->GetSelectedId();
			m_goban->SetLabelMode((eLabelType)id);
			break;
		}
		case eGCID_SHOW_NO:
		{
			if (param1!=CBN_SELCHANGE)
			{
				break;
			}
			cglsSplitButton* btn = (cglsSplitButton*)from;

			int index = btn->GetSelectIndex();			

			if (index==0)
			{
				m_goban->ShowNo(False);
			}
			else
			{
				m_goban->ShowNo(True);
			}
			break;
		}
		case eGCID_DELETE_NODE:
		{
			m_goban->DeleteNode(0);
			break;
		}
		case eGCID_TOP:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, MOVE_TOP, list, (from==0));
			break;
		}
		case eGCID_NEXT:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, 1, list, (from==0));
			break;
		}
		case eGCID_PP:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, -10, list, (from==0));
			break;
		}
		case eGCID_PREV:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, -1, list, (from==0));
			break;
		}
		case eGCID_NN:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, 10, list, (from==0));
			break;
		}
		case eGCID_END:
		{
			aStoneList list;
			stone_t** stone = m_goban->Goto(0, MOVE_END, list, (from==0));
			break;
		}
		case eGCID_NEXT_BRANCH:
		{
			aStoneList branches;
			m_goban->GotoNextBranch(m_goban->GetCurrentGoId(), branches);
			break;
		}
		case eGCID_PREV_BRANCH:
		{
			aStoneList branches;
			m_goban->GotoPrevBranch(m_goban->GetCurrentGoId(), branches);
			break;
		}
		case eGCID_ADD_NEW:
		{
			unsigned int flag=0;
			unsigned int condition=cyiGoEngine::DefaultCondition(flag);
			m_goban->AddForEdit(0, 0, 0, condition, flag);
			break;
		}
		case eGCID_PASS:
		{
			m_goban->PutStone(0, eST_PASS, 0);
			break;
		}
		case eGCID_CLOSE:
		{
			m_goban->DeleteGoban(0, True);
			break;
		}
		case eGCID_COPY:
		{
			m_goban->CopyAndAdd();
			break;
		}
		case eGCID_CALCULATE_START:
		{
			m_goban->StartCalculateMode(0);
			break;
		}
		case eGCID_CALCULATE_END:
		{
			goid_t goid=0;

			int winer;
			int score;
			m_goban->CalculateEnd(goid, 0, 0, winer, score, False);

			int result=((winer<<29)|score);

			break;
		}
		case eGCID_GIVEUP:
		{
			break;
		}
		case eGCID_CANCEL:
		{
			break;
		}
		default:
			break;
	}
	return True;
}
#endif


// --------------------------------------
void CGoEditorApp::InitailizeHotKey()
{
	cglsSystem* system=m_main->GetSystem();

	if (system)
	{
		system->RegisterKeyCommand(eGCC_CTRL_S, eGCID_CURRENT_SAVE);
		system->RegisterKeyCommand(eGCC_CTRL_C, eGCID_COPY);
	}
}


CGoEditorApp* g_app=0;
// -----------------------------------
CGoEditorApp* xGetMyApp()
{
	return g_app;
}


#include "resource.h"

#define MAX_LOADSTRING 128

// -----------------------------------
Font* g_systemFont=0;
float g_systemFontSize=0;

// -----------------------------------
Font* xGetSystemFont(int w, int h)
{
	const int high=1024;
	const int low=300;
	const float max=12.0;
	const float min=9.0;
	float size=12.0;
	Font* ret=0;

	if (h>high)
	{
		if (g_systemFontSize!=max)
		{
			g_systemFontSize=max;
			ret= new Font(L"Meiryo UI", max);
		}
	}
	else if (h<low)
	{
		if (g_systemFontSize!=min)
		{
			g_systemFontSize=min;
			ret= new Font(L"Meiryo UI", min);
		}
	}
	else
	{
		size = min+(max-min)*(float)(h-low)/(float)(high-low);
		if (g_systemFontSize!=size)
		{
			g_systemFontSize=size;
			ret= new Font(L"Meirio UI", size);
		}
	}

	if (ret)
	{
		if (g_systemFont)
			delete g_systemFont;
		g_systemFont=ret;
		return ret;
	}

	return 0;

}

// -----------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	char szTitle[128];
	char szTitleDisplay[128];
	char szSoftwareName[128];
	char szSoftwareNameDisplay[128];
/*
	int n=0;
	void* p = yiFileLoad("c:\\vfrwin\\joseki.bin", &n);

	cyiMemoryStream* stream = yiUtilCompress(p, n);

	if (stream)
	{
		stream->ExportToFile("c:\\vfrwin\\joseki.bin.zip");
		delete stream;
	}

	p = yiFileLoad("c:\\vfrwin\\joseki.bin.zip", &n);
	stream = yiUtilDeCompress(p, n);

	if (stream)
	{
		::DeleteFileA("c:\\vfrwin\\joseki.ex.bin");
		stream->ExportToFile("c:\\vfrwin\\joseki.ex.bin");
		delete stream;
	}
*/
#ifdef JOSEKI_DICTIONARY
	strcpy(szTitle, "GoYotsuyaJosekiDictionary");
	strcpy(szTitleDisplay, "Go!よつや定石辞典");
	strcpy(szSoftwareName, "YotsuyaSoft");
	strcpy(szSoftwareNameDisplay, "よつやソフト");
#else
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_APP_TITLE_DISPLAY, szTitleDisplay, MAX_LOADSTRING);
	LoadString(hInstance, IDS_SOFTWARE_NAME, szSoftwareName, MAX_LOADSTRING);
	LoadString(hInstance, IDS_SOFTWARE_NAME_DISPLAY, szSoftwareNameDisplay, MAX_LOADSTRING);
#endif

	cMyMainFrame* wnd = new cMyMainFrame();
	CGoEditorApp app(szSoftwareName, szTitle, False);
	g_app=&app;

	if (app.Initialize(lpCmdLine)==False)
	{
		return 0;
	}

	//cglsMessageBox* xxxx = cglsMessageBox::xNEW();
	//cGlassInterface::xDELETE(xxxx);

	// グローバル文字列を初期化しています。
	char szDisplay[256];
	app.GetProperty((const char*)szTitle, "DISPLAY", szDisplay);

	int x=CW_USEDEFAULT, y=CW_USEDEFAULT, w=CW_USEDEFAULT, h=CW_USEDEFAULT;

	app.m_bIsSizeDefault=True;

	if (szDisplay[0])
	{
		int n=sscanf(szDisplay, "(%d,%d,%d,%d)", &x, &y, &w, &h);

		if (n==4)
		{
			app.m_bIsSizeDefault=False;
		}

		if (w<300)
			w=300;
		if (h<300)
			h=300;
	}

	/*wnd.SetFont(xGetSystemFont(w, h));*/
	if (wnd->Create(hInstance, szTitleDisplay, x, y, w, h, 
		LoadIcon(hInstance, MAKEINTRESOURCE(110)), LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)), xGetSystemFont(w, h), IDC_YIGEDITOR)==False)
	{
		delete wnd;
		return False;
	}

	app.SetMainWindow(*wnd, wnd);
	app.OnMainWindowCreateAfter(lpCmdLine);



	// TODO: CreateWindow.
	// app.SetMainWindow(m_hWnd);

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		//{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		//}
	}

	delete wnd;

	if (g_systemFont!=0)
		delete g_systemFont;
	app.Finalize();

	return 0;
}

// ----------------------------------------
static char* g_pszJosekiNameHigh[] = {
	"選択なし",
	"小目",
	"目はずし",
	"高目",
	"星",
	"三々",
	0
};

// ----------------------------------------
static char* g_pszJosekiNameMiddle[] = {
	"なし",
	"小桂馬がかり",
	"一間高がかり",
	"二間高かかり",
	"大桂馬かかり",
	"高かかり",
	"隅がかり",
	"打ち込み",
	"その他",
	0
};

// ----------------------------------------
static char* g_pszJosekiNameDetail[] = {
	"なし",
	"一間ばさみ",
	"一間高ばさみ",
	"二間ばさみ",
	"二間高ばさみ",
	"三間ばさみ",
	"三間高ばさみ",
	"こすみ",
	"下つけ",
	"上つけ",
	"一間とび",
	"小桂馬",
	"大桂馬",
	"両がかり"
	"その他",
	0
};

#include "sgfManager.h"

static int xGetJosekiName(char* pszJosekiName, cyiTreeNode* node, int& _h, int& _l, int& _d)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();
	cyiTreeNode* t = node;
	int joseki_code=0;
	int h=0, l=0, d=0;
	Bool bIsHasAName=False;
	_h=0;
	_l=0;
	_d=0;

	pszJosekiName[0]=0;
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

	strcpy(pszJosekiName, g_pszJosekiNameHigh[_h]);

	if (_l==0)
	{
		if (bIsHasAName==True)
		{
			return 1;
		}
		return 0;
	}
	strcat(pszJosekiName, "-");
	strcat(pszJosekiName, g_pszJosekiNameMiddle[_l]);

	if (_d==0)
	{
		if (bIsHasAName==True)
		{
			return 1;
		}
		return 0;
	}

	strcat(pszJosekiName, "-");
	strcat(pszJosekiName, g_pszJosekiNameDetail[_d]);

	if (bIsHasAName==True)
		return 1;

	return 0;
}