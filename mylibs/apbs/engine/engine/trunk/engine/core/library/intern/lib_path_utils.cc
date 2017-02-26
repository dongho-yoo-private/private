/**
 * \file lib_path_util.cc
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief パスユーティリティ
 */

#include <core/library/lib_path_util.h>

#include <core/debug/debug_assert.h>
#include <core/library/lib_string.h>

ADK_BEGIN_NAMESPACE

/* local macro definition */
/**
 * \~english
 * \~japanese
 *   \brief 小さい方の値
 */
#define LIB_MIN2(x, y) ((x) < (y) ? (x) : (y))
/**
 * \~english
 * \~japanese
 *   \brief セパレータ
 */
#define SEP '/'

/* local function definition */
/**
 * \~english
 * \~japanese
 *   \brief 文字列中から最後の '/'/'\\' のアドレスを返す
 *   \param str [in] 文字列
 */
static const Char *GetLastSlash(const Char *str) {
  const Char *ret;
  const Char *front, *back;

  front = strrchr(str, '/');
  back = strrchr(str, '\\');

  if (front == 0) {
    ret = back;
  } else if (back == 0) {
    ret = front;
  } else {
    if (front < back) {
      ret = back;
    } else {
      ret = front;
    }
  }

  return ret;
}
/**
 * \~english
 * \~japanese
 *   \brief 文字列中から最後の '.' のアドレスを返す
 *   \param str [in] 文字列
 */
static const Char *GetLastDot(const Char *str) {
  const Char *ret;

  ret = strrchr(str, '.');

  return ret;
}

/* extern function impelementation */
/* utility */
Bool LIB_SplitPathDirFile(Char *dir, Char *file, U32 dirlen, U32 filelen,
                          const Char *str) {
  Bool ret = true;

  ADK_ASSERT(dir);
  ADK_ASSERT(file);
  if (dir == 0 || file == 0) {
    ret = false;
  } else {
    const Char *slash = GetLastSlash(str);
    const U32 slashlen = slash == 0 ? 0 : (U32)(slash - str) + 1;

    if (slash == 0) {
      dir[0] = '\0';
    } else {
      LIB_strncpy(dir, str, LIB_MIN2(slashlen + 1, dirlen));
    }
    LIB_strncpy(file, str + slashlen, filelen);
  }

  return ret;
}
Bool LIB_SplitPathDirFileExt(Char *dir, Char *file, Char *ext, U32 dirlen,
                             U32 filelen, U32 extlen, const Char *str) {
  Bool ret = true;

  ADK_ASSERT(dir);
  ADK_ASSERT(file);
  ADK_ASSERT(ext);
  if (dir == 0 || file == 0 || ext == 0) {
    ret = false;
  } else {
    const Char *slash = GetLastSlash(str), *dot = GetLastDot(str);
    const U32 slashlen = slash == 0 ? 0 : (U32)(slash - str) + 1;

    if (slash == 0) {
      dir[0] = '\0';
    } else {
      LIB_strncpy(dir, str, LIB_MIN2(slashlen + 1, dirlen));
    }

    if (dot == 0) {
      LIB_strncpy(file, str + slashlen, filelen);
      ext[0] = '\0';
    } else {
      LIB_strncpy(file, str + slashlen, (S32)(dot - str + 1));
      file[dot - (str + slashlen)] = '\0';
      LIB_strncpy(ext, dot, extlen);
    }
  }

  return ret;
}
void LIB_JoinDirFile(Char *dst, const U32 maxlen, const Char *dir,
                     const Char *file) {
  U32 dirlen = (U32)LIB_strnlen(dir, maxlen);

  if (dirlen == maxlen) {
    memcpy(dst, dir, dirlen);
    dst[dirlen - 1] = '\0';
    return;
  } else {
    memcpy(dst, dir, dirlen + 1);
  }

  if (dirlen + 1 >= maxlen) {
    return;
  }

  if ((dirlen > 0) && (dst[dirlen - 1] != SEP)) {
    dst[dirlen++] = SEP;
    dst[dirlen] = '\0';
  }

  if (dirlen >= maxlen) {
    return;
  }

  LIB_strncpy(dst + dirlen, file, maxlen - dirlen);
}

ADK_END_NAMESPACE

