#include <stdio.h>

typedef struct {
    unsigned a:1;
    unsigned b:1;
    unsigned c:1;
    unsigned d:2;
} struct_aaaa;


int main(int argc, char** argv)
{
    struct_aaaa aaaa = {1, 0, 1, 3 };
    printf("size:%zd\n", sizeof(aaaa));

    printf("%d, %d, %d, %d\n", (int)aaaa.a, (int)aaaa.b, (int)aaaa.c, (int)aaaa.d);

    return 0;
}
