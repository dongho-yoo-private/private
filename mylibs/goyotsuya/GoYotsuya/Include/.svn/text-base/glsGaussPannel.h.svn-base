#pragma once
#include "GlassInterface.h"


typedef struct xsGlsGaussPannelDetail {
	float fShadowWidth;
	float fBlureRad;
	float fEdge;
	color_t colorTop;
	color_t colorBottom;
} sGlsGaussPannelDetail;

// ------------------------------------
class xEXPORT  cglsGaussPannel : public cGlassInterface
{
public:
	cglsGaussPannel(void);
	virtual ~cglsGaussPannel(void);


	// --------------------------------
	static void xDefaultDetail(sGlsGaussPannelDetail* detail);

	// --------------------------------
	void SetGaussPannelDetail(sGlsGaussPannelDetail* detail);

	// ------------------------------------
	virtual Bool OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX=0, int offsetY=0);

	// ------------------------------------
	virtual void Destroy();


	// -----------------------------
	virtual Bitmap* GetBgBitmap();

	// -----------------------------
	virtual Bool InvalidateRect(RECT* lpRect, Bool bErase=0, Bool bIsDirect=False);

	// -----------------------------
	virtual Bool GetClientRect(LPRECT lpRect);


protected:
	// ------------------------------------
	virtual Bitmap* GetParentBg(Bitmap* bmp=0, RECT* rect=0, cGlassInterface* sender=0);

	// -----------------------------
	virtual void Update(Bitmap* bmp, RECT* rect=0, RECT* _childRect=0, int offsetX=0, int offsetY=0, Bool bIsLocalBufferUpdate=False);


	HandleShadow m_hShadow;
	Bitmap* m_bmpBehind;
	Bitmap* m_bmpShadow;
	Bitmap* m_bmpFrame;
	Bitmap* m_bmpBluredBg;

	float m_fBlureRad;
	float m_fEdge;
	float m_fShadowWidth;

	color_t m_colorTop;
	color_t m_colorBottom;

	//Bool m_bIsAlwaysDraw;

	Bool m_bIsAllRangeDraw;

	Bool m_bIsDirect;

	int m_prevUpdateOrder;
};
