/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_types.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツに関する型を定義します
 */

#ifndef GUI_PARTS_TYPES_H_
#define GUI_PARTS_TYPES_H_

#include <core/math/math_library.h>
#include <core/types.h>

ADK_BEGIN_NAMESPACE

/* TODO とりあえずの実装 */
typedef void Node;

/**
 * \~english
 * \~japanese
 *  \brief パーツ名の最大長
 */
static const int kGuiPartsNameLen = 63;
/**
 * \~english
 * \~japanese
 *  \brief パーツ内で構成されるパーツのタイプ定義
 */
typedef enum {
  kGuiPartsTypeLabelTTF,
  kGuiPartsTypeLayer,
  kGuiPartsTypeNode,
  kGuiPartsTypeButton,
  kGuiPartsTypeScrollView,
  kGuiPartsTypeTableView,
  kGuiPartsTypeTableViewCell,
  kGuiPartsTypeSprite,
  kGuiPartsTypeSpriteFrame,
  kGuiPartsTypeTexture
} TGuiPartsType;
/**
 * \~english
 * \~japanese
 *  \brief パーツ一つの情報
 */
typedef struct {
  char name[kGuiPartsNameLen + 1];
  TGuiPartsType type;
} TGuiPartsInfo;
/**
 * \~english
 * \~japanese
 *  \brief ボタンのコールバック定義　
 */
typedef void (*TGuiPartsButtonCallback)(void *userdata);
/**
 * \~english
 * \~japanese
 *  \brief メニューのコールバック定義　
 */
typedef void (*TGuiPartsMenuCallback)(void *userdata);
/**
 * \~english
 * \~japanese
 *  \brief テキストアライメント
 */
typedef enum {
  kVerticalTextAlignmentTop,
  kVerticalTextAlignmentCenter,
  kVerticalTextAlignmentBottom
} TVerticalTextAlignment;
typedef enum {
  kTextAlignmentLeft,
  kTextAlignmentCenter,
  kTextAlignmentRight
} TTextAlignment;
/**
 * \~english
 * \~japanese
 *  \brief フォント定義(ちょっとこの構造体はないなぁ...)
 */
typedef struct _TFontShadow {
 public:
  // shadow is not enabled by default
  _TFontShadow() : m_shadowEnabled(false) {}

  // true if shadow enabled
  bool m_shadowEnabled;
  // shadow x and y offset
  TSize m_shadowOffset;
  // shadow blurrines
  float m_shadowBlur;
  // shadow opacity
  float m_shadowOpacity;
} TFontShadow;
typedef struct _TFontStroke {
 public:
  // stroke is disabled by default
  _TFontStroke() : m_strokeEnabled(false) {}

  // true if stroke enabled
  bool m_strokeEnabled;
  // stroke color
  TColor m_strokeColor;
  // stroke size
  float m_strokeSize;
} TFontStroke;
static const S32 kFontNameLen = 256;
typedef struct _TFontDefinition {
 public:
  _TFontDefinition()
      : m_alignment(kTextAlignmentCenter),
        m_vertAlignment(kVerticalTextAlignmentTop) {
    m_dimensions.width = 0.0f;
    m_dimensions.height = 0.0f;
    m_fontFillColor.r = 255;
    m_fontFillColor.g = 255;
    m_fontFillColor.b = 255;
    m_fontFillColor.a = 255;
  }

  // font name
  char m_fontName[kFontNameLen];
  // font size
  int m_fontSize;
  // horizontal alignment
  TTextAlignment m_alignment;
  // vertical alignment
  TVerticalTextAlignment m_vertAlignment;
  // renering box
  TSize m_dimensions;
  // font color
  TColor m_fontFillColor;
  // font shadow
  TFontShadow m_shadow;
  // font stroke
  TFontStroke m_stroke;
} TFontDefinition;

ADK_END_NAMESPACE

#endif /* GUI_PARTS_TYPES_H_ */

