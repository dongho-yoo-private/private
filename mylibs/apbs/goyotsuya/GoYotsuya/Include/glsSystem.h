#pragma once

#include "yistd.h"

#define WM_GLS_SYSTEM_COMMAND (WM_USER+1024)
class cglsKeyCenter;
class cglsCommandCenter;
class cGlassInterface;

// ------------------------------------------
class xEXPORT cglsSystem
{
public:
	cglsSystem(void);
	~cglsSystem(void);

	// --------------------------------------
	void RegisterCommand(cGlassInterface* obj, int nSystemId, int id=0);

	// --------------------------------------
	void RegisterKeyCommand(int code, int nSystemId);

	// --------------------------------------
	void RegisterSystemKeyCommand(int code, int nSystemId);

	// --------------------------------------
	int GetCommandId(cGlassInterface* obj, int id);

	// --------------------------------------
	int GetKeyCommandId(int code);

	// --------------------------------------
	int GetSysKeyCommandId(int code);

	// --------------------------------------
	void EnableCommand(int nSystemId, Bool bIsEnable);

	// --------------------------------------
	void RegisterKeyCenter(cglsKeyCenter* center);

	// --------------------------------------
	void RegisterCommandCenter(cglsCommandCenter* center);

	// --------------------------------------
	cglsKeyCenter* GetKeyCenter();

	// --------------------------------------
	cglsCommandCenter* GetCommandCenter();

	// --------------------------------------
	const char* GetSystemDir();

	// --------------------------------------
	const char* GetDataDir();

	// --------------------------------------
	void GetSystemDir(const char* pszSystemDir);

	// --------------------------------------
	void GetDataDir(const char* pszDataDir);

	// --------------------------------------
	int GetPerformance();

	// --------------------------------------
	void SetPerformance(int n);

	// --------------------------------------
	void SetDisplayResolution(int x, int y);

	// --------------------------------------
	void GetDisplayResolution(int&x, int&y);

	// --------------------------------------
	// 0: square.
	// 1: 16:9
	// 2: 16:10
	int GetDisplayType();

private:
	// --------------------------------------
	cyiList* m_listCommand;

	// --------------------------------------
	cyiList* m_listKey;

	// --------------------------------------
	cyiList* m_listSysKey;

	// --------------------------------------
	cglsCommandCenter* m_cmdCenter;

	// --------------------------------------
	cglsKeyCenter* m_keyCenter;

	// --------------------------------------
	char m_szSystemDir[MAX_PATH];

	// --------------------------------------
	char m_szDataDir[MAX_PATH];

protected:
	// --------------------------------------
	int m_performanceLevel;

	// --------------------------------------
	POINT m_ptResolution;
};

// --------------------------------------
class xEXPORT cglsCommandCenter {
public:
	cglsCommandCenter(){};
	~cglsCommandCenter(){};

public:
	// --------------------------------------
	virtual Bool OnCommand(int nSystemId, cGlassInterface* from, IntPtr param1=0, LongPtr param2=0)=0;

	// --------------------------------------
	virtual Bool PostCommand(int nSystemId, cGlassInterface* from, unsigned int param1=0, unsigned int param2=0)=0;

};

// --------------------------------------
class xEXPORT cglsKeyCenter {
public:
	cglsKeyCenter(){};
	~cglsKeyCenter(){};

public:
	//------------------------------------
	virtual void OnChar(int nKeyCode, cGlassInterface* from)=0;

	// ------------------------------------
	virtual void OnImeChar(int nKeyCode, cGlassInterface* from)=0;

	// --------------------------------------
	virtual void OnSysKey(int nKeyCode, cGlassInterface* from)=0;

	// --------------------------------------
	virtual void OnStartComposition(unsigned long param, cGlassInterface* from)=0;

	// --------------------------------------
	virtual void OnComposition(unsigned long param, cGlassInterface* from)=0;

	// --------------------------------------
	virtual void OnEndComposition(unsigned long param, cGlassInterface* from)=0;

};