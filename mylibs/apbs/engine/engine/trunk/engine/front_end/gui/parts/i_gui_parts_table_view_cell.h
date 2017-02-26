//
//  i_gui_parts_table_view_cell.h
//  af-engine
//
//  Created by CJ0048 on 2014/04/15.
//  Copyright (c) 2014年 B04350. All rights reserved.
//

#ifndef __af_engine__i_gui_parts_table_view_cell__
#define __af_engine__i_gui_parts_table_view_cell__

#include <core/math/math_library.h>
#include <core/types.h>
#include <front_end/gui/parts/gui_parts_types.h>
#include <front_end/gui/parts/i_gui_parts.h>
#include <llr/llr_types.h>

ADK_BEGIN_NAMESPACE


class IGuiPartsTableViewCell : public IGuiParts {
public:
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~IGuiPartsTableViewCell();
 
  /*
   * Cocos2d-xのCCTableViewCellラッパーメソッドたち
   */
  virtual unsigned int GetIdx() = 0;
  virtual void SetIdx(unsigned int idx) = 0;
  virtual void Reset() = 0;
  virtual void SetObjectID(unsigned int idx) = 0;
  virtual unsigned int GetObjectID() = 0;
};

ADK_END_NAMESPACE

#endif /* defined(__af_engine__i_gui_parts_table_view_cell__) */
