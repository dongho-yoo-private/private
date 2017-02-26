/**
 * \file math_vector_intern_ios.h
 * \ingroup math
 * \author tomonori tanaka
 * \~english
 * \~japanese
 */

#ifndef MATH_VECTOR_INTERN_IOS_H_
#define MATH_VECTOR_INTERN_IOS_H_

/* operator implementation -------------------*/
/* ベクトル: 4次元 */
MATH_INLINE adk::TVector4 operator+(const adk::TVector4& u,
                                    const adk::TVector4& v) {
  adk::TVector4 t;
  t.x = u.x + v.x;
  t.y = u.y + v.y;
  t.z = u.z + v.z;
  t.w = u.w + v.w;
  return t;
}
MATH_INLINE adk::TVector4 operator+(const adk::TVector4& v, const F32 s) {
  adk::TVector4 t;
  t.x = v.x * s;
  t.y = v.y * s;
  t.z = v.z * s;
  t.w = v.w * s;
  return t;
}
MATH_INLINE adk::TVector4 operator+(const F32 s, const adk::TVector4& v) {
  adk::TVector4 t;
  t.x = v.x + s;
  t.y = v.y + s;
  t.z = v.z + s;
  t.w = v.w + s;
  return t;
}
MATH_INLINE adk::TVector4 operator-(const adk::TVector4& u,
                                    const adk::TVector4& v) {
  adk::TVector4 t;
  t.x = u.x - v.x;
  t.y = u.y - v.y;
  t.z = u.z - v.z;
  t.w = u.w - v.w;
  return t;
}
MATH_INLINE adk::TVector4 operator-(const adk::TVector4& v, const F32 s) {
  adk::TVector4 t;
  t.x = v.x - s;
  t.y = v.y - s;
  t.z = v.z - s;
  t.w = v.w - s;
  return t;
}
MATH_INLINE adk::TVector4 operator-(const F32 s, const adk::TVector4& v) {
  adk::TVector4 t;
  t.x = s - v.x;
  t.y = s - v.y;
  t.z = s - v.z;
  t.w = s - v.w;
  return t;
}
MATH_INLINE adk::TVector4 operator-(const adk::TVector4& v) {
  adk::TVector4 t;
  t.x = -v.x;
  t.y = -v.y;
  t.z = -v.z;
  t.w = -v.w;
  return t;
}
MATH_INLINE adk::TVector4 operator*(const adk::TVector4& u,
                                    const adk::TVector4& v) {
  adk::TVector4 t;
  t.x = u.x * v.x;
  t.y = u.y * v.y;
  t.z = u.z * v.z;
  t.w = u.w * v.w;
  return t;
}
MATH_INLINE adk::TVector4 operator*(const F32 s, const adk::TVector4& v) {
  adk::TVector4 t;
  t.x = v.x * s;
  t.y = v.y * s;
  t.z = v.z * s;
  t.w = v.w * s;
  return t;
}
MATH_INLINE adk::TVector4 operator*(const adk::TVector4& v, const F32 s) {
  adk::TVector4 t;
  t.x = v.x * s;
  t.y = v.y * s;
  t.z = v.z * s;
  t.w = v.w * s;
  return t;
}
MATH_INLINE adk::TVector4 operator/(const adk::TVector4& u,
                                    const adk::TVector4& v) {
  adk::TVector4 t;
  t.x = u.x / v.x;
  t.y = u.y / v.y;
  t.z = u.z / v.z;
  t.w = u.w / v.w;
  return t;
}
MATH_INLINE adk::TVector4 operator/(const adk::TVector4& v, const F32 s) {
  adk::TVector4 t;
  t.x = v.x / s;
  t.y = v.y / s;
  t.z = v.z / s;
  t.w = v.w / s;
  return t;
}
MATH_INLINE adk::TVector4 operator/(const F32 s, const adk::TVector4& v) {
  adk::TVector4 t;
  t.x = s / v.x;
  t.y = s / v.y;
  t.z = s / v.z;
  t.w = s / v.w;
  return t;
}
MATH_INLINE Bool operator==(const adk::TVector4& u, const adk::TVector4& v) {
  return (u.x == v.x && u.y == v.y && u.z == v.z && u.w == v.w);
}
/* ベクトル: 3次元 */
MATH_INLINE adk::TVector3 operator+(const adk::TVector3& u,
                                    const adk::TVector3& v) {
  adk::TVector3 t;
  t.x = u.x + v.x;
  t.y = u.y + v.y;
  t.z = u.z + v.z;
  return t;
}
MATH_INLINE adk::TVector3 operator+(const adk::TVector3& v, const F32 s) {
  adk::TVector3 t;
  t.x = v.x + s;
  t.y = v.y + s;
  t.z = v.z + s;
  return t;
}
MATH_INLINE adk::TVector3 operator+(const F32 s, const adk::TVector3& v) {
  adk::TVector3 t;
  t.x = v.x + s;
  t.y = v.y + s;
  t.z = v.z + s;
  return t;
}
MATH_INLINE adk::TVector3 operator-(const adk::TVector3& u,
                                    const adk::TVector3& v) {
  adk::TVector3 t;
  t.x = u.x - v.x;
  t.y = u.y - v.y;
  t.z = u.z - v.z;
  return t;
}
MATH_INLINE adk::TVector3 operator-(const adk::TVector3& v, const F32 s) {
  adk::TVector3 t;
  t.x = v.x - s;
  t.y = v.y - s;
  t.z = v.z - s;
  return t;
}
MATH_INLINE adk::TVector3 operator-(const F32 s, const adk::TVector3& v) {
  adk::TVector3 t;
  t.x = v.x - s;
  t.y = v.y - s;
  t.z = v.z - s;
  return t;
}
MATH_INLINE adk::TVector3 operator-(const adk::TVector3& v) {
  adk::TVector3 t;
  t.x = -v.x;
  t.y = -v.y;
  t.z = -v.z;
  return t;
}
MATH_INLINE adk::TVector3 operator*(const adk::TVector3& u,
                                    const adk::TVector3& v) {
  adk::TVector3 t;
  t.x = u.x * v.x;
  t.y = u.y * v.y;
  t.z = u.z * v.z;
  return t;
}
MATH_INLINE adk::TVector3 operator*(const F32 s, const adk::TVector3& v) {
  adk::TVector3 t;
  t.x = v.x * s;
  t.y = v.y * s;
  t.z = v.z * s;
  return t;
}
MATH_INLINE adk::TVector3 operator*(const adk::TVector3& v, const F32 s) {
  adk::TVector3 t;
  t.x = v.x * s;
  t.y = v.y * s;
  t.z = v.z * s;
  return t;
}
MATH_INLINE adk::TVector3 operator/(const adk::TVector3& u,
                                    const adk::TVector3& v) {
  adk::TVector3 t;
  t.x = u.x / v.x;
  t.y = u.y / v.y;
  t.z = u.z / v.z;
  return t;
}
MATH_INLINE adk::TVector3 operator/(const adk::TVector3& v, const F32 s) {
  adk::TVector3 t;
  t.x = v.x / s;
  t.y = v.y / s;
  t.z = v.z / s;
  return t;
}
MATH_INLINE adk::TVector3 operator/(const F32 s, const adk::TVector3& v) {
  adk::TVector3 t;
  t.x = s / v.x;
  t.y = s / v.y;
  t.z = s / v.z;
  return t;
}
MATH_INLINE Bool operator==(const adk::TVector3& u, const adk::TVector3& v) {
  return (u.x == v.x && u.y == v.y && u.z == v.z);
}
/* ベクトル: 2次元 */
MATH_INLINE adk::TVector2 operator+(const adk::TVector2& u,
                                    const adk::TVector2& v) {
  adk::TVector2 t;
  t.x = u.x + v.x;
  t.y = u.y + v.y;
  return t;
}
MATH_INLINE adk::TVector2 operator+(const adk::TVector2& v, const F32 s) {
  adk::TVector2 t;
  t.x = v.x + s;
  t.y = v.y + s;
  return t;
}
MATH_INLINE adk::TVector2 operator+(const F32 s, const adk::TVector2& v) {
  adk::TVector2 t;
  t.x = v.x + s;
  t.y = v.y + s;
  return t;
}
MATH_INLINE adk::TVector2 operator-(const adk::TVector2& u,
                                    const adk::TVector2& v) {
  adk::TVector2 t;
  t.x = u.x - v.x;
  t.y = u.y - v.y;
  return t;
}
MATH_INLINE adk::TVector2 operator-(const adk::TVector2& v, const F32 s) {
  adk::TVector2 t;
  t.x = v.x - s;
  t.y = v.y - s;
  return t;
}
MATH_INLINE adk::TVector2 operator-(const F32 s, const adk::TVector2& v) {
  adk::TVector2 t;
  t.x = s - v.x;
  t.y = s - v.y;
  return t;
}
MATH_INLINE adk::TVector2 operator-(const adk::TVector2& v) {
  adk::TVector2 t;
  t.x = -v.x;
  t.y = -v.y;
  return t;
}
MATH_INLINE adk::TVector2 operator*(const adk::TVector2& u,
                                    const adk::TVector2& v) {
  adk::TVector2 t;
  t.x = u.x * v.x;
  t.y = u.y * v.y;
  return t;
}
MATH_INLINE adk::TVector2 operator*(const F32 s, const adk::TVector2& v) {
  adk::TVector2 t;
  t.x = v.x * s;
  t.y = v.y * s;
  return t;
}
MATH_INLINE adk::TVector2 operator*(const adk::TVector2& v, const F32 s) {
  adk::TVector2 t;
  t.x = v.x * s;
  t.y = v.y * s;
  return t;
}
MATH_INLINE adk::TVector2 operator/(const adk::TVector2& u,
                                    const adk::TVector2& v) {
  adk::TVector2 t;
  t.x = u.x / v.x;
  t.y = u.y / v.y;
  return t;
}
MATH_INLINE adk::TVector2 operator/(const adk::TVector2& v, const F32 s) {
  adk::TVector2 t;
  t.x = v.x / s;
  t.y = v.y / s;
  return t;
}
MATH_INLINE adk::TVector2 operator/(const F32 s, const adk::TVector2& v) {
  adk::TVector2 t;
  t.x = s / v.x;
  t.y = s / v.y;
  return t;
}
MATH_INLINE Bool operator==(const adk::TVector2& u, const adk::TVector2& v) {
  return (u.x == v.x && u.y == v.y);
}

ADK_BEGIN_NAMESPACE

/* struct operator implementation -------------------*/
/* ベクトル: 4次元 */
MATH_INLINE void TVector4::operator+=(const F32 s) { *this = *this + s; }
MATH_INLINE void TVector4::operator+=(const adk::TVector4& v) {
  *this = *this + v;
}
MATH_INLINE void TVector4::operator-=(const F32 s) { *this = *this - s; }
MATH_INLINE void TVector4::operator-=(const adk::TVector4& v) {
  *this = *this - v;
}
MATH_INLINE void TVector4::operator*=(const F32 s) { *this = *this * s; }
MATH_INLINE void TVector4::operator*=(const adk::TVector4& v) {
  *this = *this * v;
}
MATH_INLINE void TVector4::operator/=(const F32 s) { *this = *this / s; }
MATH_INLINE void TVector4::operator/=(const adk::TVector4& v) {
  *this = *this / v;
}
/* ベクトル: 3次元 */
MATH_INLINE void TVector3::operator+=(const F32 s) { *this = *this - s; }
MATH_INLINE void TVector3::operator+=(const adk::TVector3& v) {
  *this = *this + v;
}
MATH_INLINE void TVector3::operator-=(const F32 s) { *this = *this - s; }
MATH_INLINE void TVector3::operator-=(const adk::TVector3& v) {
  *this = *this - v;
}
MATH_INLINE void TVector3::operator*=(const F32 s) { *this = *this * s; }
MATH_INLINE void TVector3::operator*=(const adk::TVector3& v) {
  *this = *this * v;
}
MATH_INLINE void TVector3::operator/=(const F32 s) { *this = *this / s; }
MATH_INLINE void TVector3::operator/=(const adk::TVector3& v) {
  *this = *this / v;
}
/* ベクトル: 2次元 */
MATH_INLINE void TVector2::operator+=(const F32 s) {
  x += s;
  y += s;
}
MATH_INLINE void TVector2::operator+=(const adk::TVector2& v) {
  x += v.x;
  y += v.y;
}
MATH_INLINE void TVector2::operator-=(const F32 s) {
  x -= s;
  y -= s;
}
MATH_INLINE void TVector2::operator-=(const adk::TVector2& v) {
  x -= v.x;
  y -= v.y;
}
MATH_INLINE void TVector2::operator*=(const F32 s) {
  x *= s;
  y *= s;
}
MATH_INLINE void TVector2::operator*=(const adk::TVector2& v) {
  x *= v.x;
  y *= v.y;
}
MATH_INLINE void TVector2::operator/=(const F32 s) {
  x /= s;
  y /= s;
}
MATH_INLINE void TVector2::operator/=(const adk::TVector2& v) {
  x /= v.x;
  y /= v.y;
}

/* inline function implementation -------------------*/
/* 初期化(X=0.0 Y=0.0 Z=0.0 W=1.0) */
MATH_INLINE void MathVectorUnit(adk::TVector4* v) {
  v->x = v->y = v->z = 0.0f;
  v->w = 1.0f;
}
MATH_INLINE void MathVectorUnit(adk::TVector3* v) { v->x = v->y = v->z = 0.0f; }
MATH_INLINE void MathVectorUnit(adk::TVector2* v) { v->x = v->y = 0.0f; }
/* セット */
MATH_INLINE void MathVectorSet(adk::TVector4* v, const F32 x, const F32 y,
                               const F32 z, const F32 w) {
  v->x = x;
  v->y = y;
  v->z = z;
  v->w = w;
}
MATH_INLINE void MathVectorSet(adk::TVector3* v, const F32 x, const F32 y,
                               const F32 z) {
  v->x = x;
  v->y = y;
  v->z = z;
}
MATH_INLINE void MathVectorSet(adk::TVector2* v, const F32 x, const F32 y) {
  v->x = x;
  v->y = y;
}
/* コピー */
MATH_INLINE void MathVectorCopy(adk::TVector4* v1, const adk::TVector4* v2) {
  *v1 = *v2;
}
MATH_INLINE void MathVectorCopy(adk::TVector4* v1, const adk::TVector3* v2) {
  v1->x = v2->x;
  v1->y = v2->y;
  v1->z = v2->z;
}
MATH_INLINE void MathVectorCopy(adk::TVector4* v1, const adk::TVector2* v2) {
  v1->x = v2->x;
  v1->y = v2->y;
}
MATH_INLINE void MathVectorCopy(adk::TVector3* v1, const adk::TVector4* v2) {
  v1->x = v2->x;
  v1->y = v2->y;
  v1->z = v2->z;
}
MATH_INLINE void MathVectorCopy(adk::TVector3* v1, const adk::TVector3* v2) {
  *v1 = *v2;
}
MATH_INLINE void MathVectorCopy(adk::TVector3* v1, const adk::TVector2* v2) {
  v1->x = v2->x;
  v1->y = v2->y;
}
MATH_INLINE void MathVectorCopy(adk::TVector2* v1, const adk::TVector4* v2) {
  v1->x = v2->x;
  v1->y = v2->y;
}
MATH_INLINE void MathVectorCopy(adk::TVector2* v1, const adk::TVector3* v2) {
  v1->x = v2->x;
  v1->y = v2->y;
}
MATH_INLINE void MathVectorCopy(adk::TVector2* v1, const adk::TVector2* v2) {
  *v1 = *v2;
}
/* 加算 */
MATH_INLINE void MathVectorAdd(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x + v3->x;
  v1->y = v2->y + v3->y;
  v1->z = v2->z + v3->z;
  v1->w = v2->w + v3->w;
}
MATH_INLINE void MathVectorAdd(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x + v3->x;
  v1->y = v2->y + v3->y;
  v1->z = v2->z + v3->z;
}
MATH_INLINE void MathVectorAdd(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x + v3->x;
  v1->y = v2->y + v3->y;
}
MATH_INLINE void MathVectorAdd(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x + v3->x;
  v1->y = v2->y + v3->y;
  v1->z = v2->z + v3->z;
}
MATH_INLINE void MathVectorAdd(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x + v3->x;
  v1->y = v2->y + v3->y;
  v1->z = v2->z + v3->z;
}
MATH_INLINE void MathVectorAdd(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x + v3->x;
  v1->y = v2->y + v3->y;
}
MATH_INLINE void MathVectorAdd(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x + v3->x;
  v1->y = v2->y + v3->y;
}
MATH_INLINE void MathVectorAdd(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x + v3->x;
  v1->y = v2->y + v3->y;
}
MATH_INLINE void MathVectorAdd(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x + v3->x;
  v1->y = v2->y + v3->y;
}
/* 減算 */
MATH_INLINE void MathVectorSub(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x - v3->x;
  v1->y = v2->y - v3->y;
  v1->z = v2->z - v3->z;
  v1->w = v2->w - v3->w;
}
MATH_INLINE void MathVectorSub(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x - v3->x;
  v1->y = v2->y - v3->y;
  v1->z = v2->z - v3->z;
}
MATH_INLINE void MathVectorSub(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x - v3->x;
  v1->y = v2->y - v3->y;
}
MATH_INLINE void MathVectorSub(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x - v3->x;
  v1->y = v2->y - v3->y;
  v1->z = v2->z - v3->z;
}
MATH_INLINE void MathVectorSub(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x - v3->x;
  v1->y = v2->y - v3->y;
  v1->z = v2->z - v3->z;
}
MATH_INLINE void MathVectorSub(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x - v3->x;
  v1->y = v2->y - v3->y;
}
MATH_INLINE void MathVectorSub(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x - v3->x;
  v1->y = v2->y - v3->y;
}
MATH_INLINE void MathVectorSub(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x - v3->x;
  v1->y = v2->y - v3->y;
}
MATH_INLINE void MathVectorSub(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x - v3->x;
  v1->y = v2->y - v3->y;
}
/* 乗算 */
MATH_INLINE void MathVectorMul(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x * v3->x;
  v1->y = v2->y * v3->y;
  v1->z = v2->z * v3->z;
  v1->w = v2->w * v3->w;
}
MATH_INLINE void MathVectorMul(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x * v3->x;
  v1->y = v2->y * v3->y;
  v1->z = v2->z * v3->z;
}
MATH_INLINE void MathVectorMul(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x * v3->x;
  v1->y = v2->y * v3->y;
}
MATH_INLINE void MathVectorMul(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x * v3->x;
  v1->y = v2->y * v3->y;
  v1->z = v2->z * v3->z;
}
MATH_INLINE void MathVectorMul(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x * v3->x;
  v1->y = v2->y * v3->y;
  v1->z = v2->z * v3->z;
}
MATH_INLINE void MathVectorMul(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x * v3->x;
  v1->y = v2->y * v3->y;
}
MATH_INLINE void MathVectorMul(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x * v3->x;
  v1->y = v2->y * v3->y;
}
MATH_INLINE void MathVectorMul(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x * v3->x;
  v1->y = v2->y * v3->y;
}
MATH_INLINE void MathVectorMul(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x * v3->x;
  v1->y = v2->y * v3->y;
}
/* 除算 */
MATH_INLINE void MathVectorDiv(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x / v3->x;
  v1->y = v2->y / v3->y;
  v1->z = v2->z / v3->z;
  v1->w = v2->w / v3->w;
}
MATH_INLINE void MathVectorDiv(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x / v3->x;
  v1->y = v2->y / v3->y;
  v1->z = v2->z / v3->z;
}
MATH_INLINE void MathVectorDiv(adk::TVector4* v1, const adk::TVector4* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x / v3->x;
  v1->y = v2->y / v3->y;
}
MATH_INLINE void MathVectorDiv(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x / v3->x;
  v1->y = v2->y / v3->y;
  v1->z = v2->z / v3->z;
}
MATH_INLINE void MathVectorDiv(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x / v3->x;
  v1->y = v2->y / v3->y;
  v1->z = v2->z / v3->z;
}
MATH_INLINE void MathVectorDiv(adk::TVector3* v1, const adk::TVector3* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x / v3->x;
  v1->y = v2->y / v3->y;
}
MATH_INLINE void MathVectorDiv(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector4* v3) {
  v1->x = v2->x / v3->x;
  v1->y = v2->y / v3->y;
}
MATH_INLINE void MathVectorDiv(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector3* v3) {
  v1->x = v2->x / v3->x;
  v1->y = v2->y / v3->y;
}
MATH_INLINE void MathVectorDiv(adk::TVector2* v1, const adk::TVector2* v2,
                               const adk::TVector2* v3) {
  v1->x = v2->x / v3->x;
  v1->y = v2->y / v3->y;
}
/* スケール */
MATH_INLINE void MathVectorScale(adk::TVector4* v1, const adk::TVector4* v2,
                                 const F32 scale) {
  v1->x = v2->x * scale;
  v1->y = v2->y * scale;
  v1->z = v2->z * scale;
  v1->w = v2->w * scale;
}
MATH_INLINE void MathVectorScale(adk::TVector3* v1, const adk::TVector3* v2,
                                 const F32 scale) {
  v1->x = v2->x * scale;
  v1->y = v2->y * scale;
  v1->z = v2->z * scale;
}
MATH_INLINE void MathVectorScale(adk::TVector2* v1, const adk::TVector2* v2,
                                 const F32 scale) {
  v1->x = v2->x * scale;
  v1->y = v2->y * scale;
}
/* 単位 */
MATH_INLINE void MathVectorNormalize(adk::TVector4* v1,
                                     const adk::TVector4* v2) {
  F32 invLen = 1.0f / (F32)MathSqrt(v2->x * v2->x + v2->y * v2->y +
                                        v2->z * v2->z + v2->w * v2->w);

  MathVectorScale(v1, v2, invLen);
}
MATH_INLINE void MathVectorNormalize(adk::TVector3* v1,
                                     const adk::TVector3* v2) {
  F32 invLen =
      1.0f / (F32)MathSqrt(v2->x * v2->x + v2->y * v2->y + v2->z * v2->z);

  MathVectorScale(v1, v2, invLen);
}
MATH_INLINE void MathVectorNormalize(adk::TVector2* v1,
                                     const adk::TVector2* v2) {
  F32 invLen = 1.0f / (F32)MathSqrt(v2->x * v2->x + v2->y * v2->y);

  MathVectorScale(v1, v2, invLen);
}
/* 長さ */
MATH_INLINE F32 MathVectorLength(const adk::TVector4* v) {
  return MathSqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}
MATH_INLINE F32 MathVectorLength(const adk::TVector3* v) {
  return MathSqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}
MATH_INLINE F32 MathVectorLength(const adk::TVector2* v) {
  return MathSqrt(v->x * v->x + v->y * v->y);
}
/* 長さ */
MATH_INLINE F32 MathVectorLength2(const adk::TVector4* v) {
  return v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w;
}
MATH_INLINE F32 MathVectorLength2(const adk::TVector3* v) {
  return v->x * v->x + v->y * v->y + v->z * v->z;
}
MATH_INLINE F32 MathVectorLength2(const adk::TVector2* v) {
  return v->x * v->x + v->y * v->y;
}
/* 距離(v2 - v1) */
MATH_INLINE F32
MathVectorDistance(const adk::TVector4* v1, const adk::TVector4* v2) {
  adk::TVector4 vDist;

  MathVectorSub(&vDist, v2, v1);

  return MathVectorLength(&vDist);
}
MATH_INLINE F32
MathVectorDistance(const adk::TVector3* v1, const adk::TVector3* v2) {
  adk::TVector3 vDist;

  MathVectorSub(&vDist, v2, v1);

  return MathVectorLength(&vDist);
}
MATH_INLINE F32
MathVectorDistance(const adk::TVector2* v1, const adk::TVector2* v2) {
  adk::TVector2 vDist;

  MathVectorSub(&vDist, v2, v1);

  return MathVectorLength(&vDist);
}
/* 内積 */
MATH_INLINE F32
MathVectorDot(const adk::TVector4* v1, const adk::TVector4* v2) {
  return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w;
}
MATH_INLINE F32
MathVectorDot(const adk::TVector3* v1, const adk::TVector3* v2) {
  return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}
MATH_INLINE F32
MathVectorDot(const adk::TVector2* v1, const adk::TVector2* v2) {
  return v1->x * v2->x + v1->y * v2->y;
}
/* 外積 */
MATH_INLINE void MathVectorCross(adk::TVector4* v1, const adk::TVector4* v2,
                                 const adk::TVector4* v3) {
  v1->x = v2->y * v3->z - v2->z * v3->y;
  v1->y = v2->z * v3->x - v2->x * v3->z;
  v1->z = v2->x * v3->y - v2->y * v3->x;
}
MATH_INLINE void MathVectorCross(adk::TVector3* v1, const adk::TVector3* v2,
                                 const adk::TVector3* v3) {
  v1->x = v2->y * v3->z - v2->z * v3->y;
  v1->y = v2->z * v3->x - v2->x * v3->z;
  v1->z = v2->x * v3->y - v2->y * v3->x;
}
/* 線形補間：(v2 + (v3 - v2) * t) */
MATH_INLINE void MathVectorLerp(adk::TVector4* v1, const adk::TVector4* v2,
                                const adk::TVector4* v3, const F32 t) {
  *v1 = *v2 + ((*v3 - *v2) * t);
}
MATH_INLINE void MathVectorLerp(adk::TVector3* v1, const adk::TVector3* v2,
                                const adk::TVector3* v3, const F32 t) {
  *v1 = *v2 + ((*v3 - *v2) * t);
}
MATH_INLINE void MathVectorLerp(adk::TVector2* v1, const adk::TVector2* v2,
                                const adk::TVector2* v3, const F32 t) {
  *v1 = *v2 + ((*v3 - *v2) * t);
}
/* 反射 */
MATH_INLINE Bool MathVectorReflection(adk::TVector4* v1,
                                      const adk::TVector4* vec,
                                      const adk::TVector4* normal) {
  F32 n = MathVectorDot(vec, normal);

  v1->x = vec->x - 2.0f * normal->x * n;
  v1->y = vec->y - 2.0f * normal->y * n;
  v1->z = vec->z - 2.0f * normal->z * n;

  return (n > 0.0f);
}
MATH_INLINE Bool MathVectorReflection(adk::TVector3* v1,
                                      const adk::TVector3* vec,
                                      const adk::TVector3* normal) {
  F32 n = MathVectorDot(vec, normal);

  v1->x = vec->x - 2.0f * normal->x * n;
  v1->y = vec->y - 2.0f * normal->y * n;
  v1->z = vec->z - 2.0f * normal->z * n;

  return (n > 0.0f);
}
MATH_INLINE Bool MathVectorReflection(adk::TVector2* v1,
                                      const adk::TVector2* vec,
                                      const adk::TVector2* normal) {
  F32 n = MathVectorDot(vec, normal);

  v1->x = vec->x - 2.0f * normal->x * n;
  v1->y = vec->y - 2.0f * normal->y * n;

  return (n > 0.0f);
}

ADK_END_NAMESPACE

#endif /* MATH_VECTOR_INTERN_IOS_H_ */

