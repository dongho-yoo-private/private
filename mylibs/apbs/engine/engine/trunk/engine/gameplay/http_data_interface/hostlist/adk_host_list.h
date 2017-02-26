/**
 * \file adk_host_list.h
 * \ingroup adkgrp_hostlist
 * \author tomonori tanaka & dongho.yoo
 * \~english
 * \~japanese
 *   \brief 接続先サーバー情報を扱うところ
 */


#ifndef ADK_HOSTLIST_H__
#define ADK_HOSTLIST_H__


#include <core/types.h>
#include <debug_profiling/error/result.h>
#include <core/memory_allocation/guarded_alloc.h>


ADK_BEGIN_NAMESPACE

/**\brief ADK_HostListの中で使われるエラーコードです。
*/
typedef enum {
  kHLE_SUCCESS,
  kHLE_MASTER_WAS_NOT_FOUND
} THostListError;

/* class definition */
class ADK_HostList{
public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   */
  ADK_HostList();
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  virtual ~ADK_HostList();
  /**
   * \~english
   * \~japanese
   *   \brief サーバー情報を使用するための初期化処理を行います
   *   \return Result
   */
  Result Initialize();
  /**
   * \~english
   * \~japanese
   *   \brief 使用されているサーバー情報を終了します
   *   \return 終了処理の成功判定。以下の値が返ります
   */
  Result Terminate();
  /**
   * \~english
   * \~japanese
   *   \brief サーバー情報クラスを返します
   *   \return \ref ADK_HostList
   */
  const Char* GetMasterServerUrl() const;

  /* server management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief [未実装]接続可能なサーバー数を返します。
   *   \return サーバー数
   */
  int GetServerCount() const;
  /**
   * \~english
   * \~japanese
   *   \brief [未実装]接続可能なサーバーのURLを返します
   *   \param[in] server 取得したいサーバーのインデックス 0 <= server < \ref GetServerCount()
   *   \return URL
   */
  const char *GetServerUrl( S32 server) const;
  /**
   * \~english
   * \~japanese
   *   \brief [未実装]接続可能なサーバーの表示名を返します
   *   \param[in] server 取得したいサーバーのインデックス 0 <= server < \ref GetServerCount()
   *   \return 名前
   */
  const char *GetServerDisplayName( S32 server) const;
  /**
   * \~english
   * \~japanese
   *   \brief 接続先サーバーの URL を設定します。
   *   \attention 現状はネイティブ側で接続先URLが設定されているので用意した関数。\n
   *              最終的には SDK 側で接続先サーバー数と各サーバー情報を取得できるAPIを提供して選択してもらう形式にする。
   *   \param[in] url 接続先サーバーの URL
   */
  Result SetAccessPoint( const char *url);
  /**
   * \~english
   * \~japanese
   *   \brief 接続先サーバーの URL を返します
   *   \return URL
   */
  const char *GetAccessPoint() const;
  /**
   * \~english
   * \~japanese
   *   \brief 接続先サーバーの SUffix を返します
   *   \return Suffix
   */
  const char *GetAccessPointSuffix() const;

private:
  /* vars */
  /**
   * \~english
   * \~japanese
   *   \brief アクセスポイント
   */
  struct{
    char  *url;
    char  *suffix;
  }_accessPoint;

  S32  _hostCounts;
  Char*  _masterUrl;

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("ADK_HostList");
  ADK_NON_COPYABLE_CLASS(ADK_HostList);
};


ADK_END_NAMESPACE


#endif  /* ADK_HOSTLIST_H__ */


