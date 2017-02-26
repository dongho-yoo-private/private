#pragma once
#include "yctrlbase.h"
#include "cyctrlxII.h"
#include "yiResource.h"
#include "cyctrlResource.h"

class cyRadiox :
	public cyctrlbase
{
public:
	cyRadiox(void);
	~cyRadiox(void);

	// ------------------------------
	Bool AddItem(const wchar_t* pszText, int distance);

	// ------------------------------
	Bool Create(cyctrlbase* ctrl, int id, int x, int y, color_t color, color_t txtcol, int* retval);

	// ------------------------------
	virtual Bool Create(cyctrlbase* base, int x, int y);

	// ------------------------------------
	virtual Bool OnCtrlxDraw(int id, int subId, Bitmap* srf, Bitmap* bg, Font* fnt, sControlxII* s, eControlxEventII eventId);

	// ------------------------------------
	virtual Bool OnCtrlxBgDraw(int id, Bitmap* srf, int x, int y);

	// ------------------------------------
	virtual Bool OnMakeItem(int id, int subId, Font* fnt, sControlxII* s);


	// ------------------------------------
	virtual Bool EnableItem(Bool bIsEnable);

	// ------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps=0);

	// ------------------------------------
	virtual void Move(UINT x, UINT y);

	// ------------------------------
	void Destroy();

	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled);

	// ------------------------------
	void SetValue(int n);

//DEFINE_COMMANDS_BEGIN
//	COMMAND(OnRadioClicked, 
//DEFINE_COMMANDS_END
private:
	cyctrlxII* m_cx;
	int m_nRadioCnt;
	color_t m_radio_color;
	color_t m_txtColor;
	int* m_retval;
	sControlxII* m_prevSelected;
	int m_id;
};
