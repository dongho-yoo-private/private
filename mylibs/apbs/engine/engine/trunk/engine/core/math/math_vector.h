/**
 * \file math_vector.h
 * \ingroup math
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief ベクトル演算定義部
 */

#ifndef MATH_VECTOR_H_
#define MATH_VECTOR_H_

/* operator definitions -------------------*/
/* ベクトル: 4次元 */
MATH_INLINE adk::TVector4 operator+(const adk::TVector4 &u,
                                    const adk::TVector4 &v);
MATH_INLINE adk::TVector4 operator+(const adk::TVector4 &v, const F32 s);
MATH_INLINE adk::TVector4 operator+(const F32 s, const adk::TVector4 &v);
MATH_INLINE adk::TVector4 operator-(const adk::TVector4 &u,
                                    const adk::TVector4 &v);
MATH_INLINE adk::TVector4 operator-(const adk::TVector4 &v, const F32 s);
MATH_INLINE adk::TVector4 operator-(const F32 s, const adk::TVector4 &v);
MATH_INLINE adk::TVector4 operator-(const adk::TVector4 &v);
MATH_INLINE adk::TVector4 operator*(const adk::TVector4 &u,
                                    const adk::TVector4 &v);
MATH_INLINE adk::TVector4 operator*(const F32 s, const adk::TVector4 &v);
MATH_INLINE adk::TVector4 operator*(const adk::TVector4 &v, const F32 s);
MATH_INLINE adk::TVector4 operator/(const adk::TVector4 &u,
                                    const adk::TVector4 &v);
MATH_INLINE adk::TVector4 operator/(const adk::TVector4 &v, const F32 s);
MATH_INLINE adk::TVector4 operator/(const F32 s, const adk::TVector4 &v);
MATH_INLINE Bool operator==(const adk::TVector4 &u, const adk::TVector4 &v);
/* ベクトル: 3次元 */
MATH_INLINE adk::TVector3 operator+(const adk::TVector3 &u,
                                    const adk::TVector3 &v);
MATH_INLINE adk::TVector3 operator+(const adk::TVector3 &v, const F32 s);
MATH_INLINE adk::TVector3 operator+(const F32 s, const adk::TVector3 &v);
MATH_INLINE adk::TVector3 operator-(const adk::TVector3 &u,
                                    const adk::TVector3 &v);
MATH_INLINE adk::TVector3 operator-(const adk::TVector3 &v, const F32 s);
MATH_INLINE adk::TVector3 operator-(const F32 s, const adk::TVector3 &v);
MATH_INLINE adk::TVector3 operator-(const adk::TVector3 &v);
MATH_INLINE adk::TVector3 operator*(const adk::TVector3 &u,
                                    const adk::TVector3 &v);
MATH_INLINE adk::TVector3 operator*(const F32 s, const adk::TVector3 &v);
MATH_INLINE adk::TVector3 operator*(const adk::TVector3 &v, const F32 s);
MATH_INLINE adk::TVector3 operator/(const adk::TVector3 &u,
                                    const adk::TVector3 &v);
MATH_INLINE adk::TVector3 operator/(const adk::TVector3 &v, const F32 s);
MATH_INLINE adk::TVector3 operator/(const F32 s, const adk::TVector3 &v);
MATH_INLINE Bool operator==(const adk::TVector3 &u, const adk::TVector3 &v);
/* ベクトル: 2次元 */
MATH_INLINE adk::TVector2 operator+(const adk::TVector2 &u,
                                    const adk::TVector2 &v);
MATH_INLINE adk::TVector2 operator+(const adk::TVector2 &v, const F32 s);
MATH_INLINE adk::TVector2 operator+(const F32 s, const adk::TVector2 &v);
MATH_INLINE adk::TVector2 operator-(const adk::TVector2 &u,
                                    const adk::TVector2 &v);
MATH_INLINE adk::TVector2 operator-(const adk::TVector2 &v, const F32 s);
MATH_INLINE adk::TVector2 operator-(const F32 s, const adk::TVector2 &v);
MATH_INLINE adk::TVector2 operator-(const adk::TVector2 &v);
MATH_INLINE adk::TVector2 operator*(const adk::TVector2 &u,
                                    const adk::TVector2 &v);
MATH_INLINE adk::TVector2 operator*(const F32 s, const adk::TVector2 &v);
MATH_INLINE adk::TVector2 operator*(const adk::TVector2 &v, const F32 s);
MATH_INLINE adk::TVector2 operator/(const adk::TVector2 &u,
                                    const adk::TVector2 &v);
MATH_INLINE adk::TVector2 operator/(const adk::TVector2 &v, const F32 s);
MATH_INLINE adk::TVector2 operator/(const F32 s, const adk::TVector2 &v);
MATH_INLINE Bool operator==(const adk::TVector2 &u, const adk::TVector2 &v);

/* struct -------------------*/
#if WITH_WIN32
#include "core/math/intern/math_vector_win32.h"
#elif WITH_COCOA
#include "core/math/intern/math_vector_cocoa.h"
#elif WITH_CARBON
#include "core/math/intern/math_vector_carbon.h"
#elif WITH_IOS
#include "core/math/intern/math_vector_ios.h"
#elif WITH_X11
#include "core/math/intern/math_vector_x11.h"
#elif WITH_ANDROID
#include "core/math/intern/math_vector_android.h"
#elif WITH_PSP
#include "core/math/intern/math_vector_psp.h"
#elif WITH_VITA
#include "core/math/intern/math_vector_vita.h"
#elif WITH_PS3
#include "core/math/intern/math_vector_ps3.h"
#elif WITH_PS4
#include "core/math/intern/math_vector_ps4.h"
#elif WITH_XBOX360
#include "core/math/intern/math_vector_xbox360.h"
#elif WITH_WII
#include "core/math/intern/math_vector_wii.h"
#elif WITH_3DS
#include "core/math/intern/math_vector_3ds.h"
#endif

ADK_BEGIN_NAMESPACE

/* inline function definition -------------------*/
/** 初期化(X=0.0 Y=0.0 Z=0.0 W=1.0)
*/
MATH_INLINE void MathVectorUnit(adk::TVector4 *v);
MATH_INLINE void MathVectorUnit(adk::TVector3 *v);
MATH_INLINE void MathVectorUnit(adk::TVector2 *v);
/** 設定
*/
MATH_INLINE void MathVectorSet(adk::TVector4 *v, const F32 x, const F32 y,
                               const F32 z, const F32 w);
MATH_INLINE void MathVectorSet(adk::TVector3 *v, const F32 x, const F32 y,
                               const F32 z);
MATH_INLINE void MathVectorSet(adk::TVector2 *v, const F32 x, const F32 y);
/** コピー
*/
MATH_INLINE void MathVectorCopy(adk::TVector4 *v1, const adk::TVector4 *v2);
MATH_INLINE void MathVectorCopy(adk::TVector4 *v1, const adk::TVector3 *v2);
MATH_INLINE void MathVectorCopy(adk::TVector4 *v1, const adk::TVector2 *v2);
MATH_INLINE void MathVectorCopy(adk::TVector3 *v1, const adk::TVector4 *v2);
MATH_INLINE void MathVectorCopy(adk::TVector3 *v1, const adk::TVector3 *v2);
MATH_INLINE void MathVectorCopy(adk::TVector3 *v1, const adk::TVector2 *v2);
MATH_INLINE void MathVectorCopy(adk::TVector2 *v1, const adk::TVector4 *v2);
MATH_INLINE void MathVectorCopy(adk::TVector2 *v1, const adk::TVector3 *v2);
MATH_INLINE void MathVectorCopy(adk::TVector2 *v1, const adk::TVector2 *v2);
/** 加算
*/
MATH_INLINE void MathVectorAdd(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorAdd(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorAdd(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector2 *v3);
MATH_INLINE void MathVectorAdd(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorAdd(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorAdd(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector2 *v3);
MATH_INLINE void MathVectorAdd(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorAdd(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorAdd(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector2 *v3);
/** 減算
*/
MATH_INLINE void MathVectorSub(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorSub(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorSub(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector2 *v3);
MATH_INLINE void MathVectorSub(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorSub(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorSub(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector2 *v3);
MATH_INLINE void MathVectorSub(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorSub(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorSub(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector2 *v3);
/** 乗算
*/
MATH_INLINE void MathVectorMul(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorMul(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorMul(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector2 *v3);
MATH_INLINE void MathVectorMul(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorMul(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorMul(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector2 *v3);
MATH_INLINE void MathVectorMul(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorMul(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorMul(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector2 *v3);
/** 除算
*/
MATH_INLINE void MathVectorDiv(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorDiv(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorDiv(adk::TVector4 *v1, const adk::TVector4 *v2,
                               const adk::TVector2 *v3);
MATH_INLINE void MathVectorDiv(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorDiv(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorDiv(adk::TVector3 *v1, const adk::TVector3 *v2,
                               const adk::TVector2 *v3);
MATH_INLINE void MathVectorDiv(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector4 *v3);
MATH_INLINE void MathVectorDiv(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector3 *v3);
MATH_INLINE void MathVectorDiv(adk::TVector2 *v1, const adk::TVector2 *v2,
                               const adk::TVector2 *v3);
/** スケール
*/
MATH_INLINE void MathVectorScale(adk::TVector4 *v1, const adk::TVector4 *v2,
                                 const F32 MathVectorScale);
MATH_INLINE void MathVectorScale(adk::TVector3 *v1, const adk::TVector3 *v2,
                                 const F32 MathVectorScale);
MATH_INLINE void MathVectorScale(adk::TVector2 *v1, const adk::TVector2 *v2,
                                 const F32 MathVectorScale);
/** 単位
*/
MATH_INLINE void MathVectorNormalize(adk::TVector4 *v1,
                                     const adk::TVector4 *v2);
MATH_INLINE void MathVectorNormalize(adk::TVector3 *v1,
                                     const adk::TVector3 *v2);
MATH_INLINE void MathVectorNormalize(adk::TVector2 *v1,
                                     const adk::TVector2 *v2);
/** 長さ
*/
MATH_INLINE F32 MathVectorLength(const adk::TVector4 *v);
MATH_INLINE F32 MathVectorLength(const adk::TVector3 *v);
MATH_INLINE F32 MathVectorLength(const adk::TVector2 *v);
/** 長さ
*/
MATH_INLINE F32 MathVectorLength2(const adk::TVector4 *v);
MATH_INLINE F32 MathVectorLength2(const adk::TVector3 *v);
MATH_INLINE F32 MathVectorLength2(const adk::TVector2 *v);
/** 距離(v3 - v2)
*/
MATH_INLINE F32
    MathVectorDistance(const adk::TVector4 *v2, const adk::TVector4 *v3);
MATH_INLINE F32
    MathVectorDistance(const adk::TVector3 *v2, const adk::TVector3 *v3);
MATH_INLINE F32
    MathVectorDistance(const adk::TVector2 *v2, const adk::TVector2 *v3);
/** 距離(v3 - v2)
*/
MATH_INLINE F32
    MathVectorDistance2(const adk::TVector4 *v2, const adk::TVector4 *v3);
MATH_INLINE F32
    MathVectorDistance2(const adk::TVector3 *v2, const adk::TVector3 *v3);
MATH_INLINE F32
    MathVectorDistance2(const adk::TVector2 *v2, const adk::TVector2 *v3);
/** 内積
*/
MATH_INLINE F32
    MathVectorDot(const adk::TVector4 *v2, const adk::TVector4 *v3);
MATH_INLINE F32
    MathVectorDot(const adk::TVector3 *v2, const adk::TVector3 *v3);
MATH_INLINE F32
    MathVectorDot(const adk::TVector2 *v2, const adk::TVector2 *v3);
/**
 * 外積(とりあえず、内部的にはadk::TVector3として処理。adk::TVector4の外積の使い道がわからないので)
*/
MATH_INLINE void MathVectorCross(adk::TVector4 *v1, const adk::TVector4 *v2,
                                 const adk::TVector4 *v3);
MATH_INLINE void MathVectorCross(adk::TVector3 *v1, const adk::TVector3 *v2,
                                 const adk::TVector3 *v3);
/** 線形補間：(v2 + (v3 - v2)  *t)
*/
MATH_INLINE void MathVectorLerp(adk::TVector4 *v1, const adk::TVector4 *v2,
                                const adk::TVector4 *v3, const F32 t);
MATH_INLINE void MathVectorLerp(adk::TVector3 *v1, const adk::TVector3 *v2,
                                const adk::TVector3 *v3, const F32 t);
MATH_INLINE void MathVectorLerp(adk::TVector2 *v1, const adk::TVector2 *v2,
                                const adk::TVector2 *v3, const F32 t);
/** 反射
*/
MATH_INLINE Bool MathVectorReflection(adk::TVector4 *v1,
                                      const adk::TVector4 *vec,
                                      const adk::TVector4 *normal);
MATH_INLINE Bool MathVectorReflection(adk::TVector3 *v1,
                                      const adk::TVector3 *vec,
                                      const adk::TVector3 *normal);
MATH_INLINE Bool MathVectorReflection(adk::TVector2 *v1,
                                      const adk::TVector2 *vec,
                                      const adk::TVector2 *normal);

ADK_END_NAMESPACE

#endif /* MATH_VECTOR_H_ */

