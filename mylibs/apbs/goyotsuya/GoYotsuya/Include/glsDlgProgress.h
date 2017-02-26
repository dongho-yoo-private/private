#pragma once

#include "yctrlbase.h"

class xEXPORT cglsDlgProgress : public cyctrlbase
{
public:
	cglsDlgProgress(void);
	~cglsDlgProgress(void);

	// ------------------------------------------------------------------------
	Bool Create(HWND hWndParent, const char* pszCaption, const char* pszContents, int min, int max, int* progress, Bool bIsModal, Bool* pIsCanceled=0);

	// ------------------------------------------------------------------------
	void Destroy();

	// ------------------------------------------------------------------------
	void SetProgress(int n, char* pszText=0);

	// ------------------------------------------------------------------------
	void SetProgressOK(char* pszText=0);

	// ------------------------------------------------------------------------
	void Cancel();

protected:
	// ------------------------------------------------------------------------
	LRESULT OnInitDialog(WPARAM wp, LPARAM lp, Bool& handled);


	// ------------------------------------------------------------------------
	LRESULT OnCancel(HWND hWnd, int code);

	// ------------------------------------------------------------------------
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

private:
	int* m_progress_pointer;
	Bool* m_bIsCalceledPointer;
	int  m_nMaxCnt;
	int  m_nMinCnt;
	int  m_nProgress;

	char m_szInitString[256];
	char m_szCaption[256];

	HWND m_hProgress;
	HWND m_hProgressText;


DEFINE_MESSAGES_BEGIN
	MESSAGE(OnTimer, WM_TIMER);
	MESSAGE(OnInitDialog, WM_INITDIALOG)
DEFINE_MESSAGES_END

DEFINE_COMMANDS_BEGIN
	COMMAND(OnCancel, IDCANCEL);
DEFINE_COMMANDS_END
};
