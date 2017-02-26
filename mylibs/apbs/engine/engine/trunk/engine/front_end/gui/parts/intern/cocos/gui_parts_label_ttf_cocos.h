/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file gui_parts_label_ttf_cocos.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief TTFラベル定義部 for cocos2d-x
 */

#ifndef GUI_PARTS_LABEL_TTF_COCOS_H_
#define GUI_PARTS_LABEL_TTF_COCOS_H_

#include <label_nodes/CCLabelTTF.h>

#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>
#include <front_end/gui/parts/i_gui_parts_label_ttf.h>

ADK_BEGIN_NAMESPACE

class GuiPartsLabelTTFCocos : public IGuiPartsLabelTTF {
 public:
  
  GuiPartsLabelTTFCocos();
  virtual ~GuiPartsLabelTTFCocos();
  virtual Bool Init();
  
  virtual Bool initWithString(const char *string, const char *fontName, float fontSize);
  virtual Bool initWithString(const char *string, const char *fontName, float fontSize,
                              const TSize& dimensions, TTextAlignment hAlignment);
  virtual Bool initWithString(const char *string, const char *fontName, float fontSize,
                              const TSize& dimensions, TTextAlignment hAlignment, TVerticalTextAlignment vAlignment);
  virtual Bool initWithStringAndTextDefinition(const char *string, TFontDefinition &textDefinition);
  virtual void setTextDefinition(TFontDefinition *theDefinition);
  virtual void getTextDefinition(TFontDefinition &dst);
  virtual void enableShadow(const TSize &shadowOffset, float shadowOpacity, float shadowBlur, bool mustUpdateTexture = true);
  virtual void disableShadow(bool mustUpdateTexture = true);
  virtual void enableStroke(const TColor &strokeColor, float strokeSize, bool mustUpdateTexture = true);
  virtual void disableStroke(bool mustUpdateTexture = true);
  virtual void setFontFillColor(const TColor &tintColor, bool mustUpdateTexture = true);
  virtual void setString(const char *label);
  virtual const char* getString();
  virtual void getHorizontalAlignment(TTextAlignment &dst);
  virtual void setHorizontalAlignment(TTextAlignment alignment);
  virtual void getVerticalAlignment(TVerticalTextAlignment &dst);
  virtual void setVerticalAlignment(TVerticalTextAlignment verticalAlignment);
  virtual void getDimensions(TSize& dst);
  virtual void setDimensions(const TSize &dim);
  virtual float getFontSize();
  virtual void setFontSize(float fontSize);
  virtual const char* getFontName();
  virtual void setFontName(const char *fontName);
  
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
  
  cocos2d::CCLabelTTF *label_;

  /**
   * \~english
   * \~japanese
   *  \brief クラス内 new/delete オペレータ定義
   */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("GuiPartsLabelTTFCocos");
  /**
   * \~english
   * \~japanese
   *  \brief コピー禁止定義
   */
  ADK_NON_COPYABLE_CLASS(GuiPartsLabelTTFCocos);

};

ADK_END_NAMESPACE

#endif /* defined(__af_engine__gui_parts_label_ttf_cocos__) */
