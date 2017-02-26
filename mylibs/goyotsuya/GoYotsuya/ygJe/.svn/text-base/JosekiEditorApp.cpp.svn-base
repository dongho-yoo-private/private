#include "JosekiEditorApp.h"
#include "MainWindow.h"
#include "yistd.h"
#include "resource.h"


// ------------------------
extern HWND xCreateJosekiControl(HWND hWnd, int x, int y);
#define MAX_LOADSTRING 128

CJosekiEditorApp::CJosekiEditorApp(const char* pszSoftwareName, const char* pszApplicationName, Bool bIsMultiple)
:cyiGoApp(pszSoftwareName, pszApplicationName, bIsMultiple)
{
}

CJosekiEditorApp::~CJosekiEditorApp(void)
{
}

GdiplusStartupInput __g_gdiSI__;
ULONG_PTR           __g_gdiToken__;

// ------------------------
Bool CJosekiEditorApp::Initialize(const char* pszCommandLine)
{
	GdiplusStartup(&__g_gdiToken__, &__g_gdiSI__, NULL);
	return __super::Initialize(pszCommandLine);
}

#include "AppGoYotsuya.h"
// ------------------------
void CJosekiEditorApp::OnMainWindowCreateAfter(const char* pszCommandLine)
{
	if (strcmp(pszCommandLine, "ROTATE")==0)
	{
		__xxSetRotateModeEnable();
	}

	cAppGoYotsuya& app = cAppGoYotsuya::GetInstance();

	int w;
	cAppGoYotsuya::GetInstance().m_goban->GetSize(&w);
	
	g_hRemocon = xCreateJosekiControl(m_hMainWnd, w+32, 0);
	m_wnd->RegisterChild((cGlassInterface*)g_hRemocon);

	m_goban->RegisterNodeChangeNotifyWindow(g_hRemocon);
	app.SetMode(::eAM_JOSEKI_EDITOR);

	//app.m_goban->SetBrancheViewMode(eBVM_SHOW_ONE_NODE);
	sGobanContentsDetail detail;
	cyiGoEngine::xDefaultDetail(&detail);
	detail.eBranchViewModeValue=eBVM_SHOW_ONE_NODE;
	detail.eContentsMode=eGCM_JOSEKI_EDITOR;
	detail.eFormat = eGoFileType::eGFT_BIN;

	goid_t id=0;

	//sSoftwareLisenseCode license;
	//if (app.m_goban->GetLicenseCode(".\\data\\joseki.bin", &license)==True)
	//{
	//	id = app.m_goban->AddForEditByFile(".\\data\\joseki.bin", &detail);//eGoFileType::eGFT_SGF, eGNT_JOSEKI_DICTIONARY_EDITOR);
	//}

	if (id==0)
	{
		detail.eFormat=eGFT_SGF;
		id = app.m_goban->AddForEditByFile(".\\data\\joseki.dat", &detail);//eGoFileType::eGFT_SGF, eGNT_JOSEKI_DICTIONARY_EDITOR);
	}

	if (id)
	{
		char szBackupFileName[256];
		SYSTEMTIME time;
		GetLocalTime(&time);
		yiMakeDir(".\\data\\backups");
		wsprintf(szBackupFileName, ".\\data\\backups\\Joseki.dat.%04d.%02d.%02d.%02d.%02d.%02d", 
			(int)time.wYear, (int)time.wMonth, (int)time.wDay,
				(int)time.wHour, (int)time.wMinute, (int)time.wSecond);
		CopyFile(".\\data\\joseki.dat", szBackupFileName, False);
	}

	//app.m_goban->SearchResult();
	app.m_hControlWnd = g_hRemocon;

	RECT rect;
	RECT rectp;

	GetWindowRect(g_hRemocon, &rect);

	if (m_bIsSizeDefault==True)
	{
		cGlassInterface* obj=(cGlassInterface*)m_param;
		obj->ReSize(rect.right+10, (rect.bottom-rect.top)+50);
	}
	//SetWindowPos(m_hMainWnd, 0, 0, 0, rect.right+10, (rect.bottom-rect.top)+50 , SWP_NOZORDER|SWP_NOMOVE);
	//InvalidateRect(m_hMainWnd, 0, False);
	return ;
}

// ------------------------
Bool CJosekiEditorApp::Finalize()
{
	GdiplusShutdown(__g_gdiToken__);
	
	return __super::Finalize();
}

CJosekiEditorApp* g_app=0;
// -----------------------------------
CJosekiEditorApp* xGetMyApp()
{
	return g_app;
}



int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;
	char szTitle[128];
	char szTitleDisplay[128];
	char szSoftwareName[128];
	char szSoftwareNameDisplay[128];

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_APP_TITLE_DISPLAY, szTitleDisplay, MAX_LOADSTRING);
	LoadString(hInstance, IDS_SOFTWARE_NAME, szSoftwareName, MAX_LOADSTRING);
	LoadString(hInstance, IDS_SOFTWARE_NAME_DISPLAY, szSoftwareNameDisplay, MAX_LOADSTRING);

	CMainWindow wnd;
	CJosekiEditorApp app(szSoftwareName, szTitle, False);
	g_app=&app;

	app.Initialize();

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
	}

	if (wnd.Create(hInstance, szTitleDisplay, x, y, w, h, 
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NORMAL)), LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)), new Font(L"Meiryo", 11.0), IDC_GOYOTSUYA, 0, WS_OVERLAPPEDWINDOW/*WS_OVERLAPPED|WS_CAPTION| WS_SYSMENU|WS_THICKFRAME*/)==False)
	{
		return False;
	}

	app.SetMainWindow(wnd.m_hWnd, &wnd);
	app.OnMainWindowCreateAfter(lpCmdLine);

	// TODO: CreateWindow.
	// app.SetMainWindow(m_hWnd);

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//wnd.Destroy();

	app.Finalize();
}

