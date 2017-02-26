#pragma once

#include "glsStackPannel.h"
#include "glsButton.h"

// -----------------------------------------
enum eGobanControlSet {
	eGCS_TOP = 1,
	eGCS_PP = 1<<1,
	eGCS_PREV = 1<<2,
	eGCS_NEXT = 1<<3,
	eGCS_NN = 1<<4,
	eGCS_END = 1<<5,
	eGCS_SHOW_NO = 1<<6,
	eGCS_NEXT_BRANCH=1<<7,
	eGCS_PREV_BRANCH=1<<8,
	eGCS_LABEL=1<<9,
	eGCS_ADDSTONE= 1<<10,
	eGCS_AUTOPLAY= 1<<11,
	eGCS_DELETE_NODE=1<<12,
	eCGS_CALCULATE_END=1<<13,
	eCGS_CALCULATE_STOP=1<<14,
	eGCS_PASS=1<<15,
	eGCS_WAIT=1<<16,
	eGCS_HUU=1<<17,
	eGCS_PAUSE=1<<18,
	eGCS_CANCEL=1<<19,
	eGCS_CLOSE=1<<21,
	eGCS_GIVEUP=1<<22,
	eGCS_STUDY_END=1<<23,
	eGCS_ONE_MORE_TIME=1<<24,
	eGCS_COPY=1<<25,
	eGCS_SIMPLE = (eGCS_TOP|eGCS_PP|eGCS_PREV|eGCS_NEXT|eGCS_NN|eGCS_END|eGCS_AUTOPLAY|eGCS_SHOW_NO),
	eGCS_NORMAL_EDITOR = (eGCS_SIMPLE | eGCS_NEXT_BRANCH | eGCS_PREV_BRANCH | eGCS_LABEL | eGCS_ADDSTONE | eGCS_DELETE_NODE |eGCS_PASS),
	eGCS_CALCULATE_MODE = (eCGS_CALCULATE_END|eCGS_CALCULATE_STOP),
	eGCS_PLAY	= (eGCS_PASS|eGCS_GIVEUP|eGCS_WAIT|eGCS_HUU|eGCS_PAUSE|eGCS_CANCEL),
	eGCS_STUDY	= (eGCS_TOP|eGCS_PP|eGCS_PREV|eGCS_NEXT|eGCS_NN|eGCS_END|eGCS_STUDY_END|eGCS_ONE_MORE_TIME),
	eGCS_WATCH  = (eGCS_CLOSE|eGCS_COPY),
	eGCS_SUPER_SIMPLE=(eGCS_TOP|eGCS_PP|eGCS_PREV|eGCS_NEXT|eGCS_NN|eGCS_END)
};


// -----------------------------------------
typedef struct {
	sGlsButtonDetail btnStyle;
	int nJumpCnt;
	Font* font;
	//eGobanControlLayOut eLayout;
} sGlsGobanControlDetail;

// -----------------------------------------
class cglsGobanControl : public cglsStackPannel
{
public:
	cglsGobanControl(void);
	~cglsGobanControl(void);

	// -----------------------------------------
	Bool Create(HWND hWnd, int id, float scale, int x, int y, eGobanControlSet set=eGCS_SIMPLE, sGlsGobanControlDetail* detail=0);

	// -----------------------------------------
	Bool ChangeControlSet(eGobanControlSet set);

	// -----------------------------------------
	Bool SetScale(float scale);

private:
	// -----------------------------------------
	void xCreateButton(int id, int bitmapId, wchar_t* pszText, wchar_t* comment, int systemId=0);

	// -----------------------------------------
	void CreateButtonz(float scale);

	// -----------------------------------------
	eGobanControlSet m_eControlSet;

	float m_scale;
};
