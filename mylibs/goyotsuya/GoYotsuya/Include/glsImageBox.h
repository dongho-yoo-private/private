#pragma once
#include "yi.h"
#include "GlassInterface.h"

enum eGlsImageBoxViewStyle {
	eGIBVS_NONE,	// 左上に表示（スクロール機能？）
	eGIBVS_CENTER,	// 真中に置く。
	eGIBVS_FIX,		// 伸ばす
	eGIBVS_TILE,	// タイル
	eGIBVS_TRIM,	// トリム
	eGIBVS_TRIM_BORDER_FIXED,	// 枠をイメージに合わす（サイズ固定)
	eGIBVS_TRIM2,				// イメージで枠に合わす。
	eGIBVS_FULL					// イメージをフルに表示（サイズ変更）

};

typedef struct {
	sGlsDetail gls;
	eGlsImageBoxViewStyle eInitialStyle;
	Bool bIsClickEvent;
	Bool bIsDragAndDrop;
	Bool bIsBorderDropShadow;
	Bool bHasBorder;
	Bool bAutoResize;
	Pen* pen;
	Bool bIsImageListMode;
	Bool bIsMouseParent;
} sGlsImageBoxDetail;

class xEXPORT cglsImageBox : public cGlassInterface
{
public:
	cglsImageBox(void);
	virtual ~cglsImageBox(void);

	// ----------------------------------
	static void xDefaultDetail(sGlsImageBoxDetail* detail);

	// ----------------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, int w, int h, Bitmap* bmp, sGlsImageBoxDetail* detail=0);

	// ----------------------------------
	void SetImage(const char* pszImagePath, eGlsImageBoxViewStyle style);

	// ----------------------------------
	void SetImage(wchar_t* pwszImagePath, eGlsImageBoxViewStyle style);

	// ----------------------------------
	void SetImage(Bitmap* bmp, eGlsImageBoxViewStyle style);

	// ----------------------------------
	void SetImage(void* raw, int size, eGlsImageBoxViewStyle style);

	// ----------------------------------
	void Clear();

	// ----------------------------------
	virtual Bool ReSize(UINT x, UINT y, Bool bIsRepaint=0);

	// ----------------------------------
	virtual void Destroy();

	// ----------------------------------
	void AppendBitmap(int id, Bitmap* bmp, Bool bDelete=False);

	// ----------------------------------
	Bitmap* GetBitmap(int id);

	// ----------------------------------
	Bitmap* SetBitmap(int id, Bitmap* bmp, Bool bDelete=False);

	// ----------------------------------
	void SelectBitmap(int id, Bool bDrawDirect=True, Bool bIsForced=False);

	// ----------------------------------
	int GetSelectedBitmap();

	// ----------------------------------
	void ClearBitmaps();

	// ----------------------------------
	Bitmap* ExportBitmap(int w, int h);
protected:
	// ----------------------------------
	LRESULT OnDropFiles(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ----------------------------------
	Bitmap* CreateImage(Bitmap* dest, Bitmap* bmp, int _w, int _h, eGlsImageBoxViewStyle style);

	// ----------------------------------
	LRESULT OnMouse(int id, WPARAM wParam, LPARAM lParam, Bool& handled);

	// ----------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
private:
	// ----------------------------------
	void xDrawDragAndDrop();

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnDropFiles, WM_DROPFILES)
	MESSAGEEX(OnMouse, WM_MOUSEMOVE)
	MESSAGEEX(OnMouse, WM_LBUTTONDOWN)
	MESSAGEEX(OnMouse, WM_LBUTTONUP)
	MESSAGEEX(OnMouse, WM_RBUTTONDOWN)
	MESSAGEEX(OnMouse, WM_RBUTTONUP)
DEFINE_MESSAGES_END

	wchar_t m_szImagePath[256];

	unsigned int m_myFlag;

	eGlsImageBoxViewStyle m_eStyle;
	Pen* m_pen;

	cyiList* m_listBitmaps;

	int m_nCurrentSelectedId;

};
