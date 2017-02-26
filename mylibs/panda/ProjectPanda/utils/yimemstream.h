#ifndef __MEM_STREAM_H__
#define __MEM_STREAM_H__

#include "yi.h"
#include "yistd.h"

typedef Handle HandleMemStream

// ----------------------------
EXTERN HandleMemStream yimsCreate(Bool bUseMutex, int pagesize);

// ----------------------------
EXTERN_C void yimsDestroy(Handle h);

// ----------------------------
EXTERN_C Bool yimsWrite(HandleMemStream h, const void* buffer, int n);

// ----------------------------
EXTERN_C Bool yimsRead(HandleMemStream h, void* buffer, int nBufferSize);

// ----------------------------
EXTERN_C Bool yimsToFile(HandleMemStream h, const char* pszFileName);

// ----------------------------
EXTERN_C unsigned int yimsGetCurrentOffset(HandleMemStream h);

// ----------------------------
EXTERN_C unsigned int yimsSetCurrentOffset(HandleMemStream h, unsigned int offset);


#endif
