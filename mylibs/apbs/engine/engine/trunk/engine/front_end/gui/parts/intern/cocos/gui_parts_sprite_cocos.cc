/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_sprite_cocos.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief スプライトパーツクラス実装部 for Cocos2d-x
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_sprite_cocos.h>

#include <front_end/gui/parts/intern/cocos/gui_parts_texture_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_sprite_frame_cocos.h>

ADK_BEGIN_NAMESPACE

GuiPartsSpriteCocos::GuiPartsSpriteCocos() : sprite_(NULL) {
#if 0
  SetPartsType(kGuiPartsTypeSprite);
#endif
}
GuiPartsSpriteCocos::~GuiPartsSpriteCocos() {}

Bool GuiPartsSpriteCocos::Init() {
  Bool ret = true;

  sprite_ = new cocos2d::CCSprite;
  ADK_ASSERT(sprite_);
  if (sprite_ == NULL) {
    ret = false;
  } else {
    if (sprite_->init() == false) {
      delete sprite_, sprite_ = 0;
    } else {
#if 0
      sprite_->autorelease();
#endif
    }
  }

  return ret;
}

void GuiPartsSpriteCocos::SetContentSize(TSize& size) {
  cocos2d::CCSize content_size(size.width, size.height);
  sprite_->setContentSize(content_size);
}
void GuiPartsSpriteCocos::GetContentSize(TSize& dst) {
  const cocos2d::CCSize& size = sprite_->getContentSize();
  dst.width = size.width;
  dst.height = size.height;
}

Bool GuiPartsSpriteCocos::InitWithTexture(IGuiPartsTexture* pTexture) {
  Bool ret = true;

  ADK_ASSERT(sprite_);
  if (sprite_ == NULL) {
    ret = false;
  } else {
    cocos2d::CCTexture2D* cctexture =
        ((GuiPartsTextureCocos*)pTexture)->GetCCTexture();
    if (sprite_->initWithTexture(cctexture) == false) {
      return false;
    } else {
      sprite_->autorelease();
    }
  }

  return ret;
}
Bool GuiPartsSpriteCocos::InitWithTexture(IGuiPartsTexture* pTexture,
                                          const TRect& rect) {
  Bool ret = true;

  ADK_ASSERT(sprite_);
  if (sprite_ == NULL) {
    ret = false;
  } else {
    cocos2d::CCTexture2D* cctexture =
        ((GuiPartsTextureCocos*)pTexture)->GetCCTexture();
    cocos2d::CCRect rectangle(rect.left, rect.top, rect.width, rect.height);
    if (sprite_->initWithTexture(cctexture, rectangle) == false) {
      ret = false;
    } else {
      sprite_->autorelease();
    }
  }

  return ret;
}
Bool GuiPartsSpriteCocos::InitWithTexture(IGuiPartsTexture* pTexture,
                                          const TRect& rect, Bool rotated) {
  Bool ret = true;

  cocos2d::CCTexture2D* cctexture =
      ((GuiPartsTextureCocos*)pTexture)->GetCCTexture();
  cocos2d::CCRect rectangle(rect.left, rect.top, rect.width, rect.height);
  if (sprite_->initWithTexture(cctexture, rectangle, rotated) == false) {
    ret = false;
  } else {
    sprite_->autorelease();
  }

  return ret;
}
Bool GuiPartsSpriteCocos::InitWithSpriteFrame(
    IGuiPartsSpriteFrame* pSpriteFrame) {
  Bool ret = true;

  cocos2d::CCSpriteFrame* ccspriteframe =
      ((GuiPartsSpriteFrameCocos*)pSpriteFrame)->GetCCSpriteFrame();
  if (sprite_->initWithSpriteFrame(ccspriteframe) == false) {
    ret = false;
  } else {
    sprite_->autorelease();
  }

  return ret;
}
Bool GuiPartsSpriteCocos::InitWithSpriteFrameName(
    const char* pszSpriteFrameName) {
  Bool ret = true;

  if (sprite_->initWithSpriteFrameName(pszSpriteFrameName) == false) {
    ret = false;
  } else {
    sprite_->autorelease();
  }

  return ret;
}
Bool GuiPartsSpriteCocos::InitWithFile(const char* pszFilename) {
  Bool ret = true;

  if (sprite_->initWithFile(pszFilename) == false) {
    ret = false;
  } else {
    sprite_->autorelease();
  }

  return ret;
}
Bool GuiPartsSpriteCocos::InitWithFile(const char* pszFilename,
                                       const TRect& rect) {
  Bool ret = true;

  cocos2d::CCRect rectangle(rect.left, rect.top, rect.width, rect.height);
  if (sprite_->initWithFile(pszFilename, rectangle) == false) {
    ret = false;
  } else {
    sprite_->autorelease();
  }

  return ret;
}

void GuiPartsSpriteCocos::SetTexture(IGuiPartsTexture* texture) {
  cocos2d::CCTexture2D* cctexture =
      ((GuiPartsTextureCocos*)texture)->GetCCTexture();
  sprite_->setTexture(cctexture);
}
IGuiPartsTexture* GuiPartsSpriteCocos::GetTexture(void) {
  ADK_ASSERT(false);
  return NULL;
}
void GuiPartsSpriteCocos::SetBlendFunc(TBlendFunc& blendFunc) {
  cocos2d::ccBlendFunc bf = {(GLenum)blendFunc.src, (GLenum)blendFunc.dst};
  sprite_->setBlendFunc(bf);
}
void GuiPartsSpriteCocos::GetBlendFunc(TBlendFunc& dst) {
  cocos2d::ccBlendFunc blendfunc = sprite_->getBlendFunc();
  dst.src = blendfunc.src;
  dst.dst = blendfunc.dst;
}
void GuiPartsSpriteCocos::SetScaleX(float fScaleX) {
  sprite_->setScaleX(fScaleX);
}
void GuiPartsSpriteCocos::SetScaleY(float fScaleY) {
  sprite_->setScaleY(fScaleY);
}
void GuiPartsSpriteCocos::SetPosition(const TVector2& pos) {
  cocos2d::CCPoint point(pos.x, pos.y);
  sprite_->setPosition(point);
}
void GuiPartsSpriteCocos::SetRotation(float fRotation) {
  sprite_->setRotation(fRotation);
}
void GuiPartsSpriteCocos::SetRotationX(float fRotationX) {
  sprite_->setRotationX(fRotationX);
}
void GuiPartsSpriteCocos::SetRotationY(float fRotationY) {
  sprite_->setRotationY(fRotationY);
}
void GuiPartsSpriteCocos::SetSkewX(float sx) { sprite_->setSkewX(sx); }
void GuiPartsSpriteCocos::SetSkewY(float sy) { sprite_->setSkewY(sy); }
void GuiPartsSpriteCocos::RemoveChild(IGuiParts* pChild, Bool bCleanup) {
  cocos2d::CCNode* ccnode = (cocos2d::CCNode*)pChild->GetNode();
  sprite_->removeChild(ccnode, bCleanup);
}
void GuiPartsSpriteCocos::RemoveAllChildrenWithCleanup(Bool bCleanup) {
  sprite_->removeAllChildrenWithCleanup(bCleanup);
}
void GuiPartsSpriteCocos::ReorderChild(IGuiParts* pChild, int zOrder) {
  cocos2d::CCNode* ccnode = (cocos2d::CCNode*)pChild->GetNode();
  sprite_->reorderChild(ccnode, zOrder);
}
void GuiPartsSpriteCocos::SortAllChildren() { sprite_->sortAllChildren(); }
void GuiPartsSpriteCocos::SetScale(float fScale) { sprite_->setScale(fScale); }
void GuiPartsSpriteCocos::SetVertexZ(float fVertexZ) {
  sprite_->setVertexZ(fVertexZ);
}
void GuiPartsSpriteCocos::SetAnchorPoint(const TVector2& anchor) {
  cocos2d::CCPoint point(anchor.x, anchor.y);
  sprite_->setAnchorPoint(point);
}
void GuiPartsSpriteCocos::IgnoreAnchorPointForPosition(Bool value) {
  sprite_->ignoreAnchorPointForPosition(value);
}
void GuiPartsSpriteCocos::Draw(void) { sprite_->draw(); }
void GuiPartsSpriteCocos::SetColor(const TColor& color3) {
  cocos2d::ccColor3B color = {color3.r, color3.g, color3.b};
  sprite_->setColor(color);
}
void GuiPartsSpriteCocos::UpdateDisplayedColor(const TColor& parentColor) {
  cocos2d::ccColor3B color = {parentColor.r, parentColor.g, parentColor.b};
  sprite_->updateDisplayedColor(color);
}
void GuiPartsSpriteCocos::SetOpacity(Byte opacity) {
  sprite_->setOpacity(opacity);
}
void GuiPartsSpriteCocos::SetOpacityModifyRGB(Bool modify) {
  sprite_->setOpacityModifyRGB(modify);
}
Bool GuiPartsSpriteCocos::IsOpacityModifyRGB(void) {
  return sprite_->isOpacityModifyRGB();
}
void GuiPartsSpriteCocos::UpdateDisplayedOpacity(Byte parentOpacity) {
  sprite_->updateDisplayedOpacity(parentOpacity);
}
void GuiPartsSpriteCocos::UpdateTransform(void) { sprite_->updateTransform(); }
IGuiPartsSpriteBatchNode* GuiPartsSpriteCocos::GetBatchNode(void) {
#if 1
  ADK_ASSERT(false);
  return NULL;
#else
  sprite_->getBatchNode();
#endif
}
void GuiPartsSpriteCocos::SetBatchNode(
    IGuiPartsSpriteBatchNode* pobSpriteBatchNode) {
  ADK_ASSERT(false);
  sprite_->setBatchNode(NULL);
}
void GuiPartsSpriteCocos::SetTextureRect(const TRect& rect) {
  cocos2d::CCRect rectangle(rect.left, rect.top, rect.width, rect.height);
  sprite_->setTextureRect(rectangle);
}
void GuiPartsSpriteCocos::SetTextureRect(const TRect& rect, Bool rotated,
                                         const TSize& untrimmedSize) {
  cocos2d::CCRect rectangle(rect.left, rect.top, rect.width, rect.height);
  cocos2d::CCSize size(untrimmedSize.width, untrimmedSize.height);
  sprite_->setTextureRect(rectangle, rotated, size);
}
void GuiPartsSpriteCocos::SetVertexRect(const TRect& rect) {
  cocos2d::CCRect rectangle(rect.left, rect.top, rect.width, rect.height);
  sprite_->setVertexRect(rectangle);
}
void GuiPartsSpriteCocos::SetDisplayFrame(IGuiPartsSpriteFrame* pNewFrame) {
  cocos2d::CCSpriteFrame* ccspriteframe =
      ((GuiPartsSpriteFrameCocos*)pNewFrame)->GetCCSpriteFrame();
  sprite_->setDisplayFrame(ccspriteframe);
}
Bool GuiPartsSpriteCocos::IsFrameDisplayed(IGuiPartsSpriteFrame* pFrame) {
  cocos2d::CCSpriteFrame* ccspriteframe =
      ((GuiPartsSpriteFrameCocos*)pFrame)->GetCCSpriteFrame();
  return sprite_->isFrameDisplayed(ccspriteframe);
}
IGuiPartsSpriteFrame* GuiPartsSpriteCocos::DisplayFrame(void) {
#if 1
  ADK_ASSERT(false);
  return NULL;
#else
  sprite_->displayFrame();
#endif
}
void GuiPartsSpriteCocos::SetDisplayFrameWithAnimationName(
    const char* animationName, int frameIndex) {
  sprite_->setDisplayFrameWithAnimationName(animationName, frameIndex);
}
Bool GuiPartsSpriteCocos::IsDirty(void) { return sprite_->isDirty(); }
void GuiPartsSpriteCocos::SetDirty(Bool bDirty) { sprite_->setDirty(bDirty); }
#if 0
  ccV3F_C4B_T2F_Quad GetQuad(void){}
#endif
Bool GuiPartsSpriteCocos::IsTextureRectRotated(void) {
  return sprite_->isTextureRectRotated();
}
U32 GuiPartsSpriteCocos::GetAtlasIndex(void) {
  return sprite_->getAtlasIndex();
}
void GuiPartsSpriteCocos::SetAtlasIndex(unsigned int uAtlasIndex) {
  sprite_->setAtlasIndex(uAtlasIndex);
}
void GuiPartsSpriteCocos::GetTextureRect(TRect& dst) {
  const cocos2d::CCRect& rectangle = sprite_->getTextureRect();
  dst.left = rectangle.origin.x;
  dst.top = rectangle.origin.y;
  dst.width = rectangle.size.width;
  dst.height = rectangle.size.height;
}
IGuiPartsTextureAtlas* GuiPartsSpriteCocos::GetTextureAtlas(void) {
#if 1
  ADK_ASSERT(false);
  return NULL;
#else
  sprite_->getTextureAtlas();
#endif
}
void GuiPartsSpriteCocos::SetTextureAtlas(
    IGuiPartsTextureAtlas* pobTextureAtlas) {
#if 1
  ADK_ASSERT(false);
#else
  sprite_->setTextureAtlas();
#endif
}
void GuiPartsSpriteCocos::GetOffsetPosition(TVector2& dst) {
  const cocos2d::CCPoint& point = sprite_->getOffsetPosition();
  dst.x = point.x;
  dst.y = point.y;
}
Bool GuiPartsSpriteCocos::IsFlipX(void) { return sprite_->isFlipX(); }
void GuiPartsSpriteCocos::SetFlipX(Bool bFlipX) { sprite_->setFlipX(bFlipX); }
Bool GuiPartsSpriteCocos::IsFlipY(void) { return sprite_->isFlipY(); }
void GuiPartsSpriteCocos::SetFlipY(Bool bFlipY) { sprite_->setFlipY(bFlipY); }

IGuiParts* GuiPartsSpriteCocos::GetCustomClass(const char* name) {
  ADK_ASSERT(false);
  return NULL;
}
IGuiParts* GuiPartsSpriteCocos::AttachCreateParts(const Char* partsname,
                                                  TGuiPartsType type) {
  ADK_ASSERT(false);
  return NULL;
}
Bool GuiPartsSpriteCocos::SetButtonCallback(const Char* name,
                                            TGuiPartsButtonCallback callback,
                                            void* userdata) {
  ADK_ASSERT(false);
  return false;
}
Bool GuiPartsSpriteCocos::SetMenuCallback(const Char* name,
                                          TGuiPartsMenuCallback callback,
                                          void* userdata) {
  ADK_ASSERT(false);
  return false;
}
void GuiPartsSpriteCocos::SetTouchPriority(S32 priority) { ADK_ASSERT(false); }
S32 GuiPartsSpriteCocos::GetTouchPriority() {
  ADK_ASSERT(false);
  return NULL;
}
void GuiPartsSpriteCocos::SetVisible(Bool visible) {
  sprite_->setVisible(visible);
}
Bool GuiPartsSpriteCocos::IsVisible() { return sprite_->isVisible(); }
void GuiPartsSpriteCocos::AddChild(IGuiParts* pChild) {
  cocos2d::CCNode* ccnode = (cocos2d::CCNode*)pChild->GetNode();
  sprite_->addChild(ccnode);
}
void GuiPartsSpriteCocos::AddChild(IGuiParts* pChild, int zOrder) {
  cocos2d::CCNode* ccnode = (cocos2d::CCNode*)pChild->GetNode();
  sprite_->addChild(ccnode, zOrder);
}
void GuiPartsSpriteCocos::AddChild(IGuiParts* pChild, int zOrder, int tag) {
  cocos2d::CCNode* ccnode = (cocos2d::CCNode*)pChild->GetNode();
  sprite_->addChild(ccnode, zOrder, tag);
}
Bool GuiPartsSpriteCocos::CreateWithFile(const Char* filename,
                                         const TGuiPartsInfo* parts,
                                         S32 parts_count) {
  ADK_ASSERT(false);
}
Bool GuiPartsSpriteCocos::Dispose() { ADK_ASSERT(false); }
void GuiPartsSpriteCocos::SetNode(void* node) {
  sprite_ = (cocos2d::CCSprite*)node;
}
void* GuiPartsSpriteCocos::GetNode() { return sprite_; }

ADK_END_NAMESPACE

