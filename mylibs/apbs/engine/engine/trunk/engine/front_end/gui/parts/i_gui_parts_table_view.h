/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file i_gui_parts_table_view.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief テーブルビュークラス定義部
 */

#ifndef I_GUI_PARTS_TABLE_VIEW_H_
#define I_GUI_PARTS_TABLE_VIEW_H_

#include <core/math/math_library.h>
#include <core/types.h>
#include <front_end/gui/parts/gui_parts_types.h>
#include <front_end/gui/parts/i_gui_parts.h>
#include <front_end/gui/parts/i_gui_parts_table_view_cell.h>
#include <llr/llr_types.h>

ADK_BEGIN_NAMESPACE

class IGuiPartsNode;
class IGuiPartsScrollView;
class GuiTouch;
class GuiEvent;
class GuiTableView;
class IGuiPartsTableViewCell;
class IGuiPartsTableView;

typedef enum {
  kTableViewFillTopDown,
  kTableViewFillBottomUp
} TGuiTableViewVerticalFillOrder;

// 廃止
#if 0
class GuiTableViewCell {
 public:
  GuiTableViewCell();
  virtual ~GuiTableViewCell();

  unsigned int GetIdx();
  void SetIdx(unsigned int idx);
  void Reset();
  void SetObjectID(unsigned int idx);
  unsigned int GetObjectID();
  void SetCocos(const void* cocos);
  void* GetCocos();
  
  
  
 private:
  unsigned int idx_;
  void* cocos_;
};
#endif


class IGuiTableViewDelegate {
 public:
  virtual void TableCellTouched(IGuiPartsTableView* table, IGuiPartsTableViewCell* cell) = 0;
  virtual void TableCellHighlight(IGuiPartsTableView* table, IGuiPartsTableViewCell* cell) = 0;
  virtual void TableCellUnhighlight(IGuiPartsTableView* table, IGuiPartsTableViewCell* cell) = 0;
  virtual void TableCellWillRecycle(IGuiPartsTableView* table, IGuiPartsTableViewCell* cell) = 0;
};
class IGuiTableViewDataSource {
 public:
  virtual ~IGuiTableViewDataSource() {}
  virtual TSize TableCellSizeForIndex(IGuiPartsTableView*table, unsigned int idx) = 0;
  virtual TSize CellSizeForTable(IGuiPartsTableView*table) = 0;
  virtual IGuiPartsTableViewCell* TableCellAtIndex(IGuiPartsTableView*table, unsigned int idx) = 0;
  virtual unsigned int NumberOfCellsInTableView(IGuiPartsTableView*table) = 0;
};
/**
 * \~english
 * \~japanese
 *  \brief テーブルビューパーツクラス
 */
class IGuiPartsTableView : public IGuiParts {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~IGuiPartsTableView();

  /*
   * Cocos2d-xのGuiTableViewラッパーメソッドたち
   */
  virtual IGuiTableViewDataSource* GetDataSource() = 0;
  virtual void SetDataSource(IGuiTableViewDataSource* source) = 0;
  virtual IGuiTableViewDelegate* GetDelegate() = 0;
  virtual void SetDelegate(IGuiTableViewDelegate* pDelegate) = 0;
  virtual void SetVerticalFillOrder(TGuiTableViewVerticalFillOrder order) = 0;
  virtual TGuiTableViewVerticalFillOrder GetVerticalFillOrder() = 0;
  virtual bool InitWithViewSize(TSize &size, IGuiPartsNode* container = NULL) = 0;
  virtual void UpdateCellAtIndex(unsigned int idx) = 0;
  virtual void InsertCellAtIndex(unsigned int idx) = 0;
  virtual void RemoveCellAtIndex(unsigned int idx) = 0;
  virtual void ReloadData() = 0;
  virtual IGuiPartsTableViewCell *DequeueCell() = 0;
  virtual IGuiPartsTableViewCell *CellAtIndex(unsigned int idx) = 0;
  virtual void ScrollViewDidScroll(IGuiPartsScrollView* view) = 0;
  virtual void ScrollViewDidZoom(IGuiPartsScrollView* view) = 0;
  virtual bool TouchBegan(GuiTouch *pTouch, GuiEvent *pEvent) = 0;
  virtual void TouchMoved(GuiTouch *pTouch, GuiEvent *pEvent) = 0;
  virtual void TouchEnded(GuiTouch *pTouch, GuiEvent *pEvent) = 0;
  virtual void TouchCancelled(GuiTouch *pTouch, GuiEvent *pEvent) = 0;
  virtual Bool InitWithSize(IGuiTableViewDataSource* data_source, const TSize& size) = 0;
};

ADK_END_NAMESPACE

#endif /* I_GUI_PARTS_TABLE_VIEW_H_ */
