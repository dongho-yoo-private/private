/**
 * \file lib_path_util_ios.mm
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief パスユーティリティ
 */


#include <core/library/lib_path_util.h>
#include <core/library/lib_string.h>


ADK_BEGIN_NAMESPACE


/* extern function implementation */
/* device path information management functionally */
Bool LIB_GetSystemDir(Char *dst,U32 dstlen) {
  Bool ret = true;
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSLocalDomainMask, YES);
  NSString *base;

  if ([paths count]<=0) {
    [pool drain];
    ret = false;
  } else {
    base = [paths objectAtIndex:0];
    LIB_strncpy( dst, [base cStringUsingEncoding:NSASCIIStringEncoding], dstlen);
    [pool drain];
  }

  return ret;
}
Bool LIB_GetUserDir(Char *dst,U32 dstlen) {
  Bool ret = true;
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
  NSString *base;
  if ([paths count]<=0) {
    [pool drain];
    ret = false;
  } else {
    base = [paths objectAtIndex:0];
    LIB_strncpy( dst, [base cStringUsingEncoding:NSASCIIStringEncoding], dstlen);
    [pool drain];
  }

  return ret;
}
Bool LIB_GetSaveDataDir( Char *dst, U32 dstlen)
{
  Bool ret = true;
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
  NSString *base;
  if ([paths count]<=0) {
    [pool drain];
    ret = false;
  } else {
    base = [paths objectAtIndex:0];
    LIB_strncpy( dst, [base cStringUsingEncoding:NSASCIIStringEncoding], dstlen);
    [pool drain];
  }

  return ret;
}
Bool LIB_GetResourceDir(Char *dst,U32 dstlen) {
  Bool ret = true;
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
//  NSArray *paths= NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
  NSString *base;
  base = [[NSBundle mainBundle] bundlePath];
  if (base==nil) {
    [pool drain];
    ret = false;
  } else {
    LIB_strncpy( dst, [base cStringUsingEncoding:NSASCIIStringEncoding], dstlen);
    [pool drain];
  }

  return ret;
}
Bool LIB_GetTemporaryDir(Char *dst,U32 dstlen) {
  Bool ret = true;
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
  NSString *base;
  if ([paths count]<=0) {
    [pool drain];
    ret = false;
  } else {
    base = [paths objectAtIndex:0];
    LIB_strncpy( dst, [base cStringUsingEncoding:NSASCIIStringEncoding], dstlen);
    [pool drain];
  }

  return ret;
}
Bool LIB_GetDesktopDir( Char *dst, U32 dstlen) {
  Bool ret = true;
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDesktopDirectory, NSUserDomainMask, NO);
  NSString *base;
  if ([paths count]<=0) {
    [pool drain];
    ret = false;
  } else {
    base = [paths objectAtIndex:0];
    LIB_strncpy( dst, [base cStringUsingEncoding:NSASCIIStringEncoding], dstlen);
    [pool drain];
  }

  return ret;
}


ADK_END_NAMESPACE


