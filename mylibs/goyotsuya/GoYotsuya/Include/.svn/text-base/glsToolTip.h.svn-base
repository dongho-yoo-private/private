#pragma once

#include "glsWindow.h"

#define WM_SHOW_ANIMATION (WM_USER+1)

typedef struct {
	float fShadowWidth;
	float fShadowEdge;
	float fShadowLevel;
	Brush* brush;
	color_t colorBorer;
	color_t colorFont;
	color_t colorBg;
} sGlsToolTipDetail;



class cglsToolTip : public cglsWindow
{
public:
	cglsToolTip(void);
	~cglsToolTip(void);

	// -----------------------------------------
	static void xDefaultDetail(sGlsToolTipDetail* detail);

	// -----------------------------------------
	static cglsToolTip* GetInstance(HINSTANCE hInstance=0, Font* font=0, sGlsToolTipDetail* detail=0);

	// -----------------------------------------
	Bool Create(HINSTANCE hInstance, Font* font, sGlsToolTipDetail* detail=0);

	// ---------------------------------------
	Bool SetText(const char* pszText, void* from);

	// ---------------------------------------
	Bool SetText(const wchar_t* pszText, void* from);

	// ---------------------------------------
	virtual void Show();

	// ---------------------------------------
	virtual void Hide(Bool bClear);
protected:
	// -----------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ---------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps);

	// ---------------------------------------
	LRESULT OnActivate(WPARAM wParam, LPARAM lParam, Bool&bIshandled);

	// ---------------------------------------
	LRESULT OnFocus(WPARAM wParam, LPARAM lParam, Bool&bIshandled);


private:
	// ------------------------------------
	Bool xUpdate();

// ------------------------------------
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnSizeChanged, WM_SHOW_ANIMATION)
	MESSAGE(OnActivate, WM_ACTIVATE)
	MESSAGE(OnFocus, WM_SETFOCUS)
DEFINE_MESSAGES_END

	wchar_t* m_pszText;
	HandleShadow m_hShadow;
	HBITMAP m_hBmp;
	float m_fShadowWidth;
	float m_fShadowEdge;
	float m_fShadowLevel;
	Brush* m_brush;
	color_t m_colorBorder;
	color_t m_colorFont;
	color_t m_colorBg;

	float m_fTransparency;

	HandleAnit m_hAnit;
	void* m_from;

	static cglsToolTip* m_me;

};
