#pragma once

#include "yistd.h"
#include "yctrls.h"
#include "stonemap.h"
#include "glsWindow.h"
#include "glsButton.h"
#include "glsComboBox.h"
#include "glsSplitButton.h"
#include "glsStackPannel.h"
#include "yctrlDlgPreviewDetail.h"

#define MAX_GOBAN_CNT 128

#define WM_PREVIEW_SELECTED (WM_USER+577)
#define WM_PREVIEW_PLAY (WM_PREVIEW_SELECTED +1)
#define WM_REQ_ADD_BITMAP (WM_PREVIEW_PLAY +1)
#define WM_XSEARCHMAP_END (WM_REQ_ADD_BITMAP+1)


// -----------------------------------
typedef struct xsPrevStruct {
	cyiTreeNode* node;
	stone_t** stone;
} sPrevStruct;

enum ePreviewMode {
	ePM_JOSEKI_JITEN,
	ePM_NORMAL,
	ePM_TAB
};

typedef struct {
	int nWidth;
	int nCount;
	int nMinimumWidth;
	int nMinimumCount;
} sGlsPreviewGoban;



class xEXPORT cglsPreviewGoban : public cglsWindow, cglsDrawObject
{
public:
	cglsPreviewGoban(void);
	virtual ~cglsPreviewGoban(void);

	static void xDefaultDetail(sGlsPreviewGoban* detail);


	// ---------------------------------
	Bool Create(HWND hWnd, int width, int nCount, int nLineCount=19, ePreviewMode mode=ePM_JOSEKI_JITEN);

	// ---------------------------------
	Bool CreateAsChild(HWND hWnd, int id, int x, int y, int nLineCount=19, ePreviewMode mode=ePM_NORMAL, sGlsPreviewGoban* detail=0);

	// ---------------------------------
	Bool UpdateTree(cyiTreeNode* top, Bool bIsJosekiMode=False);

	// ---------------------------------
	Bool AddNode(cyiTreeNode* top, stone_t** stone);

	// ---------------------------------
	int AutoReSize(int nHeight);

	// ---------------------------------
	void Clear();

	// ---------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps=0);

	//// ------------------------------------
	//virtual LRESULT OnMouseMove(WPARAM wp, LPARAM lp);

	// ------------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

	// ------------------------------------
	virtual LRESULT OnClicked(WPARAM wp, LPARAM lp);

	// --------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// --------------------------
	virtual void DestroyBitmap(sDrawItem* item);

	// --------------------------
	virtual Bool OnNullBitmapDraw(sDrawItem* item, void* pExtra=0);

	// --------------------------
	void OnThreadStart(int id, int param1, int param2);

	// --------------------------
	virtual LRESULT OnDestroy(WPARAM wp, LPARAM lp);

	// --------------------------
	virtual Bool OnBgDraw();

	// ------------------------------------
	virtual LRESULT OnNotHandledCommand(WPARAM wp, LPARAM lp);

	Bitmap* m_bmpBgGoban;

protected:

private:
	// ---------------------------------
	Bool StartCreateBitmap();


DEFINE_MESSAGES_BEGIN
	MESSAGE(OnVScroll, WM_VSCROLL);
	MESSAGE(OnMouseMove, WM_MOUSEMOVE);
	MESSAGE(OnPreviewDetailClosed, WM_CLOSED_PREVIEW_DETAIL);
	MESSAGE(OnRequestAddBitmap, WM_REQ_ADD_BITMAP);
	MESSAGE(OnXSearchEnd, WM_XSEARCHMAP_END);
	MESSAGE(OnFocus, WM_SETFOCUS);
	MESSAGE(OnNodeChanged, WM_NODE_CHANGED);
DEFINE_MESSAGES_END

DEFINE_COMMANDS_BEGIN
	COMMAND(OnPlay, 2);
	COMMAND(OnBack, 1);
	COMMAND(OnPlayTop, 3);
	COMMAND(OnPlayEnd, 4);
	COMMAND(OnFilterChanged, 5);
	COMMAND(OnDetail, 6);
	COMMAND(OnPreviewSelected, 11);
 DEFINE_COMMANDS_END

	// -------------------------------
	LRESULT OnPlay(HWND hWnd, int code);

	// -------------------------------
	LRESULT OnBack(HWND hWnd, int code);

	// -------------------------------
	LRESULT OnPlayTop(HWND hWnd, int code);

	// -------------------------------
	LRESULT OnPlayEnd(HWND hWnd, int code);

	// -------------------------------
	LRESULT OnDetail(HWND hWnd, int code);

	// -------------------------------
	LRESULT OnPreviewSelected(HWND hWnd, int code);




	// -------------------------------
	LRESULT OnFilterChanged(HWND hWnd, int code);

	// -------------------------------
	LRESULT OnVScroll(WPARAM wp, LPARAM lp, Bool& Handled);

	// -------------------------------
	LRESULT OnRequestAddBitmap(WPARAM wp, LPARAM lp, Bool& Handled);

	// -------------------------------
	LRESULT OnNodeChanged(WPARAM wp, LPARAM lp, Bool& Handled);

	// ---------------------------------------
	LRESULT OnMouseMove(WPARAM wp, LPARAM lp, Bool& handled);

	// -------------------------------
	LRESULT OnPreviewDetailClosed(WPARAM wp, LPARAM lp, Bool& handled);

	// -------------------------------
	LRESULT OnXSearchEnd(WPARAM wp, LPARAM lp, Bool& handled);

	// -------------------------------
	LRESULT OnFocus(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------
	void DrawGoban(int index, sPrevStruct* data);

	// ---------------------------------
	Bool xSearchMap(cyiTreeNode* top, sSearchFilter* filter);

	// ---------------------------------
	Bitmap* CreateGobanBitmap(char** bmp);

	cglsStackPannel* m_panel;

	Bitmap* m_gobanz[MAX_GOBAN_CNT];
	Bitmap* m_bmpWhite;
	Bitmap* m_bmpBlack;

	Bitmap* m_currPage;
	Bitmap* m_nextPage;
	Bitmap* m_prevPage;
	Bitmap* m_bmpGoban;


	cyctrlDlgPreviewDetail m_dlgDetail;

	cglsSplitButton m_cmbFilter;
	//cglsComboBox m_cmbFilter;
	cglsButton m_btnDetail;

	int m_nFilter;

	cyiTreeNode* m_currentNode;
	cyiTreeNode* m_prevNode;

	int m_nPrevStartIndex;

	cyiTreeNode* m_selectedNode;

	cyiList m_listHistory;

	int m_nCurrentPlayIndex;

	cyiList* previewList;
	cyiList* listDeleteList;

	int m_nPrevWidth;
	int m_nLineCount;
	int m_nBlockSize;
	int m_nShowCnt;
	int m_nStartIndex;
	int m_nHitCount;
	int m_nSelectedIndex;

	int	m_nBufferringCount;
	HANDLE m_hThreadEvent;

	Bool m_bIsSlowLoad;

	Bool m_bIsSearchStop;

	Bool m_bIsDataInvalid;


	Bool m_bModePlay;

	Bool m_bIsThreadStop;

	float m_coord[19];

	sSearchFilter m_sSearchFilter;
	cStoneMap map;

	HandleMutexx m_hPaintMutex;
	HandleMutexx m_hUpdteThreadMutex;

	Bool m_bIsJosekiMode;

	ePreviewMode m_eMode;

	sGlsPreviewGoban m_detail;

	HandleMutexx m_hMutexThread2;

	cyiTreeNode* m_lastNode;

	int m_WaitThreadCnt;

	private:
	// ---------------------------------
	Bool xCreate(HWND hWnd, int id, int x, int y, int width, int nCount, int nLineCount, int nWindowStyle, int nWindowStyleEx);
};
