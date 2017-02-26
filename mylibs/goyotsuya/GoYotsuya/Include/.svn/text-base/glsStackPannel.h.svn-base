#pragma once

#include "yistd.h"
#include "GlassInterface.h"
#include "glsGaussPannel.h"
#include "yiList.h"



enum eContorlxAlign {
	eCXA_CENTER	= 1,
	eCXA_LEFT	= 2,
	eCXA_RIGHT	= 4,
	eCXA_VCENTER = eCXA_CENTER<<4,
	eCXA_TOP		= eCXA_LEFT<<4,
	eCXA_BOTTOM	= eCXA_RIGHT<<4
};

enum eStackPannelScrollBarState {
	eSPSBS_MOUSE_LEAVE=0,
	eSPSBS_MOUSE_ENTER=1,
	eSPSBS_MOUSE_DOWN=2,
	eSPSBS_DRAG
};

	//Bool m_bIsMouseUpNothing;

	//Bool m_BgSelectedMode;

	//Bool m_bIsBgSelectedLock;

	//Bool m_bIsBufferSizeFixed;

	////Bool m_bIsBufferSizeFixedMode;

	//Bool m_bIsBufferSizeFixedVirtualMode;

	//Bool m_bIsInsertAndCreate;

	//Bool m_bIsScrollBarShow;

	//sGlsStackPannelDetail m_detail;

enum eStackPannelFlag {
	eSPF_FIXED_MODE=1<<1,
	eSPF_VERTICAL=1<<2,
	eSPF_SHOW_SCROLLBAR=1<<3,
	eSPF_USE_GAUSS_PANNEL=1<<4,
	eSPF_INSERT_AND_CREATE=1<<5,
	eSPF_ENABLE_RESIZE=1<<6,
	eSPF_SCROLL_DIRECT=1<<7,
	eSPF_NORMAL_SCROLLMODE=1<<8,
	eSPF_BUFFERSIZE_FIXED=1<<9,
	eSPF_BUFFERSIZE_FIXED_VIRTUAL=1<<10,
	eSPF_MOUSEMOVE_NOTHING=1<<11,
	eSPF_BG_SELECTED_MODE=1<<12,
	eSPF_BG_SELECT_LOCK=1<<13,
	eSPF_RESIZE_IS_MANUAL=1<<14
	
};

// ----------------------------------
typedef struct xsGlsStackPannelDetail {
	sGlsDetail gls;
	sGlsGaussPannelDetail gauss;
	// ここから拡張
	Bool bIsFixedMode;
	Bool bIsVertical;
	Bool bIsShowScrollBar;
	Bool bUseGaussPannel;
	Bool bIsInsertAndCreate;
	Bool bEnableResize;
	Bool bIsScrollDrawDirect;
	Bool bIsNomalScrollMode;
	Bool bIsReSizeManual;

	eStackAlign eAlign;

	int nScrollAnimationInterval;
	int nScrollPageAnimationInterval;
	int nScrollPageSlowAnimationInterval;
	eGlsAnimationPattern eScrollPattern;
	eGlsAnimationPattern eScrollPagePattern;
	eGlsAnimationPattern eScrollPageSlowPattern;
	float fEnterTransparency;
	float fLeaveTransparency;
	float fScrollBarEnterTransparency;
	float fScrollBarLeaveTransparency;

	const wchar_t* pszToolTip;
} sGlsStackPannelDetail;

#define WM_STACKPANNEL_SCROLLED (WM_USER+16)
#define WM_STACKPANNEL_SCROLL_END (WM_USER+17)

#define GLS_INGNORE_SENDER ((cGlassInterface*)0xFFFFFFFF)


#define UPDATE_SKIP_CALCULATE (-1.0)

class xcglsTabCtrl;

class xEXPORT  cglsStackPannel : public cglsGaussPannel
{
public:
	cglsStackPannel(void);
	~cglsStackPannel(void);

	static void xDefaultDetail(sGlsStackPannelDetail* detail);
	static void xDefaultItemDetail(sGlsItemDetail* detail);

	// --------------------------------------------
	Bool CreateFixed(HWND hWnd, int id, RECT* rect, RECT* margin, cglsDrawObject* obj, Bool bIsVertical, Bool bIsNotOwnDraw, eStackAlign align, sGlsStackPannelDetail& m_detail);

	// --------------------------------------------
	Bool CreateDynamicSize(HWND hWnd, int id, POINT* pt, RECT* margin, cglsDrawObject* obj, Bool bIsVertical, Bool bIsNotOwnDraw, eStackAlign align, sGlsStackPannelDetail& m_detail);

	// --------------------------------------------
	Bool Create(HWND hWnd, int id, RECT* rect, RECT* margin, cglsDrawObject* obj, sGlsStackPannelDetail* detail=0);//Bool bIsVertical, Bool bIsNotOwnDraw=False, eStackAlign align=eSA_CENTER);

	// --------------------------------------------
	// システムエリア―はスクロールされない。
	Bool SetSystemArea(RECT* rect, Bool bIsTopFrom);

	// --------------------------------------------
	virtual void Destroy();

	// -----------------------------
	int Add(int id, Bitmap* bmp, const wchar_t* pszText, int distance, const wchar_t* pszToolTip=0, sGlsItemDetail* detail=0);

	// -----------------------------
	int Add(cGlassInterface* pannel, int distance, eStackAlign align = eSA_LEFT);

	// -----------------------------
	int AddFreeItem(int id, int x, int y, Bitmap* bmp, const wchar_t* pszText, const wchar_t* pszToolTip=0, sGlsItemDetail* detail=0);

	// -----------------------------
	int AddFreeGlsItem(int id, int x, int y, cGlassInterface* obj);

	// -----------------------------
	int IndexToId(int index);

	// -----------------------------
	int IdToIndex(int id);




	// -----------------------------
	sDrawItem** GetExportItemz(Bool bIsRemoveFromList=True);

	// -----------------------------
	Bool ImportItemz(sDrawItem** p, int n);

	// -----------------------------
	int Insert(int id, int index, Bitmap* bmp, const wchar_t* pszText, int distance, const wchar_t* pszToolTip=0, sGlsItemDetail* detail=0);

	// -----------------------------
	int Insert(int index, cGlassInterface* obj, int distance, eStackAlign align = eSA_LEFT);

	// -----------------------------
	int InsertLF(int index, int distance);

	// -----------------------------
	int AddLF(int distance);

	// -----------------------------
	int InsertSpace(int index, int width, int height);

	// -----------------------------
	int AppSpace(int width, int height);

	// -----------------------------
	void SetScrollNotifyWindow(HWND hWnd);


	// ------------------------------------
	virtual void GetMargin(RECT& rect);

protected:
	// -----------------------------
	int Insert(int index, sDrawItem* item);

	// -----------------------------
	virtual Bool OnSelfBgDraw(Bitmap* bmp, RECT* rect, RECT* rectChild);

	// -----------------------------
	virtual Bool OnSelfFgDraw(Bitmap* bmp, RECT* rect, RECT* rectChild);

	// -----------------------------
	virtual Bool xOnItemMouseDown(sDrawItem* item);

	// -----------------------------
	virtual Bool xOnItemMouseUp(sDrawItem* item);

	// -----------------------------
	virtual Bool xOnItemMouseMove(sDrawItem* item);

	// -----------------------------
	virtual Bool xOnItemMouseLeave(sDrawItem* item);

	// -----------------------------
	virtual Bool OnItemAnimation(sDrawItem* item, int start, int end);


public:
	// -----------------------------
	int InsertFreeItem(int id, int index, int x, int y, Bitmap* bmp, const wchar_t* pszText, const wchar_t* pszToolTip=0, sGlsItemDetail* detail=0);

	// -----------------------------
	int InsertFreeGlsItem(int id, int index, int x, int y, cGlassInterface* obj);

	// -----------------------------
	void SetItem(int id, int bitmapId, const wchar_t* pszText, Bool bIsUpdate=True);

	// -----------------------------
	int GetId(int index);

	// -----------------------------
	int GetIndex(int id);

	// -----------------------------
	void xDestroyItem(sDrawItem * item, Bool bIsFree=True);

	// -----------------------------
	void Remove(unsigned int id, Bool bIsFree=True);

	// -----------------------------
	Bool ReSize(UINT x, UINT y, Bool bIsRepaint=0);

protected:
	// -----------------------------
	sDrawItem* RemoveFromList(unsigned int id);

	// -----------------------------
	sDrawItem* GetDrawItem(unsigned int id);




public:
	// ------------------------------------
	virtual Bool InvalidateRect(RECT* lpRect, Bool bErase=False, Bool bIsDirect=False, Bool bIsForced=False);

	// -----------------------------
	virtual void Clear();

	// -----------------------------
	virtual void Update(eGlsUpdateMode mode=eGUM_UPDATE, eDrawItemStatus status=eDIS_NORMAL);

	// -----------------------------
	virtual void Update(sDrawItem* item, eDrawItemStatus status=eDIS_NORMAL);

	// -----------------------------
	virtual void Update(Bitmap* bmp, RECT* rect=0, RECT* rect_child=0, int offsetX=0, int offsetY=0,  Bool bIsLocalBufferUpdate=False);//, cGlassInterface* from=GLS_INGNORE_SENDER);

	// ---------------------------------------
	virtual Bool UpdateBg(eDrawItemStatus status=eDIS_NORMAL, Bool bIsHitted=True);

	// ---------------------------------------
	virtual Bool UpdateFg(eDrawItemStatus status=eDIS_NORMAL, Bool bIsHitted=True);

	// ---------------------------------------
	virtual Bool OnItemOwnerDraw(Graphics* g, sDrawItem* item, RECT* clipRect);

	// ------------------------------------
	virtual Bool OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX=0, int offsetY=0);


	//// -----------------------------
	//Bool ItemzLock(Bool bIsWait=True);

	//// -----------------------------
	//Bool ItemzUnlock();


	// -----------------------------
	void EnableItem(int id, Bool bIsEnable);

	// -----------------------------
	int Scroll(int pos, int nMilliSec, int StopPos=0, int nStopPosMilliSec=0);

	// ---------------------------------------
	Bool SetScrollPos(int pos);

	// ---------------------------------------
	Bool SetScrollRange(int min, int max, int pagesize, Bool bIsUpdateForced=False);

	// -----------------------------
	void SetBufferSize(RECT& rect, Bool bIsVertual=False);

	// -----------------------------
	int GetItemCount();

	// -----------------------------
	virtual sDrawItem* GetItem(int id);

	// -----------------------------
	int GetCurrentScrolledPos(float* ratio=0);

	// ------------------------------------
	Bool ShowItem(Bool bIsShow, int id);

	// ------------------------------------
	Bool IsItemVisible(int id);


protected:
	// ---------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps);

	// ---------------------------------------
	virtual LRESULT OnClicked(sDrawItem* item);

	// ---------------------------------------
	LRESULT OnClicked(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual LRESULT OnLButtonRepeated(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnMouseEnter(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnMouseMove(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	virtual LRESULT OnMouseLButtonDown(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	virtual LRESULT OnMouseLButtonUp(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	virtual LRESULT OnMouseLButtonDblClick(WPARAM wp, LPARAM lp, Bool& handled);


	// ---------------------------------------
	LRESULT OnScrolled(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnBgAnimation(WPARAM wp, LPARAM lp, Bool& handled);
	
	// ---------------------------------------
	LRESULT OnFgAnimation(WPARAM wp, LPARAM lp, Bool& handled);

	// ------------------------------------
	virtual LRESULT OnLButtonLongPressed(WPARAM lParam, LPARAM wParam);



	// ---------------------------------------
	virtual void xCalculatePosition(RECT* rectAll, Bool bIsLastItemOnly=False);

	// ------------------------------------
	virtual LRESULT OnNotHandledCommand(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual int GetBgIndex(eDrawItemStatus status, Bool bIsHitted, Bool& bIsSkipAnimation);

	// ---------------------------------------
	virtual int GetFgIndex(eDrawItemStatus status, Bool bIsHitted, Bool& bIsSkipAnimation);

	// --------------------------------
	//virtual void InitDetail();


	int m_itemMoveOrgX;
	int m_itemMoveOrgY;

	// ------------------------------------
	LRESULT OnDragStart(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ------------------------------------
	LRESULT OnDrag(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ------------------------------------
	LRESULT OnDragEnd(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------
	sDrawItem* xFindItem(int x, int y, cyiList* list);

private:
	// -----------------------------
	void UpdateFixedMode(Bitmap* bmp, RECT* rect, RECT* rect_child=0, int offsetX=0, int offsetY=0);

	// ------------------------------------
	Bool xIsItemIsValid(sDrawItem* item);

	// ------------------------------------
	Bool xMoveItem(sDrawItem* item, int x, int y);

	sDrawItem* m_itemDraged;

protected:
	// ------------------------------------
	virtual Bool IsBgAnimation();

	// ------------------------------------
	virtual int GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation, Bitmap** ret=0);

	// -----------------------------
	virtual void GetScrollPos(int&x,int&y);

	// -----------------------------
	LRESULT OnMouseWheel(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------
	LRESULT OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled);


	// ---------------------------------------
	cyiList* m_listItem;

	// ---------------------------------------
	cyiList m_listFreeItem;


	// ---------------------------------------
	Bitmap* m_bg;

	// ---------------------------------------
	RECT m_rectSystem;

protected:

	// ---------------------------------------
	void* xGetItemById(int id);

	// ---------------------------------------
	sDrawItem* xGetItemIn(int x, int y);

	// ---------------------------------------
	void* xGetItemByIndex(int index);

	// ---------------------------------------
	int xGetItemIndex(int id);


	// ---------------------------------------
	void xReDrawObject();



	// ---------------------------------------
	Bool xDrawItem(sDrawItem* item, eDrawItemStatus status=eDIS_NORMAL);

	// ------------------------------------
	virtual LRESULT OnGlsAnimationEx(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);


private:
	// ---------------------------------------
	Bool xCreateBg(int w, int h, Bool bIsForced=False);

protected:

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnMouseMove, WM_MOUSEMOVE)
	MESSAGE(OnMouseLButtonDown, WM_LBUTTONDOWN)
	MESSAGE(OnMouseLButtonUp, WM_LBUTTONUP)
	MESSAGE(OnGlsAnimationEx, WM_GLS_ANIMATIONEX)
	MESSAGE(OnScrolled, WM_STACKPANNEL_SCROLLED)
	MESSAGE(OnBgAnimation, WM_GLS_BG_ANIMT)
	MESSAGE(OnFgAnimation, WM_GLS_FG_ANIMT)
	MESSAGE(OnDragStart, WM_GLS_DRAG_START)
	MESSAGE(OnDrag, WM_GLS_DRAG)
	MESSAGE(OnDragEnd, WM_GLS_DRAG_END)
	MESSAGE(OnMouseWheel, WM_MOUSEWHEEL);
	MESSAGE(OnSizeChanged, WM_SIZE)
	MESSAGE(OnMouseLButtonDblClick, WM_LBUTTONDBLCLK)
DEFINE_MESSAGES_END


	int m_nItemzLength;
	int m_nItemzWidth;
	int m_nRadioSize;

	//sDrawItemEx m_bgItem;

	//Bitmap* m_bmp;
	//Graphics* m_g;

public:
	//Bool m_bIsVertical;


protected:
	unsigned int m_eStackFlag;

	unsigned int m_wMargin;
	unsigned int m_hMargin;
	//BOOL m_bCoordChanged;

	// 32個まで
	RECT m_rectMargin;
	//Bool m_bIsSizeFixed;

	eStackAlign m_eAlign;

	sDrawItem m_fgItem;
	//
	sDrawItem* m_itemMouseDown;

	//
	sDrawItem* m_itemMouseMove;

	//
	sDrawItem* m_itemSelected;

	//
	eDrawItemStatus m_eSelectedStatus;


	unsigned int m_nScrollPos;

	HandleAnit m_hScrollAnit;


	float m_fScrollBarLeaveTransparency;
	float m_fScrollBarEnterTransparency;


	typedef struct {
		int min;
		int max;
		int pos;
		int page;
		// ui
		int m_nScrollBarWidth;
		int m_nScrollBarLength;
		int m_nScrollBarStartPos;
		int m_nScrollBarRange;
	} sScrollInfo;

	sScrollInfo m_scrollInfo;
private:
	HWND m_hScrollNotifyWnd;
	Bool m_bIsRequestCheckItem;
	Bool m_bIsBeforeAdded;




	sDrawItem m_itemScrollBarPrev;
	sDrawItem m_itemScrollBarNext;
	sDrawItem m_itemScrollBar;
	sDrawItem m_itemScrollArea;
	sDrawItem m_itemScrollBarPage;
	int m_nBeforeScrollPos;

	RECT m_rectScrollArea;

	int m_nLastSelectedParts;


	// ---------------------------------------------
	Bool xCreateScrollBar(Bool bIsRedrawOnly=False);

	// ---------------------------------------------
	int xGetScrollParts(int x, int y);

	// ---------------------------------------------
	LRESULT xOnScrollBarDragStart(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------
	LRESULT xOnScrollBarDrag(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------
	LRESULT xOnScrollBarDragEnd(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------
	Bool xSetScrollItemStatus(int nIndex, bit32_t* mask, float factor);

	// ---------------------------------------------
	void xDrawScrollBar(Graphics* g, RECT* clipRect=0, Bool bIsAbsCoord=False);

	// ---------------------------------------------
	Bool xOnLButtonDownScrollBar(int xPos, int yPos);

	// ---------------------------------------------
	LRESULT xOnMouseMoveScrollBar(WPARAM wParam, LPARAM lParam);

	// ---------------------------------------------
	LRESULT xOnMouseLeaveScrollBar(WPARAM wParam, LPARAM lParam);

	// ---------------------------------------------
	void xScrollBarUpdate(int xPos, int yPos, Bool bIsAllRange=False);

	friend class xcglsTabCtrl;
	friend class xcglsTabItem;
	eStackPannelScrollBarState m_eScrollBarState;
	int m_nSelectedScrollBarParts;

	float m_fScrollLastFactor;

};
