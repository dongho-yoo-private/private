/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_cc_parts_scroll_view.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief スクロールビューパーツクラス実装部 for Cocos2d-x
 */

#include <front_end/gui/parts/intern/cocos/gui_cc_parts_scroll_view.h>

#include <core/debug/logging.h>

ADK_BEGIN_NAMESPACE

GuiCcPartsScrollView::GuiCcPartsScrollView() {}
GuiCcPartsScrollView::~GuiCcPartsScrollView() {}

GuiCcPartsScrollView *GuiCcPartsScrollView::create(cocos2d::CCSize size,
                                                   cocos2d::CCNode *container) {
  GuiCcPartsScrollView *ret = new GuiCcPartsScrollView();
  if (ret != NULL && ret->initWithViewSize(size, container)) {
    ret->autorelease();
  } else {
    CC_SAFE_DELETE(ret);
  }
  return ret;
}

bool GuiCcPartsScrollView::ccTouchBegan(cocos2d::CCTouch *pTouch,
                                        cocos2d::CCEvent *pEvent) {
  return cocos2d::extension::CCScrollView::ccTouchBegan(pTouch, pEvent);
}
void GuiCcPartsScrollView::ccTouchMoved(cocos2d::CCTouch *pTouch,
                                        cocos2d::CCEvent *pEvent) {
  cocos2d::extension::CCScrollView::ccTouchMoved(pTouch, pEvent);
}

ADK_END_NAMESPACE

