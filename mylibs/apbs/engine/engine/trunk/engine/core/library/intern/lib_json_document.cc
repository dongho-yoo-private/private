/**
 * \file lib_json_document.h
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief JSONドキュメント定義部
 */

#include <core/library/lib_json_document.h>

#include <ctype.h> /* tolowe() */

#include <core/debug/logging.h>
#include <core/library/lib_string.h>
#include <debug_profiling/error/result.h>
#include <debug_profiling/error/err_library_result_types.h>

ADK_BEGIN_NAMESPACE

#define WITH_LITTLE_ENDIAN 0
#define WITH_BIG_ENDIAN 1

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define WITH_ENDIAN WITH_LITTLE_ENDIAN
#else
#define WITH_ENDIAN WITH_BIG_ENDIAN
#endif
#else
#define WITH_ENDIAN WITH_LITTLE_ENDIAN
#endif /* __BYTE_ORDER__ */

#define BUF_MAX (256)

/* local function implementation */
static Double Pow10(S32 n) {
  static const Double powtable[] = {
      1e-308, 1e-307, 1e-306, 1e-305, 1e-304, 1e-303, 1e-302, 1e-301, 1e-300,
      1e-299, 1e-298, 1e-297, 1e-296, 1e-295, 1e-294, 1e-293, 1e-292, 1e-291,
      1e-290, 1e-289, 1e-288, 1e-287, 1e-286, 1e-285, 1e-284, 1e-283, 1e-282,
      1e-281, 1e-280, 1e-279, 1e-278, 1e-277, 1e-276, 1e-275, 1e-274, 1e-273,
      1e-272, 1e-271, 1e-270, 1e-269, 1e-268, 1e-267, 1e-266, 1e-265, 1e-264,
      1e-263, 1e-262, 1e-261, 1e-260, 1e-259, 1e-258, 1e-257, 1e-256, 1e-255,
      1e-254, 1e-253, 1e-252, 1e-251, 1e-250, 1e-249, 1e-248, 1e-247, 1e-246,
      1e-245, 1e-244, 1e-243, 1e-242, 1e-241, 1e-240, 1e-239, 1e-238, 1e-237,
      1e-236, 1e-235, 1e-234, 1e-233, 1e-232, 1e-231, 1e-230, 1e-229, 1e-228,
      1e-227, 1e-226, 1e-225, 1e-224, 1e-223, 1e-222, 1e-221, 1e-220, 1e-219,
      1e-218, 1e-217, 1e-216, 1e-215, 1e-214, 1e-213, 1e-212, 1e-211, 1e-210,
      1e-209, 1e-208, 1e-207, 1e-206, 1e-205, 1e-204, 1e-203, 1e-202, 1e-201,
      1e-200, 1e-199, 1e-198, 1e-197, 1e-196, 1e-195, 1e-194, 1e-193, 1e-192,
      1e-191, 1e-190, 1e-189, 1e-188, 1e-187, 1e-186, 1e-185, 1e-184, 1e-183,
      1e-182, 1e-181, 1e-180, 1e-179, 1e-178, 1e-177, 1e-176, 1e-175, 1e-174,
      1e-173, 1e-172, 1e-171, 1e-170, 1e-169, 1e-168, 1e-167, 1e-166, 1e-165,
      1e-164, 1e-163, 1e-162, 1e-161, 1e-160, 1e-159, 1e-158, 1e-157, 1e-156,
      1e-155, 1e-154, 1e-153, 1e-152, 1e-151, 1e-150, 1e-149, 1e-148, 1e-147,
      1e-146, 1e-145, 1e-144, 1e-143, 1e-142, 1e-141, 1e-140, 1e-139, 1e-138,
      1e-137, 1e-136, 1e-135, 1e-134, 1e-133, 1e-132, 1e-131, 1e-130, 1e-129,
      1e-128, 1e-127, 1e-126, 1e-125, 1e-124, 1e-123, 1e-122, 1e-121, 1e-120,
      1e-119, 1e-118, 1e-117, 1e-116, 1e-115, 1e-114, 1e-113, 1e-112, 1e-111,
      1e-110, 1e-109, 1e-108, 1e-107, 1e-106, 1e-105, 1e-104, 1e-103, 1e-102,
      1e-101, 1e-100, 1e-99,  1e-98,  1e-97,  1e-96,  1e-95,  1e-94,  1e-93,
      1e-92,  1e-91,  1e-90,  1e-89,  1e-88,  1e-87,  1e-86,  1e-85,  1e-84,
      1e-83,  1e-82,  1e-81,  1e-80,  1e-79,  1e-78,  1e-77,  1e-76,  1e-75,
      1e-74,  1e-73,  1e-72,  1e-71,  1e-70,  1e-69,  1e-68,  1e-67,  1e-66,
      1e-65,  1e-64,  1e-63,  1e-62,  1e-61,  1e-60,  1e-59,  1e-58,  1e-57,
      1e-56,  1e-55,  1e-54,  1e-53,  1e-52,  1e-51,  1e-50,  1e-49,  1e-48,
      1e-47,  1e-46,  1e-45,  1e-44,  1e-43,  1e-42,  1e-41,  1e-40,  1e-39,
      1e-38,  1e-37,  1e-36,  1e-35,  1e-34,  1e-33,  1e-32,  1e-31,  1e-30,
      1e-29,  1e-28,  1e-27,  1e-26,  1e-25,  1e-24,  1e-23,  1e-22,  1e-21,
      1e-20,  1e-19,  1e-18,  1e-17,  1e-16,  1e-15,  1e-14,  1e-13,  1e-12,
      1e-11,  1e-10,  1e-9,   1e-8,   1e-7,   1e-6,   1e-5,   1e-4,   1e-3,
      1e-2,   1e-1,   1e+0,   1e+1,   1e+2,   1e+3,   1e+4,   1e+5,   1e+6,
      1e+7,   1e+8,   1e+9,   1e+10,  1e+11,  1e+12,  1e+13,  1e+14,  1e+15,
      1e+16,  1e+17,  1e+18,  1e+19,  1e+20,  1e+21,  1e+22,  1e+23,  1e+24,
      1e+25,  1e+26,  1e+27,  1e+28,  1e+29,  1e+30,  1e+31,  1e+32,  1e+33,
      1e+34,  1e+35,  1e+36,  1e+37,  1e+38,  1e+39,  1e+40,  1e+41,  1e+42,
      1e+43,  1e+44,  1e+45,  1e+46,  1e+47,  1e+48,  1e+49,  1e+50,  1e+51,
      1e+52,  1e+53,  1e+54,  1e+55,  1e+56,  1e+57,  1e+58,  1e+59,  1e+60,
      1e+61,  1e+62,  1e+63,  1e+64,  1e+65,  1e+66,  1e+67,  1e+68,  1e+69,
      1e+70,  1e+71,  1e+72,  1e+73,  1e+74,  1e+75,  1e+76,  1e+77,  1e+78,
      1e+79,  1e+80,  1e+81,  1e+82,  1e+83,  1e+84,  1e+85,  1e+86,  1e+87,
      1e+88,  1e+89,  1e+90,  1e+91,  1e+92,  1e+93,  1e+94,  1e+95,  1e+96,
      1e+97,  1e+98,  1e+99,  1e+100, 1e+101, 1e+102, 1e+103, 1e+104, 1e+105,
      1e+106, 1e+107, 1e+108, 1e+109, 1e+110, 1e+111, 1e+112, 1e+113, 1e+114,
      1e+115, 1e+116, 1e+117, 1e+118, 1e+119, 1e+120, 1e+121, 1e+122, 1e+123,
      1e+124, 1e+125, 1e+126, 1e+127, 1e+128, 1e+129, 1e+130, 1e+131, 1e+132,
      1e+133, 1e+134, 1e+135, 1e+136, 1e+137, 1e+138, 1e+139, 1e+140, 1e+141,
      1e+142, 1e+143, 1e+144, 1e+145, 1e+146, 1e+147, 1e+148, 1e+149, 1e+150,
      1e+151, 1e+152, 1e+153, 1e+154, 1e+155, 1e+156, 1e+157, 1e+158, 1e+159,
      1e+160, 1e+161, 1e+162, 1e+163, 1e+164, 1e+165, 1e+166, 1e+167, 1e+168,
      1e+169, 1e+170, 1e+171, 1e+172, 1e+173, 1e+174, 1e+175, 1e+176, 1e+177,
      1e+178, 1e+179, 1e+180, 1e+181, 1e+182, 1e+183, 1e+184, 1e+185, 1e+186,
      1e+187, 1e+188, 1e+189, 1e+190, 1e+191, 1e+192, 1e+193, 1e+194, 1e+195,
      1e+196, 1e+197, 1e+198, 1e+199, 1e+200, 1e+201, 1e+202, 1e+203, 1e+204,
      1e+205, 1e+206, 1e+207, 1e+208, 1e+209, 1e+210, 1e+211, 1e+212, 1e+213,
      1e+214, 1e+215, 1e+216, 1e+217, 1e+218, 1e+219, 1e+220, 1e+221, 1e+222,
      1e+223, 1e+224, 1e+225, 1e+226, 1e+227, 1e+228, 1e+229, 1e+230, 1e+231,
      1e+232, 1e+233, 1e+234, 1e+235, 1e+236, 1e+237, 1e+238, 1e+239, 1e+240,
      1e+241, 1e+242, 1e+243, 1e+244, 1e+245, 1e+246, 1e+247, 1e+248, 1e+249,
      1e+250, 1e+251, 1e+252, 1e+253, 1e+254, 1e+255, 1e+256, 1e+257, 1e+258,
      1e+259, 1e+260, 1e+261, 1e+262, 1e+263, 1e+264, 1e+265, 1e+266, 1e+267,
      1e+268, 1e+269, 1e+270, 1e+271, 1e+272, 1e+273, 1e+274, 1e+275, 1e+276,
      1e+277, 1e+278, 1e+279, 1e+280, 1e+281, 1e+282, 1e+283, 1e+284, 1e+285,
      1e+286, 1e+287, 1e+288, 1e+289, 1e+290, 1e+291, 1e+292, 1e+293, 1e+294,
      1e+295, 1e+296, 1e+297, 1e+298, 1e+299, 1e+300, 1e+301, 1e+302, 1e+303,
      1e+304, 1e+305, 1e+306, 1e+307, 1e+308};

  ADK_ASSERT(n <= 308);

  return n < -308 ? 0.0 : powtable[n + 308];
}

static Bool IsHex(const Char c) {
  Bool ret;

  if (c >= '0' && c <= '9') {
    ret = true;
  } else if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
    ret = true;
  } else {
    ret = false;
  }

  return ret;
}

/* local class definition */
/**
 * \~english
 * \~japanese
 *   \brief 文字列ストリーム
 */
class JsonStream {
 public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   */
  JsonStream(const Char *src) : cur_(src), src_(src) {}

  /* read management functionally */
  ADK_INLINE Char Peek() const { return *cur_; }
  ADK_INLINE Char Take() { return *cur_++; }
  ADK_INLINE size_t Tell() const { return cur_ - src_; }
  ADK_INLINE const Char *Ptr() const { return cur_; }

  /* operation management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief ホワイトスペースをスキップする
   */
  void SkipWhiteSpace() {
    JsonStream s = *this;
    while (s.Peek() == ' ' || s.Peek() == '\n' || s.Peek() == '\r' ||
           s.Peek() == '\t') {
      s.Take();
    }
    *this = s;
  }

 private:
  /* vars */
  /**
   * \~english
   * \~japanese
   *   \brief カレントの読み込み位置
   */
  const Char *cur_;
  /**
   * \~english
   * \~japanese
   *   \brief オリジナル
   */
  const Char *src_;

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("JsonStream");
};
/**
 * \~english
 * \~japanese
 *  \brief JSON読み取り時の特殊文字の文字列長
 */
#define SPECIAL_NUMBER_LEN (4)
/**
 * \~english
 * \~japanese
 *   \brief 文字列読み取り
 */
class LIB_JsonReader {
 public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   */
  LIB_JsonReader() { special_.is_segment = false; }

  /* parse management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief JSON解析
   *   \param[in/out] doc ドキュメント
   *   \param[in] stream ストリーム
   */
  Result Parse(LIB_JsonDocument &doc, JsonStream &stream) {
    Result ret = ResultSuccess;

    stream.SkipWhiteSpace();
    if (stream.Peek() == '\0') {
      doc.SetError("JSON is not present in the text.", stream.Tell());
      ret = ADK_RESULT_PROGRAM(
          ResultUnknown, kLibraryComponent, kLibJsonDocument,
          ERR_LIB_JDOC_READER_PARSE_JSON_IS_NOT_PRESENT_IN_THE_TEXT);
    } else {
      /**
       * まず始めに '{' or '[' が必ず存在するはずなので探しにいく
       * 見つからなかったらエラーとして処理する
       */
      switch (stream.Peek()) {
        case '{': {
          ParseObject(doc, stream);
          break;
        }
        case '[': {
          ParseArray(doc, stream);
          break;
        }
        default: {
          doc.SetError("characters that you expect to root does not exist.",
                       stream.Tell());
          ret = ADK_RESULT_PROGRAM(
              ResultInvalidVariable, kLibraryComponent, kLibJsonDocument,
              ERR_LIB_JDOC_READER_PARSE_CHARS_THAT_YOU_EXPECT_TO_ROOT_DOES_NOT_EXIST);
          break;
        }
      }
      stream.SkipWhiteSpace();
      if (stream.Peek() != '\0') {
        doc.SetError("Nothing should follow the root object or array.",
                     stream.Tell());
        ret = ADK_RESULT_PROGRAM(
            ResultUnknown, kLibraryComponent, kLibJsonDocument,
            ERR_LIB_JDOC_READER_PARSE_NOTHING_SHOULD_FOLLOW_THE_ROOT_OBJECT_OR_ARRAY);
      }
    }

    return ret;
  }

 private:
  /* parse management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief Null解析
   */
  void ParseNull(LIB_JsonDocument &doc, JsonStream &stream) {
    ADK_ASSERT(stream.Peek() == 'n');
    stream.Take(); /* skip 'n' */

    if (stream.Take() == 'u' && stream.Take() == 'l' && stream.Take() == 'l') {
      doc.AddNull();
    } else {
      doc.SetError("Invalid value", stream.Tell());
    }
  }
  /**
   * \~english
   * \~japanese
   *   \brief True解析
   */
  void ParseTrue(LIB_JsonDocument &doc, JsonStream &stream) {
    ADK_ASSERT(stream.Peek() == 't');
    stream.Take(); /* skip 't' */

    if (stream.Take() == 'r' && stream.Take() == 'u' && stream.Take() == 'e') {
      doc.AddBool(true);
    } else {
      doc.SetError("Invalid value", stream.Tell());
    }
  }
  /**
   * \~english
   * \~japanese
   *   \brief False解析
   */
  void ParseFalse(LIB_JsonDocument &doc, JsonStream &stream) {
    ADK_ASSERT(stream.Peek() == 'f');
    stream.Take(); /* skip 'f' */

    if (stream.Take() == 'a' && stream.Take() == 'l' && stream.Take() == 's' &&
        stream.Take() == 'e') {
      doc.AddBool(false);
    } else {
      doc.SetError("Invalid value", stream.Tell());
    }
  }
  /**
   * \~english
   * \~japanese
   *   \brief 文字列解析
   */
  void ParseString(LIB_JsonDocument &doc, JsonStream &stream) {
#if 0
    static const Char  escape[256] = {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0, 0,'\"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'/', 
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'\\', 0, 0, 0, 
      0, 0,'\b', 0, 0, 0,'\f', 0, 0, 0, 0, 0, 0, 0,'\n', 0, 
      0, 0,'\r', 0,'\t', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
#endif

    JsonStream s = stream;
    ADK_ASSERT(s.Peek() == '\"');
    s.Take(); /* skip '\"' */
    const Char *start = s.Ptr();
    U32 segment_len = 0;

    for (;;) {
      const Char c = s.Take();
      /*
       * 文字列パース時に'\n'等の文字コードに対応する為に'\\'チェックをしないようにする
       */
      if (c == 0x5c) { /* '\\' */
        segment_len = static_cast<U32>((s.Ptr() - start) - 1);
        const Char segment = s.Take();
        if (segment == 's' || segment == 'u') {
          special_.number[0] = segment;
          special_.number[1] = s.Take();
          special_.number[2] = s.Take();
          special_.number[3] = '\0';
          if (s.Peek() != '"') {
            continue;
          } else {
            if ((special_.number[1] == '3' && special_.number[2] == '2') ||
                (special_.number[1] == '6' && special_.number[2] == '4')) {
              special_.is_segment = true;
            } else {
              continue;
            }
          }
        }
      } else if (c == '"') {
        if (special_.is_segment != false) {
          doc.AddString(start, segment_len);
        } else {
          doc.AddString(start, static_cast<U32>((s.Ptr() - start) - 1));
        }
        stream = s;
        break;
      }
    }
  }
  /**
   * \~english
   * \~japanese
   *   \brief 数値解析
   */
  void ParseNumber(LIB_JsonDocument &doc, JsonStream &stream) {
    JsonStream s = stream; /* Copy the local for optimization */

    Bool minus = false;
    if (s.Peek() == '-') {
      minus = true;
      s.Take();
    }

    U32 i;
    Bool try64bit = false;
    if (s.Peek() == '0') {
      i = 0;
      s.Take();
    } else if (s.Peek() >= '1' && s.Peek() <= '9') {
      i = s.Take() - '0';

#if 1
      while (s.Peek() >= '0' && s.Peek() <= '9') {
        if (i >= 214748364) { /* 2^31 = 2147483648 */
          if (i != 214748364 || s.Peek() > '8') {
            try64bit = true;
            break;
          }
        } else if (i >= 429496729) { /* 2^32 - 1 = 4294967295 */
          if (i != 429496729 || s.Peek() > '5') {
            try64bit = true;
            break;
          }
        }
        i = i * 10 + (s.Take() - '0');
      }
#else
      if (minus) {
        while (s.Peek() >= '0' && s.Peek() <= '9') {
          if (i >= 214748364) { /* 2^31 = 2147483648 */
            if (i != 214748364 || s.Peek() > '8') {
              try64bit = true;
              break;
            }
          }
          i = i * 10 + (s.Take() - '0');
        }
      } else {
        while (s.Peek() >= '0' && s.Peek() <= '9') {
          if (i >= 429496729) { /* 2^32 - 1 = 4294967295 */
            if (i != 429496729 || s.Peek() > '5') {
              try64bit = true;
              break;
            }
          }
          i = i * 10 + (s.Take() - '0');
        }
      }
#endif
    } else {
      doc.SetError("Expect a value here.", stream.Tell());
      return;
    }

    U64 i64 = 0;
    Bool useDouble = false;
    if (try64bit) {
      i64 = i;
      if (minus) {
        while (s.Peek() >= '0' && s.Peek() <= '9') {
          if (i64 >= 922337203685477580uLL) { /* 2^63 = 9223372036854775808 */
            if (i64 != 922337203685477580uLL || s.Peek() > '8') {
              useDouble = true;
              break;
            }
          }
          i64 = i64 * 10 + (s.Take() - '0');
        }
      } else {
        while (s.Peek() >= '0' && s.Peek() <= '9') {
          if (i64 >=
              1844674407370955161uLL) { /* 2^64 - 1 = 18446744073709551615 */
            if (i64 != 1844674407370955161uLL || s.Peek() > '5') {
              useDouble = true;
              break;
            }
          }
          i64 = i64 * 10 + (s.Take() - '0');
        }
      }
    }

    double d = 0.0;
    if (useDouble) {
      d = static_cast<Double>(i64);
      while (s.Peek() >= '0' && s.Peek() <= '9') {
        if (d >= 1E307) {
          doc.SetError("Number too big to store in double", stream.Tell());
          return;
        }
        d = d * 10 + (s.Take() - '0');
      }
    }

    int expFrac = 0;
    if (s.Peek() == '.') {
      if (!useDouble) {
        d = try64bit ? static_cast<Double>(i64) : static_cast<Double>(i);
        useDouble = true;
      }
      s.Take();

      if (s.Peek() >= '0' && s.Peek() <= '9') {
        d = d * 10 + (s.Take() - '0');
        --expFrac;
      } else {
        doc.SetError("At least one digit in fraction part", stream.Tell());
        return;
      }

      while (s.Peek() >= '0' && s.Peek() <= '9') {
        if (expFrac > -16) {
          d = d * 10 + (s.Peek() - '0');
          --expFrac;
        }
        s.Take();
      }
    }

    int exp = 0;
    if (s.Peek() == 'e' || s.Peek() == 'E') {
      if (!useDouble) {
        d = try64bit ? static_cast<Double>(i64) : static_cast<Double>(i);
        useDouble = true;
      }
      s.Take();

      bool expMinus = false;
      if (s.Peek() == '+') {
        s.Take();
      } else if (s.Peek() == '-') {
        s.Take();
        expMinus = true;
      }

      if (s.Peek() >= '0' && s.Peek() <= '9') {
        exp = s.Take() - '0';
        while (s.Peek() >= '0' && s.Peek() <= '9') {
          exp = exp * 10 + (s.Take() - '0');
          if (exp > 308) {
            doc.SetError("Number too big to store in double", stream.Tell());
            return;
          }
        }
      } else {
        doc.SetError("At least one digit in exponent", s.Tell());
        return;
      }

      if (expMinus) {
        exp = -exp;
      }
    }

    if (s.Peek() == 'x') {
      s.Take();
      i = 0;
      while (IsHex(s.Peek())) {
        Char c = s.Take();
        S16 n;

        if (c >= '0' && c <= '9') {
          n = c - '0';
        } else {
          if ('a' <= (c = tolower(c)) && c <= 'f') {
            n = c - 'a' + 10;
          } else {
            ADK_ASSERT(0);
          }
        }
        i = i * 16 + n;
      }
    }

    if (useDouble) {
      d *= Pow10(exp + expFrac);
      doc.AddDouble(minus ? -d : d);
    } else {
      if (special_.is_segment != false) {
        if (strcmp(special_.number, "s32") == 0) {
          if (minus) {
            doc.AddS32(-static_cast<S32>(try64bit == 0 ? i : i64));
          } else {
            doc.AddS32(static_cast<S32>(try64bit == 0 ? i : i64));
          }
        } else if (strcmp(special_.number, "u32") == 0) {
          if (minus) {
            doc.AddU32(try64bit == 0 ? i : static_cast<U32>(i64));
          } else {
            doc.AddU32(try64bit == 0 ? i : static_cast<U32>(i64));
          }
        } else if (strcmp(special_.number, "s64") == 0) {
          if (minus) {
            doc.AddS64(-static_cast<S64>(try64bit == 0 ? i : i64));
          } else {
            doc.AddS64(static_cast<S64>(try64bit == 0 ? i : i64));
          }
        } else if (strcmp(special_.number, "u64") == 0) {
          if (minus) {
            doc.AddU64(try64bit == 0 ? static_cast<U64>(i) : i64);
          } else {
            doc.AddU64(try64bit == 0 ? static_cast<U64>(i) : i64);
          }
        }
        special_.is_segment = false;
      } else {
        if (try64bit) {
          if (minus)
            doc.AddS64(-static_cast<int64_t>(i64));
          else
            doc.AddU64(i64);
        } else {
          if (minus) {
            doc.AddS32(-static_cast<int>(i));
          } else {
            doc.AddS32(i);
          }
        }
      }
    }

    stream = s; /* restore */
  }
  /**
   * \~english
   * \~japanese
   *   \brief オブジェクト解析
   */
  void ParseObject(LIB_JsonDocument &doc, JsonStream &stream) {
    ADK_ASSERT(stream.Peek() == '{');

    stream.Take(); /* skip '{' character */
    doc.StartObject();

    stream.SkipWhiteSpace();

    if (stream.Peek() == '}') {
      /* 空のオブジェクトが生成された */
      stream.Take(); /* skip '}' character */
      doc.EndObject(0);
      return;
    }

    for (U32 membercount = 0;;) {
      /*
       * オブジェクト内で、まず始めに登場するであろう値の名前のパースを行う。
       * - 名前文字列のパースを行う
       * - 値のパースを行う
       */

      if (stream.Peek() != '"') {
        doc.SetError("The name of the member of the object is not a string.",
                     stream.Tell());
        break;
      }
      ParseString(doc, stream);
      stream.SkipWhiteSpace();

      if (stream.Take() != ':') {
        doc.SetError("Colon does not exist after the name.", stream.Tell());
        break;
      }
      stream.SkipWhiteSpace();
      ParseValue(doc, stream);
      stream.SkipWhiteSpace();

      membercount++;

      switch (stream.Take()) {
        case ',': {
          stream.SkipWhiteSpace();
          break;
        }
        case '}': {
          doc.EndObject(membercount);
          return;
        }
        default: {
          doc.SetError(
              "It is not a '}' or ',' is the end of the object members.",
              stream.Tell());
        }
      }
    }
  }
  /**
   * \~english
   * \~japanese
   *  \brief 配列解析
   */
  void ParseArray(LIB_JsonDocument &doc, JsonStream &stream) {
    ADK_ASSERT(stream.Peek() == '[');
    stream.Take(); /* skip '[' */
    doc.StartArray();
    stream.SkipWhiteSpace();

    if (stream.Peek() == ']') {
      /* 空のオブジェクトが生成された */
      stream.Take(); /* skip ']' character */
      doc.EndArray(0);
      return;
    }

    for (U32 elementcount = 0;;) {
      ParseValue(doc, stream);
      ++elementcount;
      stream.SkipWhiteSpace();

      switch (stream.Take()) {
        case ',': {
          stream.SkipWhiteSpace();
          break;
        }
        case ']': {
          doc.EndArray(elementcount);
          return;
        }
        default: {
          doc.SetError(
              "It is not a '}' or ',' is the end of the object members.",
              stream.Tell());
        }
      }
    }
  }
  /**
   * \~english
   * \~japanese
   *   \brief 文字列解析
   */
  void ParseValue(LIB_JsonDocument &doc, JsonStream &stream) {
    switch (stream.Peek()) {
      case 'n': {
        ParseNull(doc, stream);
        break;
      }
      case 't': {
        ParseTrue(doc, stream);
        break;
      }
      case 'f': {
        ParseFalse(doc, stream);
        break;
      }
      case '"': {
        ParseString(doc, stream);
        break;
      }
      case '{': {
        ParseObject(doc, stream);
        break;
      }
      case '[': {
        ParseArray(doc, stream);
        break;
      }
      default: { ParseNumber(doc, stream); }
    }
  }

/* vars */
#if 0
  /**
   * \~english
   * \~japanese
   *   \brief カレントの読み込み位置
   */
  const Char *cur_;
  /**
   * \~english
   * \~japanese
   *   \brief オリジナル
   */
  const Char *src_;
#endif
  /**
   * \~english
   * \~japanese
   *  \brief 特殊文字列
   */
  struct {
    /**
     * \~english
     * \~japanese
     *  \brief 特殊文字列が存在したかのフラグ
     */
    Bool is_segment;
    /**
     * \~english
     * \~japanese
     *  \brief 特殊文字列が存在したかのフラグ
     */
    Char number[SPECIAL_NUMBER_LEN];
  } special_;

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("JsonStream");
};

/* class implementation */
/* constructor & destructor */
LIB_JsonDocument::LIB_JsonDocument() {
  value_.cur = 0;
  value_.top = 0;
  value_.isStrDup = 0;
  memset(&binary_, 0, sizeof(binary_));
}
LIB_JsonDocument::~LIB_JsonDocument() { value_.stack.Release(); }

/* parse management functionally */
Result LIB_JsonDocument::Parse(Char *json, Bool isHoldBinary,
                               void (*disposeHoldBinary)(void *)) {
  Result ret;

  if (json == 0) {
    ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kLibraryComponent,
                             kLibJsonDocument,
                             ERR_LIB_JDOC_PARSE_INVALID_VARIABLE);
  } else {
    binary_.json = json;
    binary_.isHoldBinary = isHoldBinary;
    binary_.disposeHoldBinary = disposeHoldBinary;

    memset(&error_, 0, sizeof(error_));
    value_.stack.Clear();

    JsonStream stream(json);
    LIB_JsonReader reader;
    ret = reader.Parse(*this, stream);
    if (ret.IsSuccess() != false) {
      Assign();
    }
  }

  return ret;
}
Result LIB_JsonDocument::Release() {
  Result ret = ResultSuccess;

  value_.stack.Clear();
  this->Dispose(value_.isStrDup);

  if (binary_.isHoldBinary != false) {
    ADK_ASSERT(binary_.disposeHoldBinary);
    binary_.disposeHoldBinary((void *)binary_.json), binary_.json = 0;
    binary_.isHoldBinary = false;
    binary_.disposeHoldBinary = 0;
  }

  return ret;
}

void LIB_JsonDocument::BeginObject(const Char *name) {
  if (name != 0) {
    AddString(name, LIB_strlen(name), false);
  }
  StartObject();
}
void LIB_JsonDocument::EndObject(U32 membercount) {
  EndObject(static_cast<S32>(membercount));
}
void LIB_JsonDocument::BeginArray(const Char *name) {
  if (name != 0) {
    AddString(name, LIB_strlen(name), false);
  }
  StartArray();
}
void LIB_JsonDocument::EndArray(U32 elementcount) {
  EndArray(static_cast<S32>(elementcount));
}
void LIB_JsonDocument::AddNull(const Char *name) {
  AddString(name, LIB_strlen(name), false);
  AddNull();
}
void LIB_JsonDocument::AddBool(const Char *name, Bool b) {
  AddString(name, LIB_strlen(name), false);
  AddBool(b);
}
void LIB_JsonDocument::AddString(const Char *name, const Char *str) {
  AddString(name, LIB_strlen(name), false);
  AddString(str, LIB_strlen(str), false);
}
void LIB_JsonDocument::AddS32(const Char *name, S32 i) {
  AddString(name, LIB_strlen(name), false);
  AddS32(i);
}
void LIB_JsonDocument::AddU32(const Char *name, U32 u) {
  AddString(name, LIB_strlen(name), false);
  AddU32(u);
}
void LIB_JsonDocument::AddS64(const Char *name, S64 i) {
  AddString(name, LIB_strlen(name), false);
  AddS64(i);
}
void LIB_JsonDocument::AddU64(const Char *name, U64 u) {
  AddString(name, LIB_strlen(name), false);
  AddU64(u);
}
void LIB_JsonDocument::AddDouble(const Char *name, Double d) {
  AddString(name, LIB_strlen(name), false);
  AddDouble(d);
}
void LIB_JsonDocument::AddBinary(const Char *name, const void *b, U32 size) {
  AddString(name, LIB_strlen(name), false);
  AddBinary(b, size);
}
void LIB_JsonDocument::AddDocument(const Char *name,
                                   const LIB_JsonDocument *doc) {
  AddString(name, LIB_strlen(name), false);
  AddDocument(doc);
}
void LIB_JsonDocument::AddValue(const Char *name, const LIB_JsonValue *val) {
  AddString(name, LIB_strlen(name), false);
  AddValue(val);
}
void LIB_JsonDocument::Assign() {
  LIB_JsonValue *value = value_.stack.Pop<LIB_JsonValue>(1);
  this->Copy(value);
}
Result LIB_JsonDocument::CreateString(Char **dst, U32 *length) {
  Result ret = ResultSuccess;

  value_.stack.Clear();
  CreateString(*this);
  *((Char *)(value_.stack.Push<Byte>(1))) = '\0';

  *length = static_cast<U32>(value_.stack.GetSize());
  *dst = static_cast<Char *>(
      ADK_Calloc(1, *length, "LIB_JsonDocument::CreateString"));
  if (dst == 0) {
    ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kLibraryComponent,
                             kLibJsonDocument,
                             ERR_LIB_JDOC_CREATE_STRING_OUT_OF_RESOURCE);
  } else {
    memcpy(*dst, value_.stack.Bottom<Byte>(), *length);
  }

  return ret;
}
Result LIB_JsonDocument::CreateStringWithoutTopObject(Char **dst, U32 *length) {
  Result ret = ResultSuccess;

  value_.stack.Clear();
  CreateString(*this);
  *((Char *)(value_.stack.Push<Byte>(1))) = '\0';

  *length = static_cast<U32>(value_.stack.GetSize());
  *dst = static_cast<Char *>(
      ADK_Calloc(1, *length, "LIB_JsonDocument::CreateStringWithoutTopObject"));
  if (dst == 0) {
    ret = ADK_RESULT_PROGRAM(
        ResultOutOfResource, kLibraryComponent, kLibJsonDocument,
        ERR_LIB_JDOC_CREATE_STRING_INTERNAL_OUT_OF_RESOURCE);
  } else {
    const Byte *string = value_.stack.Bottom<Byte>();
    *length = *length - 2;
    memcpy(*dst, &string[1], *length);
    (*dst)[*length - 1] = '\0';
  }

  return ret;
}
void LIB_JsonDocument::CreateStringLength(U32 *len,
                                          const LIB_JsonValue &value) {
  U32 size;

  if (value.IsObject() != false || value.IsArray() != false) {
    *len += 1; /* add '{' or '[' */
    size = value.Size();
    if (size > 0) {
      const LIB_TJsonMember *member = &value.GetMember(0);
      const S32 last = size - 1;
      for (S32 index = 0;; ++index, ++member) {
        *len += 1 + member->name.GetStringLength() + 1 +
                1; /* '\"' + name + '\"' + ':' */
        if (member->data.IsNull() != false) {
          *len += 1 + 1 + 1 + 1; /* 'n' + 'u' + 'l' + 'l' */
        } else if (member->data.IsTrue() != false) {
          *len += 1 + 1 + 1 + 1; /* 't' + 'r' + 'u' + 'e' */
        } else if (member->data.IsFalse() != false) {
          *len += 1 + 1 + 1 + 1 + 1; /* 'f' + 'a' + 'l' + 's' + 'e' */
        } else if (member->data.IsString() != false) {
          *len +=
              1 + member->data.GetStringLength() + 1; /* '\"' + string + '\"' */
        } else if (member->data.IsObject() != false ||
                   member->data.IsArray() != false) {
          CreateStringLength(len, member->data);
        } else if (member->data.IsNumber() != false) {
          if (member->data.IsS32() != false) {
            Char str[16];
            sprintf(str, "%d", member->data.GetS32());
            *len += LIB_strlen(str);
          } else if (member->data.IsU32() != false) {
            Char str[16];
            sprintf(str, "%u", member->data.GetU32());
            *len += LIB_strlen(str);
          } else if (member->data.IsS64() != false) {
            Char str[32];
            sprintf(str, "%lld", member->data.GetS64());
            *len += LIB_strlen(str);
          } else if (member->data.IsU64() != false) {
            Char str[32];
            sprintf(str, "%llu", member->data.GetU64());
            *len += LIB_strlen(str);
          } else if (member->data.IsDouble() != false) {
            Char str[32];
            sprintf(str, "%lf", member->data.GetDouble());
            *len += LIB_strlen(str);
          }
        } else {
          ADK_ASSERT(0);
        }

        if (index == last) {
          break;
        } else {
          *len += 1; /* add ',' */
        }
      }
    }
    *len += 1; /* add '}' or ']' */
  } else {
    ADK_ASSERT(0);
  }
}
void LIB_JsonDocument::CreateString(const LIB_JsonValue &value) {
  U32 size;

  if (value.IsObject()) {
    StrCat("{");
    size = value.Size();
    if (size > 0) {
      const LIB_TJsonMember *member = &value.GetMember(0);
      const S32 last = size - 1;
      for (S32 index = 0;; ++index, ++member) {
        StrCat("\""); /* '\"' + name + '\"' + ':' */
        StringCat(member->name.GetStringLength(), member->name.GetString());
        StrCat("\"");
        StrCat(":");

        if (member->data.IsNull() != false) {
          StrCat("null"); /* 'n' + 'u' + 'l' + 'l' */
        } else if (member->data.IsTrue() != false) {
          StrCat("true"); /* 't' + 'r' + 'u' + 'e' */
        } else if (member->data.IsFalse() != false) {
          StrCat("false"); /* 'f' + 'a' + 'l' + 's' + 'e' */
        } else if (member->data.IsString() != false) {
          StrCat("\""); /* '\"' + string + '\"' */
          StringCat(member->data.GetStringLength(), member->data.GetString());
          StrCat("\"");
        } else if (member->data.IsObject() != false ||
                   member->data.IsArray() != false) {
          CreateString(member->data);
        } else if (member->data.IsNumber() != false) {
          if (member->data.IsS32() != false) {
            StrCat("%d", member->data.GetS32());
          } else if (member->data.IsU32() != false) {
            StrCat("%u", member->data.GetU32());
          } else if (member->data.IsS64() != false) {
            StrCat("%lld", member->data.GetS64());
          } else if (member->data.IsU64() != false) {
            StrCat("%llu", member->data.GetU64());
          } else if (member->data.IsDouble() != false) {
            StrCat("%lf", member->data.GetDouble());
          }
        } else {
          ADK_ASSERT(0);
        }

        if (index == last) {
          break;
        } else {
          StrCat(","); /* add ',' */
        }
      }
    }
    StrCat("}");
  } else if (value.IsArray()) {
    StrCat("[");
    size = value.Size();
    if (size > 0) {
      const LIB_JsonValue *val = &value[0];
      const S32 last = size - 1;
      for (S32 index = 0;; ++index, ++val) {
        CreateString(*val);

        if (index == last) {
          break;
        } else {
          StrCat(","); /* add ',' */
        }
      }
    }
    StrCat("]");
  } else if (value.IsNull()) {
    StrCat("null");
  } else if (value.IsTrue()) {
    StrCat("true");
  } else if (value.IsFalse()) {
    StrCat("false");
  } else if (value.IsString() != false) {
    StrCat("\""); /* '\"' + string + '\"' */
    StringCat(value.GetStringLength(), value.GetString());
    StrCat("\"");
  } else if (value.IsNumber()) {
    if (value.IsS32() != false) {
      StrCat("%d", value.GetS32());
    } else if (value.IsU32() != false) {
      StrCat("%u", value.GetU32());
    } else if (value.IsS64() != false) {
      StrCat("%lld", value.GetS64());
    } else if (value.IsU64() != false) {
      StrCat("%llu", value.GetU64());
    } else if (value.IsDouble() != false) {
      StrCat("%lf", value.GetDouble());
    }
  } else {
    ADK_ASSERT(0);
  }
}
void LIB_JsonDocument::StrCat(const Char *str, ...) {
  static char buf[BUF_MAX];
  size_t len = 0;
  va_list args;

  va_start(args, str);
  len += vsprintf(buf + len, str, args);
  ADK_ASSERT(len <= BUF_MAX);
  va_end(args);

  memcpy(value_.stack.Push<Byte>((U32)len), buf, len);
}
void LIB_JsonDocument::StringCat(U32 length, const Char *str) {
  memcpy(value_.stack.Push<Byte>(length), str, length);
}

/* value management functionally */
void LIB_JsonDocument::StartObject() {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(LIB_kJsonObject);
}
void LIB_JsonDocument::EndObject(S32 membercount) {
  LIB_TJsonMember *members = value_.stack.Pop<LIB_TJsonMember>(membercount);
  value_.stack.Top<LIB_JsonValue>()->AddMember(members, membercount);
}
void LIB_JsonDocument::StartArray() {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(LIB_kJsonArray);
}
void LIB_JsonDocument::EndArray(S32 elementcount) {
  LIB_JsonValue *elements = value_.stack.Pop<LIB_JsonValue>(elementcount);
  value_.stack.Top<LIB_JsonValue>()->AddElements(elements, elementcount);
}
void LIB_JsonDocument::AddNull() {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(LIB_kJsonNull);
}
void LIB_JsonDocument::AddBool(Bool b) {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue((bool)b);
}
void LIB_JsonDocument::AddString(const Char *str, U32 length,
                                 Bool isWriteNull) {
  if (isWriteNull != false) {
    Char *end = static_cast<Char *>((Char *)str + length);
    *end = '\0';
  }
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(str, length, false);
}
void LIB_JsonDocument::AddS32(S32 i) {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(i);
}
void LIB_JsonDocument::AddU32(U32 u) {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(u);
}
void LIB_JsonDocument::AddS64(S64 i) {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(i);
}
void LIB_JsonDocument::AddU64(U64 u) {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(u);
}
void LIB_JsonDocument::AddDouble(Double d) {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(d);
}
void LIB_JsonDocument::AddBinary(const void *b, U32 size) {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(b, size);
}
void LIB_JsonDocument::AddDocument(const LIB_JsonDocument *doc) {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(doc);
}
void LIB_JsonDocument::AddValue(const LIB_JsonValue *val) {
  new (value_.stack.Push<LIB_JsonValue>()) LIB_JsonValue(val);
}

/* parse management functionally */
void LIB_JsonDocument::SetError(const Char *msg, size_t offset) {
  error_.isError = true;
  error_.msg = msg;
  error_.offset = static_cast<U32>(offset);
  //CRITICAL_LOG("Json parse error: Offset=%d\n", error_.offset);
  ADK_ASSERT(error_.isError == false);
  //  ADK_ASSERT_MSG(error_.isError==false,error_.msg);
}

ADK_END_NAMESPACE

