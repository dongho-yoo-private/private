#pragma once

#include "glsStackPannel.h"



typedef struct xsGlsListBoxDetail {
	sGlsStackPannelDetail stack;
	color_t colorFont1;
	color_t colorFont2;
	color_t colorBg1;
	color_t colorBg2;
	color_t colorCheck;
	color_t colorSelection;
	color_t colorSelectionBg;
	Bool bIsMultiSelect;
	Bool bIsAutoSort;
	Bool bIsSelectionCheck;
	Bool bIsMouseEnterNotify;
	Bool bIsNoClicked;
	Bool bIsListView;
	Bool bIsMsgLikeMenu;
} sGlsListBoxDetail;

typedef struct {
	int width;
	eGlsItemAlign align;
} sColumnInfo;

typedef struct {
	Bitmap* bmp;
	wchar_t* pszText;
} sListViewItem;

enum eGlsContentsType {
	eGCT_TEXT, /* wchar_t* */
	eGCT_STRING, /* char* */
	eGCT_NUMERIC /*Integer*/
};

class sListViewColumn {
public:
	sListViewColumn(int width, const wchar_t* text, eGlsItemAlign align=eGIA_LEFT, eGlsContentsType type=eGCT_TEXT)
	{
		m_width=width;
		m_align=align;
		m_type=type;
		m_pszText=(wchar_t*)text;
	}

	int m_width;
	wchar_t* m_pszText;
	eGlsItemAlign m_align;
	eGlsContentsType m_type;

};


// ------------------------------------------------------------
class  xEXPORT  cglsListBox : public cglsStackPannel, public cglsDrawObject
{
public:
	cglsListBox(void);
	~cglsListBox(void);

	static void xDefaultDetail(sGlsListBoxDetail* detail);

	// ------------------------------------------------------------
	static int MeasureItemWidth(wchar_t* pszText, Font* font, Bool bIsChecked, int* height);

	// ------------------------------------------------------------
	Bool Create(HWND hWnd, int id, int x, int y, int w, int h, sGlsListBoxDetail* detail=0);

	// ------------------------------------------------------------
	Bool AddItem(int id, Bitmap* bmp, const wchar_t* pszString, color_t colorFont=0, void* param=0);

	// ------------------------------------------------------------
	Bool AddItemDetail(int id, Bitmap* bmp, const wchar_t* pszString, color_t color, void* param=0);

	// ------------------------------------------------------------
	Bool AddItem(int id, Bitmap* bmp, const char* pszString, color_t colorFont=0, void* param=0);

	// ------------------------------------------------------------
	Bool InsertItem(int id, int index, Bitmap* bmp, const wchar_t* pszString, color_t colorFont=0, void* param=0);

	// ------------------------------------------------------------
	//Bool AddItemEx(int id, Bitmap* bmp, color_t colorFont, const wchar_t* pszString, ...);

	// ------------------------------------------------------------
	Bool AddItemEx(int id, sListViewItem* list, color_t colorFont);

	// ------------------------------------------------------------
	Bool InsertItemEx(int id, int index, Bitmap* bmp, const wchar_t* pszString, ...);

	// ------------------------------------------------------------
	Bool InsertItemEx(int id, int index, sListViewItem* list, color_t colorFont=Color::Black);

	// ------------------------------------------------------------
	Bool UpdateItemEx(int id, int index, sListViewItem* list, color_t colorFont=Color::Black);

	// ------------------------------------------------------------
	Bool SetColumn(int nColumnCount, int n, ...);

	// ------------------------------------------------------------
	Bool SetColumn(int nColumnCount, sListViewColumn* column);

	// ------------------------------------------------------------
	Bool AddItem(cGlassInterface* object, void* param=0);

	// ------------------------------------------------------------
	Bool InsertItem(int index, cGlassInterface* object, void* param=0);


	// ------------------------------------------------------------
	int GetSelectedIndex();

	// ------------------------------------------------------------
	int SelectItem(sDrawItem* item);

	// ------------------------------------------------------------
	int SelectItem(int index, int id, Bool bIsSelected);

	// ------------------------------------------------------------
	int GetSelectedItem(wchar_t* pszText=0);

	// ------------------------------------------------------------
	void* GetItemParam(int index);

	// ------------------------------------------------------------
	void* GetSelectedItemParam();

	// ------------------------------------------------------------
	int SelectItem(int index);

	// ------------------------------------------------------------
	Bool Sort(Bool bOrderRight=True);

	// ---------------------------------------
	virtual void Update(eGlsUpdateMode mode = eGUM_UPDATE, eDrawItemStatus status = eDIS_NORMAL);

	// ---------------------------------------
	virtual int SetFont(Font* font, Bool bIsReference=1);

	// ---------------------------------------
	Bool ReSizeByItemz();

	// -------------------------------
	virtual void Clear();



protected:
	// --------------------------
	Bitmap* MakeListViewImage(sDrawItem* item, Bool reverse=False);

	// --------------------------
	Bool OnNullBitmapDrawListView(sDrawItem* item, void* pExtra);

	// --------------------------
	virtual Bool OnNullBitmapDraw(sDrawItem* item, void* pExtra=0);

	// --------------------------
	virtual Bool OnSelfBgDraw(Bitmap* bmp, RECT* rect, RECT* rectChild);

	// --------------------------
	virtual Bool OnSelfFgDraw(Bitmap* bmp, RECT* rect, RECT* rectChild);

	// --------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// ------------------------------------
	virtual int GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation, Bitmap** ret=0);

	// ------------------------------------
	virtual LRESULT OnNotHandledCommand(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual void xCalculatePosition(RECT* rectAll, Bool bIsLastItemOnly=False);

	// ---------------------------------------
	virtual LRESULT OnMouseLButtonDown(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ------------------------------------
	int GetStatusToBitmapIndexForMulti(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation, Bitmap** ret=0);

	// -----------------------------
	virtual Bool xOnItemMouseDown(sDrawItem* item);

	// -----------------------------
	virtual Bool xOnItemMouseUp(sDrawItem* item);

	// -----------------------------
	virtual Bool xOnItemMouseMove(sDrawItem* item);

private:
	unsigned int m_ctrlFlag;

	// --------------------------
	Bitmap* m_bmpListBg;

	// --------------------------
	Bitmap* m_bmpCover;

	// --------------------------
	Bitmap* m_bmpCheck;

	// --------------------------
	int m_nColumn;

	// --------------------------
	sListViewColumn* m_columns;

	int m_nBeforeFontHeight;

	color_t m_colorFont1;
	color_t m_colorFont2;
	color_t m_colorBg1;
	color_t m_colorBg2;
	color_t m_colorSelection;
	color_t m_colorSelectionBg;
	color_t m_colorCheck;

	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnMouseLButtonDown, WM_LBUTTONDOWN) 
	DEFINE_MESSAGES_END

};
