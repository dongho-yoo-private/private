/**
 * \file err_dms_result.cc
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief データマネージメントシステムエラー実装部
 */

#include <debug_profiling/error/intern/err_dms_result.h>

#include <debug_profiling/error/err_dms_result_types.h>

ADK_BEGIN_NAMESPACE

/* extern function implementation */
const Char *GetDmsResultMessage(const Result &result) {
  const Char *ret = "message";

  switch (result.GetFunc()) {
    case ERR_DMS_INITIALIZE: {
      switch (result.GetCode()) {
        case 0: {
          ret = "DMS::Initialize: Found \"/adk/schema\" directory.";
          break;
        }
        case 1: {
          ret = "DMS::Initialize: Failed to schemas memory alloc.";
          break;
        }
      }
      break;
    }
    case ERR_DMS_TERMINATE: {
      ret = "DMS::Terminate";
      break;
    }
    case ERR_DMS_INIT: {
      ret = "DMS::Init";
      break;
    }
    case ERR_DMS_REQUEST_TO_WAS: {
      ret = "DMS::RequestToWas";
      break;
    }
    case ERR_DMS_DISPOSE_REQUEST_TO_WAS: {
      ret = "DMS::DisposeRequestToWas";
      break;
    }
    case ERR_DMS_JSON_PARSE: {
      switch (result.GetCode()) {
        case 0: {
          ret = "DMS_SchemaJson::Parse: Invalid json format.";
          break;
        }
      }
      break;
    }
    case ERR_DMS_JSON_CREATE_RESPONSE_DATA_INTERNAL: {
      switch (result.GetCode()) {
        case 0: {
          ret =
              "DMS_SchemaJson::CreateResponseDataInternal: Failed to create "
              "value.";
          break;
        }
      }
      break;
    }
    case ERR_DMS_JSON_PARSE_RESPONSE_DATA_INTERNAL: {
      switch (result.GetCode()) {
        case 0: {
          ret =
              "DMS_SchemaJson::ParseResponseDataInternal: Failed to create "
              "value.";
          break;
        }
      }
      break;
    }
    case ERR_DMS_JSON_PARSE_RESPONSE_DATA_VALUE: {
      switch (result.GetCode()) {
        case 0: {
          ret =
              "DMS_SchemaJson::ParseResponseDataValue: Failed to create value.";
          break;
        }
      }
      break;
    }
  }

  return ret;
}

ADK_END_NAMESPACE

