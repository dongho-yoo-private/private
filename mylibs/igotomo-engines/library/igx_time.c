#include "igx_time.h"
#include "debug/igx_debug.h"
#include <stdio.h>

#ifdef __POSIX__
//# include <time.h>
# include <sys/time.h>
#endif


#define xMINUTE_MILLISEC (60000)
#define xHOUR_MILLISEC (3600000)

#define IS_LEAP_YEAR(a) (((a%4)==0 && (a%100)!=0)||((a%100)==0&&(a%400)==0))

static const struct __global__{
    bit32_t sum_month[12]; 
    bit32_t sum_month_leap[12]; 
} global = {
 {31, 59, 90, 120, 151, 191, 212, 243, 273, 303, 334, 365}, {31, 60, 91, 121, 152, 192, 213, 244, 274, 304, 335, 366}
};


// -------------------------------------
ubit64_t igx_get_current_msec(ubit64_t before)
{
  struct timeval tv;/* = {0,0};*/

  if (gettimeofday(&tv, 0)==-1)
  {
    return 0;
  }
  
  bit64_t msec = (ubit64_t)((ubit64_t)tv.tv_sec*1000+(ubit64_t)tv.tv_sec/1000);
  bit64_t ret = msec-before;

  if (ret<0)
  {
    return (ubit64_t)(((ubit64_t)0xFFFFFFFFFFFFFFFF - before)+msec);
  }
 
  return (ubit64_t)ret; 
}

// -------------------------------------
const char* igx_time_GetStringMsec(ubit64_t msec, char* buffer)
{
  sprintf(buffer, "%u.%03u", (ubit32_t)msec/1000, (ubit32_t)(msec%1000));
  return buffer;
}

// -------------------------------------
igx_time_t igx_time_local_time()
{
  time_t t=time(0);
  struct tm* curr = localtime(&t);
  igx_time_t ret;
  curr->tm_year+=1900;

  IGX_TIME_SET(ret, curr->tm_year, curr->tm_mon+1, curr->tm_mday, curr->tm_hour, curr->tm_min, curr->tm_sec, 0);
  return ret;
}
char* igx_time_to_string(char* buffer, const igx_time_t date, const char* format)
{
    sprintf(buffer, format, 
                (int)date.year, (int)date.month, (int)date.day, 
                        (int)date.hour, (int)date.min, (int)date.sec, (int)date.msec);
    return buffer;
}
igx_time_t igx_sting_to_time(const char* time, const char* format)
{
    /** メモリが壊れない様に。。。**/
    char dummy_buffer[256];
    igx_time_t ret;
    bit32_t y=0,m=0,d=0,hh=0,mm=0,ss=0;

    if (format!=0)
    {
        if (sscanf(time, format, &y, &m, &d, &hh, &mm, &ss)==0)
        {
            if (sscanf(time, format, &hh,&mm,&ss)==0)
            {
                IGX_TIME_SET(ret, y, m, d, hh, mm, ss, 0);
                return ret;
            }
        }
        IGX_TIME_SET(ret, y, m, d, hh, mm, ss, 0);
        return ret;
    }
    if (sscanf(time, "%d-%d-%d %d:%d:%d", &y, &m, &d, &hh, &mm, &ss)==0)
    {
        if (sscanf(time, "%d:%d:%d:", &hh,&mm,&ss)!=0)
        {
            IGX_TIME_SET(ret, y, m, d, hh, mm, ss, 0);
            return ret;
        }
    }
    else
    {
        IGX_TIME_SET(ret, y, m, d, hh, mm, ss, 0);
        return ret;
    }
    if (sscanf(time, "%d/%d/%d %d:%d:%d", &y, &m, &d, &hh, &mm, &ss)==0)
    {
         if (sscanf(time, "%d:%d:%d:", &hh,&mm,&ss)!=0)
        {
            IGX_TIME_SET(ret, y, m, d, hh, mm, ss, 0);
            return ret;
        }
   }
   IGX_TIME_SET(ret, y, m, d, hh, mm, ss, 0);
   return ret;
}
static bit32_t get_leap_count(ubit32_t year)
{
    const bit32_t before_year = year-1;
    return (before_year>>2)-(before_year/100)+(before_year/400);
}
// -------------------------------------
static ubit64_t get_sum_date(ubit32_t year, ubit32_t month, ubit32_t date)
{
    int sum = 365*(year-1);
    int leap_count = get_leap_count(year);
  
    sum+=leap_count;
    if (IS_LEAP_YEAR(year)==true)
    {
        sum+=global.sum_month[month-2];
    }
    else
    {
        sum+=global.sum_month_leap[month-2];
    }
  
    sum+=date;
    return sum;
}

#define xDAY2SEC (86400)
#define xHOUR2SEC (3600)
#define xMIN2SEC (60)

static ubit64_t get_sum_sec(ubit64_t day, ubit32_t hour, ubit32_t min, ubit32_t sec, ubit32_t msec)
{
    return (day*xDAY2SEC+hour*xHOUR2SEC+min*xMIN2SEC+sec)*1000+msec;
}
ubit64_t igx_time_date_to_msec(igx_time_t t)
{
    ubit64_t day = get_sum_date(t.year, t.month, t.day);
    return get_sum_sec(day, t.hour, t.min, t.sec, t.msec);
}
igx_weekday_t igx_time_get_week(igx_time_t time)
{
    bool_t current_leap_year = IS_LEAP_YEAR(time.year);
    bit32_t sum = (time.year-1)*365+((time.year-1)/4)-((time.year-1)/100)+((time.year-1)/400);
    sum+= current_leap_year?global.sum_month[time.month-1]:global.sum_month_leap[time.month-1]+time.day;
  
    return (igx_weekday_t )(sum%7);
}
ubit64_t igx_time_check(const char* title, ubit64_t before)
{
    ubit64_t now = igx_get_current_msec(before);
    
    if (before==0)
    {
        IGX_DEBUG("++++++++++++ %s: start!");
    }
    else
    {
        const bit64_t diff = now-before;
        IGX_DEBUG("++++++++++++ %s: %u.%03u",title,(ubit32_t)(diff/1000), (ubit32_t)(diff%1000));
    }

    return now;
}
static ubit32_t count_year(ubit32_t total_day, ubit32_t* remain_day)
{
    ubit32_t count = (total_day/365);
    ubit32_t remain = (total_day%365);
    ubit32_t leap_year_count = get_leap_count(count+1);

    while(remain<leap_year_count)
    {
        --count;
        remain+=365;
        leap_year_count = (count>>2)-(count/100)+(count/400);
    }

    remain-=leap_year_count;
    *remain_day=remain;
    return count+1;
}
igx_time_t igx_time_msec_to_time(ubit64_t msec)
{
    igx_time_t time;
    const ubit64_t total_sec = (msec/1000);
    const ubit64_t total_min = total_sec/60;
    const ubit64_t total_hour = total_min/60;
    const ubit32_t total_day = total_hour/24;
    ubit32_t sec=(total_sec%60), min=(total_min%60), hour=(total_hour%24);
    ubit32_t remain_day, year = count_year(total_day, &remain_day); 
    ubit32_t month = 0;

    //IGX_DEBUG("total_sec:%ld, total_min:%ld, totla_hour:%ld, total_day:%d, sec:%d, min:%d, hour:%d", total_sec, total_min, total_hour, total_day, sec, min, hour);
    
    if (IS_LEAP_YEAR(year)==true)
    {
        for (;global.sum_month_leap[month]<remain_day;++month);
        ++month;
        remain_day-=global.sum_month_leap[month-1];
    }
    else
    {
        for (;global.sum_month[month]<remain_day;++month); 
        ++month;
        remain_day-=global.sum_month[month-1];
    }
    IGX_TIME_SET(time, year, month, remain_day, hour, min, sec, (msec%1000));
    return time;
}

