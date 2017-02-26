/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_common_loader.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief ファイルからパーツの内容を保持する共通ノードを生成します
 */

#ifndef PARTS_BUTTON_LOADER_H_
#define PARTS_BUTTON_LOADER_H_

#include <extensions/CCBReader/CCControlButtonLoader.h>

#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_button.h>

ADK_BEGIN_NAMESPACE

class PartsControlButtonLoader
    : public cocos2d::extension::CCControlButtonLoader {
 public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(PartsControlButtonLoader, loader);
  CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(PartsButton);
};

ADK_END_NAMESPACE

#endif /* PARTS_BUTTON_LOADER_H_ */
