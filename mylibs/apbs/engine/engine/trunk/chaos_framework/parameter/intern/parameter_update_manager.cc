/**
 * Copyright 2014 Applibot Inc.
 * \author hiromu kato
 * \file parameter_update_manager.h
 * \ingroup parameter_group
 * \~english
 * \~japanese
 *  \brief パラメーターをアップデートする機構
 */
#include "parameter_update_manager.h"
#include <core/types.h>

CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief ワーカスレッド（ParameterUpdateManagerのOnThread()メソッドを呼び出します)
 */
static void* _threadFunction(void *arg) {
  //printf("in _threadFunction\n");
  ParameterUpdateManager::TRequestQueue* req_data = (ParameterUpdateManager::TRequestQueue*)arg;
  req_data->dm->OnThread();
  
  return NULL;
}

/**
 * \~english
 * \~japanese
 *  \brief デフォルトコンストラクタ
 */
ParameterUpdateManager::ParameterUpdateManager()
{
  for(int i=0;i<kMaxRequestQueue;i++) {
    request_queue[i].is_used = false;
    request_queue[i].dm = 0;
    request_queue[i].api_name = kApiUnknown;
    request_queue[i].IsRequestCompleted = 0;
    request_queue[i].user_data = 0;
  }
}


ParameterUpdateManager::~ParameterUpdateManager()
{
}

/**
 * \~english
 * \~japanese
 *  \brief データマネジャの初期化処理を行います
 */
Bool ParameterUpdateManager::Initialize() {
  return true;
}
/**
 * \~english
 * \~japanese
 *  \brief データマネジャの終了処理を行います
 */
Bool ParameterUpdateManager::Terminate() {
  return true;
}

/**
 * \~english
 * \~japanese
 *  \brief ADKに非同期でデータリクエストをします。またワーカスレッドを生成します。
 */
Bool ParameterManager::RequestUpdate(TApiName api_name, const void* request_data, void* user_data) {
  Bool ret = false;
  
  //スレッドキューの設定
  int thread_num;
  for(thread_num=0;thread_num<kMaxRequestQueue;thread_num++) {
    if(request_queue[thread_num].is_used == false) {
      request_queue[thread_num].is_used = true;//利用中に設定
      request_queue[thread_num].dm = this;
      request_queue[thread_num].api_name = api_name;
      request_queue[thread_num].user_data = user_data;
      break;
    }
  }

  
  ret = api_requestor_->Request(api_name, request_data, &request_queue[thread_num].IsRequestCompleted);  
#if 0
  switch (api_name) {
    case kApiMyPage:
    {
      //request_queue[thread_num].IsRequestCompleted = adk4chaos::mypage_top::IsRequestCompleted;
      //adk::Result result = adk4chaos::mypage_top::RequestAsync(CompletedRequest);
      //if(result.IsSuccess()) ret = true;
      break;
    }
    default:
      break;
  }
#endif
  
  /* スレッドの生成 */
  pthread_mutex_init(&data_manager_mutex, NULL);
  pthread_create(&data_manager_thread, NULL, _threadFunction, &request_queue[thread_num]);
  pthread_detach(data_manager_thread);
  
  return ret;
}

/**
 * \~english
 * \~japanese
 *   \brief リクエストが完了するまで待ち、リクエスト完了後イベント登録します。
 */
void ParameterUpdateManager::OnThread() {
  printf("in OnThread\n");
  
  pthread_mutex_lock(&data_manager_mutex);

  Bool is_completed = false;
  
  for(int i=0;i<kMaxRequestQueue;i++) {
    if(request_queue[i].is_used == true) {
      while(1) {
        printf("test\n");
        adk4chaos::mypage_top::IsRequestCompleted(&is_completed);
        if(is_completed) {
          //イベント登録
          // game_manager->AddEvent();
          break;
        }
        //TODO 無限ループになるため一時的にbreakで抜けているが最後には削除する
        break;
      }
    }
  }
  
  pthread_mutex_unlock(&data_manager_mutex);
  pthread_mutex_destroy(&data_manager_mutex);
}

/**
 * \~english
 * \~japanese
 *   \brief ADKへのリクエストからレスポンスが帰ったときに呼ばれるコールバック関数。
 *          データのエラーチェックとデータセット(必要なデータの加工と保持)をします。
 */
void ParameterUpdateManager::CompletedRequest(S32 code, const Char *message) {
  //printf("Error Code=%d / Error Message=%s\n", code, message)

  /* エラー判定（エラーハンドラーの呼び出し） */
  //result = error_handler->CheckError();

  /* apiごとの処理 */
  switch (code) {
    case kApiMyPage:
      break;
      
    default:
      break;
  }
  
  /* データ加工 */
  // this->some_processing();
  
  /* (必要なものは)データ保持 */
  
  /* GameManagerにデータ処理が終わったことを知らせる */
  // game_manager->CanAccessToData();
  //
}
 
CHAOS_END_NAMESPACE
