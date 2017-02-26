/**
 * \file err_dms_result_types.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief データマネージャシステムリザルト定義部
 */


#ifndef ERR_DMS_RESULT_TYPES_H_
#define ERR_DMS_RESULT_TYPES_H_


ADK_BEGIN_NAMESPACE


#define ERR_DMS_INITIALIZE (0)
#define ERR_DMS_TERMINATE (1)
#define ERR_DMS_INIT (2)
#define ERR_DMS_REQUEST_TO_WAS (3)
#define ERR_DMS_DISPOSE_REQUEST_TO_WAS (4)
#define ERR_DMS_JSON_PARSE (5)
#define ERR_DMS_JSON_CREATE_RESPONSE_DATA_INTERNAL (6)
#define ERR_DMS_JSON_PARSE_RESPONSE_DATA_INTERNAL (7)
#define ERR_DMS_JSON_PARSE_RESPONSE_DATA_VALUE (8)
#define ERR_DMS_JSON_PARSE_COMMON_RESONSE_INVALID_BASE_RESPONSE (9)
#define ERR_DMS_JSON_PARSE_COMMON_RESONSE_INVALID_BASE_COMMON (10)


ADK_END_NAMESPACE


#endif  /* ERR_DMS_RESULT_TYPES_H_ */


