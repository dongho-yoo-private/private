#pragma once

#include "GlassInterface.h"
#include "glsToolButton.h"
#include "glsDrawObject.h"
#include "yit.h"

enum eGlsSpinBoxType {
	eGSBT_NUMERIC,
	eGSBT_IMAGE
};

enum eGlsSpinBoxScrollMode {
	eGPBSM_LINEAR,
	eGPBSM_NONLINEAR
};

typedef struct 
{
	sGlsDetail gls;
	Brush* bgBrush;
	color_t colorFont;
	color_t colorBg;
	float fEdge;
	float fScrollTerm;
	Bool bIsShowButton;
	eGlsSpinBoxScrollMode eMode;
}sGlsSpinBoxDetail;

typedef struct {
	HandleAnit hScrollAnimation;
	float fTerm;
	Bool bIsInit;
	Bool bIsReqAnimationUpdte;
	int value;
	float fCurrentPos;
	float fBeforePos;
	float fLimited;
	int nShift;

	int width;
	int distance;
} sScrollAnitInfo;

enum eSplitBoxValueFrom {
	eSBVF_CURRENT,
	eSBVF_ZERO
};

#define WM_SPINBOX_SCROLL (WM_USER+512)

class xEXPORT cglsSpinBox : public cGlassInterface
{
public:
	cglsSpinBox(void);
	virtual ~cglsSpinBox(void);

	// -----------------------------
	static void xDefaultDetail(sGlsSpinBoxDetail* detail);

	// -----------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, int nSpinCount, sGlsSpinBoxDetail* detail=0);


	// -----------------------------
	Bool SetNumer(int n, int min, int max, int nDefaultValue=0, int nShift=0);

	// -----------------------------
	void InsertLabel(const wchar_t* label, int index, color_t colorFont);

	// -----------------------------
	void SetLimited(int n, int no);

	// -----------------------------
	void SetTotalLimited(float from, float to);

	// ------------------------------
	Bool AddString(int n, wchar_t* str, int number, Bool bIsDefault=False);

	// ------------------------------
	Bool Clear(int n);

	// ------------------------------
	//Bool ScrollTo(float fIndex, int nTerm);

	Bool SetValue(int value, eSplitBoxValueFrom eFrom=eSBVF_CURRENT, Bool bAnimation=True, int nAnimationTerm=100);

	// ------------------------------
	IntPtr GetValue(int nIndex=-1);

	// -----------------------------------------------------
	int GetValueAfterAnimation();


	// ------------------------------------
	Bool IsNowAnimation();

private:
	// ------------------------------
	int xGetValue(int nIndex);

protected:
	// -----------------------------
	virtual void Update(Bitmap* bmp, RECT* rect=0, RECT* _childRect=0, int offsetX=0, int offsetY=0, Bool bIsLocalBufferUpdate=False);

	// ---------------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnMouseEnter(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	LRESULT OnMouseLButtonDown(WPARAM wp, LPARAM lp, Bool bIsHandled);

	// -----------------------------
	virtual void Update(eGlsUpdateMode mode=eGUM_UPDATE, eDrawItemStatus status=eDIS_NORMAL);

private:
	// ------------------------------
	float xGetFactor(float f, int n);

	// ------------------------------
	float xGetFactorIndex(int index);
	
	// ---------------------------------------
	LRESULT xPosToIndex(int x, int y);

	// ------------------------------
	LRESULT OnScrollBtn(HWND hWnd, int code);

	// ------------------------------
	//void RunNextAnimation(int index);

	// ------------------------------
	void RunNextAnimation(int nIndex, float before, float after,Bool bIsForced=False, float term=1.0);

	// ------------------------------
	int xIsBetween(int index, float from, float to);

	// ------------------------------
	float xIsNextAnimation(int index, float cur, float before);

	// ------------------------------
	LRESULT OnScroll(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ------------------------------
	LRESULT xScroll(int nIndex, int nTerm=100, int nAnimationType=eGAP_SIFO);

	// ------------------------------
	cglsToolButton* m_btn;

	// -----------------------------
	Bitmap* xCreateBmpCover(int width, int nFontHeight);

	// -----------------------------
	void xReSize(int h);




	// ------------------------------
	Bool m_bIsShowButton;

	// ------------------------------
	int m_nSpinCount;

	// ------------------------------
	Brush* m_brush;

	// ------------------------------
	color_t m_colorFont;

	// ------------------------------
	color_t m_colorBg;

	// ------------------------------
	sScrollAnitInfo** m_scrinfo;

	// ------------------------------
	Bool** m_bIsReqAnimation;

	// ------------------------------
	cglsImageList** m_list;

	// ------------------------------
	//cyiList* m_listDraw;

	// ------------------------------
	float m_fMax;

	// ------------------------------
	float m_fMin;

	// ------------------------------
	float m_fNextMax;

	eGlsSpinBoxScrollMode m_eMode;

	Bitmap* m_bmpCover;

	int m_sumWidth;

	int m_labelWidth;

	int m_fScrollTerm;

	// ------------------------------------
	unsigned int m_nAfterValue;

	// ------------------------------
	//float m_min;

	// ------------------------------
	//float m_max;

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnScroll, WM_SPINBOX_SCROLL)
	MESSAGE(OnMouseLButtonDown, WM_LBUTTONDOWN)
DEFINE_MESSAGES_END

DEFINE_COMMANDS_BEGIN
	COMMAND(OnScrollBtn, 1)
DEFINE_COMMANDS_END

};
