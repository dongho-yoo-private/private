/**
 * Copyright 2014 Applibot Inc.
 * \author hosono hiroaki
 * \file parts_member.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツの持つデータメンバを管理します
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_member.h>

#include <core/debug/debug_assert.h>

ADK_BEGIN_NAMESPACE

PartsMember::PartsMember() : count_(0) {}

bool PartsMember::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget,
                                            const char* pMemberVariableName,
                                            cocos2d::CCNode* pNode) {

  ADK_ASSERT(pMemberVariableName != NULL);
  TNode* node = &nodes_[count_];
  strcpy(node->name, pMemberVariableName);
  node->node = pNode;
  node->name_len = strlen(node->name);

  ++count_;

  return true;
}

static Bool IsChild(cocos2d::CCNode* parent, cocos2d::CCNode* node)
{
  cocos2d::CCNode* _parent = node->getParent();
  while(_parent)
  {
    if (_parent==parent)
    {
      break;
    }
    _parent=_parent->getParent();
  }
  return (_parent!=0);
}

cocos2d::CCNode* PartsMember::GetNode(cocos2d::CCNode* parent, const char* name)
{
  TNode* node = nodes_;

  ADK_ASSERT(name != NULL);

  size_t name_len = strlen(name);

  for (int i = 0; i < count_; ++i, ++node) {
  
    if (IsChild(parent, node->node)==false)
    {
      continue;
    }
    if (name_len != node->name_len) {
      continue;
    }
    if (memcmp(name, node->name, node->name_len) == 0) {
      return node->node;
    }
  }
  return NULL;
}

cocos2d::CCNode* PartsMember::GetNode(const char* name) {
  TNode* node = nodes_;

  ADK_ASSERT(name != NULL);

  size_t name_len = strlen(name);

  printf("node name source %s\n", name);
  for (int i = 0; i < count_; ++i, ++node) {
    printf("node name %s\n", node->name);
    if (name_len != node->name_len) {
      continue;
    }
    if (memcmp(name, node->name, node->name_len) == 0) {
      return node->node;
    }
  }
  return NULL;
}

ADK_END_NAMESPACE

