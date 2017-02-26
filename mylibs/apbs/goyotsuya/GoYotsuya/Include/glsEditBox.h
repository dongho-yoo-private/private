#pragma once
#include "GlassInterface.h"
#include "glsDrawObject.h"
#include "yit.h"
#include <imm.h>
#include "glsScrollBar.h"
#pragma comment(lib, "imm32.lib")

#undef GetCharWidth

#define WM_EDIT_MESSAGE (WM_USER+4096)
#define WM_EDIT_SCROLL  (WM_EDIT_MESSAGE+1)

enum eGlsEditBoxLayout {
	eGEBL_LEFT,
	eGEBL_CENTER,
	eGEBL_RIGHT
};

typedef struct xsGlsEditBoxDetail {
	sGlsDetail gls;
	Bool bIsMutipleLine;
	Bitmap* bmpCarot;
	color_t colorBg;//=xdetail.colorBg;
	color_t colorBorder;//=xdetail.colorBorder;
	color_t colorFont;//=xdetail.colorFont;
	color_t colorSelection;//=xdetail.colorSelection;
	color_t colorSelectionFont;
	
	color_t colorStaicModeFont;
	color_t colorStaticModeBg;
	int nCarotTimer;
	Bool bIsStaticEditorMode;
	Bool bIsStaticPopupMode;
	Bool bIsFontSizeVariable;
	Bool bIsScrollAnimation;
	int nAutoResizeWidth;
	eGlsItemAlign eAlign;
	//cGlassInterface* otherObject;
	cGlsAssistant* assistant;
	int  nEnterCommand;
	Bool bIsMouseDownShow;
	//Bool bIsEnterCommand;

}sGlsEditBoxDetail;

typedef struct {
	float width;
	float height;
	wchar_t ch;
	color_t colorFont;
	color_t colorBg;
	Font* font;
	Bitmap* bmp;
} sGlsEditBoxCharElement;

enum eGlsTextImeMode {
	eGTIM_IME_ON,
	eGTIM_IME_OFF,
	eGTIM_IME_ALWAYS_OFF
};

#define xTEXT_MARGINE 2

class cglsEditBox;
xEXPORT cglsEditBox* xcglsEditBox();

class  xEXPORT cglsEditBox : public cGlassInterface, cglsDrawObject
{
public:
	cglsEditBox(void);
	virtual ~cglsEditBox(void);

	// --------------------------------
	static void xDefaultDetail(sGlsEditBoxDetail* detail);

	// -------------------------------
	Bool Create(HWND hWnd, int id, int x, int y, int w, int h, sGlsEditBoxDetail* detail=0, const wchar_t* pwszInitialComment=0);

	// -------------------------------
	int GetText(wchar_t* text, int nBufferSize);

	// -------------------------------
	int GetText(char* text, int nBufferSize);

	// -------------------------------
	int SetContextText(wchar_t* text);

	// -------------------------------
	//int GetText(char* text, int nBufferSize);

	// -------------------------------
	Bool SetText(const char* text, int nAnimationSpeed=0, int term=0);

	// -------------------------------
	Bool SetText(const wchar_t* text, int nAnimationSpeed=0, int term=0);

	// -------------------------------
	Bool SetTextEx(const wchar_t* text, color_t colorFont, color_t colorBg, Font* font=0, int nAnimationSpeed=0, int term=0);

	// -------------------------------
	Bool AppendTextEx(const wchar_t* text, color_t colorFont, color_t colorBg, Font* font=0);

	// -------------------------------
	Bool ScrollToLine(int nLine);

	// -------------------------------
	void SetTopPos(int nLine);

	// -------------------------------
	cGlsAssistant* GetAssistant();

	// -------------------------------
	void SetPasswordMode(Bool bIsPasswordMode);

	// -------------------------------
	void SetNoImeMode(Bool bIsNoImeMode, char* filter=0);

	// -------------------------------
	void SetNumeric(Bool bIsNumeric);

	// -------------------------------
	// n==-1 –³ŒÀ
	void SetMaxLength(int n);


	// -----------------------------
	void EnableInput(Bool bIsEnable);

	// -----------------------------
	void SetImeMode(eGlsTextImeMode eMode);

	// -------------------------------
	//Bool SetText(char* text, int nBufferSize);

	int  m_nAutoResizeWidth;

	// -----------------------------
	virtual void Update(eGlsUpdateMode mode=eGUM_FORCED, eDrawItemStatus status=eDIS_NORMAL);
	

private:
	// -------------------------------
	Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// -------------------------------
	Bitmap* CreateCarot(Bool bIsSelection=False);

	// -------------------------------
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -------------------------------
	LRESULT OnImeChar(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -------------------------------
	LRESULT OnChar(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -------------------------------
	LRESULT OnSysKeyDown(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -------------------------------
	LRESULT OnSysKeyUp(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -------------------------------
	LRESULT OnStartComposition(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -------------------------------
	LRESULT OnEndComposition(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -------------------------------
	LRESULT OnComposition(WPARAM wParam, LPARAM lParam, Bool& handled);

	//// -------------------------------
	//LRESULT OnSetText(WPARAM wParam, LPARAM lParam, Bool& handled);

	//// -------------------------------
	//LRESULT OnGetText(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------
	virtual LRESULT OnMouseLButtonDown(WPARAM wp, LPARAM lp, BOOL& handled);

public:
	// -----------------------------
	Bool InsertString(int n, const wchar_t* pszString);

	// -----------------------------
	Bool InsertStringEx(int n, const wchar_t* pszString, color_t colorFont, color_t colorBg, Font* font=0);

	// -----------------------------
	Bool DeleteString(int from, int to);

	// -----------------------------
	Bool SetCurSel(int to);

	// -----------------------------
	Bool SetCarotPos(int pos);

private:
	// -----------------------------
	Bool UpdateTextForMultiline();

	// -----------------------------
	Bool UpdateText();

	// -----------------------------
	Bool IsSelectionArea(int pos);


	// -----------------------------
	int GetCoordiateToPos(int n);

	// -----------------------------
	int GetCoordiateToPos(int x, int y);

	// -----------------------------
	float GetPosToCoordiate(int n, float& height);

	// -----------------------------
	float GetCharWidth(wchar_t ch, float& h);

	// -----------------------------
	virtual void Update(Bitmap* bmp, RECT* rect=0, RECT* _childRect=0, int offsetX=0, int offsetY=0, Bool bIsLocalBufferUpdate=False);


	// -----------------------------
	virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam, BOOL& handled);

	// -----------------------------
	virtual LRESULT OnMouseLButtonUp(WPARAM wParam, LPARAM lParam, BOOL& handled);

	// -----------------------------
	virtual LRESULT OnMouseLButtonDblClick(WPARAM wParam, LPARAM lParam, BOOL& handled);

	// -----------------------------
	LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam, BOOL& handled);

	// -----------------------------
	LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam, BOOL& handled);

	// -----------------------------
	//virtual void Update(eGlsUpdateMode mode=eGUM_UPDATE, eDrawItemStatus status=eDIS_NORMAL);
	// -----------------------------
	//virtual LRESULT OnMouseEnter(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	int xGetNextLinePos(int pos);

	// ------------------------------------
	int xGetPrevLinePos(int pos);

	// ------------------------------------
	int xGetLineTop(int pos);

	// ------------------------------------
	int xGetLineEnd(int pos);

	// -----------------------------
	int GetBottomLineHeight(int height);

	// -----------------------------
	int GetTopLineHeight(int height);


	// ---------------------------------------
	LRESULT OnVScroll(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------
	LRESULT OnScroll(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ---------------------------------------
	LRESULT OnMouseWheel(WPARAM wp, LPARAM lp, BOOL& handled);

	// -----------------------------
	virtual void OnFontChanged();

// ------------------------------------
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnTimer, WM_TIMER)
	MESSAGE(OnImeChar, WM_IME_CHAR)
	MESSAGE(OnChar, WM_CHAR)
	MESSAGE(OnSysKeyDown, WM_KEYDOWN)
	MESSAGE(OnSysKeyUp, WM_KEYUP)
	MESSAGE(OnMouseLButtonDown, WM_LBUTTONDOWN)
	MESSAGE(OnStartComposition, WM_IME_STARTCOMPOSITION)
	MESSAGE(OnComposition, WM_IME_COMPOSITION)
	MESSAGE(OnEndComposition, WM_IME_ENDCOMPOSITION)
	MESSAGE(OnMouseMove, WM_MOUSEMOVE)
	MESSAGE(OnMouseLButtonUp, WM_LBUTTONUP)
	MESSAGE(OnMouseLButtonDblClick, WM_LBUTTONDBLCLK)
	MESSAGE(OnKillFocus, WM_KILLFOCUS)
	MESSAGE(OnSetFocus, WM_SETFOCUS)
	MESSAGE(OnVScroll, WM_VSCROLL)
	MESSAGE(OnScroll, WM_EDIT_SCROLL)
	MESSAGE(OnMouseWheel, WM_MOUSEWHEEL)
DEFINE_MESSAGES_END



	point_t m_posCarot;
	point_t m_posSelection;
	float m_posStart;

	Bitmap* m_bmpCarotSelection;
	Bitmap* m_bmpCarot;
	Bitmap* m_bmpBuffer;
	SolidBrush* m_brushBg;
	wchar_t* m_txt;
	//cyiList* m_listText;
	cyitList<sGlsEditBoxCharElement>* m_listText;

	int	m_nScrollHeight;
	int m_nCarotTimer;
	eGlsTextImeMode m_eImeMode;



	color_t m_colorBg;//=xdetail.colorBg;
	color_t m_colorBorder;//=xdetail.colorBorder;
	color_t m_colorFont;//=xdetail.colorFont;
	color_t m_colorSelection;//=xdetail.colorSelection;
	color_t m_colorSelectionFont;
	color_t m_colorStaicModeFont;
	color_t m_colorStaticModeBg;


	// ---------------------------
	Bool m_bIsMouseDownShow;

	// ---------------------------
	Bool m_bIsMultiLine;

	// ---------------------------
	int m_nEnterCommand;

	// ---------------------------
	Bool m_bIsUpdated;

	// ---------------------------
	Bool m_bIsCarotShow;

	// ---------------------------
	Bool m_bSelectionMode;

	// ---------------------------
	Bool m_bNowModeEditor;

	// ---------------------------
	Bool m_bIsStaticEditorMode;

	// ---------------------------
	Bool m_bIsFontSizeVariable;

	// ---------------------------
	Bool m_bIsPasswordMode;

	// ---------------------------
	Bool m_bIsNoImeMode;

	// ---------------------------
	Bool m_bIsHeightIsFontSize;


	// ---------------------------
	cyiList* m_listLineInfo;

	// ---------------------------
	//eGlsEditBoxLayout 
	eGlsItemAlign m_eLayout;

	// ---------------------------
	HandleAnit m_hScrollAnit;

	HIMC m_hImc;

	HandleShadow m_hShadow;
	
private:
	Bool xGetSelectedString(wchar_t* buffer);
	cGlsAssistant* m_assistant;
	cglsScrollBar* m_scroll;

	wchar_t m_szContextText[32];

	char* m_filter;

	int m_nScreenTop;
	int m_nScreenBottom;


	int  m_nMaxLength;
	int m_nPasswordWidth;

	unsigned int m_ctrlFlag;

	wchar_t* m_pwszInitialComment;

	Bool m_bIsNoInput;

	float m_fShowCharFactor;

};
