#pragma once
#include "glsStackPannel.h"

// ---------------------------------------
typedef struct xsGlsScrollBarDetail {
	int  nBarMinSize;
	float fTransparency;
	Bool bIsVertical;
} sGlsScrollBarDetail;

// ---------------------------
class xEXPORT cglsScrollBar : public cglsStackPannel
{
public:
	cglsScrollBar(void);
	~cglsScrollBar(void);

	static void xDefaultDetail(sGlsScrollBarDetail* detail);

	// ---------------------------
	Bool Create(cGlassInterface* parent, sGlsScrollBarDetail* detail=0);

	// ---------------------------------------
	Bool SetRange(int min, int max, int pagesize);

	// ---------------------------------------
	void SetPos(int n);

	// ---------------------------------------
	int GetPos();

};
