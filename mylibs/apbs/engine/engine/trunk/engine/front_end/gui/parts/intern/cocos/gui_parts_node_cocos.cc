/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_node_cocos.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief ノードパーツクラス定義部 for Cocos2d-x
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_node_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_system_cocos.h>

#include <CCNode.h>

ADK_BEGIN_NAMESPACE

GuiPartsNodeCocos::GuiPartsNodeCocos() : node_(NULL) {
#if 0
  SetPartsType(kGuiPartsTypeNode);
#endif
}
GuiPartsNodeCocos::~GuiPartsNodeCocos() {}

Bool GuiPartsNodeCocos::Init() {
  Bool ret = true;

  node_ = new cocos2d::CCNode;
  ADK_ASSERT(node_);
  if (node_ == NULL) {
    ret = false;
  } else {
    if (node_->init() == false) {
      delete node_, node_ = 0;
    } else {
      node_->autorelease();
    }
  }

  return ret;
}

IGuiParts *GuiPartsNodeCocos::GetCustomClass(const char *name) {
  ADK_ASSERT(false);
  return NULL;
}
IGuiParts *GuiPartsNodeCocos::AttachCreateParts(const Char *partsname,
                                                TGuiPartsType type) {
  IGuiParts* root = GetRootGuiParts();
  ADK_ASSERT(root);
  
  return ((GuiPartsCocos*)root)->AttachCreatePartsInternal(this, partsname, type);
}
Bool GuiPartsNodeCocos::SetButtonCallback(const Char *name,
                                          TGuiPartsButtonCallback callback,
                                          void *userdata) {
  IGuiParts* root = GetRootGuiParts();
  ADK_ASSERT(root);
  return root->SetButtonCallback(name, callback, userdata);
}
Bool GuiPartsNodeCocos::SetMenuCallback(const Char *name,
                                        TGuiPartsMenuCallback callback,
                                        void *userdata) {
  IGuiParts* root = GetRootGuiParts();
  ADK_ASSERT(root);
  return root->SetMenuCallback(name, callback, userdata); ADK_ASSERT(false);
}
void GuiPartsNodeCocos::SetTouchPriority(S32 priority) { ADK_ASSERT(false); }
S32 GuiPartsNodeCocos::GetTouchPriority() {
  ADK_ASSERT(false);
  return NULL;
}
void GuiPartsNodeCocos::SetVisible(Bool visible) { node_->setVisible(visible); }
Bool GuiPartsNodeCocos::IsVisible() { return node_->isVisible(); }
void GuiPartsNodeCocos::AddChild(IGuiParts *pChild) {
  cocos2d::CCNode *ccnode = (cocos2d::CCNode *)pChild->GetNode();
  node_->addChild(ccnode);
}
void GuiPartsNodeCocos::AddChild(IGuiParts *pChild, int zOrder) {
  cocos2d::CCNode *ccnode = (cocos2d::CCNode *)pChild->GetNode();
  node_->addChild(ccnode, zOrder);
}
void GuiPartsNodeCocos::AddChild(IGuiParts *pChild, int zOrder, int tag) {
  cocos2d::CCNode *ccnode = (cocos2d::CCNode *)pChild->GetNode();
  node_->addChild(ccnode, zOrder, tag);
}
Bool GuiPartsNodeCocos::CreateWithFile(const Char *filename,
                                       const TGuiPartsInfo *parts,
                                       S32 parts_count) {
  ADK_ASSERT(false);
}
Bool GuiPartsNodeCocos::Dispose() { ADK_ASSERT(false); }
void GuiPartsNodeCocos::SetNode(void *node) { node_ = (cocos2d::CCNode *)node; }
void *GuiPartsNodeCocos::GetNode() { return node_; }

ADK_END_NAMESPACE

