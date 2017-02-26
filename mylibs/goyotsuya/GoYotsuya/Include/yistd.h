#ifndef __yi_alloc_h__
#define __yi_alloc_h__

#include "yi.h"

#define YOTSUYA_LOG_PORT 33553

// ----------------------------------
xEXTERN_C int yiGetSystemLocale(char* mozi);

// ------------------------------
xEXTERN_C void* FASTCALL yitoh(void* buffer, int n);

// ------------------------------
xEXTERN_C void* FASTCALL yialloc(int n);

// ------------------------------
xEXTERN_C Bool yiIsOverXP();

// ----------------------------------
xEXTERN_C void* FASTCALL  yirealloc(void* p, int n);

// ------------------------------
xEXTERN_C void* FASTCALL yialloci(int n);

// ----------------------------------
xEXTERN_C void* FASTCALL  yiallocix(int n);

// ----------------------
xEXTERN_C void yifreex(void* p);

// ------------------------------
xEXTERN_C void yifree(void* p);

// ------------------------------
#define xTOHEAP(a) yitoheap(a, sizeof(*(a)))

// ------------------------------
xEXTERN_C void* FASTCALL yitoheap(void* buffer, int size);

// ------------------------------
xEXTERN_C Bool FASTCALL yiatoi(void* buffer, int* n);

// ------------------------------
xEXTERN_C Bool FASTCALL yiatof(void* buffer, float* n);

// ----------------------------------------
xEXTERN_C char* yistrchcpy(char*dest, const char* src, char ch);

// ------------------------------
typedef void (*OnThreadCallback)(void* param);

// ------------------------------
typedef unsigned int (__stdcall *OnThreadCallbackx)(void* param);

typedef Handle HandleThread;

#define yiThreadStart yiStartThread

#define yiThreadStartx yiStartThreadx

// ------------------------------
xEXTERN_C  Bool yiStartThread(OnThreadCallback callback, void* param, int size);

// ------------------------------
xEXTERN_C Bool yiWaitThread(HandleThread hThread, int nTimeOut);

// ------------------------------
xEXTERN_C HandleThread yiStartThreadx(OnThreadCallbackx callbackx, void* param, int size);

// ------------------------------
xEXTERN_C void* FASTCALL yiallocx(int n);

// ------------------------------
xEXTERN_C void* FASTCALL yiallocix(int n);

// ------------------------------
xEXTERN_C void yifreex(void* p);

// ------------------------------
//xEXTERN_C void* FASTCALL yimemcpy(void* dest, const void* src, int n);
#define yimemcpy memcpy

// ------------------------------
//xEXTERN_C void* FASTCALL yimemzero(void* dest, int n);
#define yimemzero ZeroMemory

// ------------------------------
//xEXTERN_C long FASTCALL yimemcmp(const void* dest, const void* mem2, int n);
#define yimemcmp memcmp

// ------------------------------
//xEXTERN_C int FASTCALL yistrlen(const char* str);
#define yistrlen strlen

// ------------------------------
//xEXTERN_C char* FASTCALL yistrcpy(char* dest, const char* src);
#define yistrcpy strcpy

// ------------------------------
xEXTERN_C char* FASTCALL yistrncpy(char* dest, const char* src, int n);

// ------------------------------
xEXTERN_C long FASTCALL yistrcmp(const char* str1, const char* str2);

// ------------------------------
xEXTERN_C long FASTCALL yistrncmp(const char* str1, const char* str2, int n);

// ------------------------------
xEXTERN_C void FASTCALL yistr(const char* str1);

// ------------------------------
xEXTERN_C void FASTCALL yistrn(const char* str1, int len);

// -----------------------------
xEXTERN_C char* FASTCALL yistricpy(char* dest, const char* from, const char* end);

// ------------------------------
xEXTERN_C char* FASTCALL yistrcpych(char* dest, const char* src, char ch);

// ------------------------------
xEXTERN_C int FASTCALL yiatoh(char* HexsaCode);

// ------------------------------
xEXTERN_C char* FASTCALL yistrheap(const char* str);

// ----------------------
xEXTERN_C char* FASTCALL yistrheapex(const char* str, char ch);

// ----------------------
xEXTERN_C wchar_t* FASTCALL yistrheapW(const wchar_t* str);

typedef char* strz;
typedef wchar_t* wstrz;

// ------------------------------
xEXTERN_C strz yistrz(strz dest, ...);

// ------------------------------
xEXTERN_C const char* yistrzGet(const strz dest, int index, char spliter);

// ------------------------------
xEXTERN_C const char* yistrzCount(const strz dest);

// ------------------------------
xEXTERN_C strz* yistrzp(int cnt);

// ------------------------------
xEXTERN_C void yistrzp_del(strz* str);

// ------------------------------
#define yistrzpSet(p, n, txt) p[n]=txt

#define yistrzpCount(p) (int)(p[-1])

#define yistrzpGet(p, n) p[n]

// ------------------------------
xEXTERN_C char* yistrstrheap(const char* str, const char* eos/*, Bool bContainEos*/);

// ------------------------------
xEXTERN_C char* yistrBetween(const char* str, const char* start, const char* end, Bool bContainEos);

// ------------------------------
xEXTERN_C char* yistrBetweenEx(const char* str, const char* start, const char* end);

// ------------------------------
xEXTERN_C char* yistrTrim(char* dest, const char* src, const char* trimz);

// ------------------------------
xEXTERN_C char* yistrTrimIndex(const char* src, const char* trimz);


// ------------------------------
xEXTERN_C int yistrlenUntil(const char* src, const char* untilz);

// ------------------------------
xEXTERN_C char* yistrSplit(const char* src, const char* untilz);

// ------------------------------
xEXTERN_C char** yistrcpySplit(char* dest, const char* src, const char* untilz);

// ------------------------------
xEXTERN_C int yistrReplaceChar(char* str, char before, char after);

// ------------------------------
xEXTERN_C char* yistrReplaceStr(const char* str, const char* before, const char* after);



// ------------------------------
xEXTERN_C void yiLogInit();

// ------------------------------
xEXTERN_C void yiLogFin();

// ------------------------------
xEXTERN_C void yiTrace(const char* fmt, ...);

// ------------------------------
xEXTERN_C void yiTraceM(const char* fmt, ...);

// ------------------------------
xEXTERN_C void yiTraceM2(const char* pszFileName, const char* fmt, ...);

// ------------------------------
xEXTERN_C void yiTraceInit(int nPortNo);

// ------------------------------
xEXTERN_C void yiTraceM3(unsigned int level, const char* fmt, ...);


// ------------------------------
//xEXTERN_C void yiTraceInit();

#ifdef _DEBUG
#define xTRACE yiTraceM
#define xTRACEF yiTraceM2
//#define xERROR xTRACE
#else
#define xTRACE
//#define xERROR xTRACE
#endif

#define YIDEBUG(buffer, ...) yiTraceM3(1, buffer, __VA_ARGS__)
#define YIINFO(buffer, ...) yiTraceM3(2, buffer, __VA_ARGS__)
#define YIWARN(buffer, ...) yiTraceM3(3, buffer, __VA_ARGS__)
#define YIERROR(buffer, ...) yiTraceM3(4, buffer, __VA_ARGS__)

#define YITRACE yiTraceM3


// ------------------------------
enum eCharacterSet {

	eCS_JAPANESE,
	eCS_KOREAN,
	eCS_ENGLISH,
	eCS_CHINESE,
	eCS_UTF8,
	eCS_SYSTEM=eCS_JAPANESE,
	
};

#define RET_SIZE (char*)0xFFFFFFFF
// ------------------------------
xEXTERN_C char* yiToMultiByteCode(char* dest, const wchar_t* pszSrc, int set);

// ------------------------------
xEXTERN_C wchar_t* yiToUnicode(wchar_t* dest, const char* pszSrc, int set);

// ------------------------------
xEXTERN_C char* yiToSjis(char* txt);

// ------------------------------
xEXTERN_C char* yiToJis(char* txt);

// ------------------------------
xEXTERN_C char* yiToEuc(char* txt);


// ------------------------------
xEXTERN_C char* yiToUtf8(char* dest, const char* pszSrc, int set);

// ------------------------------
xEXTERN_C  char* yiUtf8ToMulti(char* dest, const char* pszSrc, int set);


// ------------------------------
xEXTERN_C  char* yiJisToSjis(char* sjis);

// ------------------------------
xEXTERN_C  char* yiSjisToJis(char* jis);


#include <stdio.h>

#include "yimutex.h"
#include "yiFile.h"

#ifdef __cplusplus
#include "yitime.h"
#include "yiList.h"
#include "yiStream.h"
#include "yiBitmap.h"
#include "yiQueue.h"
#include "yiStack.h"
#include "yiTreeNode.h"
#include "yiBitmapBlender.h"
#include "yishape.h"
#include "yiGoDefs.h"
//#include "yiThreadObject.h"
#include "yiUtil.h"
#include "yiAnimationTrigger.h"

#endif

#endif
