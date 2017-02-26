/**
 * \file lib_path_util-c_api.cc
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief パスユーティリティ for CApi
 */

#include <core/library/lib_path_util-c_api.h>

#include <core/library/lib_path_util.h>

/* extern function implementation */
/* utility */
Bool ADKLIB_SplitPathDirFile(Char *dir, Char *file, U32 dirlen, U32 filelen,
                             const Char *str) {
  return adk::LIB_SplitPathDirFile(dir, file, dirlen, filelen, str);
}
Bool ADKLIB_SplitPathDirFileExt(Char *dir, Char *file, Char *ext, U32 dirlen,
                                U32 filelen, U32 extlen, const Char *str) {
  return adk::LIB_SplitPathDirFileExt(dir, file, ext, dirlen, filelen, extlen,
                                      str);
}

/* device path information management functionally */
Bool ADKLIB_GetSystemDir(Char *dst, U32 dstlen) {
  return adk::LIB_GetSystemDir(dst, dstlen);
}
Bool ADKLIB_GetUserDir(Char *dst, U32 dstlen) {
  return adk::LIB_GetUserDir(dst, dstlen);
}
Bool ADKLIB_GetSaveDataDir(Char *dst, U32 dstlen) {
  return adk::LIB_GetSaveDataDir(dst, dstlen);
}
Bool ADKLIB_GetResourceDir(Char *dst, U32 dstlen) {
  return adk::LIB_GetResourceDir(dst, dstlen);
}
Bool ADKLIB_GetTemporaryDir(Char *dst, U32 dstlen) {
  return adk::LIB_GetTemporaryDir(dst, dstlen);
}
Bool ADKLIB_GetDesktopDir(Char *dst, U32 dstlen) {
  return adk::LIB_GetDesktopDir(dst, dstlen);
}

