/**
 * \file math_vector_ios.h
 * \ingroup math
 * \author tomonori tanaka
 * \~english
 * \~japanese
 */

#ifndef MATH_VECTOR_IOS_H_
#define MATH_VECTOR_IOS_H_

ADK_BEGIN_NAMESPACE

/* struct -------------------*/
/* ベクトル: 4次元 */
ALIGNED16(struct) TVector4{
	/* vars ---------------------------------------------------------*/
	union {
		F32	f[4];
		struct {
			F32	x, y, z, w;
		};
	};

	/* operator definitions ---------------------------------------------------------*/
	MATH_INLINE void operator +=(const F32 s);
	MATH_INLINE void operator +=(const adk::TVector4 &v);
	MATH_INLINE void operator -=(const F32 s);
	MATH_INLINE void operator -=(const adk::TVector4 &v);
	MATH_INLINE void operator *=(const F32 s);
	MATH_INLINE void operator *=(const adk::TVector4 &v);
	MATH_INLINE void operator /=(const F32 s);
	MATH_INLINE void operator /=(const adk::TVector4 &v);
};
/* ベクトル: 3次元 */
struct TVector3{
	/* vars ---------------------------------------------------------*/
	union {
		F32	f[3];
		struct {
			F32	x, y, z;
		};
	};

	/* operator definitions ---------------------------------------------------------*/
	MATH_INLINE void operator +=(const F32 s);
	MATH_INLINE void operator +=(const adk::TVector3 &v);
	MATH_INLINE void operator -=(const F32 s);
	MATH_INLINE void operator -=(const adk::TVector3 &v);
	MATH_INLINE void operator *=(const F32 s);
	MATH_INLINE void operator *=(const adk::TVector3 &v);
	MATH_INLINE void operator /=(const F32 s);
	MATH_INLINE void operator /=(const adk::TVector3 &v);
};
/* ベクトル: 2次元 */
ALIGNED8(struct) TVector2{
	/* vars ---------------------------------------------------------*/
	union {
		F32		f[2];
		struct {
			F32	x, y;
		};
	};

	/* operator definitions ---------------------------------------------------------*/
	MATH_INLINE void operator +=(const F32 s);
	MATH_INLINE void operator +=(const adk::TVector2 &v);
	MATH_INLINE void operator -=(const F32 s);
	MATH_INLINE void operator -=(const adk::TVector2 &v);
	MATH_INLINE void operator *=(const F32 s);
	MATH_INLINE void operator *=(const adk::TVector2 &v);
	MATH_INLINE void operator /=(const F32 s);
	MATH_INLINE void operator /=(const adk::TVector2 &v);
};

ADK_END_NAMESPACE

#endif	/* MATH_VECTOR_IOS_H_ */

