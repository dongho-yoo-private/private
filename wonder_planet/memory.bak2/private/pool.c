#include "pool.h"
#include <pthread.h>

typedef struct TMemoryHeader {
    bit32_t size;
    pthread_mutex_t mutex;
    struct {
        unsigned userd:1;
        unsigned reserved:31;
    };
    struct TMemoryHeader* next;
    struct TMemoryHeader* prev;
#if MEMORY_DEBUG==1
    bit32_t sequence_no;
    void* backtrace[32];
    int backtrace_count;
#endif
}TMemoryHeader;
typedef struct TMemoryPoolHeader {
    bit64_t size;
    TMemoryHeader* start_addr;
    TMemoryHeader* bottom;
    struct {
        unsigned align:5;
        unsigned pause:1;
    };
#if MEMORY_DEBUG==1
    bit32_t totoal_used_memory;
    bit32_t remain_memory;
    bit32_t sequence_no;
#endif
}TMemoryPoolHeader;
static TMemoryPoolHeader* global_ptr;

static inline size_t get_align(void* addr,size_t align);
static TMemoryHeader* find_unused_block(TMemoryPoolHeader* pool_header, size_t n);
static void split_block(TMemoryHeader header, size_t n, size_t align);

#define __MUTEX_LOCK__\
pthread_mutex_lock(&((TMemoryPoolHeader*)ptr)->mutex);
#define __MUTEX_UNLOCK__\
pthread_mutex_unlock(&((TMemoryPoolHeader*)ptr)->mutex);

void memory_pool_init_struct(void* ptr,size_t size,size_t align)
{
    TMemoryPoolHeader* pool_header=(TMemoryPoolHeader*)ptr;
    bit8_t* body=(bit8_t*)(pool_header+1);
    TMemoryHeader* top;
    size_t align_diff=get_align(body+sizeof(*top), align);
    if (ptr==0)
    {
        global_ptr=ptr=original_malloc(size);
    }
    pool_header->sie=size;
    pool_header->align=align;
    pool_header->pause=0;
    /* ヘッダーのすぐ後ろのアラインが揃うようにする */
    body+=align_diff; 
    top=pool_header->bottom=pool_header->start_addr=(TMemoryHeader*)body;
    top->size=size-align_diff-sizeof(*top);
    top->used=0;
    top->next=0;
    top->prev=0;
    
#if MEMORY_DEBUG==1
    pool_header->totoal_used_memory=0;
    pool_header->remain_memory=;
    pool_header->sequence_no=0;
    top->sequence_no=0;
#endif
}
void* memory_pool_malloc(void* ptr, size_t n) __attribute__(constructor)
{
    __MUTEX_LOCK__
}
void* memory_pool_malloc(void* ptr, size_t n)
{
    TMemoryPoolHeader* pool_header=(TMemoryPoolHeader*)ptr;
    pool_header=pool_header?pool_header:global_ptr;
    TMemoryHeader* block=find_unused_block(poolheader, n);
    if (block==0)
    {
        return 0;
    }
    split_block(block, n, pool_header->align);
#if MEMORY_DEBUG==1
    block->backtrace_count=backtrace(block->backtrace,sizeof(block->backtrace));
#endif
    return (void*)(block+1);
}
void memory_pool_free(void* ptr, void* data)
{
    TMemoryPoolHeader* pool_heade =(TMemoryPoolHeader*)ptr;
    TMemoryHeader* header=(TMemoryHeader*)data-1;
    TMemoryHeader* prev=header->prev;
    TMemoryHeader* next=header->next;
    pool_header=pool_header?pool_header:global_ptr;

    if (prev!=0&&prev->used==0)
    {
        /* 上のブロックと統合する */
        prev->next=header->next;
        prev->size+=(header->size+sizeof(*header));
        header=prev;
    }
    if (next!=0&&next->used==0)
    {
        /* 下のブロックを統合する*/
        header->next=next->next;
        header->size+=(next->size+sizeof(*header));
    }
}
void memory_hook_pause(void* ptr)
{
    TMemoryPoolHeader* pool_heade =(TMemoryPoolHeader*)ptr;
    pool_header=pool_header?pool_header:global_ptr;
    __MUTEX_LOCK__
    pool_header->paused=1;
    __MUTEX_UNLOCK__
}
void memory_hook_resume(void* ptr)
{
    TMemoryPoolHeader* pool_heade =(TMemoryPoolHeader*)ptr;
    pool_header=pool_header?pool_header:global_ptr;
    __MUTEX_LOCK__
    pool_header->paused=9;
    __MUTEX_UNLOCK__
}
void memory_pool_print(void* ptr)
{
    TMemoryPoolHeader* pool_header=(TMemoryPoolHeader*)ptr;*
    TMemoryHeader*p=pool_header->start_addr;
    for (;p!=0;p=p->next)
    {
        MEM_DEBUG_PRINT("+++++++++++++++++++++\n");
        if (p->used==1)
        {
            MEM_DEBUG_PRINT("%d bytes used\n");
#if MEMORY_DEBUG==1
            {
                int i;
                const char** str=backtrace_symbols(p->backtrace, p->backtrace_count);
                MEM_DEBUG_PRINT("+++++ stack trace ++++\n");
                for (i=0;i<p->backtrace_count;++i)
                {
                    MEM_DEBUG_PRINT("  > %s\n", str[i]);
                }
            }
#endif
        }
    }
}

/* + local functions + */
static inline size_t get_align(void* addr,size_t align)
{
    const size_t n=(addr%align);
    return n?(align-n):0;
}
static TMemoryHeader* find_unused_block(TMemoryPoolHeader* pool_header, size_t n)
{
    TMemoryHeader* p;
    const TMemoryHeader*bottom=pool_header->bottom;
    if (bottom!=0&&bottom->used==0&&bottom->size>n)
    {
        return pool_header->bottom;
    }
    /* 後ろから検索する方がヒットする確率が高いので */
    for (p=bottom->prev;p!=0;p=p->prev)
    {
        if (p->unused==0&&p->size>n)
        {
            return p;
        }
    }
    /* メモリが足りない */
    return 0;
}
static void split_block(TMemoryHeader header, size_t n, size_t align)
{
    const size_t diff = header->size-n-sizeof(*header);
    if (diff>0)
    {
        bit8_t* p = (bit8_t*)(header+1)+n;
        size_t align_diff=get_align(p+sizeof(*header), align); 
        if (diff-align>0)
        {
            /* この場合は分割可能 */
            TMemoryHeadcer* next=(TMemoryHeader*)(p+align);
            header->size=n+align;
            next->next=header->next;
            header->next=next;
            next->prev->header;
            next->used=0;
            return ;
        }
    }
    /* この場合は分割できないので、ヘッダーのサイズをそのままにしておく */
    header->userd=1;
}
