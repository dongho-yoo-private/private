//
//  gui_parts_table_view_cell_cocos.cc
//  af-engine
//
//  Created by CJ0048 on 2014/04/15.
//  Copyright (c) 2014年 B04350. All rights reserved.
//

#include <front_end/gui/parts/intern/cocos/gui_parts_table_view_cell_cocos.h>
#include <support/data_support/ccCArray.h>

ADK_BEGIN_NAMESPACE

GuiPartsTableViewCellCocos::GuiPartsTableViewCellCocos() : idx_(CC_INVALID_INDEX){}

GuiPartsTableViewCellCocos::~GuiPartsTableViewCellCocos() {}

Bool GuiPartsTableViewCellCocos::Init() {
  Bool ret = true;
  
  cell_ = new cocos2d::extension::CCTableViewCell;
  
  ADK_ASSERT(cell_);
  if (cell_ == NULL) {
    ret = false;
  } else {
    if (cell_->init() == false) {
      delete cell_, cell_ = 0;
    } else {
#if 0
      cell_->autorelease();
#endif
    }
  }
  
  return ret;
}

unsigned int GuiPartsTableViewCellCocos::GetIdx()
{
  return idx_;
}

void GuiPartsTableViewCellCocos::SetIdx(unsigned int idx)
{
  idx_ = idx;
}

void GuiPartsTableViewCellCocos::Reset()
{
  idx_ = CC_INVALID_INDEX;
}

void GuiPartsTableViewCellCocos::SetObjectID(unsigned int idx)
{
  idx_ = idx;
}

unsigned int GuiPartsTableViewCellCocos::GetObjectID()
{
  return idx_;
}

IGuiParts *GuiPartsTableViewCellCocos::GetCustomClass(const char *name) {
  ADK_ASSERT(false);
  return NULL;
}
IGuiParts *GuiPartsTableViewCellCocos::AttachCreateParts(const Char *partsname,
                             TGuiPartsType type) {
  ADK_ASSERT(false);
  return NULL;
}
Bool GuiPartsTableViewCellCocos::SetButtonCallback(const Char *name,
                       TGuiPartsButtonCallback callback,
                       void *userdata) {
  ADK_ASSERT(false);
  return false;
}
Bool GuiPartsTableViewCellCocos::SetMenuCallback(const Char *name, TGuiPartsMenuCallback callback,
                     void *userdata) {
  ADK_ASSERT(false);
  return false;
}

Bool GuiPartsTableViewCellCocos::CreateWithFile(const Char *filename, const TGuiPartsInfo *parts,
                    S32 parts_count) {ADK_ASSERT(false);}
Bool GuiPartsTableViewCellCocos::Dispose() {ADK_ASSERT(false);}
void GuiPartsTableViewCellCocos::SetNode(void *node) {cell_= (cocos2d::extension::CCTableViewCell*)node;}
void* GuiPartsTableViewCellCocos::GetNode() {return cell_;}



ADK_END_NAMESPACE