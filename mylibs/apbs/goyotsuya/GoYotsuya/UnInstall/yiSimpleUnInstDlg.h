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

class cyiSimpleUnInstDlg : public cyctrlbase
{
public:
	cyiSimpleUnInstDlg(void);
	~cyiSimpleUnInstDlg(void);

	void SetResourceList(sResourcesInfo* info);

private:


	// -----------------------
	virtual LRESULT OnCreate(WPARAM wp, LPARAM lp);

	// --------------------------------
	virtual void OnThreadStart(int id, int param1, int param2);


	// --------------------------------
	cyiList* xGetInstalledFileList(const char* pszAppName);

	// --------------------------------
	cyiList* xGetApplicationList();

	// --------------------------------
	void xDeleteAll();

	// --------------------------------
	void xDelete(int nIndex);

	// --------------------------------
	//HKEY xGetRootKey();
	
	// --------------------------------
	LRESULT OnOk();

	// --------------------------------
	LRESULT OnCancel();

	// --------------------------------
	LRESULT OnDeleteAll();

	// --------------------------------
	LRESULT OnThreadEnd(WPARAM wParam, LPARAM lParam, Bool& handled);

	DEFINE_COMMANDS_BEGIN
		COMMAND_NOPARAM(OnOk, IDOK)
		COMMAND_NOPARAM(OnCancel, IDCANCEL)
		COMMAND_NOPARAM(OnDeleteAll, IDC_DELETE_ALL)
	DEFINE_COMMANDS_END

	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnThreadEnd, WM_THREAD_QUIT)
	DEFINE_MESSAGES_END
	//void StartInstall();

	cglsDlgProgress m_progress;
	char m_szInstallPath[256];
	int m_nInstallFileCount;
	sResourcesInfo* install_files;
	BOOL m_bResult;
	BOOL m_bIsQuit;
	cyiList* m_AppList;

};
