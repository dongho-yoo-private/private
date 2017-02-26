/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file i_gui_parts_texture.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief テクスチャクラス定義部
 */

#ifndef I_GUI_PARTS_TEXTURE_H_
#define I_GUI_PARTS_TEXTURE_H_

#include <core/math/math_library.h>
#include <core/types.h>
#include <front_end/gui/parts/gui_parts_types.h>
#include <front_end/gui/parts/i_gui_parts.h>
#include <llr/llr_types.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class IGuiImage;

/**
 * \~english
 * \~japanese
 *  \brief テクスチャパーツクラス
 */
class IGuiPartsTexture : public IGuiParts {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~IGuiPartsTexture();

  /*
   * Cocos2d-xのCCTexture2Dラッパーメソッドたち
   */
  virtual const Char* Description(void) = 0;
  virtual void ReleaseData(void* data) = 0;
  virtual void* KeepData(void* data, unsigned int length) = 0;
  virtual Bool InitWithData(const void* data, TTexturePixelFormat pixelFormat,
                            unsigned int pixelsWide, unsigned int pixelsHigh,
                            const TSize& contentSize) = 0;
  virtual void DrawAtPoint(const TVector2& point) = 0;
  virtual void DrawInRect(const TRect& rect) = 0;
  virtual Bool InitWithImage(IGuiImage* uiImage) = 0;
  virtual Bool InitWithString(const Char* text, const Char* fontName,
                              float fontSize, const TSize& dimensions,
                              TTextAlignment hAlignment,
                              TVerticalTextAlignment vAlignment) = 0;
  virtual Bool InitWithString(const Char* text, const Char* fontName,
                              float fontSize) = 0;
  virtual Bool InitWithString(const Char* text,
                              TFontDefinition* textDefinition) = 0;
  virtual Bool InitWithPVRFile(const Char* file) = 0;
  virtual Bool InitWithETCFile(const Char* file) = 0;
  virtual void SetTexParameters(TTexParams* texParams) = 0;
  virtual void SetAntiAliasTexParameters() = 0;
  virtual void SetAliasTexParameters() = 0;
  virtual void GenerateMipmap() = 0;
  virtual const Char* StringForFormat() = 0;
  virtual unsigned int BitsPerPixelForFormat() = 0;
  virtual unsigned int BitsPerPixelForFormat(TTexturePixelFormat format) = 0;
#if 0 /* TODO 別途extern関数用意する */
  static void SetDefaultAlphaPixelFormat(
      TTexturePixelFormat format) = 0;
  static TTexturePixelFormat DefaultAlphaPixelFormat() = 0;
  static void PVRImagesHavePremultipliedAlpha(
      Bool haveAlphaPremultiplied) = 0;
#endif
  virtual void GetContentSizeInPixels(TSize &dst) = 0;
  virtual Bool HasPremultipliedAlpha() = 0;
  virtual Bool HasMipmaps() = 0;
};

ADK_END_NAMESPACE

#endif /* I_GUI_PARTS_TEXTURE_H_ */
