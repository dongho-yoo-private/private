#include "../memory_pool_defs.h"
#include "pool.h"
#include <new>
#include <stdlib.h>

#if USE_NEW_OVERLOAD==1
void* operator new(std::size_t size)
{
    return malloc(size);
}
void* operator new(std::size_t size) throw(std::bad_alloc) 
{
  return malloc(size);
}
void* operator new[](std::size_t size) throw(std::bad_alloc) 
{
  return malloc(size);
}
void* operator new(std::size_t size, const std::nothrow_t&) throw() 
{
  return malloc(size);
}
void* operator new[](std::size_t size, const std::nothrow_t&) throw() 
{
  return malloc(size);
}
void operator delete(void* p)
{ 
    free(p); 
}
void operator delete(void* p) throw() 
{ 
    free(p); 
}
void operator delete[](void* p) throw() 
{ 
    free(p); 
}
void operator delete(void* p, const std::nothrow_t&) throw() 
{
    free(p);
}
void operator delete[](void* p, const std::nothrow_t&) throw() 
{ 
    free(p);
}
void  operator delete(void* ptr, std::size_t size) noexcept
{
    free(p);
}
void  operator delete[](void* ptr, std::size_t size) noexcept
{
    free(p);
}
#endif
