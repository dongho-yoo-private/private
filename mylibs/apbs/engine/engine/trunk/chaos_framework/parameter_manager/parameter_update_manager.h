/**
 * Copyright 2014 Applibot Inc.
 * \author hiromu kato
 * \file parameter_update_manager.h
 * \ingroup parameter_group
 * \~english
 * \~japanese
 *  \brief パラメーターをアップデートする機構
 */

#include "application/application_manager.h"
#include <core/types.h>
#include <debug_profiling/error/result.h>
#include "i_parameter_requestor.h"

typedef S32 TApiName;

CHAOS_BEGIN_NAMESPACE
class ParameterUpdateManager
{
public:
  typedef struct {
    Bool is_used;
    DataManager* dm;
    TApiName api_name;
    adk::Result (*IsRequestCompleted)(Bool*);
    void *user_data;
  } TRequestQueue;
  static const S32 kMaxRequestQueue = 2;
  TRequestQueue request_queue[kMaxRequestQueue];

  /** \brief constructor */
  ParameterUpdateManager();

  /** \brief destructor */
  ~ParameterUpdateManager();

  /**
   * \~english
   * \~japanese
   *  \brief データマネージャを初期化する
   *  \return 終了処理の成功判定。以下の値が返ります
   *  \retval true 終了処理に成功したときに返ります
   *  \retval false 終了処理に成功したときに返ります
   */
  Bool Initialize();

  /**
   * \~english
   * \~japanese
   *  \brief データマネージャを終了する
   */
  Bool Terminate();

  /**
   * \~english
   * \~japanese
   * \brief アプリケーション側ではリクエストを行うリクエスターを登録する必要があります。
  */
  void RegisterRequestor(IParameterRequestor* requestor);

  /**
   * \~english
   * \~japanese
   *   \brief ADKにリクエストを行うためのスレッドを生成します。
   *   \brief param[in] api_name リクエストするAPI名
   *   \brief param[in] request_data リクエスト時に送信するデータ
   *   \brief param[in] user_data リクエスト送信元を特定するためのデータ
   *   \brief return リクエストの成功判定。以下の値が返ります
   *   \retval TODO
   */
  Bool RequestUpdate(TApiName api_name, const void* request_data, void* user_data);

  /**
   * \~english
   * \~japanese
   *   \brief スレッド処理を行います。
   */
  void OnThread();
  
  /**
   * \~english
   * \~japanese
   *   \brief ADKで処理を行ったデータのエラーチェックとデータセットします
   */
  static void CompletedRequest(S32 code, const Char *message);
 
 
};

CHAOS_END_NAMESPACE


