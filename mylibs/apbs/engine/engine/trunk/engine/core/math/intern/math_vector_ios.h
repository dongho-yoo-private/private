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
/* �x�N�g��: 4���� */
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
/* �x�N�g��: 3���� */
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
/* �x�N�g��: 2���� */
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

