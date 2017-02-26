#pragma once

#include "glsWindow.h"
#include "glsSpinBox.h"
#include "glsStatic.h"
#include "glsButton.h"


enum eGlsTimeFlag {
	eGTF_AMPM=0x80,
	eGTF_HOUR=0x40,
	eGTF_MINUTE=0x20,
	eGTF_SECOND=0x10,
	eGTF_ALL=0xF0
};

class __declspec(dllexport) cglsTime : public cglsWindow, public cGlsAssistant
{
public:
	cglsTime(eGlsTimeFlag flag=(eGlsTimeFlag)0xF0, unsigned int nInitTime=0);
	~cglsTime(void);

	// ------------------------------------
	Bool DoModal(HWND hWndParent, char* pszTitle, int x, int y, unsigned int time, eGlsTimeFlag mode=eGTF_ALL, Font* font=0);

	// ------------------------------------
	Bool TrackPopup(HWND hWndParent, int x, int y, unsigned int time, eGlsTimeFlag mode=eGTF_ALL, Font* font=0);

	// ------------------------------------
	Bool OnInitDialog(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	Bool OnOk(HWND hWnd, int code);

	// ------------------------------------
	Bool OnCancel(HWND hWnd, int code);


	// ------------------------------------
	Bool OnClose(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	time64_t GetSelectedTime();

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
	eGlsTimeFlag m_eFlag;
	unsigned int m_initTime;
	unsigned int m_time;
	cglsStatic** m_colon;
};
