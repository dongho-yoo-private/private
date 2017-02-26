#include "yistd.h"
#include "yitrace.h"
#include "yitime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#ifdef __POSIX__
#include <syslog.h>
#include <stdarg.h>
#endif

/*
 */
int __max_buffer_size__ = 4096;


/*
 */
void yiLogStart(int level, const char* pname)
{
	openlog(pname, LOG_PID, LOG_LOCAL2);
}

/*
 */
void yiLogEnd()
{
	closelog();
}

/*
 */
void yiLog(int loglevel, const char* format, ...)
{
	char buffer[1024];
	va_list ap;
	va_start(ap, format);


#ifdef __DEBUG__
//	vprintf(format, (va_list)pArgs);
#endif

#ifndef __DEBUG__
	if (loglevel==eLL_DEBUG)
	{
		return ;
	}
#endif
	switch(loglevel)
	{
		case eLL_ERROR:
			strcpy(buffer, "[ERROR] ");
			loglevel=LOG_ERR;
		break;
		case eLL_DEBUG:
			strcpy(buffer, "[DEBUG] ");
			loglevel=LOG_DEBUG;
		break;
		case eLL_WARN:
			strcpy(buffer, "[WARN] ");
			loglevel=LOG_WARNING;
		break;
		case eLL_INFO:
			strcpy(buffer, "[INFO] ");
			loglevel=LOG_NOTICE;
		break;
		default:
			va_end(ap);
			return ;
	}
	strcat(buffer, format);

#ifdef __DEBUG__
	if (loglevel!=LOG_ERR)
	{
		vprintf(buffer, ap);//(va_list)pArgs);
	}
#endif
	if (loglevel==LOG_ERR)
	{
		vprintf(buffer, ap);//(va_list)pArgs);
	}
	va_start(ap, format);
	vsyslog(loglevel, buffer, ap);//(va_list)pArgs);
	va_end(ap);
	return ;
}


/*
 */
void yiTraceLib(const char* pParam, ...)
{
#ifdef __POSIX_LIB_DEBUG__
	va_list ap;
	char buffer[__max_buffer_size__];

	va_start(ap, pParam);

	vsprintf(buffer, pParam, ap);
	printf("%s", buffer);

	va_end(ap);
#endif
}

/**/
void yiTraceDumpChar(char* p, int dumpSize, const char* name)
{
	int i;

	if (name)
		printf("------------ dump(%s) %d bytes ------------ \n", name, dumpSize);

	for(i=0;i<dumpSize;i++) putchar(*p++);
	putchar('\n');
}

/**/
void yiTraceDump(void* p, int dumpSize, int BytePerOneLine, const char* name)
{
	char* pChar = (char*)p;
	char bufferLine[256];
	char bufferAscii[256];
	int i=0;
	int asci=0;

	printf("------------ dump(%s) %d bytes ------------ \n", name, dumpSize);
	
	for (i=0; i<BytePerOneLine; i++)
	{
		printf("%02d|", (int)i);
	}
	putchar('\n');
	for (i=0; i<BytePerOneLine; i++)
	{
		printf("---");
	}
	//putchar('\n');

	bufferAscii[0]=0;
	bufferLine[0]=0;
	for (i=0; i<dumpSize; i++) 
	{
		char buffer[16];
		if (i!=0&&i%BytePerOneLine==0) {
			putchar('\n');
			printf(bufferLine);
			printf("   %s", bufferAscii);
			bufferLine[0]=0;
			bufferAscii[0]=0;
			asci=0;
		}
		sprintf(buffer, "%02x|", (int)(0xFF&*pChar));
		strcat(bufferLine, buffer);

		if (*pChar==0 || *pChar=='\n' || *pChar=='\r' ||
			*pChar=='\t' || *pChar<=0x1F)
		{
			bufferAscii[asci]='.';
		}
		else if ((*pChar&0x80)==0)
			bufferAscii[asci]=*pChar;
		else
			bufferAscii[asci]='.';

		//printf("%02x|", (int)(0xFF&*pChar));
		pChar++;
		asci++;
		bufferAscii[asci]=0;
	}

	putchar('\n');
	printf(bufferLine);
	printf("   %s", bufferAscii);
	putchar('\n');
}


/*
 */
void yiTraceWithTime(const char* pParam, ...)
{
	va_list ap;
	const syiTime* time = yiGetCurrentTime(0);
	char strTime[128];
	char buffer[__max_buffer_size__];	
	char** pArgs = (char**)&pParam;

	va_start(ap, pParam);
	pArgs++;

	strTime[0]=(char)0;
	if (time!=0)
	{
		yiTimeToString(strTime, "YY/MM/DD hh:mm:ss", time);
	}

	vsprintf(buffer, pParam, ap);//(va_list)pArgs);

	printf("%s > %s", strTime, buffer);

	va_end(ap);
}

/**/
typedef struct __stopwatch__
{
	char name[64];
	syiSecs startSec;
	syiSecs lastSec;
} syiStopWatch;

/*
 */
HStopWatch yiStartStopWatch(const char* name, const char* format, ...)
{
	va_list ap;
	char buffer[__max_buffer_size__];
	syiStopWatch* p = (syiStopWatch*)yialloc(sizeof(syiStopWatch));

	va_start(ap, format);
	strncpy(p->name, name, 64);

	vsprintf(buffer, format, ap);//(va_list)(&format+1));

	p->startSec = yiGetCurrentSec();
	p->lastSec=p->startSec;

	printf("StopWatch %s (%s): fromTop:%d.%06d fromLast:%d.%06d \n", name, buffer, 0, 0, 0, 0);

	va_end(ap);
	return (HStopWatch)p;
}

/*
 */
void yiShowStopWatch(HStopWatch hStopWatch, const char* format, ...)
{
	va_list ap;
	char buffer[__max_buffer_size__];
	syiStopWatch*p = (syiStopWatch* )hStopWatch;
	syiSecs tempSec = yiGetCurrentSec();
	syiSecs fromTop = yiCompareSecs(p->startSec, tempSec);
	syiSecs fromLast = yiCompareSecs(p->lastSec, tempSec);

	va_start(ap, format);

	vsprintf(buffer, format, ap);//(va_list)(&format+1));

	printf("StopWatch %s (%s): fromTop:%d.%06d fromLast:%d.%06d \n", p->name, buffer, fromTop.sec, fromTop.usec, 
									fromLast.sec, fromLast.usec);
	p->lastSec = tempSec;
	va_end(ap);
}

/*
 */
void yiEndStopWatch(HStopWatch hStopWatch)
{
	yifree((void*)hStopWatch);
}
