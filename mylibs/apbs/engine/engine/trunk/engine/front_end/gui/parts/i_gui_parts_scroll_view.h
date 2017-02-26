/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file i_gui_parts.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief スクロールビュークラス定義部
 */

#ifndef I_GUI_PARTS_SCROLL_VIEW_H_
#define I_GUI_PARTS_SCROLL_VIEW_H_

#include <core/math/math_library.h>
#include <core/types.h>
#include <front_end/gui/parts/gui_parts_types.h>
#include <front_end/gui/parts/gui_parts_scroll_view_types.h>
#include <front_end/gui/parts/i_gui_parts.h>

ADK_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief スクロールビューパーツクラス
 */
class IGuiPartsScrollView : public IGuiParts {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~IGuiPartsScrollView();

  /*
   * Cocos2d-xのScrollViewラッパーメソッドたち
   */
  virtual void SetContentOffset(TVector2 &offset, Bool animated = false) = 0;
  virtual void GetContentOffset(TVector2 &dst) = 0;
  virtual void SetContentOffsetInDuration(TVector2 &offset, F32 dt) = 0;
  virtual void SetZoomScale(F32 s) = 0;
  virtual void SetZoomScale(F32 s, Bool animated) = 0;
  virtual F32 GetZoomScale() = 0;
  virtual void SetZoomScaleInDuration(F32 s, F32 dt) = 0;
  virtual void MinContainerOffset(TVector2 &dst) = 0;
  virtual void MaxContainerOffset(TVector2 &dst) = 0;
  virtual Bool IsNodeVisible(IGuiParts *parts) = 0;
  virtual void Pause(IGuiParts *parts) = 0;
  virtual void Resume(IGuiParts *parts) = 0;
  virtual Bool IsDragging() = 0;
  virtual Bool IsTouchMoved() = 0;
  virtual Bool IsBounceable() = 0;
  virtual void SetBounceable(Bool bounceable) = 0;
  virtual void GetViewSize(TSize &dst) = 0;
  virtual void SetViewSize(TSize &size) = 0;
#if 0
  virtual IGuiParts *GetContainer() = 0;
  virtual void SetContainer(IGuiParts *container) = 0;
#endif
  virtual TScrollViewDirection GetDirection() = 0;
  virtual void SetDirection(TScrollViewDirection direction) = 0;
#if 0
  virtual CCScrollViewDelegate *getDelegate() { return m_pDelegate; }
  virtual void setDelegate(CCScrollViewDelegate *pDelegate) { m_pDelegate = pDelegate; }
#endif
  virtual void SetContentSize(const TSize &size) = 0;
  virtual void GetContentSize(TSize &dst) = 0;
  virtual void UpdateInset() = 0;
  virtual Bool IsClippingToBounds() = 0;
  virtual void SetClippingToBounds(Bool bClippingToBounds) = 0;
  virtual void Visit() = 0;
  virtual void SetTouchEnabled(Bool e) = 0;

  /* ADKオリジナルメソッド群 */
  /**
   * \~english
   * \~japanese
   *  \brief 親子付けする初期値
   */
  virtual void CordinateStartPosition(const TVector2 &start) = 0;
  /**
   * \~english
   * \~japanese
   *  \brief スクロールビューの下から子を追加する
   *  \param[in] child \ref IGuiParts
   */
  virtual void AddChildFromBottom(IGuiParts *child) = 0;

 private:
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
                              S32 parts_count) = 0;
  /**
   * \~english
   * \~japanese
   *  \brief パーツを削除する
   *  \return 削除の成功判定。以下の値が返ります
   *  \retval true 削除に成功したときに返ります
   *  \retval false 削除に成功したときに返ります
   */
  virtual Bool Dispose() = 0;
};

ADK_END_NAMESPACE

#endif /* I_GUI_PARTS_SCROLL_VIEW_H_ */
