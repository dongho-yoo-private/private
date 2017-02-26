#pragma once

#include "yctrlbase.h"
#include "glsDrawObject.h"
#include "yiAnimationTrigger.h"
#include "glsSystem.h"

#include "typeinfo"

using namespace std;

#define WM_GLASSINTERFACE	(WM_USER+64)
#define WM_GLS_ANIMATION	WM_GLASSINTERFACE
#define WM_GLS_ANIMATIONEX	(WM_USER+69)
#define WM_GLS_PAINT (WM_GLASSINTERFACE+65)
#define WM_ANIMATION_SHOW (WM_GLS_PAINT+1) // 66

// for stack pannel
#define WM_GLS_BG_ANIMT (WM_USER+67) 
#define WM_GLS_FG_ANIMT (WM_USER+68) 

	//Bool m_bIsNoAnimation;
	//Bool m_bIsNotOwnDraw;
	//Bool m_bNoDraw;
	//Bool m_bIsDefaultDraw;
	//Bool m_bIsTransparentWindow;
	//Bool m_bIsDrawBuffer;

// flagz....
// 31: m_bIsNoAnimation
// 30: m_bIsNotOwnDraw;
// 29: m_bNoDraw;
// 28: m_bIsDefaultDraw
// 27: m_bIsTransparentWindow
// 26: m_bIsDrawBuffer
// 25: m_bIsWindowControl

enum eGlsFlagz {
	eGF_NO_ANIMATION=(1<<31),
	eGF_NOT_OWNDRAW=(1<<30),
	eGF_TRANSPARENT=(1<<29),
	eGF_WINDOW_CONTROL=(1<<28),
	eGF_OWN_BGDRAW=(1<<27),
	eGF_BG_CHANGED=(1<<26),
	eGF_UPDATE_MYSELF=(1<<25),
	eGF_REQ_UPDATE_BUFFER=(1<<24),
	eGF_ABS_COORDINATE=(1<<23),
	eGF_REQUEST_UPDATE=(1<<22),
	eGF_POPUP=(1<<21),
	eGF_NO_MOUSEMOVE=(1<<20),
	eGF_NO_INPUT=(1<<19),
	eGF_DBL_CLICK=(1<<18),
	eGF_FONT_FROM_PARENT=(1<<17),
	eGF_MOUSEDOWN_COMMAND=(1<<16),
	eGF_TRANSPARENT_WINDOW=(1<<15),
	eGF_NOCOMMAND=(1<<14),
	eGF_UPDATE_INDEPENDENT=(1<<13),
	eGF_FOCUS_STOP=(1<<12),
	eGF_FOCUS_MANAGE=(1<<11),
	eGF_HILITE=(1<<10),
	eGF_ANIMATION_SHOW=(1<<9),
	eGF_PARENT_CENTER=(1<<8),
	eGF_DISABLE_DISPLAY_ONLY=(1<<7),
	eGF_BUFFERED_PAINT=(1<<6),
	eGF_DRAW_MYWAY=(1<<5)
};

#define xON_GLS_FLAG(flag, value) (flag|=(bit32_t)value)
#define xOFF_GLS_FLAG(flag, value) (flag&=~((bit32_t)value))
#define xIS_GLS_FLAG(flag, value) ((flag&(bit32_t)value)!=0)
#define xSET_GLS_FLAG(flag, value, b) (flag=b?(flag|(bit32_t)value):(flag&(~((bit32_t)value)))) 


#define GLS_FLAG_NOANIMATION 

enum eGlassInterfaceStyle {
	eGIS_BUTTON,
	eGIS_STATIC
};

enum eGlsUpdateMode {
	eGUM_UPDATE,
	eGUM_FORCED,
	eGUM_SKIP_REDRAW,
	eGUM_UPDATE_BGCHANGED,
	eGUM_UPDATE_LASTITEM,
	eGUM_BG_FORCED

};


typedef struct xsForOwnerDraw {
	int x;
	int y;
	int w;
	int h;
	int org_x;
	int org_y;
} sForOwnerDraw;

class cglsPopupMenu;

enum eGlsGuiStyle {
	eGGS_DEFAULT=0,
	eGGS_GLASS,
	eGGS_GAUSS,
	eGGS_NEON,
	eGGS_NEON_WHITE
};

typedef struct xsGlsDetail {
	Font* font;
	Bool bIsNotTransparent;
	Bool bIsNoAnimation;
	Bool bIsNotOwnDraw;
	Bool bIsWindowControl;
	Bool bIsNoMouseMove;
	Bool bIsNoInput;
	Bool bIsNoSendCommand;
	int nAnimationMilliSec;
	int nMouseMoveAnimationInterval;
	eGlsAnimationPattern eMouseMovePattern;
	int nMouseLeaveAnimationInterval;
	eGlsAnimationPattern eMouseLeavePattern;
	eGlsAnimationPattern eShowPattern;
	int nShowAnimationInterval;
	Bool bIsTransparentWindow;
	float fEnterTransparency;
	float fLeaveTransparency;
	int nTooltipTimer;
	int nDragOnTimer;
	int nKeyLButtonDownTimer;
	int nKeyRepeatInterval;
	Bool bRecieveDblClick;
	Bool bMouseDownCommand;
	Bool bUpdateIndependent;
	Bool bFocusStop;
	Bool bIsFocusManageInterface;
	Bool bIsAnimationShow;
	Bool bDisableDisplayOnly;
	unsigned int nShowAnimationTime;
	unsigned int nHideAnimationTime;
	cglsPopupMenu* menu;

	Bool bDrawMyway;
	eGlsGuiStyle style;
}sGlsDetail;

enum eZOrder {
	eZO_TOP=0xFFFFFFFF,
	eZO_BOTTOM=0
};

// --------------------------------
enum eGlsObjectPosition {
	eGOP_LEFT,
	eGOP_RIGHT,
	eGOP_CENTER,
	eGOP_TOP=(eGOP_LEFT<<8),
	eGOP_BOTTOM=(eGOP_RIGHT<<8),
	eGOP_VCENTER=(eGOP_CENTER<<8),
	eGOP_VLINE=(1<<16)

};

// --------------------------------
class xEXPORT cglsPosition 
{
public:
	cglsPosition(){};//int id=(eGOP_LEFT|eGOP_RIGHT), int X=0, int Y=0);
	~cglsPosition(){};

	int id;
	int X;
	int Y;
};

class xEXPORT cGlsAssistant
{
public:
	cGlsAssistant(){m_bIsChanged=False;}
	virtual Bool ShowAssistant(HWND hWnd, int x, int y)=0;

	virtual Bool GetResult(wchar_t* buffer)=0;

	virtual Bool GetResult(unsigned int& result1, unsigned int& result2)=0;

	virtual Bool GetDefaultValue(wchar_t* buffer)=0;

	virtual Bool GetDefaultValue(unsigned int& result1, unsigned int& result2)=0;

	virtual Bool SetDefaultValue(wchar_t* str)=0;

	virtual Bool IsChanged(){return m_bIsChanged;}
protected:
	Bool m_bIsChanged;
};

//#include "yiStaticList.h"


enum eGlsPosition {
	eGP_RIGHTSIDE=0x01,
	eGP_LEFTSIDE=0x02,
	eGP_TOPSIDE=0x10,
	eGP_BOTTOMSIDE=0x20,
	eGP_RIGHT=0x04,
	eGP_LEFT=0x08,
	eGP_CENTER=0x0100,
	eGP_TOP=0x40,
	eGP_BOTTOM=0x80,
	eGP_VCENTER=0x0200
};

enum eGlsUpdateReason {
	eGUR_FORCED=0,
	eGUR_BG_CHANGED=1,
	eGUR_NO_REASON
};


// --------------------------------
enum eFontType {
	eFT_SELF, 
	eFT_PARENT
};

typedef struct  {
	int pos;
	eGlsPosition align;
	cGlassInterface* from;
} sGlsItemPosition;


#define CreateNullWindow Create
#ifdef _DEBUG
	#define __GLASSINTERFACE_NAME__ strcpy(m_szClassName, typeid(*this).name())
#else
	#define __GLASSINTERFACE_NAME__ 
#endif



class cglsStackPannel;
class cglsPopupMenu;
class cglsPopup;
class cglsStatic;

class xEXPORT cGlassInterface : public cyctrlbase
{
public:
	cGlassInterface(void);
	virtual ~cGlassInterface(void);

	// ---------------------------------------
	static cGlassInterface* xNEW();

	// ---------------------------------------
	static void xDELETE(cGlassInterface* obj);

	// ---------------------------------------
	static int xGetPos(eItemAlign align, int wAll, int margin, int w);

	// --------------------------------
	//static void CreateGlassBitmap(sDrawItem* item, Bitmap* bmp, Bitmap* str, eGlassInterfaceStyle style);

	// --------------------------------
	static cglsResourceObject* GetDefaultResource();

	// ---------------------------------------
	//static Bitmap* GetCurrentBmp(sDrawItem* item=0);

	// ---------------------------------------
	static Bitmap* xCreateItemBitmap(sDrawItem* item, Font* font);

	// ---------------------------------------
	static Bitmap* xCreateBitmap(Bitmap* bmp, wchar_t* txt, Font* font, int nLength=0, color_t color=Color::Black);

	// ---------------------------------------
	static int CalculateFontHeight(Font* font, Bool bIsGDI);

	// ---------------------------------------
	static Bitmap* xCreateBitmapEx(Bitmap* bg, Bitmap* bmp, wchar_t* txt, Font* font, color_t colorFont);

	// ---------------------------------------
	static Bool xMesureBitmap(Bitmap* bmp, wchar_t* txt, Font* font, RECT* rect, int nFixedWidth=0);

	// --------------------------------
	//Bool CreateMainWindow(HINSTANCE hInstance, int x, int y, int w, int h, Brush* bg);

	// --------------------------------
	static void xDefaultDetail(sGlsDetail* detail);

	// --------------------------------
	void SetParent(HWND hWndParent);

	// --------------------------------
	void SetParent(cGlassInterface* parent);

	// --------------------------------
	Bool Create(HWND hWndParent, HINSTANCE hInstance, int id, RECT* lpRect, sGlsDetail* detail=0);

	// -------------------------------------
	Bool Create(HWND hWndParent, int id, int x, int y, int w, int h, DWORD styleEx=0, DWORD style=0, sGlsDetail* detail=0);

	// -------------------------------------
	Bool CreateToolTip(const wchar_t* string, int nToolTipTimer, void* detail);

	// -------------------------------------
	Bool SetHiLight(Bool bIsHilight);

	// -------------------------------------
	Bool IsCreated();

	// -------------------------------------
	Bool SetItemPos(int x, int y, eGlsItemAlign align);

	// -------------------------------------
	//int GetValidChildId();

	// --------------------------------
	void SetNotifyWindow(HWND hWnd);

	// --------------------------------
	virtual void Destroy();

	// --------------------------------
	virtual void Hide();

	// --------------------------------
	virtual void Show();

	// --------------------------------
	virtual void Enable(Bool bIsEnable=True, int id=0);

	// --------------------------------
	virtual point_t GetMeasureSize(Font* font);

	// ------------------------------------
	virtual Bool ReSize(UINT x, UINT y, Bool bIsRepaint=FALSE);

	// ------------------------------------
	virtual void GetMargin(RECT& rect);

	// ------------------------------------
	Bool ReSizeNoActivate(UINT x, UINT y);

	// ------------------------------------
	void AddPositionLink(cGlassInterface* obj, int/*eGlsPosition*/ ePosition, int xDistance=0, int yDistance=0);

	// --------------------------------
	void SetAnimation(int nAnimationMilisec, int nAnimationInterval, eGlsAnimationPattern eAnimationPartern);

	// --------------------------------
	Bool ImEnabled();

	// --------------------------------
	float GetMyTransparency();

	// --------------------------------
	void SetTransparency(float f);

	//// --------------------------------
	//Bool CreateBackground(cglsDrawItem* item, );

	// ------------------------------------
#ifdef _DEBUG
	virtual int RegisterChild(cGlassInterface* child);
#else
	int RegisterChild(cGlassInterface* child);
#endif

	// ------------------------------------
	void UnRegisterChild(cGlassInterface* child);

	// ------------------------------------
	virtual int SetFont(Font* font, Bool bIsReference=True);

	// -----------------------------
	virtual sDrawItem* GetItem(int id);

	// -----------------------------
	void SetItem(int bitmapId, const wchar_t* pszText, Bool bIsUpdate=True);

	// ------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps=0);

	
	// ------------------------------------
	virtual Bool OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX=0, int offsetY=0);

	// -----------------------------
	virtual void Update(eGlsUpdateMode mode=eGUM_UPDATE, eDrawItemStatus status=eDIS_NORMAL);

	// -----------------------------
	virtual void Update(Bitmap* bmp, RECT* rect=0, RECT* _childRect=0, int offsetX=0, int offsetY=0, Bool bIsLocalBufferUpdate=False);

	// ------------------------------------
	virtual Bool Move(UINT x, UINT y, Bool bIsActivate=False);

	// ------------------------------------
	Bool Move(UINT x, UINT y, eGlsItemAlign eAlign);

	// ------------------------------------
	Bool MoveByPosition(sGlsItemPosition* x, sGlsItemPosition* y);

	// ------------------------------------
	Bool MoveByPosition(sGlsItemPosition& x, sGlsItemPosition& y);

	// -----------------------------
	cGlassInterface* GetDrawWindow(HWND& hWnd);

	// -----------------------------
	Bool GetAbsoluteCoordination(int&x, int&y);

	// -----------------------------
	int ZOrder(int n);

	// -----------------------------
	cGlassInterface* GetChild(int id);

	// -----------------------------
	Bool GetMeasureSize(int&w, int&h);

	// -----------------------------
	cglsSystem* GetSystem();

	// -----------------------------
	void RegisterSystem();

	// -----------------------------
	virtual Bool GetEffectedBg(Bitmap* dest, int x, int y, int sx, int sy, int w, int h);

	// -----------------------------
	void SetGuiStyle(eGlsGuiStyle style, Bool bSetIamOnly=False);

	// -----------------------------
	eGlsGuiStyle GetGuiStyle();

	// -----------------------------
	int GetChildCount();

protected:

	// ------------------------------------
	virtual Bitmap* GetCurrentBitmap(sDrawItem* item=0);

	// ------------------------------------
	virtual Bool OnUpdateAfter(HDC hdc);

	// ---------------------------------------
	BOOL xIsInRect(RECT* rect1, RECT* rect2, RECT& ret);

	// ---------------------------------------
	BOOL xIsInRect(yiRect* rect, int x, int y, int posX=0, int posY=0);

	// ---------------------------------------
	Bool DirectDraw(Bitmap* bmp, RECT* rect=0);

	// ---------------------------------------
	virtual Bool OnDirectDrawBefore(Graphics* g, RECT* rect=0);

	// ---------------------------------------
	virtual Bool OnDirectDrawAfter(Graphics* g, RECT* rect=0);

	// ---------------------------------------
	virtual Bitmap* GetChildFgImage(int childId, int nImageIndex, RECT* rect);


	// ---------------------------------------
	virtual Bitmap* GetParentBg(Bitmap* bmp=0, RECT* rect=0, cGlassInterface* sender=0);

	// -----------------------------
	virtual Bitmap* GetBgBitmap();

	// -----------------------------
	virtual void OnFontChanged();

	// -----------------------------
	int GetDrawOrder();

	// -----------------------------
	virtual void OnShowAnimationEnd(Bool bIsShow, Bool bIsCanceled);


public:
	// -----------------------------
	int GetFontHeight(Bool bIsGDI=FALSE);

	// -----------------------------
	int GetBlankId();
protected:
	// -----------------------------
	Bool IsBgChanged();


	// -----------------------------
	//Bool AddPoint(int id);

	// -----------------------------
	//virtual Bool SetItemPositionLine(cGlassInterface* obj, int xid, int yid);

	// -----------------------------
	//virtual int OnSetPoint(int id);

	// -----------------------------
	//void ReArrangeItem();

public:
	// -----------------------------
	void SetBgBitmap(Bitmap* bmp);

	// -----------------------------
	virtual Bool GetMeasureRect(RECT* rect);

	// -----------------------------
	void SetUserParam(void* param);

	// -----------------------------
	void* GetUserParam();

	// -----------------------------
	void RegisterFocusStopObject(cGlassInterface* obj);


protected:
	// ------------------------------------
	LRESULT OnDragStart(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ------------------------------------
	LRESULT OnDrag(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ------------------------------------
	LRESULT OnDragEnd(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ------------------------------------
	virtual LRESULT OnToolTipTimer(WPARAM wParam, LPARAM lParam);

	int m_nAbsX;

	int m_nAbsY;

	int m_nFontHeight;

	int m_nFontHeightGDI;

	wchar_t* m_pszToolTip;

	// ------------------------------------
	sGlsItemPosition* m_posX;

	// ------------------------------------
	sGlsItemPosition* m_posY;

	unsigned int m_nShowAnimationTime;
	unsigned int m_nHideAnimationTime;

	eGlsUpdateReason m_eUpdateReason;

	// ------------------------------------
	cglsDrawObject* m_DrawObject;
	Bitmap* m_bmpCurrent;

	Bitmap* m_bmpBg;
	// ------------------------------------
	//Bool m_bIsNoAnimation;
	//Bool m_bIsNotOwnDraw;
	////Bool m_bNoDraw;
	////Bool m_bIsDefaultDraw;
	//Bool m_bIsTransparentWindow;
	//Bool m_bIsDrawBuffer;

	bit32_t m_flag;

	cGlassInterface* m_glsDrawInterface;



	// ------------------------------------
	Font* m_font;

	cglsResourceObject* m_resource;
	// ------------------------------------
	sDrawItem m_item;

	// ------------------------------------
	cyiList* m_children;

	// ------------------------------------
	cyiList* m_focusChildren;

	// ------------------------------------
	cyiList* m_childwindow;

	// ------------------------------------
	cyiList* m_listpos;

	// ------------------------------------
	cGlassInterface* m_parent;

	// ------------------------------------
	//eGlsAnimationPattern m_eAnimationPattern;

	// ------------------------------------
	//int m_nRequestUpdateCount;

	int m_nRequestUpdateOrder;

	// ------------------------------------
	//cyiStaticList<unsigned long>* m_listPoint;



	// -----------------------------
	virtual int GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation);

	// -----------------------------
	virtual void GetScrollPos(int&x,int&y);

	Handle m_hPaintMutex;

public:
	// ------------------------------------
	Font* GetFont();	

	// ------------------------------------
	cglsResourceObject* GetResource();

	// ------------------------------------
	//void SetAnimationPattern(sGlsAnimationPattern* pattern);

	//// ------------------------------------
	//void SetBgAnimationPattern(sGlsAnimationPattern* pattern);

	// ------------------------------------
	virtual Bool InvalidateRect(RECT* lpRect, Bool bIsBg=False, Bool bIsDirect=False, Bool bIsForced=False);

	// ------------------------------------
	Bool AnimationShow(Bool bIsShow, sGlsAnimationEx* animation, int term, int msec);

	// ------------------------------------
	Bool AnimationShow(Bool bIsShow, int msec);

	// ------------------------------------
	Bool StartAnimation(sDrawItem* item, sGlsAnimationEx* animation, int term, int msec);

	// ------------------------------------
	void StopAnimation(sDrawItem* item, Bool bClear);



	// ------------------------------------
	Bool MutexLock(Bool bIsWait=True);

	// ------------------------------------
	void MutexUnlock();

	// ------------------------------------
	Bool GetChangedDrawArea(RECT* rect);

	// ------------------------------------
	Bool SetChangedDrawArea(RECT* rect);

	// --------------------------------------------
	Bool NextFocus();

	// --------------------------------------------
	Bool IsDblClicked();

	// --------------------------------------------
	Bool PrevFocus();


	cyiQueue m_listChangeDrawArea;
protected:
	// ------------------------------------
	void InitResource();

	// ------------------------------------
	Bool SetItemStatus(sDrawItem* item, eDrawItemStatus status);

	// ------------------------------------
	void CrossDraw(sDrawItem* item, Bitmap* bmpPrev, Bitmap* bmpNext);

	// ------------------------------------
	void OnThreadStart(int id, int param1, int param2);

	// ------------------------------------
	//virtual LRESULT OnGlsAnimation(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	virtual LRESULT OnGlsAnimationEx(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	//LRESULT OnAnimationShow(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------
	LRESULT OnClicked(sDrawItem* item);

	// ------------------------------------
	virtual void OnAnimationEnd(sGlsAnimationEx* animation);

	// ------------------------------------
	virtual void OnReqDestroy();

	// ---------------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnMouseEnter(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnClicked(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnMouseLButtonDown(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	virtual LRESULT OnMouseRButtonDown(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	virtual LRESULT OnMouseLButtonUp(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	virtual LRESULT OnMouseMove(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	virtual LRESULT OnMouseLButtonDblClick(WPARAM wp, LPARAM lp, Bool& handled);

	// ------------------------------------
	LRESULT OnChar(WPARAM wParam, LPARAM lParam, Bool& handed);

	// ------------------------------------
	LRESULT OnImeChar(WPARAM wParam, LPARAM lParam, Bool& handed);

	// ------------------------------------
	LRESULT OnSysKeyDown(WPARAM wParam, LPARAM lParam, Bool& handed);

	// ------------------------------------
	LRESULT OnSysKeyUp(WPARAM wParam, LPARAM lParam, Bool& handed);

	// ------------------------------------
	LRESULT OnStartComposition(WPARAM wParam, LPARAM lParam, Bool& handed);

	// ------------------------------------
	LRESULT OnComposition(WPARAM wParam, LPARAM lParam, Bool& handed);

	// ------------------------------------
	LRESULT OnEndComposition(WPARAM wParam, LPARAM lParam, Bool& handed);

	// ------------------------------------
	LRESULT OnGlsSystemCommand(WPARAM wParam, LPARAM lParam, Bool& handed);

	// --------------------------------
	virtual void InitDetail(sGlsDetail& detail);

	// ------------------------------------
	LRESULT OnPaintEx(WPARAM wp, LPARAM lp, Bool& handled);

	// ------------------------------------
	Bool GetUnderGroundBitmap(RECT* rect, cGlassInterface* sender=0);

	// ------------------------------------
	virtual Bool IsBgAnimation();

	// ------------------------------------
	HWND m_hNotifyWnd;

	// ------------------------------------
	cyiQueue* m_pItemRects;

	// ------------------------------------
	cyiList m_NowAnimationList;

	//// ------------------------------------
	//float m_fLeaveTransparency;

	//// ------------------------------------
	//float m_fEnterTransparency;

	//// ------------------------------------
	//float m_fsizeRate;

	// ------------------------------------
	int m_nAnimationMilliSec;

	// ------------------------------------
	int m_nMouseMoveAnimationInterval;

	// ------------------------------------
	int m_nShowAnimationInterval;

	// ------------------------------------
	int m_nMouseLeaveAnimationInterval;//=m_detail.nMouseLeaveAnimationInterval;



	// ------------------------------------
	eGlsAnimationPattern m_eMouseLeavePattern;

	// ------------------------------------
	eGlsAnimationPattern m_eMouseMovePattern;

	// ------------------------------------
	eGlsAnimationPattern m_eShowPattern;

	// ------------------------------------
	int m_nDragOnTimer;

	// ------------------------------------
	int m_nKeyLButtonDownTimer;

	// ------------------------------------
	int m_nKeyRepeatInterval;

	// ------------------------------------
	int m_nTooltipTimer;

	// ------------------------------------
	cglsSystem* m_system;

	// ------------------------------------
	HandleAnit m_hAnitShow;

	// ------------------------------------
	//HandleAnit m_hAnimation;

	// --------------------------------
	Bool m_bIsRequestDestroy;

	// --------------------------------
	cglsPopupMenu* m_menu;

	// --------------------------------
	eGlsGuiStyle m_guiStyle;

// ------------------------------------
DEFINE_MESSAGES_BEGIN
	// îpé~
	//MESSAGE(OnGlsAnimation, WM_GLS_ANIMATION)
	MESSAGE(OnGlsAnimationEx, WM_GLS_ANIMATIONEX)
	//MESSAGE(OnAnimationShow, WM_ANIMATION_SHOW)
	MESSAGE(OnMouseLButtonDown, WM_LBUTTONDOWN)
	MESSAGE(OnMouseRButtonDown, WM_RBUTTONDOWN)
	MESSAGE(OnMouseLButtonUp, WM_LBUTTONUP)
	MESSAGE(OnMouseMove, WM_MOUSEMOVE)
	MESSAGE(OnPaintEx, WM_GLS_PAINT)
	MESSAGE(OnDragStart, WM_GLS_DRAG_START)
	MESSAGE(OnDragEnd, WM_GLS_DRAG_END)
	MESSAGE(OnDrag, WM_GLS_DRAG)
	MESSAGE(OnImeChar, WM_IME_CHAR)
	MESSAGE(OnChar, WM_CHAR)
	MESSAGE(OnSysKeyDown, WM_KEYDOWN)
	MESSAGE(OnSysKeyUp, WM_KEYUP)
	MESSAGE(OnStartComposition, WM_IME_STARTCOMPOSITION)
	MESSAGE(OnComposition, WM_IME_COMPOSITION)
	MESSAGE(OnEndComposition, WM_IME_ENDCOMPOSITION)
	MESSAGE(OnGlsSystemCommand, WM_GLS_SYSTEM_COMMAND)
	MESSAGE(OnMouseLButtonDblClick, WM_LBUTTONDBLCLK)
	//MESSAGE(OnKillFocus, WM_KILLFOCUS)
	//MESSAGE(OnKillFocus, WM_SETFOCUS)
DEFINE_MESSAGES_END

private:
	// --------------------------------
	//static void xCreateGlassBitmapForBtn(sDrawItem* item, Bitmap* bmp, Bitmap* bmpStr);

	// ------------------------------------
	void xDeleteItemFromList(sDrawItem* item);

	// ------------------------------------
	Bool xGetDrawRect(RECT* result);

	// --------------------------------
	HandleMutexx m_hMutex;

	// --------------------------------
	Bool DrawImage(Bitmap* dest, Bitmap* src, int destX, int destY, int srcX, int srcY,	int srcW, int srcH);


	// --------------------------------
	friend class cglsSlide;

	// --------------------------------
	friend class cglsControl;

	// --------------------------------
	friend class xglsRadioButton;

	// --------------------------------
	friend class cglsTextPannel;

	// --------------------------------
	friend class cglsStatic;

	// --------------------------------
	friend class cglsEditBox;

	// --------------------------------
	friend class cglsPopupMenu;

	// --------------------------------
	friend class cglsStackPannel;

	// --------------------------------
	friend class cglsDialog;

	// --------------------------------
	friend class cglsGaussPannel;

	// --------------------------------
	friend class cglsGoban;

	// --------------------------------
	friend class cglsPopup;

	friend class xcglsTabItem;

	friend class cglsToolContainer;

	friend class cglsImageBox;

	//sGlsDetail m_detail;

	//float m_fTransparent;

	eFontType m_eFontType;

	//cglsPopup* m_tooltip;



	friend void OnPaintExBg(void* p);

protected:
#ifdef _DEBUG
public:
	char m_szClassName[64];
protected:
#endif

	void* m_pUserParam;
	Bool m_bIsDblClicked;
};

#include "yit.h"

typedef cyitList<Bitmap> cyiBitmapList;
typedef cyitrb<Bitmap> cyiBitmapRb;
typedef cyitNode<Bitmap> cyiBitmapNode;

class xEXPORT cglsImageList 
{
public:
	cglsImageList(int nElementCount=0, Bool bIsVertical=False, eGlsItemAlign align=eGIA_CENTER, int distance=0);
	~cglsImageList();

	// --------------------------------------------
	static cglsImageList* Import(Bitmap* bmp, int nCount, Bool bIsVertical=False, eGlsItemAlign align=eGIA_CENTER, int distance=0);

	// --------------------------------------------
	int Add(Bitmap* bmp);

	// --------------------------------------------
	int Add(wchar_t* text, Font* font, color_t colorFont, color_t colorBg);

	// --------------------------------------------
	Bool Update(Font* font);

	// --------------------------------------------
	Bitmap* Export();

	// --------------------------------------------
	Bitmap* Get(float from, int length);

	// --------------------------------------------
	//Bitmap* Get(Bitmap* bmp, float from, int length, eGlsItemAlign align=eGIA_CENTER);

	// --------------------------------------------
	Bitmap* Get(int index);

	// --------------------------------------------
	//Bitmap* Get(Bitmap* bmp, int index, eGlsItemAlign align=eGIA_CENTER);

	// --------------------------------------------
	int GetIndex(float index);

	// --------------------------------------------
	int count();

	// --------------------------------------------
	inline Bitmap* xGetBitmap(int index);

	// --------------------------------------------
	void SetNumericRange(int min, int max);

	// --------------------------------------------
	void SetLimited(int limited);

	// --------------------------------------------
	int GetWidth();

	// --------------------------------------------
	int GetHeight();

	// --------------------------------------------
	void SetBitmapSize(int w, int h, color_t bgColor);


private:
	cyitList<Bitmap>* m_list;
	cyitrb<Bitmap>* m_listFixed;
	cyiList* m_listString;

	int m_nLimited;
	int m_ElementCount;
	Bool m_bIsVertical;
	eGlsItemAlign m_eAlign;
	int m_distance;
	int m_min;
	int m_max;
	int m_w;
	int m_h;
	color_t m_colorBg;


};

typedef cGlassInterface cGls;
typedef cGlassInterface* cGlsp;
