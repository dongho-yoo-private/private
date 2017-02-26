#pragma once

#include "yctrlbase.h"

#define WM_CLOSED_PREVIEW_DETAIL WM_USER+512

enum eFirstCondition {
	eFC_ALL=0,
	eFC_JOSEKI=1,
	eFC_HAMETE=1<<1,
	eFC_JOSEKI_AFTER=1<<2,
	eFC_JOSEKI_TENUKI=1<<3,
	eFC_CASE_BY=1<<4,
	eFC_OTHER=1<<5
};

typedef struct xsSearchFilter {
	unsigned int  first_condition;
	unsigned char second_condition[32];
} sSearchFilter;

// -------------------------------
class xEXPORT cyctrlDlgPreviewDetail : public cyctrlbase
{
public:
	cyctrlDlgPreviewDetail(void);
	~cyctrlDlgPreviewDetail(void);

	// -------------------------------
	Bool CreateFromReosurce(HWND hWnd, HINSTANCE hInstance, int id, sSearchFilter* filter);

private:
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnDrawItem, WM_DRAWITEM);
DEFINE_MESSAGES_END
//
//DEFINE_COMMANDS_BEGIN
//	COMMAND(OnOK, IDOK);
//	COMMAND(OnOK, IDCANCEL);
//	COMMAND(OnAllCheck, IDC_CHK_ALL);
//DEFINE_COMMANDS_END

	// -------------------------------
	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp, Bool& Handled);

	// -------------------------------
	LRESULT OnDrawItem(WPARAM wp, LPARAM lp, Bool& Handled);

	// -------------------------------
	LRESULT OnOK(HWND hWnd, int code);

	// -------------------------------
	LRESULT OnCancel(HWND hWnd, int code);

	// ------------------------------------
	LRESULT OnAllCheck(HWND hWnd, int code);


	// ---------------------------------------
	virtual BOOL OnPaint(HDC hdc, PAINTSTRUCT* ps=0);

	// ------------------------------------
	virtual LRESULT OnCreate(WPARAM wp, LPARAM lp);

	// ------------------------------------
	virtual void OnThreadStart(int id);

private:
	// -------------------------------
	void xMakeSearchCondition();

	// ------------------------------------
	void xInitInterface();

	// ------------------------------------
	void xCheckAllEnable(Bool bIsEnable);



	Bitmap* m_bmpBg;
	Bitmap* m_bmpBgBlure;
	HandleShadow m_hShdow;
	sSearchFilter* m_filter;
};
