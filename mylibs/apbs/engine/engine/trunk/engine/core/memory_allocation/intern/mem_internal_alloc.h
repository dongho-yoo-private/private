/**
 * \file mem_internal_alloc.h
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief 内部アロケータ
 */


#ifndef MEM_INTERNAL_ALLOC_H_
#define MEM_INTERNAL_ALLOC_H_


#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


/* internal extern function definition */
/**
 * \~english
 * \~japanese
 *  \brief 大きさ len のメモリブロックを割り当て、それのアドレスを返します。
 *  \param[in] len 割り当てるメモリブロックの大きさ
 *  \return 割り当てたアドレス。
 */
extern void *MEM_InternalMalloc(size_t len);
/**
 * \~english
 * \~japanese
 *  \brief len バイト要素の n 個からなる配列をメモリブロックに割り当て、
 *          それのアドレスを返します
 *  \param[in] nmemb 要素数
 *  \param[in] len 割り当てるメモリブロックの大きさ
 *  \return 割り当てたアドレス。
 */
extern void *MEM_InternalCalloc(size_t nmemb,size_t len);
/**
 * \~english
 * \~japanese
 *  \brief ptr が示すメモリブロックのサイズを len バイトに変更して、
 *          それのアドレスを返します
 *  \param[in] ptr 元のメモリブロックアドレス
 *  \param[in] len 割り当てるメモリブロックの大きさ
 *  \return 割り当てたアドレス。
 */
extern void *MEM_InternalRealloc(void*ptr,size_t len);
/**
 * \~english
 * \~japanese
 *  \brief アライメントを指定して大きさ len のメモリブロックを割り当て、
 *         それのアドレスを返します。
 *  \param[in] alignment 割り当てるアライメント
 *  \param[in] len 割り当てるメモリブロックの大きさ
 *  \return 割り当てたアドレス。
 */
extern void *MEM_InternalMemalign(size_t alignment,size_t len);
/**
 * \~english
 * \~japanese
 *  \brief 割り当てられたメモリブロックを解放します。
 *  \param[in] addr 割り当てられたアドレス。
 */
extern void MEM_InternalFree(void *addr);


#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif  /* _MEM_INTERNAL_ALLOC_H_ */


