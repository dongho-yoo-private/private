#pragma once

#include "yctrlbase.h"

#define MAX_TOOL_TIP_STRING 80
#define TOOL_TIP_ID 7777

// --------------------------------------
class cyctrlToolTip : public cyctrlbase
{
public:
	cyctrlToolTip(void);
	~cyctrlToolTip(void);

	// -----------------------------------
	BOOL Create(HWND hWnd=0);

	// -----------------------------------
	BOOL Show(const char* pszComment);

	// -----------------------------------
	BOOL Show(const wchar_t* pszComment);

	// -----------------------------------
	void Hide();

protected:
	virtual BOOL OnPaint(HDC hdc);
	wchar_t m_tooltip[MAX_TOOL_TIP_STRING+1];

private:
	Font* m_ToolTipFont;
	HWND m_hFromWnd;
};
