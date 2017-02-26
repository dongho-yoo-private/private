/**
 * \file err_api_result.cc
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief APIリザルト実装部
 */

#include <debug_profiling/error/err_api_result_types.h>

#include <debug_profiling/error/intern/err_api_result.h>

ADK_BEGIN_NAMESPACE

/* extern function implementation */
const Char *GetApiResultMessage(const Result &result) {
  const Char *ret = 0;

  switch (result.GetFunc()) {
    case ERR_API_GET_MASTER_SERVER_URL: {
      ret = "GetMasterServerUrl";
      break;
    }
    case ERR_API_GET_SERVER_COUNT: {
      switch (result.GetCode()) {
        case 1: {
          ret = "GetServerCount: Invalid variable for \"count\"\n";
          break;
        }
        default: { ret = "GetServerCount"; }
      }
      break;
    }
    case ERR_API_GET_SERVER_URL: {
      switch (result.GetCode()) {
        case 1: {
          ret = "GetServerUrl: Invalid variable for \"url\"";
          break;
        }
        default: { ret = "GetServerUrl"; }
      }
      break;
    }
    case ERR_API_GET_SERVER_DISPLAY_NAME: {
      switch (result.GetCode()) {
        case 1: {
          ret = "GetServerDisplayName: Invalid variable for \"name\"";
          break;
        }
        default: { ret = "GetServerDisplayName"; }
      }
      break;
    }
    case ERR_API_LOGIN: {
      ret = "Login";
      break;
    }
    case ERR_API_DISPATCH_PROCESS: {
      ret = "DispatchProcess";
      break;
    }
    case ERR_API_REPLACE_PHASE: {
      ret = "ReplacePhase";
      break;
    }
    case ERR_API_GET_SCHEMA: {
      ret = "GetSchema";
      break;
    }
    case ERR_API_IS_ERROR: {
      ret = "IsError";
      break;
    }
    case ERR_API_GET_ERROR: {
      ret = "GetError";
      break;
    }
    default: {
      ret = "no message";
      break;
    }
  }

  return ret;
}

ADK_END_NAMESPACE

