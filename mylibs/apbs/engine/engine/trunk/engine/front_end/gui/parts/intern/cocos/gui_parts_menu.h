/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_menu_item.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツのメニューアイテムのコールバッククラス定義部
 */

#ifndef GUI_PARTS_MENU_ITEM_H_
#define GUI_PARTS_MENU_ITEM_H_

#include <cocos2d.h>
#include <extensions/CCBReader/CCBSelectorResolver.h>

#include <core/types.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class GuiPartsCocos;

typedef void (*MenuHandler)(cocos2d::CCObject*);

/**
 * \~english
 * \~japanese
 *  \brief
 * パーツファイルから指定されるメニューアイテムのコールバックを設定するクラス\n
 *         \ref Parts から関連づけられている
 */
class PartsMenu : public cocos2d::CCObject {

 public:
  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  PartsMenu();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~PartsMenu();
  /**
   * \~english
   * \~japanese
   *  \brief ファイルを読み込み、セレクタ名と親ノードのポインタを保持する
   *         このメソッドはPartsクラスから呼ばれる
   */
  virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(
      cocos2d::CCObject* pTarget, const char* pSelectorName,
      cocos2d::CCNode* parent, S32* index);
  /**
   * \~english
   * \~japanese
   *  \brief 使用したメニューを解放します
   *  \param[in] index メニューのインデックス
   */
  Bool ReleaseMenu(S32 index);

 private:
  void MenuCallback01(cocos2d::CCObject* pSender);
  void MenuCallback02(cocos2d::CCObject* pSender);
  void MenuCallback03(cocos2d::CCObject* pSender);
  void MenuCallback04(cocos2d::CCObject* pSender);
  void MenuCallback05(cocos2d::CCObject* pSender);
  void MenuCallback06(cocos2d::CCObject* pSender);
  void MenuCallback07(cocos2d::CCObject* pSender);
  void MenuCallback08(cocos2d::CCObject* pSender);
  void MenuCallback09(cocos2d::CCObject* pSender);
  void MenuCallback10(cocos2d::CCObject* pSender);
  void MenuCallback11(cocos2d::CCObject* pSender);
  void MenuCallback12(cocos2d::CCObject* pSender);
  void MenuCallback13(cocos2d::CCObject* pSender);
  void MenuCallback14(cocos2d::CCObject* pSender);
  void MenuCallback15(cocos2d::CCObject* pSender);
  void MenuCallback16(cocos2d::CCObject* pSender);

  Bool CallMenuCallback(S32 index);
  Bool DoCallMenuCallback(S32 index);

  static const int kMaxSelectorName = 31;

  typedef struct {
    void *next;
    char name[kMaxSelectorName + 1];
    size_t name_len;
    GuiPartsCocos *parts;
    S32 index;
  }TMenuItem;

  static const int kMaxMenuItem = 16;
  TMenuItem menu_items_[kMaxMenuItem];
  void* free_menu_item_list_;
};

ADK_END_NAMESPACE

#endif /* GUI_PARTS_MENU_ITEM_H_ */
