#pragma once
#include "GlassInterface.h"
#include "glsWindow.h"

#include "glsListBox.h"


class xEXPORT cglsListMenu : public cglsWindow, public cGlsAssistant
{
public:
	cglsListMenu(Font* font, Bool bIsNoMouseDown=False, Bool bIsMoveNotify=False);
	~cglsListMenu(void);

	// ------------------------------------
	Bool TrackPopup(HWND hWndParent, int x, int y, int w, int h, Bool bIsSelectCheck=True, Font* font=0, Bool bIsAutoScroll=True, sGlsListBoxDetail* detail=0);
	

	// ------------------------------------
	Bool RemoveItem(int index);

	// ------------------------------------
	Bool AddItem(int id, const wchar_t* text, cGlsAssistant* assistant=0);

	// ------------------------------------
	Bool AddItem(int id, Bitmap* icon, const wchar_t* text, cGlsAssistant* assistant=0);

	// ------------------------------------
	void SetExceptWindow(cGlassInterface* obj);

	// ------------------------------------
	int GetItemCount();
	
	// ------------------------------------
	int GetIndex(int id);

	// ------------------------------------
	int GetResultCode();

	// ------------------------------------
	void SetCheck(int index, int id, Bool bIsChecked);

	// ------------------------------------
	void Close();

	// ------------------------------------
	void Clear();

	// -----------------------------------------------------
	virtual Bool ShowAssistant(HWND hWnd, int x, int y);

	// -----------------------------------------------------
	virtual Bool GetResult(wchar_t* buffer);

	// -----------------------------------------------------
	virtual Bool GetResult(unsigned int& result1, unsigned int& result2);

	// -----------------------------------------------------
	virtual Bool GetDefaultValue(wchar_t* buffer);

	// -----------------------------------------------------
	virtual Bool GetDefaultValue(unsigned int& result1, unsigned int& result2);

	// -----------------------------------------------------
	virtual Bool SetDefaultValue(wchar_t* str);

	// -----------------------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------
	LRESULT OnActivate(WPARAM wParam, LPARAM lParam, Bool&bIshandled);

	// ---------------------------------------
	LRESULT OnFocus(WPARAM wParam, LPARAM lParam, Bool&bIshandled);
// ------------------------------------
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnActivate, WM_ACTIVATE)
	MESSAGE(OnFocus, WM_SETFOCUS)
DEFINE_MESSAGES_END
private:
	cyiList m_TextList;
	int m_nTotalHeight;
	int m_nTotalWidth;
	int m_nResultCode;
	int m_nInitSelect;
	int m_nInitSelectIndex;
	int m_nMaxWidth;
	int m_nMaxHeight;
	int m_nCurrSel;

	Bool m_bIsNoMouseDown;
	Bool m_bIsMoveNotify;
	Bool m_bIsSelectionCheck;
	Bool m_bIsAutoScroll;

protected:
	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	cyiList* m_listAssistant;

private:
	sGlsListBoxDetail* m_pListBoxDetail;

	//cGlassInterface* m_exceptionObject;
};
