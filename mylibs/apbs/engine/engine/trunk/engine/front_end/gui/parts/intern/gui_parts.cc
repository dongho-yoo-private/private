/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief 「パーツ」はシーンに描画される画面上のオブジェクトです。
 */

#include <front_end/gui/parts/intern/gui_parts.h>

ADK_BEGIN_NAMESPACE

GuiParts::GuiParts()/*: parts_count_(0)*/ {}
GuiParts::~GuiParts() {}

#if 0
TGuiPartsType GuiParts::GetPartsType() const {
  return type_;
}
S32 GuiParts::GetPartsCount() const { return parts_count_; }
#endif

ADK_END_NAMESPACE

