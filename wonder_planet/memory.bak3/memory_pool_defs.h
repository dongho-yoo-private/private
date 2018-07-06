/**
* Copyright (c) 2017 WWonderPlanet Inc. All Rights Reserved.
* \author dongho.yoo
*/
#ifndef _MEMORY_POOL_DEFS_H_
#define _MEMORY_POOL_DEFS_H_

#include <sys/types.h>
#include <stdio.h>

/* + type definitions + */
typedef int32_t bit32_t;
typedef int64_t bit64_t;
typedef char bit8_t;
typedef intptr_t ptr_t;
typedef bit32_t bool_t;

/* メモリプールのハンドル */
typedef struct {
    int ____unused____;
}*handle_t;

#ifndef true
#   define true 1
#endif
#ifndef false
#   define false 0
#endif

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
