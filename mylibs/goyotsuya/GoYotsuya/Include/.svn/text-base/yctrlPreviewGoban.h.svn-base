#pragma once

#include "yistd.h"
#include "yctrlbase.h"
#include "stonemap.h"
#include "glsButton.h"
#include "glsComboBox.h"
#include "yctrlDlgPreviewDetail.h"

#define MAX_GOBAN_CNT 128

#define WM_PREVIEW_SELECTED (WM_USER+577)
#define WM_PREVIEW_PLAY (WM_PREVIEW_SELECTED +1)


// -----------------------------------
typedef struct xsPrevStruct {
	cyiTreeNode* node;
	stone_t** stone;
} sPrevStruct;

class __declspec(dllexport) cyctrlPreviewGoban : cyctrlbase
{
public:
	cyctrlPreviewGoban(void);
	~cyctrlPreviewGoban(void);

	// ---------------------------------
	Bool Create(HWND hWnd, int width, int nCount, int nLineCount=19);

	// ---------------------------------
	Bool Update(cyiTreeNode* top);

	// ---------------------------------
	Bool AddNode(cyiTreeNode* top, stone_t** stone);

	// ---------------------------------
	void Clear();

	// ---------------------------------------
	virtual BOOL OnPaint(HDC hdc, PAINTSTRUCT* ps=0);

	//// ------------------------------------
	//virtual LRESULT OnMouseMove(WPARAM wp, LPARAM lp);

	// ------------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

	// ------------------------------------
	virtual LRESULT OnClicked(WPARAM wp, LPARAM lp);




private:

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnVScroll, WM_VSCROLL);
	MESSAGE(OnMouseMove, WM_MOUSEMOVE);
	MESSAGE(OnPreviewDetailClosed, WM_CLOSED_PREVIEW_DETAIL);
DEFINE_MESSAGES_END

DEFINE_COMMANDS_BEGIN
	COMMAND(OnPlay, 2);
	COMMAND(OnBack, 1);
	COMMAND(OnPlayTop, 3);
	COMMAND(OnPlayEnd, 4);
	COMMAND(OnFilterChanged, 5);
	COMMAND(OnDetail, 6);
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
	LRESULT OnFilterChanged(HWND hWnd, int code);

	LRESULT OnVScroll(WPARAM wp, LPARAM lp, Bool& Handled);

	// ---------------------------------------
	LRESULT OnMouseMove(WPARAM wp, LPARAM lp, Bool& handled);

	// -------------------------------
	LRESULT OnPreviewDetailClosed(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------
	void DrawGoban(int index, sPrevStruct* data);

	// ---------------------------------
	Bool xSearchMap(cyiTreeNode* top, sSearchFilter* filter);

	// ---------------------------------
	Bitmap* CreateGobanBitmap(char** bmp);





	Bitmap* m_gobanz[MAX_GOBAN_CNT];
	Bitmap* m_bmpWhite;
	Bitmap* m_bmpBlack;

	Bitmap* m_currPage;
	Bitmap* m_nextPage;
	Bitmap* m_prevPage;
	Bitmap* m_bmpGoban;

	cglsButton m_btnPlay;
	cglsButton m_btnPrev;
	cglsButton m_btnPlayTop;
	cglsButton m_btnPlayEnd;

	cyctrlDlgPreviewDetail m_dlgDetail;

	cglsComboBox m_cmbFilter;
	cglsButton m_btnDetail;

	int m_nFilter;
	HWND m_hNotifyWnd;

	cyiTreeNode* m_currentNode;
	cyiTreeNode* m_prevNode;

	int m_nPrevStartIndex;

	cyiTreeNode* m_selectedNode;

	cyiList m_listHistory;

	int m_nCurrentPlayIndex;

	cyiList previewList;

	int m_nPrevWidth;
	int m_nLineCount;
	int m_nBlockSize;
	int m_nShowCnt;
	int m_nStartIndex;
	int m_nHitCount;
	int m_nSelectedIndex;

	Bool m_bModePlay;

	float m_coord[19];

	sSearchFilter m_sSearchFilter;
	cStoneMap map;
};
