/**
 * \file resource-api.cc
 * \ingroup adkgrp_api
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief リソース周りのアプリケーションインタフェース
 */

#include <resource/resource-api.h>

#include <cocos2d.h>

ADK_BEGIN_NAMESPACE

#define PREFIX_ROOT_PATH "Resource"
#define MAX_PATH_LEN (512)

static void CreateFullpath(char *fullpath,const char *path) {
  sprintf(fullpath,"%s/%s",PREFIX_ROOT_PATH,path);
}

Bool AddResourceSearchPath(const char *path) {
  Bool ret = true;
  cocos2d::CCFileUtils *fileutils = cocos2d::CCFileUtils::sharedFileUtils();
  if (fileutils==NULL) {
    ret = false;
  } else {
    char fullpath[MAX_PATH_LEN];
    CreateFullpath(fullpath,path);
    fileutils->addSearchPath(fullpath);
  }
  return ret;
}
Bool AddSearchResolutionsOrder(const char *path) {
  Bool ret = true;
  cocos2d::CCFileUtils *fileutils = cocos2d::CCFileUtils::sharedFileUtils();
  if (fileutils==NULL) {
    ret = false;
  } else {
    char fullpath[MAX_PATH_LEN];
    CreateFullpath(fullpath,path);
    fileutils->addSearchResolutionsOrder(path);
  }
  return ret;
}

ADK_END_NAMESPACE

