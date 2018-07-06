#include "memory_pool.h"
#include "private/malloc_hook.h"
#include "private/make_pool.h"

static void* global_memory_pool=0;

static void Free(void* );
static void* Malloc(size_t n);

void* mp_init(void* ptr, size_t size, size_t alignment, bool_t is_debug_mode)
{
    global_memory_pool=init_memory_hook(&Malloc,&Free);
    if (ptr==0)
    {
        global_memory_pool=malloc_original(size);
    }
    memory_pool_init_struct(ptr,size,alignment,is_debug_mode);
}
void mp_destroy(void* p)
{
    if (ptr==0)
    {
        ptr=global_memory_pool;
    }
    free_original(ptr);
}
void mp_pause(void* ptr)
{
    ptr=ptr==0?global_memory_pool:ptr;
    memory_pool_pause(ptr);
}
void mp_resume()
{
    ptr=ptr==0?global_memory_pool:ptr;
    memory_pool_resume(ptr);
}
void* mp_malloc(void* ptr, size_t n)
{
    ptr=ptr==0?global_memory_pool:ptr;
    memory_pool_malloc(ptr, n);
}
void mp_free(void* ptr,void*data)
{
    ptr=ptr==0?global_memory_pool:ptr;
    memory_pool_free(ptr, n);
}
void* operator new (void* ptr, size_t size)
{
    mp_malloc(ptr, size);
}
void operator delete (void* ptr, void* data)
{
    mp_free(ptr, data);
}
static void Free(void* data)
{
    mp_free(0, data);
}
static void* Malloc(size_t n)
{
    return mp_malloc(0, n);
}
