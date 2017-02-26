/**
* Copyright (c) 2016 Applibot Inc.
* authorized by dongho.yoo.
*/
#ifndef _LIB_STACK_H_
#define _LIB_STACK_H_
#include "lib_linked_list.h"

namespace lib {
/**
* \brief スタック
*/
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
    Stack(ubit32_t max_element_count=-1, ubit32_t count_per_page=-1, 
                        Bool is_create_mutex=false, void (*deallocator)(T)=0)
    : LinkedList<T>::LinkedList(max_element_count, count_per_page, is_create_mutex, deallocator)
    {
    }
    /**
    * \brief コンストラクター
    * \param buffer 内部で使われる固定バッファー
    * \param size   バッファーのサイズ
    * \param is_create_mutex ミューテックスOn/Off
    */
    Stack(void* buffer, ubit32_t buffer_size, Bool is_create_mutex=false)
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
    inline bit32_t Push(const T& data)
    {
        return LinkedList<T>::Add(data);
    }
    /**
    * \brief ポップします。
    *
    * \param out データ
    * \return -1 はempty.
    */
    inline bit32_t Pop(T& out)
    {
        return LinkedList<T>::RemoveBottomFrom(0, &out);
    }
    /**
    * \brief 数を返します。
    */
    inline bit32_t Count()
    {
        return LinkedList<T>::Count();
    }
};
/**
* \brief バッファーを利用したstackちょっとしたアルゴリズム的にリストが必要な場合使います。
*
* \startcode
* StackB<int, MAX_ANY_COUNT> stack(false);
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
    StackBuffer(Bool is_use_mutex=false)
    :Stack<T>::Stack(buffer_, sizeof(buffer_), is_use_mutex){}
private:
    char buffer_[sizeof(Iterator<T>)*(SIZE)+sizeof(void*)+sizeof(ubit32_t)];
};

} /* namespace lib */

#endif

