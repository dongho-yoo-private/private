#pragma once

#include <atlbase.h>
#include <gdiplus.h>

#include "yi.h"

#ifndef __GDI_PLUS__
#define __GDI_PLUS__
using namespace Gdiplus;
#pragma comment(lib, "GdiPlus.lib") 
#endif

enum eShapeTextStyle {
	eSTS_NONE,
	eSTS_DROP_SHADOW,
	eSTS_GROW,
	eSTS_GAUSSGROW, 
	eSTS_FIXED,
	eSTS_FIXED_NORMAL
};

#define __DEFAULTGRAPHICS__ (&Graphics(&Bitmap(1, 1, PixelFormat32bppARGB)))

class xEXPORT cyiShapeMask
{
public:
	cyiShapeMask(int x, int y, Bitmap* bmp, Bool bIsReverse=False);
	cyiShapeMask(Rect& rect, BOOL bIsReverse=FALSE);
	~cyiShapeMask();

	Bool IsMasked(float x, float y);
	Bool IsMasked(UINT x, UINT y);
	Bool IsMasked(int x, int y);
	Bool IsMasked(Rect& rect, Rect& out);
	float GetMaskedLevel(int x, int y);

	Rect m_rect;
	Bitmap* m_bmp;
	Bool m_bIsTypeBmp;
	Bool m_bIsReverse;
	BitmapData m_bmpdata;
};
// -------------------------------
xEXPORT void yiUtilDeleteBitmap(Bitmap* bmp);

// -------------------------------
xEXPORT Bitmap* yiUtilNewBitmap(int w, int h, PixelFormat format);

// -------------------------------------------------------
typedef Handle HandleShadow;

class xEXPORT cyiShape 
{
public:
	cyiShape(void);
	~cyiShape(void);

	// -------------------------------
	static Rect& MarginRect(Rect& rect, int x, int y);

	// -------------------------------
	static Bitmap* FillRoundRect(Brush* brush, const RectF& rect, float radius, GraphicsPath* path=NULL);

	// -------------------------------
	static Bitmap* DrawRoundRect(Pen* pen, const RectF& rect, float radius, GraphicsPath* path=NULL);

	// -------------------------------
	static Bitmap* DrawRoundRectHiLight(Pen* pen, const RectF& rect, float radius, GraphicsPath* path=NULL);


	// -------------------------------
	static Bitmap* DrawRoundRectEx(Bitmap* bmpDest, Pen* pen, Brush* brush, const RectF& rect, float radius, GraphicsPath* path=NULL);

	// -------------------------------
	static Bitmap* cyiShape::SimpleShadow(Bitmap* bmp, int w, int h, float fTrans, float fEdge);


	// -------------------------------
	static Bitmap* DrawRoundRect3D(color_t light, color_t dark, const RectF& rect, float radius, float level=1.0, GraphicsPath* path=0);


	// -------------------------------
	static Bitmap* DrawRoundTriangle(Bitmap* bmp, Pen* brush, const RectF& rect, float radius, float rotate=180.0, GraphicsPath* path=NULL);

	// -------------------------------
	static Bitmap* FillRoundTriangle(Bitmap* bmp, Brush* brush, const RectF& rect, float radius, float rotate=180.0, GraphicsPath* path=NULL);

	// -------------------------------
	static Bitmap* DrawTriAngleEx(Bitmap* bmp, int nDirection, Pen* pen, Brush* brush, const RectF& rect, GraphicsPath* path=NULL);


	// -------------------------------
	static Bitmap* DrawCross(Bitmap* bmp, Pen* pen, const RectF& rect);

	// -------------------------------
	static Bitmap* FillRadialGradiation(Bitmap* dest, int xCenter, int yCenter, float rad, color_t from, color_t to);

	// -------------------------------
	static Bitmap* DrawGlassCover(int w, int h, float edge, color_t colorThone=Color::White);



	// -------------------------------
	static BOOL FillX(Graphics& g, float x, float y, float w, Color& color, float LineWidth, BOOL IsRound=FALSE);

	// -------------------------------
	static Bitmap* SetMask(Bitmap* src, Bitmap* mask, int x, int y, BOOL bIsReverse);

	// -------------------------------
	static int RotatePos(PointF& center, float radian, PointF* pos1, ...);

	// -------------------------------
	static void Reverse(Bitmap* bmp, BOOL bIsHorizental=TRUE);

	// -------------------------------
	static Bitmap* FillMikatsuki(Bitmap* bmp, int w, int h, Brush* brush, BOOL bIsHorizental=TRUE);

	// -------------------------------
	static HFONT FontToHFont(Font* font, int weight=0);

	// -------------------------------
	static void* Clone(Font* font, float fscale=1.0, wchar_t* name=0);

	// -------------------------------
	static Bitmap* BitmapString(Bitmap* bmp, const wchar_t* str, Font* fnt, Brush* brush, eShapeTextStyle style=eSTS_NONE, color_t glowcolor=0, float rad=2.0, float level=3.0);


	// -------------------------------
	static void MeasureTextSize(const wchar_t* str, Font* font, int nMaxWidth, POINT& pt);

	// -------------------------------
	static Bitmap* BitmapStringEx(Bitmap* dest, const wchar_t* str, Font* fnt, color_t colorFont, color_t colorBg, eShapeTextStyle style=eSTS_NONE, int align=(DT_LEFT|DT_TOP), int nFixedWidth=0, int margin=0);

	// -------------------------------
	static Bitmap* BitmapStringEx(Bitmap* dest, int x, int y, const wchar_t* str, Font* fnt, color_t colorFont, eShapeTextStyle style=eSTS_NONE);

	// -------------------------------
	static Bitmap* BitmapStringV(Bitmap* bmp, const wchar_t* str, Font* fnt, Brush* brush, eShapeTextStyle style=eSTS_NONE, color_t glowcolor=0, float rad=2.0, float level=3.0);

	// -------------------------------------------------------
	static Bool GetTextSize(Font* fnt, const wchar_t* txt, int& max_w, int& max_h);

	// -------------------------------------------------------
	static Bool cyiShape::GetTextSize(Font* fnt, const wchar_t* txt, float& max_w, float& max_h);

	// -------------------------------------------------------
	static color_t* BitmapToBlock(Bitmap* bmp, color_t* dest);

	// -------------------------------------------------------
	static void DeleteBlock(color_t* p);

	// -------------------------------------------------------
	static Bitmap* BlockToBitmap(color_t* mem, int w, int h);

	// -------------------------------------------------------
	static Bitmap* Clear(Bitmap* dest, cyiShapeMask* mask);



	// -------------------------------------------------------
	static HandleShadow CreateShadow(int nShadowWidth, float edge, float level, color_t=0xFF000000);

	// -------------------------------------------------------
	static int GetShadowSize(HandleShadow hShadow);

	// -------------------------------------------------------
	static void DestroyShadow(HandleShadow h);

	// -------------------------------------------------------
	static Bitmap* MakeShadowBitmap(HandleShadow hShadow, int w, int h, Bitmap* mask=0);

	// -------------------------------------------------------
	static Bool cyiShape::SaveBitmap(wchar_t* pwszImagePath, Bitmap* bmp, wchar_t* pwszMimeType=0);

	// ----------------------------------
	static Bitmap* MakeCircleInChar(int rad, wchar_t* ch, wchar_t* fontname, color_t color, color_t color_txt);

	// ----------------------------------
	static Font* GetFontByHeight(int height, wchar_t* name);//, wchar_t* fontname, color_t color, color_t color_txt);

private:
	GraphicsPath m_path;
};
