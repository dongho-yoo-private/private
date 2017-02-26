/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_scroll_view.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief スクロールビューパーツクラス実装部 for Cocos2d-x
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_scroll_view_cocos.h>

#include <front_end/gui/parts/intern/cocos/gui_cc_parts_scroll_view.h>

ADK_BEGIN_NAMESPACE

GuiPartsScrollViewCocos::GuiPartsScrollViewCocos() : scroll_view_(NULL) {
#if 0
  SetPartsType(kGuiPartsTypeScrollView);
#endif
  memset(&cordinate_, 0, sizeof(cordinate_));
}
GuiPartsScrollViewCocos::~GuiPartsScrollViewCocos() {}

Bool GuiPartsScrollViewCocos::Init() {
  Bool ret = true;

  scroll_view_ = new GuiCcPartsScrollView;
  ADK_ASSERT(scroll_view_);
  if (scroll_view_ == NULL) {
    ret = false;
  } else {
    if (scroll_view_->init() == false) {
      delete scroll_view_, scroll_view_ = 0;
      ret = false;
    } else {
      scroll_view_->autorelease();
    }
  }

  return ret;
}

void GuiPartsScrollViewCocos::SetContentOffset(TVector2 &offset,
                                               Bool animated) {
  cocos2d::CCPoint point(offset.x, offset.y);
  scroll_view_->setContentOffset(point, animated);
}
void GuiPartsScrollViewCocos::GetContentOffset(TVector2 &dst) {
  cocos2d::CCPoint point = scroll_view_->getContentOffset();
  dst.x = point.x;
  dst.y = point.y;
}
void GuiPartsScrollViewCocos::SetContentOffsetInDuration(TVector2 &offset,
                                                         F32 dt) {
  cocos2d::CCPoint point(offset.x, offset.y);
  scroll_view_->setContentOffsetInDuration(point, dt);
}

void GuiPartsScrollViewCocos::SetZoomScale(F32 s) {
  scroll_view_->setZoomScale(s);
}
void GuiPartsScrollViewCocos::SetZoomScale(F32 s, Bool animated) {
  scroll_view_->setZoomScale(s, animated);
}

F32 GuiPartsScrollViewCocos::GetZoomScale() {
  return scroll_view_->getZoomScale();
}

void GuiPartsScrollViewCocos::SetZoomScaleInDuration(F32 s, F32 dt) {
  return scroll_view_->setZoomScaleInDuration(s, dt);
}
void GuiPartsScrollViewCocos::MinContainerOffset(TVector2 &dst) {
  cocos2d::CCPoint point = scroll_view_->minContainerOffset();
  dst.x = point.x;
  dst.y = point.y;
}
void GuiPartsScrollViewCocos::MaxContainerOffset(TVector2 &dst) {
  cocos2d::CCPoint point = scroll_view_->maxContainerOffset();
  dst.x = point.x;
  dst.y = point.y;
}
Bool GuiPartsScrollViewCocos::IsNodeVisible(IGuiParts *parts) {
  cocos2d::CCNode *node = (cocos2d::CCNode *)parts->GetNode();
  return scroll_view_->isNodeVisible(node);
}
void GuiPartsScrollViewCocos::Pause(IGuiParts *parts) {
  cocos2d::CCNode *node = (cocos2d::CCNode *)parts->GetNode();
  return scroll_view_->pause(node);
}
void GuiPartsScrollViewCocos::Resume(IGuiParts *parts) {
  cocos2d::CCNode *node = (cocos2d::CCNode *)parts->GetNode();
  return scroll_view_->resume(node);
}

Bool GuiPartsScrollViewCocos::IsDragging() {
  return scroll_view_->isDragging();
}
Bool GuiPartsScrollViewCocos::IsTouchMoved() {
  return scroll_view_->isTouchMoved();
}
Bool GuiPartsScrollViewCocos::IsBounceable() {
  return scroll_view_->isBounceable();
}
void GuiPartsScrollViewCocos::SetBounceable(Bool bounceable) {
  scroll_view_->setBounceable(bounceable);
}

void GuiPartsScrollViewCocos::GetViewSize(TSize &dst) {
  cocos2d::CCSize size = scroll_view_->getViewSize();
  dst.width = size.width;
  dst.height = size.height;
}
void GuiPartsScrollViewCocos::SetViewSize(TSize &size) {
  cocos2d::CCSize ccsize(size.width, size.height);
  scroll_view_->setViewSize(ccsize);
}

#if 0
IGuiParts *GuiPartsScrollViewCocos::GetContainer() {
}
void GuiPartsScrollViewCocos::SetContainer(IGuiParts *container) {}
#endif

TScrollViewDirection GuiPartsScrollViewCocos::GetDirection() {
  return (TScrollViewDirection)scroll_view_->getDirection();
}
void GuiPartsScrollViewCocos::SetDirection(TScrollViewDirection direction) {
  scroll_view_->setDirection(
      (cocos2d::extension::CCScrollViewDirection)direction);
}

#if 0
CCScrollViewDelegate *GuiPartsScrollViewCocos::getDelegate() { return m_pDelegate {}}
void GuiPartsScrollViewCocos::setDelegate(CCScrollViewDelegate *pDelegate) { m_pDelegate = pDelegate {}}
#endif

void GuiPartsScrollViewCocos::SetContentSize(const TSize &size) {
  cocos2d::CCSize ccsize(size.width, size.height);
  scroll_view_->setContentSize(ccsize);
}
void GuiPartsScrollViewCocos::GetContentSize(TSize &dst) {
  const cocos2d::CCSize size = scroll_view_->getContentSize();
  dst.width = size.width;
  dst.height = size.height;
}

void GuiPartsScrollViewCocos::UpdateInset() { scroll_view_->updateInset(); }
Bool GuiPartsScrollViewCocos::IsClippingToBounds() {
  return scroll_view_->isClippingToBounds();
}
void GuiPartsScrollViewCocos::SetClippingToBounds(Bool bClippingToBounds) {
  scroll_view_->setClippingToBounds(bClippingToBounds);
}
void GuiPartsScrollViewCocos::Visit() { scroll_view_->visit(); }
void GuiPartsScrollViewCocos::SetTouchEnabled(Bool e) {
  scroll_view_->setTouchEnabled(e);
}

IGuiParts *GuiPartsScrollViewCocos::GetCustomClass(const char *name) {
  ADK_ASSERT(false);
  return NULL;
}
IGuiParts *GuiPartsScrollViewCocos::AttachCreateParts(const Char *partsname,
                                                      TGuiPartsType type) {
  ADK_ASSERT(false);
  return NULL;
}
Bool GuiPartsScrollViewCocos::SetButtonCallback(
    const Char *name, TGuiPartsButtonCallback callback, void *userdata) {
  ADK_ASSERT(false);
  return false;
}
Bool GuiPartsScrollViewCocos::SetMenuCallback(const Char *name,
                                              TGuiPartsMenuCallback callback,
                                              void *userdata) {
  ADK_ASSERT(false);
  return false;
}
void GuiPartsScrollViewCocos::SetTouchPriority(S32 priority) {
  ADK_ASSERT(false);
}
S32 GuiPartsScrollViewCocos::GetTouchPriority() {
  ADK_ASSERT(false);
  return NULL;
}
void GuiPartsScrollViewCocos::SetVisible(Bool visible) {
  scroll_view_->setVisible(visible);
}
Bool GuiPartsScrollViewCocos::IsVisible() { return scroll_view_->isVisible(); }
void GuiPartsScrollViewCocos::AddChild(IGuiParts *child, S32 zOrder, S32 tag) {
  cocos2d::CCNode *node = (cocos2d::CCNode *)child->GetNode();
  scroll_view_->addChild(node, zOrder, tag);
}
void GuiPartsScrollViewCocos::AddChild(IGuiParts *child, S32 zOrder) {
  cocos2d::CCNode *node = (cocos2d::CCNode *)child->GetNode();
  scroll_view_->addChild(node, zOrder);
}
void GuiPartsScrollViewCocos::AddChild(IGuiParts *child) {
  cocos2d::CCNode *node = (cocos2d::CCNode *)child->GetNode();
  scroll_view_->addChild(node);
}
Bool GuiPartsScrollViewCocos::CreateWithFile(const Char *filename,
                                             const TGuiPartsInfo *parts,
                                             S32 parts_count) {
  ADK_ASSERT(false);
}
Bool GuiPartsScrollViewCocos::Dispose() { ADK_ASSERT(false); }
void GuiPartsScrollViewCocos::SetNode(void *node) {
  scroll_view_ = (GuiCcPartsScrollView *)node;
}
void *GuiPartsScrollViewCocos::GetNode() { return scroll_view_; }

void GuiPartsScrollViewCocos::CordinateStartPosition(const TVector2 &start) {
  cordinate_.start = start;
}
void GuiPartsScrollViewCocos::AddChildFromBottom(IGuiParts *child) {
  ADK_ASSERT(child);

  child->SetPositionY(cordinate_.now.y);

  TSize child_size;
  child->GetContentSize(child_size);
  cordinate_.now.y += child_size.height;

  AddChild(child);

  const cocos2d::CCSize my_content_size = scroll_view_->getContentSize();
  cocos2d::CCSize new_content_size(my_content_size.width, cordinate_.now.y);
  scroll_view_->setContentSize(new_content_size);
}

ADK_END_NAMESPACE

