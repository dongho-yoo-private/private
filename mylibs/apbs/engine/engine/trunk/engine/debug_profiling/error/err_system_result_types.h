/**
 * \file err_system_result_types.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief システムリザルト定義部
 */


#ifndef ERR_SYSTEM_RESULT_TYPES_H_
#define ERR_SYSTEM_RESULT_TYPES_H_


ADK_BEGIN_NAMESPACE


#define ERR_SYSTEM_INITIALIZE (0)
#define ERR_SYSTEM_TERMINATE (1)
#define ERR_SYSTEM_INIT (2)
#define ERR_SYSTEM_TERM (3)
#define ERR_SYSTEM_REQUEST_TO_WAS (5)
#define ERR_SYSTEM_DISPOSE_REQUEST_TO_WAS (6)
#define ERR_SYSTEM_REPLACE_PHASE (7)
#define ERR_SYSTEM_DO_PHASE_PROCESS (8)
#define ERR_SYSTEM_REQUEST_POST_HTTP (9)


ADK_END_NAMESPACE


#endif  /* _ADK_API_RESULT_H_ */


