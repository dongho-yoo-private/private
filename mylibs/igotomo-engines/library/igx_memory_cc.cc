#include "igx_memory.h"
#include "debug/igx_debug.h"

void* operator new(size_t size, const char* tag)
{
    return igx_malloc(size, tag);
}
void operator delete(void* p, const char* null_tag)
{
    igx_free(p);
}
