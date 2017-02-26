//
//  gui_parts_table_view_cell_cocos.h
//  af-engine
//
//  Created by CJ0048 on 2014/04/15.
//  Copyright (c) 2014年 B04350. All rights reserved.
//

#ifndef __af_engine__gui_parts_table_view_cell__
#define __af_engine__gui_parts_table_view_cell__

#include <GUI/CCScrollView/CCTableViewCell.h>

#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>
#include <front_end/gui/parts/i_gui_parts_table_view_cell.h>

ADK_BEGIN_NAMESPACE

class GuiPartsTableViewCellCocos : public IGuiPartsTableViewCell {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  GuiPartsTableViewCellCocos();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~GuiPartsTableViewCellCocos();
 
  virtual Bool Init();
  
  /*
   * Cocos2d-xのCCTableViewCellラッパーメソッドたち
   */
  unsigned int GetIdx();
  void SetIdx(unsigned int idx);
  void Reset();
  void SetObjectID(unsigned int idx);
  unsigned int GetObjectID();
  
 
  
    /**
   * \~english
   * \~japanese
   *  \brief カスタムクラスを返します
   *  \param[in] name カスタムクラス名
   *  \return \ret IGuiParts
   */
  virtual IGuiParts *GetCustomClass(const Char *name);
  /**
   * \~english
   * \~japnese
   *  \brief \ref IGuiParts が持つパーツを任意の \ref TGuiPartsType で作成/ア
   *  タッチを行います
   *  \param[in] parts アタッチするパーツのインタフェース
   *  \param[in] type パーツタイプ
   *  \return 作成/アタッチの成功判定。以下の値が返ります
   *  \retval true 作成/アタッチに成功したときに返ります
   *  \retval false 作成/アタッチに失敗したときに返ります
   */
  virtual IGuiParts *AttachCreateParts(const Char *partsname,
                                       TGuiPartsType type);
  /**
   * \~english
   * \~japanese
   *  \brief ボタンのコールバックを設定します
   *  \param[in] name コールバックを設定するパーツ名
   *  \param[in] callback コールバック関数のポインタ
   *  \param[in] userdata コールバック関数の引数に設定されるユーザーデータ
   *  \return 設定の成功判定。以下の値が返ります
   *  \retval true 設定に成功したときに返ります
   *  \retval false 設定に失敗したときに返ります
   */
  virtual Bool SetButtonCallback(const Char *name,
                                 TGuiPartsButtonCallback callback,
                                 void *userdata);
  /**
   * \~english
   * \~japanese
   *  \brief メニューのコールバックを設定します
   *  \param[in] name コールバックを設定するパーツ名
   *  \param[in] callback コールバック関数のポインタ
   *  \param[in] userdata コールバック関数の引数に設定されるユーザーデータ
   *  \return 設定の成功判定。以下の値が返ります
   *  \retval true 設定に成功したときに返ります
   *  \retval false 設定に失敗したときに返ります
   */
  virtual Bool SetMenuCallback(const Char *name, TGuiPartsMenuCallback callback,
                               void *userdata);

  /**
  * \brief テーブルビューのデリゲート対応のため
  */
  ADK_INLINE void SetCocos(cocos2d::extension::CCTableViewCell *cell)
  {
    cell_=cell;
  }
  /**
  * \brief テーブルビューのデリゲート対応のため
  */
  ADK_INLINE cocos2d::extension::CCTableViewCell * GetCocos()
  {
    return cell_;
  }

 private:
  virtual void SetNode(void *node);
  virtual void* GetNode();
  /**
   * \~english
   * \~japanese
   *  \brief ファイルからパーツを作成する
   *  \param[in] filename パーツのファイル名
   *  \param[in] parts パーツ内に点在するパーツ一つ一つの情報
   *  \param[in] parts_count パーツ内のパーツ数
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval true 作成に成功したときに返ります
   *  \retval false 作成に成功したときに返ります
   */
  virtual Bool CreateWithFile(const Char *filename, const TGuiPartsInfo *parts,
                              S32 parts_count);
  /**
   * \~english
   * \~japanese
   *  \brief パーツを削除する
   *  \return 削除の成功判定。以下の値が返ります
   *  \retval true 削除に成功したときに返ります
   *  \retval false 削除に成功したときに返ります
   */
  virtual Bool Dispose();
  
  unsigned int idx_;
  cocos2d::extension::CCTableViewCell *cell_;
  
};


ADK_END_NAMESPACE

#endif /* defined(__af_engine__gui_parts_table_view_cell_cocos__) */
