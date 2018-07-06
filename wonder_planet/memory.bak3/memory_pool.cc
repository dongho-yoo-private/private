#include "memory_pool.h"
#include "private/malloc_hook.h"
#include "private/pool.h"

static void* global_memory_pool=0;

static void Free(void* );
static void* Malloc(size_t n);

handle_t mp_init(void* ptr, size_t size, size_t align)
{
    init_memory_hook(&Malloc,&Free);
    if (ptr==0)
    {
        global_memory_pool=ptr=malloc_original(size);
    }
    memory_pool_init_struct(ptr,size,align);
    return (handle_t)ptr;
}
void mp_destroy(handle_t handle)
{
    void* ptr=handle;
    if (ptr==0)
    {
        ptr=global_memory_pool;
    }
    free_original(ptr);
}
void mp_pause(handle_t handle)
{
    void* ptr=handle; 
    ptr=ptr==0?global_memory_pool:ptr;
    memory_hook_pause(ptr);
}
void mp_resume(handle_t handle)
{
    void* ptr=handle; 
    ptr=ptr==0?global_memory_pool:ptr;
    memory_hook_resume(ptr);
}
void* mp_malloc(handle_t handle, size_t n)
{
    void* ptr=handle; 
    ptr=ptr==0?global_memory_pool:ptr;
    return memory_pool_malloc(ptr, n);
}
void mp_free(handle_t handle,void*data)
{
    void* ptr=handle; 
    ptr=ptr==0?global_memory_pool:ptr;
    memory_pool_free(ptr, data);
}
void mp_print(handle_t handle)
{
    void* ptr=handle; 
    memory_pool_print(ptr);
}
void* operator new (size_t size, handle_t handle)
{
    return mp_malloc(handle, size);
}
void operator delete (void* data, handle_t handle)
{
    mp_free(handle, data);
}
static void Free(void* data)
{
    mp_free(0, data);
}
static void* Malloc(size_t n)
{
    return mp_malloc(0, n);
}
