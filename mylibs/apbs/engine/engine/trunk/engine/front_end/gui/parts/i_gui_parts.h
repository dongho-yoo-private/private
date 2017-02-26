/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file i_gui_parts.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief 描画される一パーツのインタフェース定義部
 */

#ifndef I_GUI_PARTS_H_
#define I_GUI_PARTS_H_

#include <core/types.h>
#include <front_end/gui/parts/gui_parts_types.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class GuiPartsSystem;

/**
 * \~english
 * \~japanese
 *  \brief パーツ一つを管理するクラス
 */
class IGuiParts {
  friend class GuiPartsSystem;

 public:
   /**
   * \~english
   * \~japanese
   *  \brief コンストラクター
   */
  IGuiParts();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~IGuiParts();

#if 0
  /**
   * \~english
   * \~japanese
   *  \brief パーツのタイプ \ref TGuiPartsType を返します
   *  \return \ref TGuiPartsType
   */
  virtual TGuiPartsType GetPartsType() const = 0;
#endif

  /**
   * \~english
   * \~japanese
   *  \brief カスタムクラスを返します
   *  \param[in] name カスタムクラス名
   *  \return \ret IGuiParts
   */
  virtual IGuiParts *GetCustomClass(const Char *name) = 0;
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
                                       TGuiPartsType type) = 0;
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
                                 void *userdata) = 0;
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
                               void *userdata) = 0;
 
  /**
   * \~english
   * \~japanese
   *  \brief もっともトップとなるIGuiPartsを設定します。\n
   *  名前重複回避のための対策
   *
   * \param top_parts \ref top_parts
  */
  void SetRootGuiParts(IGuiParts* top_parts);
  
  /**
   * \~english
   * \~japanese
   *  \brief もっともトップとなるIGuiPartsを取得します。\n
   *
   * \return top_parts \ref top_parts
  */
  IGuiParts* GetRootGuiParts();

  /*
   * Cocos2d-xのCCNodeラッパーメソッドたち
   */
  virtual void SetTouchPriority(S32 priority);
  virtual S32 GetTouchPriority();

  const Char *Description(void);
  virtual void SetZOrder(S32 zOrder);
  virtual void _SetZOrder(S32 z);
  virtual S32 GetZOrder();
  virtual void SetVertexZ(F32 vertexZ);
  virtual F32 GetVertexZ();
  virtual void SetScaleX(F32 fScaleX);
  virtual F32 GetScaleX();
  virtual void SetScaleY(F32 fScaleY);
  virtual F32 GetScaleY();
  virtual void SetScale(F32 scale);
  virtual F32 GetScale();
  virtual void SetPosition(const TVector2 &position);
  virtual void GetPosition(TVector2 &dst);
  virtual void SetPosition(F32 x, F32 y);
  virtual void GetPosition(F32 *x, F32 *y);
  virtual void SetPositionX(F32 x);
  virtual F32 GetPositionX(void);
  virtual void SetPositionY(F32 y);
  virtual F32 GetPositionY(void);
  virtual void SetSkewX(F32 fSkewX);
  virtual F32 GetSkewX();
  virtual void SetSkewY(F32 fSkewY);
  virtual F32 GetSkewY();
  virtual void SetAnchorPoint(const TVector2 &anchorPoint);
  virtual void GetAnchorPoint(TVector2 &dst);
  virtual void GetAnchorPointInPoints(TVector2 &dst);
  virtual void SetContentSize(const TSize &contentSize);
  virtual void GetContentSize(TSize &dst);
  virtual void SetVisible(Bool visible);
  virtual Bool IsVisible();
  virtual void SetRotation(F32 fRotation);
  virtual F32 GetRotation();
  virtual void SetRotationX(F32 fRotaionX);
  virtual F32 GetRotationX();
  virtual void SetRotationY(F32 fRotationY);
  virtual F32 GetRotationY();
  virtual void SetOrderOfArrival(U32 uOrderOfArrival);
  virtual U32 GetOrderOfArrival();
  virtual void IgnoreAnchorPointForPosition(Bool ignore);
  virtual Bool IsIgnoreAnchorPointForPosition();
  virtual void AddChild(IGuiParts *child);
  virtual void AddChild(IGuiParts *child, S32 zOrder);
  virtual void AddChild(IGuiParts *child, S32 zOrder, S32 tag);
//#if 0 タグでの取得が必要なためコメント解除 2014.04.14 kato
  virtual IGuiParts *GetChildByTag(S32 tag);
  virtual S32 GetTag();
  virtual void SetTag(S32 tag);
#if 0
  virtual CCArray *GetChildren();
#endif
  virtual U32 GetChildrenCount(void);
  virtual void SetParent(IGuiParts *parent);
#if 0
  virtual IGuiParts *GetParent();
#endif
  virtual void RemoveFromParent();
  virtual void RemoveFromParentAndCleanup(Bool cleanup);
  virtual void RemoveChild(IGuiParts *child);
  virtual void RemoveChild(IGuiParts *child, Bool cleanup);
  virtual void RemoveChildByTag(S32 tag);
  virtual void RemoveChildByTag(S32 tag, Bool cleanup);
  virtual void RemoveAllChildren();
  virtual void RemoveAllChildrenWithCleanup(Bool cleanup);
  virtual void ReorderChild(IGuiParts *child, S32 zOrder);
  virtual void SortAllChildren();

  /**
   * \~english
   * \~japanese
   *  \brief 間に合わせ実装。後で隠蔽したいが大変なので今はココに定義
   */
  /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiParts オブジェクトの初期化処理を行います
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval true 成功したときに返ります
   *  \retval false 失敗したときに返ります
   */
  virtual Bool Init() = 0;
  virtual void SetNode(void *node) = 0;
  virtual void* GetNode() = 0;

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
  
  IGuiParts* root_parts_;
};

ADK_END_NAMESPACE

#endif /* I_GUI_PARTS_H_ */
