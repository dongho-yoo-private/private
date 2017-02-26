#ifndef __yiencoex_h__
#define __yiencoex_h__
#include "yi.h"

#ifdef __cplusplus
#define EXTERN extern "C" 
#else
#define EXTERN extern 
#endif

// --------------------------------
//EXTERN void* yitoheap_s(void* buffer, int size, unsigned long security);

// --------------------------------
EXTERN void* yiencodex(void* buffer, int size, unsigned long security);

// --------------------------------
EXTERN void* yidecodex(void* buffer, int size, unsigned long security);

// --------------------------------
EXTERN void* yiencode(void* buffer, int size, unsigned long security);

// --------------------------------
EXTERN void* yidecode(void* buffer, int size, unsigned long security);

// --------------------------------
EXTERN unsigned int yichecksum(void* buffer, int size, int check_sum_interval);
/*
// --------------------------------
EXTERN char* yiToBase64(const void* buffer, int size, int* changedSize);

// --------------------------------
EXTERN void* yiToPlain(const char* buffer, int* changedSize);
*/
// --------------------------------
EXTERN Binary yiToPlain(const char* buffer);

// --------------------------------
EXTERN Binary yiToPlainForced(const char* buffer);

// --------------------------------
EXTERN char* yiToBase64(const void* buffer, int size, int* changedSize);

#endif
