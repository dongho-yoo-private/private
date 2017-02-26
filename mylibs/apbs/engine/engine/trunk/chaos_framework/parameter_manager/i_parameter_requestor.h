/**
 * Copyright 2014 Applibot Inc.
 * \author dongho yoo
 * \file scene_manager.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief Sceneのファクトリークラス。アプリ側ではこのクラスを継承して実装します。
 */

#ifndef _I_PARAMETER_REQUESTOR_H_
#define _I_PARAMETER_REQUESTOR_H_

#include <core/types.h>
#include <debug_profiling/error/result.h>
#include <parameter_manager/api_types.h>

CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief リクエストを実装するインターフェースクラス
 */
class IParameterRequestor {
public:
  /**
  * \brief このメソッドの中で、実際のリクエストを行います。　
  *
  * \param [in] api_name APIのID
  * \param [in] request_data Request時のデータ
  * \param [out] is_request_complete リクエストが終わったかどうかをチェックするフラグ
  * 
  * \return 生成したSceneクラスのポインター
  */
  virtual adk::Result Request(TApiName api_name, const void* request_data, Bool* is_request_complete)=0;
};
CHAOS_END_NAMESPACE

#endif
