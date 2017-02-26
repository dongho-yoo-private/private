/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file parts_system.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツ全体を管理するシステムクラス定義部
 */

#ifndef GUI_PARTS_SYSTEM_H_
#define GUI_PARTS_SYSTEM_H_

#include <front_end/gui/parts/intern/i_gui_parts_system.h>

ADK_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief パーツシステムクラス
 */
class GuiPartsSystem : public IGuiPartsSystem {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief パーツシステムを初期化し使用できる状態にします
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval ResultSuccess 初期化に成功したときに返ります
   *  \retval ResultDoAlready 既に初期化されているときに返ります
   *  \retval ResultOutOfResource システムリソースが不足しているときに返ります
   */
  static Result Initialize();
  /**
   * \~english
   * \~japanese
   *  \brief パースシステムを終了します
   *  \return 終了の成功判定。以下の値が返ります
   *  \retval ResultSuccess 終了に成功したときに返ります
   *  \retval ResultNotInitialized 初期化が行われていないときに返ります
   */
  static Result Terminate();
  /**
   * \~english
   * \~japanese
   *  \brief パーツシステムのインスタンスを返します
   *  \return \ref GuiPartsSystem
   */
  static GuiPartsSystem *GetInstance();

  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  GuiPartsSystem();
  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  virtual ~GuiPartsSystem();

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
                                            S32 parts_count);
  /**
   * \~english
   * \~japanese
   *  \brief \ref IGuiParts を削除します
   *  \param[in] parts \ref IGuiParts
   *  \return 削除の成功判定。以下の値が返ります
   *  \retval ResultSuccess 削除に成功したときに返ります
   */
  virtual Result DisposeParts(IGuiParts *parts);

 protected:
  /**
   * \~english
   * \~japanese
   *  \brief パーツシステムを初期化し使用できる状態にします
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval ResultSuccess 初期化に成功したときに返ります
   *  \retval ResultDoAlready 既に初期化されているときに返ります
   *  \retval ResultOutOfResource システムリソースが不足しているときに返ります
   */
  virtual Result Init() = 0;
  /**
   * \~english
   * \~japanese
   *  \brief パースシステムを終了します
   *  \return 終了の成功判定。以下の値が返ります
   *  \retval ResultSuccess 終了に成功したときに返ります
   *  \retval ResultNotInitialized 初期化が行われていないときに返ります
   */
  virtual Result Term() = 0;
  /**
   * \~english
   * \~japanese
   *  \brief ファイルから \ref IGuiParts を作成します
   *  \param[in] filename パーツファイル名
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval \ref IGuiParts 作成に成功したときに返ります
   *  \retval NULL 作成に失敗したときに返ります
   */
  virtual IGuiParts *CreateGuiParts() = 0;

  /**
   * \~english
   * \~japanese
   *  \brief 唯一無二のパーツシステム
   */
  static GuiPartsSystem *system_;
};

ADK_END_NAMESPACE

#endif /* GUI_PARTS_SYSTEM_H_ */

