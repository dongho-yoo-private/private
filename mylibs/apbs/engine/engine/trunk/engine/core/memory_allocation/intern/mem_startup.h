/**
 * \file mem_startup.h
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief ガードアロケータの起動情報に関する定義部
 */


#ifndef MEM_STARTUP_H_
#define MEM_STARTUP_H_


#include <core/types.h>


#ifdef __cplusplus
extern "C" {
#endif


/* arena info */
/**
 * \~english
 * \~japanese
 *  \brief メモりアリーナ名の文字列長
 */
#define MEM_STARTUP_ARENA_NAME_LEN  (8)
/**
 * \~english
 * \~japanese
 *  \brief メモリアリーナ情報
 */
typedef struct {
  size_t min,
         max;
  size_t size;
  Char name[MEM_STARTUP_ARENA_NAME_LEN];
}MEM_TStartupArena;


/* extern function definition */
/**
 * \~english
 * \~japanese
 *  \brief ガードアロケータの起動情報を初期化する
 *  \return 初期化の成功判定。以下の値が返ります
 *  \retval true  初期化に成功したときに返ります
 *  \retval false  初期化に失敗したときに返ります
 */
extern Bool MEM_InitStartup();
/**
 * \~english
 * \~japanese
 *  \brief 起動情報の終了を行う
 *  \return 終了の成功判定。以下の値が返ります
 *  \retval true  終了に成功したときに返ります
 *  \retval false  終了に失敗したときに返ります
 */
extern Bool MEM_TermStartup();

/**
 * \~english
 * \~japanese
 *  \brief ガードアロケータの有無フラグを返します
 *  \return 有無フラグ
 */
extern Bool MEM_IsUse();
/**
 * \~english
 * \~japanese
 *  \brief メモリアリーナ数を返します
 *  \return メモリアリーナ数
 */
extern S32 MEM_StartupGetArenaCount();
/**
 * \~english
 * \~japanese
 *  \brief メモリアリーナの情報を取得する
 *  \param[out] arena アリーナ情報格納先
 *  \param[in] index 取得したいアリーナ位置( 0 <= index < \ref MEM_StartupGetArenaCount() )
 *  \return 取得の成功判定。以下の値が返ります
 *  \retval true  取得に成功したときに返ります
 *  \retval false  取得に失敗したときに返ります
 */
extern Bool MEM_StartupGetArena(MEM_TStartupArena *arena,S32 index);


#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif  /* MEM_STARTUP_H_ */


