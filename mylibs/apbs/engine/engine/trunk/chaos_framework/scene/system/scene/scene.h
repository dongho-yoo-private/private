/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file scene.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief アプリケーション中のシーンクラス定義部
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <core/types.h>
#include <core/memory_allocation/guarded_alloc.h>
#include "application/application_types.h"
#include "scene/system/scene/scene_interface.h"

/* forward declaration */
ADK_BEGIN_NAMESPACE
class IGuiParts;
ADK_END_NAMESPACE

CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief シーン機能を提供するクラス\n
 *         このクラスに \ref Parts を関連づける
 */
class Scene : public IScene {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  Scene();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~Scene();

  /**
   * \~english
   * \~japanese
   *  \brief シーンの切り替えをリクエストする
   *  \param[in] scene 切り替えたい \ref TSceneId
   *  \param[in] binary 切り替えリクエスト時に使用するバイナリデータ
   *  \param[in] binary_size binary のサイズ
   */
  bool ReplaceScene(TSceneId scene, const void *binary, size_t binary_size);
  /**
   * \~english
   * \~japanese
   *  \brief ヘッダーの表示/非表示を設定します
   *  \param[in] visible trueなら表示
   *                     falseなら非表示
   */
  void SetHeaderVisible(bool visible);
  /**
   * \~english
   * \~japanese
   *  \brief フッターの表示/非表示を設定します
   *  \param[in] visible trueなら表示
   *                     falseなら非表示
   */
  void SetFooterVisible(bool visible);

 protected:
  /**
   * \~english
   * \~japanese
   *  \brief シーンの子としてオブジェクトを追加する
   *  \param[in] parts シーンの子にする \ref IParts
   */
  void AddPartsToScene(adk::IGuiParts *parts);
  /**
   * \~english
   * \~japanese
   *  \brief 子にしていたオブジェクトをシーンから切り離す
   */
  void RemovePartsFromScene(void *parts);

  /* Non-Copyable MixIn */
  ADK_NON_COPYABLE_CLASS(Scene);
  /* ADK Guardallocator funcation */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("Scene");
};

CHAOS_END_NAMESPACE

#endif /* SCENE_H_ */

