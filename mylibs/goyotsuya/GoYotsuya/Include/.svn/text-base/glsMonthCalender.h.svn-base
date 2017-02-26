#pragma once

#include "glsWindow.h"

enum eDialogStyle {
	eDS_BUTTONOK=0x01,
	eDS_BUTTONCANCEL=0x02
};

enum eGlsMonthCalendarMode {
	eGMCM_CAL=0x01,
	eGMCM_WATCH=0x02,
};

class xEXPORT cglsMonthCalender : public cglsWindow, public cGlsAssistant
{
public:
	cglsMonthCalender(time64_t initTime=0);
	~cglsMonthCalender(void);

	// ------------------------------------
	Bool DoModal(HWND hWndParent, char* pszTitle, int x, int y, time64_t day, eGlsMonthCalendarMode mode=eGMCM_CAL, Font* font=0);

	// ------------------------------------
	Bool TrackPopup(HWND hWndParent, int x, int y, time64_t day, eGlsMonthCalendarMode mode=eGMCM_CAL, Font* font=0);

	// ------------------------------------
	Bool OnInitDialog(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	void SetDay(time64_t time);

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

	// -----------------------------------------------------
	virtual Bool SetDefaultValue(wchar_t* value);

	// -----------------------------------------------------
	virtual void Update(Bitmap* bmp, RECT* rect=0, RECT* _childRect=0, int offsetX=0, int offsetY=0, Bool bIsLocalBufferUpdate=False);


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
	HWND m_hwndMonthCal;
	time64_t m_time;
	time64_t m_initTime;
	eGlsMonthCalendarMode m_eMode;
};
