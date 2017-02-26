#pragma once

#define MAKETIME(y, m, d, hh, mm, ss) ((((y)-2009)<<26) | ((m)<<22) | ((d)<<17) | ((hh)<<12) | ((mm)<<6) | (ss))
#define xYEAR(t)	((t>>26)+2009)
#define xMONTH(t)	((t>>22)&0x0F)
#define xDAY(t)		((t>>17)&0x1F)
#define xHOUR(t)	((t>>12)&0x1F)
#define xMINUTE(t)	((t>>6)&0x3F)
#define xSECOND(t)	(t&0x3F)

typedef unsigned __int64 time64_t;

#define MAKETIME64(y, m, d, hh, mm, ss) (time64_t)((((time64_t)y)<<26) | (((time64_t)m)<<22) | (((time64_t)d)<<17) | (((time64_t)hh)<<12) | (((time64_t)mm)<<6) | ((time64_t)ss))
#define xYEAR64(t)	(int)((t>>26))
#define xMONTH64(t)	(int)((t>>22)&0x0F)
#define xDAY64(t)		(int)((t>>17)&0x1F)
#define xHOUR64(t)	(int)((t>>12)&0x1F)
#define xMINUTE64(t)	(int)((t>>6)&0x3F)
#define xSECOND64(t)	(int)(t&0x3F)

#define MAKEDAY(y, m, d) ((y<<9)|(m<<5)|d)
#define DAY_YEAR(day) (day>>9)
#define DAY_MON(day) ((day>>5)&0x0F)
#define DAY_DAY(day) (day&0x1F)


//
class xEXPORT cyiTime
{
public:
	// ------------------------------------
	static unsigned long CurrentmSec(unsigned int before=0);

	// ------------------------------------
	static unsigned long GetSecDiff(unsigned long before, unsigned long after);

	// ------------------------------------
	static unsigned int GetSumDay(unsigned int day);

	// ------------------------------------
	static unsigned int GetLocalTime();

	// ------------------------------------
	static unsigned int GetLocalTime(unsigned int &day);

	// ------------------------------------
	static char* TimeToString(char* szTime, unsigned int time, unsigned int day, Bool bSpace=True, char* pTimeSpliter=0, char* pDaySpliter=0);

	// ------------------------------------
	static time64_t GetLocalTime64();

	// ------------------------------------
	static unsigned int IncreaseSec(unsigned int timex);

	// ------------------------------------
	static Bool IsLeapYear(unsigned int timex);


	// ------------------------------------
	static Bool ShowMonthCalender(HWND hWnd, time64_t& date);

	// ------------------------------------
	static time64_t GetFileTimeStamp(const char* pszFileName);

	// ------------------------------------
	wchar_t* DateToTime(time64_t t, wchar_t* buffer, const char* pszFormat);
};
