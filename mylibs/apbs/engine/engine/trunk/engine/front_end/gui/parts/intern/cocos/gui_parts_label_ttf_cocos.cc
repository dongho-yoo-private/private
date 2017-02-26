/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file gui_parts_label_ttf_cocos.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief TTFラベル実装部 for cocos2d-x
 */

#include "gui_parts_label_ttf_cocos.h"

ADK_BEGIN_NAMESPACE

GuiPartsLabelTTFCocos::GuiPartsLabelTTFCocos() : label_(NULL) {
#if 0
  SetPartsType(kGuiPartsTypeLabelTTFs);
#endif
}
GuiPartsLabelTTFCocos::~GuiPartsLabelTTFCocos() {}

Bool GuiPartsLabelTTFCocos::Init() {
  Bool ret = true;
  
  label_ = new cocos2d::CCLabelTTF;
  ADK_ASSERT(label_);
  if (label_ == NULL) {
    ret = false;
  } else {
    if (label_->init() == false) {
      delete label_, label_ = 0;
    } else {
#if 0
      label_->autorelease();
#endif
    }
  }
  
  return ret;
}

Bool GuiPartsLabelTTFCocos::initWithString(const char *string, const char *fontName, float fontSize){
  Bool ret = Init();
  return ret;
}
Bool GuiPartsLabelTTFCocos::initWithString(const char *string, const char *fontName, float fontSize,
                                           const TSize& dimensions, TTextAlignment hAlignment){
  Bool ret = Init();
  return ret;
}
Bool GuiPartsLabelTTFCocos::initWithString(const char *string, const char *fontName, float fontSize,
                                           const TSize& dimensions, TTextAlignment hAlignment, TVerticalTextAlignment vAlignment){
  Bool ret = Init();
  return ret;
}
Bool GuiPartsLabelTTFCocos::initWithStringAndTextDefinition(const char *string, TFontDefinition &textDefinition){
  Bool ret = Init();
  return ret;
}
void GuiPartsLabelTTFCocos::setTextDefinition(TFontDefinition *theDefinition) {
  cocos2d::ccFontDefinition font_definition;
  font_definition.m_fontSize = theDefinition->m_fontSize;
  font_definition.m_fontName = theDefinition->m_fontName;
  
  font_definition.m_dimensions.width = theDefinition->m_dimensions.width;
  font_definition.m_dimensions.height = theDefinition->m_dimensions.height;
  
  font_definition.m_alignment = (cocos2d::CCTextAlignment)theDefinition->m_alignment;
  font_definition.m_vertAlignment = (cocos2d::CCVerticalTextAlignment)theDefinition->m_vertAlignment;

  font_definition.m_fontFillColor.r = theDefinition->m_fontFillColor.r;
  font_definition.m_fontFillColor.g = theDefinition->m_fontFillColor.g;
  font_definition.m_fontFillColor.b = theDefinition->m_fontFillColor.b;
  
  font_definition.m_shadow.m_shadowEnabled = theDefinition->m_shadow.m_shadowEnabled;
  font_definition.m_shadow.m_shadowOffset.width = theDefinition->m_shadow.m_shadowOffset.width;
  font_definition.m_shadow.m_shadowOffset.height = theDefinition->m_shadow.m_shadowOffset.height;
  font_definition.m_shadow.m_shadowBlur = theDefinition->m_shadow.m_shadowBlur;
  font_definition.m_shadow.m_shadowOpacity = theDefinition->m_shadow.m_shadowOpacity;
  
  font_definition.m_stroke.m_strokeEnabled = theDefinition->m_stroke.m_strokeEnabled;
  font_definition.m_stroke.m_strokeColor.r = theDefinition->m_stroke.m_strokeColor.r;
  font_definition.m_stroke.m_strokeColor.g = theDefinition->m_stroke.m_strokeColor.g;
  font_definition.m_stroke.m_strokeColor.b = theDefinition->m_stroke.m_strokeColor.b;
  font_definition.m_stroke.m_strokeSize = theDefinition->m_stroke.m_strokeSize;
  
  label_->setTextDefinition(&font_definition);
}
void GuiPartsLabelTTFCocos::getTextDefinition(TFontDefinition &dst) {
  cocos2d::ccFontDefinition* font_definition = label_->getTextDefinition();
  
  dst.m_fontSize = font_definition->m_fontSize;
  strcpy(dst.m_fontName, font_definition->m_fontName.c_str());
  
  dst.m_dimensions.width = font_definition->m_dimensions.width;
  dst.m_dimensions.height = font_definition->m_dimensions.height;
  
  dst.m_alignment = (TTextAlignment)font_definition->m_alignment;
  dst.m_vertAlignment = (TVerticalTextAlignment)font_definition->m_vertAlignment;
  
  dst.m_fontFillColor.r = font_definition->m_fontFillColor.r;
  dst.m_fontFillColor.g = font_definition->m_fontFillColor.g;
  dst.m_fontFillColor.b = font_definition->m_fontFillColor.b;
  
  dst.m_shadow.m_shadowEnabled = font_definition->m_shadow.m_shadowEnabled;
  dst.m_shadow.m_shadowOffset.width = font_definition->m_shadow.m_shadowOffset.width;
  dst.m_shadow.m_shadowOffset.height = font_definition->m_shadow.m_shadowOffset.height;
  dst.m_shadow.m_shadowBlur = font_definition->m_shadow.m_shadowBlur;
  dst.m_shadow.m_shadowOpacity = font_definition->m_shadow.m_shadowOpacity;
  
  dst.m_stroke.m_strokeEnabled = font_definition->m_stroke.m_strokeEnabled;
  dst.m_stroke.m_strokeColor.r = font_definition->m_stroke.m_strokeColor.r;
  dst.m_stroke.m_strokeColor.g = font_definition->m_stroke.m_strokeColor.g;
  dst.m_stroke.m_strokeColor.b = font_definition->m_stroke.m_strokeColor.b;
  dst.m_stroke.m_strokeSize = font_definition->m_stroke.m_strokeSize;
}
void GuiPartsLabelTTFCocos::enableShadow(const TSize &shadowOffset, float shadowOpacity, float shadowBlur, bool updateTexture) {
  cocos2d::CCSize size;
  size.width = shadowOffset.width;
  size.height = shadowOffset.height;
  label_->enableShadow(size, shadowOpacity, shadowBlur, updateTexture);
  
}
void GuiPartsLabelTTFCocos::disableShadow(bool updateTexture) {
  label_->disableShadow(updateTexture);
}
void GuiPartsLabelTTFCocos::enableStroke(const TColor &strokeColor,
                                         float strokeSize,
                                         bool updateTexture) {
  cocos2d::ccColor3B color;
  color.r = strokeColor.r;
  color.g = strokeColor.g;
  color.b = strokeColor.b;
  label_->enableStroke(color, strokeSize, updateTexture);
}
void GuiPartsLabelTTFCocos::disableStroke(bool updateTexture) {
  label_->disableStroke(updateTexture);
}
void GuiPartsLabelTTFCocos::setFontFillColor(const TColor &tintColor,
                                             bool updateTexture) {
  cocos2d::ccColor3B color;
  color.r = tintColor.r;
  color.g = tintColor.g;
  color.b = tintColor.b;
  
  label_->setFontFillColor(color, updateTexture);
}
void GuiPartsLabelTTFCocos::setString(const char *label) {
  label_->setString(label);
}
const char* GuiPartsLabelTTFCocos::getString() {
  return label_->getString();
}
void GuiPartsLabelTTFCocos::getHorizontalAlignment(TTextAlignment &dst) {
  dst = (TTextAlignment)label_->getHorizontalAlignment();
}
void GuiPartsLabelTTFCocos::setHorizontalAlignment(TTextAlignment alignment) {
  cocos2d::CCTextAlignment cc_alignment = (cocos2d::CCTextAlignment)alignment;
  label_->setHorizontalAlignment(cc_alignment);
}
void GuiPartsLabelTTFCocos::getVerticalAlignment(TVerticalTextAlignment &dst) {
  dst = (TVerticalTextAlignment)label_->getVerticalAlignment();
}
void GuiPartsLabelTTFCocos::setVerticalAlignment(TVerticalTextAlignment alignment) {
  cocos2d::CCVerticalTextAlignment cc_alignment = (cocos2d::CCVerticalTextAlignment) alignment;
  
  label_->setVerticalAlignment(cc_alignment);
}
void GuiPartsLabelTTFCocos::getDimensions(TSize& dst) {
  cocos2d::CCSize cc_size = label_->getDimensions();
  dst.width = cc_size.width;
  dst.height = cc_size.height;
}
void GuiPartsLabelTTFCocos::setDimensions(const TSize &dim) {
  cocos2d::CCSize ccSize(dim.width, dim.height);
  label_->setDimensions(ccSize);
}
float GuiPartsLabelTTFCocos::getFontSize() {
  return label_->getFontSize();
}
void GuiPartsLabelTTFCocos::setFontSize(float fontSize) {
  label_->setFontSize(fontSize);
}
const char* GuiPartsLabelTTFCocos::getFontName() {
  return label_->getFontName();
}
void GuiPartsLabelTTFCocos::setFontName(const char *fontName) {
  label_->setFontName(fontName);
}

IGuiParts* GuiPartsLabelTTFCocos::GetCustomClass(const char* name) {
  ADK_ASSERT(false);
  return NULL;
}
IGuiParts* GuiPartsLabelTTFCocos::AttachCreateParts(const Char* partsname,
                                                  TGuiPartsType type) {
  ADK_ASSERT(false);
  return NULL;
}
Bool GuiPartsLabelTTFCocos::SetButtonCallback(const Char* name,
                                            TGuiPartsButtonCallback callback,
                                            void* userdata) {
  ADK_ASSERT(false);
  return false;
}
Bool GuiPartsLabelTTFCocos::SetMenuCallback(const Char* name,
                                          TGuiPartsMenuCallback callback,
                                          void* userdata) {
  ADK_ASSERT(false);
  return false;
}
void GuiPartsLabelTTFCocos::SetTouchPriority(S32 priority) { ADK_ASSERT(false); }
S32 GuiPartsLabelTTFCocos::GetTouchPriority() {
  ADK_ASSERT(false);
  return NULL;
}
void GuiPartsLabelTTFCocos::SetVisible(Bool visible) {
  label_->setVisible(visible);
}
Bool GuiPartsLabelTTFCocos::IsVisible() {
  return label_->isVisible();
}
void GuiPartsLabelTTFCocos::AddChild(IGuiParts* pChild) {
  cocos2d::CCNode* ccnode = (cocos2d::CCNode*)pChild->GetNode();
  label_->addChild(ccnode);
}
void GuiPartsLabelTTFCocos::AddChild(IGuiParts* pChild, int zOrder) {
  cocos2d::CCNode* ccnode = (cocos2d::CCNode*)pChild->GetNode();
  label_->addChild(ccnode, zOrder);
}
void GuiPartsLabelTTFCocos::AddChild(IGuiParts* pChild, int zOrder, int tag) {
  cocos2d::CCNode* ccnode = (cocos2d::CCNode*)pChild->GetNode();
  label_->addChild(ccnode, zOrder, tag);
}
Bool GuiPartsLabelTTFCocos::CreateWithFile(const Char* filename,
                                         const TGuiPartsInfo* parts,
                                         S32 parts_count) {
  ADK_ASSERT(false);
}
Bool GuiPartsLabelTTFCocos::Dispose() { ADK_ASSERT(false); }
void GuiPartsLabelTTFCocos::SetNode(void* node) {
  label_ = (cocos2d::CCLabelTTF*)node;
}
void* GuiPartsLabelTTFCocos::GetNode() { return label_; }


ADK_END_NAMESPACE
