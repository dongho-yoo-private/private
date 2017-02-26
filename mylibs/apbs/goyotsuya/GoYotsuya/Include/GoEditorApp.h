#pragma once


#include "yiGoApp.h"
#include "glsGoban.h"
#include "glsPopupMenu.h"
#include "glsSystem.h"


class CGoEditorApp : public cyiGoApp, public cglsCommandCenter
{
public:
	CGoEditorApp(const char* pszSoftwareName, const char* pszApplicationName, Bool bIsMultiple=0);
	~CGoEditorApp(void);

	Bool OnCreate(HWND hWnd);

	// ------------------------
	virtual Bool Initialize(const char* pszCommandLine=0);

	// ------------------------
	virtual void OnMainWindowCreateAfter(const char* pszCommandLine=0);

	// ------------------------
	virtual Bool Finalize();

	// --------------------------------------
	virtual void OnCommand(int nSystemId, cGlassInterface* from);

private:
	// --------------------------------------
	void InitailizeHotKey();
public:
	Bool m_bIsSizeDefault;
	cglsGoban* m_goban;
	cglsPopupMenu* m_gobanMenu;
	HWND m_hWnd;
	cGlassInterface* m_main;
};

extern CGoEditorApp* xGetMyApp();

