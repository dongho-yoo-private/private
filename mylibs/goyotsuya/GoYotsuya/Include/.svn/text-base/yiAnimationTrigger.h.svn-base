#ifndef __YIANIMATION_TRIGGER_H__
#define __YIANIMATION_TRIGGER_H__

#include "yi.h"

// ----------------------------------------------
enum eGlsAnimationPattern {
	eGAP_LINEAR,
	eGAP_FISO,
	eGAP_SIFO,
	eGAP_BLINK,
	eGAP_SCROLL,
	eGAP_POPUP,
	eGAP_POPO,
	eGAP_MAX_PATTERN
};

typedef Handle HandleAnit;

// --------------------------------------------------------------------
xEXPORT float yiAnitGetFactorF(float max, float index, eGlsAnimationPattern ePartern);

// --------------------------------------------------------------------
xEXPORT float yiAnitGetFactor(int max, int index, eGlsAnimationPattern ePartern);

// --------------------------------------------------------------------
xEXPORT HandleAnit yiAnitCreate();

// --------------------------------------------------------------------
xEXPORT void yiAnitDestroy(HandleAnit hAnimation);

// --------------------------------------------------------------------
xEXPORT Bool yiAnitStart(HandleAnit hAnimation, HWND hNotifyWnd, UINT msg, 
					int nAnimationTerm, int nAnimationInterval, eGlsAnimationPattern pattern, void* pUserParam);

// --------------------------------------------------------------------
xEXPORT Bool yiAnitIsAlived(HandleAnit hAnimation);

// --------------------------------------------------------------------
// flag = 0 : before
// flag = 1 : end
// flag = 2 : 
xEXPORT int yiAnitStop(HandleAnit hAnimation, Bool bIsWait);

// --------------------------------------------------------------------
xEXPORT void yiAnitWait(HandleAnit hAnimation);

// --------------------------------------------------------------------
xEXPORT void yiAnitClear(HandleAnit hAnimation);

// --------------------------------------------------------------------
xEXPORT Bool yiAnitIsCleared(HandleAnit hAnimation, float* fAbortFactor);

// --------------------------------------------------------------------
xEXPORT float yiAnitGetFactor(HandleAnit hAnimation, int i);

// --------------------------------------------------------------------
xEXPORT float yiAnitGetFactorEx(HandleAnit hAnimation, int i, eGlsAnimationPattern pattern);

// --------------------------------------------------------------------
xEXPORT void* yiAnitGetParam(HandleAnit hAnimation);

// --------------------------------------------------------------------
xEXPORT unsigned int yiAnitGetId(HandleAnit hAnimation);

// --------------------------------------------------------------------
xEXPORT int yiAnitGetDiffSec(HandleAnit hAnimation);

#endif
