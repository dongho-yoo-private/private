/**
 * \file lib_stack.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief スタック
 */


#ifndef LIB_STACK_H_
#define LIB_STACK_H_


#include <core/types.h>
#include <core/debug/debug_assert.h>
#include <core/memory_allocation/guarded_alloc.h>


ADK_BEGIN_NAMESPACE


/* class definition */
/**
 * \~english
 * \~japanese
 *   \brief スタッククラス
 */
class LIB_Stack {
 public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   *   \param[in] capacity スタックのデフォルトキャパシティ
   */
  LIB_Stack( U32 capacity=1024);
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  virtual ~LIB_Stack();

  /* operatoin management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief メモリの解放を行いスタックの使用を終了する
   */
  void Release();
  /**
   * \~english
   * \~japanese
   *   \brief クリア
   */
  void Clear();
  /**
   * \~english
   * \~japanese
   *   \brief プッシュ
   *   \param[in] count プッシュ数
   */
  template<typename T>
  T* Push( U32 count=1);
  /**
   * \~english
   * \~japanese
   *   \brief ポップ
   *   \param[in] count ポップ数
   */
  template<typename T>
  T* Pop( U32 count);
  /**
   * \~english
   * \~japanese
   *   \brief トップ
   */
  template<typename T>
  T* Top();
  /**
   * \~english
   * \~japanese
   *   \brief トップ
   */
  template<typename T>
  T* Bottom();
  /**
   * \~english
   * \~japanese
   *   \brief サイズを返します
   */
  ADK_INLINE size_t GetSize() const;
  /**
   * \~english
   * \~japanese
   *   \brief サイズを返します
   */
  ADK_INLINE size_t GetCapacity() const;

 private:
  /* vars */
  struct {
    size_t  capacity;
    Char  *cur;
    Char  *top;
    Char  *end;
  }stack_;
};
template<typename T>
T* LIB_Stack::Push( U32 count) {
  if (stack_.top + sizeof(T) * count >= stack_.end) {
    size_t new_capacity = stack_.capacity * 2;
    size_t size = GetSize();
    size_t new_size = GetSize() + sizeof(T) * count;
    if (new_capacity < new_size) {
      new_capacity = new_size;
    }
    stack_.cur = (char*)ADK_Realloc( stack_.cur, new_capacity, "Stack");
    stack_.capacity = new_capacity;
    stack_.top = stack_.cur + size;
    stack_.end = stack_.cur + stack_.capacity;
  }
  T* ret = (T*)stack_.top;
  stack_.top += sizeof(T) * count;
  return ret;
}
template<typename T>
T* LIB_Stack::Pop( U32 count) {
  ADK_ASSERT(GetSize() >= count * sizeof(T));
  stack_.top -= count * sizeof(T);
  return (T*)stack_.top;
}
template<typename T>
T* LIB_Stack::Top() {
  ADK_ASSERT(GetSize() >= sizeof(T));
  return (T*)(stack_.top - sizeof(T));
}
template<typename T>
T* LIB_Stack::Bottom() {
  return (T*)stack_.cur;
}
ADK_INLINE size_t LIB_Stack::GetSize() const {
  return stack_.top-stack_.cur;
}
ADK_INLINE size_t LIB_Stack::GetCapacity() const {
  return stack_.capacity;
}


ADK_END_NAMESPACE


#endif  /* LIB_STACK_H_ */


