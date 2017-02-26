#pragma once

#include "yistd.h"
#include "yiBitmap.h"

#include <atlbase.h>
#include <gdiplus.h>
#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

using namespace Gdiplus;

#define DEFINE_MESSAGES_BEGIN \
virtual LRESULT OnMessage(UINT message, WPARAM wp, LPARAM lp, Bool& IsHandled)\
{\
switch(message)\
{

#define MESSAGE(callback, id) \
case id:\
return callback(wp, lp, IsHandled);

#define MESSAGEEX(callback, id) \
case id:\
return callback(id, wp, lp, IsHandled);


#define DEFINE_MESSAGES_END \
default:\
	OnNotDefineMessage(message, wp, lp, IsHandled);\
break;\
}\
if (IsHandled==False)\
{\
__super::OnMessage(message, wp, lp, IsHandled);\
}\
return 0;\
}

// Notify Callback
// OnNotifyCallback(id, subId, NotifyCode)
#define DEFINE_NOTIFY_BEGIN \
virtual LRESULT OnNotify(WPARAM wp, LPARAM lp, Bool& IsHandled)\
{\
switch(wp)\
{

#define NOTIFY(callback, id) \
case id:\
{\
NMHDR* hdr=(NMHDR*)lp;\
IsHandled=True;\
return callback(id, hdr->idFrom, hdr->code);\
}

#define DEFINE_NOTIFY_END \
default:\
break;\
}\
return 0;\
}


#define DEFINE_COMMANDS_BEGIN \
virtual LRESULT OnCommand(WPARAM wp, LPARAM lp, Bool& IsHandled)\
{\
WORD wId=(wp&0xFFFF);\
switch(wId)\
{

#define COMMAND(callback, id) \
case id:\
IsHandled=True;\
return callback((HWND)lp, ((wp>>16)&0xFFFF));

#define COMMANDEX(callback, id) \
case id:\
IsHandled=True;\
return callback(wp, lp);

#define COMMAND_NOPARAM(callback, id) \
case id:\
IsHandled=True;\
return callback();


#define DEFINE_COMMANDS_END \
default:\
break;\
}\
return 0;\
}

#define WM_YCTRLBASE (WM_USER+5)

#define WM_GLS_DRAG_START (WM_YCTRLBASE + 1)
#define WM_GLS_DRAG	(WM_YCTRLBASE + 2)
#define WM_GLS_DRAG_END (WM_YCTRLBASE + 3)

class cyctrlbase;



// --------------------------------------
typedef Handle HandleGlassItem;

#define Z_ORDER_TOP_LEVEL -1
#define Z_ORDER_MOST_TOP -2

class cGlassInterface;

// ----------------------------
typedef struct xsRelayMessage {
	UINT msg;
	HWND hRelayWnd;
} sRelayMessage;

#include "yiThreadObject.h"
// ----------------------------
class xEXPORT cyctrlbase : public cyiThreadObject
{
public:
	cyctrlbase(void);
	virtual ~cyctrlbase(void);

	static LPDLGTEMPLATE CreateDialogTemplate(int x, int y, int w, int h, wchar_t* caption, int style=(WS_POPUP | WS_BORDER | WS_SYSMENU
                     | DS_MODALFRAME | WS_CAPTION | DS_SETFONT), int styleex=0, int fontSize=10, wchar_t* fontname=L"Meiryo", int btn=IDOK);

	// --------------------------------------------
	static HINSTANCE GetModuleHandle();

	// --------------------------------------------
	Bool CreateEx(HINSTANCE hInstance, unsigned int classStype=0, unsigned int style=0, Bool bUseDoubleBuffering=False);

	// --------------------------------------------
	Bool CreateChildEx(HWND hParentWnd, unsigned int classStype=0, unsigned int style=0, Bool bUseDoubleBuffering=False);

	// ------------------------------------
	BOOL Create(HWND hWndParent, int id, int x, int y, int w, int h, BOOL IsHide);

	// --------------------------------------------
	BOOL CreatePopup(HWND hWndParent, int x, int y, int w, int h, BOOL IsHide);

	// ------------------------------------
	Bool CreateFromReosurce(HWND hWndParent, HINSTANCE hInstance, int id, Bool bIsModal=False);

	// ------------------------------------
	operator HWND()
	{
		return m_hWnd;
	};

	// ------------------------------------
	virtual Bool Move(UINT x, UINT y, Bool bIsActivate=False);

	// ------------------------------------
	Bool ReSize(UINT x, UINT y, Bool bIsRepaint=False);

	// ------------------------------------
	Bool ReSizeClientRect(UINT x, UINT y, Bool bIsRepaint=False);

	// ------------------------------------
	void ZOrderTop();

	// ------------------------------------
	virtual void Show();

	// ------------------------------------
	virtual void Hide();



	// ------------------------------------
	HDC GetDC();

	// ------------------------------------
	void ReleaseDC(HDC hdc);

	// ------------------------------------
	virtual Bool InvalidateRect(RECT* lpRect, Bool bErase=False, Bool bIsDirect=False);


	// ------------------------------------
	virtual Bool GetClientRect(LPRECT lpRect);


	// ------------------------------------
	virtual void Destroy();

	// ------------------------------------
	Bitmap* GetBehindBitmap();

	// ------------------------------------
	Bitmap* MakeBmpBackground(Bitmap* bg, float edge, HandleShadow hShadow);

	// ------------------------------------
	int RegisterRelayMessage(UINT msg, HWND hWndRelay);

	//// ------------------------------------
	//void SetLButtonRepeatEnd(int nMillisec);

	//// ------------------------------------
	//void SetLButtonRepeatStart(int nMillisec);

public:
	Bool m_bShowDialog;


protected:
	// ------------------------------------
	void MouseTrackMode(BOOL enable)
	{
		m_bMouseTrackMode=enable;
		m_bMouseEnter=FALSE;
	}

	// ------------------------------------
	Bool m_nToolTipTimer;

	// ------------------------------------
	Bool m_nLButtonLongTimer;

	// ------------------------------------
	Bool m_bNowKeyRepeat;

	POINT m_ptLButtonLong;

	// ------------------------------------
	virtual LRESULT OnLButtonLongPressed(WPARAM lParam, LPARAM wParam){return 0;};

	// ------------------------------------
	void SetToolTipTimer(int n=500);

	// ------------------------------------
	void SetLButtonLongTimer(int n=500);

	// ------------------------------------
	Bool StartDrag(int x, int y);

	// ------------------------------------
	Bool EndDrag();

	// ------------------------------------
	Bool StartLButtonRepeat(int x, int y, int nMillisec);

	// ------------------------------------
	Bool EndLButtonRepeat();


	// ------------------------------------
	Bool m_bNowDrag;

	// ------------------------------------
	Bool m_bIsToolTipLock;

	// ------------------------------------
	void GetDragStartPos(int&x, int&y);

	//// ------------------------------------
	//virtual LRESULT OnDragStart(WPARAM wParam, LPARAM lParam);

	//// ------------------------------------
	//virtual LRESULT OnDrag(WPARAM wParam, LPARAM lParam, Bool& handled);

	//// ------------------------------------
	//virtual LRESULT OnDragEnd(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual LRESULT OnToolTipTimer(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	void DrawBase();

	// ------------------------------------
	virtual LRESULT OnCreate(WPARAM wp, LPARAM lp){return 0;};

	// ------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps=0);


	// ------------------------------------
	virtual LRESULT OnDestroy(WPARAM wp, LPARAM lp);


	// ------------------------------------
	virtual LRESULT OnClicked(WPARAM wp, LPARAM lp){return 0;};

	// ------------------------------------
	virtual LRESULT OnLButtonRepeated(WPARAM wp, LPARAM lp){return 0;};

	// ------------------------------------
	virtual LRESULT OnMouseEnter(WPARAM wp, LPARAM lp){return 0;};

	// ------------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp){return 0;};

	// ------------------------------------
	virtual void OnCreateAfterUpdate() {};

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled){return 0;};

	// ------------------------------------
	virtual LRESULT OnNotify(WPARAM wp, LPARAM lp, Bool& bIsHandled){return 0;};

	// ------------------------------------
	virtual LRESULT OnNotDefineMessage(UINT msg, WPARAM wp, LPARAM lp, Bool& bIsHandled){return 0;};


	// --------------------------------------------
	virtual Bool Create(cyctrlbase* base, int x, int y) {return False;};

	// --------------------------------------------
	//virtual LRESULT OnOK();

	// --------------------------------------------
	//virtual LRESULT OnCancel();

	// ------------------------------------
	virtual LRESULT OnMessage(UINT message, WPARAM wp, LPARAM lp, Bool& IsHandled)
	{
		IsHandled=FALSE;
		return 0;
	}

	// 
	virtual LRESULT OnMessageInterrupt(UINT msg, WPARAM wp, LPARAM lp, BOOL& IsHandled);

	// ------------------------------------
	virtual LRESULT OnNotHandledCommand(WPARAM wp, LPARAM lp){ return 0;};

	// ------------------------------------
	void DrawItem(Rect& rect);

	// ------------------------------------
	static HandleGlassItem CreateGlassPannelObject(Bitmap* bmp, int w, int h, float edge, Brush* brush, Bool bIsLike3D, HandleShadow hShadow);

	// ------------------------------------
	static HandleGlassItem CreateGlassPannelObject(const wchar_t* string, int w, int h, float edge, Brush* brush, Bool bIsLike3D, HandleShadow hShadow);

	// ------------------------------------
	static Bitmap* MakeGlassPannel(HandleGlassItem hItem, Bitmap* bg, int x=0, int y=0);

	// ------------------------------------
	static void DestroyGlassPannelObject(HandleGlassItem hItem);




	HWND		m_hParentWnd;

	HINSTANCE	m_hInstance;
	HANDLE		m_hResourceModule;
	BOOL		m_bMouseEnter;


	
public:
	HWND		m_hWnd;
	int			m_x;
	int			m_y;
	int			m_w;
	int			m_h;
	UINT		m_id;
	float		m_edge_size;
	Bool		m_bIsCreated;

public:
#ifndef _WIN64
	static LRESULT CALLBACK __wndproc__(HWND hWnd, UINT message, WPARAM wp, LPARAM lp);
	static INT	   CALLBACK	__dlgproc__(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
#else
	static LRESULT CALLBACK __wndproc__(HWND hWnd, UINT message, WPARAM wp, LPARAM lp);
	static INT_PTR	   CALLBACK	__dlgproc__(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
#endif

protected:
	BOOL m_bMouseTrackMode;
	BOOL m_bPushed;
	BOOL m_bIsModal;
	BOOL m_bMouseTrack;


	// ------------------------------------
	LRESULT OnPreMessage(UINT message, WPARAM wp, LPARAM lp, Bool bIsDialog=False);

	// ------------------------------------
	BOOL cyctrlbase::OnPreMessageDlg(UINT msg, WPARAM wp, LPARAM lp);

	cyiList m_child;

private:
	POINT m_ptMouseDown;
	POINT m_ptMouseMove;
	POINT m_ptToolTip;
	cyiList* m_relay_message;
	cyiQueue* m_queueToolTip;
	Bool m_bIsMouseNoInput;

protected:
	POINT m_ptDragOn;


	friend class cGlassInterface;
	friend class cglsImageBox;
};

