#pragma once

#include "glsWindow.h"
#include "glsSpinBox.h"
#include "glsStatic.h"
#include "glsButton.h"

// ------------------------------------------
class xEXPORT cglsNameLevel : public cglsWindow, public cGlsAssistant
{
public:
	cglsNameLevel(const char* pszName, int level, const char* pszLevel=0);
	cglsNameLevel(const wchar_t* pszName, int level, const wchar_t* pszLevel=0);
	~cglsNameLevel(void);


	// ------------------------------------
	void SetNameLevel(const char* pszName, int level, const char* pszLevel=0);

	// ------------------------------------
	void SetNameLevel(const wchar_t* pszName, int level, const wchar_t* pszLevel=0);

	// ------------------------------------
	Bool TrackPopup(HWND hWndParent, int x, int y, Font* font=0);

	// ------------------------------------
	Bool OnInitDialog(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	Bool OnOk(HWND hWnd, int code);

	// ------------------------------------
	Bool OnCheckAmature(HWND hWnd, int code);

	// ------------------------------------
	Bool OnCancel(HWND hWnd, int code);

	// ------------------------------------
	char* GetName();

	// ------------------------------------
	char* GetLevel(unsigned int& level);


	// ------------------------------------
	Bool OnClose(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	virtual LRESULT OnNotHandledCommand(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);

	// assitant interface
	// -----------------------------------------------------
	virtual Bool ShowAssistant(HWND hWnd, int x, int y);

	// -----------------------------------------------------
	virtual Bool GetResult(wchar_t* buffer);

	// -----------------------------------------------------
	virtual Bool GetResult(unsigned int& result1, unsigned int& result2);

	// -----------------------------------------------------
	virtual Bool GetDefaultValue(wchar_t* buffer);

	// -----------------------------------------------------
	virtual Bool GetDefaultValue(unsigned int& result1, unsigned int& result2);

	// -----------------------------------------------------
	virtual Bool SetDefaultValue(wchar_t* str);


protected:
	// -----------------------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps);

	// -----------------------------------------------------
	virtual Bool OnBgDraw();//OnPaint(HDC hdc, PAINTSTRUCT* ps);

	// -----------------------------------------------------
	void InitLevelList(Bool bIsAmature);


// ------------------------------------
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnInitDialog, WM_INITDIALOG)
	MESSAGE(OnClose, WM_CLOSE)
	//MESSAGE(OnNotify, WM_NOTIFY)
DEFINE_MESSAGES_END	

DEFINE_COMMANDS_BEGIN
	COMMAND(OnOk, IDOK)
	COMMAND(OnCheckAmature, 3)
DEFINE_COMMANDS_END

private:
	wchar_t m_szName[256];
	wchar_t m_szLevel[16];
	int		m_level;
	Bool	m_bIsAmature;

	sGoHeader* header;
};
