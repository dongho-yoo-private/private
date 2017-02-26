#ifndef __yiTimeEx_h__
#define __yiTimeEx_h__

#include "yi.h"

#ifndef _WIN32
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#endif


typedef ubit64_t time64_t;

class cyiTime
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
	static char* TimeToString(char* szTime, unsigned int day, unsigned int time, Bool bSpace=True, char* pTimeSpliter=0, char* pDaySpliter=0);

	// ------------------------------------
	static time64_t GetLocalTime64();

	// ------------------------------------
	static unsigned int IncreaseSec(unsigned int timex);

	// ------------------------------------
	static void IncreaseTime(unsigned int& time, unsigned int& day);

	// ------------------------------------
	static Bool IsLeapYear(unsigned int timex);

#ifdef _WIN32
	// ------------------------------------
	static Bool ShowMonthCalender(HWND hWnd, time64_t& date);
#endif
	// ------------------------------------
	static time64_t GetFileTimeStamp(const char* pszFileName);

	// ------------------------------------
	wchar_t* DateToTime(time64_t t, wchar_t* buffer, const char* pszFormat);
};

#endif

