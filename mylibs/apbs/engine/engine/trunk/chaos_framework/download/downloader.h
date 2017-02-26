//
//  download_manager.h
//  Chaos
//
//  Created by CJ0048 on 2014/04/03.
//
//

#ifndef _CHAOS_DOWNLOADER_H_
#define _CHAOS_DOWNLOADER_H_

#include "application/application_types.h"
#include "download/download_types.h"
#include <core/types.h>
#include <gameplay/http_data_interface/api/api.h>

#define MAX_URL_PATH_LEN (255)

/** \brief 臨時的なマクロ*/
#define MASTER_DATA_DOWNLOAD_DIRECT

CHAOS_BEGIN_NAMESPACE

/**
* \brief マスターデータやリソースをダウンロードするクラス。
*/
class Downloader {
 public:
  /**
   * \~english
   * \~japanese
   *   \brief デフォルトコンストラクタ
   */
  Downloader();
  
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  ~Downloader();
  
  /**
   * \~english
   * \~japanese
   *  \brief リソースマネージャを初期化する
   *  \return 終了処理の成功判定。以下の値が返ります
   *  \retval true 終了処理に成功したときに返ります
   *  \retval false 終了処理に成功したときに返ります
   */
  Bool Initialize();
  
  /**
   * \~english
   * \~japanese
   *  \brief リソースマネージャを終了する
   */
  void Terminate();
  
  /* \brief りソースダウンロード開始
   * 失敗時には失敗時のイベントを発行します。
   *
   * \param save_category 保存先のカテゴリ
   * \param list リソースのリスト
   * \param list_count リソースのリストの数
   *
   * \param is_back_ground trueであれば、バックグラウンド、falseであれば、ブロック
   */
  Bool DownloadResourceList(S32 save_category, const TResourceInfo* list, S32 list_count, Bool is_back_ground);
  
  /* \brief 個別のリソースダウンロード開始し、保存まで行う。(ディレクトリを含むZIPの処理
   *
   * \param save_category 保存先のカテゴリ
   * \param url ダウンロード先のurl
   */
  Bool DownloadResource(S32 save_category, const Char* url);
  
#ifdef  MASTER_DATA_DOWNLOAD_DIRECT
  /* \brief マスターデータダウンロード開始します。\n
   * 失敗時には失敗時のイベントを発行します。
   * 臨時的なインターフェース。
   *
   * \param url マスターデータをダウンロードするためのURL
   * \param useThread trueであれば、バックグラウンドで、falseであれば、ブロック
  */
  void DownloadMasterData(const Char* host_name, const Char* url,
                          const Char* request_data, Bool is_back_gournd);
#endif
  
  /* \brief プログレスの状態を取得
   *
   * \param [out] status ダウンロード時のステータス \ref TDownloadStatus
   */
  ADK_INLINE F32 GetProgress(TDownloadStatus* status)
  {
    if (status!=0)
    {
      *status = progress_status_;
    }
    return progress_;
  }
  
  /**\brief リソースをロードします。
   * \param [in] key リソースのキー（現状はファイルパス)
   * \param [in] type リソースのカテゴリ
   * \param [out] size ロードされたリソースのサイズ
   *
   * \return NULLでなければ、成功。
   */
  void* LoadResource(TResourceKey key, TResourceCategory type, S32* size);
  
  /**\brief LoadResourceでロードしたリソースをアンロードします。
   *
   * \param resource リソースのアドレス
   */
  void UnloadResource(void* resource);
  
  /**\brief リソースを保存します。
   * \param [in] key リソースのキー（現状はファイルパス)
   * \param [in] type リソースのカテゴリ
   * \param [in] data データのアドレス
   * \param [in] size データのサイズ
   *
   * \param true成功、false失敗
   */
  Bool SaveResource(TResourceKey* key, TResourceCategory type, void* data, S32 size);
  
  /**\brief ダウンロードを一時中止させます。
   */
  void PauseDownload();
  
  /**\brief ダウンロードを再開させます。
   */
  void ResumeDownload();
  
  
private:
  TResourceInfo* resource_info_;
  S32 resource_info_count_;
  TResourceInfo* hd_card_resource_info_;
  S32 hd_card_resource_info_count_;
  F32 progress_;
  S32 current_resource_download_count_;
  TDownloadStatus progress_status_;
  
  S32 user_category_id_;
  
  IntPtr thread_id_;
  
  ADK_INLINE void SetStatus(F32 progress, TDownloadStatus progress_status)
  {
    progress_=progress;
    progress_status_=progress_status;
  }
  
  
  
  Bool UnZipAndSaveToFile(S32 user_category_id, const void* resource_zip, S32 size);
  Bool SaveFile(S32 category_id, const Char* org_file_name, const void* data, U64 data_size);
  Bool is_pause_;
  
  Char master_data_url_[MAX_URL_PATH_LEN+1];
  
  
};


CHAOS_END_NAMESPACE


#endif /* defined(CHAOS_DOWNLOAD_MANAGER_H_) */
