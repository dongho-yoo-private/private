#pragma once


#include <atlbase.h>
#include <gdiplus.h>

#ifndef __GDI_PLUS__
#define __GDI_PLUS__
using namespace Gdiplus;
#pragma comment(lib, "GdiPlus.lib") 
#endif

#include "yi.h"


// -----------------------------------
typedef void (*yifBmpOwnerDraw)(Bitmap* bmp);

typedef UINT color_t;

enum eObjectType {
	eOT_BITMAP,
	eOT_DIBITMAP,
	eOT_SURFACE
};

enum eDrawAlign {
	eDA_LEFT=1,
	eDA_CENTER=2,
	eDA_RIGHT=3,
	eDA_TOP=0x10,
	eDA_VCENTER=0x20,
	eDA_BOTTOM=0x40,

	eDA_LEFT_TOP=eDA_LEFT|eDA_TOP,
	eDA_CENTER_CENTER=eDA_CENTER|eDA_VCENTER
};

// --------------------------------
class xEXPORT cyiDrawObject
{
public:
	cyiDrawObject(eObjectType m_type);
	~cyiDrawObject(void);

	// ------------------------------
	virtual void SetScale(float scale, BOOL bReduced=FALSE) = 0;

	// ------------------------------
	eObjectType GetType();


	// ------------------------------
	//virtual BOOL Draw(Graphics* pGr, float x, float y, eDrawAlign align =eDA_LEFT_TOP)=0;

	// ------------------------------
	UINT GetWidth()
	{
		return m_w;
	}

	// ------------------------------
	UINT GetHeight()
	{
		return m_h;
	}

	//// ------------------------------
	//inline UINT GetScaledWidth()
	//{
	//	return (m_scale!=1.0) ? (UINT)((float)m_w*m_scale):m_w;
	//}

	//// ------------------------------
	//inline UINT GetScaledHeight()
	//{
	//	return (m_scale!=1.0) ? (UINT)((float)m_h*m_scale):m_h;
	//}

	// -----------------------------------
	virtual BOOL Draw(Graphics* gr, float x, float y,  eDrawAlign align =eDA_LEFT_TOP){return TRUE;};

	// -----------------------------------
	virtual BOOL Draw(HDC hdc, UINT x, UINT y,  eDrawAlign align =eDA_LEFT_TOP){return TRUE;};

	// -----------------------------------
	virtual BOOL DrawAlpha(HDC hdc, UINT x, UINT y,  eDrawAlign align =eDA_LEFT_TOP, UINT tranparent=255){return TRUE;};

protected:
	UINT m_w;
	UINT m_h;
	float m_scale;
	BOOL m_bCanScale;
	BOOL m_bResourceDiscardable;

	// ------------------------------
	void GetAlignPos(float& x, float&y, eDrawAlign align);

	// ------------------------------
	void GetAlignPos(UINT& x, UINT&y, eDrawAlign align);

	// ------------------------------
	virtual UINT GetScaledWidth()
	{
		return (UINT)(m_scale*m_w);
	}

	// ------------------------------
	virtual UINT GetScaledHeight()
	{
		return (UINT)(m_scale*m_h);
	}

private:
	eObjectType m_type;
};

// -----------------------------------
class cyiDIBitmap;

// -----------------------------------
class xEXPORT cyiBitmap : public cyiDrawObject
{
public:

	operator Bitmap*()
	{
		return m_scaled_bmp ? m_scaled_bmp:m_bmp;
	}

	// -----------------------------------
	cyiBitmap(const wchar_t* pszFileName);

	// -----------------------------------
	cyiBitmap(Image* pImage);

	// -----------------------------------
	// new cyiBitmap(w, h, class pointer or NULL, yifBmpOwnerDraw);
	cyiBitmap(UINT w, UINT h, ...);

	// -----------------------------------
	~cyiBitmap(void);

	// -----------------------------------
	void SetScale(float scale, BOOL bReduced=FALSE);

	// -----------------------------------
	virtual BOOL Draw(Graphics* gr, float x, float y,  eDrawAlign align =eDA_LEFT_TOP);

	// -----------------------------------
	virtual BOOL Draw(cyiDIBitmap* dest, float x, float y,  eDrawAlign align =eDA_LEFT_TOP);


	// -----------------------------------
	Bitmap* GetBitmap();

	// ------------------------------
	virtual UINT GetScaledWidth()
	{
		return m_scaled_bmp ? m_scaled_bmp->GetWidth():m_w;
	}

	// ------------------------------
	virtual UINT GetScaledHeight()
	{
		return m_scaled_bmp ? m_scaled_bmp->GetHeight():m_h;
	}



protected:
	Bitmap* m_bmp;
	Bitmap* m_scaled_bmp;
	yifBmpOwnerDraw m_draw_fnc;
	void* m_class_instance;
	//BOOL Create(UINT w, UINT h, UINT bits=32);
};

// -----------------------------------
typedef void (*yifDIBmpOwnerDraw)(color_t* colors, UINT w, UINT h);


// -----------------------------------
class xEXPORT cyiDIBitmap: public cyiDrawObject
{
public:
	cyiDIBitmap(UINT w, UINT h, ...);
	cyiDIBitmap(UINT w, UINT h, color_t* data);
	cyiDIBitmap(cyiBitmap* p);
	cyiDIBitmap(Bitmap* bmp);
	~cyiDIBitmap();

	operator color_t*()
	{
		return m_bmp;
	}

	// -----------------------------------
	HBITMAP GetBitmap();

	// -----------------------------------
	Bitmap* Export(Bitmap* bmp);

	// -----------------------------------
	Bitmap* Export(Bitmap* bmp, int x, int y, int xSrc, int ySrc, int w, int h);

	// -----------------------------------
	virtual BOOL Draw(HDC hdc, UINT x, UINT y,  eDrawAlign align =eDA_LEFT_TOP);

	// -----------------------------------
	virtual BOOL Draw(HDC hdc, UINT x, UINT y,  UINT w, UINT h, eDrawAlign align =eDA_LEFT_TOP);


	// -----------------------------------
	virtual BOOL DrawAlpha(HDC hdc, UINT x, UINT y,  eDrawAlign align =eDA_LEFT_TOP, UINT tranparent=255);

	// -----------------------------------
	void SetScale(float scale, BOOL bReduced=FALSE);

	// ------------------------------
	inline UINT GetScaledWidth()
	{
		return m_w;
	}

	// ------------------------------
	inline UINT GetScaledHeight()
	{
		return m_h;
	}


protected:
	color_t* m_bmp;
	HBITMAP  m_hBmp;
	HDC		 m_hdc;
	yifDIBmpOwnerDraw m_func;
	void* m_class_instance;

	// -----------------------------------
	BOOL Create(UINT w, UINT h);

	// -----------------------------------
	void Destroy();

	// -----------------------------------
	BOOL Import(Bitmap* bmp);

	friend class cyiSurface;
};

// -----------------------------------
class xEXPORT cyiSurface: public cyiDIBitmap
{
public:
	cyiSurface(UINT w, UINT h);
	~cyiSurface();

	operator HDC() {return m_hdc;};

	// -----------------------------------
	void SetScale(float scale, BOOL bReduced=FALSE);

	// -----------------------------------
	void Resize(UINT w, UINT h);

	// -----------------------------------
	Bitmap* ToBitmap(Bitmap* dest, UINT x, UINT y);

	// -----------------------------------
	void Copy(int x, int y, cyiSurface* sf, int destx, int desty, int w, int h);

	// ------------------------------
	void blt(int x, int y, Bitmap* bmp, eDrawAlign align=eDA_LEFT_TOP);

	// -----------------------------------
	void blt(UINT x, UINT y, Bitmap* bmp, UINT xSrc, UINT ySrc, UINT w, UINT h);

	// -----------------------------------
	void blt(UINT x, UINT y, cyiDIBitmap* bmp, UINT xSrc=0, UINT ySrc=0, UINT w=0, UINT h=0);

	// -----------------------------------
	void blt_alpha(UINT x, UINT y, Bitmap* bmp, UINT xSrc=0, UINT ySrc=0, UINT w=0, UINT h=0);

	// -----------------------------------
	void blt_alpha(UINT x, UINT y, cyiDIBitmap* bmp, UINT xSrc=0, UINT ySrc=0, UINT w=0, UINT h=0);

	// -----------------------------------
	Bitmap* export_bmp(UINT x, UINT y, UINT w, UINT h);

	// -----------------------------------
	color_t* GetRaw(int&x, int&y);
};