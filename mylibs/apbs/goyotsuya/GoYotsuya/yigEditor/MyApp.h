#pragma once


#include "yiThreadObject.h"
#include "MyMainFrame.h"
#include "yctrlGoban.h"

class cMyApp : public cyiThreadObject
{
public:
	cMyApp(HINSTANCE hInstance, const char* pszClassname, const char* pszTitle);
	~cMyApp(void);

	// --------------------------
	static cMyApp& GetMe();

	// --------------------------
	Bool CreateMainFrame();

	// --------------------------
	void OnLoop();

	cyctrlGoban* m_goban;

private:
	// --------------------------
	cMyMainFrame m_MainFrame;
	// --------------------------
	HINSTANCE m_hInstance;
	// --------------------------
	char* m_pszClassName;
	// --------------------------
	char* m_pszTitle;

	static cMyApp* m_me;
};


