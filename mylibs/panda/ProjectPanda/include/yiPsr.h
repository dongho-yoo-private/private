#ifndef __yi_psr_h__
#define __yi_psr_h__
#include "yi.h"
#include "yiList.h"


// -------------------------------
Bool yiPsrSplit(const char* p, const char* spliter, cyiList& list);

// -------------------------------
char* yiPsrUntilCpy(char* dest, const char* src, const char* until);

// -------------------------------
char* yiPsrFromCpy(char* dest, const char* src, const char* from);

// -------------------------------
char* yiPsrBetweenCpy(char* dest, const char* src, const char* from, const char* until);


#endif
