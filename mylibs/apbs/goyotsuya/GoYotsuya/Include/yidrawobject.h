#pragma once

//#include <atlbase.h>
//#include <gdiplus.h>
//#include <windows.h>
//
//#include "yiBitmap.h"
//
//#ifndef __GDI_PLUS__
//using namespace Gdiplus;
//#pragma comment(lib, "GdiPlus.lib") 
//#endif
//
//enum eObjectType {
//	eOT_BITMAP,
//	eOT_DIBITMAP
//};
//
//enum eDrawAlign {
//	eDA_LEFT=1,
//	eDA_CENTER=2,
//	eDA_RIGHT=3,
//	eDA_TOP=0x10,
//	eDA_VCENTER=0x20,
//	eDA_BOTTOM=0x40,
//
//	eDA_LEFT_TOP=eDA_LEFT|eDA_TOP,
//	eDA_CENTER_CENTER=eDA_CENTER|eDA_VCENTER
//};
//
//// --------------------------------
//class cyiDrawObject
//{
//public:
//	cyiDrawObject(eObjectType m_type);
//	~cyiDrawObject(void);
//
//	// ------------------------------
//	virtual void SetScale(float scale) = 0;
//
//	// ------------------------------
//	eObjectType GetType();
//
//
//	// ------------------------------
//	virtual BOOL Draw(Graphics* pGr, float x, float y, eDrawAlign align =eDA_LEFT_TOP)=0;
//
//	// ------------------------------
//	UINT GetWidth()
//	{
//		return m_w;
//	}
//
//	// ------------------------------
//	UINT GetHeight()
//	{
//		return m_h;
//	}
//
//	// ------------------------------
//	UINT GetScaledWidth()
//	{
//		return (UINT)((float)m_w*m_scale);
//	}
//
//	// ------------------------------
//	UINT GetScaledHeight()
//	{
//		return (UINT)((float)m_h*m_scale);
//	}
//
//protected:
//	UINT m_w;
//	UINT m_h;
//	float m_scale;
//	BOOL m_bCanScale;
//	BOOL m_bResourceDiscardable;
//
//private:
//	eObjectType m_type;
//};

// -----------------------------------------
//class ImageObject : public DrawObject
//{
//public:
//	// -----------------------------------------
//	ImageObject();
//
//
//	// -----------------------------------------
//	ImageObject(const char* pszFileName, BOOL bCanScale=FALSE, BOOL discard=TRUE);
//
//	// -----------------------------------------
//	ImageObject(Bitmap* bmp, BOOL bCanScale=FALSE, BOOL discard=TRUE);
//
//	// -----------------------------------------
//	ImageObject(Image* img, BOOL bCanScale=FALSE, BOOL discard=FALSE);
//
//	// -----------------------------------------
//	ImageObject(UINT w, UINT h, BOOL bCanScale=FALSE);
//
//	~ImageObject();
//
//	// ------------------------------
//	BOOL Create(const char* pszFileName, BOOL bCanScale=FALSE);
//
//	// ------------------------------
//	BOOL Create(const wchar_t* pszFileName, BOOL bCanScale=FALSE);
//
//	// ------------------------------
//	BOOL Create(Image* bmp, BOOL bCanScale=FALSE);
//
//	// ------------------------------
//	BOOL Create(UINT id, LPCSTR restype, HINSTANCE hInstance, BOOL bCanScale=FALSE);
//
//	// ------------------------------------
//	operator Image*()
//	{
//		return m_Image;
//	}
//
//	// ------------------------------------
//	operator Bitmap*()
//	{
//		return m_Bitmap;
//	}
//
//	// ------------------------------
//	virtual BOOL Draw(Graphics* pGr, float x, float y, eDrawAlign align=eDA_LEFT_TOP);
//
//	// ------------------------------
//	virtual void SetScale(float scale);
//
//private:
//	Image*	m_Image;
//	Bitmap* m_Bitmap;
//
//};
//
//
//// -----------------------------------------
//class TextObject : public DrawObject
//{
//public:
//	TextObject();
//	~TextObject();
//
//	// ------------------------------
//	BOOL Create(const char* pszText, wchar_t* FontName, float fontsize, Color& color, BOOL bCanScale=0);
//
//	// ------------------------------
//	BOOL Create(const wchar_t* pszText, wchar_t* FontName, float fontsize, Color& color, BOOL bCanScale=0);
//
//	// ------------------------------
//	BOOL Create(const char* pszText, UINT size, Font* font, Color& color, BOOL bCanScale=0);
//
//	// ------------------------------
//	BOOL Create(const wchar_t* pszText, Font* font, Color& color, BOOL bCanScale=0);
//
//	// ------------------------------
//	BOOL Create(UINT id, HINSTANCE hInstance, BOOL bCanScale=FALSE);
//
//
//	// ------------------------------
//	virtual BOOL Draw(Graphics* pGr, float x, float y,  eDrawAlign align=eDA_LEFT_TOP);
//
//	// ------------------------------
//	virtual void SetScale(float scale);
//
//private:
//	wchar_t* m_str;
//	Font*	m_font;
//	Brush*	m_brush;
//	UINT  m_txtlen;
//	BOOL m_bFontNew;
//	float m_fontsize;
//	wchar_t m_fontname[32];
//};
//

//
//// -----------------------------------------
//class ComplexObject : public DrawObject
//{
//public:
//	ComplexObject();
//	~ComplexObject();
//
//	BOOL Create(POINT* pos, DrawObject* obj, ...);
//
//private:
//	cyiList* m_pList;
//};