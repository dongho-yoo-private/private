/**
 * \file logging.h
 * \ingrup log
 * \author Kohei konno
 * \~english
 * \~japanese
 * \brief デバッグ機能
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include <core/types.h>
#include <core/debug/logging_types.h>
#include <stdio.h>

ADK_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 * \brief ログを出力する時に付くログレベルのラベル長さ。
 */
#define LOG_HEADER_SIZE (16)
/**
 * \~english
 * \~japanese
 * \brief ログを出力する時に付くログ時間ラベルの長さ。
 */
#define LOG_DATETIME_SIZE (32)
/**
 * \~english
 * \~japanese
 * \brief ログを出力する時に付くログ文字列の最大長さ。
 */
#define LOG_MAX_SIZE (128 - LOG_HEADER_SIZE - LOG_DATETIME_SIZE)
/**
 * \~english
 * \~japanese
 * \brief ログを出力するファイル名の最大長。
 */
#define LOG_FILENAME_MAX_SIZE (1024)
/**
 * \~english
 * \~japanese
 * \brief
 * ログをファイルに出力時ファイル名が指定されなかった時のデフォルトファイル名
 */
#define LOG_DEFAULT_FILENAME "/Users/temp/error.log"
/**
 * \~english
 * \~japanese
 * \brief 主に実装者が開発中に必要なときに組み込むログを出力します。
 */
#define DEBUG_LOG(...)                                                      \
  adk::Log::GetInstance()->output("[DEBUG]", adk::kDebugLog, __FUNCTION__, \
                             __VA_ARGS__)
/**
 * \~english
 * \~japanese
 * \brief
 * ファイルの読み込み完了、受信したパケット情報、現在のメモリ状況など情報をを出力します。
 */
#define INFO_LOG(...)                                                     \
  adk::Log::GetInstance()->output("[INFO]", adk::kInfoLog, __FUNCTION__, \
                             __VA_ARGS__)
/**
 * \~english
 * \~japanese
 * \brief
 * 何が起きたか、フローが変わったのか、イベントが発生したか等をを出力します。
 */
#define NOTICE_LOG(...)                                                       \
  adk::Log::GetInstance()->output("[NOTICE]", adk::kNoticeLog, __FUNCTION__, \
                             __VA_ARGS__)
/**
 * \~english
 * \~japanese
 * \brief エンジニアに警告し、すぐ確認させる為のログを出力します。
 */
#define WARNING_LOG(...)                                          \
  adk::Log::GetInstance()->output("[WARNING]", adk::kWarningLog, \
                             __FUNCTION__, __VA_ARGS__)
/**
 * \~english
 * \~japanese
 * \brief
 * アプリケーション的にあり得るエラーハンドリング時に出力すべきログを出力します。
 */
#define ERROR_LOG(...)                                                      \
  adk::Log::GetInstance()->output("[ERROR]", adk::kErrorLog, __FUNCTION__, \
                             __VA_ARGS__)
/**
 * \~english
 * \~japanese
 * \brief
 * コードとして不正なアクセスやオーバーフローなど、実行エラーログを出力します。
 */
#define CRITICAL_LOG(...)                                           \
  adk::Log::GetInstance()->output("[CRITICAL]", adk::kCriticalLog, \
                             __FUNCTION__, __VA_ARGS__)
/**
 * \~english
 * \~japanese
 * \brief
 * アプリケーションが想定しないエラーが発生した際に設定すべきログを出力します。
 */
#define FATAL_LOG(...)                                                      \
  adk::Log::GetInstance()->output("[FATAL]", adk::kFatalLog, __FUNCTION__, \
                             __VA_ARGS__)

class Log {
 public:
  Log();
  ~Log();
  /**
   * \~english
   * \~japanese
   * \brief ログ出力クラスのシングルトン
   */
  static Log* GetInstance();
  /**
   * \~english
   * \~japanese
   * \brief ログ出力の開始と停止を設定します。
   */
  void setLogOutput(TLogOutput log_output);
  /**
   * \~english
   * \~japanese
   * \brief 出力するログレベルを設定します。
   */
  void setLogLevel(TLogLevel log_level);
  /**
   * \~english
   * \~japanese
   * \brief ログの出力先を設定します。
   */
  void setLogMode(TLogMode log_mode);
  /**
   * \~english
   * \~japanese
   * \brief ファイル名を指定する。
   */
  void setFileName(const char* filename);
  void output(const char* header, TLogLevel level, const char* func,
              const char* format, ...);
  static bool Initialize();
  static bool Terminate();

 private:
  TLogLevel log_level_;
  TLogMode log_mode_;
  TLogOutput log_output_;
  static Log* instance_;
  char filename_[LOG_FILENAME_MAX_SIZE];
  const char* filename;
  char time_buf_[LOG_DATETIME_SIZE];
  FILE* file_;

  void setDateTime();
};

ADK_END_NAMESPACE

#endif /* LOGGING_H_ */

