#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "memory_pool.h"

static void dump(const char* title, const void* buffer, size_t size, size_t column_num);
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

handle_t handle;

static void* _malloc(size_t n)
{
    printf("malloc %zd\n", n);
    void* p = mp_malloc(handle, n);
    memset(p, 'A', n);
    mp_print(handle,0);
    getchar();
    return p;
}
static void _free(void* data)
{
    mp_free(handle,data);
    mp_print(handle,0);
    getchar();
}

int main(int argc, char** argv)
{
    char buffer1[32] = {0,};;
    char buffer[2024];
    char buffer2[32] = {0,};;
    handle = mp_init(&buffer, sizeof(buffer),16);
    mp_print(handle,0);
    getchar();
    void* p = _malloc(128);
    void* p1 = _malloc(256);
    void* p2=_malloc(32);
    dump("dump", buffer, sizeof(buffer)+128, 16);
    dump("dump before ", buffer1, sizeof(buffer1), 16);
    dump("dump after", buffer2, sizeof(buffer2), 16);
    _free(p1);
    void* p3=_malloc(31);
    _free(p);
    void* p5 = _malloc(128);
    _free(p);
    _free(p2);
    _free(p3);
    printf("main exit\n");
    return 0;
}


static void dump(const char* title, const void* buffer, size_t size, size_t column_num)
{
	char* pChar = (char*)buffer;
	char bufferLine[256];
	char bufferAscii[256];
	int i=0;
	int asci=0;
    const int fd = fileno(stdout);

	dprintf(fd, "------------ dump(%s) %zd bytes ------------ \n", title, size);
	
	for (i=0; i<column_num; i++)
	{
		dprintf(fd, "%02d|", (int)i);
	}
	dprintf(fd, "\n");
	for (i=0; i<column_num; i++)
	{
		dprintf(fd, "---");
	}
	//dprintf(fd, "\n");

	bufferAscii[0]=0;
	bufferLine[0]=0;
	for (i=0; i<size; i++) 
	{
		char buffer[16];
		if (i!=0&&i%column_num==0) {
			dprintf(fd, "\n");
			dprintf(fd, "%s", bufferLine);
			dprintf(fd, "   %s", bufferAscii);
			bufferLine[0]=0;
			bufferAscii[0]=0;
			asci=0;
		}
		sprintf(buffer, "%02x|", (int)(0xFF&*pChar));
		strcat(bufferLine, buffer);

		if (*pChar==0 || *pChar=='\n' || *pChar=='\r' ||
			*pChar=='\t' || *pChar<=0x1F)
		{
			bufferAscii[asci]='.';
		}
		else if ((*pChar&0x80)==0)
			bufferAscii[asci]=*pChar;
		else
			bufferAscii[asci]='.';

		//printf(fd, "%02x|", (int)(0xFF&*pChar));
		pChar++;
		asci++;
		bufferAscii[asci]=0;
	}

	dprintf(fd, "\n");
	dprintf(fd, "%s", bufferLine);
	dprintf(fd, "   %s", bufferAscii);
	dprintf(fd, "\n");
}
