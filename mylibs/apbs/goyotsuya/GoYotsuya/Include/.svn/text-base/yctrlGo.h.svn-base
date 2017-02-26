#pragma once
#include "yctrlbase.h"
#include "yctrlGoban.h"
#include "yctrlController.h"

class cyctrlGo :
	public cyctrlbase
{
public:
	cyctrlGo(void);
	~cyctrlGo(void);

protected:

	// ----------------------------------------
	virtual BOOL OnPaint(HDC hdc);

	// ----------------------------------------
	virtual LRESULT OnCreate(WPARAM wp, LPARAM lp);


private:
	cyctrlGoban* m_goban;
	cyctrlController* m_ctrler;
	cyiSurface* m_surface;
};
