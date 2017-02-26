//
//  file_manager.h
//  Chaos
//
//  Created by CJ0048 on 2014/04/03.
//
//

#ifndef __Chaos__file_manager__
#define __Chaos__file_manager__

#include <core/types.h>
#include "file/file_types.h"
#include "file/binary_data.h"
#include "application/application_types.h"

CHAOS_BEGIN_NAMESPACE

typedef struct {
  S32 category_id;
  S32 user_category_id;
  Char path[MAX_FILE_PATH+1];
} TCategoryTable;


/**
* \brief ファイルの管理を行います。\n
*/
class FileManager{
 public:
  /**
   * \~english
   * \~japanese
   *   \brief デフォルトコンストラクタ
   */
  FileManager();
  
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  virtual ~FileManager();

  /**
   * \~english
   * \~japanese
   *   \brief FileManagerの初期化処理を行います。
   */
  Bool Initialize();
  
  /**
   * \~english
   * \~japanese
   *   \brief FileManagerの終了化処理を行います。
   */
  Bool Terminate();
  
  /**
   * \brief ファイルを保存します。
   *   \param  filename [in]    user_categoryからの相対パス
   *   \param data[in]         保存するデータへのポインタ
   *   \param data_size [in]   保存するデータのサイズ
  */
  Bool SaveFile(const Char* filename, const void* data, U64 data_size);

  /**
   * \~english
   * \~japanese
   *   \brief ファイルを保存します。
   *   \param category_id [in] カテゴリID(カテゴリIDから保存するパスを特定する）
   *   \param  filename [in]    user_categoryからの相対パス
   *   \param data[in]         保存するデータへのポインタ
   *   \param data_size [in]   保存するデータのサイズ
   */
  Bool SaveFile(S32 category_id, const Char* filename,
                const void* data,
                U64 data_size);
  /**
   * \~english
   * \~japanese
   *   \brief ファイルを保存します。
   *   \param  category_id [in] カテゴリID
   *   \param  filename [in]    user_categoryからの相対パス
   *   \param data[in]         保存するデータへのポインタ
   *   \param data_size [in]   保存するデータのサイズ
   */
  Bool SaveFileWithCheckSum(S32 category_id, const Char* filename,
                            const void* data,
                            U64 data_size);
 
  /**
  * \brief ファイルを読み込みます。
  *
  *   \param  filename [in]    user_categoryからの相対パス
  *   \return バイナリデータへのポインタ
  */
  BinaryData* LoadFile(const Char* filename);
  
  /**
   * \~english
   * \~japanese
   *   \brief ファイルを読み込みます。
   *   \param  category_id [in] カテゴリID
   *   \param  filename [in]    user_categoryからの相対パス
   *   \return バイナリデータへのポインタ
   */
  BinaryData* LoadFile(S32 category_id, const Char* filename);
  
  /**
   * \~english
   * \~japanese
   *   \brief ファイルを読み込みます。
   *   \param  category_id [in] カテゴリID
   *   \param  filename [in]    user_categoryからの相対パス
   *   \return バイナリデータへのポインタ
   */
  BinaryData* LoadFileWithCheckSum(S32 user_category_id, const Char* filename);
 
  /**
   * \~english
   * \~japanese
   *   \brief カテゴリIDに対応するディレクトリパスを登録します。
   *   \param category_id [in] カテゴリID \ref TFileManagerCategory
   *   \param user_category_id [in] ユーザが指定するカテゴリID
   *   \param path [in]        登録するディレクトリパス
   *  \paramm is_create_directory 登録時にディレクトリを作成するかどうか
   */
  Bool RegisterCategoryPath(TFileManagerCategory category_id, S32 user_category_id, const Char *path, Bool is_create_directory);
  
  /**
   * \~english
   * \~japanese
   *   \brief カテゴリIDに対応するディレクトリパスを取得します。
   *   \param category_id [in] カテゴリID
   *   \return パスへのポインタが返ります。
   */
  const Char* GetUserCategoryPath(S32 user_category_id);
  
   /**
   * \~english
   * \~japanese
   *   \brief カテゴリIDに対応するディレクトリパスを取得します。
   *   \param category_id [in] カテゴリID
   *   \return パスへのポインタが返ります。
   */
  const Char* GetCategoryPath(TFileManagerCategory category_id);
  
  /**
   * \~english
   * \~japanese
   *   \brief ディレクトリを作成します。
   *   \param dir_name 作成するディレクトリ名
   *   \param category_id カテゴリID 
   *   \return ディレクトリの作成が成功したか失敗したかを返します
   *   \retval true  ディレクトリの作成成功
   *   \retval false ディレクトリの作成失敗
   */
  Bool MakeDirectory(S32 user_category, const Char *dir_name);
  
  /**
   * \~english
   * \~japanese
   * \param user_category_id ユーザカテゴリID
   * \param filename 削除するファイル名
   *
   * \return 削除されたかどうか
   * \retval true  削除された
   * \retval false 削除されなかった
  */
  Bool DeleteFile(S32 user_category_id, const Char* filename);
  
 private:
   /**
   * \~english
   * \~japanese
   *   \brief チェックサムを計算します。
   *   \param  data [in] データ
   *   \param  dat_size [in] データサイズ
   *   \return チェックサムの計算結果
   */
  U32 CalculateCheckSum(const void* data, U64 data_size);
 
  TCategoryTable category_table_[MAX_CATEGORY_COUNT];
  S32  category_table_count_;
  
  /** \brief 保存ディレクトリ*/
  Char save_category_path_[MAX_FILE_PATH+1];
  /** \brief 臨時的なディレクトリ*/
  Char temporary_category_path_[MAX_FILE_PATH+1];
  /** \brief リソースディレクトリ*/
  Char resource_category_path_[MAX_FILE_PATH+1];
  /** \brief ユーザディレクトリ*/
  Char user_category_path_[MAX_FILE_PATH+1];
  
};


CHAOS_END_NAMESPACE

#endif /* defined(__Chaos__file_manager__) */
