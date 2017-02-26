/**
 * \file adk_dms.h
 * \ingroup adkgrp_dms
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief Data management system 定義部
 */


#ifndef ADK_DATA_MANAGEMENT_SYSTEM_
#define ADK_DATA_MANAGEMENT_SYSTEM_


#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <debug_profiling/error/result.h>
#include <gameplay/http_data_interface/dms/dms_schema.h>


ADK_BEGIN_NAMESPACE


/* class definition */
/**
 * \~english
 * \~japanese
 *  \brief WASから受け取った情報を管理しネイティブへと情報を中継するクラス
 */
class DMS {
 public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  DMS();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~DMS();

  /* initialize & terminate */
  /**
   * \~english
   * \~japanese
   *  \brief DMSの初期化処理を行います。
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval ResultSuccess 初期化に成功したときに返ります
   *  \retval ResultNotFileExist スキーマファイルが一つも存在しなかったときに返ります
   */
  Result Initialize();
  /**
   * \~english
   * \~japanese
   *  \brief DMSの終了処理を行います。
   *  \return 終了の成功判定。以下の値が返ります
   *  \retval ResultSuccess  終了に成功したときに返ります
   */
  Result Terminate();

  /* schema managment functionally */
  /**
   * \~english
   * \~japanese
   *  \brief URLが該当するスキーマを探して返します
   *  \param[in] url アクセス先URL
   *  \return \ref DMS_Schema
   */
  DMS_Schema *FindSchema(const Char *url);
  /**
   * \~english
   * \~japanese
   *  \brief スキーマのリクエスト情報をリセットする
   */
  void ResetRequestSchema();
  /**
   * \~english
   * \~japanese
   *  \brief 共通レスポンスのスキーマを返す
   */
  ADK_INLINE DMS_Schema *GetCommonResponseSchema();

private:
  /* vars */
  /**
   * \~english
   * \~japanese
   *  \brief スキーマ情報
   */
  struct {
    S32 count;
    DMS_Schema *schemas;
    DMS_Schema *common_response;
  }schema_;

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("DMS");
  ADK_NON_COPYABLE_CLASS(DMS);
};
ADK_INLINE DMS_Schema *DMS::GetCommonResponseSchema() {
  return schema_.common_response;
}


ADK_END_NAMESPACE


#endif  /* ADK_DATA_MANAGEMENT_SYSTEM_ */


