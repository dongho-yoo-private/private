/**
 * \file mem_block.h
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief メモリブロック定義部
 */


#ifndef MEM_BLOCK_H_
#define MEM_BLOCK_H_


#include <core/types.h>
#include <core/memory_allocation/guarded_alloc.h>


/* memory block function definition */
/**
 * \~english
 * \~japanese
 *  \brief メモリブロックを開始します
 *  \param[in] name ブロック名
 *  \return ブロックハンドル
 */
extern ADK_MEMBLOCK_HANDLE MEM_BlockStart(const Char *name);
/**
 * \~english
 * \~japanese
 *  \brief 開始されているメモリブロックを停止します
 *  \param[in] h ブロックハンドル
 *  \return 停止の成功判定。以下の値が返ります
 *  \retval true  停止に成功したときに返ります
 *  \retval false  停止に失敗したときに返ります
 */
extern Bool MEM_BlockStop(ADK_MEMBLOCK_HANDLE h);
/**
 * \~english
 * \~japanese
 *  \brief 現在設定されているブロック名を返します
 *  \return 名前
 */
extern const char *MEM_BlockGetNowName();
/**
 * \~english
 * \~japanese
 *  \brief アロックカウントをインクリメントする
 */
extern void MEM_BlockIncrement();
/**
 * \~english
 * \~japanese
 *  \brief ブロック情報を出力する
 */
extern void MEM_OutputBlockInfo();


#endif  /* MEM_BLOCK_H_ */


