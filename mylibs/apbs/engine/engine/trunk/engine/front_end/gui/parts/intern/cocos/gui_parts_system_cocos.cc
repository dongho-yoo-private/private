/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file gui_parts_system_cocos.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツ全体を管理するシステムクラス実装部 for Cocos2d-x
 */

#include <front_end/gui/parts/intern/cocos/gui_parts_system_cocos.h>

#include <front_end/gui/parts/intern/cocos/gui_parts_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_label_ttf_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_node_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_scroll_view_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_table_view_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_table_view_cell_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_sprite_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_sprite_frame_cocos.h>
#include <front_end/gui/parts/intern/cocos/gui_parts_texture_cocos.h>

ADK_BEGIN_NAMESPACE

GuiPartsSystemCocos::GuiPartsSystemCocos()
    : context_scene_(NULL),
      parts_control_(),
      parts_menu_(),
      active_parts_(NULL) {}
GuiPartsSystemCocos::~GuiPartsSystemCocos() {}


IGuiParts *GuiPartsSystemCocos::CreateGuiPartsWithType(TGuiPartsType type, void* node)
{
  IGuiParts* ret = NULL;
  switch (type) {
    case kGuiPartsTypeLabelTTF: {
      ret = new GuiPartsLabelTTFCocos;
      break;
    }
    case kGuiPartsTypeNode: {
      ret = new GuiPartsNodeCocos;
      break;
    }
    case kGuiPartsTypeScrollView: {
      ret = new GuiPartsScrollViewCocos;
      break;
    }
    case kGuiPartsTypeTableView: {
      ret = new GuiPartsTableViewCocos;
      break;
    }
    case kGuiPartsTypeTableViewCell: {
      ret = new GuiPartsTableViewCellCocos;
      break;
    }
    case kGuiPartsTypeSprite: {
      ret = new GuiPartsSpriteCocos;
      break;
    }
    case kGuiPartsTypeSpriteFrame: {
      ret = new GuiPartsSpriteFrameCocos;
      break;
    }
    case kGuiPartsTypeTexture: {
      ret = new GuiPartsTextureCocos;
      break;
    }
    default: { ADK_ASSERT_MSG(false, "Not supported parts type.\n"); }
  }
  
  ret->SetNode(node);
  
  //if (parts_member)
  //{
  //  memcpy(&((GuiPartsCocos*)ret)->loader_, parts_member, sizeof(((GuiPartsCocos*)ret)->loader_));
  //}
  
  ADK_ASSERT(ret);
  return ret;
}

IGuiParts* GuiPartsSystemCocos::CreateGuiPartsWithType(TGuiPartsType type) {

  IGuiParts* ret = CreateGuiPartsWithType(type, 0);
  
  if (ret != NULL) {
    if (ret->Init() == false) {
      delete ret, ret = 0;
    }
  }
  return ret;
}
IGuiPartsSprite* GuiPartsSystemCocos::CreateGuiPartsSprite() {
  return (IGuiPartsSprite*)CreateGuiPartsWithType(kGuiPartsTypeSprite);
}
IGuiPartsSprite* GuiPartsSystemCocos::CreateGuiPartsSpriteWithFile(
    const char* filename) {
  IGuiPartsSprite* ret =
      (IGuiPartsSprite*)CreateGuiPartsWithType(kGuiPartsTypeSprite);
  ADK_ASSERT(ret);
  if (ret == NULL) {
  } else {
    if (ret->InitWithFile(filename) == false) {
      delete ret, ret = 0;
    }
  }
  return ret;
}
IGuiPartsSprite* GuiPartsSystemCocos::CreateGuiPartsSpriteWithFile(
    const char* filename, const TRect& rect) {
  IGuiPartsSprite* ret =
      (IGuiPartsSprite*)CreateGuiPartsWithType(kGuiPartsTypeSprite);
  ADK_ASSERT(ret);
  if (ret == NULL) {
  } else {
    if (ret->InitWithFile(filename, rect) == false) {
      delete ret, ret = 0;
    }
  }
  return ret;
}
IGuiPartsSprite* GuiPartsSystemCocos::CreateGuiPartsSpriteWithTexture(
    IGuiPartsTexture* pTexture) {
  IGuiPartsSprite* ret =
      (IGuiPartsSprite*)CreateGuiPartsWithType(kGuiPartsTypeSprite);
  ADK_ASSERT(ret);
  if (ret == NULL) {
  } else {
    if (ret->InitWithTexture(pTexture) == false) {
      delete ret, ret = 0;
    }
  }
  return ret;
}
IGuiPartsSprite* GuiPartsSystemCocos::CreateGuiPartsSpriteWithTexture(
    IGuiPartsTexture* pTexture, const TRect& rect) {
  IGuiPartsSprite* ret =
      (IGuiPartsSprite*)CreateGuiPartsWithType(kGuiPartsTypeSprite);
  ADK_ASSERT(ret);
  if (ret == NULL) {
  } else {
    if (ret->InitWithTexture(pTexture, rect) == false) {
      delete ret, ret = 0;
    }
  }
  return ret;
}
IGuiPartsSprite* GuiPartsSystemCocos::CreateGuiPartsSpriteWithSpriteFrame(
    IGuiPartsSpriteFrame* spriteframe) {
  ADK_ASSERT(false);
  return NULL;
}
IGuiPartsSprite* GuiPartsSystemCocos::CreateGuiPartsSpriteWithSpriteFrameName(
    const Char* framename) {
  ADK_ASSERT(false);
  return NULL;
}

Result GuiPartsSystemCocos::AddChild(cocos2d::CCNode* node) {
  Result ret = ResultSuccess;
  context_scene_->addChild(node);
  return ret;
}
Result GuiPartsSystemCocos::RemoveChild(cocos2d::CCNode* node) {
  Result ret = ResultSuccess;
  context_scene_->removeChild(node);
  return ret;
}

Result GuiPartsSystemCocos::AddGuiParts(IGuiParts* parts) {
  Result ret = ResultSuccess;
  if (parts == NULL) {
    ret = ResultInvalidVariable;
  } else {
    AddChild((cocos2d::CCNode*)parts->GetNode());
  }
  return ret;
}
Result GuiPartsSystemCocos::RemoveGuiParts(IGuiParts* parts) {
  Result ret = ResultSuccess;
  if (parts == NULL) {
    ret = ResultInvalidVariable;
  } else {
    RemoveChild((cocos2d::CCNode*)parts->GetNode());
  }
  return ret;
}

Result GuiPartsSystemCocos::Init() {
  Result ret = ResultSuccess;
  context_scene_ = cocos2d::CCScene::create();
  if (context_scene_ == 0) {
    ret = ResultOutOfResource;
  } else {
    cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
    ADK_ASSERT(director);
    director->runWithScene(context_scene_);
  }
  return ret;
}
Result GuiPartsSystemCocos::Term() {
  Result ret = ResultSuccess;
  if (context_scene_ != 0) {
    cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
    ADK_ASSERT(director);
    director->popScene();
    context_scene_ = 0;
  }
  return ret;
}
IGuiParts* GuiPartsSystemCocos::CreateGuiParts() {
  IGuiParts* ret = new GuiPartsCocos;
  return ret;
}

ADK_END_NAMESPACE

