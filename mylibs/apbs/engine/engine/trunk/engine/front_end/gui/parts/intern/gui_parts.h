/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki & tanaka tomonori
 * \file parts.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief シーン中に描画される「パーツ」を表すクラス定義部
 */

#ifndef PARTS_OBJECT_PARTS_H_
#define PARTS_OBJECT_PARTS_H_

#include <front_end/gui/parts/i_gui_parts.h>
#if 0
#include <scene/system/parts/gui_parts_member.h>
#include <scene/system/parts/gui_parts_types.h>
#endif

ADK_BEGIN_NAMESPACE

/* forward declaration */
class PartsLoader;

/**
 * \~english
 * \~japanese
 *  \brief パーツ一つを管理するクラス
 */
class GuiParts : public IGuiParts {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  GuiParts();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~GuiParts();

#if 0
  /**
   * \~english
   * \~japanese
   *  \brief パーツのタイプ \ref TGuiPartsType を返します
   *  \return \ref TGuiPartsType
   */
  virtual TGuiPartsType GetPartsType() const;
  /**
   * \~english
   * \~japanese
   *  \brief パーツ内で管理するパーツ数を返します
   *  \return パーツ数
   */
  virtual S32 GetPartsCount() const;
#endif

 protected:
#if 0
  /**
   * \~english
   * \~japanese
   *  \brief パーツタイプを設定する
   *  \param[in] type \ref TGuiPartsType
   */
  ADK_INLINE void SetPartsType(TGuiPartsType type);

  /**
   * \~english
   * \~japanese
   *  \brief パーツタイプ
   */
  TGuiPartsType type_;
  /**
   * \~english
   * \~japanese
   *  \brief 実際に持っているパーツ数
   */
  S32 parts_count_;
#endif
};
#if 0
ADK_INLINE void GuiParts::SetPartsType(TGuiPartsType type) {
  type_ = type;
}
#endif

ADK_END_NAMESPACE

#endif /* defined(PARTS_OBJECT_PARTS_H_) */
