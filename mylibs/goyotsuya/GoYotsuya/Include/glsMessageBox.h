#pragma once
#include "glsDialog.h"

class xEXPORT cglsMessageBox : public cglsDialog
{
public:
	cglsMessageBox(void);
	virtual ~cglsMessageBox(void);

	// ---------------------------------------
	static cglsMessageBox* xNEW();

	// ----------------------------------------------
	// For modal
	static int Show(cGlassInterface* parent, const wchar_t* pszMessage, const wchar_t* pszCaption, int btn, Font* font=0);

	// ----------------------------------------------
	int ShowByTimer(cGlassInterface* parent, const wchar_t* pszMessage, const wchar_t* pszCaption, int btn, Bool bIsModal, int ntime, int nShowAnimationtime=0, int nHideAnimationTime=0, Font* font=0, Point* pt=0);


	// ----------------------------------------------
	// For modless
	int Show(int id, cGlassInterface* parent, const wchar_t* pszMessage, const wchar_t* pszCaption, int btn, Font* font=0, Point* pt=0);

	// ----------------------------------------------
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------------------
	void SetNotifyCode(int code);

	// --------------------------------
	virtual void Destroy();
protected:
	
	// ---------------------------------------------------
	int Create(int id, cGlassInterface* parent, const wchar_t* pszMessage, const wchar_t* pszCaption, int btn, Bool bIsModal=True, Font* font=0, int time=0, int nShowAnimationtime=200, int nHideAnimationTime=300, Point* pt=0);

	//// ---------------------------------------------------
	//void SetNotifyCode(int code);

	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ----------------------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	virtual void OnReqDestroy();

private:
	// ---------------------------------------------------
	wchar_t* m_pszMessage;

	// ---------------------------------------------------
	wchar_t* m_pszCaption;

	// ---------------------------------------------------
	POINT m_ptMessage;

	// ---------------------------------------------------
	int m_btnId;

	int m_nTimer;

	int m_nNotifyCode;

	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnTimer, WM_TIMER)
	DEFINE_MESSAGES_END


};
