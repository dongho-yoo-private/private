#pragma once

#include "yi.h"
#include <windows.h>
#include <atlbase.h>
#include <gdiplus.h>



#ifndef __GDI_PLUS__
#define __GDI_PLUS__
using namespace Gdiplus;
#pragma comment(lib, "GdiPlus.lib") 
#endif



typedef unsigned int color_t;

//#define __COLOR_B__(color) ((0xFF000000&color)>>24)
//#define __COLOR_G__(color) ((0x00FF0000&color)>>16)
//#define __COLOR_R__(color) ((0x0000FF00&color)>>8)
//#define __COLOR_A__(color) (0x000000FF&color)

#define __COLOR__(a, r, g, b) ((a&0xFF)|RGB(r, g, b))

#define __COLOR_A__ __COLOR_Ap__
#define __COLOR_R__ __COLOR_Rp__
#define __COLOR_G__ __COLOR_Gp__
#define __COLOR_B__ __COLOR_Bp__


#define __COLOR_Ap__(color) ((UINT)((0xFF000000&color)>>24))
#define __COLOR_Rp__(color) ((UINT)((0x00FF0000&color)>>16))
#define __COLOR_Gp__(color) ((UINT)((0x0000FF00&color)>>8))
#define __COLOR_Bp__(color) ((UINT)(0x000000FF&color))

#define __COLORp__(a, r, g, b) ((a<<24)|(r<<16)|(g<<8)|b)
#define __COLORt__(a, color) ((a<<24)|(color&0x00FFFFFF))

#define BITMAP32(w, h)	yiUtilNewBitmap(w, h, PixelFormat32bppARGB)//new Bitmap(w, h, PixelFormat32bppARGB)
#define DELETEBMP(bmp)	yiUtilDeleteBitmap(bmp)

typedef struct xxPOINTF_ {
	float x;
	float y;
} xPOINTF;

typedef struct xxRECTF_ {
	float x;
	float y;
	float w;
	float h;
}xRECTF;

//class cyiBitmap {
//public:
//	cyiBitmap();
//	~cyiBitmap();
//};

#include "yiShape.h"


enum eDesolveType {
	eDT_TRANSPARENT,
	eDT_AND,
	eDT_OR,
	eDT_DEFAULT=eDT_TRANSPARENT
};

enum eBitmapMergeOption{
	eBMO_NORMAL=0,
	eBMO_AVER,
	eBMO_LINEAR
};

#define cBB cyiBitmapBlender

// ----------------------------
class xEXPORT cyiBitmapBlender
{
public:
	cyiBitmapBlender(void);
	~cyiBitmapBlender(void);

	cyiBitmapBlender(HDC hdc, UINT w, UINT h);

	cyiBitmapBlender(HBITMAP hBitmap);


	// ------------------------------------------------------
	static color_t ColorShift(color_t color, int a, int r, int g, int b);

	// ------------------------------------------------------
	static color_t ColorContrast(color_t color, float level);


	// ------------------------------------------------------
	static HBITMAP	CreateBitmap(UINT w, UINT h);


	// ------------------------------------------------------
	static LPVOID	GetBits(HBITMAP hBitmap);

	// ------------------------------------------------------
	static Bitmap* FillColor(Bitmap* bmp, color_t color, Rect* rect=0);


	// ------------------------------------------------------
	static LPVOID Blure32(LPVOID p32Rgbs, UINT rad, UINT transparent=100, xRECTF* pBound=0);

	// ------------------------------------------------------
	static Bitmap* Blure(Bitmap* src, UINT rad, UINT transparent=100, cyiShapeMask* mask=0);


	// ------------------------------------------------------
	static Bitmap* MakeMaskImage(Bitmap* src, color_t color);

	// ------------------------------------------------------
	static Bitmap* NeonSign(Bitmap* src, color_t color=Color::Red, Bool bIsStrong=False);

	// ------------------------------------------------------
	static LPVOID GaussBlure32(LPVOID p32Rgbs, float rad, UINT transparent=100, xRECTF* pBound=0);

	// ------------------------------------------------------
	static Bitmap* GaussBlure(Bitmap* src, float rad, UINT transparent=100, cyiShapeMask* pBound=0);

	// ------------------------------------------------------
	static Bitmap* cyiBitmapBlender::GaussBlureX(Bitmap* src, float rad, UINT transparent, cyiShapeMask* pBound);

	// ------------------------------------------------------
	static Bitmap* GaussBlureXX(Bitmap* src, float rad, UINT transparent, cyiShapeMask* pBound);

	// ------------------------------------------------------
	static Bitmap* GaussBlureEx(Bitmap* src, float rad, Bool* stopped);

	// ------------------------------------------------------
	static Bitmap* GaussBlureEx(void* raw, int w, int h, float rad, Bool* stopped);

	// ------------------------------------------------------
	static Bitmap* GaussBlureXX(color_t* dest, color_t* src, int w, int h, float rad, UINT transparent, cyiShapeMask* pBound);

	// ------------------------------------------------------
	static Bitmap* GaussBlureInnerGrow(Bitmap* src, float rad,  float trans=1.0, BOOL bIsCopy=True);

	// ------------------------------------------------------
	static Bitmap* GaussBlureEx(Bitmap* dest, Bitmap* src, float rad, Rect* rect=0);

	// ------------------------------------------------------
	static LPVOID DropShadow32(LPVOID p32Rgbs, UINT x, UINT y, float rad, float transparent, xRECTF* pBound=0);

	// ------------------------------------------------------
	static Bitmap* DropShadow(Bitmap* src, UINT x, UINT y, float rad, float transparent, color_t color=0, cyiShapeMask* mask=0);

	// ------------------------------------------------------
	static Bitmap* DropShadowEx(Bitmap* src, int x, int y, float _rad, float tras, color_t _color=0);

	// ------------------------------------------------------
	static Bitmap* MakeShadow(Bitmap* bmp, color_t color, float rad, float fTransparency);

	// ------------------------------------------------------
	//static Bitmap* DropShadowEx(Bitmap* src, UINT x, UINT y, float rad, float transparent, color_t color=0, cyiShapeMask* mask=0);

	// ------------------------------------------------------
	static Bitmap* ShadowBitmap(Bitmap* src, float rad, color_t color=0xFF000000, float transparent=1.0, cyiShapeMask* mask=0);

	// ------------------------------------------------------
	static Bitmap* ShadowBitmapx(Bitmap* src,  float rad, color_t xcolor, float transparent=1.0, cyiShapeMask* shapemask=0);

	// ------------------------------------------------------
	static Bitmap* GlowBitmap(Bitmap* src, float rad, color_t color=0xFF000000, cyiShapeMask* mask=0);

	// ------------------------------------------------------
	static Bitmap* GlowBitmapEx(Bitmap* src, float rad, float level=1.5, color_t color=0xFF000000);


	// ------------------------------------------------------
	static LPVOID GrayScale32(LPVOID p32Rgbs);

	// ------------------------------------------------------
	static Bitmap* GrayScale(Bitmap* src, float level);

	// ------------------------------------------------------
	static Bitmap* Transparent(Bitmap* src, float level);

	// ------------------------------------------------------
	static color_t GetGrayScaleColor(color_t color, float level);

	// ------------------------------------------------------
	static LPVOID ArgbShift(LPVOID p32Rgbs, int a, int r, int g, int b);

	// ------------------------------------------------------
	static Bitmap* ArgbShift(Bitmap* src, int a, int r, int g, int b);

	// ------------------------------------------------------
	static Bitmap* SetMask(Bitmap* src, Bitmap* mask, int x, int y, BOOL bIsReverse=FALSE);

	// ------------------------------------------------------
	//static Bitmap* BitmapCopy(Bitmap* bmp, const Bitmap* src);

	// -------------------------------
	static void  Reverse(Bitmap* bmp, BOOL bIsHorizental=TRUE);

	//---------------------------------------------------
	static Bitmap* RotateCopy(Bitmap* dest, Bitmap* src, int n, int xDest=0, int yDest=0,  int xSrc=0, int ySrc=0, int wSrc=0, int hSrc=0, cyiShapeMask* mask=0);

	//---------------------------------------------------
	static void DeleteBitmap(Bitmap* bmp);

	//---------------------------------------------------
	static Bitmap* BitmapCopy(Bitmap* dest, Bitmap* src, int xDest=0, int yDest=0,  int xSrc=0, int ySrc=0, int wSrc=0, int hSrc=0);

	//---------------------------------------------------
	static Bitmap* BitmapMerge(Bitmap* dest, Bitmap* src, int xDest=0, int yDest=0,  int xSrc=0, int ySrc=0, int wSrc=0, int hSrc=0, eBitmapMergeOption option=eBMO_NORMAL);

	//---------------------------------------------------
	static Bitmap* CreateBitmap(HDC hdc, int x, int y, int w, int h);

	//---------------------------------------------------
	static Bitmap* Bitmapx(int w, int h, color_t* data, int depth=32);

	//---------------------------------------------------
	static HBITMAP CreateHBitmap(int w, int h, color_t** data, int nBitCount=32);

	//---------------------------------------------------
	static HBITMAP CreateHBitmapV5(int w, int h, Bitmap* bmp);


	//---------------------------------------------------
	static HDC CreateMemoryDC(int w, int h, color_t** data, int nBitCount=32);

	//---------------------------------------------------
	static void DestroyMemoryDC(HDC hdc);

	//---------------------------------------------------
	static Bitmap* CopyBmpFromHDC(HDC hdc, int nSrcX, int nSrcY, int nDestX, int nDestY, int nDestW, int nDestH);


	//---------------------------------------------------
	static Bitmap* Desolve(Bitmap* bmp, Bitmap* bmp2, float factor, eDesolveType type=eDT_DEFAULT);

	//---------------------------------------------------
	static Bitmap* Average(Bitmap* bmp, Bitmap* bmp2, float factor);

private:
	// ------------------------------------------------------
	BOOL GetAvPixel(float x, float y, UINT& color, xRECTF* pBoundary=0);

	// ------------------------------------------------------
	BOOL GetAvPixelz(xPOINTF* pt, UINT* color, int num, xRECTF* pBoundary=0);

	// ------------------------------------------------------
	BOOL GetCirclePixel(float x, float y, float radius, UINT n, xPOINTF* color);

	// ------------------------------------------------------
	color_t GetAverageColor(const color_t* colors, int n);


	// ------------------------------------------------------
	HBITMAP m_hBitmap;
	BITMAP  m_bitmap;
	HDC		m_hdc;
};
