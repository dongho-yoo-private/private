#include "../memory_pool_defs.h"
#include "pool.h"

#if USE_NEW_OVERLOAD==1
void* operator new(std::size_t size) throw(std::bad_alloc) 
{
  return malloc(0, size);
}
void* operator new[](std::size_t size) throw(std::bad_alloc) 
{
  return malloc(0,size);
}
void* operator new(std::size_t size, const std::nothrow_t&) throw() 
{
  return malloc(0,size);
}
void* operator new[](std::size_t size, const std::nothrow_t&) throw() 
{
  return malloc(size);
}
void operator delete(void* p) throw() 
{ 
    free(0,p); 
}
void operator delete[](void* p) throw() 
{ 
    free(0,p); 
}
void operator delete(void* p, const std::nothrow_t&) throw() 
{
    free(0,p);
}
void operator delete[](void* p, const std::nothrow_t&) throw() 
{ 
    free(0,p);
}
#endif
