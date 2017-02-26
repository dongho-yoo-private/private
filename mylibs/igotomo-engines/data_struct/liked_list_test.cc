#ifdef __UNIT_TEST__
#include "itx_linked_list.h"
#include <stdio.h>

int main(int argc, void** argv)
{
    struct abc {
        int a;
        int b;
        int c;
    } temp;
    char buffer[1024];
    ItxLinkedList<struct abc> ll(buffer, sizeof(buffer));

    for (int i=0; i<10000; ++i)
    {
        temp.a=i;
        temp.b=i+1;
        temp.c=(temp.a+temp.b);
        if (ll.Add(temp)==-1)
        {
            printf("list is full!! %d\n", i);
            break;
        }
    }
    
    for (ItxNode<struct abc>* node=ll.GetTopFrom();node;node=node->next)
    {
        printf("list...(%d, %d, %d)\n", node->data.a,node->data.b,node->data.c);
    }
    return 0;
}

#endif
