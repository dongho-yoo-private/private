/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_common_loader.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief ファイルからパーツの内容を保持する共通ノードを生成します
 */

#ifndef PARTS_NODE_LOADER_H_
#define PARTS_NODE_LOADER_H_

#include <extensions/CCBReader/CCNodeLoader.h>

#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_common_node_loader.h>

ADK_BEGIN_NAMESPACE

class PartsNodeLoader : public cocos2d::extension::CCLayerLoader {
 public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(PartsNodeLoader, loader);
  CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(PartsCommonNodeLoader);
};

ADK_END_NAMESPACE

#endif /* PARTS_NODE_LOADER_H_ */
