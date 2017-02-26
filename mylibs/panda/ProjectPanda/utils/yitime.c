#include "yitime.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include "yitrace.h"

int __zero_year__ = 2009;


// -------------------------------
unsigned int yiLocalTime()
{
	time_t t=time(0);
	struct tm* curr = localtime(&t);

	return (((curr->tm_year+1900-2009)<<26) |
				(curr->tm_mon<<22) |
					(curr->tm_mday<<17) |
						(curr->tm_hour<<12) |
							(curr->tm_min<<6) |
								curr->tm_sec);
}

// -------------------------------------
unsigned int yiLocalTime2(unsigned int* ptime)
{
	time_t t=time(0);
	struct tm* curr = localtime(&t);

	if (ptime)
		*ptime= (curr->tm_hour<<12)|(curr->tm_min<<6)|curr->tm_sec;
	return ((curr->tm_year+1900)<<9)|((curr->tm_mon+1)<<5)|(curr->tm_mday);
	
}



/**/
const syiTime* yiGetCurrentTime(syiTime* out_time)
{
	static syiTime _time;
#ifdef __POSIX__

	time_t t=time(0);
	struct tm* curr = localtime(&t);

	if (curr==0)
	{
		yiTraceLib("yiGetCurrentTime() Failure......\n");
		_time.y=-1;
		return &_time;
	}

	_time.y = curr->tm_year+1900;
	_time.m = curr->tm_mon;
	_time.d = curr->tm_mday;
	_time.h = curr->tm_hour;
	_time.mm = curr->tm_min;
	_time.s = curr->tm_sec;

	if (out_time!=0)
	{
		*out_time=_time;
	}

	yiTraceLib("Current Time .....\n");	
	yiTraceLib("%4d/%2d/%2d %2d:%2d:%2d\n",_time.y, _time.m, _time.d, 
						_time.h, _time.mm, _time.s);
	
#else
#endif
	return &_time;
}

/**/
syiSecs yiGetCurrentSec()
{
	syiSecs sec;
#ifdef __POSIX__
	struct timeval tv;/* = {0,0};*/

	if (gettimeofday(&tv, 0)==-1)
	{
		return sec;
	}

	sec.sec=tv.tv_sec;
	sec.usec=tv.tv_usec;

	return sec;
#endif

#ifdef __WIN32__
	DWORD time = GetTickCount();
	sec.sec=tv.tv_sec;
	sec.usec=tv.tv_usec;
#endif /*__POSIX__*/

	return sec;
}

/*
 */
syiSecs yiCompareSecs(syiSecs sec1, syiSecs sec2)
{
	long sec  = sec2.sec-sec1.sec;
	long usec = sec2.usec-sec1.usec;
	syiSecs temp;

	sec = (usec<0) ? sec-1:sec;
	usec = (usec<0) ? usec+1000000 : usec;

	temp.sec=(unsigned int)sec;
	temp.usec=(unsigned int)usec;

	return temp;
}



/**/
static int __get_same_char_cnt__(const char*p, char ch)
{
	int cnt=0;
	while(*p++==ch) cnt++;

	return cnt;
}
/*
 */
static void __time_to_string(char* p, int n)
{
	if (p !=0)
	{
		int nContinuosCnt = __get_same_char_cnt__(p, *p);
		int i = 0;
		int bufferlen=0;
		char buffer[5];

		sprintf(buffer, "%d", n);
		bufferlen = strlen(buffer);

		for (i=0; i<nContinuosCnt; i++)
		{
			if (bufferlen-nContinuosCnt+i<0)
			{
				p[i]='0';
				continue;
			}
			p[i] = buffer[bufferlen-nContinuosCnt+i];
		}
	}

	return ;
}

#include <string.h>

/*
 * foramt ex)
 * "YY:MM:SS"
 * "Y:M:SS"
 * "Y/M/SS"
 * "YYYY/MM/DD hh:mm:ss"
 */
Bool yiTimeToString(char* buffer, const char* format, const syiTime* time)
{
	char buffer_format[128]; //= (char*)format;//(char*)yiToUpperCode(format);
	const char* pTags[] = { "Y", "M", "D", "h", "m", "s", 0};
	int cnt=0;
	char* p= buffer_format;

	if (buffer==0 ||
		format==0 ||
			time==0)
	{
		return False;
	}

	strcpy(buffer_format, format);

	while(pTags[cnt])
	{
		int* pTime = (int*)time;
		char* pTemp = strstr(p, pTags[cnt]);
		__time_to_string(pTemp, pTime[cnt++]);
	}

	strcpy(buffer, p);

	return True;
}

/*
 */
Bool yiTimeValueToString(char* buffer, const char* format, TimeValue time)
{
	syiTime t = yiTimeValueToDate(time);
	return yiTimeToString(buffer, format, &t);
}

/**/
Bool yiIsLeapYear(int n)
{
	if (n%4==0)
	{
		if (n%100==0)
		{
			if (n%400==0)
			{
				return True;
			}
			return False;
		}

		return True;
	}

	return False;
}

/**/
int yiGetSumDate(syiTime* time)
{
	int sum = 365*(time->y-1);
	int i=0;
	int nLeapCount= (time->y-1)/4-(time->y-1)/100+(time->y-1)/400;
	const int months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	sum+=nLeapCount;

	for (i=0; i<time->m-1; i++) sum+=months[i];

	if (time->m>2)
	{
		if (yiIsLeapYear(time->y)==True)
		{
			sum++;
		}
	}

	sum+=time->d;

	return sum;

}

/**/
int yiGetday(syiTime* time)
{
	return yiGetSumDate(time)%7;
}


/**/
void yiZeroYear(int n)
{
	__zero_year__=n;
}

/**/
TimeValue yiDateToTimeValue(syiTime* time)
{
	time->y-=__zero_year__;
	return time->y<<26 | time->m<<22 | time->d<<17 | time->h<<12 | time->m<<6 | time->s;
}

/**/
syiTime yiTimeValueToDate(TimeValue time)
{
	syiTime _time = { (time>>26)+__zero_year__, (time>>22)&15, (time>>17)&31, (time>>12)&31, (time>>6)&63, time&63 };
	return _time;
}

/**/
syiTime yiTimeCompare(syiTime time, syiTime time2)
{
	syiTime tempTime;
	int* p= (int*)&time;
	int* p2= (int*)&time2;
	int* temp= (int*)&time2;
	int i=0;

	for (i=0; i<6; i++)
	{
		temp[i]=p2[i]-p[i];
	}

	memcpy(&tempTime, temp, sizeof(int)*6);

	return tempTime;
}

