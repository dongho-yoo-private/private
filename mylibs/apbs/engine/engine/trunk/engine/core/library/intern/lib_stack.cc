/**
 * \file lib_stack.cc
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief スタック実装部
 */

#include <core/library/lib_stack.h>

ADK_BEGIN_NAMESPACE

/* class implementation */
/* constructor & destructor */
LIB_Stack::LIB_Stack(U32 capacity) {
  ADK_ASSERT(capacity > 0);
  stack_.capacity = capacity;
  stack_.cur = (Char*)ADK_Malloc(capacity, "LIB_Stack");
  stack_.top = stack_.cur;
  stack_.end = stack_.cur + capacity;
}
LIB_Stack::~LIB_Stack() {}

/* operatoin management functionally */
void LIB_Stack::Release() {
  if (stack_.cur != 0) {
    ADK_Free(stack_.cur), stack_.cur = 0;
  }
}
void LIB_Stack::Clear() { stack_.top = stack_.cur; }

ADK_END_NAMESPACE

