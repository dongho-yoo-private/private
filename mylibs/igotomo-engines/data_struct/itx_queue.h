/**
* Copyright (c) 2015 Igotomo.
* authorized by dongho.yoo.
*/
#ifndef _ITX_QUEUE_H_
#define _ITX_QUEUE_H_

#include "itx_linked_list.h"

/**
* @param キューのテンプレートクラス
*/
template <class T> 
class ItxQueue : private ItxLinkedList<T>
{
public:
    /**
    * @brief コンストラクター
    * @param max_element_size 最大のサイズ-1の場合は無限
    * @param count_per_page 一ページに入るエレメントの数。\n
    * ItxLinkedListはページ単位でメモリを割り当てます。-1はデフォルト値 \ref ITX_LLIST_INITIAL_ELEMENT_COUNT 
    * @param is_create_mutex  ミューテックスOn/OFF
    * @param deallocator メモリから削除時のコールバック
    */
    ItxQueue(ubit32_t max_element_count=-1, ubit32_t count_per_page=-1, 
                        bool_t is_create_mutex=false, void (*deallocator)(T)=0)
    :ItxLinkedList<T>::ItxLinkedList(max_element_count, count_per_page, is_create_mutex, deallocator)
    {
    }
    /**
    * @brief コンストラクター
    * @param buffer 内部で使われる固定バッファー
    * @param size   バッファーのサイズ
    * @param is_create_mutex ミューテックスOn/Off
    */
    ItxQueue(void* buffer, size_t buffer_size, bool_t is_create_mutex=false)
    :ItxLinkedList<T>::ItxLinkedList(buffer, buffer_size, is_create_mutex)
    {
    }
    /**
    * @brief デストラクター
    */
    virtual ~ItxQueue()
    {
    }
    /**
    * @brief キューに入れます。
    *
    * @param data データ
    * @return -1 はエラー overflow
    */
    inline bit32_t EnQueue(const T& data)
    {
        return ItxLinkedList<T>::Insert(data);
    }
    /**
    * @brief キューから値を取得し、値を取り出します。
    *
    * @param out データ
    * @return -1 はempty.
    */
    inline bit32_t DeQueue(T& out)
    {
        return ItxLinkedList<T>::RemoveBottomFrom(0, &out);
    }
    inline ubit32_t Count()
    {
        return ItxLinkedList<T>::m_count;
    }
};
/**
* @brief バッファーを利用したqueueちょっとしたアルゴリズム的にリストが必要な場合使います。
*
* @startcode
* ItxQueueBuffer<int, MAX_ANY_COUNT> queue(false);
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
* @endcode
*/
template <typename T, size_t SIZE> 
class ItxQueueBuffer: public ItxQueue<T>
{
public:
    /**
    * @brief コンストラクター
    * @param buffer 内部で使われる固定バッファー
    * @param size   バッファーのサイズ
    * @param is_create_mutex ミューテックスOn/Off
    */
    ItxQueueBuffer(bool_t is_use_mutex=false)
    :ItxQueue<T>::ItxQueue(m_buffer, sizeof(m_buffer), is_use_mutex){}
    virtual ~ItxQueueBuffer() {}
private:
    char m_buffer[sizeof(ItxNode<T>)*(SIZE)+sizeof(void*)+sizeof(ubit32_t)];
};


 
#endif
