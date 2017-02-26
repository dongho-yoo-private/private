#ifdef _GOYOTSUYA

#pragma once

#include "glsdialog.h"
#include "ypnGoNetwork.h"

class cyigRegisterDialog :
	public cglsDialog
{
public:
	cyigRegisterDialog(void);
	~cyigRegisterDialog(void);

	// ------------------------------------------------------
	Bool RegisterUser(cGlassInterface* parent, socket_t sock, Font* font=0);

	// ------------------------------------------------------
	Bool GetValue(char* szUserName, char* password);

protected:
	// --------------------------------------------
	Bool ShowHelp(const wchar_t* help);

	// --------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	virtual void OnExitLoop();

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIshandled);

private:
	// --------------------------------------------
	void xSendCode();

	// --------------------------------------------
	void xSendEmail();

	// --------------------------------------------
	void xRegisterUser();

	// --------------------------------------------
	Bool xCheckValue(wchar_t* buffer);


	char m_szId[16];
	char m_szPassword[8];

	unsigned int m_userid;

	unsigned int m_page;

	socket_t m_sock;

	char m_szEmailAddress[81];

};
#endif