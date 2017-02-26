#pragma once

#include "glsWindow.h"
#include "glsSpinBox.h"
#include "glsStatic.h"
#include "glsButton.h"

// ------------------------------------------
class xEXPORT cglsResult : public cglsWindow, public cGlsAssistant
{
public:
	cglsResult(unsigned int result, Bool bIsHalfScore);
	~cglsResult(void);


	// ------------------------------------
	Bool TrackPopup(HWND hWndParent, int x, int y, float fKomi, Font* font=0);

	// ------------------------------------
	Bool OnInitDialog(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	Bool OnOk(HWND hWnd, int code);

	// ------------------------------------
	Bool OnCancel(HWND hWnd, int code);

	// ------------------------------------
	void Set(unsigned int result, Bool bIsHalfScore);

	// ------------------------------------
	unsigned int Get();


	// ------------------------------------
	Bool OnClose(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	unsigned int GetGoResult();

	// assitant interface
	// -----------------------------------------------------
	virtual Bool ShowAssistant(HWND hWnd, int x, int y);

	// -----------------------------------------------------
	virtual Bool GetResult(wchar_t* buffer);

	// -----------------------------------------------------
	virtual Bool GetResult(unsigned int& result1, unsigned int& result2);

	// -----------------------------------------------------
	virtual Bool GetDefaultValue(wchar_t* buffer);

	// -----------------------------------------------------
	virtual Bool GetDefaultValue(unsigned int& result1, unsigned int& result2);

	// -----------------------------------------------------
	virtual Bool SetDefaultValue(wchar_t* str);


protected:
	// -----------------------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps);

	// -----------------------------------------------------
	virtual Bool OnBgDraw();//OnPaint(HDC hdc, PAINTSTRUCT* ps);


// ------------------------------------
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnInitDialog, WM_INITDIALOG)
	MESSAGE(OnClose, WM_CLOSE)
	//MESSAGE(OnNotify, WM_NOTIFY)
DEFINE_MESSAGES_END	

DEFINE_COMMANDS_BEGIN
	COMMAND(OnOk, IDOK)
	COMMAND(OnCancel, IDCANCEL)
DEFINE_COMMANDS_END

private:
	unsigned int m_result;
	Bool m_bIsHalfScore;
};
