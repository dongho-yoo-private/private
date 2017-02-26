/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_call_func.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツファイルから指定される関数呼び出しのコールバックの実装部
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_call_func.h>

#include <core/debug/debug_assert.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_common_node_loader.h>

ADK_BEGIN_NAMESPACE

PartsCallFunc::PartsCallFunc()
: count_(0)
{
}

PartsCallFunc::~PartsCallFunc(){
  
}
cocos2d::SEL_CallFuncN PartsCallFunc::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName, cocos2d::CCNode* parent){
  
  ADK_ASSERT(pSelectorName != NULL);
  ADK_ASSERT(count_ < kMaxCallFunc);
  
  CallFunc* callFunc = &call_funcs_[count_];
  ++count_;
  
  callFunc->name_len = strlen(pSelectorName);
  ADK_ASSERT(callFunc->name_len < kMaxSelectorName);
  
  memcpy(callFunc->name, pSelectorName, callFunc->name_len);
  callFunc->parent = parent;
  return (cocos2d::SEL_CallFuncN)&PartsCommonNodeLoader::CallFuncCallback;
}

void PartsCallFunc::SetHandler(const char* pSelectorName, CallFuncHandler* handler){
  CallFunc* call_func = FindCallFuncName(pSelectorName);
  call_func->handler = *handler;
}

PartsCallFunc::CallFunc* PartsCallFunc::FindCallFuncName(const char* name){
  
  CallFunc* call_func = call_funcs_;
  for (int i = 0; i < count_; ++i, ++call_func){
    size_t name_len = strlen(name);
    if (name_len != call_func->name_len){
      continue;
    }
    if (memcmp(call_func->name, name, name_len) == 0){
      return call_func;
    }
  }
  ADK_ASSERT(false);
  
  return NULL;
}

PartsCallFunc::CallFunc* PartsCallFunc::FindCallFuncParent(cocos2d::CCNode* node){
  
  CallFunc* call_func = call_funcs_;
  for (int i = 0; i < count_; ++i, ++call_func){
    if (node == call_func->parent){
      return call_func;
    }
  }
  ADK_ASSERT(false);
  
  return NULL;
}

void PartsCallFunc::CallFuncCallback(CCObject* pSender){
  printf("PartsControl::CallFuncCallback\n");
  CallFunc* call_func = FindCallFuncParent(((cocos2d::CCNode*)pSender)->getParent());
  ADK_ASSERT(call_func->handler != NULL);
  (call_func->handler)(pSender);
}

ADK_END_NAMESPACE
