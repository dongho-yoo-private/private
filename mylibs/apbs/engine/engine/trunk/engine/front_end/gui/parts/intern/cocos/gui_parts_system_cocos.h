/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_system_cocos.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツ全体を管理するシステムクラス定義部 for Cocos2d-x
 */

#ifndef GUI_PARTS_SYSTEM_COCOS_H_
#define GUI_PARTS_SYSTEM_COCOS_H_

#include <cocos2d.h>

#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_control.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_menu.h>
#include <front_end/gui/parts/intern/gui_parts_system.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class GuiPartsCocos;

/**
 * \~english
 * \~japanese
 *  \brief パーツシステムクラス
 */
class GuiPartsSystemCocos : public GuiPartsSystem {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  GuiPartsSystemCocos();
  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  virtual ~GuiPartsSystemCocos();

  /**
   * \~english
   * \~japanese
   *  \brief 指定した \ref TGuiPartsType の \ref IGuiParts を作成する
   *  \param[in] type \ref TGuiPartsType
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval \ref IGuiParts 作成に成功したときに返ります
   *  \retval NULL 作成に失敗したときに返ります
   */
  virtual IGuiParts *CreateGuiPartsWithType(TGuiPartsType type);
  /**
   * \~english
   * \~japanese
   *  \brief 指定した \ref TGuiPartsType の \ref IGuiParts を作成するが、\n
   *  内部ではcocosをnodeの参照として生成する。
   *  \param[in] type \ref TGuiPartsType
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval \ref IGuiParts 作成に成功したときに返ります
   *  \retval NULL 作成に失敗したときに返ります 
  */
  virtual IGuiParts *CreateGuiPartsWithType(TGuiPartsType type, void* node);

  /**
   * \~english
   * \~japanese
   *  \brief スプライトラッパー系
   */
  virtual IGuiPartsSprite* CreateGuiPartsSprite();
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithFile(const char* filename);
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithFile(const char* filename,
                                                        const TRect& rect);
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithTexture(
      IGuiPartsTexture* pTexture);
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithTexture(
      IGuiPartsTexture* pTexture, const TRect& rect);
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithSpriteFrame(
      IGuiPartsSpriteFrame* spriteframe);
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithSpriteFrameName(
      const Char *framename);

  /**
   * \~english
   * \~japanese
   *  \brief cocos2d::CCNode を子として追加します
   *  \param[in] parts \ref IGuiParts
   *  \return 追加の成功判定。以下の値が返ります
   *  \retval ResultSuccess 追加に成功したときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  Result AddChild(cocos2d::CCNode *node);
  /**
   * \~english
   * \~japanese
   *  \brief cocos2d::CCNode を子として追加します
   *  \param[in] parts \ref IGuiParts
   *  \return 追加の成功判定。以下の値が返ります
   *  \retval ResultSuccess 追加に成功したときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  Result RemoveChild(cocos2d::CCNode *node);
  /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiParts をシステムに追加します
   *  \param[in] parts \ref IGuiParts
   *  \return 追加の成功判定。以下の値が返ります
   *  \retval ResultSuccess 追加に成功したときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  virtual Result AddGuiParts(IGuiParts *parts);
  /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiParts をシステムから切り離します
   *  \param[in] parts \ref IGuiParts
   *  \return 切り離しの成功判定。以下の値が返ります
   *  \retval ResultSuccess 切り離しに成功したときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  virtual Result RemoveGuiParts(IGuiParts *parts);

  /**
   * \~english
   * \~japanese
   *  \brief パーツコントロールインスタンスを返します
   */
  ADK_INLINE PartsControl* GetPartsControlInstance();
  /**
   * \~english
   * \~japanese
   *  \brief パーツメニューインスタンスを返します
   */
  ADK_INLINE PartsMenu* GetPartsMenuInstance();

  /**
   * \~english
   * \~japanese
   *  \brief パーツをアクティブにします
   *  \param[in] parts アクティブにする \ref GuiPartsCocos
   */
  ADK_INLINE void ActivateGuiParts(GuiPartsCocos *parts);
  /**
   * \~english
   * \~japanese
   *  \brief アクティブなパーツを返します
   *  \return \ref GuiPartsCocos
   */
  ADK_INLINE GuiPartsCocos* GetActiveGuiParts();

 protected:
  /**
   * \~english
   * \~japanese
   *  \brief パーツシステムを初期化し使用できる状態にします
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval ResultSuccess 初期化に成功したときに返ります
   *  \retval ResultDoAlready 既に初期化されているときに返ります
   *  \retval ResultOutOfResource システムリソースが不足しているときに返ります
   */
  virtual Result Init();
  /**
   * \~english
   * \~japanese
   *  \brief パースシステムを終了します
   *  \return 終了の成功判定。以下の値が返ります
   *  \retval ResultSuccess 終了に成功したときに返ります
   *  \retval ResultNotInitialized 初期化が行われていないときに返ります
   */
  virtual Result Term();

  /**
   * \~english
   * \~japanese
   *  \brief ファイルから \ref IGuiParts を作成します
   *  \param[in] filename パーツファイル名
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval \ref IGuiParts 作成に成功したときに返ります
   *  \retval NULL 作成に失敗したときに返ります
   */
  virtual IGuiParts *CreateGuiParts();
 private:
  /**
   * \~english
   * \~japanese
   *  \brief アプリケーションの基盤となるCocos2d-xのシーンオブジェクト\n
   *         各シーンで描画するものは全てこのシーンの子になる。
   */
  cocos2d::CCScene *context_scene_;
  /**
   * \~english
   * \~japanese
   *  \brief パーツコントロール
   */
  PartsControl parts_control_;
  /**
   * \~english
   * \~japanese
   *  \brief パーツメニュー
   */
  PartsMenu parts_menu_;
  /**
   * \~english
   * \~japanese
   *  \brief アクティブな \ref GuiPartsCocos
   */
  GuiPartsCocos *active_parts_;
};
ADK_INLINE PartsControl* GuiPartsSystemCocos::GetPartsControlInstance() {
  return &parts_control_;
}
ADK_INLINE PartsMenu* GuiPartsSystemCocos::GetPartsMenuInstance() {
  return &parts_menu_;
}
ADK_INLINE void GuiPartsSystemCocos::ActivateGuiParts(GuiPartsCocos *parts) {
  active_parts_ = parts;
}
ADK_INLINE GuiPartsCocos* GuiPartsSystemCocos::GetActiveGuiParts() {
  return active_parts_;
}

ADK_END_NAMESPACE

#endif /* GUI_PARTS_SYSTEM_COCOS_H_ */

