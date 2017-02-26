/**
 * \file mem_small_arena.h
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief 小さいサイズのアロケータ用アリーナ定義部
 */


#ifndef MEM_SMALL_ARENA_H_
#define MEM_SMALL_ARENA_H_


#include <core/memory_allocation/guarded_alloc_types.h>
#include <core/memory_allocation/intern/mem_i_arena.h>
#include <core/types.h>


/* arena api function definition */
/**
 * \~english
 * \~japanese
 *  \brief 小さいメモリアロケータ用のアリーナを作成します
 *  \param[in] name アリーナ名
 *  \return 作成された \ref MEM_TArena
 *  \retval 0  作成に失敗したときに返ります
 */
extern MEM_TArena *MEM_CreateSmallArena(const Char *name);


#endif  /* MEM_SMALL_ARENA_H_ */


