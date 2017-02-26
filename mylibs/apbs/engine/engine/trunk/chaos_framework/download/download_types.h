//
//  download_types.h
//  Chaos
//
//  Created by CJ0048 on 2014/04/03.
//
//

#ifndef Chaos_download_types_h
#define Chaos_download_types_h

#include <core/types.h>

/**
 * \~english
 * \~japanese
 *  \brief リソース定義
 */
#define MAX_URL_SIZE (512)
#define MAX_DATE_SIZE (32)
#define MAX_DOWNLOAD_FILE_NUM (512)


CHAOS_BEGIN_NAMESPACE

/**\brief リソース処理時のステータス*/
typedef enum {
  /**\brief ダウンロード開始前*/
  kDownloadStatusIdle,
  /**\brief ダウンロード中*/
  kDownloadStatusDownloading,
  /**\brief 展開中*/
  kDownloadStatusExtracting,
  /**\brief 保存中*/
  kDownloadStatusSave,
  /**\brief 終了*/
  kDownloadStatusDone,
  /**\brief 終了*/
  kDownloadStatusError,
} TDownloadStatus;

/**\brief リソースのカテゴリー*/
typedef enum {
  /**\brief イメージ*/
  kResourceCategoryImage,
  /**\brief カードイメージ (カテゴリを細分化できる？)*/
  kResourceCategoryCardImage,
  /**\brief チャプターリソース*/
  kResourceCategoryChapter,
  /**\brief CCBI */
  kResourceCategoryCCBI,
} TResourceCategory;

typedef const Char* TResourceKey;

/**\brief リソースファイルの情報*/
typedef struct {
  /**\brief リソースID */
  int  resource_id;
  /**\brief url */
  char url[MAX_URL_SIZE];
  /**\brief 生成時のタイムスタンプ*/
  char create_date[MAX_DATE_SIZE];
} TResourceInfo;



#define MAX_FILE_PATH (255)


#define DUMMY_IMAGE_NAME "ui_dummy.png"

#ifndef KEY_STATUS
#define KEY_STATUS "status"
#endif

#define KEY_CATEGORY_RESOURCES "resources"
# define KEY_URL "url"
#define KEY_CATEGORY_RESOURCES_CHAPTER "chapterResource"
#define KEY_CATEGORY_RESOURCES_HD_CARD "hdCardResource"

CHAOS_END_NAMESPACE

#endif
