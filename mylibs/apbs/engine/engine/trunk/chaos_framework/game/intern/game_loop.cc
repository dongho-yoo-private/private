/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file game_loop.cc
 * \ingroup game_group
 * \~english
 * \~japanese
 *  \brief ゲームのメインループとするクラスの実装部\n
 *         TODO いずれ破棄するかもしれない!!!!
 */

#include "game/game_loop.h"

#include "application/application_manager.h"
#include "event/event_system.h"

CHAOS_BEGIN_NAMESPACE

GameLoop::GameLoop() {
}
GameLoop::~GameLoop() {
}
void GameLoop::update(float dt) {
  ApplicationManager *application_manager = ApplicationManager::GetInstance();

  EventSystem *event_system = application_manager->GetEventSystem();
  event_system->DispatchEvent();
}

CHAOS_END_NAMESPACE

