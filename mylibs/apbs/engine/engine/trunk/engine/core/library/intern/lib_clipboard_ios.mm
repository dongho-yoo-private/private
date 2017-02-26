/**
 * \file lib_clipboard.mm
 * \ingroup adkgrp_library
 * \author ishii yasutaka
 * \~english
 * \~japanese
 *   \brief クリップボード情報に関する定義部です
 */


#include <core/library/lib_clipboard.h>

#include <sys/sysctl.h>
#import <UIKit/UIKit.h>

#include <core/debug/logging.h>
#include <core/debug/debug_assert.h>
#include <core/memory_allocation/guarded_alloc.h>


ADK_BEGIN_NAMESPACE

void LIB_SetClipBoard(const Char* text)
{
  NSString *_text = [NSString stringWithCString:text encoding:NSUTF8StringEncoding];
  UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
  [pasteboard setValue:_text forPasteboardType:@"public.utf8-plain-text"];
}


ADK_END_NAMESPACE


