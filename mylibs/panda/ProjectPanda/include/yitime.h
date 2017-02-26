#ifndef __yi_time_h__
#define __yi_time_h__

#include "yi.h"

/*
*/
typedef struct __syiTime 
{
	int y;
	int m;
	int d;
	int h;
	int mm;
	int s;
} syiTime ;

/*
*/
typedef struct __syiSecs
{
	unsigned int sec;
	unsigned int usec;
} syiSecs;

typedef unsigned long TimeValue;

/**/
EXTERN_C const syiTime* yiGetCurrentTime(syiTime* out_time);

/**/
EXTERN_C syiSecs yiGetCurrentSec();

/**/
EXTERN_C syiSecs yiCompareSecs(syiSecs sec1, syiSecs sec2);

/**/
EXTERN_C Bool yiTimeToString(char* buffer, const char* format, const syiTime* time);

/**/
EXTERN_C Bool yiTimeValueToString(char* buffer, const char* format, TimeValue time);

/**/
EXTERN_C Bool yiIsLeapYear(int n);

/**/
EXTERN_C int yiGetSumDate(syiTime* time);

/**/
EXTERN_C int yiGetday(syiTime* time);

/**/
EXTERN_C long yiDateTolong(syiTime time);

/**/
EXTERN_C syiTime yiTimeValueToDate(TimeValue time);

/**/
EXTERN_C syiTime yiTimeCompare(syiTime time, syiTime time2);

#endif

