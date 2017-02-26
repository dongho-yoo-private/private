#include "igx_mem_page.h"
#include "igx_memory.h"
#include "thread/igx_thread.h"
#include "debug/igx_debug.h"

#ifdef __POSIX__
#   include <unistd.h>
#   include <stdio.h>
#endif

#define ITX_PMALLOC(size) malloc(size) 
#define ITX_PFREE free

/**@brief デフォルトのアライメントは8でよい？*/
#define ITX_PMALLOC_ALIGN (8)

void* operator new(size_t size, IgxMemPage* handle, const char* comment) 
{
   return pmalloc(handle, size);
}
void* operator new[](size_t size, IgxMemPage* handle, const char* comment) 
{
    return pmalloc(handle, size);
}
void operator delete(void* pv, IgxMemPage* handle, const char* comment) 
{
    // 何もしなくてよい。
}
void operator delete[](void* pv, IgxMemPage* handle, const char* comment) 
{
    // 何もしなくてよい。
}
void* pmalloc(IgxMemPage* handle,  size_t size)
{
    if (handle!=0)
    {
        return handle->Pop(size);
    }
    return igx_malloc(size, "pmalloc");
}
void pfree(IgxMemPage* handle, void* mem)
{
    if (handle!=0)
    {
        handle->Release(mem);
        return ;
    }
    igx_free(mem);
}
static ubit8_t* get_aligned_memory(ubit8_t* p, bit32_t align, ubit32_t* moved_offset)
{
    ubit32_t ret = ((uintptr_t)p%align);
   
    if (ret==0)
    {
        if (moved_offset!=0)
        {
            *moved_offset=0;
        }
        return p;
    }
    
    ret=align-ret; 
    if (moved_offset!=0)
    {
        *moved_offset=ret;
    }
    return p+ret;
}
IgxMemPage::IgxMemPage(char* buffer, size_t buffer_size, const igx_mem_page_option_t* option/*=0*/)
: m_page_top(0), m_page_end(0)
{
    //IGX_ASSERT(0);
    igx_mutex_init(&m_mutex);
    memset(&m_flags, 0, sizeof(m_flags));

    m_flags.alignment = (option!=0)?option->alignment:ITX_PMALLOC_ALIGN;
    m_flags.use_heap =  (option!=0)?option->additional_allocation:false;
    m_flags.max_page_count = 0xFF;

#ifdef ITX_MEM_PAGE_DEBUG
    m_allocated_page_count=0;
    IgxMemPage::Register(this);
#endif
    Initialize(buffer, buffer_size, m_flags.use_heap, m_flags.alignment);
}
IgxMemPage::IgxMemPage(bit32_t page_count/*=-1*/, const igx_mem_page_option_t* option/*=0*/)
: m_page_top(0), m_page_end(0)
{
    const size_t page_size = igx_get_page_size();

    //IGX_ASSERT(0);
    igx_mutex_init(&m_mutex);
    memset(&m_flags, 0, sizeof(m_flags));
    m_flags.alignment = (option!=0)?option->alignment:ITX_PMALLOC_ALIGN;
    m_flags.max_page_count=(bit8_t)page_count;

#ifdef ITX_MEM_PAGE_DEBUG
    m_allocated_page_count=0;
    IgxMemPage::Register(this);
#endif
    Initialize(page_count, option?option->alignment:ITX_PMALLOC_ALIGN);
}
IgxMemPage::~IgxMemPage()
{
    igx_mutex_term(&m_mutex);
    //IGX_ASSERT(0); 
#ifdef ITX_MEM_PAGE_DEBUG
    IgxMemPage::UnRegister(this);
#endif
}

bool_t IgxMemPage::Initialize(char* buffer, size_t buffer_size, bool_t is_allocation, ubit8_t alignment)
{
    if (m_page_top!=0 ||
            m_page_end!=0)
    {
        return false;
    }
    ubit32_t offset;
    m_page_top=m_page_end=(struct page*)buffer;
    m_page_top->next=0;
    m_page_top->indicator = get_aligned_memory(m_page_top->data, alignment, &offset);
    m_page_top->remain_memory= buffer_size-(sizeof(struct page)-4)-(size_t)offset;
    m_flags.page_count=1;
    m_flags.alignment=alignment;
    m_flags.use_heap=is_allocation;
    m_flags.max_page_count=(m_flags.use_heap==true)?0xFF:1;
    m_flags.first_page_external=true;
    m_flags.use_heap=is_allocation;
    
    IGX_ASSERT(m_page_top->remain_memory>0);
    return true;
}
bool_t IgxMemPage::Initialize(size_t page_count/*=-1*/, ubit8_t alignment)
{
    if (m_page_top!=0 ||
            m_page_end!=0)
    {
        return false;
    }
    m_flags.page_count=0; 
    m_flags.max_page_count=page_count;
    m_flags.alignment=alignment;
    m_flags.first_page_external=false;
    m_flags.use_heap=true;

    return true;

}
size_t IgxMemPage::get_page_data_size()
{
    ubit32_t offset;
    char* p = (char*)get_aligned_memory(m_page_top->data, m_flags.alignment, &offset);
    const size_t page_size = igx_get_page_size();
    return (page_size-(sizeof(struct page)-4)-offset-igx_mem_header_size());
}
/* この関数の中で最終的に実際に割り当てるサイズを決定する。
   その理由はプールのサイズをページサイズにする際にページこととり、
   メモリのフラグメンテーションをなくすためである。 */
struct IgxMemPage::page* IgxMemPage::create_page(size_t size)
{
    /* エラーチェック */
    if (m_flags.max_page_count!=0xFF &&
            m_flags.max_page_count==m_flags.page_count)
    {
        IGX_WARN("arrived at max page count (%d)", m_flags.page_count);
        return 0;
    }
 
    /* 必要なサイズは、ヘッダー+アライメントのサイズ+データサイズ */
    const size_t header_size     = (sizeof(*m_page_top)-sizeof(m_page_top->data));
    const size_t need_size       = header_size+size+(m_flags.alignment-1);
    const size_t page_size       = igx_get_page_size();
    const size_t need_page_count = (need_size/page_size)+((need_size%page_size)?1:0);
    const size_t allocated_size  = need_page_count*page_size;

    /* メモリをページ単位で割り当てた */
    struct page* new_page = (struct page*)ITX_PMALLOC(allocated_size);
    IGX_ASSERT(new_page);
    
    ubit32_t offset;
    new_page->indicator     = get_aligned_memory(new_page->data, m_flags.alignment, &offset);
    new_page->remain_memory = allocated_size-header_size-offset;//-size;
    new_page->next = 0;

    //IGX_DEBUG("create_page: size:%zd, allocated_size:%zd, remain:%zd, page_size:%zd, need_page_count:%zd",
    //        size, allocated_size, new_page->remain_memory, page_size, need_page_count);

    if (m_page_end==0)
    {
        m_page_top=m_page_end=new_page;
    }
    else
    {
        m_page_end->next=new_page;
        m_page_end=new_page;
    }
    ++m_flags.page_count;

#ifdef ITX_MEM_PAGE_DEBUG
    m_allocated_page_count+=need_page_count;
#endif

    return new_page;
}
void* IgxMemPage::Pop(size_t size)
{
    struct page* p = m_page_top;
    const size_t need_size = (size+m_flags.alignment-1);
    const size_t page_size = igx_get_page_size();
    void* ret;


    IGX_ASSERT(size!=1);

    /* 割り当てるサイズがデフォルトページサイズより大きい場合は、
       ページサイズの倍数で割り当てる。 
       そうすることによって、連続したページ空間を確保できる。 */
    if (need_size>page_size)
    {
        ubit32_t offset;
        igx_mutex_lock(&m_mutex);
        struct page* p = create_page(need_size);
        ret = p->indicator;
        p->indicator = (ubit8_t*)get_aligned_memory((ubit8_t*)ret+size, m_flags.alignment, &offset);
        p->remain_memory -= (offset+size);
        p->remain_memory = (p->remain_memory<0)?0:p->remain_memory;
        igx_mutex_unlock(&m_mutex);
        return ret;
    }

    igx_mutex_lock(&m_mutex);
    if (p==0)
    {
        p=m_page_end=m_page_top = create_page(need_size);
        IGX_ASSERT(p);
    }

    for (;p;p=p->next)
    {
        /* 残りのメモリが足りている場合 */
        if (p->remain_memory>=size)
        {
            ubit32_t offset;
            ret = p->indicator;
            p->indicator = (ubit8_t*)get_aligned_memory((ubit8_t*)ret+size, m_flags.alignment, &offset);
            p->remain_memory -= (offset+size);
            p->remain_memory = (p->remain_memory<0)?0:p->remain_memory;

            igx_mutex_unlock(&m_mutex);
            return ret;
        }
        //getchar();
    }

    /* 割り当てるページが見つからなかった場合 */
    if (p==0)
    {
        ubit32_t offset;
        struct page* p = create_page(size);
        if (p==0)
        {
            igx_mutex_unlock(&m_mutex);
            return 0;
        }
        ret = p->indicator;
        p->indicator = get_aligned_memory(p->indicator+size, m_flags.alignment, &offset);
        p->remain_memory -= (offset+size);
        p->remain_memory = (p->remain_memory<0)?0:p->remain_memory;
    }

    igx_mutex_unlock(&m_mutex);

    return ret;
}
void IgxMemPage::Release(void* data)
{
#if 0
    struct page* page = find_page(data);
    if (page==NULL)
    {
        /* ??? */
        IGX_ASSERT(0);
        return ;
    }
#endif
    return ;
}
void IgxMemPage::Clear()
{
    if (m_flags.use_heap == true)
    {
        struct page* p = m_page_top;
        
        p = (m_flags.first_page_external==true)?m_page_top->next:p;
        for (;p;)
        {
            struct page* temp = p->next;
            ITX_PFREE(p); 
            p=temp;
        }
        if (m_flags.first_page_external==true)
        {
            m_page_top->next=0;
        }
    }
    else
    {
        if (m_flags.first_page_external!=true)
        {
            m_page_top=m_page_end=0;
        }
    }

#ifdef ITX_MEM_PAGE_DEBUG
    m_allocated_page_count=0;
#endif
    /* おそらくここはクリアーしなくてもよい。
    m_page_size=0;
    m_flags.page_count=0;
    m_flags.max_page_count=0;
    m_flags.use_heap=false;
    */
}

#ifdef ITX_MEM_PAGE_DEBUG
#include "data_struct/itx_linked_list.h"

/**
* デバッグ用。
*/
static ItxLinkedListBuffer<IgxMemPage*, 256> ___mem_page_list___;

/**
* @brief 割り当てられたページのメモリの使用状況を出力します。
*/
void IgxMemPage::PrintStatus()
{
    IGX_DEBUG("++++++++++++++++++++++++++++++++++++++++++++++++++++");
    IGX_DEBUG("IgxMemPage used count: %d", ___mem_page_list___.Count());
    bit32_t total=0, total_page=0;
    ITX_LLIST_FOR_EACH_BEGIN(___mem_page_list___, IgxMemPage*, mem_page)
        IGX_DEBUG("%p: mem:%d bytes, page:%d", mem_page, mem_page->GetUsingMemorySize(), mem_page->GetUsingPageCount());
        total+=mem_page->GetUsingMemorySize();
        total_page+=mem_page->GetUsingPageCount();
    ITX_LLIST_FOR_EACH_END
    IGX_DEBUG("IgxMemPage toal %dbytes, page: %d", total, total_page);
}

/**
* デバッグ用。
*/
void IgxMemPage::Register(IgxMemPage* mem_page)
{
    ___mem_page_list___.Add(mem_page);
}
/**
* デバッグ用。
*/
void IgxMemPage::UnRegister(IgxMemPage* mem_page)
{
    ItxNode<IgxMemPage*>* node=___mem_page_list___.GetTopFrom();
    for (;node;node=node->next)
    {
        if (mem_page==**node)
        {
           ___mem_page_list___.RemoveNode(node);
            return;
        }
    }
}

extern "C" {
HITX igx_new_mem_page(char* buffer, size_t buffer_size, bit32_t alignment, bool_t additional_buffer)
{
    igx_mem_page_option_t opt = {additional_buffer, false, static_cast<ubit8_t>(alignment)};
    IgxMemPage* mem;
    
    if (buffer!=0)
    {
        mem = IGX_NEW("igx_new_mem_page") IgxMemPage(buffer, buffer_size, &opt);
    }
    else
    {
        mem = IGX_NEW("igx_new_mem_page") IgxMemPage(-1, &opt);
    }
    return (HITX)mem;
}
void igx_clear_mem_page(HITX handle)
{
    IgxMemPage* mem = (IgxMemPage* )handle;
    mem->Clear();
}
void igx_delete_mem_page(HITX handle)
{
    IgxMemPage* mem = (IgxMemPage* )handle;
    mem->Clear();
    mem->~IgxMemPage();
    IGX_DEL(mem);
}
void* pmalloc_c(HITX handle, size_t size)
{
    IgxMemPage* mem = (IgxMemPage* )handle;
    return mem->Pop(size);
}

void igx_mem_page_print_status()
{
    IgxMemPage::PrintStatus();
}
}


#endif
