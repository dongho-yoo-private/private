#pragma once

#include "yctrlbase.h"
#include "yistd.h"

// ------------------------------------------
enum eControllerButtonSet {
	eCBS_EDIT,
	eCBS_VIEW,
	eCBS_PLAY,
	eCBS_DEFAULT=eCBS_EDIT
};

enum eControllerButtonCommand {
	eCBC_NEXT=0,
	eCBC_PREV,
	eCBC_NNEXT,
	eCBC_PPREV,
	eCBC_SHOW_NUM,
	eCBC_AUTO_PLAY,
	eCBC_NEXT_COMMENT,
	eCBC_PREV_COMMENT,
	eCBC_NEXT_BRANCH,
	eCBC_PREV_BRANCH,
	eCBC_DELETE_BRANCH,
	eCBC_DETAILS
};

// ------------------------------------------
class cyctrlController : public cyctrlbase
{
public:
	cyctrlController(void);
	~cyctrlController(void);

	// ---------------------------------------
	BOOL AddCommand(const char* pszText, int id);

	// ---------------------------------------
	BOOL AddCommand(Bitmap* bmp, int id);

	// ---------------------------------------
	BOOL Create(HWND m_hWnd, int x, int y);

	// ---------------------------------------
	void Show(eControllerButtonSet btnSet=eCBS_DEFAULT);


protected:
	virtual BOOL OnPaint(HDC hdc);

	cyiList m_btnList;




};
