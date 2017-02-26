/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_call_func.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツファイルから指定される関数呼び出しのコールバッククラスの定義部
 */

#ifndef SCENE_SYSTEM_PARTS_CALL_FUNC_H_
#define SCENE_SYSTEM_PARTS_CALL_FUNC_H_

#include <cocos2d.h>
#include <extensions/CCBReader/CCBSelectorResolver.h>

#include <core/types.h>

ADK_BEGIN_NAMESPACE

typedef void (*CallFuncHandler)(cocos2d::CCObject*);

/**
 * \~english
 * \~japanese
 *  \brief パーツファイルから指定される関数呼び出しのコールバックを設定するクラス\n
 *         \ref Parts から関連づけられている
 */
class PartsCallFunc
: public cocos2d::CCObject
{
  
public:
  /**
   * \~english
   * \~japanese
   *  \brief コンストラクタ
   */
  PartsCallFunc();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~PartsCallFunc();
  /**
   * \~english
   * \~japanese
   *  \brief ファイルを読み込み、セレクタ名と親ノードのポインタを保持する
   *         このメソッドはPartsクラスから呼ばれる
   */
  virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::CCObject* pTarget,
                                                                const char* pSelectorName,
                                                                cocos2d::CCNode* parent
                                                                );
  /**
   * \~english
   * \~japanese
   *  \brief 呼び出す関数を設定する
   *  \param pSelectorName[in] 設定されるセレクタ名
   *  \param handler[in] 設定する関数のポインタ
   */
  void SetHandler(const char* pSelectorName, CallFuncHandler* handler);
  
  void CallFuncCallback(cocos2d::CCObject* pSender);

private:
  
  static const int kMaxSelectorName = 31;
  
  typedef struct {
    cocos2d::CCNode* parent;
    char name[kMaxSelectorName + 1];
    size_t name_len;
    CallFuncHandler handler;
  }CallFunc;
  
  CallFunc* FindCallFuncName(const char* name);
  CallFunc* FindCallFuncParent(cocos2d::CCNode* parent);
  
  static const int kMaxCallFunc = 16;
  
  CallFunc call_funcs_[kMaxCallFunc];
  int count_;

};

ADK_END_NAMESPACE

#endif /* defined(SCENE_SYSTEM_PARTS_CALL_FUNC_H_) */
