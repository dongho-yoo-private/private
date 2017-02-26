#pragma once

#include "yi.h"

typedef HANDLE HSHAREDMEMORY;

// -----------------------------------
xEXTERN_C HSHAREDMEMORY CreateSharedMemory(const char* name, int nMemorySize, BOOL bUseMutex, BOOL bUseLikeMessage, const char* pszFileName);

// -----------------------------------
xEXTERN_C HSHAREDMEMORY OpenSharedMemory(const char* name, int size, BOOL bUseMutex, BOOL bUseLikeMessage, const char* pszFileName);

// -----------------------------------
xEXTERN_C void* GetPtr(HSHAREDMEMORY hSharedMemory);

// -----------------------------------
xEXTERN_C void ReleasePtr(HSHAREDMEMORY hSharedMemory);

// -----------------------------------
xEXTERN_C BOOL csmSendMsg(HSHAREDMEMORY hSharedMemory, void* data, int size);

// -----------------------------------
xEXTERN_C BOOL csmRcvMsg(HSHAREDMEMORY hSharedMemory, void* data, int* size);

// -----------------------------------
xEXTERN_C void CloseSharedMemory(HSHAREDMEMORY hSharedMemory);

// -----------------------------------
xEXTERN_C BOOL csmBufferEmpty(HSHAREDMEMORY hSharedMemory);

// -----------------------------------
xEXTERN_C BOOL csmBufferClear(HSHAREDMEMORY hSharedMemory);
