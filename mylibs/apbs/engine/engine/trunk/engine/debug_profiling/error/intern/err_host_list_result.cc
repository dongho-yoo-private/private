/**
 * \file err_host_list_result.cc
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief ホストリストリザルト実装部
 */

#include <debug_profiling/error/intern/err_host_list_result.h>

#include <debug_profiling/error/err_hostlist_result_types.h>

ADK_BEGIN_NAMESPACE

/* extern function implementation */
const Char *GetHostListResultMessage(const Result &result) {
  const Char *ret = "no message";

  switch (result.GetFunc()) {
    case ERR_HOST_LIST_INITIALIZE: {
      switch (result.GetCode()) {
        case 0: {
          ret = "ADK_HostList::Initialize: Failed to System::GetSystem().";
          break;
        }
        case 1: {
          ret = "ADK_HostList::Initialize: Failed to system->GetStartupInfo().";
          break;
        }
      }
      break;
    }
    case ERR_HOST_LIST_SET_ACCESS_POINT: {
      switch (result.GetCode()) {
        case 0: {
          ret = "ADK_HostList::Initialize: Invalid variable = url.";
          break;
        }
        case 1: {
          ret = "ADK_HostList::Initialize: Failed to create url string.";
          break;
        }
      }
      break;
    }
  }

  return ret;
}

ADK_END_NAMESPACE

