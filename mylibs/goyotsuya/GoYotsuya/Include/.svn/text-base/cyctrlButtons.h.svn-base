#pragma once

#include "yistd.h"
#include "yctrlbase.h"
#include "yiList.h"


// ---------------------------------------
typedef struct xsButtonsStruct {
	int id;
	cyiBitmap* bmp;
	wchar_t name[32];
	unsigned int fgColor;
	unsigned int bgColor;
	RECT rect;
} sButtonsStruct;

class cyctrlButtons : public cyctrlbase
{
public:
	cyctrlButtons(void);
	~cyctrlButtons(void);

	// --------------------------------------------
	BOOL Create(HWND hWndParent, int x, int y, Brush* bgNormal, Brush* fntNormal, Pen* pen,  BOOL IsHide);


	// -----------------------------
	int Add(int id, cyiBitmap* bmp, const wchar_t* pszText/*, unsigned int fgColor, unsigned int bgColor*/);

	// -----------------------------
	void Update();

protected:
	// ---------------------------------------
	virtual BOOL OnPaint(HDC hdc);

	// ---------------------------------------
	virtual LRESULT OnClicked(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	LRESULT OnMouseMove(WPARAM wp, LPARAM lp, BOOL& handled);

private:
	void DrawButtons(Graphics* bmp, sButtonsStruct* btn, int nMode);

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnMouseMove, WM_MOUSEMOVE)
DEFINE_MESSAGES_END

	cyiList m_buttons;
	Bitmap* m_bmp;
	Brush* m_bgBrush;
	Brush* m_fntBrush;
	Pen* m_pen;

	sButtonsStruct* m_selected_btn;
};
