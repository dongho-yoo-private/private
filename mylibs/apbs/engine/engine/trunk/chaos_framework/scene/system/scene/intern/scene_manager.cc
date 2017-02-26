/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file scene_manager.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief アプリケーション中のシーン管理を行うクラス実装部
 */

#include "application/application_manager.h"
#include "scene/system/scene/scene_manager.h"

#include <new>

#include <front_end/gui/parts/gui_parts.h>

//#include "scene/scene/my_page.h"

CHAOS_BEGIN_NAMESPACE

SceneManager::SceneManager() : active_scene_(0), scene_factory_(0), scene_api_convertor_(0) {}
SceneManager::~SceneManager() {}

bool SceneManager::Initialize() {
  bool ret = true;

  adk::Result result = adk::InitializeGuiPartsSystem();
  if (result.IsSuccess() == false) {
    ret = false;
  } else {
  }

  return ret;
}

bool SceneManager::InitializeScene() {
    ApplicationManager* application_manager = ApplicationManager::GetInstance();
    IApplication* app = application_manager->GetApplication();
    active_scene_ = ReplaceScene(app->GetFirstSceneId());
  
    return (active_scene_!=0);
}
bool SceneManager::Terminate() {
  bool ret = true;
  if (scene_factory_)
  {
    delete scene_factory_;
    scene_factory_=0;
  }
  if (scene_api_convertor_)
  {
    delete scene_api_convertor_;
    scene_api_convertor_ =0;
  }
  return ret;
}
void SceneManager::RegisterSceneFactory(ISceneFactory* factory)
{
  scene_factory_=factory;
}
void SceneManager::RegisterSceneApiConvertor(ISceneApiConvertor* convertor)
{
  scene_api_convertor_=convertor;
}
IScene *SceneManager::CreateScene(TSceneId scene) {
  Scene *ret = 0;
  ADK_ASSERT(scene_factory_);
  ret = scene_factory_->CreateScene(scene);

  if (ret != 0) {
    if (ret->Initialize() == false) {
      delete ret, ret = 0;
    } else {
      if (ret->Load() == false) {
        delete ret, ret = 0;
      } else {
        if (ret->BuildScene() == false) {
          delete ret, ret = 0;
        }
      }
    }
  }
  return (IScene*)ret;
}
TApiName SceneManager::GetApiName(TSceneId scene_id) const {

  return scene_api_convertor_->GetApiName(scene_id);
}
IScene *SceneManager::ReplaceScene(TSceneId scene) {
  static ADK_MEMBLOCK_HANDLE handle = 0;
  if (handle != 0) {
    ADK_EndMemoryBlock(handle), handle = 0;
  }
  static int number = 0;
  char name[16];
  sprintf(name, "%04d", number);
  ++number;
  handle = ADK_BeginMemoryBlock(name);
  if (active_scene_ != 0) {
    active_scene_->Terminate();
    delete active_scene_, active_scene_ = 0;
    ADK_OutputMemoryListForTXT(false, "test", "0000", true);
  }

  IScene *ret = CreateScene(scene);
  active_scene_ = ret;

  return ret;
}


CHAOS_END_NAMESPACE

