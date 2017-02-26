/**
 * \file err_result.cc
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief エラーリザルト実装部
 */

#include <debug_profiling/error/result.h>

#include <debug_profiling/error/result_types.h>
#include <debug_profiling/error/intern/err_api_result.h>
#include <debug_profiling/error/intern/err_dms_result.h>
#include <debug_profiling/error/intern/err_error_system_result.h>
#include <debug_profiling/error/intern/err_host_list_result.h>
#include <debug_profiling/error/intern/err_library_result.h>
#include <debug_profiling/error/intern/err_network_protocol_result.h>
#include <debug_profiling/error/intern/err_phase_result.h>
#include <debug_profiling/error/intern/err_request_manager_result.h>
#include <debug_profiling/error/intern/err_startup_info_result.h>
#include <debug_profiling/error/intern/err_system_result.h>

ADK_BEGIN_NAMESPACE

/* class implementation */
/* constructor & destructor */
Result::Result() {
  code_.bit.failure = false;
  code_.bit.result = 0;
  code_.bit.fatal = false;
  code_.bit.program = false;
  code_.bit.component = kNoneComponent;
  code_.bit.func = 0;
  code_.bit.code = 0;
}
Result::Result(Bool failure, U32 result, Bool fatal, Bool programError) {
  code_.bit.failure = failure;
  code_.bit.result = result;
  code_.bit.fatal = fatal;
  code_.bit.program = programError;
  code_.bit.component = kNoneComponent;
  code_.bit.func = 0;
  code_.bit.code = 0;
}
Result::Result(const Result &result, Bool fatal, Bool programError,
               U32 component, U32 func, U32 code) {
  code_.bit.failure = result.IsFailure();
  code_.bit.result = result.GetResult();
  code_.bit.fatal = fatal;
  code_.bit.program = programError;
  code_.bit.component = component;
  code_.bit.func = func;
  code_.bit.code = code;
}
Result::~Result() {}

/* output management functionally */
void Result::GetMessage(Char *dst, U32 dstlen) const {
  static const Char *kExsitingResult[] = {
      "Success",    "NotInitialized", "DoAlready",       "InvalidVariable",
      "OutOfRange", "OutOfResource",  "ConnectionError", "ConnectionTimeout"};
  static const Char *kComponentName[] = {
      "None",        "Api",     "Library",     "System",
      "ErrorSystem", "Log",     "StartupInfo", "NetworkProtocol",
      "HostList",    "Request", "DMS",         "Phase"};

  const Char *msg = "no message";
  const U32 component = this->GetComponent();

  if (component != kNoneComponent) {
    const Char *(*MessageFuncs[kMaxComponent])(const Result &result) = {
        0,                               GetApiResultMessage,
        GetLibraryResultMessage,         GetSystemResultMessage,
        GetErrorSystemResultMessage,     GetStartupInfoResultMessage,
        GetNetworkProtocolResultMessage, GetHostListResultMessage,
        GetRequestManagerResultMessage,  GetDmsResultMessage,
        GetPhaseResultMessage, };
    msg = MessageFuncs[component](*this);
  }

  snprintf(dst, dstlen, "%s / %s / %s", kComponentName[component],
           kExsitingResult[this->GetResult()], msg);
}

ADK_END_NAMESPACE

