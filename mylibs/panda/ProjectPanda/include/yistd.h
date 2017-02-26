#ifndef __yi_alloc_h__
#define __yi_alloc_h__

#include "yi.h"

// ------------------------------
EXTERN_C void* FASTCALL yialloc(int n);

// ------------------------------
EXTERN_C void yifree(void* p);

// ------------------------------
EXTERN_C void* FASTCALL yimemcpy(void* dest, const void* src, int n);

// ------------------------------
EXTERN_C void* FASTCALL yimemzero(void* dest, int n);

// ------------------------------
EXTERN_C long FASTCALL yimemcmp(const void* dest, const void* mem2, int n);

// ------------------------------
EXTERN_C int FASTCALL yistrlen(const char* str);

// ------------------------------
EXTERN_C char* FASTCALL yistrcpy(char* dest, const char* src);

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

#include "yitrace.h"
#include "yitime.h"
#include "yimutex.h"
#include "yifile.h"

#endif
