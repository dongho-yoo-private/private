/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file parts_cocos.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief シーン中に描画される「パーツ」を表すクラス実装部 for Cocos2d-x
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>

#include <stdarg.h>

#include <cocos2d.h>
#include <extensions/CCBReader/CCBReader.h>
#include <extensions/CCBReader/CCNodeLoader.h>
#include <extensions/CCBReader/CCNodeLoaderLibrary.h>
#include <extensions/cocos-ext.h>

#include <core/debug/debug_assert.h>
#include <core/library/lib_string.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_button_loader.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_layer_loader.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_node_loader.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_scroll_view_loader.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_system_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_member.h>

ADK_BEGIN_NAMESPACE

GuiPartsCocos::GuiPartsCocos() : node_(NULL),attach_node_(false) {
  memset(&loader_, 0, sizeof(loader_));
  memset(&button_, 0, sizeof(button_));
  memset(&menu_, 0, sizeof(menu_));
}
GuiPartsCocos::~GuiPartsCocos() {}

Bool GuiPartsCocos::Init() {
  return true;
}

Bool GuiPartsCocos::CreateWithFile(const Char* filename,
                                   const TGuiPartsInfo* parts,
                                   S32 parts_count) {
  Bool ret = true;

  ADK_ASSERT(filename);

  GuiPartsSystemCocos* system =
      (GuiPartsSystemCocos*)GuiPartsSystem::GetInstance();
  system->ActivateGuiParts(this);

  cocos2d::extension::CCNodeLoaderLibrary* node_loader =
      CreateLoaders(parts, parts_count);
  if (node_loader == NULL) {
    ret = false;
  } else {
    cocos2d::extension::CCBReader* ccb_reader =
        new cocos2d::extension::CCBReader(node_loader);
    if (ccb_reader == NULL) {
      ret = false;
    } else {
      node_ = ccb_reader->readNodeGraphFromFile(filename);
    }
  }

  return ret;
}
Bool GuiPartsCocos::Dispose() {
#if 0
  if (node_ != NULL && attach_node_ == false) {
    node_->release(), node_ = NULL;
  }
#endif
  return true;
}
IGuiParts *GuiPartsCocos::GetCustomClass(const char *name) {
  for (S32 index=0;index<loader_.loader_count;++index) {
    if (LIB_strcmp(loader_.loader_names[index],name)==0) {
      return loader_.loader_parts[index];
    }
  }

  return NULL;
}


IGuiParts* GuiPartsCocos::AttachCreatePartsInternal(IGuiParts* parent, const Char *partsname, TGuiPartsType type)
{
  IGuiParts* ret = NULL;
  for (S32 index = 0; index < loader_.member_count; ++index) {
    cocos2d::CCNode* node = loader_.members[index]->GetNode((cocos2d::CCNode*)parent->GetNode(), partsname);
    if (node != NULL) {
      GuiPartsSystemCocos* system =
          (GuiPartsSystemCocos*)GuiPartsSystem::GetInstance();
      
      // 中でcocosを生成しているので、リークになってしまう。
      ret = system->CreateGuiPartsWithType(type, node);
      ret->SetRootGuiParts(this);
      ADK_ASSERT(ret);
      break;
    }
  }

  return ret;
}

IGuiParts* GuiPartsCocos::AttachCreateParts(const Char* partsname,
                                            TGuiPartsType type) {
  return AttachCreatePartsInternal(this, partsname, type);
}
Bool GuiPartsCocos::SetButtonCallback(const Char* name,
                                      TGuiPartsButtonCallback callback,
                                      void* userdata) {
  Bool ret = true;
  ADK_ASSERT(button_.callback_count < kMaxButtonCallback);
  if (button_.callback_count >= kMaxButtonCallback) {
    ret = false;
  } else {
    TButtonCallbackInfo* button_callback =
        &button_.callbacks[button_.callback_count];
    LIB_strncpy(button_callback->name, name, kGuiPartsNameLen);
    button_callback->callback = callback;
    button_callback->userdata = userdata;
    ++button_.callback_count;
  }
  return ret;
}
Bool GuiPartsCocos::SetMenuCallback(const Char* name,
                                    TGuiPartsMenuCallback callback,
                                    void* userdata) {
  Bool ret = true;
  ADK_ASSERT(menu_.callback_count < kMaxMenuCallback);
  if (menu_.callback_count >= kMaxMenuCallback) {
    ret = false;
  } else {
    TMenuCallbackInfo* menu_callback = &menu_.callbacks[menu_.callback_count];
    LIB_strncpy(menu_callback->name, name, kGuiPartsNameLen);
    menu_callback->callback = callback;
    menu_callback->userdata = userdata;
    ++menu_.callback_count;
  }
  return ret;
}

void GuiPartsCocos::SetTouchPriority(S32 priority) {
  ((cocos2d::CCLayer*)GetNode())->setTouchPriority(priority);
}
S32 GuiPartsCocos::GetTouchPriority() {
  return ((cocos2d::CCLayer*)GetNode())->getTouchPriority();
}
void GuiPartsCocos::SetVisible(Bool visible) {
  ((cocos2d::CCLayer*)GetNode())->setVisible(visible);
}
Bool GuiPartsCocos::IsVisible() {
  return ((cocos2d::CCLayer*)GetNode())->isVisible();
}
void GuiPartsCocos::AddChild(IGuiParts* child) {
  cocos2d::CCNode *node = (cocos2d::CCNode*)GetNode();
  node->addChild((cocos2d::CCNode*)child->GetNode());
  child->SetParent(this);
}
void GuiPartsCocos::AddChild(IGuiParts* child, S32 zOrder) {
  cocos2d::CCNode *node = (cocos2d::CCNode*)GetNode();
  node->addChild((cocos2d::CCNode*)child->GetNode(),zOrder);
  child->SetParent(this);
}
void GuiPartsCocos::AddChild(IGuiParts* child, S32 zOrder, S32 tag) {
  cocos2d::CCNode *node = (cocos2d::CCNode*)GetNode();
  node->addChild((cocos2d::CCNode*)child->GetNode(),zOrder,tag);
  child->SetParent(this);
}

cocos2d::extension::CCNodeLoaderLibrary* GuiPartsCocos::CreateLoaders(
    const TGuiPartsInfo* parts, S32 parts_count) {
  ADK_ASSERT(parts);
  cocos2d::extension::CCNodeLoaderLibrary* ret =
      cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
  if (ret != NULL) {
    const TGuiPartsInfo* info = parts;
    TPartsClass* pc = loader_.classes;
    for (S32 index = 0; index < parts_count; ++index, ++info, ++pc) {
      strncpy(pc->name, info->name, kGuiPartsNameLen);
      pc->loader = CreateLoader(info->type);
      pc->type = info->type;
      ADK_ASSERT(pc->loader);
      ret->registerCCNodeLoader(pc->name, pc->loader);
      ++loader_.count;
    }
  }

  return ret;
}
cocos2d::extension::CCNodeLoader* GuiPartsCocos::CreateLoader(
    TGuiPartsType parts_type) {
  cocos2d::extension::CCNodeLoader* ret = NULL;
  switch (parts_type) {
    case kGuiPartsTypeLayer: {
      ret = new PartsLayerLoader;
      break;
    }
    case kGuiPartsTypeNode: {
      ret = new PartsNodeLoader;
      break;
    }
    case kGuiPartsTypeButton: {
      ret = new PartsControlButtonLoader;
      break;
    }
    case kGuiPartsTypeScrollView: {
      ret = new PartsScrollViewLoader;
      break;
    }
  }
  ADK_ASSERT(ret);
  return ret;
}
void GuiPartsCocos::AddMember(PartsMember *member) {
  ADK_ASSERT(loader_.member_count<kMaxPartsMember);
  loader_.members[loader_.member_count] = member;
  ++loader_.member_count;
}
void GuiPartsCocos::AddLoaderClass(cocos2d::CCNode *node,const char *name) {
  ADK_ASSERT(loader_.loader_count<kMaxLoader);
  const S32 pos = loader_.loader_count;
  GuiPartsSystemCocos* system =
      (GuiPartsSystemCocos*)GuiPartsSystem::GetInstance();
  loader_.loader_parts[pos] = system->CreateGuiPartsWithType(kGuiPartsTypeNode); /* とりあえず一律Nodeとして作成しとく */
  ADK_ASSERT(loader_.loader_parts[pos]);
  loader_.loader_parts[pos]->SetNode(node);
  LIB_strncpy(loader_.loader_names[pos],name,kLoaderNameLen);
  ++loader_.loader_count;
}
Bool GuiPartsCocos::DoButtonCallback(const Char* name) {
    TButtonCallbackInfo* callback = button_.callbacks;
  for (S32 index = 0; index < button_.callback_count; ++index, ++callback) {
    if (LIB_strcmp(callback->name, name) != 0) continue;
    if (callback->callback != NULL) {
      (callback->callback)(callback->userdata);
    }
    return true;
  }
  return false;
}
Bool GuiPartsCocos::DoMenuCallback(const Char* name) {
  TMenuCallbackInfo* callback = menu_.callbacks;
  for (S32 index = 0; index < menu_.callback_count; ++index, ++callback) {
    if (LIB_strcmp(callback->name, name) != 0) continue;
    if (callback->callback != NULL) {
      (callback->callback)(callback->userdata);
    }
    return true;
  }
  return false;
}
void GuiPartsCocos::SetNode(void* node) {
    node_ = (cocos2d::CCNode*)node;
    attach_node_ = true;
}
void* GuiPartsCocos::GetNode() { return node_; }

ADK_END_NAMESPACE

