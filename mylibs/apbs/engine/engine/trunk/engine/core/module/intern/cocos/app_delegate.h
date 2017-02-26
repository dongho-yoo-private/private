/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file app_delegate.h
 * \ingroup application_group
 * \~english
 * \~japanese
 *  \brief 
 */

#ifndef  APP_DELEGATE_H_
#define  APP_DELEGATE_H_

#include <cocos2d.h>

#include <core/types.h>
#include <core/module/intern/cocos/main_loop.h>

/**
 * \~english
 *  \brief cocos2d Application.
 *         The reason for implement as private inheritance is to hide some interface call by CCDirector.
 * \~japanese
 *  \brief 
 */
class AppDelegate : private cocos2d::CCApplication {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  AppDelegate();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~AppDelegate();

  /**
   * \~english
   *  \brief Implement CCDirector and CCScene init code here.
   *  \return true Initialize success, app continue.
   *  \return false Initialize failed, app terminate.
   * \~japanese
   *  \brief 
   */
  virtual bool applicationDidFinishLaunching();
  /**
   * \~english
   *  \brief The function be called when the application enter background
   * \~japanese
   *  \brief 
   */
  virtual void applicationDidEnterBackground();
  /**
   * \~english
   *  \brief The function be called when the application enter foreground
   * \~japanese
   *  \brief 
   */
  virtual void applicationWillEnterForeground();
 private:
  /**
   * \~english
   * \~japanese
   *  \brief アプリケーションが表示する画面をスクリーンサイズに合わせる
   */
  void AdjustScreen();
  /**
   * \~engilsh
   * \~japanese
   *  \brief Cocos2d-x用メインループオブジェクト
   */
  MainLoop main_loop_;
};

#endif  /* APP_DELEGATE_H_ */

