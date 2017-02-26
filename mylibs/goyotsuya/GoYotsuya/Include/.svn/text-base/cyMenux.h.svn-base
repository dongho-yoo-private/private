#pragma once
#include "yctrlbase.h"
#include "cyctrlxII.h"


class cyMenux :
	public cyctrlbase
{
public:
	cyMenux(void);
	~cyMenux(void);

	// --------------------------------
	Bool Create(cyctrlbase* parent, int id, color_t bgColor, color_t fntColor);

	// -------------------------------
	void AddItem(int id, wchar_t* pszItem);

	// -------------------------------
	void AddItem(int id, cyctrlbase* ctrl);

	// -------------------------------
	void Show(int x, int y);

protected:
	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled);

	// ---------------------------------
	virtual Bool OnCtrlxBgDraw(int id, Bitmap* srf, int x, int y);

	// ------------------------------------
	virtual Bool OnCtrlxDraw(int id, int subId, Bitmap* srf, Bitmap* bg, 
						   Font* fnt, sControlxII* s, eControlxEventII eventId);

	// ---------------------------------
	LRESULT OnKillFocus(WPARAM wp, LPARAM lp, Bool& bIsHandled);

	// ---------------------------------
	LRESULT OnSelectedItem(WPARAM wp, LPARAM lp, Bool& bIsHandled);

	// ------------------------------------
	virtual void Move(UINT x, UINT y);


	// ------------------------------------
	virtual BOOL  OnPaint(HDC hdc, PAINTSTRUCT* ps=0);


DEFINE_MESSAGES_BEGIN
	MESSAGE(OnKillFocus, WM_KILLFOCUS)
	MESSAGE(OnSelectedItem, WM_ITEM_MOUSE_ENTER)
DEFINE_MESSAGES_END
private:
	cyctrlxII m_xx;
	int m_currSel;
	color_t m_bgColor;
	color_t m_fntColor;
};
