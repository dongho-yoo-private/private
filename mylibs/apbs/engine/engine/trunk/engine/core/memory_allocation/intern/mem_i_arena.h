/**
 * \file mem_i_arena.h
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief メモリアリーナインターフェース定義部
 */


#ifndef MEM_I_ARENA_H_
#define MEM_I_ARENA_H_


#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>


/* arena types */
/**
 * \~english
 * \~japanese
 *  \brief メモリアリーナ
 */
typedef struct MEM_TArena MEM_TArena;


/* arena api function definition */
/**
 * \~english
 * \~japanese
 *  \brief メモリアリーナを作成する
 *  \param[in] name アリーナ名
 *  \return 作成された \ref MEM_TArena
 *  \retval 0 作成に失敗したときに返ります
 */
extern MEM_TArena *MEM_CreateArena(const Char *name);
/**
 * \~english
 * \~japanese
 *  \brief メモリアリーナを削除する
 *  \param[in] arena メモリアリーナ
 *  \return 削除の成功判定。以下の値が返ります
 *  \retval true 削除に成功したときに返ります
 *  \retval false 削除に失敗したときに返ります
 */
extern Bool MEM_DisposeArena(MEM_TArena *arena);
/**
 * \~english
 * \~japanese
 *  \brief メモリの詳細コールバックを設定する
 */
extern void MEM_SetFuncs(
    MEM_TArena *arena,
    size_t (*_get_buffer_size)(size_t,size_t),
    Bool (*_init)(MEM_TArena*,void*,size_t,size_t),
    Bool (*_term)(MEM_TArena*),

    void* (*_malloc)(MEM_TArena*,size_t,size_t,const char*,ADK_TAllocMethod),
    void* (*_calloc)(MEM_TArena*,size_t,size_t,const char*,ADK_TAllocMethod),
    void (*_free)(MEM_TArena*,void*),

    size_t (*_get_addr_size)(MEM_TArena*,void*),
    size_t (*_get_arena_size)(MEM_TArena*),
    size_t (*_get_header_size)(MEM_TArena*),
    size_t (*_get_tot_size)(MEM_TArena*),
    size_t (*_get_allocation_count)(MEM_TArena*),
    size_t (*_get_tot_blocksize)(MEM_TArena*,const char*),
    size_t (*_check_address)(MEM_TArena*,const void*),

    Bool (*_print_info)(MEM_TArena*),
    Bool (*_now_print_info)(MEM_TArena*),
    Bool (*_output_memory_list_for_txt)(MEM_TArena*,const Char*,const Char*,Bool),

    void *userdata);
/**
 * \~english
 * \~japanese
 *  \brief ユーザーデータを返します
 *  \param[in] arena メモりアリーナ
 */
extern void *MEM_ArenaGetUserData(MEM_TArena *arena);
/**
 * \~english
 * \~japanese
 *  \brief アリーナの名前を返します
 *  \param[in] arena メモリアリーナ
 *  \return 文字列
 */
extern const Char *MEM_ArenaGetName(MEM_TArena *arena);

/**
 * \~english
 * \~japanese
 *  \brief アリーナの作成に必要なバッファサイズを返します
 *  \param[in] arenaSize 用意したいアリーナサイズ
 *  \param[in] max アリーナ内で確保する最大数
 *  \return 付加情報を含めたサイズ
 */
extern size_t MEM_GetArenaBufferSize(MEM_TArena *arena,size_t arenaSize,size_t max);
/**
 * \~english
 * \~japanese
 *  \brief 初期化処理を行います
 *  \param[in] arena メモリアリーナ
 *  \param[in] buffer アリーナ用のバッファへのポインタ
 *  \param[in] arenaSize アリーナサイズ
 *  \param[in] bufferSize バッファサイズ
 *  \param[in] min 最小確保サイズ
 *  \param[in] max 最大確保サイズ
 *  \return 初期化の成功判定。以下の値が返ります
 *  \retval true 初期化に成功したときに返ります
 *  \retval false 初期化に失敗したときに返ります
 */
extern Bool MEM_InitArena(
    MEM_TArena *arena,
    void *buffer,
    size_t arenaSize,
    size_t bufferSize,
    size_t min,
    size_t max);
/**
 * \~english
 * \~japanese
 *  \brief 終了処理を行います
 *  \param[in] arena メモリアリーナ
 *  \return 終了の成功判定。以下の \ref adk::Result が返ります
 *  \retval true 終了に成功したときに返ります
 *  \retval false 終了に失敗したときに返ります
 */
extern Bool MEM_TermArena(MEM_TArena *arena);
/**
 * \~english
 * \~japanese
 *  \brief サイズから該当するアリーナを返します
 *  \param[in] len メモリサイズ
 *  \return \ref MEM_TArena
 */
extern MEM_TArena *MEM_GetArenaFromLen(size_t len);
/**
 * \~english
 * \~japanese
 *  \brief アドレスから該当するアリーナを返します
 *  \param[in] addr アドレス
 *  \return \ref MEM_TArena
 */
extern MEM_TArena *MEM_GetArenaFromAddr(void *addr);
/**
 * \~english
 * \~japanese
 *  \brief アリーナの最小確保サイズを返します
 *  \param[in] arena メモリアリーナ
 *  \return 確保サイズ
 */
extern size_t MEM_ArenaGetMin(MEM_TArena *arena);
/**
 * \~english
 * \~japanese
 *  \brief アリーナの最大確保サイズを返します
 *  \param[in] arena メモリアリーナ
 *  \return 確保サイズ
 */
extern size_t MEM_ArenaGetMax(MEM_TArena *arena);

/**
 * \~english
 * \~japanese
 *  \brief 大きさ size のメモリブロックを割り当て、それのアドレスを返します。
 *  \param[in] arena メモリアリーナ
 *  \param[in] size 割り当てるメモリブロックの大きさ
 *  \param[in] align 割り当てるアドレスのアライメント値
 *  \param[in] str メモリに関連付ける名前(static である必要があります。)
 *  \param[in] method メモリの確保方法
 *  \return 割り当てたアドレス。
 */
extern void *MEM_ArenaMalloc(
    MEM_TArena *arena,
    size_t size,
    size_t align,
    const char *str,
    ADK_TAllocMethod method);
/**
 * \~english
 * \~japanese
 *  \brief 大きさ size のメモリブロックを割り当て、それのアドレスを返します。
 *  \param[in] arena メモリアリーナ
 *  \param[in] size 割り当てるメモリブロックの大きさ
 *  \param[in] align 割り当てるアドレスのアライメント値
 *  \param[in] str メモリに関連付ける名前(static である必要があります。)
 *  \param[in] method メモリの確保方法
 *  \return 割り当てたアドレス。
 */
extern void *MEM_ArenaCalloc(
    MEM_TArena *arena,
    size_t size,
    size_t align,
    const char *str,
    ADK_TAllocMethod method);
/**
 * \~english
 * \~japanese
 *  \brief 割り当てられたメモリブロックを解放します。
 *  \param[in] arena メモリアリーナ
 *  \param[in] addr 割り当てられたアドレス。
 */
extern void MEM_ArenaFree(
    MEM_TArena *arena,
    void *addr);

/**
 * \~english
 * \~japanese
 *  \brief アドレスに割り当てられたサイズを返します
 *  \param[in] arena メモリアリーナ
 *  \return サイズ
 */
extern size_t MEM_ArenaGetAddrSize(
    MEM_TArena *arena,
    void *addr);
/**
 * \~english
 * \~japanese
 *  \brief アリーナサイズを返します
 *  \param[in] arena メモリアリーナ
 *  \return サイズ
 */
extern size_t MEM_ArenaGetArenaSize(MEM_TArena *arena);
/**
 * \~english
 * \~japanese
 *  \brief アリーナ管理用のヘッダ領域のサイズを返します
 *  \param[in] arena メモリアリーナ
 *  \return サイズ
 */
extern size_t MEM_ArenaGetHeaderSize(MEM_TArena *arena);
/**
 * \~english
 * \~japanese
 *  \brief アリーナが管理するバッファ全体のサイズを返します
 *  \param[in] arena メモリアリーナ
 *  \return サイズ
 */
extern size_t MEM_ArenaGetTotSize(MEM_TArena *arena);
/**
 * \~english
 * \~japanese
 *  \brief ブロックが保持するサイズを返します
 *  \param[in] blockname ブロック名
 *  \return サイズ
 */
extern size_t MEM_ArenaGetTotBlockSize(const char *blockname);
/**
 * \~english
 * \~japanese
 *  \brief アドレスがアリーナが管理する領域か返します
 *  \param[in] arena メモリアリーナ
 *  \return 管理フラグ。以下の値が返ります
 *  \retval true  アドレスがアリーナ管理内だったときに返ります
 *  \retval false  アドレスがアリーナ管理外だったときに返ります
 */
extern Bool MEM_ArenaCheckAddress(MEM_TArena *arena,const void *addr);

/* debug management functionally */
/**
 * \~english
 * \~japanese
 *  \brief アリーナ情報を出力します
 */
extern void MEM_PrintArenaInfo();
/**
 * \~english
 * \~japanese
 *  \brief アリーナ情報をTXT形式で出力します
 *  \param[in] arena メモリアリーナ
 *  \param[in] filename ファイル名
 *  \param[in] output_backtrace バックトレースを出力するかのフラグ
 */
extern void MEM_ArenaOutputMemoryListForTXT(
    const Char *filename,const Char *blockname,Bool output_backtrace);
/**
 * \~english
 * \~japanese
 *  \brief 現在のメモリの使用状況をログ出力します
 */
extern void MEM_ArenaPrintMemoryInformation();
/**
 * \~english
 * \~japanese
 *  \brief アリーナ全体で確保されているアロケーションカウントを返します
 *  \return アロケーションカウント
 */
extern size_t MEM_ArenaGetAllocationCount();


#endif  /* MEM_I_ARENA_H_ */


