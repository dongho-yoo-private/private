/**
 * \file lib_path_util_osx.mm
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief パスユーティリティ for OSX
 */

#include <core/library/lib_path_util.h>

#include <debug_assert.h>

#include <core/library/lib_string.h>


ADK_BEGIN_NAMESPACE


/* extern function implementation */
/* device path information management functionally */
Bool LIB_GetSystemDir( Char *dst, U32 dstlen) {
  dst[0] = '\0';
  return true;
}
Bool LIB_GetUserDir( Char *dst, U32 dstlen) {
  dst[0] = '\0';
  return true;
}
Bool LIB_GetSaveDataDir( Char *dst, U32 dstlen) {
  dst[0] = '\0';
  return true;
}
Bool LIB_GetResourceDir( Char *dst, U32 dstlen) {
  dst[0] = '\0';
  return true;
}
Bool LIB_GetTemporaryDir( Char *dst, U32 dstlen) {
  dst[0] = '\0';
  return true;
}
Bool LIB_GetDesktopDir( Char *dst, U32 dstlen) {
  dst[0] = '\0';
  return true;
}


ADK_END_NAMESPACE


