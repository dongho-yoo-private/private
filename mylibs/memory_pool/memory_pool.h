/*
* CopyRight 2017 (c) dongho.yoo.
* \author dongho.yoo.
*/
#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__
#include "memory_pool_defs.h"

#ifdef __cplusplus
#   define EXTERN_C extern "C"
#else
#   define EXTERN_C extern
#endif
/**
* \brief メモリプールを初期化します。
*
* \param buffer メモリプールに使うメモリのアドレス、0の場合はmemory_pool_initの中で生成します。\n
* NULLの場合は、内部でヒープ上に指定したサイズ文のメモリを割り当てます。
* \param size 指定したメモリのサイズ、ptrが0の場合はサイズ分を内部で割り当てます。
* \return メモリプールのハンドル
*
* \startcode
* ex) バッファーをこちらから用意してメモリプールを利用する場合
* 
* int main(int argc, char* argv[])
* {
*   char buffer[4096];
*   handle_t h_memory = mp_init(buffer, sieof(buffer), 8);  
*   void* anyData=mp_malloc(h_memory,anySize);
*   AnyClass anyclass = new(h_memory) AnyClass();
*               :
*               :
*   mp_destroy(h_memory);
*   return 0;
* }
*
* ex) mallocなどをhookしてプールに利用する場合
* 
* int main(int argc, char* argv[])
* {
*   mp_init(NULL, MAX_APPLICATION_MEMORY_SIZE, 16);  
*   void* anyData=malloc(anySize);
*   AnyClass anyclass = new AnyClass();
*               :
*               :
*   mp_destroy(NULL);
*   return 0;
* }
* \endcode
*/
EXTERN_C handle_t mp_init(void* buffer, size_t size, size_t align);
/**
* \brief メモリプールを削除します。
* 
* \param handle メモリプールのハンドル
*/
EXTERN_C void mp_destroy(handle_t handle);
/**
* \brief メモリプールの機能を一時無効にします。\n
* 場合によっては、こちらからmallocを呼び出すが、ライブラリの中でfreeする場合などは、\n
* ライブラリの中では直接libcを参照する可能性があるためにそれを回避するためのものです。\n
* mp_hook_resumeで再開します。
*
* \param handle メモリプールのハンドル
*/
EXTERN_C void mp_hook_pause(handle_t handle);
/**
* \brief メモリプールの機能を再開します。
* \param handle メモリプールのハンドル
*/
EXTERN_C void mp_hook_resume(handle_t ptr);
/**
* \brief メモリプールからメモリを割り当てます。\n
* ptrが0の場合は、mallocと同じ
* \param handle メモリプールのハンドル
* \param n 割り当てるメモリのサイズ
* \return 割り当てられたメモリのアドレス
*/
EXTERN_C void* mp_malloc(handle_t handle, size_t n);
/**
* \brief メモリプールからメモリを削除します。\n
* ptrが0の場合は、freeと同じ
* \param handle メモリプールのハンドル
* \param data mp_mallocで割り当てられたアドレス
*/
EXTERN_C void mp_free(handle_t ptr,void*data);
/**
* \brief 現在のメモリプールの状態を出力します。（デバッグ用）
*/
EXTERN_C void mp_print(handle_t handle, bit32_t sequence_no);
/**
* \brief シーケンス番号を設定します。\n
* 特定の区間で使われているメモリを調べるために使われます。\n
* リークの調査などに役に立ちます。
* \param handle ハンドル
* \param sequence_no シーケンス番号
*
* \startcode
* ex)
* static void AnyClassFreed(void* data)
* {
*   while(0); // ここにBreak Pointを貼って監視する
* }
* void AnyView::initialize()
* {
*                 :
*                 :
*     mp_set_sequence_no(nullptr,1);
*                 :
*                 :
* }
* void AnyView::transitNextView()
* {
*                 :
*                 :
*     mp_set_sequence_no(nullptr,2);
*                 :
*                 :
* }
*
* void NextView::anyButtonTapped()
* {
*                   :
*                   :
*      // AnyViewのリソースが残っていないはずのタイミングで、
*      // メモリの状態をプリントします。
*      mp_print(nullptr,1);
*
* }
* \endcode
*/
EXTERN_C void mp_set_sequence_no(handle_t handle, bit32_t sequence_no);

#if MEMORY_DEBUG==1
typedef void (*mp_trap_t)(void* addr);
/**
* \brief 特定のメモリが解放するタイミングで指定したコールバックが呼び出されるようにします。\n
* メモリの二重解放などの検出用で使えます。
*
* \param handle メモリぷーるのハンドル、NULLの場合はグローバルのメモリプール
* \param addr トラップを仕掛けるメモリのアドレス
* \param callback メモリ解放時に呼び出されるコールバック
*
* \startcode
* ex)
* static void AnyClassFreed(void* data)
* {
*   while(0); // ここにBreak Pointを貼って監視する
* }
* void AnyClass::initialize()
* {
*                 :
*                 :
*     mp_set_trap(nullptr,this,&AnyClassFreed);
*                 :
*                 :
* }
* \endcode
*/
EXTERN_C void mp_set_trap(handle_t handle, void* addr, mp_trap_t callback);
#endif

#endif
