#pragma once

#include "glsStackPannel.h"
#include "glsDrawObject.h"
#include "glsListBox.h"





// -------------------------------
class __declspec(dllexport) cglsListView : public cGlassInterface, public cglsDrawObject
{
public:
	cglsListView(void);
	~cglsListView(void);


	// -------------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, int w, int h, Font* font=0);

	// -------------------------------
	Bool CreateColumn(int n, sListViewColumn* column, ...);//int width, ...);

	// -------------------------------
	Bool AddItem(int id, color_t colorFont, Bitmap* bmp, wchar_t* pwszItemText, ...);

	// -------------------------------
	Bool InsertItem(int id, int index, color_t colorFont, Bitmap* bmp, wchar_t* pwszItemText, ...);

	// -------------------------------
	Bool UpdateItem(int id, int index, color_t colorFont, Bitmap* bmp, wchar_t* pwszItemText, ...);

	// -------------------------------
	Bool Sort(int index, Bool bIsUpper);

	// -------------------------------
	virtual void Clear();

	// -------------------------------
	int GetSelectedIndex();

	// -------------------------------
	int GetSelectedId();

	// -------------------------------
	int SelectIndex(int index);

	// -------------------------------
	virtual int SetFont(Font* font, Bool bIsReference=1);

private:
	// --------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// --------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// -------------------------------
	cyiList* m_listRaws;

	// -------------------------------
	cglsStackPannel* m_pannelHeader;

	// -------------------------------
	cglsListBox* m_listbox;

	// -------------------------------
	Bool m_bIsUp;

	// -------------------------------
	int m_nSelectIndex;

	int m_nColumn;
	sListViewColumn* m_columns;
};
