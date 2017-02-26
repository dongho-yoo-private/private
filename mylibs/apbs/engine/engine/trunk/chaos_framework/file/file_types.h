//
//  file_types.h
//  Chaos
//
//  Created by CJ0048 on 2014/04/03.
//
//

#ifndef Chaos_file_types_h
#define Chaos_file_types_h
#include <application/application_types.h>

#define MAX_FILE_PATH (255)
#define MAX_CATEGORY_COUNT (255)

CHAOS_BEGIN_NAMESPACE

typedef S32 TCategoryId;

/**\brief 大きなカテゴリ*/
typedef enum {
  /**\breif データ保存のためのディレクトリ*/
  kFileManagerCategorySave=1,
  /**\brief ユーザディレクトリ*/
  kFileManagerCategoryUser,
  /**\brief リソースディレクトリー ReadOnly*/
  kFileManagerCategoryResource,
  /**\brief テンポラリーディレクトリ 臨時的なディレクトリ*/
  kFileManagerCategoryTemporary,
  
} TFileManagerCategory;

CHAOS_END_NAMESPACE

#endif
