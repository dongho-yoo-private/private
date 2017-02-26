#pragma once

#include "yistd.h"
#include "yctrlbase.h"
#include "yiList.h"


// ---------------------------------------
typedef struct xsCtrlx {
	int id;
	Bitmap* bmp;
	wchar_t name[256];
	float txt_w;
	float txt_h;
	int distance;
	int w_fixed;
	int h_fixed;
	Bool bIsEnable;
	RECT rect;
	void* p;
	Bitmap* pBmp[10];
	int wmin;
	int wmax;
} sControlx;

enum eCtrlxItemPos {
	eCIP_CENTER=-1,
	eCIP_LEFT=-2,
	eCIP_RIGHT=-3
};

enum eCtrlxItemOffset {
	eCIO_LEFT_FROM = 0x01,
	eCIO_CENTER_FROM = 0x02,
	eCIO_RIGHT_FROM = 0x04,
	eCIO_TOP_FROM = (eCIO_LEFT_FROM<<8),
	eCIO_VCENTER_FROM = (eCIO_CENTER_FROM<<8),
	eCIO_BOTTOM_FROM = (eCIO_RIGHT_FROM<<8)
};

enum eControlxEvent {
	eCE_BACKGROUND,
	eCE_MOUSE_IN,
	eCE_MOUSE_LEAVE,
	eCE_MOUSE_LDOWN,
	eCE_MOUSE_LUP,
	eCE_MOUSE_LUPX,

	eCE_MOUSE_CLICKED,
	eCE_INIT,
	eCE_DISABLE
};

enum eContorlxAlign {
	eCXA_CENTER	= 1,
	eCXA_LEFT	= 2,
	eCXA_RIGHT	= 4,
	eCXA_VCENTER = eCXA_CENTER<<4,
	eCXA_TOP		= eCXA_LEFT<<4,
	eCXA_BOTTOM	= eCXA_RIGHT<<4
};

typedef Bool (*xControlDraw)(int id, Graphics* g, Font* fnt, sControlx* x, eControlxEvent xevent);

class cyctrlx : public cyctrlbase
{
public:
	cyctrlx(void);
	~cyctrlx(void);

	// --------------------------------------------
	static void GetBmpPos(sControlx* s, float* x, float* y, unsigned int w, unsigned int h, eContorlxAlign align);

	// --------------------------------------------
	static void GetTextPos(sControlx* s, float* x, float* y, eContorlxAlign align);

	// --------------------------------------------
	static void GetTextPos(sControlx* s, float* x, float* y, unsigned int w, unsigned int h, eContorlxAlign align);

	// --------------------------------------------
	BOOL Create(HWND hWndParent, int x, int y, int w_margin, int h_margin, xControlDraw drawf, BOOL bIsHide=True, Font* fnt=0, BOOL bIsVertical=False, unsigned int w=0, unsigned int h=0);

	// --------------------------------------------
	BOOL Create(cyctrlbase* parent, int x, int y, int w_margin, int h_margin, xControlDraw drawf, BOOL bIsHide=True, Font* fnt=0, BOOL bIsVertical=False, unsigned int w=0, unsigned int h=0);

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
	void Remove(unsigned int id);

	// -----------------------------
	void Update(Bool bIsShow=True, unsigned int wmax=0, unsigned int hmax=0);

	// -----------------------------
	//void UpdateRotate(int nLotate, int index, Bool bIsShow, unsigned int wmax, unsigned int hmax);


	// -----------------------------
	void EnableItem(int id, Bool bIsEnable);

	// -----------------------------
	/*void Scroll(int index);*/




protected:
	// ---------------------------------------
	virtual BOOL OnPaint(HDC hdc, PAINTSTRUCT* ps);

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



	cyiList m_x;
	Bitmap* m_bmp;
	Graphics* m_g;

	Bool m_bIsVertical;
	Bool m_bgFixed;
	xControlDraw m_xdraw;
	sControlx** m_xpp;
	sControlx*	m_selected_item;
	Font* m_xfont;
	unsigned int m_wMargin;
	unsigned int m_hMargin;
	Handle m_hMutex;
	BOOL m_bCoordChanged;

	cyctrlbase* m_parent;

};
