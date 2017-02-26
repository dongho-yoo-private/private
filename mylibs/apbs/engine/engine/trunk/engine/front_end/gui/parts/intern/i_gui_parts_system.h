/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file i_gui_parts_system.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツ全体を管理するシステムインタフェース定義部
 */

#ifndef I_GUI_PARTS_SYSTEM_H_
#define I_GUI_PARTS_SYSTEM_H_

#include <core/math/math_library.h>
#include <core/types.h>
#include <debug_profiling/error/result.h>
#include <front_end/gui/parts/gui_parts_types.h>

ADK_BEGIN_NAMESPACE

/* forward declaration */
class IGuiParts;
class IGuiPartsSprite;
class IGuiPartsSpriteFrame;
class IGuiPartsTexture;

/**
 * \~english
 * \~japanese
 *  \brief パーツシステムクラス
 */
class IGuiPartsSystem {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  virtual ~IGuiPartsSystem();

  /**
   * \~english
   * \~japanese
   *  \brief ファイルから \ref IGuiParts を作成します
   *  \param[in] filename パーツファイル名
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval \ref IGuiParts 作成に成功したときに返ります
   *  \retval NULL 作成に失敗したときに返ります
   */
  virtual IGuiParts *CreateGuiPartsWithFile(const char *filename,
                                            const TGuiPartsInfo *parts,
                                            S32 parts_count) = 0;
  /**
   * \~english
   * \~japanese
   *  \brief 指定した \ref TGuiPartsType の \ref IGuiParts を作成する
   *  \param[in] type \ref TGuiPartsType
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval \ref IGuiParts 作成に成功したときに返ります
   *  \retval NULL 作成に失敗したときに返ります
   */
  virtual IGuiParts *CreateGuiPartsWithType(TGuiPartsType type) = 0;
 
  /**
   * \~english
   * \~japanese
   *  \brief 指定した \ref TGuiPartsType の \ref IGuiParts を作成するが、\n
   *  内部ではcocosをnodeの参照として生成する。
   *  \param[in] type \ref TGuiPartsType
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval \ref IGuiParts 作成に成功したときに返ります
   *  \retval NULL 作成に失敗したときに返ります 
  */
  virtual IGuiParts *CreateGuiPartsWithType(TGuiPartsType type, void* node)=0;
  
  /**
   * \~english
   * \~japanese
   *  \brief スプライトラッパー系
   */
  virtual IGuiPartsSprite* CreateGuiPartsSprite() = 0;
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithFile(const char* filename) = 0;
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithFile(const char* filename,
                                                        const TRect& rect) = 0;
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithTexture(
      IGuiPartsTexture* pTexture) = 0;
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithTexture(
      IGuiPartsTexture* pTexture, const TRect& rect) = 0;
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithSpriteFrame(
      IGuiPartsSpriteFrame* spriteframe) = 0;
  virtual IGuiPartsSprite* CreateGuiPartsSpriteWithSpriteFrameName(
      const Char *framename) = 0;
  /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiParts を削除します
   *  \param[in] parts \ref IGuiParts
   *  \return 削除の成功判定。以下の値が返ります
   *  \retval ResultSuccess 削除に成功したときに返ります
   */
  virtual Result DisposeParts(IGuiParts *parts) = 0;
  /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiParts をシステムに追加します
   *  \param[in] parts \ref IGuiParts
   *  \return 追加の成功判定。以下の値が返ります
   *  \retval ResultSuccess 追加に成功したときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  virtual Result AddGuiParts(IGuiParts *parts) = 0;
  /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiParts をシステムから切り離します
   *  \param[in] parts \ref IGuiParts
   *  \return 切り離しの成功判定。以下の値が返ります
   *  \retval ResultSuccess 切り離しに成功したときに返ります
   *  \retval ResultInvalidVariable 不正な引数が設定されたときに返ります
   */
  virtual Result RemoveGuiParts(IGuiParts *parts) = 0;
};

ADK_END_NAMESPACE

#endif /* I_GUI_PARTS_SYSTEM_H_ */

