#ifndef __yi_psr_h__
#define __yi_psr_h__

#include "yi.h"
#include "yiList.h"


// -------------------------------
extern Bool FASTCALL yiPsrSplit(const char* p, const char* spliter, cyiList& list);

// -------------------------------
extern char* FASTCALL yiPsrUntilCpy(char* dest, const char* src, const char* until);

// -------------------------------
extern char* FASTCALL yiPsrFromCpy(char* dest, const char* src, const char* from);

// -------------------------------
extern char* FASTCALL yiPsrBetweenCpy(char* dest, const char* src, const char* from, const char* until);


#endif
