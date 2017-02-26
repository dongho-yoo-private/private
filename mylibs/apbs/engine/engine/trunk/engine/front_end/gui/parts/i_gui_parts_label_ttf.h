/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file i_gui_parts_label_ttf.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief TTFラベル定義部
 */

#ifndef I_GUI_PARTS_LABEL_TTF_H_
#define I_GUI_PARTS_LABEL_TTF_H_

#include <core/math/math_library.h>
#include <core/types.h>
#include <front_end/gui/parts/gui_parts_types.h>
#include <front_end/gui/parts/i_gui_parts_sprite.h>

ADK_BEGIN_NAMESPACE

class IGuiPartsLabelTTF : public IGuiParts {
 public:
  virtual ~IGuiPartsLabelTTF();
  
  /*
   * Cocos2d-xのCCLabelTTFラッパーたち
   */
  
  virtual Bool initWithString(const char *string, const char *fontName, float fontSize) = 0;
  virtual Bool initWithString(const char *string, const char *fontName, float fontSize,
                              const TSize& dimensions, TTextAlignment hAlignment) = 0;
  virtual Bool initWithString(const char *string, const char *fontName, float fontSize,
                              const TSize& dimensions, TTextAlignment hAlignment, TVerticalTextAlignment vAlignment) = 0;
  virtual Bool initWithStringAndTextDefinition(const char *string, TFontDefinition &textDefinition) = 0;
  virtual void setTextDefinition(TFontDefinition *theDefinition) = 0;
  virtual void getTextDefinition(TFontDefinition &dst) = 0;
  virtual void enableShadow(const TSize &shadowOffset, float shadowOpacity, float shadowBlur, bool mustUpdateTexture = true) = 0;
  virtual void disableShadow(bool mustUpdateTexture = true) = 0;
  virtual void enableStroke(const TColor &strokeColor, float strokeSize, bool mustUpdateTexture = true) = 0;
  virtual void disableStroke(bool mustUpdateTexture = true) = 0;
  virtual void setFontFillColor(const TColor &tintColor, bool mustUpdateTexture = true) = 0;
  virtual void setString(const char *label) = 0;
  virtual const char* getString() = 0;
  virtual void getHorizontalAlignment(TTextAlignment &dst) = 0;
  virtual void setHorizontalAlignment(TTextAlignment alignment) = 0;
  virtual void getVerticalAlignment(TVerticalTextAlignment &dst) = 0;
  virtual void setVerticalAlignment(TVerticalTextAlignment verticalAlignment) = 0;
  virtual void getDimensions(TSize& dst) = 0;
  virtual void setDimensions(const TSize &dim) = 0;
  virtual float getFontSize() = 0;
  virtual void setFontSize(float fontSize) = 0;
  virtual const char* getFontName() = 0;
  virtual void setFontName(const char *fontName) = 0;
};

ADK_END_NAMESPACE

#endif /* defined(__af_engine__i_gui_parts_labelttf__) */
