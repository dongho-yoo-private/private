/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file i_gui_parts_sprite_frame.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief スプライトフレームクラス定義部
 */

#ifndef I_GUI_PARTS_SPRITE_FRAME_H_
#define I_GUI_PARTS_SPRITE_FRAME_H_

#include <core/math/math_library.h>
#include <core/types.h>
#include <front_end/gui/parts/gui_parts_types.h>
#include <front_end/gui/parts/i_gui_parts.h>
#include <llr/llr_types.h>

ADK_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief スプライトパーツクラス
 */
class IGuiPartsSpriteFrame : public IGuiParts {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~IGuiPartsSpriteFrame();
};

ADK_END_NAMESPACE

#endif /* I_GUI_PARTS_SPRITE_FRAME_H_ */
