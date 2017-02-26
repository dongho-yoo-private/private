#pragma once

#include "glsWindow.h"

class cyigRoomNoPopup : public cglsWindow
{
public:
	cyigRoomNoPopup(void);
	~cyigRoomNoPopup(void);

	// ------------------------------------
	Bool TrackPopup(HWND hWndParent, int x, int y, int n=0, Font* font=0);

	// ---------------------------------------------------
	Bool GetText(wchar_t* pszText, int nBufferSize);

	// ---------------------------------------------------
	Bool GetText(char* pszText, int nBufferSize);

	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ---------------------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// -----------------------------------------------------
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------
	virtual Bool OnBgDraw();

	char m_szPassword[16];
	int		m_roomNo;

private:
	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnTimer, WM_TIMER)
	DEFINE_MESSAGES_END


	int m_n;
	Font* m_font;

};
