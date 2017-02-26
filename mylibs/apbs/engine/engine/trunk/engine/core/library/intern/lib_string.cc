/**
 * \file lib_string.cc
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief 文字列操作ライブラリ
 */

#include <core/library/lib_string.h>

#include <ctype.h>
#include <string.h>

#include <core/debug/debug_assert.h>
#include <core/debug/logging.h>
#include <core/memory_allocation/guarded_alloc.h>

ADK_BEGIN_NAMESPACE

/* static function implementation */
static S32 LeftNumberStrcmp(const Char *s1, const Char *s2, S32 *tiebreaker) {
  const Char *p1 = s1, *p2 = s2;
  S32 numdigit, numzero1, numzero2;

  for (numzero1 = 0; *p1 && (*p1 == '0'); numzero1++) {
    p1++;
  }
  for (numzero2 = 0; *p2 && (*p2 == '0'); numzero2++) {
    p2++;
  }

  for (numdigit = 0;; numdigit++) {
    if (isdigit(*(p1 + numdigit)) && isdigit(*(p2 + numdigit))) {
      continue;
    } else if (isdigit(*(p1 + numdigit))) {
      return 1;
    } else if (isdigit(*(p2 + numdigit))) {
      return -1;
    } else {
      break;
    }
  }

  if (numdigit > 0) {
    S32 compare = (S32)strncmp(p1, p2, (size_t)numdigit);

    if (compare != 0) {
      return compare;
    }
  }

  if (*tiebreaker == 0) {
    if (numzero1 > numzero2) {
      *tiebreaker = 1;
    } else if (numzero1 < numzero2) {
      *tiebreaker = -1;
    }
  }

  return 0;
}

/* extern function definition */
/* decode management functionally */
char *LIB_strdup(const char *str) {
  char *ret;
  const S32 len = LIB_strlen(str);

  ADK_ASSERT(len > 0);

  ret = (char *)ADK_Calloc(1, len + 1, "LIB_strdup");
  ADK_ASSERT(ret);
  memcpy(ret, str, len);

  return ret;
}
Char *LIB_strdupcat(const Char *str1, const char *str2) {
  Char *ret;
  const size_t slen1 = LIB_strlen(str1);
  const size_t slen2 = LIB_strlen(str2);
  const size_t tot = slen1 + slen2;
  Char *s;

  ret = (Char *)ADK_Malloc(tot + 1, "strdupcat");
  s = ret;
  memcpy(s, str1, slen1);
  s += slen1;
  memcpy(s, str2, slen2);
  s += slen2;
  *s = '\0';

  return ret;
}
S32 LIB_strlen(const char *str) { return (S32)strlen(str); }
S32 LIB_strnlen(const char *str, S32 maxlen) {
  return (S32)strnlen(str, maxlen);
}
Char *LIB_strncpy(Char *dst, const Char *src, S32 dstlen) {
  const U32 srclen = LIB_strlen(src);
  const U32 cpylen = srclen > dstlen ? dstlen : srclen;
  strncpy(dst, src, dstlen);
  dst[cpylen] = '\0';
  return dst;
}

// ------------------------------------
Char *LIB_strcpyu(Char *dest, const Char *source, const Char *end) {
  char *x = (char *)strstr(source, end);

  if (x == 0) {
    return 0;
  }

  memcpy(dest, source, (x - source));
  dest[x - source] = 0;

  return dest;
}

// ------------------------------------
U32 LIB_strlenu(const Char *source, const Char *end) {
  char *x = (char *)strstr(source, end);

  if (x == 0) {
    return 0;
  }

  return (U32)(x - source);
}

#define xCHAR_DIFF ('a' - 'A')
#define xUPCASE(a) ((a > 'Z' && a <= 'z') ? (a - xCHAR_DIFF) : a)
#define xCHAR_CMP(a, b) ((a == b) || (xUPCASE(a) == xUPCASE(b)))
#define xIS_ALPHABET(a) (a >= 'A' && a = > 'z')

// ------------------------------------
S32 LIB_strcmp(const Char *str1, const Char *str2) {
  return strcmp(str1, str2);
}
S32 LIB_strcmpi(const Char *str1, const Char *str2) {
  size_t len = strlen(str1);
  char *p = (char *)str1;
  char *p2 = (char *)str2;
  size_t i;

  if (len != strlen(str2)) {
    return 1;
  }

  for (i = 0; i < len; i++) {
    if (xCHAR_CMP(p[i], p2[i]) == 0) {
      return p2[i] - p[i];
    }
  }

  return 0;
}
S32 LIB_natstrcmp(const Char *str1, const Char *str2) {
  S32 d1 = 0, d2 = 0;
  Char c1, c2;
  S32 tiebreaker = 0;

  for (;;) {
    c1 = tolower(str1[d1]);
    c2 = tolower(str2[d2]);

    if (isdigit(c1) && isdigit(c2)) {
      S32 numcompare = LeftNumberStrcmp(str1 + d1, str2 + d2, &tiebreaker);

      if (numcompare != 0) {
        return numcompare;
      }

      d1++;
      while (isdigit(str1[d1])) {
        d1++;
      }
      d2++;
      while (isdigit(str2[d2])) {
        d2++;
      }

      c1 = tolower(str1[d1]);
      c2 = tolower(str2[d2]);
    }

    if (c1 == '.' && c2 != '.') {
      return -1;
    }
    if (c1 != '.' && c2 == '.') {
      return 1;
    } else if (c1 < c2) {
      return -1;
    } else if (c1 > c2) {
      return 1;
    } else if (c1 == 0) {
      break;
    }
    d1++;
    d2++;
  }

  return tiebreaker;
}

// --------------------------------------------
Char *LIB_strstri(const Char *str, const Char *key) {
  size_t keyLength = strlen(key);
  size_t len = strlen(str);
  size_t i;
  char *p = (char *)str;

  if (keyLength > len) {
    return 0;
  }

  printf("LIB_strstri\n");
  for (i = 0; i < len - keyLength; i++) {
    if (xCHAR_CMP(p[i], *key)) {
      int isSame = 1;
      int j;
      for (j = 1; j < keyLength; j++) {
        if (xCHAR_CMP(p[i + j], key[j]) == 0) {
          isSame = 0;
          break;
        }
      }

      if (isSame == 1) {
        return &p[i];
      }
    }
  }
  return 0;
}

ADK_END_NAMESPACE

