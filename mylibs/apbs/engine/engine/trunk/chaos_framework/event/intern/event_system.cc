/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file event_system.cc
 * \ingroup event_group
 * \~english
 * \~japanese
 *  \brief アプリケーション扱うイベントの操作を行うシステム実装部
 */

#include "event/event_system.h"

#include <application/application_manager.h>
#include <event/event_key_types.h>
#include <scene/system/scene/scene_manager.h>
#include <scene/system/scene/scene_types.h>

CHAOS_BEGIN_NAMESPACE


static ADK_INLINE void *ListNext(void *t) {
  return *(reinterpret_cast<void**>(t));
}
static ADK_INLINE void ListSetNext(void *t, void *n) {
  *(reinterpret_cast<void**>(t)) = n;
}
void ListPush(void **list, void *element) {
  ListSetNext(element, *list);
  *list = element;
}
void *ListPop(void **list) {
  void *result = *list;
  *list = ListNext(*list);
  return result;
}
Bool IsList(void *list) {
  return list==0 ? false : true;
}

EventSystem::EventSystem()
  : free_event_list_(0),
    do_event_list_(0) {
}
EventSystem::~EventSystem() {
}
bool EventSystem::Initialize() {
  TEvent *event = events_;
  for (S32 count=0;count<kEventMax;++count,++event) {
    ListPush(&free_event_list_,event);
  }
  return true;
}
bool EventSystem::Terminate() {
  return true;
}
void EventSystem::DispatchEvent() {
  if (IsList(do_event_list_)) {
    TEvent *event_object = (TEvent*)ListPop(&do_event_list_);
    const Event *event = &event_object->event;
    switch(event->GetType()) {
      case Event::kSuccessfulUpdateData: {
        break;
      }
      case Event::kFailedUpdateData: {
        break;
      }
      case Event::kNextScene: {
        ApplicationManager *application_manager = ApplicationManager::GetInstance();
        SceneManager *scene_manager = application_manager->GetSceneManager();
        const EventArgument *event_argument = event->KeyForArgument(EVENT_KEY_SCENE_ID);
        scene_manager->ReplaceScene((TSceneId)event_argument->GetS32());
        break;
      }
      default : {
        ADK_ASSERT(false);
      }
    }
    event_object->event.Release();
    ListPush(&free_event_list_,event_object);
  }
}
Event* EventSystem::CreateEvent() {
  Event *ret = 0;
  if (IsList(free_event_list_)) {
    TEvent *event = (TEvent*)ListPop(&free_event_list_);
    ret = &event->event;
  }
  return ret;
}
void EventSystem::AddEvent(Event *event) {
  TEvent *event_object = FoundEvent(event);
  ListPush(&do_event_list_,event_object);
}
EventSystem::TEvent* EventSystem::FoundEvent(Event *event) {
  TEvent *ret = events_;
  for(S32 count=0;count<kEventMax;++count,++ret) {
    if (&ret->event==event) return ret;
  }
  ADK_ASSERT(false);
  return 0;
}

CHAOS_END_NAMESPACE

