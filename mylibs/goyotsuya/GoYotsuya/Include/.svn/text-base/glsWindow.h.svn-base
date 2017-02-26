#pragma once

#include "GlassInterface.h"

// -------------------------------------
class xEXPORT cglsWindow : public cGlassInterface
{
public:
	cglsWindow(void);
	~cglsWindow(void);

	// -------------------------------------
	Bool Create(HINSTANCE hInstance,  char* pszTitle, int x, int y, int w, int h, HICON hIcon, HICON hIconSmall, Font* font=0, int menuId=0, DWORD styleEx=0, DWORD style=0, Bool bIsHide=False);

	// -------------------------------------
	Bool CreateForAero(HINSTANCE hInstance,  char* pszTitle, int x, int y, int w, int h, HICON hIcon, HICON hIconSmall, Font* font=0, int menuId=0, DWORD styleEx=0, DWORD style=0);

	// -------------------------------------
	Bool DoModal(HWND hWndParent, HINSTANCE hInstance,  char* pszTitle, int x, int y, int w, int h, Font* font=0, int style=0);

	// -------------------------------------
	Bool CreatePopup(HWND hWndParent, int x, int y, int w, int h, Font* font=0, int style=0);

	// -------------------------------------
	Bool cglsWindow::MessageLoop(Bool UnfocusExit);

	// -------------------------------------
	virtual Bool ReSize(UINT x, UINT y, Bool bIsRepaint=0);



protected:

	// ---------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps);

	// ---------------------------------------
	virtual Bool OnBgDraw();

	// -----------------------------
	virtual Bitmap* GetBgBitmap();

	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ---------------------------------------
	LRESULT OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------
	virtual LRESULT OnMouseLButtonDown(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------
	virtual LRESULT OnMouseRButtonDown(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------
	virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	virtual Bool OnUpdateAfter(HDC hdc);

	// ---------------------------------------
	LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------
	LRESULT OnActivate(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------
	Bitmap* m_bg;

	// ---------------------------------------
	Bool m_bIsModal;

	// ---------------------------------------
	Bool m_bIsPopup;

	// ---------------------------------------
	Bool m_bHasPopup;

	// ---------------------------------------
	Bool m_bIsExitLoop;

	// ---------------------------------------
	Bool m_bIsExitCode;

	// ---------------------------------------
	cGlassInterface* m_exceptionObject;

// ------------------------------------
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnSizeChanged, WM_SIZE)
	MESSAGE(OnMouseLButtonDown, WM_LBUTTONDOWN)
	MESSAGE(OnMouseRButtonDown, WM_RBUTTONDOWN)
	MESSAGE(OnMouseMove, WM_MOUSEMOVE)
	MESSAGE(OnKillFocus, WM_KILLFOCUS)
	MESSAGE(OnActivate, WM_ACTIVATE)
DEFINE_MESSAGES_END

};
