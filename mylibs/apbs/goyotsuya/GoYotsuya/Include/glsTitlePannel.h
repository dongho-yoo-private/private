#pragma once

#include "glsDialog.h"
#include "glsEditBox.h"

typedef struct {
	sGlsGaussPannelDetail gauss;
} sGlsTitlePannelDetail;

#include "yctrls.h"

// ----------------------------------------
class cglsTitlePannel : public cglsDialog
{
public:
	cglsTitlePannel(void);
	~cglsTitlePannel(void);

	// ----------------------------------------
	Bool Create(HWND hwnd, int id, int x, int y, int w, int h,	const wchar_t* pszTitle, Font* font=0);

	// ----------------------------------------
	Bool SetTitle(const wchar_t* pszTitle);

	// ----------------------------------------
	wchar_t* GetTitle();

	// ----------------------------------------
	LRESULT OnTitleEdited(HWND hWnd, int code);

	// ----------------------------------------
	LRESULT OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ----------------------------------------
	LRESULT OnNotifyNodeChanged(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ----------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	//// ----------------------------------------
	//virtual void OnFontChanged();

	// ------------------------------------
	virtual int SetFont(Font* font);



private:
	wchar_t m_szCurrentTitle[256];
	cglsEditBox m_editor;

	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnSizeChanged, WM_SIZE)
		MESSAGE(OnNotifyNodeChanged, WM_NODE_CHANGED);
	DEFINE_MESSAGES_END

	DEFINE_COMMANDS_BEGIN
		COMMAND(OnTitleEdited, 1)
	DEFINE_COMMANDS_END
};
