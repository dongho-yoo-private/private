#pragma once

#include "GlassInterface.h"

typedef struct {
	sGlsDetail gls;
	color_t colorFont;
	color_t colorBg;
	Bool bIsMultiLine;
	Bool bIsAutoFontSize;
	int id;
	eShapeTextStyle eStyle;
	int nFixedWidth;
}sGlsStaticDetail;

xEXPORT cglsStatic* xcglsStatic();

typedef struct {
	int EnterSpeed; // 100 msecÇ…èàóùÇ≥ÇÍÇÈÇ◊Ç´ï∂éöêîÅB
	int LeaveSpeed; // 100 msecÇ…èàóùÇ≥ÇÍÇÈÇ◊Ç´ï∂éöêîÅB
	int EnterMaxDelay; // è„å¿íl
	int LeaveMaxDelay;

} sGlsStaticAnimation;

#define WM_STATIC_ANIMATION (WM_USER+1)

// --------------------------------
class xEXPORT cglsStatic : public cGlassInterface
{
public:
	cglsStatic(void);
	~cglsStatic(void);

	// --------------------------------
	static void xDefaultDetail(sGlsStaticDetail* detail);

	// --------------------------------
	Bool Create(HWND hWnd, int x, int y, int w, int h, Bitmap* icon, const wchar_t* pszText, sGlsStaticDetail* detail=0);

	// --------------------------------
	void SetText(Bitmap* icon, const wchar_t* pszText);

	// --------------------------------
	void SetText(Bitmap* icon, const char* pszText);

	// --------------------------------
	const wchar_t* GetText();

	// ------------------------------------
	virtual Bool OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX=0, int offsetY=0);

	// ------------------------------------
	virtual Bitmap* GetBgBitmap();

	// -----------------------------
	static Font* xGetVariableFont(Font* font, wchar_t* pszText, int w, int h, int aw, int ah);

	// -----------------------------
	virtual void Update(eGlsUpdateMode mode=eGUM_UPDATE, eDrawItemStatus status=eDIS_NORMAL);


private:
	color_t m_colorFont;
	color_t m_colorBg;
	Bool	m_bIsMultiLine;
	Bool	m_bIsAutoFontSize;

	Bitmap* m_icon;
	Bitmap* m_bmpStr;

	int m_nBmpStrWidth;
	int m_nBmpStrHeight;

	Font* m_fontAuto;
	eShapeTextStyle m_eTextStyle;

	int m_nFixedWidth;

	Bitmap* m_bg;
};

typedef class glsStatic cglsLabel;