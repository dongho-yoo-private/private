/**
 * Copyright 2014 Applibot Inc.
 * \author hiromu kato
 * \file parameter_update_manager.h
 * \ingroup parameter_group
 * \~english
 * \~japanese
 *  \brief パラメーターをアップデートする機構
 */
#include "parameter/parameter_update_manager.h"
#include "game/game_manager.h"
#include "error_handler/error_handler.h"
#include <core/types.h>
#include <core/library/lib_thread.h>

CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief ワーカスレッド（ParameterUpdateManagerのOnThread()メソッドを呼び出します)
 */
static void _threadFunction(void *arg) {
  //printf("in _threadFunction\n");
  ParameterUpdateManager::TRequestQueue* req_data = (ParameterUpdateManager::TRequestQueue*)arg;
  req_data->dm->OnThread();
  
  return ;
}

/**
 * \~english
 * \~japanese
 *  \brief デフォルトコンストラクタ
 */
ParameterUpdateManager::ParameterUpdateManager()
: api_requestor_(0),
handle_mutex_(0)
{
  for(int i=0;i<kMaxRequestQueue;i++) {
    request_queue[i].is_used = false;
    request_queue[i].dm = 0;
    request_queue[i].api_name = -1;
    request_queue[i].requestor= 0;
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
  handle_mutex_ = adk::LIB_MutexCreate();
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

void ParameterUpdateManager::RegisterRequestor(chaos::IParameterRequestor *requestor)
{
  api_requestor_=requestor;
}

/**
 * \~english
 * \~japanese
 *  \brief ADKに非同期でデータリクエストをします。またワーカスレッドを生成します。
 */
Bool ParameterUpdateManager::RequestUpdate(TApiName api_name, const void* request_data, void* user_data) {
  Bool ret = false;
  
  //スレッドキューの設定
  int thread_num;
  for(thread_num=0;thread_num<kMaxRequestQueue;thread_num++) {
    if(request_queue[thread_num].is_used == false) {
      request_queue[thread_num].is_used = true;//利用中に設定
      request_queue[thread_num].dm = this;
      request_queue[thread_num].api_name = api_name;
      request_queue[thread_num].user_data = user_data;
      request_queue[thread_num].requestor=api_requestor_;
      break;
    }
  }

  ADK_ASSERT(api_requestor_);
  ErrorHandler* error_handler = ApplicationManager::GetInstance()->GetErrorHandler();
  ADK_ASSERT(error_handler);
  error_handler->Clear();
  ret = api_requestor_->Request(api_name, request_data);
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
  IntPtr handle = adk::LIB_ThreadCreate(_threadFunction, &request_queue[thread_num], false);
  ADK_ASSERT(handle);
//  pthread_mutex_init(&data_manager_mutex, NULL);
//  pthread_create(&data_manager_thread, NULL, _threadFunction, &request_queue[thread_num]);
//  pthread_detach(data_manager_thread);
  
  return ret;
}

/**
 * \~english
 * \~japanese
 *   \brief リクエストが完了するまで待ち、リクエスト完了後イベント登録します。
 */
void ParameterUpdateManager::OnThread() {
  printf("in OnThread\n");
  
  //pthread_mutex_lock(&data_manager_mutex);
  LIB_MutexLock(handle_mutex_, false);

  Bool is_completed = false;
  
  for(int i=0;i<kMaxRequestQueue;i++) {
    if(request_queue[i].is_used == true) {
      while(1) {
        if(request_queue[i].requestor->IsComplete(request_queue[i].api_name)) {
          GameManager* game_manager = ApplicationManager::GetInstance()->GetGameManager();
          game_manager->RequestEventDataUpdateCompleted(true, request_queue[i].user_data);
          break;
        }
        //TODO 無限ループになるため一時的にbreakで抜けているが最後には削除する
        usleep(10);
      }
    }
  }
  LIB_MutexUnlock(handle_mutex_);
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
  ApplicationManager* application_manager = ApplicationManager::GetInstance();
  const ErrorHandler* error_handler = application_manager->GetErrorHandler();
  
  if (error_handler->GetSystemErrorCode()!=kSystemErrorSuccess)
  {
    // TODO. エラー処理
    if (error_handler->GetSystemErrorCode()==kSystemErrorCodeApplication)
    {
      //const TApplicationErrorInfo* app_error = error_handler->GetApplicationErrorInfo();
      //api_requestor_->OnError(code, (S32)app_error->application_error_code);
    }
    return ;
  }

  /* apiごとの処理 */
  switch (code) {
//    case kApiMyPage:
//      break;
//      
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
