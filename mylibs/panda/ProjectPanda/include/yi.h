#ifndef __yi_h__
#define __yi_h__

typedef unsigned long Handle;
typedef unsigned char HandleBuffer[32];

#define FASTCALL __attribute__((fastcall)) 
#define CDECL __attribute__((cdecl)) 
#define STDCALL __attribute__((stdcall)) 

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

typedef unsigned int Bool;
#define True 1
#define False 0

#define MEM_ZERO(a) yimemzero(&a, sizeof(a))

typedef int int32;

#ifdef __x64__
#	define __long_byte_shift__	3
#	define __long_bit_shift__	6
#	define __long_bit_cnt__		64
#	define __LONG_SIGNED_BIT__	0x8000000000000000
#	define __LONG_FULL_BIT__	0xFFFFFFFFFFFFFFFF
#	define YISTR(str) (const char*)(str"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0")
#else
#	define __long_byte_shift__	2
#	define __long_bit_shift__	5
#	define __long_bit_cnt__		32
#	define __LONG_SIGNED_BIT__	0x80000000
#	define __LONG_FULL_BIT__	0xFFFFFFFF
#	define YISTR(str) (const char*)(str"\0\0\0\0\0\0\0")
#endif

#define __long_shift__ __long_byte_shift__	

#define __shift_mod__(n, a) (~(__LONG_FULL_BIT__<<n)&(long)a)

#define bytetobit(n) (n<<3)

#define sizeofbit(a) bytetobit(sizeof(a))

#define offsetof(type, member) (uInt)(&(((type*)Null)->member))

#define __sse2_bit_shift__ 4
#define YI_ALIGN_SHIFT __sse2_bit_shift__ 

#define YI_ALIGNMENT 16
#define YI_DEF_STRLEN 8

#define __YI_ALIGNED__ __attribute__((aligned(YI_ALIGNMENT)))

#define __is_yi_aligned__(x, y) \
	!(__shift_mod__(YI_ALIGN_SHIFT, (long)x) || __shift_mod__(YI_ALIGN_SHIFT, (long)y))

typedef char yiBuffer[256] __YI_ALIGNED__;

#define IPADDRESS(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)
#endif
