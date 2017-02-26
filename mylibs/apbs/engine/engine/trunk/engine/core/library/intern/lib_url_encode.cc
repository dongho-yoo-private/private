/**
 * \file lib_url_encode.cc
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief URLエンコードライブラリ
 */

#include <core/library/lib_url_encode.h>

#include <core/debug/logging.h>
#include <debug_profiling/error/err_library_result_types.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/library/lib_string.h>

ADK_BEGIN_NAMESPACE

/* extern function implementation */
Result LIB_URLEncode(Char **dst, const Char *str) {
  Result ret = ResultSuccess;
  const Char *p = str;
  Char *enc, c;
  U32 len;

  if (str == 0) {
    ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kLibraryComponent,
                             kLibUrlEncode,
                             ERR_URL_ENCODE_ENCODE_INVALID_VARIABLE_STR);
    goto errend;
  }
  len = LIB_strlen(str);

  *dst = (Char *)ADK_Malloc(3 * len + 3, "URLEncode");
  if (*dst == 0) {
    ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kLibraryComponent,
                             kLibUrlEncode,
                             ERR_URL_ENCODE_ENCODE_OUT_OF_RESOURCE);
    goto errend;
  }

  enc = *dst;
  for (; *p != '\0'; ++p) {
    c = *p;

    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') || (c == '\'') || (c == '*') || (c == ')') ||
        (c == '(') || (c == '-') || (c == '.') || (c == '_')) {
      *enc = c;
      ++enc;
    } else if (c == ' ') {
#if 1
      *enc = '+';
      ++enc;
#else
      *enc = '%';
      ++enc;
      *enc = '2';
      ++enc;
      *enc = '0';
      ++enc;
#endif
    } else {
      *enc = '%';
      ++enc;

      sprintf(enc, "%02x", c);
      enc = enc + 2;
    }
  }

  *enc = '\0';

  return ret;

errend:

  return ret;
}
Result LIB_URLDispose(Char *str) {
  Result ret = ResultSuccess;

  if (str == 0) {
    ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kLibraryComponent,
                             kLibUrlEncode,
                             ERR_URL_ENCODE_DISPOSE_INVALID_VARIABLE);
  } else {
    ADK_Free(str), str = 0;
  }

  return ret;
}

ADK_END_NAMESPACE

