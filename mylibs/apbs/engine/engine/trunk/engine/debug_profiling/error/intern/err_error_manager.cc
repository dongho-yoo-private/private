/**
 * \file err_error_manager.cc
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief エラーマネージャ実装部
 */

#include <debug_profiling/error/err_error_manager.h>

#include <core/library/lib_string.h>
#include <debug_profiling/error/result_types.h>

ADK_BEGIN_NAMESPACE

/* class implementation */
/* constructor & destructor */
ERR_ErrorManager::ERR_ErrorManager()
    : _isErrorProcess(false), _isError(false) {}
ERR_ErrorManager::~ERR_ErrorManager() {}

/* initialize & terminate */
Result ERR_ErrorManager::Initialize() { return ResultSuccess; }
Result ERR_ErrorManager::Terminate() { return ResultSuccess; }

/* error handling manegement functionally */
Result ERR_ErrorManager::StartErrorProcess() {
  Result ret = ResultSuccess;

  if (_isErrorProcess != false) {
    ret = ResultDoAlready;
  } else {
    _isErrorProcess = true;
    _isError = false;
  }

  return ret;
}
Result ERR_ErrorManager::EndErrorProcess() {
  Result ret = ResultSuccess;

  if (_isErrorProcess == false) {
    ret = ResultNotInitialized;
  } else {
    _isErrorProcess = false;
  }

  return ret;
}
Result ERR_ErrorManager::SetErrorMessage(S32 code, const Char *message) {
  Result ret = ResultSuccess;

  if (_isErrorProcess == false) {
    ret = ResultNotInitialized;
  } else {
    _isError = true;
    _errorCode = code;
    LIB_strncpy(_errorMessage, message, kErrorMessageLen);
  }

  return ret;
}

ADK_END_NAMESPACE

