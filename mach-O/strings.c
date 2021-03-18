#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

static char* _strstr(char *p,const char *str,size_t size)
{
    const size_t n = strlen(str);
    while(size--)
    {
        if (size<n)
        {
            return 0;
        }
        if (memcmp(p,str,n)==0)
        {
            return p;
        }
        ++p;
    } 
    return 0;
}

int64_t find_strings(const char *filename,const char *str)
{
    struct stat s;
    size_t size;
    FILE *fp;
    char*p,*pNext;
    const size_t len=strlen(str);
    size_t remain;

    if (stat(filename,&s)!=0)
    {
        return 0;
    }
    fp=fopen(filename,"r");
    if (fp==0)
    {
        printf("file not found!!! %s\n",filename);
        return 0;
    }
    size=s.st_size;
    pNext=p=(char*)malloc(size+1);
    printf("file size!!! %zd\n",size);
    if (fread((char*)p,size,1,fp)!=1)
    {
        fclose(fp);
        free(p);
        printf("read error!!! %s\n",filename);
        return 0;
    }
    fclose(fp);
    remain=size;
    while((pNext=_strstr(pNext,str,remain))!=0)
    {
        intptr_t diff = (intptr_t)pNext-(intptr_t)p;
        printf("%ld : %s\n",diff,str);
        pNext+=len;
        if (diff>remain)
        {
            break;
        }
        remain-=diff;
    }
    printf("successed\n");
    free(p);
    return 0;
}
int main(int argc,const char **argvs)
{
    find_strings(argvs[1],argvs[2]);
    return 0;
}
