// Setup.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "yi.h"
#include "glsWindow.h"
#include "Setup.h"
#include "resource.h"

#include "yctrls.h"
#include "yiSimpleSetupDlg.h"

const sResourcesInfo g_resources[] = {
	{IDR_BIN_YILIBS, "yiLibs.dll", "Bin", FALSE},
	{IDR_BIN_YCTRLGO, "yctrlGo.dll", "Bin", FALSE},
	{IDR_BIN_YCTRL, "yictrl.dll", "Bin", FALSE},
	{IDR_BIN_YIGEDITOR, "yigEditor.exe", "Bin", TRUE},
	{IDR_BIN_YIGOENGINE, "yiGoEngine.dll", "Bin", FALSE},
	{0, 0}
};

HINSTANCE g_xhInstance;



sSoftwareInfo* g_pSoftwareInfo=0;
HandleArchive g_hArchive=0;
int g_nArchiveCount=0;

#include <process.h>

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	yictrl_initialize();

	// 1. まず、リソースファイルをロードする。
	HRSRC hResInfo = ::FindResource(hInstance, MAKEINTRESOURCEA(IDR_BIN_IMAGE), "BIN");
	DWORD size = ::SizeofResource(hInstance, hResInfo);
	void* data = ::LockResource(::LoadResource(hInstance, hResInfo));

	g_hArchive = yiArchiveCreate(data, size);

	if (g_hArchive==0)
	{
		::MessageBoxA(::GetDesktopWindow(), "ファイルが壊れています。", "エラー", MB_OK|MB_ICONERROR);
		return 0;
	}

	while(1)
	{
		Binary bin = yiArchiveEnum(g_hArchive, g_nArchiveCount++);
		if (bin==0)
			break;

		if (g_nArchiveCount==1)
		{
			g_pSoftwareInfo = (sSoftwareInfo*)bin;
			continue;
		}

		syiInstallFormat* tar = (syiInstallFormat*)bin;



		if (tar->flag32&0x40000000)
		{
			int n = (xBIN_SIZE(tar)-sizeof(syiInstallFormat));
			void* p = (void*)&tar[1];

			char path[256];
			char filename[256];
			GetTempPathA(256, path);
			//GetTempFileNameA(path, 0, 0, filename);
			wsprintf(filename, "%s%s", path, tar->szFileName);

			// まず、実行形式ファイルを実行させる。
			if (tar->flag32&0x80000000)
			{
				cyiMemoryStream* st = yiUtilDeCompress(p, n);
				st->ExportToFile(filename);
				delete st;
			}
			else
			{!
				yiFileWrite(filename ,p, 0, n);
			}

			STARTUPINFO startup;
			memset(&startup, 0, sizeof(startup));

			PROCESS_INFORMATION process;

			startup.cb = sizeof(STARTUPINFO);
			startup.wShowWindow = SW_SHOWNORMAL;
			CreateProcess(filename, 0, 0, 0, False, NORMAL_PRIORITY_CLASS, 0, 0, &startup, &process);

			WaitForSingleObject(process.hProcess, INFINITE);
			
		}
	}

	cyiSimpleSetupDlg* dlg = new cyiSimpleSetupDlg();

	dlg->SetResourceList((sResourcesInfo*)g_resources);
	dlg->CreateFromReosurce(0, hInstance, IDD_SETUP_START, True);

	//if (dlg->IsSuccess()==True)
	//{
	//	int n = ::MessageBox(GetDesktopWindow(), "インストールが正常に終わりました。", "お知らせ", MB_OK|MB_ICONINFORMATION);
	//}

	delete dlg;

	yictrl_finalize();
	return 0;
}
