/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_table_view_cocos.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief テーブルビューパーツクラス定義部 for Cocos2d-x
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_table_view_cocos.h>

#include <front_end/gui/parts/i_gui_parts_node.h>
#include <front_end/gui/parts/i_gui_parts_scroll_view.h>

ADK_BEGIN_NAMESPACE

/** 臨時的に */
static GuiPartsTableViewCocos table_view_;

#if 0 // 破棄
GuiPartsTableViewCellCocos::GuiPartsTableViewCellCocos() : idx_(CC_INVALID_INDEX), cocos_(0){}
GuiPartsTableViewCellCocos::~GuiPartsTableViewCellCocos() {}

unsigned int GuiPartsTableViewCellCocos::GetIdx() {return idx_;}
void GuiPartsTableViewCellCocos::SetIdx(unsigned int idx){idx_ = idx;}
void GuiPartsTableViewCellCocos::Reset(){idx_ = CC_INVALID_INDEX;}
void GuiPartsTableViewCellCocos::SetObjectID(unsigned int idx){idx_ = idx;}
unsigned int GuiPartsTableViewCellCocos::GetObjectID(){return idx_;}
void GuiPartsTableViewCellCocos:.SetCocos(const void* cocos){cocos_= (void*)cocos;}
void* GuiPartsTableViewCellCocos::GetNode(){return cocos_;}

#endif

TableViewDelegate::TableViewDelegate() : delegate_(NULL) {
}
TableViewDelegate::~TableViewDelegate() {
}
void TableViewDelegate::scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {
}
void TableViewDelegate::scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {
}
void TableViewDelegate::tableCellTouched(
    cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell) {
  GuiPartsTableViewCellCocos _cell;
  _cell.SetIdx(cell->getIdx());
  _cell.SetObjectID(cell->getObjectID());
  _cell.SetCocos(cell);

  table_view_.SetCocos(table);
  delegate_->TableCellTouched((IGuiPartsTableView*)&table_view_, &_cell);
}
void TableViewDelegate::tableCellHighlight(
    cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell) {
   GuiPartsTableViewCellCocos _cell;
  _cell.SetIdx(cell->getIdx());
  _cell.SetObjectID(cell->getObjectID());

  table_view_.SetCocos(table);
  delegate_->TableCellHighlight((IGuiPartsTableView*)&table_view_, &_cell);
}
void TableViewDelegate::tableCellUnhighlight(
    cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell) {
   GuiPartsTableViewCellCocos _cell;
  _cell.SetIdx(cell->getIdx());
  _cell.SetObjectID(cell->getObjectID());

  table_view_.SetCocos(table);
  delegate_->TableCellUnhighlight((IGuiPartsTableView*)&table_view_, &_cell);
}
void TableViewDelegate::tableCellWillRecycle(
    cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell) {
   GuiPartsTableViewCellCocos _cell;
  _cell.SetIdx(cell->getIdx());
  _cell.SetObjectID(cell->getObjectID());

  table_view_.SetCocos(table);
  delegate_->TableCellWillRecycle((IGuiPartsTableView*)&table_view_, &_cell);
}
TableViewDataSource::TableViewDataSource() : data_source_(NULL) {
}
TableViewDataSource::~TableViewDataSource() {
}
cocos2d::CCSize TableViewDataSource::tableCellSizeForIndex(
    cocos2d::extension::CCTableView *table, unsigned int idx) {
  
  table_view_.SetCocos(table);
  TSize size = data_source_->TableCellSizeForIndex((IGuiPartsTableView*)&table_view_ ,idx);
  return cocos2d::CCSize(size.width,size.height);
}
cocos2d::CCSize TableViewDataSource::cellSizeForTable(cocos2d::extension::CCTableView *table) {
  TSize size = data_source_->CellSizeForTable(0);
  return cocos2d::CCSize(size.width,size.height);
}
cocos2d::extension::CCTableViewCell* TableViewDataSource::tableCellAtIndex(
    cocos2d::extension::CCTableView *table, unsigned int idx) {
  
  table_view_.SetCocos(table);
  GuiPartsTableViewCellCocos* cell = (GuiPartsTableViewCellCocos*)data_source_->TableCellAtIndex((IGuiPartsTableView*)&table_view_, idx);
  
  if (cell!=0)
  {
    return (cocos2d::extension::CCTableViewCell*)cell->GetCocos();
  }
  return 0;
}
unsigned int TableViewDataSource::numberOfCellsInTableView(cocos2d::extension::CCTableView *table) {
  table_view_.SetCocos(table);
  return data_source_->NumberOfCellsInTableView((IGuiPartsTableView*)&table_view_);
}

GuiPartsTableViewCocos::GuiPartsTableViewCocos():table_view_(NULL) {
}
GuiPartsTableViewCocos::~GuiPartsTableViewCocos() {
}

Bool GuiPartsTableViewCocos::Init()
{
  Bool ret = true;
  return ret;
}

Bool GuiPartsTableViewCocos::InitWithSize(IGuiTableViewDataSource* data_source, const TSize& size) {
  Bool ret = true;

  table_view_ = new cocos2d::extension::CCTableView;
  
  ADK_ASSERT(table_view_);
  if (table_view_==NULL) {
    ret = false;
  } else {
    data_source_.SetDataSource(data_source);
    table_view_->initWithViewSize(cocos2d::CCSize(size.width, size.height));
    table_view_->setDataSource(&data_source_);
    table_view_->autorelease();
    table_view_->UpdateInitialize();
  }
  return ret;
}



IGuiTableViewDataSource* GuiPartsTableViewCocos::GetDataSource() {
  return data_source_.GetDataSource();
}
void GuiPartsTableViewCocos::SetDataSource(IGuiTableViewDataSource* source) {
  data_source_.SetDataSource(source);
}
IGuiTableViewDelegate* GuiPartsTableViewCocos::GetDelegate() {
  return delegate_.GetDelegate();
}
void GuiPartsTableViewCocos::SetDelegate(IGuiTableViewDelegate* pDelegate) {
  delegate_.SetDelegate(pDelegate);
  table_view_->setDelegate(&delegate_);
}
void GuiPartsTableViewCocos::SetVerticalFillOrder(TGuiTableViewVerticalFillOrder order) {
  table_view_->setVerticalFillOrder((cocos2d::extension::CCTableViewVerticalFillOrder)order);
}
TGuiTableViewVerticalFillOrder GuiPartsTableViewCocos::GetVerticalFillOrder() {
  return (TGuiTableViewVerticalFillOrder)table_view_->getVerticalFillOrder();
}
bool GuiPartsTableViewCocos::InitWithViewSize(TSize &size, IGuiPartsNode* container) {
  cocos2d::CCSize sz(size.width,size.height);
  cocos2d::CCNode *ccnode = (cocos2d::CCNode*)container->GetNode();
  return table_view_->initWithViewSize(sz,ccnode);
}
void GuiPartsTableViewCocos::UpdateCellAtIndex(unsigned int idx) {
  table_view_->updateCellAtIndex(idx);
}
void GuiPartsTableViewCocos::InsertCellAtIndex(unsigned int idx) {
  table_view_->insertCellAtIndex(idx);
}
void GuiPartsTableViewCocos::RemoveCellAtIndex(unsigned int idx) {
  table_view_->removeCellAtIndex(idx);
}
void GuiPartsTableViewCocos::ReloadData() {
  table_view_->reloadData();
}
IGuiPartsTableViewCell *GuiPartsTableViewCocos::DequeueCell() {
  cocos2d::extension::CCTableViewCell *cccell = table_view_->dequeueCell();
  if (cccell!=0)
  {
    cell_.SetIdx(cccell->getIdx());
    cell_.SetCocos(cccell);
    return &cell_;
  }
  return 0;
}
IGuiPartsTableViewCell* GuiPartsTableViewCocos::CellAtIndex(unsigned int idx) {
  cocos2d::extension::CCTableViewCell *cccell = table_view_->cellAtIndex(idx);
  cell_.SetIdx(cccell->getIdx());
  cell_.SetCocos(cccell);
  return &cell_;
}
void GuiPartsTableViewCocos::ScrollViewDidScroll(IGuiPartsScrollView* view) {
  table_view_->scrollViewDidScroll((cocos2d::extension::CCScrollView*)view->GetNode());
}
void GuiPartsTableViewCocos::ScrollViewDidZoom(IGuiPartsScrollView* view) {
  table_view_->scrollViewDidZoom((cocos2d::extension::CCScrollView*)view->GetNode());
}
bool GuiPartsTableViewCocos::TouchBegan(GuiTouch *pTouch, GuiEvent *pEvent) {
  ADK_ASSERT(false);
  return false;
}
void GuiPartsTableViewCocos::TouchMoved(GuiTouch *pTouch, GuiEvent *pEvent) {
  ADK_ASSERT(false);
}
void GuiPartsTableViewCocos::TouchEnded(GuiTouch *pTouch, GuiEvent *pEvent) {
  ADK_ASSERT(false);
}
void GuiPartsTableViewCocos::TouchCancelled(GuiTouch *pTouch, GuiEvent *pEvent) {
  ADK_ASSERT(false);
}

IGuiParts *GuiPartsTableViewCocos::GetCustomClass(const char *name) {
  ADK_ASSERT(false);
  return NULL;
}
IGuiParts *GuiPartsTableViewCocos::AttachCreateParts(const Char *partsname,
                             TGuiPartsType type) {
  ADK_ASSERT(false);
  return NULL;
}
Bool GuiPartsTableViewCocos::SetButtonCallback(const Char *name,
                       TGuiPartsButtonCallback callback,
                       void *userdata) {
  ADK_ASSERT(false);
  return false;
}
Bool GuiPartsTableViewCocos::SetMenuCallback(const Char *name, TGuiPartsMenuCallback callback,
                     void *userdata) {
  ADK_ASSERT(false);
  return false;
}
void GuiPartsTableViewCocos::SetTouchPriority(S32 priority) { table_view_->setTouchPriority(priority); }
S32 GuiPartsTableViewCocos::GetTouchPriority() { return table_view_->getTouchPriority(); }
void GuiPartsTableViewCocos::SetVisible(Bool visible) {table_view_->setVisible(visible);}
Bool GuiPartsTableViewCocos::IsVisible() {return table_view_->isVisible();}
void GuiPartsTableViewCocos::AddChild(IGuiParts *child) {
  cocos2d::CCNode *node = (cocos2d::CCNode*)child->GetNode();
  table_view_->addChild(node);
}
void GuiPartsTableViewCocos::AddChild(IGuiParts *child, S32 zOrder) {
  cocos2d::CCNode *node = (cocos2d::CCNode*)child->GetNode();
  table_view_->addChild(node,zOrder);
}
void GuiPartsTableViewCocos::AddChild(IGuiParts *child, S32 zOrder, S32 tag) {
  cocos2d::CCNode *node = (cocos2d::CCNode*)child->GetNode();
  table_view_->addChild(node,zOrder,tag);
}
Bool GuiPartsTableViewCocos::CreateWithFile(const Char *filename, const TGuiPartsInfo *parts,
                    S32 parts_count) {ADK_ASSERT(false);}
Bool GuiPartsTableViewCocos::Dispose() {ADK_ASSERT(false);}
void GuiPartsTableViewCocos::SetNode(void *node) {table_view_ = (cocos2d::extension::CCTableView*)node;}
void* GuiPartsTableViewCocos::GetNode() {return table_view_;}

ADK_END_NAMESPACE

