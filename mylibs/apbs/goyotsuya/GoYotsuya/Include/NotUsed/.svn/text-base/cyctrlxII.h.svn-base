#pragma once

#include "yistd.h"
#include "yctrlbase.h"
#include "yiList.h"


#define WM_ITEM_MOUSE_ENTER (WM_USER+128)

enum eCtrlxItemPosII {
	eCIPII_CENTER=-1,
	eCIPII_LEFT=-2,
	eCIPII_RIGHT=-3
};

enum eCtrlxItemOffsetII {
	eCIOII_LEFT_FROM = 0x01,
	eCIOII_CENTER_FROM = 0x02,
	eCIOII_RIGHT_FROM = 0x04,
	eCIOII_TOP_FROM = (eCIOII_LEFT_FROM<<8),
	eCIOII_VCENTER_FROM = (eCIOII_CENTER_FROM<<8),
	eCIOII_BOTTOM_FROM = (eCIOII_RIGHT_FROM<<8)
};



enum eContorlxAlignII {
	eCXAII_CENTER	= 1,
	eCXAII_LEFT	= 2,
	eCXAII_RIGHT	= 4,
	eCXAII_VCENTER = eCXAII_CENTER<<4,
	eCXAII_TOP		= eCXAII_LEFT<<4,
	eCXAII_BOTTOM	= eCXAII_RIGHT<<4
};

//typedef Bool (*xControlDraw)(int id, Graphics* g, Font* fnt, sControlxII* x, eControlxEvent xevent);

class cyctrlxII : public cyctrlbase
{
public:
	cyctrlxII(void);
	~cyctrlxII(void);

	// --------------------------------------------
	static void GetBmpPos(sControlxII* s, float* x, float* y, unsigned int w, unsigned int h, eContorlxAlignII align);

	// --------------------------------------------
	static void GetTextPos(sControlxII* s, float* x, float* y, eContorlxAlignII align);

	// --------------------------------------------
	static void GetTextPos(sControlxII* s, float* x, float* y, unsigned int w, unsigned int h, eContorlxAlignII align);

	// --------------------------------------------
	Bool Create(cyctrlbase* base, int id, int x, int y, int w_margin, int h_margin, BOOL bIsHide=True, Font* fnt=0, BOOL bIsVertical=False, unsigned int w=0, unsigned int h=0);

	// --------------------------------------------
	virtual Bool Create(cyctrlbase* base, int x, int y);

	// --------------------------------------------
	//BOOL CreateRotate(HWND hWndParent, int x, int y, int w_max, int h_max, xControlDraw drawf, BOOL bIsHide=True, Font* fnt=0, BOOL bIsVertical=False, unsigned int w=0, unsigned int h=0);

	// -----------------------------
	//void Update(Bool bIsShow=True, int n=0, unsigned int wmax=0, unsigned int hmax=0);

	// -----------------------------
	void UpdateRotate(int nLotate, Bool bIsShow=True, int n=0, unsigned int wmax=0, unsigned int hmax=0);

	// --------------------------------------------
	void Destroy();

	// -----------------------------
	int Add(int id, Bitmap* bmp, const wchar_t* pszText, int distance, int marginX, int marginY, void* pParam, const wchar_t* pszToolTip=0, Bool bIFixedMode=False);
	
	// -----------------------------
	int Addx(int id, Bitmap* bmp, const wchar_t* pszText, int distance, 
				 int marginX, int marginY, int maxw, int minw, void* pParam, const wchar_t* pszToolTip);

	// -----------------------------
	int Add(int id, cyctrlbase* ctrl, int distance, int marginX, int marginY, void* pParam);

	// -----------------------------
	Bool cyctrlxII::ArrangeItems(int&x, int&y, int wmax=0, int hmax=0);

	// -----------------------------
	void Remove(unsigned int id);

	// -----------------------------
	void Update(Bool bIsShow=True, Bool bIsArrang=False, unsigned int wmax=0, unsigned int hmax=0);

	// -----------------------------
	//void UpdateRotate(int nLotate, int index, Bool bIsShow, unsigned int wmax, unsigned int hmax);


	// -----------------------------
	void EnableItem(int id, Bool bIsEnable);

	// -----------------------------
	int SelectionChange(int id);

	// -----------------------------
	/*void Scroll(int index);*/


protected:
	// ---------------------------------------
	virtual BOOL OnPaint(HDC hdc, PAINTSTRUCT* ps=0);

	// ---------------------------------------
	virtual LRESULT OnClicked(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	LRESULT OnMouseMove(WPARAM wp, LPARAM lp, BOOL& handled);

	// ---------------------------------------
	LRESULT OnMouseLButtonDown(WPARAM wp, LPARAM lp, BOOL& handled);

	// ---------------------------------------
	LRESULT OnMouseLButtonUp(WPARAM wp, LPARAM lp, BOOL& handled);


private:

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnMouseMove, WM_MOUSEMOVE)
	MESSAGE(OnMouseLButtonDown, WM_LBUTTONDOWN)
	MESSAGE(OnMouseLButtonUp, WM_LBUTTONUP)
DEFINE_MESSAGES_END



	cyiList m_xx;


	Bool m_bIsVertical;
	Bool m_bgFixed;
	sControlxII** m_xpp;
	sControlxII*	m_selected_item;
	Font* m_xfont;
	unsigned int m_wMargin;
	unsigned int m_hMargin;
	Handle m_hMutex;
	BOOL m_bCoordChanged;

	//cyctrlbase* m_parent;
	int m_id;

};
