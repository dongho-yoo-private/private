/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_scroll_view.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief ノードパーツクラス定義部 for Cocos2d-x
 */

#ifndef GUI_PARTS_NODE_COCOS_H_
#define GUI_PARTS_NODE_COCOS_H_

#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>
#include <front_end/gui/parts/gui_parts_scroll_view_types.h>
#include <front_end/gui/parts/i_gui_parts_node.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class GuiCcPartsNode;

/**
 * \~english
 * \~japanese
 *  \brief ノードパーツクラス
 */
class GuiPartsNodeCocos : public IGuiPartsNode {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  GuiPartsNodeCocos();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~GuiPartsNodeCocos();

  /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiPartsNode オブジェクトの初期化処理を行います
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval true 成功したときに返ります
   *  \retval false 失敗したときに返ります
   */
  virtual Bool Init();

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
  virtual void SetNode(void *node);
  virtual void* GetNode();

  cocos2d::CCNode *node_;

  /**
   * \~english
   * \~japanese
   *  \brief クラス内 new/delete オペレータ定義
   */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("GuiPartsNodeCocos");
  /**
   * \~english
   * \~japanese
   *  \brief コピー禁止定義
   */
  ADK_NON_COPYABLE_CLASS(GuiPartsNodeCocos);
};

ADK_END_NAMESPACE

#endif /* GUI_PARTS_NODE_COCOS_H_ */
