/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_menu_item.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツのメニューアイテムのコールバッククラス実装部
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_menu.h>

#include <core/debug/debug_assert.h>
#include <core/library/lib_list.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_common_node_loader.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_system_cocos.h>

using namespace cocos2d;
using namespace cocos2d::extension;

ADK_BEGIN_NAMESPACE

PartsMenu::PartsMenu() : free_menu_item_list_(NULL) {
  memset(&menu_items_, 0, sizeof(menu_items_));
  TMenuItem* menu_item = menu_items_;
  for (S32 count = 0; count < kMaxMenuItem; ++count, ++menu_item) {
    menu_item->index = count;
    LIB_ForwardLinkedListPush(&free_menu_item_list_, menu_item);
  }
}
PartsMenu::~PartsMenu() {}

cocos2d::SEL_MenuHandler PartsMenu::onResolveCCBCCMenuItemSelector(
    cocos2d::CCObject* pTarget, const char* pSelectorName,
    cocos2d::CCNode* parent, S32* index) {

  ADK_ASSERT(pSelectorName != NULL);

  TMenuItem* menu_item =
      (TMenuItem*)LIB_ForwardLinkedListPop(&free_menu_item_list_);
  ADK_ASSERT(menu_item);
  menu_item->name_len = strlen(pSelectorName);
  ADK_ASSERT(menu_item->name_len < kMaxSelectorName);
  memcpy(menu_item->name, pSelectorName, menu_item->name_len);
  *index = menu_item->index;
  GuiPartsSystemCocos* system =
      (GuiPartsSystemCocos*)GuiPartsSystem::GetInstance();
  menu_item->parts = system->GetActiveGuiParts();
  switch (menu_item->index) {
    case 0:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback01);
    case 1:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback02);
    case 2:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback03);
    case 3:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback04);
    case 4:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback05);
    case 5:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback06);
    case 6:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback07);
    case 7:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback08);
    case 8:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback09);
    case 9:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback10);
    case 10:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback11);
    case 11:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback12);
    case 12:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback13);
    case 13:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback14);
    case 14:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback15);
    case 15:
      return (SEL_MenuHandler)(&PartsMenu::MenuCallback16);
    default:
      return NULL;
  }
}

Bool PartsMenu::ReleaseMenu(S32 index) {
  Bool ret = true;
  LIB_ForwardLinkedListPush(&free_menu_item_list_, &menu_items_[index]);
  return ret;
}

void PartsMenu::MenuCallback01(cocos2d::CCObject* pSender) {
  CallMenuCallback(0);
}
void PartsMenu::MenuCallback02(cocos2d::CCObject* pSender) {
  CallMenuCallback(1);
}
void PartsMenu::MenuCallback03(cocos2d::CCObject* pSender) {
  CallMenuCallback(2);
}
void PartsMenu::MenuCallback04(cocos2d::CCObject* pSender) {
  CallMenuCallback(3);
}
void PartsMenu::MenuCallback05(cocos2d::CCObject* pSender) {
  CallMenuCallback(4);
}
void PartsMenu::MenuCallback06(cocos2d::CCObject* pSender) {
  CallMenuCallback(5);
}
void PartsMenu::MenuCallback07(cocos2d::CCObject* pSender) {
  CallMenuCallback(6);
}
void PartsMenu::MenuCallback08(cocos2d::CCObject* pSender) {
  CallMenuCallback(7);
}
void PartsMenu::MenuCallback09(cocos2d::CCObject* pSender) {
  CallMenuCallback(8);
}
void PartsMenu::MenuCallback10(cocos2d::CCObject* pSender) {
  CallMenuCallback(9);
}
void PartsMenu::MenuCallback11(cocos2d::CCObject* pSender) {
  CallMenuCallback(10);
}
void PartsMenu::MenuCallback12(cocos2d::CCObject* pSender) {
  CallMenuCallback(11);
}
void PartsMenu::MenuCallback13(cocos2d::CCObject* pSender) {
  CallMenuCallback(12);
}
void PartsMenu::MenuCallback14(cocos2d::CCObject* pSender) {
  CallMenuCallback(13);
}
void PartsMenu::MenuCallback15(cocos2d::CCObject* pSender) {
  CallMenuCallback(14);
}
void PartsMenu::MenuCallback16(cocos2d::CCObject* pSender) {
  CallMenuCallback(15);
}

Bool PartsMenu::CallMenuCallback(S32 index) {
  GuiPartsSystemCocos* system =
      (GuiPartsSystemCocos*)GuiPartsSystem::GetInstance();
  PartsMenu* menu_item = system->GetPartsMenuInstance();
  return menu_item->DoCallMenuCallback(index);
}
Bool PartsMenu::DoCallMenuCallback(S32 index) {
  TMenuItem* menu_item = &menu_items_[index];
  if (menu_item->parts != NULL) {
    menu_item->parts->DoMenuCallback(menu_item->name);
  }
  return true;
}

ADK_END_NAMESPACE

