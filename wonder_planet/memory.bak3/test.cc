#include <stdlib.h>
#include "memory_pool.h"

int main(int argc, char** argv)
{
    mp_init(0, 1024*1024);
    mp_print(0);
    void* p;
    p=malloc(1024);
    p=malloc(16);
    p=malloc(32);
    mp_print(0);
    return 0;
}
