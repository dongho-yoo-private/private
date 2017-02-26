#pragma once

#include "glsDrawObject.h"

class cButtonSet : public cglsDrawObject
{
public:
	cButtonSet(void);
	cButtonSet(int id);
	~cButtonSet(void);

	void SetBgColor(color_t color, color_t color2);

protected:
	// --------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// --------------------------
	virtual void DestroyBitmap(sDrawItem* item);

	color_t m_bgColor;
	color_t m_fgColor;
};
