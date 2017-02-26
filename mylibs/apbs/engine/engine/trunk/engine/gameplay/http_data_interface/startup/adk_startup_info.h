/**
 * \file adk_startup_info.h
 * \ingroup adkgrp_startup
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief ADKの起動情報に関する定義部
 */


#ifndef ADK_STARTUP_INFO_H_
#define ADK_STARTUP_INFO_H_


#include <core/debug/logging.h>
#include <core/library/lib_json_document.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/types.h>
#include <debug_profiling/error/result.h>
#include <network/http/adk_np_types.h>


ADK_BEGIN_NAMESPACE


typedef enum {
  kSIE_SUCCESS=0,
  kSIE_RESOURCE_DIR_NOT_FOUND=1,
  kSIE_KEY_WAS_NOT_FOUND,
  kSIE_INVALID_VALUE
} TStartupInfoError;

/**
 * \~english
 * \~japanese
 *  \brief ADK起動情報管理クラス
 */
class ADK_StartupInfo {
 public:
   /**
    * \~english
    * \~japanese
    *   \brief コンストラクタ
    */
  ADK_StartupInfo();
   /**
    * \~english
    * \~japanese
    *   \brief デストラクタ
    */
  ~ADK_StartupInfo();

  /**
   * \~english
   * \~japanese
   *  \brief 起動情報の初期化処理を行います
   *  \param[in] filename 読み込むリソースファイル名
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval ResultSuccess 初期化に成功したときに返ります
   *  \retval ResultDoAlready 既に初期化されているのに呼び出されたときに返ります
   *  \retval ResultOutOfResource システムリソースが不足しているときに返ります
   */
  Result Initialize(const char* filename);
  /**
   * \~english
   * \~japanese
   *  \brief 起動情報の終了処理を行います
   *  \return 終了の成功判定。以下の値が返ります
   *  \retval ResultSuccess 終了に成功したときに返ります
   *  \retval ResultNotInitialized 初期化されてないのに呼び出されたときに返ります
   */
  Result Terminate();

  /* debug management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief デバッグの有無を返します
   *  \return デバッグフラグ。以下の値が返ります
   *  \retval true デバッグが有効なときに返ります
   *  \retval false デバッグが無効なときに返ります
   */
  Bool IsDebug() const;
  /**
   * \~english
   * \~japanese
   *  \brief ログレベルを返します
   *  \return ログレベル。\ref TLogLevel
   */
  TLogLevel GetLogLevel() const;
  /**
   * \~english
   * \~japanese
   *  \brief ログの出力先を返します
   *  \return 出力先。\ref TLogOutput
   */
  TLogMode GetLogOutput() const;
  /**
   * \~english
   * \~japanese
   *  \brief ログを出力する際のファイル名を返します
   *  \return ログファイル名
   */
  const Char* GetLogFileName() const;

  /* host list management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief マスターサーバーのホスト名を返します
   *  \return ホスト名
   */
  const Char* GetMasterServerUrl() const;
  /**
   * \~english
   * \~japanese
   *  \brief ホスト名リスト数を返します
   *  \return リスト数
   */
  S32 GetHostListCount() const;
  /**
   * \~english
   * \~japanese
   *  \brief ホスト名を返します
   *  \param[in] host ホスト名のインデックス 0 <= host < \ref GetHostListCount()
   *  \return ホスト名
   */
  const Char *GetHost(S32 host) const;
  /**
   * \~english
   * \~japanese
   *  \brief ホストのディスプレイ名を返します
   *  \param[in] host ホスト名のインデックス 0 <= host < \ref GetHostListCount()
   *  \return ディスプレイ名
   */
  const Char *GetHostDisplayName(S32 host) const;

  /* network management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief キープアライブ時間を返します
   *  \return 時間
   */
  S32 GetKeepAliveTime() const;
  /**
   * \~english
   * \~japanese
   *  \brief 自動リトライ数を返します
   *  \return リトライ数
   */
  S32 GetAutoRetryCount() const;
  /**
   * \~english
   * \~japanese
   *  \brief 接続時のタイムアウト時間を返します
   *  \return タイムアウト時間
   */
  S32 GetConnectTimeout() const;
  /**
   * \~english
   * \~japanese
   *  \brief 読み込み時のタイムアウト時間を返します
   *  \return タイムアウト時間
   */
  S32 GetReadTimeout() const;
  /**
   * \~english
   * \~japanese
   *  \brief リクエストヘッダのキーのバッファサイズ
   */
  U32 GetHttpKeyLength() const;
  /**
   * \~english
   * \~japanese
   *  \brief リクエストヘッダの値のバッファサイズ
   */
  U32 GetHttpValueLength() const;

private:
  /* vars */
  /**
   * \~english
   * \~japanese
   *  \brief "adk_startup.adkinfo"のバイナリ情報
   */
  struct {
    /**
     * \~english
     * \~japanese
     *  \brief バイナリの先頭アドレス
     */
    Byte  *addr;
    /**
     * \~english
     * \~japanese
     *  \brief バイナリサイズ
     */
    U32 size;
  }_binary;
  /**
   * \~english
   * \~japanese
   *  \brief "adk_startup.adkinfo"を読み込んだJSONドキュメント
   */
  LIB_JsonDocument  _jdoc;

  /**
   * \~english
   * \~japanese
   *   \brief クラス内 new/delete オペレータ定義
   */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("ADK_StartupInfo");
  /**
   * \~english
   * \~japanese
   *   \brief コピー禁止定義
   */
  ADK_NON_COPYABLE_CLASS(ADK_StartupInfo);
};

ADK_END_NAMESPACE
#endif
