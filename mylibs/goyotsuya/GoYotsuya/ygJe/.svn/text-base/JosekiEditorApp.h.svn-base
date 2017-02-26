#pragma once

#include "yiGoApp.h"
#include "MainWindow.h"
#include "glsGoban.h"


class CJosekiEditorApp : public cyiGoApp
{
public:
	CJosekiEditorApp(const char* pszSoftwareName, const char* pszApplicationName, Bool bIsMultiple=0);
	~CJosekiEditorApp(void);

	// ------------------------
	virtual Bool Initialize(const char* pszCommandLine=0);

	// ------------------------
	virtual void OnMainWindowCreateAfter(const char* pszCommandLine=0);

	// ------------------------
	virtual Bool Finalize();

private:
	HWND g_hRemocon;

public:
	Bool m_bIsSizeDefault;
	CMainWindow* m_wnd;
	cglsGoban* m_goban;
};


// -----------------------------------
extern CJosekiEditorApp* xGetMyApp();
