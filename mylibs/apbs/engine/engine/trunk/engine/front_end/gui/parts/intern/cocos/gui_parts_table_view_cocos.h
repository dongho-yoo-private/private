/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_table_view.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief テーブルビューパーツクラス定義部 for Cocos2d-x
 */

#ifndef GUI_PARTS_TABLE_VIEW_COCOS_H_
#define GUI_PARTS_TABLE_VIEW_COCOS_H_

#include <extensions/GUI/CCScrollView/CCTableView.h>

#include <core/math/math_library.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>
#include <front_end/gui/parts/i_gui_parts_sprite.h>
#include <front_end/gui/parts/i_gui_parts_table_view.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_table_view_cell_cocos.h>

ADK_BEGIN_NAMESPACE


class TableViewDelegate : public cocos2d::extension::CCTableViewDelegate {
 public:
  TableViewDelegate();
  virtual ~TableViewDelegate();

  virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
  virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);

  virtual void tableCellTouched(
      cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
  virtual void tableCellHighlight(
      cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
  virtual void tableCellUnhighlight(
      cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
  virtual void tableCellWillRecycle(
      cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);

  ADK_INLINE void SetDelegate(IGuiTableViewDelegate *delegate);
  ADK_INLINE IGuiTableViewDelegate *GetDelegate() const;
 private:
  IGuiTableViewDelegate *delegate_;
};
ADK_INLINE void TableViewDelegate::SetDelegate(IGuiTableViewDelegate *delegate) {
  delegate_ = delegate;
}
ADK_INLINE IGuiTableViewDelegate *TableViewDelegate::GetDelegate() const {
  return delegate_;
}
class TableViewDataSource : public cocos2d::extension::CCTableViewDataSource {
 public:
  TableViewDataSource();
  virtual ~TableViewDataSource();
  virtual cocos2d::CCSize tableCellSizeForIndex(
      cocos2d::extension::CCTableView *table, unsigned int idx);
  virtual cocos2d::CCSize cellSizeForTable(
      cocos2d::extension::CCTableView *table);
  virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(
      cocos2d::extension::CCTableView *table, unsigned int idx);
  virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

  ADK_INLINE void SetDataSource(IGuiTableViewDataSource *data_source);
  ADK_INLINE IGuiTableViewDataSource *GetDataSource() const;
 private:
  IGuiTableViewDataSource *data_source_;
};
ADK_INLINE void TableViewDataSource::SetDataSource(IGuiTableViewDataSource *data_source) {
  data_source_ = data_source;
}
ADK_INLINE IGuiTableViewDataSource *TableViewDataSource::GetDataSource() const {
  return data_source_;
}
/**
 * \~english
 * \~japanese
 *  \brief テーブルビューパーツクラス
 */
class GuiPartsTableViewCocos : public IGuiPartsTableView {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  GuiPartsTableViewCocos();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~GuiPartsTableViewCocos();
  /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiPartsTableViewCocos オブジェクトの初期化処理を行います
   *
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval true 成功したときに返ります
   *  \retval false 失敗したときに返ります
  */
  virtual Bool Init();
   /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiPartsTableViewCocos オブジェクトの初期化処理を行います
   *  \param data_source \ref IGuiTableViewDataSource
   *  \param size 初期化時のサイズ
   *
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval true 成功したときに返ります
   *  \retval false 失敗したときに返ります
   */
  virtual Bool InitWithSize(IGuiTableViewDataSource* data_source, const TSize& size);

  /*
   * Cocos2d-xのGuiTableViewラッパーメソッドたち
   */
  virtual IGuiTableViewDataSource* GetDataSource();
  virtual void SetDataSource(IGuiTableViewDataSource* source);
  virtual IGuiTableViewDelegate* GetDelegate();
  virtual void SetDelegate(IGuiTableViewDelegate* pDelegate);
  virtual void SetVerticalFillOrder(TGuiTableViewVerticalFillOrder order);
  virtual TGuiTableViewVerticalFillOrder GetVerticalFillOrder();
  virtual bool InitWithViewSize(TSize &size, IGuiPartsNode* container = NULL);
  virtual void UpdateCellAtIndex(unsigned int idx);
  virtual void InsertCellAtIndex(unsigned int idx);
  virtual void RemoveCellAtIndex(unsigned int idx);
  virtual void ReloadData();
  virtual IGuiPartsTableViewCell *DequeueCell();
  virtual IGuiPartsTableViewCell *CellAtIndex(unsigned int idx);
  virtual void ScrollViewDidScroll(IGuiPartsScrollView* view);
  virtual void ScrollViewDidZoom(IGuiPartsScrollView* view);
  virtual bool TouchBegan(GuiTouch *pTouch, GuiEvent *pEvent);
  virtual void TouchMoved(GuiTouch *pTouch, GuiEvent *pEvent);
  virtual void TouchEnded(GuiTouch *pTouch, GuiEvent *pEvent);
  virtual void TouchCancelled(GuiTouch *pTouch, GuiEvent *pEvent);


  /**
  * \brief テーブルビューのデリゲート対応のため
  */
  ADK_INLINE void SetCocos(cocos2d::extension::CCTableView*table_view)
  {
    table_view_=table_view;
  }
  /**
  * \brief テーブルビューのデリゲート対応のため
  */
  ADK_INLINE cocos2d::extension::CCTableView* GetCocos()
  {
    return table_view_;
  }


  virtual void SetNode(void *node);
  virtual void* GetNode();
 private:
  /*
   * とりあえず継承の設計が良いのが思いつかんので後ほど決める
   * 現状は同一メソッドを、各々のクラスで重複実装する
   */
  virtual IGuiParts *GetCustomClass(const char *name);
  virtual IGuiParts *AttachCreateParts(const Char *partsname,
                                       TGuiPartsType type);
  virtual Bool SetButtonCallback(const Char *name,
                                 TGuiPartsButtonCallback callback,
                                 void *userdata);
  virtual Bool SetMenuCallback(const Char *name, TGuiPartsMenuCallback callback,
                               void *userdata);
  virtual void SetTouchPriority(S32 priority);
  virtual S32 GetTouchPriority();
  virtual void SetVisible(Bool visible);
  virtual Bool IsVisible();
  virtual void AddChild(IGuiParts *child);
  virtual void AddChild(IGuiParts *child, S32 zOrder);
  virtual void AddChild(IGuiParts *child, S32 zOrder, S32 tag);
  virtual Bool CreateWithFile(const Char *filename, const TGuiPartsInfo *parts,
                              S32 parts_count);
  virtual Bool Dispose();

  cocos2d::extension::CCTableView *table_view_;
  TableViewDelegate delegate_;
  TableViewDataSource data_source_;
  GuiPartsTableViewCellCocos cell_;

  /**
   * \~english
   * \~japanese
   *  \brief クラス内 new/delete オペレータ定義
   */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("GuiPartsTableViewCocos");
  /**
   * \~english
   * \~japanese
   *  \brief コピー禁止定義
   */
  ADK_NON_COPYABLE_CLASS(GuiPartsTableViewCocos);
};

ADK_END_NAMESPACE

#endif /* GUI_PARTS_TABLE_VIEW_COCOS_H_ */
