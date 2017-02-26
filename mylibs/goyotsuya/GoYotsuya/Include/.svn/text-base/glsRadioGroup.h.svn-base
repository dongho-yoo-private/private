#pragma once

#include "glsStackPannel.h"


// -----------------------------
typedef struct 
{
	sGlsDetail gls;
	color_t colorRadio;
	color_t colorRadioBorder;
	color_t colorText;
	Bool	bIsAutoArrange;
	int		nDistance;
} sRadioGroupDetail;

class cglsCheckGroup;

// -----------------------------
class xEXPORT cglsRadioGroup : public cglsStackPannel
{
public:
	cglsRadioGroup(void);
	~cglsRadioGroup(void);

	// -----------------------------
	static void xDefaultDetail(sRadioGroupDetail* detail);

	// -----------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, sRadioGroupDetail* detail=0);

	// -----------------------------
	Bool AddItem(int id, const wchar_t* pszText);

	// -----------------------------
	virtual void SetChecked(int index);

	// -----------------------------
	void SetCheckedById(int id);

	// -----------------------------
	int GetChecked();

	// -----------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

private:
	Bool bIsAutoArrange;
	color_t m_colorRadio;
	color_t m_colorText;
	color_t m_colorRadioBorder;
	int m_nDistance;
	int m_checkedIndex;
	Bool m_bIsTypeCheck;

	friend class cglsCheckGroup;

};


// -----------------------------
class xglsRadioButton : public cGlassInterface
{
public:
	xglsRadioButton();
	virtual ~xglsRadioButton();

	// -----------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, const wchar_t* text, color_t colorRadio, color_t colorText, color_t colorRadioBorder, sGlsDetail* detail=0);

	// -----------------------------
	void SetCheck(Bool bIsChecked);

	// -----------------------------
	Bool GetCheck();


protected:
	// -----------------------------
	virtual void Update(Bitmap* bmp, RECT* rect=0, RECT* _childRect=0, int offsetX=0, int offsetY=0, Bool bIsLocalBufferUpdate=False);

	virtual int xglsRadioButton::GetStatusToBitmapIndex(sDrawItem* item, eDrawItemStatus status, Bool& bIsNoAnimation);

	// -----------------------------
	wchar_t m_szText[256];

	// -----------------------------
	Bitmap* m_bmpText;

	// -----------------------------
	Bitmap* m_bmpCover;

private:
	color_t m_colorRadio;
	color_t m_colorText;
	color_t m_colorRadioBorder;

	Bool m_bIsChecked;

	Bitmap* m_bg;

	int m_nRadioSize;

	Bool m_bIsTypeCheck;

	friend class cglsRadioGroup;

};