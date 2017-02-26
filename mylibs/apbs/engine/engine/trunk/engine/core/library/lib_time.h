/**
 * \author dongho yoo
 * \file lib_time.h
 * \~english
 * \~japanese
 *   \brief Time library
 */


#ifndef LIB_TIME_H__
#define LIB_TIME_H__

#include <core/types.h>

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

#define __POSIX__

ADK_BEGIN_NAMESPACE

/** @brief 日付を保持する64bitの構造体*/
typedef struct {
  /**@brief 年*/
  unsigned long long year:35;
  /**@brief 月*/
  unsigned long long month:4;
  /**@brief 日*/
  unsigned long long day:5;
  /**@brief 時間*/
  unsigned long long hour:5;
  /**@brief 分*/
  unsigned long long min:6;
  /**@brief 秒*/
  unsigned long long sec:6;
} LIB_TDate;

/** @brief 曜日*/
typedef enum {
  /**\brief 日曜日*/
  kWeekDaySun=0,
  /**\brief 月曜日*/
  kWeekDayMon,
  /**\brief 火曜日*/
  kWeekDayTue,
  /**\brief 水曜日*/
  kWeekDayWed,
  /**\brief 木曜日*/
  kWeekDayThu,
  /**\brief 金曜日*/
  kWeekDayFri,
  /**\brief 土曜日*/
  kWeekDaySat,
} LIB_TWeekDay;


/** @brief 現在の時間と指定した時間をミリ秒単位で受け取ります。
*
* @param before 以前のミリ秒
* @return beforeと現在の差分を返します。
*/
EXTERN_C U64 LIB_TimeGetCurrentMsec(U64 before);

/** @brief ミリ秒を秒.ミリ秒の形式で返します。
*
* @param msec ミリ秒
* @param buffer 受け取る文字列のバッファー
*
* @return 指定したバッファーのアドレス
*/
EXTERN_C const char* LIB_TimeGetStringMsec(U64 msec, char* buffer);

/** @brief 現在の日時を取得します。
*
* @return LIB_TDate 形式の日付
*/
EXTERN_C LIB_TDate LIB_TimeGetLocalTime();

/** @brief 時刻の文字列を取得します。
*
* @return 64ビット形式の日付
*/
EXTERN_C const Char* LIB_TimeGetTimeString(LIB_TDate t, char* buffer);

/** @brief 時刻の文字列を取得します。
*
* @return 日時を秒に換算した値
*/
EXTERN_C U64 LIB_TimeDateToSec(LIB_TDate t);


/** @brief 日時の差(t1-t2)を秒単位で取得します。
*
* @return LIB_TDate 形式の日付
*/
EXTERN_C S64 LIB_TimeGetDiff(LIB_TDate t1, LIB_TDate t2);

/**
* \brief 曜日を取得します。
* 
* \param time　日時
* \return 曜日 \ref LIB_TWeekDay
*/
EXTERN_C LIB_TWeekDay LIB_TimeGetWeek(LIB_TDate time);


/** @brief 日時の差(t1-t2)を秒単位で取得します。
*
* \param source 文字列の日付
* \format ex) "YYYY-MM-DD hh:mm:ss"
*
* @return LIB_TDate 形式の日付
*/
EXTERN_C LIB_TDate LIB_TimeGetTimeByFormat(const Char* source, const Char* format);


ADK_END_NAMESPACE
// 廃止予定
#if 0 
/**@brief LIB_TDateから年を取得します。*/
#define mLIB_TimeYear(n) (U32)((n>>26))

/**@brief LIB_TDateから月を取得します。*/
#define mLIB_TimeMonth(n) (U32)((n>>22)&0x0F)

/**@brief LIB_TDateから日を取得します。*/
#define mLIB_TimeDate(n) (U32)((n>>17)&0x1F)

/**@brief LIB_TDateから時間を取得します。*/
#define mLIB_TimeHour(n) (U32)((n>>12)&0x1F)

/**@brief LIB_TDateから分を取得します。*/
#define mLIB_TimeMin(n) (U32)((n>>6)&0x3F)

/**@brief LIB_TDateから秒を取得します。*/
#define mLIB_TimeSec(n) (U32)(n&0x3F)

/**@brief LIB_TDate形式の時間を生成するマクロです。\n
* ex) TLib_Date date = mLIB_Time(2013, 10, 23, 19, 9, 0);
*/
#define mLIB_Time(year, month, date, hour, min, sec) (LIB_TDate)(sec|(min<<6)|(hour<<12)|(date<<17)|(month<<22)|(year<<26))
#endif

#endif
