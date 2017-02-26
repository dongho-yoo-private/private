#pragma once
#include "glsDialog.h"
#include "ypnGoNetwork.h"

class cyigNoIdDialog : public cglsDialog
{
public:
	cyigNoIdDialog(void);
	~cyigNoIdDialog(void);

	// -----------------------------------
	Bool ConnectRoom(cGlassInterface* parent, unsigned int* remoteRoomNo, char* pszRemoteName, Font* font=0);

	// -----------------------------------
	Bool GetValue(int* nRoomNo, char* pszUserName);

protected:
	// --------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	//virtual void OnExitLoop();

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	char m_szRemoteName[16];

	// ------------------------------------
	int m_nRoomNo;
};
