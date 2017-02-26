/**
 * \file err_startup_info_result.cc
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief 起動設定リザルト実装部
 */

#include <debug_profiling/error/intern/err_startup_info_result.h>

ADK_BEGIN_NAMESPACE

/* extern function implementation */
const Char *GetStartupInfoResultMessage(const Result &result) {
  const Char *ret;

  switch (result.GetCode()) {
    case 0: {
      ret = "TestMessage";
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

