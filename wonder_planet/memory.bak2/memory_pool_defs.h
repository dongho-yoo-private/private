/**
* Copyright (c) 2017 WWonderPlanet Inc. All Rights Reserved.
* \author dongho.yoo
*/
#ifndef _MEMORY_POOL_DEFS_H_
#define _MEMORY_POOL_DEFS_H_

/* + type definitions + */
typedef int32_t bit32_t;
typedef int64_t bit64_t;
typedef char bit8_t;
typedef intptr_t ptr_t;

/**
* \brief mallocフックする場合は、1
*/
#define USE_MALLOCK_HOOKER 1
/**
* \brief newのオーバーロード時
*/
#define USE_NEW_OVERLOAD 1

#define MEMORY_DEBUG 1

#define MEM_DEBUG_PRINT printf

#endif
