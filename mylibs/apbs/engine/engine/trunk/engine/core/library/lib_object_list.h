/**
 * \file lib_object_list.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief オブジェクト
 */


#ifndef LIB_OBJECT_LIST_H_
#define LIB_OBJECT_LIST_H_


#include <core/types.h>
#include <core/library/lib_i_object.h>


ADK_BEGIN_NAMESPACE


/* struct */
/**
 * \~english
 * \~japanese
 *   \brief オブジェクトリストのベース
 */
typedef struct TListBase {
  void  *first,
        *last;
}TListBase;


/* extern function definition */
/**
 * \~english
 * \~japanese
 *   \brief リストの先頭に追加します。
 *   \return 追加の成功判定。以下の値が返ります。
 *   \retval true  追加に成功したときに返ります。
 *   \retval false  追加に失敗したときに返ります。
 */
Bool ObjectListAddHead( TListBase *lb, IObject *obj);
/**
 * \~english
 * \~japanese
 *   \brief リストの最後尾に追加します。
 *   \return 追加の成功判定。以下の値が返ります。
 *   \retval true  追加に成功したときに返ります。
 *   \retval false  追加に失敗したときに返ります。
 */
Bool ObjectListAddTail( TListBase *lb, IObject *obj);
/**
 * \~english
 * \~japanese
 *   \brief リストからリンクを切り離します。
 *   \return 切り離しの成功判定。以下の値が返ります。
 *   \retval true  切り離しに成功したときに返ります。
 *   \retval false  切り離しに失敗したときに返ります。
 */
Bool ObjectListRemLink( TListBase *lb, IObject *obj);
/**
 * \~english
 * \~japanese
 *   \brief リストのリンク数を返します。
 *   \param lb [in] リスト
 *   \return リンク数
 */
S32 ObjectListCount( TListBase *lb);


ADK_END_NAMESPACE


#endif  /* LIB_OBJECT_LIST_H_ */


