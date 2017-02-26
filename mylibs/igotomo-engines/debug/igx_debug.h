/**
* Copyright (C) 2015 igotomo.
*
* authorized by dongho.yoo.
*/
#ifndef _IGX_DEBUG_H_
#define _IGX_DEBUG_H_

#define IGX_DEBUG(...) igx_log(kLogLevelDebug, __VA_ARGS__)
#define IGX_INFO(...) igx_log(kLogLevelInfo, __VA_ARGS__)
#define IGX_WARN(...) igx_log(kLogLevelWarn, __VA_ARGS__);\
igx_log(kLogLevelWarn, "%s:%d in %s\n", __FILE__, __LINE__, __func__)
#define IGX_ERROR(...) igx_log(kLogLevelError, __VA_ARGS__);\
igx_log(kLogLevelError, "%s:%d in %s\n", __FILE__, __LINE__, __func__)
#define IGX_CRITICAL(...) igx_log(kLogLevelCritical, __VA_ARGS__);\
igx_log(kLogLevelCritical, "%s:%d in %s\n", __FILE__, __LINE__, __func__)

#ifdef DEBUG
#   define IGX_ASSERT(condition) \
if ((condition)==0)\
{\
IGX_CRITICAL("Assertion: %s:%d %s()", __FILE__,__LINE__, __func__);\
igx_assert((intptr_t)condition, "");\
}
#   define IGX_ASSERT_MSG(condition, ...) \
if ((condition)==0)\
{\
IGX_CRITICAL("Assertion: %s:%d %s()", __FILE__,__LINE__, __func__);\
igx_assert((intptr_t)condition, __VA_ARGS__);\
}
#   define IGX_DEBUG_LONG(...) printf(__VA_ARGS__)
#else
#   define IGX_ASSERT(condition) 
#   define IGX_ASSERT_MSG(condition, format, ...) 
#endif

#define IGX_DEBUG_CONSOLE(level) igx_log_init(kLogOutputConsole, level, 0)

#include "igx_log.h"
#include "igx_stack_trace.h"

#endif
