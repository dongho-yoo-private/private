#pragma once

#include "glsWindow.h"
#include "AppGoYotsuya.h"

#define WM_MERGE_CONFIRM (WM_USER+1024)

class CMainWindow : public cglsWindow
{
public:
	CMainWindow(void);
	~CMainWindow(void);

protected:
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnPreviewSelected,WM_PREVIEW_SELECTED);
	MESSAGE(OnSizeChanged,WM_SIZE);
	MESSAGE(OnPutStoned,WM_PUT_STONED);
	MESSAGE(OnQuitOptimize,WM_QUIT_OPTIMIZE);
	MESSAGE(OnSearchResult,WM_QUIT_SEARCHRESULT);
	MESSAGE(OnNodeChanged,WM_NODE_CHANGED);
	MESSAGE(OnClose,WM_CLOSE);
DEFINE_MESSAGES_END

	// ---------------------------------------------------
	LRESULT OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnPreviewSelected(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnPutStoned(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnQuitOptimize(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnSearchResult(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnNodeChanged(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	LRESULT OnClose(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ---------------------------------------------------
	virtual LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);

	// ---------------------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------
	virtual Bool OnBgDraw();

private:
	cAppGoYotsuya* m_app;

	Bitmap* m_TextureImg;
};
