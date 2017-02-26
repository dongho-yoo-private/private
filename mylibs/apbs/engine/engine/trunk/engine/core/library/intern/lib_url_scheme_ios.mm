/**
 * \file lib_url_scheme.mm
 * \ingroup adkgrp_library
 * \author ishii yasutaka
 * \~english
 * \~japanese
 *   \brief アプリケーション・ストアに関する定義部です
 */


#include <core/library/lib_url_scheme.h>

#include <sys/sysctl.h>
#import <UIKit/UIKit.h>

#include <core/debug/logging.h>
#include <core/debug/debug_assert.h>
#include <core/memory_allocation/guarded_alloc.h>


ADK_BEGIN_NAMESPACE

void LIB_OpenUrlScheme(const Char* url)
{
  NSURL *_url = [NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]];
  [[UIApplication sharedApplication] openURL:_url];
}


ADK_END_NAMESPACE


