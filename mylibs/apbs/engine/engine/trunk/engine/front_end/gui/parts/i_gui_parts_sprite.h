/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file i_gui_parts_sprite.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief スプライトクラス定義部
 */

#ifndef I_GUI_PARTS_SPRITE_H_
#define I_GUI_PARTS_SPRITE_H_

#include <core/math/math_library.h>
#include <core/types.h>
#include <front_end/gui/parts/gui_parts_types.h>
#include <front_end/gui/parts/i_gui_parts.h>
#include <llr/llr_types.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class IGuiPartsTexture;
class IGuiPartsSpriteBatchNode;
class IGuiPartsSpriteFrame;
class IGuiPartsTextureAtlas;

/**
 * \~english
 * \~japanese
 *  \brief スプライトパーツクラス
 */
class IGuiPartsSprite : public IGuiParts {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~IGuiPartsSprite();

  /*
   * Cocos2d-xのCCNodeラッパーメソッドたち
   */
  virtual void SetContentSize(TSize &size) = 0;
  virtual void GetContentSize(TSize &dst) = 0;
  /*
   * Cocos2d-xのCCSpriteラッパーメソッドたち
   */
  virtual Bool InitWithTexture(IGuiPartsTexture* pTexture) = 0;
  virtual Bool InitWithTexture(IGuiPartsTexture* pTexture, const TRect& rect) = 0;
  virtual Bool InitWithTexture(IGuiPartsTexture* pTexture, const TRect& rect,
                               Bool rotated) = 0;
  virtual Bool InitWithSpriteFrame(IGuiPartsSpriteFrame* pSpriteFrame) = 0;
  virtual Bool InitWithSpriteFrameName(const char* pszSpriteFrameName) = 0;
  virtual Bool InitWithFile(const char* pszFilename) = 0;
  virtual Bool InitWithFile(const char* pszFilename, const TRect& rect) = 0;
  virtual void SetTexture(IGuiPartsTexture* texture) = 0;
  virtual IGuiPartsTexture* GetTexture(void) = 0;
  virtual void SetBlendFunc(TBlendFunc &blendFunc) = 0;
  virtual void GetBlendFunc(TBlendFunc &dst) = 0;
  virtual void SetScaleX(float fScaleX) = 0;
  virtual void SetScaleY(float fScaleY) = 0;
  virtual void SetPosition(const TVector2& pos) = 0;
  virtual void SetRotation(float fRotation) = 0;
  virtual void SetRotationX(float fRotationX) = 0;
  virtual void SetRotationY(float fRotationY) = 0;
  virtual void SetSkewX(float sx) = 0;
  virtual void SetSkewY(float sy) = 0;
  virtual void RemoveChild(IGuiParts* pChild, Bool bCleanup) = 0;
  virtual void RemoveAllChildrenWithCleanup(Bool bCleanup) = 0;
  virtual void ReorderChild(IGuiParts* pChild, int zOrder) = 0;
  virtual void SortAllChildren() = 0;
  virtual void SetScale(float fScale) = 0;
  virtual void SetVertexZ(float fVertexZ) = 0;
  virtual void SetAnchorPoint(const TVector2& anchor) = 0;
  virtual void IgnoreAnchorPointForPosition(Bool value) = 0;
  virtual void Draw(void) = 0;
  virtual void SetColor(const TColor& color3) = 0;
  virtual void UpdateDisplayedColor(const TColor& parentColor) = 0;
  virtual void SetOpacity(Byte opacity) = 0;
  virtual void SetOpacityModifyRGB(Bool modify) = 0;
  virtual Bool IsOpacityModifyRGB(void) = 0;
  virtual void UpdateDisplayedOpacity(Byte parentOpacity) = 0;
  virtual void UpdateTransform(void) = 0;
  virtual IGuiPartsSpriteBatchNode* GetBatchNode(void) = 0;
  virtual void SetBatchNode(IGuiPartsSpriteBatchNode* pobSpriteBatchNode) = 0;
  virtual void SetTextureRect(const TRect& rect) = 0;
  virtual void SetTextureRect(const TRect& rect, Bool rotated,
                              const TSize& untrimmedSize) = 0;
  virtual void SetVertexRect(const TRect& rect) = 0;
  virtual void SetDisplayFrame(IGuiPartsSpriteFrame* pNewFrame) = 0;
  virtual Bool IsFrameDisplayed(IGuiPartsSpriteFrame* pFrame) = 0;
  virtual IGuiPartsSpriteFrame* DisplayFrame(void) = 0;
  virtual void SetDisplayFrameWithAnimationName(const char* animationName,
                                                int frameIndex) = 0;
  virtual Bool IsDirty(void) = 0;
  virtual void SetDirty(Bool bDirty) = 0;
#if 0
  virtual ccV3F_C4B_T2F_Quad GetQuad(void) = 0;
#endif
  virtual Bool IsTextureRectRotated(void) = 0;
  virtual U32 GetAtlasIndex(void) = 0;
  virtual void SetAtlasIndex(unsigned int uAtlasIndex) = 0;
  virtual void GetTextureRect(TRect &dst) = 0;
  virtual IGuiPartsTextureAtlas* GetTextureAtlas(void) = 0;
  virtual void SetTextureAtlas(IGuiPartsTextureAtlas* pobTextureAtlas) = 0;
  virtual void GetOffsetPosition(TVector2 &dst) = 0;
  virtual Bool IsFlipX(void) = 0;
  virtual void SetFlipX(Bool bFlipX) = 0;
  virtual Bool IsFlipY(void) = 0;
  virtual void SetFlipY(Bool bFlipY) = 0;
};

ADK_END_NAMESPACE

#endif /* I_GUI_PARTS_SPRITE_H_ */
