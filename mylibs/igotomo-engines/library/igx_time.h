/**
 * Copyright 2015 (c) igotomo.
 * authorized by dongho.yoo.
*/
#ifndef _IGX_TIME_H_
#define _IGX_TIME_H_

#include "igx_types.h"

#ifdef __cplusplus
#define IGX_TIME_EXTERN extern "C"
#else
#define IGX_TIME_EXTERN extern
#endif

/** @brief 日付を保持する64bitの構造体*/
typedef union {
    ubit64_t date;
    struct 
    {
        /**@brief 年*/
        unsigned year:25;
        /**@brief 月*/
        unsigned month:4;
        /**@brief 日*/
        unsigned day:5;
        /**@brief 時間*/
        unsigned hour:5;
        /**@brief 分*/
        unsigned min:6;
        /**@brief 秒*/
        unsigned sec:6;
        /**@brief ミリ秒*/
        unsigned msec:10;
    } ;
} igx_time_t;

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
} igx_weekday_t;

#define IGX_TIME_SET(date, yy, mm, dd, hh, m, ss, _msec) \
date.year=yy, date.month=mm, date.day=dd, date.hour=hh, date.min=m, date.sec=ss, date.msec=_msec;

/** @brief 現在の時間と指定した時間をミリ秒単位で受け取ります。
*
* @param before 以前のミリ秒
* @return beforeと現在の差分を返します。
*/
IGX_TIME_EXTERN ubit64_t igx_time_get_current_msec(ubit64_t before);
/**
* @brief igx_time_t形式のデータをミリ秒に変更します。
*
* @return ミリ秒
*/
IGX_TIME_EXTERN ubit64_t igx_time_to_msec(igx_time_t time);
/**
* @brief ミリ秒をigx_time_t形式に変換します。
* @param msec ミリ秒
* @return  igx_time_t形式のデータ
*/
IGX_TIME_EXTERN igx_time_t igx_time_msec_to_time(ubit64_t msec);
/** @brief ミリ秒を秒.ミリ秒の形式で返します。
*
* @param msec ミリ秒
* @param buffer 受け取る文字列のバッファー
*
* @return 指定したバッファーのアドレス
*/
IGX_TIME_EXTERN const char* igx_time_get_string_msec(ubit64_t msec, char* buffer);

/** @brief 現在の日時を取得します。
*
* @return igx_time_t 形式の日付
*/
IGX_TIME_EXTERN igx_time_t igx_time_local_time();
/** @brief 時刻の文字列を取得します。
*
* @return 日時を秒に換算した値
*/
IGX_TIME_EXTERN ubit64_t igx_time_date_to_sec(igx_time_t t);
/** @brief 日時の差(t1-t2)を秒単位で取得します。
*
* @return igx_time_t 形式の日付
*/
IGX_TIME_EXTERN bit64_t igx_time_diff(igx_time_t t1, igx_time_t t2);
/**
* @brief 曜日を取得します。
* 
* @param time　日時
* @return 曜日 \ref LIB_TWeekDay
*/
IGX_TIME_EXTERN igx_weekday_t igx_time_get_week(igx_time_t time);
/** 
* @brief 日時の差(t1-t2)を秒単位で取得します。
*
* @param source 文字列の日付
* @format ex) "%02d/%02d/%02d %02d:%02d:%02d.%03d"
*
* @return igx_time_t 形式の日付
*/
IGX_TIME_EXTERN char* igx_time_to_string(char* buffer, const igx_time_t date, const char* format);
/**
* @brief 文字列の時間をigx_time_tに変換します。
* 
* @param time 文字列の時間
* @param format フォーマット ("%d-%d-%d %d:%d:%d")、ゼロ指定の場合は自動的に複数のパターンをチェックします。
* @return igx_time_t形式のデータ
*/
IGX_TIME_EXTERN igx_time_t igx_sting_to_time(const char* time, const char* format);
/** 
* @brief 区間の時間を計るときに使います。
*
* @param title  タイトル
* @param before 前回はかった時間
*
* @return 現在の時間（ミリ秒）
*/
IGX_TIME_EXTERN ubit64_t igx_time_check(const char* titile, ubit64_t before);

#endif
