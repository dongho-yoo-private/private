/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file scene.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief アプリケーション中のシーンクラス実装部
 */

#include "scene/system/scene/scene.h"

#include "application/application_manager.h"
#include "scene/system/scene/scene_manager.h"
#include "game/game_manager.h"

CHAOS_BEGIN_NAMESPACE

Scene::Scene() {
}
Scene::~Scene() {
}

bool Scene::ReplaceScene(TSceneId scene,const void *binary,size_t binary_size) {
  ApplicationManager *application_manager = ApplicationManager::GetInstance();
  GameManager *game_manager = application_manager->GetGameManager();
  return game_manager->RequestEventReplaceScene(scene,binary,binary_size);
}
void Scene::SetHeaderVisible(bool visible) {
}
void Scene::SetFooterVisible(bool visible) {
}

CHAOS_END_NAMESPACE

