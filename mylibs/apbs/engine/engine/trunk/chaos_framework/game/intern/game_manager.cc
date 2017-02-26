/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file game_manager.cc
 * \ingroup game_manager_group
 * \~english
 * \~japanese
 *  \brief アップケーション全体を統括しマネジメントに関する実装部
 */

#include "game/game_manager.h"

#include <core/debug/logging.h>
#include <core/library/lib_list.h>

#include "parameter/api_types.h"
#include "parameter/parameter_manager.h"
#include "parameter/parameter_update_manager.h"
#include "event/event.h"
#include "event/event_key_types.h"
#include "event/event_system.h"
#include "game/game_loop.h"
#include "scene/system/scene/scene_manager.h"

// 臨時的な措置
#define ADK_DEBUG_LOG(a)

CHAOS_BEGIN_NAMESPACE

GameManager::GameManager()
  : game_loop_(0) {
  memset(&request_data_,0,sizeof(request_data_));
}
GameManager::~GameManager() {
}
Bool GameManager::Initialize() {
  Bool ret = true;

  for (S32 count=0;count<kMaxDataRequest;++count) {
    adk::LIB_ForwardLinkedListPush(
        &request_data_.queue_list,&request_data_.queues[count]);
  }

  if (game_loop_) {
    ret = false;
  } else {
    game_loop_ = new GameLoop;
    if (game_loop_==0) {
      ADK_DEBUG_LOG("GameManager::Initialize(): Out of resource.\n");
      ret = false;
    } else {
    }
  }
  return ret;
}
Bool GameManager::Terminate() {
  Bool ret = true;
  if (game_loop_==0) {
    ret = false;
  } else {
  }
  return ret;
}
Bool GameManager::RequestEventReplaceScene(
    TSceneId scene_id,const void *binary,size_t binary_size) {
  Bool ret = true;
  ApplicationManager *application_manager = ApplicationManager::GetInstance();
  const SceneManager *scene_manager = application_manager->GetSceneManager();
  const TApiName api_name = scene_manager->GetApiName(scene_id);

  if (api_name==-1/*kApiUnknown*/) {
    AddEventReplaceScene(scene_id);
  } else {
    if (adk::LIB_IsForwardLinkedList(request_data_.queue_list)==false) {
    } else {
      TRequestDataQueue *queue =
          (TRequestDataQueue*)adk::LIB_ForwardLinkedListPop(
              &request_data_.queue_list);
      queue->next = 0;
      queue->scene_id = scene_id;

      ParameterUpdateManager* parameter_update_manager = application_manager->GetParameterUpdateManager();
      parameter_update_manager->RequestUpdate(api_name, binary, queue);
      /*
       * この後 DataManager::RequestUpdate により
       * リクエスト完了のイベントが発生する
       * そしたら、シーンを切り替える
       */
    }
  }

  return ret;
}
Bool GameManager::AddEventReplaceScene(TSceneId scene_id) {
  Bool ret = true;
  ApplicationManager *application_manager = ApplicationManager::GetInstance();
  EventSystem *event_system = application_manager->GetEventSystem();

  Event *replace_event = event_system->CreateEvent();
  if (replace_event==0) {
    ret = false;
  } else {
    replace_event->SetType(Event::kNextScene);
    replace_event->AddS32(EVENT_KEY_SCENE_ID,scene_id);
    event_system->AddEvent(replace_event);
  }

  return ret;
}
Bool GameManager::RequestEventDataUpdateCompleted(Bool is_success,const void *userdata)
{
  TRequestDataQueue* scene_data = (TRequestDataQueue*)userdata;
  
  if (is_success)
  {
    AddEventReplaceScene(scene_data->scene_id);
  }
  else
  {
    return false;
  }
  return true;
}

CHAOS_END_NAMESPACE

