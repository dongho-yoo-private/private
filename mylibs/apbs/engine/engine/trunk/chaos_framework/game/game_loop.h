/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file game_loop.h
 * \ingroup game_group
 * \~english
 * \~japanese
 *  \brief ゲームのメインループとするクラスの定義部\n
 *         TODO いずれ破棄するかもしれない!!!!
 */

#ifndef GAME_LOOP_H_
#define GAME_LOOP_H_

#include <core/memory_allocation/guarded_alloc.h>

#include "application/application_types.h"

CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief ゲームのメインループを行うクラス
 */
class GameLoop {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  GameLoop();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  ~GameLoop();

  /**
   * \~english
   * \~japanese
   *  \brief CCObject からの継承。\n
   *         メインフレームよりCocoaから呼び出される更新メソッド\n
   *         ゲームのメインループとして使用する。
   *  \param[in] dt 更新時間(delta time) <- 時間なのか？フレームなのか？
   */
  virtual void update(float dt);

  /* Non-Copyable MixIn */
  ADK_NON_COPYABLE_CLASS(GameLoop);
  /* ADK Guardallocator funcation */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("GameLoop");
};

CHAOS_END_NAMESPACE

#endif  /* GAME_LOOP_H_ */

