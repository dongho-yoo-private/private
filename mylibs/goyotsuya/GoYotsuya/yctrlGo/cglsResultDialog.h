#pragma once

#include "glsDialog.h"
#include "yiGoDefs.h"
#include "yiGoEngine.h"

typedef struct {

}sDetailResult;

class cglsResultDialog : public cglsDialog
{
public:
	cglsResultDialog(void);
	virtual ~cglsResultDialog(void);

	// --------------------------------------------
	Bool ShowResult(cGlassInterface* parent, goid_t id, sGoHeader* header, sResult* result=0, Font* font=0);

protected:
	// --------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp, BOOL& handled);

	sGoHeader* m_header;

	sResult m_result;

};
