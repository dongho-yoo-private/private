/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file i_parts.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief 描画される一パーツのインタフェース実装部
 */

#include <cocos2d.h>
#include <front_end/gui/parts/i_gui_parts.h>

ADK_BEGIN_NAMESPACE

IGuiParts::IGuiParts():root_parts_(0) {}
IGuiParts::~IGuiParts() {}

IGuiParts* IGuiParts::GetRootGuiParts()
{
  return root_parts_;
}
void IGuiParts::SetRootGuiParts(IGuiParts* root_parts)
{
  root_parts_=root_parts;
}


/*
 * ここでは実装すべきじゃないが、間に合わせのため
 */
const Char *IGuiParts::Description(void) {
  return ((cocos2d::CCNode*)GetNode())->description();
}
void IGuiParts::SetZOrder(S32 zOrder) {
  ((cocos2d::CCNode*)GetNode())->setZOrder(zOrder);
}
void IGuiParts::_SetZOrder(S32 z) {
  ((cocos2d::CCNode*)GetNode())->_setZOrder(z);
}
S32 IGuiParts::GetZOrder() {
  return ((cocos2d::CCNode*)GetNode())->getZOrder();
}
void IGuiParts::SetVertexZ(F32 vertexZ) {
  ((cocos2d::CCNode*)GetNode())->setVertexZ(vertexZ);
}
F32 IGuiParts::GetVertexZ() {
  return ((cocos2d::CCNode*)GetNode())->getVertexZ();
}
void IGuiParts::SetScaleX(F32 fScaleX) {
  ((cocos2d::CCNode*)GetNode())->setScaleX(fScaleX);
}
F32 IGuiParts::GetScaleX() {
  return ((cocos2d::CCNode*)GetNode())->getScaleX();
}
void IGuiParts::SetScaleY(F32 fScaleY) {
  ((cocos2d::CCNode*)GetNode())->setScaleY(fScaleY);
}
F32 IGuiParts::GetScaleY() {
  return ((cocos2d::CCNode*)GetNode())->getScaleY();
}
void IGuiParts::SetScale(F32 scale) {
  ((cocos2d::CCNode*)GetNode())->setScale(scale);
}
F32 IGuiParts::GetScale() {
  return ((cocos2d::CCNode*)GetNode())->getScale();
}
void IGuiParts::SetPosition(const TVector2 &position) {
  cocos2d::CCPoint point(position.x,position.y);
  ((cocos2d::CCNode*)GetNode())->setPosition(point);
}
void IGuiParts::GetPosition(TVector2 &dst) {
  const cocos2d::CCPoint &point = ((cocos2d::CCNode*)GetNode())->getPosition();
  dst.x = point.x;
  dst.y = point.y;
}
void IGuiParts::SetPosition(F32 x, F32 y) {
  ((cocos2d::CCNode*)GetNode())->setPosition(x,y);
}
void IGuiParts::GetPosition(F32 *x, F32 *y) {
  ((cocos2d::CCNode*)GetNode())->getPosition(x,y);
}
void IGuiParts::SetPositionX(F32 x) {
  ((cocos2d::CCNode*)GetNode())->setPositionX(x);
}
F32 IGuiParts::GetPositionX(void) {
  return ((cocos2d::CCNode*)GetNode())->getPositionX();
}
void IGuiParts::SetPositionY(F32 y) {
  ((cocos2d::CCNode*)GetNode())->setPositionY(y);
}
F32 IGuiParts::GetPositionY(void) {
  return ((cocos2d::CCNode*)GetNode())->getPositionY();
}
void IGuiParts::SetSkewX(F32 fSkewX) {
  ((cocos2d::CCNode*)GetNode())->setSkewX(fSkewX);
}
F32 IGuiParts::GetSkewX() {
  return ((cocos2d::CCNode*)GetNode())->getSkewX();
}
void IGuiParts::SetSkewY(F32 fSkewY) {
  ((cocos2d::CCNode*)GetNode())->setSkewY(fSkewY);
}
F32 IGuiParts::GetSkewY() {
  return ((cocos2d::CCNode*)GetNode())->getSkewY();
}
void IGuiParts::SetAnchorPoint(const TVector2 &anchorPoint) {
  cocos2d::CCPoint point(anchorPoint.x,anchorPoint.y);
  ((cocos2d::CCNode*)GetNode())->setAnchorPoint(point);
}
void IGuiParts::GetAnchorPoint(TVector2 &dst) {
  const cocos2d::CCPoint &point = ((cocos2d::CCNode*)GetNode())->getAnchorPoint();
  dst.x = point.x;
  dst.y = point.y;
}
void IGuiParts::GetAnchorPointInPoints(TVector2 &dst) {
  const cocos2d::CCPoint &point = ((cocos2d::CCNode *)GetNode())->getAnchorPointInPoints();
  dst.x = point.x;
  dst.y = point.y;
}
void IGuiParts::SetContentSize(const TSize &contentSize) {
  const cocos2d::CCSize size(contentSize.width,contentSize.height);
  ((cocos2d::CCNode*)GetNode())->setContentSize(size);
}
void IGuiParts::GetContentSize(TSize &dst) {
  const cocos2d::CCSize &size = ((cocos2d::CCNode*)GetNode())->getContentSize();
  dst.width = size.width;
  dst.height = size.height;
}
void IGuiParts::SetVisible(Bool visible) {
  ((cocos2d::CCNode*)GetNode())->setVisible(visible);
}
Bool IGuiParts::IsVisible() {
  return ((cocos2d::CCNode*)GetNode())->isVisible();
}
void IGuiParts::SetRotation(F32 fRotation) {
  ((cocos2d::CCNode*)GetNode())->setRotation(fRotation);
}
F32 IGuiParts::GetRotation() {
  return ((cocos2d::CCNode*)GetNode())->getRotation();
}
void IGuiParts::SetRotationX(F32 fRotaionX) {
  ((cocos2d::CCNode*)GetNode())->setRotationX(fRotaionX);
}
F32 IGuiParts::GetRotationX() {
  return ((cocos2d::CCNode*)GetNode())->getRotationX();
}
void IGuiParts::SetRotationY(F32 fRotationY) {
  ((cocos2d::CCNode*)GetNode())->setRotationY(fRotationY);
}
F32 IGuiParts::GetRotationY() {
  return ((cocos2d::CCNode*)GetNode())->getRotationY();
}
void IGuiParts::SetOrderOfArrival(U32 uOrderOfArrival) {
  ((cocos2d::CCNode*)GetNode())->setOrderOfArrival(uOrderOfArrival);
}
U32 IGuiParts::GetOrderOfArrival() {
  return ((cocos2d::CCNode*)GetNode())->getOrderOfArrival();
}
void IGuiParts::IgnoreAnchorPointForPosition(Bool ignore) {
  ((cocos2d::CCNode*)GetNode())->ignoreAnchorPointForPosition(ignore);
}
Bool IGuiParts::IsIgnoreAnchorPointForPosition() {
  return ((cocos2d::CCNode*)GetNode())->isIgnoreAnchorPointForPosition();
}
void IGuiParts::AddChild(IGuiParts *child) {
  ((cocos2d::CCNode*)GetNode())->addChild((cocos2d::CCNode*)child->GetNode());
}
void IGuiParts::AddChild(IGuiParts *child, S32 zOrder) {
  ((cocos2d::CCNode*)GetNode())->addChild((cocos2d::CCNode*)child->GetNode(),zOrder);
}
void IGuiParts::AddChild(IGuiParts *child, S32 zOrder, S32 tag) {
  ((cocos2d::CCNode*)GetNode())->addChild((cocos2d::CCNode*)child->GetNode(),zOrder);
}
//#if 0 タグでの取得が必要なためコメント解除 2014.04.14 kato
IGuiParts *IGuiParts::GetChildByTag(S32 tag) {
  //((cocos2d::CCNode*)GetNode())->getChildByTag(tag);
  return (IGuiParts*)(((cocos2d::CCNode*)GetNode())->getChildByTag(tag));
}
//タグが必要なためGet/Set実装 2014.04.16 kato
S32 IGuiParts::GetTag() {
  return ((cocos2d::CCNode*)GetNode())->getTag();
}
void IGuiParts::SetTag(S32 tag) {
  ((cocos2d::CCNode*)GetNode())->setTag(tag);
}
#if 0
CCArray *IGuiParts::GetChildren() {
  ((cocos2d::CCNode*)GetNode())->getChildren();
}
#endif
U32 IGuiParts::GetChildrenCount(void) {
  return ((cocos2d::CCNode*)GetNode())->getChildrenCount();
}
void IGuiParts::SetParent(IGuiParts *parent) {
  ((cocos2d::CCNode*)GetNode())->setParent((cocos2d::CCNode*)parent->GetNode());
  //parent_=parent;
}
void IGuiParts::RemoveFromParent() {
  ((cocos2d::CCNode*)GetNode())->removeFromParent();
}
void IGuiParts::RemoveFromParentAndCleanup(Bool cleanup) {
  ((cocos2d::CCNode*)GetNode())->removeFromParentAndCleanup(cleanup);
}
void IGuiParts::RemoveChild(IGuiParts *child) {
  ((cocos2d::CCNode*)GetNode())->removeChild((cocos2d::CCNode*)child->GetNode());
}
void IGuiParts::RemoveChild(IGuiParts *child, Bool cleanup) {
  ((cocos2d::CCNode*)GetNode())->removeChild((cocos2d::CCNode*)child->GetNode(),cleanup);
}
void IGuiParts::RemoveChildByTag(S32 tag) {
  ((cocos2d::CCNode*)GetNode())->removeChildByTag(tag);
}
void IGuiParts::RemoveChildByTag(S32 tag, Bool cleanup) {
  ((cocos2d::CCNode*)GetNode())->removeChildByTag(tag,cleanup);
}
void IGuiParts::RemoveAllChildren() {
  ((cocos2d::CCNode*)GetNode())->removeAllChildren();
}
void IGuiParts::RemoveAllChildrenWithCleanup(Bool cleanup) {
  ((cocos2d::CCNode*)GetNode())->removeAllChildrenWithCleanup(cleanup);
}
void IGuiParts::ReorderChild(IGuiParts *child, S32 zOrder) {
  ((cocos2d::CCNode*)GetNode())->reorderChild((cocos2d::CCNode*)child->GetNode(),zOrder);
}
void IGuiParts::SortAllChildren() {
  ((cocos2d::CCNode*)GetNode())->sortAllChildren();
}

void IGuiParts::SetTouchPriority(S32 priority) { ADK_ASSERT(false); }
S32 IGuiParts::GetTouchPriority() {
  ADK_ASSERT(false);
  return 0;
}

ADK_END_NAMESPACE

