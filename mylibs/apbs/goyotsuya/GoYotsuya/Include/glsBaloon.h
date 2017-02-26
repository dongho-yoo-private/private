#pragma once

#include "glsWindow.h"
#include "glsStatic.h"




typedef struct 
{
	sGlsDetail gls;
	float fShadowDepth;
	Brush* bg;
} sGlsBaloonDetail;

class cglsBaloon;

xEXPORT cglsBaloon* xcglsBaloon();

class xEXPORT cglsBaloon : public cGlassInterface
{
public:
	cglsBaloon(void);
	virtual ~cglsBaloon(void);

	// -----------------------------
	static void xDefaultDetail(sGlsBaloonDetail* detail);

	// -----------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, const wchar_t* pszContext, Bool m_bIsArrowUp, sGlsBaloonDetail* detail=0);

	// -----------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, const char* pszContext, Bool m_bIsArrowUp, sGlsBaloonDetail* detail=0);

	// -----------------------------
	Bool SetText(const wchar_t* pszText, Bool bIsArrowUp);



	// -----------------------------
	int GetText(wchar_t* pszText, int bufferSize);

	// -----------------------------
	void GetBaloonPos(POINT& pt);

	// -----------------------------
	virtual void Destroy();

	// -----------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// -----------------------------
	virtual void Show();

protected:
	// -----------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual Bool OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX=0, int offsetY=0);

	// -----------------------------
	virtual Bitmap* GetBgBitmap();

	// -----------------------------
	virtual LRESULT OnMouseEnter(WPARAM wParam, LPARAM lParam);

private:
	cglsStatic* m_comment;
	Bitmap* m_bg;
	Bool m_bIsArrowUp;
	float m_fShadowDepth;
	wchar_t m_wszContext[256];
	POINT m_pt;
};


