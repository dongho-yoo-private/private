//
//  download_manager.cc
//  Chaos
//
//  Created by CJ0048 on 2014/04/03.
//
//

#include "download/downloader.h"
#include <core/types.h>
#include <core/library/lib_string.h>
#include <core/library/lib_thread.h>
#include <core/library/lib_json_document.h>
#include <core/library/lib_zlib.h>
#include <core/library/lib_path_util.h>
#include <network/http/intern/http.h>
#include <network/http/adk_http_response.h>
#include "parameter/parameter_manager.h"
#include "file/file_types.h"
#include "file/file_manager.h"

#include "game/game_manager.h"

#define MAX_FILE_NAME_LEN (255)

CHAOS_BEGIN_NAMESPACE

Downloader::Downloader()
: resource_info_(0),
resource_info_count_(0),
hd_card_resource_info_(0),
hd_card_resource_info_count_(0),
progress_(0.0),
progress_status_(kDownloadStatusIdle),
current_resource_download_count_(0),
is_pause_(false),
user_category_id_(0)
{
  
}

Downloader::~Downloader()
{
}

Bool Downloader::Initialize()
{
  master_data_url_[0]=0;
  return true;
}

void Downloader::Terminate()
{
  if (resource_info_)
  {
    free(resource_info_);
  }
  resource_info_count_=0;
  
  return ;
}

#ifdef  MASTER_DATA_DOWNLOAD_DIRECT

#define MAX_URL_LEN (255)
#define MAX_HOST_NAME_LEN (255)
#define MAX_HTTP_HEADER_LEN (255)
#define MAX_REQUEST_DATA_LEN (1023)

typedef struct {
  Char host_name[MAX_HOST_NAME_LEN+1];
  Char url[MAX_URL_LEN+1];
  Char http_header[MAX_HTTP_HEADER_LEN+1];
  Char request_data[MAX_REQUEST_DATA_LEN+1];
} TMasterDataDownloadParam;

TMasterDataDownloadParam g_master_data_param;
static const adk::HttpResponse* HttpGetMasterData(const Char* host_name, const Char* url, const Char* request_data)
{
  Char _hostname[MAX_HOST_NAME_LEN+1];
  Char header[MAX_HTTP_HEADER_LEN];
  adk::HandleHttp http = adk::HttpCreate(0, host_name, 0, 0);
  
  ADK_ASSERT(http);
  
  S32 data_size = strlen(request_data);
  
  adk::HttpMakeHeaderInBuffer(header, sizeof(header), host_name,
                              data_size, 0, url, 0, 0, 0);
  adk::THttpError error = adk::HttpMessage(http, header, request_data, data_size);
  
  if (error!=0)
  {
    return 0;
  }
  
  const adk::HttpResponse* response = adk::HttpGetResponse(http);
  
  adk::HttpClose(http);
  
  return response;
}

static void OnStartMasterDataUpdateDirect(void* user_data)
{
  Downloader* downloader = (Downloader*)user_data;
  
  downloader->DownloadMasterData(g_master_data_param.host_name, g_master_data_param.url,
                                  g_master_data_param.request_data, false);
  return ;
}
void Downloader::DownloadMasterData(const Char* host_name, const Char* url,
                                    const Char* request_data, Bool is_back_ground)
{
  thread_id_=0;
  if (is_back_ground==true)
  {
    strcpy(g_master_data_param.url, url);
    strcpy(g_master_data_param.request_data, request_data);
    thread_id_ = adk::LIB_ThreadCreate(OnStartMasterDataUpdateDirect, this, false);
    if (thread_id_==0)
    {
      /** TODO Generate Event */
      SetStatus(0.0, kDownloadStatusIdle);
    }
    return ;
  }
  
  SetStatus(0.0, kDownloadStatusDownloading);
  const adk::HttpResponse* download_data = HttpGetMasterData(host_name, url, request_data);
  
  if (download_data==0 ||
      download_data->GetStatus()!=200)
  {
    /** TODO Generate Event */
    SetStatus(0.0, kDownloadStatusIdle);
    return ;
  }
  
  adk::LIB_JsonDocument doc;
  SetStatus(progress_, kDownloadStatusExtracting);
 
  if (download_data->GetStatus()!=200)
  {
    return ;
  }
 
  /** 臨時的な対応。
  * 何故かstatusが入ってない。
  */
  if (download_data->GetDataSize()<1024)
  {
    return ;
  }
  
  if (doc.Parse((Char *)download_data->GetData(), false, 0)!= adk::ResultSuccess)
  {
    /** TODO Generate Event */
    delete download_data;
    SetStatus(0.0, kDownloadStatusIdle);
    return ;
  }
  
  //TODO ParameterManagerインスタンスの取得
  ParameterManager* pm = ApplicationManager::GetInstance()->GetParameterManager();
  
  const adk::LIB_TJsonMember& status = doc[KEY_STATUS];
 
#if 0
  if (status.data.IsNumber()==false)
  {
    /** TODO Generate Event*/
    SetStatus(0.0, kDownloadStatusIdle);
    doc.Release();
    return ;
  }

  if (status.data.GetS32()!=200)
  {
    /** TODO Generate Event*/
    SetStatus(0.0, kDownloadStatusIdle);
    doc.Release();
    return ;
  }
#endif

  pm->OnPreload(doc);
  
  /** リソースリストを取得*/
#if 0
  const adk::LIB_TJsonMember& resources = doc[KEY_CATEGORY_RESOURCES];
  if (resources.data.IsArray()==true)
  {
    LoadResources(resources.data);
  }
  
  adk::LIB_TJsonMember& chapterResources = doc[KEY_CATEGORY_RESOURCES_CHAPTER];
  if (chapterResources.data.IsArray()==true)
  {
    LoadChapterResources(chapterResources.data);
  }
  
  adk::LIB_TJsonMember& hdCardResources = doc[KEY_CATEGORY_RESOURCES_HD_CARD];
  if (hdCardResources.data.IsArray()==true)
  {
    LoadHdCardResources(hdCardResources.data);
  }
#endif
  
  SetStatus(100.0, kDownloadStatusIdle);
  
  doc.Release();
  delete download_data; 
  
}
#endif

static const adk::HttpResponse* HttpGet(const Char* url)
{
  adk::THttpError error;
  adk::HandleHttp http = adk::HttpCreate(0, url, 0, &error);
  if (http==0)
  {
    return 0;
  }
  error = adk::HttpGet(http, url);
  
  if (error!=0)
  {
    adk::HttpClose(http);
    return 0;
  }
  return adk::HttpGetResponse(http);
}

Bool Downloader::DownloadResource(S32 user_category_id, const Char* url)
{
  const adk::HttpResponse* download_resource = HttpGet(url);
  
  SetStatus(progress_, kDownloadStatusExtracting);
  
  if (download_resource==0)
  {
    return false;
  }
  
  /** zip名のディレクトリを作っておく。なぜなら、zipの中にディレクトリがない場合があるため*/
  S32 len = strlen(url);
  Char* p=(Char*)url;
  char dir[MAX_FILE_NAME_LEN+1];
  dir[0]=0;
  for (int i=0; i<len; i++)
  {
    if (p[len-1-i]=='/')
    {
      adk::LIB_strcpyu(dir,p+len-i,".");
      break;
    }
  }
  
  if (dir[0])
  {
    CHAOS::FileManager* file_manager = CHAOS::ApplicationManager::GetInstance()->GetFileManager();
    file_manager->MakeDirectory(user_category_id, dir);
  }
  
  Bool ret = UnZipAndSaveToFile(user_category_id, download_resource->GetData(), download_resource->GetDataSize());
  delete download_resource;
  return ret;
}

static void OnDownloadResourceList(void* user_param)
{
  Downloader* download_manager = (Downloader*)user_param;
  download_manager->DownloadResourceList(0, 0, 0, false);
}

Bool Downloader::DownloadResourceList(S32 user_category_id, const TResourceInfo* list, S32 list_count, Bool is_back_ground)
{
  if (list)
  {
    resource_info_= (TResourceInfo*)list;
    resource_info_count_=list_count;
  }
  
  if (user_category_id)
  {
    user_category_id_=user_category_id;
  }
  
  if (thread_id_==0 &&
      is_back_ground==true)
  {
    thread_id_ = adk::LIB_ThreadCreate(OnDownloadResourceList, this, true);
    if (thread_id_==0)
    {
      SetStatus(0.0, kDownloadStatusError);
      return false;
    }
  }
  
  for (S32 i=current_resource_download_count_; i<resource_info_count_;++i)
  {
    F32 progress = ((F32)i/(F32)resource_info_count_)*100.0;
    
    if (is_pause_)
    {
      current_resource_download_count_=i;
      SetStatus(0.0, kDownloadStatusIdle);
      break;
    }
    
    SetStatus(progress, kDownloadStatusIdle);
    if (DownloadResource(user_category_id, resource_info_[i].url)==false)
    {
      /** TODO generate event. */
      SetStatus(progress, kDownloadStatusError);
      return false;
    }
  }
  return true;
}


#define TEMPORARY_FILE_NAME "tempfile"

Bool Downloader::UnZipAndSaveToFile(S32 category_id, const void* resource_zip, S32 size)
{
  //TODO FileManagerインスタンスの取得
  FileManager* file_manager = ApplicationManager::GetInstance()->GetFileManager();
  
  bool isSuccess = false;
  unsigned int beforeSize=0;
  char* extract_buffer=0;
  S32 before_size=0;
  
  // HZIP hZip = LIBZ_UnzOpen(resource_zip, size);
  /** とりあえず、効率は悪いが、臨時的な対応 */
  const Char* temporary_path = file_manager->GetCategoryPath(kFileManagerCategoryTemporary);
  
  ADK_ASSERT(temporary_path);
  char temporary_file_path[MAX_FILE_PATH+1];

  /** 臨時的な対応*/
  int n = snprintf(temporary_file_path, MAX_FILE_PATH, "%s/%s", temporary_path, TEMPORARY_FILE_NAME);
  ADK_ASSERT(n!=MAX_FILE_PATH);
  
  if (file_manager->SaveFile(temporary_file_path, resource_zip, size)==false)
  {
    return false;
  }
  
  adk::HZIP hZip = adk::LIBZ_UnzOpen(temporary_file_path);
  
  if (hZip==0)
  {
    return false;
  }
  
  if (LIBZ_UnzGoToFirstFile(hZip)==false)
  {
    return false;
  }
  
 while(1)
  {
    Char filename[MAX_FILE_PATH+1];
    S32 file_size;
    
    if (LIBZ_UnzGetCurrentFileInfo(hZip, filename, MAX_FILE_PATH, &file_size)==false)
    {
      goto __ERROR;
    }
    
    if (LIBZ_UnzLocateFile(hZip, filename, 0)==false)
    {
      goto __ERROR;
    }
    
    if (LIBZ_UnzOpenCurrentFile(hZip)==false)
    {
      goto __ERROR;
    }
   
    /** .DS_Storeが入って来る場合の対応に関しては考えるべき */
    if (strstr(filename, "MACOSX")!=0 ||
        strstr(filename, "Thumbs.db")!=0 ||
          strstr(filename, ".DS_Store")!=0)
    {
      if (LIBZ_UnzGoToNextFile(hZip)==false)
      {
        // EOF?
        break;
      }
      continue;
    }
    
    /** it is a directory */
    if (file_size==0)
    {
      if (file_manager->MakeDirectory(category_id, filename)==false)
      {
        /** TODO. 既にある場合もあるので何もしない。。。
        * MakeDictionaryの中で既にディレクトリがあるかどうかを見るのもいいかも。
        */
        // goto __ERROR;
      }
      
      if (LIBZ_UnzGoToNextFile(hZip)==false)
      {
        // EOF?
        break;
      }
      continue;
    }
    
    if (before_size<file_size)
    {
      if (extract_buffer)
      {
        free(extract_buffer);
      }
      extract_buffer = (Char*)malloc(file_size+1);
    }
    extract_buffer[file_size]=0;
    
    S32 n = LIBZ_UnzReadCurrentFile(hZip, extract_buffer, file_size);
    //ADK_ASSERT(n==size);
   
    if (SaveFile(category_id, filename, extract_buffer, file_size)==false)
    {
      // とりあえず、今のところは。。。。
      // goto __ERROR;
    }
    
    LIBZ_UnzCloseCurrentFile(hZip);
    
    if (LIBZ_UnzGoToNextFile(hZip)==false)
    {
      break;
    }
  }
  
  if (extract_buffer)
  {
    free(extract_buffer);
  }
  
  if (hZip)
  {
    LIBZ_UnzClose(hZip);
  }
  
  return true;
  
__ERROR:
  if (extract_buffer)
  {
    free(extract_buffer);
  }
  
  if (hZip)
  {
    LIBZ_UnzCloseCurrentFile(hZip);
    LIBZ_UnzClose(hZip);
  }
  return false;
}

/**\brief ダウンロードを一時中止させます。
 */
void Downloader::PauseDownload()
{
  is_pause_=true;
  adk::LIB_ThreadJoin(thread_id_);
  thread_id_=0;
}

/**\brief ダウンロードを再開させます。
 */
void Downloader::ResumeDownload()
{
  is_pause_=false;
  DownloadResourceList(0, 0, 0, true);
}

Bool Downloader::SaveFile(S32 category_id, const Char* org_file_name, const void* data, U64 data_size)
{
  //TODO FileManagerインスタンスの取得
  FileManager *file_manager = ApplicationManager::GetInstance()->GetFileManager();
  return file_manager->SaveFile(category_id, org_file_name, data, data_size);
}


CHAOS_END_NAMESPACE
