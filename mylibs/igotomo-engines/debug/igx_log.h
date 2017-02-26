/**
* Copyright (C) 2015 igotomo.
*
* authorized by dongho.yoo.
*/
#ifndef _IGX_LOG_H_
#define _IGX_LOG_H_

#include <stdarg.h>
#include "igx_types.h"

#ifdef __cplusplus
#   define IGX_LOG_EXTERN extern "C"
#else
#   define IGX_LOG_EXTERN extern 
#endif

typedef enum {
    kLogOutputConsole,
    kLogOutputFile,
    kLogOutputSyslog
} igx_log_output_t;

typedef enum {
    /**@brief user このログはコミットしない。*/
    kLogLevelUser = 0,
    /**@brief 致命的なエラー修復不能の場合*/
    kLogLevelCritical = 1,
    /**@brief 論理的エラー*/
    kLogLevelError = (2|kLogLevelCritical),
    /**@brief 警告*/
    kLogLevelWarn = (4|kLogLevelError),
    /**@brief 情報(ボタンが押されたなどのイベント専用) 普段のログレベルはinfo */
    kLogLevelInfo = (8|kLogLevelWarn),
    /**@brief for debug なんでも。。。*/
    kLogLevelDebug = (0x10|kLogLevelInfo),
    /**@brief 全て*/
    kLogLevelAll=0xFF
}igx_log_level_t;

/**
* @brief ログを初期化します。
* @param output 出力先を指定します。
* @param level  ログレベルを設定します。
* @param filename 出力先がkLogOutputFileの場合は、ファイル名を指定します。
*/
IGX_LOG_EXTERN void igx_log_init(igx_log_output_t output, igx_log_level_t level, const void* extra_param);
/**
* @brief ログを出力します。
* @param format ログのフォーマット
* @param level  ログレベル
*/
IGX_LOG_EXTERN void igx_log(igx_log_level_t level, const char* format, ...);
/**
* @brief メモリの内容をダンプします。
* @param title タイトル
* @param buffer ダンプするメモリアドレス
* @param size ダンプサイズ
* @param column_num 一行に表示するバイト数
*/
IGX_LOG_EXTERN void igx_log_dump(const char* titile, const void* buffer, size_t size, size_t column_num);
/**
* @brief サイズ指定で文字列を出力します。
* @param buffer 出力する文字列の先頭
* @param len サイズ　
*/
IGX_LOG_EXTERN void igx_print_char(const char* buffer, size_t len);

#endif

