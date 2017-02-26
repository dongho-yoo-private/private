#pragma once

#include "glsDialog.h"
#include "yctrls.h"

class cglsStoneInfoDlg : public cglsDialog
{
public:
	cglsStoneInfoDlg(void);
	~cglsStoneInfoDlg(void);

	// ----------------------------------------
	Bool Create(HWND hwnd, int id, int x, int y, int w, int h, cyiTreeNode* t, Font* font=0);

	// ----------------------------------------
	Bool UpdateTree(cyiTreeNode* t);

private:
	// ----------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual void OnItemReArranged();

	// ----------------------------------------
	LRESULT OnNotifyNodeChanged(WPARAM wParam, LPARAM lParam, Bool& handled);

	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnNotifyNodeChanged, WM_NODE_CHANGED);
	DEFINE_MESSAGES_END

	cyiTreeNode* m_node;

};
