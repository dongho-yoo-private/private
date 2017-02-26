/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_member.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツの持つデータメンバを管理します
 */

#ifndef PARTS_OBJECT_ON_PARTS_MANAGER_H_
#define PARTS_OBJECT_ON_PARTS_MANAGER_H_

#include <cocos2d.h>
#include <extensions/CCBReader/CCBMemberVariableAssigner.h>

#include <core/types.h>

ADK_BEGIN_NAMESPACE

class PartsMember : cocos2d::extension::CCBMemberVariableAssigner {

 public:
  PartsMember();

  virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget,
                                         const char* pMemberVariableName,
                                         cocos2d::CCNode* pNode);

  cocos2d::CCNode* GetNode(const char* name);
  
  /** パーツの名前の重複を防ぐための対策、親子関係で有効にする。*/
  cocos2d::CCNode* GetNode(cocos2d::CCNode* parent, const char* name);
  ADK_INLINE int GetPartsCount() const { return count_; }

 private:
  static const int kMaxClassName = 31;
  typedef struct {
    char name[kMaxClassName + 1];
    size_t name_len;
    cocos2d::CCNode* node;
  } TNode;

  static const int kMaxNode = 64;

  TNode nodes_[kMaxNode];
  int count_;
};

ADK_END_NAMESPACE

#endif /* defined(PARTS_OBJECT_ON_PARTS_MANAGER_H_) */
