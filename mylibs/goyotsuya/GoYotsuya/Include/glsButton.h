#pragma once

#include "GlassInterface.h"
#include "glsDrawObject.h"

class cglsBgManager;

#define ITEM_NOCHANGED (-1)

typedef struct xsGlsButtonDetail {
	float   edge;
	color_t colorHilight; //__COLORp__(0, 127, 255, 127)
	RECT rect;
	Bool bIsMouseDownNotify;
	Bool bIsOnOff;
	Bool bHasBorder;
	unsigned int bmpOnOff;
	wchar_t* pszOnOff;
	sGlsDetail gls;
} sGlsButtonDetail;



class xEXPORT cglsButton : public cGlassInterface, public cglsDrawObject
{
public:
	cglsButton(void);
	virtual ~cglsButton(void);

	// --------------------------------
	static void xDefaultDetail(sGlsButtonDetail* detail);

	// --------------------------------
	static IntPtr GetMeasureHeight(Font* font);

	// ------------------------------------
	Bool Create(HWND hWndParent, int id, int x, int y, int w, int h, const char* pszCaption);

	// ------------------------------------
	Bool Create(HWND hParent, int id, int x, int y, Bitmap* bmp, const wchar_t* pszText, const wchar_t* pszToolTip=0, sGlsButtonDetail* detail=0, cglsDrawObject* obj=0);

	// ------------------------------------
	Bool SetItemChange(Bitmap* bmp, wchar_t* pszText);

	// ------------------------------------
	Bool IsOn();

	// ------------------------------------
	Bool SetOnOff(Bool bIsOnOff);


protected:
	// --------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// --------------------------
	virtual int GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation);



private:
	sGlsButtonDetail m_detail;
};



