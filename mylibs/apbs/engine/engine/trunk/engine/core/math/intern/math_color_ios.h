/**
 * \file math_color_ios.h
 * \ingroup math
 * \author tomonori tanaka
 * \~english
 * \~japanese
 */

#ifndef MATH_COLOR_IOS_H_
#define MATH_COLOR_IOS_H_

ADK_BEGIN_NAMESPACE

/* struct -------------------*/
/* RGBAカラー */
struct TColor {
  /* vars ---------------------------------------------------------*/
  union {
    U32 c;
    struct {
      U8 r, g, b, a;
    };
  };

  /* operator definitions
   * ---------------------------------------------------------*/
  MATH_INLINE void operator+=(const adk::TColor &c);
  MATH_INLINE void operator-=(const adk::TColor &c);
  MATH_INLINE void operator*=(const F32 s);
  MATH_INLINE void operator*=(const adk::TColor &c);
  MATH_INLINE void operator/=(const F32 s);
  MATH_INLINE void operator/=(const adk::TColor &c);
};
/* カラー: 不動少数 */
struct TColorF {
  /* vars ---------------------------------------------------------*/
  F32 r, g, b, a;

  /* operator definitions
   * ---------------------------------------------------------*/
  MATH_INLINE void operator+=(const adk::TColorF &c);
  MATH_INLINE void operator-=(const adk::TColorF &c);
  MATH_INLINE void operator*=(const F32 s);
  MATH_INLINE void operator*=(const adk::TColorF &c);
  MATH_INLINE void operator/=(const F32 s);
  MATH_INLINE void operator/=(const adk::TColorF &c);
};

ADK_END_NAMESPACE

#endif /* MATH_COLOR_IOS_H_ */

