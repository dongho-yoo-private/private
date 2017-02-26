#pragma once

#include "yistd.h"
#include "yctrlbase.h"
#include "yiList.h"
#include "cyctrlxII.h"


enum eDlgButtonSet {
	eDBS_OK,
	eDBS_OKCANCEL,
	eDBS_YESNO,
	eDBS_ADJUSTCANCEL
};

//extern int yiMessageBox(HWND hWnd, UINT msgId, int x, int y, int w, int h, const wchar_t* pszMessage, int buttons);

#define MAX_UI_COUNT 16

class cydlgx : public cyctrlbase
{
public:
	cydlgx(void);
	~cydlgx(void);

	// ----------------------------------
	Bool Create(HWND hWndParent, int wMargin, int hMargin);


	// ----------------------------------
	//Bool AddCtrlx(cyctrlxII* ctrl, int x, int y, eCtrlxItemOffset offset/*eTOP_LEFT*/);

	// ----------------------------------
	//int DoModal(cyctrlbase* base, int x, int y, int w, int h, const wchar_t* pszMessage, int DefaultButton);

	// ----------------------------------
	int DoModal(HWND hWndParent, int x, int y, int w, int h, const wchar_t* pszMessage, int DefaultButton);

	// --------------------------------------------
	int DoModal(cyctrlbase* base, const wchar_t* pszMessage, int DefaultButton, HWND hWndLoop);

	// --------------------------------------------
	// for dialog.
	int DoModal(cyctrlbase* base, const wchar_t* pszCaption, int w, int h, Bool bIsModal=False);

	// --------------------------------------------
	virtual void OnDefineUiz(cyctrlxII* uiz, int w, int h){};

	// --------------------------------------------
	void ChangeBackground(Bitmap* bmp);

	// --------------------------------------------
	void OnDrawMessageBox(Bitmap* bg);

	// --------------------------------------------
	void OnDrawDialog(Bitmap* bg);

	// ----------------------------------
	int DoModal();

	// ----------------------------------
	void DrawDefDlgBg();

	// ----------------------------------
	void SetUiz(int index, cyctrlbase* ui);

	// --------------------------------------------
	void  UpdateUiz(int index=-1);

	// ----------------------------------
	void UseDefaultButton(eDlgButtonSet buttonSet);

protected:
	// ---------------------------------------
	virtual BOOL OnPaint(HDC hdc, PAINTSTRUCT* ps=0);

	// ---------------------------------------
	virtual void OnIDOK();
	
	// ---------------------------------------
	virtual void OnIDCancel();

	// ---------------------------------------
	virtual void OnUiInitializeEnd(){};

	// ---------------------------------------
	LRESULT OnSize(WPARAM wp, LPARAM lp, BOOL& handled);

	// ---------------------------------------
	LRESULT OnIdOk(HWND hWnd, WPARAM wp);

	// ---------------------------------------
	LRESULT OnIdCancel(HWND hWnd, WPARAM wp);

	// ------------------------------------
	virtual Bool OnCtrlxDraw(int id, int subId, Bitmap* srf, Bitmap* bg, Font* fnt, sControlxII* s, eControlxEventII eventId);

	// ------------------------------------
	virtual Bool OnCtrlxBgDraw(int id, Bitmap* srf, int x, int y);

protected:

	Bool OnDefButtonDraw(Bitmap* srf, Bitmap* bg, Font* fnt, sControlxII* s, eControlxEventII eventId, int resId);

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnSize, WM_SIZE)
DEFINE_MESSAGES_END

DEFINE_COMMANDS_BEGIN
	COMMAND(OnIdOk, IDOK)
	COMMAND(OnIdOk, IDCANCEL)
DEFINE_COMMANDS_END



	UINT m_wMargin;
	UINT m_hMargin;
	int m_nItemStartY;
	Bool m_bIsCreated;
	//cyiList m_ctrlxs;
	//cyctrlxII m_uiz;
	cyctrlbase* m_uiz[MAX_UI_COUNT];
	int m_result;
	HWND m_hWndParent;
	int m_id;
	int m_nResult;
	Bool m_bIsModal;
	Bool m_bIsMessageBox;

	// for message box
	Bitmap* m_pBg;
	Bitmap* m_pStr;
	Bitmap* m_pMask;
	Bitmap* m_pShadow;
	cyctrlxII* m_pBtns;
};


// ----------------------------------
extern int yiMessageBox(cyctrlbase* base, int x, int y, const wchar_t* pszMessage, int buttons);
extern int yiMessageBox(HWND hWnd, int x, int y, const wchar_t* pszMessage, int buttons);
extern int yiMessageBox(cyctrlbase* base, const wchar_t* pszMessage, int buttons, cydlgx** out, HWND hWndLoop=0);
