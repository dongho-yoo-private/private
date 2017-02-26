/**
 * \file err_api_result_types.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief APIリザルト定義部
 */


#ifndef ERR_API_RESULT_TYPES_H_
#define ERR_API_RESULT_TYPES_H_

#include <core/types.h>

ADK_BEGIN_NAMESPACE


#define ERR_API_GET_MASTER_SERVER_URL (0)
#define ERR_API_GET_SERVER_COUNT (1)
#define ERR_API_GET_SERVER_URL (2)
#define ERR_API_GET_SERVER_DISPLAY_NAME (3)
#define ERR_API_LOGIN (4)
#define ERR_API_DISPATCH_PROCESS (5)
#define ERR_API_REPLACE_PHASE (6)
#define ERR_API_GET_SCHEMA (7)
#define ERR_API_IS_ERROR (8)
#define ERR_API_GET_ERROR (9)
#define ERR_API_ADD_JSON_COMMON_PARAMETER (10)
#define ERR_API_APPLY_HTTP_REQUEST_HEADER (11)
#define ERR_API_SET_DEBUG_LOG_LEVEL (12)
#define ERR_API_REQUEST_POST_HTTP (13)


ADK_END_NAMESPACE


#endif  /* _ADK_API_RESULT_H_ */


