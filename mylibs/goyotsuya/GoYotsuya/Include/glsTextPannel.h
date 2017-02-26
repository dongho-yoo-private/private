#pragma once

#include "glsDialog.h"
#include "glsEditBox.h"
#include "glsStatic.h"
#include "glsSystem.h"
#include "yctrls.h"

enum eGlsTextPannelStyle {
	eGTPS_EDIT,
	eGTPS_PLAY,
	eGTPS_JOSEKI
};
class cglsTextPannel : public cglsDialog, public cglsKeyCenter
{
public:
	cglsTextPannel(void);
	~cglsTextPannel(void);

	// -----------------------------
	Bool Create(HWND hWnd, int id, int x, int y, int w, int h, Bool bIsBlack, sGoHeader* header, eGlsTextPannelStyle style=eGTPS_EDIT, Font* font=0);//, sGlsPlayerPannelDetail* detail);

	// -----------------------------
	void SetTextPannelStyle(eGlsTextPannelStyle style);

	// -----------------------------
	void SetText(wchar_t* buffer);

	// -----------------------------
	wchar_t* GetText(int* length);

	// -----------------------------
	void AppendText(const wchar_t* buffer);

	// -----------------------------
	Bool IsMessageInGobanVisible();

	// -----------------------------
	void SetEnableInput(Bool bIsEnable);


protected:
	// ------------------------------------
	virtual Bool OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX, int offsetY);

	// ------------------------------------
	void OnItemReArranged();

	// -----------------------------
	void Update(Bitmap* bmp, RECT* rect, RECT* _childRect, int offsetX, int offsetY, Bool bIsLocalBufferUpdate);

	// --------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	//------------------------------------
	virtual void OnChar(int nKeyCode, cGlassInterface* from);

	// ------------------------------------
	virtual void OnImeChar(int nKeyCode, cGlassInterface* from);

	// --------------------------------------
	virtual void OnSysKey(int nKeyCode, cGlassInterface* from);

	// --------------------------------------
	virtual void OnStartComposition(unsigned long param, cGlassInterface* from);

	// --------------------------------------
	virtual void OnComposition(unsigned long param, cGlassInterface* from);

	// --------------------------------------
	virtual void OnEndComposition(unsigned long param, cGlassInterface* from);

	// ------------------------------------
	LRESULT OnNodeChanged(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ------------------------------------
	LRESULT OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

private:
	cglsEditBox* m_editor;

	// ------------------------------------
	cglsEditBox* m_inputEditor;

	// ------------------------------------
	cglsCheckGroup* m_check;

	cglsEditBox* m_currFocusEditor;

	cyiTreeNode* m_currentNode;

	color_t m_bgColor;

	eGlsTextPannelStyle m_eStyle;

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnNodeChanged, WM_NODE_CHANGED)
	MESSAGE(OnSizeChanged, WM_SIZE)
DEFINE_MESSAGES_END

// ------------------------------------
//DEFINE_MESSAGES_BEGIN
//	MESSAGE(OnSizeChanged, WM_SIZE)
//DEFINE_MESSAGES_END
//	MESSAGE(OnImeChar, WM_IME_CHAR)
//	MESSAGE(OnChar, WM_CHAR)
//	MESSAGE(OnSysKeyDown, WM_KEYDOWN)
//	MESSAGE(OnSysKeyUp, WM_KEYUP)
//	MESSAGE(OnStartComposition, WM_IME_STARTCOMPOSITION)
//	MESSAGE(OnComposition, WM_IME_COMPOSITION)

};
