/**
* Copyright 2017 (c) WonderPlanet Inc. All Rights Reserved.
* \author dongho.yoo
*/
#include "pool.h"
#include "../malloc_hook.h"
#include <pthread.h>
#include <execinfo.h>
#include <assert.h>

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
    struct tm timestamp;
#endif
}memory_header_t;

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
    bit32_t total_used_memory;
    bit32_t remain_memory;
    bit32_t sequence_no;
#endif
}memory_pool_header_t;
#pragma pack()

static inline size_t get_align(void* addr,size_t align);
static memory_header_t* find_unused_block(memory_pool_header_t* pool_header, size_t n);
static void split_block(memory_pool_header_t* pool_header,memory_header_t* header, size_t n, size_t align);
static void free_block(memory_pool_header_t* pool_header, memory_header_t* block);
static inline const char* get_bytes(char* buffer, size_t n);

#define __MUTEX_LOCK__ \
pthread_mutex_lock(&((memory_pool_header_t*)ptr)->mutex);
#define __MUTEX_UNLOCK__ \
pthread_mutex_unlock(&((memory_pool_header_t*)ptr)->mutex);

void memory_pool_init_struct(void* ptr,size_t size,size_t align)
{
    //printf("sizeof(%zd)\n", sizeof(memory_header_t));
    memory_pool_header_t* pool_header=(memory_pool_header_t*)ptr;
    bit8_t* body=(bit8_t*)(pool_header+1);
    memory_header_t* top;
    size_t align_diff=get_align(body+sizeof(*top), align);
    assert(ptr);
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
    pool_header->total_used_memory=0;
    pool_header->remain_memory=size;
    pool_header->sequence_no=0;
    top->sequence_no=0;
#endif
}
void* memory_pool_malloc(void* ptr, size_t n)
{
    memory_pool_header_t* pool_header=(memory_pool_header_t*)ptr;
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
        assert(0);
        return 0;
    }
    split_block(pool_header,block, n, pool_header->align);
    block->sequence_no=pool_header->sequence_no;
#if MEMORY_DEBUG==1
    block->backtrace_count=backtrace(block->backtrace,sizeof(block->backtrace));
    pool_header->total_used_memory+=(block->size+sizeof(*pool_header));
    pool_header->remain_memory-=(block->size+sizeof(*pool_header));
#endif
    __MUTEX_UNLOCK__
    return (void*)(block+1);
}
void memory_pool_free(void* ptr, void* data)
{
    bit32_t freed_memory=0;
    memory_pool_header_t* pool_header =(memory_pool_header_t*)ptr;
    if (pool_header->pause==1)
    {
        free_original(data);
        return ;
    }
    __MUTEX_LOCK__
    {
        memory_header_t* header=(memory_header_t*)data-1;
        free_block(pool_header, header);
    }
    __MUTEX_UNLOCK__
}
void memory_hook_pause(void* ptr)
{
    memory_pool_header_t* pool_header=(memory_pool_header_t*)ptr;
    __MUTEX_LOCK__
    pool_header->pause=1;
    __MUTEX_UNLOCK__
}
void memory_hook_resume(void* ptr)
{
    memory_pool_header_t* pool_header =(memory_pool_header_t*)ptr;
    __MUTEX_LOCK__
    pool_header->pause=0;
    __MUTEX_UNLOCK__
}
void memory_pool_print(void* ptr, const char* filters[], bit32_t sequence_no)
{
    memory_pool_header_t* pool_header=(memory_pool_header_t*)ptr;
    memory_header_t*p=pool_header->start_addr;
    char temp[32];
    MEM_DEBUG_PRINT("%s\n","> Memory Pool Info");
#if MEMORY_DEBUG==1
    MEM_DEBUG_PRINT("  used: %s\n",get_bytes(temp,pool_header->total_used_memory));
    MEM_DEBUG_PRINT("  remain: %s\n",get_bytes(temp,pool_header->remain_memory));
#endif
    for (;p!=0;p=p->next)
    {
        if (sequence_no!=0&&p->sequence_no!=sequence_no)
        {
           continue; 
        }
        if (p->used==1)
        {
            MEM_DEBUG_PRINT(" 0x%p > %s  used\n", p+1,get_bytes(temp,p->size));
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
        else
        {
            MEM_DEBUG_PRINT(" 0x%p > %s unused\n", p+1,get_bytes(temp,p->size));
        }
    }
}
void memory_pool_set_sequence_no(void* ptr, bit32_t sequence_no)
{
    memory_pool_header_t* pool_header=(memory_pool_header_t*)ptr;
    __MUTEX_LOCK__
    pool_header->sequence_no=sequence_no;
    __MUTEX_UNLOCK__
}
void memory_pool_free_by_sequence_no(void* ptr, bit32_t sequence_no)
{
    memory_pool_header_t* pool_header=(memory_pool_header_t*)ptr;
    memory_header_t* p=pool_header->start_addr;
    __MUTEX_LOCK__
    for (;p!=0;p=p->next)
    {
        if (p->sequence_no==sequence_no)
        {
            free_block(ptr,p);
            p=pool_header->start_addr;
            continue;
        }
    }
    __MUTEX_UNLOCK__
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
    header->used=1;
    if (diff>0)
    {
        bit8_t* p = (bit8_t*)(header+1)+n;
        size_t align_diff=get_align(p+sizeof(*header), align); 
        if (diff-align_diff>0)
        {
            /* この場合は分割可能 */
            memory_header_t* next=(memory_header_t*)(p+align_diff);
            header->size=n+align_diff;
            next->next=header->next;
            header->next=next;
            next->prev=header;
            next->size=diff-align_diff;
            next->used=0;
            if (next->next==0)
            {
                memory_pool_header->bottom=next;
            }
            return ;
        }
    }
    /* この場合は分割できないので、ヘッダーのサイズをそのままにしておく */
}
static void free_block(memory_pool_header_t* pool_header, memory_header_t* block)
{
    memory_header_t* header=block;
    memory_header_t* prev=header->prev;
    memory_header_t* next=header->next;
    bit32_t freed_memory=header->size;
    header->used=0; 
    header->sequence_no=0; 
    if (prev!=0&&prev->used==0)
    {
        /* 上のブロックと統合する */
        prev->next=header->next;
        prev->size+=(header->size+sizeof(*header));
        header=prev;
        freed_memory+=sizeof(*header);
    }
    if (next!=0&&next->used==0)
    {
        /* 下のブロックを統合する*/
        header->next=next->next;
        header->size+=(next->size+sizeof(*header));
        freed_memory+=sizeof(*header);
    }
    if (header->next==0)
    {
        pool_header->bottom=header;
    }
#if MEMORY_DEBUG==1
    pool_header->total_used_memory-=freed_memory;
    pool_header->remain_memory+=freed_memory;
#endif
    ;
}
static inline const char* get_bytes(char* buffer, size_t n)
{
    if (n>=1024*1024)
    {
        sprintf(buffer,"%0.1f Mbytes",(float)n/(1024*1024));
    }
    else if (n>=1024)
    {
        sprintf(buffer,"%0.1f Kbytes",(float)n/1024);
    }
    else
    {
        sprintf(buffer,"%zd bytes",n);
    }
    return buffer; 
}
