#pragma once
#include "yiBitmap.h"
#include "yctrlbase.h"


enum eButtonStyle {
	eBS_CLOSE,
	eBS_ARROW_RIGHT,
	eBS_ARROW_LEFT,
	eBS_ARROW_UP,
	eBS_ARROW_DOWN,
	eBS_PLUS,

	eBS_NONE

};

// -------------------------------------------
class cyctrlButton : public cyctrlbase
{
public:
	cyctrlButton(void);
	~cyctrlButton(void);

	// ---------------------------------------
	BOOL Create(HWND hParent, int x, int y, int w, int h, Bitmap* bmp);

	// ---------------------------------------
	BOOL Create(HWND hParent, int x, int y, int w, int h, wchar_t* string);


protected:
	// ---------------------------------------
	virtual LRESULT OnMouseEnter(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual BOOL OnPaint(HDC hdc);

	// ---------------------------------------
	LRESULT OnLButtonDown(WPARAM wp, LPARAM lp, BOOL& IsHandled);

	// ---------------------------------------
	LRESULT OnLButtonUp(WPARAM wp, LPARAM lp, BOOL& IsHandled);


private:
	void DrawClose(Graphics* g);
	UINT m_mode; // 0: normal, 1: over, 2: pushed, 3:disable, or 0x8000 : default
	cyiDrawObject* m_dobjs[3];
	eButtonStyle m_style;

	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnLButtonDown, WM_LBUTTONDOWN)
		MESSAGE(OnLButtonUp, WM_LBUTTONUP)
	DEFINE_MESSAGES_END
};
