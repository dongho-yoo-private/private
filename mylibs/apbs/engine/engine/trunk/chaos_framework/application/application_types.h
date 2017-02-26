/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file application_manager_types.h
 * \ingroup application_manager_group
 * \~english
 * \~japanese
 *  \brief アップケーションに必要な定義部
 */

#ifndef APPLICATION_TYPES_H_
#define APPLICATION_TYPES_H_

/**
 * \~english
 * \~japanese
 *  \brief アプリケーションのネームスペースをサポートするかの定義
 */
#define WITH_SUPPORT_NAMESPACE
#ifdef WITH_SUPPORT_NAMESPACE
/**
 * \~english
 * \~japanese
 *  \brief Chaosの開始ネームスペース
 */
# define CHAOS_BEGIN_NAMESPACE  namespace chaos{
/**
 * \~english
 * \~japanese
 *  \brief Chaosの終了ネームスペース
 */
# define CHAOS_END_NAMESPACE  }
/**
 * \~english
 * \~japanese
 *  \brief Chaosネームスペース
 */
# define CHAOS chaos
#else
/**
 * \~english
 * \~japanese
 *  \brief Chaosの開始ネームスペース
 */
# define CHAOS_BEGIN_NAMESPACE
/**
 * \~english
 * \~japanese
 *  \brief Chaosの終了ネームスペース
 */
# define CHAOS_END_NAMESPACE
/**
 * \~english
 * \~japanese
 *  \brief Chaosネームスペース
 */
# define CHAOS
#endif  /* WITH_SUPPORT_NAMESPACE */

/*
 * アプリケーションでサポートするコンポーネント定義
 */
#define WITH_PARAMETER_MANAGER
#define WITH_ERROR_HANDLER
#define WITH_EVENT_SYSTEM
#define WITH_GAME_MANAGER
#define WITH_SCENE_MANAGER
#define WITH_FILE_MANAGER
#define WITH_PARAMETER_UPDATE_MANAGER
#define WITH_SCHEDULE_MANAGER
//#define WITH_TDD

#endif  /* APPLICATION_TYPES_H_ */

