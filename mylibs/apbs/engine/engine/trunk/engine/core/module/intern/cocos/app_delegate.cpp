/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file app_delegate.h
 * \ingroup application_group
 * \~english
 * \~japanese
 *  \brief
 */

#include <core/module/intern/cocos/app_delegate.h>

#include <core/debug/debug_assert.h>
#include <core/module/entrypoint.h>

typedef struct {
  cocos2d::CCSize size;
  char directory[100];
} TScreen;

namespace {
const TScreen gNormalScreen = {cocos2d::CCSizeMake(320, 480), "iphone"};
const TScreen gRetinaScreen = {cocos2d::CCSizeMake(640, 960), "iphonehd"};
const cocos2d::CCSize gDesignResolutionSize = cocos2d::CCSizeMake(320, 480);
}

/**
 * \~english
 * \~japanese
 *  \brief アプリケーションの基準となるFPS
 */
#define APPLICATION_FPS (60.0f)
/**
 * \~english
 * \~japanese
 *  \brief Cocos2d-xの基準となるアニメーションインターバル
 */
#define COCOS_ANIMATION_INTERVAL (1.0f / APPLICATION_FPS)

AppDelegate::AppDelegate() : main_loop_() {}

AppDelegate::~AppDelegate() {}

bool AppDelegate::applicationDidFinishLaunching() {
  Bool ret = true;

  cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
  ADK_ASSERT(director);
  cocos2d::CCEGLView *egl_view = cocos2d::CCEGLView::sharedOpenGLView();
  ADK_ASSERT(egl_view);
  director->setOpenGLView(egl_view);
  director->setDisplayStats(true);
  director->setAnimationInterval(COCOS_ANIMATION_INTERVAL);

  AdjustScreen();

  ret = ADK_InitializeApplication();
  if (ret != false) {
    cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
    cocos2d::CCScheduler *scheduler = director->getScheduler();
    scheduler->scheduleUpdateForTarget(&main_loop_, 10, false);
  }

  return ret;
}
void AppDelegate::applicationDidEnterBackground() {
  cocos2d::CCDirector::sharedDirector()->stopAnimation();
}
void AppDelegate::applicationWillEnterForeground() {
  cocos2d::CCDirector::sharedDirector()->startAnimation();
}
/* TODO.adk4app : RetinaでScene画面がずれる対処として応急処置 */
void AppDelegate::AdjustScreen() {
  cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
  cocos2d::CCEGLView *egl_view = cocos2d::CCEGLView::sharedOpenGLView();

  /* Set the design resolution */
  egl_view->setDesignResolutionSize(gDesignResolutionSize.width,
                                    gDesignResolutionSize.height,
                                    kResolutionShowAll);

  cocos2d::CCSize frameSize = egl_view->getFrameSize();
  std::vector<std::string> serach_paths;
  serach_paths.push_back("iphonehd/");
  serach_paths.push_back(gRetinaScreen.directory);
  director->setContentScaleFactor(gRetinaScreen.size.height /
                                  gDesignResolutionSize.height);

  cocos2d::CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(
      serach_paths);
}
