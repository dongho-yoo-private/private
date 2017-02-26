/**
 * \file math_types.h
 * \ingroup math
 * \author tomonori tanaka
 * \~english
 * \~japanese
 */

#ifndef MATH_TYPES_H_
#define MATH_TYPES_H_

#include <float.h>

#include <core/types.h>

ADK_BEGIN_NAMESPACE

#ifndef M_PI
# define M_PI 3.14159265358979323846f
#endif
#ifndef M_PI_2
# define M_PI_2 1.57079632679489661923f
#endif
#ifndef M_SQRT2
# define M_SQRT2 1.41421356237309504880f
#endif
#ifndef M_SQRT1_2
# define M_SQRT1_2 0.70710678118654752440f
#endif
#ifndef M_1_PI
# define M_1_PI 0.318309886183790671538f
#endif
#ifndef M_E
# define M_E 2.7182818284590452354f
#endif
#ifndef M_LOG2E
# define M_LOG2E 1.4426950408889634074f
#endif
#ifndef M_LOG10E
# define M_LOG10E 0.43429448190325182765f
#endif
#ifndef M_LN2
# define M_LN2 0.69314718055994530942f
#endif
#ifndef M_LN10
# define M_LN10 2.30258509299404568402f
#endif

/* non-standard  defines, used in some places */
#ifndef MAXFLOAT
# define MAXFLOAT ((float)3.40282347e+38)
#endif
/*  define -------------------*/
/* 数学定数定義 */
#define MATH_2PI (6.283185307179586232f)
#define MATH_PI (3.14159265358979323846f)
#define MATH_HALF_PI (MATH_PI * 0.5f)
#define MATH_RADS_PER_DEG (MATH_2PI/ 360.0f)
#define MATH_DEGS_PER_RAD (360.0f / MATH_2PI)
#define MATH_EPSILON (FLT_EPSILON) /*  */
#define MATH_INFINITY (FLT_MAX)    /*  */

/* override definitions -------------------*/
#define MATH_INLINE

struct TVector4; /* Vector: x,y,z,w */
typedef struct TVector4 TVector4;
struct TVector3; /* Vector: x,y,z */
typedef struct TVector3 TVector3;
struct TVector2; /* Vector: x,y */
typedef struct TVector2 TVector2;
struct TMatrix44; /* Matrix::4x4 */
typedef struct TMatrix44 TMatrix44;
struct TMatrix34; /* Matrix::3x4 */
typedef struct TMatrix34 TMatrix34;
struct TMatrix33; /* Matrix: 3x3 */
typedef struct TMatrix33 TMatrix33;
struct TMatrix22; /* Matrix: 2x2 */
typedef struct TMatrix22 TMatrix22;
struct TQuat; /* Quat */
typedef struct TQuat TQuat;
struct TColor; /* カラー */
typedef struct TColor TColor;
struct TColorF; /* カラー: 不動少数 */
typedef struct TColorF TColorF;
/**
 * \~english
 * \~japanese
 *  \brief Cocos2d-x間に合わせ
 */
typedef struct {
  F32 width, height;
} TSize;
/**
 * \~english
 * \~japanese
 *  \brief Cocos2d-x間に合わせ
 */
typedef struct {
  F32 left, top, width, height;
} TRect;

ADK_END_NAMESPACE

#endif /* MATH_TYPES_H_ */
