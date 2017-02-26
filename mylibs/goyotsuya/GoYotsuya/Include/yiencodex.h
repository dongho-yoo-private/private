#ifndef __yiencoex_h__
#define __yiencoex_h__

#ifdef __cplusplus
#define EXTERN extern "C" __declspec(dllexport)
#else
#define EXTERN extern __declspec(dllexport)
#endif

#include "yi.h"

// --------------------------------
EXTERN unsigned int yichecksum(void* buffer, int size, int check_sum_interval);

// --------------------------------
EXTERN void* yiencodex(void* buffer, int size, unsigned long security);

// --------------------------------
EXTERN void* yidecodex(void* buffer, int size, unsigned long security);

// --------------------------------
EXTERN void* yiencode(void* buffer, int size, unsigned long security);

// --------------------------------
EXTERN void* yidecode(void* buffer, int size, unsigned long security);

// --------------------------------
EXTERN Binary yiToPlain(const char* buffer);

// --------------------------------
EXTERN Binary yiToPlainForced(const char* buffer);

// --------------------------------
EXTERN char* yiToBase64(const void* buffer, int size, int* changedSize);


#endif
