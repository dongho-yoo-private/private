#include "yiDrawObject.h"

#define MAX_STRING 256


//
// cyiDrawObject
//




///
/// ImageObject
///

//// ------------------------------
//ImageObject::ImageObject()
//			: cyiDrawObject(eOT_IMAGE)
//{
//	m_Image=NULL;
//	m_Bitmap=NULL;
//}
//
//// ------------------------------
//ImageObject::ImageObject(const char* pszFileName, BOOL bCanScale, BOOL discard)
//: cyiDrawObject(eOT_IMAGE)
//{
//	m_Image=NULL;
//	m_Bitmap=NULL;
//	m_bResourceDiscardable=discard;
//	Create(pszFileName, bCanScale);	
//}
//
//// ------------------------------
//ImageObject::ImageObject(Bitmap* bmp, BOOL bCanScale, BOOL discard)
//: cyiDrawObject(eOT_IMAGE)
//{
//	m_Image=NULL;
//	m_Bitmap=NULL;
//	m_bResourceDiscardable=discard;
//	Create(bmp, bCanScale);	
//}
//
//// ------------------------------
//ImageObject::ImageObject(Image* img, BOOL bCanScale, BOOL discard)
//: cyiDrawObject(eOT_IMAGE)
//{
//	m_Image=NULL;
//	m_Bitmap=NULL;
//	m_bResourceDiscardable=discard;
//	Create(img, bCanScale);	
//}
//
//// ------------------------------
//ImageObject::ImageObject(UINT w, UINT h, BOOL bCanScale)
//: cyiDrawObject(eOT_IMAGE)
//{
//	m_Image= new Bitmap(w, h);
//	m_Bitmap=NULL;
//	m_bCanScale=bCanScale;
//	m_bResourceDiscardable=TRUE;
//
//	Graphics g(m_Image);
//	g.Clear(Color(0,0,0,0));
//	
//	m_w = w;
//	m_h = h;
//}
//
//// ------------------------------
//ImageObject::~ImageObject()
//{
//	if (m_bResourceDiscardable)
//	{
//		if (m_Image)
//			delete m_Image;
//	}
//
//	if (m_Bitmap)
//		delete m_Bitmap;
//}
//
//
//
//// ------------------------------
//BOOL ImageObject::Create(const char* pszFileName, BOOL bCanScale)
//{
//	wchar_t buffer[MAX_STRING+1];
//	size_t size;
//
//	if (::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszFileName, -1, buffer, MAX_STRING)==0)
//	{
//		return FALSE;
//	}
//
//	return Create(buffer, bCanScale);
//}
//
//// ------------------------------
//BOOL ImageObject::Create(const wchar_t* pszFileName, BOOL bCanScale)
//{
//	if (m_Image!=NULL)
//		delete m_Image;
//	m_Image = new Image(pszFileName);
//
//	m_w = m_Image->GetWidth();
//	m_h = m_Image->GetHeight();
//
//	m_bCanScale=bCanScale;
//
//	return TRUE;
//}
//
//// ------------------------------
//BOOL ImageObject::Create(Image* bmp, BOOL bCanScale)
//{
//	m_w = bmp->GetWidth();
//	m_h = bmp->GetHeight();
//
//	m_Image=bmp;
//	m_bCanScale=bCanScale;
//
//	return TRUE;
//}
//
//// ------------------------------
////BOOL ImageObject::Create(Image* p, BOOL bCanScale)
////{
////	return TRUE;
////}
//
//
//// ------------------------------
//BOOL ImageObject::Create(UINT id, LPCSTR restype, HINSTANCE hInstance, BOOL bCanScale)
//{
//	return TRUE;
//}
//
//
//// ------------------------------
//void ImageObject::SetScale(float scale)
//{
//	if (m_bCanScale==FALSE)
//	{
//		return ;
//	}
//
//	if (m_Bitmap!=NULL)
//		delete m_Bitmap;
//
//	m_scale = scale;
//	float w = scale*(float)m_w;
//	float h = scale*(float)m_h;
//
//	m_Bitmap = new Bitmap((UINT)w, (UINT)h);
//
//	Graphics g(m_Bitmap);
//	g.SetSmoothingMode(SmoothingModeHighQuality);
//
//	g.DrawImage(m_Image, (float)0.0, (float)0.0, w, h);
//
//}
//
//// ------------------------------
//BOOL ImageObject::Draw(Graphics* pGr, float x, float y, eDrawAlign align)
//{
//	Image* pImage = m_Bitmap?m_Bitmap:m_Image;
//	float _x;
//	float _y; 
//
//	if (align & eDA_LEFT)
//	{
//		_x = x;
//	}
//	else if (align & eDA_CENTER)
//	{
//		_x=(x-(float)(pImage->GetWidth()>>1));
//	}
//	else
//	{
//		_x=(x-(float)pImage->GetWidth());
//	}
//
//	if (align & eDA_TOP)
//	{
//		_y = x;
//	}
//	else if (align & eDA_VCENTER)
//	{
//		_y=(y-(float)(pImage->GetHeight()>>1));
//	}
//	else
//	{
//		_y=(y-(float)pImage->GetHeight());
//	}
//
//	pGr->DrawImage(pImage, _x, _y);
//
//	return TRUE;
//}
//
/////
///// TextObject
/////
//
//// ------------------------------
//TextObject::TextObject()
//			: cyiDrawObject(eOT_IMAGE)
//{
//	m_str=NULL;
//	m_font=NULL;
//	m_brush=NULL;
//}
//
//// ------------------------------
//TextObject::~TextObject()
//{
//	if (m_bFontNew==FALSE)
//		delete m_font;
//
//	if (m_str)
//		delete m_str;
//
//	if (m_brush)
//		delete m_brush;
//}
//
//// ------------------------------
//BOOL TextObject::Create(const char* pszText, wchar_t* FontName, float fontsize, Color& color, BOOL bCanScale)
//{
//	wchar_t buffer[MAX_STRING+1];
//	UINT size;
//
//	//if (::mbstowcs_s(&size, buffer, MAX_STRING, pszText, strlen(pszText))!=0)
//	if (::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszText, -1, buffer, MAX_STRING)==0)
//	{
//		return FALSE;
//	}
//
//	return Create(buffer, FontName, fontsize, color, bCanScale);	
//}
//
//// ------------------------------
//BOOL TextObject::Create(const wchar_t* pszText, wchar_t* FontName, float fontsize, Color& color, BOOL bCanScale)
//{
//
//	m_font = new Font(FontName, fontsize);
//
//	return Create(pszText, m_font, color, bCanScale);
//}
//
//// ------------------------------
//BOOL TextObject::Create(const char* pszText, UINT size, Font* font, Color& color, BOOL bCanScale)
//{
//	wchar_t buffer[MAX_STRING+1];
//
//	if (::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszText, -1, buffer, MAX_STRING)==0)
//	{
//		return FALSE;
//	}
//
//	return Create(buffer, font, color, bCanScale);	
//}
//
//// ------------------------------
//BOOL TextObject::Create(const wchar_t* pszText, Font* font, Color& color, BOOL bCanScale)
//{
//	m_fontsize	= font->GetSize();
//	m_txtlen	= ::wcslen(pszText);
//	m_str		= new wchar_t[m_txtlen+1];
//	::wcscpy(m_str, pszText);
//	m_brush = new SolidBrush((Color)color);
//
//
//
//	if (m_font==NULL)
//	{
//		m_bFontNew = FALSE;
//		m_font=font;
//	}
//	else
//	{
//		m_bFontNew = TRUE;
//	}
//
//	m_bCanScale=bCanScale;
//
//
//	
//	//RectF bound;
//	//int len = wcslen(pszText);
//
//
//	//Status ret = ::MeasureString(pszText, len, m_font, RectF(0, 0, (float)1024, (float)100), &bound);
//
//	//m_w = (UINT)(bound.Width)+1;
//	//m_h = (UINT)(bound.Height)+1;
//
//	return TRUE;
//}
//
//// ------------------------------
//BOOL TextObject::Create(UINT id, HINSTANCE hInstance, BOOL bCanScale)
//{
//	return TRUE;
//}
//
//
//// ------------------------------
//void TextObject::SetScale(float scale)
//{
//	if (m_bCanScale==FALSE)
//	{
//		return ;
//	}
//
//	if (m_font!=NULL)
//		delete m_font;
//
//	m_scale = scale;
//
//	m_font = new Font(m_fontname, m_scale*m_fontsize);
//}
//
//// ------------------------------
//BOOL TextObject::Draw(Graphics* pGr, float x, float y,  eDrawAlign align)
//{
//	
//	pGr->DrawString(m_str, m_txtlen, m_font, RectF(x, y, 0, 0), NULL, m_brush);
//	return TRUE;
//}
