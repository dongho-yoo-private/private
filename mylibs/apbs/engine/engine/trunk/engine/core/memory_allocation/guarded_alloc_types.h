/**
 * \file adk_guard_alloc_types.h
 * \ingroup adkgrp_galloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief ガードアロケータに必要な定義群
 */

#ifndef GUARDED_ALLOC_TYPES_H_
#define GUARDED_ALLOC_TYPES_H_

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include <core/debug/debug_assert.h>
#include <core/types.h>


/* debug function */
/**
 * \~english
 * \~japanese
 *  \brief メモリに名前を付加するデバッグ機能のON/OFF
 */
#define ADK_WITH_MEMORY_NAME
/**
 * \~english
 * \~japanese
 *  \brief メモリにブロック情報を付加するデバッグ機能のON/OFF
 */
#define ADK_WITH_MEMORY_BLOCK
/**
 * \~english
 * \~japanese
 *  \brief メモリにコールスタックを付加するデバッグ機能のON/OFF
 */
//#define ADK_WITH_MEMORY_CALLSTACK


/**
 * \~english
 * \~japanese
 *  \brief メモリ確保の方法
 */
typedef enum {
  /**
   * \~english
   * \~japanese
   *  前回確保した位置から確保する
   */
  ADK_kPrevAlloc,
  /**
   * \~english
   * \~japanese
   *  前方から確保する
   */
  ADK_kFrontAlloc,
  /**
   * \~english
   * \~japanese
   *  後方から確保する
   */
  ADK_kBackAlloc
}ADK_TAllocMethod;

/**
 * \~english
 * \~japanese
 *  \brief メモリブロックのハンドル
 */
ADK_DECLARATE_HANDLE(ADK_MEMBLOCK_HANDLE);

#endif  /* GUARDED_ALLOC_TYPES_H_ */

