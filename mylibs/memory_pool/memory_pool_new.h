#ifndef __MEMORY_POOL_NEW_H__
#define __MEMORY_POOL_NEW_H__
#include "memory_pool_defs.h"

/**
* \brief メモリプールから new します。
* \param handle メモリプールのハンドル
*
* \startcode
* // メモリプールのハンドルからアクセスする場合
* AnyClass* instance = new(handle) AnyClass();
* // グローバルのメモリリプールを使う場合は通常のnewでOK
* AnyClass* instance = new AnyClass();
* \endcode
*/
extern void* operator new (size_t size, handle_t handle);
/**
* \brief メモリプールから delete します。
* \param handle メモリプールのハンドル
*/
extern void operator delete (void* data, handle_t handle);

#endif
