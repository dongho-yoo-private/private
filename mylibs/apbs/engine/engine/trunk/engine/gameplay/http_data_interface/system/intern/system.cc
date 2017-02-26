/**
 * \file system.cc
 * \ingroup adkgrp_system
 * \author tomonori tanaka
 * \~english
 *   \brief ADK System
 * \~japanese
 *   \brief ADK システム
 */

#include <gameplay/http_data_interface/system/system.h>

#include <core/debug/debug_assert.h>
#include <core/debug/logging.h>
#include <core/library/lib_string.h>
#include <debug_profiling/error/err_system_result_types.h>
#include <network/http/adk_np_http_packet.h>

ADK_BEGIN_NAMESPACE

/* static function implementation */
/**
 * \~english
 * \~japanese
 *   \brief メモリスレッドのロックコールバック
 */
static void MemoryLock(void) {
  System *system = System::GetSystem();
  ADK_ASSERT(system);

  system->DoMemoryLock();
}
/**
 * \~english
 * \~japanese
 *   \brief メモリスレッドのアンロックコールバック
 */
static void MemoryUnlock(void) {
  System *system = System::GetSystem();
  ADK_ASSERT(system);

  system->DoMemoryUnlock();
}
#if 0
/**
 * \~english
 * \~japanese
 *   \brief メモリアロケータ用デバッグ出力
 */
static void MemoryPrintError( const Char *str) {
  DEBUG_LOG( str);
}
#endif

/* class implementation */
System *System::system_ = 0;
Result System::Initialize() {
  Result ret = ResultSuccess;

  if (system_ != 0) {
    ret = ADK_RESULT_PROGRAM(ResultDoAlready, kSystemComponent,
                             ERR_SYSTEM_INITIALIZE, 0);
  } else {
    system_ = new System;
    if (system_ == 0) {
      ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kSystemComponent,
                               ERR_SYSTEM_INITIALIZE, 1);
    } else {
      ret = system_->Init();
      if (ret.IsSuccess() == false) {
        delete system_, system_ = 0;
      }
    }
  }

  return ret;
}
Result System::Terminate() {
  Result ret = ResultSuccess;

  if (system_ == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kSystemComponent,
                             ERR_SYSTEM_TERMINATE, 0);
  } else {
    ret = system_->Term();
    delete system_, system_ = 0;
  }

  return ret;
}
System *System::GetSystem() { return system_; }

/* constructor & destructor */
System::System()
    : is_do_terminate_(false),
      startup_(NULL),
      hostlist_(NULL),
      _dms(NULL),
      request_manager_(NULL),
      error_manager_(NULL) {
  memory_.initialized = false;
  thread_.initialized = false;
  memset(&network_protocol_, 0, sizeof(network_protocol_));
}
System::~System() {}

/* initialize & terminate */
Result System::Init() {
  Result ret = ResultSuccess;
  S32 result;

  //  ADK_SetPrintErrorCallback( MemoryPrintError);

  if (memory_.initialized != false || thread_.initialized != false ||
      startup_ != 0 || hostlist_ != 0 || _dms != 0 || request_manager_ != 0 ||
      error_manager_ != 0 ||
      network_protocol_.protocol != 0) {
    ret = ADK_RESULT_PROGRAM(ResultDoAlready, kSystemComponent, ERR_SYSTEM_INIT,
                             0);
    goto errend;
  }

  result = pthread_mutex_init(&memory_.mutex, 0);
  if (result != 0) {
    ret =
        ADK_RESULT_PROGRAM(ResultUnknown, kSystemComponent, ERR_SYSTEM_INIT, 1);
    goto errend;
  } else {
    ADK_SetLockCallback(MemoryLock, MemoryUnlock);
    memory_.initialized = true;
  }

  result = Log::Initialize();
  if (result == false) {
    goto errend;
  }

  startup_ = new ADK_StartupInfo;
  if (startup_ == 0) {
    ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kSystemComponent,
                             ERR_SYSTEM_INIT, 2);
    goto errend;
  } else {
    ret = startup_->Initialize("adk/adk_startup.adkinfo");
    if (ret.IsSuccess() == false) {
      goto errend;
    }
  }

  error_manager_ = new ERR_ErrorManager;
  if (error_manager_ == 0) {
    ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kSystemComponent,
                             ERR_SYSTEM_INIT, 2);
  } else {
    ret = error_manager_->Initialize();
    if (ret.IsSuccess() == false) {
      goto errend;
    }
  }

  hostlist_ = new ADK_HostList;
  if (hostlist_ == 0) {
    ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kSystemComponent,
                             ERR_SYSTEM_INIT, 3);
    goto errend;
  } else {
    ret = hostlist_->Initialize();
    if (ret.IsSuccess() == false) {
      goto errend;
    }
  }

  _dms = new DMS;
  if (_dms == 0) {
    ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kSystemComponent,
                             ERR_SYSTEM_INIT, 4);
    goto errend;
  } else {
    ret = _dms->Initialize();
    if (ret.IsSuccess() == false) {
      goto errend;
    }
  }

  request_manager_ = new RequestManager;
  if (_dms == 0) {
    ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kSystemComponent,
                             ERR_SYSTEM_INIT, 5);
    goto errend;
  } else {
    ret = request_manager_->Initialize();
    if (ret.IsSuccess() == false) {
      goto errend;
    }
  }

  network_protocol_.protocol = new NetworkProtocol;
  if (network_protocol_.protocol == 0) {
    ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kSystemComponent,
                             ERR_SYSTEM_INIT, 7);
    goto errend;
  } else {
    ret = network_protocol_.protocol->Initialize();
    if (ret.IsSuccess() == false) {
      goto errend;
    }
  }

  result = pthread_mutex_init(&thread_.mutex, 0);
  if (result != 0) {
    ret = ADK_RESULT_PROGRAM(ResultDoAlready, kSystemComponent, ERR_SYSTEM_INIT,
                             8);
    goto errend;
  } else {
    thread_.initialized = true;
  }

  return ret;

errend:

  Term();

  return ret;
}
Result System::Term() {
  Result ret = ResultSuccess;

  is_do_terminate_ = true;

  if (thread_.initialized == false) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kSystemComponent,
                             ERR_SYSTEM_TERM, 0);
  } else {
    pthread_mutex_destroy(&thread_.mutex);
    thread_.initialized = false;
  }

  if (network_protocol_.protocol == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kSystemComponent,
                             ERR_SYSTEM_TERM, 1);
  } else {
    ret = network_protocol_.protocol->Terminate(true);
    delete network_protocol_.protocol, network_protocol_.protocol = 0;
  }

  if (request_manager_ == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kSystemComponent,
                             ERR_SYSTEM_TERM, 3);
  } else {
    ret = request_manager_->Terminate();
    delete request_manager_, request_manager_ = 0;
  }

  if (_dms == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kSystemComponent,
                             ERR_SYSTEM_TERM, 4);
  } else {
    ret = _dms->Terminate();
    delete _dms, _dms = 0;
  }

  if (hostlist_ == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kSystemComponent,
                             ERR_SYSTEM_TERM, 5);
  } else {
    ret = hostlist_->Terminate();
    delete hostlist_, hostlist_ = 0;
  }

  if (error_manager_ == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kSystemComponent,
                             ERR_SYSTEM_TERM, 5);
  } else {
    ret = error_manager_->Terminate();
    delete error_manager_, error_manager_ = 0;
  }

  if (startup_ == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kSystemComponent,
                             ERR_SYSTEM_TERM, 5);
  } else {
    ret = startup_->Terminate();
    delete startup_, startup_ = 0;
  }

  Log::Terminate();

  if (memory_.initialized != false) {
    ADK_SetLockCallback(0, 0);
    pthread_mutex_destroy(&memory_.mutex);
    memory_.initialized = false;
  }

  is_do_terminate_ = false;

  return ret;
}

/* ADK process */
Result System::Login(const Char *host) { return SetAccessPoint(host); }
Result System::DispatchProcess() {
  Result ret = ResultSuccess;
  return ret;
}

/* host list management functionally */
Result System::SetAccessPoint(const Char *url) {
  return hostlist_->SetAccessPoint(url);
}

/* request management functionally */
Result System::Request(const Char *url, IRequest::TRequestType reqtype,
                       DMS_Schema *schema) {
  Result ret = request_manager_->Request(url, reqtype, schema);
  if (ret.IsSuccess()) {
#if 1
    while (schema->IsRequestCompleted() == false) {
      usleep(100);
    }
    /* 通信処理が中断されていたら失敗とする。 */
    if (schema->IsInterrupte() != false) {
      ret = ResultUnknown;
    }
    if (ret.IsSuccess() == false) {
      schema->DisposeResponseData();
    }
#else
    ret = request_manager_->DoRequestProcess();
#endif
  }
  return ret;
}
Result System::RequestAsync(const Char *url, IRequest::TRequestType reqtype,
                            DMS_Schema *schema) {
  Result ret = request_manager_->Request(url, reqtype, schema);
  return ret;
}
void System::DoRequestProcess() {
  pthread_mutex_lock(&thread_.mutex);
  request_manager_->DoRequestProcess();
  pthread_mutex_unlock(&thread_.mutex);
}

/* memory management functionally */
void System::DoMemoryLock() { pthread_mutex_lock(&memory_.mutex); }
void System::DoMemoryUnlock() { pthread_mutex_unlock(&memory_.mutex); }

/* DMS management functionally */
DMS_Schema *System::GetSchema(const Char *url) {
  DMS_Schema *ret = 0;

  if (_dms != 0) {
    ret = _dms->FindSchema(url);
  }

  return ret;
}
DMS_Schema *System::GetCommonResponseSchema() {
  DMS_Schema *ret = 0;

  if (_dms != 0) {
    ret = _dms->GetCommonResponseSchema();
  }

  return ret;
}
void System::ResetRequestSchema() { _dms->ResetRequestSchema(); }

/* network protocol managment functionally */
Result System::RequestToWas(const HttpResponse **response, const Char *url,
                            const void *data, U32 datasize) {
  Result ret = ResultSuccess;
  if (response == 0) {
    ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kSystemComponent,
                             ERR_SYSTEM_REQUEST_TO_WAS, 0);
  } else if (url == 0) {
    ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kSystemComponent,
                             ERR_SYSTEM_REQUEST_TO_WAS, 1);
  } else {
    const S32 headerCount = request_manager_->GetHttpRequestHeaderCount();
    const TRequestHttpHeader *headers =
        request_manager_->GetHttpRequestHeader();
    NpHttpPacket packet(url, data, datasize, headers, headerCount);

    ret = network_protocol_.protocol->Request(&packet, 10000);
    if (ret.IsSuccess() != false) {
      *response = packet.GetHttpResponse();
      if (*response == 0) {
        ret = ResultUnknown;
      }
    }
  }

  return ret;
}
Result System::DisposeRequestToWas(const HttpResponse *response) {
  Result ret = ResultSuccess;

  if (response == 0) {
    ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kSystemComponent,
                             ERR_SYSTEM_DISPOSE_REQUEST_TO_WAS, 0);
  } else {
    delete response, response = 0;
  }

  return ret;
}

/* error handling management functionally */
Result System::StartErrorProcess() {
  return error_manager_->StartErrorProcess();
}
Result System::EndErrorProcess() { return error_manager_->EndErrorProcess(); }
Bool System::IsError() { return error_manager_->IsError(); }
Result System::SetErrorMessage(S32 code, const Char *message) {
  return error_manager_->SetErrorMessage(code, message);
}
Result System::GetErrorMessage(S32 *code, Char *message, U32 messageLen) const {
  Result ret = ResultSuccess;

  if (code == 0 || message == 0) {
    ret = ResultInvalidVariable;
  } else {
    *code = error_manager_->GetErrorCode();
    LIB_strncpy(message, error_manager_->GetErrorMessage(), messageLen);
  }

  return ret;
}
/* http management functionally */
Result System::AddJsonCommonParameter(const Char *key, const Char *value) {
  return request_manager_->AddJsonCommonParameter(key, value);
}
Result System::AddJsonCommonParameter(const Char *key, S32 value) {
  return request_manager_->AddJsonCommonParameter(key, value);
}
Result System::AddJsonCommonParameter(const Char *key, U64 value) {
  return request_manager_->AddJsonCommonParameter(key, value);
}
Result System::ApplyHttpRequestHeader(const Char *key, const Char *value) {
  return request_manager_->ApplyHttpRequestHeader(key, value);
}
Result System::RequestPostHttp(const HttpResponse **data, const Char *url,
                               Bool user_request_header) {
#if 1
  Result ret = ResultSuccess;
  if (data == 0 || url == 0) {
    ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kSystemComponent,
                             ERR_SYSTEM_REQUEST_POST_HTTP, 0);
  } else {
    if (user_request_header != false) {
      const S32 headerCount = request_manager_->GetHttpRequestHeaderCount();
      const TRequestHttpHeader *headers =
          request_manager_->GetHttpRequestHeader();
      NpHttpPacket packet(url, 0, 0, headers, headerCount);

      ret = network_protocol_.protocol->Request(&packet, 10000);
      if (ret.IsSuccess() != false) {
        *data = packet.GetHttpResponse();
        if (*data == 0) {
          ret = ResultUnknown;
        }
      }
    } else {
      NpHttpPacket packet(url);

      ret = network_protocol_.protocol->Request(&packet, 10000);
      if (ret.IsSuccess() != false) {
        *data = packet.GetHttpResponse();
        if (*data == 0) {
          ret = ResultUnknown;
        }
      }
    }
  }

  return ret;
#else
  return request_manager_->RequestPostHttp(data, url);
#endif
}

/* debug management functionally */
void System::SetDebugLogLevel(TLogLevel level) {
  Log *log = Log::GetInstance();
  ADK_ASSERT(log);
  log->setLogLevel(level);
}

ADK_END_NAMESPACE

