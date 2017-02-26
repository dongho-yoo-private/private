#pragma once

#include "glsDrawObject.h"

enum eGlsResourceId {
	eGRID_SHADOW=1,
	eGRID_NEXT,
	eGRID_TOP,
	eGRID_PP,
	eGRID_NN,
	eGRID_PREV,
	eGRID_END,
	eGRID_CLOSE_BTN,
	eGRID_GLS_HILIGHT,
	eGRID_SCROLL_UP,
	eGRID_SCROLL_DOWN,
	eGRID_SCROLL_LEFT,
	eGRID_SCROLL_RIGHT,
	eGRID_ARROW_UP,
	eGRID_ARROW_DOWN,
	eGRID_ARROW_LEFT,
	eGRID_ARROW_RIGHT,
	eGRID_ICON_MENU,
	eGRID_ICON_DETAIL,
	eGRID_NOT_SHOW_NO,
	eGRID_SHOW_NO,
	eGRID_SHOW_NO_FROM,
	eGRID_TOOL_ALEFT,
	eGRID_TOOL_ARIGHT,
	eGRID_TOOL_AUP,
	eGRID_TOOL_ADOWN,
	eGRID_TOOL_PLUS,
	eGRID_TOOL_MINUS,
	eGRID_TOOL_CROSS,
	eGRID_TOOL_CHAR,
	eGRID_RADIO,
	eGRID_CHECK,
	eGRID_ICON_MESSAGE,
	eGRID_CHANGED_MARK,
	eGRID_NEXT_BRANCH,
	eGRID_PREV_BRANCH,
	eGRID_PASS
};

enum eGlsDrawId {
	eGDID_GLASS_BTN_PANNEL=1,
	eDID_TAB_ITEM_BG,
	eDID_TAB_BG,
	eDID_SCROLL_BAR,
	eDID_SCROLL_PAGE,
	eGDID_GLASS_SPLIT_PANNEL,
	eGDID_GLASS_TOOL_PANNEL,
	eGDID_NEON_BTN_PANNEL,
	eGDID_NEON_SPLIT_PANNEL,
	eGDID_CHAR_IN_CIRCLE
};


// ------------------------------
typedef struct xsGeneralDrawItem {
	RECT rect[2];
	Bitmap* pBmp[2];
	color_t aColor[2];
	Brush* pBrush[2];
	Pen* pPen[2];
} sGeneralDrawItem;

// ------------------------------
typedef struct xsGeneralDrawItemEx {
	RECT rect[4];
	Bitmap* pBmp[4];
	color_t aColor[4];
	Brush* pBrush[4];
	Pen* pPen[4];
} sGeneralDrawItemEx;


class xEXPORT  cglsResourceBitmap : public cglsResourceObject
{
public:
	cglsResourceBitmap(void);
	~cglsResourceBitmap(void);

	// ---------------------------------------
	virtual Bool Initialize();

	// ---------------------------------------
	virtual Bitmap* GetBitmap(int id, int subId, IntPtr w, IntPtr h, color_t color=0, void* extra=0);

	// ---------------------------------------
	virtual Bitmap* Draw(int id, int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra=0);


};
