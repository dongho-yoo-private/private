/**
 * \file adk_request_manager.cc
 * \ingroup adkgrp_request
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief リクエストマネージャ
 */

#include <gameplay/http_data_interface/request/adk_request_manager.h>

#include <errno.h>
#include <unistd.h> /* used for usleep() */

#include <core/debug/debug_assert.h>
#include <core/debug/logging.h>
#include <core/library/lib_string.h>
#include <debug_profiling/error/err_request_result_types.h>
#include <gameplay/http_data_interface/system/system.h>

#if 1
/* static function implementation */
static void *NetworkThread(void *data) {
  adk::System *system = adk::System::GetSystem();

  for (; system->IsDoTerm() == false;) {
    system->DoRequestProcess();
    usleep(1);
  }

  return NULL;
}
#endif

ADK_BEGIN_NAMESPACE

/* request */
/**
 * \~english
 * \~japanese
 *   \brief リクエストキュー
 */
typedef struct RequestQueue {
  IRequest *request;
  DMS_Schema *schema;
} RequestQueue;

/* class implementation */
/* constructor & destructor */
RequestManager::RequestManager() : _requests(0) {
  network_.thread.initialized = false;
  _request.queue.initmutex = false;
  memset(&httpHeader_, 0, sizeof(httpHeader_));
  common_.paramCount = 0;
  common_.request = 0;
}
RequestManager::~RequestManager() {}

/* initialize & terminate */
Result RequestManager::Initialize() {
  Result ret = ResultSuccess;

  if (_requests != 0) {
    ret = ADK_RESULT_PROGRAM(ResultDoAlready, kRequestComponent,
                             kRequestInterface,
                             ERR_REQUEST_MANAGER_INITIALIZE_DO_ALREADY);
    goto initerr;
  }
  _requests = LIB_ListCreate();
  if (_requests == 0) {
    ret =
        ADK_RESULT_PROGRAM(ResultUnknown, kRequestComponent, kRequestInterface,
                           0 /*ERR_REQUEST_MANAGER_INITIALIZE_UNKNOWN*/);
    goto initerr;
  }

  if (_request.queue.initmutex != false) {
    ret = ADK_RESULT_PROGRAM(ResultDoAlready, kRequestComponent,
                             kRequestInterface,
                             ERR_REQUEST_MANAGER_INITIALIZE_DO_ALREADY_MUTEX);
    goto initerr;
  } else {
    S32 result = pthread_mutex_init(&_request.queue.mutex, 0);
    if (result != 0) {
      ret = ADK_RESULT_PROGRAM(
          ResultUnknown, kRequestComponent, kRequestInterface,
          ERR_REQUEST_MANAGER_INITIALIZE_FAILED_TO_INIT_MUTEX);
      goto initerr;
    } else {
      _request.queue.initmutex = true;
    }
  }
#if 1
  if (network_.thread.initialized != false) {
    ret = ADK_RESULT(ResultDoAlready, true, false, kRequestComponent, 0, 0);
    goto initerr;
  } else {
    S32 result = pthread_create(&network_.thread.thread, 0, NetworkThread, 0);
    if (result != 0) {
      ret = ADK_RESULT(ResultUnknown, true, false, kRequestComponent, 0, 0);
      goto initerr;
    } else {
      // pthread_detach( network_.thread.thread);
      network_.thread.initialized = true;
    }
  }
#endif
  if (httpHeader_.data.binary != 0) {
    ret = ADK_RESULT_PROGRAM(
        ResultDoAlready, kRequestComponent, kRequestInterface,
        ERR_REQUEST_MANAGER_INITIALIZE_DO_ALREADY_HEADER_BINARY);
    goto initerr;
  } else {
    System *system = System::GetSystem();
    ADK_ASSERT(system);
    httpHeader_.data.key = system->GetHttpKeyLength();
    httpHeader_.data.value = system->GetHttpValueLength();
    httpHeader_.data.once = httpHeader_.data.key * httpHeader_.data.value;
    httpHeader_.data.size = httpHeader_.data.once * ADK_HTTP_REQUEST_HEADER_MAX;
    httpHeader_.data.binary =
        (Byte *)ADK_Calloc(1, httpHeader_.data.size, "HeaderData");
    if (httpHeader_.data.binary == 0) {
      ret = ADK_RESULT_PROGRAM(
          ResultUnknown, kRequestComponent, kRequestInterface,
          ERR_REQUEST_MANAGER_INITIALIZE_FAILED_TO_ALLOC_HEADER_DATA);
      goto initerr;
    }
    httpHeader_.headers = (TRequestHttpHeader *)ADK_Calloc(
        1, sizeof(TRequestHttpHeader) * ADK_HTTP_REQUEST_HEADER_MAX,
        "HttpHeaders");
    if (httpHeader_.headers == 0) {
      ret = ADK_RESULT_PROGRAM(
          ResultUnknown, kRequestComponent, kRequestInterface,
          ERR_REQUEST_MANAGER_INITIALIZE_FAILED_TO_ALLOC_HEADERS);
      goto initerr;
    } else {
      TRequestHttpHeader *header = httpHeader_.headers;
      for (S32 index = 0; index < ADK_HTTP_REQUEST_HEADER_MAX;
           ++index, ++header) {
        Byte *pos = httpHeader_.data.binary + (httpHeader_.data.once * index);
        header->key = (Char *)pos;
        header->value = (Char *)(pos + httpHeader_.data.key);
      }
    }
  }

  return ret;

initerr:

  Terminate();

  return ret;
}
Result RequestManager::Terminate() {
  Result ret = ResultSuccess;

  if (httpHeader_.headers != 0) {
    ADK_Free(httpHeader_.headers), httpHeader_.headers = 0;
  }
  if (httpHeader_.data.binary != 0) {
    ADK_Free(httpHeader_.data.binary), httpHeader_.data.binary = 0;
  }
  if (common_.request != 0) {
    ADK_Free(common_.request), common_.request = 0;
  }
  if (network_.thread.initialized == false) {
    ret =
        ADK_RESULT(ResultNotInitialized, true, false, kRequestComponent, 0, 0);
  } else {
    int result = pthread_join(network_.thread.thread, 0);
    if (result != 0) {
      switch (result) {
        case ESRCH:
          printf("ESRCH\n");
          break;
        case EINVAL:
          printf("EINVAL\n");
          break;
        case EDEADLK:
          printf("EDEADLK\n");
          break;
      }
    }
    network_.thread.initialized = false;
  }

  if (_request.queue.initmutex == false) {
    ret =
        ADK_RESULT(ResultNotInitialized, true, false, kRequestComponent, 0, 0);
  } else {
    pthread_mutex_destroy(&_request.queue.mutex);
    _request.queue.initmutex = false;
  }
  if (_requests != 0) {
    LIB_ListDestroy(_requests);
  }

  return ret;
}

/* request management functionally */
Result RequestManager::Request(const Char *url, IRequest::TRequestType reqtype,
                               DMS_Schema *schema) {
  Result ret = ResultSuccess;
  RequestQueue *queue = 0;
  IRequest *request = 0;
  S32 result = 0;

  if (schema == 0) {
    ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kRequestComponent,
                             kRequestInterface,
                             ERR_REQUEST_MANAGER_REQUEST_INVALID_VARIABLE);
    goto reqerr;
  }

  request = new IRequest();
  ADK_ASSERT(request);
  if (request == 0) {
    ret = ADK_RESULT_PROGRAM(
        ResultOutOfResource, kRequestComponent, kRequestInterface,
        ERR_REQUEST_MANAGER_REQUEST_OUT_OF_RESOURCE_REQUEST_OBJECT);
    goto reqerr;
  }

  ret = request->Initialize(url, reqtype, schema->GetRequestData());
  if (ret.IsSuccess() == false) {
    goto reqerr;
  }

  queue = (RequestQueue *)ADK_Malloc(sizeof(RequestQueue), "RequestQueue");
  if (queue == 0) {
    ret = ADK_RESULT_PROGRAM(
        ResultOutOfResource, kRequestComponent, kRequestInterface,
        ERR_REQUEST_MANAGER_REQUEST_OUT_OF_RESOURCE_REQUEST_QUEUE);
    goto reqerr;
  }

  pthread_mutex_lock(&_request.queue.mutex);
  queue->request = request;
  queue->schema = schema;
  result = LIB_ListAdd(_requests, queue);
  pthread_mutex_unlock(&_request.queue.mutex);
  if (result == 0) {
    ret =
        ADK_RESULT_PROGRAM(ResultUnknown, kRequestComponent, kRequestInterface,
                           ERR_REQUEST_MANAGER_REQUEST_UNKNOWN_MUTEX_UNLOCK);
    goto reqerr;
#if 1 /* DoRequestProcess() \
         をシステムで呼び出すようになったのでココでは呼び出さない */
  }
#else
  } else {
    DoRequestProcess();
  }
#endif

  return ret;

reqerr:

  if (request != 0) {
    request->Terminate();
    delete request, request = 0;
  }

  if (queue != 0) {
    ADK_Free(queue), queue = 0;
  }

  return ret;
}
void RequestManager::DoRequestProcess() {
  TListNode *first;
  RequestQueue *queue = 0;

  first = LIB_ListGetTopFrom(_requests, 0);
  if (first != 0) {
    queue = (RequestQueue *)first->ref;
    pthread_mutex_lock(&_request.queue.mutex);
    LIB_ListRemove(_requests, first);
    pthread_mutex_unlock(&_request.queue.mutex);
  }

  if (queue != 0) {
    System *system = System::GetSystem();
    ADK_ASSERT(system);
    IRequest *request = queue->request;
    ADK_ASSERT(request);
    DMS_Schema *schema = queue->schema;
    ADK_ASSERT(schema);
    Bool isError = false;

    schema
        ->ResetRequest(); /* とりあえずフェーズ機構が無効なのでリセットする事にする
                             */

    Result result = request->DoRequest();
    if (result.IsSuccess() == false) {
      /* リクエスト処理に失敗した */
      system->SetErrorMessage(0xffffffff, "request error.");
      isError = true;
    } else {
      if (request->GetStatus() != 200) {
#if 1
        system->SetErrorMessage(request->GetStatus(), "Failed to request");
        /*
         * 通信に失敗した場合でもレスポンスデータ(JSON)はあるので
         * 共通パラメータのみパースする
         */
        if (request->GetResponseData() != 0) {
          INFO_LOG("Response binary info = \n");
#if 0
          BINARY_LOG((const Char *)request->GetResponseData());
#endif
          /* 先にCommonスキーマをリセットする */
          DMS *dms = system->GetDMS();
          DMS_Schema *common = dms->GetCommonResponseSchema();
          if (common != 0) {
            common->DisposeResponseData();
          }

          result = schema->ParseResponseData(
              request->GetResponseData(), request->GetResponseDataSize() + 1);
        }
#else
#if 1
        system->SetErrorMessage(request->GetStatus(), "Failed to request");
#else
        /* リクエストは成功したが、どうも不明なリクエストだった...etc */
        if (request->GetResponseData() != 0) {
          _error.jdoc.Release();
          result =
              _error.jdoc.Parse((Char *)request->GetResponseData(), false, 0);
          if (result.IsSuccess() == false) {
            system->SetErrorMessage(request->GetStatus(), "response error.");
          } else {
            LIB_TJsonMember &json_result = _error.jdoc["result"];
            LIB_TJsonMember &json_message = _error.jdoc["message"];
            if (json_result.data.IsValid() != false &&
                json_message.data.IsValid() != false) {
              S32 result_code = json_result.data.GetS32();
              const Char *result_message = json_message.data.GetString();
              system->SetErrorMessage(result_code, result_message);
            }
          }
        }
#endif
#endif
        isError = true;
      } else {
        INFO_LOG("Response binary info = \n");
#if 0
        ADK_BINARY_LOG((const Char *)request->GetResponseData());
#endif
        /* 先にCommonスキーマをリセットする */
        DMS *dms = system->GetDMS();
        DMS_Schema *common = dms->GetCommonResponseSchema();
        if (common != 0) {
          common->DisposeResponseData();
        }
        /*
         * ここで渡すレスポンスデータのサイズには、
         * レスポンスデータが持つ終端の '\0' を含める
         */
        result = schema->ParseResponseData(request->GetResponseData(),
                                           request->GetResponseDataSize() + 1);
      }
    }
    request->Terminate();
    delete request, request = 0;

    if (isError != false) {
      schema->DoRequestInterruption();
    }

    schema->DoRequestEnd();

    ADK_Free(queue), queue = 0;
  }
}

/* http management functionally */
Result RequestManager::AddJsonCommonParameter(const Char *key,
                                              const Char *value) {
  Result ret = ResultSuccess;
  TJsonCommonParameter *param = FindJsonCommonParameter(key);
  if (param == 0) {
    param = CreateJsonCommonParameter(key);
  }
  if (param == 0) {
    ret = ResultOutOfRange;
  } else {
    strcpy(param->data.string, value);
    param->data.type = 0;
    CreateJsonCommonParameterString();
  }

  return ret;
}
Result RequestManager::AddJsonCommonParameter(const Char *key, S32 value) {
  Result ret = ResultSuccess;
  TJsonCommonParameter *param = FindJsonCommonParameter(key);
  if (param == 0) {
    param = CreateJsonCommonParameter(key);
  }
  if (param == 0) {
    ret = ResultOutOfRange;
  } else {
    param->data.i32 = value;
    param->data.type = 1;
    CreateJsonCommonParameterString();
  }

  return ret;
}
Result RequestManager::AddJsonCommonParameter(const Char *key, U64 value) {
  Result ret = ResultSuccess;
  TJsonCommonParameter *param = FindJsonCommonParameter(key);
  if (param == 0) {
    param = CreateJsonCommonParameter(key);
  }
  if (param == 0) {
    ret = ResultOutOfRange;
  } else {
    param->data.u64 = value;
    param->data.type = 2;
    CreateJsonCommonParameterString();
  }

  return ret;
}
Result RequestManager::ApplyHttpRequestHeader(const Char *key,
                                              const Char *value) {
  Result ret = ResultSuccess;
  TRequestHttpHeader *header = httpHeader_.headers;
  S32 count = 0;
  for (count = 0; count < ADK_HTTP_REQUEST_HEADER_MAX; ++count, ++header) {
    if (header->key[0] == '\0' || LIB_strcmp(header->key, key) != 0) {
      continue;
    }
    strcpy(header->key, key);
    strcpy(header->value, value);
    break;
  }
  if (count == ADK_HTTP_REQUEST_HEADER_MAX) {
    header = &httpHeader_.headers[httpHeader_.count];
    strcpy(header->key, key);
    strcpy(header->value, value);
    ++httpHeader_.count;
  }

  return ret;
}

/* http management functionally */
TJsonCommonParameter *RequestManager::FindJsonCommonParameter(const Char *key) {
  TJsonCommonParameter *param = common_.params;
  S32 count = 0;
  for (count = 0; count < ADK_JSON_COMMON_PARAMETER_MAX; ++count, ++param) {
    if (param->key[0] == '\0') continue;
    if (LIB_strcmp(param->key, key) == 0) return param;
  }

  return 0;
}
TJsonCommonParameter *RequestManager::CreateJsonCommonParameter(
    const Char *key) {
  TJsonCommonParameter *ret = 0;
  ADK_ASSERT(common_.paramCount < ADK_JSON_COMMON_PARAMETER_MAX);
  if (common_.paramCount == ADK_JSON_COMMON_PARAMETER_MAX) {
    ret = 0;
  } else {
    ret = &common_.params[common_.paramCount];
    strcpy(ret->key, key);
    ++common_.paramCount;
  }

  return ret;
}
Result RequestManager::CreateJsonCommonParameterString() {
  adk::Result ret = adk::ResultSuccess;

  if (common_.request != 0) {
    ADK_Free(common_.request), common_.request = 0;
  }

  if (common_.paramCount > 0) {
    adk::LIB_JsonDocument jdoc;
    TJsonCommonParameter *param = common_.params;
    U32 jsonlen;

    jdoc.BeginObject();
    for (S32 index = 0; index < common_.paramCount; ++index, ++param) {
      if (param->data.type == 0) {
        jdoc.AddString(param->key, param->data.string);
      } else if (param->data.type == 1) {
        jdoc.AddS32(param->key, param->data.i32);
      } else if (param->data.type == 2) {
        jdoc.AddU64(param->key, param->data.u64);
      }
    }
    jdoc.EndObject((U32)common_.paramCount);
    jdoc.Assign();

    ret = jdoc.CreateStringWithoutTopObject(&common_.request, &jsonlen);
    jdoc.Release();
  }

  return ret;
}

ADK_END_NAMESPACE

