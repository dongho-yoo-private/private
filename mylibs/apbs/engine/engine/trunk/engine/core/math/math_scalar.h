/**
 * \file math_scalar.h
 * \ingroup math
 * \author tomonori tanaka
 * \~english
 * \~japanese
 */

#ifndef MATH_SCALAR_H_
#define MATH_SCALAR_H_

ADK_BEGIN_NAMESPACE

/**
*/
MATH_INLINE F32 MathSqrt(F32 x);
/**
*/
MATH_INLINE F32 MathAbs(F32 x);
/**
*/
MATH_INLINE F32 MathCos(F32 x);
/**
*/
MATH_INLINE F32 MathSin(F32 x);
/**
*/
MATH_INLINE F32 MathTan(F32 x);
/**
*/
MATH_INLINE F32 MathCot(F32 x);
/**
*/
MATH_INLINE F32 MathAcos(F32 x);
/**
*/
MATH_INLINE F32 MathAsin(F32 x);
/**
*/
MATH_INLINE F32 MathAtan(F32 x);
/**
*/
MATH_INLINE F32 MathAtan2(F32 x, F32 y);
/**
*/
MATH_INLINE F32 MathExp(F32 x);
/**
*/
MATH_INLINE F32 MathLog(F32 x);
/**
*/
MATH_INLINE F32 MathPow(F32 x, F32 y);
/**
*/
MATH_INLINE Bool MathFuzzyZero(F32 x);
/**
*/
MATH_INLINE Bool MathFuzzyEqual(F32 a, F32 eps);
/**
*/
MATH_INLINE Bool MathGreaterEqual(F32 a, F32 eps);
/**
*/
MATH_INLINE S32 MathIsNegative(F32 x);
/**
*/
MATH_INLINE F32 MathRadians(F32 x);
/**
*/
MATH_INLINE F32 MathDegrees(F32 x);
/**
*/
MATH_INLINE F32 MathFSel(F32 a, F32 b, F32 c);

ADK_END_NAMESPACE

#endif /* MATH_SCALAR_H_ */

