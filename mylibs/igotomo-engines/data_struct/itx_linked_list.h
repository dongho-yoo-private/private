/**
* Copyright (c) 2015 Igotomo.
* authorized by dongho.yoo.
*/
#ifndef _IGX_LLIST_H_
#define _IGX_LLIST_H_

#include "igx_types.h"
#include "thread/igx_thread.h"
#include "library/igx_memory.h"
#include "library/igx_mem_page.h"
#include "debug/igx_debug.h"
#include "itx_node.h"
#include <stdlib.h>
#include <string.h>
//#include <cstddef>

/**@brief 初期割り当てるデフォルトのリストの数*/
#define ITX_LLIST_INITIAL_ELEMENT_COUNT (32)
#define ITX_DEFAULT_PAGE_SIZE (ITX_LLIST_INITIAL_ELEMENT_COUNT*sizoef(ItxNode)+sizeof(void*))
#define ITX_LLIST_FREE(a) IGX_FREE(a)
#define ITX_LLIST_MALLOC(a) IGX_MALLOC(a, "ITX_LLIST")
#define ITX_MEMCPY_HI_SPEED_SIZE (256)
#define IGX_LLIST_COPY_DATA(a, b) \
if (a!=0)\
{\
if (sizeof(T)<ITX_MEMCPY_HI_SPEED_SIZE)\
{\
    *(a)=*(b);\
}\
else\
{\
    memcpy(a, b, sizeof(T));\
}\
}

/**
* @brief Linked List クラス
*/
template <class T> 
class ItxLinkedList 
{
public:
    ItxLinkedList();
    /**
    * @brief コンストラクター
    * @param count_per_page 一ページに入るエレメントの数。\n
    * ItxLinkedListはページ単位でメモリを割り当てます。-1はデフォルト値 \ref ITX_LLIST_INITIAL_ELEMENT_COUNT 
    * @param max_element_size 最大のサイズ-1の場合は無限
    * @param is_create_mutex  ミューテックスOn/OFF
    * @param deallocator メモリから削除時のコールバック
    */
    ItxLinkedList(bit32_t count_per_page, bit32_t max_element_count=-1, 
                        bool_t is_create_mutex=false, void (*deallocator)(T)=0)
    : m_count(0), m_top(0), m_bottom(0), m_use_mutex(is_create_mutex), m_page_top(0), m_mem_page(0)
    {
        if (is_create_mutex==true)
        {
            igx_mutex_init(&m_mutex);
        }
        memset(&m_memory, 0, sizeof(m_memory));
        m_memory.max_element_count=max_element_count;
        m_memory.deallocator = deallocator;
        m_memory.count_per_page = (count_per_page==-1)?ITX_LLIST_INITIAL_ELEMENT_COUNT:count_per_page;
        if (max_element_count!=-1)
        {
            m_memory.count_per_page = (max_element_count<m_memory.count_per_page)?max_element_count:m_memory.count_per_page;
        }
    }
    /**
    * @brief コンストラクター
    * @param buffer 内部で使われる固定バッファー
    * @param size   バッファーのサイズ
    * @param is_create_mutex ミューテックスOn/Off
    */
    ItxLinkedList(void* buffer, size_t buffer_size, bool_t is_create_mutex=false)
    : m_count(0), m_top(0), m_bottom(0), m_use_mutex(is_create_mutex), m_page_top(0),m_mem_page(0)
    {
        if (is_create_mutex==true)
        {
            igx_mutex_init(&m_mutex);
        }
        memset(&m_memory, 0, sizeof(m_memory));
        m_page_top = (struct page*)buffer;
        memset(buffer, 0, buffer_size);
        //IGX_ASSERT(0);
        m_memory.max_element_count=((buffer_size-sizeof(struct page*))/sizeof(*m_top));
        m_memory.count_per_page = m_memory.max_element_count;
        m_page_top->element_count = m_memory.count_per_page;
    }
    /**
    * @brief メモりページを利用してメモリを割り当てます。
    *
    * @param count_per_page 一ページあたりのエレメントの数
    */
    ItxLinkedList(bit32_t count_per_page, IgxMemPage* mem_page, bool_t is_create_mutex=false, void (*deallocator)(T)=0)
    :  m_mem_page(mem_page), m_count(0), m_top(0), m_bottom(0), m_use_mutex(is_create_mutex), m_page_top(0)
    {
        //ItxLinkedList<T>::ItxLinkedList(count_per_page, -1, is_create_mutex, deallocator);
        if (is_create_mutex==true)
        {
            igx_mutex_init(&m_mutex);
        }
        memset(&m_memory, 0, sizeof(m_memory));
        m_memory.max_element_count=-1;
        m_memory.deallocator = deallocator;
        m_memory.count_per_page = (count_per_page==-1)?ITX_LLIST_INITIAL_ELEMENT_COUNT:count_per_page;

    }
    /**
    * @brief デストラクター
    */
    virtual ~ItxLinkedList()
    {
        Clear();
        if (m_memory.is_allocated==true && m_top!=0)
        {
            struct page* p=m_page_top;
            while(p!=0) p=p->next_page, ITX_LLIST_FREE(p);
        }
        if (m_use_mutex==true)
        {
            igx_mutex_term(&m_mutex);
        }
    }
    /**
    * @brief リストの数を取得します。
    */
    inline ubit32_t Count()
    {
        return m_count;
    }
    /** 
    * @brief mutex lock
    */
    inline bool_t lock()
    {
        if (m_use_mutex==true)
        {
            return igx_mutex_lock(&m_mutex);
        }
        return false;
    }
    /** 
    * @brief mutex unlock
    */
    inline void unlock()
    {
        if (m_use_mutex==true)
        {
            igx_mutex_unlock(&m_mutex);
        }
    }
    /** 
    * @brief 初期化を行います。　
    * @return trueは成功、falseは失敗
    */
    bool_t Init();
    /**
    * @brief 後ろからのインデックスの後ろに追加します。
    * @brief indexの前に追加します。
    * @param data データ
    *
    * @return データの数
    */
    bit32_t Add(const T& data, ubit32_t index=0);
    /**
    * @brief indexの前に挿入します。
    * @param data データ
    *
    * @return データの数
    */
    bit32_t Insert(const T& data, ubit32_t index=0);
    /**
    * @brief トップからindex番目のデータを取得します。
    * @param data データ
    * @return ItxNode
    *
    * @startcode
    * for (ItxNode<AnyData>* node = list.GetTopFrom();node;++node)
    * {
    *     AnyData& data = *node;
    *                   :
    *                   :
    * }
    * @endcode
    */
    ItxNode<T>* GetTopFrom(bit32_t index=0);
    /**
    * @brief 後ろからindex番目のデータを取得します。
    * @param data データ
    *
    * @return ItxNode
    */
    ItxNode<T>* GetBottomFrom(bit32_t index=0);
    /**
    * @brief 前からindex番目のデータを削除します。
    *
    * @param index インデックス
    * @param out 削除されるデータ、ゼロの場合は無視されます。
    * @return 残りのデータの数(-1は空っぽ)
    */
    bit32_t RemoveTopFrom(bit32_t index, T* out=0);
    /**
    * @brief 前からindex番目のデータを削除します。
    *
    * @param index インデックス
    * @param out 削除されるデータ、ゼロの場合は無視されます。
    * @return 残りのデータの数 (-1は空っぽ)
    */
    bit32_t RemoveBottomFrom(bit32_t index, T* out=0);
    /**
    * @brief 指定したノードを削除します。
    * @param t ノード
    * @return 残りのデータの数 (-1は空っぽ)
    */
    bit32_t RemoveNode(ItxNode<T>* t);
    /** 
    * @brief 全データを削除します。
    */
    void Clear();
    /**
    * @brief ソート
    * @param ソート関数
    *
    * @startcode
    * bit32_t AnySortFunction(const AnyType& t1, const AnyType& t2);
    *
    * list.Sort(&AnySortFunction);
    * @endcode
    */
    void Sort(bit32_t (*sort_func)(const T&, const T&));

protected:
    ubit32_t m_count;
private:
    ItxNode<T>* m_top;
    ItxNode<T>* m_bottom;
    struct page {
        struct page* next_page;
        ubit32_t element_count;
        ItxNode<T> nodes[1];
    } *m_page_top;
    struct {
        bool_t is_allocated;
        ubit32_t allocation_size;
        bit32_t  max_element_count;
        bit32_t  count_per_page;
        void (*deallocator)(T);
    } m_memory;

    igx_mutex_t m_mutex;
    bool_t m_use_mutex;
    IgxMemPage* m_mem_page;

    struct page* create_page(ubit32_t element_count);
    ItxNode<T>* get_brank_node();
    ubit32_t remove_node(ItxNode<T>* node);
    
};

#if 0
#define ITX_LLIST_MUTEX_LOCK \
if (m_use_mutex!=0) igx_mutex_lock(&m_mutex)
#define ITX_LLIST_MUTEX_UNLOCK \
if (m_use_mutex!=0) igx_mutex_lock(&m_mutex)
#else
#define ITX_LLIST_MUTEX_LOCK 
#define ITX_LLIST_MUTEX_UNLOCK 
#endif

template <typename T>
bool_t ItxLinkedList<T>::Init()
{
    return true;
}
template <typename T>
bit32_t ItxLinkedList<T>::Add(const T& data, ubit32_t index/*=0*/)
{
    ITX_LLIST_MUTEX_LOCK;
    ItxNode<T>* brank_node = get_brank_node();

    if (brank_node==0)
    {
        // 何かの制限にかかっている。
        ITX_LLIST_MUTEX_UNLOCK;
        return -1;
    }
    
    IGX_LLIST_COPY_DATA(&brank_node->data, &data);

    ItxNode<T>* node = GetBottomFrom(index);

    if (node!=0)
    {
        ItxNode<T>* next=node->next;

        m_bottom=(node==m_bottom)?brank_node:m_bottom;
        node->next=brank_node;
        brank_node->prev=node;
        brank_node->next=next;

        if (next!=0)
        {
            next->prev=brank_node;
        }
    }
    else
    {
        m_bottom=m_top=brank_node;
    }
    
    ITX_LLIST_MUTEX_UNLOCK;
    return ++m_count;
}
template <typename T>
bit32_t ItxLinkedList<T>::Insert(const T& data, ubit32_t index/*=0*/)
{
    ITX_LLIST_MUTEX_LOCK;
    ItxNode<T>* brank_node = get_brank_node();
   
    if (brank_node==0)
    {
        // 何かの制限にかかっている。
        ITX_LLIST_MUTEX_UNLOCK;
        return -1;
    }

    IGX_LLIST_COPY_DATA(&brank_node->data, &data);

    if (index>=m_count)
    {
        Add(data);
        ITX_LLIST_MUTEX_UNLOCK;
        return m_count;
    }

    ItxNode<T>* node = GetTopFrom(index);
    if (node!=0)
    {
        ItxNode<T>* prev=node->prev;

        m_top=(node==m_top)?brank_node:m_top;
        node->prev=brank_node;
        brank_node->next=node;
        brank_node->prev=prev;

        if (prev)
        {
            prev->next=brank_node;
        }
    }
    else
    {
        m_bottom=m_top=brank_node;
    }
    
    ITX_LLIST_MUTEX_UNLOCK;
    return ++m_count;
}
template <typename T>
ItxNode<T>* ItxLinkedList<T>::GetTopFrom(bit32_t index/*=0*/)
{
    if (m_top==0)
    {
        IGX_WARN("m_top is null!!");
        IGX_ASSERT(m_count==0);
        return 0;
    }
    ITX_LLIST_MUTEX_LOCK;
    bit32_t i;
    ItxNode<T>* node = m_top;
    for (i=0; i<index&&node!=0; ++i) node=node->next;
    ITX_LLIST_MUTEX_UNLOCK;
    return (i==index)?node:0;
}
template <typename T>
ItxNode<T>* ItxLinkedList<T>::GetBottomFrom(bit32_t index/*=0*/)
{
    if (m_bottom==0)
    {
        return 0;
    }
    ITX_LLIST_MUTEX_LOCK;
    bit32_t i;
    ItxNode<T>* node = m_bottom;
    for (i=0; i<index&&node!=0; ++i) node=node->prev;
    ITX_LLIST_MUTEX_UNLOCK;
    return (i==index)?node:0;
}
template <typename T>
bit32_t ItxLinkedList<T>::RemoveTopFrom(bit32_t index, T* out/*=0*/)
{
    if (m_count==0)
    {
       return -1; 
    }
    ItxNode<T>* node = GetTopFrom(index);
    IGX_ASSERT(node!=0);
    IGX_LLIST_COPY_DATA(out, &node->data);
    bit32_t ret = remove_node(node);

    return ret;
}
template <typename T>
bit32_t ItxLinkedList<T>::RemoveBottomFrom(bit32_t index, T* out/*=0*/)
{
    if (m_count==0)
    {
       return -1; 
    }
    ItxNode<T>* node = GetBottomFrom(index);
    IGX_LLIST_COPY_DATA(out, &node->data);
    bit32_t ret = remove_node(node);

    return ret;
}
template <typename T>
bit32_t ItxLinkedList<T>::RemoveNode(ItxNode<T>* node)
{
    return remove_node(node);
}
template <typename T>
void ItxLinkedList<T>::Clear()
{
    while(RemoveTopFrom(0)>=0);
}
template <typename T>
ItxNode<T>* ItxLinkedList<T>::get_brank_node()
{
    struct page* last_page=0;
    ubit32_t page_count=0;
    ubit32_t element_count=0;

    for (struct page* curr = m_page_top; curr; curr=curr->next_page, ++page_count)
    {
        const int count = curr->element_count;
        last_page=curr;
        element_count+=count;
        for (int i=0;i<count;++i)
        {
            if ((curr->nodes[i].next==0)&&(curr->nodes[i].prev==0))
            {
                if ((curr->nodes+i)==m_top||(curr->nodes+i)==m_bottom)
                {
                    /* この場合はノードが一個のみ残っている場合*/
                    continue;
                }
                return &curr->nodes[i];
            }
        }
    }
    if (element_count==m_memory.max_element_count)
    {
        // 上限に達した場合
        IGX_WARN("capacity is over!! %d\n", element_count);
        IGX_ASSERT(0);
        return 0;
    }

    // 以下のケースは全て割り当てるベキ。
    const bit32_t remain_element_count = (m_memory.max_element_count-element_count);
    element_count = (remain_element_count>m_memory.count_per_page)?m_memory.count_per_page:remain_element_count;
    element_count=(m_memory.max_element_count==-1)?m_memory.count_per_page:remain_element_count;

    struct page* new_page = create_page(element_count);
    if (last_page!=0)
    {
        last_page->next_page=new_page;
    }
    else
    {
        m_page_top = new_page;
    }

    return new_page->nodes;
}
template <typename T>
ubit32_t ItxLinkedList<T>::remove_node(ItxNode<T>* node)
{
    ItxNode<T>* prev=node->prev;
    ItxNode<T>* next=node->next;
    node->next=0;
    node->prev=0;

    if (prev!=0)
    {
        prev->next=next;
    }
    if (next!=0)
    {
        next->prev=prev;
    }
    m_top = (node==m_top)?next:m_top;
    m_bottom = (node==m_bottom)?prev:m_bottom;

    return --m_count;
}
template <typename T>
struct ItxLinkedList<T>::page* ItxLinkedList<T>::create_page(ubit32_t element_count)
{
    ubit32_t page_size = ((element_count-1)*sizeof(ItxNode<T>)+sizeof(struct page));

    struct page* new_page;
    if (m_mem_page!=0)
    {
        new_page = (struct page*)pmalloc(m_mem_page, page_size);
    }
    else
    {
        new_page  = (struct page*)ITX_LLIST_MALLOC(page_size);
        m_memory.is_allocated=true;
    }
    memset(new_page, 0, sizeof(*new_page));
    new_page->element_count=element_count;

    return new_page;
}
template <typename T>
void ItxLinkedList<T>::Sort(bit32_t (*sort_func)(const T&, const T&))
{
    for (ItxNode<T>* node=m_top;node; node=node->next)
    {
        bool_t is_changed=false;
        for (ItxNode<T>* sub_node=node; sub_node->next!=0; sub_node=sub_node->next)
        {
            if (sort_func(sub_node->data, sub_node->next->data)>0)
            {
                ItxNode<T>* next=sub_node->next;
                ItxNode<T>* prev=sub_node->prev;
                sub_node->next=sub_node->next->next;
                sub_node->prev=sub_node->next->prev;
                sub_node->next->next=next;
                sub_node->next->prev=prev;
                is_changed=true;
            }
        }
        
        if (is_changed==false)
        {
            // これ以上、ソートする必要がない。
            break;
        }
    }
}


/**
* @brief バッファーを利用したLinkedList ちょっとしたアルゴリズム的にリストが必要な場合使います。
*
* @startcode
* ItxLinkedListBuffer<int, MAX_ANY_COUNT> list(false);
*
* for (int i=0; i<MAX_ANY_COUNT; ++i)
  {
            :
            :
      list.Add(any_value);
  }
* @endcode
*/
template <typename T, size_t SIZE> 
class ItxLinkedListBuffer : public ItxLinkedList<T>
{
public:
    /**
    * @brief コンストラクター
    * @param buffer 内部で使われる固定バッファー
    * @param size   バッファーのサイズ
    * @param is_create_mutex ミューテックスOn/Off
    */
    ItxLinkedListBuffer (bool_t use_mutex=false)
    :ItxLinkedList<T>::ItxLinkedList(m_buffer, sizeof(m_buffer), use_mutex){}
    /**
    * @brief デストラクター
    */
    virtual ~ItxLinkedListBuffer (){}
private:
    char m_buffer[sizeof(ItxNode<T>)*(SIZE)+sizeof(void*)+sizeof(ubit32_t)];
};

/**
* @brief マクロ
* @startcode
    ITX_LLIST_FOR_EACH_BEGIN(list, any_type_t, value)
        if (value==any_value)
        {
            :
            :
        }
    ITX_LLIST_FOR_EACH_END
* @endcode
**/
#define ITX_LLIST_FOR_EACH_BEGIN(list, type, value) \
for (ItxNode<type>*node=list.GetTopFrom();node;node=node->next)\
{\
type value = **node;
    

#define ITX_LLIST_FOR_EACH_END }

#endif
