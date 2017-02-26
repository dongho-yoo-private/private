#pragma once

#include "glsDialog.h"


class cglsQuestionDialog : public cglsDialog
{
public:
	cglsQuestionDialog(void);
	~cglsQuestionDialog(void);

	// ------------------------------
	int Show(cGlassInterface* parent, int nQuestion, Font* font=0);

protected:
	// --------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	virtual void OnExitLoop();

private:
	int m_nQuestion;
};
