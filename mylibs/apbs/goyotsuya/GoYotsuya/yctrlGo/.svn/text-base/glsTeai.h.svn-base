#pragma once

#include "glsWindow.h"
#include "glsSpinBox.h"
#include "glsStatic.h"
#include "glsButton.h"

class xEXPORT cglsTeai : public cglsWindow, public cGlsAssistant, public cglsDrawObject
{
public:
	cglsTeai(int n=0);
	~cglsTeai(void);

	// ------------------------------------
	Bool TrackPopup(HWND hwnd, int x, int y, int n=-1, Font* font=0);

	// ------------------------------------
	Bool Create(cGlassInterface* parenth, int id, int x, int y, int n=-1, Font* font=0);

	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	Bool OnOk(HWND hWnd, int code);

	// ------------------------------------
	Bool OnClose(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);


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

	// --------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra);

// ------------------------------------
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnClose, WM_CLOSE)
DEFINE_MESSAGES_END	

DEFINE_COMMANDS_BEGIN
	COMMAND(OnOk, IDOK)
DEFINE_COMMANDS_END

private:
	int m_n;
	Bool m_bIsAssistantMode;
};
