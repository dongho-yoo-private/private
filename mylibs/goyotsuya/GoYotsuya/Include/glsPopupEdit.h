#pragma once

#include "glsWindow.h"

enum eglsPopupEditMode {
	ePEM_NORMAL=1,
	ePEM_NUMERIC=2,
	ePEM_PASSWORD=4
};

class xEXPORT cglsPopupEdit : public cglsWindow, public cGlsAssistant
{
public:
	cglsPopupEdit(eglsPopupEditMode mode=ePEM_NORMAL, int n=5, Font* font=0);
	virtual ~cglsPopupEdit(void);

	// ------------------------------------
	Bool TrackPopup(HWND hWndParent, int x, int y, int n=0, Font* font=0);

	// ---------------------------------------------------
	Bool GetText(wchar_t* pszText, int nBufferSize);

	// ---------------------------------------------------
	Bool GetText(char* pszText, int nBufferSize);

	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ---------------------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);


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

	// -----------------------------------------------------
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam, Bool& handled);

private:
	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnTimer, WM_TIMER)
	DEFINE_MESSAGES_END
	eglsPopupEditMode m_mode;

	wchar_t m_szResult[1024];

	int m_n;
	Font* m_font;
};
