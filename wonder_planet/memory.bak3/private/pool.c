/**
* Copyright 2017 (c) WonderPlanet Inc. All Rights Reserved.
* \author dongho.yoo
*/
#include "pool.h"
#include "malloc_hook.h"
#include <pthread.h>
#include <execinfo.h>

/* 最近のアーキテクチャーではデフォルトで8だが、明示的にしておく*/
#pragma pack(8)
typedef struct memory_header_t {
    bit32_t size;
    struct {
        unsigned used:1;
        unsigned reserved:31;
    };
    struct memory_header_t* next;
    struct memory_header_t* prev;
#if MEMORY_DEBUG==1
    bit32_t sequence_no;
    void* backtrace[32];
    int backtrace_count;
#endif
}memory_header_t;
#pragma pack()

typedef struct memory_pool_header_t {
    size_t size;
    pthread_mutex_t mutex;
    memory_header_t* start_addr;
    memory_header_t* bottom;
    struct {
        unsigned align:5;
        unsigned pause:1;
    };
#if MEMORY_DEBUG==1
    bit32_t totoal_used_memory;
    bit32_t remain_memory;
    bit32_t sequence_no;
#endif
}memory_pool_header_t;
static memory_pool_header_t* global_ptr=0;

static inline size_t get_align(void* addr,size_t align);
static memory_header_t* find_unused_block(memory_pool_header_t* pool_header, size_t n);
static void split_block(memory_pool_header_t* pool_header,memory_header_t* header, size_t n, size_t align);
static void* Malloc(size_t n);
static void Free(void* data);

#define __MUTEX_LOCK__ \
pthread_mutex_lock(&((memory_pool_header_t*)ptr)->mutex);
#define __MUTEX_UNLOCK__ \
pthread_mutex_unlock(&((memory_pool_header_t*)ptr)->mutex);

void memory_pool_init_struct(void* ptr,size_t size,size_t align)
{
    memory_pool_header_t* pool_header=(memory_pool_header_t*)ptr;
    bit8_t* body=(bit8_t*)(pool_header+1);
    memory_header_t* top;
    size_t align_diff=get_align(body+sizeof(*top), align);
    if (ptr==0)
    {
        ptr=global_ptr=(memory_pool_header_t*)malloc_original(size);
        init_memory_hook(&Malloc,&Free);
    }
    pool_header->size=size;
    pool_header->align=align;
    pool_header->pause=0;
    /* ヘッダーのすぐ後ろのアラインが揃うようにする */
    body+=align_diff; 
    top=pool_header->bottom=pool_header->start_addr=(memory_header_t*)body;
    top->size=size-align_diff-sizeof(*top);
    top->used=0;
    top->next=0;
    top->prev=0;

    pthread_mutex_init(&pool_header->mutex,0);
    
#if MEMORY_DEBUG==1
    pool_header->totoal_used_memory=0;
    pool_header->remain_memory=top->size;
    pool_header->sequence_no=0;
    top->sequence_no=0;
#endif
}
void* memory_pool_malloc(void* ptr, size_t n)
{
    memory_pool_header_t* pool_header=(memory_pool_header_t*)ptr;
    pool_header=pool_header?pool_header:global_ptr;
    memory_header_t* block;
    if (pool_header->pause==1)
    {
        return malloc_original(n);
    }
    __MUTEX_LOCK__
    block=find_unused_block(pool_header, n);
    if (block==0)
    {
        __MUTEX_UNLOCK__
        return 0;
    }
    split_block(pool_header,block, n, pool_header->align);
#if MEMORY_DEBUG==1
    block->backtrace_count=backtrace(block->backtrace,sizeof(block->backtrace));
#endif
    __MUTEX_LOCK__
    return (void*)(block+1);
}
void memory_pool_free(void* ptr, void* data)
{
    memory_pool_header_t* pool_header =(memory_pool_header_t*)ptr;
    if (pool_header->pause==1)
    {
        free_original(data);
        return ;
    }
    __MUTEX_LOCK__
    {
        memory_header_t* header=(memory_header_t*)data-1;
        memory_header_t* prev=header->prev;
        memory_header_t* next=header->next;
        pool_header=pool_header?pool_header:global_ptr;
        header->used=0; 
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
        if (header->next==0)
        {
            pool_header->bottom=header;
        }
    }
    __MUTEX_UNLOCK__
}
void memory_hook_pause(void* ptr)
{
    memory_pool_header_t* pool_header=(memory_pool_header_t*)ptr;
    pool_header=pool_header?pool_header:global_ptr;
    __MUTEX_LOCK__
    pool_header->pause=1;
    __MUTEX_UNLOCK__
}
void memory_hook_resume(void* ptr)
{
    memory_pool_header_t* pool_header =(memory_pool_header_t*)ptr;
    pool_header=pool_header?pool_header:global_ptr;
    __MUTEX_LOCK__
    pool_header->pause=0;
    __MUTEX_UNLOCK__
}
void memory_pool_print(void* ptr)
{
    memory_pool_header_t* pool_header=(memory_pool_header_t*)ptr;
    memory_header_t*p=pool_header->start_addr;
    for (;p!=0;p=p->next)
    {
        MEM_DEBUG_PRINT("%s\n","+++++++++++++++++++++");
        if (p->used==1)
        {
            MEM_DEBUG_PRINT("%d bytes used\n", p->size);
#if MEMORY_DEBUG==1
            {
                int i;
                char** str=backtrace_symbols(p->backtrace, p->backtrace_count);
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
    const size_t n=((ptr_t)addr%align);
    return n?(align-n):0;
}
static memory_header_t* find_unused_block(memory_pool_header_t* pool_header, size_t n)
{
    memory_header_t* p;
    const memory_header_t*bottom=pool_header->bottom;
    if (bottom!=0&&bottom->used==0&&bottom->size>n)
    {
        return pool_header->bottom;
    }
    /* 後ろから検索する方がヒットする確率が高いので */
    for (p=bottom->prev;p!=0;p=p->prev)
    {
        if (p->used==0&&p->size>n)
        {
            return p;
        }
    }
    /* メモリが足りない */
    return 0;
}
static void split_block(memory_pool_header_t* memory_pool_header, memory_header_t* header, size_t n, size_t align)
{
    const size_t diff = header->size-n-sizeof(*header);
    if (diff>0)
    {
        bit8_t* p = (bit8_t*)(header+1)+n;
        size_t align_diff=get_align(p+sizeof(*header), align); 
        if (diff-align>0)
        {
            /* この場合は分割可能 */
            memory_header_t* next=(memory_header_t*)(p+align);
            header->size=n+align;
            next->next=header->next;
            header->next=next;
            next->prev=header;
            next->used=0;
            if (next->next==0)
            {
                memory_pool_header->bottom=next;
            }
            return ;
        }
    }
    /* この場合は分割できないので、ヘッダーのサイズをそのままにしておく */
    header->used=1;
}
static void* Malloc(size_t n)
{
    return memory_pool_malloc(global_ptr,n);
}
static void Free(void* data)
{
    memory_pool_free(global_ptr,data);
}
