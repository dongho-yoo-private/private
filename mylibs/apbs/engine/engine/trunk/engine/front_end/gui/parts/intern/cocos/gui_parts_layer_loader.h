/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_common_loader.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief ファイルからパーツの内容を保持する共通ノードを生成します
 */

#ifndef PARTS_LAYER_LOADER_H_
#define PARTS_LAYER_LOADER_H_

#include <extensions/CCBReader/CCLayerLoader.h>

#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_common_node_loader.h>

ADK_BEGIN_NAMESPACE

class PartsLayerLoader : public cocos2d::extension::CCLayerLoader {
 public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(PartsLayerLoader, loader);
#if 0 /* ADK側で追加処理があるのでマクロ展開して実装 for gui_parts_layer_loader.cc */
  virtual PartsCommonNodeLoader *createCCNode(
      cocos2d::CCNode *pParent, cocos2d::extension::CCBReader *pCCBReader);
#else
  CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(PartsCommonNodeLoader);
#endif
};

ADK_END_NAMESPACE

#endif /* PARTS_LAYER_LOADER_H_ */
