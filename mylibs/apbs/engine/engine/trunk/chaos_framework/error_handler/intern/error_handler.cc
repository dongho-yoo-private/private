/** 
 * Copyright 2014 Applibot Inc.
 *
 * Author: Dongho.Yoo.
*/
#include "error_handler/error_handler.h"
#include "error_handler/error_handler_types.h"

#include <core/types.h>
#include <string.h>

CHAOS_BEGIN_NAMESPACE

/**\brief エラーメッセージとポップアップのサイズを取得します。*/
//static void DoCommonError(ErrorData* errData, const adk::LIB_JsonDocument* response);

ErrorHandler::ErrorHandler()
{
}

ErrorHandler::~ErrorHandler()
{
}

Bool ErrorHandler::Initialize()
{
  memset(&application_error_info_, 0, sizeof(application_error_info_));
  return true;
}

Bool ErrorHandler::Terminate()
{
  return true;
}

// --------------------------------
Bool SetError(const adk::LIB_TJsonMember& common)
{
  // TODO. 共通エラーに関してはサーバ側に形式を変更してもらう
  const adk::LIB_TJsonMember& status = common.data[KEY_SATUS];
  return false;
}

// --------------------------------
Bool ErrorHandler::SetError(const TErrorInfo* info)
{
  memset(&application_error_info_, 0, sizeof(application_error_info_));
  if (info->system_error_code==kSystemErrorCodeApplication)
  {
    /**TODO */
    application_error_info_.application_error_code = info->application_error_code;
    strncpy(application_error_info_.error_detail, info->error_detail, sizeof(application_error_info_.error_detail));

    /** ここはサーバ側に形式を変えてもらう */
    // DoCommonError(info);
    return true;
  }
  else
  {
    /**TODO */
    // Generate Event  
      
  }
  return true;
}

#if 0
static void DoCommonError(ErrorData* errData, const adk::LIB_JsonDocument* response)
{
  const S32 detailCode = errData->GetDetailCode();
  switch (detailCode)
  {
      /**\brief アカウント警告、アカウント一時停止、アカウント永久停止 */
      case kACCOUNT_STATE_WARNING_CODE:
      case kACCOUNT_STATE_FREEZE_LV1_CODE:
      case kACCOUNT_STATE_FREEZE_LV2_CODE:
      {
          errData->SetDetailCode(kAPP_UPDATE_FORCE_CODE);
          /**\brief TODO ここでGameManagerにシーンの切り替えをお願いする。*/
          // TODO 必要な情報を設定してから
          // GameManager::GetInstance()->ReplaceScene("CCBSceneAccountStateNotification", Scene::CCBSceneAccountStateNotificationBuilderLoader::loader());
          break;
      }
      /**\brief 任意のアップデート*/
      case kAPP_UPDATE_FORCE_CODE:
      case kAPP_UPDATE_OPTIONAL_CODE:
      case kUPDATE_CODE:
      {
          errData->SetDetailCode(detailCode);
          errData->SetDescription(GetStringFromResources(kRES_STR_NOTIFY_NEW_VERSION));
          break;
      }
      case kRESTRICTION_FUNCTION_CODE:
      {
          const adk::LIB_TJsonMember& mtMessage = (*response)[GetStringFromResources(kRES_STR_COMMENT_KEY)];
          const adk::LIB_TJsonMember& mtTitle = (*response)[GetStringFromResources(kRES_STR_TITLE_KEY)];
        
          errData->SetDescription(GetStringFromResources(kRES_NOW_RESTRICTION));
        
          if (mtMessage.data.IsString())
          {
            errData->SetMessage(mtMessage.data.GetString());
          }
          if (mtTitle.data.IsString())
          {
            errData->SetTitle(mtTitle.data.GetString());
          }
        
#if 0
                        POPUP_SCENE_ADDCHILD(Util::PopupUtil::maintenancePopup(messageText ? messageText->getCString() : "", titleText ? titleText->getCString() : ""));
#endif
          break;
      }
      case kALL_MAINTENANCE_CODE:
      {
        /* TODO: このコードは重複。*/
        const adk::LIB_TJsonMember& mtMessage = (*response)[GetStringFromResources(kRES_STR_COMMENT_KEY)];
        const adk::LIB_TJsonMember& mtTitle = (*response)[GetStringFromResources(kRES_STR_TITLE_KEY)];
        
        errData->SetMessage(GetStringFromResources(kRES_STR_NOW_ALL_MAINTENANCE));
        errData->SetDetailCode(kALL_MAINTENANCE_CODE);
        
        if (mtMessage.data.IsString())
        {
            errData->SetMessage(mtMessage.data.GetString());
        }
        if (mtTitle.data.IsString())
        {
            errData->SetTitle(mtTitle.data.GetString());
        }
        
       
        /* TODO: ここの処理はSceneの中で行う。*/
        #if 0
                        Scene::CCBSceneMaintenance* parts = (Scene::CCBSceneMaintenance*)Util::Util::loadCustomParts("CCBSceneMaintenance", Scene::CCBSceneMaintenanceBuilderLoader::loader());
                        
                        CCString *messageText = dynamic_cast<CCString*>(response->objectForKey(MAINTENANCE::MAINTENANCE_MESSAGE));
                        CCString *titleText = dynamic_cast<CCString*>(response->objectForKey(MAINTENANCE::MAINTENANCE_TITLE));
                        
                        parts->setTitle(titleText->getCString());
                        parts->setWebData(messageText->getCString());
                        
                        Util::Util::replaceSceneFadeOut(parts);
        #endif
          break;
      }

      default:
          break;
  }

    return ;
}

#endif

CHAOS_END_NAMESPACE
