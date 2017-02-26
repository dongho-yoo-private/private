#pragma once
#include "yctrlbase.h"

// ----------------------------------
enum eCtrlTableStyle
{
	eCTS_WITH_BRDER,
	eCTS_WITH_BORDER_TITLE
};



// ----------------------------------
typedef struct zsExtendedTableStyle {
	UINT w;
	UINT h;
	UINT row;
	UINT col;
	
	BOOL bDrawFrameBorder;
	UINT frame_border_color;
	UINT background_color;
	UINT def_col_size;
	UINT def_raw_size;
	float def_font_size;
	const wchar_t* def_font_name;

}*zsExtendedTableStyle ;




typedef struct sBaseCellInfo {
	int w;
	int h;
	int marginX;
	int margineY;
	eCtrlAlign align;
	UINT fontcolor;

} sBaseCellInfo;

// ----------------------------------
class yctrlTable : public cyctrlbase
{
public:
	yctrlTable(int row, int col, int w, int h);
	yctrlTable(int w, int h);
	~yctrlTable(void);

	int AddRecord(

	// -----------------------------------------
	BOOL Create(void* AnyClass, ...);


	// -----------------------------------------
	void Set(int row, int col, const char* txt);

	// -----------------------------------------
	void Set(int row, int col, const wchar_t* txt);

	// -----------------------------------------
	void Set(int row, int col, DrawObject* object);

	// -----------------------------------------
	void Set(int row, int col, yiList* objectlist);

	// ---------------------------------------
	virtual BOOL OnPaint(HDC hdc);

private:
	int m_row;
	int m_col;
	int m_margin;
	UINT m_basecolor;
	UINT m_bordercolor;
	DrawObject* object;
	UINT m_frmcolor;

};
