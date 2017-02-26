/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_common_node_loader.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief
 * ファイルからパーツの内容を読み込み、内部ノードに設定するクラスの実装部
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_common_node_loader.h>

#include <core/debug/logging.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_system_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>

using namespace cocos2d;
using namespace cocos2d::extension;

ADK_BEGIN_NAMESPACE

#define WITH_LOG

PartsCommonNodeLoader::~PartsCommonNodeLoader() {
  GuiPartsSystemCocos* system =
      (GuiPartsSystemCocos*)GuiPartsSystem::GetInstance();
  PartsControl* control = system->GetPartsControlInstance();
  for (S32 index = 0; index < control_.control_count; ++index) {
    control->ReleaseControl(control_.index[index]);
  }
  PartsMenu* menu = system->GetPartsMenuInstance();
  for (S32 index = 0; index < menu_.menu_count; ++index) {
    menu->ReleaseMenu(menu_.index[index]);
  }
}

bool PartsCommonNodeLoader::init() {
  bool ret = CCLayer::init();
  if (ret != false) {
    GuiPartsSystemCocos* system =
        (GuiPartsSystemCocos*)GuiPartsSystem::GetInstance();
    GuiPartsCocos *active_parts = system->GetActiveGuiParts();
    ADK_ASSERT(active_parts);
    active_parts->AddMember(&parts_member_);
  }
  return ret;
}

SEL_CallFuncN PartsCommonNodeLoader::onResolveCCBCCCallFuncSelector(
    CCObject* pTarget, const char* pSelectorName) {
#ifdef WITH_LOG
  printf("onResolveCCBCCCallFuncSelector(): %s\n", pSelectorName);
#endif /* WITH_LOG */
  return parts_call_func_.onResolveCCBCCCallFuncSelector(pTarget, pSelectorName,
                                                         parent_);
}

SEL_MenuHandler PartsCommonNodeLoader::onResolveCCBCCMenuItemSelector(
    CCObject* pTarget, const char* pSelectorName) {
#ifdef WITH_LOG
  printf("onResolveCCBCCMenuItemSelector(): %s\n", pSelectorName);
#endif /* WITH_LOG */
  GuiPartsSystemCocos* system =
      (GuiPartsSystemCocos*)GuiPartsSystem::GetInstance();
  PartsMenu* menu = system->GetPartsMenuInstance();
  SEL_MenuHandler ret = menu->onResolveCCBCCMenuItemSelector(
      pTarget, pSelectorName, parent_, &menu_.index[menu_.menu_count]);
  ++menu_.menu_count;
  return ret;
}

SEL_CCControlHandler PartsCommonNodeLoader::onResolveCCBCCControlSelector(
    CCObject* pTarget, const char* pSelectorName) {
#ifdef WITH_LOG
  printf("onResolveCCBCCControlSelector(): %s\n", pSelectorName);
#endif /* WITH_LOG */
  ADK_ASSERT(control_.control_count<kMaxControl);
  GuiPartsSystemCocos* system =
      (GuiPartsSystemCocos*)GuiPartsSystem::GetInstance();
  PartsControl* control = system->GetPartsControlInstance();
  SEL_CCControlHandler ret = control->onResolveCCBCCControlSelector(
      pTarget, pSelectorName, parent_, &control_.index[control_.control_count]);
  ++control_.control_count;
  return ret;
}

bool PartsCommonNodeLoader::onAssignCCBMemberVariable(
    CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
#ifdef WITH_LOG
  printf("onAssignCCBMemberVariable(): %s\n", pMemberVariableName);
#endif /* WITH_LOG */
  parent_ = pNode;
  return parts_member_.onAssignCCBMemberVariable(pTarget, pMemberVariableName,
                                                 pNode);
}

void PartsCommonNodeLoader::CallFuncCallback(CCObject* pSender) {
  parts_call_func_.CallFuncCallback(pSender);
}

void PartsCommonNodeLoader::MenuCallback(CCObject* pSender) {}

ADK_END_NAMESPACE
