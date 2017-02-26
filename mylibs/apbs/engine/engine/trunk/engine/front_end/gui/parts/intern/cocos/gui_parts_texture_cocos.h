/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_texture_cocos.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief テクスチャパーツクラス定義部 for Cocos2d-x
 */

#ifndef GUI_PARTS_TEXTURE_COCOS_H_
#define GUI_PARTS_TEXTURE_COCOS_H_

#include <textures/CCTexture2D.h>

#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>
#include <front_end/gui/parts/gui_parts_scroll_view_types.h>
#include <front_end/gui/parts/i_gui_parts_texture.h>

ADK_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief テクスチャパーツクラス
 */
class GuiPartsTextureCocos : public IGuiPartsTexture {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  GuiPartsTextureCocos();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~GuiPartsTextureCocos();

  /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiPartsSprite オブジェクトの初期化処理を行います
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval true 成功したときに返ります
   *  \retval false 失敗したときに返ります
   */
  virtual Bool Init();

  /*
   * Cocos2d-xのCCTexture2Dラッパーメソッドたち
   */
  virtual const Char* Description(void);
  virtual void ReleaseData(void* data);
  virtual void* KeepData(void* data, unsigned int length);
  virtual Bool InitWithData(const void* data, TTexturePixelFormat pixelFormat,
                            unsigned int pixelsWide, unsigned int pixelsHigh,
                            const TSize& contentSize);
  virtual void DrawAtPoint(const TVector2& point);
  virtual void DrawInRect(const TRect& rect);
  virtual Bool InitWithImage(IGuiImage* uiImage);
  virtual Bool InitWithString(const Char* text, const Char* fontName,
                              float fontSize, const TSize& dimensions,
                              TTextAlignment hAlignment,
                              TVerticalTextAlignment vAlignment);
  virtual Bool InitWithString(const Char* text, const Char* fontName,
                              float fontSize);
  virtual Bool InitWithString(const Char* text,
                              TFontDefinition* textDefinition);
  virtual Bool InitWithPVRFile(const Char* file);
  virtual Bool InitWithETCFile(const Char* file);
  virtual void SetTexParameters(TTexParams* texParams);
  virtual void SetAntiAliasTexParameters();
  virtual void SetAliasTexParameters();
  virtual void GenerateMipmap();
  virtual const Char* StringForFormat();
  virtual unsigned int BitsPerPixelForFormat();
  virtual unsigned int BitsPerPixelForFormat(TTexturePixelFormat format);
  virtual void GetContentSizeInPixels(TSize &dst);
  virtual Bool HasPremultipliedAlpha();
  virtual Bool HasMipmaps();

  cocos2d::CCTexture2D* GetCCTexture() {return NULL;}

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

  cocos2d::CCTexture2D *texture_;
  /**
   * \~english
   * \~japanese
   *  \brief クラス内 new/delete オペレータ定義
   */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("GuiPartsTextureCocos");
  /**
   * \~english
   * \~japanese
   *  \brief コピー禁止定義
   */
  ADK_NON_COPYABLE_CLASS(GuiPartsTextureCocos);
};

ADK_END_NAMESPACE

#endif /* GUI_PARTS_TEXTURE_COCOS_H_ */
