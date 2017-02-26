//
//  tdd_menu.h
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#ifndef __Chaos__tdd_menu__
#define __Chaos__tdd_menu__

#include "tdd/tdd_types.h"

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
CHAOS_BEGIN_NAMESPACE
/**
 * \~english
 * \~japanese
 *  \brief 画面描画とイベントを取得するクラス
 */
class TDDMenu : public cocos2d::CCLayer,
public CCTableViewDataSource,
public CCTableViewDelegate
{
  
public:
  TDDMenu();
  virtual ~TDDMenu();
  
  virtual bool init();
  
  //CCTableViewDataSource
  virtual CCSize cellSizeForTable(CCTableView* table);
  virtual CCTableViewCell* tableCellAtIndex(CCTableView* table,unsigned int idx);
  virtual unsigned int numberOfCellsInTableView(CCTableView* table);
  
  //CCTableViewDelegate
  virtual void scrollViewDidScroll(CCScrollView * view);
  virtual void scrollViewDidZoom(CCScrollView* view);
  virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
  
  CREATE_FUNC(TDDMenu);
  
  /**
   * \~english
   * \~japanese
   *  \brief テーブルビューに表示するリストをセットする関数。TDDBaseが使用します。
   */
  void setTableItemList(std::vector<const char*> itemList);
  int menuLevel_;
private:
  /**
   * \~english
   * \~japanese
   *  \brief テーブルビュー
   */
  CCTableView* tableView;
  /**
   * \~english
   * \~japanese
   *  \brief テーブルビュー表示用の文字列
   */
  std::vector<const char*> cellNameList;
  /**
   * \~english
   * \~japanese
   *  \brief デバイス画面サイズ
   */
  CCSize windowSize;
  /**
   * \~english
   * \~japanese
   *  \brief テーブルビューに文字列をセットする関数です。
   */
  void setTableView();
  /**
   * \~english
   * \~japanese
   *  \brief 不正なデータ取得の際、警告メッセージを表示します。
   */
  void showWarningMessage();
  /**
   * \~english
   * \~japanese
   *  \brief 閉じるボタンクリック時のコールバック関数です。
   */
  void menuCloseCallback(CCObject* pSender);
  
};
CHAOS_END_NAMESPACE

#endif /* defined(__Chaos__tdd_menu__) */
