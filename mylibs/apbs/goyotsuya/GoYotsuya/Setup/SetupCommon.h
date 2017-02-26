#pragma once

#include "yistd.h"

// --------------------------------
extern const char* xEnumSoftware(int index);

// --------------------------------
extern Bool xIsUninstall(const char* pszFileName);

// --------------------------------
extern HKEY xGetRootKey();

// --------------------------------
extern HKEY xGetSoftwareKey(const char* pszSoftware);

// --------------------------------
extern cyiList* xGetSoftwareList();

// --------------------------------
extern cyiList* xGetSoftwareShortcutList(const char* pszSoftware, cyiList* list=0);

// --------------------------------
extern cyiList* xGetSoftwareFileList(const char* pszSoftware);

// --------------------------------
extern Bool xGetSoftwareInstallDir(const char* pszSoftware, char* path);

// --------------------------------
extern Bool xIsCommonDelete();

// --------------------------------
extern Bool xGetGroupDisplayName(char* display);

// --------------------------------
extern Bool xGetSoftwareInstallDir(const char* pszSoftware, char* path);


extern char g_szGroupName[32];