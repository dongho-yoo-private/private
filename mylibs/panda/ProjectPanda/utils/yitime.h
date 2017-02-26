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
} syiTime;

/*
*/
typedef struct __syiSecs
{
	unsigned int sec;
	unsigned int usec;
} syiSecs;

#ifndef __MACTYPES__
typedef ubit32_t TimeValue;
#endif

#define YLT_YEAR(n) ((n>>26)+2009)
#define YLT_MONTH(n) ((n>>22)&0x0F)
#define YLT_DAY(n) ((n>>17)&0x1F)
#define YLT_HOUR(n) ((n>>12)&0x1F)
#define YLT_MIN(n) ((n>>6)&0x3F)
#define YLT_SEC(n) (n&0x3F)

#define DAY_YEAR(n) (n>>9)
#define DAY_MON(n) (((n>>5)&0x0F))
#define DAY_DAY(n) (n&0x1F)
/**/
EXTERN_C unsigned int yiLocalTime();

/**/
EXTERN_C unsigned int yiLocalTime2(unsigned int* time);


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

#define MAKETIME(y, m, d, hh, mm, ss) ((((y)-2009)<<26) | ((m)<<22) | ((d)<<17) | ((hh)<<12) | ((mm)<<6) | (ss))
#define xYEAR(t)	((t>>26)+2009)
#define xMONTH(t)	((t>>22)&0x0F)
#define xDAY(t)		((t>>17)&0x1F)
#define xHOUR(t)	((t>>12)&0x1F)
#define xMINUTE(t)	((t>>6)&0x3F)
#define xSECOND(t)	(t&0x3F)

#define MAKEDAY(y, m, d) ((y<<9)|(m<<5)|d)
//#define DAY_YEAR(day) (day>>9)
//#define DAY_MON(day) ((day>>5)&0x0F)
//#define DAY_DAY(day) (day&0x1F)

#endif

