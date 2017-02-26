#pragma once

#include "GlassInterface.h"
#include "glsDrawObject.h"
#include "glsButton.h"
#include "glsPopupMenu.h"
#include "glsListMenu.h"


enum eSplitButtonStyle {
	eSBS_LEFT_FULLDOWN=0x01,
	eSBS_RIGHT_FULLDOWN=0x02,
	eSBS_NO_FULLDOWN=0
};


typedef struct {
	float   edge;
	color_t colorHilight; //__COLORp__(0, 127, 255, 127)
	RECT rect;
	eSplitButtonStyle eStyle;

	sGlsDetail gls;
} sGlsSplitButtonDetail;

class xEXPORT  cglsSplitButton : public cGlassInterface, public cglsDrawObject
{
public:
	cglsSplitButton(void);
	~cglsSplitButton(void);

	static void xDefaultDetail(sGlsSplitButtonDetail* detail);

	// ---------------------------
	Bool Create(HWND hWnd, int id, int x, int y, int w, int h, const wchar_t* pszToolTip=0, sGlsSplitButtonDetail* detail=0);

	// ---------------------------
	Bool AddItem(int subId, Bitmap* icon, const wchar_t* defultText, const wchar_t* pszComment=0, Bool bIsSelected=False);

	// ---------------------------
	Bool AddString(wchar_t* text);

	// ---------------------------
	void Clear();

	// ---------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* param);

	// -----------------------------
	virtual void Update(eGlsUpdateMode mode=eGUM_UPDATE, eDrawItemStatus status=eDIS_NORMAL);

	// -----------------------------------------------------
	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp, Bool& bIsHandled);

	// ---------------------------------------
	virtual LRESULT OnClicked(WPARAM wp, LPARAM lp);

	// -----------------------------------------------------
	int GetItemCount();

	// -----------------------------------------------------
	int SelectIndex(int n);

	// -----------------------------------------------------
	int GetSelectIndex();

	// -----------------------------------------------------
	int GetSelectedId();

	// -----------------------------------------------------
	Bool GetSelectedText(wchar_t* buffer);

	// -----------------------------------------------------
	//void SetBgBrush(Brush* brush);


	Bool m_bIsBeforeMenuShow;

private:
	Bool m_bIsAutoWidth;

	Bitmap* m_bmpDefault[3];
	cyiList* m_listSplitBtnItem;
	int m_nSelectedIndex;

	sGlsSplitButtonDetail m_detail;
	//cglsPopupMenu* m_menu;
	cglsListMenu* m_menu;

	Bitmap* m_bmpRightFullDown;

	Bool m_bIsImageOnly;

	//Brush* m_Bgbrush;

	float m_factor;

	// ------------------------------------
	virtual LRESULT OnMouseLButtonDown(WPARAM wParam, LPARAM lParam, BOOL& handled);

	// ------------------------------------
	virtual LRESULT OnMouseLButtonUp(WPARAM wParam, LPARAM lParam, BOOL& handled);

	// -----------------------------
	virtual int GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation);

// ------------------------------------
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnMouseLButtonDown, WM_LBUTTONDOWN)
	MESSAGE(OnMouseLButtonUp, WM_LBUTTONUP)
DEFINE_MESSAGES_END

	RECT m_rectRight;
	Bool m_bIsRightDown;
	eSplitButtonStyle m_eStyle;




};
