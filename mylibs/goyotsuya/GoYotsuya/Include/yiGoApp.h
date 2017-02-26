#pragma once

#include "yistd.h"



class __declspec(dllexport) cyiGoApp
{
public:
	// ------------------------
	cyiGoApp(const char* pszSoftwareName, const char* pszApplicationName, Bool bIsMultiple=FALSE);

	// ------------------------
	~cyiGoApp(void);

	// ------------------------
	static Bool GetApplicationDirectory(const char* pszSoftwareName, char* szApplicationDir);

	// ------------------------
	static Bool SetAppAttributeStr(const char* pszAppDir, const char* pszAppName, const char* key, char* value);

	// ------------------------
	static Bool SetAppAttributeInt(const char* pszAppDir, const char* pszAppName, const char* key, int value);

	// ------------------------
	static Bool GetAppAttributeStr(const char* pszAppDir, const char* pszAppName, const char* key, char* value);

	// ------------------------
	static Bool GetAppAttributeInt(const char* pszAppDir, const char* pszAppName, const char* key, int* value, int nDefault);

	// ------------------------
	virtual Bool Initialize(const char* pszCommandLine=0);

	// ------------------------
	virtual Bool Finalize();

	// ------------------------
	virtual void OnApplicationDuplicated(HWND hWnd);

	// ------------------------
	virtual void OnMainWindowCreateAfter(const char* pszCommandLine=0);

	// ------------------------
	void SetMainWindow(HWND hWnd, void* param=0);

	// ------------------------
	Bool GetProperty(const char* pszSection, const char* Key, char* value);

	// ------------------------
	Bool GetPropertyInt(const char* pszSection, const char* Key, int& value, int defaultValue=-1);

	// ------------------------
	Bool SetProperty(const char* pszSection, const char* Key, char* value);

	// ------------------------
	Bool SetPropertyInt(const char* pszSection, const char* Key, int n);

protected:
	// ------------------------
	char m_szSoftwareName[64];
	// ------------------------
	char m_szAppName[64];
	// ------------------------
	char m_szInstDir[256];


	// ------------------------
	Bool m_bIsMultiple;

	// ------------------------
	HWND m_hMainWnd;

	// ------------------------
	void* m_param;

	// ------------------------
	HANDLE m_hMutex;
};
