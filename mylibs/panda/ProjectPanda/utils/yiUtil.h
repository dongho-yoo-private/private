#pragma once
#include "yi.h"
#include "yiw32.h"


#define HEADER_STRING "WinPackV2"

typedef struct {
	char szHeader[32];
	char szProduct[64];
	char szGroupName[64];
	char szGroupNameDisplay[64];
	char szSoftwareName[64];
	char szSoftwareNameDisplay[64];
	char szVersion[32];
	char szLicenseCode[128];
	unsigned int flag32; // 0x80000000 -> 64bit. , 0x40000000 -> upmgr started, 0x20000000-> upmgr downloading, 0x10000000 -> upmgr downloaded, 0x08000000-> installing, 0x00008000 -> self extrac
	unsigned int reserved[16];
}sSoftwareInfo;

typedef struct {
	char szHeader[32];
	char szProduct[64];
	char szGroupName[64];
	char szGroupNameDisplay[64];
	char szSoftwareName[64];
	char szSoftwareNameDisplay[64];
	char szVersion[32];
	char szLicenseCode[128];
	unsigned int flag32; // 0x80000000 -> 64bit. , 0x40000000 -> upmgr started, 0x20000000-> upmgr downloading, 0x10000000 -> upmgr downloaded, 0x08000000-> installing, 0x04000000 -> has a licensekey., 0x02000000 -> setup and bin, 0x01000000 -> for update.
	unsigned int checksum;
	unsigned int reserved[16];

	// ex> http://xxxx.xxxx.com/update/ -> デフォルトファイルを読み込む(application name.version.update.txt)
	char szUpdateUrl[512];
	char szUpdateImage[128];
}sSoftwareInfoV2;

#ifdef _WIN32


// -------------------------------
xEXTERN_C int yiGetPropertyInt(const char* pszFileName, const char* key, int defaultValue);
// -------------------------------
xEXTERN_C Bool yiGetProperty(const char* pszFileName, const char* key, char* value, const char* defaultValue);
// -------------------------------
xEXTERN_C Bool yiSetPropertyInt(const char* pszFileName, const char* key, int n);
// -------------------------------
xEXTERN_C Bool yiSetProperty(const char* pszFileName, const char* key, const char* value);


// -------------------------------
xEXTERN_C int yiUtilGetCpuCount();

// -------------------------------
xEXTERN_C Bool yiUtilSetSystemTray(HWND hWnd, int id,HICON hIcon, unsigned int notifyMsg, const char* tip);

// -------------------------------
xEXTERN_C Bool yiUtilDeleteFromSystemTray(HWND hWnd, int id);

// -----------------------------------------------------
xEXTERN_C Bool yiUtilRegisterSoftware(const char* pszAppRoot, sSoftwareInfo* info, const char* pszExecAppName);

// -----------------------------------------------------
xEXTERN_C Bool yiUtilUnRegisterSoftware(sSoftwareInfo* info);

// -----------------------------------------------------
xEXTERN_C Bool yiUtilGetSoftwarePath(char* path, const char* pszAppName);

enum eSystemTrayIconId {
	eSTIID_NONE=0,
	eSTIID_INFO=1,
	eSTIID_WARN=2,
	eSTIID_ERROR=3
};

// -------------------------------
xEXTERN_C Bool yiUtilOutSystemTrayMessage(HWND hWnd, const char* pszTitle, const char* pszMessage, int iconId);
#else
#	ifdef __cplusplus
#		define xEXTERN_C extern "C"
#	else
#		define xEXTERN_C extern 
#	endif
#endif

xEXTERN_C Bool yiIsNumeric(char* buffer);

// -------------------------------
xEXTERN_C int stricmpe(const char* p1, const char* p2);



// -------------------------------
xEXTERN_C int yiUtilGetAge(unsigned int birth, unsigned int curr);

// -------------------------------
xEXTERN_C Bool yiUtilCreateShortCutKey(const char* pszPath, const char* pszSrcPath, const char* pszParam);

#ifdef __cplusplus

#ifdef _WIN32
// -------------------------------
xEXPORT void yiUtilRectConvert(RECT& rect);

// ---------------------------------------
xEXTERN_C Bool yiUtilIsInRect(RECT* rect1, RECT* rect2, RECT& ret);

// ---------------------------------------
xEXTERN_C Bool yiUtilIsInPos(RECT* rect, int x, int y);


// ------------------------------------
xEXTERN_C Bool yiUtilGetAddRect(RECT* result, RECT* rect1, RECT* rect2);

// ------------------------------------
xEXTERN_C Bool yiUtilGetAddRect2(RECT* result, RECT* rect1, RECT* rect2);

// ------------------------------------
xEXTERN_C Bool yiUtilToClipBoard(const wchar_t* pszText);

// ------------------------------------
xEXTERN_C wchar_t* yiUtilFromClipBoard(wchar_t* pszText, int* length);

// ------------------------------------
xEXTERN_C Bool yiUtilPointIsInRect(RECT* rect, int x, int y);


// ---------------------------------------
xEXTERN_C Bool yiNullDACL(SECURITY_ATTRIBUTES* sa);

// ---------------------------------------
xEXTERN_C BOOL yiDestroyNullDACL( SECURITY_ATTRIBUTES *pSA );

// -------------------------------
xEXTERN_C void yiBeep(int type);//char* yiHttpGet(const char* pszServer, const char* pszTarget, int* length);

// -------------------------------
xEXTERN_C HKEY yiUtilRegOpenKeyForAll(HKEY hKey, const char* pszSubKey);

// -------------------------------
xEXTERN_C HKEY yiUtilRegOpenKeyForRead(HKEY hKey, const char* pszSubKey);

// -------------------------------
xEXTERN_C HKEY yiUtilRegCreateKey(HKEY hKey, const char* pszSubKey);
#endif

#endif


// ---------------------------------------
xEXTERN_C void yiToHttpName(char* dest, const char* src);

// ---------------------------------------
xEXTERN_C void yiUrlGetHostAndPath(const char* url, char* pszServer, char* path, int* port, char* protocol);

// ---------------------------------------
xEXTERN_C char* yiHttpGet(const char* url, int nTimeOut, int length);//const char* pszTarget, int* length);

// ---------------------------------------
xEXTERN_C Binary yiHttpGetEx(const char* url, int nTimeOut, Bool bIsCache);

// ---------------------------------------
//xEXTERN_C char* yiHttpGet(address_t addr, const char* pszTarget, int* length);

// ---------------------------------------
xEXTERN_C const char* GetAreaName(int code, int locale);

// ---------------------------------------
xEXTERN_C const wchar_t* GetAreaNameW(int code, int locale);



#ifdef _WIN32
// ---------------------------------------
// __wpIsLicenseKeyOk
typedef int (__stdcall* wpfnIsLicenseKeyOk)(sSoftwareInfo* info, const char* pszLicenseKey);

// ---------------------------------------
// __wpOnInitialize
typedef BOOL (__stdcall* wpfnOnInitialize)(HINSTANCE hInstance, sSoftwareInfo* info);

// ---------------------------------------
// __wpOnFinalize
typedef BOOL  (__stdcall* wpfnOnFinalize)(sSoftwareInfo* info, const char* pszInstallDir);
#endif
