/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_sprite_cocos.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief スプライトパーツクラス定義部 for Cocos2d-x
 */

#ifndef GUI_PARTS_SPRITE_COCOS_H_
#define GUI_PARTS_SPRITE_COCOS_H_

#include <sprite_nodes/CCSprite.h>

#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>
#include <front_end/gui/parts/i_gui_parts_sprite.h>

ADK_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief スプライトパーツクラス
 */
class GuiPartsSpriteCocos : public IGuiPartsSprite {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  GuiPartsSpriteCocos();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~GuiPartsSpriteCocos();

  virtual Bool Init();

  /*
   * Cocos2d-xのCCNodeラッパーメソッドたち
   */
  virtual void SetContentSize(TSize &size);
  virtual void GetContentSize(TSize &dst);
  /*
   * Cocos2d-xのCCSpriteラッパーメソッドたち
   */
  virtual Bool InitWithTexture(IGuiPartsTexture* pTexture);
  virtual Bool InitWithTexture(IGuiPartsTexture* pTexture, const TRect& rect);
  virtual Bool InitWithTexture(IGuiPartsTexture* pTexture, const TRect& rect,
                               Bool rotated);
  virtual Bool InitWithSpriteFrame(IGuiPartsSpriteFrame* pSpriteFrame);
  virtual Bool InitWithSpriteFrameName(const char* pszSpriteFrameName);
  virtual Bool InitWithFile(const char* pszFilename);
  virtual Bool InitWithFile(const char* pszFilename, const TRect& rect);
  virtual void SetTexture(IGuiPartsTexture* texture);
  virtual IGuiPartsTexture* GetTexture(void);
  virtual void SetBlendFunc(TBlendFunc &blendFunc);
  virtual void GetBlendFunc(TBlendFunc &dst);
  virtual void SetScaleX(float fScaleX);
  virtual void SetScaleY(float fScaleY);
  virtual void SetPosition(const TVector2& pos);
  virtual void SetRotation(float fRotation);
  virtual void SetRotationX(float fRotationX);
  virtual void SetRotationY(float fRotationY);
  virtual void SetSkewX(float sx);
  virtual void SetSkewY(float sy);
  virtual void RemoveChild(IGuiParts* pChild, Bool bCleanup);
  virtual void RemoveAllChildrenWithCleanup(Bool bCleanup);
  virtual void ReorderChild(IGuiParts* pChild, int zOrder);
  virtual void SortAllChildren();
  virtual void SetScale(float fScale);
  virtual void SetVertexZ(float fVertexZ);
  virtual void SetAnchorPoint(const TVector2& anchor);
  virtual void IgnoreAnchorPointForPosition(Bool value);
  virtual void Draw(void);
  virtual void SetColor(const TColor& color3);
  virtual void UpdateDisplayedColor(const TColor& parentColor);
  virtual void SetOpacity(Byte opacity);
  virtual void SetOpacityModifyRGB(Bool modify);
  virtual Bool IsOpacityModifyRGB(void);
  virtual void UpdateDisplayedOpacity(Byte parentOpacity);
  virtual void UpdateTransform(void);
  virtual IGuiPartsSpriteBatchNode* GetBatchNode(void);
  virtual void SetBatchNode(IGuiPartsSpriteBatchNode* pobSpriteBatchNode);
  virtual void SetTextureRect(const TRect& rect);
  virtual void SetTextureRect(const TRect& rect, Bool rotated,
                              const TSize& untrimmedSize);
  virtual void SetVertexRect(const TRect& rect);
  virtual void SetDisplayFrame(IGuiPartsSpriteFrame* pNewFrame);
  virtual Bool IsFrameDisplayed(IGuiPartsSpriteFrame* pFrame);
  virtual IGuiPartsSpriteFrame* DisplayFrame(void);
  virtual void SetDisplayFrameWithAnimationName(const char* animationName,
                                                int frameIndex);
  virtual Bool IsDirty(void);
  virtual void SetDirty(Bool bDirty);
#if 0
  virtual ccV3F_C4B_T2F_Quad GetQuad(void);
#endif
  virtual Bool IsTextureRectRotated(void);
  virtual U32 GetAtlasIndex(void);
  virtual void SetAtlasIndex(unsigned int uAtlasIndex);
  virtual void GetTextureRect(TRect &dst);
  virtual IGuiPartsTextureAtlas* GetTextureAtlas(void);
  virtual void SetTextureAtlas(IGuiPartsTextureAtlas* pobTextureAtlas);
  virtual void GetOffsetPosition(TVector2 &dst);
  virtual Bool IsFlipX(void);
  virtual void SetFlipX(Bool bFlipX);
  virtual Bool IsFlipY(void);
  virtual void SetFlipY(Bool bFlipY);

 private:
  /*
   * とりあえず継承の設計が良いのが思いつかんので後ほど決める
   * 現状は同一メソッドを、各々のクラスで重複実装する
   */
  virtual IGuiParts *GetCustomClass(const char *name);
  virtual IGuiParts *AttachCreateParts(const Char *partsname,
                                       TGuiPartsType type);
  virtual Bool SetButtonCallback(const Char *name,
                                 TGuiPartsButtonCallback callback,
                                 void *userdata);
  virtual Bool SetMenuCallback(const Char *name, TGuiPartsMenuCallback callback,
                               void *userdata);
  virtual void SetTouchPriority(S32 priority);
  virtual S32 GetTouchPriority();
  virtual void SetVisible(Bool visible);
  virtual Bool IsVisible();
  virtual void AddChild(IGuiParts *child);
  virtual void AddChild(IGuiParts *child, S32 zOrder);
  virtual void AddChild(IGuiParts *child, S32 zOrder, S32 tag);
  virtual Bool CreateWithFile(const Char *filename, const TGuiPartsInfo *parts,
                              S32 parts_count);
  virtual Bool Dispose();
  virtual void SetNode(void *node);
  virtual void* GetNode();

  cocos2d::CCSprite *sprite_;
  /**
   * \~english
   * \~japanese
   *  \brief クラス内 new/delete オペレータ定義
   */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("GuiPartsSpriteCocos");
  /**
   * \~english
   * \~japanese
   *  \brief コピー禁止定義
   */
  ADK_NON_COPYABLE_CLASS(GuiPartsSpriteCocos);
};

ADK_END_NAMESPACE

#endif /* GUI_PARTS_SPRITE_COCOS_H_ */
