#pragma once

#include "yistd.h"
#include "yctrlbase.h"
#include "resource.h"

#define WM_ADD_CONSOLE (WM_USER+1)
#define WM_THREAD_QUIT (WM_USER+2)
#define WM_SYSTRAY_CLICKED (WM_USER+3)


class cyiLoggerDlg : public cyctrlbase
{
public:
	cyiLoggerDlg(void);
	~cyiLoggerDlg(void);

	// -----------------------
	virtual LRESULT OnCreate(WPARAM wp, LPARAM lp);

	// -----------------------
	LRESULT OnOk();

	// -----------------------
	LRESULT OnCancel();

	// -----------------------
	LRESULT OnClear();

	// -----------------------
	LRESULT OnAddConsole(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -----------------------
	LRESULT OnSystemTrayClicked(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -----------------------
	LRESULT OnClose(WPARAM wParam, LPARAM lParam, Bool& handled);

	// --------------------------------
	Bool IsOutput(unsigned int code);

	// --------------------------------
	Bool PutError(const char* p);

private:
	DEFINE_COMMANDS_BEGIN
		COMMAND_NOPARAM(OnOk, IDOK)
		COMMAND_NOPARAM(OnCancel, IDCANCEL)
		COMMAND_NOPARAM(OnClear, IDC_CLEAR)
	DEFINE_COMMANDS_END

	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnAddConsole, WM_ADD_CONSOLE)
		MESSAGE(OnThreadQuit, WM_THREAD_QUIT)
		MESSAGE(OnSystemTrayClicked, WM_SYSTRAY_CLICKED)
		MESSAGE(OnClose, WM_CLOSE)
	DEFINE_MESSAGES_END

	// --------------------------------
	virtual void OnThreadStart(int id, int param1, int param2);

	// -----------------------
	LRESULT OnThreadQuit(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -----------------------
	Bool		 m_bIsTeminateThread;
	// -----------------------
	unsigned int m_mode;
	// -----------------------
	int			 m_nBufferSize;
	// -----------------------
	int			 m_nPortNo;
	// -----------------------
	char		 m_szDir[256];//=0;
};
