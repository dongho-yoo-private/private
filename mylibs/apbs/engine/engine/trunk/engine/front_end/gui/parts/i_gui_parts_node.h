/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file i_gui_parts_node.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief ノードクラス定義部
 */

#ifndef I_GUI_PARTS_NODE_H_
#define I_GUI_PARTS_NODE_H_

#include <core/math/math_library.h>
#include <core/types.h>
#include <front_end/gui/parts/gui_parts_types.h>
#include <front_end/gui/parts/i_gui_parts.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class GuiPartsSystem;

/**
 * \~english
 * \~japanese
 *  \brief ノードパーツクラス
 */
class IGuiPartsNode : public IGuiParts {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~IGuiPartsNode();
};

ADK_END_NAMESPACE

#endif /* I_GUI_PARTS_NODE_H_ */
