/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file main_loop.h
 * \ingroup application_group
 * \~english
 * \~japanese
 *  \brief 
 */

#ifndef MAIN_LOOP_H_
#define MAIN_LOOP_H_

#include <cocos2d.h>

#include <core/types.h>

/**
 * \~english
 *  \brief 
 * \~japanese
 *  \brief 
 */
class MainLoop : public cocos2d::CCObject {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  MainLoop();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~MainLoop();

  /**
   * \~english
   * \~japanese
   *  \brief CCObject からの継承。\n
   *         メインフレームよりCocoaから呼び出される更新メソッド\n
   *         ゲームのメインループとして使用する。
   *  \param[in] dt 更新時間(delta time) <- 時間なのか？フレームなのか？
   */
  virtual void update(float dt);
};

#endif  /* MAIN_LOOP_H_ */

