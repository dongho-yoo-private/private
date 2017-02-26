#pragma once

#include "glsRadioGroup.h"

typedef struct {
	sGlsDetail gls;
	color_t colorCheck;
	color_t colorCheckBorder;
	color_t colorText;
	Bool	bIsAutoArrange;
	int		nDistance;
}sCheckGroupDetail;

// ----------------------------------------
class cglsCheckGroup;
xEXPORT cglsCheckGroup* xcglsCheckGroup();

// ----------------------------------------
class xEXPORT cglsCheckGroup : public cglsRadioGroup
{
public:
	cglsCheckGroup(void);
	~cglsCheckGroup(void);

	// -----------------------------
	static void xDefaultDetail(sCheckGroupDetail* detail);

	// -----------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, sCheckGroupDetail* detail=0);

	// -----------------------------
	virtual void SetChecked(int index);

	// -----------------------------
	void SetChecked(int index, Bool bIsChecked);

	// -----------------------------
	Bool GetChecked(int index, int id=-1);

};
