#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "memory_pool.h"

class ABCDE {
public:
    ABCDE()
    {
        printf("constructor called\n");
    }
    ~ABCDE()
    {
        printf("destructor called\n");
    }
    int a;
};

static char* load_file(const char* fllename)
{
        FILE* fp=fopen(fllename, "r");
        assert(fp);
        fseek(fp, 0, SEEK_END);
        size_t size = ftell(fp);
        fseek(fp,0,SEEK_SET);
        printf("fp:%p, %s size:%zd\n",fp,fllename,size);
        char* p = (char*)malloc(size+1);
        fread(p, size, 1, fp);
        p[size]=0;
        fclose(fp);
        return p;
}

int main(int argc, char** argv)
{

    char buffer[2024];
    handle_t handle = mp_init(&buffer, sizeof(buffer),16);
    mp_print(handle,0);
    getchar();
    void* p = mp_malloc(handle, 128);
    mp_print(handle,0);
    getchar();
    void* p1 = mp_malloc(handle, 256);
    mp_print(handle,0);
    getchar();
    void* p2 = mp_malloc(handle, 32);
    mp_print(handle,0);
    getchar();
    printf("malloc 31\n");
    void* p4 = mp_malloc(handle, 31);
    mp_print(handle,0);
    getchar();
    mp_free(handle,p1);
    mp_print(handle,0);
    getchar();
    printf("malloc 128\n");
    void* p5 = mp_malloc(handle, 128);
    mp_print(handle,0);
    getchar();
    return 0;
}
