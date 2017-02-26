#ifndef __yi_alloc_h__
#define __yi_alloc_h__

#include "yi.h"

// ------------------------------
typedef char* strz;

#define xSTR(str) &str[2]
#define xSTRLEN(str) *((unsigned short*)str) 


// ----------------------------------------
typedef char* string_t;


// ----------------------------------------
EXTERN_C ubit32_t yitohash(const char* pszStr, int len);

// ----------------------------------------
EXTERN_C string_t yitostrx(const char* pszStr);

// ----------------------------------
EXTERN_C char16_t* yiAsciiToUnicode(const char* pszString);

// ----------------------------------
EXTERN_C int FASTCALL  yiallocSize(void* p);

// ----------------------------------
EXTERN_C void FASTCALL  yifreeex(void* p);

// ----------------------------------
EXTERN_C void* FASTCALL  yiallocexi(int n);

// ----------------------------------
EXTERN_C void* FASTCALL  yiallocex(int n);

// ------------------------------
//EXTERN_C void* yialloc(int n);
#define yialloc malloc

// ------------------------------
EXTERN_C void* FASTCALL yialloci(int n);

// ------------------------------
EXTERN_C void* FASTCALL yirealloc(void* p, int n);

// ------------------------------
EXTERN_C void* FASTCALL yitoheap(void* buffer, int n);

// ------------------------------
EXTERN_C void* FASTCALL yitoheap_s(void* buffer, int n);

// ------------------------------
EXTERN_C void* FASTCALL yitoheapi(void* buffer, int n);

// ------------------------------
EXTERN_C void yifree(void* p);

// ------------------------------
EXTERN_C void* FASTCALL yimemcpy(void* dest, const void* src, int n);

// ------------------------------
EXTERN_C void* FASTCALL yimemzero(void* dest, int n);

// ------------------------------
EXTERN_C long FASTCALL yimemcmp(const void* dest, const void* mem2, int n);

// ------------------------------
EXTERN_C char* FASTCALL yistrlist(const char* str, ...);

// ------------------------------
EXTERN_C int FASTCALL yistrlen(const char* str);

// ------------------------------
//EXTERN_C char* FASTCALL yistrcpy(char* dest, const char* src);
// ------------------------------------
EXTERN_C char* yistrcpy(char* dest, const char* src, const char* until);


// ------------------------------
EXTERN_C int yistrlenUntil(const char* src, const char* untilz);

// ------------------------------
EXTERN_C char* yistrcpyUntil(char* dest, const char* src, const char* untilz);

// ------------------------------
EXTERN_C char* FASTCALL yistrncpy(char* dest, const char* src, int n);

// ------------------------------
EXTERN_C long FASTCALL yistrcmp(const char* str1, const char* str2);

// ------------------------------
EXTERN_C long FASTCALL yistrncmp(const char* str1, const char* str2, int n);

// ------------------------------
EXTERN_C void FASTCALL yistr(const char* str1);

// ------------------------------
EXTERN_C void FASTCALL yistrn(const char* str1, int len);

// -----------------------------
EXTERN_C char* FASTCALL yistricpy(char* dest, const char* from, const char* end);

// ------------------------------
EXTERN_C char* FASTCALL yistrcpych(char* dest, const char* src, char ch);

// ------------------------------
EXTERN_C int FASTCALL yiatoh(char* HexsaCode);

// ------------------------------
EXTERN_C char* FASTCALL yistrnheap(const char* str, int n);

// ------------------------------
EXTERN_C char* FASTCALL yistrheap(const char* str);

// ------------------------------
EXTERN_C char* FASTCALL yistrSub(const char* str, int nStart, int nEnd);

// ------------------------------
EXTERN_C char* FASTCALL yistrGetLine(const char* str, int n, char** next);

// ------------------------------
EXTERN_C char* FASTCALL yistrTrim(const char* str, Bool bSpaceOnly);

// ------------------------------
EXTERN_C char* yistrTrimEx(char* dest, const char* src, const char* trimz);

// ------------------------------
EXTERN_C char** FASTCALL yistrSplit(const char* str, const char* spliter, int* cnt);

// ------------------------------
EXTERN_C strz yistrSplitEx(const char* src, const char* untilz);

// ------------------------------
EXTERN_C char* FASTCALL yistrGetBetween(const char* str, const char* start, const char* end, char** next);

// ------------------------------
EXTERN_C void yistrFree(char* str);

// ------------------------------
EXTERN_C void yistrSplitFree(char** str);

// ------------------------------
EXTERN_C char* yistrchcpy(char* dest, const char* src, char ch);



// ------------------------------
typedef wchar_t* wstrz;

// ------------------------------
EXTERN_C int yistrcmpr(const char* org, const char* str);

// ------------------------------
EXTERN_C int yistrncmpr(const char* org, const char* str, int n);

// ------------------------------
EXTERN_C strz yistrz(strz dest, ...);

// ------------------------------
EXTERN_C const char* yistrzGet(const strz dest, int index, char spliter);

// ------------------------------
EXTERN_C int yistrzCount(const strz dest);

// ------------------------------
EXTERN_C strz* yistrzp(int cnt);

// ------------------------------
EXTERN_C void yistrzp_del(strz* str);

// ------------------------------
#define yistrzpSet(p, n, txt) p[n]=txt

#define yistrzpCount(p) (int)(p[-1])

#define yistrzpGet(p, n) p[n]

typedef Handle HandlePipe;

// ------------------------------
EXTERN_C Bool FASTCALL yipipeCreate(HandlePipe* hrPipe, HandlePipe* hwPipe);

// ------------------------------
EXTERN_C Bool FASTCALL yipipeClose(HandlePipe hrPipe, HandlePipe hwPipe);

// ------------------------------
EXTERN_C int FASTCALL yipipeWrite(HandlePipe hwPipe, void* buffer, int n);

// ------------------------------
EXTERN_C int FASTCALL yipipePut(HandlePipe hwPipe, char n);

// ------------------------------
EXTERN_C int FASTCALL yipipeGet(HandlePipe hwPipe, int nTimeOut);

// ------------------------------
EXTERN_C int FASTCALL yipipeRead(HandlePipe hrPipe, void* buffer, int n);

// -------------------------------
EXTERN_C int FASTCALL yipipeRead(HandlePipe hrPipe, void* buffer, int n);

// -------------------------------
EXTERN_C Binary yiBinary(void* p, int size, ...);

// -------------------------------
EXTERN_C void yiBinaryDelete(Binary p);

// ----------------------------
EXTERN_C ubit32_t yirand32(Bool bIsInitial);

// ------------------------------
EXTERN_C Bool yiatof(void* buffer, float* n);

// ------------------------------
EXTERN_C Bool yiatoi(const char* buffer, int* n);

// ------------------------------
enum eCharacterSet {
	eCS_JAPANESE,
	eCS_KOREAN,
	eCS_ENGLISH,
	eCS_CHINESE,
	eCS_UTF8,
	eCS_SYSTEM=eCS_JAPANESE,
	
};

#include "yitrace.h"
#include "yitime.h"
#include "yimutex.h"
#include "yifile.h"
#include "yisock.h"
#include "yiProfile.h"

#ifdef __cplusplus
#include "yiList.h"
#include "yiQueue.h"
#include "yiStack.h"
#endif

#endif
