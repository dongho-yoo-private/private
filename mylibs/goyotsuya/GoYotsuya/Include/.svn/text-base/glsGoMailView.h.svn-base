#pragma once
#include "glsDialog.h"

// ----------------------------
class xEXPORT cglsGoMailView : public cglsDialog
{
public:
	cglsGoMailView(void);
	virtual ~cglsGoMailView(void);

	// ----------------------------
	Bool Create(cGlassInterface* parent, int x, int y, int w, int h, cyiList* email, Font* font=0);

	// ----------------------------
	Bool UpdateMyView(int tabIndex=0);
	
protected:
	// ------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	cyiList* m_servers;

	// ------------------------------------
	int m_nLastSelctedTabId;
};
