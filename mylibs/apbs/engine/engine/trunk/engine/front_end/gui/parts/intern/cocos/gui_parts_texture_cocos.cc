/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_texture_cocos.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief テクスチャパーツクラス定義部 for Cocos2d-x
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_texture_cocos.h>

ADK_BEGIN_NAMESPACE

GuiPartsTextureCocos::GuiPartsTextureCocos() : texture_(NULL) {
#if 0
  SetPartsType(kGuiPartsTypeTexture);
#endif
}
GuiPartsTextureCocos::~GuiPartsTextureCocos() {}
Bool GuiPartsTextureCocos::Init() {
  Bool ret = true;

  texture_ = new cocos2d::CCTexture2D();
  ADK_ASSERT(texture_);
  if (texture_ == NULL) {
    ret = false;
  } else {
  }

  return ret;
}
const Char *GuiPartsTextureCocos::Description(void) {
  ADK_ASSERT(texture_);
  return texture_->description();
}
void GuiPartsTextureCocos::ReleaseData(void *data) {
  ADK_ASSERT(texture_);
  texture_->releaseData(data);
}
void *GuiPartsTextureCocos::KeepData(void *data, unsigned int length) {
  ADK_ASSERT(texture_);
  return texture_->keepData(data, length);
}
Bool GuiPartsTextureCocos::InitWithData(const void *data,
                                        TTexturePixelFormat pixelFormat,
                                        unsigned int pixelsWide,
                                        unsigned int pixelsHigh,
                                        const TSize &contentSize) {
  ADK_ASSERT(texture_);
  cocos2d::CCSize size(contentSize.width, contentSize.height);
  Bool ret =
      texture_->initWithData(data, (cocos2d::CCTexture2DPixelFormat)pixelFormat,
                             pixelsWide, pixelsHigh, size);
  return ret;
}
void GuiPartsTextureCocos::DrawAtPoint(const TVector2 &point) {
  ADK_ASSERT(texture_);
  cocos2d::CCPoint pt(point.x, point.y);
  texture_->drawAtPoint(pt);
}
void GuiPartsTextureCocos::DrawInRect(const TRect &rect) {
  ADK_ASSERT(texture_);
  cocos2d::CCRect rectangle(rect.left, rect.top, rect.width, rect.height);
  texture_->drawInRect(rectangle);
}
Bool GuiPartsTextureCocos::InitWithImage(IGuiImage *uiImage) {
  ADK_ASSERT(texture_);
#if 0
  cocos2d::CCImage *ccimage = ((GuiImageCocos*)uiImage)->GetCCImage();
  return texture_->initWithImage();
#else
  return false;
#endif
}
Bool GuiPartsTextureCocos::InitWithString(const Char *text,
                                          const Char *fontName, float fontSize,
                                          const TSize &dimensions,
                                          TTextAlignment hAlignment,
                                          TVerticalTextAlignment vAlignment) {
  ADK_ASSERT(texture_);
  cocos2d::CCSize dimensions_size(dimensions.width, dimensions.height);
  return texture_->initWithString(text, fontName, fontSize, dimensions_size,
                                  (cocos2d::CCTextAlignment)hAlignment,
                                  (cocos2d::CCVerticalTextAlignment)vAlignment);
}
Bool GuiPartsTextureCocos::InitWithString(const Char *text,
                                          const Char *fontName,
                                          float fontSize) {
  ADK_ASSERT(texture_);
  return texture_->initWithString(text, fontName, fontSize);
}
Bool GuiPartsTextureCocos::InitWithString(const Char *text,
                                          TFontDefinition *textDefinition) {
  ADK_ASSERT(texture_);
  /*
   * こういった ADK to Cocos2d-x のパラメータ変換が多様されるようなら、
   * どこかに関数定義する。cocos_support.hとか作る？
   */
  cocos2d::ccFontDefinition definition;
  definition.m_fontName = textDefinition->m_fontName;
  definition.m_fontSize = textDefinition->m_fontSize;
  definition.m_alignment =
      (cocos2d::CCTextAlignment)textDefinition->m_alignment;
  definition.m_vertAlignment =
      (cocos2d::CCVerticalTextAlignment)textDefinition->m_vertAlignment;
  definition.m_dimensions.width = textDefinition->m_dimensions.width;
  definition.m_dimensions.height = textDefinition->m_dimensions.height;
  definition.m_fontFillColor.r = textDefinition->m_fontFillColor.r;
  definition.m_fontFillColor.g = textDefinition->m_fontFillColor.g;
  definition.m_fontFillColor.b = textDefinition->m_fontFillColor.b;
  definition.m_shadow.m_shadowEnabled =
      textDefinition->m_shadow.m_shadowEnabled;
  definition.m_shadow.m_shadowOffset.width =
      textDefinition->m_shadow.m_shadowOffset.width;
  definition.m_shadow.m_shadowOffset.height =
      textDefinition->m_shadow.m_shadowOffset.height;
  definition.m_shadow.m_shadowBlur = textDefinition->m_shadow.m_shadowBlur;
  definition.m_shadow.m_shadowOpacity =
      textDefinition->m_shadow.m_shadowOpacity;
  definition.m_stroke.m_strokeEnabled =
      textDefinition->m_stroke.m_strokeEnabled;
  definition.m_stroke.m_strokeColor.r =
      textDefinition->m_stroke.m_strokeColor.r;
  definition.m_stroke.m_strokeColor.g =
      textDefinition->m_stroke.m_strokeColor.g;
  definition.m_stroke.m_strokeColor.b =
      textDefinition->m_stroke.m_strokeColor.b;
  definition.m_stroke.m_strokeSize = textDefinition->m_stroke.m_strokeSize;
  return texture_->initWithString(text, &definition);
}
Bool GuiPartsTextureCocos::InitWithPVRFile(const Char *file) {
  ADK_ASSERT(texture_);
  return texture_->initWithPVRFile(file);
}
Bool GuiPartsTextureCocos::InitWithETCFile(const Char *file) {
  ADK_ASSERT(texture_);
  return texture_->initWithETCFile(file);
}
void GuiPartsTextureCocos::SetTexParameters(TTexParams *texParams) {
  ADK_ASSERT(texture_);
  cocos2d::ccTexParams params;
  params.minFilter = texParams->minFilter;
  params.magFilter = texParams->magFilter;
  params.wrapS = texParams->wrapS;
  params.wrapT = texParams->wrapT;
  return texture_->setTexParameters(&params);
}
void GuiPartsTextureCocos::SetAntiAliasTexParameters() {
  ADK_ASSERT(texture_);
  texture_->setAntiAliasTexParameters();
}
void GuiPartsTextureCocos::SetAliasTexParameters() {
  ADK_ASSERT(texture_);
  texture_->setAliasTexParameters();
}
void GuiPartsTextureCocos::GenerateMipmap() {
  ADK_ASSERT(texture_);
  texture_->generateMipmap();
}
const Char *GuiPartsTextureCocos::StringForFormat() {
  ADK_ASSERT(texture_);
  return texture_->stringForFormat();
}
unsigned int GuiPartsTextureCocos::BitsPerPixelForFormat() {
  ADK_ASSERT(texture_);
  return texture_->bitsPerPixelForFormat();
}
unsigned int GuiPartsTextureCocos::BitsPerPixelForFormat(
    TTexturePixelFormat format) {
  ADK_ASSERT(texture_);
  return texture_->bitsPerPixelForFormat();
}
void GuiPartsTextureCocos::GetContentSizeInPixels(TSize &dst) {
  ADK_ASSERT(texture_);
  const cocos2d::CCSize size = texture_->getContentSizeInPixels();
  dst.width = size.width;
  dst.height = size.height;
}
Bool GuiPartsTextureCocos::HasPremultipliedAlpha() {
  ADK_ASSERT(texture_);
  return texture_->hasPremultipliedAlpha();
}
Bool GuiPartsTextureCocos::HasMipmaps() {
  ADK_ASSERT(texture_);
  return texture_->hasMipmaps();
}

IGuiParts *GuiPartsTextureCocos::GetCustomClass(const char *name) {
  ADK_ASSERT(false);
  return NULL;
}
IGuiParts *GuiPartsTextureCocos::AttachCreateParts(const Char *partsname,
                                                   TGuiPartsType type) {
  ADK_ASSERT(false);
  return NULL;
}
Bool GuiPartsTextureCocos::SetButtonCallback(const Char *name,
                                             TGuiPartsButtonCallback callback,
                                             void *userdata) {
  ADK_ASSERT(false);
  return false;
}
Bool GuiPartsTextureCocos::SetMenuCallback(const Char *name,
                                           TGuiPartsMenuCallback callback,
                                           void *userdata) {
  ADK_ASSERT(false);
  return false;
}
void GuiPartsTextureCocos::SetTouchPriority(S32 priority) { ADK_ASSERT(false); }
S32 GuiPartsTextureCocos::GetTouchPriority() {
  ADK_ASSERT(false);
  return NULL;
}
void GuiPartsTextureCocos::SetVisible(Bool visible) {
#if 1
  ADK_ASSERT(false);
#else
  texture_->setVisible(visible);
#endif
}
Bool GuiPartsTextureCocos::IsVisible() {
#if 1
  ADK_ASSERT(false);
  return false;
#else
  return texture_->isVisible();
#endif
}
void GuiPartsTextureCocos::AddChild(IGuiParts *pChild) {
#if 1
  ADK_ASSERT(false);
#else
  cocos2d::CCTexture2D *ccnode = (cocos2d::CCTexture2D *)pChild->GetNode();
  texture_->addChild(ccnode);
#endif
}
void GuiPartsTextureCocos::AddChild(IGuiParts *pChild, int zOrder) {
#if 1
  ADK_ASSERT(false);
#else
  cocos2d::CCTexture2D *ccnode = (cocos2d::CCTexture2D *)pChild->GetNode();
  texture_->addChild(ccnode, zOrder);
#endif
}
void GuiPartsTextureCocos::AddChild(IGuiParts *pChild, int zOrder, int tag) {
#if 1
  ADK_ASSERT(false);
#else
  cocos2d::CCTexture2D *ccnode = (cocos2d::CCTexture2D *)pChild->GetNode();
  texture_->addChild(ccnode, zOrder, tag);
#endif
}
Bool GuiPartsTextureCocos::CreateWithFile(const Char *filename,
                                          const TGuiPartsInfo *parts,
                                          S32 parts_count) {
  ADK_ASSERT(false);
}
Bool GuiPartsTextureCocos::Dispose() { ADK_ASSERT(false); }
void GuiPartsTextureCocos::SetNode(void *node) {
  texture_ = (cocos2d::CCTexture2D *)node;
}
void *GuiPartsTextureCocos::GetNode() { return texture_; }

ADK_END_NAMESPACE

