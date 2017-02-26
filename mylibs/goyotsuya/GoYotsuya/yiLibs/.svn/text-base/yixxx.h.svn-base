#ifndef __yi_h__
#define __yi_h__

typedef unsigned long Handle;
typedef unsigned char HandleBuffer[32];

#ifdef __POSIX__
#	define FASTCALL __attribute__((fastcall)) 
#	define CDECL __attribute__((cdecl)) 
#	define STDCALL __attribute__((stdcall)) 
#else
#	define FASTCALL __fastcall
#	ifndef CDECL
#		define CDECL __cdecl
#	endif
#	define STDCALL __stdcall
#endif

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

typedef unsigned int Bool;
#define True 1
#define False 0

/*#define MEM_ZERO(a) yimemzero(&a, sizeof(a))*/
#define MEM_ZERO(a) memset(&a, 0, sizeof(a))

#ifndef int32
typedef int int32;
#endif

#ifndef Int32
typedef int Int32;
typedef unsigned int uInt32;
#endif

typedef uInt32 Bit32;

typedef unsigned short Bit16;

typedef unsigned char Bit8;

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

#define offsetof(type, member) (unsigned int)(&(((type*)0)->member))

#define __sse2_bit_shift__ 4
#define YI_ALIGN_SHIFT __sse2_bit_shift__ 

#define YI_ALIGNMENT 16
#define YI_DEF_STRLEN 8

#define __YI_ALIGNED__ __attribute__((aligned(YI_ALIGNMENT)))

#define __is_yi_aligned__(x, y) \
	!(__shift_mod__(YI_ALIGN_SHIFT, (long)x) || __shift_mod__(YI_ALIGN_SHIFT, (long)y))

#ifdef __POSIX__
typedef char yiBuffer[256] __YI_ALIGNED__;
#endif

//#define IPADDRESS(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)
#define __ipaddress__(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)

#define __highword__(n) = ((Int32)n>>16)
#define __loword__(n) = (0x0000FFFF&(int32)n)

#define __highbyte__(n) = ((Int32)n>>8)
#define __lowbyte__(n) = (0x000000FF&(Int32)n)

#define FILE_YOTSUYA_PREFIX 0x53544f59 // "YOTS"

#define credit_sizeof(a) (sizeof(a)-sizeof(Int32))

#define __8to32__(a,b,c,d)  (((uInt32)a<<24)|((uInt32)b<<16)|((uInt32)c<<8)|(uInt32)d)

#define __16to32__(a,b)  (((uInt32)a<<16)|(uInt32)b);

#define __32to8__(value32, n) ((value32>>(n<<3)) & 0x000000FF)

#define __32to16__(value32,n)  ((value32>>(n<<4)) & 0x0000FFFF)

#define __p8to32__(p, n) *((Bit32*)(&p[n]))

#define __set8to32__(p, n, value) (*((Bit32*)(&p[n])) = value)

#define __setbiton__(n32, n) ((1<<n) | n32)

#define __setbitoff__(n32, n) ((~(1<<n)) & n32)

#define __biton__(n32, n) ((1<<n) | n32)

#define __date32__(y, m, d) ((y<<9) | (m<<5) | (d))

#define __year__(date) (date>>9)

#define __month__(date) (((date>>5)&0x0000000F)+1)

#define __day__(date) ((date&0x0000001F)+1)

#ifndef __MAX_PATH__ 
#define __MAX_PATH__ 256
#endif

#ifdef _WIN32
#define yiTrace
#define SYSLOG
#endif

#ifdef _WIN32
#include <windows.h>
typedef int pthread_t;
#endif

#endif
