/**
 * \file math_scalar_intern_ios.h
 * \ingroup math
 * \author tomonori tanaka
 * \~english
 * \~japanese
 */

#ifndef MATH_SCALAR_IOS_H_
#define MATH_SCALAR_IOS_H_

ADK_BEGIN_NAMESPACE

/* inline function implementation -------------------*/
/*  */
MATH_INLINE F32 MathSqrt(F32 x) { return sqrt(x); }
/*  */
MATH_INLINE F32 MathAbs(F32 x) { return abs(x); }
/*  */
MATH_INLINE F32 MathCos(F32 x) { return cosf(x); }
/*  */
MATH_INLINE F32 MathSin(F32 x) { return sinf(x); }
/*  */
MATH_INLINE F32 MathTan(F32 x) { return tanf(x); }
/**
*/
MATH_INLINE F32 MathCot(F32 x) { return 1.0f / tanf(x); }
/*  */
MATH_INLINE F32 MathAcos(F32 x) {
  ADK_ASSERT(x <= 1.0f);
  return acosf(x);
}
/*  */
MATH_INLINE F32 MathAsin(F32 x) { return asinf(x); }
/*  */
MATH_INLINE F32 MathAtan(F32 x) { return atanf(x); }
/*  */
MATH_INLINE F32 MathAtan2(F32 x, F32 y) { return atan2f(x, y); }
/*  */
MATH_INLINE F32 MathExp(F32 x) { return expf(x); }
/*  */
MATH_INLINE F32 MathLog(F32 x) { return logf(x); }
/*  */
MATH_INLINE F32 MathPow(F32 x, F32 y) { return powf(x, y); }
/*  */
MATH_INLINE Bool MathFuzzyZero(F32 x) { return MathAbs(x) < MATH_EPSILON; }
/*  */
MATH_INLINE Bool MathFuzzyEqual(F32 a, F32 eps) {
  return (((a) <= eps) && !((a) < -eps));
}
/*  */
MATH_INLINE Bool greaterMathFuzzyEqual(F32 a, F32 eps) {
  return (!((a) <= eps));
}
/*  */
MATH_INLINE S32 MathIsNegative(F32 x) { return x < 0.0f ? 1 : 0; }
/*  */
MATH_INLINE F32 MathRadians(F32 x) { return x * MATH_RADS_PER_DEG; }
/*  */
MATH_INLINE F32 MathDegrees(F32 x) { return x * MATH_DEGS_PER_RAD; }
/*  */
MATH_INLINE F32 MathFSel(F32 a, F32 b, F32 c) { return a >= 0 ? b : c; }

ADK_END_NAMESPACE

#endif /* MATH_SCALAR_IOS_H_ */

