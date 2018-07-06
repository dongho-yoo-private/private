#include "memory_pool_new.h"
#include "memory_pool.h"

void* operator new (size_t size, handle_t handle)
{
    return mp_malloc(handle, size);
}
void operator delete (void* data, handle_t handle)
{
    mp_free(handle, data);
}

