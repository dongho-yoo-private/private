#pragma once
#include "yi.h"





#ifdef _WIN32

// -------------------------------
xEXTERN_C int yiUtilGetCpuCount();

// -------------------------------
xEXTERN_C Bool yiUtilSetSystemTray(HWND hWnd, int id,HICON hIcon, unsigned int notifyMsg, const char* tip);

// -------------------------------
xEXTERN_C Bool yiUtilDeleteFromSystemTray(HWND hWnd, int id);

enum eSystemTrayIconId {
	eSTIID_NONE=0,
	eSTIID_INFO=1,
	eSTIID_WARN=2,
	eSTIID_ERROR=3
};

// -------------------------------
xEXTERN_C Bool yiUtilOutSystemTrayMessage(HWND hWnd, const char* pszTitle, const char* pszMessage, int iconId);

#endif

// -------------------------------
xEXTERN_C int stricmpe(const char* p1, const char* p2);

// -------------------------------
xEXTERN_C Binary yiBinary(void* p, int size, ...);

// -------------------------------
xEXTERN_C void yiBinaryDelete(Binary p);

// -------------------------------
xEXTERN_C int yiUtilGetAge(unsigned int birth, unsigned int curr);

// -------------------------------
xEXTERN_C Bool yiUtilCreateShortCutKey(const char* pszPath, const char* pszSrcPath, const char* pszParam);

#ifdef __cplusplus
// -------------------------------
xEXPORT void yiUtilRectConvert(RECT& rect);

// ---------------------------------------
xEXTERN_C Bool yiUtilIsInRect(RECT* rect1, RECT* rect2, RECT& ret);
#endif

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

xEXTERN_C char* yiHttpGet(const char* pszServer, const char* pszTarget, int* length);

xEXTERN_C void yiBeep(int type);//char* yiHttpGet(const char* pszServer, const char* pszTarget, int* length);


// ---------------------------------------
xEXTERN_C const char* GetAreaName(int code, int locale);

// ---------------------------------------
xEXTERN_C const wchar_t* GetAreaNameW(int code, int locale);

// -------------------------------
xEXTERN_C HKEY yiUtilRegOpenKeyForAll(HKEY hKey, const char* pszSubKey);

// -------------------------------
xEXTERN_C HKEY yiUtilRegOpenKeyForRead(HKEY hKey, const char* pszSubKey);

// -------------------------------
xEXTERN_C HKEY yiUtilRegCreateKey(HKEY hKey, const char* pszSubKey);


