#pragma once

#include "yctrlbase.h"
#include "resource.h"

// ----------------------------------
class cMyMainFrame : public cyctrlbase
{
public:
	cMyMainFrame();
	~cMyMainFrame();

	Bool Create(HINSTANCE hInstance, const char* pszClassName, const char* pszTitle);

private:

	// ---------------------------------------
	virtual BOOL OnPaint(HDC hdc, PAINTSTRUCT* ps=0);

	// ---------------------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wp, LPARAM lp);

	// ------------------------------------
	virtual LRESULT OnDestroy(WPARAM wp, LPARAM lp);

	// ---------------------------------------------------------------
	LRESULT OnAbout(HWND hWnd, int code);

	// ---------------------------------------------------------------
	LRESULT OnExit(HWND hWnd, int code);



private:

//DEFINE_MESSAGES_BEGIN
//	MESSAGE(OnXXXXX, WM_XXXX);
//DEFINE_MESSAGES_END

DEFINE_COMMANDS_BEGIN
	COMMAND(OnAbout, IDM_ABOUT);
	COMMAND(OnExit, IDM_EXIT);
DEFINE_COMMANDS_END

};