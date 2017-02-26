// UnInstall.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "UnInstall.h"
#include "yistd.h"
#include "process.h"
#include "resource.h"

#include "yctrls.h"
#include "yiSimpleUnInstDlg.h"

// HKEY_LOCAL_MACHINE\Software\GroupName\SoftwareName
// HKEY_LOCAL_MACHINE\Software\GroupName\SoftwareName
// HKEY_LOCAL_MACHINE\Software\GroupName\Common

//const char* g_szGroupName		= "#!$%&GROUPNAME                                                  ";

Bool g_bIsDeleteAll=False;
//const char* g_szSoftwareName	= "#!$%&GROUPNAME                                                  ";

INT_PTR CALLBACK OnConfirmDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}

extern char g_szGroupName[32];//={0,};

// lpCmdLine is null, confirm dialog.
// lpCmdLine is GroupName, SoftwareName
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	xTRACE("Uninstall START = [%s]", lpCmdLine);
	

	char szStartFlag[32]={0, };

	if (sscanf(lpCmdLine, "%s %s", g_szGroupName, szStartFlag)==1)
	{	
		// �܂��A�t�@�C����temp�f�B���N�g���ɃR�s�[
		char tempdir[256];
		char szFileName[256];
		char destFileName[256];
		char name[128];
		char ext[16];

		if (g_szGroupName[0]==0)
		{
			MessageBox(GetDesktopWindow(), "�A���C���X�g�[���Ώۂ�����܂���B", "�G���[", MB_OK|MB_ICONERROR);
			return 0;
		}

		GetModuleFileNameA(hInstance, szFileName, 256);

		yiFileGetNameAndExt(szFileName, 0, name, ext);
		GetTempPathA(256, tempdir);
		wsprintf(destFileName, "%s\\%s.%s", tempdir, name, ext);

		if (CopyFile(szFileName, destFileName, FALSE)==TRUE)
		{
			// temp�t�@�C���̃t�@�C�������s
			PROCESS_INFORMATION info;
			STARTUPINFO startup;
			char szCommand[256];

			memset(&startup, 0, sizeof(startup));
			memset(&info, 0, sizeof(info));

			wsprintf(szCommand, "%s \"%s %s\" ", destFileName, g_szGroupName, "1");
			xTRACE("Uninstall CreateProcess = [%s]", destFileName);
			if (CreateProcess(0, szCommand, 0, 0, FALSE, NORMAL_PRIORITY_CLASS, 0, tempdir, &startup, &info)==FALSE)
			{
				return 0;
			}
		}
		xTRACE("Uninstall CopyFile failure=%d", GetLastError());
		return 0;
	}

	char szGroupName[32];
	strcpy(szGroupName, &g_szGroupName[1]);
	strcpy(g_szGroupName, szGroupName);
	xTRACE("Uninstall GroupName:%s", g_szGroupName);
	if (g_szGroupName[0]==0)
	{
		MessageBox(GetDesktopWindow(), "�A���C���X�g�[���Ώۂ�����܂���B", "�G���[", MB_OK|MB_ICONERROR);
		return 0;
	}

	Sleep(500);
	yictrl_initialize();

	cyiSimpleUnInstDlg* dlg = new cyiSimpleUnInstDlg();

	dlg->CreateFromReosurce(0, hInstance, IDD_UNINSTALL, True);
	delete dlg;

	yictrl_finalize();



	// �f�B���N�g����ini�t�@�C������폜�Ώۃt�@�C�����폜



	// �f�B���N�g���̍폜

	// �V���[�g�J�b�g�̍폜

	// ���W�X�g���̍폜

	return 0;
}

