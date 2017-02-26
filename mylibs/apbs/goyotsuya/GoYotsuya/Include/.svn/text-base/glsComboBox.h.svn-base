#pragma once

#include "yctrlbase.h"
#include "glsStackPannel.h"
#include "glsDrawObject.h"
#include "glsButton.h"
#include "glsEditBox.h"

typedef struct  {
	sGlsButtonDetail btn;
	sGlsEditBoxDetail edit;
} sGlsComboBoxDetail;

class xEXPORT cglsComboBox : public cglsStackPannel, cglsDrawObject
{
public:
	cglsComboBox(void);
	~cglsComboBox(void);

	// ------------------------------------
	Bool Create(HWND hWndParent, int id, int x, int y, int w, int h);

	// ------------------------------------
	static void xDefaultDetail(sGlsComboBoxDetail* detail);

	// ------------------------------------
	Bool Create(HWND hWndParent, int id, int x, int y, int w, int h, sGlsComboBoxDetail* detail);

	// ------------------------------------
	Bool AddString(const char* pszStr);

	// ------------------------------------
	Bool SetSelectIndex(int n);

	// ------------------------------------
	int GetSelectIndex();

private:
	// ------------------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* param=0);

	cglsEditBox* m_edit;

	color_t m_colorBtn;
};
