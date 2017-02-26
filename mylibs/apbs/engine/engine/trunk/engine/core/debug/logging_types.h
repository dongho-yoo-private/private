/**
 * \file log_types.h
 * \ingrup log
 * \author Kohei konno
 * \~english
 * \~japanese
 * \brief デバッグ機能
 */

#ifndef LOGGING_TYPES_H_
#define LOGGING_TYPES_H_

#include <core/types.h>

ADK_BEGIN_NAMESPACE

typedef enum {
  /**
   * \~english
   * \~japanese
   * \brief ログレベルを未指定し、出力しない。
   */
  kNoneLog = 0x0000,
  /**
   * \~english
   * \~japanese
   * \brief 主に実装者が開発中に必要なときに組み込むログ。
   */
  kDebugLog = 0x0001,
  /**
   * \~english
   * \~japanese
   * \brief
   * ファイルの読み込み完了、受信したパケット情報、現在のメモリ状況など情報。
   */
  kInfoLog = 0x0002,
  /**
   * \~english
   * \~japanese
   * \brief 何が起きたか、フローが変わったのか、イベントが発生したか等。
   */
  kNoticeLog = 0x0004,
  /**
   * \~english
   * \~japanese
   * \brief エンジニアに警告し、すぐ確認させる為のログ。
   */
  kWarningLog = 0x0008,
  /**
   * \~english
   * \~japanese
   * \brief アプリケーション的にあり得るエラーハンドリング時に出力すべきログ。
   */
  kErrorLog = 0x0010,
  /**
   * \~english
   * \~japanese
   * \brief コードとして不正なアクセスやオーバーフローなど、実行エラーログ。
   */
  kCriticalLog = 0x0020,
  /**
   * \~english
   * \~japanese
   * \brief アプリケーションが想定しないエラーが発生した際に設定すべきログ。
   */
  kFatalLog = 0x0040,
  /**
   * \~english
   * \~japanese
   * \brief 全てのログ。
   */
  kAllLog = 0xFFFF
} TLogLevel;

typedef enum {
  /**
   * \~english
   * \~japanese
   * \brief 全てのログ出力を停止します。
   */
  kStopLog = 0,
  /**
   * \~english
   * \~japanese
   * \brief 全てのログ出力を開始します。
   */
  kStartLog = 1
} TLogOutput;

typedef enum {
  /**
   * \~english
   * \~japanese
   * \brief 全てのログをコンソールに出力します。
   */
  kConsoleLog = 0,
  /**
   * \~english
   * \~japanese
   * \brief 全てのログをファイルに書き出します。
   */
  kFileLog = 1
} TLogMode;

ADK_END_NAMESPACE

#endif /* LOGGING_TYPES_H_ */

