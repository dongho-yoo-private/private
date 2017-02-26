#pragma once

#include "yistd.h"
#include "GlassInterface.h"

enum eLayout {
	eL_LEFT=0x01,
	eL_CENTER=0x02,
	eL_RIGHT=0x04,
	eL_TOP=0x10,
	eL_VCENTER=0x20,
	eL_BOTTOM=0x40
};

class cglsLayoutObject
{
public:
	cglsLayoutObject(void);
	~cglsLayoutObject(void);

	// -----------------------------------
	Bool CreateLayout(cGlassInterface* main);

	// -----------------------------------
	Bool ReSize(int x, int y, int w, int h);

	// -----------------------------------
	int AddLayout(int id, RECT* rect);

	// -----------------------------------
	int AddLayout(int id, int x, int y);

	// -----------------------------------
	int AddLayout(int id, RECT* rect, eLayout layout=(eL_LEFT|eL_TOP));

	// -----------------------------------
	int SetLayout(int id, Rect* rect);

	// -----------------------------------
	Bool SetItem(cGlassInterface* item, int nLayoutId, eLayout layout=(eL_LEFT|eL_TOP));

	// -----------------------------------
	Bool Update();

private:
	cyiList m_listLayout;
	cyiList m_item;

	// -----------------------------------
	Bool xGetPos(int x, int y, int w, int h, RECT* allRect, eLayout elayout);

	// -----------------------------------
	Bool xMoveItem(cGlassInterface* item, int id, eLayout elayout);

	int m_x;
	int m_y;
	int m_w;
	int m_h;

	cGlassInterface* m_main;
};
