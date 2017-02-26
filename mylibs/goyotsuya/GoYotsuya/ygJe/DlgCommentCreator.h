#pragma once

#include "yistd.h"
#include "yctrlbase.h"
#include "resource.h"

class cDlgCommentCreator : public cyctrlbase
{
public:
	cDlgCommentCreator(void);
	~cDlgCommentCreator(void);

	char m_szResult[4096];
private:
DEFINE_COMMANDS_BEGIN
	COMMAND(OnCombo, IDC_CMB_OBJECT);
	COMMAND(OnCombo, IDC_CMB_SECOND);
	COMMAND(OnCombo, IDC_CMB_THIRD);
	COMMAND(OnCombo, IDC_CMB_FOURTH);
	COMMAND(OnBtn, IDC_GA);
	COMMAND(OnBtn, IDC_NA);
	COMMAND(OnBtn, IDC_HA);
	COMMAND(OnBtn, IDC_DEHA);
	COMMAND(OnBtn, IDC_MO);
	COMMAND(OnBtn, IDC_DE);
	COMMAND(OnBtn, IDC_NO);
	COMMAND(OnBtn, IDC_TEN);
	COMMAND(OnBtn, IDC_TEN2);
	COMMAND(OnLF, IDC_LF);
	COMMAND(OnBtn, IDC_NI);
	COMMAND(OnBtn, IDC_NIHA);
	COMMAND(OnBtn, IDC_NODE);
	COMMAND(OnBtn, IDC_BLACK);
	COMMAND(OnBtn, IDC_WHITE);
	COMMAND(OnBtn, IDC_TE);
	COMMAND(OnBtn, IDC_WO);
	COMMAND(OnClear, IDC_CLEAR);
	COMMAND(OnCombo, IDC_CMB_HISTORY);
DEFINE_COMMANDS_END


	// -----------------------
	virtual LRESULT OnCreate(WPARAM wp, LPARAM lp);

	// -----------------------
	virtual LRESULT OnOK();

	// -----------------------
	virtual LRESULT OnCancel();

	// -----------------------
	LRESULT OnBtn(HWND hWnd, int code);
	// -----------------------
	LRESULT OnLF(HWND hWnd, int code);
	//// -----------------------
	//LRESULT OnHA(HWND hWnd, int code);
	//// -----------------------
	//LRESULT OnDEHA(HWND hWnd, int code);
	//// -----------------------
	//LRESULT OnMO(HWND hWnd, int code);
	//// -----------------------
	//LRESULT OnDE(HWND hWnd, int code);
	//// -----------------------
	//LRESULT OnNA(HWND hWnd, int code);

	// -----------------------
	LRESULT OnClear(HWND hWnd, int code);

	// -----------------------
	LRESULT OnCombo(HWND hWnd, int code);

	//// -----------------------
	//LRESULT OnSecond(HWND hWnd, int code);

	//// -----------------------
	//LRESULT OnThird(HWND hWnd, int code);

	//// -----------------------
	//LRESULT OnFourth(HWND hWnd, int code);


	// -----------------------
	void xAddComment(const char* pszComment);

	// -----------------------
	void xSaveCComment(const char* pszName);

	cyiList m_history;
};
