/**
 * \file lib_i_object.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief オブジェクトインターフェース
 */


#ifndef LIB_I_OBJECT_H_
#define LIB_I_OBJECT_H_


#include <core/types.h>
#include <core/memory_allocation/guarded_alloc.h>


ADK_BEGIN_NAMESPACE


/* class definition */
/**
 * \~english
 * \~japanese
 *   \brief オブジェクト
 */
class IObject {
 public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   */
  IObject() : next_(0), prev_(0){}
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  virtual ~IObject(){}

  /* list management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief 次のオブジェクトへのアドレスを設定します。
   *   \param next [in] オブジェクトのアドレス
   */
  ADK_INLINE void SetNext( IObject *next);
  /**
   * \~english
   * \~japanese
   *   \brief 次のオブジェクトへのアドレスを返します。
   *   \return オブジェクトのアドレス
   */
  ADK_INLINE IObject *GetNext() const;
  /**
   * \~english
   * \~japanese
   *   \brief 前のオブジェクトへのアドレスを設定します。
   *   \param prev [in] オブジェクトのアドレス
   */
  ADK_INLINE void SetPrev( IObject *prev);
  /**
   * \~english
   * \~japanese
   *   \brief 前のオブジェクトへのアドレスを返します。
   *   \return オブジェクトのアドレス
   */
  ADK_INLINE IObject *GetPrev() const;

private:
  /* vars */
  /**
   * \~english
   * \~japanese
   *   \brief 次のオブジェクトへのポインタ
   */
  IObject  *next_;
  /**
   * \~english
   * \~japanese
   *   \brief 前のオブジェクトへのポインタ
   */
  IObject  *prev_;

  /**
   * \~english
   * \~japanese
   *   \brief クラス内 new/delete オペレータ定義
   */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("IObject");
};


/* class inline method implementation */
ADK_INLINE void IObject::SetNext( IObject *next) {
  next_ = next;
}
ADK_INLINE IObject *IObject::GetNext() const {
  return next_;
}
ADK_INLINE void IObject::SetPrev( IObject *prev) {
  prev_ = prev;
}
ADK_INLINE IObject *IObject::GetPrev() const {
  return prev_;
}


ADK_END_NAMESPACE


#endif  /* LIB_I_OBJECT_H_ */


