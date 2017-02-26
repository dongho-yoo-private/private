#pragma once

#include "yctrlbase.h"
#include "yiList.h"
#include "yctrlToolTip.h"
#include "yctrlButton.h"



enum eTabDirection {
	eTD_VERTICAL,
	eTD_HORIZENTAL
};

// ---------------------------------------
class cyTabData 
{
public:
	cyTabData(cyctrlbase* _ctrl, const char* pszTitle, UINT _bgColor)
	{
		::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszTitle, -1, title, 256);
		ctrl=_ctrl;
		bgColor = _bgColor;
		btn=NULL;
		btnId=0;
	}

	cyctrlbase* ctrl;
	wchar_t title[256];
	UINT bgColor;
	cyctrlButton* btn;
	WORD btnId;
};

// ---------------------------------------
class cyctrlTab : public cyctrlbase
{
public:
	cyctrlTab(void);
	~cyctrlTab(void);

	// ---------------------------------------
	UINT Add(cyctrlbase* ctrl, const char* pszTitle, UINT bgColor);

	// ---------------------------------------
	void Remove(UINT index);

	// ---------------------------------------
	BOOL Create(HWND hWndParent, UINT id, int x, int y, int w, int h, 
			int maxlen, int minlen, eTabDirection dir, BOOL bHasCloseButton=FALSE);

	// ---------------------------------------
	BOOL flip(int index);

protected:
	// ---------------------------------------
	virtual BOOL OnPaint(HDC hdc);

	// ---------------------------------------
	virtual LRESULT OnClicked(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp);


private:


	void DrawTabs();

	cyiList ctrllist;
	eTabDirection dir;
	int m_maxlen;
	int m_minlen;
	int m_dir;
	int m_currentIndex;
	WORD m_btnId;
	cyctrlbase* m_current;

	Bitmap* m_bufferBitmap;

	int m_message;
	HWND m_hNotifyWnd;

	cyctrlToolTip m_ToolTip;
	int m_prevselectedIndex;

	BOOL m_bHasCloseButton;

	void Update();

	LRESULT OnMouseMove(WPARAM wp, LPARAM lp, BOOL& handled);

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnMouseMove, WM_MOUSEMOVE)
DEFINE_MESSAGES_END



};
