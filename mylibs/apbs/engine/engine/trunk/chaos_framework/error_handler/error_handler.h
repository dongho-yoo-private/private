/** 
 * Copyright 2014 Applibot Inc.
 *
 * Author: Dongho.Yoo.
*/
#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_

#include <core/types.h>
#include <core/library/lib_json_document.h>
#include "error_handler/error_handler_types.h"
#include "application/application_types.h"

CHAOS_BEGIN_NAMESPACE

/** \brief エラー処理を行うクラスです。
*
*/
class ErrorHandler
{
public:
  /**\brief constructor&destructor*/
  ErrorHandler();
  ~ErrorHandler();

  /**
  * \brief 初期化処理を行います。
  *
  * \return true成功、false失敗　
  */
  Bool Initialize();
  /**
  * \brief 終了処理を行います。
  *
  * \return true成功、false失敗　
  */
  Bool Terminate();

  /**
  * @brief 共通エラー処理を行います。
  *
  * @param error_info  \brief TErrorInfo
  *
  * @return trueであれば、エラーありで、falseであれば、エラー無し
  */
  Bool SetError(const TErrorInfo* error_info);
 
  /**
  * \brief エラーをクリアーします。
  */
  void Clear()
  {
    system_error_code_ = kSystemErrorSuccess;
    application_error_info_.application_error_code=kNO_ERROR;
  }

  /**
  * @brief 共通エラー処理を行います。
  *
  * @param common_header jsonの共通ヘッダーが格納されているjson
  *
  * @return trueであれば、エラーありで、falseであれば、エラー無し
  */
  Bool SetError(const adk::LIB_TJsonMember& common_header);
 
  /** 
  * \brief システムエラーコードを返します。
  *
  * \return \ref TSystemErrorCode
  */
  ADK_INLINE const TSystemErrorCode GetSystemErrorCode() const
  {
    return system_error_code_;
  }
  
  /**
  * \brief アプリケーションエラー情報を取得します。
  *
  * \return \ref TApplicationErrorInfo
  */
  ADK_INLINE const TApplicationErrorInfo* GetApplicationErrorInfo() const
  {
    return &application_error_info_;
  }

private:
  TApplicationErrorInfo application_error_info_;
  TSystemErrorCode system_error_code_;
};

CHAOS_END_NAMESPACE  

#endif
