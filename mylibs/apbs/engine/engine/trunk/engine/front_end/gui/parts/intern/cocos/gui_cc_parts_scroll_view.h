/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_scroll_view.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief スクロールビューパーツクラス定義部
 */

#ifndef GIU_CC_PARTS_SCROLL_VIEW_H_
#define GIU_CC_PARTS_SCROLL_VIEW_H_

#include <extensions/CCBReader/CCScrollViewLoader.h>

#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_cc_parts_scroll_view.h>

ADK_BEGIN_NAMESPACE

class GuiCcPartsScrollView : public cocos2d::extension::CCScrollView {
 public:
  CREATE_FUNC(GuiCcPartsScrollView);

  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  GuiCcPartsScrollView();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~GuiCcPartsScrollView();

  static GuiCcPartsScrollView *create(cocos2d::CCSize size,
                                      cocos2d::CCNode *container = NULL);

  virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
  virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
};

ADK_END_NAMESPACE

#endif /* GIU_CC_PARTS_SCROLL_VIEW_H_ */
