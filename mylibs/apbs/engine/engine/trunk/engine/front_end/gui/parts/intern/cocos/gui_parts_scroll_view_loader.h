/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_file_loader.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief ファイルからパーツの内容を保持する共通ノードを生成します
 */

#ifndef PARTS_SCROLL_VIEW_LOADER_H_
#define PARTS_SCROLL_VIEW_LOADER_H_

#include <extensions/CCBReader/CCScrollViewLoader.h>

#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_cc_parts_scroll_view.h>

ADK_BEGIN_NAMESPACE

class PartsScrollViewLoader : public cocos2d::extension::CCScrollViewLoader {
 public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(PartsScrollViewLoader, loader);
  CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuiCcPartsScrollView);
};

ADK_END_NAMESPACE

#endif /* PARTS_SCROLL_VIEW_LOADER_H_ */
