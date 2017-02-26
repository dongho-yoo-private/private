#include "igx_memory.h"
#include "igx_time.h"
#include "debug/igx_debug.h"
#include "thread/igx_thread.h"
#include <stdlib.h>
#include <string.h>

#ifdef __POSIX__
#   include <unistd.h>
#endif

#if OS_IS_LINUX==1
#   include <sys/mman.h>
#   include <stdio.h>
#   include <fcntl.h>
#endif
#if OS_IS_MACOSX==1
#   include <sys/types.h>
#   include <sys/mman.h>
#endif
#if OS_IS_IOS==1
#   include <sys/types.h>
#   include <sys/mman.h>
#endif

#define IGX_MEMORY_ALIGN (8)

static struct igx_memory_t {
    struct {
        struct igx_memory_t* next; 
        struct igx_memory_t* prev; 
        const char* tag;
        igx_time_t time;
        void* back_trace[16];
        size_t size;
        ubit32_t reserved;
    } header;
    char data[IGX_MEMORY_ALIGN];
}* g_memory_top=0,*g_memory_end=0;
static igx_mutex_t g_mutex;


size_t igx_mem_header_size()
{
    return sizeof(*g_memory_top);
}
static void add_memory_to_node(struct igx_memory_t* p)
{
    bool_t is_initialized=false;
    if (is_initialized==false)
    {
        igx_mutex_init(&g_mutex);
        is_initialized=true;
    }
    igx_mutex_lock(&g_mutex);
    if (g_memory_end!=0)
    {
        g_memory_end->header.next=p;
        p->header.next=0;
        p->header.prev=g_memory_end;
        g_memory_end->header.next=p;
        g_memory_end=p;
    }
    else
    {
       g_memory_top=g_memory_end=p;
       p->header.next=p->header.prev=0;
    }
    igx_mutex_unlock(&g_mutex);
}
static void remove_memory_from_node(struct igx_memory_t* p)
{
    struct igx_memory_t* prev=p->header.prev;
    struct igx_memory_t* next=p->header.next;
    if (prev!=0)
    {
        prev->header.next=next;
    }
    if (next!=0)
    {
        next->header.prev=prev;
    }
    if (p==g_memory_top)
    {
        g_memory_top=next;
    }
    if (p==g_memory_end)
    {
        g_memory_end=prev;
    }
}
static struct igx_memory_t* get_header_addr(void* p)
{
    struct igx_memory_t* header = (struct igx_memory_t*)(((char*)p)-sizeof(header->header));
    return header;
}
static void* get_aligned_addr(void* p, ubit32_t align, bit32_t* offset)
{
    ubit8_t* aligned = (ubit8_t*)p;
    const ubit32_t remain = ((intptr_t)aligned%align);

    if (remain==0)
    {
        *offset=0;
        return p;
    }
    *offset=(align-remain);
    aligned+=*offset;
    return aligned;
}
static struct igx_memory_t* create_memory(size_t size, const char* tag)
{
    struct igx_memory_t* p = (struct igx_memory_t*)malloc(size+sizeof(*p));
    p->header.tag=tag;
    p->header.time=igx_time_local_time();
    p->header.size=size;
    igx_get_stacktrace(p->header.back_trace, 16);
    p->header.reserved=0; 

    return p;
}
void* igx_malloc(size_t size, const char* tag)
{
#ifdef IGX_MEMORY_DEBUG
    bit32_t offset;
    void* ret;
    struct igx_memory_t* p = create_memory(size, tag);
    add_memory_to_node(p);
    ret = (void*)get_aligned_addr(p->data, IGX_MEMORY_ALIGN, &offset);
    
    ((ubit8_t*)ret)[-1]=(ubit8_t)offset;
    //igx_log_dump("igx_malloc dump", ret-4, 16, 16);
    return ret;
#else
    return malloc(size);
#endif
}
void* igx_realloc(void* before, size_t size, const char* tag)
{
#ifdef IGX_MEMORY_DEBUG
    bit32_t offset;
    void* ret;
    struct igx_memory_t* p; 
    struct igx_memory_t* temp = get_header_addr(before);
    remove_memory_from_node(temp);

    p = (struct igx_memory_t*)realloc(temp, size+sizeof(*p));

    add_memory_to_node(p);
    p->header.tag=tag;
    p->header.time=igx_time_local_time();
    p->header.size=size;
    p->header.reserved=0;
    igx_get_stacktrace(p->header.back_trace, 16);

    ret = (void*)get_aligned_addr(p->data, IGX_MEMORY_ALIGN, &offset);
    
    if (offset!=0)
    {
        ((ubit8_t*)ret)[-1]=(ubit8_t)offset;
    }
    return ret;
#else
    return realloc(before, size);
#endif
}
void igx_free(void* p)
{
#ifdef IGX_MEMORY_DEBUG
    struct igx_memory_t* header;
    ubit8_t offset = ((ubit8_t*)p)[-1];
    //igx_log_dump("igx_malloc dump", (char*)p-4/*sizeof(*header)*/, 16, 16);

#ifdef IGX_PRINT_FREE_BACKTRACE
    //igx_print_current_stacktrace();
#endif
    header = get_header_addr((ubit8_t*)p-offset); 

    remove_memory_from_node(header);
    free(header);
#else
    free(p);
#endif
}
void* igx_vmalloc(size_t size)
{
#ifdef __LINUX__
    int fd;
    void*ret;
    fd = open("/dev/zero", O_RDONLY);
    ret = mmap(0, size, PROT_WRITE|PROT_READ , MAP_PRIVATE, fd, 0);
    close(fd);
    *((bit32_t*)ret)=size;
    return ((bit32_t*)ret+1);
#endif
#ifdef __BSD__
    void*ret;
    ret = mmap(0, size, PROT_WRITE|PROT_READ , MAP_SHARED | MAP_ANON/*MAP_PRIVATE*/, -1, 0);
    *((bit32_t*)ret)=size;
    return ((bit32_t*)ret+1);
#endif

    IGX_CRITICAL("unsupport os!!\n");
    IGX_ASSERT(0);
    return 0;
}
void igx_vfree(void* data)
{
    bit32_t* p= (bit32_t*)data;
    --p;
    munmap((void*)p, (size_t)*p);
}
void igx_memory_print(const char* tag)
{
    struct igx_memory_t* p = g_memory_top;
    bool_t tag_is_found=false;
    ubit32_t total_used_memory=0, total_allocated_count=0;
    bool_t is_print_all = (tag!=0&&*tag=='*');
    IGX_DEBUG("========== memory info =============");
    if (tag!=0)
    {
        char time_buffer[32];
        for (;p;p=p->header.next)
        {
            if (is_print_all==true ||
                     strstr(p->header.tag, tag)!=0)
            {
                IGX_DEBUG("%s:(%p)[%s] %dbytes-------------", p->header.tag, p, igx_time_to_string(time_buffer, p->header.time, "%02d/%02d/%02d %02d:%02d:%02d"), p->header.size);
                igx_print_stacktrace(p->header.back_trace, 16, 2);
                IGX_DEBUG("");
                tag_is_found=true;
            }
            total_used_memory+=p->header.size;
            ++total_allocated_count;
        }/* for */
    }/* if tag */
    if (tag!=0 &&
            tag_is_found==false)
    {
        IGX_DEBUG("tag %s is not fount");
    }
    IGX_DEBUG("Total Used Memory: %d bytes", total_used_memory);
    IGX_DEBUG("Memory allocate count: %d ", total_allocated_count);
    IGX_DEBUG("====================================;");
}

size_t igx_get_page_size()
{
#ifdef __POSIX__
    return getpagesize();
#endif
    return 4096;
}
