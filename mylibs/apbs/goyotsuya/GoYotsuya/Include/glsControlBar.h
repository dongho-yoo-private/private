#pragma once

#include "glsStackPannel.h"
#include "glsPageScroll.h"
#include "glsSlide.h"

typedef struct {
	sGlsGaussPannelDetail gauss;
	

} sGlsControlBarDetail;

class cglsControlBar : public cglsStackPannel
{
public:
	cglsControlBar(void);
	~cglsControlBar(void);

	// -------------------------------------------
	Bool Create(cGlassInterface* parent, int id, Font* font, eGlsItemAlign align, eGlsGuiStyle style, sGlsStackPannelDetail* detail);

	// -------------------------------------------
	int AddPage(cGlassInterface* obj);

	// -------------------------------------------
	int SetPageCount(int n);

	// -------------------------------------------
	cGlassInterface* GetPage(int id);

	// -------------------------------------------
	Bool ShowPage(int id, eSlideBarScrollDirection dir=eSBSD_TORIGHT);

	// -------------------------------------------
	virtual Bool ReSize(uInt32 x, uInt32 y, Bool bIsRepaint=False);



protected:
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual Bool OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX, int offsetY);

	// ------------------------------------
	virtual Bitmap* GetBgBitmap();

private:
	cglsPageScroll* m_scroll;
	cglsSlide*		m_slide;

	eGlsItemAlign m_align;
};
