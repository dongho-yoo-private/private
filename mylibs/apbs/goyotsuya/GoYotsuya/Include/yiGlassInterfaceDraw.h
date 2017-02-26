#pragma once

#define GLIF_DEFAULT_MARGIN 10

#include "yistd.h"

class cyiGlassInterfaceDraw
{
public:
	cyiGlassInterfaceDraw(void);
	~cyiGlassInterfaceDraw(void);

	// ---------------------------
	static void MakeShadowImage(int id, int nShadowWidth, float edge, float level=1.0);

	// ---------------------------
	static Bitmap* GetShadowBitmap(int id, int w, int h, cyiShapeMask* pMask=0);

	// ---------------------------
	static Bitmap* DrawBackground(int _w, int _h, int nShadowSize, int nShadowShift, float edge, float level, Brush* brush=0);

	// ---------------------------
	static Bitmap* DrawBackground(const wchar_t* caption, int w, int h, float edge, float level, Brush* brush=0);

	// ---------------------------
	static Bitmap* DrawDefaultButton(Bitmap* icon, const wchar_t* pszText, int xMargin=GLIF_DEFAULT_MARGIN, int yMargin=GLIF_DEFAULT_MARGIN, Font* _font=0);

	// ---------------------------
	static 	Bitmap* DrawDefaultButton(Bitmap* icon, const wchar_t* pszText, RECT* rect, Font* _font, color_t color=0xFFFFFFFF);

	// ---------------------------
	static 	Bitmap* DrawDefaultButtonDisable(Bitmap* icon, const wchar_t* pszText, RECT* rect, Font* _font);

	// ---------------------------
	static 	Bitmap* MakeRadioCircle(int rad, color_t color, Bool bIsSelected);
	
	// ---------------------------
	static 	Bitmap* MakeCheckMark(int width, color_t color, Bool bIsSelected);
};
