#pragma once

#include "glsStackPannel.h"
#include "glsDrawObject.h"

typedef struct xsGlsPopupMenuDetail {
	sGlsStackPannelDetail stack;
	Bool bIsBlureInterface;
	float fBlureRadius;
	float fEdgeRadius;
	float fShadowWidth;
	float fSelectionEdgeRadius;
	color_t colorFont;
	color_t colorSelectionTop;
	color_t colorSelectionBottom;
	color_t colorSelectionLine;
} sGlsPopupMenuDetail;

//#define WM_GLS_TAB (WM_GLASSINTERFACE+32)
//#define WM_GLS_TAB_SELCHANGED (WM_GLS_TAB+1)


// --------------------------------------
class xEXPORT cglsPopupMenu : public cglsStackPannel, cglsDrawObject, public cGlsAssistant
{
public:
	cglsPopupMenu(void);
	~cglsPopupMenu(void);

	static void xDefaultDetail(sGlsPopupMenuDetail* detail);

	// -----------------------------------------------------
	Bool CreateFromResource(HWND hWnd, int id);

	// -----------------------------------------------------
	Bool CreateFromResource(HWND hWnd, HINSTANCE hInstance, int id);

	// -----------------------------------------------------
	Bool Create(HWND hWnd, int id, Bool bIsMenu=True);


	// -----------------------------------------------------
	Bool AddItem(int id, Bitmap* bmpIcon, wchar_t* pszText, cglsPopupMenu* child=0);


	// -----------------------------------------------------
	Bool Show(int x, int y, int align=TPM_LEFTALIGN | TPM_TOPALIGN );

	// -----------------------------------------------------
	Bool ShowAndWait(int x, int y, int align=TPM_LEFTALIGN | TPM_TOPALIGN );

	// -----------------------------------------------------
	Bool Create(HWND hWnd, int id, int x, int y, sGlsPopupMenuDetail* detail=0);

	// -----------------------------------------------------
	void SetCheck(int index, Bool bCheck, Bool bIsTypeRadio);

	// -----------------------------------------------------
	void Destroy();


	// -----------------------------------------------------
	//Bool AddItem(int id, Bitmap* icon, wchar_t* pszItemString, cglsPopupMenu* pChildMenu=0);

	// -----------------------------------------------------
	Bool InsertItem(int index, int id, Bitmap* icon, wchar_t* pszItemString, cglsPopupMenu* pChildMenu=0);

	// -----------------------------------------------------
	void Hide();

	void SetParentMenu(cglsPopupMenu*);

	// -----------------------------------------------------
	//virtual int OnCreate(WPARAM wParam, LPARAM lParam);

	// assitant interface
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
	virtual Bool SetDefaultValue(wchar_t* buffer);

	// --------------------------------
	virtual void Enable(Bool bIsEnable=True, int id=0);


protected:
	// -----------------------------------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// -----------------------------------------------------
	virtual void DestroyBitmap(sDrawItem* item);

	// ---------------------------------------
	virtual Bitmap* GetChildFgImage(int childId, int nImageIndex, RECT* rect);

	// ---------------------------------------
	virtual LRESULT OnMouseLButtonDown(WPARAM wp, LPARAM lp, BOOL& handled);

	// ---------------------------------------
	virtual LRESULT OnMouseMove(WPARAM wp, LPARAM lp, BOOL& handled);

	// ------------------------------------
	virtual LRESULT OnNotify(WPARAM wp, LPARAM lp, Bool& bIsHandled);

	// -----------------------------------------------------
	LRESULT OnShow(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -----------------------------------------------------
	void OnThreadStart(int id, int param1, int param2);


private:
	// ------------------------------------
	sGlsPopupMenuDetail m_detail;

	// ------------------------------------
	cglsPopupMenu* m_parentMenu;
	
	// ------------------------------------
	cglsPopupMenu* m_ShowChild;

	Bitmap* m_bmpItemForeGround;

	// ------------------------------------
	HMENU m_hMenu;

	// ------------------------------------
	HWND m_hMenuWnd;

	Bool m_bIsExit;

	int m_nResultId;

	Bool m_bIsNoNotify;

	int m_systemId;

// ------------------------------------
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnShow, WM_SHOWWINDOW)
DEFINE_MESSAGES_END

};
