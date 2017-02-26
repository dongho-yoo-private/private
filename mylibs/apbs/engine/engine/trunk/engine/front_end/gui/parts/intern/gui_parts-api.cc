/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file parts_api.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツに関するインタフェース定義部
 */

#include <front_end/gui/parts/gui_parts-api.h>

#include <core/debug/debug_assert.h>
#include <debug_profiling/error/result_types.h>
#include <front_end/gui/parts/intern/gui_parts_system.h>

ADK_BEGIN_NAMESPACE

Result InitializeGuiPartsSystem() {
  return GuiPartsSystem::Initialize();
}
Result TerminateGuiPartsSystem() {
  return GuiPartsSystem::Terminate();
}
IGuiParts *CreateGuiPartsWithFile(const char *filename, const TGuiPartsInfo *parts,
                            S32 parts_count) {
  IGuiParts *ret = NULL;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system!=NULL) {
    ret = system->CreateGuiPartsWithFile(filename, parts, parts_count);
  }
  return ret;
}
IGuiParts *CreateGuiPartsWithType(TGuiPartsType type){
  IGuiParts *ret = NULL;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system!=NULL) {
    ret = system->CreateGuiPartsWithType(type);
  }
  return ret;
}
IGuiPartsSprite* CreateGuiPartsSprite() {
  IGuiPartsSprite *ret = NULL;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system!=NULL) {
    ret = system->CreateGuiPartsSprite();
  }
  return ret;
}
IGuiPartsSprite* CreateGuiPartsSpriteWithFile(const char* filename) {
  IGuiPartsSprite *ret = NULL;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system!=NULL) {
    ret = system->CreateGuiPartsSpriteWithFile(filename);
  }
  return ret;
}
IGuiPartsSprite* CreateGuiPartsSpriteWithFile(const char* filename,
                                              const TRect& rect) {
  IGuiPartsSprite *ret = NULL;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system!=NULL) {
    ret = system->CreateGuiPartsSpriteWithFile(filename,rect);
  }
  return ret;
}
IGuiPartsSprite* CreateGuiPartsSpriteWithTexture(IGuiPartsTexture* pTexture) {
  IGuiPartsSprite *ret = NULL;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system!=NULL) {
    ret = system->CreateGuiPartsSpriteWithTexture(pTexture);
  }
  return ret;
}
IGuiPartsSprite* CreateGuiPartsSpriteWithTexture(IGuiPartsTexture* pTexture,
                                                 const TRect& rect) {
  IGuiPartsSprite *ret = NULL;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system!=NULL) {
    ret = system->CreateGuiPartsSpriteWithTexture(pTexture,rect);
  }
  return ret;
}
IGuiPartsSprite* CreateGuiPartsSpriteWithSpriteFrame(
    IGuiPartsSpriteFrame* pSpriteFrame) {
  IGuiPartsSprite *ret = NULL;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system!=NULL) {
    ret = system->CreateGuiPartsSpriteWithSpriteFrame(pSpriteFrame);
  }
  return ret;
}
IGuiPartsSprite* CreateGuiPartsSpriteWithSpriteFrameName(
    const char* pszSpriteFrameName) {
  IGuiPartsSprite *ret = NULL;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system!=NULL) {
    ret = system->CreateGuiPartsSpriteWithSpriteFrameName(pszSpriteFrameName);
  }
  return ret;
}
Result DisposeGuiParts(IGuiParts *parts) {
  Result ret = ResultSuccess;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system == NULL) {
    ret = ResultNotInitialized;
  } else {
    ret = system->DisposeParts(parts);
  }
  return ret;
}
Result AddGuiParts(IGuiParts *parts) {
  Result ret = ResultSuccess;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system == NULL) {
    ret = ResultNotInitialized;
  } else {
    ret = system->AddGuiParts(parts);
  }
  return ret;
}
Result RemoveGuiParts(IGuiParts *parts) {
  Result ret = ResultSuccess;
  GuiPartsSystem *system = GuiPartsSystem::GetInstance();
  ADK_ASSERT(system);
  if (system == NULL) {
    ret = ResultNotInitialized;
  } else {
    ret = system->RemoveGuiParts(parts);
  }
  return ret;
}

ADK_END_NAMESPACE

