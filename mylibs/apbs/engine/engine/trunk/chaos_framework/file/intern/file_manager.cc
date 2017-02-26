#include <file/file_manager.h>
#include <core/library/lib_file_operations.h>
#include <core/library/lib_path_util.h>
#include <core/debug/logging.h>

CHAOS_BEGIN_NAMESPACE

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
  
}

Bool FileManager::Initialize()
{
  memset(category_table_, 0, sizeof(category_table_));
  category_table_count_=0;
  
  save_category_path_[0]=0;
  temporary_category_path_[0]=0;
  resource_category_path_[0]=0;
  user_category_path_[0]=0;
  
  return true;
}

Bool FileManager::Terminate()
{
  return true;
}
Bool FileManager::SaveFile(const Char* filename, const void* data, U64 data_size)
{
  adk::LIB_TFileObject* file = adk::LIB_FileOpen(filename, adk::LIB_kFileTypeWrite);
  
  if (file==0)
  {
    CRITICAL_LOG("FileManager::SaveFile (%s) open error!!\n");
    return false;
  }
  
  //ファイルへ書き込む
  if(adk::LIB_FileWrite(file, data, data_size)==false)
  {
    adk::LIB_FileClose(file);
    return false;
  }
 
  //ファイルをクローズする
  adk::LIB_FileClose(file);
  
  DEBUG_LOG("SaveFile (%s) %d bytes\n", filename, (U32)data_size);
  return true;
}

Bool FileManager::SaveFile(S32 category_id, const Char* filename, const void* data, U64 data_size)
{
  //パスを取得する
  const Char* path = GetUserCategoryPath(category_id);
  Char pathPlusFilename[MAX_FILE_PATH+1];
  S32 n = snprintf(pathPlusFilename, MAX_FILE_PATH, "%s%s", path, filename);
  ADK_ASSERT(n<MAX_FILE_PATH);
  
  
  return SaveFile(pathPlusFilename, data, data_size);
}

Bool FileManager::SaveFileWithCheckSum(S32 category_id, const Char* filename, const void* data, U64 data_size)
{
  //パスを取得する
  const Char* path = GetUserCategoryPath(category_id);
  Char pathPlusFilename[MAX_FILE_PATH+1];
  S32 n = snprintf(pathPlusFilename, MAX_FILE_PATH, "%s%s", path, filename);
  ADK_ASSERT(n<MAX_FILE_PATH);
  
  //ファイルオープンする
  adk::LIB_TFileObject* file = adk::LIB_FileOpen(pathPlusFilename, adk::LIB_kFileTypeWrite);
  if(file==0){
    return false;
  }
#if 0
  //チェックサムを計算する
  U32 *checksum = (U32*)((Char*)data + data_size);
  *checksum = CalculateCheckSum(data, data_size);
  
  //dataの後ろにチェックサムを付加してファイルへ書き込む
  if(adk::LIB_FileWrite(file, data, data_size + sizeof(U32))==false)
  {
    adk::LIB_FileClose(file);
    return false;
  }
#endif
  if(adk::LIB_FileWrite(file, data, data_size + sizeof(U32))==false)
  {
    adk::LIB_FileClose(file);
    return false;
  }

  U32 checksum = CalculateCheckSum(data, data_size);
  if (adk::LIB_FileWrite(file, &checksum, sizeof(checksum))==false)
  {
    adk::LIB_FileClose(file);
    return false;
  }
  
  //ファイルをクローズする
  adk::LIB_FileClose(file);
  return true;
}

BinaryData* FileManager::LoadFile(const Char *filename){
  //ファイルオープンする
  adk::LIB_TFileObject* file = adk::LIB_FileOpen(filename, adk::LIB_kFileTypeRead);
  if(file==0){
    adk:ERROR_LOG("file(%s) open error\n", filename);
    return 0;
  }
  
  //ファイルを読み込む
  //注意：メモリ確保しているので呼び出し側で解放処理が必要
  U64 data_size= adk::LIB_FileSize(file);
  BinaryData* binary_object = new BinaryData(data_size+1);
  U64 size = adk::LIB_FileRead(binary_object->GetBuffer(), file, binary_object->GetSize());
  if(size != data_size)
  {
    delete binary_object;
    adk::LIB_FileClose(file);
    ERROR_LOG("file(%s) read error\n", filename);
    return 0;
  }
  Char* data = static_cast<Char*>(binary_object->GetBuffer());
  data[data_size]=0;

  //ファイルをクローズする
  adk::LIB_FileClose(file);
  return binary_object;
}

BinaryData* FileManager::LoadFile(S32 category_id, const Char *filename){
  
  //パスを取得する
  const Char* path = GetUserCategoryPath(category_id);
  Char pathPlusFilename[MAX_FILE_PATH+1];
  S32 n = snprintf(pathPlusFilename, MAX_FILE_PATH, "%s%s", path, filename);
  ADK_ASSERT(n<MAX_FILE_PATH);
  
  return LoadFile(pathPlusFilename);
}

BinaryData* FileManager::LoadFileWithCheckSum(S32 category_id, const Char *filename){
  //パスを取得する
  const Char* path = GetUserCategoryPath(category_id);
  Char pathPlusFilename[MAX_FILE_PATH+1];
  S32 n = snprintf(pathPlusFilename, MAX_FILE_PATH, "%s%s", path, filename);
  ADK_ASSERT(n<MAX_FILE_PATH);
  
  //ファイルオープンする
  adk::LIB_TFileObject* file = adk::LIB_FileOpen(pathPlusFilename, adk::LIB_kFileTypeRead);
  if(file==0){
    adk:ERROR_LOG("file(%s) open error\n", pathPlusFilename);
    return 0;
  }
  
  //ファイルを読み込む
  //注意：メモリ確保しているので呼び出し側で解放処理が必要
  U64 data_size= adk::LIB_FileSize(file);
  BinaryData* binary_object = new BinaryData(data_size); // data + checksum
  U64 size = adk::LIB_FileRead(binary_object->GetBuffer(), file, data_size);
  
  if(size != data_size)
  {
    delete binary_object;
    adk::LIB_FileClose(file);
    ERROR_LOG("file(%s) read error\n", pathPlusFilename);
    return 0;
  }

  Char* data = (Char*)binary_object->GetBuffer();
  U32* checksum = (U32*)(data+data_size-sizeof(U32));
  if (*checksum == CalculateCheckSum(data, data_size-sizeof(U32)))
  {
    data[data_size-sizeof(U32)]=0;
    binary_object->SetSize(data_size-sizeof(U32));
  } else {
    //チェックサムが一致しない
    // ファイルが壊れた！
    CRITICAL_LOG("invalid checksum (%s)\n", pathPlusFilename);
    return 0;
  }

  //ファイルをクローズする
  adk::LIB_FileClose(file);
  return binary_object;
}

U32 FileManager::CalculateCheckSum(const void *data, U64 data_size)
{
  U32 sum=0;
  
  Char* d = (Char*)data;
  for(S32 i=0; i<data_size; i++)
  {
    sum += d[i];
  }
  
  return sum;
}

static TCategoryTable* GetCategoryTable(const TCategoryTable* table, S32 table_count, S32 user_category_id)
{
  for (int i=0; i<table_count; i++)
  {
    if (table[i].user_category_id==user_category_id)
    {
      return (TCategoryTable*)&table[i];
    }
  }
  return 0;
}

Bool FileManager::RegisterCategoryPath(TFileManagerCategory category_id, S32 user_category_id, const Char* path, Bool is_make_dir)
{
  //すでに登録されているカテゴリIDと同じカテゴリIDでは登録できないようにする
  TCategoryTable* category = GetCategoryTable(category_table_, category_table_count_, user_category_id);
  
  if (category)
  {
    ADK_ASSERT_MSG(!category, "This category is already registered !!");
    return false;
  }
 
  if (category_table_count_==MAX_CATEGORY_COUNT)
  {
    ADK_ASSERT_MSG(category_table_count_!=MAX_CATEGORY_COUNT, "over MAX_CATEGORY_COUNT !!");
    return false;
  }
  
  category = &category_table_[category_table_count_++];
  category->category_id = category_id;
  category->user_category_id= user_category_id;
  S32 n=snprintf(category->path, sizeof(category->path), "%s%s/", GetCategoryPath(category_id), path);

  if (n==sizeof(category->path))
  {
    ADK_ASSERT_MSG(n<sizeof(category->path), "path is too long!")
    return false;
  }
  
  if (is_make_dir)
  {
    MakeDirectory(user_category_id, "");
  }

  return true;
}

#if 0
// このメソッド必要であるかどうかもよく分からない。
// どうしよう。
void FileManager::UnregisterCategoryPath(S32 category_id)
{
  //このメソッドは呼ばれることはないはず
  return ;
}
#endif

const Char* FileManager::GetCategoryPath(TFileManagerCategory category_id)
{
  switch (category_id)
  {
    case kFileManagerCategorySave:
    {
      if (save_category_path_[0]==0)
      {
        adk::LIB_GetSaveDataDir(save_category_path_, sizeof(save_category_path_));
      }
      return save_category_path_;
    }
    case kFileManagerCategoryTemporary:
    {
      if (temporary_category_path_[0]==0)
      {
        adk::LIB_GetTemporaryDir(temporary_category_path_, sizeof(temporary_category_path_));
      }
      return temporary_category_path_;
    }
    case kFileManagerCategoryResource:
    {
      if (resource_category_path_[0]==0)
      {
        adk::LIB_GetResourceDir(resource_category_path_, sizeof(resource_category_path_));
      }
      return resource_category_path_;
    }
    case kFileManagerCategoryUser:
    {
      if (user_category_path_[0]==0)
      {
        adk::LIB_GetUserDir(user_category_path_, sizeof(user_category_path_));
      }
      return user_category_path_;
    }
    default:
      break;
  }
  
  ADK_ASSERT_MSG(0, "Invalid category_id\n");
  return 0;
}



const Char* FileManager::GetUserCategoryPath(S32 category_id)
{

  for (int i=0; i<category_table_count_; i++)
  {
    if (category_table_[i].category_id==category_id)
    {
      return category_table_[i].path;
    }
  }
  ADK_ASSERT(0);
  return 0;
}
  

Bool FileManager::MakeDirectory(S32 category_id, const Char* dir_name)
{
  S32 ret=0;
  Char full_path[MAX_FILE_PATH+1];
  S32 n = snprintf(full_path, MAX_FILE_PATH, "%s%s", GetUserCategoryPath(category_id), dir_name);
  ADK_ASSERT(n<MAX_FILE_PATH);

/** これはどうしよう。。。
* adk::PathUtilに入れる。
*/
#ifdef ANDROID_CHAOS
  ret = mkdir(full_path, 0770);
#else
  ret = mkpath_np(full_path, 0755);
#endif

  return (ret==0);
}


CHAOS_END_NAMESPACE
