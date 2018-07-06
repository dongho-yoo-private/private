/**
* Copyright (c) 2017 Wonderplanet Inc. All Rights Reserved.
* \author dongho.yoo
*/
#include "malloc_hook.h"
#include "../memory_pool_defs.h"
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <assert.h>


static struct {
    void* (*malloc)(size_t ); 
    void* (*calloc)(size_t,size_t); 
    void* (*realloc)(void*,size_t); 
    void (*free)(void*); 
    TMemoryFreeHandler free_handler;
    TMemoryAllocationHandler allocate_handler;
} memory_hook_fncs={0,};

void init_memory_hook(TMemoryAllocationHandler handler, TMemoryFreeHandler free_handler)
{
    memory_hook_fncs.free_handler=free_handler;
    memory_hook_fncs.allocate_handler=handler;
    /*  次に引っかかるシンボルのアドレスを取る 
        Androidでもうまくいくか？ */
    memory_hook_fncs.malloc=(void* (*)(size_t))dlsym(RTLD_NEXT, "malloc");
    memory_hook_fncs.calloc=(void* (*)(size_t, size_t))dlsym(RTLD_NEXT, "calloc");
    memory_hook_fncs.realloc=(void* (*)(void*, size_t))dlsym(RTLD_NEXT, "realloc");
    memory_hook_fncs.free=(void (*)(void*))dlsym(RTLD_NEXT, "free");
}
void* malloc_original(size_t size)
{
    if (memory_hook_fncs.malloc!=0)
    {
        return memory_hook_fncs.malloc(size);
    }
    assert(0);
    return 0;
}
void* calloc_original(size_t n,size_t size)
{
    if (memory_hook_fncs.calloc!=0)
    {
        return memory_hook_fncs.calloc(n,size);
    }
    assert(0);
    return 0;
}
void* realloc_original(void* data, size_t size)
{
    if (memory_hook_fncs.realloc!=0)
    {
        return memory_hook_fncs.realloc(data,size);
    }
    assert(0);
    return 0;
}
void free_original(void* data)
{
    if (memory_hook_fncs.malloc!=0)
    {
        return memory_hook_fncs.free(data);
    }
    assert(0);
}

/* ここでmallocを実装する、Linke順番によってこちらの方が先にリンクされる */
#if USE_MALLOCK_HOOKER==1
void* malloc(size_t n)
{
    if (memory_hook_fncs.allocate_handler!=0)
    {
        return memory_hook_fncs.allocate_handler(n);
    }
    return malloc_original(n);
}
void* calloc(size_t n, size_t size)
{
    if (memory_hook_fncs.allocate_handler!=0)
    {
        return memory_hook_fncs.allocate_handler(n*size);
    }
    return calloc_original(n,size);
}
void* realloc(void* data, size_t n)
{
    if (memory_hook_fncs.allocate_handler!=0)
    {
        void* ret=memcpy(memory_hook_fncs.allocate_handler(n), data, n);
        free(data);
        return ret; 
    }
    return realloc_original(data,n);
}
void free(void* data)
{
    if (memory_hook_fncs.free_handler!=0)
    {
        memory_hook_fncs.free_handler(data);
    }
    return free_original(data);
}
#endif
