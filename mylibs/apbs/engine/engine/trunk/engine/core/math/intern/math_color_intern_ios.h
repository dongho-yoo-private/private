/**
 * \file math_color_intern_ios.h
 * \ingroup library
 * \brief カラー内部実装
 */

#ifndef MATH_COLOR_INTERN_IOS_H_
#define MATH_COLOR_INTERN_IOS_H_

/* operator implementation -------------------*/
/* カラー */
MATH_INLINE adk::TColor operator+(const adk::TColor &u, const adk::TColor &v) {
  adk::TColor t;
  t.r = u.r + v.r;
  t.g = u.g + v.g;
  t.b = u.b + v.b;
  t.a = u.a + v.a;
  return t;
}
MATH_INLINE adk::TColor operator-(const adk::TColor &u, const adk::TColor &v) {
  adk::TColor t;
  t.r = u.r - v.r;
  t.g = u.g - v.g;
  t.b = u.b - v.b;
  t.a = u.a - v.a;
  return t;
}
MATH_INLINE adk::TColor operator-(const adk::TColor &c) {
  adk::TColor t;
  t.r = -c.r;
  t.g = -c.g;
  t.b = -c.b;
  t.a = -c.a;
  return t;
}
MATH_INLINE adk::TColor operator*(const adk::TColor &u, const adk::TColor &v) {
  adk::TColor t;
  t.r = u.r * v.r;
  t.g = u.g * v.g;
  t.b = u.b * v.b;
  t.a = u.a * v.a;
  return t;
}
MATH_INLINE adk::TColor operator*(const F32 s, const adk::TColor &v) {
  adk::TColor t;
  t.r = (U8)(s * (F32)v.r);
  t.g = (U8)(s * (F32)v.g);
  t.b = (U8)(s * (F32)v.b);
  t.a = (U8)(s * (F32)v.a);
  return t;
}
MATH_INLINE adk::TColor operator*(const adk::TColor &v, const F32 s) {
  adk::TColor t;
  t.r = (U8)((F32)v.r * s);
  t.g = (U8)((F32)v.g * s);
  t.b = (U8)((F32)v.b * s);
  t.a = (U8)((F32)v.a * s);
  return t;
}
MATH_INLINE adk::TColor operator/(const adk::TColor &u, const adk::TColor &v) {
  adk::TColor t;
  t.r = u.r / v.r;
  t.g = u.g / v.g;
  t.b = u.b / v.b;
  t.a = u.a / v.a;
  return t;
}
MATH_INLINE adk::TColor operator/(const adk::TColor &v, const F32 s) {
  adk::TColor t;
  t.r = (U8)((F32)v.r / s);
  t.g = (U8)((F32)v.g / s);
  t.b = (U8)((F32)v.b / s);
  t.a = (U8)((F32)v.a / s);
  return t;
}
MATH_INLINE adk::TColor operator/(const F32 s, const adk::TColor &v) {
  adk::TColor t;
  t.r = (U8)(s / (F32)v.r);
  t.g = (U8)(s / (F32)v.g);
  t.b = (U8)(s / (F32)v.b);
  t.a = (U8)(s / (F32)v.a);
  return t;
}
MATH_INLINE Bool operator==(const adk::TColor &u, const adk::TColor &v) {
  return (u.r == v.r && u.g == v.g && u.b == v.b && u.a == v.a);
}
/* カラー: 不動少数 */
MATH_INLINE adk::TColorF operator+(const adk::TColorF &u,
                                   const adk::TColorF &v) {
  adk::TColorF t;
  t.r = u.r + v.r;
  t.g = u.g + v.g;
  t.b = u.b + v.b;
  t.a = u.a + v.a;
  return t;
}
MATH_INLINE adk::TColorF operator-(const adk::TColorF &u,
                                   const adk::TColorF &v) {
  adk::TColorF t;
  t.r = u.r - v.r;
  t.g = u.g - v.g;
  t.b = u.b - v.b;
  t.a = u.a - v.a;
  return t;
}
MATH_INLINE adk::TColorF operator-(const adk::TColorF &c) {
  adk::TColorF t;
  t.r = -c.r;
  t.g = -c.g;
  t.b = -c.b;
  t.a = -c.a;
  return t;
}
MATH_INLINE adk::TColorF operator*(const adk::TColorF &u,
                                   const adk::TColorF &v) {
  adk::TColorF t;
  t.r = u.r * v.r;
  t.g = u.g * v.g;
  t.b = u.b * v.b;
  t.a = u.a * v.a;
  return t;
}
MATH_INLINE adk::TColorF operator*(const F32 s, const adk::TColorF &v) {
  adk::TColorF t;
  t.r = (U8)(s * (F32)v.r);
  t.g = (U8)(s * (F32)v.g);
  t.b = (U8)(s * (F32)v.b);
  t.a = (U8)(s * (F32)v.a);
  return t;
}
MATH_INLINE adk::TColorF operator*(const adk::TColorF &v, const F32 s) {
  adk::TColorF t;
  t.r = (U8)(s * (F32)v.r);
  t.g = (U8)(s * (F32)v.g);
  t.b = (U8)(s * (F32)v.b);
  t.a = (U8)(s * (F32)v.a);
  return t;
}
MATH_INLINE adk::TColorF operator/(const adk::TColorF &u,
                                   const adk::TColorF &v) {
  adk::TColorF t = {u.r / v.r, u.g / v.g, u.b / v.b, u.a / v.a};
  return t;
}
MATH_INLINE adk::TColorF operator/(const adk::TColorF &v, const F32 s) {
  adk::TColorF t;
  t.r = (U8)((F32)v.r / s);
  t.g = (U8)((F32)v.g / s);
  t.b = (U8)((F32)v.b / s);
  t.a = (U8)((F32)v.a / s);
  return t;
}
MATH_INLINE adk::TColorF operator/(const F32 s, const adk::TColorF &v) {
  adk::TColorF t;
  t.r = (U8)(s / (F32)v.r);
  t.g = (U8)(s / (F32)v.g);
  t.b = (U8)(s / (F32)v.b);
  t.a = (U8)(s / (F32)v.a);
  return t;
}
MATH_INLINE Bool operator==(const adk::TColorF &u, const adk::TColorF &v) {
  return (u.r == v.r && u.g == v.g && u.b == v.b && u.a == v.a);
}

ADK_BEGIN_NAMESPACE

/* macro -------------------*/
#define iMathColorF32ToU8(f) ((U8)(f * 255.0f))
#define iMathColorU8ToF32(u) ((F32)u * 255.0f)

/* struct operator implementation
 * ---------------------------------------------------------*/
/* カラー */
MATH_INLINE void TColor::operator+=(const adk::TColor &c) { *this = *this + c; }
MATH_INLINE void TColor::operator-=(const adk::TColor &c) { *this = *this - c; }
MATH_INLINE void TColor::operator*=(const F32 s) { *this = *this * s; }
MATH_INLINE void TColor::operator*=(const adk::TColor &c) { *this = *this * c; }
MATH_INLINE void TColor::operator/=(const F32 s) { *this = *this / s; }
MATH_INLINE void TColor::operator/=(const adk::TColor &c) { *this = *this / c; }
/* カラー: 不動少数 */
MATH_INLINE void TColorF::operator+=(const adk::TColorF &c) {
  *this = *this + c;
}
MATH_INLINE void TColorF::operator-=(const adk::TColorF &c) {
  *this = *this - c;
}
MATH_INLINE void TColorF::operator*=(const F32 s) { *this = *this * s; }
MATH_INLINE void TColorF::operator*=(const adk::TColorF &c) {
  *this = *this * c;
}
MATH_INLINE void TColorF::operator/=(const F32 s) { *this = *this / s; }
MATH_INLINE void TColorF::operator/=(const adk::TColorF &c) {
  *this = *this / c;
}

/* inline function definitions -------------------*/
MATH_INLINE void MathColorUnit(adk::TColor *c) { c->c = 0x00000000; }
MATH_INLINE void MathColorUnit(adk::TColorF *c) {
  c->r = c->g = c->b = c->a = 0.0f;
}
MATH_INLINE void MathColorSet(adk::TColor *c, const U8 r, const U8 g,
                              const U8 b, const U8 a) {
  c->r = r;
  c->g = g;
  c->b = b;
  c->a = a;
}
MATH_INLINE void MathColorSet(adk::TColorF *c, const F32 r, const F32 g,
                              const F32 b, const F32 a) {
  c->r = r;
  c->g = g;
  c->b = b;
  c->a = a;
}
MATH_INLINE void MathColorCopy(adk::TColor *c1, const adk::TColor *c2) {
  *c1 = *c2;
}
MATH_INLINE void MathColorCopy(adk::TColor *c1, const adk::TColorF *c2) {
  c1->r = (U8)((F32)c2->g * 255.0f);
  c1->g = (U8)((F32)c2->g * 255.0f);
  c1->b = (U8)((F32)c2->b * 255.0f);
  c1->a = (U8)((F32)c2->a * 255.0f);
}
MATH_INLINE void MathColorCopy(adk::TColorF *c1, const adk::TColor *c2) {
  c1->r = iMathColorU8ToF32(c2->r);
  c1->g = iMathColorU8ToF32(c2->g);
  c1->b = iMathColorU8ToF32(c2->b);
  c1->a = iMathColorU8ToF32(c2->a);
}
MATH_INLINE void MathColorCopy(adk::TColorF *c1, const adk::TColorF *c2) {
  *c1 = *c2;
}
MATH_INLINE void MathColorAdd(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColor *c3) {
  c1->r = c2->r + c3->r;
  c1->g = c2->g + c3->g;
  c1->b = c2->b + c3->b;
  c1->a = c2->a + c3->a;
}
MATH_INLINE void MathColorAdd(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColorF *c3) {
  c1->r = c2->r + (U8)((F32)c3->r * 255.0f);
  c1->g = c2->g + (U8)((F32)c3->g * 255.0f);
  c1->b = c2->b + (U8)((F32)c3->b * 255.0f);
  c1->a = c2->a + (U8)((F32)c3->a * 255.0f);
}
MATH_INLINE void MathColorAdd(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColor *c3) {
  c1->r = c2->r + (F32)c3->r / 255.0f;
  c1->g = c2->g + (F32)c3->g / 255.0f;
  c1->b = c2->b + (F32)c3->b / 255.0f;
  c1->a = c2->a + (F32)c3->a / 255.0f;
}
MATH_INLINE void MathColorAdd(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColorF *c3) {
  c1->r = c2->r + c3->r;
  c1->g = c2->g + c3->g;
  c1->b = c2->b + c3->b;
  c1->a = c2->a + c3->a;
}
MATH_INLINE void MathColorSub(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColor *c3) {
  c1->r = c2->r - c3->r;
  c1->g = c2->g - c3->g;
  c1->b = c2->b - c3->b;
  c1->a = c2->a - c3->a;
}
MATH_INLINE void MathColorSub(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColorF *c3) {
  c1->r = c2->r - (U8)((F32)c3->r * 255.0f);
  c1->g = c2->g - (U8)((F32)c3->g * 255.0f);
  c1->b = c2->b - (U8)((F32)c3->b * 255.0f);
  c1->a = c2->a - (U8)((F32)c3->a * 255.0f);
}
MATH_INLINE void MathColorSub(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColor *c3) {
  c1->r = c2->r - (F32)c3->r / 255.0f;
  c1->g = c2->g - (F32)c3->g / 255.0f;
  c1->b = c2->b - (F32)c3->b / 255.0f;
  c1->a = c2->a - (F32)c3->a / 255.0f;
}
MATH_INLINE void MathColorSub(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColorF *c3) {
  c1->r = c2->r - c3->r;
  c1->g = c2->g - c3->g;
  c1->b = c2->b - c3->b;
  c1->a = c2->a - c3->a;
}
MATH_INLINE void MathColorMul(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColor *c3) {
  c1->r = c2->r * c3->r;
  c1->g = c2->g * c3->g;
  c1->b = c2->b * c3->b;
  c1->a = c2->a * c3->a;
}
MATH_INLINE void MathColorMul(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColorF *c3) {
  c1->r = c2->r * (U8)((F32)c3->r * 255.0f);
  c1->g = c2->g * (U8)((F32)c3->g * 255.0f);
  c1->b = c2->b * (U8)((F32)c3->b * 255.0f);
  c1->a = c2->a * (U8)((F32)c3->a * 255.0f);
}
MATH_INLINE void MathColorMul(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColor *c3) {
  c1->r = c2->r * (F32)c3->r / 255.0f;
  c1->g = c2->g * (F32)c3->g / 255.0f;
  c1->b = c2->b * (F32)c3->b / 255.0f;
  c1->a = c2->a * (F32)c3->a / 255.0f;
}
MATH_INLINE void MathColorMul(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColorF *c3) {
  c1->r = c2->r * c3->r;
  c1->g = c2->g * c3->g;
  c1->b = c2->b * c3->b;
  c1->a = c2->a * c3->a;
}
MATH_INLINE void MathColorDiv(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColor *c3) {
  c1->r = c2->r / c3->r;
  c1->g = c2->g / c3->g;
  c1->b = c2->b / c3->b;
  c1->a = c2->a / c3->a;
}
MATH_INLINE void MathColorDiv(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColorF *c3) {
  c1->r = c2->r / (U8)((F32)c3->r * 255.0f);
  c1->g = c2->g / (U8)((F32)c3->g * 255.0f);
  c1->b = c2->b / (U8)((F32)c3->b * 255.0f);
  c1->a = c2->a / (U8)((F32)c3->a * 255.0f);
}
MATH_INLINE void MathColorDiv(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColor *c3) {
  c1->r = c2->r / (F32)c3->r / 255.0f;
  c1->g = c2->g / (F32)c3->g / 255.0f;
  c1->b = c2->b / (F32)c3->b / 255.0f;
  c1->a = c2->a / (F32)c3->a / 255.0f;
}
MATH_INLINE void MathColorDiv(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColorF *c3) {
  c1->r = c2->r / c3->r;
  c1->g = c2->g / c3->g;
  c1->b = c2->b / c3->b;
  c1->a = c2->a / c3->a;
}
MATH_INLINE void MathColorScale(adk::TColor *c1, const adk::TColor *c2,
                                const F32 scale) {
  c1->r = (U8)((F32)c2->r * scale);
  c1->g = (U8)((F32)c2->g * scale);
  c1->b = (U8)((F32)c2->b * scale);
  c1->a = (U8)((F32)c2->a * scale);
}
MATH_INLINE void MathColorScale(adk::TColorF *c1, const adk::TColorF *c2,
                                const F32 scale) {
  c1->r = c2->r * scale;
  c1->g = c2->g * scale;
  c1->b = c2->b * scale;
  c1->a = c2->a * scale;
}
MATH_INLINE void MathColorLerp(adk::TColor *c1, const adk::TColor *c2,
                               const adk::TColor *c3, const F32 t) {
  *c1 = *c2 + ((*c3 - *c2) * t);
}
MATH_INLINE void MathColorLerp(adk::TColorF *c1, const adk::TColorF *c2,
                               const adk::TColorF *c3, const F32 t) {
  *c1 = *c2 + ((*c3 - *c2) * t);
}

ADK_END_NAMESPACE

#endif /* MATH_COLOR_INTERN_IOS_H_ */

