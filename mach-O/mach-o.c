#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "mach-o-types.h"

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

MACHO_header_64_t *MACHO_Load(const char *filename,size_t *_size)
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
    return (MACHO_header_64_t*)p;
}
void MACHO_UnLoad(MACHO_header_64_t *p)
{
    if (p!=0)
    {
        free(p);
    }
}
void show_segment_section(const char *filename)
{
    size_t totalsize=0;
    MACHO_header_64_t *mach=MACHO_Load(filename,0);
    MACHO_load_command_t *cmds=(MACHO_load_command_t *)(mach+1);

    dump("header",mach,sizeof(*mach),16);

    printf("magic:%x\n",mach->magic);
    printf("cputype:%x\n",mach->cputype);
    printf("cpusubtype:%x\n",mach->cpusubtype);
    printf("cmds:%x\n",mach->ncmds);
    printf("sizeofcmds:%x\n",mach->sizeofcmds);

    for (int i=0;i<mach->ncmds;++i)
    {
        char* p=(char*)cmds;
        if (cmds->cmd==LC_SEGMENT_64)
        {
            MACHO_segment_command_64_t *seg=(MACHO_segment_command_64_t *)cmds;
            MACHO_section_64_t *sec=(MACHO_section_64_t *)(seg+1);
            printf("===== %s\n",seg->segname);
            printf("vmaddr: %llx\n",seg->vmaddr);
            printf("vmsize: %lld\n",seg->vmsize);
            printf("fileoff: %llx\n",seg->fileoff);
            printf("filesize: %lld\n",seg->filesize);
            printf("sections: %d\n",seg->nsects);
            for (int j=0;j<seg->nsects;++j)
            {
                printf("\t > %s\n",sec[j].sectname);
                printf("\t > addr:%llx\n",sec[j].addr);
                printf("\t > size:%lld\n",sec[j].size);
                printf("\t > offset:%x\n",sec[j].offset);
                printf("\t > align:%d\n",sec[j].align);
            }
            //dump(seg->segname,cmds,cmds->cmdsize,16);
            //printf("======== %x : %d\n",cmds->cmd,cmds->cmdsize);
        }
        p+=cmds->cmdsize;
        totalsize+=cmds->cmdsize;
        cmds=(MACHO_load_command_t *)p;
    }
    printf("total command size:%zd bytes\n",totalsize);
    MACHO_UnLoad(mach);
    return ;
}
void dump_section(const char *filename,const char *segname, const char *secname)
{
    MACHO_header_64_t *mach=MACHO_Load(filename,0);
    MACHO_load_command_t *cmds=(MACHO_load_command_t *)(mach+1);
    size_t offset=0,size=0;
    int is_found=0;
    char *p=0;

    for (int i=0;i<mach->ncmds;++i)
    {
        p=(char*)cmds;
        if (cmds->cmd==LC_SEGMENT_64)
        {
            MACHO_segment_command_64_t *seg=(MACHO_segment_command_64_t *)cmds;
            MACHO_section_64_t *sec=(MACHO_section_64_t *)(seg+1);

            for (int j=0;j<seg->nsects;++j)
            {
                if (is_found==0&&
                    strcmp(seg->segname,segname)==0 &&
                    strcmp(sec[j].sectname,secname)==0)
                {
                    offset=sec[j].offset;
                    size=sec[j].size;
                    is_found=1;
                }
            }
        }
        p+=cmds->cmdsize;
        cmds=(MACHO_load_command_t *)p;
    }

    if (is_found!=1)
    {
        printf("not found %s[%s]\n",segname,secname);
        return ;
    }
    printf("====== %s[%s]\n",segname,secname);
    dump("",(char*)mach+offset,size,16);
    MACHO_UnLoad(mach);
    return ;
}
static void find_segment(const char *filename,int offset,char *segname, char *secname, int64_t* addr)
{
    MACHO_header_64_t *mach=MACHO_Load(filename,0);
    MACHO_load_command_t *cmds=(MACHO_load_command_t *)(mach+1);
    int is_found=0;
    char *p=0;

    for (int i=0;i<mach->ncmds;++i)
    {
        p=(char*)cmds;
        if (cmds->cmd==LC_SEGMENT_64)
        {
            MACHO_segment_command_64_t *seg=(MACHO_segment_command_64_t *)cmds;
            MACHO_section_64_t *sec=(MACHO_section_64_t *)(seg+1);

            for (int j=0;j<seg->nsects;++j)
            {
                if (sec[j].offset<offset&&
                        (sec[j].offset+sec[j].size)>offset)
                {
                    *addr=sec[j].addr+(offset-sec[j].offset);
                    strcpy(segname,seg->segname);
                    strcpy(secname,sec[j].sectname);
                    is_found=1;
                    break;
                }
            }
        }
        if (is_found==1)
        {
            break;
        }
        p+=cmds->cmdsize;
        cmds=(MACHO_load_command_t *)p;
    }
    MACHO_UnLoad(mach);
}
void find_addr(const char *filename,char *segname,char *secname,int64_t addr,int64_t *_offset,int64_t *_addr)
{
    size_t size;
    MACHO_header_64_t *mach=MACHO_Load(filename,&size);
    MACHO_load_command_t *cmds=(MACHO_load_command_t *)(mach+1);
    int is_found=0;
    char *p=0;
    int *p_int=(int*)cmds;
    int cnt=size/4;
    int64_t offset=0;

    for (int i=0;i<cnt;++i)
    {
        int64_t *p_int64=(int64_t*)p_int;
        if (*p_int64==addr)
        {
            offset=i*4;
            *_offset=offset;
            break;
        }
        ++p_int;
    }

    for (int i=0;i<mach->ncmds;++i)
    {
        p=(char*)cmds;
        if (cmds->cmd==LC_SEGMENT_64)
        {
            MACHO_segment_command_64_t *seg=(MACHO_segment_command_64_t *)cmds;
            MACHO_section_64_t *sec=(MACHO_section_64_t *)(seg+1);

            for (int j=0;j<seg->nsects;++j)
            {
                if (sec[j].offset<offset&&
                        (sec[j].offset+sec[j].size)>offset)
                {
                    *_addr=sec[j].addr+(offset-sec[j].offset);
                    strcpy(segname,seg->segname);
                    strcpy(secname,sec[j].sectname);
                    is_found=1;
                    break;
                }
            }
        }
        if (is_found==1)
        {
            break;
        }
        p+=cmds->cmdsize;
        cmds=(MACHO_load_command_t *)p;
    }
    
    MACHO_UnLoad(mach);
}
int main(int argc,const char **argvs)
{
    if (argc==2)
    {
        show_segment_section(argvs[1]);
    }
    else if (argc==3)
    {
        char seg[128],sec[128];
        int64_t addr=0;
        find_segment(argvs[1],atoi(argvs[2]),seg,sec,&addr);
        printf("%s[%s] addr:%lld\n",seg,sec,addr);
    }
    else if (argc==4)
    {
        if (strcmp(argvs[2],"-p")==0)
        {
            char seg[128],sec[128];
            int64_t offset,addr=0;
            find_addr(argvs[1],seg,sec,atoll(argvs[3]),&offset,&addr);
            printf("%s[%s] offset:%lld addr:%lld\n",seg,sec,offset,addr);
        }
        else
        {
            dump_section(argvs[1],argvs[2],argvs[3]);
        }
    }
    else
    {
        printf("Usages\n");
        printf("./mach-o [file path] [segment name = optional] [section name = optional]\n");
    }
    return 0;
}



