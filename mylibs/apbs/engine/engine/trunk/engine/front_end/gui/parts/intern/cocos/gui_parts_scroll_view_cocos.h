/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_scroll_view.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief スクロールビューパーツクラス定義部 for Cocos2d-x
 */

#ifndef GUI_PARTS_SCROLL_VIEW_COCOS_H_
#define GUI_PARTS_SCROLL_VIEW_COCOS_H_

#include <extensions/CCBReader/CCScrollViewLoader.h>

#include <core/math/math_library.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_cc_parts_scroll_view.h>
#include <front_end/gui/parts/i_gui_parts_sprite.h>
#include <front_end/gui/parts/i_gui_parts_scroll_view.h>
#include <front_end/gui/parts/gui_parts_scroll_view_types.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class GuiCcPartsScrollView;

/**
 * \~english
 * \~japanese
 *  \brief スクロールビューパーツクラス
 */
class GuiPartsScrollViewCocos : public IGuiPartsScrollView {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  GuiPartsScrollViewCocos();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~GuiPartsScrollViewCocos();

  /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiPartsScrollViewCocos オブジェクトの初期化処理を行います
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval true 成功したときに返ります
   *  \retval false 失敗したときに返ります
   */
  virtual Bool Init();

  /*
   * Cocos2d-xのScrollViewラッパーメソッドたち
   */
  void SetContentOffset(TVector2 &offset, Bool animated = false);
  void GetContentOffset(TVector2 &dst);
  void SetContentOffsetInDuration(TVector2 &offset, F32 dt);
  void SetZoomScale(F32 s);
  void SetZoomScale(F32 s, Bool animated);
  F32 GetZoomScale();
  void SetZoomScaleInDuration(F32 s, F32 dt);
  void MinContainerOffset(TVector2 &dst);
  void MaxContainerOffset(TVector2 &dst);
  Bool IsNodeVisible(IGuiParts *parts);
  void Pause(IGuiParts *parts);
  void Resume(IGuiParts *parts);
  Bool IsDragging();
  Bool IsTouchMoved();
  Bool IsBounceable();
  void SetBounceable(Bool bounceable);
  void GetViewSize(TSize &dst);
  void SetViewSize(TSize &size);
#if 0
  IGuiParts *GetContainer();
  void SetContainer(IGuiParts *container);
#endif
  TScrollViewDirection GetDirection();
  void SetDirection(TScrollViewDirection direction);
#if 0
  CCScrollViewDelegate *getDelegate() { return m_pDelegate; }
  void setDelegate(CCScrollViewDelegate *pDelegate) { m_pDelegate = pDelegate; }
#endif
  void SetContentSize(const TSize &size);
  void GetContentSize(TSize &dst);
  void UpdateInset();
  Bool IsClippingToBounds();
  void SetClippingToBounds(Bool bClippingToBounds);
  void Visit();
  void SetTouchEnabled(Bool e);

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

  /* ADKオリジナルメソッド群 */
  /**
   * \~english
   * \~japanese
   *  \brief 親子付けする初期値
   */
  virtual void CordinateStartPosition(const TVector2 &start);
  /**
   * \~english
   * \~japanese
   *  \brief スクロールビューの下から子を追加する
   *  \param[in] child \ref IGuiParts
   */
  virtual void AddChildFromBottom(IGuiParts *child);

  GuiCcPartsScrollView *scroll_view_;

  /**
   * \~english
   * \~japanese
   *  \brief 親子付け
   */
  struct {
    TVector2 start;
    TVector2 now;
  }cordinate_;

  /**
   * \~english
   * \~japanese
   *  \brief クラス内 new/delete オペレータ定義
   */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("GuiPartsScrollViewCocos");
  /**
   * \~english
   * \~japanese
   *  \brief コピー禁止定義
   */
  ADK_NON_COPYABLE_CLASS(GuiPartsScrollViewCocos);
};

ADK_END_NAMESPACE

#endif /* GUI_PARTS_SCROLL_VIEW_COCOS_H_ */
