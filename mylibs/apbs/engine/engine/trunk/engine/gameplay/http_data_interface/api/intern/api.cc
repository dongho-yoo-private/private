/**
 * \file adk-api.cc
 * \ingroup adkgrp_api
 * \author tomonori tanaka
 * \~english
 *   \brief ADK Application interface
 * \~japanese
 *   \brief ADK アプリケーションインターフェース
 */

#include <gameplay/http_data_interface/api/api.h>

#include <core/library/lib_string.h>
#include <debug_profiling/error/err_api_result_types.h>
#include <gameplay/http_data_interface/system/system.h>

ADK_BEGIN_NAMESPACE

/* extern function implementation */
/* initialize & terminate */
Result HttpDataInterface::Initialize() { return System::Initialize(); }
Result HttpDataInterface::Terminate() { return System::Terminate(); }

/* Access point management functionally */
Result HttpDataInterface::GetMasterServerUrl(Char *url, U32 urllen) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_GET_MASTER_SERVER_URL, 0);
  } else {
    LIB_strncpy(url, system->GetMasterServerUrl(), urllen);
  }

  return ret;
}
Result HttpDataInterface::GetServerCount(S32 *count) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_GET_SERVER_COUNT, 0);
  } else {
    if (count == 0) {
      ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kApiComponent,
                               ERR_API_GET_SERVER_COUNT, 1);
    } else {
      *count = system->GetServerCount();
    }
  }

  return ret;
}
Result HttpDataInterface::GetServerUrl(Char *url, U32 urllen, S32 server) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_GET_SERVER_URL, 0);
  } else {
    if (url == 0) {
      ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kApiComponent,
                               ERR_API_GET_SERVER_URL, 1);
    } else {
      LIB_strncpy(url, system->GetServerUrl(server), urllen);
    }
  }

  return ret;
}
Result HttpDataInterface::GetServerDisplayName(Char *name, U32 namelen,
                                               S32 server) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_GET_SERVER_DISPLAY_NAME, 0);
  } else {
    if (name == 0) {
      ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kApiComponent,
                               ERR_API_GET_SERVER_DISPLAY_NAME, 1);
    } else {
      LIB_strncpy(name, system->GetServerDisplayName(server), namelen);
    }
  }

  return ret;
}

/* ADK process */
Result HttpDataInterface::Login(const Char *host) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent, ERR_API_LOGIN,
                             0);
  } else {
    ret = system->Login(host);
  }

  return ret;
}
Result HttpDataInterface::DispatchProcess() {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_DISPATCH_PROCESS, 0);
  } else {
    ret = system->DispatchProcess();
  }

  return ret;
}

/* DMS management functionally */
DMS_Schema *HttpDataInterface::GetSchema(const Char *url) {
  DMS_Schema *ret = 0;
  System *system = System::GetSystem();

  if (system != 0) {
    ret = system->GetSchema(url);
  }

  return ret;
}
/* error handling management functionally */
Bool HttpDataInterface::IsError() {
  Bool ret = false;
  System *system = System::GetSystem();

  if (system != 0) {
    ret = system->IsError();
  }

  return ret;
}
Result HttpDataInterface::GetError(S32 *code, Char *message, U32 messageLen) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_GET_ERROR, 0);
  } else {
    ret = system->GetErrorMessage(code, message, messageLen);
  }

  return ret;
}
/* http management functionally */
Result HttpDataInterface::AddJsonCommonParameter(const Char *key,
                                                 const Char *value) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_ADD_JSON_COMMON_PARAMETER, 0);
  } else {
    ret = system->AddJsonCommonParameter(key, value);
  }

  return ret;
}
Result HttpDataInterface::AddJsonCommonParameter(const Char *key, S32 value) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_ADD_JSON_COMMON_PARAMETER, 0);
  } else {
    ret = system->AddJsonCommonParameter(key, value);
  }

  return ret;
}
Result HttpDataInterface::AddJsonCommonParameter(const Char *key, U64 value) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_ADD_JSON_COMMON_PARAMETER, 0);
  } else {
    ret = system->AddJsonCommonParameter(key, value);
  }

  return ret;
}
Result HttpDataInterface::ApplyHttpRequestHeader(const Char *key,
                                                 const Char *value) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_APPLY_HTTP_REQUEST_HEADER, 0);
  } else {
    ret = system->ApplyHttpRequestHeader(key, value);
  }

  return ret;
}
Result HttpDataInterface::RequestPostHttp(const HttpResponse **data,
                                          const Char *url,
                                          Bool user_request_header) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_REQUEST_POST_HTTP, 0);
  } else {
    ret = system->RequestPostHttp(data, url, user_request_header);
  }

  return ret;
}

/* debug management functionally */
Result HttpDataInterface::SetDebugLogLevel(TLogLevel level) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kApiComponent,
                             ERR_API_SET_DEBUG_LOG_LEVEL, 0);
  } else {
    system->SetDebugLogLevel(level);
  }

  return ret;
}

ADK_END_NAMESPACE

