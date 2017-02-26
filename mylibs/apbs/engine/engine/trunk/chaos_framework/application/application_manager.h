/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file application_manager.h
 * \ingroup application_manager_group
 * \~english
 * \~japanese
 *  \brief アップケーション全体を統括しマネジメントに関する定義部
 */

#ifndef APPLICATION_MANAGER_H_
#define APPLICATION_MANAGER_H_

#include <core/types.h>
#include <core/memory_allocation/guarded_alloc.h>
#include "application/application_types.h"
#include "application/i_application.h"

CHAOS_BEGIN_NAMESPACE

/* forward declaration */
#ifdef WITH_PARAMETER_MANAGER
  class ParameterManager;
#endif  /* WITH_PARAMETER_MANAGER */
#ifdef WITH_ERROR_HANDLER
  class ErrorHandler;
#endif  /* WITH_ERROR_HANDLER */
#ifdef WITH_EVENT_SYSTEM
  class EventSystem;
#endif  /* WITH_EVENT_SYSTEM */
#ifdef WITH_GAME_MANAGER
  class GameManager;
#endif  /* WITH_GAME_MANAGER */
#ifdef WITH_SCENE_MANAGER
  class SceneManager;
#endif  /* WITH_SCENE_MANAGER */
#ifdef WITH_TDD
  class TDDBase;
#endif  /* WITH_TDD */
#ifdef WITH_PARAMETER_UPDATE_MANAGER
  class ParameterUpdateManager;
#endif  /* WITH_PARAMETER_UPDATE_MANAGER */
#ifdef WITH_FILE_MANAGER
  class FileManager;
#endif  /* WITH_FILE_MANAGER*/
#ifdef WITH_SCHEDULE_MANAGER
  class ScheduleManager;
#endif  /* WITH_SCHEDULE_MANAGER*/

/**
 * \~english
 * \~japanese
 *  \brief アプリケーション全体を管理、統括するクラスです。
 *         このクラスから \ref GameManager や \ref TDDBase など、
 *         アプリケーション進行の元となる管理を行います。
 */
class ApplicationManager {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  ApplicationManager();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  ~ApplicationManager();

  /**
   * \~english
   * \~japanese
   *  \brief アプリケーションマネージャを初期化する
   *  \return 初期化処理の成功判定。以下の値が返ります
   *  \retval true 初期化処理に成功したときに返ります
   *  \retval false 初期化処理に成功したときに返ります
   */
  Bool Initialize(IApplication* application);
  /**
   * \~english
   * \~japanese
   *  \brief アプリケーションマネージャを終了する
   *  \return 終了処理の成功判定。以下の値が返ります
   *  \retval true 終了処理に成功したときに返ります
   *  \retval false 終了処理に成功したときに返ります
   */
  Bool Terminate();
 
  /**
  * \brief 登録したアプリケーションを取得します。
  */
  ADK_INLINE IApplication* GetApplication()
  {
    return application_;
  }
  
   /**
   * \~english
   * \~japanese
   *  \brief アプリケーションマネージャのインスタンスを返します
   *  \return \ref ApplicationManager
   */
  static ApplicationManager *GetInstance();
#ifdef WITH_GAME_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief ゲームマネージャオブジェクトを返します
   *  \return \ref GameManager
   */
  ADK_INLINE GameManager* GetGameManager();
#endif  /* WITH_GAME_MANAGER */
#ifdef WITH_TDD
  /**
   * \~english
   * \~japanese
   *  \brief TDDオブジェクトを返します
   *  \return \ref TDDBase
   */
  ADK_INLINE TDDBase* GetTDD();
#endif  /* WITH_TDD */
#ifdef WITH_ERROR_HANDLER
  /**
   * \~english
   * \~japanese
   *  \brief エラーハンドラーオブジェクトを返します
   *  \return \ref ErrorHandler
   */
  ADK_INLINE ErrorHandler* GetErrorHandler();
#endif  /* WITH_ERROR_HANDLER */
#ifdef WITH_PARAMETER_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief データマネージャオブジェクトを返します
   *  \return \ref ParameterManager
   */
  ADK_INLINE ParameterManager* GetParameterManager();
#endif  /* WITH_DATA_MANAGER */
#ifdef WITH_PARAMETER_UPDATE_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief データマネージャオブジェクトを返します
   *  \return \ref ParameterManager
   */
  ADK_INLINE ParameterUpdateManager* GetParameterUpdateManager();
#endif  /* WITH_PARAMETER_UPDATE_MANAGER */
#ifdef WITH_FILE_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief データマネージャオブジェクトを返します
   *  \return \ref ParameterManager
   */
  ADK_INLINE FileManager* GetFileManager();
#endif  /* WITH_FILE_MANAGER*/
#ifdef WITH_SCENE_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief シーンマネージャオブジェクトを返します
   *  \return \ref SceneManager
   */
  ADK_INLINE SceneManager* GetSceneManager();
#endif  /* WITH_SCENE_MANAGER */
#ifdef WITH_EVENT_SYSTEM
  /**
   * \~english
   * \~japanese
   *  \brief イベントシステムオブジェクトを返します
   *  \return \ref EventSystem
   */
  ADK_INLINE EventSystem* GetEventSystem();
#endif  /* WIHT_EVENT_SYSTEM */
#ifdef WITH_SCHEDULE_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief スケジュールマネージャオブジェクトを返します
   *  \return \ref ScheduleManager
   */
  ADK_INLINE ScheduleManager* GetScheduleManager();
#endif  /* WIHT_SCHEDULE_MANAGER */
  
  
 private:
  /**
   * \~english
   * \~japanese
   *  \brief 唯一無二のアプリケーションマネージャオブジェクト
   */
  static ApplicationManager *application_manager_;
#ifdef WITH_PARAMETER_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief アプリケーションで使われるデータ管理オブジェクト
   */
  ParameterManager *parameter_manager_;
#endif  /* WITH_DATA_MANAGER */
#ifdef WITH_PARAMETER_UPDATE_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief アプリケーションで使われるデータ管理オブジェクト
   */
  ParameterUpdateManager *parameter_update_manager_;
#endif  /* WITH_PARAMETER_UPDATE_MANAGER*/
#ifdef WITH_FILE_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief アプリケーションで使われるデータ管理オブジェクト
   */
  FileManager *file_manager_;
#endif  /* WITH_FILE_MANAGER*/
#ifdef WITH_ERROR_HANDLER
  /**
   * \~english
   * \~japanese
   *  \brief アプリケーション中のエラーを管理するオブジェクト
   */
  ErrorHandler *error_handler_;
#endif  /* WITH_ERROR_HANDLER */
#ifdef WITH_EVENT_SYSTEM
  /**
   * \~english
   * \~japanese
   *  \brief アプリケーションで使われるイベント管理オブジェクト
   */
  EventSystem *event_system_;
#endif  /* WITH_EVENT_SYSTEM */
#ifdef WITH_GAME_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief ゲーム進行を管理するオブジェクト
   */
  GameManager *game_manager_;
#endif  /* WITH_GAME_MANAGER */
#ifdef WITH_SCENE_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief アプリケーションでシーン管理を行うオブジェクト
   */
  SceneManager *scene_manager_;
#endif  /* WITH_SCENE_MANAGER */
#ifdef WITH_SCHEDULE_MANAGER
  /**
   * \~english
   * \~japanese
   *  \brief アプリケーションで使われるスケジュール管理オブジェクト
   */
  ScheduleManager *schedule_manager_;
#endif  /* WITH_SCHEDULE_MANAGER */
#ifdef WITH_TDD
  /**
   * \~english
   * \~japanese
   *  \brief アプリケーションのTDD機能を提供するオブジェクト
   */
  TDDBase *tdd_;
#endif  /* WITH_TDD */

  /** \brief 呼び出すアプリケーション
  */
  IApplication* application_;

  /* Non-Copyable MixIn */
  ADK_NON_COPYABLE_CLASS(ApplicationManager);
  /* ADK Guardallocator funcation */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("ApplicationManager");
};
#ifdef WITH_PARAMETER_MANAGER
ADK_INLINE ParameterManager* ApplicationManager::GetParameterManager() {
  return parameter_manager_;
}
#endif  /* WITH_DATA_MANAGER */
#ifdef WITH_ERROR_HANDLER
ADK_INLINE ErrorHandler* ApplicationManager::GetErrorHandler() {
  return error_handler_;
}
#endif  /* WITH_ERROR_HANDLER */
#ifdef WITH_EVENT_SYSTEM
ADK_INLINE EventSystem* ApplicationManager::GetEventSystem() {
  return event_system_;
}
#endif  /* WITH_EVENT_SYSTEM */
#ifdef WITH_GAME_MANAGER
ADK_INLINE GameManager* ApplicationManager::GetGameManager() {
  return game_manager_;
}
#endif  /* WITH_GAME_MANAGER */
#ifdef WITH_SCENE_MANAGER
ADK_INLINE SceneManager* ApplicationManager::GetSceneManager() {
  return scene_manager_;
}
#endif  /* WITH_SCENE_MANAGER */
#ifdef WITH_TDD
ADK_INLINE TDDBase* ApplicationManager::GetTDD() {
  return tdd_;
}
#endif  /* WITH_TDD */
#ifdef WITH_FILE_MANAGER
ADK_INLINE FileManager* ApplicationManager::GetFileManager() {
  return file_manager_;
}
#endif  /* WITH_FILE_MANAGER */
#ifdef WITH_PARAMETER_UPDATE_MANAGER
ADK_INLINE ParameterUpdateManager* ApplicationManager::GetParameterUpdateManager() {
  return parameter_update_manager_;
}
#endif  /* WITH_PARAMETER_UPDATE_MANAGER */
#ifdef WITH_SCHEDULE_MANAGER
ADK_INLINE ScheduleManager* ApplicationManager::GetScheduleManager() {
  return schedule_manager_;
}
#endif  /* WITH_SCHEDULE_MANAGER */




CHAOS_END_NAMESPACE

#endif  /* APPLICATION_MANAGER_H_ */

