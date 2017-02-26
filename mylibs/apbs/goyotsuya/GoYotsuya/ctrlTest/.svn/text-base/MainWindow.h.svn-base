#pragma once

#include "glsWindow.h"
#include "ypn.h"

#define GOBAN_TEST

class MainWindow : public cglsWindow
{
public:
	MainWindow(void);
	~MainWindow(void);

protected:
	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ---------------------------------------------------
	virtual LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	
	// ---------------------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------
	virtual Bool OnBgDraw();

	// ---------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps);

	// ---------------------------------------
	LRESULT OnTabItemNotify(int id, int subId, int code);

	// ---------------------------------------
	LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------
	LRESULT OnVScroll(WPARAM wParam, LPARAM lParam, Bool& handled);


	// ---------------------------------------------------
	LRESULT OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnGesture(WPARAM wParam, LPARAM lParam, Bool& handled);

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnRButtonUp, WM_RBUTTONUP)
	MESSAGE(OnVScroll, WM_VSCROLL)
	MESSAGE(OnTimer, WM_TIMER)
	MESSAGE(OnSizeChanged, WM_SIZE)
	MESSAGE(OnGesture, WM_GESTURE)
DEFINE_MESSAGES_END
	// ------------------------------------
DEFINE_NOTIFY_BEGIN
	NOTIFY(OnTabItemNotify, 1)
DEFINE_NOTIFY_END

	Image* m_img;
	Bitmap* m_img2;
	Bitmap* m_TextureImg;
};
