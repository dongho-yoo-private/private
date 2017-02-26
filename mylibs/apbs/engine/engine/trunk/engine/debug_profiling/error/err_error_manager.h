/**
 * \file err_error_manager.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief エラーマネージャ定義部
 */


#ifndef ERR_ERROR_MANAGER_H_
#define ERR_ERROR_MANAGER_H_


#include <debug_profiling/error/result.h>
#include <core/memory_allocation/guarded_alloc.h>


ADK_BEGIN_NAMESPACE


/* class definition */
/**
 * \~english
 * \~japanese
 *   \brief エラーマネージャクラス
 */
class ERR_ErrorManager {
public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   */
  ERR_ErrorManager();
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  ~ERR_ErrorManager();

  /* initialize & terminate */
  /**
   * \~english
   * \~japanese
   *   \brief 初期化処理を行います
   *   \return 初期化処理の成功判定。以下の値が返ります
   *   \retval ResultSuccess    初期化処理に成功したときに返ります
   *   \retval ResultOutOfResource  システムリソースが不足しているときに返ります
   */
  Result Initialize();
  /**
   * \~english
   * \~japanese
   *   \brief 終了処理を行います
   *   \return 終了処理の成功判定。以下の値が返ります
   *   \retval ResultSuccess      終了に成功したときに返ります
   *   \retval ResultNotInitialized  初期化処理が行われていないときに返ります
   */
  Result Terminate();

  /* error handling manegement functionally */
  /**
   * \~english
   * \~japanese
   *   \brief エラー処理を開始します
   *   \return 開始の成功判定。以下の値が返ります
   *   \retval ResultSuccess  開始に成功したときに返ります
   *   \retval ResultDoAlready  既に開始されているときに返ります
   */
  Result StartErrorProcess();
  /**
   * \~english
   * \~japanese
   *   \brief エラー処理を終了します
   *   \return 終了の成功判定。以下の値が返ります
   *   \retval ResultSuccess      終了に成功したときに返ります
   *   \retval ResultNotInitialized  開始処理が行われていないときに返ります
   */
  Result EndErrorProcess();
  /**
   * \~english
   * \~japanese
   *   \brief エラーが発生しているかを返します
   *   \return エラー発生フラグ
   *   \retval true  エラーが発生しているときに返ります
   *   \retval false  エラーが発生していないとき、又はエラー処理中に呼び出されたときに返ります
   */
  ADK_INLINE Bool IsError() const;
  /**
   * \~english
   * \~japanese
   *   \brief エラーメッセージを設定します
   *   \param[in] code エラーコード
   *   \param[in] message エラーメッセージ
   */
  Result SetErrorMessage(S32 code,const Char *message);
  /**
   * \~english
   * \~japanese
   *   \brief エラーコードを返します
   *   \attention エラー処理中には不正な値が返ります
   *   \return コード
   */
  ADK_INLINE S32 GetErrorCode() const;
  /**
   * \~english
   * \~japanese
   *   \brief エラーメッセージを返します
   *   \attention エラー処理中には不正な値が返ります
   *   \return メッセージ
   */
  ADK_INLINE const Char *GetErrorMessage() const;

private:
  /* vars */
  /**
   * \~english
   * \~japanese
   *   \brief エラー処理フラグ
   */
  Bool  _isErrorProcess;
  /**
   * \~english
   * \~japanese
   *   \brief エラー発生フラグ
   */
  Bool  _isError;
  /**
   * \~english
   * \~japanese
   *   \brief エラーメッセージ文字列長
   */
  static const U32  kErrorMessageLen = 512;
  /**
   * \~english
   * \~japanese
   *   \brief エラーコード
   */
  S32  _errorCode;
  /**
   * \~english
   * \~japanese
   *   \brief エラーメッセージ
   */
  Char  _errorMessage[kErrorMessageLen];

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("Result");
};
ADK_INLINE Bool ERR_ErrorManager::IsError() const {
  /* エラー処理中はエラー発生は返さない */
  return _isErrorProcess!=false ? false : _isError;
}
ADK_INLINE S32 ERR_ErrorManager::GetErrorCode() const {
  return _errorCode;
}
ADK_INLINE const Char *ERR_ErrorManager::GetErrorMessage() const {
  return _errorMessage;
}


ADK_END_NAMESPACE


#endif  /* ERR_ERROR_MANAGER_H_ */


