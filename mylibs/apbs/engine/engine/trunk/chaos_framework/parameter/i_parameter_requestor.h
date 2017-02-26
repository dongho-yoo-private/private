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
#include <parameter/api_types.h>

#include <application/application_types.h>

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
  virtual Bool Request(TApiName api_name, const void* request_data)=0;
  
  /**
  * \brief リクエストが終了したかどうかをチェックします。
  * 
  * \return true 成功、false 失敗
  */
  virtual Bool IsComplete(TApiName api_name)=0;
  
  /**
  * \brief エラー発生時に呼び出されます。
  */
  virtual void OnError(TApiName api_name, S32 error_code)=0;
  
  /**
  * \brief リクエストが終了したタイミングで呼び出されます。
  * 
  * \return true 成功、false 失敗
  */
  virtual Bool Completed(TApiName api_name)=0;

  /**
  * \brief リクエスト関連の初期化処理を行います。\n
  * 1. 共通ヘッダーの作成
  * 2. サーバの接続先の設定など
  */
  virtual Bool Initialize()=0;
  
};
CHAOS_END_NAMESPACE

#endif
