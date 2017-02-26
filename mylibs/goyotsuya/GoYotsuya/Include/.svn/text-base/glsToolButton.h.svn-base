#pragma once
#include "glsStackPannel.h"

enum eGlsToolButtonType {
	eGTBT_ALEFT=0x10,
	eGTBT_ARIGHT,
	eGTBT_AUP,
	eGTBT_ADOWN,
	eGTBT_PLUS,
	eGTBT_MINUS,
	eGTBT_CROSS,
	eGTBT_CHAR,
	eGTBT_USER
};

enum eGlsToolButtonShapeStyle {
	eGTBSS_FILL,
	eGTBSS_DRAW,
	eGTBSS_USERDRAW
};

typedef struct {
	sGlsStackPannelDetail stack;
	float fScale;
	float fShadowWidth;
}sGlsToolButtonDetail;

class xEXPORT  cglsToolButton : public cglsStackPannel, public cglsDrawObject
{
public:
	cglsToolButton(void);
	virtual ~cglsToolButton(void);

	// --------------------------------------------------
	static void xDefaultDetail(sGlsToolButtonDetail* detail);

	// --------------------------------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, int w, int h, sGlsToolButtonDetail* detail=0);

	// --------------------------------------------------
	Bool Add(eGlsToolButtonType type, eGlsToolButtonShapeStyle style, cglsDrawObject* obj=0);

protected:
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// ------------------------------------
	virtual Bool OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX=0, int offsetY=0);

	// ---------------------------------------
	virtual LRESULT OnClicked(sDrawItem* item);

	// ------------------------------------
	virtual LRESULT OnLButtonRepeated(WPARAM wp, LPARAM lp);



	// ------------------------------------
	float m_fScale;


};
