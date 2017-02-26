/**
* Copyright (c) 2015 Igotomo.
* authorized by dongho.yoo.
*/
#ifndef _ITX_STACK_H_
#define _ITX_STACK_H_
#include "itx_linked_list.h"

template <class T> 
class Stack : private LinkedList<T>
{
public:
    /**
    * \brief コンストラクター
    * \param max_element_size 最大のサイズ-1の場合は無限
    * \param count_per_page 一ページに入るエレメントの数。\n
    * LinkedListはページ単位でメモリを割り当てます。-1はデフォルト値 \ref ITX_LLIST_INITIAL_ELEMENT_COUNT 
    * \param is_create_mutex  ミューテックスOn/OFF
    * \param deallocator メモリから削除時のコールバック
    */
    Stack(U32 max_element_count=-1, U32 count_per_page=-1, 
                        bool_t is_create_mutex=false, void (*deallocator)(T)=0)
    : LinkedList<T>::LinkedList(max_element_count, count_per_page, is_create_mutex, deallocator)
    {
    }
    /**
    * \brief コンストラクター
    * \param buffer 内部で使われる固定バッファー
    * \param size   バッファーのサイズ
    * \param is_create_mutex ミューテックスOn/Off
    */
    Stack(void* buffer, U32 buffer_size, bool_t is_create_mutex=false)
    : LinkedList<T>::LinkedList(buffer, buffer_size, is_create_mutex)
    {
    }
    /**
    * \brief デストラクター
    */
    virtual ~Stack()
    {
        // TODO.
    }
    /**
    * \brief プッシュします。
    *
    * \param data データ
    * \return -1 はエラー overflow
    */
    inline S32 Push(const T& data)
    {
        return LinkedList<T>::Add(data);
    }
    /**
    * \brief ポップします。
    *
    * \param out データ
    * \return -1 はempty.
    */
    inline S32 Pop(T& out)
    {
        return LinkedList<T>::RemoveBottomFrom(0, &out);
    }
    /**
    * \brief 数を返します。
    */
    inline S32 Count()
    {
        return LinkedList<T>::Count();
    }
};
/**
* \brief バッファーを利用したstackちょっとしたアルゴリズム的にリストが必要な場合使います。
*
* \startcode
* StackBuffer<int, MAX_ANY_COUNT> stack(false);
*
* for (int i=0; i<MAX_ANY_COUNT; ++i)
  {
            :
            :
      stack.Push(any_value);
  }

  while(stack.Pop(any_valu)>=0)
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
class StackB: public Stack<T>
{
public:
    /**
    * \brief コンストラクター
    * \param buffer 内部で使われる固定バッファー
    * \param size   バッファーのサイズ
    * \param is_create_mutex ミューテックスOn/Off
    */
    StackBuffer(bool_t is_use_mutex=false)
    :Stack<T>::Stack(m_buffer, sizeof(m_buffer), is_use_mutex){}
private:
    char m_buffer[sizeof(ItxNode<T>)*(SIZE)+sizeof(void*)+sizeof(U32)];
};



#endif
