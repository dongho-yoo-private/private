/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file i_gui_parts.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief スクロールビューに関する定義部
 */

#ifndef GUI_PARTS_SCROLL_VIEW_TYPES_H_
#define GUI_PARTS_SCROLL_VIEW_TYPES_H_

#include <core/types.h>

ADK_BEGIN_NAMESPACE

typedef enum {
  kCCScrollViewDirectionNone = -1,
  kCCScrollViewDirectionHorizontal = 0,
  kCCScrollViewDirectionVertical,
  kCCScrollViewDirectionBoth
} TScrollViewDirection;

ADK_END_NAMESPACE

#endif /* GUI_PARTS_SCROLL_VIEW_TYPES_H_ */
