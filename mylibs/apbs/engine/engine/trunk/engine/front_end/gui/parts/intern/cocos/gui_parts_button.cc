/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_common_loader.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief ファイルからパーツの内容を保持する共通ノードを生成します
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_button.h>

#include <core/debug/logging.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_system_cocos.h>

ADK_BEGIN_NAMESPACE

PartsButton::PartsButton() {}
PartsButton::~PartsButton() {}
bool PartsButton::ccTouchBegan(cocos2d::CCTouch *pTouch,
                               cocos2d::CCEvent *pEvent) {
  bool is_began =
      cocos2d::extension::CCControlButton::ccTouchBegan(pTouch, pEvent);
  return is_began;
}
void PartsButton::ccTouchMoved(cocos2d::CCTouch *pTouch,
                               cocos2d::CCEvent *pEvent) {
  cocos2d::extension::CCControlButton::ccTouchMoved(pTouch, pEvent);
}
void PartsButton::ccTouchEnded(cocos2d::CCTouch *pTouch,
                               cocos2d::CCEvent *pEvent) {
  cocos2d::extension::CCControlButton::ccTouchEnded(pTouch, pEvent);
}
void PartsButton::ccTouchCancelled(cocos2d::CCTouch *pTouch,
                                   cocos2d::CCEvent *pEvent) {
  cocos2d::extension::CCControlButton::ccTouchCancelled(pTouch, pEvent);
}

ADK_END_NAMESPACE

