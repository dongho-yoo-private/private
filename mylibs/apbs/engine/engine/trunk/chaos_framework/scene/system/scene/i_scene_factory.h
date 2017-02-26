/**
 * Copyright 2014 Applibot Inc.
 * \author dongho yoo
 * \file scene_manager.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief Sceneのファクトリークラス。アプリ側ではこのクラスを継承して実装します。
 */

#ifndef _I_SCENE_FACTORY_H_
#define _I_SCENE_FACTORY_H_

#include "scene.h"

CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief SceneのIDからSceneクラスを生成するファクトリークラス
 */
class ISceneFactory {
public:
  /**
  * \brief このメソッドの中で、idに当てはまるSceneを生成します。
  *
  * \param scene_id SceneId, 始めに表示されるSceneはIDを０にしてください。
  * 
  * \return 生成したSceneクラスのポインター
  */
  virtual Scene* CreateScene(TSceneId scene_id)=0;
};
CHAOS_END_NAMESPACE

#endif
