#pragma once

#include "yistd.h"
#include "glsStackPannel.h"
#include "glsDrawObject.h"

#define WM_GLS_TAB (WM_GLASSINTERFACE+16)
#define WM_TAB_SEL_CHANGED WM_GLS_TAB
#define WM_TAB_ITEM_REMOVED (WM_GLS_TAB+1)


#define TCN_REMOVED (TCN_SELCHANGE+16)
#define TCN_REMOVE_CONFIRM (TCN_SELCHANGE+17)

// ----------------------------------------------
typedef struct xsGlsTabDetail {
	//unsigned short wMinLength;
	//unsigned short wMaxLength;
	sGlsStackPannelDetail stack;
	unsigned short wMaxShowCount;
	unsigned short wMinShowCount;
	unsigned short wMargin;
	unsigned short hMargin;
	Bool bHasCloseButton;
	Font* font;
	Brush* brush;
	Pen* pUnderPen;
	Pen* pTopPen;
} sGlsTabDetail;

typedef struct xsTabItemDetail {

}sTabItemDetail;

// ----------------------------------------------
typedef struct xsGlsTabItem {
	Bool   bIsVertical;
	Bool	bHasChangedMark;
	float  radius; // default 3
	color_t NormalTopColor;
	color_t NormalTopColorMask; // 0x1FFFFFFF
	color_t NormalBaseColor; // 0x7FFFFFFF
	color_t DownTopColor;
	color_t DownTopColorMask; // 0x7FFFFFFF
	color_t DownBaseColor; // 0x7FFFFFFF
	color_t MoveTopColor;
	color_t MoveTopColorMask; // 0x1FFFFFFF
	color_t MoveBaseColor; // Color(200, 255, 255, 255)
	color_t HiLightColor;
	Bool bIsNoLock;

	//Brush* NormalBgBrush;
	//Brush* MoveBgBrush;
	//Brush* SelectedBgBrush;
	//Pen* NormalPen;
	//Pen* MovePen;
	//Pen* SelectedPen;
	//Bool bHasClosedButton;
} sGlsTabItem;

// ----------------------------------------------
class xcglsTabCtrl : public cglsStackPannel, cglsDrawObject
{
public:
	xcglsTabCtrl(void);
	virtual ~xcglsTabCtrl(void);

	// ----------------------------------------------
	static void xDefaultDetail(sGlsTabDetail* detail);

	// ----------------------------------------------
	static void xDefaultItemDetail(sGlsTabItem* detail, color_t color=Color::YellowGreen);


	// ----------------------------------------------
	Bool Create(HWND hWndParent, int id, int x, int y, int w, sGlsTabDetail* detail=0);

	// ----------------------------------------------
	int AddItem(int id, Bitmap* icon, const wchar_t* pszTab, Bool bHasClosedButton=False, sGlsTabItem* item=0);

	// ----------------------------------------------
	int InsertItem(int index, int id, Bitmap* icon, const wchar_t* pszTab, Bool bHasClosedButton=False, sGlsTabItem* item=0);

	// ----------------------------------------------
	int DeleteItem(int index, int id=-1, int FocusId=-1, Bool bUpdateAfter=True);

	// ----------------------------------------------
	int SetItem(int id, int bitmapId, const wchar_t* pszText, Bool bIsUpdate=1);

	// ----------------------------------------------
	int ScrollItem(int currFormIndex);

	// ----------------------------------------------
	void SetCurSel(int index, int id=-1, Bool bIsNotify=True, Bool bUpdate=True);

	// ----------------------------------------------
	int GetCurSel();

	// ----------------------------------------------
	int GetMaxItemLength();

	// ----------------------------------------------
	int GetMinItemLength();

	// ----------------------------------------------
	int GetItemLength();

	// ----------------------------------------------
	Bool IsItemVisible(int id);

	// ----------------------------------------------
	Bool SetItemChanged(int id, Bool bIsTrue);

	// ----------------------------------------------
	Bool SetItemShowButton(int id, Bool bIsTrue);

	// ----------------------------------------------
	void GetVisibleLastItemPos(int& x, int& y);

	// ----------------------------------------------
	void GetVisibleFirstItemPos(int& x, int& y);


protected:

	// ----------------------------------------------
	Bool xUpdateItemzLength(Bool bIsRedraw=False);

	// ----------------------------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled);

	// ------------------------------------
	virtual LRESULT OnToolTipTimer(WPARAM wp, LPARAM lp);

	//// -------------------------
	//virtual LRESULT cglsStackPannel::OnMouseLeave(WPARAM wp, LPARAM lp);

	// -----------------------------
	void OnUpdateBufferBefore(Graphics*g, Bitmap* bmpBuffer, int x, int y, int w, int h);

	// ---------------------------------------
	int GetBgIndex(eDrawItemStatus status, Bool bIsHitted, Bool& bIsSkipAnimation);

	// ---------------------------------------
	int xGetItemzHeight();

//DEFINE_COMMANDS_BEGIN
//	COMMAND(OnItemClicked, 
//DEFINE_COMMANDS_END


private:
	// ----------------------------------------------
	sGlsTabDetail m_detail;

	// ----------------------------------------------
	NMHDR m_hdr;

	friend class xcglsTabItem;
	friend class cglsTabCtrl;

	Bool m_bIsNoLock;
};
#include "glsToolButton.h"

// ----------------------------------------------
class xEXPORT cglsTabCtrl : public cglsStackPannel, public cglsDrawObject
{
public:
	cglsTabCtrl(void);
	virtual ~cglsTabCtrl(void);

	// ----------------------------------------------
	static void xDefaultDetail(sGlsTabDetail* detail);

	// ----------------------------------------------
	static void xDefaultItemDetail(sGlsTabItem* detail, color_t color=Color::YellowGreen);


	// ----------------------------------------------
	Bool Create(HWND hWndParent, int id, int x, int y, int w, sGlsTabDetail* detail=0);

	// ----------------------------------------------
	int AddItem(int id, Bitmap* icon, const wchar_t* pszTab, Bool bHasClosedButton=False, sGlsTabItem* item=0);

	// ----------------------------------------------
	int InsertItem(int index, int id, Bitmap* icon, const wchar_t* pszTab, Bool bHasClosedButton=False, sGlsTabItem* item=0);

	// ----------------------------------------------
	int SetItem(int id, int bitmapId, const wchar_t* pszText, Bool bIsUpdate=1);

	// ----------------------------------------------
	int DeleteItem(int index, int id=-1, int FocusId=-1, Bool bUpdateAfter=True);

	// ----------------------------------------------
	int GetItemCount();

	// -----------------------------
	int GetId(int index);

	// ----------------------------------------------
	void SetCurSel(int index, int id=-1, Bool bIsNotify=True, Bool bUpdate=True);

	// --------------------------------
	void SetNotifyWindow(HWND hWnd);

	// ----------------------------------------------
	int GetCurSel();

	// ----------------------------------------------
	int GetIndex(int id);

	// ----------------------------------------------
	Bool ReSize(int length, Font* font=0);

	// ----------------------------------------------
	int GetMeasureHeight();

	// ----------------------------------------------
	Bool ShowItem(int id, Bool bIsShow);

	// -----------------------------
	virtual void Update(eGlsUpdateMode mode=eGUM_UPDATE, eDrawItemStatus status=eDIS_NORMAL);

	// -----------------------------
	Bool IsItemVisible(int id);

	// ----------------------------------------------
	Bool SetItemChanged(int id, Bool bIsTrue);

	// ----------------------------------------------
	Bool SetItemShowButton(int id, Bool bIsTrue);


protected:

	// ----------------------------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled);

	// ------------------------------------
	virtual LRESULT OnLButtonLongPressed(WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnLButtonRepeated(WPARAM wParam, LPARAM lParam);

private:
	xcglsTabCtrl* m_tab;
	int m_nLength;

	sDrawItem* m_itemArrowNext;
	sDrawItem* m_itemArrowPrev;
	cglsToolButton* m_btnScroll;


	// ------------------------------------
	Bool xShowScrollButton(Bool bContainSize=False, Bool bForced=False);

	// ------------------------------------
	Bool xHideScrollButton(Bool bContainSize=False, Bool bForced=False);

	// ------------------------------------
	LRESULT OnScrollPrev(HWND hWnd, int code);

	// ------------------------------------
	LRESULT OnScrollNext(HWND hWnd, int code);

	// ------------------------------------
	void OnNotifyDeleted();

	friend class xcglsTabCtrl;


};
