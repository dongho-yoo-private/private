/**
 * \file math_color.h
 * \ingroup math
 * \author tomonori tanaka
 * \~english
 * \~japanese
 */

#ifndef MATH_COLOR_H_
#define MATH_COLOR_H_

/* カラー */
MATH_INLINE adk::TColor operator+(const adk::TColor &u, const adk::TColor &v);
MATH_INLINE adk::TColor operator-(const adk::TColor &u, const adk::TColor &v);
MATH_INLINE adk::TColor operator-(const adk::TColor &v);
MATH_INLINE adk::TColor operator*(const adk::TColor &u, const adk::TColor &v);
MATH_INLINE adk::TColor operator*(const F32 s, const adk::TColor &v);
MATH_INLINE adk::TColor operator*(const adk::TColor &v, const F32 s);
MATH_INLINE adk::TColor operator/(const adk::TColor &u, const adk::TColor &v);
MATH_INLINE adk::TColor operator/(const adk::TColor &v, const F32 s);
MATH_INLINE adk::TColor operator/(const F32 s, const adk::TColor &v);
MATH_INLINE adk::TColor operator/=(const adk::TColor &u, const adk::TColor &v);
MATH_INLINE adk::TColor operator/=(const adk::TColor &v, const F32 s);
MATH_INLINE adk::TColor operator/=(const F32 s, const adk::TColor &v);
MATH_INLINE Bool operator==(const adk::TColor &u, const adk::TColor &v);
/* カラー: 不動少数 */
MATH_INLINE adk::TColorF operator+(const adk::TColorF &u,
                                   const adk::TColorF &v);
MATH_INLINE adk::TColorF operator-(const adk::TColorF &u,
                                   const adk::TColorF &v);
MATH_INLINE adk::TColorF operator-(const adk::TColorF &v);
MATH_INLINE adk::TColorF operator*(const adk::TColorF &u,
                                   const adk::TColorF &v);
MATH_INLINE adk::TColorF operator*(const F32 s, const adk::TColorF &v);
MATH_INLINE adk::TColorF operator*(const adk::TColorF &v, const F32 s);
MATH_INLINE adk::TColorF operator/(const adk::TColorF &u,
                                   const adk::TColorF &v);
MATH_INLINE adk::TColorF operator/(const adk::TColorF &v, const F32 s);
MATH_INLINE adk::TColorF operator/(const F32 s, const adk::TColorF &v);
MATH_INLINE adk::TColorF operator/=(const adk::TColorF &u,
                                    const adk::TColorF &v);
MATH_INLINE adk::TColorF operator/=(const adk::TColorF &v, const F32 s);
MATH_INLINE adk::TColorF operator/=(const F32 s, const adk::TColorF &v);
MATH_INLINE Bool operator==(const adk::TColorF &u, const adk::TColorF &v);

#if WITH_WIN32
#include "core/math/intern/math_color_win32.h"
#elif WITH_COCOA
#include "core/math/intern/math_color_cocoa.h"
#elif WITH_CARBON
#include "core/math/intern/math_color_carbon.h"
#elif WITH_IOS
#include "core/math/intern/math_color_ios.h"
#elif WITH_X11
#include "core/math/intern/math_color_x11.h"
#elif WITH_ANDROID
#include "core/math/intern/math_color_android.h"
#elif WITH_PSP
#include "core/math/intern/math_color_psp.h"
#elif WITH_VITA
#include "core/math/intern/math_color_vita.h"
#elif WITH_PS3
#include "core/math/intern/math_color_ps3.h"
#elif WITH_XBOX360
#include "core/math/intern/math_color_xbox360.h"
#elif WITH_WII
#include "core/math/intern/math_color_wii.h"
#elif WITH_3DS
#include "core/math/intern/math_color_3ds.h"
#elif WITH_CAFE
#include "core/math/intern/math_color_cafe.h"
#endif

ADK_BEGIN_NAMESPACE

/* inline function definitions -------------------*/
/**
 * \brief 初期化(R=0.0 G=0.0 B=0.0 A=0.0)
*/
MATH_INLINE void MathColorUnit(adk::TColor *c);
MATH_INLINE void MathColorUnit(adk::TColorF *c);
/**
 * \brief 設定
*/
MATH_INLINE void MathColorSet(adk::TColor *c, const U8 r, const U8 g,
                              const U8 b, const U8 a);
MATH_INLINE void MathColorSet(adk::TColorF *c, const F32 r, const F32 g,
                              const F32 b, const F32 a);
/* *
  *\brief コピー
*/
MATH_INLINE void MathColorCopy(adk::TColor *c1, const adk::TColor *c2);
MATH_INLINE void MathColorCopy(adk::TColor *c1, const adk::TColorF *c2);
MATH_INLINE void MathColorCopy(adk::TColorF *c1, const adk::TColor *c2);
MATH_INLINE void MathColorCopy(adk::TColorF *c1, const adk::TColorF *c2);
/* *
  *\brief 加算
*/
MATH_INLINE void MathColorMathColorAdd(adk::TColor *c1, const adk::TColor *c2,
                                       const adk::TColor *c3);
MATH_INLINE void MathColorAdd(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColorF *c3);
MATH_INLINE void MathColorAdd(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColor *c3);
MATH_INLINE void MathColorAdd(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColorF *c3);
/* *
  *\brief 減算
*/
MATH_INLINE void MathColorSub(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColor *c3);
MATH_INLINE void MathColorSub(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColorF *c3);
MATH_INLINE void MathColorSub(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColor *c3);
MATH_INLINE void MathColorSub(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColorF *c3);
/* *
  *\brief 乗算
*/
MATH_INLINE void MathColorMul(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColor *c3);
MATH_INLINE void MathColorMul(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColorF *c3);
MATH_INLINE void MathColorMul(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColor *c3);
MATH_INLINE void MathColorMul(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColorF *c3);
/* *
  *\brief 除算
*/
MATH_INLINE void MathColorDiv(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColor *c3);
MATH_INLINE void MathColorDiv(adk::TColor *c1, const adk::TColor *c2,
                              const adk::TColorF *c3);
MATH_INLINE void MathColorDiv(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColor *c3);
MATH_INLINE void MathColorDiv(adk::TColorF *c1, const adk::TColorF *c2,
                              const adk::TColorF *c3);
/* *
  *\brief スケール
*/
MATH_INLINE void MathColorScale(adk::TColor *c1, const adk::TColor *c2,
                                const F32 scale);
MATH_INLINE void MathColorScale(adk::TColorF *c1, const adk::TColorF *c2,
                                const F32 scale);
/* *
  *\brief 線形補間：(c2 + (c3 - c2)  *t)
*/
MATH_INLINE void MathColorLerp(adk::TColor *c1, const adk::TColor *c2,
                               const adk::TColor *c3, const F32 t);
MATH_INLINE void MathColorLerp(adk::TColorF *c1, const adk::TColorF *c2,
                               const adk::TColorF *c3, const F32 t);

ADK_END_NAMESPACE

#endif /* MATH_COLOR_H_ */

