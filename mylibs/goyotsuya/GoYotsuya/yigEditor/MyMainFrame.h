#pragma once

#include "glsWindow.h"
#include "glsgoban.h"
#include "GoEditorApp.h"
#include "resource.h"

#define WM_NOTIFY_FROM_NETWORK 1025
#define WM_POST_COMMAND 1026
#define WM_NOTIFY_FROM_SERVER 1027

// ----------------------------------
class cMyMainFrame : public cglsWindow
{
public:
	cMyMainFrame();
	~cMyMainFrame();

	// ------------------------------------
#ifdef _DEBUG
	virtual int RegisterChild(cGlassInterface* child);
#endif

protected:
	// -------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// -------------------------------------
	virtual LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);

	// -------------------------------------
	virtual Bool OnBgDraw();

	// -------------------------------------
	LRESULT OnClose(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnEnterSizeMove(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnPreviewSelected(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnNodeChanged(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnPreviewPlay(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnCopyData(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnNotifyFromNetwork(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnNotifyFromServer(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnOpenFile(HWND hWnd, int code);

	// ---------------------------------------------------
	LRESULT OnNew(HWND hWnd, int code);

	// ---------------------------------------------------
	LRESULT OnSave(HWND hWnd, int code);

	// ---------------------------------------------------
	LRESULT OnSaveAs(HWND hWnd, int code);

	// ---------------------------------------------------
	LRESULT OnPreview(HWND hWnd, int code);

	// ---------------------------------------------------
	LRESULT OnChangeBg(HWND hWnd, int code);

	// ---------------------------------------------------
	virtual LRESULT OnNotDefineMessage(UINT msg, WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------
	LRESULT OnPostCommand(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------
	LRESULT OnWhois(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -------------------------------------
	virtual Bitmap* GetBgBitmap();

DEFINE_MESSAGES_BEGIN	
	MESSAGE(OnPostCommand, WM_POST_COMMAND)
	MESSAGE(OnClose,WM_CLOSE)
	MESSAGE(OnSizeChanged,WM_SIZE)
	MESSAGE(OnPreviewSelected, WM_PREVIEW_SELECTED)
	MESSAGE(OnPreviewPlay, WM_PREVIEW_PLAY)
	MESSAGE(OnNodeChanged,WM_NODE_CHANGED)
	MESSAGE(OnCopyData, WM_COPYDATA)
	MESSAGE(OnExitSizeMove,WM_EXITSIZEMOVE)
	MESSAGE(OnEnterSizeMove,WM_ENTERSIZEMOVE)
	MESSAGE(OnNotifyFromNetwork, WM_NOTIFY_FROM_NETWORK)
	MESSAGE(OnNotifyFromServer, WM_NOTIFY_FROM_SERVER)
	//MESSAGE(OnWhois, (WM_USER+129))
DEFINE_MESSAGES_END
//
DEFINE_COMMANDS_BEGIN
	COMMAND(OnOpenFile, IDC_OPEN_FILE)
	COMMAND(OnNew, IDC_CREATE_NEW)
	COMMAND(OnSave, IDC_SAVE)
	COMMAND(OnSaveAs, IDC_SAVE_AS)
	COMMAND(OnPreview, IDC_SHOW_PREVIEW)
	COMMAND(OnChangeBg, IDC_CHANGE_BG)
DEFINE_COMMANDS_END

private:
	CGoEditorApp* m_app;
	Image* m_imgBg;
	Bitmap* m_imgDefault;
	Bool m_bEnterSizeMove;
	Bool m_bOnSizeChangedCalled;

#ifdef _GOYOTSUYA
	int m_nNetworkUserPacket[eGNPMSG_COUNT];
#endif

};