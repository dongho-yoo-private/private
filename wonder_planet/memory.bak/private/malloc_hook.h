/**
* Copyright (c) 2017 Wonderplanet Inc. All Rights Reserved.
* \author dongho.yoo
*/

#ifndef __MEMORY_HOOKER_H__
#define __MEMORY_HOOKER_H__

#include "../memory_pool_defs.h"

#ifdef __cplusplus
#   define EXTERN_C extern "C"
#endif

typedef void* (*TMemoryAllocationHandler)(size_t size);

EXTERN_C int init_memory_hook(TMemoryAllocationHandler handler, TMemoryFreeHHandler free_handler);
EXTERN_C void* malloc_original(size_t size);
EXTERN_C calloc_original(size_t n,size_t size);
EXTERN_C realloc_original(void* data, size_t size);
EXTERN_C void free_original(void* data);

/* new, deleteのオーバーロード */
#ifdef __cplusplus

#endif

#endif /* __MEMORY_HOOKER_H__ */
