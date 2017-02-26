/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file scene_interface.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief アプリケーション中のシーンの元となるインタフェース定義部
 */

#ifndef SCENE_INTERFACE_H_ 
#define SCENE_INTERFACE_H_

#include "scene/system/scene/scene_types.h"

CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief アプリケーションで実装していく各シーンのインタフェース
 */
class IScene {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~IScene();

  /**
   * \~english
   * \~japanese
   *  \brief シーンの初期化処理を行う
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval true 初期化に成功したときに返ります
   *  \retval false 初期化に失敗したときに返ります
   */
  virtual bool Initialize() = 0;
  /**
   * \~english
   * \~japanese
   *  \brief シーンに必要なリソースのロード処理を行います。\n
   *         (.ccbiファイル等)
   *  \return ロードの成功判定。以下の値が返ります
   *  \retval true ロードに成功したときに返ります
   *  \retval false ロードに失敗したときに返ります
   */
  virtual bool Load() = 0;
  /**
   * \~english
   * \~japanese
   *  \brief シーンを構築します。
   *  \return 構築の成功判定。以下の値が返ります
   *  \retval true 構築に成功したときに返ります
   *  \retval false 構築に失敗したときに返ります
   */
  virtual bool BuildScene() = 0;
  /**
   * \~english
   * \~japanese
   *  \brief シーンの終了処理を行う
   *  \return 終了の成功判定。以下の値が返ります
   *  \retval true 終了に成功したときに返ります
   *  \retval false 終了に失敗したときに返ります
   */
  virtual bool Terminate() = 0;
};

CHAOS_END_NAMESPACE

#endif  /* SCENE_INTERFACE_H_ */

