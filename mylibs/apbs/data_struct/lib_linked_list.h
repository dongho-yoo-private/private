/**
* Copyright (c) 2016 Applibot Inc.
* authorized by dongho.yoo.
*/
#ifndef _LIB_LINKED_LIST_H_
#define _LIB_LINKED_LIST_H_

#include "library/library_types.h"
#include "lib_iterator.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
//#include <cstddef>

/**\brief 初期割り当てるデフォルトのリストの数*/
#define kLListInitialElementCount (32)
/**\brief メモリ上のデフォルトのサイズ。内訳はデータの数分のメモリと次のページを持つポインター*/
#define kDefaultPageSize (kListInitialElementCount*sizeof(Iterator<T>)+sizeof(void*))
/** 
* リスト生成時にmutexをONにした場合、
* Add、Insert、Remove時に自動で排他処理をする場合は、以下のマクロを1にすること。
* 以下のマクロが向こうの状態では、Lock、Unlockを手動で呼ぶ必要がある。
* デフォルトでは手動で。
*/
#define _LLIST_AUTO_LOCK_ 0
#define _LLIST_FREE(a) free(a)
#define _LLIST_MALLOC(a) malloc(a)
/* Linux上のgnuのlibcでは、256バイトからsseのような128bitレジスターを使い始める。
なので、Androidは256バイトだと思うのだが、iOSはわからない。実験が必要かも。*/
#ifdef CC_PLATFORM_IOS
#   define kMemoryHighSppedSize (256)
#else
#   define kMemoryHighSppedSize (256)
#endif
#define _LLIST_COPY_DATA(a, b) \
if (a!=0)\
{\
if (sizeof(T)<kMemoryHighSppedSize)\
{\
    *(a)=*(b);\
}\
else\
{\
    memcpy(a, b, sizeof(T));\
}\
}

namespace lib {
/**
* \brief Linked List クラス
*/
template <typename T>
class LinkedList 
{
public:
    /**\brief イテレータ */
    typedef Iterator<T> iterator;
public:
    LinkedList();
    /**
    * \brief コンストラクター(動的メモリ確保用)
    * \param count_per_page 一ページに入るエレメントの数。\n
    * LinkedListはページ単位でメモリを割り当てます。-1はデフォルト値 \ref kLListInitialElementCount 
    * \param max_element_size 最大のサイズ-1の場合は無限
    * \param is_create_mutex  ミューテックスOn/OFF
    * \param deallocator メモリから削除時のコールバック
    */
    LinkedList(S32 count_per_page, S32 max_element_count=-1, 
                        Bool is_create_mutex=false, void (*deallocator)(T)=0)
    : count_(0), top_(0), bottom_(0), use_mutex_(is_create_mutex), page_top_(0)
    {
        if (is_create_mutex==true)
        {
            pthread_mutex_init(&mutex_, NULL);
        }
        memset(&memory_, 0, sizeof(memory_));
        memory_.max_element_count=max_element_count;
        memory_.deallocator = deallocator;
        memory_.count_per_page = (count_per_page==-1)?kLListInitialElementCount:count_per_page;
        if (max_element_count!=-1)
        {
            memory_.count_per_page = (max_element_count<memory_.count_per_page)?max_element_count:memory_.count_per_page;
        }
        
        node_end_.prev=NULL;
        node_end_.next=NULL;
        top_=&node_end_;
        bottom_=&node_end_;
    }
    /**
    * \brief コンストラクター(固定メモリ用)
    * \param buffer 内部で使われる固定バッファー
    * \param size   バッファーのサイズ
    * \param is_create_mutex ミューテックスOn/Off
    */
    LinkedList(void* buffer, size_t buffer_size, Bool is_create_mutex=false)
    : count_(0), top_(0), bottom_(0), use_mutex_(is_create_mutex), page_top_(0)
    {
        if (is_create_mutex==true)
        {
            pthread_mutex_init(&mutex_, NULL);
        }
        memset(&memory_, 0, sizeof(memory_));
        page_top_ = (struct page*)buffer;
        memset(buffer, 0, buffer_size);
        //JOKER_ASSERT(0);
        memory_.max_element_count=((buffer_size-sizeof(struct page*))/sizeof(*top_));
        memory_.count_per_page = memory_.max_element_count;
        page_top_->element_count = memory_.count_per_page;
        
        node_end_.prev=NULL;
        node_end_.next=NULL;
        /*top_=&node_end_;
        bottom_=&node_end_;*/
        top_=NULL;
        bottom_=NULL;
    }
    /**
    * \brief デストラクター
    */
    virtual ~LinkedList()
    {
        Clear();
        if (memory_.is_allocated==true && top_!=0)
        {
            struct page* p=page_top_;
            while(p!=0) p=p->next_page, _LLIST_FREE(p);
        }
        if (use_mutex_==true)
        {
            pthread_mutex_destroy(&mutex_);
        }
    }
    /**
    * \brief リストの数を取得します。
    */
    inline U32 Count();
    /**
    * \brief リストの数を取得します。
    */
    inline U32 Count() const;
    /** 
    * \brief mutex lock
    */
    inline Bool Lock();
    /** 
    * \brief mutex unlock
    */
    inline void UnLock();
    /** 
    * \brief mutex lock
    */
    inline Bool Lock() const;
    /** 
    * \brief mutex unlock
    */
    inline void UnLock() const;
    /**
    * \brief 初期化を行います。　
    * @return trueは成功、falseは失敗
    */
    Bool Init();
    /**
    * \brief 後ろからのインデックスの後ろに追加します。
    * \brief indexの前に追加します。
    * \param data データ
    *
    * @return データの数
    */
    S32 Add(T data, U32 index=0);
    /**
    * \brief indexの前に挿入します。
    * \param data データ
    *
    * @return データの数
    */
    S32 Insert(T data, U32 index=0);
    /**
    * \brief index番目のデータを取得します。
    * \param index インデックス
    * \return データ
    */
    T& operator[](S32 index);
    T& operator[](S32 index) const;
    /**
    * \brief トップからindex番目のデータを取得します。
    * \param index トップからのindex
    * \return \ref lib::Iterator<T>
    *
    * \startcode
    * lib::LinkedList<TAnyType> list;
    * int n;
    *         :
    *         :
    *         :
    * for (lib::LinkedList::iterator node = list.begin(n);list.end()!=node;--node)
    * {
    *     TAnyType& data = *node;
    *                   :
    *                   :
    * }
    * \endcode
    */
    T& GetTopFrom(S32 index=0);
    T& GetTopFrom(S32 index=0) const;
    /**
    * \brief GetTopFromのindex=0時と同じ
    * \return \ref lib::Iterator<T> 
    *
    * \startcode
    * lib::LinkedList<TAnyType> list;
    *         :
    *         :
    *         :
    * for (lib::LinkedList::iterator node = list.begin();node;--node)
    * {
    *     TAnyType& data = *node;
    *                   :
    *                   :
    * }
    * \endcode
    */
    iterator Begin();
    iterator Begin() const;
    /**
    * \brief 後ろからindex番目のデータを取得します。
    * \param index 後ろからのインデックス
    * \return \ref lib::Iterator<T>
    * \startcode
    * lib::LinkedList<TAnyType> list;
    *         :
    *         :
    *         :
    * for (lib::LinkedList::iterator node = list.GetBottomFrom();node;--node)
    * {
    *     TAnyType& data = *node;
    *                   :
    *                   :
    * }
    * \endcode
    */
    T& GetBottomFrom(S32 index=0);
    /**
    * \brief GetTopFromのindex=0時と同じ
    * \return Node
    *
    * \startcode
    * lib::LinkedList<TAnyType> list;
    *         :
    *         :
    *         :
    * for (lib::LinkedList::iterator node = list.end();list.begin()!=node;--node)
    * {
    *     TAnyType& data = *node;
    *                   :
    *                   :
    * }
    * \endcode
    */
    Iterator<T> End();
    Iterator<T> End() const;
    /**
    * \brief 前からindex番目のデータを削除します。
    *
    * \param index インデックス
    * \param out 削除されるデータ、ゼロの場合は無視されます。
    * @return 残りのデータの数(-1は空っぽ)
    */
    S32 RemoveTopFrom(S32 index, T* out=0);
    /**
    * \brief 前からindex番目のデータを削除します。
    *
    * \param index インデックス
    * \param out 削除されるデータ、ゼロの場合は無視されます。
    * @return 残りのデータの数 (-1は空っぽ)
    */
    S32 RemoveBottomFrom(S32 index, T* out=0);
    /**
    * \brief 指定したノードを削除します。
    * \param t ノード
    * @return 残りのデータの数 (-1は空っぽ)
    */
    S32 Remove(Iterator<T>& t);
    /** 
    * \brief 全データを削除します。
    */
    void Clear();
    /**
    * \brief ソート
    * \param ソート関数
    *
    * \startcode
    * S32 AnySortFunction(const AnyType val1, const AnyType val2);
    *
    * list.Sort(&AnySortFunction);
    * \endcode
    */
    void Sort(S32 (*sort_func)(T,T));
    /**
    * \brief コピーします。
    */
    LinkedList<T>& operator=(const LinkedList<T>& src);

protected:
    U32 count_;
    iterator* top_;
    iterator* bottom_;
    struct page {
        struct page* next_page;
        U32 element_count;
        iterator nodes[1];
    } *page_top_;
    struct {
        Bool is_allocated;
        U32 allocation_size;
        S32  max_element_count;
        S32  count_per_page;
        void (*deallocator)(T);
    } memory_;
    
    /* 最後を示すイテレータ*/
    iterator node_end_;
    
    pthread_mutex_t mutex_;
    Bool use_mutex_;

    struct page* create_page(U32 element_count);
    iterator* get_brank_node();
    U32 remove_node(Iterator<T>* node);
    void init_node();
    Iterator<T>* get_node_top_from(S32 index);
    const Iterator<T>* get_node_top_from(S32 index) const;
    Iterator<T>* get_node_bottom_from(S32 index);
};
/**
* \brief 固定バッファーを利用したLinkedList ちょっとしたアルゴリズム的にリストが必要な場合使います。
*
* \startcode
* LinkedListB<int, MAX_ANY_COUNT> list(false);
*
* for (int i=0; i<MAX_ANY_COUNT; ++i)
  {
            :
            :
      list.push_back(any_value);
  }
* \endcode
*/
template <typename T, size_t SIZE> 
class LinkedListB : public LinkedList<T>
{
public:
    /**
    * \brief コンストラクター
    * \param buffer 内部で使われる固定バッファー
    * \param size   バッファーのサイズ
    * \param is_create_mutex ミューテックスOn/Off
    */
    LinkedListB(Bool use_mutex=false)
    :LinkedList<T>::LinkedList(buffer_, sizeof(buffer_), use_mutex){}
    /**
    * \brief デストラクター
    */
    virtual ~LinkedListB (){}
    LinkedListB<T, SIZE>& operator=(const LinkedListB& src);
    inline U32 GetBufferSize();
private:
    unsigned char buffer_[sizeof(Iterator<T>)*(SIZE)+sizeof(void*)+sizeof(U32)];
};

template <typename T, size_t SIZE>
LinkedListB<T, SIZE>& LinkedListB<T, SIZE>::operator=(const LinkedListB<T, SIZE>& src)
{
    LinkedList<T>::Clear();
    for (typename LinkedList<T>::iterator i = src.begin();src.end()!=i;++i)
    {
        LinkedList<T>::Add(*i);
    }
    return *this;
}
template <typename T, size_t SIZE>
inline U32 LinkedListB<T, SIZE>::GetBufferSize()
{
    return SIZE;
}

#if _LLIST_AUTO_LOCK_
#define _LLIST_MUTEX_LOCK \
if (use_mutex_!=0) pthread_mutex_lock(&mutex_)
#define _LLIST_MUTEX_UNLOCK \
if (use_mutex_!=0) pthread_mutex_unlock(&mutex_)
#else
#define _LLIST_MUTEX_LOCK 
#define _LLIST_MUTEX_UNLOCK 
#endif


template <typename T>
Bool LinkedList<T>::Init()
{
    return true;
}
template <typename T>
inline Bool LinkedList<T>::Lock() 
{
    if (use_mutex_==true)
    {
        return pthread_lock(&mutex_);
    }
    return false;
}
template <typename T>
inline void LinkedList<T>::UnLock()
{
    if (use_mutex_==true)
    {
        pthread_unlock(&mutex_);
    }
}
template <typename T>
inline Bool LinkedList<T>::Lock() const
{
    if (use_mutex_==true)
    {
        return pthread_lock(&mutex_);
    }
    return false;
}
template <typename T>
inline void LinkedList<T>::UnLock() const
{
    if (use_mutex_==true)
    {
        pthread_unlock(&mutex_);
    }
}
template <typename T>
inline U32 LinkedList<T>::Count()
{
    return count_;
}
template <typename T>
inline U32 LinkedList<T>::Count() const
{
    return count_;
}
template <typename T>
S32 LinkedList<T>::Add(T data, U32 index/*=0*/)
{
    _LLIST_MUTEX_LOCK;
    Iterator<T>* brank_node = get_brank_node();

    if (brank_node==0)
    {
        /* 何かの制限にかかっている。*/
        _LLIST_MUTEX_UNLOCK;
        return -1;
    }
    
    _LLIST_COPY_DATA(&brank_node->data, &data);
    Iterator<T>* node = get_node_bottom_from(index);
    if (node!=&node_end_)
    {
        Iterator<T>* next=node->next;

        bottom_=(node==bottom_)?brank_node:bottom_;
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
        bottom_=top_=brank_node;
    }
    
    _LLIST_MUTEX_UNLOCK;
    return ++count_;
}
template <typename T>
S32 LinkedList<T>::Insert(T data, U32 index/*=0*/)
{
    _LLIST_MUTEX_LOCK;
    Iterator<T>* brank_node = get_brank_node();
   
    if (brank_node==0)
    {
        /* 何かの制限にかかっている。*/
        _LLIST_MUTEX_UNLOCK;
        return -1;
    }

    _LLIST_COPY_DATA(&brank_node->data, &data);

    if (index>=count_)
    {
        Add(data);
        _LLIST_MUTEX_UNLOCK;
        return count_;
    }

    Iterator<T>* node = get_node_top_from(index);
    if (node!=0)
    {
        Iterator<T>* prev=node->prev;

        top_=(node==top_)?brank_node:top_;
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
        bottom_=top_=brank_node;
    }
    
    _LLIST_MUTEX_UNLOCK;
    return ++count_;
}
template <typename T>
T& LinkedList<T>::operator[](S32 index)
{
    return GetTopFrom(index);
}
template <typename T>
T& LinkedList<T>::operator[](S32 index) const
{
    return GetTopFrom(index);
}
template <typename T>
T& LinkedList<T>::GetTopFrom(S32 index/*=0*/)
{
    return get_node_top_from(index)->data;
}
template <typename T>
T& LinkedList<T>::GetTopFrom(S32 index/*=0*/) const
{
    return (T&)get_node_top_from(index)->data;
}
template <typename T>
typename lib::Iterator<T>* LinkedList<T>::get_node_top_from(S32 index)
{
    _LLIST_MUTEX_LOCK;
    S32 i;
    Iterator<T>* node = top_;
    for (i=0; i<index&&node!=0; ++i) node=node->next;
    _LLIST_MUTEX_UNLOCK;
    return (i==index)?node:&node_end_;
}
template <typename T>
const typename lib::Iterator<T>* LinkedList<T>::get_node_top_from(S32 index) const
{
    _LLIST_MUTEX_LOCK;
    S32 i;
    Iterator<T>* node = top_;
    for (i=0; i<index&&node!=0; ++i) node=node->next;
    _LLIST_MUTEX_UNLOCK;
    return (i==index)?node:&node_end_;
}
template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::Begin()
{
    return *get_node_top_from(0);
}
template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::Begin() const
{
    return *get_node_top_from(0);
}
template <typename T>
T& LinkedList<T>::GetBottomFrom(S32 index/*=0*/)
{
    return get_node_bottom_from(index)->data;
}
template <typename T>
typename lib::Iterator<T>* LinkedList<T>::get_node_bottom_from(S32 index/*=0*/)
{
    if (bottom_->prev==NULL)
    {
        return &node_end_;
    }
    _LLIST_MUTEX_LOCK;
    S32 i;
    Iterator<T>* node = bottom_->prev;
    for (i=0; i<index&&node!=0; ++i) node=node->prev;
    _LLIST_MUTEX_UNLOCK;
    return (i==index)?node:&node_end_;
}
template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::End()
{
    return node_end_;
}
template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::End() const
{
    return node_end_;
}
template <typename T>
S32 LinkedList<T>::RemoveTopFrom(S32 index, T* out/*=0*/)
{
    S32 ret=-1;
    if (count_==0)
    {
       return ret;
    }
    Iterator<T>* node = get_node_top_from(index);
    _LLIST_COPY_DATA(out, &node->data);
    ret = (S32)remove_node(node);
    return ret;
}
template <typename T>
S32 LinkedList<T>::RemoveBottomFrom(S32 index, T* out/*=0*/)
{
    S32 ret=-1;
    if (count_==0)
    {
       return ret;
    }
    Iterator<T>* node = get_node_bottom_from(index);
    _LLIST_COPY_DATA(out, &node->data);
    ret = (S32)remove_node(node);
    return ret;
}
template <typename T>
S32 LinkedList<T>::Remove(Iterator<T>& node)
{
    return remove_node(&node);
}
template <typename T>
void LinkedList<T>::Clear()
{
    while(RemoveTopFrom(0)>=0);
}
template <typename T>
Iterator<T>* LinkedList<T>::get_brank_node()
{
    struct page* last_page=0;
    U32 page_count=0;
    U32 element_count=0;

    for (struct page* curr = page_top_; curr; curr=curr->next_page, ++page_count)
    {
        const int count = curr->element_count;
        last_page=curr;
        element_count+=count;
        for (int i=0;i<count;++i)
        {
            if ((curr->nodes[i].next==0)&&(curr->nodes[i].prev==0))
            {
                if ((curr->nodes+i)==top_||(curr->nodes+i)==bottom_)
                {
                    /* この場合はノードが一個のみ残っている場合*/
                    continue;
                }
                return &curr->nodes[i];
            }
        }
    }
    if (element_count==memory_.max_element_count)
    {
        /* 上限に達した場合 */
        JOKER_WARN("capacity is over!! %d\n", element_count);
        JOKER_ASSERT(0);
        return 0;
    }

    /* 以下のケースは全て割り当てるベキ。*/
    const S32 remain_element_count = (memory_.max_element_count-element_count);
    element_count = (remain_element_count>memory_.count_per_page)?memory_.count_per_page:remain_element_count;
    element_count=(memory_.max_element_count==-1)?memory_.count_per_page:remain_element_count;

    struct page* new_page = create_page(element_count);
    if (last_page!=0)
    {
        last_page->next_page=new_page;
    }
    else
    {
        page_top_ = new_page;
    }

    return new_page->nodes;
}
template <typename T>
U32 LinkedList<T>::remove_node(Iterator<T>* node)
{
    if ((void*)node==(void*)&node_end_)
    {
        return 0;
    }
    
    Iterator<T>* prev=node->prev;
    Iterator<T>* next=node->next;
    node=NULL;
    if (prev!=0)
    {
        node=prev->next;
        prev->next=next;
    }
    if (next!=0)
    {
        node=next->prev;
        next->prev=prev;
    }
    if (node!=NULL)
    {
        node->next=NULL;
        node->prev=NULL;
    }
    node=(node==NULL)?top_:node;

    top_ = (node==top_)?next:top_;
    bottom_ = (node==bottom_)?prev:bottom_;

    return --count_;
}
template <typename T>
struct LinkedList<T>::page* LinkedList<T>::create_page(U32 element_count)
{
    U32 page_size = ((element_count-1)*sizeof(Iterator<T>)+sizeof(struct page));

    struct page* new_page;
    new_page  = (struct page*)_LLIST_MALLOC(page_size);
    memory_.is_allocated=true;
    memset(new_page, 0, sizeof(*new_page));
    new_page->element_count=element_count;

    return new_page;
}
template <typename T>
void LinkedList<T>::Sort(S32 (*sort_func)(T, T))
{
    for (iterator it=Begin();End()!=it;++it)
    {
        Bool is_changed=false;
        for (iterator it2=it; End()!=it2; ++it2)
        {
            if (it2.next==NULL)
            {
                break;
            }
            if (sort_func(*it2, it2.next->data)>0)
            {
                Iterator<T>* next=it2.next;
                Iterator<T>* prev=it2.prev;
                it2.next=it2.next->next;
                it2.prev=it2.next->prev;
                it2.next->next=next;
                it2.next->prev=prev;
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
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& src)
{
    Clear();
    for (iterator it = src.begin();src.end()!=it;++it)
    {
        Add(*it);
    }
    return *this;
}


template <typename T>
void LinkedList<T>::init_node()
{
    node_end_.prev = NULL;
    node_end_.next=NULL;
    
    top_=&node_end_;
    bottom_=&node_end_;
}

} /* namespace lib */
#endif
