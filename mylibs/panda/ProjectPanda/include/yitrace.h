#ifndef __yi_trace_h__
#define __yi_trace_h__
#include "yi.h"

#include <stdio.h>
#ifdef __POSIX__
#include <stdarg.h>
#include <errno.h>
#endif

#ifdef __WSX_DEBUG_LIB__
#define __WSX_DEBUG__(a, ...) printf(a, __VARLIST__);
#define __WSX_ERROR__(a, ...) fprintf(stderr, a, __VARLIST__);
#else
#define __WSX_DEBUG__(a, ...) 
#define __WSX_ERROR__(a, ...) 
#endif 

//#define SYSLOG(a, format, ...) printf("%s:(%d) "format, __FILE__, __LINE__, __VA_ARGS__) 
#define SYSLOG(a, ...) printf(__VA_ARGS__) 

/**/
typedef enum __eyiLogLevel__ {
	WSX_LOG_ERROR,
	WSX_LOG_WARN,
	WSX_LOG_INFO,
	WSX_LOG_DEBUG
}eyiLogLevel;

typedef Handle HStopWatch;

/**/
EXTERN_C void yiLogStart(eyiLogLevel level, const char* pname);

/**/
EXTERN_C void yiLogEnd();

/**/
EXTERN_C void yiTrace(const char* pParm, ...);

/**/
EXTERN_C void yiTraceWithTime(const char* pParm, ...);

/**/
EXTERN_C void yiTraceLib(const char* pParm, ...);

/**/
EXTERN_C void yiTraceDump(void* p, int dumpSize, const char* name);

/**/
EXTERN_C void yiTraceDumpChar(char* p, int dumpSize, const char* name);

/*
 */
EXTERN_C HStopWatch yiStartStopWatch(const char* name, const char* format, ...);

/*
 */
EXTERN_C void yiShowStopWatch(HStopWatch hStopWatch, const char* format, ...);

/*
 */
EXTERN_C void yiEndStopWatch(HStopWatch hStopWatch);

#endif /*__yi_trace_h__*/
