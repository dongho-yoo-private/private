/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_common_node_loader.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief
 * ファイルからパーツの内容を読み込み、内部ノードに設定するクラスの定義部
 */

#ifndef __Chaos__parts_common_node_loader__
#define __Chaos__parts_common_node_loader__

#include <cocos2d.h>
#include <extensions/CCBReader/CCBAnimationManager.h>
#include <extensions/CCBReader/CCBMemberVariableAssigner.h>
#include <extensions/CCBReader/CCBSelectorResolver.h>

#include <front_end/gui/parts/intern/cocos/gui_parts_call_func.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_member.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_menu.h>

namespace cocos2d {
namespace extension {
class CCBReader;
}
}

ADK_BEGIN_NAMESPACE

class PartsCommonNodeLoader
    : public cocos2d::CCLayer,
      public cocos2d::extension::CCBMemberVariableAssigner,
      public cocos2d::extension::CCBSelectorResolver {
 public:
  CREATE_FUNC(PartsCommonNodeLoader);

  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~PartsCommonNodeLoader();
  /**
   * \~english
   * \~japanese
   *  \brief ローダーの初期化処理を行います
   */
  virtual bool init();
  /**
   * \~english
   * \~japanese
   *  \brief
   * ファイルを読み込み、関数呼び出しのセレクタ名と親ノードのポインタを保持する
   */
  virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(
      CCObject* pTarget, const char* pSelectorName);
  /**
   * \~english
   * \~japanese
   *  \brief
   * ファイルを読み込み、メニューアイテムのコールバックのセレクタ名と親ノードのポインタを保持する
   */
  virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(
      CCObject* pTarget, const char* pSelectorName);
  /**
   * \~english
   * \~japanese
   *  \brief
   * ファイルを読み込み、コントロールのコールバックのセレクタ名と親ノードのポインタを保持する
   */
  virtual cocos2d::extension::SEL_CCControlHandler
      onResolveCCBCCControlSelector(CCObject* pTarget,
                                    const char* pSelectorName);
  /**
   * \~english
   * \~japanese
   *  \brief ファイルを読み込み、メンバ変数名とノードを保持する
   */
  virtual bool onAssignCCBMemberVariable(CCObject* pTarget,
                                         const char* pMemberVariableName,
                                         CCNode* pNode);

  void CallFuncCallback(cocos2d::CCObject* pSender);
  void MenuCallback(cocos2d::CCObject* pSender);

  /**
   * \~english
   * \~japanese
   *  \brief \ref PartsMember を返します
   *  \return \ref PartsMember
   */
  ADK_INLINE PartsMember *GetMember();

 private:
  static const S32 kMaxControl = 64;
  struct {
    S32 index[kMaxControl];
    S32 control_count;
  } control_;
  static const S32 kMaxMenu = 16;
  struct {
    S32 index[kMaxMenu];
    S32 menu_count;
  } menu_;
  PartsCallFunc parts_call_func_;
  PartsMember parts_member_;
  PartsMenu parts_menu_;
  CCNode* parent_;
};

ADK_INLINE PartsMember *PartsCommonNodeLoader::GetMember() {
  return &parts_member_;
}

ADK_END_NAMESPACE

#endif /* defined(__Chaos__parts_common_node_loader__) */
