#pragma once
#include "GlassControlz.h"
#include "glsDialog.h"
#include "ypnGoNetwork.h"

enum eyigUserDialogMode
{
	eUDM_FULL,
	eUDM_RPY,
	eUDM_NORMAL
};

class cyigUserDialog : public cglsDialog
{
public:
	cyigUserDialog(void);
	~cyigUserDialog(void);

	// -----------------------------------
	static Bool xDefaultCondition(int id, unsigned int& condition1, unsigned int& condition2);

	// -----------------------------------
	Bool Show(cGlassInterface* parent, int groupId, int userId, 
								sGoGroupInfo* loginedGroup, CUserList* listUser, sGoGroupInfo* generalGroup=0, 
									int nSelectedRule=0, sGoYotsuyaGoRules* ruleList=0, Font* font=0);

	// -----------------------------------
	Bool Show(cGlassInterface* parent, const char* pszFrom, unsigned int con1, unsigned con2, cyiList* rules=0, Font* font=0);

	// -----------------------------------
	Bool Show(cGlassInterface* parent, unsigned int remoteId, const char* pszRemoteUserName, unsigned int con1, unsigned int con2, Font* font=0);

	// -----------------------------------
	HandleUser GetValue(unsigned int& condition1, unsigned int& condition2);

protected:
	// -----------------------------------
	Bool Create(cGlassInterface* parent, const wchar_t* pszCaption, Font* font);

	// --------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ------------------------------------
	//LRESULT OnTimer(WPARAM wParam, LPARAM lParam, Bool& handled);

private:

	void xShowContents();

	// -----------------------------------
	void xEnalbeLimitedTime();

	// -----------------------------------
	void xShowNigiri();

	// -----------------------------------
	void xEnalbeReadSecond();

	// -----------------------------------
	void xResetControlState();

	// -----------------------------------
	void xSetValue(unsigned int condition1, unsigned int condition2);

	// -----------------------------------
	void xGetValue(unsigned int& condition1, unsigned int& condition2);

	// ------------------------------------
	void UpdateUserForMyFriends();

	// ------------------------------------
	void UpdateUserListForNoId();

	// ------------------------------------
	virtual void OnExitLoop();

	//eyigUserDialogMode m_eMode;

	// -----------------------------------
	unsigned int m_condition1;

	// -----------------------------------
	unsigned int m_condition2;

	// -----------------------------------
	sGoYotsuyaGoRules* m_rules;

	eyigUserDialogMode m_mode;

	// -----------------------------------
	CUserList* m_userz;

	// -----------------------------------
	cyiList* m_rulez;

	// -----------------------------------
	sGoGroupInfo* m_loginedGroup;

	// -----------------------------------
	sGoGroupInfo* m_generalGroup;

	// -----------------------------------
	int m_nSelectedRule;

	// -----------------------------------
	sGoYotsuyaGoRules m_remoteRule;

	// -----------------------------------
	HandleUser m_hSelectedUser;

//DEFINE_MESSAGES_BEGIN
//	MESSAGE(OnTimer, WM_TIMER)
//DEFINE_MESSAGES_END




};
