#pragma once

#include "glsStackPannel.h"
#include "glsTabCtrl.h"



class xcglsTabItem : public cglsStackPannel, cglsDrawObject
{
public:
	xcglsTabItem(void);
	~xcglsTabItem(void);

	// ------------------------------------------
	Bool Create(HWND hWndParent, int id, uIntPtr bmpId, wchar_t* pszCaption, eItemAlign align=eIA_LEFT, cglsDrawObject* object=0, Bool bHasClosed=False, sGlsTabItem* item=0);

	// ------------------------------------------
	int GetItemLength();

	// ------------------------------------------
	Bool SetItemLength(int n);

	// ------------------------------------------
	int SetText(const char* pszText);

	// ------------------------------------------
	int SetText(const wchar_t* pszText);

	// ------------------------------------------
	int SetChanged(Bool bIsTrue);

	// ------------------------------------------
	int SetButton(Bool bIsTrue);

	// ------------------------------------------
	void Select(Bool bIsSelected=True);

	// ------------------------------------------
	int GetBgIndex(eDrawItemStatus status, Bool bIsHitted, Bool& bIsSkipAnimation);

	// ------------------------------------------
	virtual LRESULT OnMouseLButtonDown(WPARAM wp, LPARAM lp, Bool& handled);

	// ------------------------------------------
	virtual LRESULT OnMouseLButtonUp(WPARAM wp, LPARAM lp, Bool& handled);

protected:

//DEFINE_MESSAGES_BEGIN
//	MESSAGE(OnMouseLButtonDown, WM_LBUTTONDOWN)
//DEFINE_MESSAGES_END
DEFINE_COMMANDS_BEGIN
	COMMAND(OnItemClose, 3)
DEFINE_COMMANDS_END


	// ------------------------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// ------------------------------------------
	virtual void DestroyBitmap(sDrawItem* item);


	// ---------------------------------------
	virtual void xCalculatePosition(RECT* rectAll);

	// ---------------------------------------
	virtual LRESULT OnMessageInterrupt(UINT msg, WPARAM wp, LPARAM lp, BOOL& IsHandled);

	//// -------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);


	// ---------------------------------------
	LRESULT OnItemClose(HWND hWnd, int Id);

	// ---------------------------------------
	//LRESULT OnMouseLButtonDown(WPARAM wp, LPARAM lp, BOOL& handled);

	// ------------------------------------------
	virtual Bool UpdateBg(eDrawItemStatus status=eDIS_NORMAL, Bool bIsHitted=False);

	// ------------------------------------------
	int m_nLength;

	 sGlsTabItem m_detail;

private:
	// ------------------------------------------
	int xGetOtherItemsLength();

	// ------------------------------------------
	int xGetTextLength();

	// ------------------------------------------
	int m_nOtherItemsLength;

	// ------------------------------------------
	int m_nTextLength;

	// ------------------------------------------
	int m_nOriginLength;

	// ------------------------------------------
	Bool m_bIsDelete;

	// ------------------------------------------
	Bool m_bIsTextReduced;

	friend class xcglsTabCtrl;

};
