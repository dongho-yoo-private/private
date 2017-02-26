#pragma once

#include "resource.h"

//typedef struct xsSoftwareSetupInfo {
//	char szSoftwareName[MAX_SOFTWARE_NAME+1];
//	wchar_t szSoftwareDisplayName[MAX_SOFTWARE_NAME+1]; // プログラムファイルの登録名
//	char szSoftwarePath[MAX_PATH];
//} sSoftwareSetupInfo;
//
//typedef struct xsAppSetupInfo {
//	char szApplicationName[MAX_SOFTWARE_NAME+1];
//	wchar_t szApplicationDisplayName[MAX_SOFTWARE_NAME+1]; // ショートカットで使われる名前
//	Bool bIsExecutable;
//	char szInstallPath[MAX_PATH];
//}sAppSetupInfo;

typedef struct {
	char szGroupName[32];
	char szGroupNameDisplay[32];
	char szSoftwareName[32];
	char szSoftwareNameDisplay[32];
}sSoftwareInfo;