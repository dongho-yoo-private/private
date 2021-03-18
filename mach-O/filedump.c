#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

static void dump(const char *title, const void *buffer, size_t size, size_t column_num)
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

static void *UnLoad(void *p)
{
    free(p);
}
static void *Load(const char *filename,size_t *_size)
{
    struct stat s;
    size_t size;
    FILE *fp;
    void *p;

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
    if (_size!=0)
    {
        *_size=size;
    }
    p=malloc(size+1);
    if (fread((char*)p,size,1,fp)!=1)
    {
        fclose(fp);
        free(p);
        printf("read error!!! %s\n",filename);
        return 0;
    }
    fclose(fp);
    return p;
}
int main(int argc,char **argvs)
{
    size_t size;
    char* p=(char*)Load(argvs[1],&size);
    int per_bytes=argvs[4]==0?16:atoi(argvs[4]);
    
    dump("",p+atoi(argvs[2]),atoi(argvs[3]),per_bytes);
    UnLoad(p);
    return 0;
}
