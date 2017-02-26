#pragma once

#include <atlbase.h>
#include <gdiplus.h>
#include <windows.h>

#ifndef __GDI_PLUS__
using namespace Gdiplus;
#pragma comment(lib, "GdiPlus.lib") 
#define __GID_PLUS__
#endif

#include "yiBitmap.h"
#include "yctrlbase.h"
#include "yctrlButton.h"
#include "yctrlTab.h"
#include "yctrlToolTip.h"

#include "cyctrlResource.h"

// ---------------------------------------------
extern void xDrawDefaultButton(Bitmap* icon, const wchar_t* pszName, int id, int subkey);

// --------------------------------
extern Bool yictrl_initialize();

// --------------------------------
extern void yictrl_finalize();

#define WM_GOBAN_ADDED			(WM_USER + 277)
#define WM_CHANGE_DEAD_STONE	(WM_GOBAN_ADDED+1)
#define WM_NODE_CHANGED			(WM_CHANGE_DEAD_STONE+1)
#define WM_INITSTONE			(WM_NODE_CHANGED+1)
#define WM_LOCALE_CHANGED		(WM_INITSTONE+1)
#define WM_PUT_STONED			(WM_LOCALE_CHANGED+1)
#define WM_QUIT_OPTIMIZE		(WM_PUT_STONED+1)
#define WM_QUIT_SEARCHRESULT	(WM_QUIT_OPTIMIZE+1)