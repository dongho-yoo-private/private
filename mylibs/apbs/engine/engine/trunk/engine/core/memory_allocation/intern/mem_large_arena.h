/**
 * \file mem_large_arena.h
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief 大きいサイズのアロケータ用アリーナ定義部
 */


#ifndef MEM_LARGE_ARENA_H_
#define MEM_LARGE_ARENA_H_


#include <core/memory_allocation/guarded_alloc_types.h>
#include <core/memory_allocation/intern/mem_i_arena.h>
#include <core/types.h>


/* arena api function definition */
/**
 * \~english
 * \~japanese
 *  \brief 大きいメモリアロケータ用のアリーナを作成します
 *  \param[in] name アリーナ名
 *  \return 作成された \ref MEM_TArena
 *  \retval 0  作成に失敗したときに返ります
 */
extern MEM_TArena *MEM_CreateLargeArena(const Char *name);


#endif  /* MEM_LARGE_ARENA_H_ */


