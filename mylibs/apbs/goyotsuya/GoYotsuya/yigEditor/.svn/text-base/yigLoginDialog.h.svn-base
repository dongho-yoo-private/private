#pragma once
#include "glsDialog.h"


class cyigLoginDialog : public cglsDialog
{
public:
	cyigLoginDialog(void);
	virtual ~cyigLoginDialog(void);

	// -----------------------------------
	Bool Login(cGlassInterface* parent, wchar_t* pszName, wchar_t* pszPassword, int mode=0, Font* font=0);

	// -----------------------------------
	Bool Login(cGlassInterface* parent, char* pszName, char* pszPassword, int mode=0, Font* font=0);

	// -----------------------------------
	Bool IsNormalLogin();

	// -----------------------------------
	void* GetLoginData(int n);

protected:
	// --------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	virtual void OnExitLoop();

	//virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	LRESULT OnNotifyFromTab(int id, int subId, int code);

DEFINE_NOTIFY_BEGIN
	NOTIFY(OnNotifyFromTab, 16)
DEFINE_NOTIFY_END

private:
	void xShowContents();

	// -----------------------------------
	Bool Create(cGlassInterface* parent, Font* font);

	wchar_t m_szId[32];
	wchar_t m_szPass[32];

	Bool m_bIsNormalLogin;
};
