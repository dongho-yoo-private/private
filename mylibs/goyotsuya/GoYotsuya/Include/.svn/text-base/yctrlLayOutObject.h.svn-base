#pragma once

#include "yistd.h"
#include "yctrlbase.h"

// -------------------
enum eControlLayout
{
	eCL_VERTICAL = 0x01,
	eCL_HORIZENTAL=0x02,

	eCL_FROM_LEFT=0x10,
	eCL_FROM_RIGHT=0x20,
	eCL_FROM_TOP=0x30,
	eCL_FROM_BOTTOM=0x40
};

class cyctrlLayOutObject
{
public:
	cyctrlLayOutObject(void);
	~cyctrlLayOutObject(void);

	// --------------------
	void Create(HWND hParent, int x, int y, int w, int h, eControlLayout eLayOut=eCL_FROM_LEFT, Bitmap** ppbmpDoubleBuffer=0);

	// --------------------
	void CreateByWindow(HWND hParent, int x, int y, int w, int h, eControlLayout eLayOut=eCL_FROM_LEFT, Bitmap** ppbmpDoubleBuffer=0);

	// --------------------
	void Show();
	
	// --------------------
	void Hide();

	// --------------------
	void Move();

	// --------------------
	int AddControl(cyctrlbase* ctrl, int x, int y);

	// --------------------
	int AddControl(HWND hWnd, int x, int y);

	// --------------------
	void SetScale(float scale);

};
