#pragma once

#include "GlassInterface.h"


typedef struct  {
	Bool bShowSearchCtrl;
	Bool bShowScrollBar;
	Bool bShowInfoView;
	Bool bShowSelect;
	Bool bShowDelete;
	int  nPreviewHeight;
	color_t colorText;
	color_t colorBg;
	Font* font;
} sGlsGoListDetail;

// ------------------------------
class cglsGoListView : cGlassInterface
{
public:
	cglsGoListView(void);
	~cglsGoListView(void);

	static void xDefaultDetail(sGlsGoListDetail* detail);

	// ------------------------------
	Bool Create(cGlassInterface* gls, int x, int y, int w, int h, sGlsGoListDetail* detail=0);

	// ------------------------------
	Bool AddItem(int id, const char* pszBlack, const char* pszWhite, int teai, int cnt, unsigned int result, unsigned int day, unsigned int time, color_t color=0);

	// ------------------------------
	Bool InsertItem(int id, int index, const char* pszBlack, const char* pszWhite, int teai, int cnt, unsigned int result, unsigned int day, unsigned int time, color_t color=0);

	// ------------------------------
	void ClearItem();

	// ------------------------------
	int UpdateItem(int id, int index, const char* pszBlack, const char* pszWhite, int teai, int cnt, unsigned int result, unsigned int day, unsigned int time, color_t color=0);

	// ------------------------------
	int GetCurrentIndex();

	// ------------------------------
	int IndexToId(int index);

	// ------------------------------
	virtual void Destroy();

protected:
	// ------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam);

private:
	// ------------------------------------
	void xCreateSearchControl();

	// ------------------------------------
	void xCreateInfoControl();

	// ------------------------------------
	void OnThreadStart(int id, int param1, int param2);


	// ------------------------------------
	cglsStackPannel* m_searchCtrl;

	// ------------------------------------
	// とりあえず、ここには全ての棋譜リストをもたらそう。
	cyiList* m_goz;

	unsigned int m_xflag;
	color_t m_color;
	color_t m_colorBg;

};
