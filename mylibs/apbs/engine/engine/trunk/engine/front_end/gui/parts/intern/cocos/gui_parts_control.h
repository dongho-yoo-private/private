/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_control.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツのコントロールボタンのコールバッククラス定義部
 */

#ifndef SCENE_SYSTEM_PARTS_CONTROL_H_
#define SCENE_SYSTEM_PARTS_CONTROL_H_

#include <cocos2d.h>
#include <extensions/CCBReader/CCBSelectorResolver.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class GuiPartsCocos;

typedef void (*ControlHandler)(cocos2d::CCObject*,
                               cocos2d::extension::CCControlEvent);

/**
 * \~english
 * \~japanese
 *  \brief
 * パーツファイルから指定されるコントロールのコールバックを設定するクラス\n
 *         \ref Parts から関連づけられている
 */
class PartsControl : public cocos2d::CCObject {

 public:
  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  PartsControl();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~PartsControl();
  /**
   * \~english
   * \~japanese
   *  \brief ファイルを読み込み、セレクタ名と親ノードのポインタを保持する
   *         このメソッドはPartsクラスから呼ばれる
   */
  virtual cocos2d::extension::SEL_CCControlHandler
      onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget,
                                    const char* pSelectorName,
                                    cocos2d::CCNode* parent, S32* index);
  /**
   * \~english
   * \~japanese
   *  \brief 使用したコントローラを解放します
   *  \param[in] index コントロールのインデックス
   */
  Bool ReleaseControl(S32 index);
  /**
   * \~english
   * \~japanese
   *  \brief 呼び出す関数を設定する
   *  \param pSelectorName[in] 設定されるセレクタ名
   *  \param handler[in] 設定する関数のポインタ
   */
  void SetHandler(const char* pSelectorName, ControlHandler* handler);

 private:
  void ControlCallback01(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback02(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback03(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback04(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback05(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback06(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback07(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback08(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback09(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback10(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback11(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback12(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback13(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback14(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback15(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback16(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback17(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback18(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback19(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback20(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback21(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback22(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback23(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback24(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback25(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback26(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback27(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback28(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback29(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback30(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback31(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback32(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback33(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback34(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback35(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback36(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback37(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback38(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback39(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback40(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback41(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback42(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback43(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback44(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback45(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback46(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback47(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback48(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback49(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback50(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback51(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback52(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback53(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback54(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback55(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback56(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback57(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback58(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback59(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback60(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback61(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback62(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback63(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);
  void ControlCallback64(cocos2d::CCObject* pSender,
                         cocos2d::extension::CCControlEvent controlEvent);

  Bool CallControlCallback(S32 index);
  Bool DoCallControlCallback(S32 index);

  static const int kMaxSelectorName = 31;

  typedef struct {
    void* next;
    int index;
    cocos2d::CCNode* parent;
    char name[kMaxSelectorName + 1];
    size_t name_len;
    ControlHandler handler;
    GuiPartsCocos *parts;
  } Control;

  Control* FindControlName(const char* name);
  Control* FindControlParent(cocos2d::CCNode* node);

  static const int kMaxControl = 64;

  Control controls_[kMaxControl];
  int count_;
  void* free_control_list_;
};

ADK_END_NAMESPACE

#endif /* defined(PARTS_OBJECT_CONTROL_ON_PARTS_LISTENER_H_) */
