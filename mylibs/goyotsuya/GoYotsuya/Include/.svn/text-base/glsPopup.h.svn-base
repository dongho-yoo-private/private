#pragma once

#include "glsGaussPannel.h"

typedef struct  {
	sGlsDetail gls;
	sGlsGaussPannelDetail gauss;
	color_t colorFont;
	RECT margin;

}sGlsPopupDetail;

class cglsPopup : public cglsGaussPannel, public cglsDrawObject
{
public:
	cglsPopup(void);
	~cglsPopup(void);

	// -----------------------
	static void xDefaultDetail(sGlsPopupDetail* detail);

	// -----------------------
	Bool Create(HWND hWnd, int x, int y, int id, Bitmap* icon, const wchar_t* text, sGlsPopupDetail* detail=0);

	//// -----------------------
	//Bool Create(HWND hWnd, int x, int y, int id, cGlassInterface* obj, sGlsPopupDetail* detail=0);

	// -----------------------
	void Destroy();

	// -----------------------
	Bool Show(int nAnimationTime);

	// -----------------------
	Bool Hide(int nAnimationTime);

	// -----------------------
	Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// -----------------------
	virtual int GetDrawOrder();

private:
	color_t m_colorFont;
	RECT m_rectMargin;
};
