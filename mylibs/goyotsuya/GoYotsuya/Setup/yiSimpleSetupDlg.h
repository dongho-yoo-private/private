#pragma once

#include "yctrlbase.h"
#include "glsDlgProgress.h"
#include "resource.h"

typedef struct xsResourcesInfo {
	int id;
	const char* pszFileName;
	const char* pszSubDir;
	Bool bNeedShortCut;
} sResourcesInfo;

#define WM_THREAD_QUIT (WM_USER+333)

class cyiSimpleSetupDlg : public cyctrlbase
{
public:
	cyiSimpleSetupDlg(void);
	~cyiSimpleSetupDlg(void);

	void SetResourceList(sResourcesInfo* info);

	Bool IsSuccess();

private:

	// -----------------------
	virtual LRESULT OnCreate(WPARAM wp, LPARAM lp);

	// -----------------------
	LRESULT OnOK();

	// -----------------------
	LRESULT OnCancel();

	// --------------------------------
	virtual void OnThreadStart(int id, int param1, int param2);
	
	// -----------------------
	LRESULT OnOpenDir(HWND hWnd, int code);

	// -----------------------
	LRESULT OnThreadQuit(WPARAM wParam, LPARAM lParam, Bool& handled);

	DEFINE_COMMANDS_BEGIN
		COMMAND_NOPARAM(OnOK, IDOK)
		COMMAND_NOPARAM(OnCancel, IDCANCEL)
		COMMAND(OnOpenDir, IDC_OPENDIR)
	DEFINE_COMMANDS_END

	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnThreadQuit, WM_THREAD_QUIT)
	DEFINE_MESSAGES_END

	//void StartInstall();

	cglsDlgProgress m_progress;
	char m_szInstallPath[256];
	int m_nInstallFileCount;
	sResourcesInfo* install_files;
	BOOL m_bResult;
	BOOL m_bInstallQuit;

};
