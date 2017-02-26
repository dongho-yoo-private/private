/**
* Copyright (c) 2016 Applibot Inc.
* authorized by dongho.yoo.
*/
#ifndef _LIB_QUEUE_H_
#define _LIB_QUEUE_H_

#include "lib_linked_list.h"

namespace lib {
/**
* \param キューのテンプレートクラス
*/
template <class T> 
class Queue : private LinkedList<T>
{
public:
    /**
    * \brief コンストラクター
    * \param max_element_size 最大のサイズ-1の場合は無限
    * \param count_per_page 一ページに入るエレメントの数。\n
    * LinkedListはページ単位でメモリを割り当てます。-1はデフォルト値 \ref kLListInitialElementCount
    * \param is_create_mutex  ミューテックスOn/OFF
    * \param deallocator メモリから削除時のコールバック
    */
    Queue(U32 max_element_count=-1, U32 count_per_page=-1, 
                        Bool is_create_mutex=false, void (*deallocator)(T)=0)
    :LinkedList<T>::LinkedList(max_element_count, count_per_page, is_create_mutex, deallocator)
    {
    }
    /**
    * \brief コンストラクター
    * \param buffer 内部で使われる固定バッファー
    * \param size   バッファーのサイズ
    * \param is_create_mutex ミューテックスOn/Off
    */
    Queue(void* buffer, size_t buffer_size, Bool is_create_mutex=false)
    :LinkedList<T>::LinkedList(buffer, buffer_size, is_create_mutex)
    {
    }
    /**
    * \brief デストラクター
    */
    virtual ~Queue()
    {
    }
    /**
    * \brief キューに入れます。
    *
    * \param data データ
    * \return -1 はエラー overflow
    */
    inline S32 EnQueue(const T& data)
    {
        return LinkedList<T>::Insert(data);
    }
    /**
    * \brief キューから値を取得し、値を取り出します。
    *
    * \param out データ
    * \return -1 はempty.
    */
    inline S32 DeQueue(T& out)
    {
        return LinkedList<T>::RemoveBottomFrom(0, &out);
    }
    inline U32 Count()
    {
        return LinkedList<T>::count_;
    }
};
/**
* \brief バッファーを利用したqueueちょっとしたアルゴリズム的にリストが必要な場合使います。
*
* \startcode
* QueueB<int, MAX_ANY_COUNT> queue(false);
*
* for (int i=0; i<MAX_ANY_COUNT; ++i)
  {
            :
            :
      queue.Enqueue(any_value);
  }

  while(queue.Dequeue(any_valu)>=0)
  {
            :
            :
    AnyFunction(any_value);
            :
            :
  }
* \endcode
*/
template <typename T, size_t SIZE> 
class QueueB: public Queue<T>
{
public:
    /**
    * \brief コンストラクター
    * \param buffer 内部で使われる固定バッファー
    * \param size   バッファーのサイズ
    * \param is_create_mutex ミューテックスOn/Off
    */
    QueueBuffer(Bool is_use_mutex=false)
    :Queue<T>::Queue(buffer_, sizeof(buffer_), is_use_mutex){}
    virtual ~QueueBuffer() {}
private:
    char buffer_[sizeof(Iterator<T>)*(SIZE)+sizeof(void*)+sizeof(U32)];
};

}
 
#endif

