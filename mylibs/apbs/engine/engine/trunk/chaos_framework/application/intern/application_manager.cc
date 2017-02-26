/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file application_manager.cc
 * \ingroup application_manager_group
 * \~english
 * \~japanese
 *  \brief アップケーション全体を統括しマネジメントに関する実装部
 */

#include "application/application_manager.h"

#ifdef WITH_PARAMETER_MANAGER
# include "parameter/parameter_manager.h"
#endif  /* WITH_PARAMETER_MANAGER*/
#ifdef WITH_ERROR_HANDLER
# include "error_handler/error_handler.h"
#endif  /* WITH_ERROR_HANDLER */
#ifdef WITH_EVENT_SYSTEM
# include "event/event_system.h"
#endif  /* WITH_EVENT_SYSTEM */
#ifdef WITH_GAME_MANAGER
# include "game/game_manager.h"
#endif  /* WITH_GAME_MANAGER */
#ifdef WITH_SCENE_MANAGER
# include "scene/system/scene/scene_manager.h"
#endif  /* WITH_SCENE_MANAGER */
#ifdef WITH_TDD
# include "tdd/tdd_base.h"
#endif  /* WITH_TDD */
#ifdef WITH_PARAMETER_UPDATE_MANAGER
# include "parameter/parameter_update_manager.h"
#endif  /* WITH_PARAMETER_UPDATE_MANAGER*/
#ifdef WITH_FILE_MANAGER
# include "file/file_manager.h"
#endif  /* WITH_FILE_MANAGER*/
#ifdef WITH_SCHEDULE_MANAGER
# include "schedule/schedule_manager.h"
#endif  /* WITH_SCHEDULE_MANAGER*/


CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief イニシャライザリストによるメンバ変数の初期化を行います
 */
ApplicationManager *ApplicationManager::application_manager_ = 0;
ApplicationManager::ApplicationManager()
  :
#ifdef WITH_PARAMETER_MANAGER
    parameter_manager_(0)
#endif  /* WITH_DATA_MANAGER */
#ifdef WITH_ERROR_HANDLER
  , error_handler_(0)
#endif  /* WITH_ERROR_HANDLER */
#ifdef WITH_EVENT_SYSTEM
  , event_system_(0)
#endif  /* WITH_EVENT_SYSTEM */
#ifdef WITH_GAME_MANAGER
  , game_manager_(0)
#endif  /* WITH_GAME_MANAGER */
#ifdef WITH_SCENE_MANAGER
  , scene_manager_(0)
#endif  /* WITH_SCENE_MANAGER */
#ifdef WITH_TDD
  , tdd_(0)
#endif  /* WITH_TDD */
#ifdef WITH_FILE_MANAGER
  , file_manager_(0)
#endif  /* WITH_FILE_MANAGER*/
#ifdef WITH_PARAMETER_UPDATE_MANAGER
  , parameter_update_manager_(0)
#endif  /* WITH_PARAMETER_UPDATE_MANAGER */
  , application_(0)
#ifdef WITH_SCHEDULE_MANAGER
  , schedule_manager_(0)
#endif  /* WITH_SCHEDULE_MANAGER */
{
}
/**
 * \~english
 * \~japanese
 *  \brief do nothing
 */
ApplicationManager::~ApplicationManager() {
}

/**
 * \~english
 * \~japanese
 *  \brief アプリケーション進行選択画面の初期化処理を行います
 */
Bool ApplicationManager::Initialize(IApplication* application) {
  if (application_manager_!=0) {
    //ADK_CRITICAL_LOG("The application manager is already initialized.");
    goto init_error;
  } else {
    application_manager_ = this;
  }
  
  if (application==0) {
    goto init_error;
  } else {
    application_=application;
  }

#ifdef WITH_TDD
  tdd_ = new TDDBase;
  if (tdd_==0) {
    ADK_ASSERT(false);
  } else {
# if 0
    Bool result = tdd_->Initialize();
    if (result==false) {
      goto init_error;
    }
# endif
  }
#endif  /* WITH_TDD */

#ifdef WITH_ERROR_HANDLER
  error_handler_ = new ErrorHandler;
  if (error_handler_==0) {
    ADK_ASSERT(false);
  } else {
# if 0
    Bool result = error_handler_->Initialize();
    if (result==false) {
      goto init_error;
    }
# endif
  }
#endif  /* WITH_ERROR_HANDLER */

#ifdef WITH_PARAMETER_MANAGER
  parameter_manager_ = new ParameterManager;
  if (parameter_manager_==0) {
    ADK_ASSERT(false);
  } else {
    Bool result = parameter_manager_->Initialize();
    if (result==false) {
      goto init_error;
    }
  }
#endif  /* WITH_PARAMETER_MANAGER */

#ifdef WITH_EVENT_SYSTEM
  event_system_ = new EventSystem;
  if (event_system_==0) {
    ADK_ASSERT(false);
  } else {
    Bool result = event_system_->Initialize();
    if (result==false) {
      goto init_error;
    }
  }
#endif  /* WITH_EVENT_SYSTEM */

#ifdef WITH_GAME_MANAGER
  game_manager_ = new GameManager;
  if (game_manager_==0) {
    ADK_ASSERT(false);
  } else {
    Bool result = game_manager_->Initialize();
    if (result==false) {
      goto init_error;
    }
  }
#endif  /* WITH_GAME_MANAGER */

#ifdef WITH_SCENE_MANAGER
  scene_manager_ = new SceneManager;
  if (scene_manager_==0) {
    ADK_ASSERT(false);
  } else {
    Bool result = scene_manager_->Initialize();
    if (result==false) {
      goto init_error;
    }
  }
#endif  /* WITH_SCENE_MANAGER */

#ifdef WITH_FILE_MANAGER
  file_manager_ = new FileManager;
  if (file_manager_ ==0) {
    ADK_ASSERT(false);
  } else {
    Bool result = file_manager_->Initialize();
    if (result==false) {
      goto init_error;
    }
  }
#endif  /* WITH_FILE_MANAGER*/

#ifdef WITH_PARAMETER_UPDATE_MANAGER
  parameter_update_manager_ = new ParameterUpdateManager;
  if (parameter_update_manager_ ==0) {
    ADK_ASSERT(false);
  } else {
    Bool result = parameter_update_manager_->Initialize();
    if (result==false) {
      goto init_error;
    }
  }
#endif  /* WITH_PARAMETER_UPDATE_MANAGER*/

#ifdef WITH_SCHEDULE_MANAGER
  schedule_manager_ = new ScheduleManager;
  if (schedule_manager_ ==0) {
    ADK_ASSERT(false);
  } else {
    Bool result = schedule_manager_->Initialize();
    if (result==false) {
      goto init_error;
    }
  }
#endif  /* WITH_SCHEDULE_MANAGER*/


  if (application_)
  {
    application_->Initialize();
  }
  
#ifdef WITH_SCENE_MANAGER
  scene_manager_->InitializeScene();
#endif /* WITH_SCENE_MANAGER */
  return true;

init_error:

  Terminate();

  return false;
}
/**
 * \~english
 * \~japanese
 *  \brief アプリケーションマネージャを終了する
 */
Bool ApplicationManager::Terminate() {
  Bool ret = true;

#ifdef WITH_GAME_MANAGER
  if (game_manager_==0) {
    ret = false;
  } else {
    delete game_manager_, game_manager_ = 0;
  }
#endif  /* WITH_GAME_MANAGER */

#ifdef WITH_EVENT_SYSTEM
  if (event_system_==0) {
    ret = false;
  } else {
    delete event_system_, event_system_ = 0;
  }
#endif  /* WITH_EVENT_SYSTEM */

#ifdef WITH_SCENE_MANAGER
  if (scene_manager_==0) {
    ret = false;
  } else {
    delete scene_manager_, scene_manager_ = 0;
  }
#endif  /* WITH_SCENE_MANAGER */

#ifdef WITH_PARAMETER_MANAGER
  if (parameter_manager_==0) {
    ret = false;
  } else {
    delete parameter_manager_, parameter_manager_ = 0;
  }
#endif  /* WITH_PARAMETER_MANAGER*/

#ifdef WITH_PARAMETER_UPDATE_MANAGER
  if (parameter_update_manager_==0) {
    ret = false;
  } else {
    delete parameter_update_manager_, parameter_update_manager_= 0;
  }
#endif  /* WITH_PARAMETER_UPDATE_MANAGER*/

#ifdef WITH_FILE_MANAGER
  if (file_manager_==0) {
    ret = false;
  } else {
    delete file_manager_, file_manager_= 0;
  }
#endif  /* WITH_FILE_MANAGER*/

#ifdef WITH_ERROR_HANDLER
  if (error_handler_==0) {
    ret = false;
  } else {
    delete error_handler_, error_handler_ = 0;
  }
#endif  /* WITH_ERROR_HANDLER */

#ifdef WITH_TDD
  if (tdd_==0) {
    ret = false;
  } else {
    delete tdd_, tdd_ = 0;
  }
#endif  /* WITH_TDD */
  
#ifdef WITH_SCHEDULE_MANAGER
  if (schedule_manager_==0) {
    ret = false;
  } else {
    delete schedule_manager_, schedule_manager_= 0;
  }
#endif  /* WITH_SCHEDULE_MANAGER*/

  if (application_)
  {
    application_->Terminate();
  }

  return ret;
}
ApplicationManager *ApplicationManager::GetInstance() {
  return application_manager_;
}

CHAOS_END_NAMESPACE

