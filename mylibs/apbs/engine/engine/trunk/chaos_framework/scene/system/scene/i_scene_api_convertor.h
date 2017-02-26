
/**
 * Copyright 2014 Applibot Inc.
 * \author dongho yoo
 * \file scene_manager.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief Sceneのファクトリークラス。アプリ側ではこのクラスを継承して実装します。
 */

#ifndef _I_SCENE_API_CONVERTOR_H_
#define _I_SCENE_API_CONVERTOR_H_

#include <application/application_types.h>
#include <scene/system/scene/scene_types.h>
#include <parameter/api_types.h>

CHAOS_BEGIN_NAMESPACE
/**
 * \~english
 * \~japanese
 *  \brief SceneのIDからSceneクラスを生成するファクトリークラス
 */
class ISceneApiConvertor {
public:
  /**
  * \brief このメソッドの中で、idに当てはまるSceneを生成します。
  *
  * \param scene_id Sceneに結びつくID
  * \return ApiName
  */
  virtual TApiName GetApiName(TSceneId scene_id)=0;
};
CHAOS_END_NAMESPACE

#endif
