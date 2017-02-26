#pragma once
#include "GlassInterface.h"


enum eGlsControlItemAlign {
	eGCIA_LEFT=1,
	eGCIA_CENTER=2,
	eGCIA_RIGHT=4,
	eGCIA_VCENTER=0x10,
	eGCIA_TOP=0x20,
	eGCIA_BOTTOM=0x40
};

enum eGlsControlSystemPos {
	eGCSP_MARGIN=0x80000001,
	eGCSP_CENTER=0x80000002,
	eGCSP_RIGHT =0x80000003
};

enum eGlsControlItemMode {
	eGCIM_SIZE_FIXED=1,
	eGCIM_SIZE_DYNAMIC
};


typedef struct {
	eGlsControlItemMode mode;
	eGlsItemAlign		align;
	color_t				bgColor;
	Bool				bCommandNotify;
}sGlsControlDetail;

// ---------------------------------------------
#define MAX_POSITION_COUNT 16

class xEXPORT cglsControl : public cGlassInterface
{
public:
	cglsControl(void);
	~cglsControl(void);

	// ---------------------------------------------
	static void xDefaultDetail(sGlsControlDetail* detail);

	// ---------------------------------------------
	Bool Create(cGlassInterface* parent, HINSTANCE hInstance, int id, RECT* lpRect, sGlsControlDetail* detail);

	// ---------------------------------------------
	virtual void Destroy();

	// ---------------------------------------------
	virtual Bool ReSize(UINT x, UINT y, Bool bIsRepaint);

	// ---------------------------------------------
	// ���̊֐����Ăяo�����ƁA
	// 1. OnUpdateFont���Ăяo����A
	// 2. OnItemUpdate���Ăяo����A
	// 3. UpdateForced���Ăяo����A
	// 4. OnUpdatePosition���Ăяo�����B
	Bool ReArrangeItemz();

protected:
	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	virtual Bool OnCreate();

	// ------------------------------------
	virtual Bool OnUpdatePosition()=0;

	// ------------------------------------
	virtual Bool OnDefItemArrange()=0;

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);



	sGlsItemPosition m_posX[MAX_POSITION_COUNT];
	sGlsItemPosition m_posY[MAX_POSITION_COUNT];
private:
	sGlsControlDetail m_detail;
};



#define DEFINE_POSITION() \
Bool OnUpdatePosition()\
{
#define POSITION_X(id, x, align, from) \
if (id<MAX_POSITION_COUNT)\
	m_xz[id]={x, align, from};\
}

#define POSITION_Y(id, y) \
if (id<MAX_POSITION_COUNT)\
	m_yz[id]={x, align, from};\
}

#define DEFINE_END() \
return True;\
}

#define ITEM_ARRANGE() \
Bool OnDefItemArrange()\
{
#define ITEM_POS(id, id_x, id_y)\
{\
cGlassInterface* child = GetChild(id);\
if (child)\
{\
	if (id<MAX_POSITION_COUNT)\
		child->MoveByPosition(&m_xz[id_x], &m_yz[id_y]);\
}\
}

