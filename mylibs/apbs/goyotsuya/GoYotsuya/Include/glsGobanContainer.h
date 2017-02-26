#pragma once

#include "glsWindow.h"
#include "glsGoban.h"

class cglsGobanContainer : public cglsWindow
{
public:
	cglsGobanContainer(void);
	~cglsGobanContainer(void);

	Bool SetStream(IStream* stream);

	// -------------------------------------
	Bool ReSize(UINT x, UINT y, Bool bIsRepaint=0);

protected:
	// -------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// -------------------------------------
	virtual LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);

	// -------------------------------------
	virtual Bool OnBgDraw();

	// -------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& handled);


	cglsGoban* m_goban;
};
