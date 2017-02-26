/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_control.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツのコントロールボタンのコールバッククラス実装部
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_common_node_loader.h>

#include <core/debug/debug_assert.h>
#include <core/library/lib_list.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_control.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_system_cocos.h>

using namespace cocos2d;
using namespace cocos2d::extension;

ADK_BEGIN_NAMESPACE

PartsControl::PartsControl() : count_(0), free_control_list_(NULL) {
  memset(&controls_, 0, sizeof(controls_));
  Control* control = controls_;
  for (int count = 0; count < kMaxControl; ++count, ++control) {
    control->index = count;
    LIB_ForwardLinkedListPush(&free_control_list_, control);
  }
}
PartsControl::~PartsControl() {}

cocos2d::extension::SEL_CCControlHandler
PartsControl::onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget,
                                            const char* pSelectorName,
                                            cocos2d::CCNode* parent,
                                            S32* index) {
  ADK_ASSERT(pSelectorName != NULL);
  ADK_ASSERT(count_ < kMaxControl);
  
  Control* control = (Control*)LIB_ForwardLinkedListPop(&free_control_list_);
  ADK_ASSERT(control);
  control->name_len = strlen(pSelectorName);
  ADK_ASSERT(control->name_len < kMaxSelectorName);
  memcpy(control->name, pSelectorName, control->name_len);
  *index = control->index;
  GuiPartsSystemCocos* system =
  (GuiPartsSystemCocos*)GuiPartsSystem::GetInstance();
  control->parts = system->GetActiveGuiParts();
  switch (control->index) {
    case 0:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback01);
    case 1:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback02);
    case 2:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback03);
    case 3:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback04);
    case 4:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback05);
    case 5:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback06);
    case 6:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback07);
    case 7:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback08);
    case 8:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback09);
    case 9:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback10);
    case 10:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback11);
    case 11:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback12);
    case 12:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback13);
    case 13:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback14);
    case 14:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback15);
    case 15:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback16);
    case 16:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback17);
    case 17:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback18);
    case 18:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback19);
    case 19:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback20);
    case 20:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback21);
    case 21:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback22);
    case 22:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback23);
    case 23:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback24);
    case 24:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback25);
    case 25:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback26);
    case 26:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback27);
    case 27:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback28);
    case 28:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback29);
    case 29:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback30);
    case 30:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback31);
    case 31:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback32);
    case 32:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback33);
    case 33:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback34);
    case 34:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback35);
    case 35:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback36);
    case 36:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback37);
    case 37:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback38);
    case 38:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback39);
    case 39:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback40);
    case 40:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback41);
    case 41:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback42);
    case 42:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback43);
    case 43:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback44);
    case 44:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback45);
    case 45:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback46);
    case 46:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback47);
    case 47:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback48);
    case 48:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback49);
    case 49:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback50);
    case 50:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback51);
    case 51:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback52);
    case 52:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback53);
    case 53:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback54);
    case 54:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback55);
    case 55:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback56);
    case 56:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback57);
    case 57:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback58);
    case 58:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback59);
    case 59:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback60);
    case 60:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback61);
    case 61:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback62);
    case 62:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback63);
    case 63:
      return (SEL_CCControlHandler)(&PartsControl::ControlCallback64);
    default:
      return NULL;
  }
}
Bool PartsControl::ReleaseControl(S32 index) {
  Bool ret = true;
  LIB_ForwardLinkedListPush(&free_control_list_, &controls_[index]);
  return ret;
}

void PartsControl::SetHandler(const char* pSelectorName,
                              ControlHandler* handler) {
  Control* control = FindControlName(pSelectorName);
  control->handler = *handler;
}

PartsControl::Control* PartsControl::FindControlName(const char* name) {
  
  Control* control = controls_;
  for (int i = 0; i < count_; ++i, ++control) {
    size_t name_len = strlen(name);
    if (name_len != control->name_len) {
      continue;
    }
    if (memcmp(control->name, name, name_len) == 0) {
      return control;
    }
  }
  ADK_ASSERT(false);
  
  return NULL;
}

PartsControl::Control* PartsControl::FindControlParent(CCNode* node) {
  
  Control* control = controls_;
  for (int i = 0; i < count_; ++i, ++control) {
    if (node == control->parent) {
      return control;
    }
  }
  ADK_ASSERT(false);
  
  return NULL;
}

void PartsControl::ControlCallback01(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(0);
  }
}
void PartsControl::ControlCallback02(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(1);
  }
}
void PartsControl::ControlCallback03(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(2);
  }
}
void PartsControl::ControlCallback04(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(3);
  }
}
void PartsControl::ControlCallback05(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(4);
  }
}
void PartsControl::ControlCallback06(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(5);
  }
}
void PartsControl::ControlCallback07(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(6);
  }
}
void PartsControl::ControlCallback08(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(7);
  }
}
void PartsControl::ControlCallback09(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(8);
  }
}
void PartsControl::ControlCallback10(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(9);
  }
}
void PartsControl::ControlCallback11(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(10);
  }
}
void PartsControl::ControlCallback12(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(11);
  }
}
void PartsControl::ControlCallback13(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(12);
  }
}
void PartsControl::ControlCallback14(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(13);
  }
}
void PartsControl::ControlCallback15(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(14);
  }
}
void PartsControl::ControlCallback16(CCObject* pSender,
                                     CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(15);
  }
}
void PartsControl::ControlCallback17(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(16);
  }
}
void PartsControl::ControlCallback18(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(17);
  }
}
void PartsControl::ControlCallback19(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(18);
  }
}
void PartsControl::ControlCallback20(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(19);
  }
}
void PartsControl::ControlCallback21(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(20);
  }
}
void PartsControl::ControlCallback22(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(21);
  }
}
void PartsControl::ControlCallback23(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(22);
  }
}
void PartsControl::ControlCallback24(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(23);
  }
}
void PartsControl::ControlCallback25(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(24);
  }
}
void PartsControl::ControlCallback26(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(25);
  }
}
void PartsControl::ControlCallback27(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(26);
  }
}
void PartsControl::ControlCallback28(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(27);
  }
}
void PartsControl::ControlCallback29(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(28);
  }
}
void PartsControl::ControlCallback30(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(29);
  }
}
void PartsControl::ControlCallback31(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(30);
  }
}
void PartsControl::ControlCallback32(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(31);
  }
}
void PartsControl::ControlCallback33(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(32);
  }
}
void PartsControl::ControlCallback34(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(33);
  }
}
void PartsControl::ControlCallback35(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(34);
  }
}
void PartsControl::ControlCallback36(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(35);
  }
}
void PartsControl::ControlCallback37(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(36);
  }
}
void PartsControl::ControlCallback38(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(37);
  }
}
void PartsControl::ControlCallback39(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(38);
  }
}
void PartsControl::ControlCallback40(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(39);
  }
}
void PartsControl::ControlCallback41(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(40);
  }
}
void PartsControl::ControlCallback42(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(41);
  }
}
void PartsControl::ControlCallback43(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(42);
  }
}
void PartsControl::ControlCallback44(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(43);
  }
}
void PartsControl::ControlCallback45(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(44);
  }
}
void PartsControl::ControlCallback46(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(45);
  }
}
void PartsControl::ControlCallback47(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(46);
  }
}
void PartsControl::ControlCallback48(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(47);
  }
}
void PartsControl::ControlCallback49(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(48);
  }
}
void PartsControl::ControlCallback50(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(49);
  }
}
void PartsControl::ControlCallback51(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(50);
  }
}
void PartsControl::ControlCallback52(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(51);
  }
}
void PartsControl::ControlCallback53(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(52);
  }
}
void PartsControl::ControlCallback54(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(53);
  }
}
void PartsControl::ControlCallback55(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(54);
  }
}
void PartsControl::ControlCallback56(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(55);
  }
}
void PartsControl::ControlCallback57(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(56);
  }
}
void PartsControl::ControlCallback58(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(57);
  }
}
void PartsControl::ControlCallback59(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(58);
  }
}
void PartsControl::ControlCallback60(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(59);
  }
}
void PartsControl::ControlCallback61(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(60);
  }
}
void PartsControl::ControlCallback62(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(61);
  }
}
void PartsControl::ControlCallback63(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(62);
  }
}
void PartsControl::ControlCallback64(
                                     cocos2d::CCObject* pSender,
                                     cocos2d::extension::CCControlEvent controlEvent) {
  if (controlEvent == CCControlEventTouchUpInside) {
    CallControlCallback(63);
  }
}
Bool PartsControl::CallControlCallback(S32 index) {
  GuiPartsSystemCocos* system =
  (GuiPartsSystemCocos*)GuiPartsSystem::GetInstance();
  PartsControl* control = system->GetPartsControlInstance();
  return control->DoCallControlCallback(index);
}
Bool PartsControl::DoCallControlCallback(S32 index) {
  Control* control = &controls_[index];
  if (control->parts != NULL) {
    control->parts->DoButtonCallback(control->name);
  }
  return true;
}

ADK_END_NAMESPACE
