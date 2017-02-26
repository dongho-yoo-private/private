#include "StdAfx.h"
#include "MyApp.h"
#include "resource.h"

// -------------------------
cMyApp* cMyApp::m_me = 0;




cMyApp::cMyApp(HINSTANCE hInstance, const char* pszClassname, const char* pszTitle)
{
	m_hInstance=hInstance;
	m_pszClassName=(char*)pszClassname;
	m_pszTitle=(char*)pszTitle;

	cMyApp::m_me=this;

}

cMyApp::~cMyApp(void)
{
}

// --------------------------
cMyApp& cMyApp::GetMe()
{
	return *m_me;
}

// --------------------------
Bool cMyApp::CreateMainFrame()
{
	return m_MainFrame.Create(m_hInstance, m_pszClassName, m_pszTitle);
}

// --------------------------
void cMyApp::OnLoop()
{
	MSG msg;
	HACCEL hAccelTable;

	hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_YIGEDITOR));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	delete this;
}

