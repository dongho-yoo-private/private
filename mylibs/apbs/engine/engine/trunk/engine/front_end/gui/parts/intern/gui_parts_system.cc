/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file parts_system.cc
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief パーツ全体を管理するシステムクラス実装部
 */

#include <front_end/gui/parts/intern/gui_parts_system.h>

#include <debug_profiling/error/result_types.h>
#if defined(WITH_COCOS)
#include <front_end/gui/parts/intern/cocos/gui_parts_system_cocos.h>
#else
#error not graphics type;
#endif
#include <front_end/gui/parts/i_gui_parts.h>

ADK_BEGIN_NAMESPACE

GuiPartsSystem *GuiPartsSystem::system_ = NULL;
Result GuiPartsSystem::Initialize() {
  Result ret = ResultSuccess;
  if (system_ != NULL) {
    ret = ResultDoAlready;
  } else {
#if defined(WITH_COCOS)
    system_ = new GuiPartsSystemCocos;
#else
#error not graphics type;
#endif
    if (system_ == NULL) {
      ret = ResultOutOfResource;
    } else {
      ret = system_->Init();
      if (ret.IsSuccess() == false) {
        delete system_, system_ = 0;
      }
    }
  }
  return ret;
}
Result GuiPartsSystem::Terminate() {
  Result ret = ResultSuccess;
  if (system_ == NULL) {
    ret = ResultNotInitialized;
  } else {
    ret = system_->Term();
    delete system_, system_ = NULL;
  }
  return ret;
}
GuiPartsSystem *GuiPartsSystem::GetInstance() { return system_; }

GuiPartsSystem::GuiPartsSystem() {}
GuiPartsSystem::~GuiPartsSystem() {}
IGuiParts *GuiPartsSystem::CreateGuiPartsWithFile(const char *filename,
                                                  const TGuiPartsInfo *parts,
                                                  S32 parts_count) {
  IGuiParts *ret = CreateGuiParts();

  if (ret != NULL) {
    Bool result = ret->CreateWithFile(filename, parts, parts_count);
    if (result == false) {
      delete ret, ret = NULL;
    }
  }

  return ret;
}
Result GuiPartsSystem::DisposeParts(IGuiParts *parts) {
  Result ret = ResultSuccess;
  if (parts == NULL) {
    ret = ResultInvalidVariable;
  } else {
    parts->Dispose();
    delete parts, parts = NULL;
  }
  return ret;
}

ADK_END_NAMESPACE

