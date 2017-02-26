#include <core/library/lib_time.h>

#if defined(WITH_ANDROID)
# include <time.h>
#elif defined(WITH_IOS)
# include <sys/time.h>
#endif

ADK_BEGIN_NAMESPACE

typedef struct __syiSecs
{
  U32 sec;
  U32 usec;
} syiSecs;

#define xMINUTE_MILLISEC (60000)
#define xHOUR_MILLISEC (3600000)

#define IS_LEAP_YEAR(a) (((a%4)==0 && (a%100)!=0)||((a%100)==0&&(a%400)==0))

S32 g_sum_month[] = {0, 31, 59, 90, 120, 151, 191, 212, 243, 273, 303, 334, 365};
S32 g_sum_month_leap[] = {0, 31, 60, 91, 121, 152, 192, 213, 244, 274, 304, 335, 366};

// -------------------------------------
U64 LIB_TimeGetCurrentMsec(U64 before)
{
  struct timeval tv;/* = {0,0};*/

  if (gettimeofday(&tv, 0)==-1)
  {
    return 0;
  }
  
  S64 msec = (U64)((U64)tv.tv_sec*1000+(U64)tv.tv_sec/1000);
  S64 ret = msec-before;

  if (ret<0)
  {
    return (U64)(((U64)0xFFFFFFFFFFFFFFFF - before)+msec);
  }
 
  return (U64)ret; 
}

// -------------------------------------
const char* LIB_TimeGetStringMsec(U64 msec, char* buffer)
{
  sprintf(buffer, "%u.%03u", (U32)msec/1000, (U32)(msec%1000));
  return buffer;
}

#define xSET_DATE(date, yy, mm, dd, hh, m, ss) \
date.year=yy;\
date.month=mm;\
date.day=dd;\
date.hour=hh;\
date.min=m;\
date.sec=ss;

// -------------------------------------
LIB_TDate LIB_TimeGetLocalTime()
{
  time_t t=time(0);
  struct tm* curr = localtime(&t);
  LIB_TDate ret;// = {curr->tm_year, curr->tm_mon, curr->tm_mday, curr->tm_hour, curr->tm_min, curr->tm_sec};
  curr->tm_year+=1900;

  xSET_DATE(ret, curr->tm_year, curr->tm_mon+1, curr->tm_mday, curr->tm_hour, curr->tm_min, curr->tm_sec);
  return ret;
}


/** とりあえず、間に合わせで適当
*/
LIB_TDate LIB_TimeGetTimeByFormat(const Char* source, const Char* format)
{
  Char* pIndex = (Char*)source;
  S32 year=0, month=0, day=0, hour=0, min=0, sec=0;
  S32 ret;
  LIB_TDate date;
  if (strstr(format, "-"))
  {
    ret = sscanf(source, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec);
  }
  else if (strstr(format, "/"))
  {
    ret = sscanf(source, "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec);
  }
  else
  {
    ret = sscanf(source, "%d:%d:%d", &hour, &min, &sec);
  }
  
  xSET_DATE(date, year, month, day, hour, min, sec);
  return date;
}



// -------------------------------------
const Char* LIB_TimeGetTimeString(LIB_TDate t, char* buffer)
{
  sprintf(buffer, "%04d/%02d/%02d %02d:%02d:%02d", (S32)t.year,  (S32)t.month, (S32)t.day,
              (S32)t.hour, (S32)t.min, (S32)t.sec);

  return buffer;
}


static Bool xIsLeapYear(S32 n)
{
  if (n%4==0)
  {
    if (n%100==0)
    {
      if (n%400==0)
      {
        return true;
      }
      return false;
    }

    return true;
  }

  return false;
}



// -------------------------------------
static U64 xGetSumDate(U32 year, U32 month, U32 date)
{
  int sum = 365*(year-1);
  int i=0;
  int nLeapCount= (year-1)/4-(year-1)/100+(year-1)/400;
  const int months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  sum+=nLeapCount;

  for (i=0; i<month-1; i++) sum+=months[i];

  if (month>2)
  {
    if (xIsLeapYear(year)==true)
    {
      sum++;
    }
  }

  sum+=date;

  return sum;
}

#define xDAY2SEC (86400)
#define xHOUR2SEC (3600)
#define xMIN2SEC (60)

// -------------------------------------
static U64 xGetSumSec(U64 day, U32 hour, U32 min, U32 sec)
{
  return day*xDAY2SEC+hour*xHOUR2SEC+min*xMIN2SEC+sec;
}

// -------------------------------------
U64 LIB_TimeDateToSec(LIB_TDate t)
{
  U64 day = xGetSumDate(t.year, t.month, t.day);
  return xGetSumSec(day, t.hour, t.min, t.sec);
}

// -------------------------------------
S64 LIB_TimeGetDiff(LIB_TDate t1, LIB_TDate t2)
{
  return LIB_TimeDateToSec(t1)-LIB_TimeDateToSec(t2);  
}

LIB_TWeekDay LIB_TimeGetWeek(LIB_TDate time)
{
  Bool current_leap_year = IS_LEAP_YEAR(time.year);
  S32 sum = (time.year-1)*365+((time.year-1)/4)-((time.year-1)/100)+((time.year-1)/400);
  sum+= current_leap_year?g_sum_month[time.month-1]:g_sum_month_leap[time.month-1]+time.day;

  return (LIB_TWeekDay)(sum%7);
}




ADK_END_NAMESPACE
