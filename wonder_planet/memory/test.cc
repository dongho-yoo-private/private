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

    mp_init(0, 1024*1024);
    mp_print(0);
    void* p;
    void*p1=malloc(1024);
    void*p2=malloc(16);
    mp_set_sequence_no(0, 1);
    char* file = load_file("./private/pool.c");
    void*p3=malloc(32);
    void*p5=malloc(123456);
    memset(p2, 2, 16);
    free(p2);
    getchar();
    mp_print(0);
    getchar();
    free(p3);
    mp_set_sequence_no(0, 2);
    mp_print(0);
    getchar();
    p=malloc(512);
    memset(p, 2, 512);
    mp_print(0);
    getchar();
    p=malloc(512);
    mp_print(0);
    getchar();
    free(p1);
    mp_print(0);
    ABCDE* abcde=new ABCDE();
    mp_print(0);
    delete abcde;
    mp_print(0);
    mp_print(0,1);
    return 0;
}
