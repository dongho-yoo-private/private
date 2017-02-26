/**
 * \file mem_list.h
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief アロケータ用メモリリスト
 */


#ifndef MEM_LIST_H_
#define MEM_LIST_H_


#include <core/types.h>


#ifdef __cplusplus
extern "C" {
#endif


/* list function definition */
/**
 * \~english
 * \~japanese
 *  \brief 新たな要素をリストに追加します
 *  \param[in,out] list 基盤となるリスト
 *  \param[in] element 追加要素
 */
extern void MEM_ListPush(void **list,void *element);
/**
 * \~english
 * \~japanese
 *  \brief 手前の要素を取り出して返します
 *  \param[in] list 基盤となるリスト
 *  \return 要素のアドレス。以下の値が返ります
 *  \retval 0  要素が存在しないときに返ります
 */
extern void *MEM_ListPop(void **list);
/**
 * \~english
 * \~japanese
 *  \brief リストの要素が存在するかを返します
 *  \return 存在するかの判定結果。以下の値が返ります
 *  \retval true  要素が存在するときに返ります
 *  \retval false  要素が存在しないときに返ります
 */
extern Bool MEM_IsList(void *list);


#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif  /* MEM_LIST_H_ */


