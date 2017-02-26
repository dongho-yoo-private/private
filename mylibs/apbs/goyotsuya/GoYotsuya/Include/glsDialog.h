#pragma once
//#include "glsWindow.h"
#include "GlassControlz.h"
#include "glsDrawObject.h"


enum eGlassInterfaceType {
	eGIT_BUTTON=1,
	eGIT_SPLIT_BUTTON,
	eGIT_LABEL,
	eGIT_RADIO,
	eGIT_CHECK,
	eGIT_LISTBOX,
	eGIT_EDIT_BOX,
	eGIT_PICTURE
};

enum eItemMargin {
	eIM_SMALL_X=0x01,
	eIM_X=0x02,
	eIM_LARGE_X=0x04,
	eIM_SMALL_Y=0x10,
	eIM_Y=0x20,
	eIM_LARGE_Y=0x40
};

typedef struct {
	int id;
	eGlassInterfaceType type;
	int bitmapId;
	const wchar_t* text;
	int posFrom;
	int position;
	unsigned int param1;
	unsigned int param2;
	int margin;
} sGlsDialogResources;

#define DEFINE_RESOURCE_BEGIN(a) \
	static sGlsDialogResources a[] = {

#define DEFINE_RESOURCE(id, type, text, posFrom, position, margin) \
{ id, type, 0, text, posFrom, position, 0, 0 , margin},

#define DEFINE_RESOURCE_PARAM(id, type, text, posFrom, position, param1, param2, margin) \
{ id, type, 0, text, posFrom, position, param1, param2 , margin},

#define DEFINE_SPILIT_BUTTON(id, texts, comments, posFrom, position, color, n, margin) \
{ id, eGIT_SPLIT_BUTTON, (int)comments, (const wchar_t*)texts, posFrom, position, color, n, margin},

#define DEFINE_EDIT_BOX(id, n, bIsNoIme, bIsNumeric, nMaxLength, posFrom, position, margin) \
{id, eGIT_EDIT_BOX, bIsNoIme, (const wchar_t*)bIsNumeric, posFrom, position, nMaxLength, n, margin},

#define DEFINE_RESOURCE_END() \
{ 0, (eGlassInterfaceType)0, 0, 0, 0, 0, 0, 0, 0} \
};


typedef struct xsGlsDialogDetail {
	sGlsGaussPannelDetail gauss;
	Bool bHasClosebutton;
	Bool bHasCaption;
	Bool bIsMovable;
	Font* font;
	Bool bIsAutoDestroy;
	Bool bIsAnimationShow;
	unsigned int nShowAnimationTime;
	unsigned int nHideAnimationTime;

} sGlsDialogDetail;


enum eGlsDialogItemAlign {
	eIP_LEFT=1,
	eIP_CENTER=2,
	eIP_RIGHT=4,
	eIP_VCENTER=0x10,
	eIP_TOP=0x20,
	eIP_BOTTOM=0x40
};

typedef struct {
	int xNo;
	int yNo;
	RECT rect;
	eGlsDialogItemAlign eAlign;
} sGlsDialogStackLayout;

enum eObjectLayout {
	eOL_ABSOLUTE_X=0x01,
	eOL_LAYOUT_X=0x02,
	eOL_ABSOLUTE_Y=0x10,
	eOL_LAYOUT_Y=0x20
};

enum eGlsDialogAlign {
	eGDA_LEFT=0x01,
	eGDA_CENTER=0x02,
	eGDA_RIGHT=0x04,
	eGA_TOP=0x10,
	eGA_VCENTER=0x20,
	eGA_BOTTOM=0x40
};

// --------------------------------
class xEXPORT  cglsDialog : public cglsGaussPannel, cglsDrawObject
{
public:
	cglsDialog(void);
	virtual ~cglsDialog(void);

	// --------------------------------
	static void xDefaultDetail(sGlsDialogDetail* detail);

	// -----------------------------------
	Bool Create(HWND hWndParent, int id, const wchar_t* pszCaption, int x, int y, int w, int h, sGlsDialogDetail* detail/*=0*/);

	// -----------------------------------
	int DoModal(HWND hWndParent, int id, const wchar_t* pszCaption, int x, int y, int w, int h, sGlsDialogDetail* detail/*=0*/);

	// --------------------------------
	virtual void Destroy();

	// -----------------------------------
	virtual void Show();

	// -----------------------------------
	virtual void Hide();

	// -----------------------------------
	virtual Bool IsButtonOnlyOK();

	// -----------------------------------
	LRESULT OnMouseMove(WPARAM wp, LPARAM lp, BOOL& handled);

	// ------------------------------------
	virtual int SetFont(Font* font);

	// ------------------------------------
	Bool CreateChildFromResource(sGlsDialogResources* resources);

	// ------------------------------------
	Bool ReArrangeItemz();


protected:
	// ------------------------------------
	virtual void OnReqDestroy();

	// -----------------------------
	virtual void OnShowAnimationEnd(Bool bIsShow, Bool bIsCanceled);


	// -----------------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// -----------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	//virtual Bool OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect);

	// ------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps=0);

	// ------------------------------------
	virtual void OnExitLoop();

	// ------------------------------------
	void GetContentsOffset(int&x, int&y);

	// ------------------------------------
	void SetContentsSize(int w, int h);

	// ------------------------------------
	void GetMaxWidthHeight(int from, int to, int&w, int& h, Bool bIsVisibleOnly=False);

	// ------------------------------------
	void ShowChildz(int from, int to, Bool bIsShow);

	// ------------------------------------
	//Bool ReArrangeItemz(RECT* rect);

	// ------------------------------------
	//Bool AddLayout(cGlassInterface* obj, int x, int y, POINT* pt=0);


	// ------------------------------------
	//virtual int OnSetLayout(int id, Bool bIsVertical);

	// ------------------------------------
	//virtual int GetArrangedLayoutY(int id);


	// ------------------------------------
	cglsStatic* CreateLabel(const wchar_t* str, sGlsStaticDetail* detail);


	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	virtual LRESULT OnSizeChanged(WPARAM wParam, LPARAM lParam, BOOL& handled);


	// ------------------------------------
	void UpdateButton(eGlsItemAlign align, int distance);

	// ------------------------------------
	void CreateButton(int id, eGlsItemAlign align=eGIA_CENTER, int distance=0);

	// ------------------------------------
	void SetButtonText(int id, const wchar_t* text);

	// ------------------------------------
	void EnableButton(int id, Bool bEnable);

	// ------------------------------------
	int AddButton(const wchar_t* name, int id);

	// ---------------------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ---------------------------------------------------
	//virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);


// ------------------------------------
DEFINE_MESSAGES_BEGIN
	MESSAGE(OnMouseMove, WM_MOUSEMOVE)
	MESSAGE(OnSizeChanged, WM_SIZE)
DEFINE_MESSAGES_END

private:
	void xOnMessageLoop();
	//cyiStaticList<sGlsChildLayout>* m_listItemPos;

	//int* m_nLayoutX;
	//int* m_nLayoutY;
	//int  m_nLayoutPoint[6];

protected:
	POINT m_ptItemDistance;
	Bool m_bIsExitLoop;

	int m_returnValue;

	int m_nContentsHeight;
	int m_nContentsWidth;
	Bool m_bIsSizeFixedMode;
	wchar_t m_szCaption[256];
	eGlsItemAlign m_eButtonAlign;

	float m_fMarginX;
	float m_fMarginY;

	int m_nButtonzId;
};
