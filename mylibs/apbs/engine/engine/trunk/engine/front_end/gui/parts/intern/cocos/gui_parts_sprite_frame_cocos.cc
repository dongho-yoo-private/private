/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_sprite_frame_cocos.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief スプライトフレームパーツクラス実装部 for Cocos2d-x
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_sprite_frame_cocos.h>

ADK_BEGIN_NAMESPACE

GuiPartsSpriteFrameCocos::GuiPartsSpriteFrameCocos() : sprite_frame_(NULL) {}
GuiPartsSpriteFrameCocos::~GuiPartsSpriteFrameCocos() {}
Bool GuiPartsSpriteFrameCocos::Init() {
  Bool ret = true;

  sprite_frame_ = new cocos2d::CCSpriteFrame;
  ADK_ASSERT(sprite_frame_);
  if (sprite_frame_ == NULL) {
    ret = false;
  } else {
    sprite_frame_->autorelease();
  }

  return ret;
}

IGuiParts *GuiPartsSpriteFrameCocos::GetCustomClass(const char *name) {
  ADK_ASSERT(false);
  return NULL;
}
IGuiParts *GuiPartsSpriteFrameCocos::AttachCreateParts(const Char *partsname,
                                                       TGuiPartsType type) {
  ADK_ASSERT(false);
  return NULL;
}
Bool GuiPartsSpriteFrameCocos::SetButtonCallback(
    const Char *name, TGuiPartsButtonCallback callback, void *userdata) {
  ADK_ASSERT(false);
  return false;
}
Bool GuiPartsSpriteFrameCocos::SetMenuCallback(const Char *name,
                                               TGuiPartsMenuCallback callback,
                                               void *userdata) {
  ADK_ASSERT(false);
  return false;
}
void GuiPartsSpriteFrameCocos::SetTouchPriority(S32 priority) {
  ADK_ASSERT(false);
}
S32 GuiPartsSpriteFrameCocos::GetTouchPriority() {
  ADK_ASSERT(false);
  return NULL;
}
void GuiPartsSpriteFrameCocos::SetVisible(Bool visible) {
#if 1
  ADK_ASSERT(false);
#else
  sprite_frame_->setVisible(visible);
#endif
}
Bool GuiPartsSpriteFrameCocos::IsVisible() {
#if 1
  ADK_ASSERT(false);
  return false;
#else
  return sprite_frame_->isVisible();
#endif
}
void GuiPartsSpriteFrameCocos::AddChild(IGuiParts *pChild) {
#if 1
  ADK_ASSERT(false);
#else
  cocos2d::CCSpriteFrame *ccnode = (cocos2d::CCSpriteFrame *)pChild->GetNode();
  sprite_frame_->addChild(ccnode);
#endif
}
void GuiPartsSpriteFrameCocos::AddChild(IGuiParts *pChild, int zOrder) {
#if 1
  ADK_ASSERT(false);
#else
  cocos2d::CCSpriteFrame *ccnode = (cocos2d::CCSpriteFrame *)pChild->GetNode();
  sprite_frame_->addChild(ccnode, zOrder);
#endif
}
void GuiPartsSpriteFrameCocos::AddChild(IGuiParts *pChild, int zOrder,
                                        int tag) {
#if 1
  ADK_ASSERT(false);
#else
  cocos2d::CCSpriteFrame *ccnode = (cocos2d::CCSpriteFrame *)pChild->GetNode();
  sprite_frame_->addChild(ccnode, zOrder, tag);
#endif
}
Bool GuiPartsSpriteFrameCocos::CreateWithFile(const Char *filename,
                                              const TGuiPartsInfo *parts,
                                              S32 parts_count) {
  ADK_ASSERT(false);
}
Bool GuiPartsSpriteFrameCocos::Dispose() { ADK_ASSERT(false); }
void GuiPartsSpriteFrameCocos::SetNode(void *node) {
  sprite_frame_ = (cocos2d::CCSpriteFrame *)node;
}
void *GuiPartsSpriteFrameCocos::GetNode() { return sprite_frame_; }

ADK_END_NAMESPACE

