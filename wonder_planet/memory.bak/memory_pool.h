/**
* CopyRight 2017 (c) WonderPlanet Inc. All Rights Reserved.
* \author dongho.yoo.
*/
#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

/**
* \brief メモリプールを初期化します。
* 
* \param ptr メモリプールに使うメモリのアドレス、0の場合はmemory_pool_initの中で生成します。
* \param size 指定したメモリのサイズ、ptrが0の場合はサイズ分を内部で割り当てます。
* \return 指定したメモリのアドレス
*/
extern void* mp_init(void* ptr, size_t size, size_t alignment=16, bool_t is_debugmode=false);
/**
* \brief メモリプールを削除します。
* 
* \param ptr memory_poo_initの戻り値
*/
extern void mp_destroy(void* ptr);
/**
* \brief メモリプールの機能を一時無効にします。
* \param ptr メモリぷーるのアドレス
*/
extern void mp_hook_pause(void* ptr);
/**
* \brief メモリプールの機能を再開します。
*/
extern void mp_hook_resume();
/**
* \brief メモリプールからメモリを割り当てます。\n
* ptrが0の場合は、mallocと同じ
* \param ptr メモリプールのアドレス
* \param n 割り当てるメモリのサイズ
* \return 割り当てられたメモリのアドレス
*/
extern void* mp_malloc(void* ptr, size_t n);
/**
* \brief メモリプールからメモリを削除します。\n
* ptrが0の場合は、freeと同じ
* \param ptr メモリプールのアドレス
* \param data mp_mallocで割り当てられたアドレス
*/
extern void mp_free(void* ptr,void*data);

#ifdef __cplusplus
/**
* \brief メモリプールから new します。
*/
extern void* operator new (void* ptr, size_t size);
/**
* \brief メモリプールから delete します。
*/
extern void operator delete (void* ptr, void* data);
#endif /* __cplusplus */

#endif
