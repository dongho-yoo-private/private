/**
* Copyright (c) 2017 Wonderplanet Inc. All Rights Reserved.
* \author dongho.yoo
*/

#ifndef __MEMORY_HOOKER_H__
#define __MEMORY_HOOKER_H__

#include "../memory_pool_defs.h"

#ifdef __cplusplus
#   define EXTERN_C extern "C"
#else
#   define EXTERN_C extern
#endif

typedef void* (*TMemoryAllocationHandler)(size_t size);
typedef void (*TMemoryFreeHandler)(void* data);
/**
* \brief malloc系のシステムコールをフックします。
* \param handler malloc系が呼び出された際に呼び出されるコールバック
* \param free_handler freeが呼び出された際に呼び出されるコールバック
*/
EXTERN_C void init_memory_hook(TMemoryAllocationHandler handler, TMemoryFreeHandler free_handler);
/**
* \brief libcのmalloc
*/
EXTERN_C void* malloc_original(size_t size);
/**
* \brief libcのcalloc
*/
EXTERN_C void* calloc_original(size_t n,size_t size);
/**
* \brief libcのrealloc
*/
EXTERN_C void* realloc_original(void* data, size_t size);
/**
* \brief libcのfree
*/
EXTERN_C void free_original(void* data);

/* new, deleteのオーバーロード */
#ifdef __cplusplus

#endif

#endif /* __MEMORY_HOOKER_H__ */
