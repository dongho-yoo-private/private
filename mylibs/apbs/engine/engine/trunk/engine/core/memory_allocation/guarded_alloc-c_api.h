/**
 * \file guarded_alloc-c_api.h
 * \ingroup adkgrp_galloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief ガードアロケータC-API定義部
 */

#ifndef GUARDED_ALLOC_C_API_H_
#define GUARDED_ALLOC_C_API_H_

#include <stddef.h>

#include <core/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* initialize & terminate */
/**
 * \~english
 * \~japanese
 *  \brief アロケータの初期化を行います
 *  \deprecated この関数はADK内部で呼び出されるもので
 *              アプリケーションから呼び出さないで下さい
 *  \return 初期化の成功判定。以下の値が返ります。
 *  \retval true 初期化に成功したときに返ります。
 *  \retval false 初期化に失敗したときに返ります。
 */
extern Bool ADK_InitAlloc();
/**
 * \~english
 * \~japanese
 *  \brief アロケータの終了を行います。
 *  \deprecated この関数はADK内部で呼び出されるもので
 *               アプリケーションから呼び出さないで下さい
 *  \return 終了の成功判定。以下の値が返ります。
 *  \retval true  終了に成功したときに返ります。
 *  \retval false  終了に失敗したときに返ります。
 */
extern Bool ADK_TermAlloc();

/* allocator management functionally */
/**
 * \~english
 * \~japanese
 *  \brief 大きさ len のメモリブロックを割り当て、それのアドレスを返します。
 *  \param[in] len 割り当てるメモリブロックの大きさ
 *  \param[in] str メモリに関連付ける名前(static である必要があります。)
 *  \return 割り当てたアドレス。
 */
extern void *ADK_Malloc(size_t len,const char *str);
/**
 * \~english
 * \~japanese
 *  \brief len バイト要素の n 個からなる配列をメモリブロックに割り当て、
 *          それのアドレスを返します
 *  \param[in] n 要素数
 *  \param[in] len 割り当てるメモリブロックの大きさ
 *  \param[in] str メモリに関連付ける名前(static である必要があります。)
 *  \return 割り当てたアドレス。
 */
extern void *ADK_Calloc(size_t n,size_t len,const char *str);
/**
 * \~english
 * \~japanese
 *  \brief ptr が示すメモリブロックのサイズを len バイトに変更して、
 *          それのアドレスを返します
 *  \param[in] ptr 元のメモリブロックアドレス
 *  \param[in] len 割り当てるメモリブロックの大きさ
 *  \param[in] str メモリに関連付ける名前(static である必要があります。)
 *  \return 割り当てたアドレス。
 */
extern void *ADK_Realloc(void *ptr,size_t len,const char *str);
/**
 * \~english
 * \~japanese
 *  \brief 割り当てられたメモリブロックを解放します。
 *  \param[in] addr 割り当てられたアドレス。
 */
extern void ADK_Free(void *addr);
/**
 * \~english
 * \~japanese
 *  \brief アドレスが保持しているサイズを返します
 *  \param[in] addr 割り当てられたアドレス。
 *  \return サイズ
 *  \retval 0  割り当てられたアドレスではないときに返ります
 */
extern size_t ADK_GetAddrSize(void *addr);
/**
 * \~english
 * \~japanese
 *  \brief ガードアロケータの使用を有効にします
 */
extern void ADK_EnableGuardAllocator();
/**
 * \~english
 * \~japanese
 *  \brief ガードアロケータの使用を無効にします
 */
extern void ADK_DisableGuardAllocator();
/**
 * \~english
 * \~japanese
 *  \brief ガードアロケータが有効かを返します
 *  \return 有効かのフラグ。以下の値が返ります
 *  \retval true  ガードアロケータが有効なときに返ります
 *  \retval false  ガードアロケータが無効なときに返ります
 */
extern Bool ADK_IsValid();

/* thread safe management functionally */
/**
 * \~english
 * \~japanese
 *  \brief 複数のスレッドから安全なメモリ割り当てを行うための
 *          ロック機能を設定します。\n
 *          ロック機能を無効にするには 'null' ポインタを渡します。
 *  \param[in] lock ロックコールバック
 *  \param[in] unlock アンロックコールバック
 */
extern void ADK_SetLockCallback(void (*lock)(void),void (*unlock)(void));
/**
 * \~english
 * \~japanese
 *  \brief スレッドのロックを行います
 */
extern void ADK_LockMemory();
/**
 * \~english
 * \~japanese
 *  \brief スレッドのアンロックを行います
 */
extern void ADK_UnlockMemory();

/* debug management functionally */
/**
 * \~english
 * \~japanese
 *  \brief 現在のメモリ状況をTXT形式で出力します
 *  \param[in] isLock ロック処理を行うかのフラグ
 *                     trueならロックをします
 *  \param[in] filename ファイル名
 *  \param[in] blockname ブロック名
 *  \param[in] output_backtrace バックトレースを出力するかのフラグ
 */
extern void ADK_OutputMemoryListForTXT(
    Bool isLock,const char *filename,const char *blockname,Bool output_backtrace);
/**
 * \~english
 * \~japanese
 *  \brief 指定のアドレスがガードアロケータで管理しているアドレス領域かチェックする
 *  \return false  指定のアドレスが管理内だったときに返ります
 *  \return true  指定のアドレスが管理外だったときに返ります
 */
extern Bool ADK_CheckBufferAddress(const void *addr);
/**
 * \~english
 * \~japanese
 *  \brief 現在のメモリの使用状況をログ出力します
 */
extern void ADK_PrintMemoryInformation();
/**
 * \~english
 * \~japanese
 *  \brief メモリへのブロック割り当てを開始します
 *  \attention この関数を呼び出した後に必ず \ref ADK_EndBlock を呼び出す必要があります
 *  \param[in] name ブロック名
 *  \return ブロックハンドル
 */
extern ADK_MEMBLOCK_HANDLE ADK_BeginMemoryBlock(const char *name);
/**
 * \~english
 * \~japanese
 *  \brief メモリへのブロック割り当てを終了します
 *  \param[in] h ブロックハンドル
 *  \return 割り当て終了の成功判定。以下の値が返ります
 *  \retval true  割り当て終了に成功したときに返ります
 *  \retval false  割り当て終了に失敗したときに返ります
 */
extern Bool ADK_EndMemoryBlock(ADK_MEMBLOCK_HANDLE h);
/**
 * \~english
 * \~japanese
 *  \brief 現在のアロケーションカウントを取得します
 *  \return アロケーションカウント
 */
extern size_t ADK_GetAllocationCount();


extern size_t ADK_ResetAllocCount();
extern size_t ADK_GetAllocCount();


#ifdef __cplusplus
}
#endif  /* __cplusplus */


/* alloc funcs for C++ only */
#ifdef __cplusplus
#include <new>
#include <unistd.h>

/**
 * \~english
 * \~japanese
 *  \brief クラス内 new/delete オペレータ定義
 */
#if 1
#define ADK_MEM_CXX_CLASS_ALLOC_FUNCS(name) \
  public: \
   void *operator new(size_t size) { \
     return ADK_Malloc(size,name); \
   } \
   void operator delete(void *addr) { \
     ADK_Free(addr); \
   } \
   void *operator new[](size_t size) {\
     return ADK_Malloc(size,name); \
   } \
   void operator delete[](void *addr) { \
     ADK_Free(addr); \
   } \
   void *operator new(size_t/*size*/,void *addr) { \
     return addr; \
   }


#else
#define ADK_MEM_CXX_CLASS_ALLOC_FUNCS(name) \
    public: \
     void *operator new(size_t size) throw(std::bad_alloc) { \
       return ADK_Malloc(size,name); \
     } \
     void operator delete(void *addr) throw(){ \
       ADK_Free(addr); \
     } \
     void *operator new[](size_t size) throw(std::bad_alloc){ \
       return ADK_Malloc(size,name); \
     } \
     void operator delete[](void *addr) throw(){ \
       ADK_Free(addr); \
     } \
     void *operator new(size_t size,void *addr) { \
       return addr; \
     }


#endif

#if 0
void *operator new(size_t size,const char *str) throw(std::bad_alloc);
void *operator new[](size_t size,const char *str) throw(std::bad_alloc);
#endif
#endif /* __cplusplus */

#endif /* GUARDED_ALLOC_H_ */

