#pragma once


#include "glsDialog.h"
#include "glsButton.h"
#include "glsTabCtrl.h"
#include "glsListMenu.h"
#include "glsMessageBox.h"
#include "svry/svrydefs.h"
#include "glsGobanCommon.h"

enum eGlsToolContainerStyle {
	eGTCS_MENU=1,
	eGTCS_FRIENDS=2,
	eGTCS_GROUPS=4,
	eGTCS_TAB=8,
	eGTCS_MSG=0x10,

	eGTCS_BEFORE_LOGIN		= (eGTCS_MENU|eGTCS_TAB),
	eGTCS_AFTER_LOGIN		= (eGTCS_MENU|eGTCS_MSG|eGTCS_TAB),
	eGTCS_AFTER_LOGIN_NOID	= eGTCS_BEFORE_LOGIN
};

class xEXPORT cglsToolContainer : public cglsDialog
{
public:
	cglsToolContainer(void);
	~cglsToolContainer(void);

	// ---------------------------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, int h, cglsPopupMenu* menu, eGlsToolContainerStyle style, HWND hWndMenuNotifyWnd, HWND hWndNotifyTab, Font* font=0, eGobanAppMode mode=eGAM_NORMAL);

	// ------------------------------------
	void SetPannelStyle(eGlsToolContainerStyle style);

	// --------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// --------------------------------------------
	cglsTabCtrl* GetTab();

	// --------------------------------------------
	void UpdateMessage(cyiList* list);

	// --------------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// --------------------------------------------
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// --------------------------------------------
	void AddMessage(sMsg* msg, Bool bShowMessage, const wchar_t* menuMsg=0);


private:
	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	LRESULT OnShowMenu(HWND hWnd, int code);

	// ------------------------------------
	eGlsToolContainerStyle m_eStyle;

	// ------------------------------------
	HWND m_hMenuNotifyWnd;

	// ------------------------------------
	HWND m_hTabNotifyWnd;

	// ------------------------------------
	cglsPopupMenu* m_menu;

	// ------------------------------------
	cglsListMenu* m_msgMenu;

	// ------------------------------------
	cyiList* m_listMsg;

	int m_nCurrSel;

	Bool m_bIsMessageButtonOn;

	eGobanAppMode m_mode;

	cglsMessageBox* m_msgBox;

	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnTimer, WM_TIMER)
	DEFINE_MESSAGES_END
//DEFINE_COMMANDS_BEGIN
//	COMMAND(OnShowMenu, 16)
//DEFINE_COMMANDS_END

};
