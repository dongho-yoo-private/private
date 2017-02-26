#pragma once

#include "glsStackPannel.h"

typedef struct 
{
	Bool bIsNumeric;
	color_t color;
	eGlsGuiStyle style;
} sGlsPageScrollDetail;


class xEXPORT cglsPageScroll : public cglsStackPannel
{
public:
	cglsPageScroll(void);
	~cglsPageScroll(void);

	// ---------------------------------------------
	static void xDefaultDetail(sGlsPageScrollDetail* detail);

	// ---------------------------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, Font* font, int nPageCount, Bool bHasArrowButton, sGlsPageScrollDetail* detail);

	// ---------------------------------------------
	int SetPageCount(int n);

	// ---------------------------------------------
	int GetPage();

	// ---------------------------------------------
	int SetPage(int n);

protected:
	// ---------------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

private:
	// ---------------------------------------------
	Bool xCreateButtonz();

	// ---------------------------------------------
	Bool m_bHasArrowButton;

	// ---------------------------------------------
	color_t m_color;

	int m_nPageCount;
};
