/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_common_loader.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief ファイルからパーツの内容を保持する共通ノードを生成します
 */

#ifndef PARTS_BUTTON_H_
#define PARTS_BUTTON_H_

#include <extensions/CCBReader/CCControlButtonLoader.h>

#include <core/types.h>

ADK_BEGIN_NAMESPACE

class PartsButton : public cocos2d::extension::CCControlButton {
 public:
  CREATE_FUNC(PartsButton);

  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  PartsButton();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~PartsButton();

  virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
  virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
  virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
  virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch,
                                cocos2d::CCEvent *pEvent);
};

ADK_END_NAMESPACE

#endif /* PARTS_BUTTON_H_ */
