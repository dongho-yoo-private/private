/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file scene_manager.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief アプリケーション中のシーン管理を行うクラス定義部
 */

#ifndef SCENE_MANAGER_H_ 
#define SCENE_MANAGER_H_

#include <core/types.h>
#include <application/application_types.h>
#include <scene/system/scene/scene_types.h>
#include <parameter/api_types.h>
#include "i_scene_factory.h"
#include "i_scene_api_convertor.h"


CHAOS_BEGIN_NAMESPACE

/* forward declaration */
class IScene;

/**
 * \~english
 * \~japanese
 *  \brief アプリケーション中に登場するシーンを管理するクラス
 */
class SceneManager {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  SceneManager();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~SceneManager();

  /**
   * \~english
   * \~japanese
   *  \brief シーンマネージャを初期化します
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval true 初期化に成功したときに返ります
   *  \retval false 初期化に失敗したときに返ります
   */
  bool Initialize();
  /**
   * \~english
   * \~japanese
   *  \brief シーンマネージャを終了します
   *  \return 終了の成功判定。以下の値が返ります
   *  \retval true 終了に成功したときに返ります
   *  \retval false 終了に失敗したときに返ります
   */
  bool Terminate();
  /**
   * \~english
   * \~japanese
   *  \brief 初期のシーンに遷移します。
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval true 初期化に成功したときに返ります
   *  \retval false 初期化に失敗したときに返ります
  */
  bool InitializeScene();
  /**\brief 
  * ISceneFactoryを登録します。\n
  * アプリケーション側では、ISceneFactoryのインスタンスを設定しなければなりません。
  *
  * \param factory \ref ISceneFactory
  */
  void RegisterSceneFactory(ISceneFactory* facotry);

  /**\brief
  * アプリケーション側のインターフェースになる、sceneのIDをAPIに変換してくれるインスタンスを設定しなければなりません。
  *
  * \param factory \ref ISceneApiConvertor
  */
  void RegisterSceneApiConvertor(ISceneApiConvertor* convertor);

  /**
   * \~english
   * \~japanese
   *  \brief 現在動いているシーンを scene で指定されたシーンへ切り替えます
   *  \param scene \ref TSceneId
   *  \return 切り替えた \ref Scene
   */
  IScene *ReplaceScene(TSceneId scene);
  /**
   * \~english
   * \~japense
   *  \brief シーンに関連したAPIを取得します。
   *  \param[in] scene_id シーンID
   *  \return \ref TApiName
   */
  TApiName GetApiName(TSceneId scene_id) const;

 private:
  /**
   * \~english
   * \~japanese
   *  \brief scene で指定したシーンの作成を行います
   *  \param scene \ref TSceneId
   */
  IScene *CreateScene(TSceneId scene);

  /**
   * \~english
   * \~japanese
   *  \brief 現在アクティブなシーン
   */
  IScene *active_scene_;

  /**
   * \~english
   * \~japanese
   *  \brief アプリケーション側のインターフェース
  */
  ISceneFactory * scene_factory_;

  /**
   * \~english
   * \~japanese
   *  \brief アプリケーション側のインターフェース
  */
  ISceneApiConvertor* scene_api_convertor_;
};

CHAOS_END_NAMESPACE

#endif  /* SCENE_MANAGER_H_ */

